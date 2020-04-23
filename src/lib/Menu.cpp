/* MENU.cpp
 *   by Tim Müller (11774606)
 *
 * C++ Programmeermethoden - Assignment 3
 *
 * Created:
 *   23/04/2020, 13:55:13
 * Last edited:
 *   23/04/2020, 22:43:02
 * Auto updated?
 *   Yes
 *
 * Description:
 *   This file implements the menu. The menu can be navigated by writing
 *   characters to the stdin (like pressing keys) during the program's
 *   execution, and only when explicit input is needed does the user
 *   actually need to type something.
**/

#include <sstream>
#include <iostream>
#include <fstream>
#include <cstring>
#include <cerrno>
#include <thread>

#include "Menu.hpp"

using namespace std;
using namespace GameOfLife;


const Params default_params = {
    // step_delay (chrono::milliseconds)
    chrono::milliseconds(100),
    // viewport_stepsize_h (int)
    5,
    // viewport_stepsize_v (int)
    5,
    // alive_char (char)
    'O',
    // dead_char (char)
    '.',
    // border_char (char)
    '@',
    // alive_prob (float)
    0.5
};


Menu::Menu(Random& rand, World& world)
    : rand(rand),
    world(world),
    params(default_params)
{}



std::string Menu::render_world() {
    stringstream sstr;
    sstr << this->world.render(this->params);
    return sstr.str();
}

std::string Menu::render_mainmenu() {
    stringstream sstr;
    sstr << endl << "(Main Menu) > ";
    return sstr.str();
}

std::string Menu::render_help() {
    stringstream sstr;
    sstr << endl << "This menu is used to control the Game of Life simulation." << endl << endl;
    sstr << "Possible options are:" << endl;

    // Quit
    sstr << "    (h)elp:" << endl;
    sstr << "        Shows this help menu." << endl;
    sstr << "    (s)top, (e)xit or (q)uit:" << endl;
    sstr << "        Stops the execution of the program." << endl;
    sstr << "    (c)lean:" << endl;
    sstr << "        Empties the entire board." << endl;
    sstr << "    (r)andomize:" << endl;
    sstr << "        Resets the board to a random configuration of alive and dead cells." << endl;
    sstr << "        The probability of a cell being alive can be set in the parameter menu." << endl;
    sstr << "    (o)ne:" << endl;
    sstr << "        Runs the simulation one time and shows the output." << endl;
    sstr << "    (g)o:" << endl;
    sstr << "        Runs the simulation one-hundred times and shows the intermediate output." << endl;
    sstr << "        The delay between two steps can be set in the parameter menu." << endl;
    sstr << "    (m)ove:" << endl;
    sstr << "        Opens the menu to control the viewport." << endl;
    sstr << "    (p)arameters:" << endl;
    sstr << "        Opens the parameter menu to configure parameters of the simulation." << endl;
    sstr << "    (f)ile:" << endl;
    sstr << "        Opens the file menu to load pattern files." << endl;

    return sstr.str();
}

std::string Menu::render_movemenu() {
    stringstream sstr;
    sstr << endl << "(Move Sub-Menu) > ";
    return sstr.str();
}

std::string Menu::render_parametersmenu() {
    stringstream sstr;
    sstr << endl << "(Parameters Sub-Menu) > ";
    return sstr.str();
}

std::string Menu::render_filemenu() {
    stringstream sstr;
    sstr << endl << "(File Sub-Menu) > ";
    return sstr.str();
}



std::string Menu::cleanup(std::string text) {
    stringstream sstr;
    bool seen_char = false;
    bool seen_space = false;
    bool quote_mode = false;
    for (size_t i = 0; i < text.size(); i++) {
        char c = text[i];

        // Skip all whitelines
        if (c == '"') {
            quote_mode = !quote_mode;
        } else if (!quote_mode && (c == ' ' || c == '\t')) {
            seen_space = true;
            continue;
        }

        // Print one of 'em if we have a character before
        if (seen_space && seen_char) {
            sstr << ' ';
        }
        seen_space = false;
        seen_char = true;

        // Print the character, but convert everything to lowercase
        sstr << c;
    }
    return sstr.str();
}

std::string Menu::lowercase(std::string text) {
    for (size_t i = 0; i < text.size(); i++) {
        text[i] = tolower(text[i]);
    }
    return text;
}

std::string Menu::split(std::string text, std::vector<std::string>& args) {
    std::string cmd;
    stringstream buffer;
    bool quote_mode = false;
    for (size_t i = 0; i < text.size(); i++) {
        char c = text[i];
        if (c == '"') {
            // Enter quote mode
            quote_mode = !quote_mode;
        } else if (!quote_mode && c == ' ') {
            // Write either to cmd if it's the first, or else to args
            if (cmd.empty()) {
                cmd = buffer.str();
            } else {
                args.push_back(buffer.str());
            }
            // Reset the buffer
            buffer.str("");
            buffer.clear();
        } else {
            buffer << c;
        }
    }
    // Push the last buffer
    if (cmd.empty()) {
        cmd = buffer.str();
    } else {
        args.push_back(buffer.str());
    }
    // Done
    return cmd;
}



int Menu::run() {
    // Start with an empty world
    this->world.clean();
    // Render the world once to begin with
    cout << Menu::render_world();

    while (true) {
        // Main menu start
        cout << Menu::render_mainmenu();

        // Wait for user input
        std::string input;
        if (!getline(cin, input)) {
            // For some reason, the stream ended
            if (!cin.eof() && cin.fail()) {
                // It was an error
                cerr << "Could not read character from stream: " << strerror(errno) << endl;
            } else {
                cout << endl;
            }
            // Either way, stop
            return errno;
        }

        // Run different options
        input = Menu::lowercase(Menu::cleanup(input));
        if (input == "h" || input == "help") {
            // Print the help function
            cout << this->render_help();
        } else if (input == "q" || input == "e" || input == "s" || input == "quit" || input == "exit" || input == "stop") {
            // We stop
            return 0;
        } else if (input == "c" || input == "clean") {
            // Clean the world
            this->world.clean();
            cout << endl << this->render_world();
            cout << endl << "Cleaned the world." << endl;
        } else if (input == "r" || input == "randomize") {
            // Initialize the world randomly
            this->world.randomize(this->rand, this->params.alive_prob);
            cout << endl << this->render_world();
            cout << endl << "Reset the world to a random state." << endl;
        } else if (input == "o" || input == "one") {
            // Move the simulation one step ahead
            this->world.simulate();
            cout << endl << this->render_world();
        }  else if (input == "g" || input == "go") {
            // Move the simulation one-hunderd steps ahead
            for (int i = 0; i < 100; i++) {
                this->world.simulate();
                cout << endl << endl << endl << endl << endl << Menu::render_world();
                this_thread::sleep_for(this->params.step_delay);
            }
        } else if (input == "m" || input == "move") {
            // Run the move submenu
            int return_code = this->run_move();
            if (return_code != 0) {
                return return_code;
            }
        } else if (input == "p" || input == "parameters") {
            // Run the parameters submenu
            int return_code = this->run_parameters();
            if (return_code != 0) {
                return return_code;
            }
        } else if (input == "f" || input == "file") {
            // Run the file submenu
            int return_code = this->run_file();
            if (return_code != 0) {
                return return_code;
            }
        } else {
            cout << endl << "Unknown command '" << input << "'" << endl;
            cout << "(type 'help' to see a list of available options)" << endl;
        }
    }
}

int Menu::run_move() {
    // Move menu start
    while (true) {
        cout << Menu::render_movemenu();

        // Wait for user input
        std::string input;
        if (!getline(cin, input)) {
            // For some reason, the stream ended
            if (!cin.eof() && cin.fail()) {
                // It was an error
                cerr << "Could not read character from stream: " << strerror(errno) << endl;
            } else {
                cout << endl;
            }
            // Either way, stop
            return errno;
        }

        // Run different options
        input = Menu::lowercase(Menu::cleanup(input));
        if (input == "h" || input == "help") {
            // Print the help function
            cout << endl << "This menu is used to control the viewport." << endl << endl;
            cout << "Possible actions are:" << endl;
            cout << "    (h)elp:" << endl;
            cout << "        This help menu." << endl;
            cout << "    (u)p:" << endl;
            cout << "        Move the viewport up with one step." << endl;
            cout << "    (r)ight:" << endl;
            cout << "        Move the viewport right with one step." << endl;
            cout << "    (d)own:" << endl;
            cout << "        Move the viewport down with one step." << endl;
            cout << "    (l)eft:" << endl;
            cout << "        Move the viewport left with one step." << endl;
            cout << "    (s)top, (e)xit, (q)uit, (b)ack, (d)one or (c)ancel:" << endl;
            cout << "        Go back to the previous menu." << endl;
            cout << "How large one step is, is configurable in the parameters menu." << endl;
        } else if (input == "u" || input == "up") {
            // Move one step up
            int cells = this->world.move_viewport(0, -this->params.viewport_stepsize_v);
            cout << endl << this->render_world();
            cout << endl << "Moved viewport up with " << cells << " cells" << endl;
        } else if (input == "r" || input == "right") {
            // Move one step right
            int cells = this->world.move_viewport(this->params.viewport_stepsize_h, 0);
            cout << endl << this->render_world();
            cout << endl << "Moved viewport right with " << cells << " cells" << endl;
        } else if (input == "d" || input == "down") {
            // Move one step down
            int cells = this->world.move_viewport(0, this->params.viewport_stepsize_v);
            cout << endl << this->render_world();
            cout << endl << "Moved viewport down with " << cells << " cells" << endl;
        } else if (input == "l" || input == "left") {
            // Move one step left
            int cells = this->world.move_viewport(-this->params.viewport_stepsize_h, 0);
            cout << endl << this->render_world();
            cout << endl << "Moved viewport left with " << cells << " cells" << endl;
        } else if (input == "s" || input == "e" || input == "q" || input == "b" || input == "d" || input == "c" ||
                   input == "stop" || input == "exit" || input == "quit" || input == "back" || input == "done" || input == "cancel") {
            // Return cleanly
            return 0;
        } else {
            cout << endl << "Unknown command '" << input << "'" << endl;
            cout << "(type 'help' to see a list of available options)" << endl;
        }
    }
}

int Menu::run_parameters() {
    // Parameters menu start
    while (true) {
        cout << Menu::render_parametersmenu();

        // Wait for user input
        std::string input;
        if (!getline(cin, input)) {
            // For some reason, the stream ended
            if (!cin.eof() && cin.fail()) {
                // It was an error
                cerr << "Could not read character from stream: " << strerror(errno) << endl;
            } else {
                cout << endl;
            }
            // Either way, stop
            return errno;
        }

        // Preprocess the input
        input = Menu::cleanup(input);
        vector<std::string> args;
        input = Menu::split(input, args);
        input = Menu::lowercase(input);

        // Run different options
        if (input == "h" || input == "help") {
            // Print the help function
            cout << endl << "This menu is used to control set the parameters of the simulation." << endl << endl;
            cout << "Possible actions are:" << endl;
            cout << "    (h)elp:" << endl;
            cout << "        This help menu." << endl;
            cout << "    (p)aram <set|get> <name> [value]:" << endl;
            cout << "        Sets or gets the value of a parameter. If setting it, the value must be" << endl;
            cout << "        specified. A list of parameters to " << endl;
            cout << "    (s)top, (e)xit, (q)uit, (b)ack, (d)one or (c)ancel:" << endl;
            cout << "        Go back to the previous menu." << endl;
            cout << endl << "Possible parameters to modify are:" << endl;
            cout << "    step_size_h:" << endl;
            cout << "        Determines by how many cells the viewport moves horizontally with a" << endl;
            cout << "        single step. Input must be an integer larger than 0." << endl;
            cout << "    step_size_v:" << endl;
            cout << "        Determines by how many cells the viewport moves vertically with a single" << endl;
            cout << "        step. Input must be an integer larger than 0." << endl;
            cout << "    alive_prob:" << endl;
            cout << "        The probability of a cell being alive when the world is randomized." << endl;
            cout << "        Input is a double in the range [0, 1]." << endl;
            cout << "    char_alive:" << endl;
            cout << "        Determines the character used to represent alive cells. Input must be a" << endl;
            cout << "        single character." << endl;
            cout << "    char_dead:" << endl;
            cout << "        Determines the character used to represent dead cells. Input must be a" << endl;
            cout << "        single character." << endl;
            cout << "    char_border:" << endl;
            cout << "        Determines the character used to represent cells outside the world." << endl;
            cout << "        Input must be a single character." << endl;
            cout << "    step_delay:" << endl;
            cout << "        Determines the delay (in milliseconds) between two steps during the 'go'" << endl;
            cout << "        command. Input must be an integer number larger than 0." << endl;
        } else if (input == "s" || input == "e" || input == "q" || input == "b" || input == "d" || input == "c" ||
                   input == "stop" || input == "exit" || input == "quit" || input == "back" || input == "done" || input == "cancel") {
            // Return cleanly
            return 0;
        } else if (input == "p" || input == "param") {
            // Check if it's a set or get
            if (args.size() < 1 || (args[0] != "set" && args[0] != "get")) {
                cout << endl << "Usage: (p)aram <set|get> <name> [value]" << endl;
                continue;
            }
            bool is_set = args[0] == "set";

            // Make sure the size of the arguments is correct now we know the setiness / gettiness
            if ((is_set && args.size() < 3) || args.size() < 2) {
                cout << endl << "Usage: (p)aram " << (is_set ? "set" : "get") << " <name>" << (is_set ? " <value>" : "") << endl;
                continue;
            }
            
            if (is_set) {
                // Set the correct value
                try {
                    this->params.set(args[1], args[2]);
                    cout << endl << "Succesfully updated " << args[1] << endl;
                } catch (std::domain_error& e) {
                    cout << endl << "Unknown parameter '" << args[1] << "'" << endl;
                    cout << "(type 'help' to see a list of available options)" << endl;
                } catch (std::invalid_argument& e) {
                    cout << endl << "Cannot parse '" << args[2] << "' for parameter " << args[1] << endl;
                } catch (std::out_of_range& e) {
                    cout << endl << "Value '" << args[2] << "' is too large for parameter " << args[1] << endl;
                }
            } else {
                // Fetch the value using the params class
                try {
                    std::string result = this->params.get(args[1]);
                    cout << endl << "Parameter " << args[1] << " has value: " << result << endl;
                } catch (std::domain_error& e) {
                    cout << endl << "Unknown parameter '" << args[1] << "'" << endl;
                    cout << "(type 'help' to see a list of available options)" << endl;
                }
            }
        } else {
            cout << endl << "Unknown command '" << input << "'" << endl;
            cout << "(type 'help' to see a list of available options)" << endl;
        }
    }
}

int Menu::run_file() {
    // File menu start
    while (true) {
        cout << Menu::render_filemenu();

        // Wait for user input
        std::string input;
        if (!getline(cin, input)) {
            // For some reason, the stream ended
            if (!cin.eof() && cin.fail()) {
                // It was an error
                cerr << "Could not read character from stream: " << strerror(errno) << endl;
            } else {
                cout << endl;
            }
            // Either way, stop
            return errno;
        }

        // Preprocess the input
        input = Menu::cleanup(input);
        vector<std::string> args;
        input = Menu::split(input, args);
        input = Menu::lowercase(input);

        // Run the different options
        if (input == "h" || input == "help") {
            // Print the help function
            cout << endl << "This menu is used to control set the parameters of the simulation." << endl << endl;
            cout << "Possible actions are:" << endl;
            cout << "    (h)elp:" << endl;
            cout << "        This help menu." << endl;
            cout << "    (l)oad <path>:" << endl;
            cout << "        Loads given file as pattern file: every ' ' or '.' is a dead cell," << endl;
            cout << "        newlines move to the next line and all other characters are alive cells" << endl;
            cout << "        The pattern will be placed top-leftmost in the current viewport." << endl;
            cout << "    (i)nsert:" << endl;
            cout << "        Draw a pattern onto the terminal. Spaces or dots are dead cells, enter" << endl;
            cout << "        moves on to the next line and any other character is an alive cell." << endl;
            cout << "        Use EOF (Ctrl+D) to stop the pattern. It will be placed as top-leftmost" << endl;
            cout << "        in the current viewport as possible." << endl;
            cout << "    (s)top, (e)xit, (q)uit, (b)ack, (d)one or (c)ancel:" << endl;
            cout << "        Go back to the previous menu." << endl;
        } else if (input == "s" || input == "e" || input == "q" || input == "b" || input == "d" || input == "c" ||
                   input == "stop" || input == "exit" || input == "quit" || input == "back" || input == "done" || input == "cancel") {
            // Return cleanly
            return 0;
        } else if (input == "l" || input == "load") {
            // Try to open the file in the arguments
            if (args.size() != 1) {
                cout << endl << "Usage: (l)oad <path>" << endl;
                continue;
            }

            // Open the file
            fstream in(args[0]);
            if (!in.is_open()) {
                cout << endl << "Could not open file \"" << args[0] << "\": " << strerror(errno) << endl;
                continue;
            }

            this->world.insert_pattern(in);
            cout << this->render_world();
            cout << endl << "Succesfully loaded file." << endl;
        } else if (input == "i" || input == "insert") {
            cout << endl << "(Remember: '.' or ' ' is dead, otherwise alive; Ctrl+D to stop)" << endl;
            cout << "Type your input below:" << endl;
            this->world.insert_pattern(cin);
            cout << this->render_world();
            // Reset the file stream to get rid of Ctrl+D
            cin.clear();
        } else {
            cout << endl << "Unknown command '" << input << "'" << endl;
            cout << "(type 'help' to see a list of available options)" << endl;
        }
    }
}

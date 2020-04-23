/* MENU.cpp
 *   by Tim Müller (11774606)
 *
 * Created:
 *   23/04/2020, 13:55:13
 * Last edited:
 *   23/04/2020, 18:24:49
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
#include <cstring>
#include <cerrno>
#include <thread>
#include <vector>

#include "Menu.hpp"

using namespace std;
using namespace GameOfLife;


const std::chrono::milliseconds default_fps_delay(50);
const int default_viewport_stepsize_h = 5;
const int default_viewport_stepsize_v = 5;


Menu::Menu(Random& rand, World& world)
    : rand(rand),
    world(world),
    fps_delay(default_fps_delay),
    viewport_stepsize_h(default_viewport_stepsize_h),
    viewport_stepsize_v(default_viewport_stepsize_v)
{}



std::string Menu::render_world() {
    stringstream sstr;
    sstr << this->world.render();
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



void Menu::preprocess(std::string& text) {
    stringstream sstr;
    bool seen_char = false;
    bool seen_space = false;
    for (size_t i = 0; i < text.size(); i++) {
        char c = text[i];

        // Skip all whitelines
        if ((c == ' ' || c == '\t')) {
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
        sstr << (char) tolower(c);
    }
    text = sstr.str();
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
        Menu::preprocess(input);
        if (input == "h" || input == "help") {
            // Print the help function
            cout << this->render_help();
        } else if (input == "q" || input == "e" || input == "s" || input == "quit" || input == "exit" || input == "stop") {
            // We stop
            return 0;
        } else if (input == "c" || input == "clean") {
            // Clean the world
            this->world.clean();
            cout << endl << Menu::render_world();
            cout << endl << "Cleaned the world." << endl;
        } else if (input == "r" || input == "randomize") {
            // Initialize the world randomly
            this->world.randomize(this->rand, 0.5);
            cout << endl << Menu::render_world();
            cout << endl << "Reset the world to a random state." << endl;
        } else if (input == "o" || input == "one") {
            // Move the simulation one step ahead
            this->world.simulate();
            cout << endl << Menu::render_world();
        }  else if (input == "g" || input == "go") {
            // Move the simulation one-hunderd steps ahead
            for (int i = 0; i < 100; i++) {
                this->world.simulate();
                cout << endl << endl << endl << endl << endl << Menu::render_world();
                this_thread::sleep_for(this->fps_delay);
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
            cout << endl << "Unknown command '" << input << endl;
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
        Menu::preprocess(input);
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
            int cells = this->world.move_viewport(0, -this->viewport_stepsize_v);
            cout << this->world.render();
            cout << endl << "Moved viewport up with " << cells << " cells" << endl;
        } else if (input == "r" || input == "right") {
            // Move one step right
            int cells = this->world.move_viewport(this->viewport_stepsize_h, 0);
            cout << this->world.render();
            cout << endl << "Moved viewport right with " << cells << " cells" << endl;
        } else if (input == "d" || input == "down") {
            // Move one step down
            int cells = this->world.move_viewport(0, this->viewport_stepsize_v);
            cout << this->world.render();
            cout << endl << "Moved viewport down with " << cells << " cells" << endl;
        } else if (input == "l" || input == "left") {
            // Move one step left
            int cells = this->world.move_viewport(-this->viewport_stepsize_h, 0);
            cout << this->world.render();
            cout << endl << "Moved viewport left with " << cells << " cells" << endl;
        } else if (input == "s" || input == "e" || input == "q" || input == "b" || input == "d" || input == "c" ||
                   input == "stop" || input == "exit" || input == "quit" || input == "back" || input == "done" || input == "cancel") {
            // Return cleanly
            return 0;
        } else {
            cout << endl << "Unknown command '" << input << endl;
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

        // Run different options
        Menu::preprocess(input);
        vector<std::string> args;
        input = Menu::\(input, args);
        if (input == "h" || input == "help") {
            // Print the help function
            cout << endl << "This menu is used to control set the parameters of the simulation." << endl << endl;
            cout << "Possible actions are:" << endl;
            cout << "    (h)elp:" << endl;
            cout << "        This help menu." << endl;
            cout << "    param <set|get> <name> [value]:" << endl;
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
        } else if (input == "s" || input == "e" || input == "q" || input == "b" || input == "d" || input == "c" ||
                   input == "stop" || input == "exit" || input == "quit" || input == "back" || input == "done" || input == "cancel") {
            // Return cleanly
            return 0;
        } else if (input.size() >= 6 && input.substr(0, 5) == "param" && input[5] == ' ') {
            
        } else {
            cout << endl << "Unknown command '" << input << endl;
            cout << "(type 'help' to see a list of available options)" << endl;
        }
    }
}

int Menu::run_file() {

}

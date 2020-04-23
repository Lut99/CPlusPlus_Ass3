/* MENU.hpp
 *   by Tim Müller (11774606)
 *
 * Created:
 *   23/04/2020, 13:55:18
 * Last edited:
 *   23/04/2020, 21:32:53
 * Auto updated?
 *   Yes
 *
 * Description:
 *   This file implements the menu. The menu can be navigated by writing
 *   characters to the stdin (like pressing keys) during the program's
 *   execution, and only when explicit input is needed does the user
 *   actually need to type something. This is the header file.
**/

#ifndef MENU_HPP
#define MENU_HPP

#include <string>
#include <vector>
#include <chrono>

#include "Params.hpp"
#include "Random.hpp"
#include "World.hpp"

namespace GameOfLife {
    /* The Menu class lets the user control the simulation. */
    class Menu {
        private:
            /* Stores the random object used. */
            Random& rand;
            /* Stores the world object used. */
            World& world;
            /* Stores the parameters of the menu. */
            Params params;

            /* Renders the simulation */
            std::string render_world();
            /* Renders the main menu text */
            std::string render_mainmenu();
            /* Renders the help text. */
            std::string render_help();
            /* Renders the move menu text */
            std::string render_movemenu();
            /* Renders the parameters menu text. */
            std::string render_parametersmenu();
            /* Renders the file menu text. */
            std::string render_filemenu();

            /* Removes preceding, following or double whitespaces, but respects quotes. */
            static std::string cleanup(std::string text);
            /* Converts to lowercase equivalent. Ignores quotes. */
            static std::string lowercase(std::string text);
            /* Splits given input (preprocessed) in a command (first word) and arguments (rest of words). Each word is separated by a space, but respects quotes. */
            static std::string split(std::string text, std::vector<std::string>& args);
        public:
            /* Initializes the Menu class. */
            Menu(Random& rand, World& world);
            
            /* Runs the program from the point of view of the main menu */
            int run();
            /* Runs the move submenu */
            int run_move();
            /* Runs the parameters submenu */
            int run_parameters();
            /* Runs the file submenu */
            int run_file();
    };
}

#endif

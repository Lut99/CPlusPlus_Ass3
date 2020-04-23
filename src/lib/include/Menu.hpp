/* MENU.hpp
 *   by Tim Müller (11774606)
 *
 * Created:
 *   23/04/2020, 13:55:18
 * Last edited:
 *   23/04/2020, 18:22:00
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
#include <chrono>

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
            /* Specifies the delay (in milliseconds) between two frames in go. */
            std::chrono::milliseconds fps_delay;
            /* Specifies the step size (horizontal) of the viewport. */
            int viewport_stepsize_h;
            /* Specifies the step size (vectical) of the viewport. */
            int viewport_stepsize_v;

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

            /* Converts a string to its lowercase equivalent. Also removes and preceding, following or double whitespaces. */
            static void preprocess(std::string& text);
            /* Splits given input (preprocessed) in a command (first word) and arguments (rest of words). Each word is separated by a space. */
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

/* PARAMS.hpp
 *   by Tim Müller (11774606)
 *
 * C++ Programmeermethoden - Assignment 3
 *
 * Created:
 *   23/04/2020, 20:08:44
 * Last edited:
 *   23/04/2020, 22:42:57
 * Auto updated?
 *   Yes
 *
 * Description:
 *   Stores the parameters of the Menu class neatly. Additionally, also
 *   contains some functions for writing these parameters to string and
 *   retrieving them from string. This is the header file.
**/

#ifndef PARAMS_HPP
#define PARAMS_HPP

#include <string>
#include <chrono>

namespace GameOfLife {
    /* Stores the parameters of the Menu class. */
    class Params {
        private:
            /* Parses an unsigned integer, but returns it as an integer (and errors when that overflows). */
            static int parse_uint(std::string text);
            /* Parses a single character. */
            static char parse_char(std::string text);
        public:
            /* Specifies the delay (in milliseconds) between two frames in go. */
            std::chrono::milliseconds step_delay;
            /* Specifies the step size (horizontal) of the viewport. */
            int viewport_stepsize_h;
            /* Specifies the step size (vectical) of the viewport. */
            int viewport_stepsize_v;
            /* The character that represents alive cells in the textual render of the world. */
            char alive_char;
            /* The character that represents dead cells in the textual render of the world. */
            char dead_char;
            /* The character that represents cells outside the world in the textual render of the world. */
            char border_char;
            /* Determines the chance that a randomly initialised cell is alive. */
            float alive_prob;

            /* Retrieves the value of given parameter as string. It throws a domein_error if the parameter is unknown. */
            std::string get(std::string param) const;
            /* Sets the value of given parameter from the given string. Upon failure, throws either domein_error (upon unknown parameter), invalid_argument (if not the correct type) or out_of_range (if overflowed). */
            void set(std::string param, std::string value);
    };
}

#endif

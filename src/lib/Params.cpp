/* PARAMS.cpp
 *   by Tim Müller
 *
 * C++ Programmeermethoden - Assignment 3
 *
 * Created:
 *   23/04/2020, 20:08:10
 * Last edited:
 *   23/04/2020, 22:42:55
 * Auto updated?
 *   Yes
 *
 * Description:
 *   Stores the parameters of the Menu class neatly. Additionally, also
 *   contains some functions for writing these parameters to string and
 *   retrieving them from string.
**/

#include <stdexcept>
#include <limits>

#include "Params.hpp"

using namespace std;
using namespace GameOfLife;


int Params::parse_uint(std::string text) {
    // Any errors thrown by this one are handled by the calling function in Menu
    int value = stoi(text);
    // Exclude negative numbers
    if (value < 0) {
        throw invalid_argument(text);
    }
    return value;
}

char Params::parse_char(std::string text) {
    if (text.size() != 1) {
        throw out_of_range(text);
    }
    return text[0];
}



std::string Params::get(std::string param) const {
    if (param == "step_size_h") {
        return to_string(this->viewport_stepsize_h);
    } else if (param == "step_size_v") {
        return to_string(this->viewport_stepsize_v);
    } else if (param == "alive_prob") {
        return to_string(this->alive_prob);
    } else if (param == "char_alive") {
        return string(1, this->alive_char);
    } else if (param == "char_dead") {
        return string(1, this->dead_char);
    } else if (param == "char_border") {
        return string(1, this->border_char);
    } else if (param == "step_delay") {
        return to_string(this->step_delay.count());
    }
    
    // Unknown parameter
    throw std::domain_error("");
}

void Params::set(std::string param, std::string value) {
    if (param == "step_size_h") {
        this->viewport_stepsize_h = this->parse_uint(value);
    } else if (param == "step_size_v") {
        this->viewport_stepsize_v = this->parse_uint(value);
    } else if (param == "alive_prob") {
        // This one is so simple we can directly call the c parser
        this->alive_prob = stof(value);
    } else if (param == "char_alive") {
        this->alive_char = this->parse_char(value);
    } else if (param == "char_dead") {
        this->dead_char = this->parse_char(value);
    } else if (param == "char_border") {
        this->border_char = this->parse_char(value);
    } else if (param == "step_delay") {
        this->step_delay = chrono::milliseconds(this->parse_uint(value));
    } else {
        // Unknown parameter
        throw std::domain_error("");
    }
}

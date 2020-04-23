/* RANDOM.cpp
 *   by Tim Müller (11774606)
 *
 * C++ Programmeermethoden - Assignment 3
 *
 * Created:
 *   23/04/2020, 12:14:06
 * Last edited:
 *   23/04/2020, 22:43:12
 * Auto updated?
 *   Yes
 *
 * Description:
 *   Implements a custom Random class based on a Linear Conguential Generator.
 *   Sources:
 *     - Formula for the Linear Congruential Generator:
 *       https://en.wikipedia.org/wiki/Linear_congruential_generator
 *     - Parameters for modulus, multiplicer and increment:
 *       https://aaronschlegel.me/linear-congruential-generator-r.html
**/

#include <ctime>
#include <limits>
#include <cmath>

#include "Random.hpp"

using namespace std;
using namespace GameOfLife;


const int default_modulus = numeric_limits<int>::max();
const int default_multiplier = 1103515245;
const int default_increment = 12345;


Random::Random() {
    this->prev_value = time(NULL);
    this->modulus = default_modulus;
    this->multiplier = default_multiplier;
    this->increment = default_increment;
}

Random::Random(int seed) {
    this->prev_value = seed;
    this->modulus = default_modulus;
    this->multiplier = default_multiplier;
    this->increment = default_increment;
}

Random::Random(int seed, int modulus, int multiplier, int increment) {
    this->prev_value = seed;
    this->modulus = modulus;
    this->multiplier = multiplier;
    this->increment = increment;
}



int Random::real_mod(int a, int b) {
    return a >= 0 ? a % b : (b - abs( a%b )) % b;
}



int Random::random() {
    // Update the prev_value
    this->prev_value = Random::real_mod(this->multiplier * this->prev_value + this->increment, this->modulus);
    // Return it
    return this->prev_value;
}

int Random::random(int min, int max) {
    // Generate a random number
    int rand = this->random();
    // Apply some math to get it in range
    rand = (rand % (max - min)) + min;
    return rand;
}

float Random::random_f() {
    // Generate a random integer
    int rand = this->random();
    // Convert it to a float
    return (float)rand / (float)(this->modulus - 1);
}

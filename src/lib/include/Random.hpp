/* RANDOM.hpp
 *   by Tim Müller (11774606)
 *
 * Created:
 *   23/04/2020, 12:14:46
 * Last edited:
 *   23/04/2020, 15:48:38
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
 *     - Modulo code:
 *       Source: https://stackoverflow.com/questions/12276675/modulus-with-negative-numbers-in-c
 *   This is the header file.
**/

#ifndef RANDOM_HPP
#define RANDOM_HPP

namespace GameOfLife {
    /* The Random class implements a random number generator. */
    class Random {
        private:
            int prev_value;
            int modulus;
            int multiplier;
            int increment;

            /* Static helper function that implements a real modulo. */
            static int real_mod(int a, int b);
        public:
            /* Initializes the random number generator with the current time as seed. */
            Random();
            /* Initializes the random number generator with a custom seed. */
            Random(int seed);
            /* Initializes the random number generator with a custom seed, modulus, multiplier and increment. */
            Random(int seed, int modulus, int multiplier, int increment);

            /* Generates a random integer between 0 (inclusive) and INT_MAX (exclusive). */
            int random();
            /* Generates a random integer between MIN (inclusive) and MAX (exclusive). */
            int random(int min, int max);
            /* Generates a random float between 0 (inclusive) and 1 (exclusive). */
            float random_f();
    };
}

#endif

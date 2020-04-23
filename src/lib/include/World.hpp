/* WORLD.hpp
 *   by Tim Müller (11774606)
 *
 * C++ Programmeermethoden - Assignment 3
 *
 * Created:
 *   23/04/2020, 13:06:03
 * Last edited:
 *   23/04/2020, 22:43:09
 * Auto updated?
 *   Yes
 *
 * Description:
 *   The World class implements the actual Game Of Life. This is the header
 *   file.
**/

#ifndef WORLD_HPP
#define WORLD_HPP

#include <istream>
#include <cstddef>
#include <string>

#include "Params.hpp"
#include "Random.hpp"

namespace GameOfLife {
    /* The World class implements the actual Game of Life. Aside from the simulation, it can also render the world to a textual representation, as seen through a fixed-size viewport. */
    class World {
        private:
            /* Width (in cells) of the world. */
            int width;
            /* Height (in cells) of the world. */
            int height;

            /* Widht (in cells) of the viewport. */
            int viewport_width;
            /* Height (in cells) of the viewport. */
            int viewport_height;
            /* Top-left x coordinate of the viewport. */
            int viewport_x;
            /* Top-left y coordinate of the viewport. */
            int viewport_y;

            /* The internal representation of the world in the game of life. */
            bool* world;

            /* Counts the number of neighbours around (but not including) the given cell. */
            int count_neighbours(int x, int y);
        public:
            /* Creates a new game of life simulation with the default size. */
            World();
            /* Creates a new game of life simulation with a custom size. */
            World(size_t width, size_t height);
            /* Copy constructor for the World class. */
            World(const World& other);
            /* Move constructor for the World class. */
            World(World&& other);
            /* Deconstructor for the World class. */
            ~World();
            
            /* Moves the simulation one step forward. */
            void simulate();

            /* Cleans the world (kills all cells). */
            void clean();
            /* Initializes all cells randomly. The given probability is the change that a cell is alive. */
            void randomize(Random& rand, float alive_prob);
            /* Inserts a pattern that is read from given input stream until EOF is reached. The location of inserting is equal to viewport_x, viewport_y */
            void insert_pattern(std::istream& in);

            /* Sets the viewport at a certain location. Returns the actual number of cells moved. */
            int move_viewport(int dx, int dy);
            /* Returns a render of the viewport in text. */
            std::string render(Params& params) const;

            /* Copy assignment operator for the World class. */
            World& operator=(World other);
            /* Move assignment operator for the World class. */
            World& operator=(World&& other);
            /* Swap operator for the World class. */
            friend void swap(World& w1, World& w2);
    };

    /* Swap operator for the World class. */
    void swap(World& w1, World& w2);
}

#endif

/* WORLD.cpp
 *   by Tim Müller (11774606)
 *
 * Created:
 *   23/04/2020, 13:05:37
 * Last edited:
 *   23/04/2020, 17:52:24
 * Auto updated?
 *   Yes
 *
 * Description:
 *   The World class implements the actual Game Of Life. Note that, because it
 *   uses custom memory management, copying is done using the
 *   copy-and-swap-idiom and the Rule of Four (and a half)
 *   (https://stackoverflow.com/questions/3279543/what-is-the-copy-and-swap-idiom)
**/

#include <stdexcept>
#include <sstream>
#include <cstring>
#include <cerrno>

#include "World.hpp"

using namespace std;
using namespace GameOfLife;


const int default_width = 200;
const int default_height = 200;
const int default_viewport_width = 80;
const int default_viewport_height = 40;

const char default_alive_char = 'O';
const char default_dead_char = '.';
const char default_border_char = '@';


World::World() {
    this->width = default_width;
    this->height = default_height;

    this->viewport_width = default_viewport_width;
    this->viewport_height = default_viewport_height;
    this->viewport_x = -1;
    this->viewport_y = -1;

    this->alive_char = default_alive_char;
    this->dead_char = default_dead_char;
    this->border_char = default_border_char;

    // Initialize the array
    this->world = new bool[this->width * this->height];
}

World::World(size_t width, size_t height) {
    this->width = width;
    this->height = height;

    this->viewport_width = default_viewport_width;
    this->viewport_height = default_viewport_height;
    this->viewport_x = -1;
    this->viewport_y = -1;

    this->alive_char = default_alive_char;
    this->dead_char = default_dead_char;
    this->border_char = default_border_char;

    // Initialize the array
    this->world = new bool[this->width * this->height];
}

World::World(const World& other) {
    // Copy the world parameters
    this->width = other.width;
    this->height = other.height;
    
    // Copy the viewport parameters
    this->viewport_width = other.viewport_width;
    this->viewport_height = other.viewport_height;
    this->viewport_x = other.viewport_x;
    this->viewport_y = other.viewport_y;

    // Copy the character parameters
    this->alive_char = default_alive_char;
    this->dead_char = default_dead_char;
    this->border_char = default_border_char;

    // Copy the world
    this->world = new bool[this->width * this->height];
    for (int i = 0; i < this->width * this->height; i++) {
        this->world[i] = other.world[i];
    }
}

World::World(World&& other) {
    // Copy the world parameters
    this->width = other.width;
    this->height = other.height;
    
    // Copy the viewport parameters
    this->viewport_width = other.viewport_width;
    this->viewport_height = other.viewport_height;
    this->viewport_x = other.viewport_x;
    this->viewport_y = other.viewport_y;

    // Copy the character parameters
    this->alive_char = default_alive_char;
    this->dead_char = default_dead_char;
    this->border_char = default_border_char;

    // Since it's moving, we can steal the other's world class
    this->world = other.world;
    other.world = nullptr;
}

World::~World() {
    // Delete the inner object
    if (this->world != nullptr) {
        delete[] this->world;
    }
}



int World::count_neighbours(int x, int y) {
    // Sanity check that x and y are within bounds
    if (x >= this->width || y >= this->height) {
        throw out_of_range("Position (" + to_string(x) + "," + to_string(y) + " is out of range for World with size " + to_string(this->width) + "x" + to_string(this->height));
    }

    // Set the bounds of the loop
    int x_min = x != 0 ? x - 1 : x;
    int x_max = x != this->width - 1 ? x + 1 : x;
    int y_min = y != 0 ? y - 1 : y;
    int y_max = y != this->height - 1 ? y + 1 : y;

    // Loop it
    int count = 0;
    for (int iy = y_min; iy <= y_max; iy++) {
        for (int ix = x_min; ix <= x_max; ix++) {
            // Exclude the given cell
            if (ix == x && iy == y) { continue; }
            count += this->world[iy * this->width + x];
        }
    }

    // Return the count
    return count;
}



void World::simulate() {
    // Loop through all cells
    for (int y = 0; y < this->height; y++) {
        for (int x = 0; x < this->width; x++) {
            int index = y * this->width + x;
            // Count the number of neighbours
            int neighbours = this->count_neighbours(x, y);
            // Do different things depending on if the cell is alive
            if (this->world[index]) {
                // This cell is alive, it dies if it hasn't got 2 or 3
                //   neighbours
                if (neighbours != 2 && neighbours != 3) {
                    this->world[index] = false;
                }
            } else {
                // This cell is dead, it becomes alive if it has 3 neighbours
                if (neighbours == 3) {
                    this->world[index] = true;
                }
            }
        }
    }
}



void World::clean() {
    // Loop through all cells to kill 'em off
    for (int y = 0; y < this->height; y++) {
        for (int x = 0; x < this->width; x++) {
            this->world[y * this->width + x] = false;
        }
    }
}

void World::randomize(Random& rand, float alive_prob) {
    // Loop through all cells to give them a random value
    for (int y = 0; y < this->height; y++) {
        for (int x = 0; x < this->width; x++) {
            this->world[y * this->width + x] = rand.random_f() < alive_prob;
        }
    }
}

void World::insert_pattern(std::istream& in) {
    // Read through all characters of the pattern while inserting. Error if the
    //   pattern goes out-of-range.
    int x = this->viewport_x;
    int y = this->viewport_y;
    while(true) {
        char c;
        in.get(c);
        if (in.eof()) {
            // We're done
            return;
        } else if (in.fail()) {
            // An error has occurred
            throw runtime_error("Could not read character from stream: " + string(strerror(errno)));
        }

        // Move the index and make sure we are within bounds
        if (c == '\n') {
            y++;
            x = this->viewport_x;
            continue;
        } else {
            x++;
        }
        if (x >= this->width || y >= this->height) {
            throw out_of_range("Position (" + to_string(x) + "," + to_string(y) + " is out of range for World with size " + to_string(this->width) + "x" + to_string(this->height));
        }

        // Update the cell state
        this->world[y * this->width + x] = c == '.' || c == ' ';
    }
}



int World::move_viewport(int dx, int dy) {
    // Make sure at least a part of the board is available
    int newx = this->viewport_x + dx;
    int newy = this->viewport_y + dy;

    // Clip the viewport to the edges of the screen
    if (newx < -1) { newx = -1; }
    else if (newx > this->width - this->viewport_width + 1) { newx = this->width - this->viewport_width + 1; }
    if (newy < -1) { newy = -1; }
    else if (newy > this->height - this->viewport_height + 1) { newy = this->height - this->viewport_height + 1; }

    // Update the viewport values
    this->viewport_x = newx;
    this->viewport_y = newy;

    return abs(this->viewport_x - newx) + abs(this->viewport_y - newy);
}

std::string World::render() const {
    // Loop through the particular bit of the board to create the representation
    stringstream sstr;
    for (int y = this->viewport_y; y < this->viewport_y + this->viewport_height; y++) {
        for (int x = this->viewport_x; x < this->viewport_x + this->viewport_width; x++) {
            // Print the correct characters
            if (x < 0 || x >= this->width || y < 0 || y >= this->height) {
                sstr << this->border_char;
            } else if (this->world[y * this->width + x]) {
                sstr << this->alive_char;
            } else {
                sstr << this->dead_char;
            }
        }
        sstr << endl;
    }
    // Return the result
    return sstr.str();
}



World& World::operator=(World other) {
    if (this != &other) {
        // Swap the two worlds, as other is already a copy
        swap(*this, other);
    }
    return *this;
}

World& World::operator=(World&& other) {
    if (this != &other) {
        // Swap the non-copy as we can steal
        swap(*this, other);
    }
    return *this;
}

void GameOfLife::swap(World& w1, World& w2) {
    using std::swap;

    // Swap all parameters
    swap(w1.width, w2.width);
    swap(w1.height, w2.height);
    swap(w1.viewport_width, w2.viewport_width);
    swap(w1.viewport_height, w2.viewport_height);
    swap(w1.viewport_x, w2.viewport_x);
    swap(w1.viewport_y, w2.viewport_y);
    swap(w1.alive_char, w2.alive_char);
    swap(w1.dead_char, w2.dead_char);
    swap(w1.border_char, w2.border_char);

    // Swap the world pointer
    swap(w1.world, w2.world);
}

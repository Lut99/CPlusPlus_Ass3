/* MAIN.cpp
 *   by Lut99
 *
 * C++ Programmeermethoden - Assignment 3
 * 
 * Created:
 *   23/04/2020, 16:16:15
 * Last edited:
 *   23/04/2020, 22:42:50
 * Auto updated?
 *   Yes
 *
 * Description:
 *   Entry point to the simulation. Does little else than to initialise the
 *   required objects and break them down again.
**/

#include <iostream>

#include "Random.hpp"
#include "World.hpp"
#include "Menu.hpp"

using namespace std;
using namespace GameOfLife;


int main() {
    Random rand;
    World world;
    Menu menu(rand, world);

    return menu.run();
}

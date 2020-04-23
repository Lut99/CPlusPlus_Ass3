/* MAIN.cpp
 *   by Lut99
 *
 * C++ Programmeermethoden - Assignment 3
 * Gecompiled met g++ (Ubuntu 5.7.0-3ubuntu1~18.04) op KDE Neon (Ubuntu 18.04)
 * 
 * Created:
 *   23/04/2020, 16:16:15
 * Last edited:
 *   23/04/2020, 22:58:30
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

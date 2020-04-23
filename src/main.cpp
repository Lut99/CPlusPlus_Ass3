/* MAIN.cpp
 *   by Lut99
 *
 * Created:
 *   23/04/2020, 16:16:15
 * Last edited:
 *   23/04/2020, 17:51:19
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
    World world(78, 38);
    Menu menu(rand, world);

    return menu.run();
}

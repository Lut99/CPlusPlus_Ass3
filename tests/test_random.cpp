/* TEST RANDOM.cpp
 *   by Tim Müller (11774606)
 *
 * Created:
 *   23/04/2020, 12:38:49
 * Last edited:
 *   23/04/2020, 14:20:47
 * Auto updated?
 *   Yes
 *
 * Description:
 *   This file tests the validity of the Random Class.
**/

#include <iostream>

#include "Random.hpp"

using namespace std;
using namespace GameOfLife;


bool test_all() {
    cout << "Testing default distibution..." << endl;

    int expected[] = {12345,
                      1406932605,
                      1698552177,
                      1375482709,
                      147059050,
                      284706395,
                      1686626040,
                      530165712,
                      316763081,
                      524833486};

    // Create the random object with a fixed seed
    Random rand = Random(0);

    // Do some iterations to test it
    for (int i = 0; i < 10; i++) {
        int r = rand.random();
        cout << "  Iteration " << i << ": " << r << endl;
        if (r != expected[i]) {
            cout << "FAILURE: Expected " << expected[i] << endl << endl;
            return false;
        }
    }

    cout << "Done" << endl << endl;

    return true;
}

bool test_limited() {
    cout << "Testing limited distibution..." << endl;

    int expected[] = {5, 5, 7, 9, 0, 5, 0, 2, 1, 6};

    // Create the random object with a fixed seed
    Random rand = Random(0);

    // Do some iterations to test it
    for (int i = 0; i < 10; i++) {
        int r = rand.random(0, 10);
        cout << "  Iteration " << i << ": " << r << endl;
        if (r != expected[i]) {
            cout << "FAILURE: Expected " << expected[i] << endl << endl;
            return false;
        }
    }

    cout << "Done" << endl << endl;

    return true;
}

bool test_float() {
    cout << "Testing float distibution..." << endl;

    // Create the random object with a fixed seed
    Random rand = Random(0);

    // Do some iterations. Note that we do not actually test, due to floating
    //   point rounding
    for (int i = 0; i < 10; i++) {
        float r = rand.random_f();
        cout << "  Iteration " << i << ": " << r << endl;
    }

    cout << "Done" << endl << endl;

    return true;
}

int main() {
    cout << endl << "*** TEST FOR 'Random' CLASS ***" << endl << endl;
    
    if (!test_all()) {
        return -1;
    }

    if (!test_limited()) {
        return -1;
    }

    if (!test_float()) {
        return -1;
    }

    cout << "*** DONE ***" << endl << endl;
}

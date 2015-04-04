#include "Testing.h"
#include <iostream>

using namespace std;

const unsigned int NUMBER_OF_TESTS = 1000u;

int main()
{
    bool result;
    while (true)
    {
        result = testing(NUMBER_OF_TESTS);
        if (result)
            cout << "+\n";
        else
            cout << "-\n";
    }
    system("pause");
    return 0;
}

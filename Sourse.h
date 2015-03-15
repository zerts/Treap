#include "Testing.h"
#include <iostream>

using namespace std;

int main()
{
    bool result = testing(10000, 1, 10000);
    /*Node *root = new Node(0);
    Treap *tree = new Treap(root);
    for (size_t i = 1; i < 10; i++)
    {
        tree->insert(i, i);
    }
    int number = 0;
    while (true)
    {
        tree->nextPermutation();
        number++;
        if (number == 10)
        {
            int a = 0;
        }
    }*/
    if (result)
        cout << "+\n";
    else
        cout << "-\n";
    system("pause");
    return 0;
}

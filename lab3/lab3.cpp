#include <iostream>

#include <bitset>
#include <vector>

#include <cstdlib>
#include <time.h>

using namespace std;

int main()
{
    int x0 = 4;
    int a = 32, c = 53, m = 84;

    int x = x0;

    for (int i = 0; i < 100; i++)
    {
        cout << x << endl;
        x = (x * a + c) % m;
    }

    return 0;
}

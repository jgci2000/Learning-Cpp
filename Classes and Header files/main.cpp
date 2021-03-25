#include <iostream>

#include "Class1.h"

using namespace std;

int main(int argc, char **argv)
{
    Class1 obj(2);

    // obj.set_b(1);

    cout << obj.a(2, 1) << " " << obj.get_b() << endl;

    return 0;
}


#include <iostream>

#include "Class1.h"


int Class1::a(int x, int y)
{
    return x + y;
}

int Class1::get_b()
{
    return this->b;
}

void Class1::set_b(int b)
{
    this->b = b;
}

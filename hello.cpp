#include <iostream>
#include <string>
#include <cmath>
// #include <stdlib.h>

void add_one(int &i)
{
    i++;
}

void add_one(int *arr, int *out)
{
    for (int i = 0; i < 4; i++)
        arr[i]++;
}

// g++ hello.cpp -o hello -fpermissive; ./hello


int main(int argc, char **argv)
{
    int a = 20;
    int **p = malloc(sizeof(int) * 4);

    p[0] = &a;

    // for (int i = 0; i < 4; i++)
    //     p[i] = &a;

    std::cout << p << std::endl;
    std::cout << &p << std::endl;

    std::cout << p[0] << std::endl;
    std::cout << &(p[0]) << std::endl;

    std::cout << p[0][0] << std::endl;
    std::cout << &(p[0][0]) << std::endl;
    
    // for (int i = 0; i < 4; i++)
    //     p[i] = 2*i;

    // for (int i = 0; i < 4; i++)
    //     std::cout << p[i] << std::endl;

    // add_one(p);

    // for (int i = 0; i < 4; i++)
    //     std::cout << p[i] << std::endl; 

    // std::cout << *p << std::endl;
    // std::cout << p[0] << std::endl;

    // std::cout << *(p + 1) << std::endl;
    // std::cout << p[1] << std::endl;
    
    // int *p1 = p + 1;

    // std::cout << *p1 << std::endl;
    // std::cout << p[1] << std::endl;
    // std::cout << p1[0] << std::endl;

    // std::cout << &p1 << std::endl;

    // int a = 0;

    // std::cout << a << std::endl;
    // add_one(a);
    // std::cout << a << std::endl;

    return 0;
}

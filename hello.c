#include <stdio.h>
#include <stdlib.h>

int main() 
{
    int *p = malloc(sizeof(int) * 4);

    printf("%d \n", p);
    printf("%d \n", &p);

    return 0;
}
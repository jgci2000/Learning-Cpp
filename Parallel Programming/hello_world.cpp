#include <iostream>
#include <omp.h>

int main()
{
    #pragma omp parallel
    {
        #pragma omp critical
        {
            int ID = omp_get_thread_num();
            std::cout << "My first parallel code! " << ID << std::endl;
        }
    }
}


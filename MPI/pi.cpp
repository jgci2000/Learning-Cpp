#include <iostream>
#include <cmath>
#include <chrono>
#include <mpi.h>

using std::cout;
using std::endl;


double get_pi(double min, double max, double num_steps)
{
    double step = 1.0 / num_steps;
    double pi = 0;

    for (min; min <= max; min += step)
        pi += 4 / (1 + pow(min, 2)) * step;

    return pi;
}

int main(int argc, char **argv)
{
    int root = 0;
    int rank;
    int size;

    double num_steps = 1000000;

    double pi = 0;
    double x = 0;
    double x_max = 1;

    double my_x;
    double my_x_max;

    double tmp;
    double res_pi;

    double start;
    double end;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // if (rank == root)
    // {
    //     start = MPI_Wtime();
    // }

    // my_x = (x + rank) / size;
    // my_x_max = (x_max + rank) / size;

    // res_pi = get_pi(my_x, my_x_max, num_steps);

    if (rank == root)
    {
        start = MPI_Wtime();

        double my_x_arr[size - 1];
        double my_x_max_arr[size - 1];

        for (int i = 0; i < size - 1; i++)
        {
            my_x_arr[i] = (x + i) / (size - 1);
            my_x_max_arr[i] = (x_max + i) / (size - 1);
        }

        for (int i = 1; i < size; i++)
        {
            MPI_Send(my_x_arr + i - 1, 1, MPI_DOUBLE_PRECISION, i, i, MPI_COMM_WORLD);
            MPI_Send(my_x_max_arr + i - 1, 1, MPI_DOUBLE_PRECISION, i, i, MPI_COMM_WORLD);
        }

        // res_pi = 0;
        cout << root << " root entered reduce" << endl;
        MPI_Reduce(&res_pi, &pi, 1, MPI_DOUBLE_PRECISION, MPI_SUM, root, MPI_COMM_WORLD);
        cout << root << " root reduced" << endl;

        cout << "pi: " << pi <<endl;

        end = MPI_Wtime();
        double time = end - start;
        cout << "time: " << time << endl;
    }
    else
    {
        MPI_Recv(&my_x, 1, MPI_DOUBLE_PRECISION, root, rank, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(&my_x_max, 1, MPI_DOUBLE_PRECISION, root, rank, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        res_pi = get_pi(my_x, my_x_max, num_steps);

        cout << rank << " entered reduce" << endl;
        MPI_Reduce(&res_pi, &pi, 1, MPI_DOUBLE_PRECISION, MPI_SUM, root, MPI_COMM_WORLD);
        cout << rank << " reduced" << endl;
    }
    

    // MPI_Reduce(&res_pi, &pi, 1, MPI_DOUBLE_PRECISION, MPI_SUM, 0, MPI_COMM_WORLD);

    // if (rank == root)
    //     cout << "pi: " << pi << endl;

    // if (rank == 0)
    // {
    //     pi = res_pi;

    //     for (int i = 1; i < size; i++)
    //     {
    //         MPI_Recv(&tmp, 1, MPI_DOUBLE_PRECISION, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

    //         pi += tmp;
    //     }

    //     cout << "Result: " << pi << endl;
    // }
    // else
    //     MPI_Send(&res_pi, 1, MPI_DOUBLE_PRECISION, 0, 0, MPI_COMM_WORLD);

    // if (rank == root)
    // {
    //     end = MPI_Wtime();
    //     double time = end - start;
    //     cout << "time: " << time << endl;
    // }

    MPI_Finalize();

    return 0;
}

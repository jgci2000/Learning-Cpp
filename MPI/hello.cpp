#include <iostream>
#include <mpi.h>

using std::cout;
using std::endl;


int main(int argc, char **argv)
{
    int rank;
    int size;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int msg_len = 180;
    char msg[msg_len];

    sprintf(msg, "Hello World from %d with a size of %d.", rank, size);

    if (rank == 0)
    {
        cout << msg << endl;
        for (int i = 1; i < size; i++)
        {
            MPI_Recv(msg, msg_len, MPI_CHAR, i, i, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

            cout << msg << endl;
        }
    }
    else
    {
        MPI_Send(msg, msg_len, MPI_CHAR, 0, rank, MPI_COMM_WORLD);
    }    

    MPI_Finalize();

    return 0;
}

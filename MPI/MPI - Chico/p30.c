#include "mpi.h"
#include <stdio.h>
#include <math.h>
int main(int argc, char *argv[])
{
  int n, myid, numprocs, i;
  double PI25DT = 3.141592653589793238462643;
  double mypi, pi, h, sum, x;
  double MPI_Wtime(), MPI_Wtick();
  double starttime, endtime;

  MPI_Init(&argc,&argv);
  MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
  MPI_Comm_rank(MPI_COMM_WORLD, &myid);

  while (1) {
    if (myid == 0) {
      printf("Enter the number of intervals: (0 quits) ");
      scanf("%d",&n);
      starttime = MPI_Wtime();
    }
    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
    if (n == 0)
      break;
    else {
      h = 1.0 / (double) n;
      sum = 0.0;
      for (i = myid + 1; i <= n; i += numprocs) {
        x = h * ((double)i - 0.5);
        sum += (4.0 / (1.0 + x*x));
      }
      mypi = h * sum;
      printf("I am process number %d and my contribution is %f\n", myid, mypi);
      usleep(10000*numprocs); /* Give it time to do all printfs before
                                 outputing the total result. */
      MPI_Reduce(&mypi, &pi, 1, MPI_DOUBLE, MPI_SUM, 0,
                 MPI_COMM_WORLD);
      if (myid == 0) {
        endtime = MPI_Wtime();
        printf("pi is approximately %.16f, Error is %.16f\n",
               pi, fabs(pi - PI25DT));
        printf("time is %f seconds\n", endtime-starttime-0.01*numprocs);
        /* Descontei o tempo do usleep. */
      }
    }
  }
  MPI_Finalize();
  return 0;
}

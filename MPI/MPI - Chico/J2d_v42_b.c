#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <stdbool.h>
#include <string.h>
#define NXMAX 500
#define NMAXITER 500000
#define L 1.0 
#define CRIT_PAR 0.0000001
#define PAUSA 0.2

double f(double x, double y);

int main(int argc, char *argv[])
{
  int i, j;
  int myid, numprocs;
  int nx, ny, nyleft, nyright;
  int ndims = 2;
  int periods[2] = {false, false};
  int dims[2];
  int newid, nbrbottom, nbrtop, nbrleft, nbrright;
  int myrows, mytoprow, mycols, myleftcol, iter;
  int gsizes[ndims];
  int lsizes[ndims];
  int start_indices[ndims]; 
  int memsizes[ndims];
  double t1, t2;
  double h;
  double sum2[2], sum2_global[2];
  double diff_global;

  MPI_Comm comm2d;
  MPI_Datatype coluna;
  MPI_Datatype filetype;
  MPI_Datatype memtype;

  MPI_File fh;

  MPI_Init(&argc,&argv);
  MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
  MPI_Comm_rank(MPI_COMM_WORLD, &myid);
  
  if (myid == 0) {
    printf("Escolha nx (0 p/ sair, máximo 500) ");
    (void)! scanf("%d",&nx);
  }

  t1 = MPI_Wtime();
  
  /* Todos */
  MPI_Bcast(&nx, 1, MPI_INT, 0, MPI_COMM_WORLD);
  if (nx <= 0 || nx > NXMAX) { /* Todos terminam.*/
    MPI_Finalize();
    exit(0);
  }
  ny = nx;
  nyleft = ny / 2;
  nyright = ny - nyleft;
  
  /* Get a new communicator for a decomposition of the domain */
  dims[0] = numprocs / 2;
  dims[1] = 2;
  numprocs = dims[0] * dims[1];

  MPI_Cart_create(MPI_COMM_WORLD, ndims, dims, periods, 1, &comm2d);
  /* O ultimo vai embora se for impar. */
  if (comm2d == MPI_COMM_NULL) {
    MPI_Finalize();
    exit(0);
  }

  /* Get my position in this communicator, and my neighbors */
  MPI_Comm_rank(comm2d, &newid);
  MPI_Cart_shift(comm2d, 0,  1, &nbrtop, &nbrbottom);
  MPI_Cart_shift(comm2d, 1,  1, &nbrleft, &nbrright);
  printf("myid %d, newid %d, top %d, bottom %d, left %d, right %d \n", 
      myid, newid, nbrtop, nbrbottom, nbrleft, nbrright);
  MPI_Barrier(comm2d);
  usleep(1E6 * PAUSA);

  /* Compute the actual decomposition */
  if (newid == 0) {
    int sobrantes;
    int dist[numprocs], dist_toprow[numprocs];
    sobrantes = nx - 2;  
    for (i=0; i < numprocs - 2; (i += 2)) {
      dist_toprow[i] = nx - 2 - sobrantes;
      dist_toprow[i+1] = dist_toprow[i];
      dist[i] =ceil(2.0*sobrantes/(numprocs-i));
      dist[i+1] =dist[i];
      sobrantes = sobrantes - dist[i];
   }
    dist_toprow[numprocs-2] = nx - 2 - sobrantes;
    dist_toprow[numprocs-1]=dist_toprow[numprocs-2];
    dist[numprocs-2] = sobrantes;
    dist[numprocs-1]=dist[numprocs-2];
    MPI_Scatter(&dist, 1, MPI_INT, &myrows, 1, MPI_INT, 0, comm2d);
    MPI_Scatter(&dist_toprow, 1, MPI_INT, &mytoprow, 1, MPI_INT, 0, comm2d);
  }
  else {
  MPI_Scatter(NULL, 1, MPI_INT, &myrows, 1, MPI_INT, 0, comm2d);
  MPI_Scatter(NULL, 1, MPI_INT, &mytoprow, 1, MPI_INT, 0, comm2d);
  }
  mycols = nyleft-1;
  myleftcol = 0; 
  if (newid % 2 == 1) {
    mycols = nyright - 1;
    myleftcol = nyleft - 1;
  }
  printf("newid: %d, myrows: %d, mycols %d, mytoprow: %d, myleftcol: %d, startcol: %d \n", 
      newid, myrows, mycols, mytoprow, myleftcol, newid%2);
  
  /* Inicializar */
  double(*mynew)[mycols + 2] = calloc(sizeof *mynew, (myrows + 2));
  double(*myold)[mycols + 2] = calloc(sizeof *myold, (myrows + 2));
  double(*myf)[mycols + 2] = calloc(sizeof *myf, (myrows + 2));

  /* As linhas exteriores (i=0, i=myrows+1)
   * e colunas exteriores (j=0, j=mycols+1
   * de myf nunca vao ser usadas. */

  /* Calcula funcao f */
  h = 2 * L / (nx - 1);
  for(i = 1; i < myrows + 1; i++) {
    for(j = 1; j < mycols + 1; j++) {
      myf[i][j] = f(-L + (mytoprow + i) * h, -L + (myleftcol + j) * h);
    }
  }

  /* Condicoes fronteira para o menor valor de x */
  if (nbrtop == MPI_PROC_NULL) {
    for(int j = 0; j < mycols +2; j++) {
      mynew[0][j] = 1 * (-L + (myleftcol + j) * h);
      myold[0][j] = mynew[0][j];
    }
  }
  /* Condicoes fronteira para o maior valor de x */
  if (nbrbottom == MPI_PROC_NULL) {
    for(int j = 0; j < mycols + 2; j++) {
      mynew[myrows + 1][j] = 5/2 + j/2;
      myold[myrows + 1][j] = mynew[myrows + 1][j];
    }
  }
  /* Condicoes fronteira para o menor valor de y */
  if (nbrleft == MPI_PROC_NULL) {
    for(i = 0; i < myrows + 2; i++) {
      mynew[i][0] = 1/2 + 3*i/2;
      myold[i][0] = mynew[i][0];
    }
  }
  /* Condicoes fronteira  para o maior valor de y */
  if (nbrright == MPI_PROC_NULL) {
    for(i = 0; i < myrows + 2; i++) {
      mynew[i][mycols + 1] = 2 + i; 
      myold[i][mycols + 1] = mynew[i][mycols + 1];
    }
  }  
  /* Dependendo dos valores das cond. front. 
   * nos vertices, pode ser necessario fazer
   * alteracoes. */

  /* Novo datatype */
  MPI_Type_vector(myrows, 1, mycols + 2, MPI_DOUBLE, &coluna);
  MPI_Type_commit(&coluna); 

  /* Iteracoes. Nesta fase sao todos iguais */
  /* Ja' calculo as diferencas */
  for (iter = 1; iter <= NMAXITER; iter++) {
    sum2[0] = 0;
    sum2[1] = 0;
    for(i = 1; i < myrows + 1; i++) {
      for(j = 1; j < mycols + 1; j++) {
        mynew[i][j] = 0.25 * (myold[i-1][j]+myold[i+1][j]+myold[i][j-1]
            +myold[i][j+1] - h * h * myf[i][j]);
        sum2[0] = sum2[0] + pow(mynew[i][j] - myold[i][j],2);
        sum2[1] = sum2[1] + pow(mynew[i][j],2);
      }
    }
 
    /* Vamos parar? */
    MPI_Allreduce(&sum2, &sum2_global, 2, MPI_DOUBLE, MPI_SUM, comm2d);
    diff_global = sqrt(sum2_global[0] / sum2_global[1]);
    if (diff_global < CRIT_PAR) { /* Todos terminam.*/
      if (myid == 0) {
        printf("%d %E %E s \n", iter, diff_global, CRIT_PAR);
        t2 = MPI_Wtime();
        printf("Walltime: %f \n", t2 - t1 - PAUSA);
      }

      //TODO colocar código do file
      
      /*Escreve os resultados para o ficheiro*/
      gsizes[0] = nx;
      gsizes[1] = ny;
      lsizes[0] = myrows;
      lsizes[1] = mycols + 1;
      start_indices[0] = mytoprow + 1; 
      start_indices[1] = 0;
      
      if (newid == 0 || newid == 1 || newid == numprocs-2 || newid == numprocs-1) {
        lsizes[0] = myrows +1 ; //lsizes[0]++
      }
      
      if (newid == 0 || newid == 1) {
        start_indices[0] = mytoprow;
      }
      
      if (newid%2 != 0) {
        start_indices[1] = myleftcol + 1;
      }

      MPI_Type_create_subarray(ndims, gsizes, lsizes, start_indices, MPI_ORDER_C, MPI_DOUBLE, &filetype);
      MPI_Type_commit(&filetype);

      MPI_File_open(comm2d, "array2d_b.bin", MPI_MODE_CREATE | MPI_MODE_WRONLY, MPI_INFO_NULL, &fh);
      MPI_File_set_view(fh, 0, MPI_DOUBLE, filetype, "native", MPI_INFO_NULL);

      memsizes[0] = myrows + 2;
      memsizes[1] = mycols + 2;
      start_indices[0] = 1;
      start_indices[1] = newid%2;
      
      if (newid == 0 || newid == 1) {
        start_indices[0] = 0;
      }
      
      MPI_Type_create_subarray(ndims, memsizes, lsizes, start_indices, MPI_ORDER_C, MPI_DOUBLE, &memtype);
      MPI_Type_commit(&memtype);

      MPI_File_write_all(fh, mynew, 1, memtype, MPI_STATUS_IGNORE);
      MPI_File_close(&fh);

      MPI_Finalize();
      exit(0);
    }
    
    /* Trocas */
    MPI_Sendrecv(&mynew[1][1], mycols, MPI_DOUBLE, nbrtop, 0,
        &mynew[myrows+1][1], mycols, MPI_DOUBLE, nbrbottom, 0,
        comm2d, MPI_STATUS_IGNORE);
    MPI_Sendrecv(&mynew[myrows][1], mycols, MPI_DOUBLE, nbrbottom, 1,
        &mynew[0][1], mycols, MPI_DOUBLE, nbrtop, 1,
        comm2d, MPI_STATUS_IGNORE);
    MPI_Sendrecv(&mynew[1][1], 1, coluna, nbrleft, 2, 
        &mynew[1][mycols+1], 1, coluna, nbrright, 2,
        comm2d, MPI_STATUS_IGNORE);
    MPI_Sendrecv(&mynew[1][mycols], 1, coluna, nbrright, 3,
        &mynew[1][0], 1, coluna, nbrleft, 3,
        comm2d, MPI_STATUS_IGNORE);    

    for(i = 0; i < myrows + 2; i++) {
      for(j = 0; j < mycols + 2; j++) {
        myold[i][j] = mynew[i][j];
      }
    } 
  }
  if (myid == 0) printf("Nao convergi.\n"); 
  
  MPI_Finalize();
  return 0;
}

double f(double x, double y) {
  return  2 - x * x - 10 * y + 50 * x * y;
}  

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <mpi.h>
#include <unistd.h>

#include "./shared.c"
#include "./reductions/linear.c"
#include "./reductions/nonBlockLinear.c"
#include "./reductions/binary.c"
#include "./reductions/mpiGather.c"
#include "./reductions/mpiReduce.c"
#include "./reductions/mpiWindows.c"


#ifndef SEED
  #define SEED 921
#endif
#ifndef NUM_ITER
  #define NUM_ITER 1000
#endif
#ifndef TYPE
  #define TYPE 0
#endif

int main(int argc, char* argv[])
{
  double _startTime, _endTime;
  double startTime, endTime;


  MPI_Init(NULL, NULL);
  _startTime = MPI_Wtime();
  int world_size, world_rank;
  MPI_Comm_size(MPI_COMM_WORLD, &world_size);
  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

  MPI_Barrier(MPI_COMM_WORLD);
  startTime = MPI_Wtime();

  srand(SEED * world_rank);

  #if TYPE == 0
    int iters = NUM_ITER;
  #else
    int iters = NUM_ITER / world_size;
  #endif
  
  int count = computeIters(iters);
  

  #if TYPE == 1
    count = linearReduction(count, world_size, world_rank);
  #elif TYPE == 2
    count = binaryReduction(count, world_size, world_rank);
  #elif TYPE == 3
    count = nonBlockLinearReduction(count, world_size, world_rank);
  #elif TYPE == 4
    count = mpiGather(count, world_size, world_rank);
  #elif TYPE == 5
    count = mpiReduction(count, world_size, world_rank);
  #elif TYPE == 6
    count = mpiWindows(count, world_size, world_rank);
  #endif

  MPI_Barrier(MPI_COMM_WORLD);
  endTime = MPI_Wtime();
  _endTime = MPI_Wtime();

  MPI_Finalize();
  
  if(world_rank == 0) {
    computeResult(world_size, endTime - startTime, _endTime - _startTime, count, NUM_ITER);
  }

  return 0;
}

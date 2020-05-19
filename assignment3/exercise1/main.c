#include <stdio.h>
#include <mpi.h>

int main(int argc, char* argv[])
{
  double startTime, endTime;

  MPI_Init(NULL, NULL);
  int world_size, world_rank;
  MPI_Comm_size(MPI_COMM_WORLD, &world_size);
  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

  
  printf("Hello World from rank %d from %d processes!\n", world_rank, world_size);

  MPI_Finalize();
  
  return 0;
}

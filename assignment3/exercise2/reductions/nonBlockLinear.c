int nonBlockLinearReduction(
  int localCount,
  int world_size,
  int world_rank
) {
  if(world_rank == 0) {
    MPI_Request *requests = malloc(sizeof(MPI_Request) * (world_size - 1));
    int *counts = malloc(sizeof(int) * (world_size - 1));
    int *recvs = malloc(sizeof(int) * (world_size - 1));

    for(int p = 1; p < world_size; p++) {
      MPI_Request *request = requests + (p - 1);
      int *count = counts + (p - 1);
      int *recv = recvs + (p - 1);

      *count = 1;

      MPI_Irecv(recv, *count, MPI_INT, p, 0, MPI_COMM_WORLD, request);
    }
    for(int p = 1; p < world_size; p++) {
      MPI_Request *request = requests + (p - 1);
      int *count = counts + (p - 1);
      int *recv = recvs + (p - 1);
      MPI_Wait(request, MPI_STATUS_IGNORE);

      localCount += *recv;
    }
  } else {
    MPI_Ssend(&localCount, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
  }
  return localCount;
}
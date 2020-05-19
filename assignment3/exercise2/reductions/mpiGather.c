int mpiGather(
  int localCount,
  int world_size,
  int world_rank
) {
  int *recvs;
  if(world_rank == 0) {
    recvs = malloc(sizeof(int) * world_size);
  }
  
  MPI_Gather(&localCount, 1, MPI_INT, recvs, 1, MPI_INT, 0, MPI_COMM_WORLD);

  if(world_rank == 0) {
    // Skip local result in recvs by starting at process 1
    for(int p = 1; p < world_size; p++) {
      int *recv = recvs + p;
      localCount += *recv;
    }
  }

  return localCount;
}
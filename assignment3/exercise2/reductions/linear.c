int linearReduction(
  int localCount,
  int world_size,
  int world_rank
) {
  if(world_rank == 0) {
    int recv[1];

    for(int p = 1; p < world_size; p++) {
      MPI_Recv(recv, 1, MPI_INT, p, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      localCount += recv[0];
    }
  } else {
    MPI_Send(&localCount, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
  }
  return localCount;
}
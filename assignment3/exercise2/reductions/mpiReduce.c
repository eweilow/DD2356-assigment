int mpiReduction(
  int localCount,
  int world_size,
  int world_rank
) {
  int recv = 0;
  MPI_Reduce(&localCount, &recv, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

  return recv;
}
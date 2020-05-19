int mpiWindows(
  int localCount,
  int world_size,
  int world_rank
) {
  int output = 0;

  MPI_Win win;
  MPI_Win_create(&output, sizeof(int), sizeof(int), MPI_INFO_NULL, MPI_COMM_WORLD, &win);
  MPI_Win_fence(0, win);

  MPI_Accumulate(&localCount, 1, MPI_INT, 0, 0, 1, MPI_INT, MPI_SUM, win);

  MPI_Win_fence(0, win);
  MPI_Win_free(&win);

  return output;
}
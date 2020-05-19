int binaryReduction(
  int localCount,
  int world_size,
  int world_rank
) {
  int commSteps = (int)ceil(log2(world_size));

  int width = 1;

  // bool skipExecution = false;

  int recv[1];

  for(long step = 0; step < commSteps; step++) {
    //if(true) {
      int sendTo = world_rank - width;
      int recvFrom = world_rank + width;
      int currProc = world_rank / width;
      
      if(currProc % 2 == 1) {
        // printf("step %d, %d sends %d\n", step, world_rank, sendTo);
        MPI_Send(&localCount, 1, MPI_INT, sendTo, 0, MPI_COMM_WORLD);
        // skipExecution = true;
        break;
      } else if(recvFrom < world_size) {
        // printf("step %d, %d recvs %d\n", step, world_rank, recvFrom);
        MPI_Recv(recv, 1, MPI_INT, recvFrom, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        localCount += recv[0];
      }
      width *= 2;
    //
    // MPI_Barrier(MPI_COMM_WORLD);
  }

  return localCount;
}
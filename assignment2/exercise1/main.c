#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

int main (int argc, char *argv[]) 
{
  #pragma omp parallel
  {
    int threadId = omp_get_thread_num();
    int numThreads = omp_get_num_threads();

    printf("Hello World from Thread %d of %d!\n", threadId, numThreads);
  }

  return 0;
}
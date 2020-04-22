#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h> 

#define N 1000000


#define WARMUP 2
#define RUNS 10

typedef struct {
  double maxval;
  int maxloc;
} __attribute__((aligned(64))) maxval_t;

double runIteration() {
  double* x = malloc(sizeof(double) * N);

  for(int i=0; i < N; i++){
    // Generate random number between 0 and 1
    x[i] = ((double)(rand()) / RAND_MAX)*((double)(rand()) / RAND_MAX)*((double)(rand()) / RAND_MAX)*1000;
  }
  
  double validationmaxval = 0.0; 
  int validationmaxloc = 0;

  for(int i=0; i < N; i++) {
    if(x[i] > validationmaxval) {
      validationmaxloc = i;
      validationmaxval = x[i];
    }
  }
  
  double maxval = 0.0; 
  int maxloc = 0;
  
  struct timespec ts_start;
  clock_gettime(CLOCK_MONOTONIC, &ts_start);

  #if SERIAL // Question 1.
    for(int i=0; i < N; i++) {
      if(x[i] > maxval) {
        maxloc = i;
        maxval = x[i];
      }
    }
  #elif PARALLEL1 // Question 2.
    #pragma omp parallel for
    for(int i=0; i < N; i++) {
      if(x[i] > maxval) {
        maxloc = i;
        maxval = x[i];
      }
    }
  #elif PARALLEL2 // Question 3.
    #pragma omp parallel for
    for(int i=0; i < N; i++) {
      #pragma omp critical
      if(x[i] > maxval) {
        maxloc = i;
        maxval = x[i];
      }
    }
  #elif PARALLEL3 // Question 4.
    int numThreads = omp_get_max_threads();

    int maxloc_threads[numThreads];
    double maxval_threads[numThreads];

    #pragma omp parallel
    {
      int threadId = omp_get_thread_num();
      maxloc_threads[threadId] = 0;
      maxval_threads[threadId] = 0.0;
      #pragma omp for
      for(int i = 0; i < N; i++) {
        if(x[i] > maxval_threads[threadId]) {
          maxloc_threads[threadId] = i;
          maxval_threads[threadId] = x[i];
        }
      }
    }

    for(int i=0; i < numThreads; i++) {
      if(maxval_threads[i] > maxval) {
        maxloc = maxloc_threads[i];
        maxval = maxval_threads[i];
      }
    }
  #elif PARALLEL4 // Question 5.
    int numThreads = omp_get_max_threads();

    maxval_t thread_data[numThreads];

    #pragma omp parallel
    {
      int threadId = omp_get_thread_num();
      thread_data[threadId].maxloc = 0;
      thread_data[threadId].maxval = 0.0;

      #pragma omp for 
      for(int i = 0; i < N; i++) {
        if(x[i] > thread_data[threadId].maxval) {
          thread_data[threadId].maxloc = i;
          thread_data[threadId].maxval = x[i];
        }
      }
    }

    for(int i=0; i < numThreads; i++) {
      if(thread_data[i].maxval > maxval) {
        maxloc = thread_data[i].maxloc;
        maxval = thread_data[i].maxval;
      }
    }
  #endif

  struct timespec ts_end;
  clock_gettime(CLOCK_MONOTONIC, &ts_end);

  free(x);

  if(abs(validationmaxloc - maxloc) > 0) {
    printf("maxloc = %d but expected maxloc = %d !!!\n", maxloc, validationmaxloc);
  }

  if(fabs(validationmaxval - maxval) > 0) {
    printf("maxval = %lf but expected maxval = %lf !!!\n", maxval, validationmaxval);
  }


  double startTime = ts_start.tv_sec*1e3 + ts_start.tv_nsec*1e-6;
  double endTime = ts_end.tv_sec*1e3 + ts_end.tv_nsec*1e-6;

  // printf("%ld %ld\n", ts_start.tv_sec, ts_start.tv_nsec);
  // printf("%ld %ld\n", ts_end.tv_sec, ts_end.tv_nsec);
  // printf("%lf %lf %lf\n", startTime, endTime, endTime - startTime);

  return endTime - startTime;
}

int main (int argc, char *argv[]) 
{
  double totalTime = 0.0;

  srand(time(0));
  for(int n = 0; n < RUNS + WARMUP; n++) {
    double diff = runIteration();
  
    if(n >= WARMUP) {
      totalTime += diff;
    }
  }
  
  printf("%lf\n", totalTime / (double)RUNS);

  
  return 0;
}
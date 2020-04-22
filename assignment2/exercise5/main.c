#define SCHEDULE schedule(guided)

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <omp.h>
#include "types.c"
#include "debug.c"
#include "force.c"
#include "integrate.c"

// #define USE_REDUCED 1
#define dt ((double) 0.05)
//#define N 10000
//#define STEPS 100

vect_t thread_local_forces[N];
#pragma omp threadprivate(thread_local_forces)

int main() {
  data_t data = init(N);
  #if DEBUG_PRINT
    debugPrint(data);
  #endif

  double start_time = omp_get_wtime();
  

  #pragma omp parallel
  {
    for(int i = 0; i < STEPS; i++) {
      #if USE_REDUCED
        computeForcesReduced(data.n, data.pos, data.mass, data.force, thread_local_forces);
      #else
        computeForces(data.n, data.pos, data.mass, data.force);
      #endif
      #pragma omp barrier
      integrate(data, dt);
      #pragma omp barrier
    }
  }
  double run_time = omp_get_wtime() - start_time;
  
  #if DEBUG_PRINT
    debugPrint(data);
    printf("%.2lf ms x %d = %.2lf ms\n", 1000.0*run_time, omp_get_max_threads(), 1000.0*run_time * omp_get_max_threads());
  #else
    printf("%.4lf\n", 1000.0*run_time);
  #endif

  return 0;
}
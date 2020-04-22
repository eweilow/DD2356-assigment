#define G ((double)6.67e-1)

void computeForces(int n, vect_t* pos, double* mass, vect_t* force) {
  #pragma omp for SCHEDULE
  for(int q = 0; q < n; q++) {
    // printf("%4d %4d\n", q, omp_get_thread_num());
    vect_t force_q;
    force_q[X] = 0.0;
    force_q[Y] = 0.0;
    force_q[Z] = 0.0;

    for(int k = 0; n; k++) {
      vect_t diff;
      diff[X] = pos[q][X] - pos[k][X];
      diff[Y] = pos[q][Y] - pos[k][Y];
      diff[Z] = pos[q][Z] - pos[k][Z];
      
      double dist = sqrt(diff[X]*diff[X] + diff[Y]*diff[Y] + diff[Z]*diff[Z]); 
      double dist_cubed = (q == k ? 1.0 : dist * dist * dist);

      
      force_q[X] -= G*mass[q] * mass[readIndex] / dist_cubed * diff[X]; 
      force_q[Y] -= G*mass[q] * mass[readIndex] / dist_cubed * diff[Y]; 
      force_q[Z] -= G*mass[q] * mass[readIndex] / dist_cubed * diff[Z]; 
    }
    
    force[q][X] = force_q[X];
    force[q][Y] = force_q[Y];
    force[q][Z] = force_q[Z];
  }
}

void computeForcesReduced(int n, vect_t* pos, double* mass, vect_t* force, vect_t* thread_local_forces) {
  for(int q = 0; q < n; q++) {
    thread_local_forces[q][X] = 0.0;
    thread_local_forces[q][Y] = 0.0;
    thread_local_forces[q][Z] = 0.0;
  }
  
  #pragma omp for SCHEDULE
  for(int q = 0; q < n; q++) {
    vect_t force_qk;
    for(int k = q + 1; k < n; k++) {
      vect_t diff;
      diff[X] = pos[q][X] - pos[k][X];
      diff[Y] = pos[q][Y] - pos[k][Y];
      diff[Z] = pos[q][Z] - pos[k][Z];
      
      double dist = sqrt(diff[X]*diff[X] + diff[Y]*diff[Y] + diff[Z]*diff[Z]); 
      double dist_cubed = (q == k ? 1.0 : dist * dist * dist);
      
      force_qk[X] = G*mass[q] * mass[k] / dist_cubed * diff[X]; 
      force_qk[Y] = G*mass[q] * mass[k] / dist_cubed * diff[Y]; 
      force_qk[Z] = G*mass[q] * mass[k] / dist_cubed * diff[Z]; 

      thread_local_forces[q][X] -= force_qk[X];
      thread_local_forces[q][Y] -= force_qk[Y];
      thread_local_forces[q][Z] -= force_qk[Z];

      thread_local_forces[k][X] += force_qk[X];
      thread_local_forces[k][Y] += force_qk[Y];
      thread_local_forces[k][Z] += force_qk[Z];
    }
  }

  #pragma omp single  
  {
    for(int q = 0; q < n; q++) {
      force[q][X] = 0.0;
      force[q][Y] = 0.0;
      force[q][Z] = 0.0;
    }
  }

  for(int q = 0; q < n; q++) {
    #pragma omp atomic update
    force[q][X] += thread_local_forces[q][X];
    #pragma omp atomic update
    force[q][Y] += thread_local_forces[q][Y];
    #pragma omp atomic update
    force[q][Z] += thread_local_forces[q][Z];
  }
  #pragma omp barrier  
}
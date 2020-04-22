void integrate(data_t data, double dt) {
  #pragma omp for
  for(int q = 0; q < data.n; q++) {
    // Apply velocity first to get Euler-Cromer and much better energy preservation
    data.vel[q][X] += dt / data.mass[q] * data.force[q][X];
    data.vel[q][Y] += dt / data.mass[q] * data.force[q][Y];
    data.vel[q][Z] += dt / data.mass[q] * data.force[q][Z];

    data.pos[q][X] += dt * data.vel[q][X];
    data.pos[q][Y] += dt * data.vel[q][Y];
    data.pos[q][Z] += dt * data.vel[q][Z];
  }
}
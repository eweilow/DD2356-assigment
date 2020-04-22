void debugPrint(data_t data) {
  printf("%6s | %12s %12s %12s | %12s %12s %12s | %12s %12s %12s | %12s\n", 
    "i", "p_x", "p_y", "p_z", "v_x", "v_y", "v_z", "F_x", "F_y", "F_z", "mass"
  );
  int step = data.n/4;
  if(step < 1) {
    step = 1;
  }
  for(int q = 0; q < data.n; q += step) {
    printf("%6ld | %12.6f %12.6f %12.6f | %12.6f %12.6f %12.6f | %12.6f %12.6f %12.6f | %12.6f\n", 
      q,
      data.pos[q][X],
      data.pos[q][Y],
      data.pos[q][Z],
      data.vel[q][X],
      data.vel[q][Y],
      data.vel[q][Z],
      data.force[q][X],
      data.force[q][Y],
      data.force[q][Z],
      data.mass[q]
    );
  }
}
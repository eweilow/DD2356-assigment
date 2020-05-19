int computeIters(
  int iters
) {
  double x, y, z, pi;
  
  int count = 0;
  // Calculate PI following a Monte Carlo method
  for (int iter = 0; iter < iters; iter++)
  {
    // Generate random (X,Y) points
    x = (double)random() / (double)RAND_MAX;
    y = (double)random() / (double)RAND_MAX;
    z = sqrt((x*x) + (y*y));
    
    // Check if point is in unit circle
    if (z <= 1.0)
    {
        count++;
    }
  }
  return count;
}

void computeResult(
  int size,
  double computationTime,
  double _computationTime,
  int count,
  int iter
){
  // Estimate Pi and display the result
  double pi = ((double)count / (double)iter) * 4.0;
  
  printf("N %d, TYPE %d, TIME: %.4f (%.4f), RESULT: %.16f\n", size, (int)TYPE, computationTime, _computationTime, pi);
}
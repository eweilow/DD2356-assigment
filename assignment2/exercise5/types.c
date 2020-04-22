#define DIM 3
typedef double vect_t[DIM];

#define X 0
#define Y 1
#define Z 2

typedef struct {
  int n;
  vect_t* force;
  vect_t* pos;
  vect_t* vel;
  double* mass;
} data_t;


data_t init(int n) {
  data_t* data = malloc(sizeof(data_t));
  data->n = n;
  data->force = calloc(sizeof(vect_t), n);
  data->pos = malloc(sizeof(vect_t) * n);
  data->vel = malloc(sizeof(vect_t) * n);
  data->mass = malloc(sizeof(double) * n);
  
  for (int q = 0; q < n; q++) {
    for(int d = 0; d < DIM; d++) {
		  data->pos[q][d] = (rand() / (double)(RAND_MAX)) * 2 - 1;
    }
    for(int d = 0; d < DIM; d++) {
		  data->vel[q][d] = 0.0; //(rand() / (double)(RAND_MAX)) * 2 - 1;
    }

    data->mass[q] = fabs((rand() / (double)(RAND_MAX)) * 2 - 1);
  }

  return *data;
}
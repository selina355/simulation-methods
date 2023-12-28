void write_array(char* filename,double * array ,int arraylength);
void write_2darray(char* filename,int ** array ,int dim1, int dim2);
int** initial_system(int L, int**M);
double sum_neighbours( int**M, int r1,int r2, int L);
void monte_carlo_step(int L, int**M, double T);

double calculate_magnetisation_per_spin(int**M, int L);
double calculate_energy_per_spin(int**M, int L);
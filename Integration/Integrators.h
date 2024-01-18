FILE *fp;
void euler_step(int i, double* x ,double* p, double dt)
{
    x[i] = x[i-1] + p[i-1]*dt;
    p[i] = p[i-1] -x[i-1]*dt;

}

void symplectic_euler_step(int i, double* x ,double* p, double dt)
{
    p[i] = p[i-1] -x[i-1]*dt;
    x[i] = x[i-1] + p[i]*dt;
    
}

void initialise_verlet(int i, double * x, double *v, double * a, double dt, double omega)
{
    x[1]= x[0] + v[0]*dt + 0.5*a[0]*dt*dt;
    a[1]= -omega * omega * x[1];
}

void verlet_step(int i, double *x, double*v, double*a, double dt,double omega)
{
    if (i<1){printf("verlet needs to be initialised!"); return;}
    x[i]= 2*x[i-1] - x[i-2] + a[i-1]*dt*dt;
    a[i] = -omega * omega * x[i];
    v[i-1]=(x[i]-x[i-1])/(2*dt);
}

void velocity_verlet_step(int i, double *x, double*v, double*a, double dt,double omega)
{
    //1st half kick
    double v_half = v[i-1] + dt/2*a[i-1];

    //drift
    x[i] = x[i-1] + v_half*dt;
    
    //calculate forces
    a[i]= -omega*omega*x[i];

    //2nd half kick
    v[i]=v_half+ dt/2*a[i];

}
double calculate_H(double x,double v,double k)
{
    double H= 0.5 * v*v +0.5 *k*x*x;
    return H;

}
double calculate_shadow_H(double x,double v,double k,double dt)
{
    double H= 0.5 * v*v +0.5 *k*x*x -v*x*dt/2;
    return H;
}

void write_array(char* filename,double * array ,int arraylength){

  /*Function: write_array
    writes float array to new line at the bottom of the specified file, which should already exists.
    values separated by ";".

    *********************
    Parameters:
      char* filename : existing file to write to
      double* array : array that will be written
      int arraylength: length of array to be written
  */

  //open and find end of file
  fp = fopen(filename, "r+");
  fseek(fp,0, SEEK_END);
  //print contents of array to file
  int i;
  int N =arraylength;
  for(i=0;i<N;i++){
    fprintf(fp,"%.6f;", array[i]);

  }
  //delete last ";" and replace with a linebreak
  fseek(fp,-1, SEEK_END);
  fputs("\n",fp);
  fclose(fp);


}


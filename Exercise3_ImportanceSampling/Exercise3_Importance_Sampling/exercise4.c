#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include <time.h>
#include "exercise3_C.h" 
FILE *fp;

char* FILENAME = "data4.csv";



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


//**********************************************************************************************************************************
//CRUDE MONTE CARLO AND IMPORTANCE SAMPLING 

double Integrand_monte_carlo(double x)

{
  return (exp(-x*x)*sqrt(x));

}

double crude_monte_carlo(int N, double a, double b)
/* returns crude monte estimate of the integrand in Integrand_monte_carlo
Parameters:
int N : number of steps 
int a , in b : integration intervall borders*/
{
  int i;
 
  double sum_of_f_values=0;
  for(i=0;i<N;i++)
  {
    double x = a + (b-a)*drand48();
    sum_of_f_values+=Integrand_monte_carlo(x);
  }
  double mean= sum_of_f_values /N;
  double integral = (b-a)*mean;
  return (integral);

}


double importance_sampling_f(int N)
{
 
  int i = 0;
  double sum=0;
  double integral=0;

  for(i=0;i<N;i++)
  {
    double x_i = rejection_sample_g(0.7, 1.7);//uses function from exercise3_C.c!!!!
    sum+= sqrt(x_i);
  }
  integral = sqrt(M_PI)*sum/(2*N);
  return(integral);
  
}

/* ************************************************************************************************
INTEGRATION OF COSINE */

double sample_h(void)
//sample from helper function 
{
  double c = M_PI/2 -M_PI*M_PI/16;
  double u = drand48();
  return (2-sqrt((4-4*c*u)));

}

double h(double x)
//helper function
{
  double c = M_PI/2*(1-M_PI/8);
  return 1/c*(1-0.5*x);
}


double g_star(double x)
//weighting function in importance sampling
{
  double z=M_PI/2*(1-M_PI*M_PI/24);
  return (1-0.4*x*x);
}

double rejection_sample_g_star_for_cos(double C)
//sample from the weighting function with rejection sampling using the helper function
{
  double c = M_PI/2*(1-M_PI/8);
  double z=M_PI/2*(1-M_PI*M_PI/24);

  double x= sample_h();
  double f_value = g_star(x);
  double g_value = h(x);
  double u = drand48();
  
  while(u >= f_value/(C*g_value))
  {
    x= sample_h();
    f_value = g_star(x);
    g_value = h(x);
    u = drand48();
  }
  return x;
}

double importance_sampling_cos(int N)
{
  
  int i = 0;
  double sum=0;
  double integral=0;
  double z=M_PI/2*(1-M_PI*M_PI*0.4/12);

  for(i=0;i<N;i++)
  {
    double x_i = rejection_sample_g_star_for_cos( 1.3);
    sum+= cos(x_i)/g_star(x_i);
  }

  integral = z*sum/(N);

  return(integral);
  
}








int main()
{
  srand48((unsigned)time(NULL));
  fp= fopen( FILENAME,"w");
  //write header to file
  fprintf(fp, "Ns, crude_integrals, importance_integrals,test helper sampling, test weighting sampling\n ");

  fclose(fp);

  int j=0;
  double a=0;
  double b= 1000;


  
  int Ns[] = {1000,5000,10000, 50000,100000,500000};
 

  double Ns_double[6];
  double integral_crudeMC[6];
  double integral_importance[6];
  for(j=0;j<6; j++) 
  {
    integral_crudeMC[j] = crude_monte_carlo(Ns[j],a,b);
    printf("integral crude method: %f \n", integral_crudeMC[j]);
    integral_importance[j] = importance_sampling_f(Ns[j]);
    printf("integral importance sampling method: %f \n", integral_importance[j]);
    Ns_double[j]=(double)Ns[j];
  }

  write_array(FILENAME,Ns_double,6);
  write_array(FILENAME,integral_crudeMC, 6 );
  write_array(FILENAME, integral_importance,6);

  int N=500000;
  double test_sample_h[N];
  double test_sample_g_star[N];


  for (j=0;j<N;j++)
  {
    test_sample_h[j]= sample_h();
    test_sample_g_star[j]=rejection_sample_g_star_for_cos(1.3);
  }

  write_array(FILENAME, test_sample_h,N);
  write_array(FILENAME, test_sample_g_star, N);
  N = 55;
  double cos_integral_true=1.;

  double cos_int_est=importance_sampling_cos(N);
  printf("integral of cos importance sampling method: %f \n",cos_int_est);





  



}

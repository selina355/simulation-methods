

/*
contains all the necessary functions to sample from the 1d gaussian with rejection sampling, for task 3 and 4!
*/

#include <stdlib.h>
#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <math.h>



double sample_x_from_g(double p)
/*uses inversion sampling to sample from normalised g(x) as derived in notes*/
{
  
  double x;
  double A =  1/(p+1/(2*p));
  double y = drand48();

  if (y<= A*p)
  {
    x = y/(A);
    return(x);
  }

  else
  {
    double inside_log= -y*2*p/(A)+ 2*pow(p,2) +1;
    double inside_sqrt = -(log(inside_log)-pow(p,2));
    x = pow(inside_sqrt,0.5);
    return(x);
  } 
}


double g(double x, double p)
{
  double A= 1/(p+1/(2*p));
  if (x<=p){
    return A;
  }

  else{
    return (A/(p)* x* exp(pow(p,2)-pow(x,2)));
  }
}

double f_func(double x)
{
  double y = 2*pow(1/M_PI,0.5)*exp(-x*x);
  return y;
}

double rejection_sample_g( double p, double c)
/* implenmentation of the rejection method to sample from the gaussian
 samples from the enveloping function g(X) with shape parameter p and rejects values if u <= f(x)/c*g(x) with u uniform[0,1]

 parameters: 
 int N: length of array to eb filled up
 double p: shape parameter of the function g(x)
 double c: value close to 1 that determines how close to the distribution rejections start
*/
{
  
  double x= sample_x_from_g(p);
  double f_value = f_func(x);
  double g_value = g(x,p);
  double u = drand48();

  while(u >= f_value/(c*g_value))
  {
    x= sample_x_from_g(p);
    f_value = f_func(x);
    g_value = g(x,p);
    u= drand48();
  }
  return x;
 
}

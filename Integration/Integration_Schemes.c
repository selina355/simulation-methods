#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Integrators.h"

double * x, *t, *v,*a,*H,*H_shadow,*err;

int main()
{
    fp= fopen( "Intgeration_Data.csv","w");
    fclose(fp);

    int i;
    double dt_small =  0.001;
    double dt_big = 0.01;
    double T= 10.;

    int N_small=(int) T/dt_small;
    int N_big= (int) T/dt_big;
    x = ( double *)malloc(N_small*sizeof(double));
    v = ( double *)malloc(N_small*sizeof(double));
    t= ( double *)malloc(N_small*sizeof(double));
    H = ( double *)malloc(N_small*sizeof(double));
    err= ( double *)malloc(N_small*sizeof(double));

    x[0]= 1;
    v[0]= 0;
    t[0]=0;
    H[0]=calculate_H(x[0],v[0], 1.);
    err[0]=0;

    for(int i=1;i<N_small+1; i++)
    {
        t[i]= i*dt_small;
        euler_step(i,x,v,dt_small);
        H[i]=calculate_H(x[i],v[i],1);
        err[i]= (x[i]-cos(t[i]))/cos(t[i]);
   
    }

    write_array("Intgeration_Data.csv",t,N_small);
    write_array("Intgeration_Data.csv",x,N_small);
    write_array("Intgeration_Data.csv",v,N_small);
    write_array("Intgeration_Data.csv",err,N_small);
    write_array("Intgeration_Data.csv",H,N_small);
    
    for(int i=1;i<N_small+1; i++)
    {
        symplectic_euler_step(i,x,v,dt_small);
        H[i]=calculate_H(x[i],v[i],1);
        err[i]= (x[i]-cos(t[i]))/cos(t[i]);
    }

    write_array("Intgeration_Data.csv",x,N_small);
    write_array("Intgeration_Data.csv",v,N_small);
    write_array("Intgeration_Data.csv",err,N_small);
    write_array("Intgeration_Data.csv",H,N_small);
    

    free(x);free(v);free(t);free(H);free(err);

}
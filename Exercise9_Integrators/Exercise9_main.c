#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Integrators.h"

double * x, *t, *v,*v_bad,*a,*H,*H_bad,*H_shadow,*err;

int main()
{

    fp= fopen( "Intgeration_Data.csv","w");
    fclose(fp);


    int i,j;
    double dt_small =  0.001;
    double dt_big = 0.01;
    double T= 10.;

    int N_small= (int) T/dt_small;
    int N_big= (int) T/dt_big;

//*************************** Euler small stepsize***********************************************
    x = ( double *)malloc(N_small*sizeof(double));
    v = ( double *)malloc(N_small*sizeof(double));
    
    t= ( double *)malloc(N_small*sizeof(double));
    H = ( double *)malloc(N_small*sizeof(double));
    H_shadow = ( double *)malloc(N_small*sizeof(double));
    err= ( double *)malloc(N_small*sizeof(double));

    x[0]= 1;
    v[0]= 0;
    t[0]=0;
    H[0]=calculate_H(x[0],v[0], 1.);
    H_shadow[0]= calculate_shadow_H(x[0],v[0], 1.,dt_small);
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


//*************************** Symplectic small stepsize***********************************************
    for(int i=1;i<N_small+1; i++)
    {
        symplectic_euler_step(i,x,v,dt_small);
        H[i]=calculate_H(x[i],v[i],1);
        H_shadow[i]= calculate_shadow_H(x[i],v[i],1.,dt_small);
        
        err[i]= (x[i]-cos(t[i]))/cos(t[i]);
    }
    write_array("Intgeration_Data.csv",x,N_small);
    write_array("Intgeration_Data.csv",v,N_small);
    write_array("Intgeration_Data.csv",err,N_small);
    write_array("Intgeration_Data.csv",H,N_small);
    write_array("Intgeration_Data.csv",H_shadow,N_small);
   
 //*************************** Euler big stepsize***********************************************   
    x = ( double *)realloc(x,N_big*sizeof(double));
    v = ( double *)realloc(v,N_big*sizeof(double));
    t= ( double *)realloc(t,N_big*sizeof(double));
    H = ( double *)realloc(H,N_big*sizeof(double));
    err= ( double *)realloc(err,N_big*sizeof(double));
    H_shadow=(double *)realloc(H_shadow,N_big*sizeof(double));


    x[0]= 1;
    v[0]= 0;
    t[0]=0;
    H[0]=calculate_H(x[0],v[0], 1.);
    H_shadow[0]= calculate_shadow_H(x[0],v[0], 1.,dt_big);
    err[0]=0;

    for(i=1;i<N_big+1; i++)
    {
        t[i]= i*dt_big;
        euler_step(i,x,v,dt_big);
        H[i]=calculate_H(x[i],v[i],1);
        err[i]= (x[i]-cos(t[i]))/cos(t[i]);
   
    }

    write_array("Intgeration_Data.csv",t,N_big);
    write_array("Intgeration_Data.csv",x,N_big);
    write_array("Intgeration_Data.csv",v,N_big);
    write_array("Intgeration_Data.csv",err,N_big);
    write_array("Intgeration_Data.csv",H,N_big);

//*************************** Symplectic stepsize***********************************************    
    for(i=1;i<N_big+1; i++)
    {

        symplectic_euler_step(i,x,v,dt_big);
        H[i]=calculate_H(x[i],v[i],1);
        H_shadow[i]= calculate_shadow_H(x[i],v[i],1.,dt_big);
        err[i]= (x[i]-cos(t[i]))/cos(t[i]);
   
    }
    write_array("Intgeration_Data.csv",x,N_big);
    write_array("Intgeration_Data.csv",v,N_big);
    write_array("Intgeration_Data.csv",err,N_big);
    write_array("Intgeration_Data.csv",H,N_big);
    write_array("Intgeration_Data.csv",H_shadow,N_big);
  
//****************************Velocity Verlet******************************
    N_big= 10000;
    dt_big=0.1;
    x = ( double *)realloc(x,N_big*sizeof(double));
    v = ( double *)realloc(v,N_big*sizeof(double));
    t= ( double *)realloc(t,N_big*sizeof(double));
    H = ( double *)realloc(H,N_big*sizeof(double));
    H_shadow = ( double *)realloc(H_shadow,N_big*sizeof(double));
    err= ( double *)realloc(err,N_big*sizeof(double));

    a = ( double *)malloc(N_big*sizeof(double));
    x[0]= 1.;
    v[0]= 0;
    t[0]=0;
    a[0]= -x[0];

    H[0]=calculate_H(x[0],v[0], 1.);
    err[0]=0;

    for(i=1;i<N_big+1; i++)
    {

        velocity_verlet_step(i,x,v,a,dt_big, 1.);
   
        err[i]= x[i]-cos(i*dt_big);
   
    }
    for(i=0;i<N_big+1; i++)
    {
      H[i]=calculate_H(x[i],v[i],1.);
    }
    write_array("Intgeration_Data.csv",x,N_big);
    write_array("Intgeration_Data.csv",v,N_big);
    write_array("Intgeration_Data.csv",err,N_big);
    write_array("Intgeration_Data.csv",H,N_big);

//*********************** regular verlet*********************************
    v_bad = ( double *)malloc(N_small*sizeof(double));
    H_bad = (double*)malloc(N_small*sizeof(double));
    x[0]= 1;
    v[0]= 0;
    v_bad[0]=0;
    t[0]=0;
    a[0]= -x[0];

    initialise_verlet(x,v,a,dt_big,1.);

    for(i=2;i<N_big+1; i++)
    {
        verlet_step(i,x,v,v_bad,a,dt_big,1.);
        err[i]= x[i]-cos(i*dt_big);
        
    }

    for(i=0;i<N_big+1; i++)
    {
        H[i]=calculate_H(x[i],v[i],1.);
        H_bad[i]=calculate_H(x[i],v_bad[i],1.);
        

    }

    write_array("Intgeration_Data.csv",x,N_big);
    write_array("Intgeration_Data.csv",v,N_big);
    write_array("Intgeration_Data.csv",v_bad,N_big);
    write_array("Intgeration_Data.csv",err,N_big);
    write_array("Intgeration_Data.csv",H,N_big);
    write_array("Intgeration_Data.csv",H_bad,N_big);

//****************test Velocity verlet for different timesteps*****************
    double test_dt[6]={ 0.01,0.5,1.5,2,2.5,3};
    for(j=0; j<6;j++)
    {

        for(i=1;i<N_big+1; i++)
        {
            velocity_verlet_step(i,x,v,a,test_dt[j], 1.);
            err[i]= x[i]-cos(i*test_dt[j]);
            
        }
        write_array("Intgeration_Data.csv",err,N_big);



    }
    for(j=0; j<6;j++)
    {

        for( i=1;i<N_big+1; i++)
        {
            velocity_verlet_step(i,x,v,a,test_dt[j], 1.);
            
    
        }
        write_array("Intgeration_Data.csv",x,N_big);
        write_array("Intgeration_Data.csv",v,N_big);
    }



    
    



    free(x);free(v);free(v_bad);free(t);free(a);free(H); free(H_bad);free(err);free(H_shadow);


}
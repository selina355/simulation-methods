#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <time.h>
//#include "array_alloc.h"
FILE *fp;

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

void gillespie_step_LV(int j,double * times, double * x_var, double * y_var, double* k){
    double x_curr= x_var[j-1];
    double  y_curr= y_var[j-1];
    double t_curr= times[j-1];

    double w[4];
    int i,l;

    w[0] = k[0] * x_curr;
    w[1] = k[1]* x_curr*y_curr;
    w[2] = k[2] *y_curr;
    w[3]=0.;

    for(i=0;i<3;i++){
        w[3]+=w[i];
    }

    
    
    double u = drand48()*w[3];
    double tau= -1/w[3]*log(drand48());
    times[j]= t_curr +tau;
   

    if (u<=w[0]){
        x_var[j]= x_curr +1;
    
        y_var[j]= y_curr;
    }
          
    if((u>w[0]) & (u<=(w[0]+w[1]))){
        x_var[j]= x_curr-1;
        y_var[j]= y_curr +1;
    }
       
        
    if(u>(w[0]+w[1])){
        x_var[j]= x_curr;
    
        y_var[j]= y_curr-1;

    }

    if (x_var[j]<0){
        x_var[j]=0;
    }
    if(y_var[j]<0){
        y_var[j]=0;
    }
 
  
}
void gillespie_step_Brusselator(int j,double * times, double * x_var, double * y_var, double V){
    double x_curr= x_var[j-1];
    double  y_curr= y_var[j-1];
    double t_curr= times[j-1];
    double a = 2;
    double b= 5;
    double w[5];
    int i,l;

    w[0] = a*V;
    w[1] = x_curr;
    w[2] = 1/(V*V)* x_curr*(x_curr-1)*y_curr;
    w[3]=b*x_curr;
    w[4]=0.;

    for(i=0;i<4;i++){
        w[4]+=w[i];
    }

    
    
    double u = drand48()*w[4];
    double tau= -1/w[4]*log(drand48());
    times[j]= t_curr +tau;
   

    if (u<=w[0]){
        x_var[j]= x_curr +1;
    
        y_var[j]= y_curr;
    }
          
    if((u>w[0]) & (u<=(w[0]+w[1]))){
        x_var[j]= x_curr-1;
        y_var[j]= y_curr ;
    }
       
        
    if((u>(w[0]+w[1])) &(u<= (w[0]+w[1]+w[3]))){
        x_var[j]= x_curr+1;
        y_var[j]= y_curr-1;

    }
    if((u>(w[0]+w[1]+w[3]))){
        x_var[j]= x_curr-1;
        y_var[j]= y_curr+1;

    }


    if (x_var[j]<=0){
        x_var[j]=0;
    }
    if(y_var[j]<=0){
        y_var[j]=0;
    }
}





int main()
{

    fp= fopen( "exercise_8.csv","w");
    //write header to file
    fprintf(fp, "times, Lotka x, lotka y\n ");
    fclose(fp);
    
    
    srand48((unsigned)time(NULL));

    int N=300000;
    int i=0;
    double *times =(double*)malloc(N * sizeof(double));
    double  *x= (double *)malloc(N * sizeof(double ));
    double  *y = (double *)malloc(N * sizeof(double));
    double ks_LV[3]={3,0.01,5};


    times[0]= 0.;

    //*****simulation at Fixed point x_0=500, y_0=300*******
    x[0]=500.;
    y[0]=300.;
    for(i=1;i<N;i++)
    {
        gillespie_step_LV(i,times,x,y,ks_LV);
    }
   
    printf("\n");
    write_array("exercise_8.csv",times,N);
    write_array("exercise_8.csv",x,N);
    write_array("exercise_8.csv",y,N);


    //*****simulation further from Fixed point x_0=500, y_0=150*******
    x[0]=500.;
    y[0]=150.;
    for(i=1;i<N;i++)
    {
        gillespie_step_LV(i,times,x,y,ks_LV);
    }
   
    printf("\n");
    write_array("exercise_8.csv",times,N);
    write_array("exercise_8.csv",x,N);
    write_array("exercise_8.csv",y,N);





    //*****simulation very far from Fixed point x_0= 500, y_0=50*******
    x[0]=100.;
    y[0]=40;


    for(i=1;i<N;i++)
    {
        gillespie_step_LV(i,times,x,y,ks_LV);
    }
   
    printf("\n");

    write_array("exercise_8.csv",times,N);
    write_array("exercise_8.csv",x,N);
    write_array("exercise_8.csv",y,N);

 //changing the rates: 
    double ks_LV_2[3]={3,0.001,5};
    double ks_LV_3[3]= {3,0.01,3};


    for(i=1;i<N;i++)
    {
        gillespie_step_LV(i,times,x,y,ks_LV_2);
    }
   
    printf("\n");
    write_array("exercise_8.csv",times,N);
    write_array("exercise_8.csv",x,N);
    write_array("exercise_8.csv",y,N);

    for(i=1;i<N;i++)
    {
        gillespie_step_LV(i,times,x,y,ks_LV_3);
    }
   
    printf("\n");
    write_array("exercise_8.csv",times,N);
    write_array("exercise_8.csv",x,N);
    write_array("exercise_8.csv",y,N);

 //***************BRUSSELATOR*************
    x[0]= 100;
    y[0]=5000;

    times[0]=0.;
    for(i=1;i<N;i++)
    {
        gillespie_step_Brusselator(i,times,x,y,1000);
    }
   
    printf("\n");
    write_array("exercise_8.csv",times,N);
    write_array("exercise_8.csv",x,N);
    write_array("exercise_8.csv",y,N);



    free(times);
    free(x);
    free(y);
    return 0;

}

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include "exercise3_C.h"


FILE *fp;

char* FILENAME = "data3.csv";
int rejection_counter=0;


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

void sample_2d_gaussian(int N, double *arrayx, double *arrayy)
/* sample N points from  2D gaussian*/
{   
    int i =0;

    srand48((unsigned)time(NULL));
    for(i=0; i<N;i++)
    {   
        //use the result of the proper transform
        double radius = sqrt(-2*log(1-drand48()));
        double angle = 2* M_PI * drand48();
        //go to cartesian coordinates
        arrayx[i]= radius * cos(angle);
        arrayy[i] = radius * sin(angle);

    }  
}

void wrong_sample_disk_uniform( int N, double *arrayx, double *arrayy)
/* naive approach for sampling N random points uniformly from unit disk,*/
{   
    int i =0;

    srand48((unsigned)time(NULL));
    for(i=0; i<N;i++)
    {   
        //use the naive approach from teh exercise to get radius and angle within circle
        double radius = drand48();
        double angle = 2* M_PI * drand48();

        //go to cartesian coordinates
        arrayx[i]= radius * cos(angle);
        arrayy[i] = radius * sin(angle);

    }

   
}

void right_sample_disk_uniform( int N, double *arrayx, double *arrayy)
/* right approach for actually sampling N random points uniformly from unit disk,*/
{   
    int i =0;

    srand48((unsigned)time(NULL));
    for(i=0; i<N;i++)
    {   
        //use the result of the proper transform
        double radius = sqrt(drand48());
        double angle = 2* M_PI * drand48();
        //go to cartesian coordinates
        arrayx[i]= radius * cos(angle);
        arrayy[i] = radius * sin(angle);

    }  
}







int main()
{

  int i =0;

  int N = 1000;
  fp= fopen( FILENAME,"w");
  //write header to file
  fprintf(fp, "wrongly sampled diskx, wrongly sampled disky, properly sampled disk x, properly sampled disk y, rejection_gsampling, rejection_sampling\n ");

  fclose(fp);

  double arrayx[N];
  double arrayy[N];

  //sample from unit disk the wrong way and save the points
  wrong_sample_disk_uniform(N,arrayx,arrayy);
  write_array(FILENAME,arrayx, N);
  write_array(FILENAME,arrayy,N);
  //sample from unit dik the right way and save the points to the data file
  right_sample_disk_uniform(N, arrayx,arrayy);
  write_array(FILENAME,arrayx, N);
  write_array(FILENAME,arrayy,N);

 



  N= 100000;

  double p=0.7;
  
  double test_g_sampling[N];
  double c=1.7;
  
  double array_rejection[N];

  srand48((unsigned)time(NULL));

  for (i=0;i<N;i++)
  {
    

    test_g_sampling[i]=sample_x_from_g(p);
  }

  for(i=0;i<N;i++)
  {
    array_rejection[i]= rejection_sample_g(p,c);
  }
  write_array(FILENAME, test_g_sampling,N);
  write_array(FILENAME, array_rejection,N);

  //sample from 2D gaussian

  N=100000;
  double arrayx2[N];
  double arrayy2[N];

  sample_2d_gaussian(N, arrayx2,arrayy2);
  write_array(FILENAME,arrayx2, N);
  write_array(FILENAME,arrayy2,N);





  







}

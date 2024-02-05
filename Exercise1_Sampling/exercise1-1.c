#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include <time.h>
FILE *fp;
char* FILENAME = "data1.csv";


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
void write_array_int(char* filename,int* array ,int arraylength){
  /*same as write_array, just for int arrays*/
  fp = fopen(filename, "r+");
  fseek(fp,0, SEEK_END);

  int i;
  int N =arraylength;
  for(i=0;i<N;i++){
    fprintf(fp,"%d;", array[i]);

  }
  fseek(fp,-1, SEEK_END);
  fputs("\n",fp);
  fclose(fp);


}

void sample_uniform_rec( double *arrayx, double *arrayy, double a, double b, double c, double d, int N){
  /*  fills the two given arrays with N x and y coordinates sampled from [a,b]x[c,d]
  */
  int i =0;

  //seed the random number generato
  srand48((unsigned)time(NULL));

  for(i=0;i<N;i++){
    double x= drand48()*(b-a) + a;
    double y= drand48()*(d-c) +c;

    arrayx[i]= x;
    arrayy[i]= y; 
  
  }


}

double hit_and_miss(double *arrayx, double *arrayy, int N, double a, double b, double c, double d, char mode)
{ 
  /* use the hit and miss method for estimating the area of a given shape within the rectancle [a,b]x[c,d]
  around the origin (assumes the rectancle to be at least 1x1)

  parameters:
  arrayx, arrayy : float arrays that contain the randomly generated coordinates of the "throws" within the rectangle
  int N:  number of throws
  a,b,c,d: specify the size and position of the surrounding rectangle
  mode: if mode = "r" the area of the rectangle itself is estimated
        if mode ="o" the area of a unit disk around the origin within the rectangle
  */
 
  //counts the number of hits
  int scoresum=0;


  int i;
  double area;

  // rectangle mode: everything is a hit
  if (mode == 'r')
  {
    for (i=0;i<N;i++)
    {
      scoresum++;
    } 
  }
// circle mode: hit must fall within circle of radius 1
  if (mode=='o')
  {
    for (i=0;i<N;i++)
    {
      if (arrayx[i]*arrayx[i] + arrayy[i]*arrayy[i] <= 1 )
      {
        scoresum++;
      } 
    }
  }
  
  //calculate area
  double  prob = (double)scoresum/N;
  area = prob * (b-a)*(d-c);

  return (area);

}

int main( int argc, char **argv){

  //counter variable
  int i;
  //Number of points sampled
  int N=1000;




  //intervals [a,b] , [c,d] enlcosing the sampled rectangle
  double a=-2; double b=2; double c=-1.5; double d= 1.5;
  
  //create/ file for saving the data, and write a header line with what will follow
  fp = fopen(FILENAME, "w");
  fprintf(fp, " contents: example rectangle x-coordinates; example rectangle y coordinates; hit elements of unit disk x, hit elements of unit disk y; area estimated rectangle, area estimates circle, number of throws \n");

  fclose(fp);

  //initiallise the arrays that will hold the sample coordinates
  double arrayx[N];
  double arrayy[N];

  //sample the rectangle uniformly
  sample_uniform_rec(arrayx,arrayy,a,b,c,d,N);
  for(i=0;i<N; i++)
  {
  
  }

  //write data to file
  write_array(FILENAME, arrayx,N);
  write_array(FILENAME, arrayy,N);





  // estimating the area of the rectangle and the unit disk with hit and miss, testing the functions
  double area_rect = hit_and_miss(arrayx, arrayy,N,a,b,c,d,'r');
  double area_circle = hit_and_miss(arrayx, arrayy,N,a,b,c,d,'o');

  // print out results
  float true_area_rect= (b-a)*(d-c);
  printf("estimated rect area: %.4f", area_rect);
  printf("true rect area: %.4f", true_area_rect);
  printf("estimated area circle %.8f", area_circle);

  // for test of where the hits happen, save all hits to array 
  double hit_points_x[1000]={0,};
  double hit_points_y[1000]={0,};
  for (i=0;i<N;i++)
    {
      if (arrayx[i]*arrayx[i] + arrayy[i]*arrayy[i] <= 1 )
      {
        hit_points_x[-1] = arrayx[i];
        hit_points_y[-1] = arrayy[i];

      } 
    }
  write_array(FILENAME, hit_points_x,N);
  write_array(FILENAME, hit_points_y,N);

  //see how changing the number of throws affects the accuracy of the estimation
  int Number_of_throws[9] ={50, 100,500,1000, 5000, 10000, 50000,100000};



  double estimated_rectangles[9];
  double estimated_circles[9];

  for (i=0; i<9;i++)
  {
    double arrayx[Number_of_throws[i]];
    double arrayy[Number_of_throws[i]];
    sample_uniform_rec(arrayx,arrayy,a,b,c,d,Number_of_throws[i]);
    estimated_rectangles[i]=hit_and_miss(arrayx,arrayy,Number_of_throws[i],a,b,c,d,'r');
    estimated_circles[i]=hit_and_miss(arrayx,arrayy,Number_of_throws[i],a,b,c,d,'o');
  
  }

  // save the data
  write_array(FILENAME, estimated_rectangles, 9);
  write_array(FILENAME, estimated_circles,9);
  write_array_int(FILENAME, Number_of_throws, 9);
  printf("saved");

}






 


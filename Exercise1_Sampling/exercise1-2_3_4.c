#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include <time.h>

FILE *fp;
char* FILENAME = "data2.csv";


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


int main()
{
  fp= fopen( FILENAME,"w");
  fprintf(fp,"array for n=3, array for n=4, array for n=2\n");

  fclose(fp);

    int n3 = 3;
    int n4 = 4;
    int i;

    double my=3.;
    double c= 3;
    double b= 4;
    double n=3;
    double norm= pow(c,n-1)*b*(n-1);
    int N=100000;


    
    double a;
  
    double points3[N];
    double points4[N];
    double points5[N];
    double points4_1[N];
    double points4_2[N];
    double points4_3[N];
    //seed random number generator
    srand48((unsigned)time(NULL));
   
    for(i=0;i<N;i++)
    {
      //draw random number from uniform[0,1]
      a = drand48();
      //use the results of the inversion method to sample from the desired distrubtions instead
      points3[i]=pow(a,1./(double)(n3+1));
      points4[i]=pow(a,1./(double)(n4+1));
      points5[i]=pow(8*a,1/3.);

      points4_1[i]=-log(1-a)/my;
      points4_2[i]=sqrt(-log(1-a));

      points4_3[i]=c/b*(pow(1/(1-norm*a),1/(n-1))-1);
      //points4_3[i]=c/b*(pow(1/(1-a),1/(n-1))-1);
      //printf("%f ", points4_3[i]);

    }
  //save data to file
  write_array(FILENAME, points3,N);
  write_array(FILENAME, points4,N);
  write_array(FILENAME, points5,N);
  write_array(FILENAME, points4_1,N);
  write_array(FILENAME, points4_2,N);
  write_array(FILENAME, points4_3,N);
  printf("data saved \n");
  return 0;
}

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <time.h>

#include "array_alloc.h"
FILE *fp;
char* FILENAME = "test_data.csv";

int** initial_system(int L)
{
    int **M;
    int i=0;
    int j=0;
    M = (int **)imatrix(0,L-1,0,L-1);
    for(i=0;i<L;i++)
    {
        for(j=0;j<L;j++)
        {
            double u=drand48();
            if(u<0.5)
            {
                M[i][j]=-1;
            }
            else{
                M[i][j]=1;
            }

                
        }
    }
    return M;


}
void write_array(char* filename,double * array ,int arraylength)
{
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
    for(i=0;i<N;i++)
    {
        fprintf(fp,"%.6f;", array[i]);

    }
    //delete last ";" and replace with a linebreak
    fseek(fp,-1, SEEK_END);
    fputs("\n",fp);
    fclose(fp);


}



void write_2darray(char* filename,int ** array ,int dim1, int dim2)
{
      /*Function: write_2darray
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
    int j;
    int N =dim1;
    int M =dim2;
    for(i=0;i<N;i++)
    {   
        for(j=0;j<M;j++)
        {   
        fprintf(fp,"%d;", array[i][j]);

        }
        

    }
    //delete last ";" and replace with a linebreak
    fseek(fp,-1, SEEK_END);
    fputs("\n",fp);
    fclose(fp);
}



int main()
{
    fp= fopen( FILENAME,"w");
    //write header to file
    fprintf(fp, "Magnetisations\n ");
    fclose(fp);
    srand48((unsigned)time(NULL));

    int** M_small;
    int L = 25;
    M_small= initial_system(L);
    write_2darray(FILENAME,M_small,L,L);

    free_imatrix(M_small);

   
}
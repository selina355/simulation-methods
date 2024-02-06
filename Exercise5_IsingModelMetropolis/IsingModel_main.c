#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <time.h>

#include "array_alloc.h"

int L;
int i ;
int j;
int k;

double J= 1;
const double kb= 1.;



FILE *fp;
char* FILENAME = "Ising_data.csv";


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


int** initial_system(int L){
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

double sum_neighbours( int**M, int r1,int r2, int L){
    double sum=0;
    int u=r1-1; //1st index of upper neighbor
    int d=r1+1; //1st index of down neighbor
    int l=r2-1; //2nd index of left neighbor
    int r=r2+1; //2nd index of right neighbor
    if (u==-1){
        u=L-1;
    }
    if (d==L){
        d=0;
    }
    if (l==-1){
        l=L-1;
    }
    if(r==L){
        r=0;
    }
    sum=M[u][r2]+M[d][r2]+M[r1][l]+M[r1][r];
    return sum;
}

void monte_carlo_step(int L, int**M, double T)
{
    int i=0;
    double beta= 1/(kb*T);

    for (i=0;i< L*L; i++)
    {
        //pick spin M[r1][r2] for flipping
        int r1= lrand48()%(L);
        int r2= lrand48()%(L);
        //printf("\n randomly picked indexes: %d, %d \n",r1,r2);

        double energydiff = 2*J* M[r1][r2]*sum_neighbours(M,r1,r2,L);
    
        if (energydiff<0)
        {
            M[r1][r2]=-M[r1][r2];
            //printf("flipped!\n");

        }
        else
        {
            double u =drand48();
            
            if (u < exp(-beta*energydiff))
            {
                M[r1][r2]=-M[r1][r2];
                //printf("flipped!\n");
            }
        }

    }
   
    
}

double calculate_magnetisation_per_spin(int**M, int L){
    double sum_over_all_spins=0;
    int j=0;
    int k=0;
    for(j=0;j<L;j++)
    {
        for(k=0;k<L;k++)
        {
            sum_over_all_spins+= M[j][k];
        }
    }
    return (sum_over_all_spins/(L*L));   
}

double calculate_energy_per_spin(int**M, int L)
{
    int j=0;
    int k=0;
    double sum_over_all_spins=0;
    for(j=0;j<L;j++)
    {
        for(k=0;k<L;k++)
        {
            sum_over_all_spins-=M[j][k]*sum_neighbours(M,j,k,L);
        }
    }
    return (sum_over_all_spins/(L*L)); 
}


int main()
{   
    fp= fopen( FILENAME,"w");
    //write header to file
    fprintf(fp, "Magnetisations\n ");
    fclose(fp);
    fp=fopen("Ising_models.csv", "w");
    fclose(fp);
    srand48((unsigned)time(NULL));


//************SIMULATION FOR L = 25 and the 3 Different Temperatures*********************

    int** M_small;
    L = 25;
    M_small= initial_system(L);
   

    double T1=1.;
    double Tc=  2/log(1+sqrt(2));
    double T3 = 3.;
    int N=30000;
    double Magnetisations[N];
    double Energies[N];
    N=10000;

    for(i=0;i<N;i++)
    {
        monte_carlo_step(L,M_small,T1);
        Magnetisations[i]=calculate_magnetisation_per_spin(M_small,L);   
        Energies[i]=calculate_energy_per_spin(M_small,L);
    }
    write_array(FILENAME,Magnetisations, N);
    write_array(FILENAME,Energies, N);
    write_2darray("Ising_models.csv",M_small,L,L);

    M_small= initial_system(L);
    for(i=0;i<N;i++)
    {
        monte_carlo_step(L,M_small,Tc);
        Magnetisations[i]=calculate_magnetisation_per_spin(M_small,L);   
        Energies[i]=calculate_energy_per_spin(M_small,L);
    }
    write_array(FILENAME,Magnetisations, N);
    write_array(FILENAME,Energies, N);
    write_2darray("Ising_models.csv",M_small,L,L);

    M_small= initial_system(L);
    for(i=0;i<N;i++)
    {
        monte_carlo_step(L,M_small,T3);
        Magnetisations[i]=calculate_magnetisation_per_spin(M_small,L);   
        Energies[i]=calculate_energy_per_spin(M_small,L);
    }

    write_array(FILENAME,Magnetisations, N);
    write_array(FILENAME,Energies, N);
    write_2darray("Ising_models.csv",M_small,L,L);

    free_imatrix(M_small);
     printf("DOne small system");


//************SIMULATION FOR L = 50 and the 3 Different Temperatures*********************
    int** M_medium;
    L = 50;
    M_medium = initial_system(L);
    N=30000;
    for(i=0;i<N;i++)
    {
        monte_carlo_step(L,M_medium,T1);
        Magnetisations[i]=calculate_magnetisation_per_spin(M_medium,L);   
        Energies[i]=calculate_energy_per_spin(M_medium,L);
    }
    write_array(FILENAME,Magnetisations, N);
    write_array(FILENAME,Energies, N);
    write_2darray("Ising_models.csv",M_medium,L,L);

    M_medium = initial_system(L);
    for(i=0;i<N;i++)
    {
        monte_carlo_step(L,M_medium,Tc);
        Magnetisations[i]=calculate_magnetisation_per_spin(M_medium,L);   
        Energies[i]=calculate_energy_per_spin(M_medium,L);
    }
    write_array(FILENAME,Magnetisations, N);
    write_array(FILENAME,Energies, N);
    write_2darray("Ising_models.csv",M_medium,L,L);

    M_medium = initial_system(L);
    for(i=0;i<N;i++)
    {
        monte_carlo_step(L,M_medium,T3);
        Magnetisations[i]=calculate_magnetisation_per_spin(M_medium,L);   
        Energies[i]=calculate_energy_per_spin(M_medium,L);
    }
    write_array(FILENAME,Magnetisations, N);
    write_array(FILENAME,Energies, N);
    write_2darray("Ising_models.csv",M_medium,L,L);

    free_imatrix(M_medium);
    
    printf("DOne medium system");
//************SIMULATION FOR L = 100 and the 3 Different Temperatures*********************

    L = 100;
    M_medium = initial_system(L);
    N=30000;
    for(i=0;i<N;i++)
    {
        monte_carlo_step(L,M_medium,T1);
        Magnetisations[i]=calculate_magnetisation_per_spin(M_medium,L);   
        Energies[i]=calculate_energy_per_spin(M_medium,L);
    }
    write_array(FILENAME,Magnetisations, N);
    write_array(FILENAME,Energies, N);
    write_2darray("Ising_models.csv",M_medium,L,L);

    M_medium = initial_system(L);
    for(i=0;i<N;i++)
    {
        monte_carlo_step(L,M_medium,Tc);
        Magnetisations[i]=calculate_magnetisation_per_spin(M_medium,L);   
        Energies[i]=calculate_energy_per_spin(M_medium,L);
    }
    write_array(FILENAME,Magnetisations, N);
    write_array(FILENAME,Energies, N);
    write_2darray("Ising_models.csv",M_medium,L,L);

    M_medium = initial_system(L);
    for(i=0;i<N;i++)
    {
        monte_carlo_step(L,M_medium,T3);
        Magnetisations[i]=calculate_magnetisation_per_spin(M_medium,L);   
        Energies[i]=calculate_energy_per_spin(M_medium,L);
    }
    write_array(FILENAME,Magnetisations, N);
    write_array(FILENAME,Energies, N);
    write_2darray("Ising_models.csv",M_medium,L,L);

    free_imatrix(M_medium);
    printf("DOne big system");

    
}
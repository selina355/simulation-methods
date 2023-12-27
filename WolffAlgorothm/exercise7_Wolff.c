#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include "IsingModel.h"
#include "array_alloc.h"

int L;
int i ;
int j;
int k;

double J= 1;
const double kb= 1.;
int clustersize;


FILE *fp;
char* FILENAME = "Exercise7_data.csv";


int* flip_neighbours(int L, int**M, double P_add, int r1,int r2, int center_state){

    M[r1][r2]=-M[r1][r2];
    clustersize+=1;
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

    //attempt flip of upper neighbor and go to its neighbours
    if( (M[u][r2])==center_state && (P_add <=drand48())){
        //M[u][r2] = -M[u][r2];
        flip_neighbours(L,M,P_add,u,r2,center_state);
    }

    if( (M[d][r2]==center_state) && (P_add <=drand48())){
        //M[d][r2] = -M[d][r2];
        flip_neighbours(L,M,P_add,d,r2,center_state);
    }
    if( (M[r1][l]==center_state) && (P_add <=drand48())){
        //M[r1][l] = -M[r1][l];
        flip_neighbours(L,M,P_add,r1,l,center_state);
    }
    if( (M[r1][r]==center_state) && (P_add <=drand48())){
        //M[r1][r] = -M[r1][r];
        flip_neighbours(L,M,P_add,r1,r,center_state);
    }

}


int wolff_monte_carlo_step(int L, int**M, double T){
    int i=0;
    double r = drand48();
    double beta= 1/(kb*T);
    double P_add= 1- exp(-2*beta*J);
    clustersize=0;

    //pick a random spin to start
    int r1= lrand48()%(L);
    int r2= lrand48()%(L);

    //save the initial start spin of the cluster center
    int center_state = M[r1][r2];
    flip_neighbours( L, M ,P_add,r1,r2,center_state);
    return clustersize;

}


int main()
{   
    fp= fopen( "exercise7_sim_animation.csv","w");
    //write header to file
    fprintf(fp, "frames of simulation\n ");
    fclose(fp);
    fp= fopen( "exercise7_other.csv","w");

    srand48((unsigned)time(NULL));

    L= 50;
    int** M= initial_system(L);

    int N= 10;
    double T=1.;
    double clustersizes[N];
    for (i=0;i<N;i++){
        clustersizes[i]=wolff_monte_carlo_step(L,M,T);
        write_2darray("exercise7_sim_animation.csv",M,L,L);
    }
    write_array("exercise7_other.csv", clustersizes,L);
    free_imatrix(M);




}
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

double J= 1.;
const double kb= 1.;
int clustersize;
double Tc=2/log(1+sqrt(2));


FILE *fp;
char* FILENAME = "Exercise7_data.csv";


void flip_neighbours(int L, int**M, double P_add, int r1,int r2, int center_state){
    
    //flip the spin
    M[r1][r2]=-M[r1][r2];
    clustersize+=1;

    //get indices of neighbours
    int u=r1-1; //1st index of upper neighbor
    int d=r1+1; //1st index of down neighbor
    int l=r2-1; //2nd index of left neighbor
    int r=r2+1; //2nd index of right neighbor
    //ensure periodic boundary conditions
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

    // if upper neighbor agrees with the original cluster center spin 
    // attempt to join it to the cluster (and then flip it and go to its neighbours)
    if( (M[u][r2])==center_state && (P_add >=drand48())){
        
        flip_neighbours(L,M,P_add,u,r2,center_state);
    }


    //same for all other neighbours that have the right spin
    if( (M[d][r2]==center_state) && (P_add >=drand48())){
        //M[d][r2] = -M[d][r2];
        flip_neighbours(L,M,P_add,d,r2,center_state);
    }
    if( (M[r1][l]==center_state) && (P_add >=drand48())){
        //M[r1][l] = -M[r1][l];
        flip_neighbours(L,M,P_add,r1,l,center_state);
    }
    if( (M[r1][r]==center_state) && (P_add >=drand48())){
        //M[r1][r] = -M[r1][r];
        flip_neighbours(L,M,P_add,r1,r,center_state);
    }

}


int wolff_monte_carlo_step(int L, int**M, double T){
    int i=0;

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

int try_swapp_MMC(double E1,double E2,double T1, double T2){

    double metropolis_factor= exp( (1/T2-1/T1)*(E2-E1));
    if (metropolis_factor>=1){
        return 1;
    }
    else{
        if (drand48()<=metropolis_factor){
            return 1;
        }
        return 0;
    }

    return 0;

}
void swap_states(int***M_a,int***M_b){
    int** temp= *M_a;
    int** temp1= *M_b;
    **M_a= **M_b;
    *M_b=temp;
    //M[b]=*temp0;
    
}


void swap_order(int * M, int a, int b){
    int temp = M[a];
    M[a]= M[b];
    M[b]=temp;
}


int main()
{   

    srand48((unsigned)time(NULL));

    L= 50;

    int N= 40000;
    double Tl=0.5*Tc;
    double Th= 2*Tc;
    
    fp= fopen( "exercise7_sim_animation.csv","w");
    //write header to file
    fprintf(fp, "frames of simulation\n ");
    fclose(fp);
    fp= fopen( "exercise7_other.csv","w");
    fclose(fp);
    int**M= (int **)imatrix(0,L-1,0,L-1);
    M= initial_system(L,M);
    

    double * clustersizes;
    clustersizes=(double*)dvector(0,N-1);
    
    for (i=0;i<N;i++){
        clustersizes[i]=wolff_monte_carlo_step(L,M,Tl);

        if (i%100==0){
            write_2darray("exercise7_sim_animation.csv",M,L,L);
        }
        
    }
    
    write_array("exercise7_other.csv", clustersizes,N);
   

    M= initial_system(L,M);
    for (i=0;i<N;i++)
    {
        clustersizes[i]=wolff_monte_carlo_step(L,M,Tc);

    }
    write_array("exercise7_other.csv", clustersizes,N);
  

    M= initial_system(L,M);
    for (i=0;i<N;i++)
    {
        clustersizes[i]=wolff_monte_carlo_step(L,M,Th);

    }
    write_array("exercise7_other.csv", clustersizes,N);
    free_dvector(clustersizes,0,N-1);
    free_imatrix(M);
    
    int Ls[7] ={50,65,80,95,110,135};

    N=10000;

    double * clustersizes_corr;
    double * Magnetisations=(double*)dvector(0,N-1);
    double * Magnetisations_Metropolis=(double*)dvector(0,N-1);
    int**Ms;
    int**Mm;

    for( i =0;i<6;i++){
        clustersizes_corr= (double*)dvector(0,N-1);
        Ms=(int **)imatrix(0,Ls[i]-1,0,Ls[i]-1);
        Ms=initial_system(Ls[i],Ms);
        for (j=0;j<N;j++)
        {
        clustersizes_corr[j]=wolff_monte_carlo_step(Ls[i],Ms,Tc);
        Magnetisations[j]= calculate_magnetisation_per_spin(Ms,Ls[i]);

        }
        write_array("exercise7_other.csv",clustersizes_corr,N-1);
        printf("wrote cluster size");
        write_array("exercise7_other.csv",Magnetisations,N);
        printf("wrote magnetisation");
        free_dvector(clustersizes_corr,0,N-1);
        free_imatrix(Ms);

    }
    for( i =0;i<6;i++){
        Mm=(int **)imatrix(0,Ls[i]-1,0,Ls[i]-1);
        Mm=initial_system(Ls[i],Mm);
        for (j=0;j<N;j++)
        {
        monte_carlo_step(Ls[i],Mm,Tc);
        Magnetisations_Metropolis[j]= calculate_magnetisation_per_spin(Mm,Ls[i]);

        }
        write_array("exercise7_other.csv",Magnetisations_Metropolis,N);
        printf("wrote metropolis");
        free_imatrix(Mm);

    }
    
    
    free_dvector(Magnetisations,0,N-1);
    free_dvector(Magnetisations_Metropolis,0,N-1);

    /*
    //***********************Multilple Monte carlo simulation*********************************
    double  temperatures[9]= {1.2, 1.4, 1.7,2.0, 2.3,2.6,2.9,3.2,3.5};
    int system_order[9] = {0,1,2,3,4,5,6,7,8};
    N=10;
    L=10;
    int***all_systems= imatrix3(9,L,L);
    double** energies_all= dmatrix(0,8,0,N-1);

    int swap_counter=0;
    int swap_attempts=0;
  
    for (j=0;j<9;j++){
        all_systems[j]=initial_system(L,all_systems[j]);
    }
    printf("before swap 0\n");
    for(i=0;i<L;i++){
        for (j=0;j<L;j++){
            printf("%d ",all_systems[0][i][j]);
        
        }
        printf("\n");
    }
    printf("before swap 1\n");

    for(i=0;i<L;i++){
        for (j=0;j<L;j++){
            printf("%d ",all_systems[1][i][j]);
        
        }
        printf("\n");
    }
    swap_states(&all_systems[0],&all_systems[1]);
    
    printf("after swap 0\n");

    for(i=0;i<L;i++){
        for (j=0;j<L;j++){
            printf("%d ",all_systems[0][i][j]);
        
        }
        printf("\n");
    }
    printf("after swap 1\n");

    for(i=0;i<L;i++){
        for (j=0;j<L;j++){
            printf("%d ",all_systems[1][i][j]);
        
        }
        printf("\n");
    }
    /*
    //int *** initial_systems= all_systems;
    int e;
    int f;
    for (i=0;i<N;i++){
        for (j=0;j<9;j++){
            monte_carlo_step(L,all_systems[j],temperatures[j]);
            energies_all[system_order[j]][i]=calculate_energy_per_spin(all_systems[j],L); 
            //printf(" %f ", energies_all[j][i]) ; 
        }
        //printf("\n");
        
        for(j=0;j<7;j++){
            swap_attempts+=1;
            if(try_swapp_MMC(energies_all[j][i],energies_all[j+1][i], temperatures[j], temperatures[j+1]) ==1){
                //swap_order(system_order,j,j+1);
                //swap_states(all_systems, j, j+1);
                //swap_counter+=1;
            }
        }
        
    }
    
    for(e=0;e<L;e++){
        for (f=0;f<L;f++){
            printf("%d ",all_systems[0][e][f]);
    
        }
        printf("\n");
    }
    

    fp= fopen( "exercise7_multipleMC.csv","w");
    //write header to file
    fprintf(fp, "Energies of chains at  T= {1.2, 1.4, 1.7,2.0, 2.3,2.6,2.9,3.2,3.5};\n ");
    fclose(fp);
    for (j=0;j<9;j++){
        write_array("exercise7_multipleMC.csv", energies_all[j],N);
    }
    for(j=0;j<7;j++){
            swap_attempts+=1;
            
            //swap_order(system_order,j+1,j);
            //swap_states(all_systems, j+1, j);
            swap_counter+=1;

        }
    //swap_states(all_systems,8,1);
    
    
    for (j=0;j<9;j++){
        write_array("exercise7_multipleMC.csv", energies_all[j],N);
    }
    
    */

    printf("it worked!");

    //free_dmatrix(energies_all);

    //free_imatrix3(all_systems);





    

    

        
    


}
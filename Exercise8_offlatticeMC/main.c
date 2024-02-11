#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "random.h"
#include "definitions.h"
#include "utils.h"
#include "init.h"
#include "energy.h"
#include "MC.h"

int main(int argc, char* argv[] ) //start with command line options. char*argv[] means this is an array of indetermined size of pinters to char, in other words an array of strings.argv is a pointer to string array holding the actual parameters.
{
    FILE * f,* g ,*h, *p;

    int dmax_i,box_i,runs_i;
    double dmax[6]= {0.01,0.3,0.5,0.7,1.};
    double boxsize[4] = {12.6, 6.93, 5.85, 4.64};
    double seeds[10]={1., 2.,3.,4.,5.,6.,7.,8.,9.};
    char *energyfile;
    char *acceptancefile;
    char *distancesfile;
    char *pressurefile;

    read_input_file();

    allocate_(); 

//**************************************************************
 //Exercise 1: non interacting point particles
/*
    energyfile="energy_non_interacting.dat";
    acceptancefile= "acceptance_non_interacting.dat";
    distancesfile = "distances_non_interacting.dat";
    mySys.model=0;
    
    f = fopen(energyfile,"w");
    g = fopen(acceptancefile,"w");
    h = fopen(distancesfile,"w");
    fclose(f);fclose(g);fclose(h);
    initialise_random();
    do_MC(energyfile,acceptancefile,distancesfile);
    mySys.seed=6.;
    initialise_random();
    do_MC(energyfile,acceptancefile,distancesfile);
    mySys.seed=1.3;
    initialise_random();
    do_MC(energyfile,acceptancefile,distancesfile);

    printf("finished non interacting sim");
*/
     

 //**************************************************************
 //Exercise 2: hard spheres : random initialisation
    /*
    mySys.model= 1;

    energyfile="energy_hard_spheres.dat";
    acceptancefile= "acceptance_hard_spheres.dat";
    distancesfile = "distances_hard_spheres.dat";
    f = fopen("energy_hard_spheres.dat","w");
    g = fopen("acceptance_hard_spheres.dat","w");
    h = fopen(distancesfile, "w");
    fclose(f);fclose(g);fclose(h);

    for (box_i=0; box_i<4;box_i++)
    {
        mySys.box_x=boxsize[box_i];
        mySys.box_y=boxsize[box_i];
        mySys.box_z=boxsize[box_i];

        for(dmax_i=0;dmax_i<5;dmax_i++)
        {
            printf("system with dmax %.2f and boxsize %.2f", dmax[dmax_i],boxsize[box_i]);
            mySys.disp_max= dmax[dmax_i];

            for	(runs_i=0; runs_i<10;runs_i++) 
            {
                mySys.seed= seeds[runs_i];
                initialise_random();
                do_MC(energyfile,acceptancefile,distancesfile);
                printf("%d",runs_i);

                
            }
            printf("\n");     
        }

    }
    
    
    */
   
    mySys.model= 1;
    energyfile="energy_hard_spheres_lattice.dat";
    acceptancefile= "acceptance_hard_spheres_lattice.dat";
    distancesfile="distances_hard_spheres_lattice.dat";
    f = fopen(energyfile,"w");
    g = fopen(acceptancefile,"w");
    h=fopen(distancesfile,"w");
    fclose(f);fclose(g);fclose(h);

    for (box_i=0; box_i<4;box_i++)
    {
        mySys.box_x=boxsize[box_i];
        mySys.box_y=boxsize[box_i];
        mySys.box_z=boxsize[box_i];
        
        for(dmax_i=0;dmax_i<5;dmax_i++)
        {
            printf("system with dmax %.2f and boxsize %.2f", dmax[dmax_i],boxsize[box_i]);
            mySys.disp_max= dmax[dmax_i];
            

            for	(runs_i=0; runs_i<10;runs_i++) 
            {
                mySys.seed= seeds[runs_i];
                initialise_lattice();
                printf("%d",runs_i);
                do_MC(energyfile, acceptancefile, distancesfile);
                

                
            }
            printf("\n");     
        }

    }
    /*
    mySys.model =2;
    mySys.T=0.9;
    mySys.disp_max=1.;
    energyfile="energy_LJlow.dat";
    acceptancefile= "acceptance_LJlow.dat";
    distancesfile="distances_LJlow.dat";
    pressurefile="pressure_low.dat";

    f = fopen(energyfile,"w");
    g = fopen(acceptancefile,"w");
    h=fopen(distancesfile,"w");
    p=fopen(pressurefile,"w");
    fclose(f);fclose(g);fclose(h);fclose(p);
    double boxsizes[8] = {12.6, 9.6, 7.6, 6.6, 6,5.6, 5.3,5.};

    for (box_i=0; box_i<8;box_i++)
    {
        mySys.box_x=boxsizes[box_i];
        mySys.box_y=boxsizes[box_i];
        mySys.box_z=boxsizes[box_i];

        printf("system with boxsize %.2f",boxsizes[box_i]);
        
        
        initialise_random();
        do_MC_Lennard_Jones(energyfile,acceptancefile,distancesfile,pressurefile);
                     
        
             
    }
    mySys.T=2.;
    mySys.disp_max=1.;
    energyfile="energy_LJhigh.dat";
    acceptancefile= "acceptance_LJhigh.dat";
    distancesfile="distances_LJhigh.dat";
    pressurefile="pressure_high.dat";

    f = fopen(energyfile,"w");
    g = fopen(acceptancefile,"w");
    h=fopen(distancesfile,"w");
    p=fopen(pressurefile,"w");
    fclose(f);fclose(g);fclose(h);fclose(p);
    double boxsizes2[8] = {12.6, 9.6, 7.6, 6.6, 6,5.6, 5.3,5.};

    for (box_i=0; box_i<8;box_i++)
    {
        mySys.box_x=boxsizes[box_i];
        mySys.box_y=boxsizes[box_i];
        mySys.box_z=boxsizes[box_i];

        printf("system with boxsize %.2f",boxsizes[box_i]);
        
        
        initialise_random();
        do_MC_Lennard_Jones(energyfile,acceptancefile,distancesfile,pressurefile);
                     
        
             
    }
    */

    
    

    
    

    
 
    
    // Release memory used by particles, it is not needed anymoeew
    clean_();

    return 0;
}
// gcc -o ../main main.c -lm


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
    FILE * f,* g;

    int dmax_i,box_i,runs_i;
    double dmax[6]= {0.01,0.05,0.01,0.3,0.7,1.};
    double boxsize[4] = {12.6, 6.93, 5.85, 4.64};
    double seeds[10]={1., 2.,3.,4.,5.,6.,7.,8.,9.};

    read_input_file();
    allocate_();    
 //**************************************************************
 //Exercise 1: hard spheres  
    mySys.model= 1;

    f = fopen("energy_hard_spheres.dat","w");
    g = fopen("acceptance_hard_sheres.dat","w");
    fclose(f);fclose(g);

    for (box_i=0; box_i<4;box_i++)
    {
        mySys.box_x=boxsize[box_i];
        mySys.box_y=boxsize[box_i];
        mySys.box_z=boxsize[box_i];

        for(dmax_i=0;dmax_i<6;dmax_i++)
        {
            mySys.disp_max= dmax[dmax_i];

            for	(runs_i=0; runs_i<10;runs_i++) 
            {
                mySys.seed= seeds[runs_i];
                initialise_random();
                do_MC();
                

            }
            printf("system with dmax %.2f and boxsize %.2f", dmax[dmax_i],boxsize[box_i]);
        }

    }
    
     
    
    // Release memory used by particles, it is not needed anymor
    clean_();

    return 0;
}
// gcc -o ../main main.c -lm


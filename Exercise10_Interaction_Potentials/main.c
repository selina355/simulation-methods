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
    

    //int dmax_i,box_i,runs_i;
    int i;
    double energy;
    read_input_file();
    allocate_();   
    mySys.cut_off=4.;
    mySys.max_step=1.;
    printf("%f\n", mySys.cut_off);
    /*
    initialise_random_start();
    do_MC();
    WriteConf("configuration.dat");
    */
    ReadConf("configuration.dat");
    draw_velocities();
    for (i=0;i<mySys.NPart;i++){
        calculate_accelerations(i);
    }
    printf("total kinetic energy: %f\n", kinetic_energy());
    printf("total energy %f\n ", compute_energy());
    printf("total momentum: %f \n",total_momentum());


    for( i=0;i<500;i++)
    {
        
        verlet_step(0.01);
        energy= compute_energy();
        printf("energy: %f \n",energy);
            printf("total kinetic energy: %f\n", kinetic_energy());
    printf("total energy %f\n ", compute_energy());
    printf("total momentum: %f \n",total_momentum());
         
        
    }
    for( i=0;i<200;i++){
        printf( "old %lf %lf %lf %lf %lf %lf \n", parts[i].x, parts[i].y, parts[i].z,parts[i].vx, parts[i].vy, parts[i].vz); 

    }
    



    

    
 
    // Release memory used by particles, it is not needed anymore
    clean_();

    return 0;
}
// gcc -o ../main main.c -lm


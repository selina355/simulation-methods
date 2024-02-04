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

void equalibrate_inital_pos( double rc,int i)
{
    
    char filename[100];
    read_input_file();
    allocate_();
    mySys.cut_off=rc;
    printf(" CUTOFFFF%f\n", mySys.cut_off);


    initialise_random_start();
    do_MC();
    printf("hi");
    sprintf(filename,"configuration%d.dat",i);
    WriteConf(filename);
    printf("equalibrated system with %f to file  %s\n",rc , filename);
    clean_();

    


}

int main(int argc, char* argv[] ) //start with command line options. char*argv[] means this is an array of indetermined size of pinters to char, in other words an array of strings.argv is a pointer to string array holding the actual parameters.
{
    

   
    int i;
    double energy;
    read_input_file();
    allocate_();   
    mySys.cut_off=4.;
    mySys.max_step=1.;

    //while()
    //equalibrate_inital_pos(4.,0);
    double rcs[16]={pow(2.,1./6.),1.2,1.4,1.6,1.8,2.0,2.2,2.4,2.6,2.8,3.,3.2,3.4,3.6,3.8,4.};
    for (i=0;i<16;i++){

        equalibrate_inital_pos(rcs[i],i);
    }
    
    
    /*
    ReadConf("configuration0.dat");
    draw_velocities();
    for (i=0;i<mySys.NPart;i++){
        calculate_accelerations(i);
    }
    printf("total kinetic energy: %f\n", kinetic_energy());
    printf("total energy %f\n ", compute_energy());
    printf("total momentum: %f \n",total_momentum());


    for( i=0;i<1000;i++)
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
*/  
}
// gcc -o ../main main.c -lm


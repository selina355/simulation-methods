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
    

   
    int i,j,k,l;
    double energy;
    read_input_file();
    allocate_();   
    mySys.cut_off=4.;
    mySys.max_step=1.;
    
    char init_configuration_filename[100];
    char distances_filename[100];
    FILE* f;
    //while()
    //equalibrate_inital_pos(4.,0);
    double rcs[16]={pow(2.,1./6.),1.2,1.4,1.6,1.8,2.0,2.2,2.4,2.6,2.8,3.,3.2,3.4,3.6,3.8,4.};
    /*
    for (i=0;i<1;i++){
        sprintf(init_configuration_filename,"configuration%d.dat",i);
        sprintf(distances_filename,"distances%d.dat",i);
        f= fopen(distances_filename,"w");
        //equalibrate_inital_pos(rcs[i],i);
        ReadConf(init_configuration_filename);
        mySys.cut_off=rcs[i];
        for( j=0;j<1000;j++)
        {
        verlet_step(0.01);
        if(j>500)
        {
            for (k=0;k<mySys.NPart;k++){
                for (l=0;l<k;l++){
                    fprintf(f,"%f ",r_squared_two_particles(k,l));

                }
            }
            fprintf(f,"\n");

        }

        
        energy= compute_energy();
        printf("energy: %f \n",energy);
        printf("total kinetic energy: %f\n", kinetic_energy());
        printf("total energy %f\n ", compute_energy());
        printf("total momentum: %f \n",total_momentum());
        fclose(f);
        
    }


    }
    
    
    */
   
    for (j=0;j<15;j++)
    {
    
        sprintf(init_configuration_filename,"configuration%d.dat",j);
        printf(init_configuration_filename);
        ReadConf(init_configuration_filename);
        draw_velocities();
        mySys.cut_off=rcs[j];
        
        sprintf(distances_filename,"distances%d.dat",j);
        f= fopen(distances_filename,"w");

        
        printf("total kinetic energy: %f\n", kinetic_energy());
        printf("total energy %f\n ", compute_energy());
        printf("total momentum: %f \n",total_momentum());


        for( i=0;i<1000;i++)
        {
            verlet_step(0.01);
            if(i>900)
            {
                for (k=0;k<mySys.NPart;k++){
                    for (l=0;l<k;l++){
                        fprintf(f,"%f ",r_squared_two_particles(k,l));

                    }
                }
                fprintf(f,"\n");
                

            }
            /*
            energy= compute_energy();
            printf("energy: %f \n",energy);
            printf("total kinetic energy: %f\n", kinetic_energy());
            printf("total energy %f\n ", compute_energy());
            printf("total momentum: %f \n",total_momentum());
            */
            
        }   
        fclose(f);

    }
    

    // Release memory used by particles, it is not needed anymore
    clean_();

    return 0;

}
// gcc -o ../main main.c -lm


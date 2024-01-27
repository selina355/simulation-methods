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

    read_input_file();
    allocate_();   
    mySys.cut_off=4.;
    mySys.max_step=1.;
    printf("%f\n", mySys.cut_off);
 
    initialise_random_start();
    do_MC();
    

    
 
    // Release memory used by particles, it is not needed anymoeew
    clean_();

    return 0;
}
// gcc -o ../main main.c -lm


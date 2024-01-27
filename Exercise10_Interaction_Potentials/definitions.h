#define N_MOVES 1
#define PI 3.141592653589793

typedef struct
{
    double x, y, z;

    double ox,oy,oz;	//old colloid coordinates for initalisation
    double vx,vy,vz;	//velocities
    
    double ax, ay, az;  //accelerations

} Particle;

Particle *parts = NULL; //It is always a good practice to assign a NULL value to a pointer variable in case you do not have an exact address to be assigned.

//defining the System of patchy IPCs properties

typedef struct  
{
    int NPart;
    long int step;
    long int NSteps, NPrint, Nrestart;      //number of IPCs, number of MC sweeps, frequency of configurations
    int restart;
    long seed;

    int overlap;
    //long int tries;		//array with trial number for different moves
    long int accepted;	//acceptance for different moves

    double box_x, box_y, box_z;
    double T;	
    double energy;
    double cut_off;
    double max_step;
    


} System;

System mySys;


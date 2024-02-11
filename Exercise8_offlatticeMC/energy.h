
#define min(a,b)    ({ __typeof__ (a) _a = (a);  __typeof__ (b) _b = (b); _a < _b ? _a : _b; })

double distance_two_particles(long int i, long int j){
    double distx,disty,distz;
    double distance;
    distx= MinD((parts[i].x -parts[j].x), mySys.box_x);
    disty= MinD((parts[i].y -parts[j].y), mySys.box_y);
    distz= MinD((parts[i].z -parts[j].z), mySys.box_z);

    distance= sqrt(distx*distx +disty*disty +distz*distz);
    return distance;

}
double distance_squared_two_particles(long int i, long int j){
    double distx,disty,distz;
    double distance;
    distx= MinD((parts[i].x -parts[j].x), mySys.box_x);
    disty= MinD((parts[i].y -parts[j].y), mySys.box_y);
    distz= MinD((parts[i].z -parts[j].z), mySys.box_z);

    distance= (distx*distx +disty*disty +distz*distz);
    return distance;

}

int get_overlaps(){

    int overlap=0;
    double distance;
    long int i, j;
    for(i = 0; i< mySys.NPart; i++)
    {
        for (j=0;j<i;j++)
        {
            //if(i==j){continue;}
            distance = distance_squared_two_particles(i,j);
            if (distance < mySys.sigma*mySys.sigma)
            {   
                overlap +=1;

            }
        }
    }
    return overlap;

}

double compute_energy_hard_spheres(){
    
    
    return 100000*get_overlaps();
    
    
    
}

double compute_energy_Lennard_Jones(){
   
    double en=0;
    double cut_off = mySys.box_x/2;
    double density= mySys.NPart/pow(mySys.box_x,3);
    double ratio_cut=mySys.sigma/cut_off;

    double r_squared;
    int i,j;

    for(i=0; i<mySys.NPart; i++)
    {
        for(j=0;j<i;j++)
        {
            r_squared=distance_squared_two_particles(i,j);
            //printf("distance: %f cutoff %f\n",r_squared, (mySys.cut_off*mySys.cut_off));
            if (r_squared < cut_off*cut_off)
            {
                en+= 4 * ( pow(r_squared,-6) - pow(r_squared, -3));
            }
            else 
            {
                en += 8/3*PI *density*(1/3*pow(ratio_cut,9)-pow(ratio_cut,3));
            }


        }
    }
    return en;
     
}

double calculate_pressure(){
    double cut_off = mySys.box_x/2;
    double density= mySys.NPart/pow(mySys.box_x,3);
    double ratio_cut=mySys.sigma/cut_off;
    double r_squared;
    double f_i;
    int i,j;
    double pressure= density*mySys.T;
    for(i=0; i<mySys.NPart; i++)
    {
        
        for(j=0;j<i;j++)
        {
            if(i==j){continue;}

            r_squared=distance_squared_two_particles(i,j);
            //printf("distance: %f cutoff %f\n",r_squared, (mySys.cut_off*mySys.cut_off));
            if (r_squared < cut_off*cut_off)
            {
                pressure+=  1/(3*pow(mySys.box_x,3))*4 * (12*pow(1/r_squared,6) - 6*pow(1/r_squared, 3));
            }
        
                
            


        }
    } 
    pressure += 16/3*PI *density*density*(2/3*pow(ratio_cut,9)-pow(ratio_cut,3));
    return pressure;  

}



/********************************************************************************************/

double compute_energy_translation(){
    //assumes m=1, t per timestep=1
    long int i;
    double en = 0.;
    double distx,disty,distz;
    
    for(i = 0; i< mySys.NPart; i++){
     
        distx= MinD((parts[i].x -parts[i].ox), mySys.box_x);
        disty= MinD((parts[i].y -parts[i].oy), mySys.box_y);
        distz= MinD((parts[i].z -parts[i].oz), mySys.box_z);

        en += distx*distx +disty*disty +distz*distz;
        //en += (*(parts[i].x -parts[i].ox);
        //en += (parts[i].y -parts[i].oy)*(parts[i].y -parts[i].oy);
        //en += (parts[i].z -parts[i].oz)*(parts[i].z -parts[i].oz);   
    }
    en*=0.5;
   
    

    return en;
}


double compute_energy()
{

    long int i;
    double en = 0.;
    
    //long int index;
    //ong int ix, iy, iz;
    long int j;
    if(mySys.model ==0){
        en += compute_energy_translation();
    }
    
    
    
    if(mySys.model ==1)
    {
        en += compute_energy_hard_spheres();
    }
    if(mySys.model ==2)
    {
        en += compute_energy_Lennard_Jones();
       
    }
   
    return en;

}


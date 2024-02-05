
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
int get_overlaps(){

    int overlap=0;
    double distance;
    long int i, j;
    for(i = 0; i< mySys.NPart; i++)
    {
        for (j=0;j<mySys.NPart;j++)
        {
            if(i==j){continue;}
            distance = distance_two_particles(i,j);
            if (distance < mySys.sigma)
            {   
                overlap +=1;

            }
        }
    }
    return overlap/2;

}

double compute_energy_hard_spheres(){
    
    
    return 100000*get_overlaps();
    
    
    
}

double compute_energy_pair_Lennard_Jones(long int i, long int j){
    double distance = distance_two_particles(i,j);
    double en=0;
    double cut_off = mySys.box_x/2;
    double density= mySys.NPart/pow(mySys.box_x,3);
    double ratio= mySys.sigma/distance;
    double ratio_cut=mySys.sigma/cut_off;

    if (distance < cut_off)
    {
        en= 4 * ( pow(ratio,12) - pow(ratio,6));

    }
    else 
    {
        en = 8/3*PI *density*(1/3*pow(ratio_cut,9)-pow(ratio_cut,3));
    }
    return en;
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
    en += compute_energy_translation();
    
    
    if(mySys.model ==1)
    {
        en += compute_energy_hard_spheres();
    }
    if(mySys.model ==2)
    {
        for(i = 0; i< mySys.NPart; i++)
        {
            for (j=i;j<mySys.NPart;j++)
            {
                en += compute_energy_pair_Lennard_Jones(i,j);
            }
        }
    }
   
    return en;

}


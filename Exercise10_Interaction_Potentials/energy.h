

double r_squared_two_particles(long int i, long int j){
    double distx,disty,distz;
    double distance;
    distx= MinD((parts[i].x -parts[j].x), mySys.box_x);
    disty= MinD((parts[i].y -parts[j].y), mySys.box_y);
    distz= MinD((parts[i].z -parts[j].z), mySys.box_z);

    distance= distx*distx +disty*disty +distz*distz;
    
    return distance;

}

double compute_energy(){

    double r_squared;
    double en=0;
    int i,j;

    for(i=0; i<mySys.NPart; i++)
    {
        for(j=0;j<i;j++)
        {
            r_squared=r_squared_two_particles(i,j);
            //printf("distance: %f cutoff %f\n",r_squared, (mySys.cut_off*mySys.cut_off));
            if (r_squared < mySys.cut_off*mySys.cut_off)
            {
                

                en+= 4 * ( pow(r_squared,-6) - pow(r_squared, -3));
                
            }


        }
        /*
        double vx= MinD(parts[i].x-parts[i].ox,mySys.box_x);
        double vy= MinD(parts[i].y-parts[i].oy,mySys.box_y);
        double vz= MinD(parts[i].z-parts[i].oz,mySys.box_z);

        en+=0.5*(vx*vx+vz*vz+vy*vy);
        */
        en+=0.5*(parts[i].vx*parts[i].vx + parts[i].vz*parts[i].vz + parts[i].vy*parts[i].vy);

    }

    return en;
}
double kinetic_energy()
{
    double en=0;
    int i;
    for(i=0; i<mySys.NPart; i++)
    {
        /*

        double vx= MinD(parts[i].x-parts[i].ox,mySys.box_x);
        double vy= MinD(parts[i].y-parts[i].oy,mySys.box_y);
        double vz= MinD(parts[i].z-parts[i].oz,mySys.box_z);
        */
        en+=0.5*(parts[i].vx*parts[i].vx + parts[i].vz*parts[i].vz + parts[i].vy*parts[i].vy);
    }

    return en;
}
double total_momentum()
{
    double en=0;
    int i;
    for(i=0; i<mySys.NPart; i++)
    {
        /*

        double vx= MinD(parts[i].x-parts[i].ox,mySys.box_x);
        double vy= MinD(parts[i].y-parts[i].oy,mySys.box_y);
        double vz= MinD(parts[i].z-parts[i].oz,mySys.box_z);
        */
        en+=(parts[i].vx + parts[i].vz + parts[i].vy);
    }

    return en;
}

double calculate_force_per_rad(long int i, long int j)
{
    double r_squared;
    double force=0;
    if (j==i){return 0;}
    r_squared=r_squared_two_particles(i,j);
            //printf("distance: %f cutoff %f\n",r_squared, (mySys.cut_off*mySys.cut_off));

    if (r_squared < mySys.cut_off*mySys.cut_off)
    {
        force = 4 * ( 12*pow(1/r_squared,7) - 6* pow(1/r_squared, 4));   
    }
    return force;

}

void calculate_accelerations(long int i)
{
    
    
    double force_per_rad;
    
    long int  j;
    /*
    for(i=0; i<mySys.NPart; i++)
    {
        parts[i].ax =0;
        parts[i].ay =0;
        parts[i].az =0;
    }
    for(i=0;i<mySys.NPart-1;i++){

        for(j=i+1;j<mySys.NPart;j++)
        {
            force_per_rad=calculate_force_per_rad(i,j);

            parts[i].ax += force_per_rad*MinD(parts[i].x-parts[j].x,mySys.box_x);
            parts[i].ay += force_per_rad*(MinD(parts[i].y-parts[j].y,mySys.box_y));
            parts[i].az += force_per_rad*(MinD(parts[i].z-parts[j].z,mySys.box_z));

            parts[j].ax -= force_per_rad*(MinD(parts[i].x-parts[j].x,mySys.box_x));
            parts[j].ay -= force_per_rad*(MinD(parts[i].y-parts[j].y,mySys.box_y));
            parts[j].az -= force_per_rad*(MinD(parts[i].z-parts[j].z,mySys.box_z));   
        }
    }
    
    return ;
    */
   parts[i].ax=0;
   parts[i].ay=0;
   parts[i].az=0;

   for(j=0;j< mySys.NPart;j++)
        {
            
            force_per_rad=calculate_force_per_rad(i,j);

            parts[i].ax += force_per_rad*(MinD(parts[i].x-parts[j].x,mySys.box_x));
            parts[i].ay += force_per_rad*(MinD(parts[i].y-parts[j].y,mySys.box_y));
            parts[i].az += force_per_rad*(MinD(parts[i].z-parts[j].z,mySys.box_z)); 
        }


}

void verlet_step(double dt)
{
    
    long int i;
    double v_halfx,v_halfy,v_halfz;
    

    for(i=0; i<mySys.NPart; i++)
    {   
        calculate_accelerations(i);
        //1st half kick
        v_halfx = parts[i].vx + dt/2*parts[i].ax;
        v_halfy = parts[i].vy + dt/2*parts[i].ay;
        v_halfz = parts[i].vz + dt/2*parts[i].az;
        //drift
        parts[i].x= parts[i].x + v_halfx*dt;
        parts[i].y= parts[i].y + v_halfy*dt;
        parts[i].z= parts[i].z + v_halfz*dt;
    

        
        calculate_accelerations(i);
        //2nd half kick
        parts[i].vx=v_halfx+ dt/2*parts[i].ax;
        parts[i].vy=v_halfy+ dt/2*parts[i].ay;
        parts[i].vz=v_halfz+ dt/2*parts[i].az;
        //printf("speed %f  \n", parts[i].vx);
        //sprintf("kin. energy %f \n ", kinetic_energy());
    }
    
}










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
                //printf("FUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUCJKKK: %f",en);
            }


        }
        double vx= MinD(parts[i].x-parts[i].ox,mySys.box_x);
        double vy= MinD(parts[i].y-parts[i].oy,mySys.box_y);
        double vz= MinD(parts[i].z-parts[i].oz,mySys.box_z);

        en+=0.5*(vx*vx+vz*vz+vy*vy);
    }

    return en;
}
double kinetic_energy()
{
    double en=0;
    int i;
    for(i=0; i<mySys.NPart; i++)
    {

        double vx= MinD(parts[i].x-parts[i].ox,mySys.box_x);
        double vy= MinD(parts[i].y-parts[i].oy,mySys.box_y);
        double vz= MinD(parts[i].z-parts[i].oz,mySys.box_z);
        en+=0.5*(vx*vx+vz*vz+vy*vy);
    }

    return en;
}






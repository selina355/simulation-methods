void copycoordinates(long int idp)
{
     parts[idp].ox = parts[idp].x;
     parts[idp].oy = parts[idp].y;
     parts[idp].oz = parts[idp].z;

}

void rollback(long int idp)
{
     parts[idp].x = parts[idp].ox;
     parts[idp].y = parts[idp].oy;
     parts[idp].z = parts[idp].oz;

}
void old_to_temp(long int idp){
     parts[idp].tx = parts[idp].ox;
     parts[idp].ty = parts[idp].oy;
     parts[idp].tz = parts[idp].oz;

}
void temp_to_old(long int idp)
{
     parts[idp].ox = parts[idp].tx;
     parts[idp].oy = parts[idp].ty;
     parts[idp].oz = parts[idp].tz;
}


void translation(long int idp)
{   
    //translate particle at idp forward, and copy the coordiantes of all particles tat aare not moved
    
    //long int i;
    double x_disp,y_disp,z_disp;
    //for(i=0;i<mySys.NPart;i++)
    //{
    old_to_temp(idp);
    copycoordinates(idp);

    //} 
  
    x_disp = parts[idp].x + (-1 +2*ran3(&mySys.seed) ) * mySys.disp_max;
    y_disp = parts[idp].y + (-1 +2*ran3(&mySys.seed) ) * mySys.disp_max;
    z_disp = parts[idp].z + (-1 +2*ran3(&mySys.seed) ) * mySys.disp_max;

    //enforce periodic boundary conditions
    parts[idp].x= P_Img(x_disp, mySys.box_x);
    parts[idp].y= P_Img(y_disp, mySys.box_y);
    parts[idp].z= P_Img(z_disp, mySys.box_z);
    return;


}



void do_MC_sweep()
{
    double old_en, new_en, en_diff;
    long int i, chosen_i; 
    double u;
    for (i=0;i<mySys.NPart; i++)
    {
        //compute old energy

        old_en = compute_energy();
        mySys.energy = old_en;

        //draw random particle index i
        chosen_i = (long int) (ran3(&mySys.seed )*  mySys.NPart);
        translation(chosen_i);

        new_en = compute_energy();
        en_diff= new_en-old_en;
        u = ran3(&mySys.seed);
        
        if (u > exp(en_diff/mySys.T)){
            mySys.energy= new_en;
            mySys.accepted+=1;

        }
        else {
            rollback(chosen_i);
            temp_to_old(chosen_i);

        }
    }

    
}

void do_MC(){

    //char dumpname[100];
    //char restartname[100];

    FILE* f = fopen("energy_hard_spheres.dat", "a");
    FILE* g = fopen("acceptance_hard_spheres.dat", "a"); 

    //sprintf(restartname,"restartpoint.dat");

    for(mySys.step=0; mySys.step < mySys.NSteps; mySys.step++)
    {   
        mySys.accepted=0;
        do_MC_sweep();
        //WriteConf("configurations_test.dat");
        fprintf(f,"%f,", mySys.energy);
        fprintf(g, "%ld,", mySys.accepted);



        
        //if(mySys.step % 1000 == 0)  WriteConf(restartname);
     
        //if(mySys.step % mySys.NPrint == 0){ 
            //printf("dumping...\n");
    }
    fprintf(f, "\n");
    
   
    fclose(f); fclose(g);
   
}

//my function to  initialise the system
void initialise_random(){
    //double en = 0.;
    long int i;
    for( i = 0; i < mySys.NPart; i++)
    {
        parts[i].x = ran3(&mySys.seed)*mySys.box_x;
        parts[i].y= ran3(&mySys.seed)*mySys.box_y;
        parts[i].z = ran3(&mySys.seed)*mySys.box_y;

        translation(i);
/*
        ///initialise x y z after boltzman dist
        parts[i].x = parts[i].ox + gaussrand(&mySys.seed)*sqrt(mySys.T);
        parts[i].y = parts[i].oy + gaussrand(&mySys.seed)*sqrt(mySys.T);
        parts[i].z = parts[i].oz + gaussrand(&mySys.seed)*sqrt(mySys.T);
        parts[i].x= P_Img(parts[i].x, mySys.box_x);
        parts[i].y= P_Img(parts[i].y, mySys.box_y);
        parts[i].z= P_Img(parts[i].z, mySys.box_z);
         
        en += (parts[i].x -parts[i].ox)*(parts[i].x -parts[i].ox);
        en += (parts[i].y -parts[i].oy)*(parts[i].y -parts[i].oy);
        en += (parts[i].z -parts[i].oz)*(parts[i].z -parts[i].oz);   
    
*/
    }
}


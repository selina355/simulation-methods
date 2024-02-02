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
void oldv_to_temp(long int idp){
     parts[idp].ax = parts[idp].vx;
     parts[idp].ay = parts[idp].vy;
     parts[idp].az = parts[idp].vz;

}
void temp_to_oldv(long int idp)
{
     parts[idp].ox = parts[idp].ax;
     parts[idp].oy = parts[idp].ay;
     parts[idp].oz = parts[idp].az;
}

void translation_init(long int idp)
{   
    //translate particle at idp forward, and copy the coordiantes of all particles tat are not moved
    
    //long int i;
    double x_disp,y_disp,z_disp;

    //for(i=0;i<mySys.NPart;i++)
    //{
    oldv_to_temp(idp);
    copycoordinates(idp);

    //calculate displacement/velocity
    parts[idp].vx = (-1 +2*ran3(&mySys.seed) )*mySys.max_step;
    parts[idp].vy= (-1 +2*ran3(&mySys.seed) )*mySys.max_step;
    parts[idp].vz= (-1 +2*ran3(&mySys.seed) )*mySys.max_step;
    /*
    x_disp= (-1 +2*ran3(&mySys.seed) )*mySys.max_step;
    y_disp= (-1 +2*ran3(&mySys.seed) )*mySys.max_step;
    z_disp= (-1 +2*ran3(&mySys.seed) )*mySys.max_step;
    */
    //enforce periodic boundary conditions
    parts[idp].x= P_Img(parts[idp].x + parts[idp].vx, mySys.box_x);
    parts[idp].y= P_Img(parts[idp].y + parts[idp].vx, mySys.box_y);
    parts[idp].z= P_Img(parts[idp].z + parts[idp].vx, mySys.box_z);
    // printf("max disp: %f,Displacement in x,y,z: %f,%f,%f ",mySys.max_step,x_disp,y_disp,z_disp);




    return;

}

void do_MC_sweep()
{
    double old_en, new_en, en_diff;
    double u;
    long int i, chosen_i; 

    for (i=0;i<mySys.NPart; i++)
    {
        //compute old energy
        old_en = compute_energy();
        
        mySys.energy = old_en;
        
        chosen_i = (long int) (ran3(&mySys.seed )*  mySys.NPart);
        //printf("chosen particle %ld \n", chosen_i);
        
        //printf("positions old. %f,%f,%f ; energy %f\n",parts[chosen_i].x,parts[chosen_i].y,parts[chosen_i].z,old_en);
        translation_init(chosen_i);
        new_en = compute_energy();
        //printf("positions new. %f,%f,%f ; energy %f \n",parts[chosen_i].x,parts[chosen_i].y,parts[chosen_i].z,new_en);
      
        en_diff= new_en-old_en;

        //printf("new_en %f , enegry dif = %f , new overlaps %d\n" , new_en, en_diff, mySys.overlap);
     
     
   
        u = ran3(&mySys.seed);

        if (u <exp(-en_diff/mySys.T))
        {
            mySys.energy= new_en;
            mySys.accepted+=1;
            //printf("accepted! \n");//new particle position at %.3f , %.3f , %.3f\n",parts[chosen_i].x,parts[chosen_i].y,parts[chosen_i].z);

        }
        else
        {
            rollback(chosen_i);
            temp_to_oldv(chosen_i);
              
            mySys.energy=old_en;
            //printf("not accepted!\n");

        }

    }
               
}
   
    


void do_MC()
{

    //char dumpname[100];
    //char restartname[100];

    
    //FILE* g = fopen("acceptance_hard_spheres.dat", "a"); 

    //sprintf(restartname,"restartpoint.dat");
    printf("Equilibrating Initial Conditions... ....\n");
    for(mySys.step=0; mySys.step < mySys.NSteps; mySys.step++)
    {   
        mySys.accepted=0;
        do_MC_sweep();
        
        //WriteConf("configurations_test.dat");
        //fprintf(f,"%f,", mySys.energy);
        //printf(g, "%ld,", mySys.accepted);
        printf("%f, ", mySys.energy);
        printf( "%ld,", mySys.accepted);
        printf("%f, ", kinetic_energy());
        printf("total momentum: %f \n",total_momentum());

        
        //if(mySys.step % 1000 == 0)  WriteConf(restartname);
     
        //if(mySys.step % mySys.NPrint == 0){ 
            //printf("dumping...\n");
    }
    WriteConf("inital_for_r_c=big.dat");

    //fprintf(g, "\n");

    //fclose(f); fclose(g);
}

//enforce zero tot momentum:
void draw_velocities(){
    int i;
    double tot_vx,tot_vy,tot_vz;
    tot_vx=0;tot_vy=0;tot_vz=0;

    for( i = 0; i < mySys.NPart; i++)
    {
        //initialise velocities after boltzman dist
        parts[i].vx = gaussrand(&mySys.seed)*sqrt(mySys.T);
        parts[i].vy = gaussrand(&mySys.seed)*sqrt(mySys.T);
        parts[i].vz = gaussrand(&mySys.seed)*sqrt(mySys.T);

        tot_vx+=parts[i].vx; tot_vy+=parts[i].vy; tot_vz+=parts[i].vz;
    }
    for( i = 0; i < mySys.NPart; i++)
    {

        //enforce total momentum=0
        parts[i].vx =  parts[i].vx- tot_vx/mySys.NPart;
        parts[i].vy =  parts[i].vy- tot_vy/mySys.NPart;
        parts[i].vz = parts[i].vz- tot_vz/mySys.NPart;
         
    }

}

//my function to  initialise the system
void initialise_random_start()
{
    //double en = 0.;
    long int i;
    double tot_vx,tot_vy,tot_vz;
    tot_vx=0;tot_vy=0;tot_vz=0;
   
    for( i = 0; i < mySys.NPart; i++)
    {
        //initalise positions randomly
        parts[i].x = ran3(&mySys.seed)*mySys.box_x;
        parts[i].y= ran3(&mySys.seed)*mySys.box_y;
        parts[i].z = ran3(&mySys.seed)*mySys.box_y;

        ///initialise velocities after boltzman dist
        parts[i].vx = gaussrand(&mySys.seed)*sqrt(mySys.T);
        parts[i].vy = gaussrand(&mySys.seed)*sqrt(mySys.T);
        parts[i].vz = gaussrand(&mySys.seed)*sqrt(mySys.T);

        tot_vx+=parts[i].vx; tot_vy+=parts[i].vy; tot_vz+=parts[i].vz;
    }
    
    for( i = 0; i < mySys.NPart; i++)
    {

        //enforce total momentum=0
        parts[i].vx =  parts[i].vx- tot_vx/mySys.NPart;
        parts[i].vy =  parts[i].vy- tot_vy/mySys.NPart;
        parts[i].vz = parts[i].vz- tot_vz/mySys.NPart;
    
    }
    
    printf("total kinetic energy: %f\n", kinetic_energy());
    printf("total momentum: %f \n",total_momentum());
    return;
}

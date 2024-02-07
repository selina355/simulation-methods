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
    //printf("Displacement in x,y,z: %f,%f,%f ",x_disp - parts[idp].x,y_disp-parts[idp].y,z_disp - parts[idp].z);
    //enforce periodic boundary conditions
    parts[idp].x= P_Img(x_disp, mySys.box_x);
    parts[idp].y= P_Img(y_disp, mySys.box_y);
    parts[idp].z= P_Img(z_disp, mySys.box_z);
    return;


}



void do_MC_sweep()
{
    double old_en, new_en, en_diff, old_overlaps;
    int old_o,new_o;
    long int i, chosen_i; 
    double u;

    for (i=0;i<mySys.NPart; i++)
    {
        //compute old energy
        old_en = compute_energy();
        old_o= get_overlaps();
        mySys.energy = old_en;
        mySys.overlap= old_o;


        ///printf("old en %f , old overlaps %d" , old_en, old_o);

        //draw random particle index i
        chosen_i = (long int) (ran3(&mySys.seed )*  mySys.NPart);
        
        //printf("chosen  particle %ld, at %.3f , %.3f , %.3f " , chosen_i,parts[chosen_i].x,parts[chosen_i].y,parts[chosen_i].z);
        translation(chosen_i);
        new_en = compute_energy();
        new_o = get_overlaps();
        en_diff= new_en-old_en;

        //printf("new_en %f , enegry dif = %f , new overlaps %d\n" , new_en, en_diff, mySys.overlap);
        if(mySys.model==1)
        {
            if ((new_o< old_o )|| new_o== 0)
            {
                mySys.energy= new_en;
                mySys.overlap= new_o;

                mySys.accepted+=1;
                //printf("accepted! new particle position at %.3f , %.3f , %.3f\n",parts[chosen_i].x,parts[chosen_i].y,parts[chosen_i].z);

            }
            else
            {
            rollback(chosen_i);
            temp_to_old(chosen_i);
            mySys.overlap=old_o;
            //printf("not accepted!\n");

            }

        }
        else
        {
            u = ran3(&mySys.seed);

            if (u < exp(-en_diff/mySys.T))
            {
                mySys.energy= new_en;
                mySys.overlap= new_o;

                mySys.accepted+=1;
                //printf("accepted! new particle position at %.3f , %.3f , %.3f\n",parts[chosen_i].x,parts[chosen_i].y,parts[chosen_i].z);

            }
            else
            {
                rollback(chosen_i);
                temp_to_old(chosen_i);
                mySys.overlap=old_o;
                mySys.energy=old_en;
                //printf("not accepted!\n");

            }

        }
               
    }
    //printf("%ld, %d\n",mySys.accepted,mySys.overlap);
    
}

void do_MC( char* energy_file, char*acceptance_file, char* finaldistances_file)
{

    //char dumpname[100];
    //char restartname[100];

    FILE* f = fopen(energy_file, "a");
    FILE* g = fopen(acceptance_file, "a"); 
    FILE* h=fopen(finaldistances_file, "a");

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
    //print final distances between particles
    for( int i = 0; i < mySys.NPart; i++)
    {
        for (int j=0;j<i; j++){
            fprintf(h, "%f,", distance_two_particles(i,j) );
        }  
    }

    fprintf(f, "\n");
    fprintf(g, "\n");
    fprintf(h, "\n");


    printf("%f, ", mySys.energy);
    printf( "%ld,\n", mySys.accepted);
   
    fclose(f); fclose(g);
}

//my function to  initialise the system
void initialise_random(){
    //double en = 0.;
    long int i,j,k;
    double x_disp,y_disp,z_disp;
    
    for( i = 0; i < mySys.NPart; i++)
    {

        parts[i].x = ran3(&mySys.seed)*mySys.box_x;
        parts[i].y= ran3(&mySys.seed)*mySys.box_y;
        parts[i].z = ran3(&mySys.seed)*mySys.box_y;
        translation(i);
    
    }
    return;
}


//my function to  initialise the system
void initialise_lattice(){
    
    //double[mySys.NPart] usedx, double[mySys.NPart] usedy, double[mySys.NPart] usedz; 

   
    

    // total number of particles that fit in each direction
    int nx = (int)floor(mySys.box_x/mySys.sigma)-1;
    int ny = (int)floor(mySys.box_y/mySys.sigma)-1;
    int nz = (int)floor(mySys.box_z/mySys.sigma)-1;

    int count = 0;

    
    for (int i = 0; i < nx; i++) {
        for (int j = 0; j < ny; j++)
        {
            for (int k = 0; k < nz; k++){
                if (count < mySys.NPart){
                parts[i*nz*ny + j*nz + k].x = i*mySys.sigma;  
                parts[i*nz*ny + j*nz + k].y = j*mySys.sigma;
                parts[i*nz*ny + j*nz + k].z = k*mySys.sigma;
                count++;
                }
            }
        }
        
        
    }


    mySys.overlap= get_overlaps();
    printf("INITIAL OVERLAPS %d\n",mySys.overlap);


    return;
    }
    

        

    
/*       

        translation(i);

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

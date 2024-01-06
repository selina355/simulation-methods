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


void translation(long int idp)
{   
    //translate particle at idp forward, and copy the coordiantes of all particles tat aare not moved
    
    long int i;
    double x_disp,y_disp,z_disp;
    for(i=0;i<mySys.NPart;i++)
    {
        copycoordinates(i);
    } 
  

    
    x_disp = parts[idp].x + (-1 +2*rand3(&mySys.seed) ) * mySys.disp_max;
    y_disp = parts[idp].y + (-1 +2*rand3(&mySys.seed) ) * mySys.disp_max;
    z_disp = parts[idp].z + (-1 +2*rand3(&mySys.seed) ) * mySys.disp_max;

    //enforce periodic boundary conditions
    parts[idp].x= P_Img(x_disp, mySys.box_x);
    parts[idp].y= P_Img(y_disp, mySys.box_y);
    parts[idp].z= P_Img(z_disp, mySys.box_z);


}



void do_MC_sweep()
{
    double old_en, new_en, en_diff;
    long int chosen_i; 
    double u= rand3(&mySys.seed);
    //compute old energy
;
    old_en = compute_energy();

    //draw random particle index i
    chosen_i = rand3(&mySys.seed)%mySys.NPart;
    translation(chosen_i);

    new_en = compute_energy();
    en_diff= old_en-new_en;

    if (u<= exp(en_diff/mySys.T))
    {
        rollback(chosen_i);
        return;

    }

    mySys.energy= new_en;




 //translate  i , keep rest the same 

 //compute new energy of the whole system

 //metropolisdecision move not accepted: rollback

 //

}

void do_MC(){

    char dumpname[100];
    char restartname[100];

    FILE* f = fopen("energy.dat", "a");
    FILE* g = fopen("acceptance.dat", "a"); 

    sprintf(restartname,"restartpoint.dat");
    
    for(mySys.step=0; mySys.step < mySys.NSteps; mySys.step++){
         
        do_MC_sweep();
        WriteConf()
        //if(mySys.step % 1000 == 0)  WriteConf(restartname);
     
        if(mySys.step % mySys.NPrint == 0){ 
            //printf("dumping...\n");
        }
    }
   
    fclose(f); fclose(g);
   
}


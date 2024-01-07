
#define min(a,b)    ({ __typeof__ (a) _a = (a);  __typeof__ (b) _b = (b); _a < _b ? _a : _b; })

double compute_energy_somemodel(int i, int j){

    return 0.;
}

//test
/********************************************************************************************/

double compute_energy_translation(){

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


double compute_energy(){

    int i;
    double en = 0.;
    //long int index;
    //ong int ix, iy, iz;
    //ong int j;

    en += compute_energy_translation();


    for(i = 0; i< mySys.NPart; i++){
    
    
    }
   
    return en;

}


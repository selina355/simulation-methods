
#define min(a,b)    ({ __typeof__ (a) _a = (a);  __typeof__ (b) _b = (b); _a < _b ? _a : _b; })

double compute_energy_somemodel(int i, int j){

    return 0.;
}


/********************************************************************************************/

double compute_energy_translation(){

    long int i;
    double en = 0.;

    for(i = 0; i< mySys.NPart; i++){
        en += (parts[i].x -parts[i].ox)*(parts[i].x -parts[i].ox);
        en += (parts[i].y -parts[i].oy)*(parts[i].y -parts[i].oy);
        en += (parts[i].z -parts[i].oz)*(parts[i].z -parts[i].oz);   
    }
    en*=0.5;
   
    

    return en;
}


double compute_energy(){

    int i;
    double en = 0.;
    long int index;
    long int ix, iy, iz;
    long int j;
    en += compute_energy_translation();
;

    for(i = 0; i< mySys.NPart; i++){
    
    
    }
   
    return en;

}


void allocate_()
{
    parts = ( Particle *)malloc(mySys.NPart*sizeof(Particle));   
}

void clean_()
{
    free(parts);
}



double MinD(double dx, double L){

    double dx1;
    dx1 = dx - rint(dx/L)*L;
    return dx1;
}

double P_Img (double z, double L){
        
    double z1;
    z1 = z - floor(z/L)*L;
    return z1;
}

void ReadConf(char filename[])
{
    FILE* fp = fopen(filename, "r"); 
    if(fp == NULL){ printf("File does not exist!\n"); exit(1); }

    double a, b, c,d,e,f;

    for(int i = 0; i < mySys.NPart; i++) 
    {
        fscanf(fp, "%lf %lf %lf %lf %lf %lf \n", &a, &b, &c, &d, &e, &f); 
	    parts[i].x = a; parts[i].y = b; parts[i].z = c;
        parts[i].vx=d; parts[i].vy=e; parts[i].vz=f;
        printf( "%lf %lf %lf %lf %lf %lf \n", parts[i].x, parts[i].y, parts[i].z,parts[i].vx, parts[i].vy, parts[i].vz); 
    }
    fclose(fp); 
}



void WriteConf(char filename[])
{
    FILE* fp = fopen(filename, "w"); 

    for(int i = 0; i < mySys.NPart; i++) 
    {
        fprintf(fp, "%lf %lf %lf %lf %lf %lf \n", parts[i].x, parts[i].y, parts[i].z,parts[i].vx, parts[i].vy, parts[i].vz); 
    }
    fflush(fp); fclose(fp); // Close file when we're done
}





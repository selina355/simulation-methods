void read_input_file()
{
    FILE *_myfile = fopen("param.dat", "r");
    
    char line[100];
    int i=0;
    char *token;
    char *_key=NULL;
    char *_value = NULL;

    while(fgets(line, sizeof(line), _myfile) != NULL){

      token = strtok(line, " ");
      i = 0;
      while (token != NULL) {
          if (i == 0) _key = token;
	  if (i == 2) _value = token;
          token = strtok(NULL, " ");
          i++;
      }     
   
      if(strcmp("N",_key) == 0){ mySys.NPart = atoi(_value); continue; }
      if(strcmp("N_steps",_key) == 0){ mySys.NSteps = atoi(_value); continue; }
      if(strcmp("box_x",_key) == 0){ mySys.box_x = atof(_value); continue; } 
      if(strcmp("box_y",_key) == 0){ mySys.box_y = atof(_value); continue; }
      if(strcmp("box_z",_key) == 0){ mySys.box_z = atof(_value); continue; }
      if(strcmp("temperature",_key) == 0){ mySys.T = atof(_value); continue; }
      if(strcmp("myseed",_key) == 0){ mySys.seed = atof(_value); continue; } 
      if(strcmp("cutoff",_key) == 0){ mySys.cut_off = atof(_value); continue; }
      if(strcmp("max_step",_key) == 0){ mySys.max_step = atoi(_value); continue; }  
      

    }
    
    fclose(_myfile);

}
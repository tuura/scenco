#ifdef __linux
	#include "minimising.h"
#else
	#include "D:\Projects\PRGM_WORKCRAFT\inc\minimising.h"
#endif

/*BOOLEAN FUNCTION*/
/*Following function uses espresso program (developed by Berkeley University) for finding 
and minimising logic functions, with respect encoding considered.*/
int boolean_function(int max,int bits, int cpog_count,int co){
	char *file_out, *command, c;
	int i=0,j=0,p=0,n=0,k=0,val;
	FILE *fp,*pp;
#ifdef ACT_PERCENTAGE
	float act,total,res,res_back = 101;
	printf("Percentage till complete:\n");
#endif
	/*NAME OF THE FILES FOR CALLING ESPRESSO PROGRAM*/
	command = (char*) malloc(sizeof(char) * COMMANDS_LENGTH);

	/*FILE_TEMP NAME*/
	file_out = strdup(TMP_FILE);

#ifdef __linux
	strcpy(command, ESPRESSO_PATH);
	strcat(command, " ");
	strcat(command, ESPRESSO_FILTER);
	strcat(command, file_out);
#else
	strcpy(command, "\"");
	strcat(command, ESPRESSO_PATH);
	strcat(command, "\" ");
	strcat(command, ESPRESSO_FILTER);
	strcat(command, file_out);
#endif


	
	if(!co){
		for(i=0; i<counter;i++){
			for(p=0;p<nv; p++){
				for(j=0;j<nv;j++){
					if( (val = eval_function(cpog[p][j].truth,cpog_count)) == 0 ){

						/*WRITING FILE CONTENT FOR ESPRESSO*/
						if( (fp = fopen(file_out,"w")) == NULL ){
							printf("Error on opening file.\n");
							return 1;
						}

						fprintf(fp,".i %d\n", bits); /*Number of inputs*/
						fprintf(fp,".o 1\n");
						fprintf(fp,".ilb "); /*Names of inputs*/
						for(k=0;k<bits;k++) fprintf(fp, "x_%d ", k);
						fprintf(fp,"\n");
						fprintf(fp, ".ob "); /*Names of outputs*/
						if(cpog[p][j].type == 'v')
							fprintf(fp,"%s ",cpog[p][j].source);
						else
							fprintf(fp, "%s>%s ",cpog[p][j].source,cpog[p][j].dest);
				
						fprintf(fp,"\n");
						if(tot_enc == cpog_count){
							for(k=0;k<cpog_count;k++){
								print_binary(fp,cons_perm[i][k], bits); /*Input encodings*/
								fprintf(fp," ");
								fprintf(fp, "%c",cpog[p][j].truth[k]);
								fprintf(fp,"\n");
							}
						}else{
							write_conditions(fp,cpog_count,i,p,j,bits,co);
						}
						fprintf(fp,".e");
						fclose(fp);

						/*DEBUG STOPPING: information about espresso output*/
						/*printf("CALLING ESPRESSO: %s\n", command);
						system(command);
						return 2;*/
				
						/*CALLING ESPRESSO FRAMEWORK*/
						if( (pp = popen(command,"r")) == NULL){
							printf("Error on calling espresso program: %s.\n", command);
							return 3;
						}

						/*READING ESPRESSO OUTPUT*/
						while( (c = fgetc(pp)) != '=');
						c = fgetc(pp);
						n = 0;

						while( c != ';'){
							switch(c){
								case '&':
									cpog[p][j].fun[i][n++] = '*';
									c = fgetc(pp);
									break;
								case '|':
									cpog[p][j].fun[i][n++] = '+';
									c = fgetc(pp);
									break;
								case ' ':
									c = fgetc(pp);
									break;
								case '\n':
									if( feof(pp) ){
										printf("Error on reading espresso output.\n");
										return 3;
									}
									while( (c = fgetc(pp)) != ' ');
									break;
								default:
									cpog[p][j].fun[i][n++] = c;
									c = fgetc(pp);
							}
						}
						cpog[p][j].fun[i][n] = '\0';
						/*DEBUG PRINTING: function read*/
						//printf("%s\n\n",cpog[p][j].fun[i]);
						if (!strcmp(cpog[p][j].fun[i], "()")) strcpy(cpog[p][j].fun[i], "1");
						if (!strcmp(cpog[p][j].fun[i], "")) strcpy(cpog[p][j].fun[i], "0");
				
						fclose(pp);
				
						/*DEBUG PRINTING: logic functions*/
						//printf("%s - %s\n",cpog[p][j].fun[i],cpog[p][j].fun_cond[i]);
					}else{
						if(val == 1)
							strcpy(cpog[p][j].fun[i], "1");
						else
							strcpy(cpog[p][j].fun[i], "0");
					}
				}
			}

#ifdef ACT_PERCENTAGE	
			/*PRINTING PERCENTAGE ELAPSED TO COMPLETION*/
			act = i;
			total = counter;
			res = (int)((act/total) * 100);
			if(res != res_back){
				printf("%d - ", (int)res);
				res_back = res;
			}
			fflush(stdout);
#endif
		}
	} else{
		for(i=0; i<counter;i++){
			for(p=0;p<nv; p++){
				if(cpog[p][p].type == 'v' && cpog[p][p].condition){
					if( (val = eval_function(cpog[p][p].truth_cond,cpog_count)) == 0 ){
						/*WRITING FILE CONTENT FOR ESPRESSO*/
						if( (fp = fopen(file_out,"w")) == NULL ){
							printf("Error on opening file.\n");
							return 4;
						}

						fprintf(fp,".i %d\n", bits); /*Number of inputs*/
						fprintf(fp,".o 1\n"); /*Number of outputs*/
						fprintf(fp,".ilb "); /*Names of inputs*/
						for(k=0;k<bits;k++) fprintf(fp, "x_%d ", k);
						fprintf(fp,"\n");
						fprintf(fp, ".ob "); /*Names of outputs*/
						fprintf(fp,"c(%s) ",cpog[p][p].source);

						fprintf(fp,"\n");

						if(tot_enc == cpog_count){
							for(k=0;k<cpog_count;k++){
								print_binary(fp,cons_perm[i][k], bits); /*Input encodings*/
								fprintf(fp," ");
								fprintf(fp,"%c",cpog[p][p].truth_cond[k]);
								fprintf(fp,"\n");
							}
						}
						else
							write_conditions(fp,cpog_count,i,p,p,bits,co);
						fprintf(fp,".e");
						fclose(fp);

						/*DEBUG STOPPING: information about espresso output*/
						/*if(p == 0) {
							system(command);
							return 5;
						}*/
			
						/*CALLING ESPRESSO FRAMEWORK*/
						if( (pp = popen(command,"r")) == NULL){
							printf("Error on calling espresso program: %s.\n", command);
							return 6;
						}

						/*READING FIRST OUTPUT LINE OF ESPRESSO*/
						while( (c = fgetc(pp)) != '=');
						c = fgetc(pp);
						n = 0;

						while( c != ';'){
							switch(c){
								case '&':
									cpog[p][j].fun[i][n++] = '*';
									c = fgetc(pp);
									break;
								case '|':
									cpog[p][j].fun[i][n++] = '+';
									c = fgetc(pp);
									break;
								case ' ':
									c = fgetc(pp);
									break;
								case '\n':
									if( feof(pp) ){
										printf("Error on reading espresso output.\n");
										return 3;
									}
									while( (c = fgetc(pp)) != ' ');
									break;
								default:
									cpog[p][j].fun[i][n++] = c;
									c = fgetc(pp);
							}
						}
						cpog[p][p].fun_cond[i][n] = '\0';
						if (!strcmp(cpog[p][p].fun_cond[i], "()")) strcpy(cpog[p][p].fun_cond[i], "1");
						if (!strcmp(cpog[p][p].fun_cond[i], "")) strcpy(cpog[p][p].fun_cond[i], "0");	
						fclose(pp);
					}else{
						if(val == 1)
							strcpy(cpog[p][p].fun_cond[i], "1");
						else
							strcpy(cpog[p][p].fun_cond[i], "0");

					}
				}
				/*DEBUG PRINTING: logic functions*/
				//printf("%s\n",cpog[p][p].fun_cond[i]);
			}
#ifdef ACT_PERCENTAGE	
		/*PRINTING PERCENTAGE ELAPSED TO COMPLETION*/
		act = i;
		total = counter;
		res = (int)((act/total) * 100);
		if(res != res_back){
			printf("%d - ", (int)res);
			res_back = res;
		}
		fflush(stdout);
#endif
		}
	}
#ifdef ACT_PERCENTAGE	
	printf("100\n");
	fflush(stdout);
#endif

	/*REMOVING TMP FILE AND FREE USELESS STRINGS*/
#ifdef __linux
	strcpy(command,"rm -f ");
#else
	strcpy(command,"del ");
#endif
	strcat(command, file_out);
	if(system(command) != 0){
		printf("Error on removing tmp files.\n");
		return 3;
	}
	free(file_out);
	free(command);

	return 0;
}

/*DECIDE FUNCTION*/
/*Following function, simply check if a function is 0 or 1, if this is the case it mustn't
be written in the .prg file.*/
int decide(char* function){
	if(function[0] == '1')
		return 1;
	if(function[0] == '0')
		return 2;
	if(function == NULL)
		return 3;
	return 0;
}

/*DECODER FUNCTION*/
/*Following function uses espresso program (developed by Berkeley University) for finding 
and minimising logic functions of the decoder needed if the number of bit for encoding
CPOG is not minimum.*/
int decoder_minimisation(int bits, int cpog_count){
	char* file_out, *command, c;
	int i=0,j=0,n=0,k=0,min_bits,min_tot_enc;
	FILE *fp,*pp;
#ifdef ACT_PERCENTAGE
	printf("Percentage till complete:\n");
#endif
	/*NAME OF THE FILES FOR CALLING ESPRESSO PROGRAM*/
	file_out = (char*) malloc(sizeof(char) * FILENAME_LENGTH);
	command = (char*) malloc(sizeof(char) * COMMANDS_LENGTH);

	/*FILE_TEMP NAME*/
	file_out = strdup(TMP_FILE);

	/*CONSTRUCTION ESPRESSO COMMAND*/
	strcpy(command, ESPRESSO_PATH);
	strcat(command, ESPRESSO_FILTER);
	strcat(command, file_out);

	/*ALLOCATING BASE STRUCTURES FOR DECODER*/
	min_bits = logarithm2(cpog_count);
	min_tot_enc = 1;
	for(i=0;i<min_bits;i++) min_tot_enc *=2;
	decoder = (char**) malloc(sizeof(char*) * bits);
	for(i=0;i<bits;i++)
		decoder[i] = (char*) malloc(sizeof(char) * MAX_BOOL);

	for(i=0;i<bits;i++){
		/*WRITING FILE CONTENT FOR ESPRESSO*/
		if( (fp = fopen(file_out,"w")) == NULL ){
			printf("Error on opening file.\n");
			return 1;
		}

		fprintf(fp,".i %d\n", min_bits); /*Number of inputs*/
		fprintf(fp,".o 1\n");
		fprintf(fp,".ilb "); /*Names of inputs*/
		for(k=0;k<min_bits;k++) fprintf(fp, "Y%d ", k);
		fprintf(fp,"\n");
		fprintf(fp, ".ob "); /*Names of outputs*/
		fprintf(fp,"x_%d ",i);

		fprintf(fp,"\n");
		for(j=0;j<min_tot_enc;j++){
			print_binary(fp,j, min_bits); /*Input encodings*/
			fprintf(fp," ");
			if(j < cpog_count)
				fprintf(fp, "%c",manual_file[j][i]);
			else
				fprintf(fp,"-");
			fprintf(fp,"\n");
		}
		fprintf(fp,".e");
		fclose(fp);

		/*DEBUG STOPPING: information about espresso output*/
		/*if(j == 1) {
			system(command);
			return 2;
		}*/

		/*CALLING ESPRESSO FRAMEWORK*/
		if( (pp = popen(command,"r")) == NULL){
			
			printf("Error on calling espresso program: %s.\n", command);
			return 3;
		}

		/*READING FIRST OUTPUT LINE OF ESPRESSO*/
		while( (c = fgetc(pp)) != '=');
		c = fgetc(pp);
		n = 0;

		while( c != ';'){
			switch(c){
				case '&':
					decoder[i][n++] = '*';
					c = fgetc(pp);
					break;
				case '|':
					decoder[i][n++] = '+';
					c = fgetc(pp);
					break;
				case ' ':
					c = fgetc(pp);
					break;
				case '\n':
					if( feof(pp) ){
						printf("Error on reading espresso output.\n");
						return 3;
					}
					while( (c = fgetc(pp)) != ' ');
					break;
				default:
					decoder[i][n++] = c;
					c = fgetc(pp);
			}
		}	
		decoder[i][n] = '\0';
		if (!strcmp(decoder[i], "()")) strcpy(decoder[i], "1");
		if (!strcmp(decoder[i], "")) strcpy(decoder[i], "0");

		fclose(pp);

		/*DEBUG PRINTING: logic functions*/
		//printf("DECODER %d) %s\n",i,decoder[i]);
	}

	/*REMOVING TMP FILE AND FREE USELESS STRINGS*/
	free(command);
	command = (char*) malloc(sizeof(char) * COMMANDS_LENGTH);
#ifdef __linux
	sprintf(command,"rm -f ");
#else
	sprintf(command,"del ");
#endif
	strcat(command, file_out);
	if(system(command) != 0){
		printf("Error on removing tmp files.\n");
		return 3;
	}
	free(file_out);
	free(command);

	return 0;
}

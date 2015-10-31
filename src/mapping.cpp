#ifdef __linux
	#include "mapping.h"
#else
	#include "D:\Projects\PRGM_WORKCRAFT\inc\mapping.h"
#endif

/*FINAL EQUATIONS AND ABC FUNCTION*/
/*Following function write in some file in a format compatible with abc tool (Developed by Berkeley
University), in order to get statistics about area of encoder. Moreover it calls abc tool.*/
int equations_abc(int cpog_count, int bits){
	int i,c,k,j,min_bits = 1;
	float k2;
	boolean ins = FALSE;
	FILE *fp = NULL, *pp = NULL;
	char *file_name = NULL,*file_name_abc = NULL, *string, *line, command[MAX_LINE],name[MAX_LINE];
	char dump1[MAX_NAME],dump2[MAX_NAME],dump3[MAX_NAME];
	
#ifdef ACT_PERCENTAGE
	float act,total,res,res_back = 101;
	printf("Percentage till complete:\n");
#endif

	line = (char*) malloc(sizeof(char) * MAX_LINE);
	string = (char*) malloc(sizeof(char) * MAX_BOOL);

	if(decode_flag)
		min_bits = logarithm2(cpog_count);

	for(c=0;c<counter;c++){
		//DEFINE FILE NAMES
		file_name = (char*) malloc(sizeof(char) * MAX_LINE);
		strcpy(file_name,FOLDER_NAME);
#ifdef __linux
		
		strcat(file_name,"/");
		sprintf(string,"fileXXXXXX");
		if (mkstemp(string) == -1){
			printf(".error \n");
			printf("Error on opening creating temporary file name in equation_abc: %s.\n", string);
			printf(".end_error \n");
			removeTempFiles();
			return 1;
		}
#else
		strcat(file_name,"\\");
		tmpnam (string);
#endif
		strcat(file_name, string);
		strcat(file_name, EXTENSION_ENCODING);

		file_name_abc = (char*) malloc(sizeof(char) * MAX_LINE);
		strcpy(file_name_abc,FOLDER_NAME);
#ifdef __linux
		strcat(file_name_abc,"/");
#else
		strcat(file_name_abc,"\\");
#endif
		strcat(file_name_abc, string);
		strcat(file_name_abc, EXTENSION_ENCODING);
		

		//OPNENING FILE
		if( (fp = fopen(file_name,"w")) == NULL ){
			printf("NAME FILE: %s\n", file_name);
			printf("Error on opening file.\n");
			return 1;
		}

		//INPUT NAMES
		fprintf(fp,"INORDER =");
		if(!decode_flag)
			for(k=0;k<bits;k++)
				fprintf(fp," x_%d",k);
		for(k=0;k<nv;k++)
			fprintf(fp," ACK_%s",cpog[k][k].source);
		for(k=0;k<n_cond;k++)
			fprintf(fp," %s",name_cond[k]);
		if(decode_flag){
			for(k=0;k<min_bits;k++)
				fprintf(fp," Y%d",k);
		}
		fprintf(fp,";\n");

		//OUTPUT NAMES
		fprintf(fp,"OUTORDER =");
		if(decode_flag)
			for(k=0;k<bits;k++)
				fprintf(fp," x_%d",k);
		for(k=0;k<nv;k++)
			fprintf(fp," REQ_%s",cpog[k][k].source);
		fprintf(fp,";\n");

		//DECODER
		if(decode_flag){
			for(i = 0;i<bits;i++){
				fprintf(fp,"x_%d = ", i);
				fprintf(fp,"%s;\n",decoder[i]);
			}
		}

		//FUNCTIONS
		for(i=0;i<nv; i++){
			fprintf(fp,"REQ_%s = ", cpog[i][i].source);
			ins = FALSE;
			strcpy(line, "");
			for(j=0;j<nv;j++){
				/*DEBUG PRINTING: Information about and*/
				/*if(i == 1)
					printf("%d",ins);*/
				strcpy(string,"");
				//CONDITIONS OF VERTICES
				if(j == i){
					if(cpog[j][i].condition){
						if( (!decide(cpog[j][i].fun[c]) || !decide(cpog[j][i].fun_cond[c])) && ins){
							sprintf(string," * ");
							strcat(line,string);
						}
						if (!decide(cpog[j][i].fun[c]) || !decide(cpog[j][i].fun_cond[c]))
							ins = TRUE;
						if(!decide(cpog[j][i].fun_cond[c]) && !decide(cpog[j][i].fun[c])){
							sprintf(string,"( ( (%s) * (%s) ) + (%s)) ",cpog[j][i].fun_cond[c],cpog[j][i].cond,cpog[j][i].fun[c]);
							strcat(line,string);
						}
						if(decide(cpog[j][i].fun_cond[c]) && !decide(cpog[j][i].fun[c])){
							sprintf(string,"( (%s) + (%s) ) ",cpog[j][i].cond,cpog[j][i].fun[c]);
							strcat(line,string);
						}
						if(!decide(cpog[j][i].fun_cond[c]) && decide(cpog[j][i].fun[c])){
							sprintf(string,"( (%s) * (%s) ) ",cpog[j][i].cond,cpog[j][i].fun_cond[c]);
							strcat(line,string);
						}
						if(decide(cpog[j][i].fun_cond[c]) && decide(cpog[j][i].fun[c]))
							ins = FALSE;
					}else{	
						if(!decide(cpog[j][i].fun[c]) && ins){
							sprintf(string," * ");		
							strcat(line,string);
						}			
						if(!decide(cpog[j][i].fun[c])){
							ins = TRUE;
							sprintf(string,"(%s) ",cpog[j][i].fun[c]);
							strcat(line,string);
						}
					}
				//CONDITIONS OF EDGES
				}else{
					if(decide(cpog[j][i].fun[c]) != 2){
						if(cpog[j][j].condition){
							if( (!decide(cpog[j][i].fun[c]) || !decide(cpog[j][j].fun[c]) || decide(cpog[j][j].fun_cond[c]) != 3) && ins){
								sprintf(string," * ");
								strcat(line,string);
							}
							if (!decide(cpog[j][i].fun[c]) || !decide(cpog[j][j].fun[c]) || decide(cpog[j][j].fun_cond[c]) != 3)
								ins = TRUE;

							if(!decide(cpog[j][i].fun[c]) && !decide(cpog[j][j].fun[c]) && !decide(cpog[j][j].fun_cond[c])){
								sprintf(string,"(ACK_%s + !((((%s) * (%s)) + (%s)) * (%s)))",cpog[j][i].source,cpog[j][j].fun_cond[c],cpog[j][j].cond,cpog[j][j].fun[c],cpog[j][i].fun[c]);
								strcat(line,string);
							}

							if(decide(cpog[j][i].fun[c]) && !decide(cpog[j][j].fun[c]) && !decide(cpog[j][j].fun_cond[c])){
								sprintf(string,"(ACK_%s + !(((%s) * (%s)) + (%s)))",cpog[j][i].source,cpog[j][j].fun_cond[c],cpog[j][j].cond,cpog[j][j].fun[c]);
								strcat(line,string);
							}

							if(!decide(cpog[j][i].fun[c]) && decide(cpog[j][j].fun[c]) && !decide(cpog[j][j].fun_cond[c])){
								sprintf(string,"(ACK_%s + !(((%s) * (%s)) * (%s)))",cpog[j][i].source,cpog[j][j].fun_cond[c],cpog[j][j].cond,cpog[j][i].fun[c]);
								strcat(line,string);
							}

							if(decide(cpog[j][i].fun[c]) && decide(cpog[j][j].fun[c]) && !decide(cpog[j][j].fun_cond[c])){
								sprintf(string,"(ACK_%s + !((%s) * (%s)))",cpog[j][i].source,cpog[j][j].fun_cond[c],cpog[j][j].cond);
								strcat(line,string);
							}

							if(!decide(cpog[j][i].fun[c]) && !decide(cpog[j][j].fun[c]) && decide(cpog[j][j].fun_cond[c])){
								sprintf(string,"(ACK_%s + !((%s) * (%s)))",cpog[j][i].source,cpog[j][j].fun[c],cpog[j][i].fun[c]);
								strcat(line,string);
							}

							if(decide(cpog[j][i].fun[c]) && !decide(cpog[j][j].fun[c]) && decide(cpog[j][j].fun_cond[c])){
								sprintf(string,"(ACK_%s + !(%s))",cpog[j][i].source,cpog[j][j].fun[c]);
								strcat(line,string);
							}
							if(!decide(cpog[j][i].fun[c]) && decide(cpog[j][j].fun[c]) && decide(cpog[j][j].fun_cond[c])){
								sprintf(string,"(ACK_%s + !(%s))",cpog[j][i].source,cpog[j][i].fun[c]);
								strcat(line,string);
							}

							if(decide(cpog[j][i].fun[c]) && decide(cpog[j][j].fun[c]) && decide(cpog[j][j].fun_cond[c])){
								ins = TRUE;
								sprintf(string,"(ACK_%s)",cpog[j][i].source);
								strcat(line,string);
							}
						}else{
							if(ins){
								sprintf(string," * ");
								strcat(line,string);
							}

							if (!decide(cpog[j][i].fun[c]) || !decide(cpog[j][j].fun[c]))
								ins = TRUE;

							if(!decide(cpog[j][i].fun[c]) && !decide(cpog[j][j].fun[c])){
								sprintf(string,"(ACK_%s + !((%s) * (%s)))",cpog[j][i].source,cpog[j][i].fun[c], cpog[j][j].fun[c]);
								strcat(line,string);
							}

							if(decide(cpog[j][i].fun[c]) && !decide(cpog[j][j].fun[c])){
								sprintf(string,"(ACK_%s + !(%s))",cpog[j][i].source, cpog[j][j].fun[c]);
								strcat(line,string);
							}

							if(!decide(cpog[j][i].fun[c]) && decide(cpog[j][j].fun[c])){
								sprintf(string,"(ACK_%s + !(%s))",cpog[j][i].source,cpog[j][i].fun[c]);
								strcat(line,string);
							}

							if(decide(cpog[j][i].fun[c]) && decide(cpog[j][j].fun[c])){
								ins = TRUE;
								sprintf(string,"(ACK_%s)",cpog[j][i].source);
								strcat(line,string);
							}
						}
					}
				}
			}
			//printf("\n");
			strcat(line,";\n");
			if(line[0] == ';')
				strcpy(line,"1;\n");
			fprintf(fp,"%s", line);
		}
		
		//CLOSING FILE AND FREE NAME MEMORY
		fclose(fp);

		/*ACCESSING TOOLS FOLDER*/
		if(ABCFLAG){

			if(chdir(ABC_PATH) != 0){
				printf("Error on cd command. %s\n", ABC_PATH);
				return 2;
			}
		

			/*BUILDING COMMAND CALLING ABC*/
#ifdef __linux
			strcpy(command,"./abc");
#else
			strcpy(command,"abc.exe");
#endif
			
			strcat(command, " < ");
			strcat(command, SCRIPT_PATH);

			if( (fp = fopen(SCRIPT_PATH,"w")) == NULL ){
				printf("Error on opening script file.\n");
				return 2;
			}

			/*WRITING DOWN SCRIPT FILE*/
			fprintf(fp,"read_eqn %s\n",file_name_abc);
			fprintf(fp,"read_library ");
			fprintf(fp,"%s", LIBRARY_FILE);
			fprintf(fp,"\n");
			fprintf(fp,"choice;map;ps\n");
			fprintf(fp,"choice;map;ps\n");
			fprintf(fp,"choice;map;ps\n");
			fprintf(fp,"choice;map;ps\n");
			fprintf(fp,"print_gates\n");
			fprintf(fp,"quit");
			fclose(fp);

			// DEBUG PRINTING
			/*printf("Calling ABC: %s\n",command);
			system(command);
			return 3;*/

			/*CALLING ABC*/
			if( (pp = popen(command,"r")) == NULL){
					printf("Error on calling abc program.\n");
					return 3;
			}

			/*READING RESULTS*/
			while(fgets(line,MAX_LINE,pp) != NULL){
				printf("%s\n",line);
				if(line[0] == 'T'){
					sscanf(line,"%s%s%s%d%s%s%f", string, name, dump1, &k,dump2,dump3, &k2);
					gates[c] = k;
					area[c] = k2;
					printf("%d %f\n", gates[c],area[c]);
				}
			}
			free(file_name);
			fclose(pp);

			/*GET BACK STARTING POSITION*/
			if(chdir(CURRENT_PATH) != 0){
				printf("Error on cd command. %s\n",CURRENT_PATH);
				return 2;
			}
		}

#ifdef ACT_PERCENTAGE	
		/*PRINTING PERCENTAGE ELAPSED TO COMPLETION*/
		act = c;
		total = counter;
		res = (int)((act/total) * 100);
		if(res != res_back){
			printf("%d - ", (int)res);
			res_back = res;
		}
		fflush(stdout);
#endif
	}

	//REMOVE SCRIPT FILES
	if (ABCFLAG){
#ifdef __linux
		strcpy(command,"rm -f ");
#else
		strcpy(command,"del ");
#endif
		
		strcat(command, SCRIPT_PATH);
		if(system(command) != 0){
			printf("Error on removing script file.\n");
			return 3;
		}
	}

#ifdef ACT_PERCENTAGE	
	printf("100\n");
#endif
	return 0;
}

/*WRITE CONDITIONS FUNCTION*/
/*Following function is in charge of preparing file for abc tool, if encodings with don't care
conditions occur. It sets for all the possible bits configuration '-' don't care condition.*/
void write_conditions(FILE *fp,int cpog_count,int i,int p,int q,int bits,int co){
	int j,k;
	boolean ins = FALSE;
	char *number;

	if(!DC){
		for(j=0;j<tot_enc;j++){
			ins = FALSE;
			for(k = 0;k<cpog_count;k++){
				if(j == cons_perm[i][k]){
					print_binary(fp,cons_perm[i][k], bits); /*Input encodings*/
					fprintf(fp," ");
					if(co)
						fprintf(fp,"%c",cpog[p][q].truth_cond[k]);
					else
						fprintf(fp, "%c",cpog[p][q].truth[k]);
					fprintf(fp,"\n");
					ins = TRUE;
				}
			}
			if (!ins){
				print_binary(fp,j, bits); /*Input encodings*/
				fprintf(fp," ");
				fprintf(fp,"-");
				fprintf(fp,"\n");
			}
		}
	}else{
		if(!SET){
			for(j=0;j<tot_enc;j++){
				ins = FALSE;
				print_binary(NULL,j, bits);
				number = numb;
				/*MOD !ins not present before*/
				for(k = 0;k<cpog_count;k++)
					if(!strDCcmp(number,manual_file[k],bits)){
						print_binary(fp,j, bits); /*Input encodings*/
						fprintf(fp," ");
						if(co)
							fprintf(fp,"%c",cpog[p][q].truth_cond[k]);
						else
							fprintf(fp, "%c",cpog[p][q].truth[k]);
						fprintf(fp,"\n");
						ins = TRUE;
					}
				if (!ins){
					print_binary(fp,j, bits); /*Input encodings*/
					fprintf(fp," ");
					fprintf(fp,"-");
					fprintf(fp,"\n");
				}
			
			}
		}else{
			for(j=0;j<tot_enc;j++){
				ins = FALSE;
				print_binary(NULL,j, bits);
				number = numb;
				/*MOD !ins not present before*/
				for(k = 0;k<cpog_count;k++){
					if(!DC_custom[k]){
						if(j == cons_perm[i][k]){
							print_binary(fp,cons_perm[i][k], bits); /*Input encodings*/
							fprintf(fp," ");
							if(co)
								fprintf(fp,"%c",cpog[p][q].truth_cond[k]);
							else
								fprintf(fp, "%c",cpog[p][q].truth[k]);
							fprintf(fp,"\n");
							ins = TRUE;
						}
					}else{
						char str[MAX_NAME];							
						int_to_string_DC(bits,k,cons_perm[i][k], str);
						//printf("manual: %s - str: %s\n",manual_file[k],str);

						if(!strDCcmp(number,str,bits)){
							print_binary(fp,j, bits); /*Input encodings*/
							fprintf(fp," ");
							if(co)
								fprintf(fp,"%c",cpog[p][q].truth_cond[k]);
							else
								fprintf(fp, "%c",cpog[p][q].truth[k]);
							fprintf(fp,"\n");
							ins = TRUE;
						}
					}
				}
				if (!ins){
					print_binary(fp,j, bits); /*Input encodings*/
					fprintf(fp," ");
					fprintf(fp,"-");
					fprintf(fp,"\n");
				}
			
			}
		}
	}
	return;
}

int equations_abc_cpog_size(int cpog_count, int bits){
	int i,c,k,j;
	float k2;
	FILE *fp = NULL, *pp = NULL;
	char *file_name = NULL,*file_name_abc = NULL, *string, *line, command[MAX_LINE],name[MAX_LINE];
	char dump1[MAX_NAME],dump2[MAX_NAME],dump3[MAX_NAME];
	
#ifdef ACT_PERCENTAGE
	float act,total,res,res_back = 101;
	printf("Percentage till complete:\n");
#endif

	line = (char*) malloc(sizeof(char) * MAX_LINE);
	string = (char*) malloc(sizeof(char) * MAX_NAME);

	for(c=0;c<counter;c++){
		//DEFINE FILE NAMES
		file_name = (char*) malloc(sizeof(char) * MAX_LINE);
		strcpy(file_name,FOLDER_NAME);
#ifdef __linux
		strcat(file_name,"/");
		sprintf(string,"fileXXXXXX");
		if (mkstemp(string) == -1){
			printf(".error \n");
			printf("Error on opening creating temporary file name in equation_abc: %s.\n", string);
			printf(".end_error \n");
			removeTempFiles();
			return 1;
		}
#else
		strcat(file_name,"\\");
		tmpnam (string);
#endif
		strcat(file_name, string);
		strcat(file_name, EXTENSION_ENCODING);

		file_name_abc = (char*) malloc(sizeof(char) * MAX_LINE);
		
		strcpy(file_name_abc,FOLDER_NAME);		
#ifdef __linux
		strcat(file_name_abc,"/");
#else
		strcat(file_name_abc,"\\");
#endif
		strcat(file_name_abc, string);
		strcat(file_name_abc, EXTENSION_ENCODING);

		//OPNENING FILE
		if( (fp = fopen(file_name,"w")) == NULL ){
			printf("NAME FILE: %s\n", file_name);
			printf("Error on opening file.\n");
			return 1;
		}


		//INPUT NAMES
		fprintf(fp,"INORDER =");
		for(k=0;k<bits;k++)
			fprintf(fp," x_%d",k);
		for(k=0;k<n_cond;k++)
			fprintf(fp," %s",name_cond[k]);
		fprintf(fp,";\n");

		//OUTPUT NAMES
		fprintf(fp,"OUTORDER =");
		for(k=0;k<nv;k++)
			for(j=0; j<nv; j++)
				if(cpog[k][j].type == 'v'){
					if((cpog[k][j].fun[c][0] != '0' && cpog[k][j].fun[c][0] != '1') || cpog[k][j].condition){
						fprintf(fp," %s",cpog[k][j].source);
					}
				} else {
					if(cpog[k][j].fun[c][0] != '0' && cpog[k][j].fun[c][0] != '1'){
						fprintf(fp, " %s->%s", cpog[k][j].source, cpog[k][j].dest);
					}
				}
		fprintf(fp,";\n");



		//FUNCTIONS FOR CPOG MINIMISATION
		for(i=0;i<nv; i++){
			for(j=0; j<nv; j++){
				if(cpog[i][j].type == 'v'){
					if(cpog[i][j].condition)
						fprintf(fp,"%s = (%s) + ((%s) * (%s));\n",cpog[i][j].source,cpog[i][j].fun[c], cpog[i][j].cond,cpog[i][j].fun_cond[c]);
					else
						if(cpog[i][j].fun[c][0] != '0' && cpog[i][j].fun[c][0] != '1')
							fprintf(fp,"%s = (%s);\n",cpog[i][j].source,cpog[i][j].fun[c]);
				}
				else
					if(cpog[i][j].fun[c][0] != '0' && cpog[i][j].fun[c][0] != '1')
						fprintf(fp, "%s->%s = (%s);\n", cpog[i][j].source, cpog[i][j].dest, cpog[i][j].fun[c]);
			}
		}
		
		//CLOSING FILE AND FREE NAME MEMORY
		fclose(fp);

		if(ABCFLAG){
			/*ACCESSING TOOLS FOLDER*/
			if(chdir(ABC_PATH) != 0){
				printf("Error on cd command. %s\n", ABC_PATH);
				return 2;
			}
		

			/*BUILDING COMMAND CALLING ABC*/
#ifdef __linux
			strcpy(command,"./abc");
#else
			strcpy(command,"abc.exe");
#endif
			strcat(command, " < ");
			strcat(command, SCRIPT_PATH);

			if( (fp = fopen(SCRIPT_PATH,"w")) == NULL ){
				printf("Error on opening script file.\n");
				return 2;
			}

			/*WRITING DOWN SCRIPT FILE*/
			fprintf(fp,"read_eqn %s\n",file_name_abc);
			fprintf(fp,"read_library ");
			fprintf(fp, "%s", LIBRARY_FILE);
			fprintf(fp,"\n");
			fprintf(fp,"choice;map;ps\n");
			fprintf(fp,"choice;map;ps\n");
			fprintf(fp,"choice;map;ps\n");
			fprintf(fp,"choice;map;ps\n");
			fprintf(fp,"print_gates\n");
			fprintf(fp,"quit");
			fclose(fp);

			// DEBUG PRINTING
			/*printf("Calling ABC: %s\n",command);
			system(command);
			return 3;*/

			/*CALLING ABC*/
			if( (pp = popen(command,"r")) == NULL){
					printf("Error on calling abc program.\n");
					return 3;
			}

			/*READING RESULTS*/
			while(fgets(line,MAX_LINE,pp) != NULL){
				//printf("%s\n",line);
				if(line[0] == 'T'){
					sscanf(line,"%s%s%s%d%s%s%f", string, name, dump1, &k,dump2,dump3, &k2);
					gates[c] = k;
					area[c] = k2;
					printf("%d %f\n", gates[c],area[c]);
				}
			}
			free(file_name);
			fclose(pp);

			/*GET BACK STARTING POSITION*/
			if(chdir(CURRENT_PATH) != 0){
				printf("Error on cd command. %s\n",CURRENT_PATH);
				return 2;
			}
	}

#ifdef ACT_PERCENTAGE	
		/*PRINTING PERCENTAGE ELAPSED TO COMPLETION*/
		act = c;
		total = counter;
		res = (int)((act/total) * 100);
		if(res != res_back){
			printf("%d - ", (int)res);
			res_back = res;
		}
		fflush(stdout);
#endif
	}

	if(ABCFLAG){
		//REMOVE SCRIPT FILES
#ifdef __linux
			strcpy(command,"rm -f ");
#else
			strcpy(command,"del ");
#endif
		strcat(command, SCRIPT_PATH);
		if(system(command) != 0){
			printf("Error on removing script file.\n");
			return 3;
		}
	}

#ifdef ACT_PERCENTAGE	
	printf("100\n");
#endif
	return 0;
}

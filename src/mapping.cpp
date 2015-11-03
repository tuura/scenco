#ifdef __linux
	#include "mapping.h"
#else
	#include "D:\Projects\PRGM_WORKCRAFT\inc\mapping.h"
#endif

// disable warnings about constant chars
#pragma GCC diagnostic ignored "-Wwrite-strings"

/*FINAL EQUATIONS AND ABC FUNCTION*/
/*Following function write in some file in a format compatible with abc tool (Developed by Berkeley
University), in order to get statistics about area of encoder. Moreover it calls abc tool.*/
int equations_abc(int cpog_count, int bits){
	int i,c,k,j,min_bits = 1;
	float k2;
	boolean ins = FALSE;
	FILE *fp = NULL, *pp = NULL;
	char *file_name = NULL, *string, *line, *abc_line;
	char *command,name[FILENAME_LENGTH];
	char dump1[FILENAME_LENGTH],dump2[FILENAME_LENGTH],dump3[FILENAME_LENGTH];
	
#ifdef ACT_PERCENTAGE
	float act,total,res,res_back = 101;
	printf("Percentage till complete:\n");
#endif

	abc_line = (char*) malloc(sizeof(char) * ABC_LINELENGTH);

	if(decode_flag)
		min_bits = logarithm2(cpog_count);

	for(c=0;c<counter;c++){
		//DEFINE FILE NAMES
		file_name = strdup("");
		file_name = catMem(file_name, FOLDER_NAME);
#ifdef __linux
		
		file_name = catChar(file_name, '/');
		string = strdup("abc_synthXXXXXX");
		if (mkstemp(string) == -1){
			printf(".error \n");
			printf("Error on opening creating temporary file name in equation_abc: %s.\n", string);
			printf(".end_error \n");
			removeTempFiles();
			return 1;
		}
		command = strdup("rm -f ");
		command = catMem(command, string);
		if(system(command) != 0){
			printf("Error on removing temporary dummy file.\n");
			return 3;
		}
		free(command);
#else
		file_name = catChar(file_name, "\\");
		string = (char*) malloc(sizeof(char) * FILENAME_LENGTH);
		tmpnam (string);
#endif
		file_name = catMem(file_name, string);
		line = strdup(EXTENSION_ENCODING);
		file_name = catMem(file_name, line);
		free(line);
		free(string);

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
			line = strdup("");
			for(j=0;j<nv;j++){
				/*DEBUG PRINTING: Information about and*/
				/*if(i == 1)
					printf("%d",ins);*/
				string = strdup("");
				//CONDITIONS OF VERTICES
				if(j == i){
					if(cpog[j][i].condition){
						if( (!decide(cpog[j][i].fun[c]) || !decide(cpog[j][i].fun_cond[c])) && ins){
							string = strdup(" * ");
							line = catMem(line, string);
						}
						if (!decide(cpog[j][i].fun[c]) || !decide(cpog[j][i].fun_cond[c]))
							ins = TRUE;
						if(!decide(cpog[j][i].fun_cond[c]) && !decide(cpog[j][i].fun[c])){
							string = strdup("( ( (");
							string = catMem(string, cpog[j][i].fun_cond[c]);
							string = catMem(string, ") * (");
							string = catMem(string, cpog[j][i].cond);
							string = catMem(string, ") ) + (");
							string = catMem(string, cpog[j][i].fun[c]);
							string = catMem(string, ")) ");
							line = catMem(line, string);
						}
						if(decide(cpog[j][i].fun_cond[c]) && !decide(cpog[j][i].fun[c])){
							string = strdup("( (");
							string = catMem(string, cpog[j][i].cond);
							string = catMem(string, ") + (");
							string = catMem(string, cpog[j][i].fun[c]);
							string = catMem(string, ") ) ");
							line = catMem(line, string);
						}
						if(!decide(cpog[j][i].fun_cond[c]) && decide(cpog[j][i].fun[c])){
							string = strdup("( (");
							string = catMem(string, cpog[j][i].cond);
							string = catMem(string, ") * (");
							string = catMem(string, cpog[j][i].fun_cond[c]);
							string = catMem(string, ") ) ");
							line = catMem(line, string);
						}
						if(decide(cpog[j][i].fun_cond[c]) && decide(cpog[j][i].fun[c]))
							ins = FALSE;
					}else{	
						if(!decide(cpog[j][i].fun[c]) && ins){
							string = strdup(" * ");
							line = catMem(line, string);
						}			
						if(!decide(cpog[j][i].fun[c])){
							ins = TRUE;
							string = strdup("(");
							string = catMem(string, cpog[j][i].fun[c]);
							string = catMem(string, ") ");
							line = catMem(line, string);
						}
					}
				//CONDITIONS OF EDGES
				}else{
					if(decide(cpog[j][i].fun[c]) != 2){
						if(cpog[j][j].condition){
							if( (!decide(cpog[j][i].fun[c]) || !decide(cpog[j][j].fun[c]) || decide(cpog[j][j].fun_cond[c]) != 3) && ins){
								string = strdup(" * ");
								line = catMem(line, string);
							}
							if (!decide(cpog[j][i].fun[c]) || !decide(cpog[j][j].fun[c]) || decide(cpog[j][j].fun_cond[c]) != 3)
								ins = TRUE;

							if(!decide(cpog[j][i].fun[c]) && !decide(cpog[j][j].fun[c]) && !decide(cpog[j][j].fun_cond[c])){
								string = strdup("(ACK_");
								string = catMem(string, cpog[j][i].source);
								string = catMem(string, " + !((((");
								string = catMem(string, cpog[j][j].fun_cond[c]);
								string = catMem(string, ") * (");
								string = catMem(string, cpog[j][j].cond);
								string = catMem(string, ")) + (");
								string = catMem(string, cpog[j][j].fun[c]);
								string = catMem(string, ")) * (");
								string = catMem(string, cpog[j][i].fun[c]);
								string = catMem(string, ")))");
								line = catMem(line, string);
							}

							if(decide(cpog[j][i].fun[c]) && !decide(cpog[j][j].fun[c]) && !decide(cpog[j][j].fun_cond[c])){
								string = strdup("(ACK_");
								string = catMem(string, cpog[j][i].source);
								string = catMem(string, " + !(((");
								string = catMem(string, cpog[j][j].fun_cond[c]);
								string = catMem(string, ") * (");
								string = catMem(string, cpog[j][j].cond);
								string = catMem(string, ")) + (");
								string = catMem(string, cpog[j][j].fun[c]);
								string = catMem(string, ")))");
								line = catMem(line, string);
							}

							if(!decide(cpog[j][i].fun[c]) && decide(cpog[j][j].fun[c]) && !decide(cpog[j][j].fun_cond[c])){
								string = strdup("(ACK_");
								string = catMem(string, cpog[j][i].source);
								string = catMem(string, " + !(((");
								string = catMem(string, cpog[j][j].fun_cond[c]);
								string = catMem(string, ") * (");
								string = catMem(string, cpog[j][j].cond);
								string = catMem(string, ")) * (");
								string = catMem(string, cpog[j][i].fun[c]);
								string = catMem(string, ")))");
								line = catMem(line, string);
							}

							if(decide(cpog[j][i].fun[c]) && decide(cpog[j][j].fun[c]) && !decide(cpog[j][j].fun_cond[c])){
								string = strdup("(ACK_");
								string = catMem(string, cpog[j][i].source);
								string = catMem(string, " + !((");
								string = catMem(string, cpog[j][j].fun_cond[c]);
								string = catMem(string, ") * (");
								string = catMem(string, cpog[j][j].cond);
								string = catMem(string, ")))");
								line = catMem(line, string);
							}

							if(!decide(cpog[j][i].fun[c]) && !decide(cpog[j][j].fun[c]) && decide(cpog[j][j].fun_cond[c])){
								string = strdup("(ACK_");
								string = catMem(string, cpog[j][i].source);
								string = catMem(string, " + !((");
								string = catMem(string, cpog[j][j].fun[c]);
								string = catMem(string, ") * (");
								string = catMem(string, cpog[j][i].fun[c]);
								string = catMem(string, ")))");
								line = catMem(line, string);
							}

							if(decide(cpog[j][i].fun[c]) && !decide(cpog[j][j].fun[c]) && decide(cpog[j][j].fun_cond[c])){
								string = strdup("(ACK_");
								string = catMem(string, cpog[j][i].source);
								string = catMem(string, " + !(");
								string = catMem(string, cpog[j][j].fun[c]);
								string = catMem(string, "))");
								line = catMem(line, string);
							}
							if(!decide(cpog[j][i].fun[c]) && decide(cpog[j][j].fun[c]) && decide(cpog[j][j].fun_cond[c])){
								string = strdup("(ACK_");
								string = catMem(string, cpog[j][i].source);
								string = catMem(string, " + !(");
								string = catMem(string, cpog[j][i].fun[c]);
								string = catMem(string, "))");
								line = catMem(line, string);
							}

							if(decide(cpog[j][i].fun[c]) && decide(cpog[j][j].fun[c]) && decide(cpog[j][j].fun_cond[c])){
								ins = TRUE;
								string = strdup("(ACK_");
								string = catMem(string, cpog[j][i].source);
								string = catChar(string, ')');
								line = catMem(line, string);
							}
						}else{
							if(ins){
								string = strdup(" * ");
								line = catMem(line, string);
							}

							if (!decide(cpog[j][i].fun[c]) || !decide(cpog[j][j].fun[c]))
								ins = TRUE;

							if(!decide(cpog[j][i].fun[c]) && !decide(cpog[j][j].fun[c])){
								string = strdup("(ACK_");
								string = catMem(string, cpog[j][i].source);
								string = catMem(string, " + !((");
								string = catMem(string, cpog[j][i].fun[c]);
								string = catMem(string, ") * (");
								string = catMem(string, cpog[j][j].fun[c]);
								string = catMem(string, ")))");
								line = catMem(line, string);
							}

							if(decide(cpog[j][i].fun[c]) && !decide(cpog[j][j].fun[c])){
								string = strdup("(ACK_");
								string = catMem(string, cpog[j][i].source);
								string = catMem(string, " + !(");
								string = catMem(string, cpog[j][j].fun[c]);
								string = catMem(string, "))");
								line = catMem(line, string);
							}

							if(!decide(cpog[j][i].fun[c]) && decide(cpog[j][j].fun[c])){
								string = strdup("(ACK_");
								string = catMem(string, cpog[j][i].source);
								string = catMem(string, " + !(");
								string = catMem(string, cpog[j][i].fun[c]);
								string = catMem(string, "))");
								line = catMem(line, string);
							}

							if(decide(cpog[j][i].fun[c]) && decide(cpog[j][j].fun[c])){
								ins = TRUE;
								string = strdup("(ACK_");
								string = catMem(string, cpog[j][i].source);
								string = catChar(string, ')');
								line = catMem(line, string);
							}
						}
					}
				}
				free(string);
			}
			//printf("\n");
			line = catChar(line, ';');
			if(line[0] == ';'){
				free(line);
				line = strdup("1;");
			}
			fprintf(fp,"%s\n", line);
			free(line);
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
			command = strdup("./abc");
#else
			command = strdup("abc.exe");
#endif
			
			command = catMem(command, " < ");
			command = catMem(command, SCRIPT_PATH);

			if( (fp = fopen(SCRIPT_PATH,"w")) == NULL ){
				printf("Error on opening script file.\n");
				return 2;
			}

			/*WRITING DOWN SCRIPT FILE*/
			fprintf(fp,"read_eqn %s\n",file_name);
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
			string = (char*) malloc(sizeof(char) * ABC_LINELENGTH);
			while(fgets(abc_line,ABC_LINELENGTH,pp) != NULL){
				//printf("%s\n",abc_line);
				sscanf(abc_line, "%s", string);
				if( !strcmp(string, "TOTAL") ){
					sscanf(abc_line,"%s%s%s%d%s%s%f", string, name, dump1, &k,dump2,dump3, &k2);
					gates[c] = k;
					area[c] = k2;
				}
			}
			fclose(pp);
			free(string);
			free(command);

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
		command = strdup("rm -f ");
#else
		command = strdup("del ");
#endif
		
		command = catMem(command,SCRIPT_PATH);
		if(system(command) != 0){
			printf("Error on removing script file.\n");
			return 3;
		}
		free(command);
	}

	free(abc_line);

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
	char *file_name = NULL, string[FILENAME_LENGTH], *abc_line, *s;
	char *command,name[FILENAME_LENGTH];
	char dump1[FILENAME_LENGTH],dump2[FILENAME_LENGTH],dump3[FILENAME_LENGTH];
	
#ifdef ACT_PERCENTAGE
	float act,total,res,res_back = 101;
	printf("Percentage till complete:\n");
#endif

	abc_line = (char*) malloc(sizeof(char) * ABC_LINELENGTH);

	for(c=0;c<counter;c++){
		//DEFINE FILE NAMES
		file_name = strdup(FOLDER_NAME);
#ifdef __linux
		file_name = catChar(file_name,'/');
		strcpy(string, "abc_synth.XXXXXX");
		if (mkstemp(string) == -1){
			printf(".error \n");
			printf("Error on opening creating temporary file name in equation_abc: %s.\n", string);
			printf(".end_error \n");
			removeTempFiles();
			return 1;
		}
		command = strdup("rm -f ");
		command = catMem(command, string);
		if(system(command) != 0){
			printf("Error on removing temporary dummy file.\n");
			return 3;
		}
		free(command);
#else
		file_name = catChar(file_name,'\\');
		tmpnam (string);
#endif
		file_name = catMem(file_name, string);
		s = strdup(EXTENSION_ENCODING);
		file_name = catMem(file_name, s);
		free(s);

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
			command = strdup("./abc");
#else
			command = strdup("abc.exe");
#endif
			command = catMem(command, " < ");
			command = catMem(command, SCRIPT_PATH);

			if( (fp = fopen(SCRIPT_PATH,"w")) == NULL ){
				printf("Error on opening script file.\n");
				return 2;
			}

			/*WRITING DOWN SCRIPT FILE*/
			fprintf(fp,"read_eqn %s\n",file_name);
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
			while(fgets(abc_line,ABC_LINELENGTH,pp) != NULL){
				//printf("%s\n",abc_line);
				sscanf(abc_line, "%s", string);
				if( !strcmp(string, "TOTAL") ){
					sscanf(abc_line,"%s%s%s%d%s%s%f", string, name, dump1, &k,dump2,dump3, &k2);
					gates[c] = k;
					area[c] = k2;
				}
			}
			fclose(pp);
			free(command);

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
		free(file_name);
	}

	if(ABCFLAG){
		//REMOVE SCRIPT FILES
#ifdef __linux
		command = strdup("rm -f ");
#else
		command = strdup("del ");
#endif
		command = catMem(command, SCRIPT_PATH);
		if(system(command) != 0){
			printf("Error on removing script file.\n");
			return 3;
		}
		free(command);
	}

	free(abc_line);

#ifdef ACT_PERCENTAGE	
	printf("100\n");
#endif
	return 0;
}

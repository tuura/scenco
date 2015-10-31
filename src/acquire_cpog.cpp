#ifdef __linux
	#include "acquire_cpog.h"
#elif WIN32
	#include "D:\Projects\PRGM_WORKCRAFT\inc\acquire_cpog.h"
#endif

/*READ FILE FUNCTION*/
/*Following function read non-trivial encoding constraints of the Conditional Partial Order Graphs.*/
int read_file(char *file_in,int *cpog_count, int *len_sequence){
	FILE *fp = NULL;
	char string[MAX_CPOG], dump;
	int i = 0, j = 0;

	fp = fopen(file_in, "r");
	if( fscanf(fp,"%s", string) == EOF){
		printf("File is empty. Please, introduce another file.\n");
		return 1;
	}
	*len_sequence = 1;
	(*cpog_count) = strlen(string);
	
	while(fscanf(fp,"%s", string) != EOF)
		(*len_sequence)++;
	fclose(fp);

	/*DEBUG PRINTING*/
	//printf("%d %d\n", *cpog_count, *len_sequence);

	diff = (char**) malloc(sizeof(char*) * (*len_sequence));
	for(i=0;i<(*len_sequence);i++)
		diff[i] = (char*) malloc(sizeof(char) * (*cpog_count));

	fp = fopen(file_in, "r");
	for(i = 0; i< (*len_sequence);i++){
		for(j = 0; j< (*cpog_count); j++){
			if(fscanf(fp, "%c", &diff[i][j]) == EOF){
				printf("Error on reading custom encodings.\n");
				return 3;
			}
		}
		j = fscanf(fp,"%c", &dump);
	}
	fclose(fp);

	if(verbose){
		/*NON-TRIVIAL ENCODING PRINTING*/
		printf("NON-TRIVIAL ENCODING:\n");
		for(i = 0; i <(*len_sequence) ;i++){
			for(j = 0; j< (*cpog_count); j++)
				printf("%c", diff[i][j]);
			printf("\n");
		}
	}
	return 0;
}

/*DIFFERENCE MATRIX FUNCTION*/
/*Following function computes differences among CPOG, and store them inside
a matrix.*/
int difference_matrix(int cpog_count, int len_sequence){
	int i = 0, j = 0, k = 0;
	
	/*COMPUTING DIFFERENCES AMONG CPOG*/
	for(i=0;i<cpog_count -1;i++)
		for(j=i+1;j<cpog_count;j++)
			for(k=0;k<len_sequence;k++)
				if((diff[k][i] == '0' && diff[k][j] == '1') || (diff[k][i] == '1' && diff[k][j] == '0'))
					opt_diff[i][j]++;
	
	/*OPTIMAL DIFFERENCES MATRIX PRINTING*/
	if(verbose){
		printf("\nOPTIMAL DIFFERENCES MATRIX:\n");
		for(i=0;i<cpog_count;i++){
			for(j=0;j<cpog_count;j++)
				printf("%2d ", opt_diff[i][j]);
			printf("\n");
		}
		printf("\n");
	}
	return 0;
}

/*READ_CONSTRAINTS FUNCTION*/
/*Following function read encoding constraints of every vertex and edge of CPOG*/
int read_cons(char *file_cons,int cpog_count,int *num_vert){
	FILE *fp = NULL;
	char string[MAX_LINE], name[MAX_NAME];
	int i = 0, j = 0, len = 0, v_tmp = 0, n_tmp = 0, p = 0;
	boolean ins; 

	if( (fp = fopen(file_cons, "r")) == NULL){
		printf("Error on opening constraints file.\n");
		return 1;

	}
	if( fscanf(fp,"%s", string) == EOF){
		printf("File is empty. Please, introduce another file.\n");
		return 1;
	}
	if(fclose(fp) != 0){
		printf("Error on closing constraints file.\n");
		return 1;
	}
	fp = fopen(file_cons, "r");
	while( fgets(string, MAX_LINE, fp) != NULL){
		len = strlen(string);
		for(i = (cpog_count + 8); i<len; i++){

			switch(string[i]){
				case ' ':
					/*Parsing space characters*/
					break;
				case '(':
					/*Parsing edges*/
					while(string[i] != ')') i++;
					i++;
					break;
				default:
					/*Parsing vertices name*/
					n_tmp = 0;
					while( (!isspace(string[i])) && string[i] != ':' && string[i] != EOF){
						name[n_tmp++] = string[i++];
					}
					name[n_tmp] = '\0';
					if(string[i] == ':')
						while( string[i] != ' ' && string[i] != EOF){
					/*Taking into account nested conditions on vertices*/
							if(string[i] == '('){
								p++;
								i++;
								while(p){
									while(string[i] != ')'){
										if(string[i] == '(') p++;
										i++;
									}
									p--;
									i++;
								}
							}else
								i++;
						}
					ins = TRUE;
					for(j=0;j<v_tmp;j++)
						if(strcmp(name, vertices[j]) == 0)
							ins = FALSE;
					if(ins){
						(*num_vert)++;
						strcpy(vertices[v_tmp++], name);
						/*DEBUG PRINTING: adding vertex*/
						//printf("%s inserted\n", name);
					}
					while((!isspace(string[i])) && string[i] != EOF)
						i++;
					break;
			}
		}
	}
	fclose(fp);

	return 0;
}

/*ACQUIRE CPOG FUNCTION*/
/*Following function read again constraints file in order to definetly acquire CPOG.*/
void parsing_cpog(char* file_cons,int cpog_count,int num_vert){
	FILE *fp = NULL;
	char string[MAX_LINE], name[MAX_NAME];
	char source[MAX_NAME], dest[MAX_NAME], *truth, cond[MAX_NAME];
	int i = 0, j = 0,k=0, len = 0, n_tmp = 0, p = 0;
	boolean ins, condit;

	truth = (char*) malloc(sizeof(char) * (cpog_count + 1));
	fp = fopen(file_cons, "r");
	while( fgets(string, MAX_LINE, fp) != NULL){
		sscanf(string,"%s",truth);
		
		/*DEBUG PRINTING: reading truth table*/
		//printf("%s\n", truth);

		len = strlen(string);
		
		for(i = (cpog_count + 8); i<len; i++){
			switch(string[i]){
				case ' ':
					/*Parsing space characters*/
					break;
				case '(':
					/*PARSING EDGES*/
					i++;
					/*PARSING SOURCE VERTEX*/
					n_tmp = 0;
					while((!isspace(string[i]))){
						source[n_tmp] = string[i];
						n_tmp++;
						i++;
					}
					source[n_tmp] = '\0';

					while(string[i] != '>') i++;
					i++;
					i++;
					
					/*PARSING DESTINATION VERTEX*/
					n_tmp = 0;
					while(string[i] != ')')
						dest[n_tmp++] = string[i++];
					dest[n_tmp] = '\0';
					i++;

					/*EDGE INFORMATION INSERTION*/
					ins = FALSE;
					for(j=0;j<num_vert && !ins;j++){
						fflush(stdout);
						if(strcmp(source, cpog[j][0].source) == 0)
							for(k=0;k<num_vert && !ins;k++){
								if(strcmp(dest, cpog[j][k].dest) == 0){
									ins = TRUE;
									cpog[j][k].truth = strdup(truth);
									cpog[j][k].cond = NULL;
									cpog[j][k].truth_cond = NULL;
								}
							}
					}
					break;
				default:
					/*PARSING VERTICES*/
					
					/*Parsing vertex name*/
					n_tmp = 0;
					while( (!isspace(string[i])) && string[i] != ':' && string[i] != EOF)
						name[n_tmp++] = string[i++];
					name[n_tmp] = '\0';
					
					/*Parsing condition of vertex*/
					condit = FALSE;
					if(string[i] == ':'){
						condit = TRUE;
						i++;
						n_tmp = 0;
						while( (!isspace(string[i])) && string[i] != EOF){
							/*Taking into account nested conditions on vertices*/
							if(string[i] == '('){
								p++;
								cond[n_tmp++] = string[i++];
								while(p){
									while(string[i] != ')'){
										if(string[i] == '(') p++;
										cond[n_tmp++] = string[i++];
									}
									p--;
									cond[n_tmp++] = string[i++];
								}
							}
							else
								cond[n_tmp++] = string[i++];
						}
						cond[n_tmp] = '\0';
						/*DEBUG PRINTING: current reading information*/
						//printf("condition: |%s| new char: |%c| len: %d, i: %d\n", cond, string[i],len,i);
					}

					/*VERTEX INFORMATION INSERTION*/
					ins = FALSE;
					for(j=0;j<num_vert && !ins;j++)
						if(strcmp(name, cpog[j][0].source) == 0){
							ins = TRUE;
							if (condit){
								cpog[j][j].condition = TRUE;
								cpog[j][j].cond = strdup(cond);
								cpog[j][j].truth_cond = strdup(truth);
							}
							else
								cpog[j][j].truth = strdup(truth);
						}
					break;
			}
		}
	}
	fclose(fp);

	return;
}

/*GET CONDITIONS NAMES FUNCTION*/
/*Following function parses CPOG representation in order to get names of the conditions present
in same vertices.*/
int get_conditions_names(){
	int i,k,j,p;
	boolean ins = TRUE;
	char name[MAX_NAME];
	
	for(i=0;i<nv; i++){
		if(cpog[i][i].condition){
			k = strlen(cpog[i][i].cond);
			j = 0;
			while(j<k){
				p = 0;
				switch(cpog[i][i].cond[j]){
					case '(':
						j++;
						break;
					case ' ':
						j++;
						break;
					case '!':
						j++;
						break;
					case ')':
						j++;
						break;
					case '+':
						j++;
						break;
					case '*':
						j++;
						break;
					case '\0':
						j = k;
						break;
					default:
						while( (!isspace(cpog[i][i].cond[j])) && cpog[i][i].cond[j] != '\0' && cpog[i][i].cond[j] != ')' && cpog[i][i].cond[j] != '+' && cpog[i][i].cond[j] != '*')
							name[p++] = cpog[i][i].cond[j++];
						name[p] = '\0';
						ins = TRUE;
						for(p=0;p<n_cond;p++)
							if(!strcmp(name_cond[p],name))
								ins = FALSE;
						if (ins)
							strcpy(name_cond[n_cond++],name);								
						break;
				}
			}
		}
	}
	return 0;
}

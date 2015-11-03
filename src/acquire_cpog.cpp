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

	/*NON-TRIVIAL ENCODING PRINTING*/
	// debug printing
	/*if(verbose){		
		printf("NON-TRIVIAL ENCODING:\n");
		for(i = 0; i <(*len_sequence) ;i++){
			for(j = 0; j< (*cpog_count); j++)
				printf("%c", diff[i][j]);
			printf("\n");
		}
	}*/
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
	// debug printing
	/*if(verbose){
		printf("\nOPTIMAL DIFFERENCES MATRIX:\n");
		for(i=0;i<cpog_count;i++){
			for(j=0;j<cpog_count;j++)
				printf("%2d ", opt_diff[i][j]);
			printf("\n");
		}
		printf("\n");
	}*/
	return 0;
}

/*READ_CONSTRAINTS FUNCTION*/
/*Following function read encoding constraints of every vertex and edge of CPOG*/
int read_cons(char *file_cons,int cpog_count,int *num_vert){
	FILE *fp = NULL;
	char *name, c;
	int i = 0, j = 0, v_tmp = 0, p = 0;
	boolean ins; 

	if( (fp = fopen(file_cons, "r")) == NULL){
		printf("Error on opening constraints file.\n");
		return 1;

	}
	if( feof(fp) ){
		printf("File is empty. Please, introduce another file.\n");
		return 1;
	}
	if(fclose(fp) != 0){
		printf("Error on closing constraints file.\n");
		return 1;
	}
	fp = fopen(file_cons, "r");
	while( !feof(fp) ){
		for(i = 0 ; i < cpog_count+8; i++) c = fgetc(fp);
		c = fgetc(fp);
		int h = 0;
		while( c != '\n' && c != EOF){
			if (h != 0) c = fgetc(fp);
			h++;

			switch(c){
				case ' ':
					/*Parsing space characters*/
					break;
				case '(':
					/*Parsing edges*/
					while( (c = fgetc(fp)) != ')');
					c = fgetc(fp);
					break;
				default:
					/*Parsing vertices name*/
					name = strdup("");
					while( (!isspace(c)) && c != ':' && c != EOF){
						name = catChar(name, c);
						c = fgetc(fp);
					}
					
					if(c == ':')
						while( c != ' ' && c != EOF){
					/*Taking into account nested conditions on vertices*/
							if(c == '('){
								p++;
								c = fgetc(fp);
								while(p){
									while(c != ')'){
										if(c== '(') p++;
										c = fgetc(fp);
									}
									p--;
									c = fgetc(fp);
								}
							}else
								c = fgetc(fp);
						}
					ins = TRUE;
					for(j=0;j<v_tmp;j++)
						if(strcmp(name, vertices[j]) == 0)
							ins = FALSE;
					if(ins){
						(*num_vert)++;
						vertices[v_tmp++] = strdup(name);
						/*DEBUG PRINTING: adding vertex*/
						//printf("%s inserted\n", name);
					}
					while((!isspace(c)) && c != EOF)
						c = fgetc(fp);
					free(name);
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
	char name[MAX_NAME], c;
	char source[MAX_NAME], dest[MAX_NAME], *truth, cond[MAX_NAME];
	int i = 0, j = 0,k=0, n_tmp = 0, p = 0;
	boolean ins, condit;

	truth = (char*) malloc(sizeof(char) * (cpog_count + 1));
	fp = fopen(file_cons, "r");
	while(  !feof(fp) ){
		if(fscanf(fp,"%s",truth) == 0){
			printf("Error on reading the truth table inside constraints file.\n");
			return;
		}
		
		/*DEBUG PRINTING: reading truth table*/
		//printf("%s\n", truth);

		
		for(i = 0 ; i < 8; i++) c = fgetc(fp);
		c = fgetc(fp);
		int h = 0;
		while( c != '\n' && c != EOF){
			if (h != 0) c = fgetc(fp);
			h++;

			switch(c){
				case ' ':
					/*Parsing space characters*/
					break;
				case '(':
					/*PARSING EDGES*/
					c = fgetc(fp);
					/*PARSING SOURCE VERTEX*/
					n_tmp = 0;
					while((!isspace(c))){
						source[n_tmp] = c;
						n_tmp++;
						c = fgetc(fp);
					}
					source[n_tmp] = '\0';

					while( (c = fgetc(fp)) != '>');
					c = fgetc(fp);
					c = fgetc(fp);
					
					/*PARSING DESTINATION VERTEX*/
					n_tmp = 0;
					while(c != ')'){
						dest[n_tmp++] = c;
						c = fgetc(fp);
					}
					dest[n_tmp] = '\0';
					c = fgetc(fp);

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
					while( (!isspace(c)) && c != ':' && c != EOF){
						name[n_tmp++] = c;
						c = fgetc(fp);
					}
					name[n_tmp] = '\0';
					
					/*Parsing condition of vertex*/
					condit = FALSE;
					if(c == ':'){
						condit = TRUE;
						c = fgetc(fp);
						n_tmp = 0;
						while( (!isspace(c)) && c != EOF){
							/*Taking into account nested conditions on vertices*/
							if(c == '('){
								p++;
								cond[n_tmp++] = c;
								c = fgetc(fp);
								while(p){
									while(c != ')'){
										if(c == '(') p++;
										cond[n_tmp++] = c;
										c = fgetc(fp);
									}
									p--;
									cond[n_tmp++] = c;
									c = fgetc(fp);
								}
							}
							else{
								cond[n_tmp++] = c;
								c = fgetc(fp);
							}
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
	char *name;
	
	for(i=0;i<nv; i++){
		if(cpog[i][i].condition){
			k = strlen(cpog[i][i].cond);
			j = 0;
			while(j<k){
				printf("DUP NAME WIth SPACE... ");
				fflush(stdout);
				name = strdup("");
				printf("DONE\n");
				fflush(stdout);
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
						while( (!isspace(cpog[i][i].cond[j])) &&
							cpog[i][i].cond[j] != '\0' && 
							cpog[i][i].cond[j] != ')' && 
							cpog[i][i].cond[j] != '+' && 
							cpog[i][i].cond[j] != '*'){
								printf("Before name: %s\n", name);
								fflush(stdout);
								name = catChar(name,cpog[i][i].cond[j++]);
								printf("After name: %s\n", name);
								fflush(stdout);
						}
						ins = TRUE;
						for(p=0;p<n_cond;p++)
							if(!strcmp(name_cond[p],name))
								ins = FALSE;
						if (ins)
							strcpy(name_cond[n_cond++],name);								
						break;
				}
				printf("FREE NAME... ");
				fflush(stdout);
				free(name);
				printf("DONE\n");
				fflush(stdout);
			}
		}
	}
	return 0;
}

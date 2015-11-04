#ifdef __linux
	#include "prgm_helper.h"
#else
	#include "D:\Projects\PRGM_WORKCRAFT\inc\prgm_helper.h"
#endif

/*PARSE ARGUMENTS FUNCTION*/
/*Following function parse arguments of called program.*/
int parse_arg(int argc, char *argv[]){
	int cur = 1, i, j;
	char supp[500];
	
	while(cur < argc){
		//VERBOSE MODE
		if(!strcmp(argv[cur], "-v")){
			verbose = TRUE;
		}
		//TUNE ENCODING MODE
		if(!strcmp(argv[cur], "-m")){
			if(cur+1 < argc){
				if(!strcmp(argv[cur+1], "max"))
					mode = 0;
				else if(!strcmp(argv[cur+1], "min"))
					mode = 1;
				else if(!strcmp(argv[cur+1], "all"))
					mode = 2;
				else if(!strcmp(argv[cur+1], "man")){
					mode = 3;
					if(cur+2 < argc){
						file_name = strdup(argv[cur+2]);
					}
					else{
						printf("Argument %s undefined. So far only recognized parameters are -m followed by:\n", argv[4]);
						return 1;
					}
				}
				else if(argc != 3){
					printf("man type must be followed by file with custom encoding.\n");
					return 2;
				}
			}
			else{
				printf("-m must be followed by:\n");
				printf("\tmin - for considering encodings minimising function;\n");
				printf("\tmax - for considering encodings maximising function;\n");
				printf("\tall - for considering all encodings.\n");
				printf("\tman - for setting a manual encoding by file.\n");
				return 3;
			}

		}
		//HELP OF PROGRAM
		if(!strcmp(argv[cur], "-h") || !strcmp(argv[cur], "-help")){
			print_help(argv[0]);
			return 4;
		}
		if(!strcmp(argv[cur], "-version")){
			print_version();
			return 4;
		}
		//UNFIX FIRST ENCODING
		if(!strcmp(argv[cur], "-u")){
			unfix = TRUE;
		}
		//RANDOM ENCODINGS
		if(!strcmp(argv[cur], "-r")){
			gen_mode = 0;
			if(cur + 1 < argc){
				if( atoi(argv[cur+1]) ){
					gen_perm = atoi(argv[cur+1]);
				}
			}
		}
		//CLEVER ENCODINGS GENERATION
		if(!strcmp(argv[cur], "-top")){
			gen_mode = 1;
			if(cur + 1 < argc){
				if( atoi(argv[cur+1]) ){
					gen_perm = atoi(argv[cur+1]);
				}
			}
		}
		//ALTERNATE HANDLING PREDICATES
		if (!strcmp(argv[cur], "-alt")){
			alternative = TRUE;
		}
		//SET ANY ENCODINGS
		if(!strcmp(argv[cur], "-set")){
			SET = TRUE;
			if( cur+1 < argc ){
				custom_file_name = strdup(argv[cur+1]);
			}
			else{
				printf("After -set the name of file contains custom encodings must be inserted.\n");
				return 6;
			}
		}
		//SET BIT LENGTH TO USE INSIDE PERMUTATION FUNCTIONS
		if(!strcmp(argv[cur], "-bit")){
			mod_bit_flag = TRUE;
			if(cur + 1 < argc){
				if( atoi(argv[cur+1]) ){
					mod_bit = atoi(argv[cur+1]);
				} else {
					printf("After -bit the number of bits to be using for the encoding should be inserted.\n");
					return 6;
				}
			}
		}
		//SET ESPRESSO PATH
#ifdef __linux
		if(!strcmp(argv[cur], "-e")){
			if( cur+1 < argc ){
				ESPRESSO_PATH = strdup(argv[cur+1]);
				i=0; j=0;

				// FIX THE PATH FOR __linux VERSION
				while(((unsigned int)i) < strlen(ESPRESSO_PATH)){
					if(ESPRESSO_PATH[i] != ' '){
						supp[j++] = ESPRESSO_PATH[i];
					}else{
						supp[j++] = '\\';
						supp[j++] = ' ';
					}
					i++;
				}
				supp[j] = '\0';

				strcpy(ESPRESSO_PATH,supp);
			}
			else{
				printf("After -e espresso's path must be inserted.\n");
				return 7;
			}
		}
#else
		if(!strcmp(argv[cur], "-e")){
			if( cur+1 < argc ){
				ESPRESSO_PATH = strdup(argv[cur+1]);
			}
			else{
				printf("After -e espresso's path must be inserted.\n");
				return 7;
			}
		}
#endif
		
		//SET RESULTS FOLDER PATH
#ifdef __linux
		if(!strcmp(argv[cur], "-res")){
			if( cur+1 < argc ){
				FOLDER_NAME = strdup(argv[cur+1]);
				i=0; j=0;

				// FIX THE PATH FOR __linux VERSION
				while(((unsigned int)i) < strlen(FOLDER_NAME)){
					if(FOLDER_NAME[i] != ' '){
						supp[j++] = FOLDER_NAME[i];
					}else{
						supp[j++] = '\\';
						supp[j++] = ' ';
					}
					i++;
				}
				supp[j] = '\0';

				strcpy(FOLDER_NAME,supp);

				// DEBUG PRINTING
				printf("FOLDER NAME TEMP: %s\n", FOLDER_NAME);
			}
			else{
				printf("After -res results folder's path must be inserted.\n");
				return 7;
			}
		}
#else
		if(!strcmp(argv[cur], "-res")){
			if( cur+1 < argc ){
				FOLDER_NAME = strdup(argv[cur+1]);
			}
			else{
				printf("After -res, result folder's path must be inserted.\n");
				return 7;
			}
		}
#endif
		
		//SET ABC FOLDER
		if(!strcmp(argv[cur], "-a")){
			ABCFLAG = TRUE;
			if( cur+1 < argc ){
				ABC_PATH = strdup(argv[cur+1]);

				/*i=0; j=0;

				// FIX THE PATH FOR __linux VERSION
				while(i < strlen(ABC_PATH)){
					if(ABC_PATH[i] != ' '){
						supp[j++] = ABC_PATH[i];
					}else{
						supp[j++] = '\\';
						supp[j++] = ' ';
					}
					i++;
				}
				supp[j] = '\0';

				strcpy(ABC_PATH,supp);*/
			}
			else{
				printf("After -a abc tool folder must be inserted.\n");
				return 8;
			}
		}
		//SET ANY ENCODINGS
#ifdef __linux
		if(!strcmp(argv[cur], "-lib")){
			if( cur+1 < argc ){
				LIBRARY_FILE = strdup(argv[cur+1]);

				i=0; j=0;

				// FIX THE PATH FOR __linux VERSION
				while(((unsigned int)i) < strlen(LIBRARY_FILE)){
					if(LIBRARY_FILE[i] != ' '){
						supp[j++] = LIBRARY_FILE[i];
					}else{
						supp[j++] = '\\';
						supp[j++] = ' ';
					}
					i++;
				}
				supp[j] = '\0';

				strcpy(LIBRARY_FILE,supp);
			}
			else{
				printf("After -lib gates library in genlib format must be inserted.\n");
				return 9;
			}
		}
#else
		if(!strcmp(argv[cur], "-lib")){
			if( cur+1 < argc ){
				LIBRARY_FILE = strdup(argv[cur+1]);
			}
			else{
				printf("After -lib gates library in genlib format must be inserted.\n");
				return 9;
			}
		}
#endif
		
		//OPTIMISE CPOG SIZE
		if (!strcmp(argv[cur], "-cs")){
			CPOG_SIZE = TRUE;
		}
		//DISABLE COST FUNCTION
		if (!strcmp(argv[cur], "-d")){
			DISABLE_FUNCTION = TRUE;
		}
		//USE OLD SYNTH OPCODES
		if (!strcmp(argv[cur], "-old")){
			OLD = TRUE;
		}
		cur++;
	}
	if(argc < 2){
		print_help(argv[0]);
		return 5;
	}
	else{
		file_in = strdup(argv[1]);
		//file_cons = strdup(argv[2]);
	}
	if(SET && mode == 3){
		printf("It is not allowed to use parameter -m man and -set at the same time.\n");
		return 6;
	}
	return 0;
}

void print_help(char *prog_name){
	//DOCUMENTATION
	printf("\nDocumentation: ");
	printf("%s is able to generate different kind of encodings for the CPOG graphs model and, it can\n",prog_name);
	printf("map encoding in logic gates by reading a library as well. It exploits an heuristic allows it\n");
	printf("to speed up encoding choice for minimising area used by decoder circuit.\n\n");
	//PROGRAM USAGE
	printf("Usage: %s cpog_graph.cpog -[a/alt/e/lib/m/h/r/top/u/v]\n\n",prog_name);
	printf("\t-a [abc_path]: set abc path.\n");
	printf("\t-alt: considering predicatives in a different way to build logic functions.\n");
	printf("\t-bit: [bit_length]: number of bits to be using for exhaustive search mode.\n");
	printf("\t-cs: optimise area for CPOG SIZE.\n");
	printf("\t-d: disable cost function, minimise directly the area.\n");
	printf("\t-e [espresso_path]: set espresso path.\n");
	printf("\t-res [result_path]: set result path.\n");
	printf("\t-lib [gates_library_path]: set library path.\n");
	printf("\t-m: select in which mode tool will select encodings (encoding generation predefined to generate all permutations):\n");
	printf("\t\tmax: select encodings maximise function.\n");
	printf("\t\tmin: select encodings minimise function.\n");
	printf("\t\tall: do not do any selection on encoding generated.\n");
	printf("\t\tman [cust_enc.txt]: followed by name of file contains custom encodings, encode CPOG with them.\n");
	printf("\t-h/help: show help of the tool.\n");
	printf("\t-r [num_encs]: activate random generation mode, and how many encodings generate.\n");
	printf("\t-set [cust_enc.txt]: followed by name of file contains custom encodings, encode selected POG with them.\n");
	printf("\t-top [num_encs]: activate clever encoding generation, you must specify number of encodings to generate.\n");
	printf("\t-u: do not fix first element during permutations.\n");
	printf("\t-v: selects the verbose mode of the tool.\n");
	printf("\t-version: prints version of ScEnco.\n\n");
	//EXAMPLE
	printf("Example: ");
	printf("%s arm_m0.cpog -m min -top 1000\n", prog_name);
	printf("It would generate 1000 \"clever\" encodings and will select for mapping just ones minimising function.\n\n");
	printf("%s was developed by Newcastle University (School of Electrical and Electronic and Computer Engineering).\n", prog_name);
	printf("Developers: Alessandro de Gennaro - Andrey Mokhov\n\n");
	print_version();
	return;
}

void print_version(){
	printf("ScEnco version: 1.4.2\n");
	return;
}

/*MANAGE DATA-BASE FUNCTION*/
/*Following function manage data-base according to user choice.*/
int manage_data_base(int count_max,float max,int count_min,float min, int cpog_count,int *bits){
	int i, j,k;
	FILE *fp;
	char number[MAX_NAME];
	
	switch(mode){
		case 0:
			cons_perm = (int**) malloc(sizeof(int*) * count_min);
			for(i=0;i<count_min;i++)
				cons_perm[i] = (int*) malloc(cpog_count * sizeof(int));
			k=0;
			for(i=0; i<counter;i++)
				if(weights[i] < min){
					for(j=0;j<cpog_count;j++)
						cons_perm[k][j] = perm[i][j];
					k++;
				}
			counter = count_min;
			gates = (int*) malloc(sizeof(int) * count_min);
			area = (float*) malloc(sizeof(float) * count_min);
			break;
		case 1:
			cons_perm = (int**) malloc(sizeof(int*) * count_max);
			for(i=0;i<count_max;i++)
				cons_perm[i] = (int*) malloc(cpog_count * sizeof(int));
			k=0;
			for(i=0; i<counter;i++)
				if(max == weights[i]){
					for(j=0;j<cpog_count;j++)
						cons_perm[k][j] = perm[i][j];
					k++;
				}
			counter = count_max;
			gates = (int*) malloc(sizeof(int) * count_max);
			area = (float*) malloc(sizeof(float) * count_max);
			break;
		case 2:
			cons_perm = perm;
			gates = (int*) malloc(sizeof(int) * num_perm);
			area = (float*) malloc(sizeof(float) * num_perm);
			break;
		case 3:
			fp = fopen(file_name,"r");
			cons_perm = (int**) malloc(sizeof(int*));
			cons_perm[0] = (int*) malloc(sizeof(int) * cpog_count);
			counter = 1;
			j=0;
			for (i=0;i<cpog_count;i++){
				if( (fscanf(fp,"%s", number) == EOF)){
					printf("Error: not enough encoding for this CPOG, they must be %d.\n",cpog_count);
					return 1;
				}
				if(i == 0) *bits = strlen(number);
				k = conv_int(number, -1);// mod k = conv_int(number, NULL);
				cons_perm[0][i] = k;
			}
			tot_enc = 1;
			for(i=0;i<(*bits);i++) tot_enc *= 2;
			gates = (int*) malloc(sizeof(int));
			area = (float*) malloc(sizeof(float));
			fclose(fp);

			if(DC){
				manual_file = (char**) malloc(sizeof(char*) * cpog_count);
				for(i=0;i<cpog_count;i++)
					manual_file[i] = (char*) malloc(sizeof(char) * ((*bits)+1));


				if( (fp = fopen(file_name,"r")) == NULL ){
					printf("Error on opening manual file.\n");
					return 2;
				}

				for(i=0;i<cpog_count;i++){
					if(fscanf(fp,"%s",manual_file[i]) == EOF){
						printf("Error on reading custom encodings.\n");
						return 3;
					}
				}
	
				fclose(fp);
			}
			break;
		default:
			printf("Error on the mode selection.\n");
			return 3;
	}

	return 0;
}

/*READ ENCODING SET FUNCTION*/
/*This function reads the encoding set by designer in order to fix them.*/
int read_set_encoding(int cpog_count, int *bits){
	FILE *fp = NULL;
	int i,k;
	char number[MAX_NAME];
	boolean acq = FALSE;

	fp = fopen(custom_file_name,"r");
	custom_perm = (int*) malloc(sizeof(int) * cpog_count);
	custom_perm_back = (int*) malloc(sizeof(int) * cpog_count);
	DC_custom = (boolean*) calloc(cpog_count,sizeof(boolean));

	for (i=0;i<cpog_count;i++){
		if( (fscanf(fp,"%s", number) == EOF)){
			printf("Error: not enough encoding for this CPOG, they must be %d.\n",cpog_count);
			return 1;
		}
		if(number[0] != '/'){
			// RESET number of bits
			if(!acq){
				*bits = strlen(number);
				acq = TRUE;
				tot_enc = 1;
				for(k=0;k<(*bits);k++) tot_enc *= 2;
			}
			k = conv_int(number, i);
			custom_perm[i] = k;
			custom_perm_back[i] = k;
		}
		else{
			custom_perm[i] = -1;
			custom_perm_back[i] = -1;
		}
	}

	if (fscanf(fp,"%s", number) == EOF){
		printf("Error on reading number of bits set inside file %s.\n", custom_file_name);
		return 2;
	}
	*bits = atoi(number);
	tot_enc = 1;
	for(k=0;k<(*bits);k++) tot_enc *= 2;
	fclose(fp);

	
	manual_file = (char**) malloc(sizeof(char*) * cpog_count);
	manual_file_back = (char**) malloc(sizeof(char*) * cpog_count);
	for(i=0;i<cpog_count;i++){
		manual_file[i] = (char*) malloc(sizeof(char) * ((*bits)+1));
		manual_file_back[i] = (char*) malloc(sizeof(char) * ((*bits)+1));
	}


	if( (fp = fopen(custom_file_name,"r")) == NULL ){
		printf("Error on opening manual file.\n");
		return 2;
	}

	for(i=0;i<cpog_count;i++){
		if(fscanf(fp,"%s",manual_file[i]) == EOF){
			printf("Error on reading custom encodings.\n");
			return 3;
		}
		strcpy(manual_file_back[i],manual_file[i]);
	}

	fclose(fp);
	return 0;
	
}

// removing temporary files from the HDD
void removeTempFiles(){
	char *command;

#ifdef __linux
	command = strdup("rm -f ");
	command = catMem(command, TMP_FILE);
	if (system(command) == -1){
		printf("Error on removing %s.\n", TMP_FILE);
		return;
	}
	free(command);
    	command = strdup("rm -f ");
	command = catMem(command, SCRIPT_PATH);
	if (system(command) == -1){
		printf("Error on removing %s.\n", SCRIPT_PATH);
		return;
	}
	free(command);
	command = strdup("rm -f ");
	command = catMem(command, TRIVIAL_ENCODING_FILE);
	if (system(command) == -1){
		printf("Error on removing %s.\n", TRIVIAL_ENCODING_FILE);
		return;
	}
	free(command);
	command = strdup("rm -f ");
	command = catMem(command, CONSTRAINTS_FILE);
	if (system(command) == -1){
		printf("Error on removing %s.\n", CONSTRAINTS_FILE);
		return;
	}
	free(command);
#else
    	command = strdup("del ");
	command = catMem(command, TMP_FILE);
	if (system(command) == -1){
		printf("Error on removing %s.\n", TMP_FILE);
		return;
	}
	free(command);
    	command = strdup("del ");
	command = catMem(command, SCRIPT_PATH);
	if (system(command) == -1){
		printf("Error on removing %s.\n", SCRIPT_PATH);
		return;
	}
	free(command);
	command = strdup("del ");
	command = catMem(command, TRIVIAL_ENCODING_FILE);
	if (system(command) == -1){
		printf("Error on removing %s.\n", TRIVIAL_ENCODING_FILE);
		return;
	}
	free(command);
	command = strdup("del ");
	command = catMem(command, CONSTRAINTS_FILE);
	if (system(command) == -1){
		printf("Error on removing %s.\n", CONSTRAINTS_FILE);
		return;
	}
	free(command);
#endif

	return;
}

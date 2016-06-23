#if defined(__linux) || defined(__APPLE__)
	#include "config.h"
#else
	#include "D:\Projects\SCENCO\inc\config.h"
#endif

/*GLOBAL VARIABLES*/
char **diff = NULL; 
char **name_cond; 
char **vertices;
char **manual_file;
char **manual_file_back;
char *numb;
char *file_in = NULL;
char *file_cons = NULL;
char *file_name = NULL;
char **decoder;
char *custom_file_name = NULL;
char *ESPRESSO_PATH;
char *ABC_PATH;
char *LIBRARY_FILE;
char *VERILOG_FILE;
char CURRENT_PATH[stringLimit];
char *FOLDER_NAME; 

int **opt_diff = NULL;
int counter = 0;
int **perm = NULL;
int nv=0;
int **cons_perm;
int n_cond = 0;
int *gates,mode = 1;
int tot_enc;
int gen_mode = 2;
int gen_perm = 1000;
int *custom_perm;
int *custom_perm_back;
int mod_bit = 2;

//TEMPORARY FILES
#if defined(__linux) || defined(__APPLE__)
	char TRIVIAL_ENCODING_FILE[FILENAME_LENGTH] = "/tmp/trivial.XXXXXX";
	char CONSTRAINTS_FILE[FILENAME_LENGTH] = "/tmp/constraints.XXXXXX";
	char TMP_FILE[FILENAME_LENGTH] = "/tmp/tmpfile.XXXXXX";
	char SCRIPT_PATH[FILENAME_LENGTH] = "/tmp/synth.XXXXXX";
	char BOOL_PATH[FILENAME_LENGTH] = "/tmp/bool.XXXXXX";
	char VERILOG_TMP[FILENAME_LENGTH] = "/tmp/micro.XXXXXX";
#else
	char TRIVIAL_ENCODING_FILE[FILENAME_LENGTH];
	char CONSTRAINTS_FILE[FILENAME_LENGTH];
	char TMP_FILE[FILENAME_LENGTH];
	char SCRIPT_PATH[FILENAME_LENGTH];
	char BOOL_PATH[FILENAME_LENGTH];
	char TMP_NAME[FILENAME_LENGTH];
	char VERILOG_TMP[FILENAME_LENGTH];
#endif

long long int num_perm;

float *area;
long long int *weights = NULL;

CPOG_TYPE **cpog;

boolean unfix = FALSE;
boolean verbose = FALSE;
boolean DC = FALSE;
boolean decode_flag = FALSE;
boolean SET =FALSE;
boolean ABCFLAG = FALSE;
boolean *DC_custom = NULL;
boolean CPOG_SIZE = FALSE;
boolean DISABLE_FUNCTION = FALSE;
boolean OLD = FALSE;
boolean mod_bit_flag = FALSE;
boolean VER = FALSE;


//ANDREY'S TOOL
GRAPH_TYPE *g;
int n;
char s[stringLimit];

int V;
map<string, int> eventNames;
string eventNames_str[eventsLimit];
map<string, int> eventPredicates[eventsLimit];

vector<string> scenarioNames;
vector<string> scenarioOpcodes;

string ev[eventsLimit][predicatesLimit];
string ee[eventsLimit][eventsLimit];
map<string, vector<pair<int, int> > > constraints;
map<string, vector<pair<int, int> > >::iterator cp, cq;

vector<Encoding> encodings;

vector<string> cgv;
vector<vector<int> > cge;
vector<int> literal;
vector<int> bestLiteral;

string vConditions[eventsLimit][predicatesLimit];
string aConditions[eventsLimit][eventsLimit];

// alternative = false: alpha + beta * predicate
// alternative = true : alpha * (beta + predicate)
bool alternative = false;

int main(int argc, char **argv){

	char *command;

	int bits = 0;
	int j = 0;
	int k=0;
	int err=0;
	int cpog_count = 0;
	int len_sequence = 0;
	int min_bits = 0;
	int *sol;
	int *enc;
	int count_min = 0;
	int count_max = 0;
	int num_vert = 0;
	int elements; 
	int min_disp;

	float ma;
	float MA;
	float mfma;
	float mfMA;
	long long int max = -1;
	long long int min = numeric_limits<long long int>::max();
	long int i;


	struct timeval start;
	struct timeval end;
	struct timeval begin;
	struct timeval finish;
	struct timeval detail_start;
	struct timeval detail_end;
	long secs_used;
	long precise;
	long total_time;

	FILE *fp = NULL;

	gettimeofday(&begin, NULL);

	/*PARSE PARAMETERS*/
	if( (err = parse_arg(argc, argv)) != 0){
		if (err != 4){
			printf(".error \n");
			printf("Error on parsing the argument.\n");
			printf(".end_error \n");
		}
		return 1;
	}


	// TEMPORARY FILES DEFINITION
#if defined(__linux) || defined(__APPLE__)
	if (mkstemp(TRIVIAL_ENCODING_FILE) == -1){
		printf(".error \n");
		printf("Error on opening trivial temporary file: %s.\n", TRIVIAL_ENCODING_FILE);
		printf(".end_error \n");
		removeTempFiles();
		return 1;
	}
	if (mkstemp(CONSTRAINTS_FILE) == -1){
		printf(".error \n");
		printf("Error on opening constraint temporary file: %s.\n", CONSTRAINTS_FILE);
		printf(".end_error \n");
		removeTempFiles();
		return 1;
	}
	if (mkstemp(TMP_FILE) == -1){
		printf(".error \n");
		printf("Error on opening temporary file: %s.\n", TMP_FILE);
		printf(".end_error \n");
		removeTempFiles();
		return 1;
	}
	if (mkstemp(SCRIPT_PATH) == -1){
		printf(".error \n");
		printf("Error on opening temporary file: %s.\n", SCRIPT_PATH);
		printf(".end_error \n");
		removeTempFiles();
		return 1;
	}
	if (mkstemp(BOOL_PATH) == -1){
		printf(".error \n");
		printf("Error on opening temporary file: %s.\n", BOOL_PATH);
		printf(".end_error \n");
		removeTempFiles();
		return 1;
	}
	if (mkstemp(VERILOG_TMP) == -1){
		printf(".error \n");
		printf("Error on opening temporary file: %s.\n", VERILOG_TMP);
		printf(".end_error \n");
		removeTempFiles();
		return 1;
	}
#else
	GetTempPath(FILENAME_LENGTH,TRIVIAL_ENCODING_FILE);
   	BOOL_PATH[strlen(TRIVIAL_ENCODING_FILE)-1] = '\0';
	tmpnam (TMP_NAME);
	strcat(TRIVIAL_ENCODING_FILE,TMP_NAME);

	GetTempPath(FILENAME_LENGTH,CONSTRAINTS_FILE);
	BOOL_PATH[strlen(CONSTRAINTS_FILE)-1] = '\0';
	tmpnam (TMP_NAME);
	strcat(CONSTRAINTS_FILE,TMP_NAME);

	GetTempPath(FILENAME_LENGTH,TMP_FILE);
	BOOL_PATH[strlen(TMP_FILE)-1] = '\0';
	tmpnam (TMP_NAME);
	strcat(TMP_FILE,TMP_NAME);

	GetTempPath(FILENAME_LENGTH,SCRIPT_PATH);
	BOOL_PATH[strlen(SCRIPT_PATH)-1] = '\0';
	tmpnam (TMP_NAME);
	strcat(SCRIPT_PATH,TMP_NAME);

	GetTempPath(FILENAME_LENGTH,BOOL_PATH);
    	BOOL_PATH[strlen(BOOL_PATH)-1] = '\0';
	tmpnam (TMP_NAME);
	strcat(BOOL_PATH,TMP_NAME);

	GetTempPath(FILENAME_LENGTH,VERILOG_TMP);
    	VERILOG_TMP[strlen(VERILOG_TMP)-1] = '\0';
	tmpnam (TMP_NAME);
	strcat(VERILOG_TMP,TMP_NAME);
#endif

	// READ CURRENT PATH POSITION
#if defined(__linux) || defined(__APPLE__)
	if( (fp = popen("pwd","r")) == NULL){
			printf(".error \n");
			printf("Error on pwd execution.\n");
			printf(".end_error \n");
			removeTempFiles();
			return 1;
	}
	if (fgets(CURRENT_PATH,stringLimit,fp) == NULL){
		printf(".error \n");
		printf("Error on reading current path.\n");
		printf(".end_error \n");
		removeTempFiles();
		return 1;
	}
	pclose(fp);

	k = 0;
	for(j = 0; j< ((int)strlen(CURRENT_PATH) - 1); j++){
		if(CURRENT_PATH[j] != '\\')
			CURRENT_PATH[k++] = CURRENT_PATH[j]; 
	}
	CURRENT_PATH[k++] = '/';
	CURRENT_PATH[k] = '\0';

	printf("CURRENT: %s\n", CURRENT_PATH);
	fflush(stdout);
#else
	if( (fp = popen("echo %cd%","r")) == NULL){
			printf(".error \n");
			printf("Error on pwd execution.\n");
			printf(".end_error \n");
			removeTempFiles();
			return 1;
	}
	fgets(CURRENT_PATH,stringLimit,fp);
	pclose(fp);
	int lenstr = strlen(CURRENT_PATH);
	CURRENT_PATH[lenstr-1] = '\\';
	k = 0;
#endif

	printf("Allocating memory for vertex names and graphs...");
	fflush(stdout);
	name_cond = (char**) malloc(sizeof(char*) * MAX_VERT);
	vertices = (char**) malloc(sizeof(char*) * MAX_VERT);
	g = (GRAPH_TYPE *) malloc(sizeof(GRAPH_TYPE) * scenariosLimit);
	printf("DONE\n");
	fflush(stdout);


	/*************************************************************************
	****************************BUILDING CPOG*********************************
	*************************************************************************/

	puts("\nOptimal scenarios encoding and CPOG synthesis.\n");	

	if (!alternative)
		puts("Using 'f = x + y * predicate' to deal with predicates.\n");
	else
		puts("Using 'f = x * (y + predicate)' to deal with predicates.\n");

		
	// debug scenarios
	/*FILE *fs = NULL;
	fp = fopen(file_in, "r");
	fs = fopen("scenarios.cpog", "w");
	do
	   {
	      char c = fgetc(fp);
	      if( feof(fp) )
	      {
		 break ;
	      }
	      fprintf(fs,"%c", c);
	   }while(1);
	
	fclose(fs);
	fclose(fp);
	return 1;*/

	fp = freopen(file_in, "r", stdin);

	n = 0;
	while(scanf("%s", s) == 1)
	{
		if (s[0] == '#')
		{
			if(fgets(s, sizeof(s), stdin) == NULL){
				printf(".error \n");
				puts("Error reading scenario.");
				printf(".end_error \n");
				removeTempFiles();
				return 0;
			}
			continue;
		}
		
		if (!strcmp(s, ".scenario"))
		{
			if(scanf("%s", s) == EOF){
				printf(".error \n");
				puts("Error reading scenario.");
				printf(".end_error \n");
				removeTempFiles();
				return 0;
			}
			printf("Loading scenario '%s'... ", s);
			scenarioNames.push_back(s);
			if (!readScenario()) {
				printf(".error \n");
				puts("Error reading scenario.");
				printf(".end_error \n");
				removeTempFiles();
				return 0;
			}
		}
		else
		{
			printf(".error \n");
			puts("Wrong file format.");
			printf(".end_error \n");
			removeTempFiles();
			return 0;
		}
	}

	fclose(fp);
	
	printf("\n%d scenarios have been loaded.\n", n);
	
	bool predicates_found = false;
	for(int i = 0; i < V; i++)
	if (eventPredicates[i].size() > 0)
	{
		if (!predicates_found)
		{
			predicates_found = true;
			puts("\nList of predicates:");
		}
		printf("%s:", eventNames_str[i].c_str());
		map<string, int>::iterator p = eventPredicates[i].begin(), q = eventPredicates[i].end();
		while(p != q)
		{
			string pr = p->first;
			printf(" %s", pr.c_str());
			p++;
		}
		puts("");
	}
	if (!predicates_found) puts("\nNo predicates found.");
	
	if( (fp = fopen(CONSTRAINTS_FILE,"w")) == NULL){
		printf(".error \n");
		printf("Error on opening constraints file for writing.\n");
		printf(".end_error \n");
		removeTempFiles();
		return -1;
	}

	for(int i = 0; i < V; i++)
	{
		int np = eventPredicates[i].size();
		
		for(int j = 0; j <= np; j++) ev[i][j] = "";
		if (np == 0)
		{
			for(int j = 0; j < n; j++) if (g[j].v[i]) ev[i][0] += "1"; else ev[i][0] += "0";
			constraints[ev[i][0]].push_back(make_pair(-1, i));
		}
		else
		{
			if (!alternative)
			{	
				for(int j = 0; j < n; j++) if (g[j].v[i] && !g[j].pred[i]) ev[i][0] += "1"; else ev[i][0] += "0";
				constraints[ev[i][0]].push_back(make_pair(-1, i));
				for(int k = 1; k <= np; k++)
				{
					for(int j = 0; j < n; j++)
					if (!g[j].v[i]) ev[i][k] += "0";
					else
					{
						if (g[j].pred[i] == 0) ev[i][k] += "-";
						else
						if (g[j].pred[i] == k) ev[i][k] += "1";
						else
							ev[i][k] += "0";
					}
					constraints[ev[i][k]].push_back(make_pair(-k - 1, i));
				}
			}
			else
			{
				for(int j = 0; j < n; j++) if (g[j].v[i]) ev[i][0] += "1"; else ev[i][0] += "0";
				constraints[ev[i][0]].push_back(make_pair(-1, i));
				for(int k = 1; k <= np; k++)
				{
					for(int j = 0; j < n; j++)
					if (!g[j].v[i]) ev[i][k] += "-";
					else
					{
						if (g[j].pred[i] == 0) ev[i][k] += "1";
						else
						if (g[j].pred[i] == k) ev[i][k] += "0";
						else
							ev[i][k] += "1";
					}
					constraints[ev[i][k]].push_back(make_pair(-k - 1, i));
				}
			}
		}
	}
	
	for(int i = 0; i < V; i++)
	for(int j = 0; j < V; j++)
	if (i != j)
	{
		ee[i][j] = "";
		for(int k = 0; k < n; k++)
		{
			if (g[k].e[i][j] == 2 || ME(k, i, j)) ee[i][j] += "-";
			else
			if (g[k].e[i][j] == 1) ee[i][j] += "1";
			else ee[i][j] += "0";
		}
		constraints[ee[i][j]].push_back(make_pair(i, j));
	}
	
	cp = constraints.begin(); cq = constraints.end();
	while(cp != cq)
	{
		string s = cp->first;
		fprintf(fp,"%s       ", s.c_str());
		int k = cp->second.size();
		for(int i = 0; i < k; i++)
		{
			int a = cp->second[i].first;
			int b = cp->second[i].second;
			if (a < 0)
			{
				if (a == -1) fprintf(fp," %s", eventNames_str[b].c_str());
				else fprintf(fp," %s:%s", eventNames_str[b].c_str(), getPredicateName(b, -a - 1).c_str());
			}
			else
			{
				fprintf(fp," (%s -> %s)", eventNames_str[a].c_str(), eventNames_str[b].c_str());
			}
		}
		fprintf(fp,"\n");
		cp++;
		
		Encoding e;
		
		e.constraint = s;
		e.trivial = true;
		e.constant = 0;
		
		for(int i = 0; i < n; i++) if (s[i] == '1') { e.trivial = false; break;}

		if (!e.trivial)
		{
			e.trivial = true;
			e.constant = 1;
			
			for(int i = 0; i < n; i++) if (s[i] == '0') { e.trivial = false; break;}
		}
		
		encodings.push_back(e);
	}
	fclose(fp);
	int total = constraints.size(), trivial = 0;

	
	for(int i = 0; i < total; i++) if (encodings[i].trivial) trivial++;

	printf("\n%d non-trivial encoding constraints found:\n\n", total - trivial);
	
	if( (fp = fopen(TRIVIAL_ENCODING_FILE,"w")) == NULL){
		printf(".error \n");
		printf("Error on opening constraints file for writing.\n");
		printf(".end_error \n");
		removeTempFiles();
		return -1;
	}
	for(int i = 0; i < total; i++)
		if (!encodings[i].trivial) {
			fprintf(fp,"%s\n",encodings[i].constraint.c_str());
			//printf("%s\n",encodings[i].constraint.c_str());
	}
	fclose(fp);
	
	printf("\nBuilding conflict graph... ");
	
	for(int i = 0; i < total; i++)
	if (!encodings[i].trivial)
	{
		string s = encodings[i].constraint;
		cgv.push_back(s);
		for(int j = 0; j < n; j++) if (s[j] == '0') s[j] = '1'; else if (s[j] == '1') s[j] = '0';
		cgv.push_back(s);
	}
	
	cge.resize(cgv.size());
	literal.resize(cgv.size());
	bestLiteral.resize(cgv.size());
	for(unsigned int i = 0; i < cgv.size(); i += 2) { bestLiteral[i] = i / 2; bestLiteral[i + 1] = -1;}
	
	for(unsigned int i = 0; i < cgv.size(); i++)
	for(unsigned int j = 0; j < cgv.size(); j++)
	{
		string a = cgv[i];
		string b = cgv[j];
		
		bool conflict = false;
		
		for(int k = 0; k < n; k++)
			if ((a[k] == '0' && b[k] == '1') || (a[k] == '1' && b[k] == '0'))
			{
				conflict = true;
				break;
			}
		
		if (conflict) cge[i].push_back(1); else cge[i].push_back(0);
	}
	
	printf("DONE.\n");
	fflush(stdout);

	// single literal encoding
	if (OLD){
		printf("Single literal encoding... ");
		fflush(stdout);
		int L = 0, R = cgv.size() / 2, cnt = 1;
		while(R - L > 1)
		{
			int limit = (L + R) / 2;
		
			for(unsigned int i = 0; i < cgv.size(); i++) literal[i] = -1;
		
			printf(" [%d]", cnt++);
		
			bool res = false;
			res = encode(0, limit, 0);
		
			if (res)
			{
				bestLiteral = literal;
				R = limit;
			}
			else L = limit;
		}
	
		printf("DONE.\nThe best encoding uses %d operational variables:\n", R);
		fflush(stdout);
	
		scenarioOpcodes.resize(n);

		for(int i = 0; i < n; i++) for(int j = 0; j < R; j++) scenarioOpcodes[i] += "-";
	    
		k = 0;
		for(int i = 0; i < total; i++)
		if (!encodings[i].trivial)
		{
			int id = k * 2, inv = 0;
		
			if (bestLiteral[id] == -1) inv = 1;
		
			printf("%s        ", cgv[id].c_str());
			if (inv) printf("!");
			printf("x[%d]\n", bestLiteral[id + inv]);
		
			encodings[i].literal = bestLiteral[id + inv];
			encodings[i].inverted = inv;
		
			for(int j = 0; j < n; j++) if (cgv[id][j] != '-') scenarioOpcodes[j][bestLiteral[id + inv]] = cgv[id + inv][j];

			k++;
		}
	
		for(int i = 0; i < total; i++)
		{
			string s = encodings[i].constraint;
			char tmp[10];
		
			string f = "";
			if (encodings[i].trivial)
			{
				f += '0' + encodings[i].constant;
			}
			else
			{
				sprintf(tmp, "x[%d]", encodings[i].literal);
				f = tmp;
				if (encodings[i].inverted) f = "!" + f;		
			}		
		
			for(unsigned int j = 0; j < constraints[s].size(); j++)
			{
				int a = constraints[s][j].first;
				int b = constraints[s][j].second;
			
				if (a < 0) vConditions[b][-a - 1] = f;
				else aConditions[a][b] = f;
			}
		}
	
		puts("\nVertex conditions:\n");
	
		for(int i = 0; i < V; i++)
		{
			string f = vConditions[i][0];
			map<string, int>::iterator p = eventPredicates[i].begin(), q = eventPredicates[i].end();
		
			int k = 1;
			while(p != q)
			{
				if (!alternative)
				{
					if (vConditions[i][k] != "1") f += " + " + vConditions[i][k] + " * " + (p->first);
					else f += " + " + (p->first);
					p++;
					k++;
				}
				else
				{
					if (vConditions[i][k] != "0") f += " * (" + vConditions[i][k] + " + " + (p->first) + ")";
					else f += " * " + (p->first);
					p++;
					k++;
				}
			}
			if (f.find("0 + ") == 0) f.erase(0, 4);
			if (f.find("1 * ") == 0) f.erase(0, 4);
			printf("%10s: %s\n", eventNames_str[i].c_str(), f.c_str());
		}

		puts("\nArc conditions:\n");
	
		for(int i = 0; i < V; i++)
		for(int j = 0; j < V; j++)
		if (i != j)
		{
			string f = aConditions[i][j];
			if (f == "0") continue;

			printf("%10s -> %-10s: %s\n", eventNames_str[i].c_str(), eventNames_str[j].c_str(), f.c_str());
		}

		printf("\n.scen_opcodes \n");

		for(int i = 0; i < n; i++) printf("%s\n",scenarioOpcodes[i].c_str());
		printf(".end_scen_opcodes \n");
	}

	printf("Free memory related to graphs acquisition...");
	fflush(stdout);
	free(g);
	printf("DONE\n");
	fflush(stdout);

	/*************************************************************************
	***********************READING NON-TRIVIAL ENCODINGS**********************
	*************************************************************************/
	printf("\n**************************************************************\n");
	printf("*****************READING NON-TRIVIAL ENCODINGS******************\n");
	printf("**************************************************************\n\n");
	fflush(stdout);

	//strcpy(file_in,TRIVIAL_ENCODING_FILE);
	//file_cons= strdup(CONSTRAINTS_FILE);

	/*READ NON-TRIVIAL ENCODING FILE*/
	printf("Reading non-trivial encoding file... ");
	fflush(stdout);
	if( (err = read_file(TRIVIAL_ENCODING_FILE, &cpog_count, &len_sequence)) ){
		printf(".error \n");
		printf("Error occured while reading non-trivial encoding file, error code: %d", err);
		printf(".end_error \n");
		removeTempFiles();
		return 2;
	}
	printf("DONE\n");
	fflush(stdout);

	/*SEED FOR RAND*/
	srand(time(NULL));

	/*ALLOCATING AND ZEROING DIFFERENCE MATRIX*/
	opt_diff = (int**) calloc(cpog_count, sizeof(int*));
	for(i=0;i<cpog_count;i++)
		opt_diff[i] = (int*) calloc(cpog_count, sizeof(int));

	/*COMPUTING LOGARITHM OF NUMBER OF CPOGs*/
	if(mod_bit_flag){
		bits = mod_bit;
	}else{
		bits = logarithm2(cpog_count);
	}

	/*NUMBER OF POSSIBLE ENCODING*/
	tot_enc = 1;
	for(i=0;i<bits;i++) tot_enc *= 2;
	if(mod_bit_flag){
		printf("Custom number of bits set to encode all CPOG is: %d\n",bits);		
	} else{
		printf("Miminum number of bits needed to encode all CPOG is: %d\n",bits);
	}

	// debug printing
	/*printf("Number of possible encodings: %d\n",tot_enc);
	printf("(int) binary\n");
	for(i=0;i<tot_enc;i++) 
		print_binary(stdout,i,bits);
	printf("\n");*/

	/*ANALYSIS IF IT'S A PERMUTATION OR A DISPOSITION*/
	num_perm = 1;
	if (cpog_count == tot_enc){
		/*PERMUTATION*/
		if(!unfix && !SET){
			for(i = 1; i< tot_enc; i++)
				num_perm *= i;
		}else{
			for(i = 1; i<= tot_enc; i++)
				num_perm *= i;
		}
		printf("Number of possible permutations by fixing first element: %lld\n", num_perm);
	}
	else{
		/*DISPOSITION*/
		if(!unfix && !SET){
			elements = tot_enc-1;
			min_disp = elements - (cpog_count - 1) + 1;
		}else{
			elements = tot_enc;
			min_disp = elements - (cpog_count) + 1;
		}
			num_perm = 1;
		for(i=elements; i>= min_disp; i--)
			num_perm *= i;
		printf("Number of possible dispositions by fixing first element: %lld\n", num_perm);
	}

	if(gen_mode > 1){
		if(num_perm > MAX_MEMORY || num_perm < 0){
			printf(".error \n");
			printf("Solution space is too wide to be inspected.\n");
			printf(".end_error \n");
			removeTempFiles();
			return 3;
		}
	}else{
		num_perm = gen_perm;
	}

	/*PREPARATION DATA FOR ENCODING PERMUTATIONS*/
	enc = (int*) calloc(tot_enc, sizeof(int));

	/*First element is fixed*/
	if (!unfix && !SET)
		enc[0] = 1;
	
	sol = (int*) calloc(tot_enc, sizeof(int));
	if (sol == NULL){
		printf(".error \n");
		printf("solution variable = null\n");
		printf(".end_error \n");
		removeTempFiles();
		return 3;
	}
	perm = (int**) malloc(sizeof(int*) * num_perm);
	if ( perm == NULL){
		printf(".error \n");
		printf("perm variable = null\n");
		printf(".end_error \n");
		removeTempFiles();
		return 3;
	}
	for(i=0;i<num_perm;i++){
		perm[i] = (int*) malloc(cpog_count * sizeof(int));
		if (perm[i] == NULL){
			printf(".error \n");
			printf("perm[%ld] = null\n",i);
			printf(".end_error \n");
			removeTempFiles();
			return 3;
		}
	}
	weights = (long long int*) calloc(num_perm, sizeof(long long int));


	/*BUILDING DIFFERENCE MATRIX*/
	printf("Building DM (=Difference Matrix)... ");
	fflush(stdout);
	if( (err = difference_matrix(cpog_count, len_sequence)) ){
		printf(".error \n");
		printf("Error occurred while building difference matrix, error code: %d", err);
		printf(".end_error \n");
		removeTempFiles();
		return 3;
	}
	printf("DONE\n");
	fflush(stdout);

	/*************************************************************************
	***************************ACQUISITION CPOG PART**************************
	*************************************************************************/
	printf("\n****************************************************************\n");
	printf("******READING ENCODING CONSTRAINTS OF THE ENTIRE CPOG***********\n");
	printf("****************************************************************\n\n");
	fflush(stdout);

	/*FIRST READING OF ENCODING FILE*/
	printf("First reading of encoding file...");
	fflush(stdout);
	if( (err = read_cons(CONSTRAINTS_FILE, cpog_count, &num_vert)) ){
		printf(".error \n");
		printf("Error occured while reading constraints file, error code: %d", err);
		printf(".end_error \n");
		removeTempFiles();
		return 5;
	}
	printf("DONE\n");
	fflush(stdout);

	/*DEBUG PRINTING: printing vertices information*/
	/*printf("Number of vertices: %d\n", num_vert);
	printf("Names of vertices:\n");
	for(i=0;i<num_vert;i++)
		printf("%s ", vertices[i]);
	printf("\n\n");*/

	/*CPOG ALLOCATION*/
	printf("CPOG data structure allocation...");
	fflush(stdout);
	cpog = (CPOG_TYPE**) malloc(sizeof(CPOG_TYPE*) * (num_vert));
	for(i=0;i<num_vert; i++)
		cpog[i] = (CPOG_TYPE*) malloc(sizeof(CPOG_TYPE) * (num_vert));
	printf("DONE\n");
	fflush(stdout);

	nv = num_vert; /*Due to overwriting problem*/

	/*CPOG DEFINITION*/
	for(i=0;i<num_vert; i++)
		for(j=0;j<num_vert;j++)
			if(i == j){
				cpog[i][j].type = 'v';
				cpog[i][j].source = strdup(vertices[i]);
				cpog[i][j].dest = strdup("X");
				cpog[i][j].condition = FALSE;
			}
			else{
				cpog[i][j].type = 'e';
				cpog[i][j].source = strdup(vertices[i]);
				cpog[i][j].dest = strdup(vertices[j]);
				cpog[i][j].condition = FALSE;
			}

	/*SECOND READING OF ENCODING FILE*/
	printf("Second reading of encoding file (parsing CPOG)...");
	fflush(stdout);
	parsing_cpog(CONSTRAINTS_FILE, cpog_count, num_vert);
	printf("DONE\n");
	fflush(stdout);

	printf("CPOG read properly.\n");

	/*************************************************************************
	***********************ENCODINGS GENERATION PART**************************
	*************************************************************************/

	printf("\n****************************************************************\n");
	printf("***************GENERATING ENCODINGS FOR CPOG********************\n");
	printf("****************************************************************\n\n");
	fflush(stdout);

	/*START COUNTING TIME*/
	gettimeofday(&start, NULL);

	if(OLD){
		printf("Reading encodings set (OLD flag)... ");
		fflush(stdout);
		if( (fp = fopen(custom_file_name,"w")) == NULL ){
			printf(".error \n");
			printf("Error on opening custom file.\n");
			printf(".end_error \n");
			removeTempFiles();
			return 2;
		}
		for(i=0; i<cpog_count; i++) fprintf(fp,"%s\n",scenarioOpcodes[i].c_str());
		int nbits = strlen(scenarioOpcodes[0].c_str());
		fprintf(fp, "%d", nbits);
		fclose(fp);
		printf("DONE\n");
		fflush(stdout);
	}

	if(SET){
		printf("Reading encodings set... ");
		fflush(stdout);
		if(read_set_encoding(cpog_count,&bits) != 0){
			printf(".error \n");
			printf("Error on reading encoding set.\n");
			printf(".end_error \n");
			removeTempFiles();
			return 1;
		}
		printf("DONE\n");
		printf("Check correcteness of encoding set... ");
		fflush(stdout);
		if(check_correctness(cpog_count,tot_enc,bits) != 0){
			removeTempFiles();
			return 1;
		}
		printf("DONE\n");
		fflush(stdout);
		//DEBUG PRINTING: set encodings read properly
		/*for(int y=0;y<cpog_count; y++){
			printf("%d %s %d\n",custom_perm[y], manual_file[y], DC_custom[y]);
		}*/

	}

	/*ENCODING GENERATION*/
	switch(gen_mode){
		case 0: // RANDOM SEARCH
			printf("Selected random encoding generation. %lld encodings will be generated.\n", num_perm);
			if (!SET){
				printf("Random algorithm generation unconstrained... ");
				fflush(stdout);
				rand_permutation(cpog_count, tot_enc);
				printf("DONE\n");
				fflush(stdout);
			}
			else{
				printf("Random algorithm generation constrained... ");
				fflush(stdout);
				rand_permutations_constraints_v2(cpog_count,tot_enc,bits);
				printf("DONE\n");
				fflush(stdout);
			}

			break;
		case 1: // HEURISTIC ENCODING
			printf("Selected herustic encoding generation. %lld encodings will be generated.\n", num_perm);
			// algorithm for setting the starting encoding disabled
			// best_permutations(cpog_count, tot_enc, bits);

			gettimeofday(&detail_start, NULL);
			if (!SET){
				printf("Starting encoding generation unconstrained... ");
				fflush(stdout);
				rand_permutation(cpog_count, tot_enc);
				printf("DONE\n");
				fflush(stdout);
			}
			else{
				printf("Starting encoding generation constrained... ");
				fflush(stdout);
				rand_permutations_constraints_v2(cpog_count,tot_enc,bits);
				printf("DONE\n");
				fflush(stdout);
			}
			gettimeofday(&detail_end, NULL);
			precise=(detail_end.tv_sec - detail_start.tv_sec);
			printf("Time for first vectors generation: %ld [s]\n", precise);
			fflush(stdout);

			gettimeofday(&detail_start, NULL);
			printf("Tune vector by using simulated annealing... ");
			fflush(stdout);
			start_simulated_annealing(cpog_count,tot_enc,bits);
			printf("DONE\n");
			fflush(stdout);
			gettimeofday(&detail_end, NULL);
			precise=(detail_end.tv_sec - detail_start.tv_sec);
			printf("Time for tuning vectors: %ld [s]\n", precise);
			fflush(stdout);

			break;
		default:
			if(!unfix && !SET){
				//permutation_stdalgo(cpog_count,tot_enc);
				printf("Permutation algorithm unconstrained.\n");
				fflush(stdout);
				permutation(sol,0,enc,cpog_count, tot_enc);
				printf("DONE\n");
				fflush(stdout);
			}else{
				printf("Permutation algorithm constrained.\n");
				fflush(stdout);
				permutation(sol,-1,enc,cpog_count, tot_enc);
				printf("DONE\n");

				printf("Filtering encoding... ");
				fflush(stdout);
				filter_encodings(cpog_count, bits, tot_enc);
				printf("DONE\n");
				fflush(stdout);
			}

			break;
	}

	//TIME SPENT FOR GENERATING ENCODINGS
	gettimeofday(&end, NULL);
	secs_used=(end.tv_sec - start.tv_sec);

	/*DEBUG PRINTING: permutations*/
	/*for(i=0;i<num_perm;i++){
		printf("%ld) ", i+1);
		for(j=0;j<cpog_count;j++)
			printf("%d ", perm[i][j]);
		printf("\n\n");
	}*/

	
	/*COMPUTING WEIGHT FOR EACH ENCODING*/
	max = -1;
	min = area_encodings_ssd(cpog_count, bits, &max,tot_enc,num_vert);

	printf("Maximum weight for all possible permutations: %lld\n", max);
	printf("Minimum weight for all possible permutations: %lld\n", min);

	/*COUNTING HOW MANY ENCODINGS HAVE MAX WEIGHT AND MAX AREA*/
	for(i=0; i<counter;i++){
		if(max == weights[i]){
			count_max++;
		}
	}

	printf("Number of encodings with maximum weight: %d\n", count_max);

	/*COUNTING HOW MANY ENCODINGS HAVE MIN WEIGHT AND MIN AREA*/
	for(i=0; i<counter;i++){
		if(min == weights[i]){
			count_min++;
		}
	}

	printf("Number of encodings with minimum weight: %d\n", count_min);

	/*MANAGE DATABASE PROPERLY*/
	printf("Manage database for synthesis process... ");
	fflush(stdout);
	if(manage_data_base(count_min,min,count_max,max,cpog_count,&bits)){
		printf(".error \n");
		printf("Error managing data-base.\n");
		printf(".end_error \n");
		removeTempFiles();
		return 4;
	}
	printf("DONE\n");
	fflush(stdout);

	/*DEBUG PRINTING: permutations considerated*/
	/*printf("Permutation considered: ");
	if(!DC){
		for(i=0; i<counter;i++){
			for(j=0;j<cpog_count;j++)
				print_binary(stdout,cons_perm[i][j], bits);
			printf("\n");
		}
	}
	else{
		printf("Encoding set by file:\n");
		for(i=0;i<cpog_count;i++)
			printf("%s ",manual_file[i]);
		printf("\n");
	}*/

	/*FREE VARIABLES NO MORE USEFUL*/
	printf("Free memory of the variables no more used... ");
	fflush(stdout);
	free(enc);
	free(sol);
	for(i=0;i<cpog_count;i++)
		free(opt_diff[i]);
	free(opt_diff);
	for(i=0;i<len_sequence;i++)
		free(diff[i]);
	free(diff);
	free(file_in);
	printf("DONE\n");
	fflush(stdout);

	printf("Time takes for generating encodings: %ld [s].\n",secs_used);
	fflush(stdout);
	/*************************************************************************
	***************************MAPPING PART***********************************
	*************************************************************************/

	printf("\n****************************************************************\n");
	printf("******CONVERTING CPOG CONTRAINTS INTO BOOLEAN FUNCTIONS*********\n");
	printf("****************************************************************\n\n");
	fflush(stdout);

	printf("Memory allocation phase... ");	
	fflush(stdout);
	/*ALLOCATION MEMORY FOR ALL LOGIC FUNCTIONS CONSIDERED*/
	for(i=0;i<nv; i++){
		for(j=0;j<nv;j++){
			cpog[i][j].fun = (char**) malloc(sizeof(char*) * counter);
			cpog[i][j].fun_cond = (char**) malloc(sizeof(char*) * counter);
		}
	}
	printf("DONE\n");
	fflush(stdout);

	/*START COUNTING TIME*/
	//start = clock();
	gettimeofday(&start, NULL);

	/*CONVERT TRUTH TABLES INTO BOOLEAN FUNCTION*/
	printf("Convert truth table into boolean functions of vertices and edges... ");
	fflush(stdout);
	if((err = boolean_function(max,bits,cpog_count,0)!= 0)){
		printf(".error \n");
		printf("Error on getting boolean function using Espresso. Error code: %d\n", err);
		printf(".end_error \n");
		removeTempFiles();
		return 6;
	}
	printf("DONE\n");
	fflush(stdout);


	/*CONVERT TRUTH TABLES INTO BOOLEAN FUNCTION OF CONDITION ONLY*/
	printf("Convert truth table into boolean functions of condition of vertices... ");
	fflush(stdout);
	if((err = boolean_function(max,bits,cpog_count,1)!= 0)){
		printf(".error \n");
		printf("Error on getting boolean function using Espresso. Error code: %d\n", err);
		printf(".end_error \n");
		removeTempFiles();
		return 7;
	}
	printf("DONE\n");
	fflush(stdout);

	min_bits = logarithm2(cpog_count);
	if(min_bits < bits && mode ==3){
		printf("Convert truth table into boolean functions (decoder minimisation)... ");
		fflush(stdout);
		decode_flag = TRUE;
		if(decoder_minimisation(bits,cpog_count) != 0){
			printf(".error \n");
			printf("Error on developing boolean functions of decoder.\n");
			printf(".end_error \n");
			removeTempFiles();
			return 8;
		}
		printf("DONE\n");
		fflush(stdout);
	}

	/*PRINT INFORMATION ACQUIRED FROM CPOG WITH RESPECT ENCODING CONSIDERED*/
	// debug printing
	/*if(verbose){
		for(c=0;c<counter;c++){
			printf("NAME\t\tTRUTH\t\tBOOL F.\t\tCOND\t\tTRUTH\t\tBOOL F.\n");
			printf("-----------------------------------------------------------------------------------------\n");
			for(i=0;i<nv; i++){
				for(j=0;j<nv;j++){
					if(cpog[i][j].type == 'v'){
						if(cpog[i][j].condition){
							printf("%s:\t\t%s\t\t%s\t\t%s\t\t%s\t\t%s\n",cpog[i][j].source, cpog[i][j].truth,cpog[i][j].fun[c], cpog[i][j].cond, cpog[i][j].truth_cond,cpog[i][j].fun_cond[c]);
							cnt_func +=2;
						}
						else{
							printf("%s:\t\t%s\t\t%s\n",cpog[i][j].source, cpog[i][j].truth,cpog[i][j].fun[c]);
							cnt_func++;
						}
					}
					else{
						printf("%s > %s:\t\t%s\t\t%s\n",cpog[i][j].source, cpog[i][j].dest, cpog[i][j].truth,cpog[i][j].fun[c]);
						cnt_func++;
					}
				}
			}
			printf("\n");
		}
	}*/

	gettimeofday(&end, NULL);
	secs_used=(end.tv_sec - start.tv_sec);
	printf("Conversion done successfully.\n");
	printf("Time of conversion= %ld [s].\n",secs_used);

	/*************************************************************************
	**************BUILDING FINAL BOOLEAN FUNCTIONS FOR SYNTHESYS**************
	*************************************************************************/
	printf("\n****************************************************************\n");
	printf("*******BUILDING FINAL BOOLEAN FUNCTIONS FOR SYNTHESYS***********\n");
	printf("****************************************************************\n\n");

	fflush(stdout);
	/*REMOVE OLD DIR AND MAKE NEW ONE AND ERASE USED FILES*/

#if defined(__linux) || defined(__APPLE__)
	command = strdup("rm -f ");
#else
	command = strdup("del ");
#endif
	command = catMem(command, TRIVIAL_ENCODING_FILE);
	if(system(command) != 0){
		printf(".error \n");
		printf("Error on removing trivial encoding file.\n");
		printf(".end_error \n");
		removeTempFiles();
		return 8;
	}
	free(command);

#if defined(__linux) || defined(__APPLE__)
	command = strdup("rm -f ");
#else
	command = strdup("del ");
#endif
	command = catMem(command, CONSTRAINTS_FILE);
	if(system(command) != 0){
		printf(".error \n");
		printf("Error on removing trivial encoding file.\n");
		printf(".end_error \n");
		removeTempFiles();
		return 8;
	}
	free(command);
	

	//ACQUIRE NAMES OF CONDITIONS
	printf("Getting condition names... ");
	fflush(stdout);
	if(get_conditions_names()){
		printf(".error \n");
		printf("Error on getting condition names from CPOG representation.\n");
		printf(".end_error \n");
		removeTempFiles();
		return 8;
	}
	printf("DONE\n");
	fflush(stdout);

	/*START COUNTING TIME*/
	gettimeofday(&start, NULL);

	printf("Building final Boolean equations (uses ABC if selected)... ");
	fflush(stdout);
	if(!CPOG_SIZE){
		if( (err = equations_abc(cpog_count,bits)) != 0){
			printf(".error \n");
			printf("Error on using ABC. Error code: %d.\n",err);
			printf(".end_error \n");
			removeTempFiles();
			return 9;
		}
	}else{
		if( (err = equations_abc_cpog_size(cpog_count,bits)) != 0){
			printf(".error \n");
			printf("Error on using ABC. Error code: %d.\n",err);
			printf(".end_error \n");
			removeTempFiles();
			return 9;
		}
	}
	printf("DONE\n");
	fflush(stdout);

	/*COMPUTES TIME SPENT BY FUNCTION equations_abc*/
	gettimeofday(&end, NULL);
	gettimeofday(&finish, NULL);
	secs_used=(end.tv_sec - start.tv_sec);
	total_time=(finish.tv_sec - begin.tv_sec);
	printf("Reduction and mapping done successfully.\n");
	printf("Time spent= %ld [s].\n\n",secs_used);
	printf("Total SCENCO run time: %ld [s].\n", total_time);
	fflush(stdout);
	/*************************************************************************
	*********************PRINTING OUT STATISTICS FOR WORKCRAFT****************
	*************************************************************************/
	printf("\n****************************************************************\n");
	printf("************PRINTING OUT STATISTICS FOR WORKCRAFT***************\n");
	printf("****************************************************************\n\n");
	fflush(stdout);

	// IF ABC TOOL IS NOT PRESENT OUTPUT A RANDOM ENCODING
	if(ABCFLAG == FALSE){
		i = rand() % counter;

		printf("MIN: ");
		for(k=0;k<cpog_count;k++){
			if(SET && DC_custom[k]){
				char str[MAX_NAME];
				int_to_string_DC(bits, k,cons_perm[i][k], str);
				printf("%s ",str);
			}
			else
				print_binary(stdout,cons_perm[i][k], bits);

		}
		printf("\n");
		
		printf(".start_formulae \n");
		for(j=0;j<nv; j++){
			for(k=0;k<nv;k++){
				if(cpog[j][k].type == 'v'){
					if(cpog[j][k].condition){
						/*char* pred = (char*) malloc((cpog_count + 1) *sizeof(char));
						for(int g=0; g<cpog_count; g++){
							if (cpog[j][k].truth[g] == '0' && cpog[j][k].truth_cond[g] == '0')
								pred[g] = '0';
							else
								pred[g] = '1';
						}
						pred[cpog_count] = '\0';*/
						printf("V,%s,%s,(%s) + ((%s) * (%s))\n",cpog[j][k].source,cpog[j][k].truth,cpog[j][k].fun[i], cpog[j][k].cond,cpog[j][k].fun_cond[i]);
					}
					else{
						printf("V,%s,%s,(%s)\n",cpog[j][k].source,cpog[j][k].truth,cpog[j][k].fun[i]);
					}
				}
				else{
					if(!decide(cpog[j][k].fun[i]))
						printf("A,%s,%s,%s,(%s)\n",cpog[j][k].source, cpog[j][k].dest,cpog[j][k].truth,cpog[j][k].fun[i]);
				}
			}
		}
		printf(".end_formulae \n");


		printf(".statistics \n");
		printf("Boolean equations for each element:\n");
		for(j=0;j<nv; j++){
			for(k=0;k<nv;k++){
				if(cpog[j][k].type == 'v'){
					if(cpog[j][k].condition){
						printf("%s = (%s) + ((%s) * (%s))\n",cpog[j][k].source,cpog[j][k].fun[i], cpog[j][k].cond,cpog[j][k].fun_cond[i]);
					}
					else{
						printf("%s = (%s)\n",cpog[j][k].source,cpog[j][k].fun[i]);
					}
				}
				else{
					if(!decide(cpog[j][k].fun[i]))
						printf("%s -> %s = (%s)\n",cpog[j][k].source, cpog[j][k].dest,cpog[j][k].fun[i]);
				}
			}
		}
		printf("\n");
		printf(".end_statistics \n");
		// Removing temporary files
		removeTempFiles();
		return 0;
	}

	ma = numeric_limits<float>::max();
	MA = -1;
	mfma = numeric_limits<float>::max();
	mfMA = -1;
	for(i=0;i<counter;i++){
		/*COUNT MAX ERROR AND MIN ERROR*/		
		if(mode == 2){
			if(area[i] < ma) ma = area[i];
			if(area[i] > MA) MA = area[i];
			if(weights[i] == min && area[i] < mfma) mfma = area[i];
			if(weights[i] == min && area[i] > mfMA) mfMA = area[i];
		}
		else{
			if(area[i] < ma) ma = area[i];
			if(area[i] > MA) MA = area[i];
		}
	}
	j = 0;

	/*PRINTING BEST ENCODINGS FOUND*/
	if(mode != 3){
		if(mode == 2){
			printf("RESULTS:\n");
		}
		float i_avg=0;
		float avg = 0.0;
		if(mode == 2){
			printf("ALL ENCODINGS CHARACTERISTICS:\n");
			for(i=0;i<counter;i++){
				for(k=0;k<cpog_count;k++){
					if(SET && DC_custom[k]){
						char str[MAX_NAME];
						int_to_string_DC(bits, k,cons_perm[i][k], str);
						printf("(%d) %s ",cons_perm[i][k],str);
					}
					else
						print_binary(stdout,cons_perm[i][k], bits);
				}
				printf("with %d gates and %.3f [um^2] and weight %lld.\n", gates[i],area[i],weights[i]);
				
			}
			printf("-----------------------\n\n");
		}

		for(i=0;i<counter;i++){
			/*if(mode == 2)
				fprintf(fp,"%.4f %.3f\n", weights[i], area[i]);*/
			if(area[i] == ma){
				printf(".area \n");
				printf("%.3f\n", area[i]);
				printf(".end_area \n\n");
				printf("MIN: ");
				for(k=0;k<cpog_count;k++){
					if(SET && DC_custom[k]){
						char str[MAX_NAME];
						int_to_string_DC(bits, k,cons_perm[i][k], str);
						printf("%s ",str);
					}
					else
						print_binary(stdout,cons_perm[i][k], bits);
				}
				printf("\n");
				//printf("with %d gates and %.3f [um^2].\n", gates[i],area[i]);
				printf(".start_formulae \n");
				for(j=0;j<nv; j++){
					for(k=0;k<nv;k++){
						if(cpog[j][k].type == 'v'){
							if(cpog[j][k].condition){
								/*char* pred = (char*) malloc((cpog_count + 1) *sizeof(char));
								for(int g=0; g<cpog_count; g++){
									if (cpog[j][k].truth[g] == '0' && cpog[j][k].truth_cond[g] == '0')
										pred[g] = '0';
									else
										pred[g] = '1';
								}
								pred[cpog_count] = '\0';*/
								printf("V,%s,%s,(%s) + ((%s) * (%s))\n",cpog[j][k].source,cpog[j][k].truth,cpog[j][k].fun[i], cpog[j][k].cond,cpog[j][k].fun_cond[i]);
							}
							else{
								printf("V,%s,%s,(%s)\n",cpog[j][k].source,cpog[j][k].truth,cpog[j][k].fun[i]);
							}
						}
						else{
							if(!decide(cpog[j][k].fun[i]))
								printf("A,%s,%s,%s,(%s)\n",cpog[j][k].source, cpog[j][k].dest,cpog[j][k].truth,cpog[j][k].fun[i]);
						}
					}
				}
				printf(".end_formulae \n");
				printf(".statistics \n");
				printf("Boolean equations for each element:\n");
				for(j=0;j<nv; j++){
					for(k=0;k<nv;k++){
						if(cpog[j][k].type == 'v'){
							if(cpog[j][k].condition){
								printf("%s = (%s) + ((%s) * (%s))\n",cpog[j][k].source,cpog[j][k].fun[i], cpog[j][k].cond,cpog[j][k].fun_cond[i]);
							}
							else{
								printf("%s = (%s)\n",cpog[j][k].source,cpog[j][k].fun[i]);
							}
						}
						else{
							if(!decide(cpog[j][k].fun[i]))
								printf("%s -> %s = (%s)\n",cpog[j][k].source, cpog[j][k].dest,cpog[j][k].fun[i]);
						}
					}
				}
				printf("\n");
				printf("Area and number of gates achieved with library set:\n");
				printf("Controller for CPOG consumes %.3f [um^2] and %d gates are needed.\n", area[i],gates[i]);
				printf(".end_statistics \n");

				removeTempFiles();
				return 0;
			}
		}
		if(mode == 2){
			printf("Maximum error = %.2f.\n", ((mfMA/ma) -1) *100);
			printf("Minimum error = %.2f.\n", ((mfma/ma) -1) *100);
			printf("Average error = %.2f.\n", ( ((avg/i_avg)/ma) -1) *100);
		}
		else
			printf("Overhead at the minimum point = %.2f.\n", ((MA/ma) -1) *100);
	}
	else{
		printf("Encoding set comes up with: %d gates and %.3f [um^2].\n",gates[0], area[0]);
	}

	removeTempFiles();
	
	printf("Free memory for vertex names... ");
	fflush(stdout);
	free (name_cond);
	free(vertices);
	printf("DONE");
	fflush(stdout);

	return 0;
}

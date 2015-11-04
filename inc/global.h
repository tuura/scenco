#ifndef __GLOBAL_H__
#define __GLOBAL_H__

/*GLOBAL VARIABLES*/
extern char** diff, **name_cond, **vertices, **manual_file,*numb, **manual_file_back;
extern int **opt_diff, counter, **perm, nv, **cons_perm, n_cond,*gates,mode,tot_enc,gen_mode,gen_perm, *custom_perm, *custom_perm_back, mod_bit;
extern long long int num_perm;
extern float *area, *weights;
extern CPOG_TYPE **cpog;
extern char* file_in, *file_cons,*file_name, **decoder, *custom_file_name;
extern boolean unfix, verbose, DC,decode_flag, SET,ABCFLAG, *DC_custom, CPOG_SIZE, DISABLE_FUNCTION, OLD, mod_bit_flag;
extern char *ESPRESSO_PATH, *ABC_PATH, *LIBRARY_FILE, CURRENT_PATH[stringLimit], *FOLDER_NAME;

extern char TRIVIAL_ENCODING_FILE[FILENAME_LENGTH], CONSTRAINTS_FILE[FILENAME_LENGTH], TMP_FILE[FILENAME_LENGTH], SCRIPT_PATH[FILENAME_LENGTH];

// ANDREY'S TOOL
extern GRAPH_TYPE *g;
extern int n;
extern char s[stringLimit];

extern int V;
extern map<string, int> eventNames;
extern string eventNames_str[eventsLimit];
extern map<string, int> eventPredicates[eventsLimit];

extern vector<string> scenarioNames;
extern vector<string> scenarioOpcodes;

extern string ev[eventsLimit][predicatesLimit];
extern string ee[eventsLimit][eventsLimit];
extern map<string, vector<pair<int, int> > > constraints;
extern map<string, vector<pair<int, int> > >::iterator cp, cq;

extern vector<Encoding> encodings;

extern vector<string> cgv;
extern vector<vector<int> > cge;
extern vector<int> literal;
extern vector<int> bestLiteral;

extern string vConditions[eventsLimit][predicatesLimit];
extern string aConditions[eventsLimit][eventsLimit];

// alternative = false: alpha + beta * predicate
// alternative = true : alpha * (beta + predicate)
extern bool alternative;

#endif

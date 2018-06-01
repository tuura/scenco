#if defined(__linux) || defined(__APPLE__)
	#include "verilog-parser.h"
#else
	#include "D:\Projects\SCENCO\inc\verilog-parser.h"
#endif

int buildInterface(int bits){
	int ct;
	int sigs;

	// parse controller
	if ( (ct = countDatapathUnits()) <= 0 ) return -1;
	if ( (sigs = isolateModule(VERILOG_FILE)) <= 0) {
		return -1;
	}
	modVerilog m;
	if (parseModule(sigs, (&m)) != 0) {
		return -1;
	}

	//parse datapath
	for (int i = 0; i < DATAPATH_N; i++) {
		//TODO
	}
	
	return 0;
}

int countDatapathUnits() {

	units = (char**) malloc(sizeof(char*) * nv);
	usage = (int*) malloc(sizeof(int) * nv);
	int found;
	int ct = 0;
	char word[stringLimit];

	for(int i = 0; i<nv; i++ ){
		usage[i] = 0;
		int p = 0;

		while (!isdigit(cpog[i][i].source[p])){
			word[p] = cpog[i][i].source[p];
			p++;
		}
		if (word[p-1] == '/' || word[p-1] == '_' || word[p-1] == '-') p--;
		word[p] = '\0';

		if (!strcmp(word, "GO") || !strcmp(word, "DONE")) continue;

		found = 0;
		for(int j = 0; j<ct; j++){
			if ( !strcmp(word, units[j]) ) {
				usage[j]++;
				found = 1;
			}
		}
		if (found == 0) {
			units[ct] = strdup(word);
			usage[ct] = 1;
			ct++;
		}
	}

	return ct;
}

int parseModule(int sigs, modVerilog* m) {
	char word[stringLimit];
	int n = 0, n2 = 0;

	sscanf(buffer, "%s%n", word, &n);
	(*m).name = strdup(word);

	n2 = n;
	(*m).signals = (char**) malloc(sizeof(char*) * sigs);
	for(int i = 0; i < sigs; i++) {
		sscanf(buffer+n2, "%s%n", word, &n);
		(*m).signals[i] = strdup(word);
		n2 += n;
	}

	free(buffer);
	
	return 0;
}

int isolateModule(char* file_in) {
	FILE *in = NULL;
	int c;
	int sigs = 0;

	// open files
	if ( (in = fopen(file_in, "r")) == NULL) {
		return -1;
	}

	// find module declaration
	if ( findModule(in) != 0 ) {
		fclose(in);
		return -1;
	}


	buffer = strdup("");
	// print module name
	while ( (c = fgetc(in)) != '(' ) {
		if (c == '\n' || c == '\t' || c == ' ') {
			continue;
		}
		buffer = catChar(buffer, c);
	} 
	buffer = catChar(buffer, ' ');

	// print input/outputs
	while ( (c = fgetc(in)) != EOF ) {

		if (c == ',') {
			buffer = catChar(buffer, ' ');
			sigs++;
			continue;
		}
		
		if (c == '\n' || c == '\t' || c == ' ' || c == ')') {
			continue;
		}

		if (c == ';') { 
			break;
		}

		buffer = catChar(buffer, c);
	}

	// close files
	fclose(in);

	return (sigs+1);
}

int removeComments(char* file_in, char* file_out) {
	int c;
	int c2;
	int c3;
	int c4;
	FILE *in = NULL;
	FILE *out = NULL;

	// open files
	if ( (in = fopen(file_in, "r")) == NULL) {
		return -1;
	}
	if ( (out = fopen(file_out, "w")) == NULL) {
		return -1;
	}

	// remove Verilog comments
	while ( (c = fgetc(in)) != EOF) {
		if(c == '/') {
			if ((c2 = fgetc(in)) == '/') {
				while ( (c = fgetc(in)) != '\n');
			} else if (c2 == '*') {
				do {
					if (c4 == EOF) return -1;
					while ( (c3 = fgetc(in)) != '*'){
						if (c3 == EOF) return-1;
					}
				} while ( (c4 = fgetc(in)) != '/');
				continue;

			} else {
				fputc(c, out);
				fputc(c2, out);
				continue;
			}
		}
		fputc(c, out);
	}

	// close files
	fclose(in);
	fclose(out);

	return 0;
}

// reaches "module" keyword in a Verilog file
int findModule(FILE *in) {
	char word[MAX_NAME];

	while ( fscanf(in, "%s", word) != EOF ) {
		int len = strlen(word);
		for (int i=0; i<len; i++) {
			word[i] = tolower(word[i]);
		}
		if (strcmp(word, "module") == 0) {
			return 0;
		}
	}
	return -1;
}

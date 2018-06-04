#if defined(__linux) || defined(__APPLE__)
	#include "verilog-interface.h"
#else
	#include "D:\Projects\SCENCO\inc\verilog-interface.h"
#endif

int buildInterface(int bits){
	int ct;
	int sigs;
	int msigs;
	FILE *fp = NULL;

	// parse controller
	if ( (ct = countDatapathUnits()) <= 0 ) return -1;
	if ( (sigs = isolateModule(VERILOG_FILE)) <= 0) {
		return -1;
	}
	modVerilog controller;
	if ( parseModule(sigs, &controller) != 0) {
		return -1;
	}

	// open implementation file
	if ( (fp = fopen(SYSTEM_FILE, "w")) == NULL ) {
		return -1;
	}

	writeSystemInterface(fp, bits, controller, sigs, ct);

	if (writeDecoupleMerge(fp, controller, sigs, ct) != 0) {
		fclose(fp);
		return -1;
	} 

	if ( !DATAPATH ) {
		fprintf(fp, "end module;");
		fclose(fp);
		return 0;
	}

	//parse datapath
	modVerilog *modules;
	modules = (modVerilog*) malloc( sizeof(modVerilog) * DATAPATH_N);
	for (int i = 0; i < DATAPATH_N; i++) {
		
		if (removeComments(DATAPATH_UNITS[i], VERILOG_TMP) != 0) {
			fclose(fp);
			return -1;
		}
		if ( (msigs = isolateModule(VERILOG_TMP)) <= 0) {
			fclose(fp);
			return -1;
		}
		if (parseModule(msigs, &(modules[i]) ) != 0) {
			fclose(fp);
			return -1;
		}

		if (writeModuleInterface(fp, modules[i], msigs, sigs, ct, controller)) {
			fclose(fp);
			return -1;
		}
	}

	free(modules);
	fprintf(fp, "end module;");
	fclose(fp);
	
	return 0;
}

int writeModuleInterface(FILE *fp, modVerilog m, int sigs, int csigs, int ct, modVerilog c) {
	int mc = 1;
	int sr = 1;
	int sa = 1;

	fprintf(fp, "%s %s%d ( // TODO -- connect data signals\n", m.name, m.name, mc++);
	if ( !lookupUnits(m.name, ct) ){
		for(int i=0; i<sigs; i++) {
			if (i < sigs-1) fprintf(fp, "\t.%s(),\n", m.signals[i]);
			else fprintf(fp, "\t.%s());\n\n", m.signals[i]);
		}
	} else {
		for(int i=0; i<sigs; i++) {
			int len = strlen(m.signals[i]);
			char *word = (char*) malloc(sizeof(char) * len);
			for(int j=0; j<len; j++) word[j] = tolower(m.signals[i][j]);
			word[len] = '\0';

			if ( !strcmp(word, "req")) { // connect request
				sr = 0;
				fprintf(fp,"\t.%s(REQ_%s_datapath)", m.signals[i], m.name);
			} else if ( !strcmp(word, "ack")) { // connect ack if present
				sa = 0;
				fprintf(fp,"\t.%s(ACK_%s_datapath)", m.signals[i], m.name);
			} else if ( lookUpSignal(word, c.signals, csigs) == 1 ) { // connect conditions
				fprintf(fp, "\t.%s(%s_wire)", m.signals[i], m.signals[i]);
			} else { // connect remaining signals
				fprintf(fp, "\t.%s()", m.signals[i]);
			}
			if (i < sigs-1) fprintf(fp, ",\n");
			else fprintf(fp, ");\n\n");

			free(word);
		}
		if (!sr && sa) { // if synchronous component, put delay element
			fprintf(fp, "delay del_%s%d (", m.name, mc-1);
			fprintf(fp, ".IN(REQ_%s_datapath), .OUT(ACK_%s_datapath));\n\n", m.name, m.name);
		}

	}
	return 0;
}

int lookUpSignal(char *signal, char** signals, int cs) {

	for (int i=0; i<cs; i++) {
		if (strcmp(signal, signals[i]) == 0) return 1;
	}

	return 0;
}

int lookupUnits(char* mname, int ct) {

	for (int i=0; i<ct; i++) {
		if (strcmp(mname, units[i]) == 0) return 1;
	}

	return 0;
}

int writeDecoupleMerge(FILE *fp, modVerilog controller, int sigs, int  ct) {

	int decouple = 0;
	int merge = 0;
	char *word;

	for (int i=0; i<ct; i++) {
		if (usage[i] == 1) { // decouple
			word = getNameUnit(units[i], controller.signals, sigs);
			if (word == NULL) return -1;
			fprintf(fp, "wire REQ_%s_datapath, ACK_%s_datapath;\n", word, word);
			fprintf(fp, "decouple D%d (\n", ++decouple);

			fprintf(fp, "\t.REQ_IN(REQ_%s_wire),\n", word);
			fprintf(fp, "\t.ACK_IN(ACK_%s_wire),\n", word);
			fprintf(fp, "\t.REQ_OUT(REQ_%s_datapath),\n", word);
			fprintf(fp, "\t.ACK_OUT(ACK_%s_datapath));\n\n", word);
			free(word);
		} else { // merge
			fprintf(fp, "wire REQ_%s_datapath, ACK_%s_datapath;\n", units[i], units[i]);
			catBitsMerge(fp, REQ, units[i], usage[i], controller, sigs);
			catBitsMerge(fp, ACK, units[i], usage[i], controller, sigs);
			fprintf(fp, "merge M%d #(%d) (\n", ++merge, usage[i]);
			fprintf(fp, "\t.REQ_IN(REQ_%s_merge),\n", units[i]);
			fprintf(fp, "\t.ACK_IN(ACK_%s_merge),\n", units[i]);
			fprintf(fp, "\t.REQ_OUT(REQ_%s_datapath),\n", units[i]);
			fprintf(fp, "\t.ACK_OUT(ACK_%s_datapath));\n\n", units[i]);
		}
	}

	return 0;
}

void catBitsMerge(FILE *fp, Port select, char* module, int bits, modVerilog controller, int sigs) {
	char *word;
	char *word2;
	int comma = 0;
	char* prefix;

	if (select == REQ) prefix = strdup("REQ_");
	else if (select == ACK) prefix = strdup("ACK_");


	fprintf(fp, "wire [%d:0] %s%s_merge <= {", bits-1, prefix, module);
	for(int j=0; j<sigs; j++) {
		int portType = recognisePortController(controller.signals[j]);
		if ( portType != select) {
			continue;
		}
		word = skipChar(controller.signals[j], 4);
		word2 = getModule(word);

		if ( !strcmp(word2, module) ){
			if (comma) fprintf(fp, ", ");
			fprintf(fp, "%s%s_wire", prefix, word);
			comma = 1;
		}

		free(word);
		free(word2);
	}
	fprintf(fp, "};\n");

	free(prefix);
	return;
}

char* getNameUnit(char* unit, char** signals, int sigs) {
	char *n1, *n2;
	
	for(int i=0; i<sigs; i++) {
		int portType = recognisePortController(signals[i]);
		if ( portType < 1 || portType > 2 ) {
			continue;
		}
		n1 = skipChar(signals[i], 4);
		n2 = getModule(n1);
		if ( !strcmp(n2, unit) ) {
			free(n2);
			return n1;
		}
		free(n1);
		free(n2);
	}

	return NULL;
}

int writeSystemInterface(FILE *fp, int bits, modVerilog controller, int sigs, int ct) {
	int b = 0;

	// interface
	fprintf(fp, "%s\n", VERILOG_GENERATED_MESSAGE);
	fprintf(fp, "module %s (\n", VERILOG_SYSTEM_NAME);
	fprintf(fp, "\tGO,\n");
	fprintf(fp, "\tCODE,\n");
	fprintf(fp, "\tDONE\n");
	fprintf(fp, "\t// inputs - TODO\n\n");
	fprintf(fp, "\t// outputs - TODO\n\n");
	fprintf(fp, ");\n\n");

	//wires
	fprintf(fp, "input GO;\n");
	fprintf(fp, "input [%d:0] CODE;\n", bits-1);
	fprintf(fp, "output DONE;\n\n");

	//controller wires
	for (int i=0; i<sigs; i++) {
		char *word;
		Port portType = recognisePortController(controller.signals[i]);

		switch (portType) {
			case REQ: 
				word = skipChar(controller.signals[i],4);
				fprintf(fp, "wire REQ_%s_wire;\n", word);
				free(word);
				break;

			case ACK: 
				word = skipChar(controller.signals[i],4);
				fprintf(fp, "wire ACK_%s_wire;\n", word);
				free(word);
				break;
			case OTHER: // condition
				fprintf(fp, "wire %s_wire;\n", controller.signals[i]);
		}
	}
	fprintf(fp, "\n");


	//module instance
	fprintf(fp, "%s controller (\n", controller.name);
	for (int i=0; i<sigs; i++) {
		char *word;
		Port portType = recognisePortController(controller.signals[i]);

		switch (portType) {
			case CODE:
				fprintf(fp, "\t.%s(CODE[%d])",controller.signals[i], b++);
				break;

			case REQ:
				word = skipChar(controller.signals[i],4);
				fprintf(fp, "\t.%s(REQ_%s_wire)", controller.signals[i], word);
				free(word);
				break;

			case ACK:
				word = skipChar(controller.signals[i],4);
				fprintf(fp, "\t.%s(ACK_%s_wire)", controller.signals[i], word);
				free(word);
				break;

			case GO:
				fprintf(fp,"\t.GO(GO)");
				break;

			case DONE:
				fprintf(fp,"\t.DONE(DONE)");
				break;

			default:
				fprintf(fp,"\t.%s(%s_wire)", controller.signals[i], controller.signals[i]);
				break;
		}
		if (i == sigs-1) fprintf(fp,");\n\n");
		else fprintf(fp, ",\n");
	}
	return 0;
}

int getIndexModule(char *string) {
	int index;
	char *num;
	int len = strlen(string);
	int c=0;
	num = (char*) malloc(sizeof(char) * (len) );

	if( isdigit(string[len-1]) ) {
		len--;
		while ( isdigit(string[len]) ) len--;
		len++;

		for(unsigned int i=len; i<strlen(string); i++) {
			num[c++] = string[i];
		}
		index = atoi(num);
	} else {
		index = -1;
	}

	return index; 
}

char* skipChar(char *string, int s) {
	char *name;
	int len = strlen(string);
	int c = 0;
	name = (char*) malloc(sizeof(char) * (len-s) );

	for (int i=s; i<len;i++){
		name[c++] = string[i];
	}
	name[len-s] = '\0';

	return name; 
}

Port recognisePortController(char* cport) {
	char port[MAX_PORT];

	// recognise bit/GO
	for (int i = 0; i<2; i++) {
		port[i] = cport[i];
	}
	port[2] = '\0';

	// CODE or GO bit
	if ( !strcmp(port, "x_") ) return CODE;
	if ( !strcmp(port, "GO") ) return GO;

	// recognise REQ/ACK/DONE
	for (int i = 2; i<4; i++) {
		port[i] = cport[i];
	}
	port[4] = '\0';

	if ( !strcmp(port, "REQ_") ) return REQ;
	if ( !strcmp(port, "ACK_") ) return ACK;
	if ( !strcmp(port, "DONE") ) return DONE;

	// port not recognised
	return OTHER;
}

int countDatapathUnits() {

	units = (char**) malloc(sizeof(char*) * nv);
	usage = (int*) malloc(sizeof(int) * nv);
	int found;
	int ct = 0;
	char *module, *word;

	for(int i = 0; i<nv; i++ ){
		usage[i] = 0;

		word = strdup(cpog[i][i].source);
		module = getModule(word);

		if (!strcmp(word, "GO") || !strcmp(word, "DONE")) continue;

		found = 0;
		for(int j = 0; j<ct; j++){
			if ( !strcmp(module, units[j]) ) {
				usage[j]++;
				found = 1;
			}
		}
		if (found == 0) {
			units[ct] = strdup(module);
			usage[ct] = 1;
			ct++;
		}

		free(word);
		free(module);
	}

	return ct;
}

char* getModule(char* word) {
	int len;
	char *module;

	module = strdup(word);
	len = strlen(module);

	if( isdigit(module[len-1]) ) {
		len--;
		while ( isdigit(module[len]) ) len--;
		if (module[len] == '/' || module[len] == '_' || module[len] == '-') len--;
		module[len+1] = '\0';
	}
	return module;
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

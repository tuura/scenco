#ifndef __VERILOGINTERFACE_H__
#define __VERILOGINTERFACE_H__

#if defined(__linux) || defined(__APPLE__)
	#include "config.h"
#else
	#include "D:\Projects\SCENCO\inc\config.h"
#endif

int buildInterface(int);
int removeComments(char*, char*);
int isolateModule(char*);
int parseModule(int, modVerilog*);
int findModule(FILE*);
int countDatapathUnits();
int writeSystemInterface(FILE*, int, modVerilog, int, int);
char* getModule(char*);
char* skipChar(char*, int);
Port recognisePortController(char*);
int getIndexModule(char*);
int writeDecoupleMerge(FILE*, modVerilog, int, int);
char* getNameUnit(char*, char**, int);
int writeModuleInterface(FILE*, modVerilog, int, int, int, modVerilog);
int lookupUnits(char*, int);
int lookUpSignal(char *, char**, int);
void catBitsMerge(FILE *, Port, char*, int, modVerilog, int);

#endif


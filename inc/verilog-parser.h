#ifndef __VERILOGPARSER_H__
#define __VERILOGPARSER_H__

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

#endif


#ifndef __MAPPING_H__
#define __MAPPING_H__

#if defined(__linux) || defined(__APPLE__)
	#include "config.h"
#else
	#include "D:\Projects\SCENCO\inc\config.h"
#endif

int equations_abc(int, int);
int equations_abc_cpog_size(int, int);
void replaceVerilogName();
void write_conditions(FILE*,int,int,int,int,int,int);

#endif

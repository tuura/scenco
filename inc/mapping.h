#ifndef __MAPPING_H__
#define __MAPPING_H__

#ifdef __linux
	#include "config.h"
#else
	#include "D:\Projects\PRGM_WORKCRAFT\inc\config.h"
#endif

int equations_abc(int, int);
int equations_abc_cpog_size(int, int);
void write_conditions(FILE*,int,int,int,int,int,int);

#endif

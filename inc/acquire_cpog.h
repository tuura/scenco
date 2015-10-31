#ifndef __ACQUIRE_CPOG_H__
#define __ACQUIRE_CPOG_H__

#ifdef __linux
	#include "config.h"
#else
	#include "D:\Projects\PRGM_WORKCRAFT\inc\config.h"
#endif

int read_file(char*,int*, int*);
int difference_matrix(int, int);
int read_cons(char*, int, int*);
void parsing_cpog(char*,int,int);
int get_conditions_names();

#endif


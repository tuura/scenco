#ifndef __ACQUIRE_CPOG_H__
#define __ACQUIRE_CPOG_H__

#if defined(__linux) || defined(__APPLE__)
	#include "config.h"
#else
	#include "D:\Projects\SCENCO\inc\config.h"
#endif

int read_file(char*,int*, int*);
int difference_matrix(int, int);
int read_cons(char*, int, int*);
void parsing_cpog(char*,int,int);
int get_conditions_names();

#endif


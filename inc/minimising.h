#ifndef __MINIMISING_H__
#define __MINIMISING_H__

#ifdef __linux
	#include "config.h"
#else
	#include "D:\Projects\PRGM_WORKCRAFT\inc\config.h"
#endif

int boolean_function(int,int,int,int);
int decide(char*);
int decoder_minimisation(int bits, int cpog_count);


#endif

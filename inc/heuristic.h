#ifndef __HEURISTIC_H__
#define __HEURISTIC_H__

#ifdef __linux
	#include "config.h"
#else
	#include "D:\Projects\PRGM_WORKCRAFT\inc\config.h"
#endif

float area_encodings_ssd(int, int, float*,int,int);
float weight_function(int,int);
void HD_min_v2(int*,int,int*,int*,int,int,int*,int,int,int);
void HD_min_v3(int*,int,int*,int*,int,int,int*,int,int,int);
void random_opcode_choice_v2(int*,int,int*,int*,int,int,int*,int,int,int);

#endif

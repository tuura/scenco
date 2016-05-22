#ifndef __HEURISTIC_H__
#define __HEURISTIC_H__

#if defined(__linux) || defined(__APPLE__)
	#include "config.h"
#else
	#include "D:\Projects\SCENCO\inc\config.h"
#endif

long long int area_encodings_ssd(int, int, long long int*,int,int);
long long int weight_function(int,int);
void HD_min_v2(int*,int,int*,int*,int,int,int*,int,int,int);
void HD_min_v3(int*,int,int*,int*,int,int,int*,int,int,int);
void random_opcode_choice_v2(int*,int,int*,int*,int,int,int*,int,int,int);

#endif

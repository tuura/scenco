#ifndef __PRGM_HELPER_H__
#define __PRGM_HELPER_H__

#if defined(__linux) || defined(__APPLE__)
	#include "config.h"
#else
	#include "D:\Projects\SCENCO\inc\config.h"
#endif

int parse_arg(int,char**);
void print_help(char*);
int manage_data_base(int,long long int,int,long long int,int,int*);
int read_set_encoding(int, int*);
void removeTempFiles();
void print_version();

#endif

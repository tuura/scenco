#ifndef __PRGM_HELPER_H__
#define __PRGM_HELPER_H__

#ifdef __linux
	#include "config.h"
#else
	#include "D:\Projects\PRGM_WORKCRAFT\inc\config.h"
#endif

int parse_arg(int,char**);
void print_help(char*);
int manage_data_base(int,float,int,float,int,int*);
int read_set_encoding(int, int*);
void removeTempFiles();
void print_version();

#endif

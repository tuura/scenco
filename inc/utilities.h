#ifndef __UTILITIES_H__
#define __UTILITIES_H__

#if defined(__linux) || defined(__APPLE__)
	#include "config.h"
#else
	#include "D:\Projects\SCENCO\inc\config.h"
#endif

int strDCcmp(char*,char*,int);
int eval_function(char*,int);
void print_binary(FILE*, int, int);
int conv_int(char*, int);
int logarithm2(int);
boolean try_swap_fun(int,int,int,int);
double compute_weight(int,int,int);
int compute_HD(int,int, int,int,int,int);
int check_correctness(int, int, int);
void int_to_string_DC(int, int, int, char *);
int filter_encodings(int,int,int);
char* catMem(char*, char*);
char* catChar(char*, char);
int nameTmpFiles();
void printError(const char*);
void printErrorCode(const char*, int);
void fixSpacePath(char*);

// OS dependant functions
#if defined(__linux) || defined(__APPLE__)
	int unixNameTmpFile(char* file);
#else
	int winNameTmpFile(char* file);
#endif

#endif


#ifndef __CONFIG_H__
#define __CONFIG_H__

/*CONFIGURATION DEFINEs*/
//#define ACT_PERCENTAGE		//if present, prints percentages to completion while converting functions
#define ACT_STOP		//Activate stopping generation if they are finished

#define MAX_CPOG	1000	//Max number of graphs of CPOG
#define MAX_WEIGHT	100000000//Used for finding min weight
#define MAX_LOG		10000	//2^(MAX_LOG) as max number of log_2 computable
#define FILENAME_LENGTH	300	// Max length of file names
#define ABC_LINELENGTH 	1000	// Buffer for reading ABC output file
#define MAX_NAME	100	//Max length name for each node
#define MAX_VERT	10000	//Max number of vertices of CPOG
#define MAX_MEMORY	100000    //Max memory available for Workcraft
#define MAX_ENCODINGS	100000	//Max encodings that is possible to try
#define MAX_RAND_ITERATIONS 20	//Number of Rand swapping for shuffling vector
#define MAX_FAILS	10000	//Number of fails before stopping encoding generation

#define stringLimit	1000
#define eventsLimit	1000
#define scenariosLimit	700
#define predicatesLimit	200

/*ESPRESSO CONFIGURATION ENVIRONMENT*/
#define ESPRESSO_FILTER		"-o eqntott "
#define EXTENSION_ENCODING 	".prg"

/*STANDARD LIBRARY*/
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <sys/stat.h>
#include <sys/time.h>
#include <ctime>
#include <cctype>
#include <unistd.h>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <vector>
#include <math.h>
#include <string>
#include <sstream>
#include <map>
#include <set>

using namespace std;

/*TYPE DEFINITION*/
typedef enum {FALSE, TRUE} boolean;
typedef struct struct_tag{
	char type; 		/*v = vertex, e = edge*/
	char* source, *dest; 	/*if n
				  	source = vertex_name
			       	  if e
				  	source = from vertex
				  	dest = to vertex*/
	boolean condition; 	/*if TRUE, a condition exists*/
	char* cond; 		/*condition on vertex*/
	char* truth, *truth_cond;/*truth table*/
	char **fun, **fun_cond; /*boolean function*/
}CPOG_TYPE;

typedef struct Graph_st
{
	int e[eventsLimit][eventsLimit];
	int v[eventsLimit];
	int pred[eventsLimit];
	
	bool transitiveClosure()
	{
		for(int i = 0; i < eventsLimit; i++)
		if (v[i])
			for(int j = 0; j < eventsLimit; j++)
			if (v[j] && e[j][i])
				for(int k = 0; k < eventsLimit; k++)
				if (v[k] && e[i][k]) e[j][k] = 1;
		
		for(int i = 0; i < eventsLimit; i++) if (e[i][i]) return false;
		
		for(int i = 0; i < eventsLimit; i++)
		if (v[i])
			for(int j = 0; j < eventsLimit; j++)
			if (v[j] && e[j][i])
				for(int k = 0; k < eventsLimit; k++)
				if (v[k] && e[i][k]) e[j][k] = 2;

		return true;
	}
	
}GRAPH_TYPE;

typedef struct Encoding_st
{
	string constraint;
	
	bool trivial;
	int constant;
	
	int literal;
	bool inverted;
}Encoding;

/*PROJECT INCLUDES*/
#ifdef __linux
	#include "global.h"
	#include "heuristic.h"
	#include "acquire_cpog.h"
	#include "mapping.h"
	#include "minimising.h"
	#include "permutations.h"
	#include "prgm_helper.h"
	#include "utilities.h"
	#include "simulated_annealing.h"
	#include "building_cpog.h"
#else
	#include "D:\Projects\PRGM_WORKCRAFT\inc\global.h"
	#include "D:\Projects\PRGM_WORKCRAFT\inc\heuristic.h"
	#include "D:\Projects\PRGM_WORKCRAFT\inc\acquire_cpog.h"
	#include "D:\Projects\PRGM_WORKCRAFT\inc\mapping.h"
	#include "D:\Projects\PRGM_WORKCRAFT\inc\minimising.h"
	#include "D:\Projects\PRGM_WORKCRAFT\inc\permutations.h"
	#include "D:\Projects\PRGM_WORKCRAFT\inc\prgm_helper.h"
	#include "D:\Projects\PRGM_WORKCRAFT\inc\utilities.h"
	#include "D:\Projects\PRGM_WORKCRAFT\inc\simulated_annealing.h"
	#include "D:\Projects\PRGM_WORKCRAFT\inc\building_cpog.h"
#endif

#endif

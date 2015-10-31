#ifndef __BUILDING_CPOG_H__
#define __BUILDING_CPOG_H__

#ifdef __linux
	#include "config.h"
#else
	#include "D:\Projects\PRGM_WORKCRAFT\inc\config.h"
#endif

int getEventID(string);
int getPredicateID(int, string);
string getPredicateName(int, int);
bool ME(int, int, int);
bool check(string);
bool readScenario();
bool encode(int, int, int);

#endif


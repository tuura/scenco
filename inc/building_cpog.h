#ifndef __BUILDING_CPOG_H__
#define __BUILDING_CPOG_H__

#if defined(__linux) || defined(__APPLE__)
	#include "config.h"
#else
	#include "D:\Projects\SCENCO\inc\config.h"
#endif

int getEventID(string);
int getPredicateID(int, string);
string getPredicateName(int, int);
bool ME(int, int, int);
bool check(string);
bool readScenario();
bool encode(int, int, int);

#endif


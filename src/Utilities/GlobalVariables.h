#ifndef __GLOBAL_VARIABLES_H__
#define __GLOBAL_VARIABLES_H__

#include "src/Utilities/PathFinder.h"
#include <functional>

class BasicUnit;
//PathFinder<BasicUnit> * pf;

#define PI 3.14159265;
static int GREEN_HEALTH_Z_ORDER = 100;
static int RED_HEALTH_Z_ORDER = 99;

int numSounds = 0;
static void decNumSounds(){ numSounds--; }

#endif // __GLOBAL_VARIABLES_H__

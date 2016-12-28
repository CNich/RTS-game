#ifndef __GLOBAL_VARIABLES_H__
#define __GLOBAL_VARIABLES_H__

#include "src/Utilities/PathFinder.h"
#include <functional>

class BasicUnit;
//PathFinder<BasicUnit> * pf;

#define PI 3.14159265;
static int GREEN_HEALTH_Z_ORDER = 100;
static int RED_HEALTH_Z_ORDER = 99;
static float DISPLAY_TIME_SPLASH_SCREEN = 1.0f;
static float TRANSITION_TIME = 1.0f;
extern int numSounds;

#define COCOS2D_DEBUG 1

extern int testGlobalVariable;

void cPrint(const char * format, ...);

//static void decNumSounds(){ numSounds--; }

#endif // __GLOBAL_VARIABLES_H__

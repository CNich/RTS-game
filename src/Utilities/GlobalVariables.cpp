#include "src/Utilities/GlobalVariables.h"

int testGlobalVariable = 0;
int numSounds = 0;

void cPrint(const char * format, ...){
    printf(format);
    printf("\n");
    CCLOG(format);
};

//static void decNumSounds(){ numSounds--; }

#ifndef INPUTARGS__H
#define INPUTARGS__H

#include<stdio.h>
#include<stdlib.h>

#include"..\Headers\macros.h"

typedef enum argType {
    BOOLEAN = 0,
    STRINGPUT
} argType;

typedef struct arg {
    int index;
    argType type;
    int isNecesary; // 1 -> MUST BE TO WORK, otherwise EXIT
    int found;
    char* name;
    char* value;
} arg;

STRUCT_DYNAMIC_ARR_MACRO(arg, dynamicArg);

int registerArg(int argType, char* name, int isNecesary, char* defaultValue);
void checkInuts(int argc, char **argv);
char* getvalueByIndex(int index);

#endif
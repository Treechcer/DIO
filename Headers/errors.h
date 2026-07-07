#ifndef ERRORS__H
#define ERRORS__H

#include<stdio.h>
#include"../Headers/token.h"
#include <stdlib.h>

typedef enum {
    genericLexError = 0,
    unsupportedCharacter,
    genericUnknownError,
    twoDotsFloat,
    divisionByZero,
    ASTERROR,
    UNSUPORTEDCHARACTERSEQUENCE,
    UNKNOWNERROR,
    PARSERNOTFOUNDGOTO,
    UNKNOWNVARIABLETYPE
} ErrorType;

typedef struct {
    char* errorMessage;
    ErrorType errorType;
    Position errorPos; //everything -1 => unknows position
} Error;

void errorOut(Error err);

void warningOut(Error err, int majority);

#endif
#ifndef ERRORSNEW__H
#define ERRORSNEW__H

#include "../Headers/token.h"

void raiseError(char* file, Position pos, char* errorMessage, int errorCode);
void raiseWarning(char* file, Position pos, char* errorMessage);

typedef enum ErrorType { // automatically generated from where it errors out
    LEXERERROR = 1,
    ASTERROR,
    PARSERERROR,
    HELPLIBRARIESERROR,
    UNKNOWNERROR,
} ErrorType;

#endif
#ifndef ERRORSNEW__H
#define ERRORSNEW__H

#include "../Headers/parser.h"

void raiseError(char* file, Position pos, char* errorMessage, int errorCode);
void raiseWarning(char* file, Position pos, char* errorMessage);
void raiseRunTimeError(char* file, Position pos, char* errorMessage, int errorCode, stackTrace st);

typedef enum ErrorType { // automatically generated from where it errors out
    LEXERERROR = 1,
    ASTERROR,
    PARSERERROR,
    HELPLIBRARIESERROR,
    UNKNOWNERROR,
} ErrorType;

#endif
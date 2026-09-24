#ifndef ERRORSNEW__H
#define ERRORSNEW__H

#include "../Headers/token.h"

void raiseError(char* file, Position pos, char* errorMessage);
void raiseWarning(char* file, Position pos, char* errorMessage);

#endif
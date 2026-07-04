#ifndef HELPER_FUNCTIONS__H
#define HELPER_FUNCTIONS__H

#include <stdbool.h>

#include"token.h"

long int convertToLongInt(char* value);
int convertToInt(char* value);
float convertToFloat(char* value);
double convertToDouble(char* value);
bool isDigit(char ch);
bool isAlpha(char c);
Token createToken(char* value, TokenType identifier, Position pos);
Position createPosition(int* start, int* end, int* line, char* file);
size_t getStringSize(char* string);

#endif
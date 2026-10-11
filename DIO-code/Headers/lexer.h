#ifndef LEXER_H
#define LEXER_H

#include "../Headers/dynamic_array.h"

dynamicToken lex(const char* code, char* fileName, dynamicToken toks, int isLast);
//void freeToks(dynamicToken toks);

#endif
#ifndef DYNAMIC_ARRAY_H
#define DYNAMIC_ARRAY_H

#include <stdlib.h>
#include <stdio.h>

#include "../Headers/token.h"
#include "../Headers/macros.h"

STRUCT_DYNAMIC_ARR_MACRO(Token, dynamicToken)
STRUCT_DYNAMIC_ARR_MACRO(int, dynamicInt)
STRUCT_DYNAMIC_ARR_MACRO(char, dynamicChar)

void test();

#endif
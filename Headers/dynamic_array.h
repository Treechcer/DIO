#ifndef DYNAMIC_ARRAY_H
#define DYNAMIC_ARRAY_H

#include <stdlib.h>
#include <stdio.h>

#include "../Headers/token.h"

#define DYN_PUSH(value, arr)\
    if (arr.count >= arr.size){\
        if (arr.size == 0) arr.size = 256;\
        else arr.size *= 2;\
        arr.items = realloc(arr.items, arr.size*sizeof(*arr.items));\
    }\
    arr.items[arr.count++] = value;

#define STRUCT_MACRO(type, name)\
typedef struct {\
    type *items;\
    size_t count;\
    size_t size;\
} name;\

STRUCT_MACRO(Token, dynamicToken)
STRUCT_MACRO(int, dynamicInt)

void test();

#endif
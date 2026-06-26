/*

Type file of dynamic arrays and functiosn for dynamic arrays
source of code: https://www.youtube.com/watch?v=95M6V3mZgrI (Tsoding)

*/

#include <stdlib.h>
#include <stdio.h>

#include "../Headers/token.h"

#define DYN_PUSH(value, arr)\
    if (arr.count >= arr.size){\
        if (arr.size == 0) arr.size = 256;\
        else arr.size *= 2;\
        arr.items = realloc(arr.items, arr.size*sizeof(*arr.items));\
    }\
    arr.items[arr.count++] = value;\

#define STRUCT_MACRO(type, name)\
typedef struct {\
    type *items;\
    size_t count;\
    size_t size;\
} name;

STRUCT_MACRO(Token, dynamicToken)
STRUCT_MACRO(int, dynamicInt)


void test(){
    dynamicInt a = {0, 0, 0};

    DYN_PUSH(10, a);
    DYN_PUSH(11, a);
    DYN_PUSH(12, a);
    DYN_PUSH(13, a);
    DYN_PUSH(14, a);

    for (int i = 0; i < a.count; i++){
        printf("%d\n", a.items[i]);
    }
}
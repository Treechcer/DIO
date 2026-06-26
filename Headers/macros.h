#ifndef MACROS_H
#define MACROS_H

#define DYN_PUSH(value, arr)\
    if (arr.count >= arr.size){\
        if (arr.size == 0) arr.size = 256;\
        else arr.size *= 2;\
        arr.items = realloc(arr.items, arr.size*sizeof(*arr.items));\
    }\
    arr.items[arr.count++] = value;

#define DYN_POP(arr)\
if (arr.size > 0) {arr.size--;arr.count--; arr.items++;}

#define DYN_INDEX_DEL(arr, index)\
if (index < arr.count) {\
    typeof(arr) RANDOMNAME = {0,0,0};\
    for (int i = 0; i < arr.count; i++){\
        if (index == i) continue;\
        DYN_PUSH(arr.items[i], RANDOMNAME);\
    }\
    free(arr.items);\
    arr = RANDOMNAME;\
}\

#define STRUCT_DYNAMIC_ARR_MACRO(type, name)\
typedef struct {\
    type *items;\
    size_t count;\
    size_t size;\
} name;\

#endif
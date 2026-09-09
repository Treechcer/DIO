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

//defined colors 

// Source - https://stackoverflow.com/a/3219471
// Posted by Andrejs Cainikovs, modified by community. See post 'Timeline' for change history
// Retrieved 2026-06-30, License - CC BY-SA 2.5

#define ANSI_COLOR_RED "\x1b[31m"
#define ANSI_COLOR_GREEN "\x1b[32m"
#define ANSI_COLOR_YELLOW "\x1b[33m"
#define ANSI_COLOR_BLUE "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN "\x1b[36m"
#define ANSI_COLOR_RESET "\x1b[0m"

#endif
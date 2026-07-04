#include<stdio.h>
#include<string.h>

#include"..\Headers\token.h"

const char* keywords[] = {
    "IF", "ELSEIF", "ELSE", "FOR", "WHILE"
};

int getKeyWord(char* possibleKeyWord){
    //This has a few more runs, but it's okay, it won't bottleneck lexing!

    for (int i = 0; i < (sizeof(keywords) / sizeof(keywords[0])); i++) {
        if (strcmp(keywords[i], possibleKeyWord) == 0) {
            return 1; // Found
        }
    }

    return 0;
}

int isBool(char* word){
    char* bools[] = {"false", "true"};
    for (int i = 0; i < (sizeof(bools) / sizeof(bools[0])); i++) {
        if (strcmp(bools[i], word) == 0) {
            return 1; // returns bool value as int
        }
    }

    return 0;
}

char* strToBoolStr(char* str){//this returns 1 if input string is true
    if (strcmp(str, "true") == 0) {
        return "1";
    }
    return "0";
}
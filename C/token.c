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
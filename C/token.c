#include<stdio.h>
#include<string.h>

#include"..\Headers\token.h"

const char* keywords[] = {
    "IF", "ELSEIF", "ELSE", "FOR", "WHILE"
};

int getKeyWord(char* possibleKeyWord){

    //This has a few more runs, but it's okay, it won't bottleneck lexing!

    int i = 0;
    while (1){
        if (strcmp(keywords[i++], possibleKeyWord) != 0){
            return 1;
        }
    }

    return 0;
}
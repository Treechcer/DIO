#include<stdlib.h>
#include<stdio.h>
#include"..\Headers\token.h"
#include"..\Headers\errors.h"

char* genericMessages[] = { //if message is not provided
    "genericLexError",
    "unsupportedCharacters",
    "genericUnknownError",
    "Flaot values can't have two dots"
};

void errorOut(Error err){
    char* message;
    if (err.errorMessage != NULL && err.errorMessage[0] != '\0'){
        message = err.errorMessage;
    }
    else {
        message = genericMessages[err.errorType];
    }
    
    char* file;
    if (err.errorPos.file != NULL && err.errorPos.file[0] != '\0'){
        file = err.errorPos.file;
    }
    else {
        file = "Unknown file";
    }

    if (*err.errorPos.line != -1 && *err.errorPos.start != -1 && *err.errorPos.end != -1){
        printf("Error: \"%d\" has occored with message:%s\n((%s), %i:%i-%i)", err.errorType, message, file, *err.errorPos.line, *err.errorPos.start, *err.errorPos.end);
    }
    else {
        if (err.errorPos.file != ""){
            printf("Error: \"%d\" has occored with message:%s\n(%s)", err.errorType, message, file);
        }
        else{
            printf("Error: \"%d\" has occored with message:%s", err.errorType, message);
        }
    }

    exit(1);
}
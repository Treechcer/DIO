#include<stdlib.h>
#include<stdio.h>

#include"..\Headers\token.h"
#include"..\Headers\errors.h"
#include"..\Headers\macros.h"
#include"..\Headers\helper_functions.h"

char* genericMessages[] = { //if message is not provided
    "genericLexError",
    "unsupportedCharacters",
    "genericUnknownError",
    "Float values can't have two dots",
    "You can't divide by zero",
    "Non specific error in AST",
};

void errorOut(Error err){
    char* message;
    if (err.errorMessage != NULL){
        if (getStringSize(err.errorMessage) != 0){
            message = err.errorMessage;
        }
        else{
            message = genericMessages[err.errorType];    
        }
    }
    else {
        message = genericMessages[err.errorType];
    }

    char* file;
    if (err.errorPos.file != NULL){
        if ((getStringSize(err.errorPos.file))){
            file = err.errorPos.file;
        }
        else{
            file = "Unknown file";    
        }
    }
    else {
        file = "Unknown file";
    }

    if (err.errorPos.line != NULL && err.errorPos.start != NULL && err.errorPos.end != NULL){
        printf("Error: \"%d\" has occored with message:%s\n((%s), %i:%i-%i)", err.errorType, message, file, *err.errorPos.line, *err.errorPos.start, *err.errorPos.end);
    }
    else {
        if (err.errorPos.file != NULL){
            printf("Error: \"%d\" has occored with message:%s\n(%s)", err.errorType, message, file);
        }
        else{
            printf("Error: \"%d\" has occored with message:%s", err.errorType, message);
        }
    }

    exit(1);
}

void warningOut(Error err, int majority){
    char* message;
    if (err.errorMessage != NULL){
        if (getStringSize(err.errorMessage) != 0){
            message = err.errorMessage;
        }
        else{
            message = genericMessages[err.errorType];    
        }
    }
    else {
        message = genericMessages[err.errorType];
    }

    //higher number = higher severity
    char* cols[] = {
        ANSI_COLOR_GREEN,
        ANSI_COLOR_YELLOW,
        ANSI_COLOR_RED,
    };

    majority = majority%(sizeof(cols)/sizeof(cols[0])); //prevents overflow
    
    printf("%sWARNING : %s %s\n", cols[majority], message, ANSI_COLOR_RESET);
}
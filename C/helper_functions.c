#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "../Headers/token.h"
#include "../Headers/helper_functions.h"

long int convertToLongInt(char* value){
    return atol(value);
}

int convertToInt(char* value){
    return atol(value);
}

float convertToFloat(char* value){
    return atof(value);
}

double convertToDouble(char* value){
    char* endptr;
    return strtod(value, &endptr);
}

bool isDigit(char c){
    if (c >= '0' && c <= '9'){
        return true;
    }
    return false;
}

bool isAlpha(char c){
    return ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'));
}

bool isAlphaString(char* c){
    for (int i = 0; i < strlen(c); i++){
        char ch = *c;
        if (!isAlpha(ch)){
            return 0;
        }
        c++;
    }

    return 1;
}

Token createToken(char* value, TokenType identifier, Position pos){
    return (Token) {.value = value, .identifier = identifier, .pos = pos};
}

Position createPosition(int* start, int* end, int* line, char* file){
    return (Position) {.start = start, .end = end, .line = line, .file = file};
}

size_t getStringSize(const char* string){
    if (string == NULL) return 0;
    const char *ch = string;
    size_t len = 0;
    
    while (*ch != '\0'){
        ch++;
        len++;
    }

    return len;
}

fileReadReturn readFile(char* fName){
    // FILE READ source (modified):
    // Source - https://stackoverflow.com/a/14002993
    // Posted by user529758, modified by community. See post 'Timeline' for change history
    // Retrieved 2026-07-04, License - CC BY-SA 4.0
    FILE* filePointer = fopen(fName, "rb");
    if (filePointer == NULL)
        return (fileReadReturn){.size = 0, .content = "", .exists = 0};

    fseek(filePointer, 0, SEEK_END);
    long fsize = ftell(filePointer);
    fseek(filePointer, 0, SEEK_SET);

    char *mainFile = malloc(fsize + 1);
    fread(mainFile, fsize, 1, filePointer);
    fclose(filePointer);

    mainFile[fsize] = '\0';

    fileReadReturn ret = (fileReadReturn){.size = fsize, .content = mainFile, .exists = 1};
    return ret;
}
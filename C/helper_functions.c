#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "../Headers/token.h"

long int convertToLongInt(char* value){
    return atol(value);
}

long int convertToInt(char* value){
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
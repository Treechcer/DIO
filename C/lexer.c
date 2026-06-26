#include <stdio.h>

#include <string.h>
#include"../Headers/token.h"

Token* lex(const char* code) {
    Token* toks;

    while (strlen(code) > 0){
        char c = *code;
        code++;
        printf("%c\n", c);
    }
}

Token createToken(char* value, TokenType* identifier, Position* pos){
    Token tok = {.value = value, .identifier = identifier, .pos = pos};
    return tok;
}

Position createPosition(int* start, int* end, int* line){
    Position pos = {.start = start, .end = end, .line = line};
    return pos;
}
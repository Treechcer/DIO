#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include"../Headers/token.h"
#include"..\Headers\dynamic_array.h"

Token* lex(const char* code) {
    //TODO: add dynymic array?
    Token* toks;

    int line = 1;
    int char_ = 1;

    while (strlen(code) > 0){
        char c = *code;

        switch (c) {
            case '+':
                //pseudo code - will redo, when dynamic array or something filling the role of it
                
                /*
                
                "toks.append(createToken('+', PLUS, createPosition(char_, char_, line)))"

                /*
                */
                break;
            case '-':
                
                break;
            case '*':
                
                break;
            case '/':
        
                break;
            case '\n':
                line++;
                char_ = 1;
            default:

                break;
        }

        code++;
        char_++;
        printf("%c\n", c);
    }
}

bool isDigit(char ch){
    if (ch == '0' || ch == '1' || ch == '2' || ch == '3' || ch == '4' || ch == '5' || ch == '6' || ch == '7' || ch == '8' || ch == '9'){
        return true;
    }
    return false;
}

Token createToken(char* value, TokenType identifier, Position pos){
    return (Token) {.value = value, .identifier = identifier, .pos = pos};
}

Position createPosition(int* start, int* end, int* line){
    return (Position) {.start = start, .end = end, .line = line};
}
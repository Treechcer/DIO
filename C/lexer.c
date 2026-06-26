#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include"../Headers/token.h"
#include"..\Headers\dynamic_array.h"

Token createToken(char* value, TokenType identifier, Position pos){
    return (Token) {.value = value, .identifier = identifier, .pos = pos};
}

Position createPosition(int* start, int* end, int* line){
    return (Position) {.start = start, .end = end, .line = line};
}

void writeToksOut(dynamicToken tok){
    for(int i = 0; i < tok.count; i++){
        if (tok.items[i].value){
            printf("(%i : %s)", tok.items[i].identifier, tok.items[i].value);
        }
        else{
            printf("(%i)", (tok.items[i].identifier));
        }
    }
}

bool isDigit(char ch){
    if (ch == '0' || ch == '1' || ch == '2' || ch == '3' || ch == '4' || ch == '5' || ch == '6' || ch == '7' || ch == '8' || ch == '9'){
        return true;
    }
    return false;
}

Token* lex(const char* code) {
    //Token* toks;

    dynamicToken toks = {0,0,0};

    int line = 1;
    int charPos_ = 1;

    while (strlen(code) > 0){
        char c = *code;
        Token tok = {0};

        if (isDigit(c)){
            code++;
            charPos_++;
            continue;
        }

        switch (c) {
            case '+':
                tok = createToken("+", PLUS, createPosition(&charPos_, &charPos_, &line));
                break;
            case '-':
                tok = createToken("-", MINUS, createPosition(&charPos_, &charPos_, &line));
                break;
            case '*':
                tok = createToken("*", MUL, createPosition(&charPos_, &charPos_, &line));
                break;
            case '/':
                tok = createToken("/", DIV, createPosition(&charPos_, &charPos_, &line));
                break;
            case '\n':
                line++;
                charPos_ = 1;
                break;
            case ' ':
                break;
            default:

                break;
        }
        if (tok.identifier){
            DYN_PUSH(tok, toks);
        }

        code++;
        charPos_++;
    }

    writeToksOut(toks);
}
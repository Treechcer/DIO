#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include"../Headers/token.h"
#include"..\Headers\dynamic_array.h"
#include"../Headers/errors.h"

Token createToken(char* value, TokenType identifier, Position pos){
    return (Token) {.value = value, .identifier = identifier, .pos = pos};
}

Position createPosition(int* start, int* end, int* line, char* file){
    return (Position) {.start = start, .end = end, .line = line, .file = file};
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

    printf("\n");
}

bool isDigit(char ch){
    if (ch == '0' || ch == '1' || ch == '2' || ch == '3' || ch == '4' || ch == '5' || ch == '6' || ch == '7' || ch == '8' || ch == '9'){
        return true;
    }
    return false;
}

bool isAlpha(char c){
    return ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'));
}

dynamicToken lex(const char* code, char* file) {
    //Token* toks;

    dynamicToken toks = {0,0,0};

    int line = 1;
    int charPos_ = 1;

    while (strlen(code) > 0){
        printf("%i, %i\n", line, charPos_);
        char c = *code;
        Token tok = {0};

        switch (c) {
            case '+':
                tok = createToken("+", PLUS, createPosition(&charPos_, &charPos_, &line, file));
                break;
            case '-':
                tok = createToken("-", MINUS, createPosition(&charPos_, &charPos_, &line, file));
                break;
            case '*':
                tok = createToken("*", MUL, createPosition(&charPos_, &charPos_, &line, file));
                break;
            case '/':
                tok = createToken("/", DIV, createPosition(&charPos_, &charPos_, &line, file));
                break;
            case '(':
                tok = createToken("(", LPAREN, createPosition(&charPos_, &charPos_, &line, file));
                break;
            case ')':
                tok = createToken(")", RPAREN, createPosition(&charPos_, &charPos_, &line, file));
                break;
            case '\n':
                line++;
                charPos_ = 1;
                break;
            case '=':
                tok = createToken("=", EQUALS, createPosition(&charPos_, &charPos_, &line, file));
            case ' ':
                break;
            default:
                    if (isDigit(c)){
                        dynamicChar token = {0,0,0};
                        int isFloat = false;
                        while(isDigit(c) || c == '.'){
                            if (c == '.') {
                                if (isFloat) {
                                    errorOut((Error){"", twoDotsFloat, createPosition(&charPos_, &charPos_, &line, file)});
                                } else {
                                    isFloat = 1;
                                }
                            }
                            //printf("%s : %c\n", "digit char", c);
                            DYN_PUSH(c, token);

                            code++;
                            charPos_++;
                            c = *code;
                        }

                        //for (int i = 0; i < token.count; i++){
                        //    printf("%c", token.items[i]);
                        //}

                        //printf("\n");
                        DYN_PUSH('\0', token);
                        tok = createToken(token.items, isFloat ? FLOAT : INT, createPosition(&charPos_, &charPos_, &line, file));
                    }
                    else if (isAlpha(c)){
                        dynamicChar token = {0,0,0};
                        while(isAlpha(c)){
                            //printf("%s : %c\n", "non-digit char", c);
                            DYN_PUSH(c, token);

                            code++;
                            charPos_++;
                            c = *code;
                        }

                        //for (int i = 0; i < token.count; i++){
                        //    printf("%c", token.items[i]);
                        //}
                        
                        //printf("\n");
                        DYN_PUSH('\0', token);
                        if (getKeyWord(token.items)){
                            tok = createToken(token.items, KEYWORD, createPosition(&charPos_, &charPos_, &line, file));
                        }
                        else {
                            tok = createToken(token.items, IDENTIFIER, createPosition(&charPos_, &charPos_, &line, file));
                        }
                    }
                    else{
                        errorOut((Error){"", genericLexError, createPosition(&charPos_, &charPos_, &line, file)});
                    }
                break;
        }
        if (tok.identifier){
            DYN_PUSH(tok, toks);
        }

        code++;
        charPos_++;
    }

    writeToksOut(toks);
    return toks;
}
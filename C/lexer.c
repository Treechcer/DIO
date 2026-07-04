#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include"../Headers/token.h"
#include"..\Headers\dynamic_array.h"
#include"../Headers/errors.h"
#include"..\Headers\helper_functions.h"

bool isAllowed(char c){ //special characters that are allowed and handled differently!
    return (c == '<' || c == '>');
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

dynamicToken lex(const char* code, char* fileName) {
    //Token* toks;

    dynamicToken toks = {0,0,0};

    int line = 1;
    int charPos_ = 1;

    while (strlen(code) > 0){
        char c = *code;
        Token tok = {0};

        //printf("%i, %i, %c\n", line, charPos_, c);

        switch (c) {
            case '+':
                tok = createToken("+", PLUS, createPosition(&charPos_, &charPos_, &line, fileName));
                break;
            case '-':
                tok = createToken("-", MINUS, createPosition(&charPos_, &charPos_, &line, fileName));
                break;
            case '*':
                tok = createToken("*", MUL, createPosition(&charPos_, &charPos_, &line, fileName));
                break;
            case '/':
                tok = createToken("/", DIV, createPosition(&charPos_, &charPos_, &line, fileName));
                break;
            case '^':
                tok = createToken("^", POW, createPosition(&charPos_, &charPos_, &line, fileName));
                break;
            case '(':
                tok = createToken("(", LPAREN, createPosition(&charPos_, &charPos_, &line, fileName));
                break;
            case ')':
                tok = createToken(")", RPAREN, createPosition(&charPos_, &charPos_, &line, fileName));
                break;
            case '<':
                code++;
                c = *code;
                if (c == '='){
                    tok = createToken("<=", LESSOREQAUL, createPosition(&charPos_, &charPos_, &line, fileName));
                }
                else{
                    tok = createToken("<", LESSTHAN, createPosition(&charPos_, &charPos_, &line, fileName));
                    code--;
                }
                break;
            case '>':
                code++;
                c = *code;
                if (c == '='){
                    tok = createToken(">=", MOREOREQUAL, createPosition(&charPos_, &charPos_, &line, fileName));
                }
                else if (c == '>') {
                    while (c != '\n'){
                    code++;
                    c = *code; 
                    }
                }
                else{
                    tok = createToken(">", MORETHAN, createPosition(&charPos_, &charPos_, &line, fileName));
                    code--;
                }
                break;
            case '\n':
                tok = createToken("\n", END, createPosition(&charPos_, &charPos_, &line, fileName));
                line++;
                charPos_ = 1;
                break;
            case ';':
                tok = createToken(";", END, createPosition(&charPos_, &charPos_, &line, fileName));
                break;
            case '\0':
                tok = createToken(";", END, createPosition(&charPos_, &charPos_, &line, fileName));
                break;
            case '=':
                code++;
                c = *code;
                if (c == '='){
                    tok = createToken("==", LEFTRIGHTEQUAL, createPosition(&charPos_, &charPos_, &line, fileName));
                }
                else{
                    tok = createToken("=", EQUALS, createPosition(&charPos_, &charPos_, &line, fileName));
                    code--;
                }
                break;
            case ' ':
                break;
            default:
                    if (isDigit(c)){
                        dynamicChar token = {0,0,0};
                        int isFloat = false;
                        while(isDigit(c) || c == '.'){
                            if (c == '.') {
                                if (isFloat) {
                                    errorOut((Error){"", twoDotsFloat, createPosition(&charPos_, &charPos_, &line, fileName)});
                                } else {
                                    isFloat = 1;
                                }
                            }
                            DYN_PUSH(c, token);

                            code++;
                            charPos_++;
                            c = *code;
                        }

                        DYN_PUSH('\0', token);
                        tok = createToken(token.items, isFloat ? FLOAT : INT, createPosition(&charPos_, &charPos_, &line, fileName));

                        code--;
                        charPos_--;
                        c = *code;
                    }
                    else if (isAlpha(c)){
                        dynamicChar token = {0,0,0};
                        while(isAlpha(c)){
                            DYN_PUSH(c, token);

                            code++;
                            charPos_++;
                            c = *code;
                        }
                        
                        DYN_PUSH('\0', token);
                        if (getKeyWord(token.items)){
                            tok = createToken(token.items, KEYWORD, createPosition(&charPos_, &charPos_, &line, fileName));
                        }
                        else if (isBool(token.items)) {
                            tok = createToken(strToBoolStr(token.items), INT, createPosition(&charPos_, &charPos_, &line, fileName));
                        }
                        else {
                            tok = createToken(token.items, IDENTIFIER, createPosition(&charPos_, &charPos_, &line, fileName));
                        }

                        //if I had another token right after keyword it didn't really work, so I added this and it works lol

                        code--;
                        charPos_--;
                        c = *code;
                    }
                    else{
                        errorOut((Error){"", genericLexError, createPosition(&charPos_, &charPos_, &line, fileName)});
                    }
                break;
        }
        if (tok.identifier){
            DYN_PUSH(tok, toks);
        }

        code++;
        charPos_++;
    }

    //writeToksOut(toks);
    return toks;
}
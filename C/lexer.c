#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include"../Headers/token.h"
#include"..\Headers\dynamic_array.h"
#include"../Headers/errors.h"
#include"..\Headers\helper_functions.h"

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

dynamicToken lex(const char* code, char* file) {
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

                        code--;
                        charPos_--;
                        c = *code;
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
                        
                        DYN_PUSH('\0', token);
                        if (getKeyWord(token.items)){
                            tok = createToken(token.items, KEYWORD, createPosition(&charPos_, &charPos_, &line, file));
                        }
                        else {
                            tok = createToken(token.items, IDENTIFIER, createPosition(&charPos_, &charPos_, &line, file));
                        }

                        //if I had another token right after keyword it didn't really work, so I added this and it works lol

                        code--;
                        charPos_--;
                        c = *code;
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

    //writeToksOut(toks);
    return toks;
}
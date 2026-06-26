#ifndef LEXER_H
#define LEXER_H

typedef struct Token Token;

Token* lex(const char* code);

#endif
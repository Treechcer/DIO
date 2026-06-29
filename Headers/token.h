#ifndef TOKEN_H
#define TOKEN_H

typedef struct {
    int* start;
    int* end;
    int* line;
} Position;

typedef enum {
    PLUS = 1,
    MINUS,
    MUL,
    DIV,
    INT,
    FLOAT,
    LPAREN,
    RPAREN,
    IDENTIFIER,
    EQUALS,
} TokenType;


typedef struct Token{
    char* value; //can this point to array? We will see
    TokenType identifier;
    Position pos;
} Token;

#endif
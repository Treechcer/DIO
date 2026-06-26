#ifndef TOKEN_H
#define TOKEN_H

typedef struct {
    int* start;
    int* end;
    int* line;
} Position;

typedef enum {
    PLUS = 0,
    MINUS,
    MUL,
    DIV,
    INT,
    FLOAT,
    LPAREN,
    RPAREN,
} TokenType;


typedef struct{
    const char* value; //can this point to array? We will see
    const TokenType identifier;
    const Position pos;
} Token;

#endif
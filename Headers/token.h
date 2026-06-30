#ifndef TOKEN_H
#define TOKEN_H

typedef struct {
    int* start;
    int* end;
    int* line;
    char* file;
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
    LESSOREQUALS,
    MOREOREQUALS,
    KEYWORD,
} TokenType;

typedef struct Token{
    char* value; //can this point to array? We will see
    TokenType identifier;
    Position pos;
} Token;

int getKeyWord(char* possibleKeyWord);

#endif
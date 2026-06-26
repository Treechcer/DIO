#ifndef token.h
#define token.h

typedef struct {
    int start;
    int end;
    int line;
} Position;

typedef struct{
    const char* value; //can this point to array? We will see
    const char* identifier;
    const Position pos;
} Token;

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

#endif
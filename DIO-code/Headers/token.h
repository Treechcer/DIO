#ifndef TOKEN_H
#define TOKEN_H

typedef struct {
    int* start;
    int* end;
    int* line;
    char* file;
} Position;

#include "../Headers/macros.h"

typedef enum {
    PLUS = 1,
    MINUS,
    MUL,
    DIV,
    INT,
    FLOAT,
    LESSTHAN,
    MORETHAN,
    LESSOREQAUL,
    MOREOREQUAL,
    LEFTRIGHTEQUAL,
    LPAREN,
    RPAREN,
    IDENTIFIER,
    EQUALS,
    KEYWORD,
    POW,
    END,
    GOTONAME,
    COMMA,
    COLON,
    MAYBE,
    QUOTE,
    SPACE,
    LSQUAREPAREN,
    RSQUAREPAREN,
    STRINGLITERAL,
    LSQUIGLYPAREN,
    RSQUIGLYPAREN,
    FUNCTIONRETURNTYPE,
} TokenType;

typedef struct Token{
    char* value; //can this point to array? We will see
    TokenType identifier;
    Position pos;
} Token;

typedef struct Macro {
    char* name;
    char* text;
} Macro;

typedef struct FileImport {
    char* name;
    int loadedTimes;
    int canLoadMoreThanOnce;
    int index;
} FileImport;

int getKeyWord(char* possibleKeyWord);
int isBool(char* word);
char* strToBoolStr(char* str);
int isMaybe(char* word);

STRUCT_DYNAMIC_ARR_MACRO(Macro, dynamicMacro);
STRUCT_DYNAMIC_ARR_MACRO(FileImport, dynamicFileImport);

#endif
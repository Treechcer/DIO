#ifndef TOKEN_H
#define TOKEN_H

typedef struct Position{
    int start;
    int end;
    int line;
    int nameSpace;
    char* file;
} Position;

#include "../Headers/macros.h"
#include <stddef.h>

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
    NOTEQUAL,
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
    STRING,
    ARROWACCESSOR,
    NOTOPERATOR,
    OROPERATOR,
    ANDOPERATOR
} TokenType;

typedef struct {
    char* value; //can this point to array? We will see
    TokenType identifier;
    Position pos;
} Token;

typedef struct {
    char* name;
    char* text;
} Macro;

typedef struct {
    char* name;
    int loadedTimes;
    int canLoadMoreThanOnce;
    int index;
} FileImport;

int getKeyWord(char* possibleKeyWord);
int isBool(char* word);
char* strToBoolStr(char* str);
int isMaybe(char* word);
char* switcherooBool(char* str);

STRUCT_DYNAMIC_ARR_MACRO(Macro, dynamicMacro);
STRUCT_DYNAMIC_ARR_MACRO(FileImport, dynamicFileImport);

#endif
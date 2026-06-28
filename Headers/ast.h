#ifndef AST__H
#define AST__H

#include"..\Headers\token.h"

typedef struct {
    Token tok;
    double value;
} numberNode;

typedef struct binOpNode{
    struct binOpNode* left;
    float valueLeft; //if valueLeft / valueRight is not set, then we have to do binOp because value is either calculated or just number (because that part of operator had just number vaue)
    struct binOpNode* right;
    float valueRight; //in this language all numbers will be float for easier work
    TokenType op; //NOTE: we only use PLUS, MINUS, DIV, MUL    
} binOpNode;

binOpNode* buildAst(dynamicToken toks);

#endif
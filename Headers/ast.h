#ifndef AST__H
#define AST__H

#include"..\Headers\token.h"

typedef struct {
    Token tok;
    double value;
} numberNode;

typedef struct binOpNode{
    union left {
        struct binOpNode* left;
        float value;
    };
    union right {
        struct binOpNode* right;
        float value;  
    };
    TokenType op; //NOTE: we only use PLUS, MINUS, DIV, MUL    
} binOpNode;

typedef struct node {
    union nodeType {
        struct binOpNode* binOpNode;
    };
} node;


binOpNode* buildAst(dynamicToken toks);

#endif
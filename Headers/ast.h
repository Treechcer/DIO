#ifndef AST__H
#define AST__H

#include"..\Headers\token.h"

typedef struct Node Node;

typedef enum {
    BINOPNODE,
    NUMBERNODE
} nodeType;

typedef struct numberNode{
    double value;
} numberNode;

typedef struct binOpNode{
    Node* left;
    Node* right;
    TokenType op; //NOTE: we only use PLUS, MINUS, DIV, MUL    
} binOpNode;

typedef struct Node {
    nodeType type;
    union {
        struct binOpNode* binOpNode;
        struct numberNode* numberNode;
    } data;
} Node;

// function predef
Node* buildAst(dynamicToken toks);

#endif
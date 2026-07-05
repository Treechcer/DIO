#ifndef AST__H
#define AST__H

#include"..\Headers\token.h"

typedef struct Node Node;
STRUCT_DYNAMIC_ARR_MACRO(Node*, dynamicNode);

typedef enum variableTypes {
    INTVAR = 0,
    FLOATVAR,
    STRINGVAR,
    BOOLVAR
} variableTypes;

typedef enum {
    BINOPNODE = 0,
    NUMBERNODE,
    VARIABLENODE,
    GOTONODE,
    GOTOIDENTIFIER,
    PROGRAMNODE,
} nodeType;

typedef struct programNode {
    dynamicNode nodes;
} programNode;

typedef struct numberNode{
    double value;
} numberNode;

typedef struct binOpNode{
    Node* left;
    Node* right;
    TokenType op; //NOTE: we only use PLUS, MINUS, DIV, MUL, POW, <, >, >=, <=
} binOpNode;

typedef struct variableNode {
    variableTypes type;
    char* name;
    Node* value;
} variableNode;

typedef struct gotoNode {
    char* name;
} gotoNode;

typedef struct gotoIdefier {
    char* name;
} gotoIdefier;

typedef struct Node {
    nodeType type;
    union {
        struct binOpNode* binOpNode;
        struct numberNode* numberNode;
        struct programNode* programNode;
        struct variableNode* variableNode;
        struct gotoNode* gotoNode;
        struct gotoIdefier* gotoIdefier;
    } data;
} Node;

// function predef
Node* buildAst(dynamicToken toks);
Node* createNode();

#endif
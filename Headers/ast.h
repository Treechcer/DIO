#ifndef AST__H
#define AST__H

#include"../Headers/token.h"

typedef struct Node Node;
STRUCT_DYNAMIC_ARR_MACRO(Node*, dynamicNode);

typedef enum variableTypes {
    INTVAR = 0,
    FLOATVAR,
    STRINGVAR,
    BOOLVAR,
    UNKNOWNVARTYPE,
} variableTypes;

typedef enum {
    BINOPNODE = 0,
    NUMBERNODE,
    VARIABLENODE,
    GOTONODE,
    GOTOIDENTIFIER,
    CONDITION,
    CODEBLOCK,
    FUNCTION,
    FUNCTIONCALL,
    PROGRAMNODE,
} nodeType;

typedef struct programNode {
    dynamicNode nodes;
} programNode;

typedef struct codeBlock {
    dynamicNode nodes;
} codeBlock;

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
    int initialise;
} variableNode;

typedef struct gotoNode {
    char* name;
    struct Node* binOpNode; // (condition that makes it run)
} gotoNode;

typedef struct gotoIdefier {
    char* name;
} gotoIdefier;

typedef enum{
    IFCONDITION,
    ELSEIFCONDITION,
    ELSECONDITION
} conditionType;

typedef struct condition{
    Node* binOpNode; //Nodes
    Node* codeBlock; //Nodes
    conditionType conditionType;
} condition;

typedef struct function{
    char* name;
    Node* codeBlock;
    Node* inputs[];
    //TODO: Add parametres (also local variables lol)
} function;

typedef struct functionCall {
    int countOfinputs;
    char* name;
    Node* inputs[];
} functionCall;

typedef struct Node {
    nodeType type;
    union {
        struct binOpNode* binOpNode;
        struct numberNode* numberNode;
        struct programNode* programNode;
        struct variableNode* variableNode;
        struct gotoNode* gotoNode;
        struct gotoIdefier* gotoIdefier;
        struct condition* condition;
        struct codeBlock* codeBlock;
        struct function* function;
        struct functionCall* functionCall;
    } data;
} Node;

// function predef
Node* buildAst(dynamicToken toks);
Node* createNode();

#endif
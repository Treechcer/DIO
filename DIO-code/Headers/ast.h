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
    NUMBERARRAY,
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
    LOOPNODE,
    MAYBENODE,
    STRINGNODE,
    NUMBERARRAYNODE,
    ARRAYACESSNODE,
    UNKNOWNNODE,
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
    char* lastIndex; // only used for special stuff (function calls eg.)
    int maxIndex;
    union {
        int isBool; //used for bools so they can't get numbers outside of 0 and 1
    } dataFlags;
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
    Node* alternative;
    conditionType conditionType;
} condition;

typedef struct function{
    char* name;
    Node* codeBlock;
    dynamicNode inputs;
    variableTypes returnType;
} function;

typedef struct functionCall {
    char* name;
    dynamicNode inputs;
} functionCall;

typedef struct loopNode {
    Node* codeBlock;
    Node* binOpNode;
    Node* init;
    Node* endStatement;
    enum loopType {
        WHILE,
        FOR,
    } loopType;
} loopNode;

typedef struct maybeNode{

} maybeNode;

typedef struct stringNode {
    char* value;
    int length;
    int acessIndex;
} stringNode;

typedef struct numberArrayNode {
    double* value;
    int length;
    int acessIndex;
} numberArrayNode;

typedef struct arrayAcessNode {
    int index;
} arrayAcessNode;

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
        struct loopNode* loopNode;
        struct maybeNode* maybeNode;
        struct stringNode* stringNode;
        struct arrayAcessNode* arrayAcessNode;
        struct numberArrayNode* numberArrayNode;
    } data;
    Position* pos; //TODO: init pos!
} Node;

// function predef
Node* buildAst(dynamicToken toks);
Node* createNode();

#endif
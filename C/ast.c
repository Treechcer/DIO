#include<stdio.h>

#include"..\Headers\dynamic_array.h"
#include"..\Headers\ast.h"
#include"..\Headers\helper_functions.h"
#include"..\Headers\macros.h"
#include"..\Headers\errors.h"

int g_index = 0;

//FUNCTION PREDEF

Node* parseExpression(dynamicToken* toks);

//Node* parseExpression(dynamicToken toks);

Node* createNode(){
    return malloc(sizeof(Node));
}

Token checkCurrenToken(dynamicToken* toks){
    if (g_index  < toks->count){
        return toks->items[g_index];
    }
    return (Token){0};
}

Token shiftToken(dynamicToken* toks){
    if (g_index+1 < toks->count){
        g_index++;
        return toks->items[g_index-1];
    }
}

Node* parseFactor(dynamicToken* toks){
    Token tok = checkCurrenToken(toks);

    if (tok.identifier == INT || tok.identifier == FLOAT){
        shiftToken(toks);
        Node* node = createNode();
        node->type = NUMBERNODE;
        node->data.numberNode = malloc(sizeof(numberNode));
        node->data.numberNode->value = convertToDouble(tok.value);

        return node;
    }

    if (tok.identifier == LPAREN){
        shiftToken(toks);
        Node* node = parseExpression(toks);
        if (checkCurrenToken(toks).identifier != RPAREN){
            errorOut((Error){"", genericLexError, createPosition(NULL, NULL, NULL, NULL)});
        }
        shiftToken(toks);
        return node;
    }

    return NULL;
}

Node* parseTerm(dynamicToken* toks){
    Node* left = parseFactor(toks);

    while (checkCurrenToken(toks).identifier == MUL || checkCurrenToken(toks).identifier == DIV || checkCurrenToken(toks).identifier == POW) {
        Token tokOp = shiftToken(toks);
        Node* right = parseFactor(toks);

        Node* pNode = createNode();
        pNode->type = BINOPNODE;
        pNode->data.binOpNode = malloc(sizeof(binOpNode));
        pNode->data.binOpNode->left = left;
        pNode->data.binOpNode->op = tokOp.identifier;
        pNode->data.binOpNode->right = right;

        left = pNode;
    }

    return left;
}

Node* parseExpression(dynamicToken* toks){
    Node* left = parseTerm(toks);

    while (checkCurrenToken(toks).identifier == PLUS || checkCurrenToken(toks).identifier == MINUS || checkCurrenToken(toks).identifier == LESSOREQAUL || checkCurrenToken(toks).identifier == LESSTHAN || checkCurrenToken(toks).identifier == MOREOREQUAL || checkCurrenToken(toks).identifier == MORETHAN) {
        Token tokOp = shiftToken(toks);
        Node* right = parseTerm(toks);

        Node* pNode = createNode();
        pNode->type = BINOPNODE;
        pNode->data.binOpNode = malloc(sizeof(binOpNode));
        pNode->data.binOpNode->left = left;
        pNode->data.binOpNode->op = tokOp.identifier;
        pNode->data.binOpNode->right = right;

        left = pNode;
    }

    return left;
}

Node* parseProgram(dynamicToken* toks) {
    Node* pNode = createNode();
    pNode->type = PROGRAMNODE;
    pNode->data.programNode = malloc(sizeof(programNode));
    
    pNode->data.programNode->nodes.count = 0;
    pNode->data.programNode->nodes.size = 0;
    pNode->data.programNode->nodes.items = NULL;

    while (g_index < (toks->count)-1) {
        //printf("%i : %i\n", g_index, (toks->count)-1);
        Node* node = parseExpression(toks);
        if (checkCurrenToken(toks).identifier == END){
            shiftToken(toks);
        }
        if (node == NULL){
            //printf("ERR: %i : %i\n", g_index, (toks->count)-1);
            errorOut((Error){"", ASTERROR});
        }

        DYN_PUSH(node, pNode->data.programNode->nodes);
    }
    
    return pNode;
}

Node* buildAst(dynamicToken toks){
    return parseProgram(&toks);
}
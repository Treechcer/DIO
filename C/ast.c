#include<stdio.h>

#include"..\Headers\dynamic_array.h"
#include"..\Headers\ast.h"
#include"..\Headers\helper_functions.h"
#include"..\Headers\macros.h"
#include"..\Headers\errors.h"

STRUCT_DYNAMIC_ARR_MACRO(Node, dynamicNode);

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

    while (checkCurrenToken(toks).identifier == MUL || checkCurrenToken(toks).identifier == DIV) {
        Token tokOp = shiftToken(toks);
        Node* right = parseFactor(toks);

        Node* pNode = createNode();
        pNode->type = BINOPNODE;
        pNode->data.binOpNode->left = left;
        pNode->data.binOpNode->op = tokOp.identifier;
        pNode->data.binOpNode->right = right;

        left = right;
    }

    return left;
}

Node* parseExpression(dynamicToken* toks){
    Node* left = parseFactor(toks);

    while (checkCurrenToken(toks).identifier == PLUS || checkCurrenToken(toks).identifier == MINUS) {
        Token tokOp = shiftToken(toks);
        Node* right = parseFactor(toks);

        Node* pNode = createNode();
        pNode->type = BINOPNODE;
        pNode->data.binOpNode->left = left;
        pNode->data.binOpNode->op = tokOp.identifier;
        pNode->data.binOpNode->right = right;

        left = right;
    }

    return left;
}

Node* buildAst(dynamicToken toks){

}
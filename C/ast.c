#include<stdio.h>

#include"..\Headers\dynamic_array.h"
#include"..\Headers\ast.h"
#include"..\Headers\helper_functions.h"

int g_index = 0;

//FUNCTION PREDEF

binOpNode* parseExpression(dynamicToken toks);

binOpNode* createNode(){
    return malloc(sizeof(binOpNode));
}

Token checkCurrenToken(dynamicToken toks){
    if (g_index  < toks.count){
        return toks.items[g_index];
    }
    return (Token){0};
}

Token shiftToken(dynamicToken toks){
    if (g_index+1  < toks.count){
        g_index++;
        return toks.items[g_index-1];
    }
}

binOpNode* parseFactor(dynamicToken toks){
    Token tok = checkCurrenToken(toks);

    //printf("%i\n", g_index);

    if (tok.identifier == INT || tok.identifier == FLOAT){
        shiftToken(toks);
        binOpNode* node = createNode();
        node->left = NULL;
        node->right = NULL;
        node->valueLeft = convertToFloat(tok.value);
        node->valueRight = 0;
        return node;
    }

    if (tok.identifier == LPAREN){
        shiftToken(toks);
        binOpNode* node = parseExpression(toks);
        shiftToken(toks);
        return node;
    }

    return NULL;
}

binOpNode* parseTerm(dynamicToken toks){
    binOpNode* leftNode = parseFactor(toks);

    //printf("%i\n", g_index);

    while (checkCurrenToken(toks).identifier == MUL || checkCurrenToken(toks).identifier == DIV){
        Token operatorTok = shiftToken(toks);
        binOpNode* rightNode = parseFactor(toks);
        binOpNode* parentNode = createNode();
        parentNode->left = leftNode;
        parentNode->op = operatorTok.identifier;
        parentNode->right = rightNode;

        leftNode = parentNode;
    }

    return leftNode;
}

binOpNode* parseExpression(dynamicToken toks){
    binOpNode* leftNode = parseTerm(toks);

    //printf("%i\n", g_index);

    while (checkCurrenToken(toks).identifier == PLUS || checkCurrenToken(toks).identifier == MINUS){
        Token operatorTok = shiftToken(toks);
        binOpNode* rightNode = parseTerm(toks);
        binOpNode* parentNode = createNode();
        parentNode->left = leftNode;
        parentNode->op = operatorTok.identifier;
        parentNode->right = rightNode;

        leftNode = parentNode;
    }

    return leftNode;
}

binOpNode* buildAst(dynamicToken toks){
    binOpNode* start = parseExpression(toks);
    return start;
}
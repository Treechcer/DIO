#include<stdio.h>
#include<string.h>

#include"../Headers/dynamic_array.h"
#include"../Headers/ast.h"
#include"../Headers/helper_functions.h"
#include"../Headers/macros.h"
#include"../Headers/errors.h"
#include"../Headers/token.h"

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
    return (Token){0};
}

Token checkTokenAt(dynamicToken* toks, int offset){
    int index = g_index + offset;
    if (index >= 0 && index < toks->count){
        return toks->items[index];
    }
    return (Token){0};
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

    if (tok.identifier == IDENTIFIER){
        shiftToken(toks);
        Node* node = createNode();
        node->type = VARIABLENODE;
        node->data.variableNode = malloc(sizeof(variableNode));
        node->data.variableNode->name = tok.value;
        node->data.variableNode->type = INTVAR;
        node->data.variableNode->value = NULL;

        return node;
    }

    if (tok.identifier == LPAREN){
        shiftToken(toks);
        Node* node = parseExpression(toks);
        if (checkCurrenToken(toks).identifier != RPAREN){
            errorOut((Error){"", ASTERROR, createPosition(NULL, NULL, NULL, NULL)});
        }
        shiftToken(toks);
        return node;
    }

    return NULL;
}

Node* parseTerm(dynamicToken* toks){
    Node* left = parseFactor(toks);
    if (left == NULL) return NULL;

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
    if (checkCurrenToken(toks).identifier == IDENTIFIER && checkTokenAt(toks, 1).identifier == EQUALS){
        Token nameTok = shiftToken(toks);
        shiftToken(toks);

        Node* value = parseExpression(toks);
        if (value == NULL) {
            return NULL;
        }

        Node* pNode = createNode();
        pNode->type = VARIABLENODE;
        pNode->data.variableNode = malloc(sizeof(variableNode));
        pNode->data.variableNode->name = nameTok.value;
        pNode->data.variableNode->type = INTVAR;
        pNode->data.variableNode->value = value;

        return pNode;
    }

    Node* left = parseTerm(toks);

    while (checkCurrenToken(toks).identifier == PLUS || checkCurrenToken(toks).identifier == MINUS || checkCurrenToken(toks).identifier == LESSOREQAUL || checkCurrenToken(toks).identifier == LESSTHAN || checkCurrenToken(toks).identifier == MOREOREQUAL || checkCurrenToken(toks).identifier == MORETHAN || checkCurrenToken(toks).identifier == LEFTRIGHTEQUAL) {
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

Node* parseNewVariable(dynamicToken* toks){
    Node* pNode = createNode();

    char* tv = checkCurrenToken(toks).value;
    TokenType tokT = UNKNOWNVARTYPE;
    int initialise = 0;
    int createNodeBool = 0;
    char* name;
    Node* value;

    if (checkCurrenToken(toks).identifier == IDENTIFIER && checkTokenAt(toks, 1).identifier == EQUALS){
        createNodeBool = 1;
        name = checkCurrenToken(toks).value;
        shiftToken(toks); // skips name
        shiftToken(toks); // skips =

        tokT = UNKNOWNVARTYPE;
    }
    else if (checkCurrenToken(toks).identifier == KEYWORD && (strcmp(tv, "int") == 0 || strcmp(tv, "float") == 0 || strcmp(tv, "bool") == 0)){
        createNodeBool = 1;
        shiftToken(toks); // skips int | float ........
        name = checkCurrenToken(toks).value;
        shiftToken(toks); // skips name
        shiftToken(toks); // skips =
        
        if (strcmp(tv, "int") == 0){
            tokT = INTVAR;
        }
        else if (strcmp(tv, "float") == 0) {
            tokT = FLOATVAR;
        }
        else{
            tokT = BOOLVAR;
        }
        initialise = 1;
    }

    if (createNodeBool){
        value = parseExpression(toks);
        if (value == NULL) {
            return NULL; 
        }

        Node* retNode = createNode();
        retNode->type = VARIABLENODE;
        retNode->data.variableNode = malloc(sizeof(variableNode));
        retNode->data.variableNode->name = name;
        retNode->data.variableNode->type = tokT;
        retNode->data.variableNode->value = value;
        retNode->data.variableNode->initialise = initialise;

        return retNode;
    }

    return NULL;
}

Node* parseGoto(dynamicToken* toks){
    Node* pNode = createNode();
    if (toks->items[g_index].identifier == KEYWORD && strcmp(toks->items[g_index].value, "goto") == 0){
        pNode->type = GOTONODE;
        pNode->data.gotoNode = malloc(sizeof(gotoNode));
        shiftToken(toks);
        if (checkCurrenToken(toks).identifier == LPAREN){
            pNode->data.gotoNode->binOpNode = parseExpression(toks);
        }
        else{
            dynamicToken dt = {0,0,0};
            Token tok = (Token){.value = "1 + 0", .identifier = INT};
            DYN_PUSH(tok, dt);
            pNode->data.gotoNode->binOpNode = parseExpression(toks);
        }
        pNode->data.gotoNode->name = shiftToken(toks).value;
        //printf("%s", pNode->data.gotoNode->name);

        return pNode;
    }
    else if (toks->items[g_index].identifier == GOTONAME) {
        pNode->type = GOTOIDENTIFIER;
        pNode->data.gotoIdefier = malloc(sizeof(gotoIdefier));
        pNode->data.gotoIdefier->name = shiftToken(toks).value;

        return pNode;
    }
}

Node* parseProgram(dynamicToken* toks) {
    Node* pNode = createNode();
    pNode->type = PROGRAMNODE;
    pNode->data.programNode = malloc(sizeof(programNode));
    
    pNode->data.programNode->nodes.count = 0;
    pNode->data.programNode->nodes.size = 0;
    pNode->data.programNode->nodes.items = NULL;

    while (g_index < (toks->count)-1) {
        if (checkCurrenToken(toks).identifier == END){
            shiftToken(toks);
            continue;
        }

        Node* node = parseExpression(toks);
        if (node == NULL){
            node = parseNewVariable(toks);
        }
        if (node == NULL){
            node = parseGoto(toks);
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
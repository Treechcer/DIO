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
Node* parseGenericNode(dynamicToken* toks);

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
        printf("%s\n", toks->items[g_index-1].value);
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

    while (checkCurrenToken(toks).identifier == MUL || checkCurrenToken(toks).identifier == DIV) {
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

    return NULL;
}

Node* parseCodeBlock(dynamicToken* toks, nodeType nt){
    Node* pNode = createNode();
    pNode->type = CODEBLOCK;
    pNode->data.codeBlock = malloc(sizeof(codeBlock));
    
    pNode->data.codeBlock->nodes.count = 0;
    pNode->data.codeBlock->nodes.size = 0;
    pNode->data.codeBlock->nodes.items = NULL;

    while (g_index < (toks->count)-1) {
        Token tok = checkCurrenToken(toks);
        if (tok.identifier == KEYWORD && (strcmp(tok.value, "end") == 0 || (nt == CONDITION && (strcmp(tok.value, "elseif") == 0 || strcmp(tok.value, "else") == 0)))){
            shiftToken(toks);
            break;
        }

        if (tok.identifier == END){
            shiftToken(toks);
            continue;
        }

        Node* node = parseGenericNode(toks);

        DYN_PUSH(node, pNode->data.codeBlock->nodes);
    }
    return pNode;
}

Node* parseCondition(dynamicToken* toks){
    Token tok = checkCurrenToken(toks);

    int if_ = strcmp(tok.value, "if") == 0;
    int elseif_ = strcmp(tok.value, "elseif") == 0;
    int else_ = strcmp(tok.value, "else") == 0;

    if (tok.identifier == KEYWORD && (if_ || elseif_ || else_)){
        shiftToken(toks);
        if (strcmp(checkCurrenToken(toks).value, "(") != 0 && strcmp(tok.value, "else") != 0){
            printf("RAISE ERROR LATER");
            exit(1);
        }

        Node* pNode = createNode();
        pNode->type = CONDITION;
        pNode->data.condition = malloc(sizeof(condition));

        pNode->data.condition->binOpNode = malloc(sizeof(binOpNode));
        pNode->data.condition->codeBlock = malloc(sizeof(codeBlock));

        pNode->data.condition->binOpNode = parseExpression(toks);        
        pNode->data.condition->codeBlock = parseCodeBlock(toks, CONDITION);
        
        if (if_){
            pNode->data.condition->conditionType = IFCONDITION;
        }
        else if (elseif_) {
            pNode->data.condition->conditionType = ELSEIFCONDITION;
        }
        else{
            pNode->data.condition->conditionType = ELSECONDITION;
        }

        return pNode;
    }

    return NULL;
}

Node* createFunctionParams(dynamicToken* toks, Node* pNode){
    shiftToken(toks); // name ->
    shiftToken(toks); // ( ->
    dynamicNode nodes = {0,0,0};

    while (checkCurrenToken(toks).identifier != RPAREN){
        //lowkey should work
        Node* n = parseGenericNode(toks);
        DYN_PUSH(n, nodes)
    }

    return pNode;
}

Node* parseFunctionCreate(dynamicToken* toks){
    Token tok = checkCurrenToken(toks);

    if (tok.identifier == KEYWORD && strcmp(tok.value, "def") == 0){
        Node* pNode = createNode();
        pNode->type = FUNCTION;
        pNode->data.function = malloc(sizeof(function));
        pNode->data.function->inputs = (dynamicNode){0,0,0};


        shiftToken(toks); //def
        char* name = checkCurrenToken(toks).value;
        shiftToken(toks); //name
        shiftToken(toks); //(

        while (checkCurrenToken(toks).identifier != RPAREN){
            char* type = shiftToken(toks).value; // int
            if (shiftToken(toks).identifier != COLON){
                printf("%s", checkCurrenToken(toks).value);
                printf("TODO: RAISE ERROR LATER, NO ':'\n");
                exit(1);
            }
            char* name = shiftToken(toks).value;

            //in created fuction we HAVE to define variable, in call we can call prettymuch whatever...

            Node* dynNode = createNode();
            dynNode->type = VARIABLENODE;
            dynNode->data.variableNode = malloc(sizeof(variableNode));
            dynNode->data.variableNode->name = name;

            if (strcmp("int", type) == 0){
                dynNode->data.variableNode->type = INTVAR;
            }
            else if (strcmp("float", type) == 0) {
                dynNode->data.variableNode->type = FLOATVAR;
            }
            else if (strcmp("bool", type) == 0) {
                dynNode->data.variableNode->type = BOOLVAR;
            }
            else{
                printf("TODO: RAISE ERROR, NO VAR TYPE or whatever");
                exit(1);
            }

            printf("%s. %s\n", name, type);
            DYN_PUSH(dynNode, pNode->data.function->inputs);
            if (checkCurrenToken(toks).identifier != RPAREN){
                shiftToken(toks);
            }
        }
        shiftToken(toks);

        //pNode = createFunctionParams(toks, pNode);
        pNode->data.function->name = name;
        pNode->data.function->codeBlock = parseCodeBlock(toks, FUNCTION);

        return pNode;
    }

    return NULL;
}

Node* parseFunctionCall(dynamicToken* toks){
    Token tok = checkCurrenToken(toks);
    if (tok.identifier == IDENTIFIER && checkTokenAt(toks, 1).identifier == LPAREN){
        Node* pNode = createNode();
        pNode->type = FUNCTIONCALL;
        pNode->data.functionCall = malloc(sizeof(functionCall));
        pNode->data.functionCall->name = checkCurrenToken(toks).value;

        pNode = createFunctionParams(toks, pNode);

        return pNode;
    }

    return NULL;
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

        Node* node = parseGenericNode(toks);

        DYN_PUSH(node, pNode->data.programNode->nodes);
    }
    return pNode;
}

Node* parseGenericNode(dynamicToken* toks){
    Node* node = parseFunctionCall(toks);

    if (node == NULL){
        node = parseExpression(toks);        
    }
    if (node == NULL){
        node = parseNewVariable(toks);
    }
    if (node == NULL){
        node = parseGoto(toks);
    }
    if (node == NULL){
        node = parseCondition(toks);
    }
    if (node == NULL){
        node = parseFunctionCreate(toks);
    }
    if (node == NULL){
        //printf("ERR: %i : %i\n", g_index, (toks->count)-1);
        //printf("ERR: %s \n", checkCurrenToken(toks).value);
        errorOut((Error){"", ASTERROR, toks->items[g_index].pos});
    }

    return node;
}

Node* buildAst(dynamicToken toks){
    return parseProgram(&toks);
}
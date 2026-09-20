#include<stdio.h>
#include<string.h>

#include"../Headers/dynamic_array.h"
#include"../Headers/ast.h"
#include"../Headers/helper_functions.h"
#include"../Headers/macros.h"
#include"../Headers/token.h"

int g_index = 0;

//FUNCTION PREDEF

Node* parseExpression(dynamicToken* toks);
Node* parseGenericNode(dynamicToken* toks);
int parseArrayAcessNode(dynamicToken* toks);

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
        //printf("%s\n", toks->items[g_index-1].value);
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

    if (tok.identifier == MINUS){
        Token minus = shiftToken(toks);
        Token num2 = shiftToken(toks);

        Node* node = createNode();
        node->type = NUMBERNODE;
        node->data.numberNode = malloc(sizeof(numberNode));
        node->data.numberNode->value = convertToDouble(num2.value) * (-1);
        node->pos = &tok.pos;

        return node;
    }

    if (strcmp("maybe", tok.value) == 0) {
        Token tok = shiftToken(toks);
        Node* retNode = createNode();
        retNode->type = MAYBENODE;
        retNode->data.maybeNode = malloc(sizeof(maybeNode));
        retNode->pos = &tok.pos;
        return retNode;
    }

    if (tok.identifier == INT || tok.identifier == FLOAT){
        Token tok = shiftToken(toks);
        Node* node = createNode();
        node->type = NUMBERNODE;
        node->data.numberNode = malloc(sizeof(numberNode));
        node->data.numberNode->value = convertToDouble(tok.value);
        node->pos = &tok.pos;

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
        node->pos = &tok.pos;

        return node;
    }

    if (tok.identifier == LPAREN){
        shiftToken(toks);
        Node* node = parseExpression(toks);
        if (checkCurrenToken(toks).identifier != RPAREN){
            raiseErrorMacro(checkCurrenToken(toks).pos, "Right parentheses is missing.")
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
        pNode->pos = &(Position){.start = left->pos->start, .end = right->pos->end, .line = right->pos->line, .end = right->pos->end};

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
        pNode->pos = &(Position){.start = nameTok.pos.start, .end = value->pos->end, .file = value->pos->file, .line = value->pos->line};

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
        pNode->pos = &(Position){.start = left->pos->start, .end = right->pos->end, .line = right->pos->line, .end = right->pos->end};
        left = pNode;
    }

    return left;
}

/*
Position pos => starting pos, that may or may not have an end (it'll be overwritten)
*/
Node* createNumArray(dynamicToken* toks, Position pos){
    if (checkCurrenToken(toks).identifier != LSQUIGLYPAREN){
        return NULL;
    }
    shiftToken(toks); // {

    int arrSize = 0;
    double* values = malloc(arrSize * sizeof(double));

    while(checkCurrenToken(toks).identifier != RSQUIGLYPAREN){
        arrSize++;
        values = realloc(values, arrSize * sizeof(double));
        values[arrSize - 1] = atof(shiftToken(toks).value);
        if (checkCurrenToken(toks).identifier == COMMA){
            shiftToken(toks);
        }
    }

    pos.end = checkCurrenToken(toks).pos.end;

    if (checkCurrenToken(toks).identifier == RSQUIGLYPAREN){
        shiftToken(toks);
    }

    Node* ret = createNode();
    ret->pos = &pos;
    ret->type = NUMBERARRAYNODE;
    ret->data.numberArrayNode = malloc(sizeof(numberArrayNode));
    ret->data.numberArrayNode->value = values;
    ret->data.numberArrayNode->length = arrSize;
    ret->data.numberArrayNode->acessIndex = parseArrayAcessNode(toks); 

    return ret;
}

Node* parseNewVariable(dynamicToken* toks){
    Node* pNode = createNode();

    char* tv = checkCurrenToken(toks).value;
    TokenType tokT = UNKNOWNVARTYPE;
    int initialise = 0;
    int createNodeBool = 0;
    int isBool = 0;
    char* name;
    Node* value;

    if (checkCurrenToken(toks).identifier == IDENTIFIER && checkTokenAt(toks, 1).identifier == EQUALS){
        createNodeBool = 1;
        name = checkCurrenToken(toks).value;
        Position pos = shiftToken(toks).pos; // skips name
        shiftToken(toks); // skips =

        tokT = UNKNOWNVARTYPE;
    }
    else if (checkCurrenToken(toks).identifier == KEYWORD && (strcmp(tv, "int") == 0 || strcmp(tv, "float") == 0 || strcmp(tv, "bool") == 0)){
        if (checkTokenAt(toks, 3).identifier == INT || checkTokenAt(toks, 3).identifier == FLOAT || (checkTokenAt(toks, 3).identifier == MINUS && (checkTokenAt(toks, 4).identifier == INT || checkTokenAt(toks, 4).identifier == FLOAT))){
            createNodeBool = 1;
            Position pos = shiftToken(toks).pos; // skips int | float ........
            name = checkCurrenToken(toks).value;
            shiftToken(toks); // skips name
            shiftToken(toks); // skips =

            if (strcmp(tv, "int") == 0){
                tokT = INTVAR;
            }
            else if (strcmp(tv, "float") == 0) {
                tokT = FLOATVAR;
            }
            else if (strcmp(tv, "bool") == 0){
                tokT = INTVAR;
                isBool = 1;
            }
            initialise = 1;
        }
        else {
            createNodeBool = 1;
            Position pos = shiftToken(toks).pos; // skips int (or float even?)
            name = checkCurrenToken(toks).value;
            shiftToken(toks); // skips name
            shiftToken(toks); // skips =
            //shiftToken(toks); // {
            
            Node* ret = createNumArray(toks, pos);

            Node* retNode = createNode();
            retNode->type = VARIABLENODE;
            retNode->data.variableNode = malloc(sizeof(variableNode));
            retNode->data.variableNode->type = NUMBERARRAY;
            retNode->data.variableNode->value = ret; 
            retNode->data.variableNode->initialise = 1;
            retNode->data.variableNode->name = name;

            return retNode;
        }
        
    }
    else if (checkCurrenToken(toks).identifier == KEYWORD && strcmp(tv, "string") == 0){
        createNodeBool = 1;
        Position pos = shiftToken(toks).pos; // string
        name = checkCurrenToken(toks).value;
        shiftToken(toks); // skips name
        shiftToken(toks); // skips =
        
        tokT = STRINGVAR;
        shiftToken(toks); // '

        char* val;
        int len = 0;
        int pos_ = g_index;

        while (toks->items[pos_++].identifier != QUOTE){
            len += strlen(toks->items[pos_].value);
        }
        char* value = "";
        if (len > 0){
            char* buffer = malloc(len);
            
            strcpy(buffer, checkCurrenToken(toks).value);
            shiftToken(toks);
            while(checkCurrenToken(toks).identifier != QUOTE){
                strcat(buffer, checkCurrenToken(toks).value);
                shiftToken(toks);
            }

            value = buffer;
            //printf("'%s'", buffer);
        }
        //printf("%s\n", value);
        
        //printf("%s", checkCurrenToken(toks).value);
        pos.end = shiftToken(toks).pos.end; //'

        initialise = 1;

        Node* strNode = createNode();
        strNode->type = STRINGNODE;
        strNode->data.stringNode = malloc(sizeof(stringNode));
        strNode->data.stringNode->value = value;
        strNode->data.stringNode->length = len;
        //printf("%s : %s\n", value, strNode->data.stringNode->value);

        Node* retNode = createNode();
        retNode->pos = &pos;
        retNode->type = VARIABLENODE;
        retNode->data.variableNode = malloc(sizeof(variableNode));
        retNode->data.variableNode->name = name;
        retNode->data.variableNode->type = tokT;
        retNode->data.variableNode->value = strNode; 
        retNode->data.variableNode->initialise = initialise;

        return retNode;
    }

    if (createNodeBool){
        value = parseExpression(toks);
        if (value == NULL && strcmp(checkCurrenToken(toks).value, "maybe") == 0){
            printf("Can this ever run?");
        }
        if (value == NULL) {
            return NULL; 
        }

        Node* retNode = createNode();
        retNode->pos = value->pos;
        retNode->type = VARIABLENODE;
        retNode->data.variableNode = malloc(sizeof(variableNode));
        retNode->data.variableNode->name = name;
        retNode->data.variableNode->type = tokT;
        retNode->data.variableNode->value = value;
        retNode->data.variableNode->initialise = initialise;

        //printf("%s\n", name);
        //printf("%i\n", tokT);
        //printf("%s\n", value);
        //printf("%i\n", initialise);

        if (isBool){
            //printf("%f\n", retNode->data.variableNode->value->data.numberNode->value);
            if (retNode->data.variableNode->value->data.numberNode->value > 1){
                retNode->data.variableNode->value->data.numberNode->value = 1;
            }
            else if (retNode->data.variableNode->value->data.numberNode->value < 0) {
                retNode->data.variableNode->value->data.numberNode->value = 0;
            }
        }

        return retNode;
    }

    return NULL;
}

Node* parseGoto(dynamicToken* toks){
    Node* pNode = createNode();
    if (toks->items[g_index].identifier == KEYWORD && strcmp(toks->items[g_index].value, "goto") == 0){
        pNode->pos = &toks->items[g_index].pos;
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
        pNode->pos = &toks->items[g_index].pos;
        pNode->type = GOTOIDENTIFIER;
        pNode->data.gotoIdefier = malloc(sizeof(gotoIdefier));
        pNode->data.gotoIdefier->name = shiftToken(toks).value;

        return pNode;
    }

    return NULL;
}

Node* parseCodeBlock(dynamicToken* toks, nodeType nt){
    //Should this also have start / end? Line can't be same here!
    Node* pNode = createNode();
    pNode->type = CODEBLOCK;
    pNode->data.codeBlock = malloc(sizeof(codeBlock));
    
    pNode->data.codeBlock->nodes.count = 0;
    pNode->data.codeBlock->nodes.size = 0;
    pNode->data.codeBlock->nodes.items = NULL;

    while (g_index < (toks->count)-1) {
        Token tok = checkCurrenToken(toks);
        if (tok.identifier == KEYWORD && (strcmp(tok.value, "end") == 0 || (nt == CONDITION && (strcmp(tok.value, "elseif") == 0 || strcmp(tok.value, "else") == 0)))){
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
        Token t = shiftToken(toks);
        if (strcmp(checkCurrenToken(toks).value, "(") != 0 && strcmp(tok.value, "else") != 0){
            raiseErrorMacro(checkCurrenToken(toks).pos, "Condition or else is missing.");
        }

        Node* pNode = createNode();

        pNode->pos = &t.pos;
        pNode->pos->end = checkCurrenToken(toks).pos.end;

        pNode->type = CONDITION;
        pNode->data.condition = malloc(sizeof(condition));

        pNode->data.condition->codeBlock = malloc(sizeof(codeBlock));
        pNode->data.condition->alternative = NULL;

        if (else_) {
            pNode->data.condition->binOpNode = createNode();
            pNode->data.condition->binOpNode->type = NUMBERNODE;
            pNode->data.condition->binOpNode->data.numberNode = malloc(sizeof(numberNode));
            pNode->data.condition->binOpNode->data.numberNode->value = 1;
        }
        else {
            pNode->data.condition->binOpNode = parseExpression(toks);
        }
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

        while (checkCurrenToken(toks).identifier == KEYWORD && (strcmp(checkCurrenToken(toks).value, "elseif") == 0 || strcmp(checkCurrenToken(toks).value, "else") == 0)) {
            Node* branch = parseCondition(toks);
            pNode->data.condition->alternative = branch;
            break;
        }

        if (checkCurrenToken(toks).identifier == KEYWORD && strcmp(checkCurrenToken(toks).value, "end") == 0) {
            shiftToken(toks);
        }

        return pNode;
    }

    return NULL;
}

dynamicNode createFunctionParams(dynamicToken* toks){
    //does this need handled position? I think it doesn't no?
    dynamicNode nodes = {0,0,0};

    if (checkCurrenToken(toks).identifier == LPAREN){
        shiftToken(toks);
    }

    while (checkCurrenToken(toks).identifier != RPAREN){
        Node* n = parseGenericNode(toks);

        if (n == NULL){
            raiseErrorMacro(checkCurrenToken(toks).pos, "Could't create node for this specific token.");
        }

        if (n->type == VARIABLENODE && checkCurrenToken(toks).identifier == LSQUAREPAREN){
            shiftToken(toks); // [
            n->data.variableNode->lastIndex = checkCurrenToken(toks).value;
            shiftToken(toks); // num
            shiftToken(toks); // ]
        }
        else{
            //n->data.variableNode->lastIndex = "-1";
        }

        DYN_PUSH(n, nodes);

        if (checkCurrenToken(toks).identifier == COMMA){
            shiftToken(toks);
        }
        else if (checkCurrenToken(toks).identifier != RPAREN){
            raiseErrorMacro(checkCurrenToken(toks).pos, "The right parentheses is missing");
        }
    }

    if (checkCurrenToken(toks).identifier == RPAREN){
        shiftToken(toks);
    }

    return nodes;
}

Node* parseFunctionCreate(dynamicToken* toks){
    Token tok = checkCurrenToken(toks);

    if (tok.identifier == KEYWORD && strcmp(tok.value, "def") == 0){
        Node* pNode = createNode();
        pNode->type = FUNCTION;
        pNode->data.function = malloc(sizeof(function));
        pNode->data.function->inputs = (dynamicNode){0,0,0};
        
        Token tempT = checkCurrenToken(toks);
        pNode->pos = &tempT.pos;

        shiftToken(toks); //def
        char* name = checkCurrenToken(toks).value;
        shiftToken(toks); //name
        shiftToken(toks); //(

        while (checkCurrenToken(toks).identifier != RPAREN){
            Token tokType = shiftToken(toks); // tokenType
            char* type = tokType.value; 
            if (shiftToken(toks).identifier != COLON){
                //printf("%s", checkCurrenToken(toks).value);
                raiseErrorMacro(checkCurrenToken(toks).pos, "Missing ':'");
            }
            Token t = shiftToken(toks);
            char* name = t.value;

            //in created fuction we HAVE to define variable, in call we can call prettymuch whatever...

            Node* dynNode = createNode();
            
            dynNode->pos = &tokType.pos;
            dynNode->pos->end = t.pos.end;
            
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
            else if (strcmp("any", type) == 0) {
                dynNode->data.variableNode->type = UNKNOWNVARTYPE;
            }
            else{
                raiseErrorMacro(tokType.pos, "Incorrect name for variable");
            }

            //printf("%s. %s\n", name, type);
            DYN_PUSH(dynNode, pNode->data.function->inputs);
            if (checkCurrenToken(toks).identifier != RPAREN){
                shiftToken(toks);
            }
        }
        shiftToken(toks);

        //pNode = createFunctionParams(toks, pNode);
        pNode->data.function->name = name;
        pNode->data.function->codeBlock = parseCodeBlock(toks, FUNCTION);
        if (checkCurrenToken(toks).identifier == KEYWORD && strcmp(checkCurrenToken(toks).value, "end") == 0) {
            shiftToken(toks);
        }

        return pNode;
    }

    return NULL;
}

Node* parseFunctionCall(dynamicToken* toks){
    Token tok = checkCurrenToken(toks);
    if (tok.identifier == IDENTIFIER && checkTokenAt(toks, 1).identifier == LPAREN){
        char* functionName = shiftToken(toks).value;
        Node* pNode = createNode();
        pNode->type = FUNCTIONCALL;
        pNode->data.functionCall = malloc(sizeof(functionCall));
        pNode->data.functionCall->name = functionName;
        pNode->pos = &tok.pos;
        //printf("-|- %li\n", checkCurrenToken(toks).identifier);
        //This crashes the programme? Why?
        pNode->data.functionCall->inputs = createFunctionParams(toks);

        return pNode;
    }

    return NULL;
}

Node* parseLoop(dynamicToken* toks){
    Token t = checkCurrenToken(toks);
    //printf("%i ; %s\n", t.identifier, t.value);
    if (t.identifier == KEYWORD && strcmp(t.value, "while") == 0){
        Token start = shiftToken(toks); //while
        if (checkCurrenToken(toks).identifier != LPAREN){
            raiseErrorMacro(checkCurrenToken(toks).pos, "Unclosed bracket in loop");
        }
        shiftToken(toks); //(
        Node* binOp = parseExpression(toks);

        Token end = shiftToken(toks); //)

        Node* pNode = createNode();
        pNode->pos = &start.pos;
        pNode->pos->end = end.pos.end;

        pNode->type = LOOPNODE;
        pNode->data.loopNode = malloc(sizeof(loopNode));
        pNode->data.loopNode->codeBlock = parseCodeBlock(toks, LOOPNODE);
        pNode->data.loopNode->binOpNode = binOp;
        pNode->data.loopNode->loopType = WHILE;

        if (checkCurrenToken(toks).identifier == KEYWORD && strcmp(checkCurrenToken(toks).value, "end") == 0) {
            shiftToken(toks);
        }

        return pNode;
    }
    else if (t.identifier == KEYWORD && strcmp(t.value, "for") == 0){
        Token start = shiftToken(toks); //for
        if (checkCurrenToken(toks).identifier != LPAREN){
            raiseErrorMacro(checkCurrenToken(toks).pos, "Unclosed bracket in loop");
        }
        shiftToken(toks); //(
        Node* init = parseGenericNode(toks);
        shiftToken(toks);
        Node* binOp = parseExpression(toks);
        shiftToken(toks);
        Node* endStatement = parseGenericNode(toks);
        
        //printf("%i", init->type);
        //printf("%i", endStatement->type);
        
        Token end = shiftToken(toks); //)

        Node* pNode = createNode();
        pNode->pos = &start.pos;
        pNode->pos->end = end.pos.end;
        
        pNode->type = LOOPNODE;
        pNode->data.loopNode = malloc(sizeof(loopNode));
        pNode->data.loopNode->codeBlock = parseCodeBlock(toks, LOOPNODE);
        pNode->data.loopNode->binOpNode = binOp;
        pNode->data.loopNode->init = init;
        pNode->data.loopNode->endStatement = endStatement;
        pNode->data.loopNode->loopType = FOR;

        if (checkCurrenToken(toks).identifier == KEYWORD && strcmp(checkCurrenToken(toks).value, "end") == 0) {
            shiftToken(toks);
        }

        return pNode;
    }

    return NULL;
}

Node* parseStringGeneral(dynamicToken* toks){
    if (checkCurrenToken(toks).identifier == QUOTE){
        Token start = shiftToken(toks);
        char* val;
        int len = 0;
        int pos = g_index;

        while (toks->items[pos++].identifier != QUOTE){
            len += strlen(toks->items[pos].value);
        }
        char* value = "";
        if (len > 0){
            char* buffer = malloc(len);
            
            strcpy(buffer, checkCurrenToken(toks).value);
            shiftToken(toks);
            while(checkCurrenToken(toks).identifier != QUOTE){
                strcat(buffer, checkCurrenToken(toks).value);
                shiftToken(toks);
            }

            value = buffer;
        }

        Token end = shiftToken(toks); //'

        Node* stringNode = malloc(sizeof(stringNode));
        stringNode->pos = &start.pos;
        stringNode->pos->end = end.pos.end;

        stringNode->type = STRINGNODE;
        stringNode->data.stringNode = malloc(sizeof(stringNode));
        stringNode->data.stringNode->value = value;
        stringNode->data.stringNode->length = len;

        return stringNode;
    }

    return NULL;
}

int parseArrayAcessNode(dynamicToken* toks){
    if (checkCurrenToken(toks).identifier == LSQUAREPAREN && checkTokenAt(toks, 2).identifier == RSQUAREPAREN){
        shiftToken(toks);
        int num = convertToInt(checkCurrenToken(toks).value);
        shiftToken(toks);
        shiftToken(toks);

        //Node* ret = createNode();
        //ret->type = ARRAYACESSNODE;
        //ret->data.arrayAcessNode = malloc(sizeof(arrayAcessNode));
        //ret->data.arrayAcessNode->index = num;
    
        return num;
    }

    return -1;
}

Node* parseProgram(dynamicToken* toks) {
    //printf("AST!\n");
    Node* pNode = createNode();
    pNode->type = PROGRAMNODE;
    pNode->data.programNode = malloc(sizeof(programNode));
    
    pNode->data.programNode->nodes.count = 0;
    pNode->data.programNode->nodes.size = 0;
    pNode->data.programNode->nodes.items = NULL;

    while (g_index < (toks->count)-1) {
        //printf("%i : %li\n", g_index, (toks->count)-1);
        //printf("%s : %li\n", toks->items[g_index].value, toks->items[g_index].identifier);
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
        node = parseStringGeneral(toks);
    }
    if (node == NULL){
        node = parseExpression(toks);
    }
    if (node == NULL){
        node = parseLoop(toks);
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
        //this should probably stay last?
        node = createNumArray(toks, checkCurrenToken(toks).pos);
    }
    if (node == NULL){
        printf("ERR: %i : %li\n", g_index, (toks->count)-1);
        printf("ERR: %s (ID: %i) \n", checkCurrenToken(toks).value, checkCurrenToken(toks).identifier);

        raiseErrorMacro(checkCurrenToken(toks).pos, "Generic error");
    }

    //printf("Node Type: %i\n", node->type);

    return node;
}

Node* buildAst(dynamicToken toks){
    return parseProgram(&toks);
}
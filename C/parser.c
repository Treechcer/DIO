#include<string.h>
#include<math.h>

#include"../Headers/dynamic_array.h"
#include"../Headers/ast.h"
#include"../Headers/errors.h"
#include"../Headers/parser.h"
#include"../Headers/helper_functions.h"

//predef

int getVarIndexByName(char* name);
int getVariableIntValue(int index);
double getVariableFloatValue(int index);
char* getVariableStringValue(int index);
void parseGeneric(Node* node);

dynamicVar g_vars = {0,0,0};
dynamicGoto g_gotos = {0,0,0};
dynamicFunc g_funcs = {0,0,0};
int g_skipelse = 0;

int checkCompatibleVarType(variableTypes var1, variableTypes var2, actionTypes action){
    if ((var1 == INTVAR || var1 == FLOATVAR) && (var2 == INTVAR || var2 == FLOATVAR) && (action == SUM || action == SUB || action == MULT || action == DIVI)){
        return 1;
    }
    else if ((action == CONVERT) && (var1 == INTVAR || var1 == FLOATVAR || var1 == STRINGVAR) && (var2 == INTVAR || var2 == FLOATVAR || var2 == STRINGVAR)){
        //this is more of an generally converting to another type, that shall be handled elsewhere
        return 1;
    }

    return 0;
}

void createLowLevelFunc(char* name){
    funcStruct tempFunc = {.index = g_funcs.count, .name = name, .initialised = 1, .codeBlock = NULL, .isLowLevel = 1};

    dynamicNode inputs = {0,0,0};

    Node* input = createNode();
    input->type = VARIABLENODE;
    input->data.variableNode = malloc(sizeof(variableNode));
    input->data.variableNode->initialise = 1;
    input->data.variableNode->name = "a";
    input->data.variableNode->type = STRINGVAR;
    input->data.variableNode->value = NULL;

    DYN_PUSH(input, inputs)

    tempFunc.inputs = inputs;
    DYN_PUSH(tempFunc, g_funcs);
}

void callLowLevelFunc(int index){
    char* name = g_funcs.items[index].name;
    if (strcmp(name, "out") == 0){
        int varIndex = getVarIndexByName("a");
        if (g_vars.items[varIndex].typedVar == STRINGVAR){
            printf("%s\n", getVariableStringValue(varIndex));
        }
        else if (g_vars.items[varIndex].typedVar == FLOATVAR){
            printf("%.1f\n", getVariableFloatValue(varIndex));
        }
        else if (g_vars.items[varIndex].typedVar == INTVAR){
            printf("%i\n", getVariableIntValue(varIndex));
        }
        else{
            printf("TODO: RAISE ERROR PROPERLY, INCORRECT VAR TYPE");
            exit(1);
        }
    }
}

int getFuncIndexByName(char* name){
    for(int i = 0; i < g_funcs.count; i++){
        if (strcmp(name, g_funcs.items[i].name) == 0){
            return g_funcs.items[i].index;
        }
    }

    return -1;
}

void callFunctionByName(char* name){
    for(int i = 0; i < g_funcs.count; i++){
        if (strcmp(name, g_funcs.items[i].name) == 0){
            parse(g_funcs.items[i].codeBlock);
            break;
        }
    }

}

int isFunctionLowLevel(int index){
    return g_funcs.items[index].isLowLevel;
}

int getVarIndexByName(char* name){
    for(int i = 0; i < g_vars.count; i++){
        if (strcmp(name, g_vars.items[i].name) == 0){
            //printf("%i\n", i);
            return g_vars.items[i].index;
        }
    }
    //printf("-1\n");
    return -1;
}

double getVarValueIntByName(char* name){
    for(int i = 0; i < g_vars.count; i++){
        if (strcmp(name, g_vars.items[i].name) == 0){
            return g_vars.items[i].data.intVal;
        }
    }

    return NAN;
}

double getVarValueByName(char* name){
    for(int i = 0; i < g_vars.count; i++){
        if (strcmp(name, g_vars.items[i].name) == 0){
            if (strcmp(g_vars.items[i].type, "float") == 0){
                return g_vars.items[i].data.floatVal;
            }
            return g_vars.items[i].data.intVal;
        }
    }

    return NAN;
}

double getVariableFloatValue(int index) {
    return g_vars.items[index].data.floatVal;
}

int getVariableIntValue(int index) {
    return g_vars.items[index].data.intVal;
}

char* getVariableStringValue(int index) {
    return g_vars.items[index].data.stringVal;
}

double evalBinOp(Node* node){
    if (node == NULL) return NAN;
    if (node->type == NUMBERNODE) return node->data.numberNode->value;
    if (node->type == VARIABLENODE) {
        if (node->data.variableNode->type == INTVAR || node->data.variableNode->type == FLOATVAR) 
            return getVarValueByName(node->data.variableNode->name);
        
        printf("ERROR!!! NOT CORRECT VARTYPE FOR BINOP, todo: rais correctly error");
        exit(1);
    }

    if (node->type == BINOPNODE){
        double left = evalBinOp(node->data.binOpNode->left);
        double right = evalBinOp(node->data.binOpNode->right);

        switch (node->data.binOpNode->op) {
            case PLUS:
                return left + right;
                break;
            case MINUS:
                return left - right;
                break;
            case MUL:
                return left * right;
                break;
            case DIV:
                if (right == 0) errorOut((Error){NULL, divisionByZero, NULL});
                return left / right;
                break;
            case POW:
                if (right == 0) return 1;
                double num = left;
                for (size_t i = 0; i < right-1; i++){
                    num *= left;
                }
                return num;
            case LESSTHAN:
                return (left < right);
            case LESSOREQAUL:
                return (left <= right);
            case MORETHAN:
                return (left > right);
            case MOREOREQUAL:
                return (left >= right);
            case LEFTRIGHTEQUAL:
                return (left == right);
            default:
                break;
        }
    }

    return 0;
}

/*
double BinOpEvalBinOp(binOpNode* node){
    Node* nNode = createNode();
    nNode->type = BINOPNODE;
    nNode->data.binOpNode = malloc(sizeof(binOpNode));
    nNode->data.binOpNode = node;

    return evalBinOp(nNode);
}
*/
dynamicVar evalVariable(Node* node){
    int varType = node->data.variableNode->type;
    char* name = node->data.variableNode->name;
    varStruct tempVar;

    int existingIndex = getVarIndexByName(name);
    if (varType == UNKNOWNVARTYPE && node->data.variableNode->initialise == 0){
        errorOut((Error){"", UNKNOWNVARIABLETYPE});
    }

    char* tempArr[] = {"int", "float", "string", "bool"};
    char* type = tempArr[varType];

    if (strcmp(type, "int") == 0 || strcmp(type, "float") == 0){
        //if (existingIndex != -1){
        //    //TODO: properly raise error
        //    printf("ERROR, initialise variable twice");
        //    exit(1);
        //}

        //printf("%f\n", evalBinOp(node->data.variableNode->value));

        double value = evalBinOp(node->data.variableNode->value);

        tempVar = (varStruct){.index = g_vars.count, .type = type, .name = name, .data.intVal = value, .intialised = 1, .typedVar = FLOATVAR };
        //printf("%f\n", value);
    }
    else if (strcmp(type, "string") == 0){
        //TODO: implement later
    }

    if (existingIndex >= 0){
        tempVar.index = existingIndex;
        g_vars.items[existingIndex] = tempVar;
        return g_vars;
    }

    DYN_PUSH(tempVar, g_vars);
    return g_vars;
}

gotoStruct createNewGoto(char* name){
    return (gotoStruct){.index = g_gotos.count, .intialised = 1, .name = name};
}

dynamicGoto parseGotoNode(Node* node, dynamicGoto dg){
    DYN_PUSH(createNewGoto(node->data.gotoNode->name), dg);
    return dg;
}

int isGotoIncialised(char* name, dynamicGoto* dg){
    for (int i = 0; i < dg->count; i++){
        //printf("%s, %s\n", dg->items[i].name, name);
        if (strcmp(dg->items[i].name, name) == 0){
            return i;
        }
    }

    return -1;
}

int parseGotoNameNode(Node* node, dynamicGoto* dg, Node* ast){
    if (isGotoIncialised(node->data.gotoNode->name, dg) == -1){
        errorOut((Error){.errorMessage = "Goto not found", .errorType = PARSERNOTFOUNDGOTO});
    }

    if (!evalBinOp(node->data.gotoNode->binOpNode)){
        return -1;
    }

    for (size_t i = 0; i < ast->data.programNode->nodes.count; i++){
        Node* node_ = ast->data.programNode->nodes.items[i];
        switch (node_->type) {
            case GOTOIDENTIFIER:
                if (strcmp(node_->data.gotoIdefier->name, node->data.gotoIdefier->name) == 0){
                    return i;
                }
                break;
            default:
                break;
        }
    }

    errorOut((Error){.errorMessage = "This shouldn't happen. Parser error", .errorType = UNKNOWNERROR});
}

dynamicGoto prescanForGotos(Node* wholeAst, dynamicGoto dg){
    for (size_t i = 0; i < wholeAst->data.programNode->nodes.count; i++){        
        Node* node = wholeAst->data.programNode->nodes.items[i];
        switch (node->type) {
            case GOTONODE:
                dg = parseGotoNode(node, dg);
                break;
            default:
                break;
        }
    }

    return dg;
}

void parseCondition_(Node* node){

    if (node->data.condition->conditionType == IFCONDITION){
        g_skipelse = 0;
    }

    if (evalBinOp(node->data.condition->binOpNode) && g_skipelse == 0){
        g_skipelse = 1;
        parse(node->data.condition->codeBlock);
    }
}

void parseFunction(Node* node){
    if (getFuncIndexByName(node->data.function->name) != -1){
        //TODO: properly raise error
        printf("ERROR, initialise function twice");
        exit(1);
    }

    dynamicNode input = {0,0,0};

    for (size_t i = 0; i < node->data.function->inputs.count; i++){
        DYN_PUSH(node->data.function->inputs.items[i], input);
    }

    funcStruct tempFunc = {.index = g_funcs.count, .name = node->data.function->name, .initialised = 1, .codeBlock = node->data.function->codeBlock, .isLowLevel = 0, .inputs = input};
    DYN_PUSH(tempFunc, g_funcs);
}

void parseLoopNode(Node *node){
    if (node->data.loopNode->loopType == WHILE){
        while (evalBinOp(node->data.loopNode->binOpNode)){
           parse(node->data.loopNode->codeBlock);
        }
    }
    else if (node->data.loopNode->loopType == FOR){
        parseGeneric(node->data.loopNode->init);
        while (evalBinOp(node->data.loopNode->binOpNode)){
           parse(node->data.loopNode->codeBlock);
           parseGeneric(node->data.loopNode->endStatement);
        }
    }
}

void parseFunctionCall_(Node* node){
    int index = getFuncIndexByName(node->data.functionCall->name);
    if (index == -1){
        //TODO: properly raise error
        printf("ERROR, can't call uninitialised function\n");
        exit(1);
    }

    if (node->data.functionCall->inputs.count != g_funcs.items[index].inputs.count){
        printf("ERROR, Incorrect ammount of inputs\n");
        exit(1);
    }

    dynamicVar l_vars = {0,0,0};

    for (size_t i = 0; i < node->data.functionCall->inputs.count; i++){
        //TODO: initialise vars
        //varStruct tempVar = (varStruct){.index = g_vars.count, .type = node->data, .name = name, .data.intVal = value, .intialised = 1 };
        varStruct tempVar;
        if (node->data.functionCall->inputs.items[i]->type == NUMBERNODE){
            tempVar = (varStruct){.index = g_vars.count, .type = "float", .name = g_funcs.items[index].inputs.items[i]->data.variableNode->name, .data.floatVal = node->data.functionCall->inputs.items[i]->data.numberNode->value, .intialised = 1, .typedVar = FLOATVAR };
        }
        else if (node->data.functionCall->inputs.items[i]->type == VARIABLENODE){
            tempVar = (varStruct){.index = g_vars.count, .type = "float", .name = g_funcs.items[index].inputs.items[i]->data.variableNode->name, .data.floatVal = evalBinOp(node->data.functionCall->inputs.items[i]), .intialised = 1, .typedVar = FLOATVAR };
        }
        else if (node->data.functionCall->inputs.items[i]->type == BINOPNODE){
            tempVar = (varStruct){.index = g_vars.count, .type = "float", .name = g_funcs.items[index].inputs.items[i]->data.variableNode->name, .data.floatVal = evalBinOp(node->data.functionCall->inputs.items[i]), .intialised = 1, .typedVar = FLOATVAR };
        }
        else{
            printf("TODO: RAISE ERROR WRONG FORMAT (or not implemented)");
            exit(1);
        }

        int _inx = getVarIndexByName(tempVar.name);
        if (_inx != -1){
            DYN_PUSH(g_vars.items[_inx], l_vars);
            g_vars.items[_inx] = tempVar;
        }

        DYN_PUSH(tempVar, g_vars);
    }

    if (isFunctionLowLevel(index) == 1){
        callLowLevelFunc(index);
    }
    else{
        callFunctionByName(node->data.functionCall->name);
    }

    for (size_t i = 0; i < l_vars.count; i++){
        //This does soemthign like local vars (not really) and "flushes" the value
        int _inx = getVarIndexByName(l_vars.items[i].name);
        g_vars.items[_inx] = l_vars.items[i];
    }
}

Node* astToNode(Node* ast){
    return &(Node){.type = ast->type, .data = ast->data};
}

void parseGeneric(Node* node){
    switch (node->type) {
        case BINOPNODE:
            //printf("%f\n", evalBinOp(node));
            evalBinOp(node);
            break;
        case VARIABLENODE:
            g_vars = evalVariable(node);
            break;
        case GOTOIDENTIFIER:
            g_gotos = parseGotoNode(node, g_gotos);
            break;
        case CONDITION:
            parseCondition_(node);
            break;
        case FUNCTION:
            parseFunction(node);
            //printf("%i\n", i);
            break;
        case FUNCTIONCALL:
            parseFunctionCall_(node);
            break;
        case LOOPNODE:
            parseLoopNode(node);
            break;
        default:
            printf("TODO: ADD THIS NODETYPE : %i", node->type);
            exit(1);
            break;
    }
}

void parse(Node* ast){
    //printf("PARSER");
    g_gotos = prescanForGotos(ast, g_gotos);
    for (size_t i = 0; i < ast->data.programNode->nodes.count; i++){
        Node* node = ast->data.programNode->nodes.items[i];
        if (node->type == GOTONODE){
            int temp = parseGotoNameNode(node, &g_gotos, ast);
            if (temp != -1){
                i = temp-1;
            }
        }
        else{
            parseGeneric(node);
        }
    }
}
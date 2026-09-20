#include <string.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>

#include"../Headers/dynamic_array.h"
#include"../Headers/ast.h"
#include"../Headers/parser.h"
#include"../Headers/helper_functions.h"

//predef

int getVarIndexByName(char* name);
int getVariableIntValue(int index);
double getVariableFloatValue(int index);
char* getVariableStringValue(int index);
void parseGeneric(Node* node);
double* getVariableNumArrayValue(int index);

dynamicVar g_vars = {0,0,0};
dynamicGoto g_gotos = {0,0,0};
dynamicFunc g_funcs = {0,0,0};
int g_skipelse = 0;

int checkCompatibleVarType(variableTypes var1, variableTypes var2, actionTypes action){
    if ((action == INTSUM || action == SUB || action == MULT || action == DIVI) &&  (var1 == INTVAR || var1 == FLOATVAR) && (var2 == INTVAR || var2 == FLOATVAR)){
        return 1;
    }
    else if ((action == CONVERT) && (var1 == INTVAR || var1 == FLOATVAR || var1 == STRINGVAR) && (var2 == INTVAR || var2 == FLOATVAR || var2 == STRINGVAR)){
        //this is more of an generally converting to another type, that shall be handled elsewhere
        return 1;
    }
    else if ((action == FCALL) && ((var1 == UNKNOWNVARTYPE || var2 == UNKNOWNVARTYPE) ||  (var1 == var2) || ((var1 == INTVAR || var1 == FLOATVAR || var1 == NUMBERARRAY) && (var2 == INTVAR || var2 == FLOATVAR || var2 == NUMBERARRAY)))){
        return 1;
    }
    else if ((action == CONCATSUM) && (var1 == STRINGVAR) && (var2 == STRINGVAR)){
        return 1;
    }

    return 0;
}

void createLowLevelFunc(char* name, dynamicNode inputs){
    funcStruct tempFunc = {.index = g_funcs.count, .name = name, .initialised = 1, .codeBlock = NULL, .isLowLevel = 1};
    tempFunc.inputs = inputs;
    DYN_PUSH(tempFunc, g_funcs);
}

void callLowLevelFunc(int index, Node* node){
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
        else if (g_vars.items[varIndex].typedVar == NUMBERARRAY){
            printf("{");
            double* nums = getVariableNumArrayValue(varIndex);
            
            for (int i = 0; i < g_vars.items[varIndex].data.arrayVar.length; i++){
                printf("%.1f", nums[i]);
                if (i != g_vars.items[varIndex].data.arrayVar.length - 1){
                    printf(", ");
                }
            }

            printf("}\n");
        }
        else{
            raiseErrorMacro(*node->pos, "Incorrect type in 'out' function call.");
        }
    }
    else if (strcmp(name, "exec") == 0){
        system(getVariableStringValue(getVarIndexByName("a")));
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
    //TODO: test if this is correct?
    if (name == NULL || g_vars.items == NULL || g_vars.count <= 0) {
        return -1;
    };

    for(int i = g_vars.count - 1; i >= 0; i--){
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
    return g_vars.items[index].data.arrayVar.value.stringValue;
}

double* getVariableNumArrayValue(int index) {
    return g_vars.items[index].data.arrayVar.value.numberValue;
}

int getVariableNumArrayLength(int index) {
    return g_vars.items[index].data.arrayVar.length;
}

binOpResult* evalBinOp(Node* node){
    binOpResult* res = malloc(sizeof(binOpResult));
    if (node == NULL) {
        res->varType = FLOATVAR;
        res->value.floatVar = NAN;
        return res;
    }
    if (node->type == NUMBERNODE) {
        res->varType = FLOATVAR;
        res->value.floatVar = node->data.numberNode->value;
        return res;
    }
    if (node->type == VARIABLENODE) {
        int varIndex = getVarIndexByName(node->data.variableNode->name);
        if (varIndex != -1) {
            if (g_vars.items[varIndex].typedVar == STRINGVAR) {
                res->varType = STRINGVAR;
                res->value.stringVal = getVariableStringValue(varIndex);
                return res;
            } else if (g_vars.items[varIndex].typedVar == INTVAR) {
                res->varType = INTVAR;
                res->value.intVal = getVariableIntValue(varIndex);
                return res;
            } else if (g_vars.items[varIndex].typedVar == FLOATVAR) {
                res->varType = FLOATVAR;
                res->value.floatVar = getVariableFloatValue(varIndex);
                return res;
            }
            else if (g_vars.items[varIndex].typedVar == NUMBERARRAY){
                if (node->data.variableNode->lastIndex == NULL){
                    res->varType = NUMBERARRAY;
                    //res->value.floatVar = getVariableNumArrayValue(varIndex)[g_vars.items[varIndex].data.arrayVar.value.];
                    res->value.numberArrayValue = g_vars.items[varIndex].data.arrayVar.value.numberValue;
                    res->arbitraryData.lenght = g_vars.items[varIndex].data.arrayVar.length;
                    return res;
                }
                else{
                    res->varType = FLOATVAR;
                    int indexArr = getIndexFromInput(node->data.variableNode->lastIndex);
                    if (indexArr == -1){
                        raiseErrorMacro(*node->pos, "Incorrect input in array access.");
                    }
                    res->value.floatVar = g_vars.items[varIndex].data.arrayVar.value.numberValue[indexArr];
                    return res;
                }
            }
        }
        
        raiseErrorMacro(*node->pos, "Incorrect var type for binary operations.");
    }
    if (node->type == MAYBENODE){
        res->varType = FLOATVAR;
        res->value.floatVar = (int) rand() & 1;
        return res;
    }

    if (node->type == BINOPNODE){
        binOpResult* left = evalBinOp(node->data.binOpNode->left);
        binOpResult* right = evalBinOp(node->data.binOpNode->right);

        switch (node->data.binOpNode->op) {
            case PLUS:{
                if (checkCompatibleVarType(left->varType, right->varType, INTSUM)){
                    double vLeft = 0.0, vRight = 0.0;
                    if (left->varType == INTVAR)
                        vLeft = left->value.intVal;
                    else if (left->varType == FLOATVAR)
                        vLeft = left->value.floatVar;
                    if (right->varType == INTVAR)
                        vRight = right->value.intVal;
                    else if (right->varType == FLOATVAR)
                        vRight = right->value.floatVar;

                    res->varType = FLOATVAR;
                    res->value.floatVar = vLeft + vRight;
                    
                    return res;
                }
                else if (checkCompatibleVarType(left->varType, right->varType, CONCATSUM)){
                    char* vLeft = left->value.stringVal;
                    char* vRight = right->value.stringVal;

                    char buffer[strlen(vLeft) + strlen(vRight)];

                    strcpy(buffer, vLeft);
                    strcat(buffer, vRight);
                    
                    res->varType = STRINGVAR;
                    res->value.stringVal = buffer;

                    //printf("%s", buffer);

                    return res;
                }

                raiseErrorMacro(*node->pos, "Incorrect operation PLUS and types.");

                break;
            }
            case MINUS:{
                if (checkCompatibleVarType(left->varType, right->varType, MINUS)){
                    double vLeft = 0.0, vRight = 0.0;;
                    if (left->varType == INTVAR)
                        vLeft = left->value.intVal;
                    else if (left->varType == FLOATVAR)
                        vLeft = left->value.floatVar;
                    if (right->varType == INTVAR)
                        vRight = right->value.intVal;
                    else if (right->varType == FLOATVAR)
                        vRight = right->value.floatVar;

                    res->varType = FLOATVAR;
                    res->value.floatVar = vLeft - vRight;

                    return res;
                }

                raiseErrorMacro(*node->pos, "Incorrect operation MINUS and types.");

                break;
            }
            case MUL:{
                if (checkCompatibleVarType(left->varType, right->varType, MUL)){
                    double vLeft = 0.0, vRight = 0.0;;
                    if (left->varType == INTVAR)
                        vLeft = left->value.intVal;
                    else if (left->varType == FLOATVAR)
                        vLeft = left->value.floatVar;
                    if (right->varType == INTVAR)
                        vRight = right->value.intVal;
                    else if (right->varType == FLOATVAR)
                        vRight = right->value.floatVar;
                    
                    res->varType = FLOATVAR;
                    res->value.floatVar = vLeft * vRight;

                    return res;
                }

                raiseErrorMacro(*node->pos, "Incorrect operation MULTIPLY and types.");

                break;
            }
            case DIV:{
                if (checkCompatibleVarType(left->varType, right->varType, DIV)){
                    double vLeft = 0.0, vRight = 0.0;;
                    if (left->varType == INTVAR)
                        vLeft = left->value.intVal;
                    else if (left->varType == FLOATVAR)
                        vLeft = left->value.floatVar;
                    if (right->varType == INTVAR)
                        vRight = right->value.intVal;
                    else if (right->varType == FLOATVAR)
                        vRight = right->value.floatVar;

                    if (vRight == 0) raiseErrorMacro(*node->pos, "You can't devide with zero.");

                    res->varType = FLOATVAR;
                    res->value.floatVar = vLeft / vRight;

                    return res;
                }

                raiseErrorMacro(*node->pos, "Incorrect operation DIVIDE and types.");

                break;
            }
            case POW:{
                if (checkCompatibleVarType(left->varType, right->varType, MUL)){
                    double vLeft = 0.0, vRight = 0.0;;
                    if (left->varType == INTVAR)
                        vLeft = left->value.intVal;
                    else if (left->varType == FLOATVAR)
                        vLeft = left->value.floatVar;
                    if (right->varType == INTVAR)
                        vRight = right->value.intVal;
                    else if (right->varType == FLOATVAR)
                        vRight = right->value.floatVar;

                    res->varType = FLOATVAR;
                    if (vRight == 0) {
                        res->value.floatVar = 1;
                        return res;
                    }
                    double num = vLeft;
                    for (size_t i = 0; i < vRight-1; i++){
                        num *= vLeft;
                    }

                    res->value.floatVar = num;

                    return res;
                }

                raiseErrorMacro(*node->pos, "Incorrect operation MUL and types.");
            }
            case LESSTHAN:{
                double vLeft = 0.0, vRight = 0.0;;
                if (left->varType == INTVAR)
                    vLeft = left->value.intVal;
                else if (left->varType == FLOATVAR)
                    vLeft = left->value.floatVar;
                if (right->varType == INTVAR)
                    vRight = right->value.intVal;
                else if (right->varType == FLOATVAR)
                    vRight = right->value.floatVar;

                res->varType = FLOATVAR;
                res->value.floatVar = (vLeft < vRight);
                
                return res;
            }
            case LESSOREQAUL:{
                double vLeft = 0.0, vRight = 0.0;;
                if (left->varType == INTVAR)
                    vLeft = left->value.intVal;
                else if (left->varType == FLOATVAR)
                    vLeft = left->value.floatVar;
                if (right->varType == INTVAR)
                    vRight = right->value.intVal;
                else if (right->varType == FLOATVAR)
                    vRight = right->value.floatVar;

                res->varType = FLOATVAR;
                res->value.floatVar = (vLeft <= vRight);
                
                return res;
            }
            case MORETHAN:{
                double vLeft = 0.0, vRight = 0.0;;
                if (left->varType == INTVAR)
                    vLeft = left->value.intVal;
                else if (left->varType == FLOATVAR)
                    vLeft = left->value.floatVar;
                if (right->varType == INTVAR)
                    vRight = right->value.intVal;
                else if (right->varType == FLOATVAR)
                    vRight = right->value.floatVar;

                res->varType = FLOATVAR;
                res->value.floatVar = (vLeft > vRight);
                
                return res;
            }
            case MOREOREQUAL:{
                double vLeft = 0.0, vRight = 0.0;;
                if (left->varType == INTVAR)
                    vLeft = left->value.intVal;
                else if (left->varType == FLOATVAR)
                    vLeft = left->value.floatVar;
                if (right->varType == INTVAR)
                    vRight = right->value.intVal;
                else if (right->varType == FLOATVAR)
                    vRight = right->value.floatVar;

                res->varType = FLOATVAR;
                res->value.floatVar = (vLeft >= vRight);
                
                return res;
            }
            case LEFTRIGHTEQUAL:{
                double vLeft = 0.0, vRight = 0.0;;
                if (left->varType == INTVAR)
                    vLeft = left->value.intVal;
                else if (left->varType == FLOATVAR)
                    vLeft = left->value.floatVar;
                if (right->varType == INTVAR)
                    vRight = right->value.intVal;
                else if (right->varType == FLOATVAR)
                    vRight = right->value.floatVar;

                res->varType = FLOATVAR;
                res->value.floatVar = (vLeft == vRight);

                return res;
            }
            default:
                break;
        }
    }

    res->varType = UNKNOWNVARTYPE;

    return res;
}

dynamicVar evalVariable(Node* node){
    variableTypes varType = node->data.variableNode->type;
    char* name = node->data.variableNode->name;
    varStruct tempVar;

    int existingIndex = getVarIndexByName(name);
    if (varType == UNKNOWNVARTYPE && node->data.variableNode->initialise == 0){
        raiseErrorMacro(*node->pos, "Unknown variable type.");
    }

    //TODO: remake this, this kinda sucks
    //char* tempArr[] = {"int", "float", "string", "bool", "numArr"};
    //char* type = tempArr[varType];

    if (varType == INTVAR || varType == FLOATVAR){
        binOpResult* value = evalBinOp(node->data.variableNode->value);
        if (value->varType == FLOATVAR){
            tempVar = (varStruct){.index = g_vars.count, .type = "float", .name = name, .data.floatVal = value->value.floatVar, .intialised = 1, .typedVar = FLOATVAR };
        }
        else{
            tempVar = (varStruct){.index = g_vars.count, .type = "int", .name = name, .data.intVal = value->value.intVal, .intialised = 1, .typedVar = FLOATVAR };
        }
    }
    else if (varType == STRINGVAR){
        //printf("||%s\n", node->data.variableNode->value->data.stringNode->value);
        tempVar = (varStruct){.index = g_vars.count, .type = "string", .name = name, .data.arrayVar.value.stringValue = node->data.variableNode->value->data.stringNode->value, .data.arrayVar.length = node->data.variableNode->value->data.stringNode->length, .intialised = 1, .typedVar = STRINGVAR };
    }
    else if (varType == NUMBERARRAY){
        tempVar = (varStruct){.index = g_vars.count, .type = "numArr", .name = name, .data.arrayVar.value.numberValue = node->data.variableNode->value->data.numberArrayNode->value, .data.arrayVar.length = node->data.variableNode->value->data.numberArrayNode->length, .intialised = 1, .typedVar = NUMBERARRAY };
    }

    if (existingIndex >= 0){
        //printf("existingIndex: %i (count: %i)\n", existingIndex, g_vars.count);
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
        raiseErrorMacro(*node->pos, "Unknown goto name call.");
    }

    binOpResult* res = evalBinOp(node->data.gotoNode->binOpNode);
    float cond;
    if (res->varType == INTVAR){
        cond = res->value.intVal;
    }
    else if (res->varType == FLOATVAR){
        cond = res->value.floatVar;
    }
    else{
        raiseErrorMacro(*node->pos, "Incorrect var type in goto.");
    }

    if (!cond){
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

    raiseErrorMacro(*node->pos, "This should not happen. GOTO");
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
    if (evalBinOp(node->data.condition->binOpNode)->value.floatVar){
        parse(node->data.condition->codeBlock);
    }
    else if (node->data.condition->alternative != NULL){
        parseCondition_(node->data.condition->alternative);
    }
}

void parseFunction(Node* node){
    if (getFuncIndexByName(node->data.function->name) != -1){
        raiseErrorMacro(*node->pos, "Function intialisation twice.");
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
        while (evalBinOp(node->data.loopNode->binOpNode)->value.floatVar){
            parse(node->data.loopNode->codeBlock);
        }
    }
    else if (node->data.loopNode->loopType == FOR){
        parseGeneric(node->data.loopNode->init);
        //TODO: can this be int?
        while (evalBinOp(node->data.loopNode->binOpNode)->value.floatVar){
           parse(node->data.loopNode->codeBlock);
           parseGeneric(node->data.loopNode->endStatement);
        }
    }
}

void parseFunctionCall_(Node* node){
    int index = getFuncIndexByName(node->data.functionCall->name);
    if (index == -1){
        raiseErrorMacro(*node->pos, "Can't call uninitialised functions.");
    }

    if (node->data.functionCall->inputs.count != g_funcs.items[index].inputs.count){
        raiseErrorMacro(*node->pos, "Incorrect ammount of inputs.");
    }

    dynamicVar l_vars = {0,0,0};

    for (size_t i = 0; i < node->data.functionCall->inputs.count; i++){
        varStruct tempVar;
        if (node->data.functionCall->inputs.items[i]->type == NUMBERNODE){
            tempVar = (varStruct){.index = g_vars.count, .type = "float", .name = g_funcs.items[index].inputs.items[i]->data.variableNode->name, .data.floatVal = node->data.functionCall->inputs.items[i]->data.numberNode->value, .intialised = 1, .typedVar = FLOATVAR };
        }
        else if (node->data.functionCall->inputs.items[i]->type == VARIABLENODE){
            if (g_funcs.items[index].inputs.items[i]->data.variableNode->type == INTVAR || g_funcs.items[index].inputs.items[i]->data.variableNode->type == FLOATVAR || g_funcs.items[index].inputs.items[i]->data.variableNode->type == BOOLVAR){
                binOpResult* res = evalBinOp(node->data.functionCall->inputs.items[i]);
                if (res->varType == FLOATVAR){
                    tempVar = (varStruct){.index = g_vars.count, .type = "float", .name = g_funcs.items[index].inputs.items[i]->data.variableNode->name, .data.floatVal = res->value.floatVar, .intialised = 1, .typedVar = FLOATVAR };
                }
                else if (res->varType == INTVAR){
                    tempVar = (varStruct){.index = g_vars.count, .type = "float", .name = g_funcs.items[index].inputs.items[i]->data.variableNode->name, .data.floatVal = res->value.intVal, .intialised = 1, .typedVar = FLOATVAR };
                }
                else if (res->varType == NUMBERARRAY){
                    tempVar = (varStruct){.index = g_vars.count, .type = "numArr", .name = g_funcs.items[index].inputs.items[i]->data.variableNode->name, .data.arrayVar.value.numberValue = res->value.numberArrayValue, .data.arrayVar.length = res->arbitraryData.lenght, .intialised = 1, .typedVar = NUMBERARRAY };
                }
                else{
                    printf("TEMP ERR");
                    exit(1);
                }
            }
            else if (g_funcs.items[index].inputs.items[i]->data.variableNode->type == STRINGVAR){
                char* value = getVariableStringValue(getVarIndexByName(node->data.functionCall->inputs.items[i]->data.variableNode->name));
                tempVar = (varStruct){.index = g_vars.count, .type = "string", .name = g_funcs.items[index].inputs.items[i]->data.variableNode->name, .data.arrayVar.value.stringValue = value, .data.arrayVar.length = strlen(value), .intialised = 1, .typedVar = STRINGVAR };
            }
            else if (g_funcs.items[index].inputs.items[i]->data.variableNode->type == UNKNOWNVARTYPE){
                int index = getVarIndexByName(node->data.functionCall->inputs.items[i]->data.variableNode->name);
                if (g_vars.items[index].typedVar == INTVAR){
                    double value = getVariableIntValue(index);
                    tempVar = (varStruct){.index = g_vars.count, .type = "int", .name = g_funcs.items[index].inputs.items[i]->data.variableNode->name, .data.intVal = value, .intialised = 1, .typedVar = INTVAR };
                }
                else if (g_vars.items[index].typedVar == FLOATVAR){
                    double value = getVariableFloatValue(index);
                    tempVar = (varStruct){.index = g_vars.count, .type = "float", .name = g_funcs.items[index].inputs.items[i]->data.variableNode->name, .data.floatVal = value, .intialised = 1, .typedVar = FLOATVAR };
                }
                else if (g_vars.items[index].typedVar == STRINGVAR){
                    char* valueOrig = getVariableStringValue(getVarIndexByName(node->data.functionCall->inputs.items[i]->data.variableNode->name));
                    char* value = strdup(valueOrig);
                    int index_ = getVarIndexByName(node->data.functionCall->inputs.items[i]->data.variableNode->lastIndex);

                    if (index_ != -1){
                        if (g_vars.items[index_].typedVar == FLOATVAR){
                            index_ = getVariableFloatValue(index_);
                        }
                        else if (g_vars.items[index_].typedVar == INTVAR){
                            index_ = getVariableIntValue(index_);
                        }
                        else{
                            raiseErrorMacro(*node->pos, "Incorrect var type.");
                        }
                    }
                    
                    //printf("INDEX : %i (max: %i ; %s)\n", index_, strlen(valueOrig), valueOrig);
                    if (index_ >= 0){
                        if (index_ >= strlen(valueOrig)){
                            //not sure when this happen?
                            //TODO: test

                            printf("TOOD: RAISE ERROR, NOT CORRECT INDEX : %i (max: %li ; %s)\n", index_, strlen(valueOrig), valueOrig);
                            exit(1);
                        }
                        char* buf = malloc(sizeof(char)*2);
                        buf[0] = value[index_];
                        buf[1] = '\0';
                        value = buf;
                    }

                    tempVar = (varStruct){.index = g_vars.count, .type = "string", .name = g_funcs.items[index].inputs.items[i]->data.variableNode->name, .data.arrayVar.value.stringValue = value, .data.arrayVar.length = strlen(value), .intialised = 1, .typedVar = STRINGVAR };
                }
                else if (g_vars.items[index].typedVar == NUMBERARRAY){
                    int indexArr = getIndexFromInput(node->data.functionCall->inputs.items[i]->data.variableNode->lastIndex);

                    if (index == -1){
                        raiseErrorMacro(*node->pos, "Inline array declaration not implemented yet.");
                    }
                    double* value = getVariableNumArrayValue(index);
                    if (indexArr != -1){
                        value = &value[indexArr];
                        tempVar = (varStruct){.index = g_vars.count, .type = "float", .name = g_funcs.items[index].inputs.items[i]->data.variableNode->name, .data.floatVal = *value, .intialised = 1, .typedVar = FLOATVAR };
                    }
                    else{
                        tempVar = (varStruct){.index = g_vars.count, .type = "numArr", .name = g_funcs.items[index].inputs.items[i]->data.variableNode->name, .data.arrayVar.value.numberValue = value, .data.arrayVar.length = getVariableNumArrayLength(index), .intialised = 1, .typedVar = NUMBERARRAY };
                    }
                    
                    //if (indexArr == -1){
                    //    double* value = getVariableNumArrayValue(index_);
                    //    tempVar = (varStruct){.index = g_vars.count, .type = "numArr", .name = g_funcs.items[index].inputs.items[i]->data.variableNode->name, .data.arrayVar.value.numberValue = value, .data.arrayVar.length = getVariableNumArrayLength(index_), .intialised = 1, .typedVar = NUMBERARRAY };
                    //}
                    //else{
                    //    int indexInArrayFromVal;
                    //    if (g_vars.items[indexArr].typedVar == INTVAR){
                    //        indexInArrayFromVal = getVariableIntValue(indexArr);
                    //    }
                    //    else if (g_vars.items[indexArr].typedVar == FLOATVAR){
                    //        indexInArrayFromVal = getVariableFloatValue(indexArr);
                    //    }
                    //    else{
                    //        printf("TODO: ADD ERROR, incorrect type of var array num (%li, ix: %i), type %li", indexArr, index, g_vars.items[indexArr].typedVar);
                    //        exit(1);
                    //    }
                    //    
                    //    tempVar = (varStruct){.index = g_vars.count, .type = "float", .name = g_funcs.items[index].inputs.items[i]->data.variableNode->name, .data.floatVal = value[indexInArrayFromVal], .intialised = 1, .typedVar = FLOATVAR };
                    //}
                }
                else{
                    raiseErrorMacro(*node->pos, "Incorrect binary operator return.");
                } 
            }
            else{
                raiseErrorMacro(*node->pos, "Incorrect var type while parsing function call, need implementation.");
            }
        }
        else if (node->data.functionCall->inputs.items[i]->type == BINOPNODE){
            binOpResult* res = evalBinOp(node->data.functionCall->inputs.items[i]);
            if (res->varType == FLOATVAR)
                tempVar = (varStruct){.index = g_vars.count, .type = "float", .name = g_funcs.items[index].inputs.items[i]->data.variableNode->name, .data.floatVal = res->value.floatVar, .intialised = 1, .typedVar = FLOATVAR };
            else if (res->varType == STRINGVAR){
                char* value = res->value.stringVal;
                tempVar = (varStruct){.index = g_vars.count, .type = "string", .name = g_funcs.items[index].inputs.items[i]->data.variableNode->name, .data.arrayVar.value.stringValue = value, .data.arrayVar.length = strlen(value), .intialised = 1, .typedVar = STRINGVAR };
            }
        }
        else if (node->data.functionCall->inputs.items[i]->type == STRINGNODE){
            char* value = node->data.functionCall->inputs.items[i]->data.stringNode->value;
            tempVar = (varStruct){.index = g_vars.count, .type = "string", .name = g_funcs.items[index].inputs.items[i]->data.variableNode->name, .data.arrayVar.value.stringValue = value, .data.arrayVar.length = strlen(value), .intialised = 1, .typedVar = STRINGVAR };
        }
        else if(node->data.functionCall->inputs.items[i]->type == NUMBERARRAYNODE){
            tempVar = (varStruct){.index = g_vars.count, .type = "numArr", .name = g_funcs.items[index].inputs.items[i]->data.variableNode->name, .data.arrayVar.value.numberValue = node->data.functionCall->inputs.items[i]->data.numberArrayNode->value, .data.arrayVar.length = node->data.functionCall->inputs.items[i]->data.numberArrayNode->length, .intialised = 1, .typedVar = NUMBERARRAY };
        }
        else{
            raiseErrorMacro(*node->pos, "Incorrect var type while parsing function call, implementation needed.");
        }

        int _inx = getVarIndexByName(tempVar.name);
        if (_inx != -1){
            DYN_PUSH(g_vars.items[_inx], l_vars);
            g_vars.items[_inx] = tempVar;
            g_vars.items[_inx].index = _inx;
        } else {
            tempVar.index = g_vars.count;
            DYN_PUSH(tempVar, g_vars);
        }

        if (!checkCompatibleVarType(tempVar.typedVar, g_funcs.items[index].inputs.items[i]->data.variableNode->type, FCALL)){
            raiseErrorMacro(*node->pos, "Incorrect function call type.");
            //printf("%i : %i\n", tempVar.typedVar, g_funcs.items[index].inputs.items[i]->data.variableNode->type);
            //printf("TODO: RAISE ERROR, INCORRECT CALL FUNCTION TYPE");
            //exit(1);
        }
    }

    if (isFunctionLowLevel(index) == 1){
        callLowLevelFunc(index, node);
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
            /*printf("BINOPNODE\n");*/
            //printf("%f\n", evalBinOp(node));
            evalBinOp(node);
            break;
        case VARIABLENODE:
            /*printf("VARIABLENODE\n");*/
            g_vars = evalVariable(node);
            break;
        case GOTOIDENTIFIER:
            /*printf("GOTOIDENTIFIER\n");*/
            g_gotos = parseGotoNode(node, g_gotos);
            break;
        case CONDITION:
            /*printf("CONDITION\n");*/
            parseCondition_(node);
            break;
        case FUNCTION:
            /*printf("FUNCTION\n");*/
            parseFunction(node);
            //printf("%i\n", i);
            break;
        case FUNCTIONCALL:
            /*printf("FUNCTIONCALL\n");*/
            parseFunctionCall_(node);
            break;
        case LOOPNODE:
            /*printf("LOOPNODE\n");*/
            parseLoopNode(node);
            break;
        default:
            raiseErrorMacro(*node->pos, "Node type was incorrect, maybe not implemented?");
            break;
    }
}

void writeOutTypesDebug(Node* ast){
    for (size_t i = 0; i < ast->data.programNode->nodes.count; i++){
        printf("NodeType: %i\n", ast->data.programNode->nodes.items[i]->type);
    }
}

void parse(Node* ast){
    //debug!
    //writeOutTypesDebug(ast);

    //printf("PARSER");
    //TODO: this might be problem? g_gotos will have duplicit values if codeblock?
    g_gotos = prescanForGotos(ast, g_gotos);
    for (size_t i = 0; i < ast->data.programNode->nodes.count; i++){
        Node* node = ast->data.programNode->nodes.items[i];
        //printf("nodeType: %li\n", node->type);
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
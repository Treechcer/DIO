#include<string.h>
#include <math.h>

#include"../Headers/dynamic_array.h"
#include"../Headers/ast.h"
#include"../Headers/errors.h"
#include"../Headers/parser.h"
#include"../Headers/helper_functions.h"

dynamicVar g_vars = {0,0,0};
dynamicGoto g_gotos = {0,0,0};

int getVarIndexByName(char* name){
    for(int i = 0; i < g_vars.count; i++){
        if (strcmp(name, g_vars.items[i].name) == 0){
            return g_vars.items[i].index;
        }
    }

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
    if (node->type == VARIABLENODE) return getVarValueByName(node->data.variableNode->name);

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
    char* tempArr[] = {"int", "float", "string", "bool",};

    char* type = tempArr[node->data.variableNode->type];
    char* name = node->data.variableNode->name;
    varStruct tempVar;

    int existingIndex = getVarIndexByName(name);
    if (existingIndex < 0 && node->data.variableNode->initialise == 0){
        errorOut((Error){"", UNKNOWNVARIABLETYPE});
    }

    if (strcmp(type, "int") == 0 || strcmp(type, "float") == 0){
        double value = evalBinOp(node->data.variableNode->value);

        tempVar = (varStruct){.index = g_vars.count, .type = type, .name = name, .data.intVal = value };
        printf("%f\n", value);
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

Node* astToNode(Node* ast){
    return &(Node){.type = ast->type, .data = ast->data};
}

void parse(Node* ast){
    g_gotos = prescanForGotos(ast, g_gotos);
    for (size_t i = 0; i < ast->data.programNode->nodes.count; i++){
        Node* node = ast->data.programNode->nodes.items[i];
        switch (node->type) {
            case BINOPNODE:
                printf("%f\n", evalBinOp(node));
                break;
            case VARIABLENODE:
                g_vars = evalVariable(node);
                break;
            case GOTOIDENTIFIER:
                g_gotos = parseGotoNode(node, g_gotos);
                break;
            case GOTONODE:
                int temp = parseGotoNameNode(node, &g_gotos, ast);
                if (temp != -1){
                    i = temp-1;
                }
                break;
            default:
                break;
        }
    }
}
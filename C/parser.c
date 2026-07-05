#include<string.h>
#include <math.h>

#include"..\Headers\dynamic_array.h"
#include"..\Headers\ast.h"
#include"..\Headers\errors.h"
#include"..\Headers\parser.h"
#include"..\Headers\helper_functions.h"

dynamicVar g_vars = {0,0,0};

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

    if (strcmp(type, "int") == 0 || strcmp(type, "float") == 0){
        double value = evalBinOp(node->data.variableNode->value);

        tempVar = (varStruct){.index = g_vars.count, .type = type, .name = name, .data.intVal = value };
        printf("%f\n", value);
    }
    else if (strcmp(type, "string") == 0){
        //TODO: implement later
    }

    int existingIndex = getVarIndexByName(name);
    if (existingIndex >= 0){
        tempVar.index = existingIndex;
        g_vars.items[existingIndex] = tempVar;
        return g_vars;
    }

    DYN_PUSH(tempVar, g_vars);
    return g_vars;
}

Node* astToNode(Node* ast){
    return &(Node){.type = ast->type, .data = ast->data};
}

void parse(Node* ast){
    for (size_t i = 0; i < ast->data.programNode->nodes.count; i++){        
        Node* node = ast->data.programNode->nodes.items[i];
        switch (node->type) {
            case BINOPNODE:
                printf("%f\n", evalBinOp(node));
                break;
            case VARIABLENODE:
                g_vars = evalVariable(node);
                break;
            default:
                break;
        }
    }
}
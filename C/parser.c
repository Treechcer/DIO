#include"..\Headers\dynamic_array.h"
#include"..\Headers\ast.h"
#include"..\Headers\errors.h"

double evalBinOp(Node* node){
    if (node->type == NUMBERNODE) return node->data.numberNode->value;

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
            default:
                break;
        }
    }

    return 0;
}

Node* astToNode(Node* ast){
    return &(Node){.type = ast->type, .data = ast->data};
}

void parse(Node* ast){
    for (size_t i = 0; i < ast->data.programNode->nodes.count; i++){
        Node* node = astToNode(ast->data.programNode->nodes.items[i]);
        switch (node->type) {
            case BINOPNODE:
                printf("%f\n", evalBinOp(node));
                break;
            default:
                break;
        }
    }
}
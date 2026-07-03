#include"..\Headers\dynamic_array.h"
#include"..\Headers\ast.h"
#include"..\Headers\errors.h"

double eval(Node* node){
    if (node->type == NUMBERNODE) return node->data.numberNode->value;

    if (node->type == BINOPNODE){
        double left = eval(node->data.binOpNode->left);
        double right = eval(node->data.binOpNode->right);

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
            default:
                break;
        }
    }

    return 0;
}

double parse(Node* ast){
    printf("%f", eval(ast));
    return eval(ast);
}
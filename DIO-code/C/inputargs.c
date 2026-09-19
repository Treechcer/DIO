#include<string.h>

#include "../Headers/inputargs.h"
#include "../Headers/token.h"

dynamicArg g_args;

int registerArg(int argType, char* name, int isNecesary, char* defaultValue, char oneCharName){
    if (oneCharName == ' '){
        oneCharName = name[0];
    }

    arg localArg = (arg){.index = g_args.count, .type = argType, .name = name, .isNecesary = isNecesary, .value = defaultValue, .found = 0, .shorthand = oneCharName};
    DYN_PUSH(localArg, g_args);
    return g_args.count-1;
}

void writeOutInputs(){
    //debug func

    for (int i = 0; i < g_args.count; i++){
        if (g_args.items[i].found == 0 && g_args.items[i].isNecesary == 1){
            printf("(%s, %s, %i, %i)", g_args.items[i].name, g_args.items[i].value, g_args.items[i].found, g_args.items[i].isNecesary);
        }
    }
}

char* getvalueByIndex(int index){
    return g_args.items[index].value;
}

void checkInuts(int argc, char **argv){
    for (int i = 0; i < g_args.count; i++){
        int bonus =2;
        if (g_args.items[i].name[1] != '\0'){
            bonus++;
        }
        char cmd[strlen(g_args.items[i].name) + bonus];
        cmd[0] = '-';
        if (g_args.items[i].name[1] != '\0'){
            cmd[1] = '-';
        }

        char sh[3];
        sh[0] = '-';
        sh[1] = g_args.items[i].shorthand;
        sh[2] = '\0';

        strcpy(&cmd[bonus-1], g_args.items[i].name);
        for (int j = 0; j < argc; j++){
            char* comm = argv[j];
            if (strcmp(cmd, comm) == 0 || strcmp(sh, comm) == 0){
                g_args.items[i].found = 1;

                if (g_args.items[i].type == BOOLEAN){
                    g_args.items[i].value = strToBoolStr(g_args.items[i].value);
                }
                else if (g_args.items[i].type == STRINGPUT) {
                    g_args.items[i].value = argv[j+1];
                    j++;
                }

                break;
            }
            else{
                
            }
        }
    }

    for (int i = 0; i < g_args.count; i++){
        if (g_args.items[i].found == 0 && g_args.items[i].isNecesary == 1){
            printf("Input for '%s', was not found, exiting program.\n", g_args.items[i].name);
            exit(1);
        }
    }

    writeOutInputs();
}
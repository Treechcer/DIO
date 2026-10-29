#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include"../Headers/token.h"
#include"../Headers/dynamic_array.h"
#include"../Headers/helper_functions.h"
#include"../Headers/appInfo.h"
#include"../Headers/globals.h"

typedef struct SpecialVars{
    char* name;
    dynamicToken toks;
} SpecialVars;

STRUCT_DYNAMIC_ARR_MACRO(SpecialVars, dynamicSpecialVars);

dynamicMacro g_macros = {0,0,0};
dynamicFileImport g_fileImports = {0,0,0};
dynamicSpecialVars g_specialVars = {0,0,0};

dynamicToken lex(const char* code, char* fileName, dynamicToken toks, int isLast);

SpecialVars makeSpecialVar(char* name, dynamicToken toks){
    return (SpecialVars){.name = name, .toks = toks};    
}

char* stringifyText(char* text){
    size_t len = strlen(text);
    char* retText = malloc(len + 3); 
    if (retText == NULL) {
        return NULL;
    }
    
    retText[0] = '"';

    for (size_t i = 0; i < len; i++){
        retText[1 + i] = text[i];
    }

    retText[len + 1] = '"';
    retText[len + 2] = '\0';

    return retText;
}

dynamicSpecialVars initSpecialVars(){
    dynamicToken dynToks = {0,0,0};

    char* lexThis = stringifyText(VERSION);
    dynToks = lex(lexThis, "", dynToks, 0);

    SpecialVars SpecialVar = makeSpecialVar("VERSION", dynToks);
    DYN_PUSH(SpecialVar, g_specialVars);

    dynToks = (dynamicToken){0,0,0};

    lexThis = stringifyText(getAppInfo().platform);
    dynToks = lex(lexThis, "", dynToks, 0);

    SpecialVar = makeSpecialVar("PLATFORM", dynToks);
    DYN_PUSH(SpecialVar, g_specialVars);

    free(lexThis);
}

bool isAllowed(char c){ //special characters that are allowed and handled differently!
    return (c == '<' || c == '>');
}

/*void freeToks(dynamicToken toks){
    free(toks.items);
    toks.count = 0;
    toks.size = 0;
}*/

void writeToksOut(dynamicToken tok){
    for(int i = 0; i < tok.count; i++){
        if (tok.items[i].value){
            printf("((%i) %i : %s)", i, tok.items[i].identifier, tok.items[i].value);
            if (tok.items[i].identifier == END) printf("\n");
        }
        else{
            printf("(%i) %i)", i, (tok.items[i].identifier));
            if (tok.items[i].identifier == END) printf("\n");
        }
    }

    printf("\n");
}

char moveByChar(const char** code, int* line, int* charPos, int d){
    char c;
    if (d == 1){
        (*code)++;
        (*charPos)++;
    }
    else if(d == -1){
        (*code)--;
        (*charPos)--;
    }

    c = **code;

    if (c == '\n'){
        (*line)++;
        (*charPos) = 1;
    }

    return c;
}

dynamicToken lex(const char* code, char* fileName, dynamicToken toks, int isLast) {
    if (isLast == 1){
        initSpecialVars();
    }
    //Token* toks;
    int indexFile = -1;

    for (size_t i = 0; i < g_fileImports.count; i++){
        if (strcmp(fileName, g_fileImports.items[i].name) == 0){
            indexFile = i;
            break;
        }
    }

    if (indexFile == -1){
        //we will redefine canLoadMoreThanOnce!
        FileImport a = (FileImport){.name = fileName, .canLoadMoreThanOnce = 1, .loadedTimes = 1, .index = indexFile};
        DYN_PUSH(a, g_fileImports);
    }
    else if (g_fileImports.items[indexFile].canLoadMoreThanOnce == 0){
        return toks;
    }

    int line = 1;
    int charPos_ = 1;
    char c = *code;
    int skipSapces = 1;

    if (c == '#'){
        if (moveByChar(&code, &line, &charPos_, 1) == '!'){
            while (moveByChar(&code, &line, &charPos_, 1) != '\n'){
                
            }
        }
        else{
            moveByChar(&code, &line, &charPos_, -1);
        }
    }

    while (strlen(code) > 0 && c != '\0'){
        c = *code;
        //printf("(%i, %c)\n", c, c);
        Token tok = {0};
        //printf("%i, %i, %c\n", line, charPos_, c);

        switch (c) {
            case '+':
                c = moveByChar(&code, &line, &charPos_, 1);
                if (c == '+'){
                    c = moveByChar(&code, &line, &charPos_, 1);
                    //increment
                    if (c != '+'){
                        c = moveByChar(&code, &line, &charPos_, -1);
                        DYN_PUSH(createToken("+", PLUS, createPosition(charPos_, charPos_, line, fileName)), toks);
                        DYN_PUSH(createToken("1", INT, createPosition(charPos_, charPos_, line, fileName)), toks);
                    }
                    //looper increment
                    else{
                        Token tok = toks.items[toks.count-1];
                        DYN_PUSH(createToken("=", EQUALS, createPosition(charPos_, charPos_, line, fileName)), toks);
                        DYN_PUSH(tok, toks);
                        DYN_PUSH(createToken("+", PLUS, createPosition(charPos_, charPos_, line, fileName)), toks);
                        DYN_PUSH(createToken("1", INT, createPosition(charPos_, charPos_, line, fileName)), toks);
                    }
                }
                else if (c == '='){
                    //walrus operator
                    Token varName = toks.items[toks.count - 1];
                    DYN_PUSH(createToken("=", EQUALS, createPosition(charPos_, charPos_, line, fileName)), toks);
                    DYN_PUSH(varName, toks);
                    DYN_PUSH(createToken("+", PLUS, createPosition(charPos_, charPos_, line, fileName)), toks);
                } 
                else{
                    c = moveByChar(&code, &line, &charPos_, -1);
                    tok = createToken("+", PLUS, createPosition(charPos_, charPos_, line, fileName));
                }
                break;
            case '-':
                c = moveByChar(&code, &line, &charPos_, 1);
                if (c == '-'){
                    c = moveByChar(&code, &line, &charPos_, 1);
                    //increment
                    if (c != '-'){
                        c = moveByChar(&code, &line, &charPos_, -1);
                        DYN_PUSH(createToken("-", MINUS, createPosition(charPos_, charPos_, line, fileName)), toks);
                        DYN_PUSH(createToken("1", INT, createPosition(charPos_, charPos_, line, fileName)), toks);
                    }
                    //looper increment
                    else{
                        Token tok = toks.items[toks.count-1];
                        DYN_PUSH(createToken("=", EQUALS, createPosition(charPos_, charPos_, line, fileName)), toks);
                        DYN_PUSH(tok, toks);
                        DYN_PUSH(createToken("+", MINUS, createPosition(charPos_, charPos_, line, fileName)), toks);
                        DYN_PUSH(createToken("1", INT, createPosition(charPos_, charPos_, line, fileName)), toks);
                    }
                }
                else if (c == '='){
                    //walrus operator
                    Token varName = toks.items[toks.count - 1];
                    DYN_PUSH(createToken("=", EQUALS, createPosition(charPos_, charPos_, line, fileName)), toks);
                    DYN_PUSH(varName, toks);
                    DYN_PUSH(createToken("-", MINUS, createPosition(charPos_, charPos_, line, fileName)), toks);
                }
                else{
                    c = moveByChar(&code, &line, &charPos_, -1);
                    tok = createToken("-", MINUS, createPosition(charPos_, charPos_, line, fileName));
                }
                break;
            case '*':
                c = moveByChar(&code, &line, &charPos_, 1);
                if (c == '='){
                    //walrus operator
                    Token varName = toks.items[toks.count - 1];
                    DYN_PUSH(createToken("=", EQUALS, createPosition(charPos_, charPos_, line, fileName)), toks);
                    DYN_PUSH(varName, toks);
                    DYN_PUSH(createToken("*", MUL, createPosition(charPos_, charPos_, line, fileName)), toks);
                }
                else{
                    c = moveByChar(&code, &line, &charPos_, -1);
                    tok = createToken("*", MUL, createPosition(charPos_, charPos_, line, fileName));
                }
                break;
            case '/':
                c = moveByChar(&code, &line, &charPos_, 1);
                if (c == '='){
                    //walrus operator
                    Token varName = toks.items[toks.count - 1];
                    DYN_PUSH(createToken("=", EQUALS, createPosition(charPos_, charPos_, line, fileName)), toks);
                    DYN_PUSH(varName, toks);
                    DYN_PUSH(createToken("/", DIV, createPosition(charPos_, charPos_, line, fileName)), toks);
                }
                else{
                    c = moveByChar(&code, &line, &charPos_, -1);
                    tok = createToken("/", DIV, createPosition(charPos_, charPos_, line, fileName));
                }
                break;
            case '^':
                tok = createToken("^", POW, createPosition(charPos_, charPos_, line, fileName));
                break;
            case '(':
                tok = createToken("(", LPAREN, createPosition(charPos_, charPos_, line, fileName));
                break;
            case ')':
                tok = createToken(")", RPAREN, createPosition(charPos_, charPos_, line, fileName));
                break;
            case '[':
                tok = createToken("[", LSQUAREPAREN, createPosition(charPos_, charPos_, line, fileName));
                break;
            case ']':
                tok = createToken("]", RSQUAREPAREN, createPosition(charPos_, charPos_, line, fileName));
                break;
            case '{':
                tok = createToken("{", LSQUIGLYPAREN, createPosition(charPos_, charPos_, line, fileName));
                break;
            case '}':
                tok = createToken("}", RSQUIGLYPAREN, createPosition(charPos_, charPos_, line, fileName));
                break;
            case ',':
                tok = createToken(",", COMMA, createPosition(charPos_, charPos_, line, fileName));
                break;
            case '.':
            case '!':
            case '?':{ //had to do {} for it to compile and run on MAC btw.
                char* buf = malloc(2 * sizeof(char));
                buf[0] = c;
                buf[1] = '\0';
                tok = createToken(buf, STRINGLITERAL, createPosition(charPos_, charPos_, line, fileName));
                break;
            }
            case '<':
                c = moveByChar(&code, &line, &charPos_, 1);
                if (c == '='){
                    tok = createToken("<=", LESSOREQAUL, createPosition(charPos_, charPos_, line, fileName));
                }
                else{
                    tok = createToken("<", LESSTHAN, createPosition(charPos_, charPos_, line, fileName));
                    moveByChar(&code, &line, &charPos_, -1);
                }
                break;
            case '>':
                c = moveByChar(&code, &line, &charPos_, 1);
                if (c == '='){
                    tok = createToken(">=", MOREOREQUAL, createPosition(charPos_, charPos_, line, fileName));
                }
                else if (c == '>') {
                    while (c != '\n' && c != '\r' && c != '\0' && strlen(code) > 0){
                        c = moveByChar(&code, &line, &charPos_, 1);
                    }
                }
                else if (c == '*'){
                    while (c != '\0' && strlen(code) > 0){
                        char preC = c;
                        c = moveByChar(&code, &line, &charPos_, 1);
                        if (c == '<' && preC == '*'){
                            c = moveByChar(&code, &line, &charPos_, 1);
                            break;
                        }
                    }
                }
                else{
                    tok = createToken(">", MORETHAN, createPosition(charPos_, charPos_, line, fileName));
                    code--;
                }
                break;
            case ':':
                c = moveByChar(&code, &line, &charPos_, 1);
                if (c == ':'){
                    c = moveByChar(&code, &line, &charPos_, 1);
                    char* str = "";
                    dynamicChar token = {0,0,0};
                    while(isAlpha(c)){
                        DYN_PUSH(c, token);

                        c = moveByChar(&code, &line, &charPos_, 1);
                    }
                    DYN_PUSH('\0', token);
                    if (c == ':'){
                        c = moveByChar(&code, &line, &charPos_, 1);
                        if (c == ':'){
                            tok = createToken(token.items, GOTONAME, createPosition(charPos_, charPos_, line, fileName));
                        }
                        else{
                            raiseErrorMacro(createPosition(charPos_, charPos_, line, fileName), "Unsuported character sequnce was found.");
                        }
                    }
                    else{
                        c = moveByChar(&code, &line, &charPos_, 1);
                        tok = createToken(token.items, FUNCTIONRETURNTYPE, createPosition(charPos_, charPos_, line, fileName));
                        break;
                        //raiseErrorMacro(createPosition(charPos_, charPos_, line, fileName), "Unsuported character sequnce was found.");
                    }
                }
                else{
                    tok = createToken(":", COLON, createPosition(charPos_, charPos_, line, fileName));
                    moveByChar(&code, &line, &charPos_, -1);
                    break;
                }
                break;
            case '\\':
            case '\n':
                tok = createToken("", END, createPosition(charPos_, charPos_, line, fileName));
                break;
            case ';':
                tok = createToken(";", END, createPosition(charPos_, charPos_, line, fileName));
                break;
            case '\0':
                tok = createToken(";", END, createPosition(charPos_, charPos_, line, fileName));
                break;
            case '=':
                c = moveByChar(&code, &line, &charPos_, 1);
                if (c == '='){
                    tok = createToken("==", LEFTRIGHTEQUAL, createPosition(charPos_, charPos_, line, fileName));
                }
                else{
                    tok = createToken("=", EQUALS, createPosition(charPos_, charPos_, line, fileName));
                    moveByChar(&code, &line, &charPos_, -1);
                }
                break;
            case '\r':
            case '\t':
                break;
            case ' ':
                if (!skipSapces){
                    tok = createToken(" ", SPACE, createPosition(charPos_, charPos_, line, fileName));
                }
                break;
            case '\'':
            case '"':
                tok = createToken("'", QUOTE, createPosition(charPos_, charPos_, line, fileName));
                DYN_PUSH(tok, toks);

                dynamicChar name = {0,0,0};
                c = moveByChar(&code, &line, &charPos_, 1);
                DYN_PUSH(c, name)

                while (c != '\'' && c != '"'){
                    c = moveByChar(&code, &line, &charPos_, 1);
                    if (c == '\'' || c == '"'){
                        break;
                    }
                    DYN_PUSH(c, name)
                }
                DYN_PUSH('\0', name)

                tok = createToken(name.items, IDENTIFIER, createPosition(tok.pos.start, charPos_ - 1, line, fileName));
                DYN_PUSH(tok, toks);

                tok = createToken("'", QUOTE, createPosition(charPos_, charPos_, line, fileName));

                //skipSapces = !skipSapces;
                break;
            case '#':
                c = moveByChar(&code, &line, &charPos_, 1); //#
                
                if (isAlpha(c)) {
                    dynamicChar action = {0,0,0};
                    while (isAlpha(c) && c != '\0') {
                        DYN_PUSH(c, action);
                        c = moveByChar(&code, &line, &charPos_, 1);
                    }
                    DYN_PUSH('\0', action);

                    while ((c == ' ' || c == '\t') && c != '\0') {
                        c = moveByChar(&code, &line, &charPos_, 1);
                    }

                    dynamicChar arg = {0,0,0};
                    while ((isAlpha(c) || c == '/' || c == '\\' || c == '.' || isDigit(c)) && c != '\0' || c == ' ') {
                        DYN_PUSH(c, arg);
                        c = moveByChar(&code, &line, &charPos_, 1);
                    }
                    DYN_PUSH('\0', arg);
                    //printf("%s\n", arg.items);

                    if (strcmp(action.items, "get") == 0) {
                        if (strcmp(arg.items, "alife") == 0){
                            printf("Touch grass.\n");
                        }
                        fileReadReturn fileNext = readFile(arg.items);
                        if (fileNext.exists == 0) {
                            raiseErrorMacro(createPosition(charPos_, charPos_, line, fileName), "File does not exist or could not be access by the OS.");
                        }
                        toks = lex(fileNext.content, arg.items, toks, 0);
                    }
                    else if (strcmp(action.items, "def") == 0){
                        c = moveByChar(&code, &line, &charPos_, 1);
                        dynamicChar text = {0,0,0};

                        while (c != '\n' && c != '\0') {
                            DYN_PUSH(c, text);
                            c = moveByChar(&code, &line, &charPos_, 1);
                        }
                        DYN_PUSH('\0', text);

                        Macro newMacro = (Macro){.name = arg.items, .text = text.items};

                        for (int i = 0; i < g_macros.count; i++){
                            if (strcmp(g_macros.items[i].name, arg.items) == 0){
                                raiseErrorMacro(createPosition(charPos_, charPos_, line, fileName), "Macro with this name already exists.")
                            }
                        }

                        DYN_PUSH(newMacro, g_macros);
                    }
                    else if (strcmp(action.items, "use") == 0){
                        int index = -1;
                        for (int i = 0; i < g_macros.count; i++){
                            if (strcmp(g_macros.items[i].name, arg.items) == 0){
                                index = i;
                            }
                        }

                        if (index == -1){
                            raiseErrorMacro(createPosition(charPos_, charPos_, line, fileName), "Macro with this name does not exist.")
                        }
                        
                        //printf("%s", g_macros.items[index].text);

                        toks = lex(g_macros.items[index].text, fileName, toks, 0);
                    }
                    else if (strcmp(action.items, "WIN") == 0 || strcmp(action.items, "LINUX") == 0 || strcmp(action.items, "APPLE") == 0 || strcmp(action.items, "FREEBSD") == 0 || strcmp(action.items, "ANDROID") == 0 || strcmp(action.items, "WEB") == 0){
                        int canRun = 0;
                        #if defined(_WIN32)
                            canRun = strcmp(action.items, "WIN") == 0;
                        #elif defined(__linux__)
                            canRun = strcmp(action.items, "LINUX") == 0;
                        #elif defined(__APPLE__)
                            canRun = strcmp(action.items, "APPLE") == 0;
                        #elif defined(__FreeBSD__)
                            canRun = strcmp(action.items, "FREEBSD") == 0;
                        #elif defined(__ANDROID__)
                            canRun = strcmp(action.items, "ANDROID") == 0;
                        #elif defined(__EMSCRIPTEN__) || defined(__wasm__)
                            canRun = strcmp(action.items, "WEB") == 0
                        #endif

                        if (canRun) {
                            dynamicChar text = {0,0,0};
                            while (c != '\n' && c != '\0') {
                                DYN_PUSH(c, text);
                                c = moveByChar(&code, &line, &charPos_, 1);
                            }

                            printf("%s\n", text.items);
                            toks = lex(text.items, fileName, lex(arg.items, fileName, toks, 0), 0);
                        }
                        else{
                            while (c != '\n' && c != '\0') {
                                c = moveByChar(&code, &line, &charPos_, 1);
                            }
                        }
                    }
                    //This is useless, this will get executed alawys, add it into AST/parser?
                    //else if (strcmp(action.items, "warn") == 0){
                    //    raiseWarningMacro(createPosition(charPos_, charPos_, line, fileName), arg.items)
                    //}
                    //else if (strcmp(action.items, "error") == 0){
                    //    //TODO: This crashes, maybe look into it? IT might be because errorsNew have SOME issue still that weren't fixed yet
                    //    raiseErrorMacro(createPosition(charPos_, charPos_, line, fileName), arg.items)
                    //}
                    else if (strcmp(action.items, "pragma") == 0){
                        if (strcmp(arg.items, "once") == 0){
                            g_fileImports.items[indexFile].canLoadMoreThanOnce = 0;
                            return toks;
                        }
                        else{
                            raiseErrorMacro(createPosition(charPos_, charPos_, line, fileName), "Unknown pragma argument.")
                        }
                    }
                    else{
                        raiseErrorMacro(createPosition(charPos_, charPos_, line, fileName), "Unknown middleware processor command.")
                    }
                }
                break;
            default:
                    if (isDigit(c)){
                        dynamicChar token = {0,0,0};
                        int isFloat = false;
                        while((isDigit(c) || c == '.')){
                            if (c == '.') {
                                if (isFloat) {
                                    raiseErrorMacro(createPosition(charPos_, charPos_, line, fileName), "Float can't have two dots.")
                                } else {
                                    isFloat = 1;
                                }
                            }
                            DYN_PUSH(c, token);

                            c = moveByChar(&code, &line, &charPos_, 1);
                        }

                        DYN_PUSH('\0', token);
                        tok = createToken(token.items, isFloat ? FLOAT : INT, createPosition(charPos_, charPos_, line, fileName));

                        c = moveByChar(&code, &line, &charPos_, -1);
                    }
                    else if (isAlpha(c)){
                        dynamicChar token = {0,0,0};
                        while(isAlpha(c) || isDigit(c)){
                            DYN_PUSH(c, token);

                            c = moveByChar(&code, &line, &charPos_, 1);
                            //printf("%c\n", c);
                        }
                        
                        DYN_PUSH('\0', token);
                        if (getKeyWord(token.items)){
                            tok = createToken(token.items, KEYWORD, createPosition(charPos_, charPos_, line, fileName));
                        }
                        else if (isBool(token.items)) {
                            tok = createToken(strToBoolStr(token.items), INT, createPosition(charPos_, charPos_, line, fileName));
                        }
                        //Use maybe as identifier?
                        //else if (isMaybe(token.items)){
                        //    tok = createToken("maybe", MAYBE, createPosition(charPos_, charPos_, line, fileName));
                        //}
                        else {
                            tok = createToken(token.items, IDENTIFIER, createPosition(charPos_, charPos_, line, fileName));
                        }

                        //if I had another token right after keyword it didn't really work, so I added this and it works lol

                        c = moveByChar(&code, &line, &charPos_, -1);
                    }
                    else if (c == '$'){
                        dynamicChar name = {0,0,0};
                        while (c != ' ' && c != '\n'){
                            c = moveByChar(&code, &line, &charPos_, 1);
                            DYN_PUSH(c, name);
                        }
                        name.items[name.count-1] = '\0';

                        //printf("%s\n", name.items);

                        int found = 0;
                        for (size_t i = 0; i < g_specialVars.count; i++){
                            //printf("%s\n", g_specialVars.items[i].name);
                            if (strcmp(name.items, g_specialVars.items[i].name) == 0){
                                for (size_t j = 0; j < g_specialVars.items[i].toks.count; j++){
                                    DYN_PUSH(g_specialVars.items[i].toks.items[j], toks);
                                    found = 1;
                                }
                            }
                        }

                        if (found == 0){
                            raiseErrorMacro(createPosition(charPos_, charPos_, line, fileName), "Special var not found.");
                        }

                    }
                    else{
                        //printf("%s\n", code);
                        //writeToksOut(toks);

                        //TODO: test
                        
                        char msg[] = "Unknown token at char 'C'.";
                        msg[strlen(msg) - 3] = c;

                        raiseErrorMacro(createPosition(charPos_, charPos_, line, fileName), msg)
                    }
                break;
        }

        if (tok.identifier){
            DYN_PUSH(tok, toks);
        }
        if (c == '\0'){
            break;
        }

        c = moveByChar(&code, &line, &charPos_, 1);
    }
    
    if (isLast == 1){
        free(g_macros.items);
        g_macros.count = 0;
        g_macros.size = 0;
    
        if (DEBUG_MODE == 1){
            writeToksOut(toks);
            printf("\n");
        }
    }

    return toks;
}

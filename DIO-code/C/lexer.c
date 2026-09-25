#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include"../Headers/token.h"
#include"../Headers/dynamic_array.h"
#include"../Headers/helper_functions.h"

dynamicMacro g_macros = {0,0,0};
dynamicFileImport g_fileImports = {0,0,0};

dynamicToken lex(const char* code, char* fileName, dynamicToken toks);

bool isAllowed(char c){ //special characters that are allowed and handled differently!
    return (c == '<' || c == '>');
}

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

dynamicToken lex(const char* code, char* fileName, dynamicToken toks) {
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
    char c = ' ';
    int skipSapces = 1;
    while (strlen(code) > 0 && c != '\0'){
        c = *code;
        //printf("(%i, %c)\n", c, c);
        Token tok = {0};
        //printf("%i, %i, %c\n", line, charPos_, c);

        switch (c) {
            case '+':
                tok = createToken("+", PLUS, createPosition(&charPos_, &charPos_, &line, fileName));
                break;
            case '-':
                tok = createToken("-", MINUS, createPosition(&charPos_, &charPos_, &line, fileName));
                break;
            case '*':
                tok = createToken("*", MUL, createPosition(&charPos_, &charPos_, &line, fileName));
                break;
            case '/':
                tok = createToken("/", DIV, createPosition(&charPos_, &charPos_, &line, fileName));
                break;
            case '^':
                tok = createToken("^", POW, createPosition(&charPos_, &charPos_, &line, fileName));
                break;
            case '(':
                tok = createToken("(", LPAREN, createPosition(&charPos_, &charPos_, &line, fileName));
                break;
            case ')':
                tok = createToken(")", RPAREN, createPosition(&charPos_, &charPos_, &line, fileName));
                break;
            case '[':
                tok = createToken("[", LSQUAREPAREN, createPosition(&charPos_, &charPos_, &line, fileName));
                break;
            case ']':
                tok = createToken("]", RSQUAREPAREN, createPosition(&charPos_, &charPos_, &line, fileName));
                break;
            case '{':
                tok = createToken("{", LSQUIGLYPAREN, createPosition(&charPos_, &charPos_, &line, fileName));
                break;
            case '}':
                tok = createToken("}", RSQUIGLYPAREN, createPosition(&charPos_, &charPos_, &line, fileName));
                break;
            case ',':
                tok = createToken(",", COMMA, createPosition(&charPos_, &charPos_, &line, fileName));
                break;
            case '.':
            case '!':
            case '?':
                char* buf = malloc(2 * sizeof(char));
                buf[0] = c;
                buf[1] = '\0';
                tok = createToken(buf, STRINGLITERAL, createPosition(&charPos_, &charPos_, &line, fileName));
                break;
            case '<':
                code++;
                c = *code;
                if (c == '='){
                    tok = createToken("<=", LESSOREQAUL, createPosition(&charPos_, &charPos_, &line, fileName));
                }
                else{
                    tok = createToken("<", LESSTHAN, createPosition(&charPos_, &charPos_, &line, fileName));
                    code--;
                }
                break;
            case '>':
                code++;
                c = *code;
                if (c == '='){
                    tok = createToken(">=", MOREOREQUAL, createPosition(&charPos_, &charPos_, &line, fileName));
                }
                else if (c == '>') {
                    while (c != '\n' && c != '\r' && c != '\0' && strlen(code) > 0){
                        code++;
                        c = *code;
                    }
                }
                else if (c == '*'){
                    while (c != '\0' && strlen(code) > 0){
                        char preC = c;
                        code++;
                        c = *code;
                        if (c == '<' && preC == '*'){
                            code++;
                            c = *code;
                            break;
                        }
                    }
                }
                else{
                    tok = createToken(">", MORETHAN, createPosition(&charPos_, &charPos_, &line, fileName));
                    code--;
                }
                break;
            case ':':
                code++;
                c = *code;
                if (c == ':'){
                    code++;
                    c = *code;
                    char* str = "";
                    dynamicChar token = {0,0,0};
                    while(isAlpha(c)){
                        DYN_PUSH(c, token);

                        code++;
                        charPos_++;
                        c = *code;
                    }
                    DYN_PUSH('\0', token);
                    if (c == ':'){
                        code++;
                        c = *code;
                        if (c == ':'){
                            tok = createToken(token.items, GOTONAME, createPosition(&charPos_, &charPos_, &line, fileName));
                        }
                        else{
                            raiseErrorMacro(createPosition(&charPos_, &charPos_, &line, fileName), "Unsuported character sequnce was found.");
                        }
                    }
                    else{
                        code++;
                        c = *code;
                        tok = createToken(":", FUNCTIONRETURNTYPE, createPosition(&charPos_, &charPos_, &line, fileName));
                        break;
                        //raiseErrorMacro(createPosition(&charPos_, &charPos_, &line, fileName), "Unsuported character sequnce was found.");
                    }
                }
                else{
                    code--;
                    c = *code;
                    tok = createToken(":", COLON, createPosition(&charPos_, &charPos_, &line, fileName));
                    break;
                }
                break;
            case '\\':
            case '\n':
                tok = createToken("", END, createPosition(&charPos_, &charPos_, &line, fileName));
                line++;
                charPos_ = 1;
                break;
            case ';':
                tok = createToken(";", END, createPosition(&charPos_, &charPos_, &line, fileName));
                break;
            case '\0':
                tok = createToken(";", END, createPosition(&charPos_, &charPos_, &line, fileName));
                break;
            case '=':
                code++;
                c = *code;
                if (c == '='){
                    tok = createToken("==", LEFTRIGHTEQUAL, createPosition(&charPos_, &charPos_, &line, fileName));
                }
                else{
                    tok = createToken("=", EQUALS, createPosition(&charPos_, &charPos_, &line, fileName));
                    code--;
                }
                break;
            case '\r':
            case '\t':
                break;
            case ' ':
                if (!skipSapces){
                    tok = createToken(" ", SPACE, createPosition(&charPos_, &charPos_, &line, fileName));
                }
                break;
            case '\'':
            case '"':
                tok = createToken("'", QUOTE, createPosition(&charPos_, &charPos_, &line, fileName));
                skipSapces = !skipSapces;
                break;
            case '#':
                code++; //#
                charPos_++;
                c = *code;
                
                if (isAlpha(c)) {
                    dynamicChar action = {0,0,0};
                    while (isAlpha(c) && c != '\0') {
                        DYN_PUSH(c, action);
                        code++;
                        charPos_++;
                        c = *code;
                    }
                    DYN_PUSH('\0', action);

                    while ((c == ' ' || c == '\t') && c != '\0') {
                        code++;
                        charPos_++;
                        c = *code;
                    }

                    dynamicChar arg = {0,0,0};
                    while ((isAlpha(c) || c == '/' || c == '\\' || c == '.' || isDigit(c)) && c != '\0' || c == ' ') {
                        DYN_PUSH(c, arg);
                        code++;
                        charPos_++;
                        c = *code;
                    }
                    DYN_PUSH('\0', arg);
                    //printf("%s\n", arg.items);

                    if (strcmp(action.items, "get") == 0) {
                        fileReadReturn fileNext = readFile(arg.items);
                        if (fileNext.exists == 0) {
                            raiseErrorMacro(createPosition(&charPos_, &charPos_, &line, fileName), "File does not exist or could not be access by the OS.");
                        }
                        toks = lex(fileNext.content, arg.items, toks);
                    }
                    else if (strcmp(action.items, "def") == 0){
                        code++;
                        c = *code;
                        dynamicChar text = {0,0,0};

                        while (c != '\n' && c != '\0') {
                            DYN_PUSH(c, text);
                            code++;
                            c = *code;
                        }
                        DYN_PUSH('\0', text);

                        Macro newMacro = (Macro){.name = arg.items, .text = text.items};

                        for (int i = 0; i < g_macros.count; i++){
                            if (strcmp(g_macros.items[i].name, arg.items) == 0){
                                raiseErrorMacro(createPosition(&charPos_, &charPos_, &line, fileName), "Macro with this name already exists.")
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
                            raiseErrorMacro(createPosition(&charPos_, &charPos_, &line, fileName), "Macro with this name does not exist.")
                        }
                        
                        //printf("%s", g_macros.items[index].text);

                        toks = lex(g_macros.items[index].text, fileName, toks);
                    }
                    else if (strcmp(action.items, "WIN") == 0 || strcmp(action.items, "LINUX") == 0 || strcmp(action.items, "MAC") == 0 || strcmp(action.items, "FREEBSD") == 0){
                        int canRun = 0;
                        #if defined(_WIN32)
                            canRun = strcmp(action.items, "WIN") == 0;
                        #elif defined(__linux__)
                            canRun = strcmp(action.items, "LINUX") == 0;
                        #elif defined(__APPLE__)
                            canRun = strcmp(action.items, "MAC") == 0;
                        #elif defined(__FreeBSD__)
                            canRun = strcmp(action.items, "FREEBSD") == 0;
                        #endif

                        if (canRun) {
                            dynamicChar text = {0,0,0};
                            while (c != '\n' && c != '\0') {
                                DYN_PUSH(c, text);
                                code++;
                                c = *code;
                            }
                            toks = lex(text.items, fileName, toks);
                        }
                        else{
                            while (c != '\n' && c != '\0') {
                                code++;
                                c = *code;
                            }
                        }
                    }
                    //This is useless, this will get executed alawys, add it into AST/parser?
                    //else if (strcmp(action.items, "warn") == 0){
                    //    raiseWarningMacro(createPosition(&charPos_, &charPos_, &line, fileName), arg.items)
                    //}
                    //else if (strcmp(action.items, "error") == 0){
                    //    //TODO: This crashes, maybe look into it? IT might be because errorsNew have SOME issue still that weren't fixed yet
                    //    raiseErrorMacro(createPosition(&charPos_, &charPos_, &line, fileName), arg.items)
                    //}
                    else if (strcmp(action.items, "pragma") == 0){
                        if (strcmp(arg.items, "once") == 0){
                            g_fileImports.items[indexFile].canLoadMoreThanOnce = 0;
                            return toks;
                        }
                        else{
                            raiseErrorMacro(createPosition(&charPos_, &charPos_, &line, fileName), "Unknown pragma argument.")
                        }
                    }
                    else{
                        raiseErrorMacro(createPosition(&charPos_, &charPos_, &line, fileName), "Unknown middleware processor command.")
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
                                    raiseErrorMacro(createPosition(&charPos_, &charPos_, &line, fileName), "Float can't have two dots.")
                                } else {
                                    isFloat = 1;
                                }
                            }
                            DYN_PUSH(c, token);

                            code++;
                            charPos_++;
                            c = *code;
                        }

                        DYN_PUSH('\0', token);
                        tok = createToken(token.items, isFloat ? FLOAT : INT, createPosition(&charPos_, &charPos_, &line, fileName));

                        code--;
                        charPos_--;
                        c = *code;
                    }
                    else if (isAlpha(c)){
                        dynamicChar token = {0,0,0};
                        while(isAlpha(c)){
                            DYN_PUSH(c, token);

                            code++;
                            charPos_++;
                            c = *code;
                        }
                        
                        DYN_PUSH('\0', token);
                        if (getKeyWord(token.items)){
                            tok = createToken(token.items, KEYWORD, createPosition(&charPos_, &charPos_, &line, fileName));
                        }
                        else if (isBool(token.items)) {
                            tok = createToken(strToBoolStr(token.items), INT, createPosition(&charPos_, &charPos_, &line, fileName));
                        }
                        //Use maybe as identifier?
                        //else if (isMaybe(token.items)){
                        //    tok = createToken("maybe", MAYBE, createPosition(&charPos_, &charPos_, &line, fileName));
                        //}
                        else {
                            tok = createToken(token.items, IDENTIFIER, createPosition(&charPos_, &charPos_, &line, fileName));
                        }

                        //if I had another token right after keyword it didn't really work, so I added this and it works lol

                        code--;
                        charPos_--;
                        c = *code;
                    }
                    else{
                        //printf("%s\n", code);
                        //writeToksOut(toks);

                        //TODO: test
                        
                        char* msg = "Unknown token at char 'C'.";
                        msg[strlen("Unknown token at char 'C'.")-3] = c;

                        raiseErrorMacro(createPosition(&charPos_, &charPos_, &line, fileName), msg)
                    }
                break;
        }

        if (tok.identifier){
            DYN_PUSH(tok, toks);
        }
        if (c == '\0'){
            break;
        }

        code++;
        charPos_++;
    }

    //writeToksOut(toks);
    //printf("\n");
    return toks;
}
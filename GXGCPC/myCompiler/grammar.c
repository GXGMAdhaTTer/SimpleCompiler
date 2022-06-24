#define _CRT_SECURE_NO_WARNINGS
#include "grammar.h"
#include "dict.h"

// 创建空的哈希表
HashMap* WordTable_create() {
    HashMap* wordTable = (HashMap*)calloc(1, sizeof(HashMap));

    if (wordTable == NULL){
        printf("创建哈希表失败。\n"); 
        exit(1);
    }

    wordTable->count = 0;
    wordTable->capacity = MAXKEY;
    for (int i = 0; i < (sizeof(keywords) / sizeof(keywords[0])); i++) {
        WordTable_put(wordTable, keywords[i].spelling, keywords[i].tkcode);
    }
    return wordTable;
}

//销毁哈希表
void WordTable_destroy(HashMap* map) {
    // 释放所有结点
    for (int i = 0; i < MAXKEY; i++) {
        TKWord* curr = map->tk_hashtable[i];
        while (curr != NULL) {
            // 保存后继结点
            TKWord* next = curr->next;
            free(curr);
            curr = next;
        }
    }
    // 释放HashMap结构体
    free(map);
}

//经典哈希函数
int hash(char* spelling) {
    int h = 0, g;
    while (*spelling) {
        h = (h << 4) + *spelling++;
        g = h & 0xf0000000;
        if (g)
            h ^= g >> 24;
        h &= ~g;
    }
    return h % MAXKEY;
}

//根据spelling返回tkcode
V WordTable_get(HashMap* map, K spelling) {
    // 根据spelling获取索引
    int idx = hash(spelling);
    // 遍历链表
    TKWord* curr = map->tk_hashtable[idx];
    while (curr != NULL) {
        // 判断spelling是否存在
        if (strcmp(spelling, curr->spelling) == 0) {
            return curr->tkcode;
        }
        curr = curr->next;
    }
    // 不存在这样的 spelling
    return -1; 
}

//把TkWords插入到哈希表中
V WordTable_put(HashMap* map, K spelling, V tkcode) {
    // 根据spelling获取索引
    int idx = hash(spelling);
    // 遍历链表
    TKWord* curr = map->tk_hashtable[idx];
    while (curr != NULL) {
        // 判断spelling是否存在
        if (strcmp(spelling, curr->spelling) == 0) {
            V oldValue = curr->tkcode;
            curr->tkcode = tkcode;
            return oldValue;
        }
        curr = curr->next;
    }

    // 不存在这样的 spelling, 添加结点，头插法
    // 创建键值对
    TKWord* word = (TKWord*)malloc(sizeof(TKWord));
    if (word == NULL) {
        printf("Error: malloc failed in HashMap_put.\n");
        exit(1);
    }
    word->spelling = spelling;
    word->tkcode = tkcode;
    word->next = map->tk_hashtable[idx];
    // 更新链表的头节点
    map->tk_hashtable[idx] = word;
    (map->count)++;
    return -1;
}

//移除哈希表中对应spelling的token
V WordTable_remove(HashMap* map, K spelling) {
    // 根据spelling获取索引
    int idx = hash(spelling);
    // 遍历链表
    TKWord* prev = NULL;
    TKWord* curr = map->tk_hashtable[idx];
    while (curr != NULL) {
        // 判断spelling是否存在
        if (strcmp(spelling, curr->spelling) == 0) {
            // 删除结点
            if (prev == NULL) {
                map->tk_hashtable[idx] = curr->next;
            }
            else {
                prev->next = curr->next;
            }
            // 释放空间
            V removeValue = curr->tkcode;
            free(curr);
            return removeValue;
        }
        prev = curr;
        curr = curr->next;
    }
    // 不存在这样的 spelling
    return -1;
}

double WordTable_factor(HashMap* map) {
    return (double)map->count / map->capacity;
}

//打印WordTable
void WordTable_print(HashMap* map) {
    printf("Word_HashMap: \n\n");

    for (int i = 0; i < MAXKEY; i++) {
        TKWord* curr = map->tk_hashtable[i];
        printf("T[%d]:", i);
        while (curr != NULL) {
            printf(" <\"%s\", %d>", curr->spelling, curr->tkcode);
            curr = curr->next;
        }
        printf("\n");
    }
    printf("\n");
    printf("装填因子：%.2f\n", WordTable_factor(map));
}



//简单的错误判断
Wrong* WrongList_create() {
    Wrong* wronglist = (Wrong*)calloc(1, sizeof(Wrong));
    if (wronglist == NULL) {
        printf("创建错误列表失败。\n");
        exit(1);
    }
    wronglist->wrongCount = 0;
    for (int i = 0; i < WRONGMAX; i++)
        wronglist->line[i] = 0;
    return wronglist;
}

void wrongAnalysis(TokenList* tokenList, Wrong* wrongList) {
    TKWord* curr = tokenList->head;

    //检测括号的变量
    int PA = 0, BRACE = 0, BRACKET = 0;
    int lastPa = 0, lastBrace = 0, lastBracket = 0;

    //检测分号的变量
    TKWord* prev = curr;

    while (curr != NULL) {
        //检测括号
        switch (curr->tkcode) {
        case TK_OPENPA:
            PA++;
            lastPa = curr->line;
            break;
        case TK_CLOSEPA:
            PA--;
            lastPa = curr->line;
            break;
        case TK_BEGIN:
            BRACE++;
            lastBrace = curr->line;
            break;
        case TK_END:
            BRACE--;
            lastBrace = curr->line;
            break;
        case TK_OPENBR:
            BRACKET++;
            lastBracket = curr->line;
            break;
        case TK_CLOSEBR:
            BRACKET--;
            lastBracket = curr->line;
            break;
        default:
            break;
        }
        
        //检测分号,若“}”前一个token不为“;”，则判断为错误
        if (curr->tkcode == TK_END && prev->tkcode != TK_SEMICOLON) {
            (wrongList->wrongCount)++;
            (wrongList->line[prev->line])++;
        }

        //检测错误的标识符
        if (curr->tkcode == TK_WRONGIDENT) {
            (wrongList->wrongCount)++;
            (wrongList->line[curr->line])++;
        }

        prev = curr;
        curr = curr->next;
    }


    //括号统计
    if (PA) {
        (wrongList->wrongCount)++;
        (wrongList->line[lastPa])++;
    }
    if (BRACE) {
        (wrongList->wrongCount)++;
        (wrongList->line[lastBrace])++;
    }
    if (BRACKET) {
        (wrongList->wrongCount)++;
        (wrongList->line[lastBracket])++;
    }
}

void WrongList_print(Wrong* wrongList) {
    printf("共有%d个错误：\n", wrongList->wrongCount);
    for (int i = 0; i < WRONGMAX; i++){
        if (wrongList->line[i]) {
            printf("line %d 有%d个错误。\n", i, wrongList->line[i]);
        }
    }
}

void WrongList_destroy(Wrong* wrongList) {
    free(wrongList);
}


//创建Token序列
TokenList* TokenList_create(){
    return (TokenList*)calloc(1, sizeof(TokenList));
}

//获取以字母开头的word的token
TokenCode Token_get(char* word, HashMap* map) {
    V token = WordTable_get(map, word);
    if (token == -1)
        return TK_IDENT;
    return token;
}


//统计未收录的标点符号
bool isotherpunct(char ch) {
    switch (ch) {
    case '@':
    case '$':
    case '`':
    case '#':
    case '^':
    case '~':
    case '_':
        return true;
        break;
    default:
        return false;
    }
}

//分析一行语句并输出色彩
void LineAnalysis(char* line, TokenList* tokenList, HashMap* map,Wrong* wrongList, int lineNum) {
    char ch = *line;
    char word[WORDMAX] = {0};
    
    //分析一行
    while (ch != '\0') {
        V token = -1;
        //若ch为运算符或分隔符
        switch (ch) {
        case '\n':
        case ' ':
            printf("%c", ch);
            ch = *(++line);
            continue;
            break;
        case '(':
            strcpy(word, "(");
            token = TK_OPENPA;
            ch = *(++line);
            break;
        case ')':
            strcpy(word, ")");
            token = TK_CLOSEPA;
            ch = *(++line);
            break;
        case '[':
            strcpy(word, "[");
            token = TK_OPENBR;
            ch = *(++line);
            break;
        case ']':
            strcpy(word, "]");
            token = TK_CLOSEBR;
            ch = *(++line);
            break;
        case '{':
            strcpy(word, "{");
            token = TK_BEGIN;
            ch = *(++line);
            break;
        case '}':
            strcpy(word, "}");
            token = TK_END;
            ch = *(++line);
            break;
        case ';':
            strcpy(word, ";");
            token = TK_SEMICOLON;
            ch = *(++line);
            break;
        case ',':
            strcpy(word, ",");
            token = TK_COMMA;
            ch = *(++line);
            break;
        case '"':
            word[0] = '\0';
            strncat(word, &ch, 1);
            while ((ch = *(++line)) != '"') {//读到下一个"之前
                strncat(word, &ch, 1);
            }
            strncat(word, &ch, 1);
            token = TK_CSTR;
            ch = *(++line);
            break;
        case '+':
            ch = *(++line);
            if (ch == '+') {
                strcpy(word, "++");
                token = TK_INCREMENT;
                ch = *(++line);
            }
            else {
                strcpy(word, "+");
                token = TK_PLUS;
            }
            break;
        case '-':
            ch = *(++line);
            if (ch == '-') {
                strcpy(word, "--");
                token = TK_DECREMENT;
                ch = *(++line);
            }
            else {
                strcpy(word, "-");
                token = TK_MINUS;
            }
            break;
        case '*':
            strcpy(word, "*");
            token = TK_STAR;
            ch = *(++line);
            break;
        case '/':
            //判断是否是注释，如果不是注释就认为是 /
            ch = *(++line);
            if (ch == '/') {
                word[0] = '\0';
                strcat(word, "\\\\");
                while ((ch = *(++line)) != '\n') {//整行读完
                    strncat(word, &ch, 1);
                }
                token = TK_EXPLAIN;
            }
            else if (ch == '*') {
                word[0] = '\0';
                strcat(word, "/*");
                char pre = ch;
                ch = *(++line);
                while (!(pre == '*' && ch == '/') && ch != '\0') { //读到/*
                    strncat(word, &ch, 1);
                    pre = ch;
                    ch = *(++line);
                } //可能导致注释过长
                token = TK_EXPLAIN;
                ch = *(++line);
            }
            else { //若都不是，那就是非法字符，视为标识符错误
                strcpy(word, "/");
                token = TK_DIVIDE;
                ch = *(++line);
            }

            
            break;
        case '%':
            strcpy(word, "%");
            token = TK_MOD;
            ch = *(++line);
            break;
        case '&':
            strcpy(word, "&");
            token = TK_AND;
            ch = *(++line);
            break;
        case '=':
            ch = *(++line);
            if (ch == '=') {
                strcpy(word, "==");
                token = TK_EQ;
                ch = *(++line);
            }
            else {
                strcpy(word, "=");
                token = TK_ASSIGN;
            }
            break;
        case '>':
            ch = *(++line);
            if (ch == '=') {
                strcpy(word, ">=");
                token = TK_GEQ;
                ch = *(++line);
            }
            else {
                strcpy(word, ">");
                token = TK_GT;
            }
            break;
        case '<':
            ch = *(++line);
            if (ch == '=') {
                strcpy(word, "<=");
                token = TK_LEQ;
                ch = *(++line);
            }
            else {
                strcpy(word, "<");
                token = TK_LT;
            }
            break;
        case '!':
            ch = *(++line);
            if (ch == '=') {
                strcpy(word, "!=");
                token = TK_GEQ;
                ch = *(++line);
            }
            else {
                strcpy(word, "!");
                token = TK_GT;
            }
            break;
        case '.':
            ch = *(++line);
            if (ch == '.') {
                if (*(++line) == '.') {
                    strcpy(word, "...");
                    token = TK_ELLIPSIS;
                    ch = *(++line);
                }
                else {
                    (wrongList->line[lineNum])++;
                    wrongList->wrongCount++;
                    ch = *(++line);
                } 
            }
            else {
                strcpy(word, ".");
                token = TK_DOT;
            }
            break;
        default:
            //若ch为常量
            if (isdigit(ch)) {
                word[0] = '\0';
                strncat(word, &ch, 1);
                while (isdigit(ch = *(++line))) {
                    strncat(word, &ch, 1);
                }
                token = TK_CINT; //只有数字，是整型常量
            }
            else if (isalpha(ch)) {
                //如果开头是字母，后面若只有字母，判断是否为关键字，若有下划线或者数字，就是标识符
                word[0] = '\0';
                strncat(word, &ch, 1);
                ch = *(++line);
                while (isalpha(ch) || isdigit(ch) || isotherpunct(ch)) {
                    if (isotherpunct(ch) && ch != '_') {
                        token = TK_WRONGIDENT; //如果有非下划线的字符，则将其列为错误的标识符
                    }
                    strncat(word, &ch, 1);
                    ch = *(++line);
                }
            }
            //若ch为关键字、标识符
            if (token == -1) {
                token = Token_get(word, map);
            }

            break;
        }
        

        //按颜色打印该token
        printfColor(word, token);

        //把该token插入序列中
        TokenList_insert(word, token, lineNum, tokenList);
    }
}

//插入Token
void TokenList_insert(char* word, TokenCode token, int lineNum, TokenList* tokenList){
    TKWord* newWord = (TKWord*)calloc(1, sizeof(TKWord));
    if (newWord == NULL) {
        printf("Token节点分配失败。\n");
        exit(1);
    }
    newWord->tkcode = token;
    newWord->spelling = (char* )calloc(WORDMAX, sizeof(char));
    newWord->line = lineNum;
    if (newWord->spelling == NULL) {
        printf("Token节点spelling分配失败。\n");
        exit(1);
    }
    (newWord->spelling)[0] = '\0';
    strcpy(newWord->spelling, word);

    if (tokenList->count == 0)
        tokenList->head = newWord;
    else
        tokenList->tail->next = newWord;
    tokenList->tail = newWord;
    (tokenList->count)++;
}



//销毁Token序列
void TokenList_destroy(TokenList* tokenList){
    TKWord* curr = tokenList->head;
    while (curr != NULL) {
        tokenList->head = curr->next;
        free(curr);
        curr = tokenList->head;
    }
    free(tokenList);
}

//按行输出token
void TokenList_printf(TokenList* tokenList) {
    TKWord* curr = tokenList->head;
    int pre = curr->line;
    printf("line %2d: ", 1);
    while (curr != NULL) {
        //暂时不知道怎么输出
        if (curr->line != pre) {
            printf("\n");
            printf("line %2d: " , curr->line);
        }   
        printf("<\"");
        printfColor(curr->spelling, curr->tkcode);
        printf("\", %d> ", curr->tkcode);
        pre = curr->line;
        curr = curr->next;
    }
}

//根据TOKEN值获取对应字符串类型，然后打印
void printfColor(char* ch, TokenCode token) {
    //设置颜色
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    if (token >= TK_IDENT)
        SetConsoleTextAttribute(h, FOREGROUND_INTENSITY); //函数显示灰色
    else if (token >= KW_CHAR)
        SetConsoleTextAttribute(h, FOREGROUND_GREEN, FOREGROUND_INTENSITY); //关键字显示为绿色
    else if (token >= TK_CINT)
        SetConsoleTextAttribute(h, FOREGROUND_RED | FOREGROUND_GREEN); //常量显示为褐色
    else
        SetConsoleTextAttribute(h, FOREGROUND_RED | FOREGROUND_INTENSITY); //运算符显示为红色

    //输出字符
    if (-1 == ch[0]) {
        printf("\n   End_Of_File!");
        SetConsoleTextAttribute(h, FOREGROUND_RED | FOREGROUND_GREEN |
            FOREGROUND_BLUE | FOREGROUND_INTENSITY);
    }
    else {
        printf("%s", ch);
    }

    //恢复颜色
    SetConsoleTextAttribute(h, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

}



//算术表达式分析
void ArithAnalysis(TokenList* tokenList) {

}

//语句体分析
void StatementAnalysis(TokenList* tokenList) {

}





//文件分析
void FileAnalysis(FILE* fp, HashMap* map) {
    //创建Token序列
    TokenList* myTokenList = TokenList_create();
    Wrong* myWrongList = WrongList_create();
    //分析每行并输出带色Token
    int lineNum = 0;
    char line[LINEMAX];

    while (fgets(line, LINEMAX, fp) != NULL) {
        printf("%3d |\t", ++lineNum);
        LineAnalysis(line, myTokenList, map, myWrongList, lineNum);
    }

    printf("\n\n");

    //通过tokenlist分析错误
    wrongAnalysis(myTokenList, myWrongList);

    //错误信息打印
    WrongList_print(myWrongList);
    WrongList_destroy(myWrongList);
    printf("\n\n");

    //测试：
    TokenList_printf(myTokenList);


    //结束后销毁
    TokenList_destroy(myTokenList);
    WordTable_destroy(map);
}

# SimpleCompiler
尝试写下编译器。

## 编译器词法语法分析

目前完成词法分析中的“带颜色输出”，找出错误（包括各种括号匹配，结尾的分号但是不彻底，标识符错误使用标点符号不符合规则），鉴别注释三个功能。

语法分析还在制作中。[衰]



### 运行结果：

#### 示例1

```tex
int main()
{//malegebi
int i;
for (int i = 0; i < 50; i++)

pri@tf("this is circle\n");
}
printf("Hello World!\n");
return 0;
}
```

```c
输出：

  1 |   int main()
  2 |   {\\malegebi
  3 |   int i;
  4 |   for (int i = 0; i < 50; i++)
  5 |
  6 |   pri@tf("this is circle\n");
  7 |   }
  8 |   printf("Hello World!\n");
  9 |   return 0;
 10 |   }


共有2个错误：
line 6 有1个错误。
line 10 有1个错误。


line  1: <"int", 32> <"main", 45> <"(", 17> <")", 18>
line  2: <"{", 21> <"\\malegebi", 47>
line  3: <"int", 32> <"i", 45> <";", 23>
line  4: <"for", 37> <"(", 17> <"int", 32> <"i", 45> <"=", 13> <"0", 27> <";", 23> <"i", 45> <"<", 9> <"50", 27> <";", 23> <"i", 45> <"++", 4> <")", 18>
line  6: <"pri@tf", 46> <"(", 17> <""this is circle\n"", 29> <")", 18> <";", 23>
line  7: <"}", 22>
line  8: <"printf", 45> <"(", 17> <""Hello World!\n"", 29> <")", 18> <";", 23>
line  9: <"return", 40> <"0", 27> <";", 23>
line 10: <"}", 22>
```



#### 单词哈希表

```c
Word_HashMap:

T[0]:
T[1]:
T[2]: <"for", 37>
T[3]: <"错误的函数名或变量名", 46>
T[4]:
T[5]:
T[6]:
T[7]:
T[8]:
T[9]:
T[10]:
T[11]:
T[12]: <"struct", 34>
T[13]:
T[14]:
T[15]:
T[16]:
T[17]:
T[18]: <"_align", 42>
T[19]: <"字符常量", 28>
T[20]:
T[21]: <"<=", 10>
T[22]: <"return", 40>
T[23]: <"{", 21>
T[24]: <"void", 33>
T[25]: <"}", 22>
T[26]:
T[27]:
T[28]:
T[29]:
T[30]:
T[31]: <"函数名，变量名", 45> <"++", 4>
T[32]: <"_stdcall", 44> <"_cdecl", 43>
T[33]:
T[34]:
T[35]:
T[36]:
T[37]: <"==", 7>
T[38]: <"&", 16>
T[39]:
T[40]: <"(", 17>
T[41]: <")", 18>
T[42]: <"*", 2>
T[43]: <"整型常量", 27> <"+", 0>
T[44]: <",", 24>
T[45]: <"-", 1>
T[46]: <".", 15>
T[47]: <"/", 3>
T[48]:
T[49]:
T[50]:
T[51]:
T[52]:
T[53]: <">=", 12>
T[54]:
T[55]:
T[56]: <"int", 32>
T[57]:
T[58]: <"...", 25>
T[59]: <";", 23>
T[60]: <"<", 9>
T[61]: <"continue", 38> <"=", 13>
T[62]: <">", 11>
T[63]:
T[64]:
T[65]: <"注释", 47> <"--", 5>
T[66]: <"sizeof", 41> <"字符串", 29>
T[67]:
T[68]:
T[69]:
T[70]:
T[71]:
T[72]:
T[73]:
T[74]:
T[75]:
T[76]:
T[77]:
T[78]:
T[79]:
T[80]: <"short", 31>
T[81]:
T[82]: <"if", 35> <"->", 14>
T[83]:
T[84]:
T[85]: <"else", 36>
T[86]:
T[87]: <"break", 39>
T[88]:
T[89]: <"!=", 8>
T[90]:
T[91]: <"[", 19>
T[92]:
T[93]: <"]", 20>
T[94]: <"char", 30>
T[95]:
T[96]:
T[97]:
T[98]: <"EOF", 26>
T[99]:

装填因子：0.47

```



### 代码

#### main.c

```c
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include "grammar.h"

int main() {
	//创建哈希表
	HashMap* myTable = WordTable_create();
	//测试：
	WordTable_print(myTable);
	printf("\n");

	//打开文件
	FILE* fp = fopen("sample1.txt", "r");
	if (fp == NULL)
	{
		printf("Error: Faild to open sample1.c\n");
		exit(1);
	}

	//分析文件
	FileAnalysis(fp, myTable);

	//关闭文件
	fclose(fp);
	return 0;
}
```



#### grammar.h

```c
#pragma once
#include<stdio.h>
#include<string.h>
#include<stdbool.h>
#include <stdlib.h>
#include <windows.h>
#include "dict.h"

//虽然加了算术表达式的头文件但其实还没写
#include "arithanalysis.h"

//函数语句体分析
#include "statementanalysis.h"


#define MAXKEY 100

#define LINEMAX 50
#define WORDMAX 30
#define WRONGMAX 30

typedef char* K;
typedef int V;



//单词存储结构定义
typedef struct tkWord_s { //Token结点
	V tkcode;
	K spelling;
	struct tkWord_s *next;
	int line;
} TKWord;

typedef struct tokenList_s { //Token序列
	TKWord* head, * tail;
	int count;
} TokenList;

typedef struct { //哈希表，此处征用老师写的哈希表
	TKWord* tk_hashtable[MAXKEY];
	int count;
	int capacity;
} HashMap;

typedef struct grammar_t{
	char first[20];
	int second;
}Grammar_T;

//静态结构体数组
static TKWord keywords[] = {
	/*运算符*/
	{TK_PLUS, "+", NULL,-1},
	{TK_MINUS, "-", NULL,-1},
	{TK_STAR, "*", NULL,-1},
	{TK_DIVIDE, "/", NULL,-1},
	{TK_INCREMENT, "++", NULL,- 1},
	{TK_DECREMENT, "--", NULL,-1},
	{TK_EQ, "==", NULL,-1},
	{TK_NEQ, "!=", NULL,-1},
	{TK_LT, "<", NULL,-1},
	{TK_LEQ, "<=", NULL,-1},
	{TK_GT, ">", NULL,-1},
	{TK_GEQ, ">=", NULL,-1},
	{TK_ASSIGN, "=", NULL,-1},
	{TK_POINTSTO, "->", NULL,-1},
	{TK_DOT,".", NULL,-1},
	{TK_AND,"&", NULL,-1},
	{TK_OPENPA, "(", NULL,-1},
	{TK_CLOSEPA, ")", NULL,-1},
	{TK_OPENBR, "[", NULL,-1},
	{TK_CLOSEBR, "]", NULL,-1},
	{TK_BEGIN, "{", NULL,-1},
	{TK_END, "}", NULL,-1},
	{TK_SEMICOLON, ";", NULL,-1},
	{TK_COMMA, ",", NULL,-1},
	{TK_ELLIPSIS, "...", NULL,-1},
	{TK_EOF,"EOF", NULL,-1},

	/*常量*/
	{TK_CINT, "整型常量", NULL,-1},
	{TK_CCHAR,"字符常量", NULL,-1},
	{TK_CSTR, "字符串", NULL,-1},
	{TK_EXPLAIN, "注释", NULL, -1},

	/*关键字*/
	{KW_CHAR, "char", NULL,-1},
	{KW_SHORT, "short", NULL,-1},
	{KW_INT, "int", NULL,-1},
	{KW_VOID, "void", NULL,-1},
	{KW_STRUCT, "struct", NULL,-1},
	{KW_IF, "if", NULL,-1},
	{KW_ELSE, "else", NULL,-1},
	{KW_FOR, "for", NULL,-1},
	{KW_CONTINUE, "continue", NULL,-1},
	{KW_BREAK, "break", NULL,-1},
	{KW_RETURN, "return", NULL,-1},
	{KE_SIZEOF, "sizeof", NULL,-1},

	{KW_ALIGN, "_align", NULL,-1},
	{KW_CDECL, "_cdecl", NULL,-1},
	{KW_STDCALL, "_stdcall", NULL,-1},

	/*标识符*/
	{TK_IDENT, "函数名，变量名", NULL,-1},

	/*错误的标识符或啥也不是*/
	{TK_WRONGIDENT, "错误的函数名或变量名", NULL, -1}
};

typedef struct wrong_s {
	int wrongCount; //总错误数
	int line[WRONGMAX];//记录每行的错误数
}Wrong;



//文件分析
void FileAnalysis(FILE* fp, HashMap* map);


//哈希表函数们（此处CV了老师的代码）
int hash(char* key);
HashMap* WordTable_create();
V WordTable_get(HashMap* map, K key);
V WordTable_put(HashMap* map, K key, V val);
V WordTable_remove(HashMap* map, K key);
void WordTable_print(HashMap* map);
void WordTable_destroy(HashMap* map);
double WordTable_factor(HashMap* map);


//错误记录相关
Wrong* WrongList_create();

void WrongList_print(Wrong* wrongList);

void wrongAnalysis(TokenList* tokenList, Wrong* wrongList);

void WrongList_destroy(Wrong* wrongList);



//Token序列相关函数

//创建Token序列
TokenList* TokenList_create();

//插入Token序列
void TokenList_insert(char* word, TokenCode token, int lineNum, TokenList* tokenList);

//获取以字母开头的word的token
TokenCode Token_get(char* word, HashMap* map);

//分析一行语句并输出色彩，仅包括词法分析，生成对应的Token序列
void LineAnalysis(char* line, TokenList* tokenList, HashMap* map, Wrong* wrongList, int lineNum);

//颜色打印函数
void printfColor(char* ch, TokenCode token);

//逐个TokenList，暂时不知道怎么打印
void TokenList_printf(TokenList* tokenList);

//销毁Token序列
void TokenList_destroy(TokenList* tokenList);


//语法分析

//算术表达式分析
void ArithAnalysis(TokenList* tokenList);

//语句体分析
void StatementAnalysis(TokenList* tokenList);




```



#### grammar.c

```c
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

```



#### dict.h

```c
#pragma once

typedef enum e_TokenCode {
	//运算符及分隔符
	TK_PLUS,	  // +
	TK_MINUS,	  // -
	TK_STAR,	  // *
	TK_DIVIDE,	  // /
	TK_INCREMENT, // ++
	TK_DECREMENT, //--
	TK_MOD,		  // %
	TK_EQ,		  // ==
	TK_NEQ,		  // !=
	TK_LT,		  // <
	TK_LEQ,		  // <=
	TK_GT,		  // >
	TK_GEQ,		  //>=
	TK_ASSIGN,	  // =
	TK_POINTSTO,  // ->
	TK_DOT,		  // .
	TK_AND,		  // &
	TK_OPENPA,	  //(
	TK_CLOSEPA,	  //)
	TK_OPENBR,    //[
	TK_CLOSEBR,   //]
	TK_BEGIN,	  //{
	TK_END,		  //}
	TK_SEMICOLON, //;
	TK_COMMA,	  //,
	TK_ELLIPSIS,  // ...
	TK_EOF,		 // 文件结束符

	//常量
	TK_CINT, //整型常量
	TK_CCHAR, // 字符常量
	TK_CSTR, //字符串常量


	//关键字
	KW_CHAR,	 // char
	KW_SHORT,	 // short
	KW_INT,		 // int
	KW_VOID,	 // void
	KW_STRUCT,	 // struct
	KW_IF,		 // if
	KW_ELSE,	 // else
	KW_FOR,		 // for
	KW_CONTINUE, // continue
	KW_BREAK,	 // break
	KW_RETURN,	 // return
	KE_SIZEOF,	 // sizeof
	KW_ALIGN,    //_align
	KW_CDECL,    //_cdecl
	KW_STDCALL,  //_stdcall

	//标识符
	TK_IDENT,    //函数名，变量名
	TK_WRONGIDENT, //错误的函数名或变量名

	//注释
	TK_EXPLAIN   //注释，虽然我也不知道怎么定义这个东西


}TokenCode;

```





#### arithanalysis.h

```c
#pragma once
#include<stdio.h>
#include<stdlib.h>
#include<string.h>


//这里是抄书，之后有空再改吧
typedef struct {
	char first[20];
	int second;
}Grammar_t;

void T();
void F();

int word_analysis(Grammar_t* word, const char* expr);

void Next();

void E();

void T();

void F();

int arithanalysis_main();

```



#### arithanalysis.c

```c
#include "arithanalysis.h"

char expr[50] = "";
Grammar_t word[100] = { 0 };
int wordLen = 0;
int idx = 0;
int sym;
int err = 0;

int word_analysis(Grammar_t* word, const char* expr) {
	return 1;
}


void Next() {
	if (idx < wordLen)
		sym = word[idx++].second;
	else
		sym = 0;
}

void E() {
	T();
	while (sym == 1 || sym == 2) {
		Next();
		T();
	}
}

void T() {
	F();
	while (sym == 3 || sym == 4) {
		Next();
		F();
	}
}

void F() {
	if (sym == 5) {
		Next();
	}
	else if (sym == 6) {
		Next();
		E();
		if (sym == 7) {
			Next();
		}
		else {
			err = -1;
		}
	}
	else {
		err = -1;
		puts("Wrong Expression.");
		system("pause");
	}
}

int arithanalysis_main() {
	int i;
	int err_num = word_analysis(word, expr);
	if (err_num == -1) {
		puts("Word Error!");
	}
	else {
		//词法分析成功，测试输出
		for (i = 0; i < wordLen; i++) {
			printf("%s %d\n", word[i].first, word[i].second);
		}
		//词法正确，进行语法分析
		Next();
		E();
		if (sym == 0 && err == 0)
			puts("Right Expression.");
		else
			puts("Wrong Expression.");
	}
	system("pause");
	return 0;
}

```



#### statementanalysis.h

```

```



#### statementanalysis.c

```

```


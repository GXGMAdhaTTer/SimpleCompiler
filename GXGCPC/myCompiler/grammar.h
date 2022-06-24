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

//typedef struct dynArray_words {
//	int count;
//	int capacity;
//	TKWord** tk_hashtable;
//}MyHashMap;

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




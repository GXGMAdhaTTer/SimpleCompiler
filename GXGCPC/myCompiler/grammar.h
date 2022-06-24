#pragma once
#include<stdio.h>
#include<string.h>
#include<stdbool.h>
#include <stdlib.h>
#include <windows.h>
#include "dict.h"

//��Ȼ�����������ʽ��ͷ�ļ�����ʵ��ûд
#include "arithanalysis.h"

//������������
#include "statementanalysis.h"


#define MAXKEY 100

#define LINEMAX 50
#define WORDMAX 30
#define WRONGMAX 30

typedef char* K;
typedef int V;



//���ʴ洢�ṹ����
typedef struct tkWord_s { //Token���
	V tkcode;
	K spelling;
	struct tkWord_s *next;
	int line;
} TKWord;

typedef struct tokenList_s { //Token����
	TKWord* head, * tail;
	int count;
} TokenList;

//typedef struct dynArray_words {
//	int count;
//	int capacity;
//	TKWord** tk_hashtable;
//}MyHashMap;

typedef struct { //��ϣ���˴�������ʦд�Ĺ�ϣ��
	TKWord* tk_hashtable[MAXKEY];
	int count;
	int capacity;
} HashMap;

typedef struct grammar_t{
	char first[20];
	int second;
}Grammar_T;

//��̬�ṹ������
static TKWord keywords[] = {
	/*�����*/
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

	/*����*/
	{TK_CINT, "���ͳ���", NULL,-1},
	{TK_CCHAR,"�ַ�����", NULL,-1},
	{TK_CSTR, "�ַ���", NULL,-1},
	{TK_EXPLAIN, "ע��", NULL, -1},

	/*�ؼ���*/
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

	/*��ʶ��*/
	{TK_IDENT, "��������������", NULL,-1},

	/*����ı�ʶ����ɶҲ����*/
	{TK_WRONGIDENT, "����ĺ������������", NULL, -1}
};

typedef struct wrong_s {
	int wrongCount; //�ܴ�����
	int line[WRONGMAX];//��¼ÿ�еĴ�����
}Wrong;



//�ļ�����
void FileAnalysis(FILE* fp, HashMap* map);


//��ϣ�����ǣ��˴�CV����ʦ�Ĵ��룩
int hash(char* key);
HashMap* WordTable_create();
V WordTable_get(HashMap* map, K key);
V WordTable_put(HashMap* map, K key, V val);
V WordTable_remove(HashMap* map, K key);
void WordTable_print(HashMap* map);
void WordTable_destroy(HashMap* map);
double WordTable_factor(HashMap* map);


//�����¼���
Wrong* WrongList_create();

void WrongList_print(Wrong* wrongList);

void wrongAnalysis(TokenList* tokenList, Wrong* wrongList);

void WrongList_destroy(Wrong* wrongList);



//Token������غ���

//����Token����
TokenList* TokenList_create();

//����Token����
void TokenList_insert(char* word, TokenCode token, int lineNum, TokenList* tokenList);

//��ȡ����ĸ��ͷ��word��token
TokenCode Token_get(char* word, HashMap* map);

//����һ����䲢���ɫ�ʣ��������ʷ����������ɶ�Ӧ��Token����
void LineAnalysis(char* line, TokenList* tokenList, HashMap* map, Wrong* wrongList, int lineNum);

//��ɫ��ӡ����
void printfColor(char* ch, TokenCode token);

//���TokenList����ʱ��֪����ô��ӡ
void TokenList_printf(TokenList* tokenList);

//����Token����
void TokenList_destroy(TokenList* tokenList);


//�﷨����

//�������ʽ����
void ArithAnalysis(TokenList* tokenList);

//��������
void StatementAnalysis(TokenList* tokenList);




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

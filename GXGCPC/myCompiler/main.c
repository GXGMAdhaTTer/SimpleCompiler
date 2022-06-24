#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include "grammar.h"

int main() {
	//创建哈希表
	HashMap* myTable = WordTable_create();
	//测试：
	WordTable_print(myTable);
	printf("\n");

	//打开.c文件
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
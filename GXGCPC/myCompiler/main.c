#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include "grammar.h"

int main() {
	//������ϣ��
	HashMap* myTable = WordTable_create();
	//���ԣ�
	WordTable_print(myTable);
	printf("\n");

	//��.c�ļ�
	FILE* fp = fopen("sample1.txt", "r");
	if (fp == NULL)
	{
		printf("Error: Faild to open sample1.c\n");
		exit(1);
	}

	//�����ļ�
	FileAnalysis(fp, myTable);

	//�ر��ļ�
	fclose(fp);
	return 0;
}
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

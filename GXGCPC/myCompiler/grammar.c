#define _CRT_SECURE_NO_WARNINGS
#include "grammar.h"
#include "dict.h"

// �����յĹ�ϣ��
HashMap* WordTable_create() {
    HashMap* wordTable = (HashMap*)calloc(1, sizeof(HashMap));

    if (wordTable == NULL){
        printf("������ϣ��ʧ�ܡ�\n"); 
        exit(1);
    }

    wordTable->count = 0;
    wordTable->capacity = MAXKEY;
    for (int i = 0; i < (sizeof(keywords) / sizeof(keywords[0])); i++) {
        WordTable_put(wordTable, keywords[i].spelling, keywords[i].tkcode);
    }
    return wordTable;
}

//���ٹ�ϣ��
void WordTable_destroy(HashMap* map) {
    // �ͷ����н��
    for (int i = 0; i < MAXKEY; i++) {
        TKWord* curr = map->tk_hashtable[i];
        while (curr != NULL) {
            // �����̽��
            TKWord* next = curr->next;
            free(curr);
            curr = next;
        }
    }
    // �ͷ�HashMap�ṹ��
    free(map);
}

//�����ϣ����
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

//����spelling����tkcode
V WordTable_get(HashMap* map, K spelling) {
    // ����spelling��ȡ����
    int idx = hash(spelling);
    // ��������
    TKWord* curr = map->tk_hashtable[idx];
    while (curr != NULL) {
        // �ж�spelling�Ƿ����
        if (strcmp(spelling, curr->spelling) == 0) {
            return curr->tkcode;
        }
        curr = curr->next;
    }
    // ������������ spelling
    return -1; 
}

//��TkWords���뵽��ϣ����
V WordTable_put(HashMap* map, K spelling, V tkcode) {
    // ����spelling��ȡ����
    int idx = hash(spelling);
    // ��������
    TKWord* curr = map->tk_hashtable[idx];
    while (curr != NULL) {
        // �ж�spelling�Ƿ����
        if (strcmp(spelling, curr->spelling) == 0) {
            V oldValue = curr->tkcode;
            curr->tkcode = tkcode;
            return oldValue;
        }
        curr = curr->next;
    }

    // ������������ spelling, ��ӽ�㣬ͷ�巨
    // ������ֵ��
    TKWord* word = (TKWord*)malloc(sizeof(TKWord));
    if (word == NULL) {
        printf("Error: malloc failed in HashMap_put.\n");
        exit(1);
    }
    word->spelling = spelling;
    word->tkcode = tkcode;
    word->next = map->tk_hashtable[idx];
    // ���������ͷ�ڵ�
    map->tk_hashtable[idx] = word;
    (map->count)++;
    return -1;
}

//�Ƴ���ϣ���ж�Ӧspelling��token
V WordTable_remove(HashMap* map, K spelling) {
    // ����spelling��ȡ����
    int idx = hash(spelling);
    // ��������
    TKWord* prev = NULL;
    TKWord* curr = map->tk_hashtable[idx];
    while (curr != NULL) {
        // �ж�spelling�Ƿ����
        if (strcmp(spelling, curr->spelling) == 0) {
            // ɾ�����
            if (prev == NULL) {
                map->tk_hashtable[idx] = curr->next;
            }
            else {
                prev->next = curr->next;
            }
            // �ͷſռ�
            V removeValue = curr->tkcode;
            free(curr);
            return removeValue;
        }
        prev = curr;
        curr = curr->next;
    }
    // ������������ spelling
    return -1;
}

double WordTable_factor(HashMap* map) {
    return (double)map->count / map->capacity;
}

//��ӡWordTable
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
    printf("װ�����ӣ�%.2f\n", WordTable_factor(map));
}



//�򵥵Ĵ����ж�
Wrong* WrongList_create() {
    Wrong* wronglist = (Wrong*)calloc(1, sizeof(Wrong));
    if (wronglist == NULL) {
        printf("���������б�ʧ�ܡ�\n");
        exit(1);
    }
    wronglist->wrongCount = 0;
    for (int i = 0; i < WRONGMAX; i++)
        wronglist->line[i] = 0;
    return wronglist;
}

void wrongAnalysis(TokenList* tokenList, Wrong* wrongList) {
    TKWord* curr = tokenList->head;

    //������ŵı���
    int PA = 0, BRACE = 0, BRACKET = 0;
    int lastPa = 0, lastBrace = 0, lastBracket = 0;

    //���ֺŵı���
    TKWord* prev = curr;

    while (curr != NULL) {
        //�������
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
        
        //���ֺ�,����}��ǰһ��token��Ϊ��;�������ж�Ϊ����
        if (curr->tkcode == TK_END && prev->tkcode != TK_SEMICOLON) {
            (wrongList->wrongCount)++;
            (wrongList->line[prev->line])++;
        }

        //������ı�ʶ��
        if (curr->tkcode == TK_WRONGIDENT) {
            (wrongList->wrongCount)++;
            (wrongList->line[curr->line])++;
        }

        prev = curr;
        curr = curr->next;
    }


    //����ͳ��
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
    printf("����%d������\n", wrongList->wrongCount);
    for (int i = 0; i < WRONGMAX; i++){
        if (wrongList->line[i]) {
            printf("line %d ��%d������\n", i, wrongList->line[i]);
        }
    }
}

void WrongList_destroy(Wrong* wrongList) {
    free(wrongList);
}


//����Token����
TokenList* TokenList_create(){
    return (TokenList*)calloc(1, sizeof(TokenList));
}

//��ȡ����ĸ��ͷ��word��token
TokenCode Token_get(char* word, HashMap* map) {
    V token = WordTable_get(map, word);
    if (token == -1)
        return TK_IDENT;
    return token;
}


//ͳ��δ��¼�ı�����
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

//����һ����䲢���ɫ��
void LineAnalysis(char* line, TokenList* tokenList, HashMap* map,Wrong* wrongList, int lineNum) {
    char ch = *line;
    char word[WORDMAX] = {0};
    
    //����һ��
    while (ch != '\0') {
        V token = -1;
        //��chΪ�������ָ���
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
            while ((ch = *(++line)) != '"') {//������һ��"֮ǰ
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
            //�ж��Ƿ���ע�ͣ��������ע�;���Ϊ�� /
            ch = *(++line);
            if (ch == '/') {
                word[0] = '\0';
                strcat(word, "\\\\");
                while ((ch = *(++line)) != '\n') {//���ж���
                    strncat(word, &ch, 1);
                }
                token = TK_EXPLAIN;
            }
            else if (ch == '*') {
                word[0] = '\0';
                strcat(word, "/*");
                char pre = ch;
                ch = *(++line);
                while (!(pre == '*' && ch == '/') && ch != '\0') { //����/*
                    strncat(word, &ch, 1);
                    pre = ch;
                    ch = *(++line);
                } //���ܵ���ע�͹���
                token = TK_EXPLAIN;
                ch = *(++line);
            }
            else { //�������ǣ��Ǿ��ǷǷ��ַ�����Ϊ��ʶ������
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
            //��chΪ����
            if (isdigit(ch)) {
                word[0] = '\0';
                strncat(word, &ch, 1);
                while (isdigit(ch = *(++line))) {
                    strncat(word, &ch, 1);
                }
                token = TK_CINT; //ֻ�����֣������ͳ���
            }
            else if (isalpha(ch)) {
                //�����ͷ����ĸ��������ֻ����ĸ���ж��Ƿ�Ϊ�ؼ��֣������»��߻������֣����Ǳ�ʶ��
                word[0] = '\0';
                strncat(word, &ch, 1);
                ch = *(++line);
                while (isalpha(ch) || isdigit(ch) || isotherpunct(ch)) {
                    if (isotherpunct(ch) && ch != '_') {
                        token = TK_WRONGIDENT; //����з��»��ߵ��ַ���������Ϊ����ı�ʶ��
                    }
                    strncat(word, &ch, 1);
                    ch = *(++line);
                }
            }
            //��chΪ�ؼ��֡���ʶ��
            if (token == -1) {
                token = Token_get(word, map);
            }

            break;
        }
        

        //����ɫ��ӡ��token
        printfColor(word, token);

        //�Ѹ�token����������
        TokenList_insert(word, token, lineNum, tokenList);
    }
}

//����Token
void TokenList_insert(char* word, TokenCode token, int lineNum, TokenList* tokenList){
    TKWord* newWord = (TKWord*)calloc(1, sizeof(TKWord));
    if (newWord == NULL) {
        printf("Token�ڵ����ʧ�ܡ�\n");
        exit(1);
    }
    newWord->tkcode = token;
    newWord->spelling = (char* )calloc(WORDMAX, sizeof(char));
    newWord->line = lineNum;
    if (newWord->spelling == NULL) {
        printf("Token�ڵ�spelling����ʧ�ܡ�\n");
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



//����Token����
void TokenList_destroy(TokenList* tokenList){
    TKWord* curr = tokenList->head;
    while (curr != NULL) {
        tokenList->head = curr->next;
        free(curr);
        curr = tokenList->head;
    }
    free(tokenList);
}

//�������token
void TokenList_printf(TokenList* tokenList) {
    TKWord* curr = tokenList->head;
    int pre = curr->line;
    printf("line %2d: ", 1);
    while (curr != NULL) {
        //��ʱ��֪����ô���
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

//����TOKENֵ��ȡ��Ӧ�ַ������ͣ�Ȼ���ӡ
void printfColor(char* ch, TokenCode token) {
    //������ɫ
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    if (token >= TK_IDENT)
        SetConsoleTextAttribute(h, FOREGROUND_INTENSITY); //������ʾ��ɫ
    else if (token >= KW_CHAR)
        SetConsoleTextAttribute(h, FOREGROUND_GREEN, FOREGROUND_INTENSITY); //�ؼ�����ʾΪ��ɫ
    else if (token >= TK_CINT)
        SetConsoleTextAttribute(h, FOREGROUND_RED | FOREGROUND_GREEN); //������ʾΪ��ɫ
    else
        SetConsoleTextAttribute(h, FOREGROUND_RED | FOREGROUND_INTENSITY); //�������ʾΪ��ɫ

    //����ַ�
    if (-1 == ch[0]) {
        printf("\n   End_Of_File!");
        SetConsoleTextAttribute(h, FOREGROUND_RED | FOREGROUND_GREEN |
            FOREGROUND_BLUE | FOREGROUND_INTENSITY);
    }
    else {
        printf("%s", ch);
    }

    //�ָ���ɫ
    SetConsoleTextAttribute(h, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

}



//�������ʽ����
void ArithAnalysis(TokenList* tokenList) {

}

//��������
void StatementAnalysis(TokenList* tokenList) {

}





//�ļ�����
void FileAnalysis(FILE* fp, HashMap* map) {
    //����Token����
    TokenList* myTokenList = TokenList_create();
    Wrong* myWrongList = WrongList_create();
    //����ÿ�в������ɫToken
    int lineNum = 0;
    char line[LINEMAX];

    while (fgets(line, LINEMAX, fp) != NULL) {
        printf("%3d |\t", ++lineNum);
        LineAnalysis(line, myTokenList, map, myWrongList, lineNum);
    }

    printf("\n\n");

    //ͨ��tokenlist��������
    wrongAnalysis(myTokenList, myWrongList);

    //������Ϣ��ӡ
    WrongList_print(myWrongList);
    WrongList_destroy(myWrongList);
    printf("\n\n");

    //���ԣ�
    TokenList_printf(myTokenList);


    //����������
    TokenList_destroy(myTokenList);
    WordTable_destroy(map);
}

#pragma once

typedef enum e_TokenCode {
	//��������ָ���
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
	TK_EOF,		 // �ļ�������

	//����
	TK_CINT, //���ͳ���
	TK_CCHAR, // �ַ�����
	TK_CSTR, //�ַ�������


	//�ؼ���
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

	//��ʶ��
	TK_IDENT,    //��������������
	TK_WRONGIDENT, //����ĺ������������

	//ע��
	TK_EXPLAIN   //ע�ͣ���Ȼ��Ҳ��֪����ô�����������


}TokenCode;






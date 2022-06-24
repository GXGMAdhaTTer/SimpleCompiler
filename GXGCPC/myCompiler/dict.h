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






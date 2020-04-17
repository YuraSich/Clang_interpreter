#ifndef defs

#include <iostream>
#include <fstream>
#include <string.h>

#define MAX_TEXT 10000 // ������������ ����� ������
#define MAX_LEX 100 // ������������ ����� �������
#define MAX_KEYW 5 // ����� �������� ����
typedef char TypeLex[MAX_LEX];
typedef char TypeMod[MAX_TEXT];



//�������� �����
#define TInt        1
#define TUnsigned   2
#define TWhile      3
#define TMain       4
#define TReturn     5

//�������������� � ���������
#define TIdent          20
#define TConstInt_10    30
#define TConstInt_8     31
#define TConstInt_16    32
#define TConstChar      33

//����������� �����
#define TParent_O   40
#define TParent_C   41
#define TBrace_O    42
#define TBrace_C    43
#define TComma      44

//����� ��������
#define TStar        60
#define TRSlash       61
#define TPercent     62
#define TPlus        63
#define TMinus       64
#define TShift_L     65
#define TShift_R     66
#define TGreat       67
#define TLess        68
#define TGreat_EQ    69
#define TLess_EQ     70
#define TEQ          71
#define TNot_EQ      72
#define TAssign      73
#define TSemicolon   74
#define TDPlus       75
#define TDMinus      76


//����� ��������� ������
#define TEnd   100
//��������� ������
#define TErr   200

#endif

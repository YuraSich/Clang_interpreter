#include "defs.h"
#include "Scaner.h"
#include <clocale>
#include <iostream>

static TypeLex Keyword[MAX_KEYW] = { "int", "unsigned", "while", "main", "return"};
static int IndexKeyword[MAX_KEYW]= {TInt, TUnsigned, TWhile, TMain, TReturn};
using namespace std;

TScaner:: TScaner(const char * FileName) {
        GetData(FileName);
        PutUK(0,0,1);
}

void TScaner::PutUK(int i, int j, int k){
        uk=i;
        pos = j;
        line = k;
}                                 //������������ ���������

void TScaner::GetUK(int * i, int * j, int * k){
        *i = uk;
        *j = pos;
        *k = line;
}                                    // ��������� ���������

void TScaner::PrintError(const char * err, char * a) {
        // ������ ��������� �� ������
        if (a==nullptr)
                printf("������ : %s %s(line: %d pos: %d)\n",err,a,line,pos);
        else
                printf("������ : %s. �������� ������ \'%s\'\t(line: %d pos: %d)\n",err,a,line,pos);
        exit(0);
}

void TScaner::PrintError() {
        // ������ ��������� �� ������
        printf("(line: %d pos: %d)\n",line,pos);
        exit(0);
}
int TScaner::Scaner(TypeLex l) // ��������� �������
{

        int i; // ������� ����� �������
        for (i=0; i<MAX_LEX; i++) l[i]=0; //�������� ���� �������
        i=0; // ������� ����������� � ������� i
start:
        while((t[uk]==' ' ) || (t[uk]=='\n') || (t[uk]=='\t')) {
                if (t[uk]==' ' ) {uk++; pos++;}
                if (t[uk]=='\n') {uk++; line++; pos = 0;}
                if (t[uk]=='\t') {uk++; pos+= 4;}
        }                                 // ������� ���������� ���������

        if ( (t[uk]=='/') && (t[uk+1]=='/') )
        { // ������� �����������, ���� ���������� ����� �� �\n�
                uk+=2;
                pos+=2;
                while ( (t[uk]!='\n')&&(t[uk]!='#')) {uk++; pos++;}
                goto start;
        }
        if (t[uk]=='\0') {l[0]='#'; return TEnd;}

        if (t[uk]<='9'&&t[uk]>='0') {
                if(t[uk] == '0') {
                        if(t[uk+1] == 'x') {
                                uk+=2;
                                pos+=2;
                                while (( (t[uk]<='9')&&(t[uk]>='0'))|| ( (t[uk]<='F')&&(t[uk]>='A')))
                                        if (i<MAX_LEX-1) {l[i++]=t[uk++]; pos++;} else {uk++; pos++;}
                                return TConstInt_16;
                        }
                        else {
                                uk++;
                                pos+=2;
                                while (( (t[uk]<='7')&&(t[uk]>='0')))
                                        if (i<MAX_LEX-1) {l[i++]=t[uk++]; pos++;} else {uk++; pos++;}
                                return TConstInt_8;
                        }

                }
                else {
                        while (( (t[uk]<='9')&&(t[uk]>='0')))
                                if (i<MAX_LEX-1) {l[i++]=t[uk++]; pos++;} else {uk++; pos++;}
                        return TConstInt_10;
                }
        }
        else if ( ((t[uk]>='a')&&(t[uk]<='z')) ||
                  ((t[uk]>='A')&&(t[uk]<='Z')) ) {
                // ���������� �������������
                l[i++]=t[uk++];
                pos++;
                while ( ((t[uk]<='9')&&(t[uk]>='0')) ||
                        ((t[uk]>='a')&&(t[uk]<='z')) ||
                        ((t[uk]>='A')&&(t[uk]<='Z')) || t[uk] == '_')
                        if (i<MAX_LEX-1) {l[i++]=t[uk++]; pos++;} else {uk++; pos++;}
                // ������� ������������� ��������
                int j; // �������� �� �������� �����:
                for (j=0; j<MAX_KEYW; j++)
                        if (strcmp(l, Keyword[j]) == 0) return IndexKeyword[j];
                return TIdent;
        }
        else if (t[uk]=='\'')
        {
                uk++;
                pos++;
                while ((t[uk] != '\'') && (t[uk] != '\n') && (t[uk] != '#')) {
                        if(i<MAX_LEX) {l[i++]=t[uk++]; pos++;} else {uk++; pos++;}
                }
                if (t[uk] != '\'') {
                        PrintError("�������� ���������� ���������", l);
                        return TErr;
                }
                uk++;
                return TConstChar;
        }

        else if (t[uk]=='*')
        { {l[i++]=t[uk++]; pos++;} return TStar; }
        else if (t[uk]=='/')
        { {l[i++]=t[uk++]; pos++;} return TRSlash; }
        else if (t[uk]=='%')
        { {l[i++]=t[uk++]; pos++;} return TPercent; }
        else if (t[uk]==';')
        { {l[i++]=t[uk++]; pos++;} return TSemicolon; }
        else if (t[uk]=='(')
        { {l[i++]=t[uk++]; pos++;} return TParent_O; }
        else if (t[uk]==')')
        { {l[i++]=t[uk++]; pos++;} return TParent_C; }
        else if (t[uk]=='{')
        { {l[i++]=t[uk++]; pos++;} return TBrace_O; }
        else if (t[uk]=='}')
        { {l[i++]=t[uk++]; pos++;} return TBrace_C; }
        else if (t[uk]==',')
        { {l[i++]=t[uk++]; pos++;} return TComma; }

        else if (t[uk]=='+') {
                l[i++]=t[uk++];
                pos++;
                if (t[uk]=='+') {
                        l[i++]=t[uk++];
                        pos++;
                        return TDPlus;
                }
                return TPlus;
        }

        else if (t[uk]=='-') {
                l[i++]=t[uk++];
                pos++;
                if (t[uk]=='-') {
                        l[i++]=t[uk++];
                        pos++;
                        return TDMinus;
                }
                return TMinus;
        }

        else if (t[uk]=='<') {
                l[i++]=t[uk++];
                pos++;
                if (t[uk]=='=') {
                        l[i++]=t[uk++];
                        pos++;
                        return TLess_EQ;
                }
                else if (t[uk]=='<') {
                        l[i++]=t[uk++];
                        pos++;
                        return TShift_L;
                }
                return TLess;
        }

        else if (t[uk]=='>') {
                l[i++]=t[uk++];
                pos++;
                if (t[uk]=='=') {
                        l[i++]=t[uk++];
                        pos++;
                        return TGreat_EQ;
                }
                else if (t[uk]=='>') {
                        l[i++]=t[uk++];
                        pos++;
                        return TShift_R;
                }
                return TGreat;
        }

        else if (t[uk]=='=') {
                l[i++]=t[uk++];
                pos++;
                if (t[uk]=='=') {
                        l[i++]=t[uk++];
                        pos++;
                        return TEQ;
                }
                return TAssign;
        }

        else if (t[uk]=='!') {
                l[i++]=t[uk++];
                pos++;
                if (t[uk]=='=') {
                        l[i++]=t[uk++];
                        pos++;
                        return TNot_EQ;
                }
                PrintError("�������� ������",l);
                return TErr;
        }
        else{
                l[i++]=t[uk++];
                pos++;
                PrintError("�������� ������",l); // ������
                return TErr;
        }




}

// ���� ����� � �������� �������
void TScaner::GetData(const char * FileName)
{
        setlocale(LC_ALL,"ru_RU.CP1251");
        char aa;
        string FilePath = "../scaner/";
        FilePath += FileName;
        FILE * in = fopen(FilePath.c_str(),"r");
        if (!in) {
                PrintError("����������� ������� ����",nullptr);
        }
        int i=0;
        while(!feof(in)) {
                fscanf(in,"%c",&aa);
                if (!feof(in)) t[i++]=aa;
                if (i>=MAX_TEXT-1) {
                        PrintError("������� ������� ������ ��������� ������",nullptr);
                }
        } t[i]='\0'; // ����������� ���� �\0� � ����� ������
        fclose(in);
}     // ����� GetData()

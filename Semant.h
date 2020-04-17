#ifndef __SEMAN
#define __SEMAN

#include "defs.h"
#define max(a,b) a<b? b : a


enum TypeObject {ObjVar, ObjFunc, ObjEmpty};
enum TDataType {TYPE_EMPTY, TYPE_UNKNOWN, TYPE_INTEGER, TYPE_UNSIGNED, TYPE_CHAR};


union TDataValue{
    int DataAsInt;
    unsigned DataAsUngigned;
};

struct Position{
    int uk;
    int line;
    int pos;
};


struct Node
{
    TypeLex Id;
    TypeObject TypeObj;
    TDataType DataType;
    TDataValue DataValue;
    Position PosInText;
    int Param;
};

class Tree
{
    Node * n;
    Tree * Up, * Left, * Right;

public:
    static Tree * Root;
    static Tree * Cur;

    static bool flagInterprent;
    static bool flagReturn;


    Tree(Tree * l,Tree * r,Tree * u,Node * Data);
    Tree(Node * Data);
    Tree();
    ~Tree();
    void SetLeft (Node * Data);
    void SetRight (Node * Data);
    void SetLeft ();
    void SetRight ();
    void SetPosInText (int i,int j, int k);
    void GetPosInText (int *i,int *j, int *k);

    Tree * FindUp (Tree * From, TypeLex id);
    Tree * FindUpOneLevel (Tree * From, TypeLex id);
    Tree * FindUp (TypeLex id);
    Tree * FindRightLeft(Tree * From, TypeLex id);
    Tree * FindRightLeft(TypeLex id);
    Tree * GetUp(Tree *);
    void Print();
    void PrintError(const char *, char *);

    // ������������� ������������
    void SetCur(Tree * a) ;// ���������� ������� ���� ������
    Tree * GetCur(void); // �������� �������� �������� ���� ������
    Tree * SemIncludeVar(TypeLex a, TDataType t);
    Tree * SemIncludeFunc(TypeLex a, TDataType t);
    void SemSetType(Tree *Addr, TDataType t); // ���������� ��� t ��� ���������� �� ������ Addr
    void SemSetParam(Tree *Addr, int n);// ���������� ����� ���������� ���������� n ��� ������� �� ������ Addr
    void SemControlParam(Tree *Addr, int n);// ��������� ��������� ����� ���������� ���������� n ��� ������� �� ������ Addr;
    Tree * SemGetType(TypeLex a);// ����� � ������� ���������� � ������ a � ������� ������ �� ��������������� ������� ������
    Tree * SemGetFunct(TypeLex a);// ����� � ������� ������� � ������ a � ������� ������ �� ��������������� ������� ������
    int DupControl(Tree *Addr, TypeLex a); // �������� �������������� � �� ��������� �������� ������ �����
    bool CheckComp(TDataType toType, TDataType fromType);
    bool CheckUnoOp(TDataType type);
    TDataType SemGetDataType(Tree *Addr);
    TDataType SemGetFuncReturnType(Tree *Addr);
    //////////////////////////////////
    void DeleteAfterBlock(Tree* addr);
    void FreeAfterBlock(Tree* addr);
    int GetIntValue(Tree* addr);
    unsigned GetUnsignedValue(Tree* addr);
    TDataValue * GetDataValue(Tree* addr);
    void SetVarValue(Tree* addr,TDataValue value);
    Tree* SemCopyFunc(Tree* addr);
    void SemDelExeFunc(Tree* addr);
    void SemSetExeFuncParam(Tree* addr, TDataValue Value, int pos);
    Tree * SemGetBlockStart(Tree* addr);
    void SemFuncReturnValue(TDataValue);


};
#endif

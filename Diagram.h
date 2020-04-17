#ifndef DIAGRAM_H
#define DIAGRAM_H
#include "Scaner.h"
#include "Semant.h"
class TDiagram {
private:
    TScaner *sc;
    Tree * SemTree;
public:
    TDiagram(TScaner * s, Tree * r);
    ~TDiagram(){}
    void Program(); // ���������
    void Func();    // �������
    void Decl();    // �������� ������
    void Block();   // ����
    void Operator();// ��������
    void Assign();  // ������������
    void While();   // While
    void Call(TDataValue*);    // ����� �������
    void Return();  // Return
    void A1(TDataType*, TDataValue*);
    void A2(TDataType*, TDataValue*);
    void A3(TDataType*, TDataValue*);
    void A4(TDataType*, TDataValue*);
    void A5(TDataType*, TDataValue*);
    void A6(TDataType*, TDataValue*);
    void A7(TDataType*, TDataValue*);

    void logAssing(char *,TDataType,TDataValue);
    void logChange(char *,const char*,TDataType,TDataValue);
 };

#endif // DIAGRAM_H

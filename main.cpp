#include "defs.h"
#include "Diagram.h"
#include <windows.h>

int main(int argc, const char * argv[])
{
        setlocale(LC_ALL, "Russian");
        TScaner * sc;

        if (argc<=1) sc = new TScaner("input2.txt"); // ���� �� ���������
        else sc = new TScaner(argv[1]); // �������� ����
        Tree * root = new Tree;
        TDiagram* dg = new TDiagram(sc,root);
        dg->Program();

        int type; TypeLex l;// ��������� ���������� ���� � ����������� �������
        type=sc->Scaner(l);
        if (type == TEnd)
            printf("�������������� ������ �� ����������\n");
        else
            sc->PrintError("������ ����� � ����� ���������",nullptr);
        return 0;
}

#include "defs.h"
#include "Diagram.h"
#include <windows.h>

int main(int argc, const char * argv[])
{
        setlocale(LC_ALL, "Russian");
        TScaner * sc;

        if (argc<=1) sc = new TScaner("input2.txt"); // файл по умолчанию
        else sc = new TScaner(argv[1]); // заданный файл
        Tree * root = new Tree;
        TDiagram* dg = new TDiagram(sc,root);
        dg->Program();

        int type; TypeLex l;// локальные переменные типа и изображения лексемы
        type=sc->Scaner(l);
        if (type == TEnd)
            printf("Синтаксических ошибок не обнаружено\n");
        else
            sc->PrintError("Лишний текст в конце программы",nullptr);
        return 0;
}

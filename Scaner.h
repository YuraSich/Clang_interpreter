#ifndef SCANER_H
#include "defs.h"

class TScaner{
    private:
        TypeMod t;  // исходный текст
        int uk;     // указатель текущей позиции в исходном тексте
        int line;   // текущая строка
        int pos;    // текущая позиция
    public:
        void PutUK (int,int,int);
        void GetUK (int*,int*,int*);
        [[ noreturn ]] void PrintError(const char *, char *);
        [[ noreturn ]] void PrintError();
        int Scaner (TypeLex l);
        void GetData(const char *);
        TScaner(const char *);
        ~TScaner() {}
};

#endif // SCANER_H

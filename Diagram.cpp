#include "defs.h"
#include "Diagram.h"
#include "Semant.h"
#include <stdio.h>

TDiagram::TDiagram(TScaner * s, Tree* r){
    sc=s;
    SemTree = r;
    }

void TDiagram::Program(){
    TypeLex l;
    int t, uk1, line1, pos1;
    Tree::Cur = SemTree;
    Tree::Root = SemTree;
    Tree::flagInterprent = true;
    Tree::flagReturn = false;

    while (true){
        sc->GetUK(&uk1, &line1, &pos1);
        t = sc->Scaner(l);
        if (t == TEnd){
            return;
        }
        if (t != TInt && t!= TUnsigned)
            sc->PrintError("Ожидался тип данных (int или unsigned int)", l);
        if (t == TUnsigned){
            t = sc->Scaner(l);
            if (t != TInt)
                sc->PrintError("Ожидался тип данных (unsigned int)",l);
        }
        t = sc->Scaner(l);
        if (t!=TIdent && t != TMain)
           sc->PrintError("Ожидался идентификатор",l);

        t = sc->Scaner(l);
        if (t == TParent_O){
           sc->PutUK(uk1, line1, pos1);
           Func();

        }
        else if (t == TAssign || t == TSemicolon || t == TComma) {
           sc->PutUK(uk1, line1, pos1);
           Decl();
        }
        else
            sc->PrintError("Ожидалось одно из (\'=\' \';\' \',\' \'(\')",l);
    }
}

void TDiagram::Func(){
    TypeLex l;
    int t, i, e;
    TDataType type;
    int uk,pos,line;


    t = sc->Scaner(l);
    type = TYPE_INTEGER;
    if (t == TUnsigned){
        type = TYPE_UNSIGNED;
        t = sc->Scaner(l);
    }
    t = sc->Scaner(l); // Ident || main
    e = t;
    Tree * v = SemTree->SemIncludeFunc(l, type);
    t = sc->Scaner(l); // (
    t = sc->Scaner(l); // Ident || )
    i = 0;
    while (t != TParent_C){

        if (t != TInt && t!= TUnsigned)
            sc->PrintError("Ожидался тип данных (int или unsigned int)", l);
        type = TYPE_INTEGER;
        if (t == TUnsigned){
            t = sc->Scaner(l);
            type = TYPE_UNSIGNED;
            if (t != TInt)
                sc->PrintError("Ожидался тип данных (unsigned int)",l);
        }
        t = sc->Scaner(l);
        SemTree->SemIncludeVar(l, type);
        i++;
        t = sc->Scaner(l);
        if (t != TComma && t != TParent_C)
            sc->PrintError("Ожидался ) или ,",l);
        if (t == TComma)
            t = sc->Scaner(l);
    }
    SemTree->SemSetParam(v,i);
    sc->GetUK(&uk,&pos,&line);
    v->SetPosInText(uk,pos,line);
    if (e == TMain){
        Block();
    }
    else{
        Tree::flagInterprent = false;
        Block();
        Tree::flagInterprent = true;
    }

    SemTree->SetCur(v);
    return;
}

void TDiagram::Decl(){
    TypeLex l, name;
    int t;
    TDataType type;
    t = sc->Scaner(l);
    type = TYPE_INTEGER;
    if (t == TUnsigned){
        type = TYPE_UNSIGNED;
        t = sc->Scaner(l);
    }


    do{
        t = sc->Scaner(l);
        memcpy(name,l,sizeof (l));
        if (t != TIdent)
            sc->PrintError("Ожидался идентификатор",l);
        Tree* v = SemTree->SemIncludeVar(l,type);
        t = sc->Scaner(l);

        if (t == TAssign){
            TDataType type_expr;
            TDataValue dataValue;
            A1(&type_expr,&dataValue);

            //
            SemTree->SetVarValue(v,dataValue);
            logAssing(name,type,dataValue);

            //
            t = sc->Scaner(l);
        }

    }while (t == TComma);
    if (t != TSemicolon)
        sc->PrintError("Ожидалось ; ",l);

}

void TDiagram::Block(){
    TypeLex l;
    int t, uk1, line1, pos1;
    t = sc->Scaner(l);
    SemTree->SetLeft();
    Tree * v = SemTree->GetCur();
    SemTree->SetRight();

    if (t != TBrace_O)
        sc->PrintError("Ожидалось { ",l);
    sc->GetUK(&uk1, &line1, &pos1);
    t = sc->Scaner(l);
    if(t == TBrace_C)
        return;
    while (true){
        sc->PutUK(uk1, line1, pos1);
        t = sc->Scaner(l);
        if (t == TInt || t == TUnsigned){
            if( t == TUnsigned)
                t = sc->Scaner(l);
            sc->PutUK(uk1, line1, pos1);
            Decl();
        }
        else{
            sc->PutUK(uk1, line1, pos1);
            Operator();
        }
        sc->GetUK(&uk1, &line1, &pos1);
        t = sc->Scaner(l);
        if(t == TBrace_C){
            Tree * newCur = SemTree->GetUp(SemTree->GetUp(v));
            SemTree->FreeAfterBlock(newCur);
            SemTree->SetCur(newCur);
            return;
        }
        if(t == TEnd)
            break;
    }
    sc->PrintError("Ожидалось } ",l);

}

void TDiagram::Operator(){
    TypeLex l;
    int t, uk1, line1, pos1;
    TDataType type_expr;
    TDataValue* dataValue;
    sc->GetUK(&uk1, &line1, &pos1);
    t = sc->Scaner(l);

    if (t == TReturn){
        sc->PutUK(uk1, line1, pos1);
        Return();
    }

    else if (t == TBrace_O){
        sc->PutUK(uk1, line1, pos1);
        Block();
    }

    else if (t == TWhile){
        sc->PutUK(uk1, line1, pos1);
        While();
    }

    else if (t == TIdent){
        Tree* v = SemTree->FindUp(l);
        if (v==nullptr && Tree::flagInterprent){
            t = sc->Scaner(l);
            if (t == TParent_O)
                SemTree->PrintError("Необъявленная функция",nullptr);
            else
                SemTree->PrintError("Необъявленная переменная",nullptr);
            sc->PrintError();}
        dataValue = SemTree->GetDataValue(v);
        t = sc->Scaner(l);
        if (t == TAssign){
            sc->PutUK(uk1, line1, pos1);
            Assign();
        }
        else{
            sc->PutUK(uk1, line1, pos1);
            A5(&type_expr,dataValue);
            t = sc->Scaner(l);
            if (t != TSemicolon)
                sc->PrintError("Ожидалось ; ",l);
        }

    }

    else if (t == TSemicolon){
        sc->PutUK(uk1, line1, pos1);
        t = sc->Scaner(l);
    }

    else{
        dataValue = new TDataValue();
        sc->PutUK(uk1, line1, pos1);
        A5(&type_expr,dataValue);
        t = sc->Scaner(l);
        if (t != TSemicolon)
            sc->PrintError("Ожидалось ; ",l);
    }

}

void TDiagram::Return(){
    TypeLex l;
    int t;
    t = sc->Scaner(l); // return

    TDataType type_expr;
    TDataValue dataValue;
    A1(&type_expr,&dataValue);
    SemTree->SemFuncReturnValue(dataValue);
    t = sc->Scaner(l);
    if (t != TSemicolon)
        sc->PrintError("Ожидалось ;",l);
    if(Tree::flagInterprent == true)
        Tree::flagReturn = true;
    Tree::flagInterprent = false;
    return;
}

void TDiagram::While(){
    TypeLex l;
    int t;

    bool localFlInt = Tree::flagInterprent;
    t = sc->Scaner(l); // while
    int i,j,k;
    sc->GetUK(&i,&j,&k);
    Start:
    t = sc->Scaner(l);
    if (t != TParent_O)
        sc->PrintError("Ожидалось (",l);
    TDataType type_expr;
    TDataValue dataValue;
    A1(&type_expr,&dataValue);
    if(Tree::flagInterprent && (dataValue.DataAsInt !=0))
        Tree::flagInterprent = true;
    else
        Tree::flagInterprent = false;
    t = sc->Scaner(l);
    if (t != TParent_C)
        sc->PrintError("Ожидалось )",l);
    Operator();
    if(Tree::flagInterprent){
        sc->PutUK(i,j,k);
        goto Start;
    }
    Tree::flagInterprent = localFlInt;
    if (Tree::flagReturn)
        Tree::flagInterprent = false;
    return;
}


void TDiagram::Assign(){
    TypeLex l, name;
    int t;
    Tree *v;
    TDataType VType, EType;
    TDataValue* value = new TDataValue();

    t = sc->Scaner(l); // ident
    memcpy(name,l,sizeof (l));
    v=SemTree->SemGetType(l);
    VType = SemTree->SemGetDataType(v);
    t = sc->Scaner(l); // =
    A1(&EType,value);
    SemTree -> CheckComp(VType,EType);
    //
    //
    SemTree->SetVarValue(v,*value);
    logAssing(name,VType,*value);
    //
    //
    t = sc->Scaner(l);
    if (t != TSemicolon)
        sc->PrintError("Ожидалось ;",l);
    return;
}


void TDiagram::Call(TDataValue* dataValue){
    TypeLex l;
    int t, uk1, line1, pos1, uk2, line2, pos2;
    int num;

    t = sc->Scaner(l); // ident
    Tree * v = SemTree->SemGetFunct(l);
    t = sc->Scaner(l); // (
    num = 0;
    sc->GetUK(&uk1, &line1, &pos1);
    t = sc->Scaner(l);
    if (t != TParent_C)
        sc->PutUK(uk1, line1, pos1);
    Tree * exeFunc = SemTree->SemCopyFunc(v);
    while(t != TParent_C){
        num++;
        TDataType type_expr;
        TDataValue dataValue;
        A1(&type_expr,&dataValue);
        SemTree->SemSetExeFuncParam(exeFunc,dataValue,num);
        t = sc->Scaner(l);
        if (t != TComma && t != TParent_C)
            sc->PrintError("Ожидалось , или )",l);
    }
    SemTree->SemControlParam(v,num);
    Tree * cur = nullptr, * blockStart = nullptr;
    sc->GetUK(&uk2, &line2, &pos2);
    if (SemTree->flagReturn == false){
        if (SemTree->flagInterprent){
            sc->GetUK(&uk2, &line2, &pos2);
            exeFunc->GetPosInText(&uk1, &line1, &pos1);
            sc->PutUK(uk1,pos1,line1);
            cur = SemTree->GetCur();
            blockStart = SemTree->SemGetBlockStart(exeFunc);
            SemTree->SetCur(blockStart);
            Block();
        }

        if (SemTree->flagInterprent == false  &&  SemTree->flagReturn == true){
            SemTree->flagInterprent = true;
            SemTree->flagReturn = false;

            dataValue->DataAsInt = SemTree->GetDataValue(exeFunc)->DataAsInt;
            SemTree->SetCur(cur);
            sc->PutUK(uk2, line2, pos2);
            SemTree->SemDelExeFunc(exeFunc);
        }
    }
    return;
}



void TDiagram::A1(TDataType * dataType, TDataValue* dataValue){
    TDataType VType, EType;
    TDataValue* newValue = new TDataValue();
    A2(&VType,dataValue);

    TypeLex l;
    int operation, uk1, line1, pos1;
    sc->GetUK(&uk1, &line1, &pos1);
    operation = sc->Scaner(l);

    if ((operation != TGreat && operation != TLess && operation != TGreat_EQ &&operation != TLess_EQ && operation != TEQ && operation != TNot_EQ)){
        sc->PutUK(uk1,line1,pos1);
        *dataType = VType;
        return;
    }

    while(true){
        A2(&EType,newValue);
        if (Tree::flagInterprent){
            switch (VType) {
            case TYPE_INTEGER:
                switch (EType) {
                case TYPE_INTEGER:
                    switch (operation) {
                        case TGreat:     {
                           dataValue->DataAsInt = dataValue->DataAsInt > newValue->DataAsInt;
                           logChange(nullptr,"Сравнение \'больше\' (>)", VType, *dataValue);
                           break;}
                        case TLess:    {
                           dataValue->DataAsInt = dataValue->DataAsInt < newValue->DataAsInt;
                           logChange(nullptr,"Сравнение \'меньше\' (<)", VType, *dataValue);
                           break;}
                        case TGreat_EQ:     {
                           dataValue->DataAsInt = dataValue->DataAsInt >= newValue->DataAsInt;
                           logChange(nullptr,"Сравнение \'больше или равно\' (>=)", VType, *dataValue);
                           break;}
                        case TLess_EQ:    {
                           dataValue->DataAsInt = dataValue->DataAsInt <= newValue->DataAsInt;
                           logChange(nullptr,"Сравнение \'меньше или равно\' (<=)", VType, *dataValue);
                           break;}
                        case TEQ:     {
                           dataValue->DataAsInt = dataValue->DataAsInt == newValue->DataAsInt;
                           logChange(nullptr,"Сравнение \'равно\' (==)", VType, *dataValue);
                           break;}
                        case TNot_EQ:    {
                           dataValue->DataAsInt = dataValue->DataAsInt != newValue->DataAsInt;
                           logChange(nullptr,"Сравнение \'не равно\' (!=)", VType, *dataValue);
                           break;}
                    }
                    break;
                case TYPE_UNSIGNED:
                    switch (operation) {
                        case TGreat:     {
                           dataValue->DataAsInt = dataValue->DataAsInt > newValue->DataAsUngigned;
                           logChange(nullptr,"Сравнение \'больше\' (>)", VType, *dataValue);
                           break;}
                        case TLess:    {
                           dataValue->DataAsInt = dataValue->DataAsInt < newValue->DataAsUngigned;
                           logChange(nullptr,"Сравнение \'меньше\' (<)", VType, *dataValue);
                           break;}
                        case TGreat_EQ:     {
                           dataValue->DataAsInt = dataValue->DataAsInt >= newValue->DataAsUngigned;
                           logChange(nullptr,"Сравнение \'больше или равно\' (>=)", VType, *dataValue);
                           break;}
                        case TLess_EQ:    {
                           dataValue->DataAsInt = dataValue->DataAsInt <= newValue->DataAsUngigned;
                           logChange(nullptr,"Сравнение \'меньше или равно\' (<=)", VType, *dataValue);
                           break;}
                        case TEQ:     {
                           dataValue->DataAsInt = dataValue->DataAsInt == newValue->DataAsUngigned;
                           logChange(nullptr,"Сравнение \'равно\' (==)", VType, *dataValue);
                           break;}
                        case TNot_EQ:    {
                           dataValue->DataAsInt = dataValue->DataAsInt != newValue->DataAsUngigned;
                           logChange(nullptr,"Сравнение \'не равно\' (!=)", VType, *dataValue);
                           break;}
                    }
                    break;
                default:break;
                }
                break;
            case TYPE_UNSIGNED:
                switch (EType) {
                case TYPE_INTEGER:
                    switch (operation) {
                        case TGreat:     {
                           dataValue->DataAsInt = dataValue->DataAsUngigned > newValue->DataAsInt;
                           logChange(nullptr,"Сравнение \'больше\' (>)", VType, *dataValue);
                           break;}
                        case TLess:    {
                           dataValue->DataAsInt = dataValue->DataAsUngigned < newValue->DataAsInt;
                           logChange(nullptr,"Сравнение \'меньше\' (<)", VType, *dataValue);
                           break;}
                        case TGreat_EQ:     {
                           dataValue->DataAsInt = dataValue->DataAsUngigned >= newValue->DataAsInt;
                           logChange(nullptr,"Сравнение \'больше или равно\' (>=)", VType, *dataValue);
                           break;}
                        case TLess_EQ:    {
                           dataValue->DataAsInt = dataValue->DataAsUngigned <= newValue->DataAsInt;
                           logChange(nullptr,"Сравнение \'меньше или равно\' (<=)", VType, *dataValue);
                           break;}
                        case TEQ:     {
                           dataValue->DataAsInt = dataValue->DataAsUngigned == newValue->DataAsInt;
                           logChange(nullptr,"Сравнение \'равно\' (==)", VType, *dataValue);
                           break;}
                        case TNot_EQ:    {
                           dataValue->DataAsInt = dataValue->DataAsUngigned != newValue->DataAsInt;
                           logChange(nullptr,"Сравнение \'не равно\' (!=)", VType, *dataValue);
                           break;}
                    }
                    break;
                case TYPE_UNSIGNED:
                    switch (operation) {
                        case TGreat:     {
                           dataValue->DataAsInt = dataValue->DataAsUngigned > newValue->DataAsUngigned;
                           logChange(nullptr,"Сравнение \'больше\' (>)", VType, *dataValue);
                           break;}
                        case TLess:    {
                           dataValue->DataAsInt = dataValue->DataAsUngigned < newValue->DataAsUngigned;
                           logChange(nullptr,"Сравнение \'меньше\' (<)", VType, *dataValue);
                           break;}
                        case TGreat_EQ:     {
                           dataValue->DataAsInt = dataValue->DataAsUngigned >= newValue->DataAsUngigned;
                           logChange(nullptr,"Сравнение \'больше или равно\' (>=)", VType, *dataValue);
                           break;}
                        case TLess_EQ:    {
                           dataValue->DataAsInt = dataValue->DataAsUngigned <= newValue->DataAsUngigned;
                           logChange(nullptr,"Сравнение \'меньше или равно\' (<=)", VType, *dataValue);
                           break;}
                        case TEQ:     {
                           dataValue->DataAsInt = dataValue->DataAsUngigned == newValue->DataAsUngigned;
                           logChange(nullptr,"Сравнение \'равно\' (==)", VType, *dataValue);
                           break;}
                        case TNot_EQ:    {
                           dataValue->DataAsInt = dataValue->DataAsUngigned != newValue->DataAsUngigned;
                           logChange(nullptr,"Сравнение \'не равно\' (!=)", VType, *dataValue);
                           break;}
                    }
                    break;
                default:break;
                }
                break;
            default:break;
            }

        }

        sc->GetUK(&uk1, &line1, &pos1);
        operation = sc->Scaner(l);
        if ((operation != TGreat && operation != TLess && operation != TGreat_EQ && operation != TLess_EQ && operation != TEQ && operation != TNot_EQ)){
            sc->PutUK(uk1,line1,pos1);
            SemTree ->CheckComp(VType,EType);
            *dataType = VType;
            return;
        }
    }
}

void TDiagram::A2(TDataType * dataType, TDataValue* dataValue){
    TDataType VType, EType;
    TDataValue* newValue = new TDataValue();
    A3(&VType,dataValue);

    TypeLex l;
    int t, uk1, line1, pos1;
    sc->GetUK(&uk1, &line1, &pos1);
    t = sc->Scaner(l);
    if (t != TShift_L && t != TShift_R){
        sc->PutUK(uk1,line1,pos1);
        *dataType = VType;
        return;
    }

    while(true){
        A3(&EType,newValue);
        if (Tree::flagInterprent){
            switch (t) {
            case TShift_L:     {
               dataValue->DataAsInt = dataValue->DataAsInt << newValue->DataAsInt;
               logChange(nullptr,"Сдвиг влево (<<)", VType, *dataValue);
               break;}
            case TShift_R:    {
               dataValue->DataAsInt = dataValue->DataAsInt >> newValue->DataAsInt;
               logChange(nullptr,"Сдвиг вправо (>>))", VType, *dataValue);
               break;}
            }
        }
        sc->GetUK(&uk1, &line1, &pos1);
        t = sc->Scaner(l);
        if (t != TShift_L && t != TShift_R){
            sc->PutUK(uk1,line1,pos1);
            SemTree ->CheckComp(VType,EType);
            *dataType = VType;
            return;
        }
    }
}


void TDiagram::A3(TDataType * dataType, TDataValue* dataValue){
    TDataType VType, EType;
    TDataValue* newValue = new TDataValue();
    A4(&VType,dataValue);


    TypeLex l;
    int t, uk1, line1, pos1;
    sc->GetUK(&uk1, &line1, &pos1);
    t = sc->Scaner(l);

    if (t != TPlus && t != TMinus){
        sc->PutUK(uk1,line1,pos1);
        *dataType = VType;
        return;
    }

    while(true){
        A4(&EType,newValue);
        if (Tree::flagInterprent){
            switch (t) {
            case TPlus:     {
                dataValue->DataAsInt = dataValue->DataAsInt + newValue->DataAsInt;
                logChange(nullptr,"Сложение (+)", VType, *dataValue);
                break;}
            case TMinus:    {
                dataValue->DataAsInt = dataValue->DataAsInt - newValue->DataAsInt;
                logChange(nullptr,"Вычитание (-))", VType, *dataValue);
                break;}
            }
        }
        sc->GetUK(&uk1, &line1, &pos1);
        t = sc->Scaner(l);
        if (t != TPlus && t != TMinus){
            sc->PutUK(uk1,line1,pos1);
            SemTree ->CheckComp(VType,EType);
            *dataType = VType;
            return;
        }
    }
}


void TDiagram::A4(TDataType * dataType, TDataValue* dataValue){
    TDataType VType, EType;
    TDataValue* newValue = new TDataValue();
    A5(&VType,dataValue);

    TypeLex l;
    int t, uk1, line1, pos1;
    sc->GetUK(&uk1, &line1, &pos1);
    t = sc->Scaner(l);

    if (t != TStar && t != TRSlash && t != TPercent){
        sc->PutUK(uk1,line1,pos1);
        *dataType = VType;
        return;
    }
    while(true){
        A5(&EType,newValue);
        if (Tree::flagInterprent){
            switch (t) {
            case TStar:     {
                dataValue->DataAsInt = dataValue->DataAsInt * newValue->DataAsInt;
                logChange(nullptr,"Умножение (*)", VType, *dataValue);
                break;}
            case TRSlash:    {
                dataValue->DataAsInt = dataValue->DataAsInt / newValue->DataAsInt;
                logChange(nullptr,"Деление нацело (/))", VType, *dataValue);
                break;}
            case TPercent:    {
                dataValue->DataAsInt = dataValue->DataAsInt % newValue->DataAsInt;
                logChange(nullptr,"Остаток от деление (%))", VType, *dataValue);
                break;}
            }
        }
        sc->GetUK(&uk1, &line1, &pos1);
        t = sc->Scaner(l);
        if (t != TStar && t != TRSlash && t != TPercent){
            sc->PutUK(uk1,line1,pos1);
            SemTree ->CheckComp(VType,EType);
            *dataType = VType;
            return;
        }

    }
}


void TDiagram::A5(TDataType * dataType, TDataValue* dataValue){
    TypeLex l;
    int t, uk1, line1, pos1;
    TDataType VType;
    A6(&VType,dataValue);

    sc->GetUK(&uk1, &line1, &pos1);
    t = sc->Scaner(l);
    if (t == TDPlus || t == TDMinus){
        if(!(SemTree->CheckUnoOp(VType))&& Tree::flagInterprent){
            SemTree->PrintError("Унарные опрерации над типом переменной запещены",nullptr);
            sc->PrintError();
        }
        else {
            if (Tree::flagInterprent){
                switch (t) {
                case TDPlus:     {
                    dataValue->DataAsInt = dataValue->DataAsInt + 1;
                    logChange(nullptr,"Унарный двойной плюс(Постфикс)", VType, *dataValue);
                    break;}
                case TDMinus:    {
                    dataValue->DataAsInt = dataValue->DataAsInt - 1;
                    logChange(nullptr,"Унарный двойной минус(Постфикс)", VType, *dataValue);
                    break;}
                }
            }
        }
    }
    else{
        sc->PutUK(uk1,line1,pos1);
    }
    *dataType = VType;
}

void TDiagram::A6(TDataType * dataType, TDataValue* dataValue){
    TypeLex l;
    int t, uk1, line1, pos1;
    TDataType VType;
    bool ch = false;


    sc->GetUK(&uk1, &line1, &pos1);
    t = sc->Scaner(l);

    if (t == TPlus || t == TDPlus || t == TMinus || t == TDMinus){

        ch = true;
    }

    else{
        sc->PutUK(uk1,line1,pos1);
    }

    A7(&VType,dataValue);
    if (ch){
        if(!(SemTree->CheckUnoOp(VType))&& Tree::flagInterprent){
            SemTree->PrintError("Унарные опрерации над этим типом переменных запещены",nullptr);
            sc->PrintError();
        }
        if (Tree::flagInterprent){
            switch (t) {
            case TPlus:     {
                dataValue->DataAsInt = + dataValue->DataAsInt;
                logChange(nullptr,"Унарный плюс", VType, *dataValue);
                break;}
            case TDPlus:    {
                dataValue->DataAsInt = dataValue->DataAsInt + 1;
                logChange(nullptr,"Унарный двойной плюс(Префикс)", VType, *dataValue);
                break;}
            case TMinus:    {
                dataValue->DataAsInt = - dataValue->DataAsInt;
                logChange(nullptr,"Унарный минус", VType, *dataValue);
                break;}
            case TDMinus:   {
                dataValue->DataAsInt = dataValue->DataAsInt - 1;
                logChange(nullptr,"Унарный двойной минус(Префикс)", *dataType, *dataValue);
                break;}
            }
        }
    }
    *dataType = VType;
}


void TDiagram::A7(TDataType * dataType, TDataValue* dataValue){
    TypeLex l;
    int t, uk1, line1, pos1;
    TDataType VType;
    sc->GetUK(&uk1, &line1, &pos1);
    t = sc->Scaner(l);


    if (t == TConstInt_10 || t == TConstInt_8 || t == TConstInt_16 || t == TConstChar){
        if (t == TConstChar){
            *dataType = TYPE_CHAR;
            char a = l[0];
            dataValue->DataAsInt = static_cast<int>(a);
        }
        else{
            *dataType = TYPE_INTEGER;
            if (t == TConstInt_10)
                dataValue->DataAsInt = static_cast<int>( strtoul(l,nullptr,10));
            if (t == TConstInt_8)
                dataValue->DataAsInt = static_cast<int>( strtoul(l,nullptr,8));
            if (t == TConstInt_16)
                dataValue->DataAsInt = static_cast<int>( strtoul(l,nullptr,16));
        }
    }

    else if(t == TParent_O){
        A1(&VType,dataValue);
        t = sc->Scaner(l);
        if (t != TParent_C)
            sc->PrintError("Ожидалось )",l);
        *dataType = VType;
    }

    else if (t == TIdent) {
        Tree * v = SemTree->Cur->FindUp(l);
        if (v==nullptr && Tree::flagInterprent){
            t = sc->Scaner(l);
            if (t == TParent_O)
                SemTree->PrintError("Необъявленная функция",nullptr);
            else
                SemTree->PrintError("Необъявленная переменная",nullptr);
            sc->PrintError();}
        t = sc->Scaner(l);
        if (t == TParent_O){
            sc->PutUK(uk1,line1,pos1);
            Call(dataValue);
            VType = SemTree->SemGetFuncReturnType(v);
            *dataType = VType;
        }
        else{
            sc->PutUK(uk1,line1,pos1);
            t = sc->Scaner(l);
            VType = SemTree->SemGetDataType(v);
            *dataType = VType;
            if(VType == TYPE_INTEGER)
                dataValue->DataAsInt = SemTree->GetIntValue(v);
            if(VType == TYPE_UNSIGNED)
                dataValue->DataAsUngigned = SemTree->GetUnsignedValue(v);
        }
    }
    else{
        sc->PrintError("Ожидалось одно из (Идентификатор, Константа, Вызов функции, Выражение в скобках)",l);
    }
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void TDiagram::logAssing(char * id, TDataType type, TDataValue value){
    if(!Tree::flagInterprent) return;
    switch (type) {
    case TYPE_INTEGER:  {printf("Переменной \'%s\' \tтипа int          \tприсвоено значение %d \n",id,value.DataAsInt); break;}
    case TYPE_UNSIGNED: {printf("Переменной \'%s\' \tтипа unsigned int \tприсвоено значение %u \n",id,value.DataAsUngigned); break;}
    default:
        break;
    }
}

void TDiagram::logChange(char * id,const char * op,TDataType type, TDataValue value){
    if(!Tree::flagInterprent) return;
    if(id)
        printf("Переменной \'%s\'",id);
    else
        printf("При вычислении");
    switch (type) {
    case TYPE_INTEGER:  {printf("\tтипа int          \tпри выполнении %s \t получено значение %d \n",op,value.DataAsInt); break;}
    case TYPE_UNSIGNED: {printf("\tтипа unsigned int \tпри выполнении %s \t получено значение %u \n",op,value.DataAsUngigned); break;}
    default:
        break;
    }
}





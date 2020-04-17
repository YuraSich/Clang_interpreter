// модуль Semant.cpp -- реализация семантических подпрограмм
#include <string.h>
#include "defs.h"
#include "Scaner.h"
#include "Semant.h"

Tree* Tree::Cur= static_cast<Tree*>(nullptr);
Tree* Tree::Root= static_cast<Tree*>(nullptr);
bool Tree::flagInterprent = true;
bool Tree::flagReturn = false;

Tree::~Tree(){
}


void Tree::PrintError(const char *err, char *a){
    if(!Tree::flagInterprent) return;
    // выдать сообщение об ошибке
    if (a==nullptr)
            printf("Синтаксическая ошибка : %s \n",err);
    else
            printf("Синтаксическая ошибка : %s. Неверный символ \'%s\' \n",err,a);
}

Tree::Tree (Tree * l, Tree * r, Tree * u, Node * d)
// конструктор создает узел с заданными связями и данными
{
    if(!Tree::flagInterprent) return;
        n= new Node();
        Up=u; Left=l; Right=r;
        // установили ссылки
        if (d != nullptr)
             memcpy(n, d, sizeof(Node));
        else {
             memcpy(n,&"-fake-", sizeof(Node));
        }
        // установили данные
}


Tree::Tree (Node * d)
// конструктор создает узел с заданными связями и данными
{
    if(!Tree::flagInterprent) return;
        n= new Node();
        Up=this; Left=Right=nullptr;
        // установили ссылки
        memcpy(n, d, sizeof(Node));
        // установили данные
}

Tree::Tree (void)
// конструктор создает новый узел с пустыми связями и данными
{
    if(!Tree::flagInterprent) return;
        n= new Node();
        Up=nullptr; Left=nullptr; Right=nullptr;
        n->Param = 0;
        n->TypeObj = ObjEmpty;
        n->DataType = TYPE_EMPTY;
        n->DataValue.DataAsInt = 0;
        memcpy(n->Id,&("-ROOT-"), sizeof(Node));

}

void Tree::SetLeft (Node * Data)
// создать левого потомка от текущей вершины this
{
    if(!Tree::flagInterprent) return;
        Tree * a= new Tree(nullptr,nullptr,this,Data); // новая вершина
        Cur->Left=a;
        // связали this с новой вершиной
}

void Tree::SetRight(Node * Data)
// создать правого потомка от текущей вершины this
{
    if(!Tree::flagInterprent) return;
        Tree * a= new Tree(nullptr,nullptr,this,Data); // новая вершина
        Cur->Right=a;
// связали this с новой вершиной
}

void Tree::SetLeft (){
    if(!Tree::flagInterprent) return;
        Tree * a= new Tree(nullptr,nullptr,this->Cur,nullptr); // новая вершина
        a->n->Param=0;
        a->n->TypeObj=ObjEmpty;
        a->n->DataType = this->n->DataType;
        a->n->DataValue.DataAsInt = 0;
        Cur->Left=a;
        Cur = Cur -> Left;
}

void Tree::SetRight(){
    if(!Tree::flagInterprent) return;
        Tree * a= new Tree(nullptr,nullptr,this->Cur,nullptr); // новая вершина
        a->n->Param=0;
        a->n->TypeObj=ObjEmpty;
        a->n->DataType = this->n->DataType;
        a->n->DataValue.DataAsInt = 0;
        Cur->Right=a;
        Cur = Cur -> Right;
}
Tree * Tree::FindUp(TypeLex id)
// поиск данных в дереве, начиная от текущей вершины this
// до его корня вверх по связям
{
    if(!Tree::flagInterprent) return nullptr;
        return FindUp(this->Cur, id);
}
Tree * Tree::FindUp(Tree * From, TypeLex id)
// поиск данных в дереве от заданной вершины From
// до его корня вверх по связям
{
    if(!Tree::flagInterprent) return nullptr;
        Tree * i=From;
// текущая вершина поиска
        while( (i!=nullptr) &&
               (memcmp(id, i->n->Id, max(strlen(i->n->Id),strlen(id)))!=0) )
                i=i->Up;
// поднимаемся наверх по связям
        return i;
}
Tree * Tree::FindRightLeft(TypeLex id)
// поиск прямых потомков текущей вершины this
{
    if(!Tree::flagInterprent) return nullptr;
        return FindRightLeft(this, id);
}
Tree * Tree::FindRightLeft(Tree * From, TypeLex id)
// поиск прямых потомков заданной вершины From
{
    if(!Tree::flagInterprent) return nullptr;
        Tree * i=From->Right;
// текущая вершина поиска
        while( (i!=nullptr) &&
               (memcmp(id, i->n->Id, max(strlen(i->n->Id),strlen(id)))!=0) )
                i=i->Left;
// обходим только соседей по левым связям
        return i;
}
void Tree::Print (void)
// отладочная программа печати дерева
{
    if(!Tree::flagInterprent) return;
        printf("Вершина с данными %s ----->", n->Id );
        if (Left !=nullptr) printf(" слева данные %s", Left->n->Id );
        if (Right!=nullptr) printf(" справа данные %s", Right->n->Id );
        printf("\n");
        if (Left!=nullptr) Left->Print();
        if (Right!=nullptr) Right->Print();
}



void Tree::SetCur(Tree * a){
    if(!Tree::flagInterprent) return;
// установить текущий узел дерева
        Cur=a;
}
Tree * Tree::GetCur(void){
    if(!Tree::flagInterprent) return nullptr;
// получить значение текущего узла дерева
        return Cur;
}


void Tree::SetPosInText(int i, int j, int k){
    if(!Tree::flagInterprent) return ;
    this->n->PosInText.uk= i;
    this->n->PosInText.pos= j;
    this->n->PosInText.line= k;
}

void Tree::GetPosInText(int *i, int *j, int *k){
    if(!Tree::flagInterprent) return ;
    *i = this->n->PosInText.uk;
    *j = this->n->PosInText.pos;
    *k = this->n->PosInText.line;
}


Tree * Tree::SemIncludeVar(TypeLex a, TDataType t){
    if(!Tree::flagInterprent) return nullptr;
// занесение идентификатора a в таблицу с типом t
        if (DupControl(Cur, a))
                PrintError("Повторное описание идентификатора ",a);
        Node b;
        memcpy(b.Id,a,strlen(a)+1);
        b.TypeObj = ObjVar;
        b.DataType=t;
        b.Param=0;
        // количество параметров функции
        Cur->SetLeft(&b);
        // сделали вершину - переменную
        Cur = Cur->Left;
        return Cur;
}
Tree * Tree::SemIncludeFunc(TypeLex a, TDataType r){
    if(!Tree::flagInterprent) return nullptr;
        if (DupControl(Cur, a))
                PrintError("Повторное описание идентификатора ",a);
        Tree * v;
        Node b;

        memcpy(b.Id,a,strlen(a)+1);
        b.TypeObj=ObjFunc;
        b.Param=0;
        b.DataType = r;
        Cur->SetLeft (&b);
        // сделали вершину - функцию
        Cur = Cur->Left;
        v=Cur;
        // это точка возврата после выхода из функции
        memcpy(&b.Id,&"-func_exit-",22);
        b.DataType=TYPE_EMPTY;
        b.Param=0;

        Cur->SetRight (&b);
        // сделали пустую вершину
        Cur = Cur->Right;
        return v;

}
void Tree::SemSetType(Tree* Addr, TDataType t){
    if(!Tree::flagInterprent) return ;
// установить тип t для переменной по адресу Addr
        Addr->n->DataType=t;
}
void Tree::SemSetParam(Tree* Addr, int num){
    if(!Tree::flagInterprent) return ;
// установить число формальных параметров n для функции по адресу Addr
        Addr->n->Param=num;
}
void Tree::SemControlParam(Tree *Addr, int num){
    if(!Tree::flagInterprent) return ;
// проверить равенство числа формальных параметров значению
// n для функции по адресу Addr
        if (num!=Addr->n->Param){
                PrintError("Неверное число параметров у функции ",Addr->n->Id);
                exit(1);
        }
}
Tree * Tree::SemGetType(TypeLex a){
    if(!Tree::flagInterprent) return nullptr;
// найти в таблице переменную с именем a
// и вернуть ссылку на соответствующий элемент дерева
        Tree * v=FindUp(Cur, a);
        if (v==nullptr)
                PrintError("Отсутствует описание идентификатора ",a);
        if (v->n->TypeObj==ObjFunc)
                PrintError("Неверное использование вызова функции ",a);
        return v;
}
Tree * Tree::SemGetFunct(TypeLex a){
    if(!Tree::flagInterprent) return nullptr;
        // найти в таблице функцию с именем a
        // и вернуть ссылку на соответствующий элемент дерева.

        Tree * v=FindUp(Cur, a);
        if (v==nullptr)
                PrintError("Отсутствует описание функции ",a);
        if (v->n->TypeObj!=ObjFunc)
                PrintError("Не является функцией идентификатор ",a);
        return v;
}
Tree * Tree::FindUpOneLevel(Tree * From, TypeLex id){
    if(!Tree::flagInterprent) return nullptr;
        Tree * i=From;
        // текущая вершина поиска
        while( (i!=nullptr) && (i->Up != nullptr )&&( i->Up->Right != i)) {
                if (memcmp(id, i->n->Id, max(strlen(i->n->Id),strlen(id)))==0)
                        return i; // нaшли совпадающий идентификатор
                i=i->Up;
                // поднимаемся наверх по связям
        }
        return nullptr;
}
int Tree::DupControl(Tree* Addr, TypeLex a){
    if(!Tree::flagInterprent) return 0;

        if (FindUpOneLevel(Addr, a)==nullptr) return 0;
        return 1;
}

bool Tree::CheckComp(TDataType toType, TDataType fromType){
    if(!Tree::flagInterprent) return false;
    switch (toType){
    case TYPE_CHAR:{
        switch (fromType){
            case TYPE_CHAR:
            case TYPE_INTEGER:
            case TYPE_UNSIGNED: {return true;}
            default:
                return false;
            }
        }
    case TYPE_INTEGER:
        {
        switch (fromType){
            case TYPE_CHAR:
            case TYPE_INTEGER:
            case TYPE_UNSIGNED: {return true;}
            default:
                return false;
            }
        }
    case TYPE_UNSIGNED:
        {
        switch (fromType){
            case TYPE_CHAR:
            case TYPE_INTEGER:
            case TYPE_UNSIGNED: {return true;}
            default:
                return false;
            }
        }
    default:
        return false;
    }
}

bool Tree::CheckUnoOp(TDataType type){
    if(!Tree::flagInterprent) return false;
    if(type == TYPE_INTEGER || type == TYPE_UNSIGNED)
        return true;
    return false;
}

TDataType Tree::SemGetDataType(Tree *Addr){
    if(!Tree::flagInterprent) return TYPE_EMPTY;
    return Addr->n->DataType;
}

TDataType Tree::SemGetFuncReturnType(Tree *Addr){
    if(!Tree::flagInterprent) return TYPE_EMPTY;
    return  Addr->n->DataType;
}
//////////////////////////////


Tree* Tree::GetUp(Tree * addr){
    if (addr)
        return addr->Up;
    else
        return nullptr;
}

//////////////////////////////

void Tree::FreeAfterBlock(Tree *addr){
    if(!Tree::flagInterprent) return ;
    if (addr->Left){
        Tree::DeleteAfterBlock(addr->Left);
        delete addr->Left;
    }
    addr->Left = nullptr;
}



void Tree::DeleteAfterBlock(Tree *addr){
    if(!Tree::flagInterprent) return ;
    if (addr->Right){
        DeleteAfterBlock(addr->Right);
        delete addr->Right;
        addr->Right = nullptr;
    }
    if (addr->Left){
        DeleteAfterBlock(addr->Left);
        delete addr->Left;
        addr->Left = nullptr;
    }
}

int Tree::GetIntValue(Tree* addr){
    if(!Tree::flagInterprent) return 0;
    return addr->n->DataValue.DataAsInt;
}


unsigned Tree::GetUnsignedValue(Tree* addr){
    if(!Tree::flagInterprent) return 0;
    return addr->n->DataValue.DataAsUngigned;
}

TDataValue* Tree::GetDataValue(Tree* addr){
    if(!Tree::flagInterprent) return nullptr;
    if(addr)
        return &addr->n->DataValue;
    return new TDataValue();
}

void Tree::SetVarValue(Tree *addr, TDataValue value){
    if(!Tree::flagInterprent) return;
    switch (addr->n->DataType) {
    case TYPE_INTEGER: { addr->n->DataValue.DataAsInt = value.DataAsInt; break;}
    case TYPE_UNSIGNED:{ addr->n->DataValue.DataAsUngigned = value.DataAsUngigned; break;}
    default:
        break;
    }
}

Tree* Tree::SemCopyFunc(Tree *addr){
    if(!Tree::flagInterprent) return nullptr;
    Node * b = new Node();

    memcpy(b->Id,addr->n->Id,strlen(addr->n->Id)+1);
    b->TypeObj=addr->n->TypeObj;
    b->Param=addr->n->Param;
    b->DataType = addr->n->DataType;
    b->PosInText = addr->n->PosInText;

    Tree* prot = Tree::FindUp(b->Id);
    Tree* exe = new Tree(prot->Left,nullptr,prot,b);
    if(prot->Left)
        prot->Left->Up = exe;
    prot->Left = exe;
    Tree* local_cur = exe;




    b = new Node();

    memcpy(b->Id,&"-func_exit-",22);
    b->TypeObj = ObjEmpty;
    b->DataType=TYPE_EMPTY;
    b->Param=0;

    local_cur->Right = new Tree(nullptr,nullptr,local_cur,b);
    local_cur = local_cur ->Right;

    Tree * local_proto_cur;
    local_proto_cur = addr->Right->Left;
    while(local_proto_cur){
        b = new Node();
        memcpy(b->Id,local_proto_cur->n->Id,strlen(local_proto_cur->n->Id)+1);
        b->TypeObj=local_proto_cur->n->TypeObj;
        b->Param=local_proto_cur->n->Param;
        b->DataType = local_proto_cur->n->DataType;

        local_cur->Left = new Tree(nullptr,nullptr,local_cur,b);
        local_cur = local_cur->Left;
        local_proto_cur = local_proto_cur->Left;
    }

    return exe;

}

void Tree::SemDelExeFunc(Tree *addr){
    if(!Tree::flagInterprent) return;
    Tree::DeleteAfterBlock(addr->Right);
    addr->Left->Up = addr->Up;
    delete addr;
    return;
}


void Tree::SemSetExeFuncParam(Tree *addr, TDataValue Value, int pos){
    if(!Tree::flagInterprent) return;
    Tree* local_cur = addr->Right->Left;
    while(--pos)
        local_cur = local_cur->Left;
    local_cur->n->DataValue = Value;
}

Tree* Tree::SemGetBlockStart(Tree * addr){
    if(!Tree::flagInterprent) return nullptr;
    Tree* local_cur = addr->Right->Left;
    while(local_cur->Left)
        local_cur = local_cur->Left;
    return  local_cur;
}


void Tree::SemFuncReturnValue(TDataValue value){
    if(!Tree::flagInterprent) return;
    Tree* i = Cur;
    while( (i!=nullptr) &&
           (memcmp(&"-func_exit-", i->n->Id, max(strlen(i->n->Id),strlen("-func_exit-")))!=0) )
            i=i->Up;
    if (!i)
        return;
    else
        i->Up->n->DataValue = value;

}



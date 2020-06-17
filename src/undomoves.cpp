#include "undomoves.h"

undoMoves::undoMoves()
{
    count=0;
    top=NULL;
}

QImage undoMoves::JustUndoIt()
{
    if(top==NULL) return *new QImage();
    QImage tmp;
    tmp.load("UndoBuffer" + QString::number(top->index) + ".souBackup","png");
    newn=top->prev;
    delete top;
    top=newn;
    count--;
    return tmp;
}

void undoMoves::newMove(QImage &src)
{
    newn = new stack();
    count++;
    newn->index=count;
    src.save("UndoBuffer" + QString::number(count) + ".souBackup","png",100);
    if(top)
    {
        top->next=newn;
        newn->prev=top;
    }
    top=newn;
}

stack::stack()
{
    prev=next=NULL;
}

bool undoMoves::isAvailable()
{
    if(top) return 1;
    return 0;
}

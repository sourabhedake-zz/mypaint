#ifndef UNDOMOVES_H
#define UNDOMOVES_H
#include<QImage>
class stack
{
public:
    int index;
    stack *prev, *next;
    stack();
};

class undoMoves
{
    stack *top, *newn;
    int count;
public:
    undoMoves();
    QImage JustUndoIt();
    void newMove(QImage &);
    bool isAvailable();
};

#endif // UNDOMOVES_H

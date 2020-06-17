#ifndef MYQUEUE_H
#define MYQUEUE_H
#include<mygraphics.h>

class Q: public Point
{
public:
    Q(int,int);
    Q *next;
    friend class Queue;
};

class MyQueue
{
public:
    int count;
    Q *start, *cur, *newn, *last;
    MyQueue() ;
    void ins(int, int);
    Point *del();
    bool isEmpty();
    void clearAll();
    int getCount();
};

#endif // MYQUEUE_H

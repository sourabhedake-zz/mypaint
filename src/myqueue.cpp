#include "myqueue.h"

int MyQueue::getCount()
{
    return count;
}

MyQueue::MyQueue()
{
    count=0;
    start=NULL;
}

void MyQueue::clearAll()
{
    while(start)
    {
        cur=start->next;
        delete cur;
        start=cur;
    }
    count=0;
}

Q::Q(int x, int y)
{
    this->x=x;
    this->y=y;
    this->next=NULL;
}


void MyQueue::ins(int x, int y)
{
    count++;
    newn = new Q(x,y);
    if(!start)   {start=newn; last =start;}
    else
        last->next=newn;
    last=newn;
}

bool MyQueue::isEmpty()
{
    if(start) return 0;
    return 1;
}

Point* MyQueue::del()
{
    Point *tmp = new Point;
    tmp->x=start->x;
    tmp->y=start->y;
    start=start->next;
    count--;
    return tmp;
}

PointOperations::PointOperations()
{
    count=0;
    start=NULL;
}

void PointOperations::insert(int x, int y)
{
    newn = new Point();
    newn->x=x;
    newn->y=y;
    newn->n=NULL;
    count++;
    if(!start) { start = newn;}
    else
    {
        last->n=newn;
    }
    last=newn;
}

int PointOperations::getCount()
{   return count;   }

Point PointOperations::getPoint(int index)
{
    cur=start;
    while(--index && cur)  cur=cur->n;
    return *cur;
}

void PointOperations::Erase()
{
    while(start)
    {
        cur=start;
        start=start->n;
        delete cur;
    }
    count=0;
}

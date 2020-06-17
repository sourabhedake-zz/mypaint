#ifndef MYGRAPHICS_H
#define MYGRAPHICS_H
#include<QImage>
#include<QVector>
#include<iostream>
using namespace std;
class Point
{
public:
    Point(QPoint);
    Point();
    int x, y;
    Point *n;
    friend bool isValid(int,int);
};

class PolyList
{
public:
    class poly{
    public:
        QVector <int> vx, vy;
        bool operator ==(poly);
    }Polygon;
    PolyList *next;
    void newVertex(int x, int y);
    friend class PolyOperations;
};

class DrawMethods
{
    int sign(int);
    friend bool isValid(int,int);
public:
    bool Threshold(QRgb, QRgb, int);
    void drawL(int method, int x1, int y1, int x2, int y2, QImage &img, QRgb rgb=0);
    void Bresenhnams(int cx, int cy, int r,QImage &img, QRgb, QImage *src = NULL);
};


class PolyOperations: public DrawMethods
{
public:
    int pasteX, pasteY, copyX, copyY;
    PolyList *start, *cur, *newn, *last;
    PolyOperations();
    void new_Polygon(Point *start);
    int searchPolygon(int, int);
    void getPolygon(int, QImage &img, int x, int y, QRgb rgb);
    void plot(PolyList *cur, QImage &img, QRgb);
    void clearAll();
};

class PointOperations
{
    int count;
public:
    Point *start, *newn, *cur, *last;
    PointOperations();
    int getCount();
    void insert(int,int);
    void Erase();
    Point getPoint(int);
};


class FillMethods: public DrawMethods
{
    bool isBoundary(int, int);
public:
    void floodfillPoly();
    void scanfillPoly(PointOperations Points, QImage &img);
};

bool isValid(int,int);

#endif // MYGRAPHICS_H

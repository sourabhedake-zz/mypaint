#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include<QLabel>
#include<mygraphics.h>
#include<QLineEdit>
#include<QTimer>
#include<QImage>
#include<QKeyEvent>
#include<QMouseEvent>
#include<myqueue.h>
#include<transformations.h>
#include<undomoves.h>
#include<tools.h>
#include<QFrame>
#include<mypalatte.h>
#define MAX_WIDTH   1000
#define MAX_HEIGHT  690
#define MAX 18
#define COL_MAX 28
#include<iostream>
using namespace std;
namespace Ui {  class MainWindow;}
class myFrame: public QFrame    {};
class myLabel: public QLabel
{
public:
    QString name;
    QImage *img;
    myLabel(int,int);
    void mousePressEvent(QMouseEvent *);
};

class Text: public QLineEdit
{
    float a1,a2,a3,a4;
public:
    void Analyse(QString);
    void keyPressEvent(QKeyEvent *);
};

class myPalatte;
class drawingSheet : public QLabel, public FillMethods
{
    Q_OBJECT
    int mode;
    QLabel l, lbl1;
    myFrame *frm, *frmStatus, *frmHeader, *frmPalatte;
    Text *text1;
    QLineEdit tempName;
    myLabel *lbl[MAX], *pal[COL_MAX];
    myLabel *lblOpen, *lblSave, *palFront;
    myPalatte *Palt;
    MyQueue que;
    QTimer *t1, *t2, *t3, *t4, *t5, *t6, *t7;
    PointOperations ScanPoints;
    PolyOperations Polygons;
    Transform trans;
    undoMoves Undo;
    bool enatem, alterNate;
    int ptSet, firstX,firstY, field[MAX_WIDTH][MAX_HEIGHT], r, g, b;
    int curTemp, isStraight, polyNum, ImageMagniFactor, MagniFactor, ValidXStart, ValidXEnd, ValidYStart , ValidYEnd ;
    Point p1, backup, backup2;
    FILE *f;
    QRgb  rgb;
    myTools *tools;
public:
    drawingSheet();
    void ImageMerge(QImage &target, QImage src, int x, int y);
    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void keyPressEvent(QKeyEvent *e);
    void keyReleaseEvent(QKeyEvent *e);
    void wheelEvent(QWheelEvent *e);
    friend class MainWindow;
    friend class opt;
    friend class DrawMethods;
    friend class pushB;
    friend class Text;
    friend class FillMethods;
    friend class myFrame;
    friend class myLabel;
    friend class myPalatte;
    friend bool isValid(int, int);

private slots:
    void update();
    void setToolPosition();
    void setStatusPosition();
    void setHeaderPosition();
    void setPalattePosition();
};

#endif // MAINWINDOW_H

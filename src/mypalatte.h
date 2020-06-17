#ifndef MYPALATTE_H
#define MYPALATTE_H
#include<QMouseEvent>
#include<QLabel>
#include<mainwindow.h>
class myPalatte : public QLabel
{
    QImage *img;
public:
    myPalatte();
    void mousePressEvent(QMouseEvent *);
};

#endif // MYPALATTE_H

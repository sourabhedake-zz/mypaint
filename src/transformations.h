#ifndef TRANSFORMATIONS_H
#define TRANSFORMATIONS_H
#include <mygraphics.h>
class Transform: public DrawMethods
{
public:
    inline QImage blackNwhite(QImage *img, int darkness = 0)
    {
        QImage tmp=*img;
        QRgb rgb; register unsigned int h=img->height(), w=img->width();
        int avg;
        for(register unsigned int i=0;i<w;i++)
            for(register unsigned int j=0;j<h;j++)
            {
                 rgb = img->pixel(i,j);
                 avg = ((qRed(rgb) + qGreen(rgb) + qBlue(rgb))/3) - darkness;
                 if(avg<0)avg=0;
                 tmp.setPixel(i,j, qRgb(avg, avg, avg));
            }
        return tmp;
    }
    Point rotate(Point, Point Center, int Angle);
    QImage magnify(QImage img, int x, int y, int factor);
};

#endif // TRANSFORMATIONS_H

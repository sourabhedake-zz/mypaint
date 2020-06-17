#include "mypalatte.h"

extern drawingSheet *l;
myPalatte::myPalatte()
{
    int w=255,h=255;
    img = new QImage(w,h,QImage::Format_RGB32);
    for(int i=0;i<w;i++)
        for(int j=0;j<h;j++)
            img->setPixel(i,j,(QColor::fromHsl(i,j,110)).rgb());
    setPixmap(QPixmap::fromImage(*img));
    setGeometry(400,400,w,h);
    setWindowTitle("Select Color");
}

void myPalatte::mousePressEvent(QMouseEvent *e)
{
    l->r = qRed(img->pixel(e->x(), e->y()));
    l->g = qGreen(img->pixel(e->x(), e->y()));
    l->b = qBlue(img->pixel(e->x(), e->y()));
    l->palFront->img->fill(qRgb(l->r,l->g,l->b));
    l->palFront->setPixmap(QPixmap::fromImage(*l->palFront->img));
    l->setEnabled(true);
    hide();
}

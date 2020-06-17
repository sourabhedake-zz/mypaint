#include "mygraphics.h"
#include<mainwindow.h>

extern drawingSheet *l;
Point::Point(QPoint P)
{
    this->x=P.x();
    this->y=P.y();
    this->n=NULL;
}

Point::Point()
{
    x=y=0;
    n=NULL;
}

void PolyList::newVertex(int x,int y)
{
    Polygon.vx.push_back(x);
    Polygon.vy.push_back(y);
}

PolyOperations::PolyOperations()
{
    start=NULL;
}

void PolyOperations::new_Polygon(Point *start)
{
    newn = new PolyList();
    newn->next=NULL;
    while(start)
    {
        newn->newVertex(start->x, start->y);
        start=start->n;
    }
    if(!this->start)
        this->start = newn;
    else
    {
        for(cur=this->start; cur->next;cur=cur->next);
        cur->next=newn;
    }
}

int PolyOperations::searchPolygon(int x, int y)
{
    QImage img(1000,800,QImage::Format_ARGB32);
    cur=start;
    int count=0;
    while(cur)
    {
        copyX=0;
        copyY=0;
        pasteX=0;
        pasteY=0;
        plot(cur, img,qRgb(0,0,0));
        if(img.pixel(x,y)== qRgb(0,0,0))
        {
            copyX=x;
            copyY=y;
            return count;
        }
        count++;
        cur=cur->next;
    }
    return -1;
}

bool DrawMethods::Threshold(QRgb trg, QRgb src, int threshold)
{
    if(((((qRed(src)<threshold)?0: qRed(src)-threshold) <= qRed(trg)) && (((qRed(src)>255 - threshold)?255:qRed(src)+threshold) >= qRed(trg))) &&
       ((((qGreen(src)<threshold)?0:qGreen(src)-threshold) <= qGreen(trg)) && (((qGreen(src)>255-threshold)?255:qGreen(src)+threshold) >= qGreen(trg))) &&
       ((((qBlue(src)<threshold)?0:qBlue(src)-threshold) <= qBlue(trg)) && (((qBlue(src)>255-threshold)?2551:qBlue(src)+threshold) >= qBlue(trg))))
            return 1;
    return 0;
}

void PolyOperations::getPolygon(int index, QImage &img, int x, int y, QRgb rgb)
{
    cur=start;
    while(index--)
    {
        cur=cur->next;
    }
    pasteX=x - copyX;
    pasteY=y - copyY;
    plot(cur, img, rgb);
}

void PolyOperations::clearAll()
{
    while(start)
    {
        cur=start->next;
        delete start;
        start=cur;
    }
}

void PolyOperations::plot(PolyList *cur, QImage &img, QRgb rgb)
{
    int a=cur->Polygon.vx.count()-1;
    while(a--)
    {
        drawL(1,cur->Polygon.vx.at(a+1) + pasteX,
              cur->Polygon.vy.at(a+1) + pasteY,
              cur->Polygon.vx.at(a) + pasteX,
              cur->Polygon.vy.at(a) + pasteY,
              img, rgb);
    }
}

void DrawMethods::drawL(int method, int x1, int y1, int x2, int y2, QImage &img, QRgb rgb)
{
    /* Method 1: DDA
     * Method 2: Bresenhnams
     * Method 3: DDA for Magnifying Glass( With Colour Instensity)
     */

    if(x1==x2 && y1==y2) return;
    if(method==1 || method ==3)
    {
        int length;
        if(abs(x2-x1) > abs(y2-y1))
            length=abs(x2-x1);
        else length=abs(y2-y1);
        float dx = (float)(x2-x1) / length;
        float dy = (float)(y2-y1) / length;
        float lnX=x1,lnY=y1;
        int i=1;
        if(isValid(x1,y1))
        {
            if(method==3)
            {
                QRgb col = img.pixel(x1,y1);
                img.setPixel(x1, y1,    qRgb((qRed(col)>255-int(rgb))?255:qRed(col)+rgb,
                                       (qGreen(col)>255-int(rgb))?255:qGreen(col)+rgb,
                                       (qBlue(col)>255-int(rgb))?255:qBlue(col)+rgb ));
            }
            else
                img.setPixel(x1,y1,rgb);
        }
        while(i<=length)
        {
            lnX = lnX  + dx;
            lnY = lnY  + dy;
             x1 = lnX + 0.5;
             y1 = lnY + 0.5;
            if(isValid(x1,y1))
            {
                if(method==3)
                {
                    QRgb col = img.pixel(x1,y1);
                    img.setPixel(x1, y1,    qRgb((qRed(col)>=255-int(rgb))?255:qRed(col)+rgb,
                                           (qGreen(col)>=255-int(rgb))?255:qGreen(col)+rgb,
                                           (qBlue(col)>=255-int(rgb))?255:qBlue(col)+rgb ));
                }
                else
                    img.setPixel(x1,y1,rgb);
            }
            i++;
        }
    }
    else{
        int dx,dy, x=x1, y=y1;
        dx=abs(x2-x1);
        dy=abs(y2-y1);
        int e=2*dy-dx;
        if(dx>dy)
        {
            for(int i=1;i<=dx;i++)
            {
                if(isValid(x,y) && img.pixel(x,y) != qRgb(0,0,0)) img.setPixel(x,y,rgb);
                while(e>0)
                {
                    y=y+sign(y2-y1);
                    e=e-2*dx;
                }
                x=x+sign(x2-x1);
                e=e+2*dy;
            }

        }
        else
        {
            for(int i=1;i<=dy;i++)
            {
                if(isValid(x,y) && img.pixel(x,y) != qRgb(0,0,0))img.setPixel(x,y,rgb);

                while(e>0)
                {
                    x=x+sign(x2-x1);
                    e=e-2*dy;
                }
                y=y+sign(y2-y1);
                e=e+2*dx;
            }
        }
        if(isValid(x,y)&& img.pixel(x,y) != qRgb(0,0,0))img.setPixel(x,y,rgb);
    }
}

int DrawMethods::sign(int num)
{
    if(num > 0) return 1;
    else if(num==0) return 0;
    else    return -1;
}

void DrawMethods::Bresenhnams(int cx, int cy, int r,QImage &img, QRgb rgb, QImage *src)
{
    int x=0,y=r;		//initialize the decision values.
    int limit=0;
    int err=2*(1-r);			//calculate the decision parameter
    while(y>=limit)
    {
        if(isValid(x+cx ,cy-y))
        {
            if(rgb==200)
            {
                int avg=(qRed(img.pixel(x+cx, cy-y)) + qGreen(img.pixel(x+cx, cy-y)) +qBlue(img.pixel(x+cx, cy-y)) )/3;
                img.setPixel(x+cx,cy-y,qRgb(avg,avg,avg));        //4th quadrant
            }
            else if(rgb==150)
            {
                img.setPixel(x+cx,cy-y,src->pixel(x+cx,cy-y));
            }
            else
                img.setPixel(x+cx,cy-y,rgb);
        }
        if(isValid(-x+cx,cy-y))
        {
            if(rgb==200)
            {
                int avg=(qRed(img.pixel(-x+cx,cy-y)) + qGreen(img.pixel(-x+cx,cy-y)) +qBlue(img.pixel(-x+cx,cy-y)) )/3;
                img.setPixel(-x+cx,cy-y,qRgb(avg,avg,avg));		//3rd quadrant
            }
            else if(rgb==150)
            {
                img.setPixel(-x+cx,cy-y,src->pixel(-x+cx,cy-y));
            }
            else
                img.setPixel(-x+cx,cy-y,rgb);
        }
        if(isValid(x+cx ,cy+y))
        {
            if(rgb==200)
            {
                int avg=(qRed(img.pixel(x+cx ,cy+y)) + qGreen(img.pixel(x+cx ,cy+y)) +qBlue(img.pixel(x+cx ,cy+y)) )/3;
                img.setPixel(x+cx,cy+y,qRgb(avg,avg,avg));		//1st quadrant
            }
            else if(rgb==150)
            {
                img.setPixel(x+cx,cy+y,src->pixel(x+cx,cy+y));
            }
            else
                img.setPixel(x+cx,cy+y,rgb);
        }
        if(isValid(-x+cx,cy+y))
        {
            if(rgb==200)
            {
                int avg=(qRed(img.pixel(-x+cx,cy+y)) + qGreen(img.pixel(-x+cx,cy+y)) +qBlue(img.pixel(-x+cx,cy+y)) )/3;
                img.setPixel(-x+cx,cy+y,qRgb(avg,avg,avg));		//2nd quadrant
            }
            else if(rgb==150)
            {
                img.setPixel(-x+cx,cy+y,src->pixel(-x+cx,cy+y));
            }
            else
                img.setPixel(-x+cx,cy+y,rgb);
        }
        if(err<0)
        {
            int e=(2*err)+(2*y)-1;
            if(e<=0)
            {
                x=x+1;
                err=err+(2*x)+1;
            }
             else
            {
                x=x+1;
                y=y-1;
                err=err+(2*x)-(2*y)+2;
            }
        }
       else if(err>0)
        {
           int t=(2*err)-(2*x)-1;
               if(t<=0)
                {
                    x=x+1;
                    y=y-1;
                    err=err+(2*x)-(2*y)+2;
                }
                    else
                {
                   y=y-1;
                   err=err-(2*y)+1;
                }
        }
       else
        {
            x=x+1;
            y=y-1;
            err=err+(2*x)-(2*y)+2;
        }
    }
}

void FillMethods::scanfillPoly(PointOperations Points, QImage &img)
{
    int tmp = Points.getCount();
    int *x, *y;
    x = new int[tmp];
    y = new int[tmp];
    for(int i=1;i<=tmp;i++)
    {
        x[i-1] = Points.getPoint(i).x;
        y[i-1] = Points.getPoint(i).y;
    }
    int n=tmp;
    int i,k,*inX,  yy, ymax=y[0], ymin=y[0];
    float *m, dx,dy;
    inX = new int[1000*690];
    m = new float[1000*690];
    for(i=0;i<n;i++)
    {
        if(y[i] >ymax) ymax=y[i];
        if(y[i] <ymin) ymin=y[i];

        dx = x[i+1] - x[i];
        dy = y[i+1] - y[i];
        if(!dx) m[i]=0;
        if(!dy) m[i]=1;
        if( dx && dy)
            m[i]= (float)dx/dy;
       }
    int cnt;

    for(yy=ymax; yy>=ymin; yy--)
    {
        cnt=0;
        for(i=0;i<n;i++)
        {
            if((y[i] > yy && y[i+1] <=yy) || (y[i]<=yy && y[i+1]>yy))
            {
                inX[cnt] = (x[i] + (m[i] * (yy - y[i])));
                cnt++;
            }
        }
        for(k=0; k<cnt;k++)
        {
            for(i=0;i<cnt-1;i++)
            {
                if(inX[i] >inX[i+1])
                {
                    tmp = inX[i];
                    inX[i] = inX[i+1];
                    inX[i+1]=tmp;
                }
            }
        }
        for(i=0; i<cnt-1;i+=2)
        {
            drawL(2, inX[i],yy,inX[i+1], yy, img, qRgb(l->r, l->g, l->b));
        }
    }
}






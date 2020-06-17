#include "transformations.h"
#include<math.h>
#define MagnifierSize 100

Point Transform::rotate(Point P, Point Center, int Angle)
{
    float Ang = ((float)Angle*(((float)22)/7))/180;
    Point newP;
    newP.x =  (P.x * cos(Ang)) -
                (P.y * sin(Ang)) +
                (-(Center.x * cos(Ang)) + (Center.y *sin(Ang)) +Center.x );

    newP.y =  (P.x * sin(Ang)) +
                (P.y * cos(Ang)) +
                (-(Center.x * sin(Ang)) - (Center.y *cos(Ang)) +Center.y );
    return newP;
}

QImage Transform::magnify(QImage img, int x, int y, int factor)
{
    int MagHalf = MagnifierSize/2;
    QImage tmp; tmp= (img);
    static struct pix{
        unsigned short r[MagnifierSize+1][MagnifierSize+1];
        unsigned short g[MagnifierSize+1][MagnifierSize+1];
        unsigned short b[MagnifierSize+1][MagnifierSize+1];
    } pixels;

    for(int i=0;i<=MagnifierSize;i++)
        for(int j=0; j<=MagnifierSize ;j++)
            if(isValid((x-MagHalf)+i,(y-MagHalf)+j))
            {
                pixels.r[i][j] = qRed(img.pixel((x-MagHalf)+i,(y-MagHalf)+j));
                pixels.g[i][j] = qGreen(img.pixel((x-MagHalf)+i,(y-MagHalf)+j));
                pixels.b[i][j] = qBlue(img.pixel((x-MagHalf)+i,(y-MagHalf)+j));
            }

    //FRAME FOR MAGNIFYING GLASS

    int xStart=x-(MagHalf*factor), yStart=y-(MagHalf*factor);

    for(int width =0, colIntensity=150; width-16; width++, colIntensity-=8)
    {
        drawL(3, xStart-width, yStart - width, xStart+(2*MagHalf*factor)+width, yStart-width, tmp,  colIntensity);
        drawL(3, xStart+(2*MagHalf*factor)+width, yStart -width+1, xStart+(2*MagHalf*factor)+width,yStart+(2*MagHalf*factor)+width-1, tmp,  colIntensity);
        drawL(3, xStart-width, yStart+(2*MagHalf*factor)+width, xStart+(2*MagHalf*factor)+width, yStart+(2*MagHalf*factor)+width, tmp,  colIntensity);
        drawL(3, xStart-width, yStart - width+1, xStart-width, yStart+(2*MagHalf*factor)+width-1, tmp,  colIntensity);
    }

    for(int i=x-(MagHalf*factor), m=0 ;i<=x+(MagHalf*factor);i+=factor, m++)
        for(int j=y-(MagHalf*factor), n=0; j<=y+(MagHalf*factor) ;j+=factor, n++)
            for(int k=0; k< factor; k++)
                for(int l=0; l< factor; l++)
                    if(isValid(i+k, j+l))
                        tmp.setPixel(i+k,j+l, qRgb(pixels.r[m][n], pixels.g[m][n], pixels.b[m][n]));


    return tmp;
}

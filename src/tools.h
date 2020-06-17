#ifndef TOOLS_H
#define TOOLS_H
#include<QRgb>

class myTools
{
public:
    int *bresenhnams;
    int *lineDDA;
    int *boundaryFill;
    int *floodFill;
    int *scanLineFill;
    int *bresCircle;
    int *ConCircle;
    int *Eraser;
    int *ColSelector;
    int *SelectWorkArea;
    int *importImg;
    int *copyPoly;
    int *rotPoly;
    int *EffBNW;
    int *SelEffBNW;
    int *SelEffAntiBNW;
    int *DoNothing;
    int *Brighten;
    int *Open;
    int *Save;
    myTools();
    ~myTools();
    //COLORS
};

#endif // TOOLS_H

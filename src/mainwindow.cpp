#include "mainwindow.h"
#include<math.h>
#include<QStyle>
#include<QInputDialog>
#include<QMessageBox>
#define COL_THRESHOLD       10
#define ENCRYPTION_ALPHA    '?'

QImage *img, *importImg, *backupImg;
drawingSheet *l;

drawingSheet::drawingSheet()
{
    img = new QImage(MAX_WIDTH, MAX_HEIGHT,QImage::Format_RGB32);
    backupImg = NULL;
    img->fill(qRgb(255,255,255));
    firstX=firstY=polyNum=-1;
    MagniFactor=1; ImageMagniFactor=100;
    setWindowTitle("GRAPHICS DEMOSTRATION");
    setFixedSize(MAX_WIDTH,MAX_HEIGHT);
    setPixmap(QPixmap::fromImage(*img));
    setMouseTracking(true);
    show();
    r = g =200; b=255;
    ValidXStart=ValidYStart=0;
    ValidXEnd=MAX_WIDTH-1; ValidYEnd=MAX_HEIGHT-1;
    Palt = new myPalatte();
    frm = new myFrame;
    frm->setParent(this);
    frm->setGeometry(-55,65,60,MAX*29+20);
    frm->show();
    frm->setAutoFillBackground(true);
    frm->setFrameStyle(2);
    frmHeader = new myFrame();
    frmHeader->setParent(this);
    frmHeader->setAutoFillBackground(true);
    frmHeader->setGeometry(10,-35,MAX_WIDTH-20,40);
    frmHeader->show();
    frmHeader->setFrameStyle(2);
    frmPalatte = new myFrame();
    frmPalatte->setParent(this);
    frmPalatte->setAutoFillBackground(true);
    frmPalatte->setGeometry(MAX_WIDTH-5,65,60,MAX*29+20);
    frmPalatte->show();
    frmPalatte->setFrameStyle(2);
    //LOADING TOOLS
    tools = new myTools();
    for(int i=0;i<MAX;i++)
        lbl[i] = new myLabel(40,27);
    lbl[0]->name = ("Boundary Fill (Black Boundary)");
    lbl[1]->name = ("Flood Fill");
    lbl[2]->name = ("Scan Line");
    lbl[3]->name = ("Draw Line By DDA");
    lbl[4]->name = ("Draw Line By Bresenhnams");
    lbl[5]->name = ("Draw Circle");
    lbl[6]->name = ("Draw Concentric Circles");
    lbl[7]->name = ("Eraser");
    lbl[8]->name = ("Color Selector");
    lbl[9]->name = ("Select Working Area!");
    lbl[10]->name = ("Import Image");
    lbl[11]->name = ("Copy Polygon");
    lbl[12]->name = ("Rotate Polygon");
    lbl[13]->name = ("Effect: BnW [FULL]");
    lbl[14]->name = ("Effect: BnW [SELECTIVE]");
    lbl[15]->name = ("Effect: Anti - BnW [SELECTIVE]");
    lbl[16]->name = ("Effect: Brighten");
    lbl[MAX-1]->name = ("Do Nothing!");
    lblOpen = new myLabel(40,27);
    lblSave = new myLabel(40,27);
    lblOpen->name = ("File_Open");
    lblSave->name = ("File_Save");
    int *points;
    for(int i=0;i<MAX;i++)
    {
        lbl[i]->setParent(frm);
        lbl[i]->move(10,(29 *i) +10);
        if     (lbl[i]->name == ("Boundary Fill (Black Boundary)")) points = tools->boundaryFill;
        else if(lbl[i]->name == ("Flood Fill")) points = tools->floodFill;
        else if(lbl[i]->name == ("Scan Line")) points = tools->scanLineFill;
        else if(lbl[i]->name == ("Draw Line By DDA")) points = tools->lineDDA;
        else if(lbl[i]->name == ("Draw Line By Bresenhnams")) points = tools->bresenhnams;
        else if(lbl[i]->name == ("Draw Circle")) points = tools->bresCircle;
        else if(lbl[i]->name == ("Draw Concentric Circles")) points = tools->ConCircle;
        else if(lbl[i]->name == ("Eraser")) points = tools->Eraser;
        else if(lbl[i]->name == ("Color Selector")) points = tools->ColSelector;
        else if(lbl[i]->name == ("Select Working Area!")) points = tools->SelectWorkArea;
        else if(lbl[i]->name == ("Import Image")) points = tools->importImg;
        else if(lbl[i]->name == ("Copy Polygon")) points = tools->copyPoly;
        else if(lbl[i]->name == ("Rotate Polygon")) points = tools->rotPoly;
        else if(lbl[i]->name == ("Effect: BnW [FULL]")) points = tools->EffBNW;
        else if(lbl[i]->name == ("Effect: BnW [SELECTIVE]")) points = tools->SelEffBNW;
        else if(lbl[i]->name == ("Effect: Anti - BnW [SELECTIVE]")) points = tools->SelEffAntiBNW;
        else if(lbl[i]->name == ("Effect: Brighten")) points = tools->Brighten;
        else if(lbl[i]->name == ("Do Nothing!")) points = tools->DoNothing;
        else points = tools->bresenhnams;
        for(int index=0; points[index] != -1;index+=5)
            lbl[i]->img->setPixel( points[index+1],points[index], qRgb(points[index+2],points[index+3],points[index+4]));
        lbl[i]->setPixmap(QPixmap::fromImage(*lbl[i]->img));
        lbl[i]->show();
    }
    points = tools->Open;
    for(int index=0; points[index] != -1;index+=5)
        lblOpen->img->setPixel( points[index+1],points[index], qRgb(points[index+2],points[index+3],points[index+4]));
    points = tools->Save;
    for(int index=0; points[index] != -1;index+=5)
        lblSave->img->setPixel( points[index+1],points[index], qRgb(points[index+2],points[index+3],points[index+4]));
    //SETTING PALATTE COLORS
    for(int i=0;i<COL_MAX;i++)
        pal[i] = new myLabel(20,20);
    pal[0]->img->fill(qRgb(1,1,1));
    pal[1]->img->fill(qRgb(50,50,50));
    pal[2]->img->fill(qRgb(100,100,100));
    pal[3]->img->fill(qRgb(150,150,150));
    pal[4]->img->fill(qRgb(200,200,200));
    pal[5]->img->fill(qRgb(255,255,255));
    pal[6]->img->fill(qRgb(255,00,00));
    pal[7]->img->fill(qRgb(255,50,50));
    pal[8]->img->fill(qRgb(255,100,100));
    pal[9]->img->fill(qRgb(255,155,155));
    pal[10]->img->fill(qRgb(255,200,200));
    pal[12]->img->fill(qRgb(255,215,215));
    pal[13]->img->fill(qRgb(00,255,00));
    pal[14]->img->fill(qRgb(55,255,55));
    pal[15]->img->fill(qRgb(105,255,105));
    pal[16]->img->fill(qRgb(155,255,155));
    pal[17]->img->fill(qRgb(205,255,205));
    pal[18]->img->fill(qRgb(00,000,255));
    pal[19]->img->fill(qRgb(55,55,255));
    pal[20]->img->fill(qRgb(105,105,255));
    pal[21]->img->fill(qRgb(155,155,255));
    pal[22]->img->fill(qRgb(205,205,255));
    pal[23]->img->fill(qRgb(225,225,255));
    pal[24]->img->fill(qRgb(205,205,255));
    pal[25]->img->fill(qRgb(255,255, 0));
    pal[26]->img->fill(qRgb(0,255,255));
    pal[27]->img->fill(qRgb(255,0, 255));
    for(int i=0;i<COL_MAX;i++)
    {
        pal[i]->name="COLOR";
        pal[i]->setPixmap(QPixmap::fromImage((*pal[i]->img)));
        if(i%2==0 )
            pal[i]->move(5, ((i/2)* (pal[i]->height()+10)) +15);
        else
            pal[i]->move(frmPalatte->width()/2+5, ((i/2)* (pal[i]->height()+10)) +15);
        pal[i]->setParent(frmPalatte);
        pal[i]->setFrameStyle(1);
        pal[i]->show();
    }
    palFront = new myLabel(50,35);
    palFront->name="MAIN_COLOR";
    palFront->setPixmap((QPixmap::fromImage(*palFront->img)));
    palFront->move(5,(COL_MAX/2)*30+15);
    palFront->setParent(frmPalatte);
    palFront->setFrameStyle(1);
    palFront->show();
    lblOpen->setPixmap(QPixmap::fromImage(*lblOpen->img));
    lblSave->setPixmap(QPixmap::fromImage(*lblSave->img));
    lblOpen->setParent(frmHeader);
    lblSave->setParent(frmHeader);
    tempName.setParent(frmHeader);
    lblOpen->setGeometry(260,5,40,27);
    lblSave->setGeometry(310,5,40,27);
    tempName.setGeometry(115, 5, 140, 27);
    lblOpen->show();
    lblSave->show();
    tempName.show();
    delete tools;
    frmStatus = new myFrame;
    frmStatus->setParent(this);
    frmStatus->setGeometry(10,MAX_HEIGHT-5,MAX_WIDTH-20,60);
    frmStatus->show();
    frmStatus->setStyleSheet("background-color: gray");
    frmStatus->setFrameStyle(2);
    text1 = new Text;
    text1->setGeometry(5,10,MAX_WIDTH-30,20);
    text1->setParent(frmStatus);
    text1->show();
    text1->setEnabled(false);
    text1->setStyleSheet("background-color: white");
    l.setText("Status Bar....");
    l.setStyleSheet("background-color: white");
    l.setGeometry(5,35,MAX_WIDTH-30,20);
    l.setParent(frmStatus);
    l.setFrameStyle(1);
    l.show();
    lbl1.setText("Enter Filename: ");
    lbl1.setAutoFillBackground(true);
    lbl1.setGeometry(5,5,110,27);
    lbl1.setParent(frmHeader);
    lbl1.show();
    t1 = new QTimer(this);
    t2 = new QTimer(this);
    t5 = new QTimer(this);
    t6 = new QTimer(this);
    t7 = new QTimer(this);
    connect(t1, SIGNAL(timeout()),  SLOT(update()));
    connect(t2, SIGNAL(timeout()),  SLOT(setToolPosition()));
    connect(t5, SIGNAL(timeout()),  SLOT(setStatusPosition()));
    connect(t6, SIGNAL(timeout()),  SLOT(setHeaderPosition()));
    connect(t7, SIGNAL(timeout()),  SLOT(setPalattePosition()));
}

///MODULES FOR ACTION LISTENERS
void drawingSheet::keyReleaseEvent(QKeyEvent *)
{   isStraight=0;   }

void drawingSheet::keyPressEvent(QKeyEvent *e)
{
    if(e->key() == Qt::Key_Tab)
        text1->setEnabled(!text1->isEnabled());
    else if ( e->key() == 'H' )
        isStraight = 1;
    else if ( e->key() == 'V' )
        isStraight = 2;
    else if(e->key() ==Qt::Key_Escape)
    {
        ptSet=0;
        lbl[MAX-1]->mousePressEvent(NULL);
    }
}

void drawingSheet::mouseMoveEvent(QMouseEvent *e)
{
    if (e->x()<10)  t2->start(1);
    if (e->x()>frm->width()+15) frm->move(-frm->width()+5, frm->y());
    if (e->y()>MAX_HEIGHT-10)  t5->start(1);
    if (e->y()<MAX_HEIGHT-10-frmStatus->height())  frmStatus->move(10,MAX_HEIGHT-5);
    if (e->y()<10)  t6->start(1);
    if (e->y()>frmHeader->height()+20)  frmHeader->move(10,-35);
    if (e->x()>MAX_WIDTH-10)  t7->start(1);
    if (e->x()<MAX_WIDTH - frmPalatte->width()-15) frmPalatte->move(MAX_WIDTH-5, frmPalatte->y());

    if(mode!=7 && mode!=8 && mode !=13 && mode!=16 && mode!=17)polyNum=-1;
    QImage dupImg = *img;
    if(mode==5 )
    {
        Bresenhnams(e->x(), e->y() , 8,dupImg, qRgb(100,130,185));
        Bresenhnams(e->x(), e->y() , 7,dupImg, qRgb(120,150,205));
        Bresenhnams(e->x(), e->y() , 6,dupImg, qRgb(140,170,225));
        if(e->buttons())
        {
            Bresenhnams(e->x(), e->y() , 4 ,*img, qRgb(255,255,255));
            Bresenhnams(e->x(), e->y() , 3 ,*img, qRgb(255,255,255));
            Bresenhnams(e->x(), e->y() , 2 ,*img, qRgb(255,255,255));
            Bresenhnams(e->x(), e->y() , 1 ,*img, qRgb(255,255,255));
            Bresenhnams(e->x()-1, e->y() , 1 ,*img, qRgb(255,255,255));
            Bresenhnams(e->x()+1, e->y() , 1 ,*img, qRgb(255,255,255));
            Bresenhnams(e->x(), e->y()+1 , 1 ,*img, qRgb(255,255,255));
            Bresenhnams(e->x(), e->y()-1 , 1 ,*img, qRgb(255,255,255));
            Bresenhnams(e->x(), e->y()-1 , 0 ,*img, qRgb(255,255,255));
        }
        if(MagniFactor>1)  dupImg =trans.magnify( dupImg, e->x(), e->y(),MagniFactor);
        setPixmap(QPixmap::fromImage(dupImg));
        return;
    }
    else if(mode==10)
    {
        float AspectRatio = (float)importImg->width() / importImg->height();
        QImage PHOTO = importImg->scaled(ImageMagniFactor * AspectRatio, ImageMagniFactor);
        ImageMerge(dupImg, PHOTO, e->x(), e->y());
         if(MagniFactor>1)  dupImg =trans.magnify( dupImg, e->x(), e->y(),MagniFactor);
         setPixmap(QPixmap::fromImage(dupImg));
        return;
    }

    if(!mode || !ptSet)
    {
        if(MagniFactor!=1 && que.isEmpty())
        {
            dupImg =trans.magnify( dupImg, e->x(), e->y(),MagniFactor);
            setPixmap(QPixmap::fromImage(dupImg));
        }
        return;
    }
    if(mode==1 || mode==4)
    {
        if(isStraight==1)
            drawL((mode==1)?1:4,p1.x, p1.y, e->x(),p1.y, dupImg, qRgb(140,170,225));
        else if(isStraight==2)
            drawL((mode==1)?1:4,p1.x, p1.y, p1.x, e->y(), dupImg, qRgb(140,170,225));
        else
            drawL((mode==1)?1:4,p1.x, p1.y, e->x(), e->y(), dupImg, qRgb(140,170,225));
    }
    else if(mode==2 || mode==3)
    {
        drawL(1,p1.x,p1.y,e->x(),e->y(),dupImg,qRgb(225,170,140));
        Bresenhnams(p1.x, p1.y , sqrt (pow(abs(p1.x - e->x()),2)+ pow(abs(p1.y - e->y()),2)),dupImg, qRgb(140,170,225));
    }
    else if(mode==7 && polyNum!=-1)
    {
        Polygons.getPolygon(polyNum,dupImg,e->x(), e->y(), qRgb(140,170,255));
        drawL(1,Polygons.copyX,Polygons.copyY,e->x(),e->y(),dupImg,qRgb(225,170,140));
    }

    else if(mode==8 )
    {
        if(polyNum>=0) p1 = e->pos();
        Point old = backup2;
        if(polyNum == -2)
        {
            int Angle=((p1.x-e->x()))%360;
            for(int i=0; i<Polygons.cur->Polygon.vx.count();i++)
            {
                old.x=Polygons.cur->Polygon.vx[i]; old.y = Polygons.cur->Polygon.vy[i];
                Point newP = trans.rotate(old, p1, Angle);
                if(i)drawL(1, backup.x, backup.y, newP.x, newP.y,dupImg,qRgb(140,170,255) );
                backup.x=newP.x;
                backup.y=newP.y;
                if(isValid(newP.x, newP.y)) dupImg.setPixel(newP.x, newP.y,qRgb(140,170,255));
            }
            old = trans.rotate(backup2, p1, Angle);
        }
        if(polyNum!=-1) drawL(1, p1.x, p1.y, old.x, old.y,dupImg,qRgb(225,170,140) );
    }
    else if(mode==13 && polyNum == -2)
    {
        if(!(backup.x >= e->x() || backup.y >=e->y()))
        {
            dupImg = trans.blackNwhite(img,50);
            for(int x=backup.x; x<= e->x();x++)
                for(int y=backup.y;y<= e->y();y++)
                    dupImg.setPixel(x,y,img->pixel(x,y));
        }

    }
    else if(mode==16)
    {
        if(polyNum==-2)
            for(int i=1;i<10;i++)
                Bresenhnams(e->x(), e->y(),i,*img,200);
    }
    else if(mode==17)
    {
        if(polyNum==-2)
            for(int i=1;i<10;i++)
                Bresenhnams(e->x(), e->y(),i,*img,150, backupImg);
    }


    if(MagniFactor>1)  dupImg =trans.magnify( dupImg, e->x(), e->y(),MagniFactor);
    setPixmap(QPixmap::fromImage(dupImg));
}


void drawingSheet::mousePressEvent(QMouseEvent *e)
{
    if(!isValid(e->x(), e->y()) && mode!=13)return;
    Undo.newMove(*img);
    if(mode!=7 && mode!=8 && mode!=13 && mode!=16 && mode!=17)polyNum=-1;
    if((mode==0 || mode == 11  || mode ==12) && e->buttons()==Qt::RightButton)
    {
        r=(rand()%255)+1, g=(rand()%255)+1, b=(rand()%255)+1;
        palFront->img->fill(qRgb(r,g,b));
        palFront->setPixmap(QPixmap::fromImage(*palFront->img));
    }
    if(mode==0 || mode ==11)
    {
        if(mode==0) if(img->pixel(e->x(), e->y()) == qRgb(0,0,0)) return;
        if(mode==11)rgb =img->pixel(e->x(), e->y());
        if(field[e->x()][e->y()]==0) que.ins(e->x(), e->y());
        field[e->x()][e->y()]=1;
        t1->start(1);
        return;
    }
    ptSet++;
    if(mode==1 || mode==4)
    {
        if(e->buttons() == Qt::LeftButton)
        {
            if(ptSet<=1)
            {
                ScanPoints.Erase();
                p1.x=firstX=e->x();
                p1.y=firstY=e->y();
                img->setPixel(p1.x,p1.y,qRgb(0,0,0));
                l.setText("Press H: Horizontal Line     V: Vertical Line");
            }
            else if(ptSet==2)
            {
                ptSet=1;
                l.setText("Press Right Button to complete Polygon");
                if(isStraight==1)
                {
                    drawL((mode==1)?1:4, p1.x, p1.y, e->x(),p1.y, *img, qRgb(0,0,0));
                    p1.x=e->x();
                }
                else if(isStraight==2)
                {
                    drawL((mode==1)?1:4, p1.x, p1.y, p1.x, e->y(), *img, qRgb(0,0,0));
                    p1.y=e->y();
                }
                else
                {
                    drawL((mode==1)?1:4, p1.x, p1.y, e->x(), e->y(), *img, qRgb(0,0,0));
                    p1.x=e->x();
                    p1.y=e->y();
                }
            }
            ScanPoints.insert(p1.x, p1.y);
        }
        else
        {
            if(ptSet==2)
            {
                ptSet=1;
                drawL((mode==1)?1:4, p1.x, p1.y, firstX, firstY, *img, qRgb(0,0,0));
                p1.x=firstX;
                p1.y=firstY;
                ScanPoints.insert(p1.x, p1.y);
                Polygons.new_Polygon(ScanPoints.start);
                ScanPoints.Erase();
                ScanPoints.insert(p1.x, p1.y);
            }
        }
    }
    else if(mode==2 || mode==3)
    {
        if(ptSet<=1 )
        {
            p1.x=e->x();
            p1.y=e->y();
        }
        else
        {
            if(mode==2) ptSet=0;
            Bresenhnams(p1.x, p1.y , (int)sqrt (pow(abs(p1.x - e->x()),2)+ pow(abs(p1.y - e->y()),2)),*img, qRgb(0,0,0));
        }
    }
    else if(mode==5)
    {
        Bresenhnams(e->x(), e->y() , 4 ,*img, qRgb(255,255,255));
        Bresenhnams(e->x(), e->y() , 3 ,*img, qRgb(255,255,255));
        Bresenhnams(e->x(), e->y() , 2 ,*img, qRgb(255,255,255));
        Bresenhnams(e->x(), e->y() , 1 ,*img, qRgb(255,255,255));
        Bresenhnams(e->x()-1, e->y() , 1 ,*img, qRgb(255,255,255));
        Bresenhnams(e->x()+1, e->y() , 1 ,*img, qRgb(255,255,255));
        Bresenhnams(e->x(), e->y()+1 , 1 ,*img, qRgb(255,255,255));
        Bresenhnams(e->x(), e->y()-1 , 1 ,*img, qRgb(255,255,255));
        Bresenhnams(e->x(), e->y()-1 , 0 ,*img, qRgb(255,255,255));
    }
    else if(mode==6)
    {
        ::l->r=(qRed(img->pixel(e->x(), e->y())));
        ::l->g=(qGreen(img->pixel(e->x(), e->y())));
        ::l->b=(qBlue(img->pixel(e->x(), e->y())));
        palFront->img->fill(qRgb(::l->r,::l->g,::l->b));
        palFront->setPixmap(QPixmap::fromImage(*palFront->img));
        lbl[0]->mousePressEvent(NULL);
    }
    else if(mode==7)
    {
        if(polyNum==-1)
        {
            if(img->pixel(e->x(), e->y()) == qRgb(0,0,0))
            {
                int flag = Polygons.searchPolygon(e->x(), e->y());
                polyNum=flag;
                if(polyNum!=-1)
                    l.setText("Click the region where you want to paste the polygon");
            }
        }
        else
        {
            Polygons.getPolygon(polyNum, *img, e->x(), e->y(),qRgb(0,0,0));
            ScanPoints.Erase();
            for(int i=0; i<Polygons.cur->Polygon.vx.count();i++)
            ScanPoints.insert(Polygons.cur->Polygon.vx.at(i)+Polygons.pasteX, Polygons.cur->Polygon.vy.at(i)+Polygons.pasteY);
            Polygons.new_Polygon(ScanPoints.start);
        }
    }
    else if(mode==8)
    {
        if(polyNum==-1)
        {
            if(img->pixel(e->x(), e->y()) == qRgb(0,0,0))
            {
                polyNum=Polygons.searchPolygon(e->x(), e->y());
                if(polyNum>=0)
                {
                    l.setText("Click the point about which the polygon will be rotated");
                    backup2.x=e->x();
                    backup2.y=e->y();
                }
            }
        }
        else if(polyNum>-1)
        {
            p1.x = e->x();
            p1.y = e->y();
            polyNum = -2;
            l.setText("Now adjust the angle and Left click to confirm!");
        }
        else if(polyNum == -2)
        {
            int Angle=((p1.x-e->x()))%360;
            Point old;
            QVector <int> tmpx, tmpy;
            PolyList *backupPointer = Polygons.cur;
            for(int i=0; i<Polygons.cur->Polygon.vx.count();i++)
            {
                old.x=Polygons.cur->Polygon.vx[i]; old.y = Polygons.cur->Polygon.vy[i];
                Point newP = trans.rotate(old, p1, Angle);
                if(i)drawL(1, backup.x, backup.y, newP.x, newP.y, *img,qRgb(0,0,0));
                backup.x=newP.x;
                backup.y=newP.y;
                tmpx.push_back(backup.x);
                tmpy.push_back(backup.y);
                if(isValid(newP.x, newP.y)) img->setPixel(newP.x, newP.y,qRgb(0,0,0));
            }
            ScanPoints.Erase();
            for(int i=0;i<tmpx.count();i++)
                ScanPoints.insert(tmpx[i], tmpy[i]);
            Polygons.new_Polygon(ScanPoints.start);
            Polygons.cur = backupPointer;

        }
    }
    else if(mode==10)
    {
        float AspectRatio = (float)importImg->width() / importImg->height();
        QImage PHOTO = importImg->scaled(ImageMagniFactor * AspectRatio, ImageMagniFactor);
        ImageMerge(*img, PHOTO, e->x(), e->y());
        setPixmap(QPixmap::fromImage(*img));
        return;
    }
    else if(mode==12)
    {
        if(img->pixel(e->x(), e->y()) == qRgb(0,0,0))
        {
            int flag = Polygons.searchPolygon(e->x(), e->y());
            polyNum=flag;
            if(polyNum!=-1)
            {
                ScanPoints.Erase();
                Polygons.getPolygon(polyNum, *img, e->x(), e->y(),qRgb(0,0,0));
                for(int i=0; i<Polygons.cur->Polygon.vx.count();i++)
                ScanPoints.insert(Polygons.cur->Polygon.vx.at(i)+Polygons.pasteX, Polygons.cur->Polygon.vy.at(i)+Polygons.pasteY);
                scanfillPoly(ScanPoints, *img);
                setPixmap(QPixmap::fromImage(*img));
            }
        }
    }
    else if(mode==13)
    {
        if(polyNum==-1)
        {
            backup.x=e->x();
            backup.y=e->y();
            polyNum=-2;
        }
        else if(polyNum==-2)
        {
            if(!(backup.x >= e->x() || backup.y >=e->y()))
            {
                ValidXStart=backup.x;
                ValidYStart=backup.y;
                ValidXEnd=e->x();
                ValidYEnd=e->y();

                backupImg = new QImage(img->copy(0,0,MAX_WIDTH, MAX_HEIGHT));
                *img = trans.blackNwhite(backupImg);

                for(int x=backup.x; x<= e->x();x++)
                    for(int y=backup.y;y<= e->y();y++)
                        img->setPixel(x,y,backupImg->pixel(x,y));
            }
            else
            {
                for(int x=ValidXStart; x<= ValidXEnd;x++)
                    for(int y=ValidYStart;y<= ValidYEnd;y++)
                        backupImg->setPixel(x,y,img->pixel(x,y));
                *img = backupImg->copy(0,0,MAX_WIDTH, MAX_HEIGHT);
                ValidXStart=ValidYStart=0;
                ValidXEnd=MAX_WIDTH-1;
                ValidYEnd=MAX_HEIGHT-1;
            }
           lbl[MAX-1]->mousePressEvent(NULL);
        }
    }
    else if(mode==14 || mode==15)
    {
        que.clearAll();
        que.ins(e->x(), e->y());
        field[e->x()][e->y()]=1;
        t1->start(1);
        return;
    }
    else if(mode==16)
    {
        if(polyNum==-1)
        {
            for(int i=1;i<10;i++)
                Bresenhnams(e->x(), e->y(),i,*img,200);
            polyNum=-2;
        }
        else
            polyNum=-1;
    }
    else if(mode==17)
    {
        if(polyNum==-1)
        {
            for(int i=1;i<10;i++)
                Bresenhnams(e->x(), e->y(),i,*img,150, backupImg);
            polyNum=-2;
        }
        else
            polyNum=-1;
    }
    QImage dupImg = *img;
    if(MagniFactor>1)  dupImg =trans.magnify( dupImg, e->x(), e->y(),MagniFactor);
    setPixmap(QPixmap::fromImage(dupImg));
}

void drawingSheet::wheelEvent(QWheelEvent *e)
{
    QImage dupImg= *img;
    if(mode==10)
    {
        if(e->delta()<0)
        {
            if(ImageMagniFactor>5) ImageMagniFactor-=5;
        }
        else
            if(ImageMagniFactor<3000) ImageMagniFactor+=5;

        float AspectRatio = (float)importImg->width() / importImg->height();
        QImage PHOTO = importImg->scaled(ImageMagniFactor * AspectRatio, ImageMagniFactor);
        ImageMerge(dupImg, PHOTO, e->x(), e->y());
    }
    else
    {
        if(e->delta()<0)
        {
            if(MagniFactor>1) MagniFactor--;
            else
                MagniFactor=1;
        }
        else
            if(MagniFactor<15) MagniFactor++;
        if(MagniFactor>1)
            dupImg = trans.magnify( dupImg, e->x(), e->y(), MagniFactor);
    }
    setPixmap(QPixmap::fromImage(dupImg));

}

void myLabel::mousePressEvent(QMouseEvent *)
{
    l->frm->setGeometry(-55,l->frm->y(),l->frm->width(),l->frm->height());
    l->setPixmap(QPixmap::fromImage(*::img));
    l->ptSet=0;
    if(name == "Boundary Fill (Black Boundary)")
    {
        l->mode=0;
        l->l.setText("Left Click to Paint By Selected Color! Right Click to Paint by Random Color!");
    }
    else if(name == "Draw Line By DDA")
    {
        l->mode=1;
        l->l.setText("Plot first pixel and start drawing line...");
    }
    else if(name == "Draw Circle")
    {
        l->mode=2;
        l->l.setText("Plot center first and start drawing circle...");
    }
    else if(name == "Draw Concentric Circles")
    {
        l->mode=3;
        l->l.setText("Plot center first and start drawing circle. Press Escape to Stop...");
    }
    else if(name == "Draw Line By Bresenhnams")
    {
        l->mode=4;
        l->l.setText("Plot first pixel and start drawing line...");
    }
    else if(name == "Eraser")
    {
        l->mode=5;
        l->l.setText("Click and move mouse over the portion to be erased...");
    }
    else if(name == "Color Selector")
    {
        l->mode=6;
        l->l.setText("Click and the corresponding color will be selected..");
    }
    else if(name == "Copy Polygon")
    {
        l->mode=7;
        l->polyNum=-1;
        l->l.setText("Click Polygon Edge to be Copied..");
    }
    else if(name == "Do Nothing!")
    {
        l->mode=999;
        l->l.setText("Idle Mode...");
        l->setPixmap(QPixmap::fromImage(*::img));
        l->que.clearAll();
    }
    else if(name == "Rotate Polygon")
    {
        l->mode=8;
        l->polyNum=-1;
        l->l.setText("Click the Polygon to be rotated...");
    }
    else if(name == "Import Image")
    {
        l->mode=10;
        l->l.setText("Enter Image Path...");
        QString path = QInputDialog::getText(this,"IMPORT IMAGE" ,"Enter Path");
        importImg = new QImage (path);
        float AspectRatio = (float)importImg->width() / importImg->height();
        QImage dupImg = *::img, PHOTO = importImg->scaled(l->ImageMagniFactor * AspectRatio, l->ImageMagniFactor);
        l->ImageMerge(dupImg, PHOTO, MAX_WIDTH/2, MAX_HEIGHT/2);
        l->setPixmap(QPixmap::fromImage(dupImg));
    }
    else if(name == "Flood Fill")
    {
        l->mode=11;
        l->l.setText("Left Click to Paint By Selected Color! Right Click to Paint by Random Color!");
    }
    else if(name == "Scan Line")
    {
        l->mode=12;        
        l->polyNum=-1;
        l->l.setText("Select polygon to be filled by scan line...");
    }
    else if(name == "Select Working Area!")
    {
        l->mode=13;
        l->polyNum=-1;
        l->l.setText("Use Drag n drop to select Working Area");
    }
    else if(name == "Effect: BnW [FULL]")
    {
        l->mode=14;
    }
    else if(name == "Effect: Brighten")
    {
        l->mode=15;
    }
    else if(name == "Effect: BnW [SELECTIVE]")
    {
        l->mode=16;
    }
    else if(name == "Effect: Anti - BnW [SELECTIVE]")
    {
        l->mode=17;
        backupImg = new QImage(::img->copy(0,0,MAX_WIDTH, MAX_HEIGHT));
        *::img = l->trans.blackNwhite(::img);
        l->setPixmap(QPixmap::fromImage(*::img));
    }
    else if(name == "File_Open")
    {
        FILE *f;
        QString str = l->tempName.text() + ".souImg";
        char *charStr = new char[str.length()+1];
        strcpy(charStr, str.toStdString().c_str());
        f = fopen(charStr,"r");
        if(f==NULL)
        {
            QMessageBox::information(this, "File Not Found", "Check Filename\nDon't include extension in name");
            return;
        }
        for(int i=0;i<MAX_WIDTH;i++)
        {
            for(int j=0;j<MAX_HEIGHT;j++)
            {
                int r = getc(f) - ENCRYPTION_ALPHA;
                r=r+(10*(getc(f)-ENCRYPTION_ALPHA));
                r=r+(100*(getc(f)-ENCRYPTION_ALPHA));
                int g = getc(f) -ENCRYPTION_ALPHA;
                g=g+(10*(getc(f)-ENCRYPTION_ALPHA));
                g=g+(100*(getc(f)-ENCRYPTION_ALPHA));
                int b = getc(f) -ENCRYPTION_ALPHA;
                b=b+(10*(getc(f)-ENCRYPTION_ALPHA));
                b=b+(100*(getc(f)-ENCRYPTION_ALPHA));
                ::img->setPixel(i,j,qRgb(r,g,b));
            }
            getc(f);
        }
        fclose(f);
        l->setPixmap(QPixmap::fromImage(*::img));
    }
    else if(name == "File_Save")
    {
        //SAVING TEMPLATE WITH ENCRYPTION
        FILE *f;
        QString str = l->tempName.text() + ".souImg";
        char *charStr = new char[str.length()+1];
        strcpy(charStr, str.toStdString().c_str());
        f = fopen(charStr,"w");
        for(int i=0;i<MAX_WIDTH;i++)
        {
            for(int j=0;j<MAX_HEIGHT;j++)
            {
                int r =  qRed(::img->pixel(i,j));
                int g =  qGreen(::img->pixel(i,j));
                int b =  qBlue(::img->pixel(i,j));
                int cnt=3, q;
                while(cnt)
                {
                    cnt--;
                    q=r%10;
                    r=r/10;
                    putc(ENCRYPTION_ALPHA+q,f);
                }
                cnt=3;
                while(cnt)
                {
                    cnt--;
                    q=g%10;
                    g=g/10;
                    putc(ENCRYPTION_ALPHA+q,f);
                }
                cnt=3;
                while(cnt)
                {
                    cnt--;
                    q=b%10;
                    b=b/10;
                    putc(ENCRYPTION_ALPHA+q,f);
                }
            }
            putc(10,f);
        }
        fclose(f);
    }
    else if(name == "COLOR")
    {
        l->r= qRed(img->pixel(0,0));
        l->g= qGreen(img->pixel(0,0));
        l->b= qBlue(img->pixel(0,0));
        l->palFront->img->fill(qRgb(l->r,l->g,l->b));
        l->palFront->setPixmap(QPixmap::fromImage(*l->palFront->img));
    }
    else if(name == "MAIN_COLOR")
    {
        l->setDisabled(true);
        l->Palt->show();
    }
    l->t5->start(1);
}

void Text::keyPressEvent(QKeyEvent *key)
{
    setText(text().append(key->key()));
    if(key->key() == Qt::Key_Backspace)
        setText((text().remove(text().length()-2, 2)));
    if(key->key() == Qt::Key_Return)
    {
        Analyse(text());
    }
}

void Text::Analyse(QString str)
{
    QStringList arguments = str.split(" ");
    if(arguments.isEmpty())         return;
    if(arguments[0].contains("CMD-DISABLE",Qt::CaseInsensitive))
        setEnabled(false);
    if(arguments[0].contains("EXIT",Qt::CaseInsensitive))
        exit(0);
    if(arguments[0].contains("LINE",Qt::CaseInsensitive))
    {
        if(arguments.count() == 5)
        {
            l->Undo.newMove(*img);
            a1=atof(arguments[1].toStdString().c_str());
            a2=atof(arguments[2].toStdString().c_str());
            a3=atof(arguments[3].toStdString().c_str());
            a4=atof(arguments[4].toStdString().c_str());
            setText("");
            l->drawL(1, a1,a2,a3,a4, *img, qRgb(0,0,0));
            l->setPixmap(QPixmap::fromImage(*img));
        }
        else
            setText("SYNTAX: LINE    X1    Y1    X2    Y2");
    }
    else if(arguments[0].contains("CIRCLE",Qt::CaseInsensitive))
    {
        if(arguments.count() == 4)
        {
            l->Undo.newMove(*img);
            a1=atof(arguments[1].toStdString().c_str());
            a2=atof(arguments[2].toStdString().c_str());
            a3=atof(arguments[3].toStdString().c_str());
            setText("");
            l->Bresenhnams(a1,a2,a3,*img, qRgb(0,0,0));
            l->setPixmap(QPixmap::fromImage(*img));
        }
        else
            setText("SYNTAX: CIRCLE         CENTRE_X         CENTER_Y         RADIUS_X");
    }
    else if(arguments[0].contains("MAGNIFY",Qt::CaseInsensitive))
    {
        if(arguments.count() == 0)
        {

            l->MagniFactor =2;
        }
        else
        {
            l->MagniFactor = arguments[1].toInt();
            setText("Magnification Level: 2" + QString::number(l->MagniFactor));
        }
    }
    else if(arguments[0].contains("UNDO",Qt::CaseInsensitive))
    {
        if(l->Undo.isAvailable())
        {
            *img = l->Undo.JustUndoIt();
            l->setPixmap(QPixmap::fromImage(*img));
        }
        else
            setText("No More Moves where done...");
    }
    else
         setText("COMMANDS: LINE, CIRCLE, MAGNIFY, UNDO, CMD-DISABLE, EXIT");
}

bool FillMethods::isBoundary(int x, int y)
{
    if(l->mode==0)  if (::img->pixel(x, y) == qRgb(0,0,0)) return 1;
    if(l->mode==11) if (!Threshold(::img->pixel(x, y) , l->rgb, COL_THRESHOLD)) return 1;
    if(l->mode==14) return 0;
    return 0;
}

void FillMethods::floodfillPoly()
{
    int count=l->que.getCount();
    static int tp=1;
    if(!count) return;
    do
    {
        if(!l->que.isEmpty() )
        {
            Point *p= l->que.del();
            if(l->mode!=14 && l->mode!=15)
            {
                if( img->pixel(p->x, p->y) != qRgb(l->r,l->g,l->b))
                {
                    img->setPixel(p->x, p->y, qRgb(l->r,l->g,l->b));
                }
            }
            else if(l->mode==14)
            {
                int avg = (qRed(img->pixel(p->x, p->y )) + qGreen(img->pixel(p->x, p->y ))+ qBlue(img->pixel(p->x, p->y )))/3;
                img->setPixel(p->x, p->y, qRgb(avg, avg, avg));
            }
            else if(l->mode==15)
            {
                int temp;
                img->setPixel(p->x, p->y, qRgb(((temp=qRed(img->pixel(p->x,p->y)))>250)?255:(temp+5),
                                               ((temp=qGreen(img->pixel(p->x,p->y)))>250)?255:(temp+5),
                                               ((temp=qBlue(img->pixel(p->x,p->y)))>250)?255:(temp+5)));
            }

            if(isValid(p->x-1,p->y) && l->field[p->x-1][p->y]!=1 && !isBoundary(p->x-1, p->y))
            {l->que.ins(p->x-1, p->y);
                l->field[p->x-1][p->y]=1;
            }

            if(isValid(p->x,p->y-1)&& l->field[p->x][p->y-1]!=1 && !isBoundary(p->x, p->y-1))
            {
                l->que.ins(p->x, p->y-1);
                l->field[p->x][p->y-1]=1;
            }

            if(isValid(p->x,p->y+1)&& l->field[p->x][p->y+1]!=1 && !isBoundary(p->x, p->y+1))
            {
                l->que.ins(p->x, p->y+1);
                l->field[p->x][p->y+1]=1;
            }
            if(isValid(p->x+1,p->y)&& l->field[p->x+1][p->y]!=1 && !isBoundary(p->x+1, p->y))
            {
                l->que.ins(p->x+1, p->y);
                l->field[p->x+1][p->y]=1;
            }
        }
        else
            count=1;
        count--;
    }while(count>0);

    if(l->que.isEmpty())
    {
        l->t1->stop();
        for(int i=0;i<MAX_WIDTH;i++)
            for(int j=0;j<MAX_HEIGHT;j++)
                l->field[i][j]=0;
        l->setPixmap(QPixmap::fromImage(*img));
    }
    tp = ((tp>11)?-1:tp)+1;
    if(!tp)
        l->setPixmap(QPixmap::fromImage(*img));
}

void drawingSheet::ImageMerge(QImage &target, QImage src, int x, int y)
{
    for(int i=x -src.width()/2,k=0; i<=x +src.width()/2 ;i++,k++)
        for(int j= y-src.height()/2,l=0; j<=y +src.height()/2 ;j++,l++)
        {
            if(isValid(i,j)) target.setPixel(i,j,src.pixel(k,l));
        }
}

myLabel::myLabel(int w, int h)
{
    img = new QImage(w,h, QImage::Format_RGB32);
    img->fill(qRgb(200,200,255));
    resize(w,h);
}

void drawingSheet::update()
{
    if(mode==0 || mode==11 || mode==12 || mode==14 || mode==15)
        floodfillPoly();
}

void drawingSheet::setToolPosition()
{
    if(frm->x()>10)
    {
        t2->stop();
        return;
    }
    frm->setGeometry(frm->x()+1,frm->y(),frm->width(),frm->height());
}

void drawingSheet::setStatusPosition()
{
    if(frmStatus->y()<MAX_HEIGHT - frmStatus->height()-10)
    {
        tempName.setEnabled(false);
        text1->setEnabled(true);
        text1->setText("CMD");
        t5->stop();
        return;
    }
    frmStatus->setGeometry(frmStatus->x(),frmStatus->y()-1,frmStatus->width(),frmStatus->height());
}

void drawingSheet::setHeaderPosition()
{
    if(frmHeader->y()>10)
    {
        tempName.setEnabled(true);
        text1->setEnabled(false);
        t6->stop();
        return;
    }
    frmHeader->setGeometry(frmHeader->x(),frmHeader->y()+1,frmHeader->width(),frmHeader->height());

}

void drawingSheet::setPalattePosition()
{
    if(frmPalatte->x()<MAX_WIDTH-frmPalatte->width()-10)
    {
        t7->stop();
        return;
    }
    frmPalatte->setGeometry(frmPalatte->x()-1,frmPalatte->y(),frmPalatte->width(),frmPalatte->height());

}
bool isValid(int x, int y)
{
    return ((x>=l->ValidXStart && x<=l->ValidXEnd) && (y>=l->ValidYStart && y<=l->ValidYEnd))? 1:0;
}

#include<dos.h>
#include<graphics.h>
#include<conio.h>
#include<stdio.h>
#define FALSE 0
#define TRUE  1
#define NULL  0

/* //////////////////////////// */
/* ��ͼ��ʽѡ���־ */
#define POINT        0
#define LINE         1
#define POLYGON      2
#define CLIPPINGAREA 3
/* /////////////////////////////////// */
/* ������ڼ������ķ����־ */
#define IN_IT        0
#define LEFT_ABOVE   9
#define LEFT         1
#define LEFT_BELOW   5
#define BELOW        4
#define RIGHT_BELOW  6
#define RIGHT        2
#define RIGHT_ABOVE  10
#define ABOVE        8



union REGS regs;     /* ���ı��� */
int  X_max,Y_max;     /* �����Χ���ֵ */
int x_Origin, y_Origin,x_Old,y_Old,x_New,y_New;/* ������ĳ�ʼ��,ǰһ��͵�ǰ������� */
int PointNum=0;    /* �ж�����Ƿ��ǵ�һ�ΰ��� */
int LineDrawFlag=FALSE;    /* ����껭�߱�־ */
int DrawFlag=0;                 /* ���岢��ʼ����ͼ��ʽΪ�� */
int Xleft,Xright,Ybottom,Ytop;   /* �����������ı��� */
int FillFlag=FALSE;               /* �Ƿ�����־ */
int k;       /* �������߶ε�б�� */
int ISDrawFlag=FALSE;            /* �Ƿ��߱�־ */
struct time t;                  /* ��ȡϵͳʱ��Ľṹ�� */
char C_time[50];                 /* ��ŵ�ǰʱ����ַ����� */
int oldtime1,oldtime2;            /* ����ǰһ��͵�ǰ��,�����ж��Ƿ�����һ��Ϊ����Ķ��� */
int msec;                        /* ����С���ƶ�ʱ���� */
char Run_time[50];              /* ��ų���������ʱ�� */
int Secs=0,Mins=0;                /* ���岢��ʼ�������Ѿ����е�ʱ�� */
static int writecolor=1;           /* ��ͻ�ӭ�ʵ���ɫ */
int Backcolor=GREEN;               /* ��ͼ������Χ��ɫ */
int UpFlag1=FALSE,DownFlag1=TRUE;  /* ��һ��С���ƶ����� */
int UpFlag2=TRUE,DownFlag2=FALSE;  /* �ڶ���С���ƶ����� */


typedef struct Point              /* �߶εĶ˵����ݽṹ */
{
  int x;
  int y;
  struct Point *next;
  int DirectionFlag;

}Point;


typedef struct SEG_Of_Line           /* �߶����ݽṹ */
{
   int x1;
   int y1;
   int x2;
   int y2;
  struct SEG_Of_Line *next;

}SEG_Of_Line;

Point *head;

/* //////////////////////////////////////////////////////////////////////// */
/* ɨ������������㷨������ */

push(int x,int y)            /* ��ջ���� */
{
    Point *temp;
    temp=(Point *)malloc(sizeof(Point));
    temp->x=x;
    temp->y=y;
    temp->next=head;
    head=temp;

}

pop(int *x,int *y)     /* ��ջ���� */
{
  *x=head->x;
  *y=head->y;
  head=head->next;

}

void ScanlineSeedFill(int Sx,int Sy,int boundarycolor,int fillcolor)
{                           /* ɨ����������� */

    int x_Now,y_Now,x0,y0,Rx,Lx,OldRx,OldLx;
    int RightFlag,LeftFlag; /* �Ƿ񵽴����ҷǱ߽���־ */
    int i;                  /* ѭ�����Ʊ��� */
    push(Sx,Sy);           /* ���ӵ���ջ */
    setcolor(fillcolor);
    while(head!=NULL)         /* ��ջ��Ϊ��ʱ,���������� */
    {
      pop(&x_Now,&y_Now);        /* ͷ����ջ */

      x0=x_Now+1;
      while(getpixel(x0,y_Now)!=boundarycolor)  /* �����ұߵķǱ߽��! */
          x0++;

      Rx=x0-1;
      OldRx=Rx;                     /* �������ҵ�,�Ա�����һ��ɨ����ʱ */

     x0=x_Now-1;

  while(getpixel(x0,y_Now)!=boundarycolor) /*  ������ߵķǱ߽�� */
       x0--;

    Lx=x0+1;

    line(Lx,y_Now,Rx,y_Now);          /* ��䵱ǰɨ���������� */

    OldLx=Lx;                  /* ���������,�Ա�����һ��ɨ����ʱ�� */

   y0=y_Now+1;               /* ɨ��������ɨ�� */

   RightFlag=FALSE;

  while(Lx<=Rx && RightFlag==FALSE)/* ������һ���ߵ�����ֵ,����¼ΪRx */
  {
   if(getpixel(Rx,y0)==boundarycolor)

       Rx--;
       else
   RightFlag=TRUE;                      /* ����ҵ�����ֵ,�������ұ�־Ϊ�� */
  }

   LeftFlag=FALSE;
  while(Lx<=Rx && LeftFlag==FALSE) /* ������һ���ߵ�����ֵ������¼ΪLx */
  {
   if(getpixel(Lx,y0)==boundarycolor)
          Lx++;
   else
          LeftFlag=TRUE;                          /* ����ҵ�����ֵ,���������־Ϊ�� */
  }

if(( RightFlag & LeftFlag )==TRUE &&
              (getpixel(Rx,y0)!=fillcolor || getpixel(Lx,y0)!=fillcolor))
   {                                /* ����ҵ�������ֵ������ֵ����ִ���������� */
     push(Rx,y0);           /* ����һ�����ϵ����ҵ���Ϊ���ӵ���ջ */
     for(i=Lx;i<Rx;i++)      /* ��LX��RX����ÿһ���ضε�����������Ϊ����ѹ��ջ */
        if(getpixel(i,y0)==boundarycolor && getpixel(i-1,y0)!=boundarycolor)
            push(i-1,y0);

    }


  y0=y_Now-1;              /* ɨ��������ɨ�� */

  Rx=OldRx;
  Lx=OldLx;

  RightFlag=FALSE;
  while(Lx<=Rx && RightFlag==FALSE)/* ������һ���ߵ�����ֵ,����¼ΪRX */
  {
   if(getpixel(Rx,y0)==boundarycolor)
        Rx--;
   else
        RightFlag=TRUE;             /* ����ҵ�����ֵ,�������ұ�־Ϊ�� */
  }

  LeftFlag=FALSE;
  while( Lx<=Rx &&LeftFlag==FALSE) /* ������һ���ߵ�����ֵ������¼ΪLX */
  {

    if(getpixel(Lx,y0)==boundarycolor)
         Lx++;
    else
         LeftFlag=TRUE;      /* ����ҵ�����ֵ,���������־Ϊ�� */
  }

  if(( RightFlag & LeftFlag )==TRUE &&
  (getpixel(Rx,y0)!=fillcolor || getpixel(Lx,y0)!=fillcolor))
   {                         /* ����ҵ�������ֵ������ֵ����ִ���������� */
       push(Rx,y0);            /* ����һ�����ϵ����ҵ���Ϊ���ӵ�ѹ��ջ */

       for(i=Lx;i<Rx;i++)      /* ��LX��RX����ÿһ���ضε�����������Ϊ����ѹ��ջ */
        if(getpixel(i,y0)==boundarycolor && getpixel(i-1,y0)!=boundarycolor)
           push(i-1,y0);

    }
 }
}
/* ɨ������������㷨������ */
/* ////////////////////////////////////////////////////////////////////////////// */


/* //////////////////////////////////////////////////////////////////////////////////// */
/* ����߶μ��ú����� */

SEG_Of_Line *AddSEG(SEG_Of_Line *head)               /* �����߶����� */
{
    SEG_Of_Line *p;
    p=(SEG_Of_Line *)malloc(sizeof(SEG_Of_Line));
    if(DrawFlag==POINT)
    {
      p->x1=x_New;
      p->y1=y_New;
      p->x2=x_New;
      p->y2=y_New;
    }
    if(DrawFlag==LINE || DrawFlag==POLYGON)
    {
       p->x1=x_Old;
       p->y1=y_Old;
       p->x2=x_New;
       p->y2=y_New;
    }
    p->next=head;
    head=p;
    return head;
}

Point *IsOnSEG(Point *p,int k)                /* ȷ���е����߶��� */
{
  if( k >= 0)               /* �߶�б�ʴ��ڻ������ */
  {
    if( getpixel(p->x,p->y) == WHITE )
      {}

    else if( getpixel(p->x+1,p->y+1) == WHITE )
      { p->x++;  p->y++;  }
    else if(getpixel(p->x+1,p->y) == WHITE)
        p->x++;
     else   p->y++;
      return p;
  }
  else              /* �߶�б��С���� */
  {
      if( getpixel(p->x,p->y) == WHITE )
      {}
      else if( getpixel(p->x+1,p->y-1) == WHITE )
         {
              p->x++; p->y--;
         }
      else if(  getpixel(p->x+1,p->y) == WHITE )
              p->x++;
         else p->y--;
         return p;
  }
}


Point *JudgeDirection(Point *q)             /* �ж��߶ζ˵�����ڼ��������λ�� */
{
   if(q->x < Xleft && q->y > Ytop)
         q->DirectionFlag = LEFT_ABOVE;

   if(q->x < Xleft && q->y <= Ytop && q->y >= Ybottom)
         q->DirectionFlag = LEFT;

   if(q->x < Xleft && q->y < Ybottom)
         q->DirectionFlag = LEFT_BELOW;

   if(q->x >= Xleft && q->x <= Xright && q->y < Ybottom)
         q->DirectionFlag = BELOW;

   if(q->x > Xright && q->y < Ybottom)
         q->DirectionFlag = RIGHT_BELOW;

   if(q->x > Xright && q->y >= Ybottom && q->y <= Ytop)
         q->DirectionFlag = RIGHT;

   if(q->x > Xright && q->y > Ytop)
         q->DirectionFlag = RIGHT_ABOVE;

   if(q->x >= Xleft && q->x <= Xright && q->y > Ytop)
         q->DirectionFlag = ABOVE;

   if(q->x >= Xleft && q->x <= Xright && q->y >= Ybottom && q->y <= Ytop)
         q->DirectionFlag = IN_IT;

     return q;
}


void  ClipingSEG(Point *p1,Point *p2)
{

      p1=JudgeDirection(p1);               /* ��P1��P2���Ϸ����־ */
      p2=JudgeDirection(p2);
    if( p1->DirectionFlag == 0 )             /* ���P1�ɼ� */
              return;

    else  if( (p1->DirectionFlag & p2->DirectionFlag) !=0)     /* ���P1P2 �����ɼ� */
         {
            setwritemode(OR_PUT);
            setcolor(getbkcolor());                            /* ɾ�� */
            line(p1->x,p1->y,p2->x,p2->y);
            setcolor(WHITE);
            setwritemode(XOR_PUT);
         }
         else                                         /* ���P1���ɼ�,��P1P2�пɼ����� */
MIDPoint:
       {

         Point *pm=(Point *)malloc(sizeof(Point));
         p1=JudgeDirection(p1);
         p2=JudgeDirection(p2);
         pm->x = (p1->x + p2->x)/2;
         pm->y = (p1->y + p2->y)/2;
         k = (p2->y - p1->y)*(p2->x - p1->x);         /* �߶�б�� */
         pm = IsOnSEG(pm,k);
         pm=JudgeDirection(pm);                         /* ȡP1P2�е�,���������Ϸ����־ */

         if( (pm->x - p2->x)*(pm->x - p2->x) <= 1 && (pm->y - p2->y)*(pm->y - p2->y) <= 1 )

         {                                      /* ���е���P2���㹻�ӽ�ʱ */
            Point *p=(Point *)malloc(sizeof(Point));
            p->x = pm->x;
            p->y = pm->y;
            p->DirectionFlag = pm->DirectionFlag;
            return;/*  p;                                //�����е� */
         }
        else if( (p1->DirectionFlag & pm->DirectionFlag) !=0)

           {
               setwritemode(OR_PUT);
               setlinestyle(0,1,3);
                setcolor(getbkcolor());
                line(p1->x,p1->y,pm->x,pm->y);

              setcolor(WHITE);
              setlinestyle(0,1,1);
              setwritemode(XOR_PUT);

              p1->x = pm->x;                         /* ���P1PM���ɼ�,��ɾ��,P1�ƶ���PM�� */
              p1->y = pm->y;
              p1->DirectionFlag = pm->DirectionFlag;
              free(pm);
              goto MIDPoint;                          /* ����P1P2�е� */
           }

           else                                       /* ���P1PM�ɼ�! */
           {
             p2->x = pm->x;
             p2->y = pm->y;                         /* P2�ƶ���PM�� , ����P1P2�е� */
             p2->DirectionFlag = pm->DirectionFlag;
             free(pm);
            goto MIDPoint;
           }
      }

}


SEG_Of_Line *Clipping(SEG_Of_Line *head)          /* �Լ����߶�����������߶ν��м��� */
{
    SEG_Of_Line *q;
   while(head!=NULL)                         /* ���߶�����Ϊ��,�������߶�δ���� ,�������¹��� */
   {

      Point *p1,*p2,*pold1,*pold2;
      p1=(Point *)malloc(sizeof(Point));
      p2=(Point *)malloc(sizeof(Point));
      pold1=(Point *)malloc(sizeof(Point));
      pold2=(Point *)malloc(sizeof(Point));

      q=head;
      p1->x=q->x1;
      p1->y=q->y1;
      p2->x=q->x2;
      p2->y=q->y2;

      pold1->x=p1->x;       /* ���� */
      pold1->y=p1->y;
      pold2->x=p2->x;
      pold2->y=p2->y;

       ClipingSEG(p1,p2);
       ClipingSEG(pold2,pold1);
       head=q->next;

   }
   free(q);
   return head;
}

/* ����߶μ��ú����� */
/* ////////////////////////////////////////////////////////////////////////////// */

/* ////////////////////////////////////////////////////////////////////////////// */
/* ���Ƽ��װ�ť���Ҹ������ϱ��� */

void DrawButton()
{


  /* ���Ƽ��װ�ť */
  rectangle(0,0,639,475);
  rectangle(10,115,80,135);
  rectangle(10,145,80,165);
  rectangle(10,175,80,195);
  rectangle(10,205,80,225);
  rectangle(10,235,80,255);
  rectangle(10,265,80,285);
  rectangle(10,295,80,315);


  setwritemode(OR_PUT);
  rectangle(100,35,540,475);
  setwritemode(XOR_PUT);

  setfillstyle(1, Backcolor);
  floodfill(2,2,WHITE);
  outtextxy(544,445,"RunningTime:");
  setcolor(BLACK);
  ellipse(325,18,0,360,170,15);             /* ������ӭ���洦�Ŀ�� */
  setfillstyle(1, BLACK);
  floodfill(320,18,BLACK);

  setfillstyle(1, BLUE);                    /* ����ť��ʼ������ɫ */
  floodfill(12,120,GREEN);
  floodfill(12,150,GREEN);
  floodfill(12,180,GREEN);
  floodfill(12,210,GREEN);
  floodfill(12,240,GREEN);
  floodfill(12,270,GREEN);
  floodfill(12,300,GREEN);

  setcolor(WHITE);

  outtextxy(28,122,"Point");                      /* ��ť���� */
  outtextxy(30,152,"LINE");
  outtextxy(20,182,"POLYGON");
  outtextxy(15,212,"ClipArea");
  outtextxy(30,242,"Clip");
  outtextxy(20,272,"CBoard");
  outtextxy(30,302,"Exit");


}

/* /////////////////////////////////////////////////////////////////// */
/* ������ */

void DrawChessBoard()
{
  int i,j;
  setcolor(WHITE);
 for(i=100;i<540;i+=55)
     for( j=35;j<475;j+=55)
       {
         line(i,j,i+55,j);
         line(i,j,i,j+55);
       }

      line(i,j,i-440,j);
      line(i,j,i,j-440);

 setwritemode(XOR_PUT);
  for(i=155;i<540;i+=55)
     for( j=90;j<475;j+=55)
      circle(i,j,2);
}


/* ///////////////////////////////////////////////////////// */
/* ������������ */

void Initgr()                  /* ��ʼ��ͼ��ģʽ */
{ 
  int gdriver=DETECT,gmode;
  registerbgidriver(EGAVGA_driver);
  initgraph(&gdriver,&gmode,"");
  X_max=getmaxx();
  Y_max=getmaxy();         /* �����Χ���ֵ */
}

int MouseInit(int Xp0,int Xp1,int Yp0,int Yp1)    /* ��ʼ����� */
{    /* ����Ĳ����������Χ�����Ͻ���������½����� */
  int retcode;
  regs.x.ax=0;
  int86(0x33,&regs,&regs);
retcode=regs.x.ax;
if(retcode==0) return 0;
   regs.x.ax=7;
regs.x.cx=Xp0;
regs.x.dx=Xp1;
int86(0x33,&regs,&regs);
regs.x.ax=3;
regs.x.cx=Yp0;
regs.x.dx=Yp1;
int86(0x33,&regs,&regs);
return retcode;
}


int MouseState(int *m_x,int *m_y,int *mstate)    /* ��ȡ���״̬��λ�� */
{  
   static int x0=10,y0=10,state=0;
   int xnew,ynew,ch;
   static float xxx=30;                           /* ��һ��С��ĳ�ʼ���� */
   static float yyy=430;                          /* �ڶ���С��ĳ�ʼ���� */
   do
   {
     /*
     if(kbhit())
     {
      ch=getch();
      if(ch==13)
       { *mstate=1;
         return -1;
       }
      else return ch;
      }  */
    regs.x.ax=3;
    int86(0x33,&regs,&regs);
    xnew=regs.x.cx;
    ynew=regs.x.dx;
    *mstate=regs.x.bx;

    gettime(&t);
    oldtime2=t.ti_sec;
    msec=t.ti_hund;
    if(msec%2==0)
    {

      if(DownFlag1==TRUE)                        /* ��һ��С���������˶�,���׺��������˶�,ѭ�� */
      {
       setcolor(GREEN);
       circle(570,xxx-1,8);
       setfillstyle(1, GREEN);
       floodfill(570,xxx-1,GREEN);

       setcolor(writecolor);
       circle(570,xxx,8);
       setfillstyle(1,writecolor);
       floodfill(570,xxx,writecolor);
       xxx+=1;
       if(xxx==430)
       {
          setcolor(GREEN);
          circle(570,xxx,8);
          setfillstyle(1, GREEN);
          floodfill(570,xxx,GREEN);

           DownFlag1=FALSE;
           UpFlag1=TRUE;
        }
      }
      if(UpFlag1==TRUE)
       {
         setcolor(GREEN);
         circle(570,xxx+1,8);
         setfillstyle(1,GREEN);
         floodfill(570,xxx+1,GREEN);

       setcolor(writecolor);
       circle(570,xxx,8);
       setfillstyle(1,writecolor);
       floodfill(570,xxx,writecolor);
       xxx-=1;
       if(xxx==40)
       {
          setcolor(GREEN);
          circle(570,xxx,8);
          setfillstyle(1, GREEN);
          floodfill(570,xxx,GREEN);
           DownFlag1=TRUE;
           UpFlag1=FALSE;
        }
       }

       if(DownFlag2==TRUE)                     /* �ڶ���С���������˶�,�������������˶�,ѭ�� */
      {
       setcolor(GREEN);
       circle(610,yyy-1,8);
       setfillstyle(1, GREEN);
       floodfill(610,yyy-1,GREEN);

       setcolor(writecolor);
       circle(610,yyy,8);
       setfillstyle(1, writecolor);
       floodfill(610,yyy,writecolor);
       yyy+=1;
       if(yyy==430)
       {
          setcolor(GREEN);
          circle(610,yyy,8);
          setfillstyle(1, GREEN);
          floodfill(610,yyy,GREEN);

           DownFlag2=FALSE;
           UpFlag2=TRUE;
        }
      }
      if(UpFlag2==TRUE)
       {
         setcolor(GREEN);
         circle(610,yyy+1,8);
         setfillstyle(1,GREEN);
         floodfill(610,yyy+1,GREEN);

       setcolor(writecolor);
       circle(610,yyy,8);
       setfillstyle(1,writecolor);
       floodfill(610,yyy,writecolor);
       yyy-=1;
       if(yyy==40)
       {
          setcolor(GREEN);
          circle(610,yyy,8);
          setfillstyle(1, GREEN);
          floodfill(610,yyy,GREEN);
           DownFlag2=TRUE;
           UpFlag2=FALSE;
        }
       }

       setcolor(WHITE);

     if(oldtime2!=oldtime1)
     {

      setcolor(writecolor);
      outtextxy(180,15,"Welcome to graphic precessing program!");
        writecolor++;
      while(writecolor==2||writecolor==3||writecolor==7||writecolor==8||writecolor==10)
                 writecolor++;                  /* ����ı件ӭ�ʺ�С����ɫ,��ȥ�����ֲ��ʺϵ���ɫ */

       if(writecolor==15) writecolor=1;

      setcolor(Backcolor);
      outtextxy(560,15,C_time);                  /* ��ʾϵͳ��ǰʱ�� */
      sprintf(C_time,"%2d:%02d:%02d",t.ti_hour, t.ti_min, t.ti_sec);
      setcolor(WHITE);
      outtextxy(560,15,C_time);
      if(oldtime1++==59) oldtime1=0;
      if(Secs++==60)
       {
        Secs=0;Mins++;
       }
      setcolor(Backcolor);                   /* ��ʾ������������ʱ�� */
      outtextxy(570,460,Run_time);
      sprintf(Run_time,"%02d:%02d",Mins,Secs);
      setcolor(WHITE);
      outtextxy(570,460,Run_time);
     }
    }
   }while(xnew==x0&&ynew==y0&&*mstate==state);
    state=*mstate;
    x0=xnew;
    y0=ynew;
    *m_x=xnew;
    *m_y=ynew;
    return -1;  
}


void DrawCursor(int x,int y)    /* ����굱ǰλ�û����ָ�� �� ��������ƶ���ֱ�� */
{
  int color;
  char str[50];

  line(x-6,y,x-2,y);            /* �����״ */
  line(x,y-6,x,y-3);
  line(x+2,y,x+6,y);
  line(x,y+3,x,y+6);

 if(LineDrawFlag==TRUE)
  {
    if(DrawFlag==CLIPPINGAREA)
    {

        setlinestyle(DOTTED_LINE ,1,1);     /* �������Ƽ������� */
        setcolor(GREEN);
         rectangle(x_Old,y_Old,x,y);

         setlinestyle(0,1,1);
         setcolor(WHITE);

     }
    else
  line(x_New,y_New,x,y);

  }
  color=getcolor();
  setcolor(Backcolor);
  outtextxy(10,15,str);
  sprintf(str,"(%d,%d)",x,y);  /* ��ʾ��굱ǰ������ֵ */
  setcolor(WHITE);
  outtextxy(10,15,str);
  setcolor(color);
}

/* ///////////////////////////////////////////////////////////////////////////// */
/* ///////////////////////////////////////////////////////////////////////////// */

main()
{
 int X,Y,m_state,y,a,b,i,j;
  static int fillcolor=1;  /* ���������ɫ */
  SEG_Of_Line *head;       /* �������߶ε�ͷ��� */

  head=NULL;

 Initgr();                    /* BGI��ʼ�� */

 DrawButton();
 setwritemode(XOR_PUT);       /* �趨����ģʽΪ���ģʽ */
 MouseInit(0,X_max,0,Y_max);  /* ��ʼ����� */
 a=X_max;b=Y_max;
 m_state=0;                   /* ��ʼ�����״̬Ϊ�ƶ�״̬ */
 DrawCursor(a,b);
 gettime(&t);                 /* ��ȡϵͳ��ǰʱ�� */
 sprintf(C_time,"%2d:%02d:%02d",t.ti_hour, t.ti_min, t.ti_sec);
 setcolor(WHITE);
 outtextxy(560,15,C_time);
 oldtime1=t.ti_sec;                /* ǰһ��ֵ */

 sprintf(Run_time,"%02d:%02d",Mins,Secs);
 outtextxy(544,445,"RunningTime:");
 outtextxy(570,460,Run_time);
 while(TRUE)                     /* ѭ��������Ϊ��,ʹ����һֱ�������¼� */
 {
   MouseState(&X,&Y,&m_state);     /* ��ȡ��굱ǰ״̬������ֵ */
   DrawCursor(a,b);                /* ͨ���������ģʽɾ��֮ǰ�����ָ�� */
   if(m_state==1)                   /* ������������ */
    {
     if(a>100&&a<540&&b>35&&b<475)                /* ����ڻ�ͼ������������ */
      {
        if(FillFlag==TRUE)
        {
         if( getpixel(a,b)==getbkcolor())
         {
           setwritemode(OR_PUT);
           ScanlineSeedFill(a,b,WHITE,fillcolor);  /* �����,�����̷��������� */
           setwritemode(XOR_PUT);
           fillcolor++;                             /* ����������̵���ɫ,�����һ����ɫ�ı�һ�� */
           if(fillcolor==15)  fillcolor=1;
           setcolor(writecolor);                    /* �����ٴ������ӭ��,�������������Ĵ��� */
           outtextxy(180,15,"Welcome to graphic precessing program!");
         }
        }
       if(ISDrawFlag==TRUE)
        {
        if(DrawFlag==POINT)                   /* ���ѡ�񻭵� */
        {
          x_New=a;
          y_New=b;
          putpixel(x_New,y_New,WHITE);
          head=AddSEG(head);
        }


        if(DrawFlag==LINE)                /* ���ѡ���� */
        {


         if(0==PointNum)               /* ����ǵ�һ�ε����� */
         {
          LineDrawFlag=TRUE;
           x_New=a;    y_New=b;
           x_Old=a;    y_Old=b;
           PointNum++;
         }

          else
          {
          x_Old=x_New;
          y_Old=y_New;
          x_New=a;
          y_New=b;

          LineDrawFlag=FALSE;     /* ��������껭�߱�־��Ϊ�� */
          PointNum=0;              /* ������������� */
          }

           setwritemode(OR_PUT);
           line(x_Old,y_Old,x_New,y_New);
           setwritemode(XOR_PUT);
           head=AddSEG(head);
        }

       if(DrawFlag==POLYGON)               /* ���ѡ�񻭶���� */
       {
         LineDrawFlag=TRUE;            /* ��������껭�߱�־��Ϊ�� */
         if(0==PointNum)               /* ����ǵ�һ�ε����� */
         {
          x_Origin=a; y_Origin=b;
          x_Old=a;    y_Old=b;
          x_New=a;    y_New=b;
         }
         else                         /* ������ǵ�һ�ε�������� */
         {
          x_Old=x_New;
          y_Old=y_New;
          x_New=a;
          y_New=b;
          }


         PointNum++;               /* ��¼�������������,�Ա�ȷ���Ƿ�Ҫ������껭�� */

         if((x_Origin-x_New) > -10 && (x_Origin-x_New) < 10 && (y_Origin-y_New) > -10
          && (y_Origin-y_New) < 10 &&  ((x_Origin-x_New)!=0 || (y_Origin-y_New)!=0))
           {                      /* ����߻������ʼ���㹻���ĵط� */

             LineDrawFlag=FALSE;     /* ��������껭�߱�־��Ϊ�� */
             PointNum=0;              /* ������������� */
             x_New=x_Origin; y_New=y_Origin;    /* ����ʼ�������ֵ������ǰ�� */
           }

           setwritemode(OR_PUT);
           line(x_Old,y_Old,x_New,y_New);
           setwritemode(XOR_PUT);

        }

        if(DrawFlag==CLIPPINGAREA)            /* ���ѡ�񻭼������� */
        {

            if(0==PointNum)               /* ����ǵ�һ�ε����� */
            {
              x_Old=a;    y_Old=b;
              x_New=a;
              y_New=b;
              LineDrawFlag=TRUE;
              PointNum++;
            }
           else                         /* ������ǵ�һ�ε�������� */
            {
              x_Old=x_New;
              y_Old=y_New;
              x_New=a;
              y_New=b;
              LineDrawFlag=FALSE;     /* ��������껭�߱�־��Ϊ�� */
              PointNum=0;
            }

           setwritemode(OR_PUT);            /* ���Ƽ������� */
           setlinestyle(DOTTED_LINE ,1,1);

           setcolor(GREEN);

           rectangle(x_Old,y_Old,x_New,y_New);

           setcolor(WHITE);
           setlinestyle(0,1,1);
           setwritemode(XOR_PUT);
           if(x_Old > x_New)                       /* ���������������,������������� */
            {   Xleft=x_New;  Xright=x_Old; }
           else
            {   Xleft=x_Old;  Xright=x_New; }

           if(y_Old > y_New)
            {  Ytop=y_Old;   Ybottom=y_New;}
           else
            {  Ytop=y_New;   Ybottom=y_Old;}

      }
      }

    }

     if(a>10&&a<80&&b>115&&b<135)         /* �������ѡ��ť */
        {
           if(ISDrawFlag==FALSE&&FillFlag==TRUE)
             {
               setbkcolor(BLACK);
               cleardevice();
               DrawButton();
               FillFlag=FALSE;
               setcolor(writecolor);    /* �û�ӭ�ʺ���������ͬʱ����,����һ������ʾ�����,��ͬ */
               outtextxy(180,15,"Welcome to graphic precessing program!");

              }

               setfillstyle(1, BLUE);              /* ���õ�ǰѡ�а�ť��ɫΪ��,����Ϊ��,��ͬ */
               floodfill(12,120,GREEN);
               floodfill(12,150,GREEN);
               floodfill(12,180,GREEN);
               floodfill(12,210,GREEN);
               floodfill(12,240,GREEN);
               floodfill(12,270,GREEN);
               floodfill(12,300,GREEN);
               setfillstyle(1, RED);
               floodfill(12,120,GREEN);
               setcolor(WHITE);

               outtextxy(28,122,"Point");
               outtextxy(30,152,"LINE");
               outtextxy(20,182,"POLYGON");
               outtextxy(15,212,"ClipArea");
               outtextxy(30,242,"Clip");
               outtextxy(20,272,"CBoard");
               outtextxy(30,302,"Exit");

           ISDrawFlag=TRUE;
           DrawFlag=POINT;
         }
     if(a>10&&a<80&&b>145&&b<165)          /* �������ѡ��ť */
        {
           if(ISDrawFlag==FALSE&&FillFlag==TRUE)
             {
               setbkcolor(BLACK);
               cleardevice();
               DrawButton();
               FillFlag=FALSE;
               setcolor(writecolor);
               outtextxy(180,15,"Welcome to graphic precessing program!");
              }

              setfillstyle(1, BLUE);
               floodfill(12,120,GREEN);
               floodfill(12,150,GREEN);
               floodfill(12,180,GREEN);
               floodfill(12,210,GREEN);
               floodfill(12,240,GREEN);
               floodfill(12,270,GREEN);
               floodfill(12,300,GREEN);
               setfillstyle(1, RED);
               floodfill(12,150,GREEN);
               setcolor(WHITE);

               outtextxy(28,122,"Point");
               outtextxy(30,152,"LINE");
               outtextxy(20,182,"POLYGON");
               outtextxy(15,212,"ClipArea");
               outtextxy(30,242,"Clip");
               outtextxy(20,272,"CBoard");
               outtextxy(30,302,"Exit");
           ISDrawFlag=TRUE;
           DrawFlag=LINE;
         }
     if(a>10&&a<80&&b>175&&b<195)            /* ����������ѡ��ť */
       {
         if(ISDrawFlag==FALSE&&FillFlag==TRUE)
             {
               setbkcolor(BLACK);
               cleardevice();
               DrawButton();
               FillFlag=FALSE;
               setcolor(writecolor);
               outtextxy(180,15,"Welcome to graphic precessing program!");
              }

              setfillstyle(1, BLUE);
               floodfill(12,120,GREEN);
               floodfill(12,150,GREEN);
               floodfill(12,180,GREEN);
               floodfill(12,210,GREEN);
               floodfill(12,240,GREEN);
               floodfill(12,270,GREEN);
               floodfill(12,300,GREEN);
               setfillstyle(1, RED);
               floodfill(12,180,GREEN);
               setcolor(WHITE);

               outtextxy(28,122,"Point");
               outtextxy(30,152,"LINE");
               outtextxy(20,182,"POLYGON");
               outtextxy(15,212,"ClipArea");
               outtextxy(30,242,"Clip");
               outtextxy(20,272,"CBoard");
               outtextxy(30,302,"Exit");
         ISDrawFlag=TRUE;
         DrawFlag=POLYGON;
       }
     if(a>10&&a<80&&b>205&&b<225)              /* �������������ѡ��ť */
       {
         if(ISDrawFlag==FALSE&&FillFlag==TRUE)
             {
               setbkcolor(BLACK);
               cleardevice();
               DrawButton();
               FillFlag=FALSE;
               setcolor(writecolor);
               outtextxy(180,15,"Welcome to graphic precessing program!");
              }

              setfillstyle(1, BLUE);
               floodfill(12,120,GREEN);
               floodfill(12,150,GREEN);
               floodfill(12,180,GREEN);
               floodfill(12,210,GREEN);
               floodfill(12,240,GREEN);
               floodfill(12,270,GREEN);
               floodfill(12,300,GREEN);
               setfillstyle(1, RED);
               floodfill(12,210,GREEN);
               setcolor(WHITE);

               outtextxy(28,122,"Point");
               outtextxy(30,152,"LINE");
               outtextxy(20,182,"POLYGON");
               outtextxy(15,212,"ClipArea");
               outtextxy(30,242,"Clip");
               outtextxy(20,272,"CBoard");
               outtextxy(30,302,"Exit");
         ISDrawFlag=TRUE;
         DrawFlag=CLIPPINGAREA;
       }
     if(a>10&&a<80&&b>235&&b<255)                /* ������ð�ť */
       {
          head=Clipping(head);
          setfillstyle(1, BLUE);
               floodfill(12,120,GREEN);
               floodfill(12,150,GREEN);
               floodfill(12,180,GREEN);
               floodfill(12,210,GREEN);
               floodfill(12,240,GREEN);
               floodfill(12,270,GREEN);
               floodfill(12,300,GREEN);
               setfillstyle(1, RED);
               floodfill(12,240,GREEN);
               setcolor(WHITE);

               outtextxy(28,122,"Point");
               outtextxy(30,152,"LINE");
               outtextxy(20,182,"POLYGON");
               outtextxy(15,212,"ClipArea");
               outtextxy(30,242,"Clip");
               outtextxy(20,272,"CBoard");
               outtextxy(30,302,"Exit");
       }
     if(a>10&&a<80&&b>265&&b<285)                 /* ������̰�ť */
     {
       setbkcolor(BLACK);
       cleardevice();
       ISDrawFlag=FALSE;
       DrawChessBoard();
       DrawButton();
       LineDrawFlag=FALSE;
       PointNum=0;
       head=NULL;
       setcolor(writecolor);
       outtextxy(180,15,"Welcome to graphic precessing program!");
       FillFlag=TRUE;
       setfillstyle(1, BLUE);
               floodfill(12,120,GREEN);
               floodfill(12,150,GREEN);
               floodfill(12,180,GREEN);
               floodfill(12,210,GREEN);
               floodfill(12,240,GREEN);
               floodfill(12,270,GREEN);
               floodfill(12,300,GREEN);
               setfillstyle(1, RED);
               floodfill(12,270,GREEN);
               setcolor(WHITE);

               outtextxy(28,122,"Point");
               outtextxy(30,152,"LINE");
               outtextxy(20,182,"POLYGON");
               outtextxy(15,212,"ClipArea");
               outtextxy(30,242,"Clip");
               outtextxy(20,272,"CBoard");
               outtextxy(30,302,"Exit");
     }
     if(a>10&&a<80&&b>295&&b<315)                 /* ����˳���ť */
       {
          setfillstyle(1, BLUE);
               floodfill(12,120,GREEN);
               floodfill(12,150,GREEN);
               floodfill(12,180,GREEN);
               floodfill(12,210,GREEN);
               floodfill(12,240,GREEN);
               floodfill(12,270,GREEN);
               floodfill(12,300,GREEN);
               setfillstyle(1, RED);
               floodfill(12,300,GREEN);
               setcolor(WHITE);

               outtextxy(28,122,"Point");
               outtextxy(30,152,"LINE");
               outtextxy(20,182,"POLYGON");
               outtextxy(15,212,"ClipArea");
               outtextxy(30,242,"Clip");
               outtextxy(20,272,"CBoard");
               outtextxy(30,302,"Exit");
          setcolor(BLACK);
          outtextxy(180,15,"Welcome to graphic precessing program!");   /* ������ӭ�� */
          setcolor(RED);
          outtextxy(240,15,"The program is exiting...");
          delay(100000);
          delay(100000);
          delay(100000);
          delay(100000);
          delay(100000);
          delay(100000);
          delay(100000);
          closegraph();
        }
   }

   if(m_state==2&&DrawFlag==POLYGON&&a>100&&a<540&&b>35&&b<475)
    {
      setwritemode(OR_PUT);                      /* �ڶ�������ģʽ��,���Ҽ�������� */
      ScanlineSeedFill(a,b,WHITE,fillcolor);
      setwritemode(XOR_PUT);

    }
   setcolor(WHITE);
   DrawCursor(X,Y);   /* �ڵ�ǰλ�û����ָ�� */
   a=X;
   b=Y;
}

}

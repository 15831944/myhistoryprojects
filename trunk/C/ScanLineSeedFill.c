#include<dos.h>
#include<graphics.h>
#include<conio.h>
#include<stdio.h>
#define FALSE 0
#define TRUE  1
#define NULL  0

union REGS regs;       /* ���ı��� */
int  X_max,Y_max;     /* �����Χ���ֵ */
int x_Origin, y_Origin,x_Old,y_Old,x_New,y_New;/* ������ĳ�ʼ��,ǰһ��͵�ǰ������� */
int PointNum=0;    /* �ж�����Ƿ��ǵ�һ�ΰ��� */
int LineDrawFlag=FALSE;    /* ����껭�߱�־ */

int FillFlag=FALSE;
                                                    /* ���ڳ����˳�ʱ����,�����ԭ��!! */

typedef struct Point
{  int x;
   int y;
   struct Point *next;
}Point;

Point *head1;     /* ջ�׽�� */
void push(int x,int y)            /* ��ջ���� */
{
    Point *temp;
    temp=(Point *)malloc(sizeof(Point));
    temp->x=x;
    temp->y=y;
    temp->next=head1;
    head1=temp;

}

void pop(int *x,int *y)     /* ��ջ���� */
{
 Point  *temp;
 temp=head1;
 *x=head1->x;
 *y=head1->y;
 head1=head1->next;
 free(temp);
}

void ScanlineSeedFill(int Sx,int Sy,int boundarycolor,int fillcolor)
{                           /* ɨ����������� */

    int x_Now,y_Now,x0,y0,Rx,Lx,OldRx,OldLx;
    int RightFlag,LeftFlag; /* �Ƿ񵽴����ҷǱ߽���־ */
    int i;                  /* ѭ�����Ʊ��� */
    push(Sx,Sy);           /* ���ӵ���ջ */

    while(head1!=NULL)         /* ��ջ��Ϊ��ʱ,���������� */
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
   setcolor(fillcolor);
   setwritemode(OR_PUT);
   line(Lx,y_Now,Rx,y_Now);          /* ��䵱ǰɨ���������� */
   setwritemode(XOR_PUT);
   OldLx=Lx;                  /* ���������,�Ա�����һ��ɨ����ʱ�� */

   y0=y_Now+1;               /* ɨ��������ɨ�� */

   RightFlag=FALSE;

  while(Lx<=Rx &&RightFlag==FALSE)/* ������һ���ߵ�����ֵ,����¼ΪRx */
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

if(RightFlag==TRUE&&LeftFlag==TRUE && getpixel(Rx,y0)!=boundarycolor &&
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

  if(RightFlag==TRUE&&LeftFlag==TRUE && getpixel(Rx,y0)!=boundarycolor &&
       (getpixel(Rx,y0)!=fillcolor || getpixel(Lx,y0)!=fillcolor))
   {                         /* ����ҵ�������ֵ������ֵ����ִ���������� */
       push(Rx,y0);            /* ����һ�����ϵ����ҵ���Ϊ���ӵ�ѹ��ջ */

       for(i=Lx;i<Rx;i++)      /* ��LX��RX����ÿһ���ضε�����������Ϊ����ѹ��ջ */
        if(getpixel(i,y0)==boundarycolor && getpixel(i-1,y0)!=boundarycolor)
            push(i-1,y0);
    }
 }        /* end while(head1!=null) */
}        /* end scanlineseedfill */


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

   do
   {
     if(kbhit())
     {
      ch=getch();
      if(ch==13)
       { *mstate=1;
         return -1;
       }
      else return ch;
      }
    regs.x.ax=3;
    int86(0x33,&regs,&regs);
    xnew=regs.x.cx;
    ynew=regs.x.dx;
    *mstate=regs.x.bx;
    }while(xnew==x0&&ynew==y0&&*mstate==state);
    state=*mstate;
    x0=xnew;
    y0=ynew;
    *m_x=xnew;
    *m_y=ynew;
    return -1;  
}


void DrawCursor(int x,int y)    /* ����굱ǰλ�û����ָ�� �� ��������ƶ���ֱ�� */
{  int color;
   char str[50];
  line(x-6,y,x-2,y);
  line(x,y-6,x,y-3);
  line(x+2,y,x+6,y);
  line(x,y+3,x,y+6);

 if(LineDrawFlag==TRUE)
  {

  line(x_New,y_New,x,y);

  }
  color=getcolor();
  setcolor(getbkcolor());
  outtextxy(10,20,str);
  sprintf(str,"(%d,%d)",x,y);  /* ��ʾ��굱ǰ������ֵ */
  setcolor(WHITE);
  outtextxy(10,20,str);
  setcolor(color);
}

main()
{
 int X,Y,m_state,y,a,b,i,j;


  Initgr();                        /* BGI��ʼ�� */

  setcolor(WHITE);

 setwritemode(XOR_PUT);      /* �趨����ģʽΪ���ģʽ */
 MouseInit(0,X_max,0,Y_max);  /* ��ʼ����� */
 a=X_max;b=Y_max;
 m_state=0;                   /* ��ʼ�����״̬Ϊ�ƶ�״̬ */
 DrawCursor(a,b);
 while(m_state!=2)             /* ���û�е���Ҽ� */
 {
   MouseState(&X,&Y,&m_state);     /* ��ȡ��굱ǰ״̬������ֵ */
   DrawCursor(a,b);                /* ͨ���������ģʽɾ��֮ǰ�����ָ�� */
   if(m_state==1)                   /* ������������ */
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
           line(x_Old,y_Old,x_New,y_New);       /*  ��ǰһ�㵽��ǰ�㻭�� */
           setwritemode(XOR_PUT);
    }

   DrawCursor(X,Y);   /* �ڵ�ǰλ�û����ָ�� */
   a=X;
   b=Y;
}

  DrawCursor(X,Y);
  ScanlineSeedFill(X,Y,WHITE,12);

 getch();
closegraph();
}

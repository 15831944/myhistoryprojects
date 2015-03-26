#include"stdio.h";
#include"graphics.h"
#include"math.h"
#include"dos.h"
#include"time.h"

/********************/
/*                  */
/*     ��άͼ��     */
/*                  */
/********************/
typedef struct Point      /*  �� */
{
   int x;
   int y;
}Point;

typedef struct Line       /*  �� */
{
    Point  A;/*���*/
    Point  B;/*�յ�*/
    int color; /*��ɫ*/
}Line;



typedef struct Yuan       /*  Բ */
{
   Point  o; /*Բ��*/
   int r;    /*�뾶*/
   int color;/*��ɫ*/
}Yuan;

typedef struct TuoYuan    /*�� Բ*/
{
   Point  o;/*����*/
   int a;   /*����*/
   int b;   /*����*/
   int color;/*��ɫ*/
}TuoYuan;

typedef struct Qiu        /* �� */
{
   Point  o;/*����*/
   int r;
   int color; /*��ɫ*/
}Qiu;

typedef struct Rect       /* ����*/
{
  Point LeftTop;
  Point RightDown;
  int color;
}Rect;


/********************************/
/****     ��ά�����任     ******/
/********************************/

/* ���ƽ�Ʊ任 */
void PinYi(int *x,int *y,int tx,int ty)
{
  *x = *x + tx;
  *y = *y + ty;

}



/* �����ת�任 */

void XuanZhuan(int *x,int *y,float q)
{
  float m ;
  float n;
  float Q;
  Q = (3.1415926/180)*q;
  m = (*x);
  n = (*y);
  *x = m * cos(Q) - n * sin(Q);
  *y = m * sin(Q) + n * cos(Q);
}

/* ����������ת�任 */

void XuanZhuan_RY(int m,int n,int *x,int *y,float q)/* ��(m,n)����ת*/
{

  PinYi(x,y,-m,-n);
    XuanZhuan(x,y,q);
  PinYi(x,y,m,n);
}

/* ��ķ����任 */
void FangSuo(int *x,int *y,float sx,float sy)
{
   *x =*x * sx;
   *y =*y * sy;
}

/* �������ķ����任 */
void FangSuo_RY(int m,int n,int *x,int *y,float sx,float sy) /*��(m,n)Ϊ�ο���*/
{
   PinYi(x,y,-m,-n);
     FangSuo(x,y,sx,sy);
   PinYi(x,y,m,n);
}



/* ��Ĵ��б任 */
void CuoQie(int *x,int *y,float q,int flag)
{
  float Q;
  Q = (3.1415926/180) * q;
  if(flag==0)/* yΪ������ */
  {
    *x = (float)*x + tan(Q)*((float)(*y));
    *y = *y;
  }
  else
  {
    *y = (float)*y + tan(Q)*((float)(*x));
    *x = *x;
  }

}

/* �������Ĵ��б任 */
void CuoQie_RY(int m,int n,int * x,int * y,float q,int flag) /* ��Ϊ(m,n)�ο���*/
{

    PinYi(x,y,-m,-n);
    CuoQie(x,y,q,flag);
    PinYi(x,y,m,n);

}



/***********************/
/*                     */
/*      ��άͼ��       */
/*                     */
/***********************/


typedef struct Point3D      /*  �� */
{
   int x;
   int y;
   int z;
}Point3D;

typedef struct Rect3D      /*  ������ */
{
   Point3D A;
   int a;
   int b;
   int color;
}Rect3D;


typedef struct CFT        /*������*/
{
  Point3D A;/*�����������*/
  int a;    /*    ��    */
  int b;    /*    ��    */
  int h;    /*    ��    */
}CFT;

typedef struct SLZ        /*����׶*/
{
  Point3D A;/*����׶���������*/
  int r;    /*    ����߳�    */
  int h;    /*    �ߡ���������*/
}SLZ;

typedef struct SLZU        /*������*/
{
  Point3D A;/*����׶���������*/
  int r;    /*    ����߳�    */
  int h;    /*    �ߡ���������*/
}SLZU;

typedef struct depth       /*��άʵ�������Ϣ*/
{
  char s;  /*ʵ������*/
  int  d;   /*ʵ�����*/
}Depth;


/********************************/
/****                      ******/
/****     ��ά�����任     ******/
/****                      ******/
/********************************/


/* ���ƽ�Ʊ任 */
void PinYi3D(int * x,int * y,int * z,int tx,int ty,int tz)
{
  *x = *x + tx;
  *y = *y + ty;
  *z = *z + tz;

}



/* ����ת�任 */

void XuanZhuan3D(int * x,int * y,int *z,float q,int flag)
{
  int m,n,t;
  float Q;
  m = (*x);
  n = (*y);
  t = (*z);
  Q = (3.1415926/180)*q;
  if(flag==1)   /* ��x����ת*/
  {
    *x = *x;
    *y = (float)n * cos(Q) - (float)t * sin(Q);
    *z = (float)n * sin(Q) + (float)t * cos(Q);
   }
   if(flag==2)  /* ��y����ת*/
   {
     *x = (float)m * cos(Q) + (float)t * sin(Q);
     *y = *y;
     *z = (float)(-m) * sin(Q) + (float)t * cos(Q);
   }
   if(flag==3)  /* ��z����ת*/
   {
      *x = (float)m * cos(Q) - (float)n * sin(Q);
      *y = (float)m * sin(Q) + (float)n * cos(Q);
      *z = *z;
   }

}

/* ��������ĵ����ת�任 */
void XuanZhuan3D_RY(int * x,int * y,int *z,float q,int i,int j,int k,int flag)
{

  /* if(flag==1)   /* ��ƽ����X���ҹ���(i,j,k)��ֱ�ߵ���ת*/ */
 /*  { */
  /*   PinYi3D(x,y,z,-i,-j,-k); */
 /*       XuanZhuan3D(x,y,z,q,flag); */
  /*   PinYi3D(x,y,z,i,j,k); */
  /*  } */
  /*  if(flag==2)  /* ��ƽ����Y���ҹ���(i,j,k)��ֱ�ߵ���ת*/ */
  /*  { */
  /*    PinYi3D(x,y,z,-i,-j,-k); */
  /*      XuanZhuan3D(x,y,z,q,flag); */
  /*    PinYi3D(x,y,z,i,j,k); */
  /*  } */
  /*  if(flag==3)  /* ��ƽ����Z���ҹ���(i,j,k)��ֱ�ߵ���ת*/ */
  /*  { */
   /*   PinYi3D(x,y,z,-i,-j,-k); */
   /*     XuanZhuan3D(x,y,z,q,flag); */
   /*  PinYi3D(x,y,z,i,j,k); */
  /* } */

    /* ��������޸� */

       PinYi3D(x,y,z,-i,-j,-k);
       XuanZhuan3D(x,y,z,q,flag);
       PinYi3D(x,y,z,i,j,k);


}

/* ������任 */
void FangSuo3D(int * x,int * y,int * z,float sx,float sy,float sz)
{
   *x =*x * sx;
   *y =*y * sy;
   *z =*z * sz;
}

/* �������ķ����任 */
void FangSuo3D_RY(int m,int n,int t,int * x,int * y,int *z,float sx,float sy,float sz) /*��(m,n,t)Ϊ�ο���*/
{
   PinYi3D(x,y,z,-m,-n,-t);
   FangSuo3D(x,y,z,sx,sy,sz);
   PinYi3D(x,y,z,m,n,t);
}


/************************/
/*                      */
/* ��ά����ϵ��ƽ�Ʊ任 */
/*                      */
/************************/

void PingYi3D_ZBX(int * x,int * y,int *z,int tx,int ty,int tz)
{
   *x = *x - tx;
   *y = *y - ty;
   *z = *z - tz;
}


/************************/
/*                      */
/* ��ά����ϵ����ת�任 */
/*                      */
/************************/

void XuanZhuan3D_ZBX(int * x,int * y,int *z,float q,int flag)
{
  /* int m,n,t; */
  /* float Q; */
 /*  m = (*x); */
 /*  n = (*y); */
  /* t = (*z); */
 /*  Q = (3.1415926/180 )*q; */

   XuanZhuan3D(x,y,z,-q,flag);


  /* if(flag==1)   /* x,y�����ת*/ */
  /* {                                 / */
  /*   *x = (float)m * cos(Q) + (float)n * sin(Q); */
  /*   *y = (float)(-m) * sin(Q) + (float)n * cos(Q); */
  /*   *z = *z; */
  /*  } */
  /*  if(flag==2)  /* y,z������ת*/ */
  /*  { */
  /*    *x = *x; */
  /*    *y = (float)n * cos(Q) + (float)t * sin(Q); */
  /*    *z = (float)(-n) * sin(Q) + (float)t * cos(Q); */
  /*  } */
  /*  if(flag==3)  /* z,x������ת*/ */
  /*  { */
  /*     *x = (float)m * cos(Q) - (float)t * sin(Q); */
  /*     *y = *y; */
  /*     *z = (float)m * sin(Q) + (float)t * cos(Q); */
  /*  } */

}


/********************************/
/*                              */
/* ��ά�ռ�㵽��άƽ����ͶӰ */
/*                              */
/********************************/
void Point3D_2D(int *x,int *y,int *z,Point3D B,float qx,float qy,float qz)/*ͶӰ����Ϊ(qx,qy,qz)*/
{
      int t,i,j,k;
      i = B.x; j = B.y; k = B.z;


      FangSuo3D_RY(i,j,k,x,y,z,0.4,0.4,0.4);     /* ����ڲο���B���б������� */

      PingYi3D_ZBX(x,y,z,i,j,k);                 /* ����ϵƽ�� */
      XuanZhuan3D(x,y,z,-qx,3);                  /*  XuanZhuan3D_ZBX(x,y,z,qx,3);//��Z����ת */
      XuanZhuan3D(x,y,z,-qz,2);                  /*  XuanZhuan3D_ZBX(x,y,z,qz,2);//��Y����ת */
      PingYi3D_ZBX(x,y,z,-i,-j,-k);

      t = *x; *x = *y; *y = t;                   /* 3D��2Dת��ʱ��XY������任 */

}


/***********************/
/*   ֱ�ߵ�DDA �㷨    */
/***********************/

void LineDDA(Point A,Point B,int color)
{
   int x0,y0,x1,y1,x;
   float dy,dx,y,m;

   if(A.x<B.x)
   {
      x0 = A.x;
      y0 = A.y;
      x1 = B.x;
      y1 = B.y;

      dx = x1 - x0;
      dy = y1 - y0;
      m = dy/dx;
      y = y0;
      for(x = x0;x <= x1;x++)
      {
        putpixel(x,(int)(y + 0.5),color);
        y = y + m;
      }
   }

  if(A.x>B.x)
  {
      x0 = B.x;
      y0 = B.y;
      x1 = A.x;
      y1 = A.y;

      dx = x1 - x0;
      dy = y1 - y0;
      m = dy/dx;
      y = y0;
      for(x = x0;x <= x1;x++)
      {
        putpixel(x,(int)(y + 0.5),color);
        y = y + m;
      }
  }
  if(A.x==B.x)
  {
    if(A.y<=B.y)
     {
        x0 = A.x;
        y0 = A.y;
        while(y0<=B.y)
        {
           putpixel(x0,y0,color);
           y0 = y0 +1;
        }
     }
     else
     {
        x0 = B.x;
        y0 = B.y;
        while(y0<=A.y)
        {
           putpixel(x0,y0,color);
           y0 = y0 +1;
        }
     }
  }

}

/***********************/
/*   ֱ�ߵ��е��㷨    */
/***********************/

void MidPointLine(Point A,Point B,int color)
{
  int dx,dy,incrE,incrNE,d,x,y,x0,y0,x1,y1;

   if(A.x<B.x)
   {
    if((float)(B.y - A.y)/(B.x - A.x)<1.0)
    {
      x0 = A.x;
      y0 = A.y;
      x1 = B.x;
      y1 = B.y;

      dx = x1 - x0;
      dy = y1 - y0;
      d = dx - 2 * dy;
      incrE = -2 * dy;
      incrNE = 2 * (dx - dy);
      x = x0,y = y0;
      putpixel(x,y,color);
      while(x <= x1)
       {
        if(d > 0)
        d = d + incrE;
        else
         {
          d = d + incrNE;
          y++;
       
          }
        x++;
       putpixel(x,y,color);
       }
      }
     if((float)(B.y - A.y)/(B.x - A.x)>1.0)
     {
        x0 =A.y;
        y0 =A.x;
        x1 =B.y;
        y1 =B.x;

      dx = x1 - x0;
      dy = y1 - y0;
      d = dx - 2 * dy;
      incrE = -2 * dy;
      incrNE = 2 * (dx - dy);
      x = x0,y = y0;
      putpixel(y,x,color);
      while(x <= x1)
       {
        if(d > 0)
        d = d + incrE;
        else
         {
          d = d + incrNE;
          y++;
       
          }
        x++;
       putpixel(y,x,color);
       }

     }
   }

  if(A.x>B.x)
  {
      if((float)(B.y - A.y)/(A.x - B.x)<1.0)
    {
      x0 = A.x;
      y0 = A.y;
      x1 = B.x;
      y1 = B.y;

      dx = x0 - x1;
      dy = y1 - y0;
      d = dx - 2 * dy;
      incrE = -2 * dy;
      incrNE = 2 * (dx - dy);
      x = x0,y = y0;
      putpixel(x,y,color);
      while(x >= x1)
       {
        if(d > 0)
        d = d + incrE;
        else
         {
          d = d + incrNE;
          y++;
       
          }
        x--;
       putpixel(x,y,color);
       }
      }
     if((float)(B.y - A.y)/(A.x - B.x)>1.0)
     {
        x0 =A.y;
        y0 =A.x;
        x1 =B.y;
        y1 =B.x;

      dx = x1 - x0;
      dy = y0 - y1;
      d = dx - 2 * dy;
      incrE = -2 * dy;
      incrNE = 2 * (dx - dy);
      x = x0,y = y0;
      putpixel(y,x,color);
      while(x <= x1)
       {
        if(d > 0)
        d = d + incrE;
        else
         {
          d = d + incrNE;
          y--;
       
          }
        x++;
       putpixel(y,x,color);
       }

     }
  }
  if(A.x==B.x)
  {
    if(A.y<=B.y)
     {
        x0 = A.x;
        y0 = A.y;
        while(y0<=B.y)
        {
           putpixel(x0,y0,color);
           y0 = y0 +1;
        }
     }
     else
     {
        x0 = B.x;
        y0 = B.y;
        while(y0<=A.y)
        {
           putpixel(x0,y0,color);
           y0 = y0 +1;
        }
     }
  }


}



/*********************/
/*   Բ���е��㷨    */
/*********************/


/* Բ��8�ԳƵ����ʾ */
void CirclePoints(int x0,int y0,int x,int y,int color)/* Բ��Ϊ��x0,y0);*/
{
  putpixel(x,y,color);                 /*(x,  y) */
  putpixel(x0-y0+y,y0-x0+x,color);     /*(y,  x) */
  putpixel(x0+y0-y,y0-x0+x,color);     /*(-y, x) */
  putpixel(2*x0-x,y,color);            /*(-x, y) */
  putpixel(x0-y0+y,y0+x0-x,color);     /*(y, -x) */
  putpixel(x,2*y0-y,color);            /*(x, -y) */
  putpixel(2*x0-x,2*y0-y,color);       /*(-x,-y) */
  putpixel(x0+y0-y,y0+x0-x,color);     /*(-y,-x) */
}


/* �㷨 1��Բ���е��㷨 */
void MidPointCircle_1(Point Q,int radius,int color)/* Բ��Ϊ Q */
{
  int x,y,x0,y0;
  float d;

  x0 = Q.x;
  y0 = Q.y;

  x = x0;
  y = y0+radius;
  d = 5.0/4.0 - radius;
  CirclePoints(x0,y0,x,y,color);
  while((y-y0)>(x-x0))
  {
    if(d<=0)
      d = d + 2.0*(x-x0) + 3;
    else
      {
        d = d + 2.0*((x-x0)-(y-y0)) + 5;
        y--;
      }
    x++;
    CirclePoints(x0,y0,x,y,color);
  }

}

/* �㷨 2 �������˸���������е��㷨 */

void MidPointCircle_2(Point Q,int radius,int color)/* Բ��Ϊ��x0,y0);*/
{
  int x,y,d,x0,y0;

  x0 = Q.x;
  y0 = Q.y;

  x = x0;
  y = y0 + radius;
  d = 5 - 4*radius;
  CirclePoints(x0,y0,x,y,color);
  while((y-y0)>(x-x0))
  {
   if(d<=0)
      d = d + 8*(x-x0) + 12;
    else
      {
        d = d + 8*((x-x0)-(y-y0)) + 20;
        y--;
      }
    x++;
    CirclePoints(x0,y0,x,y,color);
  }
}

/* �㷨 3 �������˳˷�������е��㷨 */
void MidPointCircle_3(Point Q,int radius,int color)/* Բ��Ϊ��x0,y0);*/
{
   int x,y,d,deltaE,deltaSE,x0,y0;

   x0 = Q.x;
   y0 = Q.y;

   x = x0;
   y = y0 + radius;
   d = 5 - 4*radius;
   deltaE = 12;
   deltaSE = 20 - 8*radius;
   CirclePoints(x0,y0,x,y,color);
   while((y-y0)>(x-x0))
   {
      if(d<=0)
      {
        d = d + deltaE;
        deltaSE=deltaSE + 8;
      }
      else
      {
         d = d + deltaSE;
         deltaSE=deltaSE + 16;
         y--;
      }
      deltaE=deltaE + 8;
      x++;
      CirclePoints(x0,y0,x,y,color);
   }

}



/***********************/
/*   ��Բ���е��㷨    */
/***********************/


/* ��Բ��4�ԳƵ����ʾ */
void EllipsePoints(int x0,int y0,int x,int y,int color) /*��Բ������Ϊ:(x0,y0)*/
{
  putpixel(x,y,color);              /*(x,  y) */
  putpixel(2*x0-x,y,color);         /*(-x, y) */
  putpixel(2*x0-x,2*y0-y,color);    /*(-x,-y) */
  putpixel(x,2*y0-y,color);         /*(x, -y) */
}


/* ��Բ���е��㷨 */
void MidPointEllipse(Point Q,int a,int b,int color)
{
  int x,y,xP,yP,x0,y0;
  float d,squrarea,squrareb;
   x0 = Q.x;
   y0 = Q.y;


  squrarea = a * a;
  squrareb = b * b;

  /*����ֽ��P*/
  xP = (int)(0.5 + squrarea/sqrt(squrarea + squrareb));
  yP = (int)(0.5 + squrareb/sqrt(squrarea + squrareb));

 /* �����ϰ벿��Բ*/
  x = x0;
  y = y0 + b;
  d = 4 * (squrareb - squrarea * b) + squrarea;
  EllipsePoints(x0,y0,x,y,color);
  while((x - x0)<xP)
  {
    if(d<=0)
       d = d + 4 * squrareb * (2 * (x - x0) + 3);
    else
       {
         d = d + 4 * squrareb * (2 * (x - x0) + 3)-8 * squrarea * ((y - y0) - 1);
         y--;
       }
    x++;
    EllipsePoints(x0,y0,x,y,color);
  }

  /* �����°벿��Բ*/
  x = x0 + a;
  y = y0;
  d = 4 * (squrarea - squrareb * a) + squrareb;
  EllipsePoints(x0,y0,x,y,color);
  while((y-y0) < yP)
  {
    if(d<=0)
       d = d + 4 * squrarea * (2 * (y - y0) + 3);
    else
       {
         d = d + 4 * squrarea * (2 * (y - y0) + 3)-8 * squrareb * ((x - x0) - 1);
         x--;
       }
     y++;
   EllipsePoints(x0,y0,x,y,color);
  }
}


/***********************/
/*     ���ε��㷨      */
/***********************/
void Rectangle(Point LeftTop,Point RightDown,int color)
{
  int x1,y1,x2,y2,x,y;
  x1 = LeftTop.x;
  x2 = RightDown.x;
  y1 = LeftTop.y;
  y2 = RightDown.y;
  x = x1;
  while(x<=x2)
  {
    putpixel(x,y1,color);
    putpixel(x,y2,color);
    x++;
  }

  y = y1;
  while(y<=y2)
  {
     putpixel(x1,y,color);
     putpixel(x2,y,color);
     y++;
  }

}


/*************************/
/******   ������     *****/
/*************************/

void QiuEC(Point o,int r,int color) /*  ����Ϊo(x,y) */
{
  float i,X,Y,ra,rb;
  int k,x,y;
  x = o.x;
  y = o.y;
  MidPointEllipse(o,r,15,color);
  MidPointCircle_1(o,r,color);
  for(i=1;i<=(r/5);i++)
  {

     MidPointEllipse(o,5*i,r,color);

  }
  k = (int)(r/4);
  for(i=1;i<4;i++)
  {
     o.y = y-(4-i)*k;
     MidPointEllipse(o,sqrt(2*r*(k*i)-k*i*k*i),i*4,color);
     o.y = y+(4-i)*k;
     MidPointEllipse(o,sqrt(2*r*(k*i)-k*i*k*i),i*4,color);
  }
}





/*****************************/
/**                         **/
/**     ���ε���ת          **/
/**                         **/
/*****************************/
void Rectangle_XuanZhuan(Rect R,float q)
{
  int x1,y1,x2,y2,x,y,color,X,Y;

  color = R.color;

  x1 = R.LeftTop.x;
  y1 = R.LeftTop.y;

  x2 = R.RightDown.x;
  y2 = R.RightDown.y;

  x = x1;
  while(x<=x2)
  {
    X = x;
    Y = y1;
    XuanZhuan(&X,&Y,q);
       putpixel(X,Y,color);

    X = x;
    Y = y2;
    XuanZhuan(&X,&Y,q);
       putpixel(X,Y,color);

    x++;
  }

  y = y1;
  while(y<=y2)
  {
     X = x1;
     Y = y;
     XuanZhuan(&X,&Y,q);
       putpixel(X,Y,color);

     X = x2;
     Y = y;
     XuanZhuan(&X,&Y,q);
       putpixel(X,Y,color);
     y++;
  }

}




/******************************/
/*                            */
/*       ��Բ��ƽ��           */
/*                            */
/******************************/

/*ƽ�ƺ���Բ��4�ԳƵ����ʾ*/
void EllipsePoints_PinYi(int x0,int y0,int x,int y,int color,int tx,int ty) /*��Բ������Ϊ:(x0,y0)*/
{
  int X,Y;
  X = x;
  Y = y;
  PinYi(&X,&Y,tx,ty);
     putpixel(X,Y,color);      /*(x,  y)*/

  X = 2*x0-x;
  Y = y;
  PinYi(&X,&Y,tx,ty);
     putpixel(X,Y,color);      /*(-x, y)*/

  X = 2*x0-x;
  Y = 2*y0-y;
  PinYi(&X,&Y,tx,ty);
     putpixel(X,Y,color);      /*(-x,-y)*/

  X = x;
  Y = 2*y0-y;
  PinYi(&X,&Y,tx,ty);
     putpixel(X,Y,color);       /*(x, -y)*/
}

/* ��Բ��ƽ��*/

void MidPointEllipse_PinYi(TuoYuan E,int tx,int ty)
{
  int x,y,xP,yP,x0,y0,a,b,color;
  float d,squrarea,squrareb;

   a = E.a;
   b = E.b;
   color = E.color;
   x0 = E.o.x;
   y0 = E.o.y;


  squrarea = a * a;
  squrareb = b * b;

  /*����ֽ��P*/
  xP = (int)(0.5 + squrarea/sqrt(squrarea + squrareb));
  yP = (int)(0.5 + squrareb/sqrt(squrarea + squrareb));

 /* �����ϰ벿��Բ*/
  x = x0;
  y = y0 + b;
  d = 4 * (squrareb - squrarea * b) + squrarea;

  EllipsePoints_PinYi(x0,y0,x,y,color,tx,ty);

  while((x - x0)<xP)
  {
    if(d<=0)
       d = d + 4 * squrareb * (2 * (x - x0) + 3);
    else
       {
         d = d + 4 * squrareb * (2 * (x - x0) + 3)-8 * squrarea * ((y - y0) - 1);
         y--;
       }
    x++;

    EllipsePoints_PinYi(x0,y0,x,y,color,tx,ty);

  }

  /* �����°벿��Բ*/
  x = x0 + a;
  y = y0;
  d = 4 * (squrarea - squrareb * a) + squrareb;

  EllipsePoints_PinYi(x0,y0,x,y,color,tx,ty);

  while((y-y0) < yP)
  {
    if(d<=0)
       d = d + 4 * squrarea * (2 * (y - y0) + 3);
    else
       {
         d = d + 4 * squrarea * (2 * (y - y0) + 3)-8 * squrareb * ((x - x0) - 1);
         x--;
       }
     y++;

   EllipsePoints_PinYi(x0,y0,x,y,color,tx,ty);

  }
}

/******************************/
/*                            */
/*       ��Բ����ת           */
/*                            */
/******************************/

/* ��Բ��ת���4�ԳƵ����ʾ */
void EllipsePoints_XuanZhuan(int m,int n,int x0,int y0,int x,int y,int color,float q) /*��Բ������Ϊ:(x0,y0),�Ƶ�(m,n)��ת*/
{
  int X,Y;
  X = x;
  Y = y;
  XuanZhuan_RY(m,n,&X,&Y,q);
     putpixel(X,Y,color);               /*(x,  y) */


  X = 2*x0-x;
  Y = y;
  XuanZhuan_RY(m,n,&X,&Y,q);
     putpixel(X,Y,color);              /*(-x, y) */


  X = 2*x0-x;
  Y = 2*y0-y;
  XuanZhuan_RY(m,n,&X,&Y,q);
     putpixel(X,Y,color);             /*(-x,-y) */

  X = x;
  Y = 2*y0-y;
  XuanZhuan_RY(m,n,&X,&Y,q);
     putpixel(X,Y,color);             /*(x, -y) */

}

/*        ��Բ����ת    */

MidPointEllipse_XuanZhuan(TuoYuan E,Point A,float q) /*��A����ת*/
{
  int x,y,xP,yP,x0,y0,a,b,color,m,n;
  float  d,squrarea,squrareb;
  m = A.x;
  n = A.y;
  x0 = E.o.x;
  y0 = E.o.y;
  a = E.a;
  b = E.b;
  color  = E.color;


  squrarea = a * a;
  squrareb = b * b;

  /*����ֽ��P*/
  xP = (int)(0.5 + squrarea/sqrt(squrarea + squrareb));
  yP = (int)(0.5 + squrareb/sqrt(squrarea + squrareb));

 /* �����ϰ벿��Բ*/
  x = x0;
  y = y0 + b;
  d = 4 * (squrareb - squrarea * b) + squrarea;

  EllipsePoints_XuanZhuan(m,n,x0,y0,x,y,color,q);

  while((x - x0)<=xP)
  {
    if(d<=0)
       d = d + 4 * squrareb * (2 * (x - x0) + 3);
    else
       {
         d = d + 4 * squrareb * (2 * (x - x0) + 3)-8 * squrarea * ((y - y0) - 1);
         y--;
       }
    x++;

  EllipsePoints_XuanZhuan(m,n,x0,y0,x,y,color,q);

  }
  /* �����°벿��Բ*/
  x = x0 + a;
  y = y0;
  d = 4 * (squrarea - squrareb * a) + squrareb;

 EllipsePoints_XuanZhuan(m,n,x0,y0,x,y,color,q);

  while((y-y0) < yP)
  {
    if(d<=0)
       d = d + 4 * squrarea * (2 * (y - y0) + 3);
    else
       {
         d = d + 4 * squrarea * (2 * (y - y0) + 3)-8 * squrareb * ((x - x0) - 1);
         x--;
       }
     y++;

  EllipsePoints_XuanZhuan(m,n,x0,y0,x,y,color,q);
  }
}


/******************************/
/*                            */
/*       ��Բ�ķ���           */
/*                            */
/******************************/

/* ��Բ�������4�ԳƵ����ʾ */
void EllipsePoints_FangSuo(int m,int n,int x0,int y0,int x,int y,int color,float sx,float sy) /*��Բ������Ϊ:(x0,y0),��(m,n)Ϊ�ο������*/
{
  int X,Y;
  X = x;
  Y = y;
  FangSuo_RY(m,n,&X,&Y,sx,sy);
     putpixel(X,Y,color);               /*(x,  y) */


  X = 2*x0-x;
  Y = y;
  FangSuo_RY(m,n,&X,&Y,sx,sy);
     putpixel(X,Y,color);              /*(-x, y) */


  X = 2*x0-x;
  Y = 2*y0-y;
  FangSuo_RY(m,n,&X,&Y,sx,sy);
     putpixel(X,Y,color);             /*(-x,-y) */

  X = x;
  Y = 2*y0-y;
  FangSuo_RY(m,n,&X,&Y,sx,sy);
     putpixel(X,Y,color);             /*(x, -y) */

}

/*        ��Բ�ķ���     */

void MidPointEllipse_FangSuo(TuoYuan E,Point A,float sx,float sy) /*��A��Ϊ�ο������*/
{
  int x,y,xP,yP,x0,y0,a,b,color,m,n;
  float  d,squrarea,squrareb;
  m = A.x;
  n = A.y;
  x0 = E.o.x;
  y0 = E.o.y;
  a = E.a;
  b = E.b;
  color  = E.color;


  squrarea = a * a;
  squrareb = b * b;

  /*����ֽ��P*/
  xP = (int)(0.5 + squrarea/sqrt(squrarea + squrareb));
  yP = (int)(0.5 + squrareb/sqrt(squrarea + squrareb));

 /* �����ϰ벿��Բ*/
  x = x0;
  y = y0 + b;
  d = 4 * (squrareb - squrarea * b) + squrarea;

  EllipsePoints_FangSuo(m,n,x0,y0,x,y,color,sx,sy);

  while((x - x0)<=xP)
  {
    if(d<=0)
       d = d + 4 * squrareb * (2 * (x - x0) + 3);
    else
       {
         d = d + 4 * squrareb * (2 * (x - x0) + 3)-8 * squrarea * ((y - y0) - 1);
         y--;
       }
    x++;

  EllipsePoints_FangSuo(m,n,x0,y0,x,y,color,sx,sy);

  }
  /* �����°벿��Բ*/
  x = x0 + a;
  y = y0;
  d = 4 * (squrarea - squrareb * a) + squrareb;

 EllipsePoints_FangSuo(m,n,x0,y0,x,y,color,sx,sy);

  while((y-y0) < yP)
  {
    if(d<=0)
       d = d + 4 * squrarea * (2 * (y - y0) + 3);
    else
       {
         d = d + 4 * squrarea * (2 * (y - y0) + 3)-8 * squrareb * ((x - x0) - 1);
         x--;
       }
     y++;

  EllipsePoints_FangSuo(m,n,x0,y0,x,y,color,sx,sy);
  }
}


/************************/
/**                    **/
/**   Բ�Ĵ��б任     **/
/**                    **/
/************************/


/* ���к�Բ��8�ԳƵ����ʾ */
void CirclePoints_CuoQie(int m,int n,int x0,int y0,int x,int y,int color,float q,int flag)/* Բ��Ϊ��x0,y0);��(m,n)Ϊ�ο���*/
{
  int X,Y;
  X = x;
  Y = y;
  CuoQie_RY(m,n,&X,&Y,q,flag);
       putpixel(X,Y,color);                /*(x,  y) */

  X = x0-y0+y;
  Y = y0-x0+x;
  CuoQie_RY(m,n,&X,&Y,q,flag);
        putpixel(X,Y,color);               /*(y,  x) */

  X = x0+y0-y;
  Y = y0-x0+x;
  CuoQie_RY(m,n,&X,&Y,q,flag);
        putpixel(X,Y,color);              /*(-y, x) */

  X = 2*x0-x;
  Y = y;
  CuoQie_RY(m,n,&X,&Y,q,flag);
        putpixel(X,Y,color);              /*(-x, y) */

  X = x0-y0+y;
  Y = y0+x0-x;
  CuoQie_RY(m,n,&X,&Y,q,flag);
        putpixel(X,Y,color);              /*(y, -x) */

  X = x;
  Y = 2*y0-y;
  CuoQie_RY(m,n,&X,&Y,q,flag);
       putpixel(X,Y,color);               /*(x, -y) */

  X =2*x0-x;
  Y =2*y0-y;
  CuoQie_RY(m,n,&X,&Y,q,flag);
       putpixel(X,Y,color);               /*(-x,-y) */

  X =x0+y0-y;
  Y =y0+x0-x;
  CuoQie_RY(m,n,&X,&Y,q,flag);
       putpixel(X,Y,color);               /*(-y,-x) */
}

/*  Բ�Ĵ��� */
void MidPointCircle_CuoQie(Yuan C,Point A,float q,int flag)/* Բ��Ϊ��x0,y0);*/
{
   int x,y,d,deltaE,deltaSE,x0,y0,radius,color,m,n;
   m = A.x;
   n = A.y;
   x0 = C.o.x;
   y0 = C.o.y;
   radius = C.r;
   color = C.color;

   x = x0;
   y = y0 + radius;
   d = 5 - 4*radius;
   deltaE = 12;
   deltaSE = 20 - 8*radius;

     CirclePoints_CuoQie(m,n,x0,y0,x,y,color,q,flag);
   while((y-y0)>(x-x0))
   {
      if(d<=0)
      {
        d = d + deltaE;
        deltaSE=deltaSE + 8;
      }
      else
      {
         d = d + deltaSE;
         deltaSE=deltaSE + 16;
         y--;
      }
      deltaE=deltaE + 8;
      x++;
      CirclePoints_CuoQie(m,n,x0,y0,x,y,color,q,flag);
   }

}


/***************************/
/**                       **/
/**     3D�ռ������      **/
/**    ƽ����X-Yƽ��      **/
/***************************/
void Rect_3DXY(Point3D A,int a,int b,int color,Point3D B,float qx,float qy,float qz)
{
   int i,j,k,X,Y,Z;
   Point3D B1,B2,B3,B4;

   B1.x = A.x + b/2.0; B1.y = A.y - a/2.0; B1.z = A.z;
   B2.x = A.x + b/2.0; B2.y = A.y + a/2.0; B2.z = A.z;
   B3.x = A.x - b/2.0; B3.y = A.y + a/2.0; B3.z = A.z;
   B4.x = A.x - b/2.0; B4.y = A.y - a/2.0; B4.z = A.z;

   X = B1.x; Y = B1.y; Z = B1.z;
   i = X; j = Y;
   while(j <= B2.y)
   {
      while(i >= B4.x)
      {
         Point3D_2D(&X,&Y,&Z,B,qx,qy,qz);
          putpixel(X,Y,color);

         i--;
         X = i; Y = j; Z = B1.z;
      }
      j++;
      X = B1.x; Y = j; Z = B1.z;
      i = X;
   }

}


/***************************/
/**                       **/
/**     3D�ռ������      **/
/**     ƽ����X-Zƽ��     **/
/***************************/
void Rect_3DXZ(Point3D A,int a,int b,int color,Point3D B,float qx,float qy,float qz)
{
   int i,j,k,X,Y,Z;
   Point3D B1,B2,B3,B4;

   B1.x = A.x + a/2.0; B1.y = A.y; B1.z = A.z - b/2.0;
   B2.x = A.x - a/2.0; B2.y = A.y; B2.z = A.z - b/2.0;
   B3.x = A.x - a/2.0; B3.y = A.y; B3.z = A.z + b/2.0;
   B4.x = A.x + a/2.0; B4.y = A.y; B4.z = A.z + b/2.0;

   X = B1.x; Y = B1.y; Z = B1.z;
   i = X; k = Z;
   while(i >= B2.x)
   {
      while(k <= B4.z)
      {
         Point3D_2D(&X,&Y,&Z,B,qx,qy,qz);
          putpixel(X,Y,color);

         k++;
         X = i; Y =A.y; Z = k;
      }
      i--;
      X = i; Y = A.y; Z = B1.z; k = Z;
   }

}

/***************************/
/**                       **/
/**     3D�ռ������      **/
/**     ƽ����Y-Zƽ��     **/
/***************************/
void Rect_3DYZ(Point3D A,int a,int b,int color,Point3D B,float qx,float qy,float qz)
{
   int i,j,k,X,Y,Z;
   Point3D B1,B2,B3,B4;

   B1.x = A.x; B1.y = A.y - a/2.0; B1.z = A.z - b/2.0;
   B2.x = A.x; B2.y = A.y + a/2.0; B2.z = A.z - b/2.0;
   B3.x = A.x; B3.y = A.y + a/2.0; B3.z = A.z + b/2.0;
   B4.x = A.x; B4.y = A.y - a/2.0; B4.z = A.z + b/2.0;

   X = B1.x; Y = B1.y; Z = B1.z;
   j = Y; k = Z;
   while(j <= B2.y)
   {
      while(k <= B4.z)
      {
         Point3D_2D(&X,&Y,&Z,B,qx,qy,qz);
          putpixel(X,Y,color);

         k++;
         X = A.x; Y =j; Z = k;
      }
      j++;
      X = A.x; Y = j; Z = B1.z; k = Z;
   }

}

/***************************/
/**                       **/
/**  3D�ռ���������ת   **/
/**    ƽ����X-Yƽ��      **/
/***************************/
void Rect_3DXY_XuanZhuan(Point3D A,int a,int b,int color,int q,Point3D D,int flag,Point3D B,float qx,float qy,float qz)
{
   int i,j,k,X,Y,Z;
   int ii,jj,kk;
   Point3D B1,B2,B3,B4;

   ii = D.x; jj = D.y; kk = D.z;

   B1.x = A.x + b/2.0; B1.y = A.y - a/2.0; B1.z = A.z;
   B2.x = A.x + b/2.0; B2.y = A.y + a/2.0; B2.z = A.z;
   B3.x = A.x - b/2.0; B3.y = A.y + a/2.0; B3.z = A.z;
   B4.x = A.x - b/2.0; B4.y = A.y - a/2.0; B4.z = A.z;

   X = B1.x; Y = B1.y; Z = B1.z;
   i = X; j = Y;
   while(j <= B2.y)
   {
      while(i >= B4.x)
      {
         XuanZhuan3D_RY(&X,&Y,&Z,q,ii,jj,kk,flag);
            Point3D_2D(&X,&Y,&Z,B,qx,qy,qz);
              putpixel(X,Y,color);

         i--;
         X = i; Y = j; Z = B1.z;
      }
      j++;
      X = B1.x; Y = j; Z = B1.z; i = X;
   }

}

/***************************/
/**                       **/
/**  3D�ռ���������ת   **/
/**    ƽ����X-Zƽ��      **/
/***************************/
void Rect_3DXZ_XuanZhuan(Point3D A,int a,int b,int color,int q,Point3D D,int flag,Point3D B,float qx,float qy,float qz)
{
   int i,j,k,X,Y,Z;
   int ii,jj,kk;
   Point3D B1,B2,B3,B4;

   ii = D.x; jj = D.y; kk = D.z;

   B1.x = A.x + a/2.0; B1.y = A.y; B1.z = A.z - b/2.0;
   B2.x = A.x - a/2.0; B2.y = A.y; B2.z = A.z - b/2.0;
   B3.x = A.x - a/2.0; B3.y = A.y; B3.z = A.z + b/2.0;
   B4.x = A.x + a/2.0; B4.y = A.y; B4.z = A.z + b/2.0;

   X = B1.x; Y = B1.y; Z = B1.z;
   i = X; k = Z;
   while(i >= B2.x)
   {
      while(k <= B4.z)
      {
         XuanZhuan3D_RY(&X,&Y,&Z,q,ii,jj,kk,flag);
            Point3D_2D(&X,&Y,&Z,B,qx,qy,qz);
              putpixel(X,Y,color);

         k++;
         X = i; Y =A.y; Z = k;
      }
      i--;
      X = i; Y = A.y; Z = B1.z; k = Z;
   }

}
/***************************/
/**                       **/
/**  3D�ռ���������ת   **/
/**    ƽ����Y-Zƽ��      **/
/***************************/
void Rect_3DYZ_XuanZhuan(Point3D A,int a,int b,int color,int q,Point3D D,int flag,Point3D B,float qx,float qy,float qz)
{
   int i,j,k,X,Y,Z;
   int ii,jj,kk;
   Point3D B1,B2,B3,B4;

   ii = D.x; jj = D.y; kk = D.z;

   B1.x = A.x; B1.y = A.y - a/2.0; B1.z = A.z - b/2.0;
   B2.x = A.x; B2.y = A.y + a/2.0; B2.z = A.z - b/2.0;
   B3.x = A.x; B3.y = A.y + a/2.0; B3.z = A.z + b/2.0;
   B4.x = A.x; B4.y = A.y - a/2.0; B4.z = A.z + b/2.0;

   X = B1.x; Y = B1.y; Z = B1.z;
   j = Y; k = Z;
   while(j <= B2.y)
   {
      while(k <= B4.z)
      {
         XuanZhuan3D_RY(&X,&Y,&Z,q,ii,jj,kk,flag);
            Point3D_2D(&X,&Y,&Z,B,qx,qy,qz);
              putpixel(X,Y,color);
         k++;
         X = A.x; Y =j; Z = k;
      }
      j++;
      X = A.x; Y = j; Z = B1.z; k = Z;
   }

}

/***************************/
/**                       **/
/**     3D�ռ�������      **/
/**    ƽ����X-Yƽ��      **/
/***************************/
void SanJiaoxing_3DXY(Point3D A,int a,int h,int color,Point3D B,float qx,float qy,float qz)
{
  int X,Y,Z,i,j,k;
  Point3D B1,B2,B3;
  Point C1,C2,C3,T;

  B1.x = A.x + h/2.0; B1.y = A.y - a/2.0; B1.z = A.z;
  B2.x = A.x + h/2.0; B2.y = A.y + a/2.0; B1.z = A.z;
  B1.x = A.x - h/2.0; B1.y = A.y; B1.z = A.z;

  X = B1.x; Y = B1.y; Z = B1.z;
    Point3D_2D(&X,&Y,&Z,B,qx,qy,qz);
  C1.x = X; C1.y = Y;

  X = B2.x; Y = B2.y; Z = B2.z;
    Point3D_2D(&X,&Y,&Z,B,qx,qy,qz);
  C2.x = X; C2.y = Y;

  X = B3.x; Y = B3.y; Z = B3.z;
    Point3D_2D(&X,&Y,&Z,B,qx,qy,qz);
  C3.x = X; C3.y = Y;

  X = B1.x; Y = B1.y; Z = B1.z;
  j = B1.y; i = B1.x;
  while(j <= B2.y)
  {
     Point3D_2D(&X,&Y,&Z,B,qx,qy,qz);
     T.x = X; T.y = Y;
     LineDDA(C3,T,color);
     j++;
  }


}

/***************************/
/**                       **/
/**      3D�ռ�Բ         **/
/**     ƽ����X-Yƽ��     **/
/***************************/

/* 3DԲ��8�ԳƵ����ʾ */
void CirclePoints_3DXY(Point3D A,int x,int y,int color,Point3D B,float qx,float qy,float qz)/* Բ��ΪA;*/
{
  int X,Y,Z;
  X = x; Y = y; Z = A.z;                /*(x,  y) */
     Point3D_2D(&X,&Y,&Z,B,qx,qy,qz);
          putpixel(X,Y,color);
  X = A.x-A.y+y; Y = A.y-A.x+x; Z = A.z;/*(y,  x) */
      Point3D_2D(&X,&Y,&Z,B,qx,qy,qz);
          putpixel(X,Y,color);
  X = A.x+A.y-y; Y = A.y-A.x+x; Z = A.z; /*(-y, x) */
      Point3D_2D(&X,&Y,&Z,B,qx,qy,qz);
          putpixel(X,Y,color);

  X = 2*A.x-x; Y = y; Z = A.z;          /*(-x, y) */
      Point3D_2D(&X,&Y,&Z,B,qx,qy,qz);
          putpixel(X,Y,color);

  X = A.x-A.y+y; Y = A.y+A.x-x; Z = A.z; /*(y, -x) */
       Point3D_2D(&X,&Y,&Z,B,qx,qy,qz);
          putpixel(X,Y,color);
  X = x; Y = 2*A.y-y; Z = A.z;          /*(x, -y) */
       Point3D_2D(&X,&Y,&Z,B,qx,qy,qz);
          putpixel(X,Y,color);

  X = 2*A.x-x; Y = 2*A.y-y; Z = A.z;    /*(-x,-y) */
       Point3D_2D(&X,&Y,&Z,B,qx,qy,qz);
          putpixel(X,Y,color);

  X = A.x+A.y-y; Y = A.y+A.x-x; Z = A.z;/*(-y,-x) */
      Point3D_2D(&X,&Y,&Z,B,qx,qy,qz);
          putpixel(X,Y,color);

}

void Yuan_3DXY(Point3D A,int r,int color,Point3D B,float qx,float qy,float qz)
{

   int x,y,d,deltaE,deltaSE,x0,y0,X,Y,Z;

   x0 = A.x;
   y0 = A.y;

   x = x0;
   y = y0 + r;
   d = 5 - 4*r;
   deltaE = 12;
   deltaSE = 20 - 8*r;


   CirclePoints_3DXY(A,x,y,color,B,qx,qy,qz);


   while((y-y0)>(x-x0))
   {
      if(d<=0)
      {
        d = d + deltaE;
        deltaSE=deltaSE + 8;
      }
      else
      {
         d = d + deltaSE;
         deltaSE=deltaSE + 16;
         y--;
      }
      deltaE=deltaE + 8;
      x++;
      CirclePoints_3DXY(A,x,y,color,B,qx,qy,qz);
   }

}

/***************************/
/**                       **/
/**      3D�ռ�Բ         **/
/**     ƽ����X-Zƽ��     **/
/***************************/

/* 3DԲ��8�ԳƵ����ʾ */
void CirclePoints_3DXZ(Point3D A,int x,int z,int color,Point3D B,float qx,float qy,float qz)/* Բ��ΪA;*/
{
  int X,Y,Z;
  X = x; Y = A.y; Z = z;                         /*(x,  z) */
     Point3D_2D(&X,&Y,&Z,B,qx,qy,qz);
          putpixel(X,Y,color);

  X = A.x - A.z + z; Y = A.y; Z = A.z - A.x + x; /*(z,  x) */
      Point3D_2D(&X,&Y,&Z,B,qx,qy,qz);
          putpixel(X,Y,color);

  X = A.x + A.z - z; Y = A.y; Z = A.z - A.x + x;/*(-z, x) */
       Point3D_2D(&X,&Y,&Z,B,qx,qy,qz);
          putpixel(X,Y,color);

  X = 2 * A.x - x; Y = A.y; Z = z;               /*(-x, z) */
       Point3D_2D(&X,&Y,&Z,B,qx,qy,qz);
          putpixel(X,Y,color);

  X = A.x - A.z + z; Y = A.y; Z = A.z + A.x - x; /*(z, -x) */
       Point3D_2D(&X,&Y,&Z,B,qx,qy,qz);
          putpixel(X,Y,color);

  X = x; Y = A.y; Z = 2 * A.z - z;               /*(x, -z) */
        Point3D_2D(&X,&Y,&Z,B,qx,qy,qz);
          putpixel(X,Y,color);

  X = 2 * A.x - x; Y = A.y; Z = 2 * A.z - z;     /*(-x,-z) */
         Point3D_2D(&X,&Y,&Z,B,qx,qy,qz);
          putpixel(X,Y,color);

  X = A.x + A.z - z; Y = A.y; Z = A.z + A.x - x; /*(-z,-x) */
         Point3D_2D(&X,&Y,&Z,B,qx,qy,qz);
          putpixel(X,Y,color);



}

void Yuan_3DXZ(Point3D A,int r,int color,Point3D B,float qx,float qy,float qz)
{

   int x,z,d,deltaE,deltaSE,x0,z0,X,Y,Z;

   x0 = A.x;
   z0 = A.z;

   x = x0;
   z = z0 + r;
   d = 5 - 4*r;
   deltaE = 12;
   deltaSE = 20 - 8*r;


   CirclePoints_3DXZ(A,x,z,color,B,qx,qy,qz);


   while((z-z0)>(x-x0))
   {
      if(d<=0)
      {
        d = d + deltaE;
        deltaSE=deltaSE + 8;
      }
      else
      {
         d = d + deltaSE;
         deltaSE=deltaSE + 16;
         z--;
      }
      deltaE=deltaE + 8;
      x++;
      CirclePoints_3DXZ(A,x,z,color,B,qx,qy,qz);
   }

}

/***************************/
/**                       **/
/**3Dƽ����X-Yƽ��Ŀռ�Բ**/
/**       ����ת          **/
/***************************/
/* 3DԲ��8�ԳƵ����ʾ */
void CirclePoints_3DXY_XuanZhuan(Point3D A,int x,int y,int color,float q,int i,int j,int k,int flag,Point3D B,float qx,float qy,float qz)/* Բ��ΪA;*/
{
  int X,Y,Z;
  X = x; Y = y; Z = A.z;                /*(x,  y) */
     XuanZhuan3D_RY(&X,&Y,&Z,q,i,j,k,flag);
      Point3D_2D(&X,&Y,&Z,B,qx,qy,qz);
         putpixel(X,Y,color);

  X = A.x-A.y+y; Y = A.y-A.x+x; Z = A.z;/*(y,  x) */
      XuanZhuan3D_RY(&X,&Y,&Z,q,i,j,k,flag);
      Point3D_2D(&X,&Y,&Z,B,qx,qy,qz);
         putpixel(X,Y,color);

  X = A.x+A.y-y; Y = A.y-A.x+x; Z = A.z; /*(-y, x) */
      XuanZhuan3D_RY(&X,&Y,&Z,q,i,j,k,flag);
      Point3D_2D(&X,&Y,&Z,B,qx,qy,qz);
         putpixel(X,Y,color);


  X = 2*A.x-x; Y = y; Z = A.z;          /*(-x, y) */
     XuanZhuan3D_RY(&X,&Y,&Z,q,i,j,k,flag);
      Point3D_2D(&X,&Y,&Z,B,qx,qy,qz);
         putpixel(X,Y,color);

  X = A.x-A.y+y; Y = A.y+A.x-x; Z = A.z; /*(y, -x) */
       XuanZhuan3D_RY(&X,&Y,&Z,q,i,j,k,flag);
      Point3D_2D(&X,&Y,&Z,B,qx,qy,qz);
         putpixel(X,Y,color);


  X = x; Y = 2*A.y-y; Z = A.z;          /*(x, -y) */
       XuanZhuan3D_RY(&X,&Y,&Z,q,i,j,k,flag);
      Point3D_2D(&X,&Y,&Z,B,qx,qy,qz);
         putpixel(X,Y,color);

  X = 2*A.x-x; Y = 2*A.y-y; Z = A.z;    /*(-x,-y) */
       XuanZhuan3D_RY(&X,&Y,&Z,q,i,j,k,flag);
      Point3D_2D(&X,&Y,&Z,B,qx,qy,qz);
         putpixel(X,Y,color);

  X = A.x+A.y-y; Y = A.y+A.x-x; Z = A.z;/*(-y,-x) */
      XuanZhuan3D_RY(&X,&Y,&Z,q,i,j,k,flag);
      Point3D_2D(&X,&Y,&Z,B,qx,qy,qz);
         putpixel(X,Y,color);
}

void Yuan_3DXY_XuanZhuan(Point3D A,int r,int color,float q,Point3D D,int flag,Point3D B,float qx,float qy,float qz)
{

   int x,y,d,deltaE,deltaSE,x0,y0,X,Y,Z,i,j,k;

   i = D.x; j = D.y; k = D.z;

   x0 = A.x;
   y0 = A.y;

   x = x0;
   y = y0 + r;
   d = 5 - 4*r;
   deltaE = 12;
   deltaSE = 20 - 8*r;


   CirclePoints_3DXY_XuanZhuan(A,x,y,color,q,i,j,k,flag,B,qx,qy,qz);


   while((y-y0)>(x-x0))
   {
      if(d<=0)
      {
        d = d + deltaE;
        deltaSE=deltaSE + 8;
      }
      else
      {
         d = d + deltaSE;
         deltaSE=deltaSE + 16;
         y--;
      }
      deltaE=deltaE + 8;
      x++;
      CirclePoints_3DXY_XuanZhuan(A,x,y,color,q,i,j,k,flag,B,qx,qy,qz);
   }

}




/***************************/
/*                         */
/*  3D�ռ���Բ���е��㷨   */
/*     ƽ����X - Y         */
/***************************/

/* ��Բ��4�ԳƵ����ʾ */
void EllipsePoints_3DXY(Point3D A,int x,int y,int color,Point3D B,float qx,float qy,float qz) /*��Բ������Ϊ:(x0,y0)*/
{
  int X,Y,Z;
  X = x; Y = y; Z = A.z;                    /*(x,  y) */
      Point3D_2D(&X,&Y,&Z,B,qx,qy,qz);
          putpixel(X,Y,color);

  X = 2 * A.x - x; Y = y; Z = A.z;          /*(-x, y) */
       Point3D_2D(&X,&Y,&Z,B,qx,qy,qz);
          putpixel(X,Y,color);

  X = 2 * A.x - x; Y = 2 * A.y - y; Z = A.z;/*(-x,-y) */
        Point3D_2D(&X,&Y,&Z,B,qx,qy,qz);
          putpixel(X,Y,color);

  X = x; Y = 2 * A.y - y; Z = A.z;          /*(x, -y) */
         Point3D_2D(&X,&Y,&Z,B,qx,qy,qz);
          putpixel(X,Y,color);

}


/* ��Բ���е��㷨 */
void MidPointEllipse_3DXY(Point3D A,int a,int b,int color,Point3D B,float qx,float qy,float qz)
{
  int x,y,xP,yP,x0,y0;
  float d,squrarea,squrareb;
   x0 = A.x;
   y0 = A.y;


  squrarea = a * a;
  squrareb = b * b;

  /*����ֽ��P*/
  xP = (int)(0.5 + squrarea/sqrt(squrarea + squrareb));
  yP = (int)(0.5 + squrareb/sqrt(squrarea + squrareb));

 /* �����ϰ벿��Բ*/
  x = x0;
  y = y0 + b;
  d = 4 * (squrareb - squrarea * b) + squrarea;
  EllipsePoints_3DXY(A,x,y,color,B,qx,qy,qz);
  while((x - x0)<xP)
  {
    if(d<=0)
       d = d + 4 * squrareb * (2 * (x - x0) + 3);
    else
       {
         d = d + 4 * squrareb * (2 * (x - x0) + 3)-8 * squrarea * ((y - y0) - 1);
         y--;
       }
    x++;
    EllipsePoints_3DXY(A,x,y,color,B,qx,qy,qz);
  }

  /* �����°벿��Բ*/
  x = x0 + a;
  y = y0;
  d = 4 * (squrarea - squrareb * a) + squrareb;
  EllipsePoints_3DXY(A,x,y,color,B,qx,qy,qz);
  while((y-y0) < yP)
  {
    if(d<=0)
       d = d + 4 * squrarea * (2 * (y - y0) + 3);
    else
       {
         d = d + 4 * squrarea * (2 * (y - y0) + 3)-8 * squrareb * ((x - x0) - 1);
         x--;
       }
     y++;
   EllipsePoints_3DXY(A,x,y,color,B,qx,qy,qz);
  }
}


/***************************/
/*                         */
/*    3D�ռ���Բ����ת     */
/*                         */
/***************************/

/* ��Բ��ת���4�ԳƵ����ʾ */
void EllipsePoints_3DXY_XhuanZhuan(Point3D A,int x,int y,int color,float q,int i,int j,int k,int flag,Point3D B,float qx,float qy,float qz) /*��Բ������Ϊ:A*/
{
  int X,Y,Z;
  X = x; Y = y; Z = A.z;                    /*(x,  y) */
     XuanZhuan3D_RY(&X,&Y,&Z,q,i,j,k,flag);
      Point3D_2D(&X,&Y,&Z,B,qx,qy,qz);
         putpixel(X,Y,color);


  X = 2 * A.x - x; Y = y; Z = A.z;          /*(-x, y) */
       XuanZhuan3D_RY(&X,&Y,&Z,q,i,j,k,flag);
         Point3D_2D(&X,&Y,&Z,B,qx,qy,qz);
         putpixel(X,Y,color);

  X = 2 * A.x - x; Y = 2 * A.y - y; Z = A.z;/*(-x,-y) */
       XuanZhuan3D_RY(&X,&Y,&Z,q,i,j,k,flag);
         Point3D_2D(&X,&Y,&Z,B,qx,qy,qz);
         putpixel(X,Y,color);
  X = x; Y = 2 * A.y - y; Z = A.z;          /*(x, -y) */
        XuanZhuan3D_RY(&X,&Y,&Z,q,i,j,k,flag);
          Point3D_2D(&X,&Y,&Z,B,qx,qy,qz);
         putpixel(X,Y,color);
}



/* ��Բ���е��㷨 */
void MidPointEllipse_3DXY_XhuanZhuan(Point3D A,int a,int b,int color,float q,Point3D D,int flag,Point3D B,float qx,float qy,float qz) /* ��ͨ�� D ��ֱ�ߵ���ת */
{
  int x,y,xP,yP,x0,y0,i,j,k;
  float d,squrarea,squrareb;
   x0 = A.x;
   y0 = A.y;
   i = D.x; j = D.y; k = D.z;

  squrarea = a * a;
  squrareb = b * b;

  /*����ֽ��P*/
  xP = (int)(0.5 + squrarea/sqrt(squrarea + squrareb));
  yP = (int)(0.5 + squrareb/sqrt(squrarea + squrareb));

 /* �����ϰ벿��Բ*/
  x = x0;
  y = y0 + b;
  d = 4 * (squrareb - squrarea * b) + squrarea;
  EllipsePoints_3DXY_XhuanZhuan(A,x,y,color,q,i,j,k,flag,B,qx,qy,qz);
  while((x - x0)<xP)
  {
    if(d<=0)
       d = d + 4 * squrareb * (2 * (x - x0) + 3);
    else
       {
         d = d + 4 * squrareb * (2 * (x - x0) + 3)-8 * squrarea * ((y - y0) - 1);
         y--;
       }
    x++;
    EllipsePoints_3DXY_XhuanZhuan(A,x,y,color,q,i,j,k,flag,B,qx,qy,qz);
  }

  /* �����°벿��Բ*/
  x = x0 + a;
  y = y0;
  d = 4 * (squrarea - squrareb * a) + squrareb;
  EllipsePoints_3DXY_XhuanZhuan(A,x,y,color,q,i,j,k,flag,B,qx,qy,qz);
  while((y-y0) < yP)
  {
    if(d<=0)
       d = d + 4 * squrarea * (2 * (y - y0) + 3);
    else
       {
         d = d + 4 * squrarea * (2 * (y - y0) + 3)-8 * squrareb * ((x - x0) - 1);
         x--;
       }
     y++;
   EllipsePoints_3DXY_XhuanZhuan(A,x,y,color,q,i,j,k,flag,B,qx,qy,qz);
  }
}
/***************************/
/**                       **/
/**    Բ�����㷨         **/
/**                       **/
/***************************/
void YuanZhu3D(Point3D A,int r,int h,Point3D B,float qx,float qy,float qz)
{
   Point3D T;
   T.x = A.x; T.y = A.y; T.z = A.z -h/2.0;
   while(T.z <= A.z + h/2.0)
   {
     Yuan_3DXY(T,r,6,B,qx,qy,qz);
     T.z++;
   }

   while(r>=0)
   {
      Yuan_3DXY(T,r,2,B,qx,qy,qz);
      r--;
   }
}

/******************************************/
/**                                      **/
/**   ��������㷨(����������������㷨) **/
/**                                      **/
/******************************************/
void Cft3D(Point3D A,int a,int b,int h,Point3D B,float qx,float qy,float qz)
{
   int X,Y,Z,i,j;
   Point3D B1,B2,B3,B4,B5,B6;
   Depth dp[6],temp;

   B1.x = A.x + b/2.0; B1.y = A.y;         B1.z = A.z;             /* ������������Ϣ(������ε����ĵ�) */

   B2.x = A.x;         B2.y = A.y + a/2.0; B2.z = A.z;

   B3.x = A.x - b/2.0; B3.y = A.y;         B3.z = A.z;

   B4.x = A.x;         B4.y = A.y - a/2.0; B4.z = A.z;

   B5.x = A.x;         B5.y = A.y;         B5.z = A.z - h/2.0;

   B6.x = A.x;         B6.y = A.y;         B6.z = A.z + h/2.0;

                                    /*�����Ϣ��ʼ��*/
   X = B1.x; Y = B1.y; Z = B1.z;
        Point3D_2D(&X,&Y,&Z,B,qx,qy,qz);
   dp[0].s = '1'; dp[0].d = Z;          /*�� 1 ������*/

    X = B2.x; Y = B2.y; Z = B2.z;
        Point3D_2D(&X,&Y,&Z,B,qx,qy,qz);
   dp[1].s = '2'; dp[1].d = Z;          /*�� 2 ������*/

    X = B3.x; Y = B3.y; Z = B3.z;
        Point3D_2D(&X,&Y,&Z,B,qx,qy,qz);
   dp[2].s = '3'; dp[2].d = Z;          /*�� 3 ������*/


   X = B4.x; Y = B4.y; Z = B4.z;
        Point3D_2D(&X,&Y,&Z,B,qx,qy,qz);
   dp[3].s = '4'; dp[3].d = Z;          /*�� 4 ������*/


   X = B5.x; Y = B5.y; Z = B5.z;
        Point3D_2D(&X,&Y,&Z,B,qx,qy,qz);
   dp[4].s = '5'; dp[4].d = Z;          /*�� 5 ������*/

   X = B6.x; Y = B6.y; Z = B6.z;
        Point3D_2D(&X,&Y,&Z,B,qx,qy,qz);
   dp[5].s = '6'; dp[5].d = Z;          /*�� 6 ������*/

    for(i = 0; i <=4; i++)                 /*  �������  */
   {
      for(j = i+1;j<=5;j++)
      {
         if(dp[i].d>dp[j].d)
         {
           temp.s = dp[i].s; temp.d = dp[i].d;
           dp[i].s = dp[j].s; dp[i].d = dp[j].d;
           dp[j].s = temp.s; dp[j].d = temp.d;
         }
      }
   }

   i = 0;

   while(i <= 5)      /*�����㷨*/
  {
    switch(dp[i++].s)
    {


    case'1': Rect_3DYZ(B1,a,h,4,B,qx,qy,qz);
             break;

    case'2': Rect_3DXZ(B2,b,h,8,B,qx,qy,qz);
             break;

    case'3': Rect_3DYZ(B3,a,h,6,B,qx,qy,qz);
             break;

    case'4': Rect_3DXZ(B4,b,h,12,B,qx,qy,qz);
             break;

    case'5': Rect_3DXY(B5,a,b,3,B,qx,qy,qz);
             break;

    case'6': Rect_3DXY(B6,a,b,1,B,qx,qy,qz);
             break;
    }
  }
}

/***************************/
/**                       **/
/**   ���������ת        **/
/**                       **/
/***************************/
void Cft3D_XuanZhuan(Point3D A,int a,int b,int h,float q,Point3D D,int flag,Point3D B,float qx,float qy,float qz)
{
   int X,Y,Z,i,j;
   Depth dp[6],temp;
   Point3D B1,B2,B3,B4,B5,B6;

   B1.x = A.x + b/2.0; B1.y = A.y;         B1.z = A.z;

   B2.x = A.x;         B2.y = A.y + a/2.0; B2.z = A.z;

   B3.x = A.x - b/2.0; B3.y = A.y;         B3.z = A.z;

   B4.x = A.x;         B4.y = A.y - a/2.0; B4.z = A.z;

   B5.x = A.x;         B5.y = A.y;         B5.z = A.z - h/2.0;

   B6.x = A.x;         B6.y = A.y;         B6.z = A.z + h/2.0;

   /*�����Ϣ��ʼ��*/
   X = B1.x; Y = B1.y; Z = B1.z;
   XuanZhuan3D_RY(&X,&Y,&Z,q,D.x,D.y,D.z,flag);
   Point3D_2D(&X,&Y,&Z,B,qx,qy,qz);
   dp[0].s = '1'; dp[0].d = Z;          /*�� 1 ������*/

    X = B2.x; Y = B2.y; Z = B2.z;
    XuanZhuan3D_RY(&X,&Y,&Z,q,D.x,D.y,D.z,flag);
        Point3D_2D(&X,&Y,&Z,B,qx,qy,qz);
   dp[1].s = '2'; dp[1].d = Z;          /*�� 2 ������*/

    X = B3.x; Y = B3.y; Z = B3.z;
    XuanZhuan3D_RY(&X,&Y,&Z,q,D.x,D.y,D.z,flag);
        Point3D_2D(&X,&Y,&Z,B,qx,qy,qz);
   dp[2].s = '3'; dp[2].d = Z;          /*�� 3 ������*/


   X = B4.x; Y = B4.y; Z = B4.z;
   XuanZhuan3D_RY(&X,&Y,&Z,q,D.x,D.y,D.z,flag);
        Point3D_2D(&X,&Y,&Z,B,qx,qy,qz);
   dp[3].s = '4'; dp[3].d = Z;          /*�� 4 ������*/


   X = B5.x; Y = B5.y; Z = B5.z;
   XuanZhuan3D_RY(&X,&Y,&Z,q,D.x,D.y,D.z,flag);
        Point3D_2D(&X,&Y,&Z,B,qx,qy,qz);
   dp[4].s = '5'; dp[4].d = Z;          /*�� 5 ������*/

   X = B6.x; Y = B6.y; Z = B6.z;
   XuanZhuan3D_RY(&X,&Y,&Z,q,D.x,D.y,D.z,flag);
        Point3D_2D(&X,&Y,&Z,B,qx,qy,qz);
   dp[5].s = '6'; dp[5].d = Z;          /*�� 6 ������*/


   for(i = 0; i <=4; i++)                 /*  �������  */
   {
      for(j = i+1;j<=5;j++)
      {
         if(dp[i].d>dp[j].d)
         {
           temp.s = dp[i].s; temp.d = dp[i].d;
           dp[i].s = dp[j].s; dp[i].d = dp[j].d;
           dp[j].s = temp.s; dp[j].d = temp.d;
         }
      }
   }
   i = 0;
   while(i <= 5)      /*�����㷨*/
  {
    switch(dp[i++].s)
    {

      case'1': Rect_3DYZ_XuanZhuan(B1,a,h,4,q,D,flag,B,qx,qy,qz);
               break;

      case'2': Rect_3DXZ_XuanZhuan(B2,b,h,12,q,D,flag,B,qx,qy,qz);
               break;

      case'3': Rect_3DYZ_XuanZhuan(B3,a,h,4,q,D,flag,B,qx,qy,qz);
               break;

      case'4': Rect_3DXZ_XuanZhuan(B4,b,h,12,q,D,flag,B,qx,qy,qz);
               break;

      case'5': Rect_3DXY_XuanZhuan(B5,a,b,1,q,D,flag,B,qx,qy,qz);
               break;

      case'6': Rect_3DXY_XuanZhuan(B6,a,b,1,q,D,flag,B,qx,qy,qz);
               break;
     }
  }
}

/***************************************/
/***                                 ***/
/***      ������ʵ��ʵ�廭��         ***/
/***                                 ***/
/***************************************/


/*****************************/
/**    ����(ƽ����X��)      **/
/*****************************/
void LanGan_X(Point3D A,int L,int h,Point3D BB,float qx,float qy,float qz)
{
  int X,Y,Z,i,j;
  Point3D B;
  Depth dp[7],temp;

  X = A.x - L/2.0; Y = A.y; Z = A.z;    /*�������ߵ����*/
     Point3D_2D(&X,&Y,&Z,B,qx,qy,qz);
  dp[0].s = 'A'; dp[0].d = Z;

  X = A.x - L/2.0+ L/4.0; Y = A.y; Z = A.z;
     Point3D_2D(&X,&Y,&Z,B,qx,qy,qz);
  dp[1].s = 'B'; dp[1].d = Z;

  X = A.x - L/2.0 + L/2.0; Y = A.y; Z = A.z;
    Point3D_2D(&X,&Y,&Z,B,qx,qy,qz);
  dp[2].s = 'C'; dp[2].d = Z;

  X = A.x - L/2.0 + 3*L/4.0; Y = A.y; Z = A.z;
     Point3D_2D(&X,&Y,&Z,B,qx,qy,qz);
  dp[3].s = 'D'; dp[3].d = Z;

  X = A.x - L/2.0 + L; Y = A.y; Z = A.z;
     Point3D_2D(&X,&Y,&Z,B,qx,qy,qz);
  dp[4].s = 'E'; dp[4].d = Z;

  for(i = 0; i <=3; i++)                 /*  �������  */
   {
      for(j = i+1;j<=4;j++)
      {
         if(dp[i].d>dp[j].d)
         {
           temp.s = dp[i].s; temp.d = dp[i].d;
           dp[i].s = dp[j].s; dp[i].d = dp[j].d;
           dp[j].s = temp.s; dp[j].d = temp.d;
         }
      }
   }

   i = 0;

   while(i <= 4)      /*�����㷨*/
  {
    switch(dp[i++].s)
    {


  case'A': B.x = A.x - L/2.0; B.y = A.y; B.z = A.z;  /*��������*/
           Cft3D(B,15,15,h + 40,BB,qx,qy,qz);
           break;

  case'B': B.x = A.x - L/2.0 + L/4.0; B.y = A.y; B.z = A.z;
           Cft3D(B,15,15,h - 15,BB,qx,qy,qz);
           break;

  case'C': B.x = A.x - L/2.0 + L/2.0; B.y = A.y; B.z = A.z;
           Cft3D(B,15,15,h - 15,BB,qx,qy,qz);
           break;

  case'D': B.x = A.x - L/2.0 + 3*L/4.0; B.y = A.y; B.z = A.z;
           Cft3D(B,15,15,h - 15,BB,qx,qy,qz);
           break;

  case'E': B.x = A.x - L/2.0 + L; B.y = A.y; B.z = A.z;
           Cft3D(B,15,15,h + 40,BB,qx,qy,qz);
           break;


   }
  }

  B.x = A.x; B.y = A.y; B.z = A.z + h/2.0;        /*��������*/
           Cft3D(B,15,L - 15,15,BB,qx,qy,qz);


  B.x = A.x; B.y = A.y; B.z = A.z - h/2.0;
           Cft3D(B,15,L - 15,15,BB,qx,qy,qz);

}

/*****************************/
/**    ����(ƽ����Y��)      **/
/*****************************/
void LanGan_Y(Point3D A,int L,int h,Point3D BB,float qx,float qy,float qz)
{
  int X,Y,Z,i,j;
  Point3D B;
  Depth dp[7],temp;

  X = A.x - L/2.0; Y = A.y; Z = A.z;    /*�������ߵ����*/
     Point3D_2D(&X,&Y,&Z,B,qx,qy,qz);
  dp[0].s = 'A'; dp[0].d = Z;

  X = A.x - L/2.0+ L/4.0; Y = A.y; Z = A.z;
     Point3D_2D(&X,&Y,&Z,B,qx,qy,qz);
  dp[1].s = 'B'; dp[1].d = Z;

  X = A.x - L/2.0 + L/2.0; Y = A.y; Z = A.z;
    Point3D_2D(&X,&Y,&Z,B,qx,qy,qz);
  dp[2].s = 'C'; dp[2].d = Z;

  X = A.x - L/2.0 + 3*L/4.0 - 10; Y = A.y; Z = A.z;
     Point3D_2D(&X,&Y,&Z,B,qx,qy,qz);
  dp[3].s = 'D'; dp[3].d = Z;

  X = A.x - L/2.0 + L; Y = A.y; Z = A.z;
     Point3D_2D(&X,&Y,&Z,B,qx,qy,qz);
  dp[4].s = 'E'; dp[4].d = Z;


  for(i = 0; i <=3; i++)                 /*  �������  */
   {
      for(j = i+1;j<=4;j++)
      {
         if(dp[i].d>=dp[j].d)
         {
           temp.s = dp[i].s; temp.d = dp[i].d;
           dp[i].s = dp[j].s; dp[i].d = dp[j].d;
           dp[j].s = temp.s; dp[j].d = temp.d;
         }
      }
   }

   i = 0;

   while(i <= 4)      /*�����㷨*/
  {
    switch(dp[i++].s)
    {


  case'A': B.x = A.x; B.y = A.y - L/2.0; B.z = A.z;  /*��������*/
           Cft3D(B,15,15,h +40,BB,qx,qy,qz);
           break;

  case'B': B.x = A.x; B.y = A.y - L/2.0 + L/4.0; B.z = A.z;
           Cft3D(B,15,15,h - 15,BB,qx,qy,qz);
           break;

  case'C': B.x = A.x ; B.y = A.y- L/2.0 + L/2.0; B.z = A.z;
           Cft3D(B,15,15,h - 15,BB,qx,qy,qz);
           break;

  case'D': B.x = A.x ; B.y = A.y- L/2.0 + 3*L/4.0; B.z = A.z;
           Cft3D(B,15,15,h - 15,BB,qx,qy,qz);
           break;

  case'E': B.x = A.x ; B.y = A.y- L/2.0 + L; B.z = A.z;
           Cft3D(B,15,15,h + 40,BB,qx,qy,qz);
           break;


   }
  }
    B.x = A.x; B.y = A.y; B.z = A.z + h/2.0;
           Cft3D(B,L - 15,15,15,BB,qx,qy,qz);


    B.x = A.x; B.y = A.y; B.z = A.z - h/2.0;
           Cft3D(B,L - 15,15,15,BB,qx,qy,qz);


}





/*****************************/
/**          ��             **/
/*****************************/
void Shu(Point3D A,int r,Point3D BB,float qx,float qy,float qz)
{
   int k;
   Point3D B;
   setbkcolor(2);
   B.x = A.x + r; B.y = A.y; B.z = A.z;
   k=0;
   while(k<360)
   {
     Rect_3DXY_XuanZhuan(B,40,20,7,k,A,3,BB,qx,qy,qz);
     k = k+30;
   }

   B.x = A.x; B.y = A.y; B.z = A.z + 50; /* ���� */
   YuanZhu3D(B,15,100,BB,qx,qy,qz);

   B.x = A.x; B.y = A.y; B.z = A.z + 15;
   k = 15;
   while(B.z>=A.z)
   {
       if((int)B.z%2==0)
          Yuan_3DXY(B,k,2,BB,qx,qy,qz);
       else
          Yuan_3DXY(B,k,4,BB,qx,qy,qz);
       B.z--;
       k = k + 1;
   }



   B.x = A.x; B.y = A.y; B.z = A.z + 100; /*��Ҷ*/
   k=80;
   while(k>=0)
   {
       Yuan_3DXY(B,k--,8,BB,qx,qy,qz);
   }

   k=100;            /*��Ҷ1*/
   while(k>=0)
   {
      if(k%2==0)
        Yuan_3DXY(B,k,3,BB,qx,qy,qz);
      else
          Yuan_3DXY(B,k,4,BB,qx,qy,qz);

      B.z = B.z + 2;
      k--;

   }

   B.x = A.x; B.y = A.y; B.z = A.z + 200;
   k=80;
   while(k>=0)
   {
       Yuan_3DXY(B,k--,8,BB,qx,qy,qz);
   }

   k=80;            /*��Ҷ2*/
   while(k>=0)
   {
      if(k%2==0)
        Yuan_3DXY(B,k,3,BB,qx,qy,qz);
      else
          Yuan_3DXY(B,k,4,BB,qx,qy,qz);

      B.z = B.z + 2;
      k--;

   }


}



/*****************************/
/**          ����           **/
/*****************************/
void DengZi(Point3D A,int a,int b,int h,Point3D B,float qx,float qy,float qz)
{
   int X,Y,Z,i,j;
   Point3D D;
   Depth dp[12],temp;
   /*  Rect_3DXY(A,a + 100,b + 100,8,B,qx,qy,qz);*/



                                              /* ��ȳ�ʼ��*/
   X =  A.x - b/2.0; Y = A.y - a/2.0; Z = A.z + h/2.0;
      Point3D_2D(&X,&Y,&Z,B,qx,qy,qz);
   dp[0].s = 'A'; dp[0].d = Z;


    X =  A.x - b/2.0; Y = A.y + a/2.0; Z = A.z + h/2.0;
      Point3D_2D(&X,&Y,&Z,B,qx,qy,qz);
   dp[1].s = 'B'; dp[1].d = Z;

   X =  A.x + b/2.0; Y = A.y - a/2.0; Z = A.z + h/2.0;
      Point3D_2D(&X,&Y,&Z,B,qx,qy,qz);
   dp[2].s = 'C'; dp[2].d = Z;


   X =  A.x + b/2.0; Y = A.y + a/2.0; Z = A.z + h/2.0;
      Point3D_2D(&X,&Y,&Z,B,qx,qy,qz);
   dp[3].s = 'D'; dp[3].d = Z;

   X =  A.x - b/2.0; Y = A.y; Z = A.z + h/2.0;
      Point3D_2D(&X,&Y,&Z,B,qx,qy,qz);
   dp[4].s = 'E'; dp[4].d = Z;

   X =  A.x + b/2.0; Y = A.y; Z = A.z + h/2.0;
      Point3D_2D(&X,&Y,&Z,B,qx,qy,qz);
   dp[5].s = 'F'; dp[5].d = Z;

   X = A.x; Y = A.y - a/2.0; Z = A.z + h;
      Point3D_2D(&X,&Y,&Z,B,qx,qy,qz);
   dp[6].s = 'H'; dp[6].d = Z;

   X = A.x; Y = A.y - a/2.0 + a/5.0; Z = A.z + h;
       Point3D_2D(&X,&Y,&Z,B,qx,qy,qz);
   dp[7].s = 'I'; dp[7].d = Z;

   X = A.x; Y = A.y - a/2.0 + 2*a/5.0; Z = A.z + h;
        Point3D_2D(&X,&Y,&Z,B,qx,qy,qz);
   dp[8].s = 'J'; dp[8].d = Z;

   X = A.x; Y = A.y - a/2.0 + 3*a/5.0; Z = A.z + h;
        Point3D_2D(&X,&Y,&Z,B,qx,qy,qz);
   dp[9].s = 'K'; dp[9].d = Z;

   X = A.x; Y = A.y - a/2.0 + 4*a/5.0; Z = A.z + h;
        Point3D_2D(&X,&Y,&Z,B,qx,qy,qz);
   dp[10].s = 'L'; dp[10].d = Z;

   X = A.x; Y = A.y - a/2.0 + a; Z = A.z + h;
        Point3D_2D(&X,&Y,&Z,B,qx,qy,qz);
   dp[11].s = 'M'; dp[11].d = Z;




    for(i = 0; i <=11; i++)                 /*  �������  */
   {
      for(j = i+1;j<=12;j++)
      {
         if(dp[i].d>dp[j].d)
         {
           temp.s = dp[i].s; temp.d = dp[i].d;
           dp[i].s = dp[j].s; dp[i].d = dp[j].d;
           dp[j].s = temp.s; dp[j].d = temp.d;
         }
      }
   }

   i = 0;

   while(i <= 11)      /*�����㷨*/
  {
    switch(dp[i++].s)
    {
    case 'A': D.x = A.x - b/2.0; D.y = A.y - a/2.0; D.z = A.z + h/2.0;  /* �ĸ��� */
              Cft3D(D,20,20,h+20,B,qx,qy,qz);
              break;


    case 'B': D.x = A.x - b/2.0; D.y = A.y + a/2.0; D.z = A.z + h/2.0;
              Cft3D(D,20,20,h+20,B,qx,qy,qz);
              break;

    case 'C': D.x = A.x + b/2.0; D.y = A.y - a/2.0; D.z = A.z + h/2.0;
              Cft3D(D,20,20,h+20,B,qx,qy,qz);
              break;


    case 'D': D.x = A.x + b/2.0; D.y = A.y + a/2.0; D.z = A.z + h/2.0;
              Cft3D(D,20,20,h+20,B,qx,qy,qz);
              break;


    case 'E': D.x = A.x - b/2.0; D.y = A.y; D.z = A.z + h;  /*�������ľ*/
              Cft3D(D,a+20,10,10,B,qx,qy,qz);
              break;


    case 'F': D.x = A.x + b/2.0; D.y = A.y; D.z = A.z + h;
              Cft3D(D,a+20,10,10,B,qx,qy,qz);
              break;

    case 'H': D.x = A.x; D.y = A.y - a/2.0; D.z = A.z + h;
              Cft3D(D,10,b+20,10,B,qx,qy,qz);
              break;

    case 'I': D.x = A.x; D.y = A.y - a/2.0 + a/5.0; D.z = A.z + h;
              Cft3D(D,10,b+20,10,B,qx,qy,qz);
              break;


    case 'J': D.x = A.x; D.y = A.y - a/2.0 + 2*a/5.0; D.z = A.z + h;
              Cft3D(D,10,b+20,10,B,qx,qy,qz);
              break;


    case 'K': D.x = A.x; D.y = A.y - a/2.0 + 3*a/5.0; D.z = A.z + h;
              Cft3D(D,10,b+20,10,B,qx,qy,qz);
              break;

    case 'L': D.x = A.x; D.y = A.y - a/2.0 + 4*a/5.0; D.z = A.z + h;
              Cft3D(D,10,b+20,10,B,qx,qy,qz);
              break;

    case 'M': D.x = A.x; D.y = A.y - a/2.0 + a; D.z = A.z + h;
              Cft3D(D,10,b+20,10,B,qx,qy,qz);
              break;

    }
  }
}

/*****************************/
/**          ����           **/
/*****************************/
void FangZi(Point3D A,int a,int b,int h,Point3D BB,float qx,float qy,float qz)
{
   int X,Y,Z,i,j,d;
   Point3D B,C;
   Depth dp[9],temp;

   X = A.x; Y = A.y; Z = A.z;         /*�����Ϣ��ʼ��*/
    Point3D_2D(&X,&Y,&Z,BB,qx,qy,qz);
   dp[0].s = 'A'; dp[0].d = Z;        /* ����������*/

   X = A.x + b/2.0 + 30; Y = A.y; Z = A.z - h/4.0;
     Point3D_2D(&X,&Y,&Z,BB,qx,qy,qz);
   dp[1].s = 'B'; dp[1].d = Z;        /*�ſ�����*/

   X = A.x + b/2.0 + a/12.0 + 60; Y = A.y - a/6.0; Z = A.z - h/4.0;
     Point3D_2D(&X,&Y,&Z,BB,qx,qy,qz);
   dp[2].s = 'C'; dp[2].d = Z;        /*��1�����*/

   X = A.x + b/2.0 + a/12.0 + 60; Y = A.y + a/6.0; Z = A.z - h/4.0;
      Point3D_2D(&X,&Y,&Z,BB,qx,qy,qz);
   dp[3].s = 'D'; dp[3].d = Z;        /*��2�����*/

   X = A.x; Y = A.y + a/2.0; Z = A.z;
      Point3D_2D(&X,&Y,&Z,BB,qx,qy,qz);
   dp[4].s = 'E'; dp[4].d = Z;       /* ����1����� */

   X = A.x; Y = A.y + a/2.0 + 25; Z = A.z - h/8.0;
       Point3D_2D(&X,&Y,&Z,BB,qx,qy,qz);
   dp[5].s = 'F'; dp[5].d = Z;       /* ��̨1����� */

   X = A.x; Y = A.y - a/2.0; Z = A.z;
       Point3D_2D(&X,&Y,&Z,BB,qx,qy,qz);
   dp[6].s = 'H'; dp[6].d = Z;       /* ����2����� */

   X = A.x; Y = A.y - a/2.0 - 25; Z = A.z - h/8.0;
       Point3D_2D(&X,&Y,&Z,BB,qx,qy,qz);
   dp[7].s = 'I'; dp[7].d = Z;         /* ��̨2����� */

   X = A.x; Y = A.y; Z = A.z + h/2.0;
       Point3D_2D(&X,&Y,&Z,BB,qx,qy,qz);
   dp[8].s = 'J'; dp[8].d = Z;         /*���������*/

    for(i = 0; i <=8; i++)                 /*  �������  */
   {
      for(j = i+1;j<=9;j++)
      {
         if(dp[i].d>dp[j].d)
         {
           temp.s = dp[i].s; temp.d = dp[i].d;
           dp[i].s = dp[j].s; dp[i].d = dp[j].d;
           dp[j].s = temp.s; dp[j].d = temp.d;
         }
      }
   }

  i = 0;
  while(i <= 9)      /*�����㷨*/
  {
    switch(dp[i++].s)
    {

       case'A': Cft3D(A,a,b,h,BB,qx,qy,qz); /* ������ */
                break;

       case'B': B.x = A.x + b/2.0; B.y = A.y; B.z = A.z - h/4.0; /* ���� */
                Rect_3DYZ(B,a/3.0,h/2.0,15,BB,qx,qy,qz);
                break;

       case'C': B.x = A.x + b/2.0 + a/12.0; B.y = A.y - a/6.0; B.z = A.z - h/4.0; /* ����1*/
                C.x = A.x + b/2.0; C.y = B.y; C.z = B.z;
                Cft3D_XuanZhuan(B,5,a/6.0,h/2.0,0,C,3,BB,qx,qy,qz);
                break;

       case'D': B.x = A.x + b/2.0 + a/12.0; B.y = A.y + a/6.0; B.z = A.z - h/4.0;  /* ����2*/
                C.x = A.x + b/2.0; C.y = B.y; C.z = B.z;
                Cft3D_XuanZhuan(B,5,a/6.0,h/2.0,0,C,3,BB,qx,qy,qz);
                break;

       case'E': B.x = A.x; B.y = A.y + a/2.0; B.z = A.z; /* ������1 */
                Rect_3DXZ(B,b/2.0,h/4.0,15,BB,qx,qy,qz);
                Rect_3DXZ(B,5,h/4.0,12,BB,qx,qy,qz); /* ���� */
                break;

       case'F': B.x = A.x; B.y = A.y + a/2.0 + 25; B.z = A.z - h/8.0; /* ��̨1*/
                Cft3D(B,50,b/2.0,5,BB,qx,qy,qz);
                break;

       case'H': B.x = A.x; B.y = A.y - a/2.0; B.z = A.z; /* ������2 */
                Rect_3DXZ(B,b/2.0,h/4.0,15,BB,qx,qy,qz);
                Rect_3DXZ(B,5,h/4.0,12,BB,qx,qy,qz); /* ���� */
                break;

       case'I': B.x = A.x; B.y = A.y - a/2.0 - 25; B.z = A.z - h/8.0; /* ��̨2*/
                Cft3D(B,50,b/2.0,5,BB,qx,qy,qz);
                break;

       case'J':  B.x = A.x; B.y = A.y; B.z = A.z + h/2.0;/* ���� */
                 Rect_3DXY(B,a + 100,b + 100,8,BB,qx,qy,qz);

                 B.x = A.x - b/2.0 - 50; B.y = A.y; B.z = A.z + h/2.0 + (b+100)/4.0;
                 C.x = B.x; C.y = B.y; C.z = A.z + h/2.0;
                 d = (b/2.0 + 50) * 2.0/sqrt(3);
                 Rect_3DYZ_XuanZhuan(B,a + 100,d,9,70,C,2,BB,qx,qy,qz);

                 B.x = A.x + b/2.0 + 50; B.y = A.y; B.z = A.z + h/2.0 + (b+100)/4.0;
                 C.x = B.x; C.y = B.y; C.z = A.z + h/2.0;
                 d = (b/2.0 + 50) * 2.0/sqrt(3);
                 Rect_3DYZ_XuanZhuan(B,a + 100,d,9,-70,C,2,BB,qx,qy,qz);

                                                           /* ��*/
                 B.x = A.x - b/2.0 - 50; B.y = A.y - a/2.0 - 50; B.z = A.z + h/2.0 + (b+100)/4.0;
                 C.x = B.x; C.y = B.y; C.z = A.z + h/2.0;
                 while(B.y <= A.y + a/2.0 + 50)
                {
                 Rect_3DYZ_XuanZhuan(B,5,d,12,70,C,2,BB,qx,qy,qz);
                 B.y = B.y + 40;
                 C.y = C.y + 40;

                 }

                  B.x = A.x + b/2.0 + 50; B.y = A.y - a/2.0 - 50; B.z = A.z + h/2.0 + (b+100)/4.0;
                  C.x = B.x; C.y = B.y; C.z = A.z + h/2.0;
                  while(B.y <= A.y + a/2.0 + 50)
                    {
                    Rect_3DYZ_XuanZhuan(B,5,d,12,-70,C,2,BB,qx,qy,qz);
                    B.y = B.y + 40;
                    C.y = C.y + 40;

                    }
                                                       /* �ݼ� */
                  B.x = A.x; B.y = A.y; B.z = A.z + h/2.0 + 45;
                  Cft3D(B,a + 100,10,10,BB,qx,qy,qz);
                  break;
        }
    }

}


/***************************/
/**���������������������� **/
/**       3D�ۺϻ���      **/
/**                       **/
/***************************/
void My3DGraphics_A(Point3D A,Point3D BB,float qx,float qy,float qz) /* �۲췽��Ϊ(qx,qy,qz)*/
{
   float a,b,h,d;
   int X,Y,Z,i,j;
   Point3D B,C;
   Depth dp[20],temp;
   setbkcolor(2);

   a = 220;
   b = 150;
   h = 200;

   B.x = A.x + b/2.0 + 250; B.y = A.y + a/2.0 + 100; B.z = A.z - 100;
   C.x = A.x + b/2.0 + 600; C.y = A.y; C.z = A.z - 100;
   i = 0;
   while(B.y>=A.x - a/2.0 - 1000)   /*ͤǰС·*/
   {
       Rect_3DXY_XuanZhuan(B,50,100,7,i,C,3,BB,qx,qy,qz);
       B.y = B.y - 80;
       i = i + 5;
   }


                                  /*�����Ϣ��ʼ��*/

   X = A.x; Y = A.y; Z = A.z;            /*��̨�����*/
      Point3D_2D(&X,&Y,&Z,BB,qx,qy,qz);
   dp[0].s = 'A'; dp[0].d = Z;

   X = A.x; Y = A.y; Z = A.z + 100 + h/2.0;    /*���ӵ����*/
      Point3D_2D(&X,&Y,&Z,BB,qx,qy,qz);
   dp[1].s = 'B'; dp[1].d = Z;

   X = A.x - a/2.0 - 500; Y = A.y + b/2.0 + 350; Z = A.z+250;
      Point3D_2D(&X,&Y,&Z,BB,qx,qy,qz); /* ��1�����*/
   dp[2].s = 'C'; dp[2].d = Z;

   X = A.x + b/2.0 + 150;Y = A.y; Z = A.z + 100 + 60; /*�������˵����*/
      Point3D_2D(&X,&Y,&Z,BB,qx,qy,qz);
   dp[3].s = 'D'; dp[3].d = Z;

   X = A.x - b/2.0 - 150; Y = A.y; Z = A.z + 100 + 60;
       Point3D_2D(&X,&Y,&Z,BB,qx,qy,qz);
   dp[4].s = 'E'; dp[4].d = Z;

   X = A.x;Y = A.y - a/2.0 - 200; Z = A.z + 100 + 60;
       Point3D_2D(&X,&Y,&Z,BB,qx,qy,qz);
   dp[5].s = 'F'; dp[5].d = Z;

   X = A.x;Y = A.y + a/2.0 + 200; Z = A.z + 100 + 60;
       Point3D_2D(&X,&Y,&Z,BB,qx,qy,qz);
   dp[6].s = 'H'; dp[6].d = Z;

   X = A.x; Y = A.y; Z = A.z + 20; /*̨��������*/
       Point3D_2D(&X,&Y,&Z,BB,qx,qy,qz);
   dp[7].s = 'I'; dp[7].d = Z;

   X = A.x;Y = A.y + a/2.0 + 400; Z = A.z - 50; /*�ұ�С̨�����*/
       Point3D_2D(&X,&Y,&Z,BB,qx,qy,qz);
   dp[8].s = 'J'; dp[8].d = Z;

   X = A.x;Y = A.y + a/2.0 + 450; Z = A.z + 100;    /*����*/
       Point3D_2D(&X,&Y,&Z,BB,qx,qy,qz);
   dp[9].s = 'K'; dp[9].d = Z;

   X = A.x; Y = A.y + a/2.0 + 300; Z = A.z + 150; /*�м������*/
       Point3D_2D(&X,&Y,&Z,BB,qx,qy,qz);
   dp[10].s = 'M'; dp[10].d = Z;

   X = A.x + a/2.0 + 900; Y = A.y; Z = A.z; /*��2*/
        Point3D_2D(&X,&Y,&Z,BB,qx,qy,qz);
   dp[11].s = 'N'; dp[11].d = Z;

   X = A.x + 400; Y = A.y + a/2.0 + 400; Z = A.z + 75;   /*С̨�ϵ�����*/
        Point3D_2D(&X,&Y,&Z,BB,qx,qy,qz);
   dp[12].s = 'O'; dp[12].d = Z;

   X = A.x - 500;Y = A.y + a/2.0 + 400; Z = A.z + 75;
        Point3D_2D(&X,&Y,&Z,BB,qx,qy,qz);
   dp[13].s = 'P'; dp[13].d = Z;

   X = A.x;Y = A.y + a/2.0 + 580; Z = A.z + 65;
       Point3D_2D(&X,&Y,&Z,BB,qx,qy,qz);
   dp[14].s = 'Q'; dp[14].d = Z;

   X = A.x + a/2.0 + 900; Y = A.y - 200; Z = A.z;  /*����2�����*/
       Point3D_2D(&X,&Y,&Z,BB,qx,qy,qz);
   dp[15].s = 'R'; dp[15].d = Z;



   for(i = 0; i <=14; i++)                 /*  �������*/
   {
      for(j = i+1;j<=15;j++)
      {
         if(dp[i].d>=dp[j].d)
         {
           temp.s = dp[i].s; temp.d = dp[i].d;
           dp[i].s = dp[j].s; dp[i].d = dp[j].d;
           dp[j].s = temp.s; dp[j].d = temp.d;
         }
      }
   }

  i = 0;
  while(i <= 15)      /*�����㷨*/
  {
    switch(dp[i++].s)
    {
       case'A':  Cft3D(A,a + 400,b + 300,h,BB,qx,qy,qz);    /* ��̨*/
                 break;

       case'B':  B.x = A.x; B.y = A.y; B.z = A.z + 200;
                 FangZi(B,a,b,h,BB,qx,qy,qz);                  /* ���� */
                 break;

       case'C':  B.x = A.x - a/2.0 - 700; B.y = A.y + b/2.0 + 350; B.z = A.z;
                 Shu(B,100,BB,qx,qy,qz);                                   /*��1*/
                 break;

       case'D':  B.x = A.x + b/2.0 + 150;B.y = A.y; B.z = A.z + 100 + 55;/*��������*/
                 LanGan_Y(B,a + 380,65,BB,qx,qy,qz);
                 break;

       case'E':  B.x = A.x - b/2.0 - 150;B.y = A.y; B.z = A.z + 100 + 55;
                 LanGan_Y(B,a + 380,65,BB,qx,qy,qz);
                 break;

       case'F':  B.x = A.x;B.y = A.y - a/2.0 - 200; B.z = A.z + 100 + 55;
                 LanGan_X(B,b + 280,65,BB,qx,qy,qz);
                 break;

       case'H':  B.x = A.x + b/3.0 + 100;B.y = A.y + a/2.0 + 200; B.z = A.z + 100 + 55;
                 LanGan_X(B,(b + 280)/3.0,65,BB,qx,qy,qz);

                 B.x = A.x - b/3.0 - 100;B.y = A.y + a/2.0 + 200; B.z = A.z + 100 + 55;
                 LanGan_X(B,(b + 280)/3.0,65,BB,qx,qy,qz);
                 break;

       case'I':  B.x = A.x;B.y = A.y; B.z = A.z + 100;     /*̨�ϵ���*/
                 Rect_3DXY(B,a + 400,b + 300,6,BB,qx,qy,qz);
                 break;

       case'J':  B.x = A.x;B.y = A.y + a/2.0 + 400; B.z = A.z - 50; /*�ұߵ�С̨*/
                 Cft3D(B,400,1000,100,BB,qx,qy,qz);

                 B.x = A.x;B.y = A.y + a/2.0 + 400; B.z = A.z; /*�ұ�̨�ϵ���*/
                 Rect_3DXY(B,400,1000,6,BB,qx,qy,qz);
                 break;

       case'K':  B.x = A.x;B.y = A.y + a/2.0 + 450; B.z = A.z; /*����1*/
                 DengZi(B,100,250,50,BB,qx,qy,qz);
                 break;

       case'M':  B.x = A.x;B.y = A.y + a/2.0 + 250; B.z = A.z + 25; /*�м������*/
                 Cft3D(B,100,b/3.0 + 100,50,BB,qx,qy,qz);

                 B.x = A.x;B.y = A.y + a/2.0 + 250; B.z = A.z + 50;
                 Rect_3DXY(B,100,b/3.0 + 100,6,BB,qx,qy,qz);
                 break;


       case'N':  B.x = A.x + a/2.0 + 900; B.y = A.y; B.z = A.z;
                 Shu(B,100,BB,qx,qy,qz);                                   /*��2*/
                 break;

       case'O':  B.x = A.x + 480;B.y = A.y + a/2.0 + 400; B.z = A.z + 65; /*С̨�ϵ�����*/
                 LanGan_Y(B,380,60,BB,qx,qy,qz);
                 break;

       case'P':  B.x = A.x - 480;B.y = A.y + a/2.0 + 400; B.z = A.z + 65;
                 LanGan_Y(B,380,60,BB,qx,qy,qz);
                 break;

       case'Q':  B.x = A.x;B.y = A.y + a/2.0 + 600; B.z = A.z + 65;
                 LanGan_X(B,980,60,BB,qx,qy,qz);
                 break;

       case'R':  B.x = A.x + a/2.0 + 900; B.y = A.y - 200; B.z = A.z; /*����2*/
                 DengZi(B,100,250,50,BB,qx,qy,qz);
                 break;

    }
  }

}


void My3DGraphics_B(Point3D A,Point3D B,float qx,float qy,float qz)
{
   int X,Y,Z;
   Depth dp[20],temp;
   CFT Cft1,Cft2,Cft3,Cft4,Cft5,Cft6;
   Point3D D1,D2,T;
   Point B1,B2;
   int a,b,i,j,k;

   setbkcolor(2);


   a = 400; b = 800;

   Rect_3DXY(A,a,b,15,B,qx,qy,qz);    /*��*/
   Rect_3DXY(A,a - 10,b - 10,6,B,qx,qy,qz);

   Rect_3DXY(A,a,5,15,B,qx,qy,qz); /* �м��һ������*/

   k = 80;

   while(k>=75)  /* �м��Բ */
   {
     Yuan_3DXY(A,k,15,B,qx,qy,qz);
     k--;
   }

   D2.x = A.x + b/4.0; D2.y = A.y; D2.z = A.z;/* ����1�µĵ������߼���б�� */
   k = 70;
   while(k>=65)
   {
      Yuan_3DXY(D2,k,15,B,qx,qy,qz);
      k--;
   }
   D2.x = A.x + b/4.0 + 35; D2.y = A.y; D2.z = A.z;
   Rect_3DXY(D2,200,70,6,B,qx,qy,qz);

   D2.x = A.x + 3 * b/8.0; D2.y = A.y - 85; D2.z = A.z;  /* б�� */
   Rect_3DXY_XuanZhuan(D2,5,b/4.0 + 6,15,-10,D2,3,B,qx,qy,qz);


   D2.x = A.x + 3 * b/8.0; D2.y = A.y + 85; D2.z = A.z;  /* б�� */
   Rect_3DXY_XuanZhuan(D2,5,b/4.0 + 6,15,10,D2,3,B,qx,qy,qz);



   D2.x = A.x + b/4.0; D2.y = A.y; D2.z = A.z;  /* ����1�µ�Բ */
   k = 50;
   while(k>=45)
   {
      Yuan_3DXY(D2,k,15,B,qx,qy,qz);
      k--;
   }
   Rect_3DXY(D2,100,5,15,B,qx,qy,qz);


   D2.x = A.x - b/4.0; D2.y = A.y; D2.z = A.z;/* ����2�µĵ������߼���б�� */
   k = 70;
   while(k>=65)
   {
      Yuan_3DXY(D2,k,15,B,qx,qy,qz);
      k--;
   }
   D2.x = A.x - b/4.0 - 35; D2.y = A.y; D2.z = A.z;
   Rect_3DXY(D2,200,70,6,B,qx,qy,qz);

   D2.x = A.x - 3 * b/8.0; D2.y = A.y - 85; D2.z = A.z;  /* б�� */
   Rect_3DXY_XuanZhuan(D2,5,b/4.0 + 6,15,10,D2,3,B,qx,qy,qz);


   D2.x = A.x - 3 * b/8.0; D2.y = A.y + 85; D2.z = A.z;  /* б�� */
   Rect_3DXY_XuanZhuan(D2,5,b/4.0 + 6,15,-10,D2,3,B,qx,qy,qz);


   D2.x = A.x - b/4.0; D2.y = A.y; D2.z = A.z;  /* ����2�µ�Բ*/
   k = 50;
   while(k>=45)
   {
      Yuan_3DXY(D2,k,15,B,qx,qy,qz);
      k--;
   }
   Rect_3DXY(D2,100,5,15,B,qx,qy,qz);
                                               /*�����Ϣ��ʼ��*/

   X = A.x - b/2.0 + 20; Y = A.y; Z = A.z + 105;   /*����2֧�ܵ����*/
     Point3D_2D(&X,&Y,&Z,B,qx,qy,qz);
   dp[0].s = 'A'; dp[0].d = Z;

   X = A.x - b/2.0 + 40; Y = A.y;  Z = A.z + 200;   /* ����2����� */
      Point3D_2D(&X,&Y,&Z,B,qx,qy,qz);
   dp[1].s = 'B'; dp[1].d = Z;


   X = A.x - b/2.0 + 60; Y = A.y; Z = A.z + 200;   /* ����2������1����� */
      Point3D_2D(&X,&Y,&Z,B,qx,qy,qz);
   dp[2].s = 'C'; dp[2].d = Z;


   X = A.x - b/2.0 + 90; Y = A.y; Z = A.z + 205;   /* ����2����� */
       Point3D_2D(&X,&Y,&Z,B,qx,qy,qz);
   dp[3].s = 'D'; dp[3].d = Z;


   X = A.x + b/2.0 - 80; Y = A.y; Z = A.z + 200;   /* ����1����� */
      Point3D_2D(&X,&Y,&Z,B,qx,qy,qz);
   dp[4].s = 'E'; dp[4].d = Z;

   X = A.x + b/2.0 - 50;  Y = A.y; Z = A.z + 220;   /* ����1������1����� */
      Point3D_2D(&X,&Y,&Z,B,qx,qy,qz);
   dp[5].s = 'F'; dp[5].d = Z;

   X = A.x + b/2.0 - 30; Y = A.y; Z = A.z + 200;     /* ����1����� */
      Point3D_2D(&X,&Y,&Z,B,qx,qy,qz);
   dp[6].s = 'H'; dp[6].d = Z;

   X = A.x + b/2.0 - 10; Y = A.y;  Z = A.z + 100;  /* ����1��֧��1�����  */
      Point3D_2D(&X,&Y,&Z,B,qx,qy,qz);
   dp[7].s = 'I'; dp[7].d = Z;

   for(i = 0; i <=6; i++)                 /*  �������*/
   {
      for(j = i+1;j<=7;j++)
      {
         if(dp[i].d>=dp[j].d)
         {
           temp.s = dp[i].s; temp.d = dp[i].d;
           dp[i].s = dp[j].s; dp[i].d = dp[j].d;
           dp[j].s = temp.s; dp[j].d = temp.d;
         }
      }
   }

  i = 0;
  while(i <= 7)      /*�����㷨*/
  {
    switch(dp[i++].s)
    {


     case'A': Cft4.A.x = A.x - b/2.0 + 30;            /*����2��֧��*/
              Cft4.A.y = A.y;
              Cft4.A.z = A.z + 105;
              Cft4.a = 10;
              Cft4.b = 10;
              Cft4.h = 200;
              Cft3D(Cft4.A,Cft4.a,Cft4.b,Cft4.h,B,qx,qy,qz);
              break;

     case'B': Cft2.A.x = A.x - b/2.0 + 40;   /* ����2 */
              Cft2.A.y = A.y;
              Cft2.A.z = A.z + 200;
              Cft2.a = 80;
              Cft2.b = 10;
              Cft2.h = 80;
              Cft3D(Cft2.A,Cft2.a,Cft2.b,Cft2.h,B,qx,qy,qz);

              break;

     case'C': Cft1.A.x = A.x - b/2.0 + 50;     /* ����2������1 */
              Cft1.A.y = A.y;
              Cft1.A.z = A.z + 210;
              Cft1.a = 10;
              Cft1.b = 15;
              Cft1.h = 10;
              Cft3D(Cft1.A,Cft1.a,Cft1.b,Cft1.h,B,qx,qy,qz);
              break;



    case'D':  D1.x = A.x - b/2.0 + 80; D1.y = A.y; D1.z = A.z + 205; /* ����2 */
              k = 20;
              while(k>=17)
              {
                Yuan_3DXY(D1,k,14,B,qx,qy,qz);
                   k--;
              }
              break;

    case'E':  D1.x = A.x + b/2.0 - 80; D1.y = A.y; D1.z = A.z + 200;
              k = 20;
               while(k>=17)        /* ����1 */
               {
                Yuan_3DXY(D1,k,14,B,qx,qy,qz);
                 k--;
               }

             break;

    case'F': Cft1.A.x = D1.x + 30;     /* ����1������1 */
             Cft1.A.y = D1.y;
             Cft1.A.z = D1.z+5;
             Cft1.a = 10;
             Cft1.b = 15;
             Cft1.h = 10;
             Cft3D(Cft1.A,Cft1.a,Cft1.b,Cft1.h,B,qx,qy,qz);

             break;

    case'H': Cft2.A.x = D1.x + 40;   /* ����1 */
             Cft2.A.y = D1.y;
             Cft2.A.z = D1.z;
             Cft2.a = 80;
             Cft2.b = 10;
             Cft2.h = 80;
             Cft3D(Cft2.A,Cft2.a,Cft2.b,Cft2.h,B,qx,qy,qz);
             break;

    case'I': Cft3.A.x = D1.x + 720;    /* ����1��֧�� */
             Cft3.A.y = D1.y+30;
             Cft3.A.z = D1.z - 100;
             Cft3.a = 10;
             Cft3.b = 10;
             Cft3.h = 200;
             Cft3D(Cft3.A,Cft3.a,Cft3.b,Cft3.h,B,qx,qy,qz);
             break;
    }
  }

}

void My3DGraphics_C(Point3D A,Point3D B,float qx,float qy,float qz)       /* ��ûд! */
{

}

/***************************/
/****                   ****/
/****   ͼ����ʾ����    ****/
/****                   ****/
/***************************/

/** ��ʾһ��ֱ�� **/
void Show_L(Line L)
{
   LineDDA(L.A,L.B,L.color);
}

/** ��ʾһ��Բ     **/


void Show_C(Yuan C)
{
   MidPointCircle_3((C.o),C.r,C.color);
}

/** ��ʾһ����Բ **/
void Show_E(TuoYuan E)
{
  MidPointEllipse(E.o,E.a,E.b,E.color);
}

/** ��ʾһ���� **/
void Show_Q(Qiu Q)
{
    QiuEC(Q.o,Q.r,Q.color);
}

/** ��ʾһ������ **/
void Show_R(Rect R)
{
   Rectangle(R.LeftTop,R.RightDown,R.color);
}

/** ��ʾһ��3D������  **/
Show3D_R(Rect3D R,Point3D B,float qx,float qy,float qz)
{
   Rect_3DXY(R.A,R.a,R.b,R.color,B,qx,qy,qz);

}

/* ��ʾһ��������*/
void Show3D_Cft(CFT C,Point3D B,float qx,float qy,float qz)/* �۲췽��Ϊ(qx,qy,qz)   */
{
   Cft3D(C.A,C.a,C.b,C.h,B,qx,qy,qz);
}



main()                  /* MAIN������ */
{
  int i;
  float k,h;

  Point PointA,PointB;
  Point * P;
  Line L1,L2;
  Yuan C1,C2;
  TuoYuan E1,E2,E3,E4;
  Qiu Q1,Q2,Q3;
  Rect R1,R2;

  Point3D D1,D2,D3;
  CFT Cft1,Cft2;
  SLZ S1,S2;
  SLZU Sz1,Sz2;


  int graphdriver = DETECT,graphmode;
  registerbgidriver(EGAVGA_driver);
  initgraph(&graphdriver,&graphmode,"");

  D1.x = 200;
  D1.y = 250;
  D1.z = -150;

   D2.x = 250;
   D2.y = 320;
   D2.z = -150;

My3DGraphics_A(D1,D1,-45,45,70);
/* Cft3D(D1,220,150,200,D1,-45,45,70); */
/* My3DGraphics_B(D2,D2,35,90,45); */
/* LanGan_X(D1,400,80,D1,-30,45,70); */
/* LanGan_Y(D1,200,80,D1,-45,45,70); */
/* FangZi(D1,300,200,400,D1,-45,45,70); */
/* Shu(D1,80,D1,-45,45,70); */
/* DengZi(D1,200,100,50,D1,-45,45,70); */
  getch();
  closegraph();
}
                  /* ������� */


 /* PointA.x = 550,PointA.y = 100;PointB.x = 100,PointB.y = 100;

  L1.A = PointA;                     /* ���Բ��� */
  L1.B = PointB;
  L1.color = 2;

    Show_L(L1);
MidPointLine(L1.A,L1.B,15);


  C1.o.x = 300;
  C1.o.y = 300;
  C1.r = 60;

  C1.color = 4;

    Show_C(C1);
    PointA.x = 300;
    PointA.y = 240;
    k = 0.001;
       while(0)
       {
         C1.color = 0;
         MidPointCircle_CuoQie(C1,PointA,k - 0.001,0);
         C1.color = 4;
         MidPointCircle_CuoQie(C1,PointA,k,0);
         k = k + 0.001;
       }

  E1.o.x = 250;
  E1.o.y = 250;
  E1.a = 60; E1.b = 30;
  E1.color = 11;
     Show_E(E1);


  E2.o.x = 380;
  E2.o.y = 250;
  E2.a = 60; E2.b = 30;
  E2.color = 11;
      Show_E(E2);


  E3.o.x = 315;
  E3.o.y = 180;
  E3.a = 30; E3.b = 60;
  E3.color = 11;
       Show_E(E3);

  E4.o.x = 315;
  E4.o.y = 320;
  E4.a = 30; E4.b = 60;
  E4.color = 11;
       Show_E(E4);

  PointA.x = 315;
  PointA.y = 248;

  R1.LeftTop.x = 313;
  R1.LeftTop.y = 248;
  R1.RightDown.x = 319;
  R1.RightDown.y = 450;
  R1.color = 2;
    Show_R(R1);

      k = 1;
      while(0)
      {
         E1.color = 0;
         E2.color = 0;
         E3.color = 0;
         E4.color = 0;
         MidPointEllipse_XuanZhuan(E1,PointA,k-1);
         MidPointEllipse_XuanZhuan(E2,PointA,k-1);
         MidPointEllipse_XuanZhuan(E3,PointA,k-1);
         MidPointEllipse_XuanZhuan(E4,PointA,k-1);
         Show_R(R1);
         E1.color = 11;
         E2.color = 11;
         E3.color = 11;
         E4.color = 11;
         MidPointEllipse_XuanZhuan(E1,PointA,k);
         MidPointEllipse_XuanZhuan(E2,PointA,k);
         MidPointEllipse_XuanZhuan(E3,PointA,k);
         MidPointEllipse_XuanZhuan(E4,PointA,k);
         k = k + 1;
      }
     PointB.x =190 ;
     PointB.y = 250;
        E1.color = 4;
         MidPointEllipse_FangSuo(E1,PointB,2,2);
       MidPointEllipse_PinYi(E1,150,0);




  Q1.o.x = 500;
  Q1.o.y = 400;
  Q1.r = 60;
  Q1.color = 11;

    Show_Q(Q1);

  R1.LeftTop.x = 50;
  R1.LeftTop.y = 150;
  R1.RightDown.x = 300;
  R1.RightDown.y = 200;
  R1.color = 11;
    Show_R(R1);

    Rectangle_XuanZhuan(R1,32);




  S1.A.x = 350;
  S1.A.y = 250;
  S1.A.z = 100;
  S1.r = 400;
  S1.h = 100;

  Show3D_S(S1,45,45,60);

  D1.x = 200;
  D1.y = 250;
  D1.z = -150;

  /*  D2.x = 250; */
  /*  D2.y = 320; */
  /*  D2.z = -150; */

My3DGraphics_A(D1,D1,-45,45,70);

/* My3DGraphics_B(D2,D2,35,90,45); */


/* My3DGraphics_C(D1,40,50,70); */

/* My3DGraphics_D(D1,45,45,45); */
  getch();
  closegraph();

}*/


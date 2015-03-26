// zhaochaDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "zhaocha.h"
#include "zhaochaDlg.h"
#include ".\zhaochadlg.h"
#define WIDTHBYTES(i) ((i+31)/32*4)
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define	WM_GETDSTDIFFRENT	WM_USER+11					//��ȡ��ͬ������Ϣ
#define	WM_USER_CLOSEWINDOW	WM_USER+12				
// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialog
{
public:
		CAboutDlg();

		// �Ի�������
		enum { IDD = IDD_ABOUTBOX };

protected:
		virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

		// ʵ��
protected:
		DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
		CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()




_declspec(dllimport) HHOOK SetHook(HWND m_hwndHelper);

// CzhaochaDlg �Ի���



CzhaochaDlg::CzhaochaDlg(CWnd* pParent /*=NULL*/)
: CDialog(CzhaochaDlg::IDD, pParent)
{
		m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CzhaochaDlg::DoDataExchange(CDataExchange* pDX)
{
		CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CzhaochaDlg, CDialog)
		ON_WM_SYSCOMMAND()
		ON_WM_PAINT()
		ON_WM_QUERYDRAGICON()
		//}}AFX_MSG_MAP
		ON_BN_CLICKED(IDC_BTN_FIND, OnBnClickedBtnFind)
		ON_MESSAGE(WM_GETDSTDIFFRENT,GoToGetDiffrent)
		ON_MESSAGE(WM_USER_CLOSEWINDOW,CloseFindWindow)
		ON_WM_CLOSE()
END_MESSAGE_MAP()


// CzhaochaDlg ��Ϣ�������

BOOL CzhaochaDlg::OnInitDialog()
{
		CDialog::OnInitDialog();

		// ��\������...\���˵�����ӵ�ϵͳ�˵��С�

		// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
		ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
		ASSERT(IDM_ABOUTBOX < 0xF000);

		CMenu* pSysMenu = GetSystemMenu(FALSE);
		if (pSysMenu != NULL)
		{
				CString strAboutMenu;
				strAboutMenu.LoadString(IDS_ABOUTBOX);
				if (!strAboutMenu.IsEmpty())
				{
						pSysMenu->AppendMenu(MF_SEPARATOR);
						pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
				}
		}

		// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
		//  ִ�д˲���
		SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
		SetIcon(m_hIcon, FALSE);		// ����Сͼ��

		//����HOOK
		m_lowHook = NULL;
		m_lowHook = SetHook(m_hWnd);

		m_btn.MoveWindow(50,50,50,50);
		m_btn.SubclassDlgItem(IDC_BTN_FIND,this);
		m_btn.SetIcon(IDR_MAINFRAME);
		m_btn.DrawBorder(FALSE);
		m_btn.DrawTransparent(TRUE);
		return TRUE;  // ���������˿ؼ��Ľ��㣬���򷵻� TRUE
}

void CzhaochaDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
		if ((nID & 0xFFF0) == IDM_ABOUTBOX)
		{
				CAboutDlg dlgAbout;
				dlgAbout.DoModal();
		}
		else
		{
				CDialog::OnSysCommand(nID, lParam);
		}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CzhaochaDlg::OnPaint() 
{
		if (IsIconic())
		{
				CPaintDC dc(this); // ���ڻ��Ƶ��豸������

				SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

				// ʹͼ���ڹ��������о���
				int cxIcon = GetSystemMetrics(SM_CXICON);
				int cyIcon = GetSystemMetrics(SM_CYICON);
				CRect rect;
				GetClientRect(&rect);
				int x = (rect.Width() - cxIcon + 1) / 2;
				int y = (rect.Height() - cyIcon + 1) / 2;

				// ����ͼ��
				dc.DrawIcon(x, y, m_hIcon);
		}
		else
		{
				CDialog::OnPaint();
		}

}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù����ʾ��
HCURSOR CzhaochaDlg::OnQueryDragIcon()
{
		return static_cast<HCURSOR>(m_hIcon);
}


/************************************************************************/
/*                                                                      */
/************************************************************************/
BOOL CzhaochaDlg::PreTranslateMessage( MSG* pMsg)
{
		if( pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE)
		{
				return TRUE;      //���ε�Esc��
		}
		if( pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
		{
				return TRUE;      //���ε�Enter��
		}

		return CDialog::PreTranslateMessage( pMsg );
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
void CzhaochaDlg::on_mouseProc( int event, int x, int y, int flags, void* param)
{
		myParam *myparam = (myParam*)param;
		if(event==CV_EVENT_LBUTTONDOWN)
		{
				::PostMessage(myparam->hwnd,WM_LBUTTONDOWN,0,MAKELONG(x+myparam->x,y+myparam->y));
		}
}


/************************************************************************/
/*                                                                      */
/************************************************************************/
void CzhaochaDlg::GetLocation(IplImage* Img,int &x1,int &x2,int &x3,int tempY)
{

		CvScalar backColor;				//�м�ı���ɫ

		for (int i=4;i< 600;i++)
		{
				if (x1 == 0) 
				{
						CvScalar tempColor = cvGet2D(Img,tempY,i);
						CvScalar tempColor1 = cvGet2D(Img,tempY,i-1);
						CvScalar tempColor2 = cvGet2D(Img,tempY,i-2);
						if (tempColor.val[0] < 100 &&					//�ҵ�ͼ1����ʼX����
								tempColor.val[1] < 100 && 
								tempColor.val[2] < 100 &&
								tempColor1.val[0] > 200 && 
								tempColor1.val[1] > 200 && 
								tempColor1.val[2] > 200 &&
								tempColor2.val[0] > 200 && 
								tempColor2.val[1] > 200 && 
								tempColor2.val[2] > 200 )
						{
								x1 = i;
								backColor = tempColor;			//���м�ı���ɫ��ֵ
								continue;
						}
				}
				else 
				{
						if (x2 == 0)
						{
								CvScalar tempColor = cvGet2D(Img,tempY,i);
								CvScalar tempColor1 = cvGet2D(Img,tempY,i-1);
								if (tempColor.val[0] == backColor.val[0] &&				//��ͼ1����X����
										tempColor.val[1] == backColor.val[1] && 
										tempColor.val[2] == backColor.val[2] &&
										tempColor1.val[0] != backColor.val[0] && 
										tempColor1.val[1] != backColor.val[1] && 
										tempColor1.val[2] != backColor.val[2]	)
								{
										x2 = i;
										continue;
								}
						}
						else
						{
								if (x3 == 0)
								{
										CvScalar tempColor = cvGet2D(Img,tempY,i);
										CvScalar tempColor1 = cvGet2D(Img,tempY,i+1);
										CvScalar tempColor2 = cvGet2D(Img,tempY,i+2);
										if (tempColor.val[0] == backColor.val[0] && 
												tempColor.val[1] == backColor.val[1] && 
												tempColor.val[2] == backColor.val[2] &&
												tempColor1.val[0] == backColor.val[0] && 
												tempColor1.val[1] == backColor.val[1] && 
												tempColor1.val[2] == backColor.val[2] &&
												tempColor2.val[0] != backColor.val[0] && 
												tempColor2.val[1] != backColor.val[1] && 
												tempColor2.val[2] != backColor.val[2] )
										{
												x3 = i+2;
												break;
										}
								}
						}
				}
		}

}
/************************************************************************/
/*                                                                      */
/************************************************************************/
//λͼ��ʽ����ͼƬ�Ĳ�������
bool CzhaochaDlg::SaveNewPicture(CString filePath,LPBYTE m_newBmpData,long filelength)
{
		CFile file;
		if( !file.Open( filePath, CFile::modeWrite|CFile::modeCreate))
				return false;
		file.Write(m_newBmpData,filelength);
		return true;
}



/************************************************************************/
/*                                                                      */
/************************************************************************/
//��ȡ��ϷͼƬ����
void CzhaochaDlg::GetSelectWindowPic(HWND hwnd)
{
		LPBYTE pDst = NULL;
		CDC *pDC;
		pDC = CDC::FromHandle(::GetDC(hwnd));
		CRect rc;
		CWnd::FromHandle(hwnd)->GetClientRect(&rc);

		int BitPerPixel = pDC->GetDeviceCaps(BITSPIXEL);//�����ɫģʽ

		int Width = rc.Width();//pDC->GetDeviceCaps(HORZRES);
		int Height = rc.Height();//pDC->GetDeviceCaps(VERTRES);

		CDC memDC;//�ڴ�DC
		memDC.CreateCompatibleDC(pDC);

		CBitmap memBitmap, *oldmemBitmap;//��������Ļ���ݵ�bitmap
		memBitmap.CreateCompatibleBitmap(pDC, Width, Height);

		oldmemBitmap = memDC.SelectObject(&memBitmap);//��memBitmapѡ���ڴ�DC
		memDC.BitBlt(0, 0, Width, Height, pDC, 0, 0, SRCCOPY);//������Ļͼ���ڴ�DC

		//���´��뱣��memDC�е�λͼ���ļ�
		BITMAP bmp;
		memBitmap.GetBitmap(&bmp);//���λͼ��Ϣ

		BITMAPFILEHEADER bfh = {0};//λͼ�ļ�ͷ
		bfh.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);//��λͼ���ݵ�ƫ����
		bfh.bfSize = bfh.bfOffBits + bmp.bmWidthBytes * bmp.bmHeight;//�ļ��ܵĴ�С
		bfh.bfType = (WORD)0x4d42;

		BITMAPINFOHEADER bih = {0};//λͼ��Ϣͷ
		bih.biBitCount = bmp.bmBitsPixel;//ÿ�������ֽڴ�С
		bih.biCompression = BI_RGB;
		bih.biHeight = bmp.bmHeight;//�߶�
		bih.biPlanes = 1;
		bih.biSize = sizeof(BITMAPINFOHEADER);
		bih.biSizeImage = bmp.bmWidthBytes * bmp.bmHeight;//ͼ�����ݴ�С
		bih.biWidth = bmp.bmWidth;//���

		BYTE *p = new BYTE[bmp.bmWidthBytes * bmp.bmHeight];//�����ڴ汣��λͼ����
		GetDIBits(memDC.m_hDC, (HBITMAP) memBitmap.m_hObject, 0, Height, p,
				(LPBITMAPINFO) &bih, DIB_RGB_COLORS);//��ȡλͼ����
		pDst = new BYTE[bfh.bfOffBits + bmp.bmWidthBytes * bmp.bmHeight];
		memcpy(pDst,&bfh,sizeof(BITMAPFILEHEADER));
		memcpy(pDst +sizeof(BITMAPFILEHEADER),&bih,sizeof(BITMAPINFOHEADER));
		memcpy(pDst+bfh.bfOffBits,p,bmp.bmWidthBytes * bmp.bmHeight);

		SaveNewPicture(_T("srcPic.bmp"),pDst,bfh.bfOffBits+bmp.bmWidthBytes * bmp.bmHeight);
		delete []p;
		delete []pDst;
		memDC.SelectObject(oldmemBitmap);
}



/************************************************************************/
/*                                                                      */
/************************************************************************/
//�Ҳ����
void CzhaochaDlg::OnBnClickedBtnFind()
{
		
		IplImage *Img;

		HWND hackHwnd = FindWindow(_T("#32770"),_T("������Ҳ�"))->GetSafeHwnd();		//��ȡ�Ҳ���Ϸ�Ĵ��ھ��

		if (hackHwnd == NULL)					//���û���ҵ�Ŀ�괰��
		{
				return;
		}
		
		GetSelectWindowPic(hackHwnd);			//��ȡ��ϷͼƬ


		if((Img = cvLoadImage("srcPic.bmp", 1)) == 0)	//���û���ҵ�ͼƬ
		{
				return;
		}
		int x1 = 0;				//��һ��ͼ����X����
		int x2 = 0;				//��һ��ͼ����X����
		int x3 = 0;				//�ڶ���ͼ����X����
		int width = 0;			//ͼ���
		int y = 190;			//ͼ��Y������ʼ����

		int tempY = 200           ;		//..........................

		while((x1 == 0 || x2 == 0 || x3 == 0 || width <= 0) && tempY <270)				//��ȡ��ͼ�����ʼ�ͽ���λ��
		{
				x1=0;
				x2=0;
				x3 = 0;
				width = 0;
				GetLocation(Img,x1,x2,x3,tempY);
				width = x2 - x1;
				tempY += 5;
		}

		
		int height = 450;				//ͼ��߶�
		if (x1 == 0 || x2 == 0 || x3 == 0 || width <= 0)		//����Ҳ�ʧ��
		{
				return;
		}

		//�������꣬�����Ҳ���
		x1 += 1;
		width -= 1;
		
		//������С���εĲ���
		int ijStart = 10;
		width -= 10;
		height -= 10;

		CvMat *dst;
		CvRect subRect;
		dst=cvCreateMat(width,height,CV_8UC3);
		subRect =cvRect(x1,y,width,height);
		cvGetSubRect(Img,dst,subRect);
		IplImage *dstLeft;
		dstLeft=cvCreateImage(cvSize(width,height),IPL_DEPTH_8U,3);
		cvGetImage(dst,dstLeft);

		subRect =cvRect(x3,y,width,height);
		cvGetSubRect(Img,dst,subRect);
		IplImage *dstRight;
		dstRight=cvCreateImage(cvSize(width,height),IPL_DEPTH_8U,3);
		cvGetImage(dst,dstRight);

		IplImage *dstXor;
		dstXor=cvCreateImage(cvSize(width,height),IPL_DEPTH_8U,3);
		cvXor(dstLeft,dstRight,dstXor);

		cvSaveImage("dstPic.bmp",dstXor);
		cvReleaseImage(&Img);

		IplImage *dstXorBinary;

		if((dstXorBinary = cvLoadImage("dstPic.bmp", 0)) == 0)
				return;
		cvThreshold(dstXorBinary,dstXorBinary,1,255,CV_THRESH_BINARY);
		dstXorBinary->origin = 1;

		int xRight;		//��С������������
		int xLeft;		//��С������������
		int yTop;		//��С������������
		int yBottom;	//��С������������

/************************************************************************/
/*���²�����ȡ��ͬ���������ɵ���С���Σ�Ϊ���ò�ͬ���򴰿���С��������ƣ�*/
/************************************************************************/
		for (int i=ijStart;i<width;i++)
				for (int j=ijStart;j<height;j++)
				{
						CvScalar tempColor = cvGet2D(dstXorBinary,j,i);
						if (tempColor.val[0] != 0)
						{
								xRight = i;
								break;
						}
				}

		for (int i=width-1;i>=ijStart ;i--)
			for (int j=ijStart;j<height;j++)
			{
					CvScalar tempColor = cvGet2D(dstXorBinary,j,i);
					if (tempColor.val[0] != 0)
					{
							xLeft = i;
							break;
					}
			}		

		for (int j=ijStart;j<height;j++)
			for (int i=ijStart;i<width;i++)
				{
						CvScalar tempColor = cvGet2D(dstXorBinary,j,i);
						if (tempColor.val[0] != 0)
						{
								yBottom = j;
								break;
						}
				}

		for (int j=height-1;j>=ijStart;j--)
			for (int i=ijStart;i<width;i++)
			{
				CvScalar tempColor = cvGet2D(dstXorBinary,j,i);
				if (tempColor.val[0] != 0)
				{
						yTop = j;
						break;
				}
			}
/************************************************************************/
/*���ϲ�����ȡ��ͬ���������ɵ���С���Σ�Ϊ���ò�ͬ���򴰿���С��������ƣ�*/
/************************************************************************/
			//������λ�ȡ�����˳�
			if (xRight - xLeft< 0 || yBottom-yTop < 0)
			{
					return;
			}

			//�õ���С����ͼ����ʾ
			dst=cvCreateMat(xRight - xLeft,yBottom-yTop,CV_8UC1);
			subRect =cvRect(xLeft,yTop,xRight - xLeft,yBottom-yTop);
			cvGetSubRect(dstXorBinary,dst,subRect);

			IplImage *dstImage;
			dstImage=cvCreateImage(cvSize(xRight - xLeft,yBottom-yTop),IPL_DEPTH_8U,1);
			cvGetImage(dst,dstImage);

			cvNamedWindow("�Ҳ�ɹ�");
			cvShowImage( "�Ҳ�ɹ�", dstImage);

			myParam *myparam = new myParam;
			myparam->img = dstXor;
			myparam->x = x1+xLeft;
			myparam->y = y+yTop;
			myparam->hwnd = hackHwnd;
			cvSetMouseCallback("�Ҳ�ɹ�",on_mouseProc,(void*)myparam);

			cvWaitKey(0); //�ȴ�����

			cvDestroyWindow( "�Ҳ�ɹ�" );//���ٴ���
			cvReleaseImage(&dstXorBinary);//�ͷ�ͼ��
			delete myparam;
			myparam = NULL;

}



/************************************************************************/
/*                                                                      */
/************************************************************************/
//������Ϣ����
LRESULT CzhaochaDlg::GoToGetDiffrent(WPARAM wParam,LPARAM lParam)
{
		HWND hWind = NULL;
		hWind = FindWindow(_T("Main HighGUI class"),_T("�Ҳ�ɹ�"))->GetSafeHwnd();	
		if ( !hWind )			//����Ҳ�ɹ���ť������
		{
				OnBnClickedBtnFind();
		}
		
		return 0L;
}


/************************************************************************/
/*                                                                      */
/************************************************************************/
LRESULT CzhaochaDlg::CloseFindWindow(WPARAM wParam,LPARAM lParam)
{
		HWND hWind = NULL;
		hWind = FindWindow(_T("Main HighGUI class"),_T("�Ҳ�ɹ�"))->GetSafeHwnd();		
		if ( hWind )			//����Ҳ�ɹ���ť������
		{
				::PostMessage(hWind,WM_CLOSE,0,0);
		}

		return 0L;
}



void CzhaochaDlg::OnClose()
{
		// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
		if ( m_lowHook )
		{		
				UnhookWindowsHookEx(m_lowHook);
		}
		CDialog::OnClose();
}


CString CzhaochaDlg::GetAppPath()
{
		CString strRet;
		TCHAR szBuff[_MAX_PATH];
		VERIFY(::GetModuleFileName(AfxGetApp()->m_hInstance, szBuff, _MAX_PATH));
		strRet=szBuff;
		int pos=strRet.ReverseFind('\\');
		strRet=strRet.Left(pos);
		return strRet;
}
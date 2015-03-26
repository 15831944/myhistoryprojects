#include "stdafx.h"
#include "51jobpost.h"
#include "51jobpostDlg.h"
#include ".\51jobpostdlg.h"
#include "CountGen.h"
#include "datatype.h"
#include "resource.h"
#include "UserFile.h"
#include "WindowFind.h"
#include "DHtmlDialogEx.h"
#include <cv.h>
#include <cxcore.h>
#include <highgui.h>
#pragma comment(lib,"cxcore.lib")
#pragma comment(lib,"cv.lib")
#pragma comment(lib,"highgui.lib")
#pragma comment(lib,"cvaux.lib")
#pragma comment(lib,"cvcam.lib")
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#define  USE_DLL_DLL 000
// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���
CListCtrl g_AccList;

/************************************************************************/
/*                                                                      */
/************************************************************************/
CString GetGenNum()
{
	char szName[255] = {0};
	DWORD dwSerialNum = 0;
	DWORD dwMaxNum = 0;
	DWORD dwSysFlag = 0;
	char szSysName[255] = {0};
	DWORD dwSysNameSize = 255;	
	BOOL bRet = GetVolumeInformation("c:\\",szName,255,&dwSerialNum,&dwMaxNum,&dwSysFlag,szSysName,dwSysNameSize);
	CString strRet = "";
	strRet.Format("%s%u%s",szName,dwSerialNum,szSysName);
	return strRet;
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
//���ܳ�16�����ַ���
CString StrEncode(CString srcStr)
{
	int             i ;
	int             nLen;
	CString         newString = "";
	CString         tempStr;
	unsigned char   ucTemp;
	nLen =  srcStr.GetLength( );

	//���ܺ�ת��16����

	for( i = 0; i < nLen; i++ )
	{
		ucTemp = ( unsigned char )srcStr.GetAt(i);
		tempStr.Format("%02X#", ucTemp^i );
		newString +=tempStr;
	}
	return newString;
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
//����16�����ַ���
CString StrDecode(CString srcStr)
{
	int             nLen;
	CString         newString = "";
	CString         tempStr;
	unsigned char   ucTemp;

	//����16�����ַ���
	int idx1 = 0; 
	int idx2 = 0;
	int i = 0;
	idx2 = srcStr.Find('#', idx1);  //����һ��ʮ�����������Կո��ֿ� 
	while(idx2 != -1) 
	{ 
		CString temp1 = "0x" + srcStr.Mid(idx1,idx2); //ȡ��һ��16�����ִ�
		int ldec = strtol(temp1, NULL, 16); //��16����ת��Ϊ10����
		CString temp;
		temp.Format("%c",(unsigned char )ldec^i);
		newString += temp;
		idx1 = idx2 + 1; 
		idx2 = srcStr.Find('#', idx1);  //����һ��16���������Կո��ֿ�
		i ++;
	} 
	return newString;

}
// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���
/************************************************************************/
/* ���ܺ���                                                             */
/************************************************************************/
CString strEnCodeEx(CString strSrc)
{
	INT i = 0;
	INT j = 0;
	INT64 fSeedA = 56789; ///     ����
	INT64 fSeedB = 54667; ///     ����
	INT64 fKey = 6968875; //      Կ��
	INT64 iKey = fKey;
	CString strGet = strSrc;
	CString strRet = "";
	CString strTemp = "";
	CHAR szRet[1000] = {0};
	for (i=0;i<strGet.GetLength();i++)
	{
		szRet[i] = (CHAR)((BYTE)strGet.GetAt(i) ^ (iKey >> 8));
		iKey = ((BYTE)szRet[i]+iKey)*fSeedA+fSeedB;
	}
	strGet.Format("%s",szRet);
	strRet = "";
	for (i=0;i<strGet.GetLength();i++)
	{
		j = (int)strGet.GetAt(i);
		if (j < 0)
		{
			j+=256;
		}		
		strTemp.Format("%c",(CHAR)(65+(j/26)));
		strRet += strTemp;
		strTemp.Format("%c",(CHAR)(65+(j%26)));
		strRet += strTemp;
	}
	return strRet;
}
/************************************************************************/
/* ���ܺ���                                                             */
/************************************************************************/
CString strDeCodeEx(CString strSrc)
{
	int i = 0;
	int j = 0;
	CString strGet;
	strGet = strSrc;
	INT64 fSeedA = 56789; ///     ����   ,
	INT64 fSeedB = 54667; ///     ����   ,
	INT64 fKey = 6968875; //     Կ��
	INT64 iKey = fKey;
	char szResult[1000]={0};
	CString strResult = "";
	CString strTemp = "";
	int index = 0;
	for (i=0;i<strGet.GetLength()/2;i++)
	{
		index = 2*i;
		j = ((int)strGet.GetAt(index)-65)*26;
		index = 2*i+1;
		j += (int)strGet.GetAt(index)-65;
		if (j>128)
		{
			j-=256;
		}
		strTemp.Format("%c",(char)j);
		strResult += strTemp;
	}
	strGet = strResult;
	for (i=0;i<strGet.GetLength();i++)
	{
		szResult[i]=(char)((byte)strGet.GetAt(i)^(iKey >> 8));
		iKey = ((BYTE)strGet.GetAt(i)+iKey)*fSeedA+fSeedB;
	}
	strResult.Format("%s",szResult);
	return strResult;
}

bool IsPowered()
{
	CString strSerial = strEnCodeEx(StrEncode(strEnCodeEx(StrEncode(GetGenNum()))));	
	bool bRet = false;
	CRegKey   regAutoRun;
	char szKey[50] = {0};
	strcpy(szKey,"IsPowered");
	if (ERROR_SUCCESS != regAutoRun.Create(HKEY_CURRENT_USER,"Software\\Microsoft\\Windows\\CurrentVersion"))
	{
		return false;
	}
	DWORD dLen = 5000;
	char szValue[5000]={0}; 
	if( ERROR_SUCCESS == regAutoRun.QueryValue(szValue,szKey,&dLen))
	{
		if (CString(szValue)==strSerial)
		{
			bRet=true; 
		}
	}
	regAutoRun.Close();
	return bRet;
}
typedef int (FAR __stdcall *FnMatchCheckCode)(char *szPath, char *szCode,int iFlag);

FnMatchCheckCode MatchCheckCode = NULL;
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
/************************************************************************/
/*                                                                      */
/************************************************************************/
CjobpostDlg::CjobpostDlg(CWnd* pParent /*=NULL*/)
: CDialog(CjobpostDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	GdiplusStartup(&m_pGdiToken,&m_gdiplusStartupInput,NULL);
	m_pByte = NULL;
	CUserFile *pFile = CUserFile::GetInstance();
	CString strAppPath = "";
	pFile->GetAppPath(strAppPath);
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
CjobpostDlg::~CjobpostDlg()
{
	CUserMessagePump *pPump = CUserMessagePump::GetInstance();
	if (pPump)
	{
		delete pPump;
		pPump = NULL;
	}
	if (m_pByte != NULL)
	{
		delete[] m_pByte;
		m_pByte = NULL;
	}
	GdiplusShutdown(m_pGdiToken);
	if(m_handle)
	{
		FreeLibrary(m_handle);
	}
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CjobpostDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_message);
	DDX_Control(pDX, IDC_LIST_COUNT, g_AccList);
	DDX_Control(pDX, IDC_BTN_CHECKCODE, m_btn);
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
BEGIN_MESSAGE_MAP(CjobpostDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_START, OnBnClickedBtnStart)
	ON_MESSAGE(USERWINDOWMSG_1,AddToList)
	ON_BN_CLICKED(IDC_BTN_SET, OnBnClickedBtnSet)
	ON_BN_CLICKED(IDC_BTN_CHECKCODE, OnBnClickedBtnCheckcode)
	ON_BN_CLICKED(IDC_BTN_Registe, OnBnClickedBtnRegiste)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_COUNT, OnNMRclickListCount)
	ON_COMMAND(ID_MENU_REGISTER, OnMenuRegister)
	ON_BN_CLICKED(IDC_BTN_STOP, OnBnClickedBtnStop)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON2, OnBnClickedButton2)
END_MESSAGE_MAP()
/************************************************************************/
/*                                                                      */
/************************************************************************/
BOOL CjobpostDlg::OnInitDialog()
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
#ifndef DEBUG
	if (GetPower() != 0)
	{
		exit(0);
	}
	if (IsPowered() == false)
	{
		MessageBox("����ע��");
		exit(0);
	}
#endif
	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	InitListCtrl();
	SetTableStyle();
	//m_btn.SubclassDlgItem(IDC_BTN_CHECKCODE,this);
	m_btn.DrawBorder(FALSE);
	//m_btn.DrawTransparent(TRUE);
	m_btn.SetTooltipText("������»�ȡ��֤��");
	m_btn.SetDefaultColors(RGB(255,0,0));
	m_btn.SetColor(CButtonST::BTNST_COLOR_BK_OUT, RGB(0x7c,0xc1,0xf1));
	m_btn.SetColor(CButtonST::BTNST_COLOR_BK_IN, RGB(0x8c,0x71,0xf1));//����ɫ
	m_btn.SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, RGB(0x7c,0xc1,0xf1));
	//m_btn.DrawBorder(FALSE);
	m_btn.SetBtnCursor(IDC_CURSOR1);
	m_handle = 0x0;
#ifdef USE_DLL_DLL
// 	CString strAppPath = "";
// 	CUserFile *pFile = CUserFile::GetInstance();
// 	pFile->GetAppPath(strAppPath);
// 	strAppPath += "CheckCodeDll.dll";
// 	m_handle = ::LoadLibraryEx((LPCTSTR)strAppPath,NULL,LOAD_WITH_ALTERED_SEARCH_PATH);
// 	if(m_handle == 0x0)
// 	{
// 		MessageBox("����CheckCodeDll.dllʧ��","����");
// 		exit(0);
// 	}
// 	MatchCheckCode = (FnMatchCheckCode)GetProcAddress(m_handle,"MatchCheckCode");
#endif	
	CUserMessagePump *pPump = CUserMessagePump::GetInstance();
	if (pPump)
	{
		pPump->SetHwnd(USERWINDOWMSG_1,m_hWnd);
	}
	AfxBeginThread((AFX_THREADPROC)ThreadFunc,(LPVOID)this);
	
	return TRUE;  // ���������˿ؼ��Ľ��㣬���򷵻� TRUE
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CjobpostDlg::OnSysCommand(UINT nID, LPARAM lParam)
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
/************************************************************************/
/*                                                                      */
/************************************************************************/
// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�
void CjobpostDlg::OnPaint() 
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
/************************************************************************/
/*                                                                      */
/************************************************************************/
//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù����ʾ��
HCURSOR CjobpostDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CjobpostDlg::OnBnClickedBtnStart()
{
	g_ExitFlag = false;
	//ע�����
	m_JobPost.BeginRegister();
	//����������	
	GetDlgItem(IDC_BTN_STOP)->EnableWindow(TRUE);
	GetDlgItem(IDC_BTN_START)->EnableWindow(FALSE);
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CjobpostDlg::SetTableStyle()
{
	CListCtrl* conList = &g_AccList;
	DWORD dwSytle=::GetWindowLong(conList->m_hWnd,GWL_STYLE);
	// ����Ϊ������ʽ
	SetWindowLong(conList->m_hWnd,GWL_STYLE,dwSytle|LVS_REPORT);
	DWORD ExStyle=conList->GetExtendedStyle();
	conList->SetExtendedStyle(ExStyle|LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
	CImageList image;
	image.Create(1,16, ILC_COLOR, 0, 0);
	conList->SetImageList(&image, LVSIL_STATE);
	conList->MoveWindow(7,5,780,310);
	conList->InsertColumn(0,"���",LVCFMT_CENTER,50);
	conList->InsertColumn(1,"����",LVCFMT_LEFT,120);
	conList->InsertColumn(2,"����",LVCFMT_LEFT,120);
	conList->InsertColumn(3,"����",LVCFMT_LEFT,120);
	conList->InsertColumn(4,"��������",LVCFMT_LEFT,100);
	conList->InsertColumn(5,"״̬",LVCFMT_LEFT,150);
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CjobpostDlg::InitListCtrl()
{
	DWORD dwSytle=::GetWindowLong(m_message.m_hWnd,GWL_STYLE);
	// ����Ϊ������ʽ
	SetWindowLong(m_message.m_hWnd,GWL_STYLE,dwSytle|LVS_REPORT);
	DWORD ExStyle=m_message.GetExtendedStyle();
	m_message.SetExtendedStyle(ExStyle|LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);

	::SendMessage(m_message.m_hWnd, LVM_SETEXTENDEDLISTVIEWSTYLE,
		LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);

	m_message.InsertColumn(0,_T("ʱ��"),LVCFMT_LEFT,125);
	m_message.InsertColumn(1,_T("��Ϣ"),LVCFMT_LEFT,390);
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
LRESULT CjobpostDlg::AddToList(WPARAM wParam,LPARAM lParam)
{
	CUserMessagePump *pMsgPump = NULL;
	pMsgPump = CUserMessagePump::GetInstance();
	if (!pMsgPump)
	{
		return 0;
	}
	CString msgBuf;
	pMsgPump->GetPumpMessage(USERWINDOWMSG_1,m_hWnd,msgBuf);
	if ( msgBuf == "" )
	{
		return 0;
	}

	int nCount = m_message.GetItemCount();
	if ( nCount >= 14 )
	{
		m_message.DeleteItem( 0 );
		m_message.DeleteItem( 0 );
		nCount -= 2;
	}
	m_message.InsertItem( nCount, "");
	if ( nCount % 2 == 0 )
	{
		m_message.SetRowSelectedBgColor(nCount, RGB(0,0,0) , false);
		m_message.SetRowTxtColor(nCount , RGB(255,0,0) , false);

	}
	else
	{
		m_message.SetRowTxtColor(nCount , RGB(0,0,255) , false);
	}
	m_message.SetRowBgColor(nCount, RGB(230,190,220), false );
	m_message.SetRowStyle(nCount,  LIS_TXTCOLOR | LIS_BGCOLOR);

	m_message.SetItemText( nCount , 0, CTime::GetCurrentTime().Format("%Y-%m-%d %H:%M:%S") );
	m_message.SetItemText( nCount , 1, msgBuf );

	::PostMessage(m_message.m_hWnd,WM_VSCROLL,SB_BOTTOM,0);

	if ( msgBuf == "�����������50�Σ������߳�" )
	{
		pMsgPump = CUserMessagePump::GetInstance();
		if (pMsgPump)
		{
			CString sendStr = "����ǵ�¼���������������û���Ϣ!";
			pMsgPump->AddToMessagePump(USERWINDOWMSG_1,sendStr);
		}

	}
	else if (msgBuf == "��ȡ��֤��ɹ�")
	{
		SetDlgItemText(IDC_EDIT_CODE,"");
		ShowCheckCode();
		GetDlgItem(IDC_EDIT_CODE)->SetFocus();
#ifdef USE_DLL_DLL
		g_iAotuCheckCode = 1;
		if (g_iAotuCheckCode)
		{
			char szCheckCode[255] = {0};
			char szPath[255] = {0};
			try
			{				
				strcpy(szPath,"c:\\checkcode.bmp");
				CString strRet = "";
				IplImage *dstXorBinary;
				if((dstXorBinary = cvLoadImage(szPath, 1)) == 0)
				{
					//ShowMessage("����ͼƬʧ��");
					g_strCheckCode = "";
				}
				IplImage * GreenChannel  = cvCreateImage( cvGetSize(dstXorBinary), 8, 1);
				IplImage * BlueChannel = cvCreateImage( cvGetSize(dstXorBinary), 8, 1);
				cvSetImageCOI(dstXorBinary,1); 
				cvCopy(dstXorBinary,BlueChannel); //��ȡ��ɫ
				cvSetImageCOI(dstXorBinary,2);
				cvCopy(dstXorBinary,GreenChannel);  //��ȡ��ɫ
				cvThreshold(BlueChannel,BlueChannel,140,255,CV_THRESH_BINARY);
				cvSaveImage("c:\\test.bmp",BlueChannel);
				cvReleaseImage(&dstXorBinary);
				cvReleaseImage(&GreenChannel);
				cvReleaseImage(&BlueChannel);
				char *result = NULL;
				result = OCR("c:\\test.bmp",-1);
				strRet.Format("%s",result);
				strRet = GetRetString(strRet);
				g_strCheckCode = LPCTSTR(strRet);
				//_bstr_t strRet = m_pOcr->ToString(_bstr_t(LPCTSTR(szPath)));
				//g_strCheckCode = LPCTSTR(strRet);
			}			
			catch (...)
			{
				g_strCheckCode = "";
			}
			SetDlgItemText(IDC_EDIT_CODE,szCheckCode);
			pMsgPump = CUserMessagePump::GetInstance();
			if (pMsgPump)
			{
				if (g_strCheckCode == "")
				{
					pMsgPump->AddToMessagePump(USERWINDOWMSG_1,"��֤��ʶ����Ϊ��,�Զ����Ϊ:1234...");
					g_strCheckCode = "1234";
				}
				else
				{
					pMsgPump->AddToMessagePump(USERWINDOWMSG_1,"��֤���Զ�ʶ����:"+g_strCheckCode+"...");
				}
			}
		}
#endif
	}
	return 0;
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CjobpostDlg::OnBnClickedBtnSet()
{
	CCountGen coutGen;
	coutGen.m_pList = &g_AccList;
	coutGen.DoModal();
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CjobpostDlg::OnBnClickedBtnCheckcode()
{
	//GetDlgItem(IDC_BTN_Registe)->EnableWindow(FALSE);
// 	SetDlgItemText(IDC_EDIT_CODE,"");
// 	g_strCheckCode = "";
}
/***************************************************************************   
*   ��������   
*   ReadBMPFile  
*   ����   
*        LPCTSTR strFileName         -   ͼ���·��

*   ����ֵ   
*         LPBYTE                     -����ͼ�����������
*   ˵��   
*         �ú��������ļ���ȡ�ķ�����ȡͼ���������   
***************************************************************************/ 
bool CjobpostDlg::ReadBMPFile(LPCTSTR strFileName,LPBYTE &m_lpBMPFileData,long& fileLength)
{
	CFile BitmapFile;
	BOOL blOpen=BitmapFile.Open(strFileName, CFile::modeRead,NULL);
	if( !blOpen )
	{
		//CString str = "���ļ�ʧ�ܣ�";
		m_lpBMPFileData = NULL;
		return false;
	}
	unsigned long FileLength=BitmapFile.GetLength();
	fileLength = FileLength;
	if (m_lpBMPFileData != NULL)
	{
		delete[] m_lpBMPFileData;
		m_lpBMPFileData = NULL;
	}
	m_lpBMPFileData = new BYTE[FileLength];
	ASSERT( m_lpBMPFileData!=NULL );
	BitmapFile.Read(m_lpBMPFileData,FileLength);

	if ( ((LPBITMAPFILEHEADER)m_lpBMPFileData)->bfType != 0x4D42 )
	{
		//CString str = "��֧�ֳ�BMP֮����ļ���";
		delete[] m_lpBMPFileData;
		m_lpBMPFileData = NULL;
		return false;;
	}
	return true;
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
//��ʾλͼ����
BOOL CjobpostDlg::ShowBmpFile(HDC hdc, int nXDest, int nYDest,LPBYTE lpDIB,CRect rect)
{
	if( lpDIB == NULL)
	{
		return FALSE;
	}
	LPBITMAPINFOHEADER lpBMPInfoHeader=(LPBITMAPINFOHEADER) (lpDIB + sizeof(BITMAPFILEHEADER));
	BITMAPINFO * lpBMPInfo =(BITMAPINFO *)(lpDIB + sizeof(BITMAPFILEHEADER));
	int iWidth =0;
	int iHeight = 0;
	if ( lpBMPInfoHeader->biWidth > rect.Width())
	{
		iWidth = rect.Width();
	}
	else
	{
		iWidth = lpBMPInfoHeader->biWidth;
	}
	if ( lpBMPInfoHeader->biHeight > rect.Height())
	{
		iHeight = rect.Height();
	}
	else
	{
		iHeight = lpBMPInfoHeader->biHeight;
	}
	SetDIBitsToDevice(hdc,nXDest,nYDest,iWidth,iHeight,
		0,0,0,lpBMPInfoHeader->biHeight,lpDIB+((BITMAPFILEHEADER *)lpDIB)->bfOffBits,lpBMPInfo,DIB_RGB_COLORS);
	return TRUE;
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CjobpostDlg::ShowCheckCode()
{
	g_strDirect = "c:\\";
	g_strFileName = "checkcode.bmp";
 	m_btn.m_strSrc = g_strDirect+g_strFileName;	
	ShowBmp(g_strDirect+g_strFileName,::GetDC(GetDlgItem(IDC_BTN_CHECKCODE)->GetSafeHwnd()));
	if (g_iAotuCheckCode)
	{
		TranstToBMP(g_strDirect+g_strFileName,g_strDirect+BMPFILE_PATH);
	}
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
BOOL CjobpostDlg::GetImageCLSID(const WCHAR* format, CLSID* pCLSID)
{
	UINT num = 0;
	UINT size = 0;
	ImageCodecInfo* pImageCodecInfo = NULL;
	GetImageEncodersSize(&num, &size);
	if(size == 0)
	{
		return FALSE;
	}
	pImageCodecInfo = (ImageCodecInfo *)malloc((size));
	if(pImageCodecInfo == NULL)
		return FALSE;
	GetImageEncoders(num, size, pImageCodecInfo);
	// Find for the support of format for image in the windows
	for(UINT i = 0; i < num; ++i)
	{
		//MimeType: Depiction for the program image
		if( wcscmp(pImageCodecInfo[i].MimeType, format) == 0)
		{
			*pCLSID = pImageCodecInfo[i].Clsid;
			free(pImageCodecInfo);
			return TRUE;
		}
	}
	free(pImageCodecInfo);
	return FALSE;
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
BOOL CjobpostDlg::TranstToBMP(CString srcPath,CString destPath)
{
	WCHAR wCH[255] = {0};
	WCHAR wCH1[255] = {0};
	wcscpy(wCH,srcPath.AllocSysString());
	Image img(wCH);//�����ͼƬ������������ʽ	
	CLSID pngClsid;
	GetImageCLSID(L"image/bmp", &pngClsid);//�����ͼƬ������������ʽ���˴�ת��ΪBMP��ʽ
	wcscpy(wCH1,   destPath.AllocSysString());
	img.Save(wCH1, &pngClsid, NULL);
	return TRUE;
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CjobpostDlg::OnBnClickedBtnRegiste()
{
	GetDlgItemText(IDC_EDIT_CODE,g_strCheckCode);
	g_strCheckCode.Trim();
	if (g_strCheckCode == "")
	{
		MessageBox("��֤�벻��Ϊ��","����");
		return;
	}
	SetDlgItemText(IDC_EDIT_CODE,"");
	CString strShow  = "";
	strShow.Format("��֤��������:%s,������֤...",g_strCheckCode);
	CUserMessagePump *pMsgPump = NULL;
	pMsgPump = CUserMessagePump::GetInstance();
	if (pMsgPump)
	{
		pMsgPump->AddToMessagePump(USERWINDOWMSG_1,strShow);
	}
	//GetDlgItem(IDC_BTN_Registe)->EnableWindow(FALSE);
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
BOOL CjobpostDlg::PreTranslateMessage( MSG* pMsg)
{
	if( pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE)
	{
		return TRUE;      //���ε�Esc��
	}
	if( pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
	{
		HWND hWnd=::GetFocus();
		int iID=::GetDlgCtrlID(hWnd);
		if(iID==IDC_EDIT_CODE)
		{
			OnBnClickedBtnRegiste();
		}
		return TRUE;      //���ε�Enter��
	}
	return CDialog::PreTranslateMessage( pMsg );
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CjobpostDlg::OnNMRclickListCount(NMHDR *pNMHDR, LRESULT *pResult)
{
// 	POSITION pos = g_AccList.GetFirstSelectedItemPosition();
// 	if (pos != NULL)
// 	{
// 		int nItem = g_AccList.GetNextSelectedItem(pos);
// 		if (g_AccList.GetItemText(nItem,5) != "��" && g_AccList.GetItemText(nItem,5) != "")
// 		{
// 			CMenu menu; 
// 			menu.LoadMenu(IDR_MENU1); //�������ȶ����ѡ�� 
// 			CMenu *pmenu = menu.GetSubMenu(0); 
// 			CPoint pos; 
// 			GetCursorPos(&pos);
// 			SetForegroundWindow();
// 			pmenu->TrackPopupMenu(TPM_LEFTALIGN,pos.x,pos.y,this);
// 		}		
// 	}
	*pResult = 0;
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CjobpostDlg::OnMenuRegister()
{
	POSITION pos = g_AccList.GetFirstSelectedItemPosition();
	if (pos != NULL)
	{
		int nItem = g_AccList.GetNextSelectedItem(pos);
		RegisterPack rgPack;
		rgPack.strAcc = g_AccList.GetItemText(nItem,1);
		rgPack.strPwd = g_AccList.GetItemText(nItem,2);
		rgPack.strMail = g_AccList.GetItemText(nItem,3);
		//AfxMessageBox(rgPack.strAcc);
		AddRegisterPackEx(rgPack);
	}
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CjobpostDlg::OnBnClickedBtnStop()
{
	g_ExitFlag = true;
	GetDlgItem(IDC_BTN_STOP)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_START)->EnableWindow(TRUE);
	m_btn.m_strSrc = "";
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CjobpostDlg::OnClose()
{
	if (g_ExitFlag == false)
	{
		g_ExitFlag = true;
		Sleep(800);
	}	
	CDialog::OnClose();
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
void ShowBmp(CString srcPath,HDC hdc)
{
	WCHAR wCH[255] = {0};
	wcscpy(wCH,srcPath.AllocSysString());
	Image img(wCH);//�����ͼƬ������������ʽ
	g_iShowWidth = 220;
	g_iShowHeight = 350;
	//g_iShowWidth += g_iOldWidthAdd;
	//g_iOldWidthAdd = 0 - g_iOldWidthAdd;
	Graphics mygraphics(hdc);//
	int iWidth=img.GetWidth() * g_iShowWidth / 100;
	int iHeight=img.GetHeight() * g_iShowHeight / 100;
	mygraphics.DrawImage(&img,2,2,iWidth,iHeight);
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CjobpostDlg::OnBnClickedButton2()
{
	CString strAcc="";
	CString strPwd="";
	CString strPhone = "";
	CString strRecommandPhone = "";
	int   lines   =   0;   
	CStdioFile   file;
	CString   cstr;
	CFileException   fe;
	CString strFilePath = "";
	char szFilter[]="Text files(*.txt)|*.txt";
	CFileDialog dlg(1,NULL,NULL,NULL,szFilter); 
	if(dlg.DoModal()==IDOK) 
	{
		strFilePath = dlg.GetPathName();
	}
	if (strFilePath=="")
	{
		return;
	}
	if(file.Open(strFilePath,CFile::modeRead,&fe))   
	{ 
		file.SeekToBegin();
		int idx1=0;
		int idx2=0;
		srand(time(NULL));
		while (file.ReadString(cstr))
		{
			strAcc = "";
			strPwd = "";
			strPhone = "";
			strRecommandPhone = "";
			idx2 =cstr.Find(",");
			if (idx2==-1)
			{
				continue;
			}
			strAcc = cstr.Left(idx2);
			cstr = cstr.Right(cstr.GetLength()-idx2-1);
			idx2 =cstr.Find(",");
			if (idx2==-1)
			{
				continue;
			}
			strPwd = cstr.Left(idx2);
			cstr = cstr.Right(cstr.GetLength()-idx2-1);
			idx2 =cstr.Find(",");
			if (idx2==-1)
			{
				continue;
			}
			strPhone = cstr.Left(idx2);
			cstr = cstr.Right(cstr.GetLength()-idx2-1);
			idx2 =cstr.Find(",");
			if (idx2==-1)
			{
				continue;
			}
			strRecommandPhone = cstr.Left(idx2);
			CListCtrl *m_pList = &g_AccList;
			//�Ѿ��ҵ�������Ҫ������
			if (strAcc !="" && strPwd != "" && strPhone != "" && strRecommandPhone != "")
			{
				int nCount = m_pList->GetItemCount();
				CString listIndex = "";
				int iIsExist = 0;
				for (int i=0;i<nCount;i++)
				{
					if (strAcc == m_pList->GetItemText(i,1) && strPhone == m_pList->GetItemText(i,3))
					{
						CString strShow = "";
						strShow.Format("%s �ظ�����",strAcc);
						CUserMessagePump *pMsgPump = NULL;
						pMsgPump = CUserMessagePump::GetInstance();
						if (!pMsgPump)
						{
							return;
						}
						pMsgPump->AddToMessagePump(USERWINDOWMSG_1,strShow);
						iIsExist = 1;
						break;
					}
				}
				if (iIsExist == 1)
				{
					continue;
				}

				RegisterPack rgPack;
				rgPack.strAcc = strAcc;					//�˺�
				rgPack.strPwd = strPwd;					//����
				rgPack.strMail = strPhone;				//����		
				rgPack.strIntroMan = strRecommandPhone;	//��������
				AddRegisterPack(rgPack);
				nCount = m_pList->GetItemCount();
				listIndex.Format("%d",nCount);
				m_pList->InsertItem(nCount,"",0);
				m_pList->SetItemText(nCount,0,listIndex);
				m_pList->SetItemText(nCount,1,strAcc);
				m_pList->SetItemText(nCount,2,strPwd);
				m_pList->SetItemText(nCount,3,strPhone);
				m_pList->SetItemText(nCount,4,strRecommandPhone);
			}
		}
		file.Close();
	}	
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CjobpostDlg::ThreadFunc(LPVOID lParam)
{
	CjobpostDlg *pParam = ((CjobpostDlg *)lParam);
	if ( pParam )
	{
		pParam->RegisterFunc();
		ShowMessageExEx("�߳��˳�");
	}

}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CjobpostDlg::RegisterFunc()
{
	ShowMessageExEx("�����ʼ��");
	HWND hwnd = NULL;
	while(true)
	{
		CDHtmlDialogEx pWindow;
 		pWindow.Create(IDD_DLG_HTMLDLG,CWnd::FromHandle(this->GetSafeHwnd()));
 		//pWindow.ShowWindow(SW_SHOW);
		pWindow.ShowWindow(SW_HIDE);
		MSG msg;
		while(GetMessage (&msg,NULL,0,0))
		{   
			if (pWindow.GetSafeHwnd() == NULL ||
				!::IsDialogMessage(pWindow.GetSafeHwnd(), &msg))  //���϶Ի�����Ϣ����
			{ 
				TranslateMessage (&msg);
				DispatchMessage (&msg);
			}
		}
	}
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
int CjobpostDlg::GetPower()
{
	CInternetSession session;
	CString strTemp = "";
	try
	{
		CStdioFile *pFile = session.OpenURL("http://blog.csdn.net/TragicGuy/archive/2010/01/08/5157814.aspx");
		if (pFile != NULL)
		{
			while(pFile->ReadString(strTemp))
			{
				strTemp.MakeLower();
				if (strTemp.Find("myownpower555") !=-1)
				{
					pFile->Close();
					delete pFile;
					pFile = NULL;
					return 0;
				}
			}

			if (pFile == NULL)
			{
				pFile->Close();
				delete pFile;
				pFile = NULL;
			}
		}
	}
	catch (...)
	{
	}
	return -1;
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
CString CjobpostDlg::GetRetString(CString strSrc)
{
	char num = 0;
	CString strRet = "";
	CString strTemp = "";
	//strSrc.MakeLower();
	for (int i= 0;i<strSrc.GetLength();i++)
	{
		num = strSrc.GetAt(i);
		if (num == 'o'||num == 'O')
		{
			num = '0';
		}
		if (num == ' ' || num == 'z' || num == 'Z')
		{
			num = '2';
		}
		if (num == 'g')
		{
			num = '9';
		}
		if (num == '$')
		{
			num = '8';
		}
		if (num == 'q')
		{
			num ='4';
		}
		if (num == 'l')
		{
			num = '1';
		}
		if (num == 'b')
		{
			num  = '6';
		}
		if ((num >= 48 && num <=57) ||
			(num >= 65 && num <=90) ||
			(num >= 97 && num <=122))
		{
			strTemp.Format("%c",num);
			strRet += strTemp;
		}
	}
	return strRet;		
}
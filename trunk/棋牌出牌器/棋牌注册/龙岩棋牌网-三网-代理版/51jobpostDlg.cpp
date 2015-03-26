#include "stdafx.h"
#include "51jobpost.h"
#include "51jobpostDlg.h"
#include ".\51jobpostdlg.h"
#include "CountGen.h"
#include "datatype.h"
#include "resource.h"
#include "UserFile.h"
#include "WindowFind.h"
#include <AFXPRIV.H>
#include "Vcode.h"
// #include <cv.h>
// #include <cxcore.h>
// #include <highgui.h>
// #pragma comment(lib,"cxcore.lib")
// #pragma comment(lib,"cv.lib")
// #pragma comment(lib,"highgui.lib")
//#pragma comment(lib,"cvaux.lib")
//#pragma comment(lib,"cvcam.lib")
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#define  USE_DLL_DLL 000
// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���
CListCtrl g_AccList;

//typedef int (FAR __stdcall *FnMatchCheckCode)(char *szPath, char *szCode,int iFlag);
// typedef BOOL (WINAPI *LOADDE)(char* FilePath,char* PassWord); 
// typedef char* (WINAPI *DEINFO)(void); 
// typedef char* (WINAPI *DEERROR)(void); 
// typedef char* (WINAPI *GETVCODE)(char* PicName,int Index);
// //FnMatchCheckCode MatchCheckCode = NULL;
// 
 HINSTANCE hInstDLL=NULL;//Vcode.dll��ģ����,�������ʱҪ�ͷ�
// 
// //��������
// LOADDE LoadDe = NULL;
// DEINFO DeInfo = NULL;
// DEERROR DeError = NULL;
// GETVCODE GetVcode = NULL;


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
// 	m_pByte = NULL;
// 	CUserFile *pFile = CUserFile::GetInstance();
// 	CString strAppPath = "";
// 	pFile->GetAppPath(strAppPath);
// 	CoInitialize(NULL);
// 	try
// 	{
// 		HRESULT hr;
// 		CLSID clsid;
// 		hr=CLSIDFromProgID(OLESTR("CodeCheck.ClassCheck"),&clsid);	
// 		IClassFactory* p_classfactory;
// 		hr=CoGetClassObject(clsid,CLSCTX_INPROC_SERVER,
// 			NULL,IID_IClassFactory, 
// 			(LPVOID*)&p_classfactory);
// 		p_classfactory->CreateInstance(NULL,__uuidof(_ClassCheck),
// 			(LPVOID*)&m_pOcr);
// 	}
// 	catch (...)
// 	{
// 		MessageBox("����ע��COM��","����");
// 	}
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
// 	if (m_pByte != NULL)
// 	{
// 		delete[] m_pByte;
// 		m_pByte = NULL;
// 	}
	GdiplusShutdown(m_pGdiToken);
	if(m_handle)
	{
		FreeLibrary(m_handle);
	}
	m_picture.Release();
	if(hInstDLL)
	{
		FreeLibrary(hInstDLL);
	}
	//CoUninitialize();
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
	//Sleep(500);
	//m_pOcr->loadcodeKey();
	CUserMessagePump *pPump = CUserMessagePump::GetInstance();
	if (pPump)
	{
		pPump->SetHwnd(USERWINDOWMSG_1,m_hWnd);
	}

// 	hInstDLL= LoadLibrary(_T("Vcode.dll"));	//����ͬĿ¼�µ�Vcode.dll
// 	if(hInstDLL)
// 	{
// 		LoadDe = (LOADDE)GetProcAddress(hInstDLL,_T("LoadDe"));
// 		//DeInfo = (DEINFO)GetProcAddress(hInstDLL,_T("DeInfo"));
// 		DeError = (DEERROR)GetProcAddress(hInstDLL,_T("DeError"));
// 		GetVcode = (GETVCODE)GetProcAddress(hInstDLL,_T("GetFileVcode"));
// 
// 		//ȡ��ǰEXEĿ¼,�ٺϳ�9you.Epu��ȫ·��
// 		HINSTANCE hInstEXE=::GetModuleHandle(NULL);
// 		CString sPath;
// // 		GetModuleFileName(hInstEXE,sPath.GetBuffer(MAX_PATH),MAX_PATH);
// // 		sPath.ReleaseBuffer();
// // 		int nEnd = sPath.ReverseFind('\\');
// // 		sPath=sPath.Left (nEnd+1);
// 		CString strAppPath = "";
// 		CUserFile *pFile = CUserFile::GetInstance();
// 		pFile->GetAppPath(strAppPath);
// 		sPath=strAppPath + _T("9you.Epu");
// 		if(LoadDe((LPTSTR)(LPCSTR)sPath,_T("")))
// 		{
// 			return TRUE;  // return TRUE  unless you set the focus to a control
// 		}
// 		else
// 		{
// 			AfxMessageBox(DeError());//��ʾ����
// 		}
// 		FreeLibrary(hInstDLL);
// 	}	
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
	m_JobPost.BeginRegister();
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
	conList->InsertColumn(1,"����",LVCFMT_LEFT,110);
	conList->InsertColumn(2,"����",LVCFMT_LEFT,110);
	conList->InsertColumn(3,"����",LVCFMT_LEFT,0);
	conList->InsertColumn(4,"���֤��",LVCFMT_LEFT,100);
	conList->InsertColumn(5,"״̬",LVCFMT_LEFT,50);
	conList->InsertColumn(6,"��ע",LVCFMT_LEFT,70);
	conList->InsertColumn(7,"�Ƽ���",LVCFMT_LEFT,110);
	conList->InsertColumn(8,"IP",LVCFMT_LEFT,140);
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
		if (g_iAotuCheckCode)
		{
			char szCheckCode[255] = {0};
			char szPath[255] = {0};
			strcpy(szPath,g_strDirect + BMPFILE_PATH);
			//strcpy(szPath,"c:\\test.bmp");
			try
			{
// 				CString strRet = "";
// 				IplImage *dstXorBinary;
// 				if((dstXorBinary = cvLoadImage(szPath, 0)) == 0)
// 				{
// 					//ShowMessage("����ͼƬʧ��");
// 					g_strCheckCode = "";
// 				}
// 				cvThreshold(dstXorBinary,dstXorBinary,135,255,CV_THRESH_BINARY);
// 				cvSaveImage("c:\\test.bmp",dstXorBinary);
// 				cvReleaseImage(&dstXorBinary);
// 				char *result = NULL;
// 				result = OCR("c:\\test.bmp",-1);
// 				strRet.Format("%s",result);
// 				strRet = GetRetString(strRet);
// 				g_strCheckCode = LPCTSTR(strRet);
// 				_bstr_t strRet = m_pOcr->CheckCode(_bstr_t(LPCTSTR(szPath)));
// 				g_strCheckCode = LPCTSTR(strRet);
				//LoadBMP(g_strDirect + BMPFILE_PATH);
				CString str;//ʶ����
				GetFileVcode((LPTSTR)(LPCSTR)(g_strDirect + BMPFILE_PATH),1,str.GetBuffer(100));//���ֻ�����
				str.ReleaseBuffer ();//�ͷŻ�����
				g_strCheckCode = str;//GetVcode((LPTSTR)(LPCSTR)(g_strDirect + BMPFILE_PATH),1);//ʶ��ͼƬ��
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
					pMsgPump->AddToMessagePump(USERWINDOWMSG_1,"��֤��ʶ����Ϊ��,�Զ����Ϊ:1234,����ע��...");
					g_strCheckCode = "1234";
				}
				else
				{
					pMsgPump->AddToMessagePump(USERWINDOWMSG_1,"��֤���Զ�ʶ����:"+g_strCheckCode+",����ע��...");
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
	SetDlgItemText(IDC_EDIT_CODE,"");
	g_strCheckCode = "r";
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
	strShow.Format("��֤��������:%s,����ע��...",g_strCheckCode);
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
	POSITION pos = g_AccList.GetFirstSelectedItemPosition();
	if (pos != NULL)
	{
		int nItem = g_AccList.GetNextSelectedItem(pos);
		if (g_AccList.GetItemText(nItem,5) != "��" && g_AccList.GetItemText(nItem,5) != "")
		{
			CMenu menu; 
			menu.LoadMenu(IDR_MENU1); //�������ȶ����ѡ�� 
			CMenu *pmenu = menu.GetSubMenu(0); 
			CPoint pos; 
			GetCursorPos(&pos);
			SetForegroundWindow();
			pmenu->TrackPopupMenu(TPM_LEFTALIGN,pos.x,pos.y,this);
		}		
	}
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
	Graphics mygraphics(hdc);//
	int iWidth=img.GetWidth() * g_iShowWidth / 100;
	int iHeight=img.GetHeight() * g_iShowHeight / 100;
	mygraphics.DrawImage(&img,2,2,iWidth,iHeight);
}
void CjobpostDlg::OnBnClickedButton2()
{
	C51JobWebPost webReg;
	webReg.ChangeMacAddr();
		return;
	
	HWND hwnd = ::FindWindow("#32770","�ʺ�ע��");
	//HWND hwnd = ::FindWindow("IEFrame","������ - ע���ʺ� - Microsoft Internet Explorer");
	char str[128]; 
	CWindowFind find;
	//hwnd = find.MyFindWindow("Internet Explorer_Server","",hwnd);
	hwnd = find.GetChildWindow(hwnd,1);
	hwnd = find.GetChildWindow(hwnd,1);
	hwnd = find.GetChildWindow(hwnd,1);
	hwnd = find.GetChildWindow(hwnd,1);
	GetClassName(hwnd,str,sizeof(str));//��ȡ���� 
	if(strcmp(str,"Internet Explorer_Server")==0)
	{
		IHTMLDocument2 *pDoc = webReg.GetDocInterface(hwnd);
		webReg.GetHtmlCtrl(pDoc);
	}
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
CString CjobpostDlg::GetRetString(CString strSrc)
{
	char num = 0;
	CString strRet = "";
	CString strTemp = "";
	for (int i= 0;i<strSrc.GetLength();i++)
	{
		num = strSrc.GetAt(i);
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
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CjobpostDlg::LoadBMP(CString &sPath)
{
	if(m_picture.p)
	{
		m_picture.Release();
		m_picture.p=NULL;
	}

	CFile file;
	if (file.Open(sPath,CFile::modeRead))
	{
		CArchive ar(&file,CArchive::load|CArchive::bNoFlushOnDelete);
		CArchiveStream arcstream(&ar);
		HRESULT hr = OleLoadPicture((LPSTREAM)&arcstream,0,false,IID_IPicture,(void**)&m_picture);

		if(SUCCEEDED(hr))
		{
			RedrawWindow();
		}
	}

}
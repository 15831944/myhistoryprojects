// ReStartAppDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ReStartApp.h"
#include "ReStartAppDlg.h"
#include ".\restartappdlg.h"
#include "UserFile.h"
#include <io.h>
#include <direct.h>
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#define  WM_USER_MODIFYINI	WM_USER + 15454
#define  WM_SET_FILEINFO "mwgate.ini"//"MonGate.ini"

BOOL g_iExitFlag = TRUE;
// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

/************************************************************************/
/*                                                                      */
/************************************************************************/
//�߳��п�����ʱ�ĺ���
void Delay(int iMiniSecond,BOOL *bExit)
{
	for (int i=0;i<iMiniSecond;i+=5)
	{
		if (*bExit)
		{
			break;
		}
		Sleep(5);
	}
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
//д��־
void WriteLog(CString srcStr,int iFlag)
{

	CTime currTime = CTime::GetCurrentTime();
	try
	{
		CString filepath = "";
		CString destStr = "";
		CString folder = "";
		CString filename = "";
		CString strAppPath = "";
		if (iFlag == 0)	//������־
		{
			folder = "restartlog";
			filename.Format("%s_restartlog.txt",currTime.Format("%Y%m%d"));
		}
		else
		{
			return;
		}
		CUserFile *pFile = CUserFile::GetInstance();
		if ( pFile )
		{
			pFile->GetAppPath(strAppPath);
		}
		CString folderName = "";
		folderName.Format("%s%s",strAppPath,folder);
		if(_access(folderName,0)!=0)
		{
			if(mkdir(folderName) != 0)//����Ŀ¼
			{
				return;
			}
		}					
		destStr.Format("%s	%s",currTime.Format("%Y-%m-%d %H:%M:%S"),srcStr);
		filepath.Format("%s\\%s",folderName,filename);
		FILE *fp;
		char szfile[1000]={0};
		sprintf(szfile,filepath);
		fp=fopen(szfile,"a+");
		fprintf(fp,"%s\r\n",destStr);
		fclose(fp);
	}
	catch (...)
	{
	}
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
CString GetContrlClassNameEx(HWND hwnd)
{
	CString strRet = "";
	TCHAR  szClassName[256]={0};	
	GetClassName(hwnd,(LPTSTR)&szClassName,256);
	strRet.Format("%s",szClassName);
	return strRet;
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
CString GetContrlWindowTextEx(HWND hwnd)
{
	CString strRet = "";
	TCHAR  szWindName[256]={0};	
	::GetWindowText(hwnd,(LPTSTR)&szWindName,256);
	strRet.Format("%s",szWindName);
	return strRet;
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CloseErrorWindowEx(HWND hwnd)
{
	try
	{
		HWND hwndChind = NULL;
		CString strWindowName = "";
		CString strClassName = "";
		hwndChind = GetWindow(hwnd,GW_CHILD);
		CWnd *pWnd = CWnd::FromHandle(hwndChind);
		bool bExist = false;
		while (pWnd)
		{	
			hwndChind = pWnd->GetSafeHwnd();
			strWindowName = GetContrlWindowTextEx(hwndChind);
			strClassName = GetContrlClassNameEx(hwndChind);
			if(strClassName =="Button" && strWindowName == "ȷ��")
			{
				WriteLog("�رմ����",0);
				::PostMessage(hwndChind,WM_LBUTTONDOWN,0,MAKELONG(1,1));
				Sleep(20);
				::PostMessage(hwndChind,WM_LBUTTONUP,0,MAKELONG(1,1));
				bExist = true;
				break;
			}
			pWnd = pWnd->GetNextWindow(GW_HWNDNEXT);
		}
		if (bExist == false)
		{
			WriteLog("δ�ҵ�ȷ����ť,����WM_CLOSE��Ϣ",0);
			::PostMessage(hwnd,WM_CLOSE,0,0);
		}
	}	
	catch (...)
	{
	}	
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
BOOL CALLBACK EnumChildProcEx(HWND hwnd, LPARAM lParam)
{   
	CString strWindowName = GetContrlWindowTextEx(hwnd);
	if (strWindowName.Find("Ӧ�ó������")>0)		//��������
	{
		//�������Ӵ��ڣ�����ҵ�ȷ����ť������˫����Ϣ
		WriteLog("�ҵ������:"+strWindowName,0);
		try
		{
			CloseErrorWindowEx(hwnd);
		}
		catch (...)
		{
		}		
		return FALSE;
	}
	else
	{
		Delay(20,&g_iExitFlag);
		if (g_iExitFlag == TRUE)
		{
			return FALSE;
		}
		EnumChildWindows(hwnd,EnumChildProcEx,lParam);
	}
	return TRUE;   
}


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


// CReStartAppDlg �Ի���



CReStartAppDlg::CReStartAppDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CReStartAppDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CReStartAppDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CReStartAppDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BTN_START, OnBnClickedBtnStart)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTN_EXIT, OnBnClickedBtnExit)
	ON_MESSAGE(WM_USER_MODIFYINI,OnReInitparam)
	ON_WM_SIZE()
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CReStartAppDlg ��Ϣ�������

BOOL CReStartAppDlg::OnInitDialog()
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
	SetIcon(m_hIcon, TRUE);		// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��
	m_iFailTime = 0;
	MoveWindow(10000,10000,0,0);		//���ش���
	//PostMessage(WM_SIZE,0,0);
	ShowWindow(SW_HIDE);
	::SetWindowLong(GetSafeHwnd(), GWL_EXSTYLE, WS_EX_TOOLWINDOW); 
	ShowWindow(SW_SHOWNORMAL);
	InitParams();
	OnBnClickedBtnStart();	
	return TRUE;  // ���������˿ؼ��Ľ��㣬���򷵻� TRUE
}

void CReStartAppDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CReStartAppDlg::OnPaint() 
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
HCURSOR CReStartAppDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CReStartAppDlg::OnBnClickedBtnStart()
{
	ReStartApp(0);
	SetTimer(1,m_iCheckInterval,NULL);
	g_iExitFlag = FALSE;
	AfxBeginThread((AFX_THREADPROC)CloseProcFun, (LPVOID)this);
	GetDlgItem(IDC_BTN_START)->EnableWindow(FALSE);
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
//����Ӧ�ó�������Ŀ¼������"\"
int CReStartAppDlg::GetAppPath(CString &strAppPath)
{
	int iRet = -3;
	try
	{
		TCHAR szBuff[_MAX_PATH];
		VERIFY(::GetModuleFileName(AfxGetApp()->m_hInstance, szBuff, _MAX_PATH));
		strAppPath.Format("%s",szBuff);
		int pos=strAppPath.ReverseFind('\\');
		strAppPath=strAppPath.Left(pos+1);		
		iRet = 0;
	}
	catch (...)
	{
	}
	return iRet;	
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CReStartAppDlg::OnTimer(UINT nIDEvent)
{
	if (nIDEvent == 1)
	{
		//��������
		int iRet = ReStartApp(1);
		if (iRet == 0)
		{
			m_iFailTime ++;
			if (m_iFailTime > 2)
			{
				SetDlgItemText(IDC_SHOWINFO,CTime::GetCurrentTime().Format("%Y-%m-%d %H:%M:%S")+" �������ʧ��,���¼���������Ϣ");
				InitParams();
				m_iFailTime = 0;
			}
		}
		
	}
	CDialog::OnTimer(nIDEvent);
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
int CReStartAppDlg::ReStartApp(int iFlag)
{
	int iRetVal = 0;	//0��ʾʧ��
	try
	{
		if (m_iRunStatus == 1)
		{
			SetDlgItemText(IDC_SHOWINFO,CTime::GetCurrentTime().Format("%Y-%m-%d %H:%M:%S")+"	���������رգ�������");	
			WriteLog("���������رգ�������",0);
			return 0;
		}
		CWnd *pWnd = NULL;
		pWnd = FindWindow(NULL,m_strWindowName);
		if (pWnd != NULL)
		{
			SetDlgItemText(IDC_SHOWINFO,CTime::GetCurrentTime().Format("%Y-%m-%d %H:%M:%S")+"	Ŀ�������������");			
			iRetVal = 1;
			return iRetVal;
		}
		CString strShow = "";
		CString strAppPath = "";
		GetAppPath(strAppPath);
		HINSTANCE iRet = ShellExecute(NULL, "open", strAppPath + m_strStrAppName, NULL, NULL, SW_SHOW);
		if (iFlag == 0)
		{
			strShow.Format("��һ����������%s",(int)iRet <32?"ʧ��":"�ɹ�");
			SetDlgItemText(IDC_SHOWINFO,CTime::GetCurrentTime().Format("%Y-%m-%d %H:%M:%S")+"	"+strShow);
			WriteLog(strShow,0);
		}
		else
		{
			strShow.Format("��������%s",(int)iRet <32?"ʧ��":"�ɹ�");
			SetDlgItemText(IDC_SHOWINFO,CTime::GetCurrentTime().Format("%Y-%m-%d %H:%M:%S")+"	"+strShow);
			WriteLog(strShow,0);
		}
		iRetVal = (int)iRet<32?0:1;
	}
	catch (...)
	{
		iRetVal = 0;
	}
	return iRetVal;
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CReStartAppDlg::OnBnClickedBtnExit()
{
	PostMessage(WM_CLOSE,0,0);
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
void CReStartAppDlg::InitParams()
{
	CUserFile *pFile = NULL;
	pFile = CUserFile::GetInstance();
	CString strTemp = "";
	int iTemp = 0;
	if (pFile)
	{
		CString strAppPath = "";
		pFile->GetAppPath(strAppPath);
		strAppPath += WM_SET_FILEINFO; //mwgate.ini
		//���ݿ���Ϣ
		pFile->ReadInitInfo(strAppPath,"window","name",m_strWindowName,iTemp,0);
		if (m_strWindowName == "")
		{
			m_strWindowName = "������������ v1.0.6.2";
		}
		pFile->ReadInitInfo(strAppPath,"window","appname",m_strStrAppName,iTemp,0);
		if (m_strStrAppName == "")
		{
			m_strStrAppName = "MonGate.exe";
		}
 		CString strVersion = "";
 		pFile->ReadInitInfo(strAppPath,"window","version",strVersion,iTemp,0);
 		m_strWindowName += " "+strVersion;	
		pFile->ReadInitInfo(strAppPath,"window","interval",strTemp,m_iCheckInterval,1);
		pFile->CheckInitValue(m_iCheckInterval,2000,200000);
		pFile->ReadInitInfo(strAppPath,"window","runstatus",strTemp,m_iRunStatus,1);
		pFile->CheckInitValue(m_iRunStatus,0,1);
		SetDlgItemText(IDC_APPNAME,"Ӧ�ó�����: "+m_strStrAppName);
		
		SetDlgItemText(IDC_WINDOWNAME,"���ڱ���: "+m_strWindowName);
		strTemp.Format("ʱ����[����]: %d",m_iCheckInterval);
		SetDlgItemText(IDC_INTERVAL,strTemp);
		SetWindowText("���ظ�������_"+m_strWindowName);
	}
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
LRESULT CReStartAppDlg::OnReInitparam(WPARAM wParam,LPARAM lParam)
{
	InitParams();
	return 0;
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CReStartAppDlg::OnSize(UINT nType, int cx, int cy)
{
	ShowWindow(SW_HIDE);
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CReStartAppDlg::CloseErrorWindow()
{
	//EnumChildWindows(NULL,EnumChildProcEx,(LPARAM)0);
}


/************************************************************************/
/*                                                                      */
/************************************************************************/
//��������������̺߳���
void CReStartAppDlg::CloseProcFun(LPVOID lpParam)
{
	WriteLog("�رմ�����߳�����",0);
	CReStartAppDlg *pWindow = (CReStartAppDlg *)lpParam;
	pWindow->CloseProc();	
	WriteLog("�رմ�����߳��˳�",0);
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CReStartAppDlg::CloseProc()
{
	HWND hwnd = NULL;
	CString strWindowName1 = "";
	CString strWindowName2 = "";
	//m_strWindowName = "�������������";
	//m_strStrAppName = "TESTstr.exe";
	while (g_iExitFlag == FALSE)
	{
		Delay(3000,&g_iExitFlag);
		if (g_iExitFlag == TRUE)
		{
			break;
		}
		hwnd = NULL;
		strWindowName1.Format("%s - Ӧ�ó������",m_strStrAppName);
		strWindowName2.Format("%s: %s - Ӧ�ó������",m_strWindowName,m_strStrAppName);
		//strWindowName1.Format("%s - Ӧ�ó������","TESTstr.exe");
		//strWindowName2.Format("%s: %s - Ӧ�ó������","�������������","TESTstr.exe");
		//WriteLog(strWindowName1,0);
		//WriteLog(strWindowName2,0);
		hwnd = ::FindWindow("#32770",strWindowName1);
		if (hwnd!=NULL)
		{
			WriteLog("�ҵ����󴰿�:"+strWindowName1,0);
			CloseErrorWindowEx(hwnd);
		}
// 		else
// 		{
// 			SetDlgItemText(IDC_SHOWINFO,CTime::GetCurrentTime().Format("%Y-%m-%d %H:%M:%S")+"	Ŀ�������������");	
// 		}
		hwnd = NULL;
		hwnd = ::FindWindow("#32770",strWindowName2);
		if (hwnd!=NULL)
		{
			WriteLog("�ҵ����󴰿�:"+strWindowName2,0);
			CloseErrorWindowEx(hwnd);
		}
// 		else
// 		{
// 			SetDlgItemText(IDC_SHOWINFO,CTime::GetCurrentTime().Format("%Y-%m-%d %H:%M:%S")+"	δ����");	
// 		}
		//EnumChildWindows(NULL,EnumChildProcEx,(LPARAM)0);		
	}
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CReStartAppDlg::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	g_iExitFlag = TRUE;
	KillTimer(1);
	Sleep(200);
	CDialog::OnClose();
}

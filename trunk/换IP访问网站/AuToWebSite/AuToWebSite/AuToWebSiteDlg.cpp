// AuToWebSiteDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "AuToWebSite.h"
#include "AuToWebSiteDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif



CString gl_strTxtPath = "";
CString gl_strBanetPath = "";
CString gl_strTracksPath = "";
CString gl_strIpGetPath = "";
int		gl_iGetType = 0; //0,1�ֱ��ʾ
int		gl_iIpDelay = 0;
CString gl_strExePath = "";

bool    g_bExitFlag = false;
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


// CAuToWebSiteDlg �Ի���




CAuToWebSiteDlg::CAuToWebSiteDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAuToWebSiteDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CAuToWebSiteDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_TYPE, m_comType);
}

BEGIN_MESSAGE_MAP(CAuToWebSiteDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BTN_BROWSER, &CAuToWebSiteDlg::OnBnClickedBtnBrowser)
	ON_BN_CLICKED(IDC_BTN_BROWSER2, &CAuToWebSiteDlg::OnBnClickedBtnBrowser2)
	ON_BN_CLICKED(IDC_BTN_BROWSER3, &CAuToWebSiteDlg::OnBnClickedBtnBrowser3)
	ON_BN_CLICKED(IDC_BTN_BROWSER4, &CAuToWebSiteDlg::OnBnClickedBtnBrowser4)
	ON_BN_CLICKED(IDC_BTN_SAVE, &CAuToWebSiteDlg::OnBnClickedBtnSave)
	ON_BN_CLICKED(IDC_BTN_QUIT, &CAuToWebSiteDlg::OnBnClickedBtnQuit)
	ON_BN_CLICKED(IDC_BTN_START, &CAuToWebSiteDlg::OnBnClickedBtnStart)
	ON_BN_CLICKED(IDC_BTN_STOP, &CAuToWebSiteDlg::OnBnClickedBtnStop)
	ON_BN_CLICKED(IDC_BUTTON1, &CAuToWebSiteDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CAuToWebSiteDlg ��Ϣ�������

BOOL CAuToWebSiteDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

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
	
	GetDlgItem(IDC_BTN_STOP)->EnableWindow(FALSE);
	GetAppPath(gl_strExePath);
	m_comType.MoveWindow(117,47,80,50);
	InitParams();

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CAuToWebSiteDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CAuToWebSiteDlg::OnPaint()
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
//
HCURSOR CAuToWebSiteDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CAuToWebSiteDlg::OnBnClickedBtnBrowser()
{
	GetFilePath(IDC_EDIT_BANET);
}

void CAuToWebSiteDlg::OnBnClickedBtnBrowser2()
{
	GetFilePath(IDC_EDIT_TRACKS);
}

void CAuToWebSiteDlg::OnBnClickedBtnBrowser3()
{
	GetFilePath(IDC_EDIT_IPGET);
}

void CAuToWebSiteDlg::OnBnClickedBtnBrowser4()
{
	GetFilePath(IDC_EDIT_URL);
}

void CAuToWebSiteDlg::GetFilePath(int nCtrlId)
{
	CString strPath = "";
	char szFilter[255] = {0};
	strcpy(szFilter,"Text files(*.txt)|*.txt");
	if (nCtrlId != IDC_EDIT_URL)
	{
		strcpy(szFilter,"Exec files(*.exe)|*.exe");
	}
	CFileDialog dlg(1,NULL,NULL,NULL,szFilter); 
	if(dlg.DoModal()==IDOK) 
	{
		strPath = dlg.GetPathName();
	}
	if (strPath=="")
	{
		return;
	}
	SetDlgItemText(nCtrlId, strPath);
}

void CAuToWebSiteDlg::InitParams()
{
	m_comType.AddString("proxylist");
	m_comType.AddString("todaylist");

	CString strNode = "set";
	CString strPath = gl_strExePath + "set.ini";
	char szInfo[255] = {0};
	::GetPrivateProfileString(strNode,"txtpath","", szInfo,255, strPath);
	gl_strTxtPath = szInfo;

	memset(szInfo,0,255);
	::GetPrivateProfileString(strNode,"tracpath","", szInfo,255, strPath);
	gl_strTracksPath = szInfo;

	memset(szInfo,0,255);
	::GetPrivateProfileString(strNode,"ipgetpath","", szInfo,255, strPath);
	gl_strIpGetPath = szInfo;

	memset(szInfo,0,255);
	::GetPrivateProfileString(strNode,"banetpath","", szInfo,255, strPath);
	gl_strBanetPath = szInfo;

	gl_iIpDelay = ::GetPrivateProfileInt(strNode,"ipdelay",0,strPath);
	gl_iGetType = ::GetPrivateProfileInt(strNode,"gettype",0,strPath);

	m_comType.SetCurSel(gl_iGetType);
	SetDlgItemInt(IDC_EDIT_IPDELAY,gl_iIpDelay);
	SetDlgItemText(IDC_EDIT_URL,gl_strTxtPath);
	SetDlgItemText(IDC_EDIT_TRACKS,gl_strTracksPath);
	SetDlgItemText(IDC_EDIT_IPGET,gl_strIpGetPath);
	SetDlgItemText(IDC_EDIT_BANET,gl_strBanetPath);
}

void CAuToWebSiteDlg::OnBnClickedBtnSave()
{
	GetDlgItemText(IDC_EDIT_URL,gl_strTxtPath);
	GetDlgItemText(IDC_EDIT_TRACKS,gl_strTracksPath);
	GetDlgItemText(IDC_EDIT_IPGET,gl_strIpGetPath);
	GetDlgItemText(IDC_EDIT_BANET,gl_strBanetPath);
	gl_iIpDelay = GetDlgItemInt(IDC_EDIT_IPDELAY);
	gl_iGetType = m_comType.GetCurSel();

	CString strNode = "set";
	CString strPath = gl_strExePath + "set.ini";
	::WritePrivateProfileString(strNode,"txtpath", gl_strTxtPath, strPath);
	::WritePrivateProfileString(strNode,"tracpath", gl_strTracksPath, strPath);
	::WritePrivateProfileString(strNode,"ipgetpath", gl_strIpGetPath, strPath);
	::WritePrivateProfileString(strNode,"banetpath", gl_strBanetPath, strPath);
	CString strTemp = "";
	strTemp.Format("%d",gl_iIpDelay);
	::WritePrivateProfileString(strNode,"ipdelay", strTemp, strPath);
	strTemp.Format("%d",gl_iGetType);
	::WritePrivateProfileString(strNode,"gettype", strTemp, strPath);
}

int CAuToWebSiteDlg::GetAppPath(CString &strAppPath)
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

//�˳�
void CAuToWebSiteDlg::OnBnClickedBtnQuit()
{
	PostMessage(WM_CLOSE,0,0);
}

//��ʼ
void CAuToWebSiteDlg::OnBnClickedBtnStart()
{
	OnBnClickedBtnSave();
	g_bExitFlag = false;
	AfxBeginThread((AFX_THREADPROC)ThreadFunc,(LPVOID)this);	
}

//ֹͣ
void CAuToWebSiteDlg::OnBnClickedBtnStop()
{
	g_bExitFlag = true;
}

/************************************************************************/
/*  ˢ���߳�                                                            */
/************************************************************************/
void CAuToWebSiteDlg::ThreadFunc(LPVOID lParam)
{
	CAuToWebSiteDlg *pParam = ((CAuToWebSiteDlg *)lParam);
	if ( pParam )
	{
		pParam->GetDlgItem(IDC_BTN_START)->EnableWindow(FALSE);
		pParam->GetDlgItem(IDC_BTN_STOP)->EnableWindow(TRUE);
		pParam->RunFunc();
		pParam->ShowMessage("�߳��˳�");
		pParam->GetDlgItem(IDC_BTN_START)->EnableWindow(TRUE);
		pParam->GetDlgItem(IDC_BTN_STOP)->EnableWindow(FALSE);
		g_bExitFlag = true;
	}
}

void CAuToWebSiteDlg::RunFunc()
{
	while(!g_bExitFlag)
	{
		Sleep(200);
	}
}

void CAuToWebSiteDlg::ShowMessage(CString strInfo)
{
	
}
void CAuToWebSiteDlg::OnBnClickedButton1()
{
	HWND hwnd = ::FindWindow("WindowsForms10.Window.8.app.0.378734a","ProxyClient 2010v1 beta1   QQ:6323623");
	hwnd = m_wndFind.GetChildWindow(hwnd,12);
	HWND hwndChild = m_wndFind.GetChildWindow(hwnd,4);
	hwndChild = m_wndFind.GetChildWindow(hwndChild,1);
	//�жϴ˾���Ƿ���ָ��һ���б�
	int iRet = ListView_GetItemCount(hwndChild);
	int iii = 0;
}

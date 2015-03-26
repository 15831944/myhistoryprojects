// TESTTESTDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "TESTTEST.h"
#include "TESTTESTDlg.h"
#include ".\testtestdlg.h"
#include <afxmt.h>
#include <afxtempl.h>
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CCriticalSection g_section;

// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���
typedef struct tagThreadParam
{
	CTESTTESTDlg* pThis;
	int iThreadNO;
} ThreadParam;

typedef int (FAR __stdcall *FCalculate)(char*);
FCalculate Calculate = NULL;
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


// CTESTTESTDlg �Ի���



CTESTTESTDlg::CTESTTESTDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTESTTESTDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_hModule = ::LoadLibraryEx((LPCTSTR)"R_Assistant.dll",NULL,LOAD_WITH_ALTERED_SEARCH_PATH);	
	Calculate = (FCalculate)GetProcAddress(m_hModule, "Calculate");
}
CTESTTESTDlg::~CTESTTESTDlg()
{
	if (m_hModule)
	{
		FreeLibrary(m_hModule);
	}
}
void CTESTTESTDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CTESTTESTDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()


// CTESTTESTDlg ��Ϣ�������

BOOL CTESTTESTDlg::OnInitDialog()
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
	SetDlgItemText(IDC_EDIT1,"source(\"E:\\\\Test Project\\\\R\\\\CN1\\\\modeling.R\")");
	SetDlgItemText(IDC_EDIT1,"E:\\R\\CN%d\\modeling.R");
	return TRUE;  // ���������˿ؼ��Ľ��㣬���򷵻� TRUE
}

void CTESTTESTDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CTESTTESTDlg::OnPaint() 
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
HCURSOR CTESTTESTDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CTESTTESTDlg::OnBnClickedOk()
{
	/*CString strPath = "";
	GetDlgItemText(IDC_EDIT1,strPath);
	char szPath[256] = {0};
	strcpy(szPath,strPath);
	int iRet = Calculate(szPath);
	CString strShow = "";
	strShow.Format("%d",iRet);
	AfxMessageBox(strShow);*/
	for (int i=1;i<2;i++)
	{
		//if (i!=6)
		//{
			///continue;
		//}
		ThreadParam *lpParam = new ThreadParam;
		lpParam->pThis = this;
		lpParam->iThreadNO = i;
		AfxBeginThread((AFX_THREADPROC)ThreadRun,(LPVOID)lpParam);
		//Sleep(1000);
	}
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CTESTTESTDlg::ThreadRun(LPVOID lpParam)
{
	ThreadParam *pParam = (ThreadParam *)lpParam;
	((CTESTTESTDlg*)pParam->pThis)->ThreadProc(pParam->iThreadNO);
	delete pParam;
	pParam = NULL;
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CTESTTESTDlg::ThreadProc(int iIndex)
{
	CString strFormat = "";
	GetDlgItemText(IDC_EDIT1,strFormat);
	CString strPath = "";
	strPath.Format(strFormat,iIndex);
	CString strShow = "";
	strShow.Format("Thread %d %s",iIndex,strPath);
	ShowMessage(strShow);
	char szPath[256] = {0};
	strcpy(szPath,strPath);
	Sleep(iIndex * 100 * 3);
	int iRet = Calculate(szPath);	
	strShow.Format("Thread %d Return:%d",iIndex,iRet);
	ShowMessage(strShow);
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CTESTTESTDlg::ShowMessage(CString strMsg)
{
	g_section.Lock();
	CString strOld = "";
	GetDlgItemText(IDC_EDIT2,strOld);
	strMsg = strOld + strMsg +"\r\n";
	SetDlgItemText(IDC_EDIT2,strMsg);
	g_section.Unlock();
}
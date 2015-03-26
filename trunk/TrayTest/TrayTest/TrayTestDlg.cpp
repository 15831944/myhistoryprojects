// TrayTestDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "TrayTest.h"
#include "TrayTestDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


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


// CTrayTestDlg �Ի���




CTrayTestDlg::CTrayTestDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTrayTestDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}
CTrayTestDlg::~CTrayTestDlg()
{
	if (m_pTray)
	{
		delete m_pTray;
	}
}
void CTrayTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CTrayTestDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON1, &CTrayTestDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CTrayTestDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CTrayTestDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CTrayTestDlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, &CTrayTestDlg::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON6, &CTrayTestDlg::OnBnClickedButton6)
	ON_WM_CLOSE()
	ON_COMMAND(ID__32771, &CTrayTestDlg::OnTrayMenu)
	ON_COMMAND(ID_MENUEXIT, &CTrayTestDlg::OnMenuexit)
	ON_COMMAND(ID_TESTMENU, &CTrayTestDlg::OnTestmenu)
	ON_COMMAND(ID_MENUSHOW, &CTrayTestDlg::OnMenushow)
END_MESSAGE_MAP()


// CTrayTestDlg ��Ϣ�������

BOOL CTrayTestDlg::OnInitDialog()
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

	m_pTray = new CSystemTray(NULL,WM_ICON_NOTIFY,"Hello, Test Message!",AfxGetApp()->LoadIcon(IDI_ICON1),IDR_MENU1);

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CTrayTestDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CTrayTestDlg::OnPaint()
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
HCURSOR CTrayTestDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CTrayTestDlg::OnBnClickedButton1()
{
	m_pTray->SetIcon(IDI_ICON1);
}

void CTrayTestDlg::OnBnClickedButton2()
{
	m_pTray->SetIcon(IDI_ICON2);
}

void CTrayTestDlg::OnBnClickedButton3()
{
	m_pTray->SetIcon(IDI_ICON3);
}

void CTrayTestDlg::OnBnClickedButton4()
{
	m_pTray->ShowBalloonTip("�����");
}

void CTrayTestDlg::OnBnClickedButton5()
{
	m_pTray->SetIconList(IDI_ICON1, IDI_ICON4);
	m_pTray->Animate(500);
}

void CTrayTestDlg::OnBnClickedButton6()
{
	m_pTray->StopAnimation(IDI_ICON2);
}

void CTrayTestDlg::OnClose()
{
	ShowWindow(SW_HIDE);
}

void CTrayTestDlg::OnTrayMenu()
{
	CDialog::OnClose();
}

void CTrayTestDlg::OnMenuexit()
{
	::PostQuitMessage(0);
}

void CTrayTestDlg::OnTestmenu()
{
	::PostQuitMessage(0);
}

void CTrayTestDlg::OnMenushow()
{
	ShowWindow(SW_SHOW);
}

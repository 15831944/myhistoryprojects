// VSHackNewDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "VSHackNew.h"
#include "VSHackNewDlg.h"

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


// CVSHackNewDlg �Ի���




CVSHackNewDlg::CVSHackNewDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CVSHackNewDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CVSHackNewDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CVSHackNewDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


// CVSHackNewDlg ��Ϣ�������

BOOL CVSHackNewDlg::OnInitDialog()
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

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CVSHackNewDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CVSHackNewDlg::OnPaint()
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
HCURSOR CVSHackNewDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//�رվ�����̺߳���
DWORD WINAPI CVSHackNewDlg::CloseWarnProc(LPVOID lpParameter)
{
	((CVSHackNewDlg *)lpParameter )->CloseWarnWindow();
	return 0;
}
//�رվ����
void CVSHackNewDlg::CloseWarnWindow()
{
	HWND hwndWarn = NULL;
	hwndWarn = ::FindWindow(_T("#32770"), _T("VS������Ϸƽ̨")); //��Ա�����
	if(hwndWarn != NULL)
	{
		//��ϵͳʱ�����30��,�ܿ��ͻ�����֤
		SYSTEMTIME systime;
		GetLocalTime(&systime); 
		union 
		{
			FILETIME ft;
			ULONGLONG ut;
		}t;
		DWORD dwMiniSeconds=30;
		SystemTimeToFileTime(&systime, &t.ft);
		t.ut-=UInt32x32To64(dwMiniSeconds,10000000);//��30��
		FileTimeToSystemTime(&t.ft, &systime);
		SetLocalTime(&systime);

		::SendMessage(hwndWarn,WM_CLOSE,0,0);

		Sleep(200);		//SLEEP 100 ����

		SystemTimeToFileTime(&systime, &t.ft);
		t.ut+=UInt32x32To64(dwMiniSeconds,10000000);//��30��
		FileTimeToSystemTime(&t.ft, &systime);
		SetLocalTime(&systime);
		//����ȷ��ϵͳʱ�仹ԭ
		//dblhitFlag = true;
	}
	hwndWarn = NULL;
	hwndWarn = ::FindWindow(_T("#32770"), _T("VSClient"));//�����ظ����뷿�侯���
	if(hwndWarn != NULL)
	{
		::SendMessage(hwndWarn,WM_CLOSE,0,0);
		Sleep(200);		//SLEEP 100 ����
		SYSTEMTIME systime;
		GetLocalTime(&systime);
		union 
		{
			FILETIME ft;
			ULONGLONG ut;
		}t;
		DWORD dwMiniSeconds=10;
		SystemTimeToFileTime(&systime, &t.ft);
		t.ut+=UInt32x32To64(dwMiniSeconds,10000000);//��10��
		FileTimeToSystemTime(&t.ft, &systime);
		SetLocalTime(&systime);
		//dblhitFlag = true;
	}
}
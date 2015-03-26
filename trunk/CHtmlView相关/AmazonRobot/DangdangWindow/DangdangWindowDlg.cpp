// DangdangWindowDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DangdangWindow.h"
#include "DangdangWindowDlg.h"
#include "CheckCodeInput.h"
#include "DangdangClass.h"
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


// CDangdangWindowDlg �Ի���




CDangdangWindowDlg::CDangdangWindowDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDangdangWindowDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CDangdangWindowDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_CountList);
}

BEGIN_MESSAGE_MAP(CDangdangWindowDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BTN_START, &CDangdangWindowDlg::OnBnClickedBtnStart)
	ON_BN_CLICKED(IDC_BTN_SUBMIT, &CDangdangWindowDlg::OnBnClickedBtnSubmit)
	ON_BN_CLICKED(IDC_BTN_IMPORT, &CDangdangWindowDlg::OnBnClickedBtnImport)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON2, &CDangdangWindowDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BTN_CODE, &CDangdangWindowDlg::OnBnClickedBtnCode)
END_MESSAGE_MAP()


// CDangdangWindowDlg ��Ϣ�������

BOOL CDangdangWindowDlg::OnInitDialog()
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
	SetTableStyle();
	m_pSubmitEvent = new CEvent(FALSE, TRUE, NULL, NULL);
	GetDlgItem(IDC_BTN_SUBMIT)->EnableWindow(FALSE);
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CDangdangWindowDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CDangdangWindowDlg::OnPaint()
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
HCURSOR CDangdangWindowDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
void CDangdangWindowDlg::SetTableStyle()
{
	CListCtrl* conList = &m_CountList;
	DWORD dwSytle=::GetWindowLong(conList->m_hWnd,GWL_STYLE);
	// ����Ϊ������ʽ
	SetWindowLong(conList->m_hWnd,GWL_STYLE,dwSytle|LVS_REPORT);
	DWORD ExStyle=conList->GetExtendedStyle();
	conList->SetExtendedStyle(ExStyle|LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
	CImageList image;
	image.Create(1,25, ILC_COLOR, 0, 0);
	conList->SetImageList(&image, LVSIL_STATE);
	conList->MoveWindow(7,5,780,490);
	conList->InsertColumn(0,"���",LVCFMT_CENTER,50);
	conList->InsertColumn(1,"����",LVCFMT_LEFT,110);
	conList->InsertColumn(2,"����",LVCFMT_LEFT,110);
	conList->InsertColumn(3,"��ַ",LVCFMT_LEFT,0);
	conList->InsertColumn(4,"�ռ���",LVCFMT_LEFT,100);
	conList->InsertColumn(5,"״̬",LVCFMT_LEFT,150);
	conList->InsertColumn(6,"��֤��",LVCFMT_LEFT,70);
}

void CDangdangWindowDlg::OnBnClickedBtnStart()
{
	m_pSubmitEvent->ResetEvent();
	int iCount = m_AutoOrderList.size();
	if (iCount < 1)
	{
		return;
	}
	for (int i = 0; i < iCount; i++)
	{
		HANDLE handle = StartWorkingEx();
		if(handle == NULL)
		{
			
		}
	}
	GetDlgItem(IDC_BTN_START)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_IMPORT)->EnableWindow(FALSE);
}

void CDangdangWindowDlg::ThreadProc()
{
	//add cart
	//login
	//caculate
	//show checkcode
	//wait for submit command
	AfxMessageBox("aa");
	::WaitForSingleObject(m_pSubmitEvent->m_hObject, INFINITE);
	AfxMessageBox("bb");
	//submit
}

void CDangdangWindowDlg::OnBnClickedBtnSubmit()
{
	MarkTheadToStop();
	m_pSubmitEvent->SetEvent();
}

void CDangdangWindowDlg::OnBnClickedBtnImport()
{
	m_AutoOrderList.push_back(1);
	m_AutoOrderList.push_back(1);
}

void CDangdangWindowDlg::OnClose()
{
	if (m_pSubmitEvent)
	{
		delete m_pSubmitEvent;
	}
	__super::OnClose();
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
BOOL CDangdangWindowDlg::PreTranslateMessage( MSG* pMsg)
{
	if( pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE)
	{
		return TRUE;      //���ε�Esc��
	}
	if( pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
	{
// 		HWND hWnd=::GetFocus();
// 		int iID=::GetDlgCtrlID(hWnd);
// 		if(iID==IDC_EDIT_CODE)
// 		{
// 			OnBnClickedBtnRegiste();
// 		}
		return TRUE;      //���ε�Enter��
	}
	return CDialog::PreTranslateMessage( pMsg );
}
void CDangdangWindowDlg::OnBnClickedButton2()
{
	CDangdangClass dangdangClass;
	dangdangClass.AddToCard("21094853", 1);
	dangdangClass.Login("tragicguy@yutone.com", "wawawo20", "");
	GoodOrderStru goodStr;
	goodStr.strRecver = "ccc";
	goodStr.strAddr = "ddd";
	goodStr.strProvince = "141";
	goodStr.strCity = "1401";
	goodStr.strTown = "1410104";
	goodStr.strPostCode = "450000";
	goodStr.strPhone = "13928272626";
	goodStr.strBillHeader = "����";
	goodStr.strBillContent = "ͼ��";
	int iRet = 0;
	dangdangClass.AddAddress(&goodStr, iRet);
}

void CDangdangWindowDlg::OnBnClickedBtnCode()
{
	CDangdangClass dangdangClass;
	bool bRet = dangdangClass.Login("tragicguy@yutone.com", "wawawo201", "");
	AfxMessageBox(bRet ? "1" : "0");
}

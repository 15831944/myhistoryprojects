// ConferenceProcesserDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ConferenceProcesser.h"
#include "ConferenceProcesserDlg.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#pragma warning(disable:4099)

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


// CConferenceProcesserDlg �Ի���




CConferenceProcesserDlg::CConferenceProcesserDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CConferenceProcesserDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CConferenceProcesserDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_list_show);
}

BEGIN_MESSAGE_MAP(CConferenceProcesserDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BTN_START, &CConferenceProcesserDlg::OnBnClickedBtnStart)
	ON_BN_CLICKED(IDC_BUTTON2, &CConferenceProcesserDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BTN_EXIT, &CConferenceProcesserDlg::OnBnClickedBtnExit)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CConferenceProcesserDlg ��Ϣ�������

BOOL CConferenceProcesserDlg::OnInitDialog()
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
	
	CWebBase::WebInit();
	InitListCtrl();
	SetWindowText("��Ƶ�Ự��ʾϵͳ����ת��");
	m_ProgramConfig.SetConfigFileName("set.ini");
	
	CString strUrl = m_ProgramConfig.ReadStringConfigParameter("set", "url", "http://svcm.paic.com.cn/svcm/display/display.xml");
	int nInterval = m_ProgramConfig.ReadIntConfigParameter("set", "interval", 10000);
	SetDlgItemText(IDC_EDIT_PATH, strUrl);
	SetDlgItemInt(IDC_EDIT_INTERVAL, nInterval);
	GetDlgItem(IDC_BUTTON2)->ShowWindow(SW_HIDE);
#ifdef DEBUG
	GetDlgItem(IDC_BUTTON2)->ShowWindow(SW_SHOW);
#endif
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
void CConferenceProcesserDlg::InitListCtrl()
{
	DWORD dwSytle=::GetWindowLong(m_list_show.m_hWnd,GWL_STYLE);
	// ����Ϊ������ʽ
	SetWindowLong(m_list_show.m_hWnd,GWL_STYLE,dwSytle|LVS_REPORT);
	DWORD ExStyle=m_list_show.GetExtendedStyle();
	m_list_show.SetExtendedStyle(ExStyle|LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);

	::SendMessage(m_list_show.m_hWnd, LVM_SETEXTENDEDLISTVIEWSTYLE,
		LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);

	m_list_show.InsertColumn(0,_T("����ID"),LVCFMT_LEFT,70);
	m_list_show.InsertColumn(1,_T("��������"),LVCFMT_LEFT,120);
	m_list_show.InsertColumn(2,_T("��ʼʱ��"),LVCFMT_LEFT,90);
	m_list_show.InsertColumn(3,_T("����ʱ��"),LVCFMT_LEFT,90);
	m_list_show.InsertColumn(4,_T("����"),LVCFMT_LEFT,140);
	m_list_show.InsertColumn(5,_T("��������"),LVCFMT_LEFT,140);
	m_list_show.InsertColumn(6,_T("��������"),LVCFMT_LEFT,60);
	m_list_show.InsertColumn(7,_T("������״̬"),LVCFMT_LEFT,70);
	m_list_show.InsertColumn(8,_T("����"),LVCFMT_LEFT,70);
	m_list_show.MoveWindow(10, 60, 900, 420);
}
void CConferenceProcesserDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CConferenceProcesserDlg::OnPaint()
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
HCURSOR CConferenceProcesserDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CConferenceProcesserDlg::OnBnClickedBtnStart()
{
	CString strUrl = "";
	GetDlgItemText(IDC_EDIT_PATH, strUrl);
	int iInterval = GetDlgItemInt(IDC_EDIT_INTERVAL);
	if(!m_DataProcess.InitInfo(strUrl, iInterval, true, true))
	{
		AfxMessageBox("��ַ�Ƿ�");
	}
	m_ProgramConfig.WriteStringConfigParameter("set", "url", strUrl);
	int nInterval = m_ProgramConfig.WriteIntConfigParameter("set", "interval", iInterval);
	m_DataProcess.SetShowHwnd(m_list_show.m_hWnd, ::GetDlgItem(m_hWnd, IDC_STATIC_NOTICE));
	m_DataProcess.StartWorking();
	GetDlgItem(IDC_BTN_START)->EnableWindow(FALSE);
}

void CConferenceProcesserDlg::OnBnClickedButton2()
{

	ConferenceInfo info;
	info.boxnos = "BOX00100";
	info.updatetime = "2011-07-01 13:00:00";
	info.version = "2.0";
	ConferenceRecord record;
	record.bookpeople = CFunction::ConvertGBKToUtf8Ex("�۾�");
	record.chairman = CFunction::ConvertGBKToUtf8Ex("�۾�");
	record.currentfloor = false;
	record.displayformat.ishighlight = true;
	record.endtime = "2011-07-01 17:00:00";
	record.starttime = "2011-07-01 11:00:00";
	record.roomid = "0001";
	record.roomname = CFunction::ConvertGBKToUtf8Ex("������A");
	record.topic = CFunction::ConvertGBKToUtf8Ex("����һ��δ����չ�滮");
	record.peoples.push_back(CFunction::ConvertGBKToUtf8Ex("����һ"));
	record.peoples.push_back(CFunction::ConvertGBKToUtf8Ex("���Ŷ�"));
	record.peoples.push_back(CFunction::ConvertGBKToUtf8Ex("������"));
	info.record.push_back(record);
	
	info.ToXml("c:\\bbb.xml");
	return;
	//m_DataProcess.ToXml();
	TiXmlDocument xmlDoc;  
	
	TiXmlDeclaration *pDeclaration = new TiXmlDeclaration(_T("1.0"),_T("utf-8"),_T(""));
	if (!pDeclaration)	return;
	xmlDoc.LinkEndChild(pDeclaration);  
	// ����һ�����ڵ㣺MyApp  
	TiXmlElement *pRootEle = TinyXmlFunction::DocNewElement("MyApp", &xmlDoc);
	// �����ӽڵ㣺Messages  
	TiXmlElement *pMsg = TinyXmlFunction::NewElement("Messages", pRootEle);
	
	TinyXmlFunction::NewElement("Welcome", pMsg, "fuck you");
	TinyXmlFunction::NewElement("Farewell", pMsg, "Thank you for using MyApp");
	
	// �����ӽڵ㣺Windows  
	TiXmlElement *pWindows = TinyXmlFunction::NewElement("Windows", pRootEle);
	if (!pWindows)  return;

	// �����ӽڵ㣺Window  
	TiXmlElement *pWindow = TinyXmlFunction::NewElement("Window", pWindows);
	if (!pWindow)  return;

	// ���ýڵ�Window��ֵ  
	pWindow->SetAttribute(_T("name"),_T("MainFrame"));  
	pWindow->SetAttribute(_T("x"),_T("5"));
	pWindow->SetAttribute(_T("y"),_T("15"));
	pWindow->SetAttribute(_T("w"),_T("400"));
	pWindow->SetAttribute(_T("h"),_T("250"));

	// �����ӽڵ㣺Connection  
	TiXmlElement *pConnection  = TinyXmlFunction::NewElement("Connection", pRootEle);
	if (!pConnection)  return;
	// ���ýڵ�Connection��ֵ  
	pConnection->SetAttribute(_T("ip"),_T("192.168.0.1"));  
	pConnection->SetAttribute(_T("timeout"),_T("123.456000"));  
	xmlDoc.SaveFile("c:\\df.xml");  
	return ;  
}

void CConferenceProcesserDlg::OnBnClickedBtnExit()
{
	PostMessage(WM_CLOSE, 0, 0);
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
BOOL CConferenceProcesserDlg::PreTranslateMessage( MSG* pMsg )
{
	if( pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE)
	{
		return TRUE;      //���ε�Esc��
	}
	if( pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
	{
		return TRUE;      //���ε�Enter��
	}
	return CDialog::PreTranslateMessage(pMsg);
}
void CConferenceProcesserDlg::OnClose()
{
	m_DataProcess.StopWorking();
	CDialog::OnClose();
}

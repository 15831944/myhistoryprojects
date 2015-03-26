// FileTransferTestDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "FileTransferTest.h"
#include "FileTransferTestDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CFileTransferTestDlg �Ի���


#define MY_SQUITMSG (WM_USER + 1000)
#define MY_CQUITMSG (WM_USER + 1001)

CFileTransferTestDlg::CFileTransferTestDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFileTransferTestDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CFileTransferTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_IPADDRESS1, m_ServerIP);
	DDX_Control(pDX, IDC_EDIT3, m_ServerPort);
	DDX_Control(pDX, IDC_EDIT1, m_ServerListenPort);
}

BEGIN_MESSAGE_MAP(CFileTransferTestDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON1, &CFileTransferTestDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CFileTransferTestDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CFileTransferTestDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CFileTransferTestDlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, &CFileTransferTestDlg::OnBnClickedButton5)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CFileTransferTestDlg ��Ϣ�������

BOOL CFileTransferTestDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	m_ServerListenPort.SetWindowTextA("4300");
	m_ServerPort.SetWindowTextA("4300");
	m_ServerIP.SetAddress(127,0,0,1);

	OnBnClickedButton1();
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CFileTransferTestDlg::OnPaint()
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
HCURSOR CFileTransferTestDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CFileTransferTestDlg::OnBnClickedButton1()
{
	if(!m_File.InitFileDLLFunction())
	{
		AfxMessageBox("��ʼ��ʧ�ܣ�");
	}
}

void CFileTransferTestDlg::OnBnClickedButton2()
{
	char buf[8];
	memset(buf, 0, 8);

	m_ServerListenPort.GetWindowText(buf, 8);
	int i = atoi(buf);

	if (!m_File.m_funcCreateFileServer(i, this->m_hWnd, MY_SQUITMSG, "1033"))
	{
		AfxMessageBox("CreateFileServerʧ�ܣ�");
	}
}

void CFileTransferTestDlg::OnBnClickedButton3()
{
	m_File.m_funcStopFileServer();
}

void CFileTransferTestDlg::OnBnClickedButton4()
{
	char buf[8];
	memset(buf, 0, 8);

	m_ServerPort.GetWindowText(buf, 8);
	int i = atoi(buf);

	char IPBuf[20];
	memset(IPBuf, 0, 20);

	m_ServerIP.GetWindowText(IPBuf, 20);

	if (!m_File.m_funcCreateFileClient(IPBuf, i, this->m_hWnd, MY_CQUITMSG, true))
	{
		AfxMessageBox("CreateFileClientʧ�ܣ�");
	}
}

void CFileTransferTestDlg::OnBnClickedButton5()
{
	m_File.m_funcStopFileClient();
}

void CFileTransferTestDlg::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	m_File.m_funcStopFileServer();
	m_File.m_funcStopFileClient();
	CDialog::OnClose();
}

LRESULT CFileTransferTestDlg::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: �ڴ����ר�ô����/����û���
	switch (message)
	{
	case MY_SQUITMSG:
		if (1 == wParam)
		{
			CString str = (LPCTSTR) lParam;
			TRACE("Receive file %s success...\r\n", str);
		}
		break;

	case MY_CQUITMSG:
		if (1 == wParam)
		{
			CString str = (LPCTSTR) lParam;
			TRACE("Sent file %s success...\r\n", str);
		}
		break;

	default:
		break;
	}
	return CDialog::DefWindowProc(message, wParam, lParam);
}

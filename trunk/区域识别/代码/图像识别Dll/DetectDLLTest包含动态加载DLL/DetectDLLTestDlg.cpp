// DetectDLLTestDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DetectDLLTest.h"
#include "DetectDLLTestDlg.h"
#include ".\detectdlltestdlg.h"
#include "LoadDllFile.h"
#include "OpenCVCheck.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

extern "C" _declspec(dllimport) void GetBinaryData(LPBYTE m_pSourse,LPBYTE &m_pNewbmp);
extern "C" _declspec(dllimport) CPoint GetCenterPoint(CString picPath,CString xmlPath);
extern "C" _declspec(dllimport) int GetVal(int a,int b,int iFlag);

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


// CDetectDLLTestDlg �Ի���



CDetectDLLTestDlg::CDetectDLLTestDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDetectDLLTestDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CDetectDLLTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CDetectDLLTestDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, OnBnClickedButton2)
END_MESSAGE_MAP()


// CDetectDLLTestDlg ��Ϣ�������

BOOL CDetectDLLTestDlg::OnInitDialog()
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
	
	return TRUE;  // ���������˿ؼ��Ľ��㣬���򷵻� TRUE
}

void CDetectDLLTestDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CDetectDLLTestDlg::OnPaint() 
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
HCURSOR CDetectDLLTestDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CDetectDLLTestDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//OnOK();

	CString sFolderPath;
	BROWSEINFO bi;
	char Buffer[MAX_PATH]; //��ʼ����ڲ���bi��ʼ
	bi.hwndOwner = NULL;
	bi.pidlRoot = NULL;
	bi.pszDisplayName = Buffer; //�˲�����ΪNULL������ʾ�Ի���
	bi.lpszTitle = "��ѡ��ͼƬ����Ŀ¼";
	bi.ulFlags = BIF_RETURNONLYFSDIRS;
	bi.lpfn = NULL;
	bi.iImage = 0;   //��ʼ����ڲ���bi����
	LPITEMIDLIST pIDList = SHBrowseForFolder(&bi);//������ʾѡ��Ի���
	if(pIDList)
	{
		SHGetPathFromIDList(pIDList, Buffer);
		//ȡ���ļ���·����Buffer��
		sFolderPath = Buffer;//��·��������һ��CString������
	}
	CFileFind finder;
	CString findName = sFolderPath + "\\*.jpg";
	BOOL findFlag = finder.FindFile(findName);
	while (findFlag)
	{
		findFlag = finder.FindNextFile();
		if (finder.IsDots())
			continue;

		CString sFileName = finder.GetFileName();
		HINSTANCE hinstLib;
		LoadDllFile loadDll;
		hinstLib = loadDll.GetDllInstance("Dll\\DetectDll.dll");
		MYPROC GetCenterPoint = (MYPROC)loadDll.GetFuncProc(hinstLib,"GetCenterPoint");
		if (GetCenterPoint)
		{
			CPoint detectPoint =GetCenterPoint(sFolderPath+"\\"+sFileName,"c:\\OpenCVTrain\\data.xml");
			CString aa;
			aa.Format("%d,%d",detectPoint.x,detectPoint.y);
			MessageBox(aa);
			FreeLibrary(hinstLib);
		}
		
		
	}
	finder.Close();
}

void CDetectDLLTestDlg::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	LPBYTE pSourse;
	LPBYTE pNewbmp;

	COpenCVCheck mycheck;
	mycheck.ReadBMPFile("C:\\aa.bmp",pSourse);

	GetBinaryData(pSourse,pNewbmp);

	delete pSourse;
	pSourse = NULL;

	CFile file;
	file.Open("E:\\aa.bmp",CFile::modeCreate|CFile::modeReadWrite);
	file.Write(pNewbmp,mycheck.GetPicLength(pNewbmp));
}

void CDetectDLLTestDlg::OnBnClickedButton2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString testStr;
	int result = GetVal(3,5,1);
	testStr.Format("%d-%d=%d",3,5,result);
	MessageBox(testStr);
}

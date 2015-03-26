// QRCodeTestDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "QRCodeTest.h"
#include "QRCodeTestDlg.h"
#include ".\qrcodetestdlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
HMODULE g_hDll;
typedef bool (__stdcall *FnWMEnCode)(const char* szPath, const char* pszStr);
typedef bool (__stdcall *FnWMDeCode)(const char* szPath, char* pszStr);
FnWMEnCode WMEnCode;
FnWMDeCode WMDeCode;
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


// CQRCodeTestDlg �Ի���



CQRCodeTestDlg::CQRCodeTestDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CQRCodeTestDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CQRCodeTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CQRCodeTestDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BTN_ENCODE, OnBnClickedBtnEncode)
	ON_BN_CLICKED(IDC_BTN_DECODE, OnBnClickedBtnDecode)
	ON_BN_CLICKED(IDC_BTN_GETPATH, OnBnClickedBtnGetpath)
END_MESSAGE_MAP()


// CQRCodeTestDlg ��Ϣ�������

BOOL CQRCodeTestDlg::OnInitDialog()
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

	char path[255] = {0};
	::GetCurrentDirectory(255, path);
	strcat(path, "\\QR.dll");
	if (NULL == (g_hDll = ::LoadLibrary(path)))
	{
		AfxMessageBox("����QR.dllʧ�ܣ�");
		exit(0);
	}
	//	MongateSetCmdType = (FnMongateTestConn)::GetProcAddress(g_hDll, "MongateSetCmdType");	
	WMEnCode = (FnWMEnCode)::GetProcAddress(g_hDll, "WMEnCode");
	WMDeCode = (FnWMDeCode)::GetProcAddress(g_hDll, "WMDeCode");	
	return TRUE;  // ���������˿ؼ��Ľ��㣬���򷵻� TRUE
}

void CQRCodeTestDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CQRCodeTestDlg::OnPaint() 
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
HCURSOR CQRCodeTestDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CQRCodeTestDlg::OnBnClickedBtnEncode()
{
	char szPath[255] = {0};
	strcpy(szPath,"e:\\code.bmp");	
	char szStr[1000] = {0};
	CString strContent = "";
	GetDlgItemText(IDC_EDIT_CONTENT,strContent);
	if (strContent.Trim() == "" || strContent.GetLength() > 140)
	{
		MessageBox("���벻�Ϸ�");
		return;
	}
	strcpy(szStr,strContent);
	if(!WMEnCode(szPath,szStr))
	{
		MessageBox("����ʧ��");
	}
	OnBnClickedBtnGetpath();
}

void CQRCodeTestDlg::OnBnClickedBtnDecode()
{
	char szPath[255] = {0};
	strcpy(szPath,"e:\\code.bmp");
	char szStr[1000] = {0};
	if(WMDeCode(szPath,szStr))
	{
		SetDlgItemText(IDC_EDIT_CONTENT,szStr);
	}
	else
	{
		MessageBox("����ʧ��");
	}
}


//��ʾλͼ����
BOOL CQRCodeTestDlg::ShowBmpFile(HDC hdc, int nXDest, int nYDest,LPBYTE lpDIB)
{
	if( lpDIB == NULL)
	{
		return FALSE;
	}
	LPBITMAPINFOHEADER lpBMPInfoHeader=(LPBITMAPINFOHEADER) (lpDIB + sizeof(BITMAPFILEHEADER));
	BITMAPINFO * lpBMPInfo =(BITMAPINFO *)(lpDIB + sizeof(BITMAPFILEHEADER));

	SetDIBitsToDevice(hdc,nXDest,nYDest,lpBMPInfoHeader->biWidth,lpBMPInfoHeader->biHeight,
		0,0,0,lpBMPInfoHeader->biHeight,lpDIB+((BITMAPFILEHEADER *)lpDIB)->bfOffBits,lpBMPInfo,DIB_RGB_COLORS);

	return TRUE;
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
bool CQRCodeTestDlg::ReadBMPFile(LPCTSTR strFileName,LPBYTE &m_lpBMPFileData)
{

	CFile BitmapFile;
	BOOL blOpen=BitmapFile.Open(strFileName, CFile::modeRead,NULL);
	if( !blOpen)
	{
		//CString str = "���ļ�ʧ�ܣ�";
		m_lpBMPFileData = NULL;
		return false;
	}

	unsigned long FileLength=BitmapFile.GetLength();
	m_lpBMPFileData = new BYTE[FileLength];

	ASSERT( m_lpBMPFileData!=NULL );
	BitmapFile.Read(m_lpBMPFileData,FileLength);

	if ( ((LPBITMAPFILEHEADER)m_lpBMPFileData)->bfType != 0x4D42 )
	{
		//"��֧�ֳ�BMP֮����ļ���";
		delete[] m_lpBMPFileData;
		m_lpBMPFileData = NULL;
		return false;;
	}
	return true;
}	

void CQRCodeTestDlg::OnBnClickedBtnGetpath()
{
	LPBYTE pBMPFileData;
	ReadBMPFile("e:\\code.bmp",pBMPFileData);
	ShowBmpFile(::GetDC(m_hWnd),50,150,pBMPFileData);
	if (pBMPFileData)
	{
		delete[] pBMPFileData;
		pBMPFileData = NULL;
	}	
}

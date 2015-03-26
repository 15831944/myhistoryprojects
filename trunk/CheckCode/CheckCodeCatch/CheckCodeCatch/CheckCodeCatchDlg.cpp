// CheckCodeCatchDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "CheckCodeCatch.h"
#include "CheckCodeCatchDlg.h"
#include "WebBase.h"
#include <io.h>
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


// CCheckCodeCatchDlg �Ի���




CCheckCodeCatchDlg::CCheckCodeCatchDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCheckCodeCatchDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	GdiplusStartup(&m_pGdiToken,&m_gdiplusStartupInput,NULL);
}
CCheckCodeCatchDlg::~CCheckCodeCatchDlg()
{
	GdiplusShutdown(m_pGdiToken);
}

void CCheckCodeCatchDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CCheckCodeCatchDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BTN_BEGIN, &CCheckCodeCatchDlg::OnBnClickedBtnBegin)
END_MESSAGE_MAP()


// CCheckCodeCatchDlg ��Ϣ�������

BOOL CCheckCodeCatchDlg::OnInitDialog()
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
	m_strHeader = "Content-Type: application/x-www-form-urlencoded";
	m_strHeader += "\r\nAccept:image/gif, image/x-xbitmap, image/jpeg, image/pjpeg, application/x-shockwave-flash, application/vnd.ms-excel, application/vnd.ms-powerpoint, application/msword, application/x-silverlight, */*";
	m_strHeader += "\r\nAccept-Language:zh-cn";
	m_strHeader += "\r\nAccept-Encoding:gzip, deflate";//
	m_strHeader += "\r\nCache-Control:no-cache";
	m_strHeader += "\r\nUser-Agent:Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1; SV1; .NET CLR 1.1.4322; .NET CLR 2.0.50727)";
	m_strHeader += "\r\nConnection:Keep-Alive";
	m_strDirect = "D:\\PIC\\";
	m_iGetCount = 1;
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CCheckCodeCatchDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CCheckCodeCatchDlg::OnPaint()
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
HCURSOR CCheckCodeCatchDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CCheckCodeCatchDlg::OnBnClickedBtnBegin()
{
	m_ExitFlag = FALSE;
	m_iTotalCount = GetDlgItemInt(IDC_EDIT_GETNUM);
	if (m_iTotalCount < 1)
	{
		return;
	}
	AfxBeginThread((AFX_THREADPROC)ThreadFunc,(LPVOID)this);
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
void CCheckCodeCatchDlg::ThreadFunc(LPVOID lParam)
{
	//CoInitialize(NULL);
	CCheckCodeCatchDlg *pParam = ((CCheckCodeCatchDlg *)lParam);
	if ( pParam )
	{
		//pParam->RegisterFunc();
		//pParam->ShowMessage("�߳��˳�");
		pParam->GetCheckCode();
	}
	//CoUninitialize();

}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CCheckCodeCatchDlg::GetCheckCode()
{
	CWebBase webBase;
	CString strServer = "www.eku8.com";
	CString strError = "";
	CString strObject = "/JH_UserConfig/FundCode.aspx";
	CString strPath = "";
	CString strDestPath = "";
	int iRet = -1;
	CString strTime = CTime::GetCurrentTime().Format("%m%d%H%M%S");;
	srand(time(NULL));
	SetDlgItemText(IDC_INFOSHOW,"�߳�����");
	while (!m_ExitFlag)
	{
		if (m_iGetCount > m_iTotalCount)
		{
			m_ExitFlag = TRUE;
			continue;
		}
		try
		{
			SetDlgItemText(IDC_INFOSHOW,"����ץȡ...");
			SYSTEMTIME   t; 
			GetLocalTime(&t); 			
			strPath.Format("%s%d-%s.gif",m_strDirect,m_iGetCount,strTime);
			strDestPath.Format("%s%d-%s.bmp",m_strDirect,m_iGetCount,strTime);
			strObject.Format("/JH_UserConfig/FundCode.aspx?time=%d%d%d&rand=%d",strTime,t.wMinute,t.wSecond,t.wMilliseconds,rand()%10000);
			m_iGetCount++;
			iRet = webBase.RequestGetPageExExEx(strServer,strObject,m_strHeader,strError,1,0,0,"",80,"","",strPath);
			//ת��һ��
			TranstToBMP(strPath,strDestPath);
			CFile::Remove(strPath);
			//��ʾ��ǰ����
			SetDlgItemInt(IDC_EDIT_GOTNUM,m_iGetCount-1);
			Sleep(10);
		}
		catch (...)
		{
			SetDlgItemText(IDC_INFOSHOW,"ץȡ�����쳣");
		}		
	}	
	SetDlgItemText(IDC_INFOSHOW,"ץȡ�߳��˳�");
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
BOOL CCheckCodeCatchDlg::TranstToBMP(CString srcPath,CString destPath)
{
	WCHAR wCH[255] = {0};
	WCHAR wCH1[255] = {0};
	wcscpy(wCH,srcPath.AllocSysString());
	Image img(wCH);//�����ͼƬ������������ʽ	
	CLSID pngClsid;
	GetImageCLSID(L"image/bmp", &pngClsid);//�����ͼƬ������������ʽ���˴�ת��ΪBMP��ʽ
	wcscpy(wCH1,   destPath.AllocSysString());
	img.Save(wCH1, &pngClsid, NULL);
	return TRUE;
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
BOOL CCheckCodeCatchDlg::GetImageCLSID(const WCHAR* format, CLSID* pCLSID)
{
	UINT num = 0;
	UINT size = 0;
	ImageCodecInfo* pImageCodecInfo = NULL;
	GetImageEncodersSize(&num, &size);
	if(size == 0)
	{
		return FALSE;
	}
	pImageCodecInfo = (ImageCodecInfo *)malloc((size));
	if(pImageCodecInfo == NULL)
		return FALSE;
	GetImageEncoders(num, size, pImageCodecInfo);
	// Find for the support of format for image in the windows
	for(UINT i = 0; i < num; ++i)
	{
		//MimeType: Depiction for the program image
		if( wcscmp(pImageCodecInfo[i].MimeType, format) == 0)
		{
			*pCLSID = pImageCodecInfo[i].Clsid;
			free(pImageCodecInfo);
			return TRUE;
		}
	}
	free(pImageCodecInfo);
	return FALSE;
}
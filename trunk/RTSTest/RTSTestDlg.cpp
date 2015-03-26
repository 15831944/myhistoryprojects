// RTSTestDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "RTSTest.h"
#include "RTSTestDlg.h"
#include "_log.h"
#include <vector>
#include <winsock.h>
using namespace std;
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
using namespace extension;
#define BEGIN_MYTRY int ii=0;##try{
#define END_MYTRY(pLogger) }catch(CException *e){}catch(...){}
// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���
HANDLE gl_hMutexHandle = NULL;
class VTMesh {
public:
	//Constructor
	VTMesh(){}
	VTMesh(double x, double y, double z)
	{
		_verts[0] = x;
		_verts[1] = y;
		_verts[2] = z;
	}
	double _verts[3];
};

#define FILE_SRCPATH "c:\\test.bmp"
#define FILE_DSTPATH "c:\\test2.bmp"
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


// CRTSTestDlg �Ի���




CRTSTestDlg::CRTSTestDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CRTSTestDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CRTSTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RICHEDIT21, m_RichEdit);
	DDX_Control(pDX, IDC_DATETIMEPICKER1, m_TimePicker);
}

BEGIN_MESSAGE_MAP(CRTSTestDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, &CRTSTestDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CRTSTestDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON1, &CRTSTestDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CRTSTestDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CRTSTestDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CRTSTestDlg::OnBnClickedButton4)
	//ON_COMMAND(fdfdf_DDD, &CRTSTestDlg::OnDdd)
	ON_UPDATE_COMMAND_UI(fdfdf_DDD, &CRTSTestDlg::OnUpdateDdd2)
	ON_COMMAND(ID_222222_222, &CRTSTestDlg::On222222222)
	ON_BN_CLICKED(IDC_BUTTON5, &CRTSTestDlg::OnBnClickedButton5)
	ON_NOTIFY(NM_SETFOCUS, IDC_DATETIMEPICKER1, &CRTSTestDlg::OnNMSetfocusDatetimepicker1)
	ON_NOTIFY(NM_THEMECHANGED, IDC_DATETIMEPICKER1, &CRTSTestDlg::OnNMThemeChangedDatetimepicker1)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_DATETIMEPICKER1, &CRTSTestDlg::OnDtnDatetimechangeDatetimepicker1)
	ON_BN_CLICKED(IDC_BUTTON6, &CRTSTestDlg::OnBnClickedButton6)
	ON_BN_CLICKED(IDC_BUTTON7, &CRTSTestDlg::OnBnClickedButton7)
	ON_BN_CLICKED(IDC_BUTTON8, &CRTSTestDlg::OnBnClickedButton8)
	ON_BN_CLICKED(IDC_BUTTON9, &CRTSTestDlg::OnBnClickedButton9)
END_MESSAGE_MAP()


// CRTSTestDlg ��Ϣ�������

BOOL CRTSTestDlg::OnInitDialog()
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

	int iViewLeft = 7;
	int iViewTop = 82;
	int iViewWidth = 596;
	int iViewHeight = 422;
	if (m_wndHtmlView.CreateHtmlView(IDC_HTML_TEST, this))
	{
		m_wndHtmlView.MoveWindow(iViewLeft, iViewTop, iViewWidth, iViewHeight);			
		m_wndHtmlView.ShowWindow(SW_SHOW);
	}
	m_wndHtmlView.SetParentWnd(this);
	//m_wndHtmlView.SetCheckIllegal(true);
	//m_wndHtmlView.Navigate("http://localhost/WebSite/Default.aspx");
	char szFileName[260] = {0};
	GetModuleFileName(NULL, szFileName, MAX_PATH);
	*(strrchr(szFileName, '\\') + 1) = '\0';
	CString strHtml = "";
	strHtml.Format("%stest.html",szFileName);
	m_wndHtmlView.Navigate(strHtml);
	//WebBrowser.ExecWB(IDM_SAVETARGET,OLECMDEXECOPT_DODEFAULT,EmptyParam,EmptyParam);
	//IDM_SAVE
//	OLECMDID_SAVE m_wndHtmlView IDM_SAVETARGET
	m_RichEdit.SetHwnd(m_hWnd,IDC_BUTTON1);
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CRTSTestDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CRTSTestDlg::OnPaint()
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
HCURSOR CRTSTestDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CRTSTestDlg::OnBnClickedOk()
{
	CString strUrl = "";
	strUrl.Format("%d",_WIN32_IE);
	AfxMessageBox(strUrl);
	strUrl = m_wndHtmlView.GetLocationURL();
	if (strUrl.Find("baidu")>0)
	{
		m_wndHtmlView.Navigate("www.google.com.hk");
	}
	else
	{
		m_wndHtmlView.Navigate("www.baidu.com");
	}
}

void CRTSTestDlg::OnBnClickedCancel()
{
	BEGIN_MYTRY		
	ii = 2;
	int i=0;
	i++;
	i--;
//	int b = 2/i;
	END_MYTRY(NULL)
		CDialog::OnCancel();
}

void CRTSTestDlg::OnBnClickedButton1()
{
	CString strTestType = "";
	GetDlgItemText(IDC_EDIT1,strTestType);
}

BOOL CRTSTestDlg::GetDefaultExecuteFile(CString strFileType)
{
	
	return 0;
}

void CRTSTestDlg::OnBnClickedButton2()
{
	CanRunThisClientLite();
}



void CRTSTestDlg::OnBnClickedButton4()
{
	event_log log("aa.txt");
	string strTest;
	strTest.append(11000,'A');
	log.log_info("%s",strTest.c_str());
}

void CRTSTestDlg::OnDdd()
{
	AfxMessageBox("���");
}

void CRTSTestDlg::OnUpdateDdd2(CCmdUI *pCmdUI)
{
	AfxMessageBox("����");
	pCmdUI->Enable(FALSE);
	switch(pCmdUI->m_nID)
	{
	case fdfdf_DDD:
		pCmdUI->Enable(FALSE);
		break;
	
	default:
		break;
	}
}

void InitList(int **pList,const int nLen)
{
	*pList = new int(nLen);
	for (int i=0;i<nLen;i++)
	{
		(*pList)[i] = i+1;
	}
}

int *ExChange(int *pList,const int nLen,const int nExChangeCount)
{
	return pList;
}
typedef enum RemotingMode : int
{
	RemoteNone = 0,
	VNCViewerReadOnly = 1,
	VNCViewerControl = 2,
	VNCServer = 4, 
	FileTransfer = 8, 
	WebChat = 16, 
	IPVoice = 32, 
	IPVideo = 64,
	VNCInitialize = 128
}RemotingMode;
RemotingMode GetMode()
{
	int mode = VNCViewerReadOnly | VNCViewerControl;
	return (RemotingMode)mode;
}
void CRTSTestDlg::OnBnClickedButton3()
{
	int iii = GetMode();
}

void CRTSTestDlg::On222222222()
{
	HBITMAP hMap;
	CBitmap bitMap;
	//bitMap.SetBitmapBits()
	
}
vector<VTMesh*> _vertices;
void CRTSTestDlg::OnBnClickedButton5()
{
	m_wndHtmlView.ExecWB(OLECMDID_SAVEAS, OLECMDEXECOPT_DONTPROMPTUSER, NULL, NULL);
	return;
	VTMesh *v = &VTMesh(1, 2, 3);
	_vertices.push_back(v);
	_vertices.push_back(v);

	/*
	CString strText = "�����?";
	TextOut(GetDC()->GetSafeHdc(),10,10,strText,strText.GetLength());
	CBitmap bitMap;
	bitMap.LoadBitmap(FILE_SRCPATH);
	BITMAP bmpInfo; 
	bitMap.GetBitmap(&bmpInfo); 
	void *lpBits = 0; 
	int biSizeImage = bmpInfo.bmWidthBytes * bmpInfo.bmHeight; //����λͼ������Ҫ�����ֽ� 
	lpBits = malloc(biSizeImage); 
	bitMap.GetBitmapBits(biSizeImage, lpBits);
	BITMAPINFO   BitmapInfo; 
	BitmapInfo.bmiHeader.biSize=sizeof(BITMAPINFOHEADER);  
	BitmapInfo.bmiHeader.biWidth=bmpInfo.bmWidth;    
	BitmapInfo.bmiHeader.biHeight=bmpInfo.bmHeight;    
	BitmapInfo.bmiHeader.biPlanes=1;    
	BitmapInfo.bmiHeader.biBitCount=32;//������λͼ����ɫλ����������λͼ�������ɫ�ģ�Ҳ����С�ڵ���8λ���Ǿ��鷳�ˣ��㻹Ҫ���ʵ��BitmapInfo.bmiColors�ṹ�����Խ��齫λͼ��Ƴ����ɫ�ģ���Ҫ��ϧ�ǵ�ռ䣡 
	BitmapInfo.bmiHeader.biCompression=BI_RGB;    
	BitmapInfo.bmiHeader.biClrUsed=0;   
	BitmapInfo.bmiHeader.biSizeImage=0;    
	BitmapInfo.bmiHeader.biXPelsPerMeter=0;    
	BitmapInfo.bmiHeader.biYPelsPerMeter=0;    
	BitmapInfo.bmiHeader.biClrImportant=0;  
	
//	StretchDIBits(*pDC, 700, 700, 3000, 3000, 0, 0, bmpInfo.bmWidth, bmpInfo.bmHeight, lpBits, &BitmapInfo, DIB_RGB_COLORS, SRCCOPY); 

	free(lpBits);


	LPBYTE pBMPFile = NULL;
	long lLen = 0;
	ReadBMPFile(FILE_SRCPATH,pBMPFile,lLen);
	CRect rct;
	GetClientRect(&rct);
	ShowBmpFile(GetDC()->GetSafeHdc(),10,10,pBMPFile,rct,lLen);
	*/
}

void CRTSTestDlg::OnNMSetfocusDatetimepicker1(NMHDR *pNMHDR, LRESULT *pResult)
{
	*pResult = 0;
}

void CRTSTestDlg::OnNMThemeChangedDatetimepicker1(NMHDR *pNMHDR, LRESULT *pResult)
{
	*pResult = 0;
}

void CRTSTestDlg::OnDtnDatetimechangeDatetimepicker1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMDATETIMECHANGE pDTChange = reinterpret_cast<LPNMDATETIMECHANGE>(pNMHDR);
	
	*pResult = 0;
}

BOOL CRTSTestDlg::ShowBmpFile(HDC hdc, int nXDest, int nYDest,LPBYTE lpDIB,CRect rect,long lLen)
{
	if( lpDIB == NULL)
	{
		return FALSE;
	}
	LPBITMAPINFOHEADER lpBMPInfoHeader=(LPBITMAPINFOHEADER) (lpDIB + sizeof(BITMAPFILEHEADER));
	BITMAPINFO * lpBMPInfo =(BITMAPINFO *)(lpDIB + sizeof(BITMAPFILEHEADER));
	int iWidth =0;
	int iHeight = 0;
	if ( lpBMPInfoHeader->biWidth > rect.Width())
	{
		iWidth = rect.Width();
	}
	else
	{
		iWidth = lpBMPInfoHeader->biWidth;
	}
	if ( lpBMPInfoHeader->biHeight > rect.Height())
	{
		iHeight = rect.Height();
	}
	else
	{
		iHeight = lpBMPInfoHeader->biHeight;
	}
	SetDIBitsToDevice(hdc,nXDest,nYDest,iWidth,iHeight,
		0,0,0,lpBMPInfoHeader->biHeight,lpDIB+((BITMAPFILEHEADER *)lpDIB)->bfOffBits,lpBMPInfo,DIB_RGB_COLORS);
	return TRUE;
}

bool CRTSTestDlg::ReadBMPFile(LPCTSTR strFileName,LPBYTE &lpBMPFileData,long& fileLength)
{
	CFile BitmapFile;
	BOOL blOpen=BitmapFile.Open(strFileName, CFile::modeRead,NULL);
	if( !blOpen )
	{
		//CString str = "���ļ�ʧ�ܣ�";
		lpBMPFileData = NULL;
		return false;
	}
	unsigned long FileLength=BitmapFile.GetLength();
	fileLength = FileLength;
	if (lpBMPFileData != NULL)
	{
		delete[] lpBMPFileData;
		lpBMPFileData = NULL;
	}
	lpBMPFileData = new BYTE[FileLength];
	ASSERT( lpBMPFileData!=NULL );
	BitmapFile.Read(lpBMPFileData,FileLength);

	if ( ((LPBITMAPFILEHEADER)lpBMPFileData)->bfType != 0x4D42 )
	{
		//CString str = "��֧�ֳ�BMP֮����ļ���";
		delete[] lpBMPFileData;
		lpBMPFileData = NULL;
		return false;;
	}
	return true;
}

void CRTSTestDlg::Show(CRTSTestDlg *pThis, int nShowTime, BOOL bSound, BOOL bHideAfterSel, LPCTSTR strWav)
{
	try
	{
		//if (m_pLogger) m_pLogger->log_info("Pop up msn: %s", this->m_strMsg);
		AttachThreadInput(GetWindowThreadProcessId(::GetForegroundWindow(),NULL), GetCurrentThreadId(), FALSE);

		this->EnableWindow(TRUE);
		//InitData();
		//InitRichEdit();
	//	m_bHideAfterSel = bHideAfterSel;

		if(bSound)
		{
		//	PlaySound(strWav, NULL, SND_FILENAME | SND_ASYNC);
		}
		//m_nShowTime = nShowTime;
	//	m_bShowLeftTime = nShowTime != 0;
	//	if(!MsnManager.HaveWnd(this))
		//	MsnManager.Add(pThis);
		int m_nScreenX = ::GetSystemMetrics(SM_CXFULLSCREEN);
		int m_nScreenY = ::GetSystemMetrics(SM_CYFULLSCREEN);
		HDC hDC = ::GetDC(::GetDesktopWindow());
		if(hDC != 0)
		{
			m_nScreenX = ::GetDeviceCaps(hDC, HORZRES);///�õ����
			m_nScreenY = ::GetDeviceCaps(hDC, VERTRES);///�õ��߶�
		}
		CRect m_rect;
		CRect rect(0, m_nScreenY, 212, 127 + m_nScreenY);
		//MoveWindow(rect);
		GetClientRect(&m_rect);
		//int m_nY = m_nScreenY - m_nTaskHight;
		int m_nX = m_nScreenX - m_rect.Width();
		//int m_nTranValue = TRANSVALUE;
		int m_hActiveWnd = NULL;
		//if(GetActiveWindow() == GetParent())
		//	m_hActiveWnd = GetParent()->m_hWnd;

		//this->OnPaint();
		CDC *pDC = this->GetDC();
		if(pDC)
		//	this->ShowLeftTime(pDC);

		//m_REMsgCtrl.SetSel(0, 0);
		//m_REMsgCtrl.HideCaret();

		//this->SetParent(this->GetDesktopWindow());
		//SetTimer(SHOW_ID, SHOW_HIDE_SPEED, NULL);
		//m_bClose = FALSE;
		//m_bTimeOut = FALSE;
		::ReleaseDC(::GetDesktopWindow(), hDC);
		this->ReleaseDC(pDC);
	}
	catch(...)
	{
		AfxMessageBox("CMsgDlg::Show unkown error.");
	}
}
void CRTSTestDlg::OnBnClickedButton6() 
{

	DWORD m_dwMajorVersion;
	DWORD m_dwMinorVersion;
	DWORD m_dwBuildNumber;
	DWORD m_dwPlatformId;

	OSVERSIONINFO osvi;//����OSVERSIONINFO���ݽṹ����
	memset(&osvi, 0, sizeof(OSVERSIONINFO));//���ռ� 
	osvi.dwOSVersionInfoSize = sizeof (OSVERSIONINFO);//�����С 
	GetVersionEx (&osvi);//��ð汾��Ϣ 
	m_dwMajorVersion=osvi.dwMajorVersion;//���汾��
	m_dwMinorVersion=osvi.dwMinorVersion;//���汾
	m_dwBuildNumber=osvi.dwBuildNumber;//������
	m_dwPlatformId=osvi.dwPlatformId;//ID��
	UpdateData(FALSE); 

	CString strShow = "";
	strShow.Format("%d.%d.%d.%d",m_dwMajorVersion,m_dwMinorVersion,m_dwBuildNumber,m_dwPlatformId);
	AfxMessageBox(strShow);

	return;
	HWND hwnd = ::FindWindow("TXGuiFoundation","������ϵ��/Ⱥ/��ҵ");
	if(hwnd)
	{
		hwnd = GetChildWindow(hwnd,2);
		if (hwnd)
		{
			int ii = 0;
			//::PostMessage(hwnd,WM_PASTE,0,0);
			::SendMessage(hwnd, WM_SETTEXT, 0, (LPARAM)_T("Hello,World"));
		}
	}

}

/************************************************************************/
HWND CRTSTestDlg::GetChildWindow(HWND hwnd,int iIndex)
{
	HWND hwndChind = NULL;
	CString strTemp = "";
	int i = 1;
	hwndChind = ::GetWindow(hwnd,GW_CHILD);
	CWnd *pWnd = CWnd::FromHandle(hwndChind);
	while (pWnd)
	{	
		hwndChind = pWnd->GetSafeHwnd();
		if (i == iIndex)
		{
			return hwndChind;
		}
		pWnd = pWnd->GetNextWindow(GW_HWNDNEXT);
		i++;
	}
	return hwndChind;
}

CString CRTSTestDlg::GetLocHostIp()
{
	WSAData wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) 
	{
		return _T("");
	}
	char szHostName[128];
	char szIp[MAX_PATH]= {0};
	CString strIpS = "";
	if(gethostname(szHostName, 128) == 0)
	{    
		struct   hostent   *   pHost;   
		int   i;   
		pHost = gethostbyname(szHostName);   

		for(i = 0; pHost != NULL && pHost->h_addr_list[i]!=NULL; i++)   
		{   
			char   strHostIp[25];  
			memset(strHostIp, 0, 25); 
			int   j;   

			for(j = 0; j < pHost->h_length; j++)   
			{   
				char  addr[10];   
				if(j > 0)   
					lstrcat(strHostIp, ".");   
				sprintf_s(addr, 10, "%u",(unsigned   int)((unsigned char*)pHost->h_addr_list[i])[j]);   
				lstrcat(strHostIp, addr); 
			}
			lstrcpy(szIp, strHostIp);
			strIpS += CString(szIp) + "\n";
		}   
	}
	WSACleanup();
	return strIpS;
}

BOOL CRTSTestDlg::CanRunThisClientLite()
{
	try
	{
		char szEuCharacterId[MAX_PATH] = {0};

		//strMutexName = strMutexName + MutexName;
		CString strMutexName = "aaaabfdsafdasfdasfdaf";
		strMutexName.Replace("\\", "|");

		HANDLE hMutex = CreateMutex(NULL, FALSE, strMutexName);
		DWORD dwError = GetLastError(); 
		if (hMutex != NULL)
		{
			if (dwError == ERROR_ALREADY_EXISTS)
			{
				AfxMessageBox("ERROR_ALREADY_EXISTS");
				ReleaseMutex(hMutex);
				CloseHandle(hMutex);
				hMutex = NULL;				
				return FALSE;
			}

			if (false)
			{
				ReleaseMutex(hMutex);
				CloseHandle(hMutex);
			}
			else
			{
				gl_hMutexHandle = hMutex;
			}
		}
		else
		{
			return FALSE;
		}
		return TRUE;
	}
	catch(...)
	{

	}
	return FALSE;
}
void CRTSTestDlg::OnBnClickedButton7()
{
	AfxMessageBox(GetLocHostIp());
	return;
	WSAData wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) 
	{
		return ;
	}


	struct hostent* pHostInfo = NULL;


	char szIP[] = "192.168.1.103";
	pHostInfo = gethostbyname(szIP);//�����������ֺ͵�ַ��Ϣ��hostent�ṹָ��
	if (pHostInfo == NULL)
	{
		pHostInfo = gethostbyaddr(szIP, strlen(szIP), AF_INET);

	}	


	SOCKET sockListen = socket(AF_INET, SOCK_STREAM, 0);  	
	SOCKADDR_IN addr;
	::memset( &addr, 0, sizeof( addr ) );
	addr.sin_addr.S_un.S_addr=htonl(INADDR_ANY);
	//memcpy(&addr.sin_addr, pHostInfo->h_addr_list[0], pHostInfo->h_length);
	addr.sin_family=AF_INET;
	addr.sin_port=htons(6000);
	bind(sockListen,(SOCKADDR*)&addr,sizeof(SOCKADDR));
	listen(sockListen,5);

	SOCKADDR_IN addrEx;
	int dLen = sizeof(addrEx);
	int iRet = getsockname(sockListen,(sockaddr *)&addrEx,&dLen);
	DWORD dRet = GetLastError();
	int iiPort = ntohs(addrEx.sin_port);

	long lAddress = ntohl(addrEx.sin_addr.S_un.S_addr);
	CString strIP = inet_ntoa(addrEx.sin_addr);
	AfxMessageBox(strIP);
	AfxMessageBox(GetLocHostIp());
	//()
	WSACleanup();
}

void CRTSTestDlg::OnBnClickedButton8()
{
	if (gl_hMutexHandle == NULL)
	{
		return;
	}
	ReleaseMutex(gl_hMutexHandle);
	CloseHandle(gl_hMutexHandle);
	gl_hMutexHandle = NULL;
}
void CRTSTestDlg::OnBnClickedButton9()
{
	ClearTrayIcon();
	return;
	HWND hwnd = ::FindWindow("Shell_TrayWnd",NULL);
	if (!hwnd)
	{
		hwnd = ::FindWindow("ToolbarWindow32",NULL);
	}
	if (!hwnd)
	{
		return;
	}

	/*
	Pos.x = wX;
	Pos.y = wY;
	::ClientToScreen(hVSListWnd, &Pos);
	::SetCursorPos(Pos.x, Pos.y);
	*/
	char szTitle[250] = {0};
	::GetWindowText(hwnd,szTitle,250);
	AfxMessageBox(szTitle);
	CRect rct;
	::GetWindowRect(hwnd,&rct);
	::PostMessage(hwnd,WM_PAINT,0,0);
	::ShowWindow(hwnd,SW_HIDE);
	::ShowWindow(hwnd,SW_SHOW);
}
#include <iostream>
using namespace std;
int CRTSTestDlg::ClearTrayIcon()
{
	HWND     hStatus=::FindWindow("Shell_TrayWnd",NULL);     //�õ����������  
	if     (hStatus==NULL)      
	{      
		cout<<"Get   Shell_TrayWnd   error!\n";      
		return   -1;      
	}      
	HWND     hNotify=::FindWindowEx(hStatus,NULL,"TrayNotifyWnd",NULL);   //���½�����    
	if     (hNotify==NULL)      
	{      
		cout<<"Get   TrayNotifyWnd   error!\n";      
		return   -1;      
	}      
	HWND     hNotify1=::FindWindowEx(hNotify,NULL,"SysPager",NULL);  
	if     (hNotify==NULL)      
	{      
		cout<<"Get   SysPager   error!\n";      
		return   -1;      
	}    
	HWND     hNotify1_0=::FindWindowEx(hNotify1,NULL,"ToolBarWindow32",NULL);//���½�����(������ʱ��)  
	if     (hNotify1_0==NULL)        
	{      
		cout<<"Get   ToolBarWindow32   error!\n";      
		return   -1;      
	}      
	//-------------------�����ǵõ����������½�һ��ط��ľ��  
	DWORD     pid   =   0;      
	GetWindowThreadProcessId(hNotify1_0,&pid);      
	if     (pid==NULL)      
	{      
		cout<<"Get   pid   error!\n";      
		return   -1;      
	}        

	HANDLE     hProcess=OpenProcess(PROCESS_QUERY_INFORMATION|PROCESS_ALL_ACCESS,true,pid);      
	if     (hProcess==NULL)      
	{      
		cout<<"Get   hd   error!\n";      
		return   -1;      
	}      
	::SendMessage(hNotify1_0,WM_PAINT   ,NULL,NULL);    
	CRect   rect;  
	::GetWindowRect(hNotify1_0,&rect);  
	::InvalidateRect(hNotify1_0,&rect,false);  
	int     iNum=::SendMessage(hNotify1_0,TB_BUTTONCOUNT   ,NULL,NULL);     //��ȡ��������ͼ�����  

	unsigned   long   n   =   0;      
	TBBUTTON     *pButton   =   new   TBBUTTON;      
	CString     strInfo   =   _T("");      
	wchar_t     name[256]   =   {0};      
	TBBUTTON     BButton;    
	unsigned       long         whd,proid;  
	CString   x;  

	for(int   i=0;   i<iNum;   i++)      
	{      
		::SendMessage(hNotify1_0,TB_GETBUTTON,i,(LPARAM)(&BButton));      
		ReadProcessMemory(hProcess,&BButton,pButton,sizeof(TBBUTTON),&n);        
		if     (pButton->iString   !=   0xffffffff)      
		{      
			try      
			{      
				ReadProcessMemory(hProcess,(void   *)pButton->iString,name,255,&n);  
			}      
			catch(...)      
			{      
			}              
			strInfo.Format("%d   :   %s\n",i+1,CString(name));
			cout<<strInfo<<endl;  
		}  

		try        
		{        
			whd=0;        
			ReadProcessMemory(hProcess,(void       *)pButton->dwData,&whd,4,&n);        
		}        
		catch(...)        
		{        
		}        
		proid=NULL;        
		GetWindowThreadProcessId((HWND)whd,&proid);        
		if(proid==NULL)  
			::SendMessage(hNotify1_0,TB_DELETEBUTTON,i,0);  
	}    
	delete   pButton;  
	return   0;
}
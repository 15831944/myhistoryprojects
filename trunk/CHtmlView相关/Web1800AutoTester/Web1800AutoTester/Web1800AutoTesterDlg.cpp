// Web1800AutoTesterDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Web1800AutoTester.h"
#include "Web1800AutoTesterDlg.h"
#include "WindowFind.h"
#include <io.h>
#include <direct.h>
#include "FileUtil.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���
bool g_ExitFlagClient = false;
bool g_ExitFlagServer = false;
bool g_bClientRequest = false; //��ʾ�ͻ��˵ĻỰ�����Ѿ���
CString g_strSpName = "";
CWindowFind g_find;
HWND g_hwndSpWindow = NULL;
HWND g_hwndSessionWindow = NULL;	//�Ự���ھ��

HWND g_hwndWarn = NULL;
int g_iInterval = 4000;


BOOL CALLBACK EnumChildProcWarn(HWND   hwnd, LPARAM lParam)
{   
	if(g_find.GetContrlClassName(hwnd)=="Static") 
	{   
		if(g_find.GetContrlWindowText(hwnd)=="�ͻ��Ѿ��˳���") 
		{
			g_hwndWarn  = hwnd;
			return FALSE;
		}
	}
	EnumChildWindows(hwnd,EnumChildProcWarn,lParam);
	return   TRUE;   
}

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


// CWeb1800AutoTesterDlg �Ի���




CWeb1800AutoTesterDlg::CWeb1800AutoTesterDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CWeb1800AutoTesterDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CWeb1800AutoTesterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_message);
	DDX_Control(pDX, IDC_LIST_MSGSP, m_spMessage);
}

BEGIN_MESSAGE_MAP(CWeb1800AutoTesterDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BTN_STARTTIMER, &CWeb1800AutoTesterDlg::OnBnClickedBtnStarttimer)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTN_STARTTEST, &CWeb1800AutoTesterDlg::OnBnClickedBtnStarttest)
	ON_BN_CLICKED(IDC_BUTTON1, &CWeb1800AutoTesterDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CWeb1800AutoTesterDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CWeb1800AutoTesterDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CWeb1800AutoTesterDlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BTN_STARTSP, &CWeb1800AutoTesterDlg::OnBnClickedBtnStartsp)
	ON_BN_CLICKED(IDC_BTN_STOPCLIENT, &CWeb1800AutoTesterDlg::OnBnClickedBtnStopclient)
	ON_BN_CLICKED(IDC_BTN_STOPSP, &CWeb1800AutoTesterDlg::OnBnClickedBtnStopsp)
END_MESSAGE_MAP()


// CWeb1800AutoTesterDlg ��Ϣ�������

BOOL CWeb1800AutoTesterDlg::OnInitDialog()
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

	SetDlgItemText(IDC_EDIT_STARFFID,"02f9cf57-31fe-44b1-ba69-a134ed37b959");
	SetDlgItemText(IDC_EDIT_URL,"http://1800cisvr30/8000000");
	CoInitialize(NULL);
	InitListCtrl();
	GetDlgItem(IDC_BTN_STOPSP)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_STOPCLIENT)->EnableWindow(FALSE);
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CWeb1800AutoTesterDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CWeb1800AutoTesterDlg::OnPaint()
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
HCURSOR CWeb1800AutoTesterDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
BOOL CALLBACK EnumChildProcExExEx(HWND hwnd, LPARAM lParam)
{   
	char strClassName[255];
	char strWindowName[255];
	memset(strClassName,0,sizeof(strClassName));   
	memset(strWindowName,0,sizeof(strWindowName));  
	if(g_find.GetContrlWindowText(hwnd) == "���Ͽͷ����� Ver 3.0")   
	{   
		if (g_find.GetContrlClassName(hwnd) == "#32770")
		{
			HWND hwndTemp = g_find.GetChildWindow(hwnd,3);
			if (g_find.GetContrlWindowText(hwndTemp) == g_strSpName)
			{
				g_hwndSpWindow  = hwnd;
				return FALSE;
			}			
		}
		else
		{
			EnumChildWindows(hwnd,EnumChildProcExExEx,lParam); 
			return TRUE;
		}
	}
	EnumChildWindows(hwnd,EnumChildProcExExEx,lParam);
	return TRUE;
}
BOOL CWeb1800AutoTesterDlg::PreTranslateMessage(MSG* pMsg)
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

void CWeb1800AutoTesterDlg::OnBnClickedBtnStarttimer()
{
	GetDlgItemText(IDC_EDIT_SPNAME,g_strSpName);
	g_hwndSpWindow = NULL;
	g_iInterval = GetDlgItemInt(IDC_EDIT_INTERVAL);
	EnumChildWindows(NULL,EnumChildProcExExEx,(LPARAM)NULL);
	if (g_hwndSpWindow == NULL)
	{
		AfxMessageBox("����δ����");
		return;
	}
	SetTimer(1,g_iInterval,NULL);
	GetDlgItem(IDC_BTN_STARTTIMER)->EnableWindow(FALSE);
}

void CWeb1800AutoTesterDlg::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == 1)
	{
		SetTimer(2,g_iInterval,NULL);
		KillTimer(1);
		if (!::IsWindow(g_hwndSpWindow))
		{
			AfxMessageBox("Ŀ��������˳�");
			return;
		}
		HWND hwnd = g_find.GetChildWindow(g_hwndSpWindow,18);
		g_find.PostBtnClick(hwnd);
	}
	else if (nIDEvent == 2)
	{
		SetTimer(1,g_iInterval,NULL);
		KillTimer(2);
		HWND hwnd = ::FindWindow("#32770","  ��ά���� ���Ͽͷ����� ");
		if (hwnd)
		{
			::PostMessage(hwnd,WM_CLOSE,0,0);
		}
	}
	else if (nIDEvent == 3)
	{
		HWND hwnd = ::FindWindow("#32770","Microsoft Internet Explorer");
		if (hwnd)
		{
			HWND hwndTemp = g_find.GetChildWindow(hwnd,4);
			CString strText = g_find.GetContrlWindowText(hwndTemp);
			if (strText.Find("�رմ��ڽ��˳���������")>=0)
			{
				hwndTemp = g_find.GetChildWindow(hwnd,1);
				g_find.PostBtnClick(hwndTemp);
			}
			else
			{
				::PostMessage(hwnd,WM_CLOSE,0,0);
			}
		}
		hwnd = ::FindWindow("Internet Explorer_TridentDlgFrame","Internet Explorer �ű�����");
		if (hwnd)
		{
			::PostMessage(hwnd,WM_CLOSE,0,0);
		}
		hwnd = ::FindWindow("Internet Explorer_TridentDlgFrame","ϵͳ���� -- ��ҳ�Ի���");
		if (hwnd)
		{
			::PostMessage(hwnd,WM_CLOSE,0,0);
		}
	}
	CDialog::OnTimer(nIDEvent);
}

void CWeb1800AutoTesterDlg::InitListCtrl()
{
	DWORD dwSytle=::GetWindowLong(m_message.m_hWnd,GWL_STYLE);
	// ����Ϊ������ʽ
	SetWindowLong(m_message.m_hWnd,GWL_STYLE,dwSytle|LVS_REPORT);
	DWORD ExStyle=m_message.GetExtendedStyle();
	m_message.SetExtendedStyle(ExStyle|LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
	::SendMessage(m_message.m_hWnd, LVM_SETEXTENDEDLISTVIEWSTYLE,
		LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);
	m_message.MoveWindow(7,250,370,270);
	m_message.InsertColumn(0,_T("ʱ��"),LVCFMT_LEFT,90);
	m_message.InsertColumn(1,_T("��Ϣ"),LVCFMT_LEFT,250);

	dwSytle=::GetWindowLong(m_spMessage.m_hWnd,GWL_STYLE);
	// ����Ϊ������ʽ
	SetWindowLong(m_spMessage.m_hWnd,GWL_STYLE,dwSytle|LVS_REPORT);
	ExStyle=m_spMessage.GetExtendedStyle();
	m_spMessage.SetExtendedStyle(ExStyle|LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
	::SendMessage(m_spMessage.m_hWnd, LVM_SETEXTENDEDLISTVIEWSTYLE,
		LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);
	m_spMessage.MoveWindow(377,250,382,270);
	m_spMessage.InsertColumn(0,_T("ʱ��"),LVCFMT_LEFT,90);
	m_spMessage.InsertColumn(1,_T("��Ϣ"),LVCFMT_LEFT,250);


}

void CWeb1800AutoTesterDlg::OnBnClickedBtnStarttest()
{
	SetTimer(3,400,NULL);
	GetDlgItem(IDC_BTN_STARTTEST)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_STOPCLIENT)->EnableWindow(TRUE);
	AfxBeginThread((AFX_THREADPROC)ThreadFunc,(LPVOID)this);
}

void CWeb1800AutoTesterDlg::OnBnClickedBtnStartsp()
{
	GetDlgItem(IDC_BTN_STARTSP)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_STOPSP)->EnableWindow(TRUE);
	AfxBeginThread((AFX_THREADPROC)ThreadFuncSp,(LPVOID)this);
}

int CWeb1800AutoTesterDlg::AddToList(CString msgBuf)
{
	if ( msgBuf == "" )
	{
		return 0;
	}
	int nCount = m_message.GetItemCount();
	if ( nCount >= 50 )
	{
		m_message.DeleteItem( 0 );
		m_message.DeleteItem( 0 );
		m_message.DeleteItem( 0 );
		nCount -= 3;
	}
	nCount = m_message.GetItemCount();
	CString strShow = m_message.GetItemText(nCount-1,1);
	if (msgBuf == strShow)
	{
		m_message.SetItemText( nCount-1 , 0, CTime::GetCurrentTime().Format("%H:%M:%S") );
		::PostMessage(m_message.m_hWnd,WM_VSCROLL,SB_BOTTOM,0);
		return 0;
	}
	m_message.InsertItem( nCount, "");	
	m_message.SetItemText( nCount , 0, CTime::GetCurrentTime().Format("%H:%M:%S") );
	m_message.SetItemText( nCount , 1, msgBuf );
	::PostMessage(m_message.m_hWnd,WM_VSCROLL,SB_BOTTOM,0);
	WriteLog(msgBuf,1);
	return 0;
}


int CWeb1800AutoTesterDlg::AddToListSp(CString msgBuf)
{
	if ( msgBuf == "" )
	{
		return 0;
	}
	int nCount = m_spMessage.GetItemCount();
	if ( nCount >= 50 )
	{
		m_spMessage.DeleteItem( 0 );
		m_spMessage.DeleteItem( 0 );
		m_spMessage.DeleteItem( 0 );
		nCount -= 3;
	}
	nCount = m_spMessage.GetItemCount();
	CString strShow = m_spMessage.GetItemText(nCount-1,1);
	if (msgBuf == strShow)
	{
		m_spMessage.SetItemText( nCount-1 , 0, CTime::GetCurrentTime().Format("%H:%M:%S") );
		::PostMessage(m_spMessage.m_hWnd,WM_VSCROLL,SB_BOTTOM,0);
		return 0;
	}
	m_spMessage.InsertItem( nCount, "");	
	m_spMessage.SetItemText( nCount , 0, CTime::GetCurrentTime().Format("%H:%M:%S") );
	m_spMessage.SetItemText( nCount , 1, msgBuf );
	::PostMessage(m_spMessage.m_hWnd,WM_VSCROLL,SB_BOTTOM,0);
	WriteLog(msgBuf,2);
	return 0;
}

void CWeb1800AutoTesterDlg::WriteLog(CString srcStr,int iFlag)
{
	CTime currTime = CTime::GetCurrentTime();
	try
	{
		CString filepath = "";
		CString destStr = "";
		CString folder = "";
		CString filename = "";
		CString strAppPath = "";
		if (iFlag == 0)			//ע��ɹ��б�
		{
			folder = "log";
			filename.Format("%s_freshList.txt",currTime.Format("%Y%m%d"));
		}
		else if(iFlag == 1)		//��Ϣ��־
		{
			folder = "log";
			filename.Format("%s_log.txt",currTime.Format("%Y%m%d"));
		}
		else if ( iFlag == 2 )	//������־
		{
			folder = "logSp";
			filename.Format("%s_log.txt",currTime.Format("%Y%m%d"));
		}
		else			//�쳣��־ (3)
		{
			folder = "exception";
			filename.Format("%s_exception.txt",currTime.Format("%Y%m%d"));
		}

		GetAppPath(strAppPath);
		CString folderName = "";
		folderName.Format("%s%s",strAppPath,folder);
		if(_access(folderName,0)!=0)
		{
			if( mkdir(folderName) != 0 )//����Ŀ¼
			{
				return;
			}
		}					
		destStr.Format("%s	%s",currTime.Format("%Y-%m-%d %H:%M:%S"),srcStr);
		filepath.Format("%s\\%s",folderName,filename);
		FILE *fp;
		char szfile[1000]={0};
		sprintf(szfile,filepath);
		fp=fopen(szfile,"a+");
		fprintf(fp,"%s\r\n",destStr);
		fclose(fp);
	}
	catch (...)
	{
	}
}


int CWeb1800AutoTesterDlg::GetAppPath(CString &strAppPath)
{
	int iRet = -3;
	try
	{
		TCHAR szBuff[_MAX_PATH];
		VERIFY(::GetModuleFileName(AfxGetApp()->m_hInstance, szBuff, _MAX_PATH));
		strAppPath.Format("%s",szBuff);
		int pos=strAppPath.ReverseFind('\\');
		strAppPath=strAppPath.Left(pos+1);		
		iRet = 0;
	}
	catch (...)
	{
		
	}
	return iRet;	
}
IHTMLDocument2* CWeb1800AutoTesterDlg::GetDocInterface(HWND hWnd) 
{
	// ������Ҫ��ʾ��װ��OLEACC.DLL,�������ǲ�֪����û�а�װMSAA
	
	HINSTANCE hInst = ::LoadLibrary( _T("OLEACC.DLL") );
	IHTMLDocument2* pDoc2=NULL;
	if ( hInst != NULL ){
		if ( hWnd != NULL ){
			CComPtr<IHTMLDocument> spDoc=NULL;
			LRESULT lRes;

			UINT nMsg = ::RegisterWindowMessage( _T("WM_HTML_GETOBJECT") );
			::SendMessageTimeout( hWnd, nMsg, 0L, 0L, SMTO_ABORTIFHUNG, 1000, (DWORD*)&lRes );

			LPFNOBJECTFROMLRESULT pfObjectFromLresult = (LPFNOBJECTFROMLRESULT)::GetProcAddress( hInst, ("ObjectFromLresult") );
			if ( pfObjectFromLresult != NULL ){
				HRESULT hr;
				hr=pfObjectFromLresult(lRes,IID_IHTMLDocument,0,(void**)&spDoc);
				if ( SUCCEEDED(hr) ){
					CComPtr<IDispatch> spDisp;
					CComQIPtr<IHTMLWindow2> spWin;
					spDoc->get_Script( &spDisp );
					spWin = spDisp;
					spWin->get_document( &pDoc2 );
				}
			}
		}
		::FreeLibrary(hInst);
	} 
	else{//���û�а�װMSAA
		AfxMessageBox(_T("������װMicrosoft Active Accessibility"));
	}
	//CoUninitialize();
	return pDoc2;
}


bool CWeb1800AutoTesterDlg::SendRequest()
{
	CString strStaffId = "";
	AddToList("���ڲ���֧�����Ĵ���");
	GetDlgItemText(IDC_EDIT_STARFFID,strStaffId);
	HWND hwnd = GetExplorerHWND();
	if (!hwnd)
	{
		AddToList("��ȡ���ʧ��");
		return false;
	}
	IHTMLDocument2* pDoc = GetDocInterface(hwnd);
	if (!pDoc)
	{
		AddToList("��ȡHTML����ʧ��");
		return false;
	}
	strStaffId = "St_" + strStaffId;
	IHTMLElement *pElement = NULL;
	GetElement(strStaffId,&pElement,pDoc);
	if (pElement == NULL)
	{
		AddToList("֧����Ա��æ�����ߣ����Ժ�������");
		pDoc->Release();
		return false;
	}
	IHTMLElement *pElementParent = NULL;
	pElement->get_parentElement(&pElementParent);
	if (!pElementParent)
	{
		AddToList("�����������ʧ��");
		pElement->Release();
		pDoc->Release();
		return false;
	}
	BSTR bstrStr;
	pElementParent->get_tagName(&bstrStr);
	CString str(bstrStr);
	::SysFreeString(bstrStr);
	str.MakeLower();
	if (str == "a")
	{
		pElementParent->click();
		pElementParent->Release();
		pElement->Release();
		pDoc->Release();
		AddToList("�ɹ����ͻỰ����");
		return true;
	}
	else
	{
		pElementParent->click();
		pElementParent->Release();
		pElement->Release();
		pDoc->Release();
		AddToList("֧����Ա��æ�����ߣ����Ժ�������");
		return false;
	}
}

void CWeb1800AutoTesterDlg::OnBnClickedButton1()
{
	SendRequest();
	return;
	CString strStaffId = "";
	GetDlgItemText(IDC_EDIT_STARFFID,strStaffId);	
	HWND hwnd = GetExplorerHWND();
	if (!hwnd)
	{
		AfxMessageBox("��ȡ���ʧ��");
		return;
	}
	IHTMLDocument2* pDoc = GetDocInterface(hwnd);
	if (!pDoc)
	{
		//pDoc->Get
	}
	strStaffId = "St_" + strStaffId;
/*
	CString strJsString = "";
	strJsString.Format("var tTable=document.getElementById('%s'); alert(tTable.childNodes.length);",strStaffId);
	strJsString = "alert('aaaaaaa');";
	IHTMLWindow2*   pWindow;
	pDoc->get_parentWindow(&pWindow);   
	VARIANT  ret;   
	BSTR bstrStrEx = strJsString.AllocSysString();
	pWindow->execScript(bstrStrEx,L"javascript",&ret);
	::SysFreeString(bstrStrEx);
	pWindow->Release();*/

	//return;
	IHTMLElement *pElement = NULL;
	GetElement(strStaffId,&pElement,pDoc);
	if (pElement == NULL)
	{
		return;
	}
	IHTMLElement *pElementParent = NULL;
	pElement->get_parentElement(&pElementParent);
	if (!pElementParent)
	{
		pElement->Release();
		return;
	}
	BSTR bstrStr;
	pElementParent->get_tagName(&bstrStr);
	CString str(bstrStr);
	::SysFreeString(bstrStr);
	str.MakeLower();
	if (str == "a")
	{
		pElementParent->click();
		pElementParent->Release();
		pElement->Release();
	}
	else
	{
		AfxMessageBox("��ǰ��������ͷ�����");
		return;
	}
}

HRESULT CWeb1800AutoTesterDlg::GetElement(LPCTSTR szElementId, IHTMLElement **pphtmlElement,IHTMLDocument2* pDoc)
{
	return GetElementInterface(szElementId, __uuidof(IHTMLElement), (void **) pphtmlElement,pDoc);
}

HRESULT CWeb1800AutoTesterDlg::GetElementInterface(LPCTSTR szElementId, REFIID riid, void **ppvObj,IHTMLDocument2* pDoc)
{
	ENSURE_ARG(ppvObj!=NULL);
	HRESULT hr = E_NOINTERFACE;
	*ppvObj = NULL;
	CComPtr<IDispatch> spdispElem;
	BOOL bCollect = FALSE;
	hr = GetElement(szElementId, &spdispElem,&bCollect,pDoc);

	if (spdispElem)
		hr = spdispElem->QueryInterface(riid, ppvObj);
	return hr;
}

HRESULT CWeb1800AutoTesterDlg::GetElement(LPCTSTR szElementId, IDispatch **ppdisp, BOOL *pbCollection /*= NULL*/,CComPtr<IHTMLDocument2> sphtmlDoc)
{
	ENSURE_ARG(ppdisp!=NULL);
	CComPtr<IHTMLElementCollection> sphtmlAll;
	CComPtr<IHTMLElementCollection> sphtmlColl;
	CComPtr<IDispatch> spdispElem;
	CComVariant varName;
	CComVariant varIndex;
	HRESULT hr = S_OK;
	//CComPtr<IHTMLDocument2> sphtmlDoc;

	*ppdisp = NULL;

	if(szElementId == NULL)
		return E_INVALIDARG;

	const CString strElementId(szElementId);
	varName.vt = VT_BSTR;
	varName.bstrVal = strElementId.AllocSysString();

#ifndef _UNICODE	
	if (!varName.bstrVal)
	{
		hr = E_OUTOFMEMORY;
		goto Error;
	}
#endif

	hr = sphtmlDoc->get_all(&sphtmlAll);
	if (sphtmlAll == NULL)
		goto Error;
	hr = sphtmlAll->item(varName, varIndex, &spdispElem);
	if (spdispElem == NULL)
	{
		hr = E_NOINTERFACE;
		goto Error;
	}

	spdispElem->QueryInterface(__uuidof(IHTMLElementCollection), (void **) &sphtmlColl);
	if (sphtmlColl)
	{
		if (pbCollection)
			*pbCollection = TRUE;
#ifdef _DEBUG
		else
		{
			TRACE(traceHtml, 0, "Warning: duplicate IDs or NAMEs.\n");
			ATLASSERT(FALSE);
		}
#endif

	}
Error:
	if (SUCCEEDED(hr))
	{
		*ppdisp = spdispElem;
		if (spdispElem)
			(*ppdisp)->AddRef();
	}
	return hr;
}

HWND CWeb1800AutoTesterDlg::GetExplorerHWND()
{
	HWND hwnd = ::FindWindow("IEFrame","���Ͽͷ����� - Microsoft Internet Explorer");
	if (!hwnd)
	{
		AddToList("֧������ҳ��δ�ҵ�");
		return NULL;
	}
	hwnd = g_find.GetChildWindow(hwnd,5);
	hwnd = g_find.GetChildWindow(hwnd,1);
	CString strShow = g_find.GetContrlClassName(hwnd);
	if (g_find.GetContrlClassName(hwnd) == "Internet Explorer_Server")
	{
		return hwnd;
	}
	return NULL;
}

void CWeb1800AutoTesterDlg::GetElementCollection(CString tagName,IHTMLElementCollection** pSpecialColl,IHTMLDocument2* pDoc)
{
	IHTMLElementCollection* pElemColl = NULL;
	IDispatch* pElemDisp = NULL;
	IHTMLElement* pElem=NULL;
	if (pDoc != NULL)
	{
		pDoc->get_all(&pElemColl);
		if (pElemColl != NULL)
		{
			_variant_t var(tagName); 
			pElemColl->tags(var,&pElemDisp);	//ָ��TAG����
			if (pElemDisp != NULL)
			{
				*pSpecialColl = (IHTMLElementCollection*)pElemDisp;
				pElemColl->Release();
				return;
			}
			pElemColl->Release();
		}
	}
}
CString CWeb1800AutoTesterDlg::GetHtmlContentEx(IHTMLDocument2* pDoc)
{
	IHTMLElement* pelmBody = NULL;
	if (!pDoc)
	{
		return "";
	}
	pDoc->get_body(&pelmBody);
	if (!pelmBody)
	{
		pDoc->Release();
		return "";
	}
	BSTR bstrStr;
	pelmBody->get_innerHTML(&bstrStr);
	CString str(bstrStr);
	::SysFreeString(bstrStr);
	CFile file;
	file.Open("c:\\tessssss.txt",CFile::modeCreate|CFile::modeWrite);
	file.Write(str,str.GetLength());
	file.Close();	
	pelmBody->Release();		
	return str;
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
CString CWeb1800AutoTesterDlg::GetKeyString(CString &srcStr,CString &strBefore,CString &strAfter)
{
	int iPosBefore = 0;
	int iPosAfter = 0;
	iPosBefore = srcStr.Find(strBefore) + strBefore.GetLength();
	iPosAfter = srcStr.Find(strAfter,iPosBefore);
	if (iPosBefore < 0 || iPosAfter < 0 || iPosAfter - iPosBefore < 0 )
	{
		return "";
	}
	else
	{
		return srcStr.Mid(iPosBefore,iPosAfter - iPosBefore);
	}
}

void CWeb1800AutoTesterDlg::ThreadFunc(LPVOID lParam)
{
	CoInitialize(NULL);
	CWeb1800AutoTesterDlg *pParam = ((CWeb1800AutoTesterDlg *)lParam);
	if ( pParam )
	{
		pParam->RunFunc();
		pParam->AddToList("�ͻ����߳��˳�");
		g_ExitFlagClient = true;
	}
	::CoUninitialize();
	pParam->GetDlgItem(IDC_BTN_STARTTEST)->EnableWindow(TRUE);
	pParam->GetDlgItem(IDC_BTN_STOPCLIENT)->EnableWindow(FALSE);
}

void CWeb1800AutoTesterDlg::ThreadFuncSp(LPVOID lParam)
{
	CoInitialize(NULL);
	CWeb1800AutoTesterDlg *pParam = ((CWeb1800AutoTesterDlg *)lParam);
	if ( pParam )
	{
		pParam->RunFuncSp();
		pParam->AddToListSp("֧�ֶ��߳��˳�");
		g_ExitFlagServer = true;
	}
	::CoUninitialize();
	pParam->GetDlgItem(IDC_BTN_STARTSP)->EnableWindow(TRUE);
	pParam->GetDlgItem(IDC_BTN_STOPSP)->EnableWindow(FALSE);
}
void CWeb1800AutoTesterDlg::RunFuncSp()
{
	HWND hwnd = NULL;
	HWND hwndSession = NULL;
	CString strCustomerName = "";
	CString strTitle = "";
	bool bAccepted = false;
	AddToListSp("֧�ֶ��߳�����");
	while(!g_ExitFlagServer)
	{
		if (::FindWindow("#32770","���Ͽͷ����� Ver 3.0") == NULL)
		{
			AddToListSp("֧�ֶ˳���δ����");
			Sleep(2000);
			continue;
		}
		strCustomerName = "";
		hwndSession = NULL;
		bAccepted = false;
		//�����Ƿ����������������У������
		hwnd = ::FindWindow("#32770","��������");
		if (hwnd && ::IsWindow(hwnd) && ::IsWindowVisible(hwnd))
		{
			AddToListSp("�пͻ����󣬽���");
			HWND hwndTemp = g_find.GetChildWindow(hwnd,6);
			CString strText = g_find.GetContrlWindowText(hwndTemp);
			if (strText.Find("����õ�Զ�̷���")>0)
			{
				strCustomerName = strText.Left(strText.Find("����õ�Զ�̷���"));
				AddToListSp("�ͻ����ƣ� "+strCustomerName);
			}			
			hwnd = g_find.GetChildWindow(hwnd,1);
			g_find.PostBtnClick(hwnd);
			::PostMessage(hwnd,WM_MOUSEACTIVATE,0,WM_LBUTTONDOWN);
			
 			::PostMessage(hwnd,WM_LBUTTONDOWN,MK_LBUTTON,MAKELONG(32,12));
 			Sleep(10);
 			::PostMessage(hwnd,WM_LBUTTONUP,0,MAKELONG(32,12));
			bAccepted = true;
			Sleep(1000);
		}
		if (!bAccepted)
		{
			AddToListSp("�ȴ��ͻ��˷�������");
			Sleep(1000);
			continue;
		}
		AddToListSp("�ȴ�5��");
		Sleep(5000);
		int iCount = 0;
		strTitle.Format("%s - ���Ͽͷ����� ���񴰿�",strCustomerName);
		while(!g_ExitFlagServer && hwndSession==NULL && iCount++ < 20)
		{
			hwndSession = ::FindWindow("#32770",strTitle);
			Sleep(500);
		}
		if(g_ExitFlagServer)
		{
			return;
		}
		if (!hwndSession)
		{
			Sleep(500);
			continue;
		}		
		AddToListSp("�ҵ��Ự����");
		Sleep(500);
		//��ʼ����ť
		
		//�ͻ��Ѿ��˳���
		//�����ʾ�ͻ��Ѿ��˳�
		
		
		HWND hwndCtrol = g_find.GetChildWindow(hwndSession,13);		
		if (::IsWindowEnabled(hwndCtrol))
		{
			g_find.PostBtnClick(hwndCtrol);
		}
		else
		{
			g_hwndWarn = NULL;
			EnumChildWindows(NULL,EnumChildProcWarn,NULL);
			if (g_hwndWarn && ::IsWindow(g_hwndWarn) && ::IsWindowVisible(g_hwndWarn))
			{
				::ShowWindow(::GetParent(g_hwndWarn),SW_HIDE);				
			}
			//�رջỰ����
			::PostMessage(hwndSession,WM_CLOSE,0,0);
		}
		AddToListSp("����Զ�̿�������");
		while(!g_ExitFlagServer)
		{
			//�ͻ��Ѿ��˳��Ի���
			//�ͻ��Ѿ��˳���
			//���Ͽͷ�����
			hwnd = ::FindWindow("#32770","��ʾ��Ϣ");
			::PostMessage(hwnd,WM_CLOSE,0,0);	
			hwnd = ::FindWindow("#32770","���Ͽͷ�����");
			::PostMessage(hwnd,WM_CLOSE,0,0);
			//���ҵ�ǰ�Ƿ񵯳���֧�ּ�¼���ڣ����������˵���Ự�������˳�
			hwnd = ::FindWindow("#32770","֧�ּ�¼");
			if (hwnd && ::IsWindow(hwnd) && ::IsWindowVisible(hwnd))
			{
				AddToListSp("�ͻ��˹ر��˻Ự���˳�");
				hwnd = g_find.GetChildWindow(hwnd,15);
				g_find.PostBtnClick(hwnd);				
				break;
			}
			Sleep(1000);
		}

		Sleep(2000);
	}
}

void CWeb1800AutoTesterDlg::RunFunc()
{
	bool bRet = false;
	int iErrorCount = 0;
	int iErrorCountEx = 0;
	IHTMLDocument2 *pDoc = NULL;
	CString strUrl = "";
	GetDlgItemText(IDC_EDIT_URL,strUrl);
	while(!g_ExitFlagClient)
	{
		Sleep(500);
		if (g_hwndSessionWindow == NULL || !::IsWindow(g_hwndSessionWindow))
		{
			//��������
			//CFileUtil::
			//�ر����е���ҳ������
			HWND hwnd = ::FindWindow("IEFrame","���Ͽͷ����� - Microsoft Internet Explorer");
			if (hwnd)
			{
				AddToList("�ر�֧������ҳ��");
				::PostMessage(hwnd,WM_CLOSE,0,0);
				Sleep(1000);
			}
			//�����ʱ�ļ�
			AddToList("ж�ؿؼ��������ʱ�ļ�");
			ClearFiles();
			AddToList("���Դ�֧������ҳ��");
			
			ShellExecute(NULL, "open", "IEXPLORE", strUrl, 0, SW_SHOWMAXIMIZED);
			//�ٴ�֧������ҳ��
			iErrorCount = 0;
			while (!g_ExitFlagClient && iErrorCountEx++<20 && ::FindWindow("IEFrame","���Ͽͷ����� - Microsoft Internet Explorer") == NULL)
			{
				Sleep(1000);
			}
			if (g_ExitFlagClient)
			{
				return;
			}
			Sleep(500);
			bRet = SendRequest();
			if (!bRet)
			{
				AddToList("��ͣ5��");
				Sleep(5000);
				continue;
			}
			//���һỰ����
			g_hwndSessionWindow = NULL;
			int iCount = 0;
			AddToList("��ʼ���һỰ����");
			while (!g_ExitFlagClient && !g_hwndSessionWindow && iCount++ < 30)
			{
				g_hwndSessionWindow = ::FindWindow("IEFrame","���Ͽͷ�����-���񴰿� - Microsoft Internet Explorer");
				Sleep(500);
			}
			if (g_ExitFlagClient)
			{
				return;
			}
			if (g_hwndSessionWindow == NULL)
			{
				AddToList("�Ựδ����");
				Sleep(500);
				continue;
			}
			AddToList("�Ự���������ɹ�");
			pDoc = NULL;
		}

		//�Ự�Ѿ����룬��ʼ���п���
		if (pDoc == NULL)
		{
			HWND hwnd = g_find.GetChildWindow(g_hwndSessionWindow,5);
			hwnd = g_find.GetChildWindow(hwnd,1);
			pDoc = GetDocInterface(hwnd);
			if (!pDoc)
			{
				iErrorCount++;
				if (iErrorCount > 5)
				{
					iErrorCount = 0;
					::PostMessage(g_hwndSessionWindow,WM_CLOSE,0,0);
					g_hwndSessionWindow = NULL;
					Sleep(500);
				}
				Sleep(1000);
				continue;;
			}
		}		
		//�ж��Ƿ��н��ܿ�����У������
		bRet = AcceptRemote(pDoc);
		if (bRet == false)
		{
			//δ�н��ܶԻ��򣬵ȴ�����
			AddToList("�ȴ�֧�ֶ˷�������");
			Sleep(1000);			
			continue;
		}
		Sleep(500);
		//���ܺ�ȴ��ж�N���Զ���Ƿ�ɹ�
		int iCount = 0;
		bool bStartSuc = false;
		CString strMsg = "";
		while (!g_ExitFlagClient && ::IsWindow(g_hwndSessionWindow) && iCount++ < 30)
		{
			ClickOcxInstall();
			HWND hwnd = ::FindWindow("#32770","��ʾ��Ϣ");
			::PostMessage(hwnd,WM_CLOSE,0,0);	
			hwnd = ::FindWindow("#32770","���Ͽͷ�����");
			::PostMessage(hwnd,WM_CLOSE,0,0);
			//ͨ��PDOC��ȡ���ݣ��ȶԱ�־�ַ���
			//�õ��Ự���ݣ�������������ɹ����ַ��������ʾ�ɹ�
			strMsg = GetInnerText(pDoc,"Layer1");
			if (strMsg.Find("�����ɹ�")>0)
			{
				bStartSuc = true;
				break;
			}
			else if (strMsg.Find("ʧ��") > 0)
			{
				bStartSuc = false;
				break;
			}
			Sleep(1000);
		}
		HWND hwnd = ::FindWindow("#32770","��ʾ��Ϣ");
		::PostMessage(hwnd,WM_CLOSE,0,0);	
		hwnd = ::FindWindow("#32770","���Ͽͷ�����");
		::PostMessage(hwnd,WM_CLOSE,0,0);
		if (g_ExitFlagClient)
		{
			return;
		}
		if (!::IsWindow(g_hwndSessionWindow))
		{
			AddToList("�Ự�ѹرգ�����");
			g_hwndSessionWindow = NULL;
		}
		//���Զ�������ɹ�����ر�
		if (bStartSuc)
		{
			AddToList("Զ�������ɹ����˳��Ự");
		}
		else
		{
			//��¼��־��Ϣ
			AddToList("Զ������ʧ�ܣ��˳��Ự");
			AddToList(strMsg);			
			//��¼��Ϣ
		}
		//�˳��Ự
		StopSession(pDoc);
		//��ⵯ���ľ���򣬵�����ǡ�
		//�ȴ�һ��ʱ��
		Sleep(3000);
		//�رջỰ���ڣ�������ڵĻ�
		if (::IsWindow(g_hwndSessionWindow))
		{
			AddToList("�رջỰ����");
			::PostMessage(g_hwndSessionWindow,WM_CLOSE,0,0);
			g_hwndSessionWindow = NULL;
		}
		Sleep(3000);
	}
}

CString CWeb1800AutoTesterDlg::GetInnerText(IHTMLDocument2 *pDoc,CString strId)
{
	if (pDoc == NULL)
	{
		return "";
	}
	IHTMLElement *pElement = NULL;
	GetElement(strId,&pElement,pDoc);
	if (pElement == NULL)
	{
		return "";
	}

	BSTR bstrStr;	
	pElement->get_innerText(&bstrStr);	
	CString str((char*)(_bstr_t)bstrStr);
	::SysFreeString(bstrStr);
	pElement->Release();	
	return str;
}

bool CWeb1800AutoTesterDlg::PostWebClick(IHTMLDocument2 *pDoc,CString strId)
{
	if (pDoc == NULL)
	{
		return false;
	}
	IHTMLElement *pElement = NULL;
	GetElement(strId,&pElement,pDoc);
	if (pElement == NULL)
	{
		return false;
	}	
	pElement->click();
	pElement->Release();
	return true;
}

bool CWeb1800AutoTesterDlg::StopSession(IHTMLDocument2 *pDoc)
{
	bool bRet = PostWebClick(pDoc,"close");
	if (bRet)
	{
		AddToList("ֹͣ�Ự");
	}
	return bRet;
}
bool CWeb1800AutoTesterDlg::AcceptRemote(IHTMLDocument2 *pDoc)
{
	bool bRet = PostWebClick(pDoc,"accept");
	if (bRet)
	{
		AddToList("����֧�ֶ�����");
	}
	return bRet;
}
void CWeb1800AutoTesterDlg::OnBnClickedButton2()
{
	if (g_hwndSessionWindow == NULL)
	{
		g_hwndSessionWindow = ::FindWindow("IEFrame","���Ͽͷ�����-���񴰿� - Microsoft Internet Explorer");		
	}
	HWND hwnd = g_find.GetChildWindow(g_hwndSessionWindow,5);
	hwnd = g_find.GetChildWindow(hwnd,1);
	IHTMLDocument2 *pDoc = GetDocInterface(hwnd);
	AcceptRemote(pDoc);
	pDoc->Release();
	

}

void CWeb1800AutoTesterDlg::ClearFiles()
{
	CString strpath = CFileUtil::GetUserPath();
	strpath = strpath;
	//strpath = "c:\\test\\test.txt\0";
	char szPath[256] = {0};
	//ж�ؿؼ�
	//
	RegistOcx("C:\\WINDOWS\\system32\\WebI800.dll",false);
	RegistOcx(strpath+"\\WebI800\\WebI800.dll",false);
	Sleep(50);
	CFileFind fileFinder;
	//strpath += "\\*.*";

	//�����ʱ�ļ�
	BOOL bRes = fileFinder.FindFile(strpath + "\\*.*");
	while(bRes)
	{
		bRes = fileFinder.FindNextFile();
		CString strPath = fileFinder.GetFileName();
		if(fileFinder.IsDirectory() && !fileFinder.IsDots())
		{			
			if ( strPath == "FirstFolder" || strPath.Find("Web1800V")>=0 || strPath == "WebI800" || strPath == "SPScreenShot" || strPath == "EUScreenShot")
			{
				try
				{
					memset(szPath,0,256);
					sprintf(szPath,"%s\\%s",strpath,strPath);
					SHFILEOPSTRUCT FileOp={0};
					FileOp.fFlags = FOF_ALLOWUNDO |   //����Żػ���վ
					FOF_NOCONFIRMATION; //������ȷ�϶Ի���
					FileOp.pFrom = szPath;
					FileOp.pTo = NULL;      //һ��Ҫ��NULL
					FileOp.wFunc = FO_DELETE;//ɾ������
					SHFileOperation(&FileOp);
				}				
				catch (...)
				{
				}
			}
		}
	}
	
}
void CWeb1800AutoTesterDlg::OnBnClickedButton3()
{
	ClearFiles();
}

void CWeb1800AutoTesterDlg::OnBnClickedButton4()
{
}

bool CWeb1800AutoTesterDlg::RegistOcx(LPCTSTR strFileName, bool bInstall)
{
	try
	{
		if(strFileName == NULL)
			return false;

		HINSTANCE hInstance = ::LoadLibrary(strFileName);
		if(hInstance < (HINSTANCE)HINSTANCE_ERROR)
		{
			return false;
		}
		

		FARPROC lpDllEntryPoint;
		if(bInstall)
			lpDllEntryPoint = GetProcAddress(hInstance, _T("DllRegisterServer"));
		else
			lpDllEntryPoint = GetProcAddress(hInstance, _T("DllUnregisterServer"));

		if(lpDllEntryPoint != NULL)   
		{   
			if(FAILED((*lpDllEntryPoint)()))   
			{
				FreeLibrary(hInstance);
				return false;
			}
			FreeLibrary(hInstance);
			return true;
		}
		else
		{
			return false;
		}
	}
	catch(...)
	{
		return false;
	}
	return true;
}
void CWeb1800AutoTesterDlg::ClickOcxInstall()
{
	HWND hwnd = ::FindWindow("#32770","Internet Explorer - ��ȫ����");
	hwnd = g_find.GetChildWindow(hwnd,16);
	if (hwnd)
	{
		g_find.PostBtnClick(hwnd);
	}
	
}

void CWeb1800AutoTesterDlg::OnBnClickedBtnStopclient()
{
	g_ExitFlagClient = true;
	GetDlgItem(IDC_BTN_STOPCLIENT)->EnableWindow(FALSE);
}

void CWeb1800AutoTesterDlg::OnBnClickedBtnStopsp()
{
	g_ExitFlagServer = true;
	GetDlgItem(IDC_BTN_STOPSP)->EnableWindow(FALSE);
}

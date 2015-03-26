// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "DbGate.h"
#include "MainFrm.h"
#include "DialogFile/MainTopDialog.h"
#include "DialogFile/MainBottomDialog.h"
#include "DialogFile/DbGateAllSet.h"
#include "DialogFile/DbGateClose.h"
#include "DialogFile/SingleSend.h"
#include <direct.h>
#include <io.h>
#include "ControlFile/UserFile.h"
#include "ControlFile/dataType.h"
#define WM_SHOWTASK WM_USER + 1000
#define WM_USERMSGQUIT WM_USER + 1001

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


int g_iBalance;	//���

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	
	ON_WM_SYSCOMMAND()
	ON_WM_CREATE()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_MENU_RUN, OnMenuRun)
	ON_COMMAND(ID_MENU_SET, OnMenuSet)
	ON_COMMAND(ID_MENU_CHECKFEE, OnMenuCheckfee)
	ON_COMMAND(ID_MENU_STOP, OnMenuStop)
	ON_UPDATE_COMMAND_UI(ID_MENU_RUN, OnUpdateMenuCmd)
	ON_UPDATE_COMMAND_UI(ID_MENU_STOP, OnUpdateMenuCmd)
	ON_WM_CLOSE()
	ON_MESSAGE(WM_SHOWTASK,OnShowTask)
	ON_MESSAGE(WM_USERMSGQUIT,OnAppExit)
	ON_MESSAGE(WM_USERMSGSEND,OnSendMessage)
	ON_MESSAGE(WM_USERMSGSTATUS,OnStatusMessage)
	ON_COMMAND(ID_APP_QUIT, OnAppQuit)
	ON_WM_TIMER()
	ON_COMMAND(ID_MENU_SINGLESEND, &CMainFrame::OnMenuSinglesend)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_NUM
	//ID_INDICATOR_SCRL,
};

/************************************************************************/
/*                                                                      */
/************************************************************************/
CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
	m_bCreateSplit = false;	
	m_bAutoStop = false;
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
CMainFrame::~CMainFrame()
{
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_TOOLBAR))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar,AFX_IDW_DOCKBAR_RIGHT);

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}
	m_StartTime = CTime::GetCurrentTime();
	m_wndStatusBar.SetPaneInfo(0, ID_SEPARATOR, SBPS_NORMAL,170);	
	m_wndStatusBar.SetPaneInfo(1, ID_SEPARATOR, SBPS_NORMAL, 120);
	m_wndStatusBar.SetPaneText(1,"��¼״̬:δ��¼");
	m_wndStatusBar.SetPaneInfo(2, ID_SEPARATOR, SBPS_NORMAL, 70);
 	m_wndStatusBar.SetPaneText(2,"����:0");
 	m_wndStatusBar.SetPaneInfo(3, ID_SEPARATOR, SBPS_NORMAL, 150);
 	m_wndStatusBar.SetPaneText(3,"�˻����:δ��ѯ");
	m_wndStatusBar.SetPaneInfo(4, ID_SEPARATOR, SBPS_NORMAL, 50);
	m_wndStatusBar.SetPaneText(4,"����:0");
	m_wndStatusBar.SetPaneInfo(5, ID_SEPARATOR, SBPS_NORMAL, 50);
	m_wndStatusBar.SetPaneText(5,"����ʱ��");
	m_wndStatusBar.SetPaneInfo(6, ID_SEPARATOR, SBPS_NORMAL, 120);
 	m_wndStatusBar.SetPaneText(6,m_StartTime.Format("%Y-%m-%d %H:%M:%S"));	
	return 0;
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	
	
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
	cs.style = WS_OVERLAPPED | WS_CAPTION | FWS_ADDTOTITLE
		| WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX 
		| WS_MAXIMIZE | WS_SYSMENU;
	if(!CFrameWnd::PreCreateWindow(cs))
		return FALSE;
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG

/************************************************************************/
/*                                                                      */
/************************************************************************/
BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT /*lpcs*/,
								CCreateContext* pContext)
{
	CRect rtWnd;
	GetClientRect(&rtWnd);

	if (m_wndSplitter.CreateStatic(this, 2, 1) == FALSE)
	{
		return FALSE;
	}
	m_wndSplitter.CreateView( 0, 0, RUNTIME_CLASS( CMainTopDialog ) , 
		CSize(0,0), pContext);
	m_wndSplitter.CreateView( 1, 0, RUNTIME_CLASS( CMainBottomDialog ), 
		CSize(0,0), pContext);
	m_wndSplitter.SetRowInfo(0,rtWnd.Height() * 4 / 7,0);
	m_wndSplitter.SetRowInfo(1,rtWnd.Height() * 3 / 7,0);
	m_wndSplitter.SetActivePane(0,0,NULL);
	m_bCreateSplit = true;
	m_ThreadRunFlag = FALSE;
	m_MWSendCtrl.SetFrameHwnd(m_hWnd);

	m_bAutoStop = false;
	int iAutoRun = 0;
	CString strAppPath = "";
	CString strAppPathEx = "";
	CUserFile *pFile = NULL;
	pFile = CUserFile::GetInstance();
	if (pFile)
	{	
		int iRet = -1;		
		CString strTemp = "";
		pFile->GetAppPath(strAppPath);
		strAppPathEx = strAppPath;
		strAppPath += MWGATEINITFILE;
		//�û�������Ϣ	
		pFile->ReadInitInfo(strAppPath,"AutoRun","AutoRun",strTemp,iAutoRun,1);
		pFile->CheckInitValue(iAutoRun,0,1);
		//��¼��־��־
		pFile->ReadInitInfo(strAppPath,"log","type",strTemp,g_iIsNeedToLog,1);
		pFile->CheckInitValue(g_iIsNeedToLog,0,1);

		pFile->ReadInitInfo(strAppPath,"debuglog","type",strTemp,g_iDebugLog,1);
		pFile->CheckInitValue(g_iDebugLog,0,1);
	}
	if ( iAutoRun == 1 )
	{
		PostMessage(WM_COMMAND,ID_MENU_RUN);
	}	
	SetToTray();
	//������������
	RunHelpTool(strAppPathEx);
	SetTimer(2,1000,NULL);
	return TRUE;
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
void CMainFrame::OnSize(UINT nType, int cx, int cy)
{
	int nHeight;
	CRect clientRect;
	CRect rect;
	GetClientRect(clientRect);
	nHeight = clientRect.Height();
	if( m_bCreateSplit )
	{
		m_wndSplitter.SetRowInfo(0,nHeight * 4 / 7,0);
		m_wndSplitter.SetRowInfo(1,nHeight * 3 / 7,0);
		m_wndSplitter.SetActivePane(0,0,NULL);
	}
	CFrameWnd::OnSize(nType, cx, cy);
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
//������ť
void CMainFrame::OnMenuRun()
{
	m_MWSendCtrl.Run();
	m_ThreadRunFlag = TRUE;
	m_bAutoStop = false;
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
//����
void CMainFrame::OnMenuSet()
{
	CDbGateAllSet dbSet;
	dbSet.DoModal();
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CMainFrame::ReSetParams()
{
	//AddToDispListShow("�����Ѹ���");
	if ( m_ThreadRunFlag == TRUE && m_MWSendCtrl.GetRunStatus() == STATUS_RUN_NORMAL)
	{
		m_MWSendCtrl.ReInitParams();
	}
	else
	{
		m_MWSendCtrl.InitParams();		
	}	
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
//��ѯ���
void CMainFrame::OnMenuCheckfee()
{
	m_MWSendCtrl.CheckFee();	
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
//ֹͣ
void CMainFrame::OnMenuStop()
{
	if (m_bAutoStop == false)				//��������Զ�ֹͣ��
	{
		m_MWSendCtrl.SetRunStatus(STATUS_STOP_MANUAL);
		m_MWSendCtrl.Stop();
	}		
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
void CMainFrame::OnUpdateMenuCmd(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(FALSE);
 	switch(pCmdUI->m_nID)
	{
  		case ID_MENU_RUN:
  			pCmdUI->Enable(!m_ThreadRunFlag);
  			break;
  		case ID_MENU_STOP:
  			pCmdUI->Enable(m_ThreadRunFlag);
  			break;
 		default:
 			break;
	}
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
void CMainFrame::OnClose()
{
	ShowWindow(SW_HIDE);
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
void CMainFrame::SetToTray()
{
	m_nid.cbSize=(DWORD)sizeof(NOTIFYICONDATA);
	m_nid.hWnd=this->m_hWnd;
	m_nid.uID=IDR_MAINFRAME;
	m_nid.uFlags=NIF_ICON|NIF_MESSAGE|NIF_TIP ;
	m_nid.uCallbackMessage = WM_SHOWTASK;//�Զ������Ϣ����
	m_nid.hIcon=LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME));
	//���ñ�������
	CString strAppPath="";
	CUserFile *pFile = CUserFile::GetInstance();
	if ( pFile )
	{
		pFile->GetAppPath(strAppPath);
	}
	CString szFilePath = strAppPath + MWGATEINITFILE;
	char szwindowName[1000];
	::GetPrivateProfileString("window", "name", "������������",(LPSTR)szwindowName,1000, szFilePath);
	g_strWindowName.Format("%s",szwindowName);
	if (g_strWindowName=="")
	{
		g_strWindowName = "������������";
	}
	if (g_strWindowName.GetLength()>128)
	{
		strncpy_s(m_nid.szTip,g_strWindowName,128);
	}
	else
	{
		strcpy_s(m_nid.szTip,g_strWindowName);
	}
	g_strVirsion = pFile->GetFileVersion(pFile->GetCurrentAppPath());
	//д��汾��Ϣ
	pFile->WriteInitInfo(strAppPath+MWGATEINITFILE,"window","version",g_strVirsion,0,0);
	//��ȡӦ�ó�������д��
	CString strAppName = "";
	CString strTemp = "";
	int iInterval = 0;
	pFile->GetAppName(strAppName);	
	pFile->WriteInitInfo(strAppPath+MWGATEINITFILE,"window","appname",strAppName,0,0);
	//�ӿڳ�������״̬ 0��ʾ����������1��ʾ�����ر�
	pFile->WriteInitInfo(strAppPath+MWGATEINITFILE,"window","runstatus","",0,1);
	pFile->ReadInitInfo(strAppPath+MWGATEINITFILE,"window","interval",strTemp,iInterval,1);
	if (iInterval < 1000)
	{
		pFile->WriteInitInfo(strAppPath+MWGATEINITFILE,"window","interval","",5000,1);
	}	
	g_strWindowName += " "+g_strVirsion;
	Shell_NotifyIcon(NIM_ADD,&m_nid);//�����������ͼ��
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
LRESULT CMainFrame::OnAppExit(WPARAM wParam,LPARAM lParam)
{
	int iRet = MessageBox("ȷ��Ҫ�˳���?","��ʾ",MB_ICONQUESTION|MB_YESNO);
	if (iRet != IDYES)
	{
		return 0;
	}
	if(!IsWindowVisible())
	{
		this->ShowWindow(SW_SHOW);
	}
	m_MWSendCtrl.AppExit();	
	Shell_NotifyIcon(NIM_DELETE,&m_nid);//ɾ��������ͼ��
	KillTimer(2);
	KillTimer(1);
	Sleep(200);
	CFrameWnd::OnClose();
	return 0;
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
LRESULT CMainFrame::OnShowTask(WPARAM wParam,LPARAM lParam)
{
	if(wParam!=IDR_MAINFRAME)
	{
		return 1;
	}
	switch(lParam)
	{
	case WM_RBUTTONUP:
		{
			CMenu menu; 
			//menu.LoadMenu(IDR_POPUPMENU); //�������ȶ����ѡ��
			menu.LoadMenu(IDR_MAINFRAME); //�������ȶ����ѡ��
			CMenu *pmenu = menu.GetSubMenu(0); 
			CPoint pos; 
			GetCursorPos(&pos);
			SetForegroundWindow();
			pmenu->TrackPopupMenu(TPM_CENTERALIGN,pos.x,pos.y,AfxGetMainWnd());
		}		
		break;
	case WM_LBUTTONDOWN:
		{
			if( !IsWindowVisible())
			{
				this->ShowWindow(SW_SHOW);
			}
			else
			{
				this->ShowWindow(SW_HIDE);
			}			
		}
		break;
	}
	return 0;
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CMainFrame::OnAppQuit()
{
	PostMessage(WM_USERMSGQUIT,0,0);
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
void CMainFrame::OnSysCommand(UINT nID, LPARAM lParam)
{
	CFrameWnd::OnSysCommand(nID, lParam);
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
LRESULT CMainFrame::OnStatusMessage(WPARAM wParam,LPARAM lParam)
{
	try
	{

		CString strShow = "";
		int iLParam = int(lParam);
		int iIndex = int(wParam);
		switch (iIndex)
		{
		case 1:		//��¼״̬
			strShow = iLParam == 0?"��¼״̬:�Ͽ�":"��¼״̬:�ѵ�¼";
			break;
		case 2:		//����
			strShow.Format("����:%d",iLParam);
			break;
		case 3:		//���
			if (iLParam == -2)
			{
				strShow = "�˻����:δ��ѯ";
			}
			else
			{
				strShow.Format("�˻����:%d",iLParam);
			}
			break;
		case 4:		//����
			strShow.Format("����:%d",iLParam);
			break;
		default:
			return 0;
		}		
		m_wndStatusBar.SetPaneText(iIndex,strShow);
	}
	catch(...)
	{		
	}
	return 0;
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CMainFrame::SendSingleMessage(CString strPhone,CString strSubport,CString strContent)
{
	m_MWSendCtrl.AddSingleSend(strPhone,strContent,strSubport);
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
LRESULT CMainFrame::OnSendMessage(WPARAM wParam,LPARAM lParam)
{
	try
	{
		//�߳���ֹ��Ϣ
		m_ThreadRunFlag = FALSE;
		int iFlag = lParam;
		switch (iFlag)
		{
		case STATUS_RUN_NORMAL:
			{
				m_bAutoStop = false;
				break;
			}
		case STATUS_STOP_ERROR:
			{
				m_bAutoStop = true;
				break;
			}
		case STATUS_STOP_MANUAL:
			{
				m_bAutoStop = false;
				break;
			}
		case STATUS_STOP_NORMAL:		//���ø���,�߳�����
			{
				m_bAutoStop = true;
				PostMessage(WM_COMMAND,ID_MENU_STOP);
				Sleep(200);
				PostMessage(WM_COMMAND,ID_MENU_RUN);
				return 0;
			}
		case STATUS_STOP_OVERRUNTIME:
			{
				m_bAutoStop = true;
				break;
			}
		default:
			{
				m_bAutoStop = true;
				break;
			}
		}
		PostMessage(WM_COMMAND,ID_MENU_STOP);
	}
	catch (...)
	{		
	}	
	return 0;
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
void CMainFrame::OnTimer(UINT nIDEvent) 
{
 	if (nIDEvent == 2)
	{		
		int iRet = m_MWSendCtrl.IsNeedToReRunOnTime();
		if ( m_ThreadRunFlag == TRUE && m_MWSendCtrl.GetRunStatus() == STATUS_RUN_NORMAL)
		{	
			//����������
			//�����̶���
			if (iRet == 1)
			{
				m_MWSendCtrl.SetRunStatus(STATUS_STOP_OVERRUNTIME);
				m_bAutoStop = true;
				//AddToDispListShow("�����ɷ���ʱ���,�Զ�ֹͣ");				
				m_MWSendCtrl.Stop();
			}
		}
		else
		{
			//ֹͣ
			//����ֹͣ����
			if (iRet == 0)
			{
				if ( m_MWSendCtrl.GetRunStatus() == STATUS_STOP_OVERRUNTIME )
				{
					//AddToDispListShow("����ɷ���ʱ���,�Զ�����");
					PostMessage(WM_COMMAND,ID_MENU_RUN);
				}				
			}
		}
 	}
	CFrameWnd::OnTimer(nIDEvent);
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
//�˺�����ʱ��ʹ��
CString CMainFrame::GetRunningTime()
{
	CTimeSpan tSpan;
	tSpan = CTime::GetCurrentTime() - m_StartTime;
	LONGLONG totalSeconds = tSpan.GetTotalSeconds();
	LONGLONG totalMinutes = tSpan.GetTotalMinutes();
	LONGLONG totalHours = tSpan.GetTotalHours();
	CString strRet="";
	strRet.Format("%d��%02dʱ%02d��%02d��",totalHours / 24,totalHours % 24,totalMinutes % 60,totalSeconds % 60);
	return strRet;
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CMainFrame::OnMenuSinglesend()
{
	CSingleSend singleSend;
	singleSend.DoModal();
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
void CMainFrame::RunHelpTool(CString strAppPath)
{
	HINSTANCE iRet = ShellExecute(NULL, "open", strAppPath + "DbHelper.exe", NULL, NULL, SW_SHOW);
	if ((int)iRet<32)
	{
		WriteLog("������������ʧ��",1);
	}
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
BOOL CMainFrame::PreTranslateMessage( MSG* pMsg )
{
	if(GetKeyState(VK_CONTROL) < 0)   
	{
		if (pMsg->wParam == 'S')
		{
			CDbGateAllSet dbSet;
			dbSet.DoModal();
		}
		return TRUE;
	}	
	return CFrameWnd::PreTranslateMessage(pMsg);
}
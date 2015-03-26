// LoserWindowDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "LoserWindow.h"
#include "LoserWindowDlg.h"
#include ".\loserwindowdlg.h"
#include "UserMessagePump.h"
#include "UserFile.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#include <afxmt.h>
#include <afxtempl.h>
#include "WindowFind.h"
#include <io.h>
#include <direct.h>
#include "afxinet.h"
#include <io.h>
#include "ADOFile/AdoRecordSet.h"
//#define  ONLY_LOSER
// #define ONLY_WINNNER
// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���
typedef struct _MyData
{
	DWORD dwSendMsg; // sendmessage������ַ
	HWND hwndParant;
	NMHDR nmh;
}MyData;
//ע�뵽Զ�̽����е�ִ�д���
static DWORD WINAPI RemoteFunc(LPVOID pData)
{	
	MyData *pMyData = (MyData*)pData;
	typedef int(__stdcall*MySendMessage)(HWND,UINT,WPARAM,LPARAM);
	MySendMessage msgSend = (MySendMessage)pMyData->dwSendMsg;
	HWND hwndParant = pMyData->hwndParant;
	NMHDR nmhdr = pMyData->nmh;
	msgSend(hwndParant,WM_NOTIFY,(WPARAM)nmhdr.idFrom,(LPARAM)&nmhdr);		//����˫����Ϣ
	return 0;
}

int g_ExitFlag = 1;
int g_iIsLogin = 0;
HTREEITEM g_hTreeItem = NULL;
CString g_strOldAcc = "";
CString g_strAppPath = "D:\\Program Files\\������Ϸ����\\ShaoWu\\Lobby.exe";
CString g_strRoomPwd = "123";
int g_iWinOrLose = 0;	//1ΪӮ,0Ϊ���
int g_iLoseNum = 0;	//����پֺ󻻺�
int g_iRoomNum = 0;
int g_iSetNum = 0;
int g_iMinBalance = 300;
int g_iMaxBalance = 1000000;
int g_iIsCheckGm = 0;
int g_iSpeed = 1;
int g_iIsHidePlayWindow = 0;
int g_iStopMinutes = 0;
int g_iCheckInterval = 0;
HWND g_hwndSet = NULL;
HWND g_hwndBack = NULL;
HWND g_hwndListView1 = NULL;
HWND g_hwndListView2 = NULL;
HWND g_hwndDesk = NULL;		//������
HWND g_hwndTopMost = NULL;
HWND g_hwndPlayWindow = NULL;
CString g_strBalance = "";

int g_iCheckGmWait = 0;
int g_iCheckGmWaitInterval = 0;
CString g_strGmStr = "";
int g_iIsSetPwd = 0;

HWND g_hwndWindow = NULL;
HWND g_hwndTitle = NULL;
HWND g_hwndContent = NULL;
HWND g_hwndPhoneUrl = NULL;
HWND g_hwndAdd = NULL;
HWND g_hwndDelete = NULL;
CString g_strWindowName = "��������-�ٶ����ɶ����� V1.5 ���ð�";
CString g_strWindowClass = "ThunderRT6FormDC";
CString g_strAddContentName  = "��������";
CString g_strAddContentClass = "ThunderRT6Frame";

CString g_strPhoneUrl = "";

CString g_strDbpath = "";
CString g_strdbPwd = "";

CString g_strExeFolder = "";
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


// CLoserWindowDlg �Ի���

typedef CArray<RegisterPack, RegisterPack> RegisterPackList;
RegisterPackList g_RegisterList;

CLoserWindowDlg::CLoserWindowDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLoserWindowDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CLoserWindowDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_ACC, m_ListAcc);
	DDX_Control(pDX, IDC_LIST_INFO, m_message);
	DDX_Control(pDX, IDC_SLIDER_SPEED, m_SpeedCtrl);
	DDX_Control(pDX, IDC_CHECK_GMCHECK, m_CheckGM);
	DDX_Control(pDX, IDC_CHECK_HIDEWINDOW, m_HideWindow);
	DDX_Control(pDX, IDC_CHECK_GMCHECKWAIT, m_gmCheckWait);
}
_declspec(dllimport) HookStruct SetHookEx(HWND m_hwndHelper);

BEGIN_MESSAGE_MAP(CLoserWindowDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BTN_START, OnBnClickedBtnStart)
	ON_BN_CLICKED(IDC_BUTTON2, OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON3, OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON6, OnBnClickedButton6)
	ON_BN_CLICKED(IDC_BUTTON7, OnBnClickedButton7)
	ON_MESSAGE(USERWINDOWMSG_1,AddToList)
	ON_BN_CLICKED(IDC_BTN_STOP, OnBnClickedBtnStop)
	ON_BN_CLICKED(IDC_BTN_EXIT, OnBnClickedBtnExit)
	ON_BN_CLICKED(IDC_BTN_INPORT, OnBnClickedBtnInport)
	ON_BN_CLICKED(IDC_RADIO_LOSE, OnBnClickedRadioLose)
	ON_BN_CLICKED(IDC_RADIO_WIN, OnBnClickedRadioWin)
	ON_BN_CLICKED(IDC_BTN_FIND, OnBnClickedBtnFind)
	ON_BN_CLICKED(IDC_BTN_SAVE, OnBnClickedBtnSave)
	ON_BN_CLICKED(IDC_BUTTON8, OnBnClickedButton8)
	ON_BN_CLICKED(IDC_BUTTON9, OnBnClickedButton9)
	ON_WM_CLOSE()
	ON_MESSAGE(WM_KILLTHREAD,StopThread)
	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDC_CHECK_GMCHECK, OnBnClickedCheckGmcheck)
	ON_BN_CLICKED(IDC_CHECK_HIDEWINDOW, OnBnClickedCheckHidewindow)
	ON_BN_CLICKED(IDC_CHECK_GMCHECKWAIT, OnBnClickedCheckGmcheckwait)
END_MESSAGE_MAP()


// CLoserWindowDlg ��Ϣ�������

BOOL CLoserWindowDlg::OnInitDialog()
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
	SetTableStyle();
	InitListCtrl();
// #ifdef _DEBUG
// #else			//������Ҫ����Ȩ��
// if (GetPower() != 0)
// {
// 	exit(0);
// }
// #endif
	CUserMessagePump *pPump = CUserMessagePump::GetInstance();
	if (pPump)
	{
		pPump->SetHwnd(USERWINDOWMSG_1,m_hWnd);
	}
	CString strAppPath = "";
	CString strTemp = "";
	int iTemp = 0;
	CUserFile *pFile = CUserFile::GetInstance();
	pFile->GetAppPath(strAppPath);
	g_strExeFolder = strAppPath;
	if (pFile)
	{
		pFile->ReadInitInfo(strAppPath+"set.ini","Fresh","path",g_strAppPath,iTemp,0);
		pFile->ReadInitInfo(strAppPath+"set.ini","Fresh","roompwd",g_strRoomPwd,iTemp,0);
		pFile->ReadInitInfo(strAppPath+"set.ini","Fresh","type",strTemp,g_iWinOrLose,1);
		pFile->ReadInitInfo(strAppPath+"set.ini","Fresh","losenum",strTemp,g_iLoseNum,1);
		pFile->ReadInitInfo(strAppPath+"set.ini","Fresh","desknum",strTemp,g_iRoomNum,1);
		pFile->ReadInitInfo(strAppPath+"set.ini","Fresh","setnum",strTemp,g_iSetNum,1);
		pFile->ReadInitInfo(strAppPath+"set.ini","Fresh","min",strTemp,g_iMinBalance,1);
		pFile->ReadInitInfo(strAppPath+"set.ini","Fresh","max",strTemp,g_iMaxBalance,1);
		pFile->CheckInitValue(g_iSetNum,0,5);
		pFile->CheckInitValue(g_iRoomNum,1,60);
		pFile->CheckInitValue(g_iLoseNum,1,60000);
		pFile->CheckInitValue(g_iWinOrLose,0,1);
		pFile->CheckInitValue(g_iMinBalance,10,3000);
		pFile->CheckInitValue(g_iMaxBalance,10000,100000000);
		pFile->ReadInitInfo(strAppPath+"set.ini","Fresh","checkgm",strTemp,g_iIsCheckGm,1);
		pFile->CheckInitValue(g_iIsCheckGm,0,1);
		
		pFile->ReadInitInfo(strAppPath +"set.ini","Fresh","hidewindow",strTemp,g_iIsHidePlayWindow,1);
		pFile->CheckInitValue(g_iIsHidePlayWindow,0,1);
		m_HideWindow.SetCheck(g_iIsHidePlayWindow);
		pFile->ReadInitInfo(strAppPath +"set.ini","Fresh","speed",strTemp,g_iSpeed,1);
		pFile->CheckInitValue(g_iSpeed,1,100);
		int iValueEx = 100 - g_iSpeed;
		pFile->CheckInitValue(iValueEx,1,100);
		m_SpeedCtrl.SetPos(iValueEx);

		pFile->ReadInitInfo(strAppPath +"set.ini","Fresh","stopminutes",strTemp,g_iStopMinutes,1);
		pFile->CheckInitValue(g_iStopMinutes,1,100);
		SetDlgItemInt(IDC_EDIT_STOPMINUTES,g_iStopMinutes);

		pFile->ReadInitInfo(strAppPath +"set.ini","Fresh","checkgmwait",strTemp,g_iCheckGmWait,1);
		pFile->ReadInitInfo(strAppPath +"set.ini","Fresh","checkinterval",strTemp,g_iCheckGmWaitInterval,1);
		pFile->ReadInitInfo(strAppPath+"set.ini","Fresh","gmstr",g_strGmStr,iTemp,0);
		pFile->CheckInitValue(g_iCheckGmWait,0,1);
		//pFile->CheckInitValue(g_iCheckGmWaitInterval,2,100);
		SetDlgItemInt(IDC_EDIT_CHECKINTERVAL,g_iCheckGmWaitInterval);
		SetDlgItemText(IDC_EDIT_GMSTR,g_strGmStr);
		m_CheckGM.SetCheck(g_iIsCheckGm);
		m_gmCheckWait.SetCheck(g_iCheckGmWait);
		OnBnClickedCheckGmcheck();
		pFile->WriteInitInfo(strAppPath +"set.ini","Fresh","setpwd",strTemp,g_iIsSetPwd,1);
		pFile->CheckInitValue(g_iIsSetPwd,0,1);
		((CButton *)GetDlgItem(IDC_CHECK_SETPWD))->SetCheck(g_iIsSetPwd);


		pFile->ReadInitInfo(strAppPath+"set.ini","Fresh","dbname",g_strDbpath,iTemp,0);
		pFile->ReadInitInfo(strAppPath+"set.ini","Fresh","dbpwd",g_strdbPwd,iTemp,0);
		pFile->ReadInitInfo(strAppPath+"set.ini","Fresh","windowname",g_strWindowName,iTemp,0);
		pFile->ReadInitInfo(strAppPath+"set.ini","Fresh","windowclass",g_strWindowClass,iTemp,0);

		SetDlgItemText(IDC_EDIT_DBNAME,g_strDbpath);
		SetDlgItemText(IDC_EDIT_DBPWD,g_strdbPwd);

		SetDlgItemText(IDC_EDIT_WINDOWNAME,g_strWindowName);
		SetDlgItemText(IDC_EDIT_WINDOWCLASS,g_strWindowClass);		
	}
	SetDlgItemInt(IDC_EDIT_MINNUM,g_iMinBalance);
	SetDlgItemInt(IDC_EDIT_MAXNUM,g_iMaxBalance);
	SetDlgItemText(IDC_EDIT_ROOMPWD,g_strRoomPwd);
	SetDlgItemText(IDC_EDIT_EXEPATH,g_strAppPath);
	SetDlgItemInt(IDC_EDIT_DESKNUM,g_iRoomNum);
	SetDlgItemInt(IDC_EDIT_SETNUM,g_iSetNum+1);
	SetDlgItemInt(IDC_EDIT_LOSENUM,g_iLoseNum);
#ifdef ONLY_LOSER
	GetDlgItem(IDC_RADIO_WIN)->ShowWindow(SW_HIDE);
	g_iWinOrLose = 0;
#else
#ifdef ONLY_WINNNER
	GetDlgItem(IDC_RADIO_LOSE)->ShowWindow(SW_HIDE);
	g_iWinOrLose = 1;
#endif
#endif
	if (g_iWinOrLose == 1)
	{
		OnBnClickedRadioWin();
		SetWindowText("���ϵ�����");
	}
	else
	{
		OnBnClickedRadioLose();
		SetWindowText("���ϵ�����");
	}
	SetHook();
	SetWindowText("���ϵ�����");
	m_iCurrTime = -100000;
	return TRUE;  // ���������˿ؼ��Ľ��㣬���򷵻� TRUE
}

void CLoserWindowDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CLoserWindowDlg::OnPaint() 
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
HCURSOR CLoserWindowDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}
/************************************************************************/
/* ��ʼˢ��                                                             */
/************************************************************************/
void CLoserWindowDlg::OnBnClickedBtnStart()
{
	if (g_ExitFlag == 0)
	{
		ShowMessage("�߳��Ѿ���������");
		return;
	}
	if (SaveParams()<0)
	{
		return;
	}
	g_ExitFlag = 0;
	AfxBeginThread((AFX_THREADPROC)ThreadFunc,(LPVOID)this);
 	GetDlgItem(IDC_BTN_START)->EnableWindow(FALSE);
 	GetDlgItem(IDC_BTN_STOP)->EnableWindow(TRUE);
}
/************************************************************************/
/*  ˢ���߳�                                                            */
/************************************************************************/
void CLoserWindowDlg::ThreadFunc(LPVOID lParam)
{
	CLoserWindowDlg *pParam = ((CLoserWindowDlg *)lParam);
	if ( pParam )
	{
		pParam->RunFunc();
		pParam->ShowMessage("�߳��˳�");
		g_ExitFlag = 1;
 		pParam->GetDlgItem(IDC_BTN_START)->EnableWindow(TRUE);
 		pParam->GetDlgItem(IDC_BTN_STOP)->EnableWindow(FALSE);
	}
}
/************************************************************************/
/*  ˢ���̺߳���                                                        */
/************************************************************************/
void CLoserWindowDlg::RunFunc()
{
	CString sendStr = "";
	ShowMessage("�߳�����");
	int iLoginError = 0;
	int iRet = -1;
	CWindowFind find;
	CString strTemp = "";	
	ShowMessage("��ʼ���ɹ�,��ʼ������ݵ�Ŀ�������");
	while (!g_ExitFlag)
	{	
		if (g_iWinOrLose = 1)
		{
			if (GetWindowCtrl() < 0)
			{		
				//ShowMessage("���Ҵ���ʧ��");	
				Sleep(1000);
				continue;
			}
		}		
		RegisterPack rgPack;		//��ȡ��һ������˺�
		iRet = GetRegisterPack(rgPack);
		if (iRet != 0)
		{
			ShowMessage("û����Ϣ����,�뵼��");	//����
			Sleep(1000);
			continue;
		}
		InsertData(rgPack.strAcc,rgPack.strBalance,rgPack.strPwd);
		g_RegisterList.RemoveAt(0);
		AddFreshResult(rgPack,0);		
		strTemp.Format("���  %s  �����",rgPack.strAcc);
		ShowMessage(strTemp);
		strTemp.Format("�ȴ�%d��",g_iCheckGmWaitInterval);
		ShowMessage(strTemp);
		Sleep(g_iCheckGmWaitInterval*1000);
	}
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
//��ȡ��������,����0��ʾ����,-1��ʾ������,-2��ʾ����
int CLoserWindowDlg::GetRegisterPack(RegisterPack &submitPack)
{
	int iRet = 0;
	try
	{
		int iSize = g_RegisterList.GetSize();
		if (iSize == 0)	//������������
		{
			iRet = -1;
		}
		else
		{
			submitPack = g_RegisterList.GetAt( 0 );			
			iRet = 0;	//������ȡ������
		}		
	}
	catch(...)
	{
		iRet = -2;		//����
	}
	return iRet;
}
/************************************************************************/
/*  ��¼                                                                */
/************************************************************************/
int CLoserWindowDlg::Login(CString strUser,CString strPwd)
{
	//��¼����
	HWND hwnd = NULL;
	HWND hwndChind = NULL;
	CWnd *pWnd = NULL;
	CWnd *pParantWnd = NULL;
	CWindowFind find;
	CString strTemp  = "";
	TCHAR   szClassName[256]={0};
	CString strShow = "";
	g_hwndTopMost = NULL;
	g_hwndTopMost = ::FindWindow("7PMIGameLobby","��Ϸ����");
	if (!g_hwndTopMost)
	{
		//��������
		HINSTANCE iRet = ShellExecute(NULL, "open", g_strAppPath, NULL, NULL, SW_SHOW);
		if ((int)iRet<32)
		{
			ShowMessage("������Ϸʧ��,��������");
			return -3;
		}
		ShowMessage("��������ɹ�");
		Sleep(1500);
		return 2;		//����2��ʾ�ظ���¼
	}
	BackToLoginStatus();
	hwnd = find.MyFindWindow("Static","��Ϸ�ʺţ�");
	if (hwnd)	//��¼����Ӵ���
	{
		hwnd = ::GetParent(hwnd);
		if (hwnd)
		{
			//hwnd = pParantWnd->GetSafeHwnd();
			//���˺ſ�
			hwndChind = find.GetChildWindow(hwnd,6);	//�˺�ѡ���
			Sleep(40);
			hwndChind = find.GetChildWindow(hwndChind,1);//�˺�����༭��
			Sleep(40);
			find.SetEditText(hwndChind,strUser);
			
			//�����
			hwndChind = find.GetChildWindow(hwnd,7);
			Sleep(40);
			find.SetEditText(hwndChind,strPwd);
			//��¼��ť
			hwndChind = find.GetChildWindow(hwnd,9);
			Sleep(40);
			find.PostBtnClick(hwndChind);
 			Sleep(1000);//�ȴ�1��,���Ƿ����쳣��Ϣ��
			hwnd = find.MyFindWindow("#32770","��ʾ��Ϣ");
			hwndChind = find.GetChildWindow(hwnd,4);
			if (hwndChind)
			{
				if (find.GetContrlWindowText(hwndChind) == "�����ʺŲ����ڻ������������������֤���ٴγ��Ե�¼��")
				{
					if (::IsWindowVisible(hwndChind) && ::IsWindowVisible(hwnd))
					{
						ShowMessage("�˺Ż����������!");
						::PostMessage(hwnd,WM_CLOSE,0,0);
						return 5;
					}
				}
			}//�������б�,����ȡֵ,���Ƿ�����Ϣ
			hwnd = find.MyFindWindow("Static","��Ϸ�ʺţ�");
			if (!hwnd)
			{
				return 0;//��¼�ɹ�,ִ����һ��
			}
			return 2;
		}
		else
		{
			ShowMessage("�����Ѿ���������");
			Sleep(200);
			return 3;
		}
	}
	else
	{
		//�ж��������Ƿ��Ѿ�����		
		ShowMessage("�����Ѿ���������");
		//�л��˺�
		hwnd = ::FindWindow("#32770","��ʾ��Ϣ");
		if (!hwnd)
		{
			::PostMessage(g_hwndTopMost,WM_CLOSE,0,0);
			Sleep(400);
			return 2;
		}
		hwndChind = find.GetChildWindow(hwnd,3);
		if (hwndChind)
		{
			CString strWinName = find.GetContrlWindowText(hwndChind);
			if ( strWinName.Find("�л�") != -1)
			{
				find.PostBtnClick(hwndChind);
				ShowMessage("�л��˺�");
				Sleep(200);
			}						
		}//�������б�,����ȡֵ,���Ƿ�����Ϣ			
		return 1;
	}
	return 0;
}
/************************************************************************/
/* ��ʾ��Ϣ                                                             */
/************************************************************************/
void CLoserWindowDlg::ShowMessage(CString strMsg)
{
	CUserMessagePump *pMsgPump = NULL;
	pMsgPump = CUserMessagePump::GetInstance();
	if (pMsgPump)
	{
		pMsgPump->AddToMessagePump(USERWINDOWMSG_1,strMsg);
	}
}
void CLoserWindowDlg::OnBnClickedButton2()
{
	Login("","");
	//�жϵ�¼���Ŀ¼��
	TryIntoRoom();
}
/************************************************************************/
/* ����Ϣ����ճ����                                                     */
/************************************************************************/
bool CLoserWindowDlg::CopyStringToClipboard(HWND hWnd, LPCTSTR lpszText)
{
	EmptyClipboard();
	HGLOBAL hglbCopy;
	LPTSTR  lptstrCopy;
	int nlen = strlen(lpszText);
	if (nlen == 0)
	{
		return FALSE;
	}
	if (!::OpenClipboard(hWnd))
		return FALSE;

	hglbCopy = GlobalAlloc(GMEM_DDESHARE, 
		(nlen + 1) * sizeof(char)); 

	if (hglbCopy == NULL) 
	{ 
		CloseClipboard(); 
		return FALSE; 
	}
	lptstrCopy = (LPTSTR)GlobalLock(hglbCopy); 
	memcpy(lptstrCopy, lpszText, nlen);
	lptstrCopy[nlen] = (TCHAR)0;  //null character
	GlobalUnlock(lptstrCopy);
	SetClipboardData(CF_TEXT, hglbCopy);
	CloseClipboard();
	return TRUE;
}

/************************************************************************/
/*  ������Ϸ����                                                        */
/************************************************************************/
int CLoserWindowDlg::TryIntoRoom()
{
	HWND hwnd = NULL;
	HWND hwndChind = NULL;
	CWnd *pWnd = NULL;
	CWnd *pParantWnd = NULL;
	CWindowFind find;
	CString strTemp  = "";
	TCHAR   szClassName[256]={0};
	CString strShow = "";
	HWND hParant = ::FindWindow("7PMIGameLobby","��Ϸ����");
	g_hwndTopMost = hParant;
	if (hParant)
	{
		hwnd = find.GetChildWindow(hParant,5);//"SysTreeView32"
		hwnd = find.GetChildWindow(hwnd,1);
		hwnd = find.GetChildWindow(hwnd,1);
		hwnd = find.GetChildWindow(hwnd,1);
		if (hwnd && find.GetContrlClassName(hwnd) =="SysTreeView32")	//��¼����Ӵ���
		{
			HTREEITEM hItem = TreeView_GetRoot(hwnd);
			HTREEITEM hChild = FindItem(hwnd,hItem,"����");
			if (!hChild)
			{
				ShowMessage("û�ҵ����𻨷���");
				return -1;
			}
			hChild = FindItem(hwnd,hChild,"��ͨ����1");
			if (!hChild)
			{
				ShowMessage("û�ҵ���ͨ����1");
				return -1;
			}
			TreeView_EnsureVisible(hwnd,hChild);
			TreeView_Select(hwnd,hChild,TVGN_CARET);
			CRect rect;
			GetTreeItemRect(hwnd,hChild,rect);
			::SetCursorPos(rect.left+120,rect.top+210);
			::SendMessage(hwnd,WM_LBUTTONDBLCLK,0,0);

			//::SendMessage(hwnd,WM_NCHITTEST,0,MAKELONG(rect.left+120,rect.top+210));
			//::SendMessage(hwnd,WM_LBUTTONDBLCLK,0,MAKELONG(rect.left+120,rect.top+210));			
			//::SendMessage(hwnd,WM_LBUTTONDBLCLK,0,MAKELONG(rect.left+140,273));
			//DblClickItemNodeEx(hwnd);
		}
		else
		{
			Sleep(500);
			return -1;
		}
	}
	else
	{
		ShowMessage("����δ����");
		g_strOldAcc = "";
		return -2;
	}
	return 0;
}
/************************************************************************/
/* ������Ϸ��                                                           */
/************************************************************************/
int CLoserWindowDlg::TryIntoDesk(int iIndex,int iSetIndex)
{
	CWindowFind find;
	CPoint pnt = DestIndexToPointEx(iIndex,iSetIndex);			//һ������
	CString strShow = "";
	pnt.x -= 9;
	pnt.y-=131;
	HWND tempHWND = g_hwndDesk;
	TCHAR   szClassName[256];     
	GetClassName(tempHWND,   szClassName,256);
	ShowMessage(szClassName);
	if ("AfxWnd70" == CString(szClassName))
	{//�ҵ�����
		MoveScrollBar(iIndex);
		::SendMessage(tempHWND, WM_LBUTTONDOWN, 0,MAKELONG(pnt.x,pnt.y));//����˫����Ϣ
		::SendMessage(tempHWND, WM_LBUTTONUP, 0,MAKELONG(pnt.x,pnt.y));//����˫����Ϣ
		//������Ϸ�����Ƿ��Ѿ�����
		Sleep(500);
		int iCount = 0;
		CString strTemp = "";
		g_hwndPlayWindow = NULL;
		while (!g_hwndPlayWindow)
		{			
			strTemp.Format("������Ϸ�ͻ��� �� ��ͨ����1 - �� %d �� ��",iIndex);
			g_hwndPlayWindow = ::FindWindow("#32770",strTemp);
			if (g_hwndPlayWindow)	//�ҵ�������Ϸ�Ĵ���	��������С,�ƶ�,���һ�ȡ���е�С��ť
			{
				::SetWindowPos(g_hwndPlayWindow,HWND_TOP,0,0,0,0,SWP_NOSIZE|SWP_SHOWWINDOW);
				::MoveWindow(g_hwndPlayWindow,20,120,500,400,TRUE);
// 				if (g_iIsHidePlayWindow)
// 				{
// 					::ShowWindow(g_hwndPlayWindow,SW_HIDE);
// 				}
				ShowMessage("�ҵ����𻨷���");
				return 0;
			}
			iCount++;
			if (iCount > 200)
			{
				ShowMessage("δ�ҵ��ҵ����𻨷���,��ȷ�������õ����ź���λ����ȷ");
				return -1;
			}
			Sleep(50);
		}
	}
	else
	{
		ShowMessage("δ�ҵ�����,�뱣֤��Ϸ������ʾ����!");
		return -1;
	}
	return 0;
}
/************************************************************************/
/* ��ʼˢ��                                                             */
/************************************************************************/
int CLoserWindowDlg::PlayGame()
{
	HWND hwnd = NULL;
	HWND hwnd100 = NULL;
	HWND hwnd200 = NULL;
	HWND hwnd400 = NULL;
	HWND hwnd800 = NULL;
	HWND hwndGiveUp = NULL;
	HWND hwndShow = NULL;
	HWND hwndStart = NULL;
	HWND hwndGen= NULL;
	HWND hwndKaipai = NULL;
	CWindowFind find;
	CString strTemp = "";
	int iCount = 0;
	hwnd = find.GetChildWindow(g_hwndPlayWindow,4);//find.MyFindWindow("AfxWnd70","",hParant);
	if (hwnd)
	{
		SetLastWaitCheckTime();
		//1������,����
		hwnd800 = find.GetChildWindow(hwnd,6);		//����ע
		hwnd400 = find.GetChildWindow(hwnd,7);
		hwnd200 = find.GetChildWindow(hwnd,8);
		hwnd100 = find.GetChildWindow(hwnd,9);
		hwndGiveUp = find.GetChildWindow(hwnd,4);	//����
		hwndShow = find.GetChildWindow(hwnd,2);		//����
		hwndStart = find.GetChildWindow(hwnd,10);	//��ʼ��Ϸ
		hwndGen = find.GetChildWindow(hwnd,5);		//��ע
		hwndKaipai = find.GetChildWindow(hwnd,3);	//����
		int iTimeJuShu = 1;
		int iAddCount = 1;
		while(true)
		{
			if (g_ExitFlag)
			{
				strTemp.Format("��������˳�,�����˳�...");
				ShowMessage(strTemp);
				return 2;
			}
			strTemp.Format("������Ϸ�ͻ��� �� ��ͨ����1 - �� %d �� ��",g_iRoomNum);
			HWND hParant = ::FindWindow("#32770",strTemp);
			if (!hParant)
			{
				strTemp.Format("��Ϸ�����Ѿ��ر�,�˳�...");
				ShowMessage(strTemp);
				return 3;
			}			
			//�жϿ�ʼ��ť�Ƿ����
			if(::IsWindowVisible(hwndStart))//::IsWindowEnabled(hwndStart)		//һ�ֿ�ʼ��ǰ,��ʱ��ĳ��Ƿ����
			{
				SetLastWaitCheckTime();
				//���GM				
				if (IsGmHere(g_hwndListView1))
				{
					ShowMessage("GM����,�˳�...");
					return 99;
				}
				//��ȡ���
				int iBalance = GetBalance(g_hwndListView1);
				g_strBalance.Format("%d",iBalance);
				if (iBalance < g_iMinBalance)	//���С���趨��,�˳�
				{
					strTemp.Format("����Ѿ�С������ֵ:%d,ˢ�����,����...",g_iMinBalance);
					ShowMessage(strTemp);					
					return 0;
				}
				if (g_iWinOrLose==1 && iBalance > g_iMaxBalance)//�����Ӯ��������������趨��,�˳�
				{
					strTemp.Format("����Ѿ���������ֵ:%d,ˢ�����,����...",g_iMaxBalance);
					ShowMessage(strTemp);
					return 0;
				}
				find.PostBtnClick(hwndStart);
				strTemp.Format("��ʼ,��%d��,���:%d",iTimeJuShu,iBalance);
				ShowMessage(strTemp);
				iTimeJuShu ++;
				Sleep(400);
			}
			if (::IsWindowEnabled(hwnd800))
			{
				if (g_iWinOrLose == 0)		//��������
				{
					if (iAddCount > 4)		//����Ѿ���ע�Ĵ�
					{
						ShowMessage("����Ѽ�ע4��,����...");
						if (::IsWindowEnabled(hwndGiveUp))
						{
							find.PostBtnClick(hwndGiveUp);
							ShowMessage("����");
							iAddCount = 1;
							Sleep(20*g_iSpeed);
							continue;
						}
					}
					find.PostBtnClick(hwnd800);
					strTemp.Format("��%d�μ�ע,��ע�� %s...",iAddCount,find.GetContrlWindowText(hwnd800));
					ShowMessage(strTemp);
					iAddCount ++;
				}
				else			//Ӯ��
				{
					find.PostBtnClick(hwnd800);
					strTemp.Format("��ע %s...",find.GetContrlWindowText(hwnd800));
					ShowMessage(strTemp);
				}
				Sleep(20*g_iSpeed);
			}
			else		//�˴�������Ӯ��ͬ������
			{
				BOOL bRet = FALSE;
				if (g_iWinOrLose == 1)//Ӯ������
				{
					bRet = ::IsWindowEnabled(hwndGen);
					if (bRet)
					{
						find.PostBtnClick(hwndGen);	
						ShowMessage("��ע");
					}					
				}
				else	//��
				{
					bRet = ::IsWindowEnabled(hwndGiveUp);					
					if (bRet)
					{
						find.PostBtnClick(hwndGiveUp);
						ShowMessage("����");
						iAddCount = 1;
					}
				}
				if (bRet)
				{
					Sleep(20*g_iSpeed);
				}
				else
				{
					if(IsOverCheckTime(g_iCheckGmWaitInterval))
					{
						ShowMessage("�ȴ�������ѹ�,���GM...");
						SetLastWaitCheckTime();
						if (IsGmHere(g_hwndListView1))
						{
							ShowMessage("GM����,�˳�...");
							return 99;
						}
					}
					ShowMessage("�ȴ���...");
					Sleep(150);
				}
			}
		}
	}
	else
	{
		ShowMessage("δ�ҵ�������ť,�˳�");
	}
	return 0;
}
/************************************************************************/
/*�������뺯��                                                          */
/************************************************************************/
int CLoserWindowDlg::SetPassWord(CString strPwd)
{
	CWindowFind find;
	find.PostBtnClick(g_hwndSet);		//������ð�ť
	Sleep(200);
	//�ж��Ƿ��ҵ����ð�ť
	HWND hwnd = NULL;
	HWND hParant  = NULL;
	HWND hChild = NULL;
	int iCount = 0;
	while (!hParant)
	{
		hParant = ::FindWindow("#32770","����ѡ��");
		if (hParant)		//�Ѿ��ҵ�������,�������ÿ�,������������
		{
			ShowMessage("�ҵ��������ÿ�");
			hwnd = find.GetChildWindow(hParant,4);		//���������
			hChild = find.GetChildWindow(hwnd,12);
			if (hChild)
			{
				((CButton*)FromHandle(hChild))->SetCheck(TRUE);
			}
			hChild = find.GetChildWindow(hwnd,10);
			if (hChild)
			{
				((CButton*)FromHandle(hChild))->SetCheck(FALSE);
			}
			hChild = find.GetChildWindow(hwnd,11);
			if (hChild)
			{
				((CButton*)FromHandle(hChild))->SetCheck(FALSE);
			}			
			hChild = find.GetChildWindow(hwnd,13);
			find.SetEditText(hChild,g_strRoomPwd);
			hwnd = find.MyFindWindow("Button","����Я�����룺");
			if (hwnd)
			{
				((CButton*)FromHandle(hwnd))->SetCheck(TRUE);
			}			
			hwnd = find.GetChildWindow(hParant,1);
			find.PostBtnClick(hwnd);
			ShowMessage("�����Ѿ�����");
			return 0;
		}
		ShowMessage("δ�ҵ��������ÿ�...");
		iCount++;
		if (iCount > 3)
		{
			ShowMessage("δ�ҵ��������ÿ�,�뱣֤��Ϸ������ʾ����!");
			return -1;
		}
		Sleep(100);
	}
	return 0;
}

/************************************************************************/
/* ��ȡ�ⲿ�����������������                                           */
/************************************************************************/
bool  CLoserWindowDlg::GetTreeItemRect(HWND hwnd, HTREEITEM TreeItem, RECT& Rect)
{
	bool result = false;
	DWORD processId = 0;
	GetWindowThreadProcessId(hwnd, &processId);
	HANDLE process = OpenProcess(PROCESS_VM_OPERATION | PROCESS_VM_READ | PROCESS_VM_WRITE, FALSE, processId);
	if (process != NULL)
	{
		LPVOID buffer = VirtualAllocEx(process, NULL, 4096, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
		if (buffer != NULL)
		{
			SIZE_T bytes;
			WriteProcessMemory(process, buffer, &TreeItem, sizeof(TreeItem), &bytes);
			result = (::SendMessage(hwnd, TVM_GETITEMRECT, FALSE, (LPARAM)buffer) != 0);
			ReadProcessMemory(process, buffer, &Rect, sizeof(Rect), &bytes);
			VirtualFreeEx(process, buffer, 0, MEM_RELEASE);
		}
		CloseHandle(process);
	}
	return result;
}
/************************************************************************/
/*  ��ȡ�ⲿ�������Ľ��������Ϣ                                        */
/************************************************************************/
CString CLoserWindowDlg::GetItemNodeText(HWND hwnd,HTREEITEM hItem)
{
	CString strText = "";
	DWORD PID;
	HANDLE hProcess;
	const int bufferLength = 512;
	LPTVITEM lpItem = (LPTVITEM)new BYTE[bufferLength];
	GetWindowThreadProcessId(hwnd, &PID);
	hProcess=OpenProcess(PROCESS_VM_OPERATION | PROCESS_VM_READ | PROCESS_VM_WRITE | PROCESS_QUERY_INFORMATION, TRUE, PID);
	PVOID buffer = VirtualAllocEx(hProcess,NULL,bufferLength,MEM_COMMIT,PAGE_READWRITE);
	lpItem->cchTextMax=512;
	lpItem->mask = TVIF_TEXT;
	lpItem->hItem = hItem;
	lpItem->pszText = LPTSTR((PBYTE)buffer+sizeof(TVITEM));
	::SendMessage(hwnd,TVM_SELECTITEM,TVGN_CARET,(long)lpItem->hItem); 
	WriteProcessMemory(hProcess,buffer,lpItem,sizeof(TVITEM),NULL); 
	::SendMessage(hwnd,TVM_GETITEM,0,(LPARAM)buffer); 
	ReadProcessMemory(hProcess,buffer,lpItem,bufferLength,NULL); 
	strText = (LPTSTR)((PBYTE)lpItem+sizeof(TVITEM));
	CloseHandle(hProcess);
	VirtualFreeEx(hProcess, lpItem, 0, MEM_RELEASE);
	delete[] lpItem;
	lpItem = NULL;
	return strText;
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
int CLoserWindowDlg::DblClickItemNode(HWND hwnd,HTREEITEM hItem)
{
	CString strText = "";
	DWORD PID;
	HANDLE hProcess;
	HWND hParant = ::GetParent(hwnd);
	GetWindowThreadProcessId(hParant, &PID);
	hProcess=OpenProcess(PROCESS_ALL_ACCESS, FALSE, PID);
	NMHDR *pnmh=(NMHDR*)VirtualAllocEx(hProcess, NULL, sizeof(NMHDR), MEM_COMMIT, PAGE_READWRITE); 
	NMHDR nmh; 
	nmh.code = NM_DBLCLK;    // Message type defined by control. 
	nmh.idFrom = ::GetDlgCtrlID(hwnd); 
	nmh.hwndFrom = hwnd; 
	WriteProcessMemory(hProcess, pnmh, &nmh, sizeof(NMHDR), NULL); 
	::SendMessage(hParant,WM_NOTIFY,(WPARAM)nmh.idFrom,(LPARAM)pnmh); 
	CloseHandle(hProcess);
	VirtualFreeEx(hProcess, pnmh, 0, MEM_RELEASE);
	return 0;
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CLoserWindowDlg::OnBnClickedButton1()
{
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CLoserWindowDlg::OnBnClickedButton3()
{
	SetTimer(1,1000,NULL);
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
//д��־
void CLoserWindowDlg::WriteLog(CString srcStr,int iFlag)
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
			filename.Format("%s_Complete.txt",currTime.Format("%Y%m%d"));
		}
		else if ( iFlag == 2 )	//������־
		{
			folder = "log";
			filename.Format("%s_log.txt",currTime.Format("%Y%m%d"));
		}
		else			//�쳣��־ (3)
		{
			folder = "exception";
			filename.Format("%s_exception.txt",currTime.Format("%Y%m%d"));
		}

		CUserFile *pFile = CUserFile::GetInstance();
		if ( pFile )
		{
			pFile->GetAppPath(strAppPath);
		}
		CString folderName = "";
		folderName.Format("%s%s",strAppPath,folder);
		if(_access(folderName,0)!=0)
		{
			if( mkdir(folderName) != 0 )//����Ŀ¼
			{
				return;
			}
		}					
		destStr.Format("%s	%s",srcStr,currTime.Format("%Y-%m-%d %H:%M:%S"));
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
void CLoserWindowDlg::OnBnClickedButton4()
{
	SetPassWord("123445");
}
/************************************************************************/
/*  �������ż�������                                                    */
/************************************************************************/
CPoint CLoserWindowDlg::DestIndexToPoint(int iIndex)
{
	int iFirstX = 624;
	int iFirstY = 686;
	int iOffSetY = 175;
	int iOffSetX = 175;
    int iDestX = 0;
	int iDestY = 0;
	int iRet= 0;
	int iTemp = 0;
	iTemp = 61-iIndex;
	iDestX = iFirstX - ((iTemp-1) % 4) * iOffSetX;
	//iIndex�仯
	iTemp = iIndex % 12;
	if (iTemp == 9 ||iTemp == 10|| iTemp == 11 ||iTemp== 0)
	{
		iDestY = iFirstY;
	}
	else if (iTemp>=5 && iTemp <=8)
	{
		iDestY = iFirstY - iOffSetY * 1;
	}
	else
	{
		iDestY = iFirstY - iOffSetY * 2;
	}	
	return CPoint(iDestX,iDestY);
}
/************************************************************************/
/*  �������ż�������                                                    */
/************************************************************************/
CPoint CLoserWindowDlg::DestIndexToPointEx(int iIndex,int iSetIndex)
{
	CPoint pt = DestIndexToPoint(iIndex);
	int iOffSetX = 0;
	int iOffSetY = 0;
	switch (iSetIndex)
	{
	case 0:
		iOffSetX = 0;
		iOffSetY = 0;
		break;
	case 1:
		iOffSetX = -63;
		iOffSetY = -28;
		break;
	case 2:
		iOffSetX = -63;
		iOffSetY = -90;
		break;
	case 3:
		iOffSetX = 0;
		iOffSetY = -110;
		break;
	case 4:
		iOffSetX = 63;
		iOffSetY = -90;
		break;
	case 5:
		iOffSetX = 63;
		iOffSetY = -28;
		break;
	}
	pt.x += iOffSetX;
	pt.y += iOffSetY;
	return pt;
}
/************************************************************************/
/* �ƶ���Ϸ���������                                                   */
/************************************************************************/
void CLoserWindowDlg::MoveScrollBar(int iIndex)
{
	int iValue = ComputeScrollUpValue(iIndex);
	CWindowFind find;
	CPoint pnt = CPoint(102,353);
	HWND tempHWND = g_hwndDesk ;    //ȡ�����ָ�봦���ھ��
	TCHAR   szClassName[256];     
	TCHAR   szWindowText[256];
	GetClassName(tempHWND,   szClassName,256);     
	::GetWindowText(tempHWND,(LPTSTR)&szWindowText,256);
	if ("AfxWnd70" == CString(szClassName))
	{
		//�ƶ����ײ�
		switch(iValue)
		{
		case 0:
			::SendMessage(tempHWND,WM_VSCROLL,SB_BOTTOM,1);
			break;
		case 1:
			::SendMessage(tempHWND,WM_VSCROLL,MAKELONG(SB_THUMBTRACK,1725),1);
			break;
		case 2:
			::SendMessage(tempHWND,WM_VSCROLL,MAKELONG(SB_THUMBTRACK,1195),1);
			break;
		case 3:
			::SendMessage(tempHWND,WM_VSCROLL,MAKELONG(SB_THUMBTRACK,665),1);
			break;
		case 4:
			::SendMessage(tempHWND,WM_VSCROLL,MAKELONG(SB_THUMBTRACK,135),1);
			break;
		}
	}
}
/************************************************************************/
/*��������������ƶ���ƫ��                                              */
/************************************************************************/
int CLoserWindowDlg::ComputeScrollUpValue(int iIndex)
{
	int iRet= 0;
	if (iIndex <=60 && iIndex >= 49)
	{
		iRet = 0;
	}
	else if (iIndex <=48 && iIndex >= 37)
	{
		iRet = 1;
	}
	else if (iIndex <=36 && iIndex >= 25)
	{
		iRet = 2;
	}
	else if (iIndex <=24 && iIndex >= 13)
	{
		iRet = 3;
	}
	else
	{
		iRet = 4;
	}
	return iRet;
}
void CLoserWindowDlg::OnBnClickedButton5()
{
	RunFunc();
}

void CLoserWindowDlg::OnBnClickedButton6()
{
	MoveScrollBar(1);
}

void CLoserWindowDlg::OnBnClickedButton7()
{
	int iValue = GetDlgItemInt(IDC_EDIT1);
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
void CLoserWindowDlg::SetTableStyle()
{
	CListCtrl* conList = &m_ListAcc;
	DWORD dwSytle=::GetWindowLong(conList->m_hWnd,GWL_STYLE);
	// ����Ϊ������ʽ
	SetWindowLong(conList->m_hWnd,GWL_STYLE,dwSytle|LVS_REPORT);
	DWORD ExStyle=conList->GetExtendedStyle();
	conList->SetExtendedStyle(ExStyle|LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
	CImageList image;
	image.Create(1,16, ILC_COLOR, 0, 0);
	conList->SetImageList(&image, LVSIL_STATE);
	conList->MoveWindow(7,35,450,200);
	conList->InsertColumn(0,"���",LVCFMT_CENTER,50);
	conList->InsertColumn(1,"�ļ���",LVCFMT_LEFT,150);
	conList->InsertColumn(2,"ͼƬ��ַ",LVCFMT_LEFT,150);
	conList->InsertColumn(3,"״̬",LVCFMT_LEFT,80);
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CLoserWindowDlg::InitListCtrl()
{
	DWORD dwSytle=::GetWindowLong(m_message.m_hWnd,GWL_STYLE);
	// ����Ϊ������ʽ
	SetWindowLong(m_message.m_hWnd,GWL_STYLE,dwSytle|LVS_REPORT);
	DWORD ExStyle=m_message.GetExtendedStyle();
	m_message.SetExtendedStyle(ExStyle|LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
	::SendMessage(m_message.m_hWnd, LVM_SETEXTENDEDLISTVIEWSTYLE,
		LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);
	m_message.MoveWindow(7,250,450,270);
	m_message.InsertColumn(0,_T("ʱ��"),LVCFMT_LEFT,125);
	m_message.InsertColumn(1,_T("��Ϣ"),LVCFMT_LEFT,280);
}
/************************************************************************/
/*  �ȼ�ֹͣ                                                            */
/************************************************************************/
LRESULT CLoserWindowDlg::StopThread(WPARAM wParam,LPARAM lParam)
{
	ShowMessage("�յ�ֹͣ�̵߳��ȼ���Ϣ...");
	OnBnClickedBtnStop();
	return 0;
}
/************************************************************************/
/* �����Ϣ��ʾ		                                                    */
/************************************************************************/
LRESULT CLoserWindowDlg::AddToList(WPARAM wParam,LPARAM lParam)
{
	CUserMessagePump *pMsgPump = NULL;
	pMsgPump = CUserMessagePump::GetInstance();
	if (!pMsgPump)
	{
		return 0;
	}
	CString msgBuf;
	pMsgPump->GetPumpMessage(USERWINDOWMSG_1,m_hWnd,msgBuf);
	if ( msgBuf == "" )
	{
		return 0;
	}
	int nCount = m_message.GetItemCount();
	if ( nCount >= 40 )
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
		m_message.SetItemText( nCount-1 , 0, CTime::GetCurrentTime().Format("%Y-%m-%d %H:%M:%S") );
		::PostMessage(m_message.m_hWnd,WM_VSCROLL,SB_BOTTOM,0);
		return 0;
	}
	m_message.InsertItem( nCount, "");
	if ( nCount % 2 == 0 )
	{
		m_message.SetRowSelectedBgColor(nCount, RGB(0,0,0) , false);
		m_message.SetRowTxtColor(nCount , RGB(255,0,0) , false);
	}
	else
	{
		m_message.SetRowTxtColor(nCount , RGB(0,0,255) , false);
	}
	m_message.SetRowBgColor(nCount, RGB(230,190,220), false);
	m_message.SetRowStyle(nCount,  LIS_TXTCOLOR | LIS_BGCOLOR);

	m_message.SetItemText( nCount , 0, CTime::GetCurrentTime().Format("%Y-%m-%d %H:%M:%S") );
	m_message.SetItemText( nCount , 1, msgBuf );
	::PostMessage(m_message.m_hWnd,WM_VSCROLL,SB_BOTTOM,0);
	if (msgBuf != "�ȴ���...")
	{
		WriteLog(msgBuf,2);
	}
	return 0;
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CLoserWindowDlg::OnBnClickedBtnStop()
{
	g_ExitFlag = 1;
	//GetDlgItem(IDC_BTN_START)->EnableWindow(TRUE);
	//GetDlgItem(IDC_BTN_STOP)->EnableWindow(FALSE);
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CLoserWindowDlg::OnBnClickedBtnExit()
{
	PostMessage(WM_CLOSE,0,0);
}
/************************************************************************/
/* ����                                                                 */
/************************************************************************/
void CLoserWindowDlg::OnBnClickedBtnInport()
{
	CString strFolder = "";
	GetDlgItemText(IDC_EDIT_EXEPATH,strFolder);
	if (strFolder == "")
	{
		MessageBox("�ļ������ļ��в���Ϊ��","����");
		return;
	}
	GetDlgItemText(IDC_EDIT_GMSTR,g_strPhoneUrl);
	g_strPhoneUrl.Trim();
// 	if (g_strPhoneUrl == "")
// 	{
// 		MessageBox("ͼƬ���ӵ�ַ����Ϊ��","����");
// 		return;
// 	}
	CFileFind finder;
	CString findName = strFolder + "\\*.*";
	BOOL findFlag = finder.FindFile(findName);
	CStdioFile   file;
	CString   cstr;
	CFileException   fe;
	int iCount = 0;
	while (findFlag)
	{
		findFlag = finder.FindNextFile();
		if (finder.IsDots())
		{
			continue;
		}		
		CString sFileName = finder.GetFileName();
		CString strTile = sFileName.Left(sFileName.GetLength() - 5);
		strTile.Trim();
		if (strTile == "")
		{
			continue;
		}		
		if(file.Open(strFolder+"\\"+sFileName,CFile::modeRead,&fe))
		{
			file.SeekToBegin();
			file.ReadString(cstr);
			cstr.Replace("&nbsp;"," ");
			cstr.Replace("<br>","\n");
			cstr.Replace("'","''");
// 			CFile file1;
// 			file1.Open("c:\\test.txt",CFile::modeCreate|CFile::modeWrite);
// 			file1.Write(cstr,cstr.GetLength());
// 			file1.Close();
// 			break;
			AddToAccList(strTile,g_strPhoneUrl,cstr);
			file.Close();
		}
		iCount ++;
	}
	finder.Close();
	CString strShow = "";
	strShow.Format("��������%d���ļ�",iCount);
	ShowMessage(strShow);
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CLoserWindowDlg::AddToAccList(CString strAcc,CString strPwd,CString strContent)
{
	CListCtrl *pList = &m_ListAcc;
	int nCount = pList->GetItemCount();
	CString listIndex = "";

	for (int i=0;i<nCount;i++)
	{
		if (strAcc == pList->GetItemText(i,1))
		{
			CString strShow = "";
			strShow.Format("%s �ظ�����",strAcc);
			ShowMessage(strShow);
			return;
		}
	}

	listIndex.Format("%d",nCount);
	pList->InsertItem(nCount,"",0);
	pList->SetItemText(nCount,0,listIndex);
	pList->SetItemText(nCount,1,strAcc);
	pList->SetItemText(nCount,2,strPwd);
	RegisterPack rgPack;
	rgPack.strAcc = strAcc;
	rgPack.strPwd = strPwd;
	rgPack.strBalance = strContent;
	g_RegisterList.Add(rgPack);
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CLoserWindowDlg::OnBnClickedRadioLose()
{
	((CButton*)GetDlgItem(IDC_RADIO_LOSE))->SetCheck(TRUE);
	((CButton*)GetDlgItem(IDC_RADIO_WIN))->SetCheck(FALSE);
	GetDlgItem(IDC_EDIT_LOSENUM)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT_MINNUM)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT_MAXNUM)->EnableWindow(FALSE);
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CLoserWindowDlg::OnBnClickedRadioWin()
{
	((CButton*)GetDlgItem(IDC_RADIO_LOSE))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_RADIO_WIN))->SetCheck(TRUE);
	GetDlgItem(IDC_EDIT_LOSENUM)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_MINNUM)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_MAXNUM)->EnableWindow(TRUE);
}

/************************************************************************/
/* ����������Ϣ                                                         */
/************************************************************************/
int CLoserWindowDlg::SaveParams()
{
	if (!g_ExitFlag)
	{
		ShowMessage("�߳�������,���ܸ�������,����ֹͣ�߳�!");
	}
	CString strAppPath = "";
	CString strTemp = "";
	CUserFile *pFile = CUserFile::GetInstance();
	pFile->GetAppPath(strAppPath);
	GetDlgItemText(IDC_EDIT_ROOMPWD,g_strRoomPwd);
	GetDlgItemText(IDC_EDIT_EXEPATH,g_strAppPath);
	GetDlgItemText(IDC_EDIT_GMSTR,g_strGmStr);

	GetDlgItemText(IDC_EDIT_DBNAME,g_strDbpath);
	GetDlgItemText(IDC_EDIT_DBPWD,g_strdbPwd);
	
	GetDlgItemText(IDC_EDIT_WINDOWNAME,g_strWindowName);
	GetDlgItemText(IDC_EDIT_WINDOWCLASS,g_strWindowClass);
	g_strDbpath.Trim();
	g_strdbPwd.Trim();
	g_strWindowName.Trim();
	g_strWindowClass.Trim();
	if (g_strDbpath == "" || g_strWindowClass == "" || g_strWindowName == "")
	{
		MessageBox("���������Ϣ�������ļ���Ϣ����Ϊ��","����");
		return -1;
	}
	g_iRoomNum = GetDlgItemInt(IDC_EDIT_DESKNUM);
	g_iSetNum = GetDlgItemInt(IDC_EDIT_SETNUM)-1;
	g_iLoseNum = GetDlgItemInt(IDC_EDIT_LOSENUM);
	g_iMinBalance = GetDlgItemInt(IDC_EDIT_MINNUM);
	g_iMaxBalance = GetDlgItemInt(IDC_EDIT_MAXNUM);
	g_iStopMinutes = GetDlgItemInt(IDC_EDIT_STOPMINUTES);
	if (((CButton*)GetDlgItem(IDC_RADIO_LOSE))->GetCheck()==1)
	{
		g_iWinOrLose = 0;
	}
	else
	{
		g_iWinOrLose = 1;
	}
	g_strAppPath.Trim();
	g_strRoomPwd.Trim();
	g_strGmStr.Trim();
	if (g_strAppPath == "" || g_strRoomPwd == "")
	{
		MessageBox("������������������Ϣ","����");
		return -1;
	}
	
	pFile->CheckInitValue(g_iSetNum,0,5);
	pFile->CheckInitValue(g_iRoomNum,1,60);
	pFile->CheckInitValue(g_iLoseNum,1,60000);
	pFile->CheckInitValue(g_iWinOrLose,0,1);
	pFile->CheckInitValue(g_iMinBalance,10,3000);
	pFile->CheckInitValue(g_iMaxBalance,10000,100000000);
	pFile->CheckInitValue(g_iStopMinutes,1,100);
	SetDlgItemText(IDC_EDIT_ROOMPWD,g_strRoomPwd);
	SetDlgItemText(IDC_EDIT_EXEPATH,g_strAppPath);
	SetDlgItemInt(IDC_EDIT_DESKNUM,g_iRoomNum);
	SetDlgItemInt(IDC_EDIT_SETNUM,g_iSetNum+1);
	SetDlgItemInt(IDC_EDIT_LOSENUM,g_iLoseNum);
	SetDlgItemInt(IDC_EDIT_MINNUM,g_iMinBalance);
	SetDlgItemInt(IDC_EDIT_MAXNUM,g_iMaxBalance);
	g_iIsCheckGm = ((CButton *)GetDlgItem(IDC_CHECK_GMCHECK))->GetCheck();
	g_iIsHidePlayWindow = m_HideWindow.GetCheck();
	pFile->CheckInitValue(g_iIsCheckGm,0,1);
	
	g_iCheckGmWait = ((CButton *)GetDlgItem(IDC_CHECK_GMCHECKWAIT))->GetCheck();
	pFile->CheckInitValue(g_iCheckGmWait,0,1);
	g_iCheckGmWaitInterval = GetDlgItemInt(IDC_EDIT_CHECKINTERVAL);
	pFile->CheckInitValue(g_iCheckGmWaitInterval,0,100);

	g_iIsSetPwd = ((CButton *)GetDlgItem(IDC_CHECK_SETPWD))->GetCheck();
	if (pFile)
	{
		pFile->WriteInitInfo(strAppPath+"set.ini","Fresh","path",g_strAppPath,0,0);
		pFile->WriteInitInfo(strAppPath+"set.ini","Fresh","roompwd",g_strRoomPwd,0,0);		
		pFile->WriteInitInfo(strAppPath+"set.ini","Fresh","type","",g_iWinOrLose,1);
		pFile->WriteInitInfo(strAppPath+"set.ini","Fresh","losenum","",g_iLoseNum,1);
		pFile->WriteInitInfo(strAppPath+"set.ini","Fresh","desknum","",g_iRoomNum,1);
		pFile->WriteInitInfo(strAppPath+"set.ini","Fresh","setnum","",g_iSetNum,1);
		pFile->WriteInitInfo(strAppPath+"set.ini","Fresh","min","",g_iMinBalance,1);
		pFile->WriteInitInfo(strAppPath+"set.ini","Fresh","max","",g_iMaxBalance,1);
		pFile->WriteInitInfo(strAppPath+"set.ini","Fresh","checkgm","",g_iIsCheckGm,1);
		pFile->WriteInitInfo(strAppPath +"set.ini","Fresh","hidewindow","",g_iIsHidePlayWindow,1);
		pFile->WriteInitInfo(strAppPath +"set.ini","Fresh","stopminutes","",g_iStopMinutes,1);
		pFile->WriteInitInfo(strAppPath +"set.ini","Fresh","checkgmwait","",g_iCheckGmWait,1);
		pFile->WriteInitInfo(strAppPath +"set.ini","Fresh","checkinterval","",g_iCheckGmWaitInterval,1);
		pFile->WriteInitInfo(strAppPath+"set.ini","Fresh","gmstr",g_strGmStr,0,0);
		pFile->WriteInitInfo(strAppPath +"set.ini","Fresh","setpwd","",g_iIsSetPwd,1);

		pFile->WriteInitInfo(strAppPath+"set.ini","Fresh","dbname",g_strDbpath,0,0);
		pFile->WriteInitInfo(strAppPath+"set.ini","Fresh","dbpwd",g_strdbPwd,0,0);
		pFile->WriteInitInfo(strAppPath+"set.ini","Fresh","windowname",g_strWindowName,0,0);
		pFile->WriteInitInfo(strAppPath+"set.ini","Fresh","windowclass",g_strWindowClass,0,0);

	}
#ifdef ONLY_LOSER
	GetDlgItem(IDC_RADIO_WIN)->ShowWindow(SW_HIDE);
	g_iWinOrLose = 0;
#else
#ifdef ONLY_WINNNER
	GetDlgItem(IDC_RADIO_LOSE)->ShowWindow(SW_HIDE);
	g_iWinOrLose = 1;
#endif
#endif
	if (g_iWinOrLose == 0)
	{
		SetWindowText("��ҿͻ���");
	}
	else
	{
		SetWindowText("Ӯ�ҿͻ���");
	}
	return 0;
}
/************************************************************************/
/*  ��������ļ�                                                        */
/************************************************************************/
void CLoserWindowDlg::OnBnClickedBtnFind()
{
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
		SetDlgItemText(IDC_EDIT_EXEPATH,sFolderPath);
	}
}
/************************************************************************/
/* ����������Ϣ                                                         */
/************************************************************************/
void CLoserWindowDlg::OnBnClickedBtnSave()
{
	SaveParams();
}
/************************************************************************/
/* �ر���Ϸ����															*/
/************************************************************************/
void CLoserWindowDlg::ClosePlayWindow()
{
	CString strTemp = "";
	strTemp.Format("������Ϸ�ͻ��� �� ��ͨ����1 - �� %d �� ��",g_iRoomNum);
	HWND hwnd = ::FindWindow("#32770",strTemp);
	if (hwnd)
	{
		::PostMessage(hwnd,WM_CLOSE,0,0);
		Sleep(100);
		//�˴�����ǿ�˵Ŀ�����Ϣ
	}
}
/************************************************************************/
/* ���˵���¼�ɹ�״̬                                                   */
/************************************************************************/
void CLoserWindowDlg::BackToLoginStatus()
{
	CWindowFind find;
	GetInitBtnInfo(g_hwndTopMost);
	find.PostBtnClick(g_hwndBack);
	Sleep(100);	
}
/************************************************************************/
/* ��ȡ�ⲿ����CListCtrl��ָ�����ֵ                                    */
/************************************************************************/
CString CLoserWindowDlg::GetListItemText(HWND hwnd,int iItem,int iSubItem)
{
	LVITEM lvitem, *plvitem;
	char ItemBuf[512],*pItem;
	DWORD PID;
	HANDLE hProcess;
	CString strRet = "";
	GetWindowThreadProcessId(hwnd, &PID);
	hProcess=OpenProcess(PROCESS_ALL_ACCESS,false,PID);
	if (!hProcess)
	{
		return "";
	}	
	plvitem=(LVITEM*)VirtualAllocEx(hProcess, NULL, sizeof(LVITEM), MEM_COMMIT, PAGE_READWRITE);
	pItem=(char*)VirtualAllocEx(hProcess, NULL, 512, MEM_COMMIT, PAGE_READWRITE);
	if ((!plvitem)||(!pItem))
	{
		CloseHandle(hProcess);
		return "";
	}
	else    //��ȡ������
	{
		lvitem.cchTextMax=512;
		lvitem.iSubItem=iSubItem;
		lvitem.pszText=pItem;
		WriteProcessMemory(hProcess, plvitem, &lvitem, sizeof(LVITEM), NULL);
		::SendMessage(hwnd,LVM_GETITEMTEXT, (WPARAM)iItem,(LPARAM)plvitem);
		ReadProcessMemory(hProcess, pItem, ItemBuf, 512, NULL);
		strRet.Format("%s",ItemBuf);
	}			
	VirtualFreeEx(hProcess, plvitem, 0, MEM_RELEASE);
	VirtualFreeEx(hProcess, pItem, 0, MEM_RELEASE);
	CloseHandle(hProcess);
	return strRet;
}
/************************************************************************/
/* ��ѯ�˺����                                                         */
/************************************************************************/
int CLoserWindowDlg::GetBalance(HWND hwnd)
{
	CString strRet = GetListItemText(hwnd,0,4);
	return atoi(strRet);
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
CString CLoserWindowDlg::GetPreUsers(HWND hwnd,int iCount)
{
	CString strRet = "";
	for (int i=0;i<iCount;i++)
	{
		strRet += GetListItemText(hwnd,0,3)+",";
	}
	return strRet;
}
/************************************************************************/
/*���GM�Ƿ�����                                                        */
/************************************************************************/
bool CLoserWindowDlg::IsGmHere(HWND hwnd)
{
	if (g_iIsCheckGm == 0)
	{
		return false;
	}
	CString strRet = GetListItemText(hwnd,1,3);
	if (strRet.Find(g_strGmStr) != -1)
	{
		return true;
	}
	return false;
}
/************************************************************************/
/*��ȡ����ָ����Ϸ��ĳ�ʼ����Ϣ                                        */
/************************************************************************/
int CLoserWindowDlg::GetInitBtnInfo(HWND hwndTopParant)
{
	CWindowFind find;
	HWND hwndChild = NULL;
	g_hwndSet = NULL;
	g_hwndBack = NULL;
	g_hwndListView1 = NULL;
	g_hwndListView2 = NULL;
	g_hwndDesk = NULL;
	hwndChild = find.GetChildWindow(hwndTopParant,8);
	HWND hwndClildEx = find.GetChildWindow(hwndTopParant,8);
	if (!hwndChild)
	{
		return -1;
	}
	g_hwndSet = find.GetChildWindow(hwndChild,8);
	g_hwndBack = find.GetChildWindow(hwndChild,7);
	hwndChild = find.GetChildWindow(hwndChild,17);
	HWND hwndOld = hwndChild;
	hwndChild = find.GetChildWindow(hwndChild,1);	
	g_hwndListView1 = find.GetChildWindow(hwndChild,1);

	hwndChild = find.GetChildWindow(hwndClildEx,21);
	hwndChild = find.GetChildWindow(hwndChild,1);	
	g_hwndDesk = find.GetChildWindow(hwndChild,1);
	if (!g_hwndBack || !g_hwndSet || !g_hwndListView1 || !g_hwndDesk)
	{
		g_hwndSet = NULL;
		g_hwndBack = NULL;
		g_hwndListView1 = NULL;
		g_hwndListView2 = NULL;
		g_hwndDesk = NULL;
		return -1;
	}
	if (find.GetContrlClassName(g_hwndBack) != "Button" ||
		find.GetContrlClassName(g_hwndSet) != "Button" ||
		find.GetContrlClassName(g_hwndListView1) != "SysListView32" ||
		find.GetContrlClassName(g_hwndDesk) != "AfxWnd70")
	{
		g_hwndSet = NULL;
		g_hwndBack = NULL;
		g_hwndListView1 = NULL;
		g_hwndListView2 = NULL;
		g_hwndDesk = NULL;
		return -1;
	}
	return 0;
}
void CLoserWindowDlg::OnBnClickedButton8()
{
	GetDlgItem(IDC_BTN_START)->EnableWindow(TRUE);
}

void CLoserWindowDlg::OnBnClickedButton9()
{
	BackToLoginStatus();
}
/************************************************************************/
/*�ر��ȼ�                                                              */
/************************************************************************/
void CLoserWindowDlg::UnloadHook()
{
// 	if(m_KeyHook.hKeyBoardHook  != NULL)
// 		UnhookWindowsHookEx(m_KeyHook.hKeyBoardHook);
// 	if(m_KeyHook.hLowKeyBoardHook != NULL)
// 		UnhookWindowsHookEx(m_KeyHook.hLowKeyBoardHook);
}
/************************************************************************/
/*�����˳�                                                              */
/************************************************************************/
void CLoserWindowDlg::OnClose()
{
	UnloadHook();
	if (g_ExitFlag == 0)
	{
		g_ExitFlag = 1;
		Sleep(500);
	}
	CDialog::OnClose();
}
/************************************************************************/
/*�����ȼ�                                                              */
/************************************************************************/
void CLoserWindowDlg::SetHook()
{
//	m_KeyHook = SetHookEx(m_hWnd);
}

/************************************************************************/
/* Զ��ע�뷽ʽ˫�������                                               */
/************************************************************************/
int CLoserWindowDlg::DblClickItemNodeEx(HWND hwnd)
{
	CString strText = "";
	DWORD PID;
	HANDLE hProcess;
	HWND hParant = ::GetParent(hwnd);
	DWORD dwProcSize = 4096;
	GetWindowThreadProcessId(hParant, &PID);
	hProcess=OpenProcess(PROCESS_ALL_ACCESS, FALSE, PID);
	if (!hProcess)
	{
		ShowMessage("��Զ�̽��̿ռ�ʧ��");
		return -1;
	}
	MyData data;
	ZeroMemory(&data, sizeof (MyData));
	HINSTANCE hUser = LoadLibrary("user32.dll");
	if (! hUser)
	{
		ShowMessage("����user32.dllʧ��");
		return -1;
	}
	data.dwSendMsg = (DWORD)GetProcAddress(hUser, "SendMessageA");
	FreeLibrary(hUser);
	if (!data.dwSendMsg)
	{
		ShowMessage("��ȡSendMessage������ַʧ��");
		return -1;
	}
	//����WM_NOTIFY��Ϣ�ṹ��
	ZeroMemory(&data.nmh, sizeof (NMHDR));
	data.nmh.code = NM_DBLCLK;
	data.nmh.hwndFrom = hwnd;
	data.nmh.idFrom = ::GetDlgCtrlID(hwnd);
	data.hwndParant = ::GetParent(hwnd);

	//����ռ�
	void *pRemoteThread	= VirtualAllocEx(hProcess, 0,dwProcSize, MEM_COMMIT|MEM_RESERVE,PAGE_EXECUTE_READWRITE);
	if (!pRemoteThread)
	{
		ShowMessage("����Զ�̺�����ַ�ռ�ʧ��");
		CloseHandle(hProcess);
		return -1;
	}
	if (!WriteProcessMemory(hProcess, pRemoteThread, &RemoteFunc,dwProcSize,0))
	{
		ShowMessage("д��Զ�̺�����ַʧ��");
		VirtualFreeEx(hProcess, pRemoteThread, dwProcSize, MEM_RELEASE);
		CloseHandle(hProcess);
		return -1;
	}
	MyData *pData = (MyData*)VirtualAllocEx(hProcess, 0,sizeof (MyData), MEM_COMMIT,PAGE_READWRITE);
	if (!pData)
	{
		VirtualFreeEx(hProcess, pRemoteThread, dwProcSize, MEM_RELEASE);
		CloseHandle(hProcess);
		ShowMessage("����Զ�̲����ռ�ʧ��");
		return -1;
	}
	if (! WriteProcessMemory(hProcess, pData, &data, sizeof (MyData), 0))
	{
		VirtualFreeEx(hProcess, pRemoteThread, dwProcSize, MEM_RELEASE);
		VirtualFreeEx(hProcess, pData, sizeof (MyData), MEM_RELEASE);
		CloseHandle(hProcess);
		ShowMessage("ע�������Ϣʧ��");
		return -1;
	}
	//����Զ���߳�
	HANDLE hThread = CreateRemoteThread(hProcess,0,0,(LPTHREAD_START_ROUTINE)pRemoteThread,(LPVOID)pData,0,0);
	if (!hThread)
	{
		VirtualFreeEx(hProcess, pRemoteThread, dwProcSize, MEM_RELEASE);
		VirtualFreeEx(hProcess, pData, sizeof (MyData), MEM_RELEASE);
		CloseHandle(hProcess);
		ShowMessage("����Զ���߳�ʧ��");
		return -1;
	}
	WaitForSingleObject(hThread,1000);
	CloseHandle(hThread);
	VirtualFreeEx(hProcess, pRemoteThread, dwProcSize, MEM_RELEASE);
	VirtualFreeEx(hProcess, pData, sizeof (MyData), MEM_RELEASE);
	CloseHandle(hProcess);
	return 0;
}

/************************************************************************/
/*������,���ҵ�һ������Ϊ����ֵ�Ľڵ�,����                              */
/************************************************************************/
HTREEITEM CLoserWindowDlg::FindItem(HWND hwnd,HTREEITEM hItem,CString strText)   
{   
	HTREEITEM hFind;
	if(hItem == NULL)   
	{
		return NULL;
	}
	CString strItemText = GetItemNodeText(hwnd,hItem);
	ShowMessage(strItemText);
	if(strItemText.Left(strText.GetLength()) == strText)//�ҵ�
	{		
		return hItem;
	}
	HTREEITEM hChild = TreeView_GetChild(hwnd,hItem);
	if(hChild)
	{
		hItem = hChild;   
		hFind = FindItem(hwnd,hItem,strText);
		if(hFind)   
		{   
			return hFind;
		}
		else   
		{			
			hItem= TreeView_GetNextItem(hwnd,TreeView_GetParent(hwnd,hItem),TVGN_NEXT);
			if (hItem)
			{
				return FindItem(hwnd,hItem,strText);
			}
		}
	}   
	else   
	{
		hItem= TreeView_GetNextItem(hwnd,hItem,TVGN_NEXT);
		if (!hItem)
		{
			return NULL;
		}
		hFind = FindItem(hwnd,hItem,strText);
		return hFind;		
	}
	return hFind;
}
/************************************************************************/
/* �ٶȵ���                                                             */
/************************************************************************/
void CLoserWindowDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	CSliderCtrl* pSc2=(CSliderCtrl*)pScrollBar;
	g_iSpeed = 100 - pSc2->GetPos();
	if (g_iSpeed < 1)
	{
		g_iSpeed = 1;
	}
	else if (g_iSpeed >100)
	{
		g_iSpeed = 100;
	}
	CString strAppPath = "";
	CString strTemp = "";
	CUserFile *pFile = CUserFile::GetInstance();
	pFile->GetAppPath(strAppPath);
	pFile->WriteInitInfo(strAppPath +"set.ini","Fresh","speed","",g_iSpeed,1);
	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CLoserWindowDlg::OnBnClickedCheckGmcheck()
{
	g_iIsCheckGm = m_CheckGM.GetCheck();
	CString strAppPath = "";
	CString strTemp = "";
	CUserFile *pFile = CUserFile::GetInstance();
	pFile->GetAppPath(strAppPath);
	pFile->WriteInitInfo(strAppPath +"set.ini","Fresh","checkgm","",g_iIsCheckGm,1);
	if (g_iIsCheckGm)
	{
		GetDlgItem(IDC_EDIT_STOPMINUTES)->EnableWindow(TRUE);
		GetDlgItem(IDC_CHECK_GMCHECKWAIT)->EnableWindow(TRUE);
		OnBnClickedCheckGmcheckwait();
	}
	else
	{
		GetDlgItem(IDC_EDIT_CHECKINTERVAL)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHECK_GMCHECKWAIT)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_STOPMINUTES)->EnableWindow(FALSE);
	}
	GetDlgItem(IDC_EDIT_GMSTR)->EnableWindow(g_iIsCheckGm);
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CLoserWindowDlg::OnBnClickedCheckHidewindow()
{
	g_iIsHidePlayWindow = m_HideWindow.GetCheck();
	CString strAppPath = "";
	CString strTemp = "";
	CUserFile *pFile = CUserFile::GetInstance();
	pFile->GetAppPath(strAppPath);
	pFile->WriteInitInfo(strAppPath +"set.ini","Fresh","hidewindow","",g_iIsHidePlayWindow,1);
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CLoserWindowDlg::AddFreshResult(RegisterPack rgPack,int iFlag)
{
	CListCtrl *pList = &m_ListAcc;
	int nCount = pList->GetItemCount();
	for (int i=0;i<nCount;i++)
	{
		if (rgPack.strAcc == pList->GetItemText(i,1))
		{
			pList->SetItemText(i,3,iFlag==0?"�ɹ�":"ʧ��");
			break;
		}
	}
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CLoserWindowDlg::ShutUpWindow()
{
	CWindowFind find;
	Sleep(150);
	HWND hwnd = ::FindWindow("#32770","��ʾ��Ϣ");
	HWND hwndChild = find.GetChildWindow(hwnd,1);
	int iCount = 1;
	while (hwnd && hwndChild)
	{
		hwndChild = find.GetChildWindow(hwnd,iCount);
		CString strWinName = find.GetContrlWindowText(hwndChild);
		if ((strWinName.Find("ȷ��") != -1 || strWinName.Find("��") != -1) && 
			find.GetContrlClassName(hwndChild) =="Button")
		{
			ShowMessage("ǿ���˳���Ϸ!");
			find.PostBtnClick(hwndChild);
		}
		iCount ++;
		if (iCount > 25)
		{
			break;
		}
	}
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
int CLoserWindowDlg::GetPower()
{
	CInternetSession session;
	CString strTemp = "";
	try
	{
		CStdioFile *pFile = session.OpenURL("http://blog.csdn.net/TragicGuy/archive/2009/11/26/4881708.aspx");
		if (pFile != NULL)
		{
			while(pFile->ReadString(strTemp))
			{
				strTemp.MakeLower();
				if (strTemp.Find("myownpower") !=-1)
				{
					pFile->Close();
					delete pFile;
					pFile = NULL;
					return 0;
				}
			}
			
			if (pFile == NULL)
			{
				pFile->Close();
				delete pFile;
				pFile = NULL;
			}
		}
	}
	catch (...)
	{
	}
	return -1;
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CLoserWindowDlg::SetLastWaitCheckTime()
{
	m_iCurrTime = ::GetTickCount();
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
bool CLoserWindowDlg::IsOverCheckTime(int iIntervalSeconds)
{
	if (g_iIsCheckGm == 0)
	{
		return false;
	}
	if (g_iCheckGmWait == 0)
	{
		return false;
	}
	DWORD t2 = 0;
	t2 = ::GetTickCount();
	DWORD tSpan = 0;
	tSpan = t2 - m_iCurrTime;
	if (tSpan > iIntervalSeconds * 1000) //iDelay ��
	{
		return true;
	}
	else
	{
		return false;
	}
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CLoserWindowDlg::OnBnClickedCheckGmcheckwait()
{
	g_iCheckGmWait = m_gmCheckWait.GetCheck();
	CString strAppPath = "";
	CString strTemp = "";
	CUserFile *pFile = CUserFile::GetInstance();
	pFile->GetAppPath(strAppPath);
	pFile->WriteInitInfo(strAppPath +"set.ini","Fresh","checkgmwait","",g_iCheckGmWait,1);
	if (g_iIsCheckGm)
	{
		GetDlgItem(IDC_EDIT_CHECKINTERVAL)->EnableWindow(g_iCheckGmWait);
	}
	else
	{
		GetDlgItem(IDC_EDIT_CHECKINTERVAL)->EnableWindow(FALSE);
	}
}
/************************************************************************/
/*  ��ȡ�ⲿ�������Ľ��������Ϣ                                        */
/************************************************************************/
void CLoserWindowDlg::SetRemoteWindowText(HWND hwnd, CString strText)
{
	DWORD PID;
	HANDLE hProcess;
	DWORD dwProcSize = 4096;
	GetWindowThreadProcessId(hwnd, &PID);
	hProcess=OpenProcess(PROCESS_ALL_ACCESS, FALSE, PID);
	if (!hProcess)
	{
		ShowMessage("��Զ�̽��̿ռ�ʧ��");
		return ;
	}
	char data[512] = {0};
	strcpy(data,strText);

	char *pData = (char*)VirtualAllocEx(hProcess,0,sizeof (char), MEM_COMMIT,PAGE_READWRITE);
	if (!pData)
	{
		VirtualFreeEx(hProcess, pData, dwProcSize, MEM_RELEASE);
		CloseHandle(hProcess);
		ShowMessage("����Զ�̲����ռ�ʧ��");
		return ;
	}
	if (! WriteProcessMemory(hProcess, pData, &data, sizeof (char), 0))
	{
		VirtualFreeEx(hProcess, pData, sizeof (char), MEM_RELEASE);
		CloseHandle(hProcess);
		ShowMessage("ע�������Ϣʧ��");
		return ;
	}
	::SetWindowText(hwnd,data);
	VirtualFreeEx(hProcess, pData, sizeof (pData), MEM_RELEASE);
	CloseHandle(hProcess);
	return ;
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
int CLoserWindowDlg::GetWindowCtrl()
{
	g_hwndWindow = ::FindWindow(g_strWindowClass,g_strWindowName);
	if (!g_hwndWindow)
	{
		ShowMessage("δ�ҵ�Ŀ�괰��,��ȷ��Ŀ�����������������������ȷ");
		return -1;
	}
	CWindowFind find;
	g_hwndWindow = find.GetChildWindow(g_hwndWindow,15);
	CString  strClassName = find.GetContrlClassName(g_hwndWindow);
	g_hwndWindow = find.GetChildWindow(g_hwndWindow,2);
	g_hwndWindow = find.GetChildWindow(g_hwndWindow,1);
	if (!g_hwndWindow)
	{
		ShowMessage("δ�ҵ����ݿ��ƴ���");
		return -2;
	}
	Sleep(20);
	g_hwndTitle = find.GetChildWindow(g_hwndWindow,3);
	if (!g_hwndTitle)
	{
		ShowMessage("δ�ҵ����ⴰ��");
		return -2;
	}
	Sleep(20);
	g_hwndContent = find.GetChildWindow(g_hwndWindow,4);
	if (!g_hwndContent)
	{
		ShowMessage("δ�ҵ����ݴ���");
		return -2;
	}
	Sleep(20);
	g_hwndPhoneUrl = find.GetChildWindow(g_hwndWindow,5);
	if (!g_hwndPhoneUrl)
	{
		ShowMessage("δ�ҵ�ͼƬ���ӿ�");
		return -2;
	}
	Sleep(20);
	g_hwndAdd = find.GetChildWindow(g_hwndWindow,6);
	if (!g_hwndAdd)
	{
		ShowMessage("δ�ҵ���Ӱ�ť");
		return -2;
	}
	return 0;
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CLoserWindowDlg::InsertData(CString strTile,CString strContent,CString strUrl)
{
	if (g_iWinOrLose = 1)
	{
		CWindowFind find;
		find.SetEditText(g_hwndTitle,strTile);
		Sleep(20);
		find.SetEditText(g_hwndContent,strContent);
		Sleep(20);
		if (strUrl != "")
		{
			find.SetEditText(g_hwndPhoneUrl,strUrl);
		}
		
		Sleep(100);
		find.PostBtnClick(g_hwndAdd);
		return ;
	}
	//�������ݿ�
	//return WriteToBase(strTile,strContent,strUrl);
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
int CLoserWindowDlg::WriteToBase(CString strTile,CString &strContent,CString strUrl)
{
	int iRet = -1;
	CAdoConnection adoConn;
	CAdoRecordSet adoRst;
	try
	{
		if ( !adoConn.IsOpen() )		//���δ�����Ϸ�����
		{
			CString strDbPath = g_strExeFolder + g_strDbpath;
			if ( !adoConn.ConnectAccess(strDbPath))
			{
				ShowMessage("�������ݿ�ʧ��");
				return -1; 
			}
			adoRst.SetAdoConnection(&adoConn);
			adoRst.SetCursorLocation(adUseClient);
		}
		CString strSQL = "";		
		//strSQL.Format("insert into [title] ([userid],[userpwd],flag)values('%s','%s',%d,)",rgPack.strAcc,rgPack.strPwd,iFlag);
		if( !adoRst.Open(strSQL,adCmdText))
		{
			iRet = -1;
			ShowMessage("�����Ϣʧ��");
		}
		else
		{
			iRet = 0;
		}
		adoRst.Close();
		adoConn.Close();
		ShowMessage("�����Ϣ�ɹ�");
		return iRet;
	}
	catch (...)
	{
		adoRst.Close();
		adoConn.Close();
		ShowMessage("�����Ϣ�쳣");
	}
	return -1;
}
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

#define  WM_GETGAMEDATA WM_USER + 4355
#define  WM_STARTGAME WM_USER + 4353
#define  SRC_PIN_PATH "srcpic.bmp"
CString g_strClassName = "MacromediaFlashPlayerActiveX";
CWindowFind g_find;

CString gl_PlayWindowName = "����30�� �� ������ - �� 1 �� ��";
CProcessManager g_ProcessMgr;
#define  ONLY_LOSER
// #define ONLY_WINNNER
// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���
typedef struct _MyData
{
	DWORD dwSendMsg; // sendmessage������ַ
	HWND hwndParant;
	NMHDR nmh;
}MyData;

bool gl_bCloseCheckWindow = false;
bool gl_bSame = false;
int gl_iSameCount = 0;
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
CString g_strAppPathFrame = "";
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
int g_iMinMoney = 0;
int g_iIsSetPwd = 0;

HWND g_hwndPlayContrl = NULL;

CString g_srcPicPath = "";

bool g_bNeedWaitGotoReLog = true;

bool g_bTimeStop = false;

int g_iWinnerWinMoney = 0;	//Ӫ��ʱ��Ǯ
int g_iSaveLeftMoney = 0;	//��������ֻ������
int g_iGetKeepMoney = 0;	//ȡ�������ж���Ǯ

int g_iHaveWinTime = 0;
/************************************************************************/
/* �Ұ�����                                                             */
/************************************************************************/
HWND gl_hwndGetIn = NULL;	//���밴ť���
HWND gl_hwndBank = NULL;	//������еľ��
HWND gl_hwndPlayWindow = NULL;	//��Ϸ���ھ��
HWND gl_hwndRoomList = NULL;	//���б�

HWND gl_hwndWaitingList = NULL;	//��ׯ�б�
HWND gl_hwndMoneyList = NULL;	//��Ϸ�����Ա�б�
HWND gl_hwndbtnApply = NULL;
HWND gl_hwndDeskWindow = NULL;
HWND gl_hwndWnd = NULL;		//��Ϸ���ľ��
//��ע�ֵľ��
HWND gl_hwndbtn100 = NULL;
HWND gl_hwndbtn500 = NULL;
HWND gl_hwndbtn1000 = NULL;
HWND gl_hwndbtn10000 = NULL;
HWND gl_hwndbtn50000 = NULL;
HWND gl_hwndbtn100000 = NULL;
HWND gl_hwndbtn500000 = NULL;
CString gl_strRunAcc = "";
CString gl_strBankPwd = "zhangan";
int gl_iMaxBalance = 0;
int gl_iMinBalance = 0;
long gl_DataAddr = 0;
BOOL CALLBACK EnumChildProcReLogin(HWND  hwndTemp, LPARAM lParam)
{   	
	if (::GetParent(hwndTemp) == g_hwndTopMost )
	{
		if (g_find.GetContrlClassName(hwndTemp) == "#32770")
		{
			if (g_find.GetContrlWindowText(hwndTemp) == "")
			{
				if (g_find.GetChildWindowNum(hwndTemp) == 3)
				{
					int iCount = 0;
					for (int i =1;i<4;i++)
					{
						HWND hwnd = g_find.GetChildWindow(hwndTemp,i);
						if (hwnd && g_find.GetContrlClassName(hwnd) == "Button")
						{
							iCount++;
						}						
					}
					if (iCount == 3)
					{
						HWND hwnd = g_find.GetChildWindow(hwndTemp,1);
						g_find.PostBtnClick(hwnd);
						g_bNeedWaitGotoReLog = false;
						return FALSE;
					}
					
				}
			}
		}
	}
	EnumChildWindows(hwndTemp,EnumChildProcReLogin,lParam);
	return   TRUE;   
}


BOOL CALLBACK EnumChildProcCheckNum(HWND  hwndTemp, LPARAM lParam)
{
	if (g_find.GetContrlClassName(hwndTemp) == "#32770")
	{
		if (g_find.GetContrlWindowText(hwndTemp) == "")
		{
			if (g_find.GetChildWindowNum(hwndTemp) == 2)
			{
				//�жϴ��ڴ�С 217x260
				RECT rect;
				GetWindowRect(hwndTemp,&rect);
				if (rect.right - rect.left == 217)
				{
					HWND hwnd = g_find.GetChildWindow(hwndTemp,2);
					if (IsWindowEnabled(hwnd))
					{
						g_find.PostBtnClick(hwnd);
						gl_bCloseCheckWindow = true;
						return FALSE;
					}
				}				
			}
		}
	}
	EnumChildWindows(hwndTemp,EnumChildProcCheckNum,lParam);
	return   TRUE;   
}

BOOL CALLBACK EnumChildProcDesk(HWND  hwndTemp, LPARAM lParam)
{

	if (g_find.GetContrlWindowText(hwndTemp) == "")
	{
		if (g_find.GetContrlClassName(hwndTemp) == "AfxWnd70")
		{
			//�жϴ��ڴ�С 217x260
			RECT rect;
			GetWindowRect(hwndTemp,&rect);
			if (rect.right - rect.left == 990 && rect.bottom-rect.top == 601)
			{
				gl_hwndDeskWindow = hwndTemp;
				return FALSE;
			}				
		}
	}
	EnumChildWindows(hwndTemp,EnumChildProcDesk,lParam);
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


// CLoserWindowDlg �Ի���

typedef CArray<RegisterPack, RegisterPack> RegisterPackList;
RegisterPackList g_RegisterList;

CLoserWindowDlg::CLoserWindowDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLoserWindowDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	CoInitialize(NULL);
	try
	{
		HRESULT hr;
		CLSID clsid;
		hr=CLSIDFromProgID(OLESTR("HookCall.UserControl1"),&clsid);	
		IClassFactory* p_classfactory;
		hr=CoGetClassObject(clsid,CLSCTX_INPROC_SERVER,
			NULL,IID_IClassFactory, 
			(LPVOID*)&p_classfactory);
		p_classfactory->CreateInstance(NULL,__uuidof(_UserControl1),
			(LPVOID*)&m_AddrCall);
	}
	catch (...)
	{
		MessageBox("����ע��COM��","����");
	}
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
_declspec(dllimport) HookStruct SetHookEx(HWND m_hwndHelper,HWND hwndShow);

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
	ON_BN_CLICKED(IDC_BTN_INIT, &CLoserWindowDlg::OnBnClickedBtnInit)
	ON_WM_TIMER()
	ON_EN_CHANGE(IDC_EDIT_PLAYNUM, &CLoserWindowDlg::OnEnChangeEditPlaynum)
	//ON_EN_CHANGE(IDC_EDIT_SETNUM2, &CLoserWindowDlg::OnEnChangeEditSetnum2)
	ON_EN_CHANGE(IDC_EDIT_SCORE, &CLoserWindowDlg::OnEnChangeEditScore)
	ON_EN_CHANGE(IDC_EDIT_MYBALANCE, &CLoserWindowDlg::OnEnChangeEditMybalance)
	ON_EN_CHANGE(IDC_EDIT_GETKEEP, &CLoserWindowDlg::OnEnChangeEditGetkeep)
	ON_BN_CLICKED(IDC_RADIO_PEASE, &CLoserWindowDlg::OnBnClickedRadioPease)
	ON_BN_CLICKED(IDC_RADIO_PEASESAME, &CLoserWindowDlg::OnBnClickedRadioPeasesame)
	ON_BN_CLICKED(IDC_RADIO_WINLEFT, &CLoserWindowDlg::OnBnClickedRadioWinleft)
	ON_BN_CLICKED(IDC_RADIO_WINRIGHT, &CLoserWindowDlg::OnBnClickedRadioWinright)
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
	LoadOldAcc();
	RestartGame();
	
#ifdef _DEBUG
#else			//������Ҫ����Ȩ��
if (GetPower() != 0)
{
	exit(0);
}
#endif
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
	if (pFile)
	{
		pFile->ReadInitInfo(strAppPath+"set.ini","Fresh","path",g_strAppPath,iTemp,0);
		g_strAppPathFrame = g_strAppPath.Left(g_strAppPath.ReverseFind('\\'));
		g_strAppPathFrame += "\\Frame.exe";
		pFile->ReadInitInfo(strAppPath+"set.ini","Fresh","roompwd",g_strRoomPwd,iTemp,0);
		pFile->ReadInitInfo(strAppPath+"set.ini","Fresh","type",strTemp,g_iWinOrLose,1);
		pFile->ReadInitInfo(strAppPath+"set.ini","Fresh","losenum",strTemp,g_iLoseNum,1);
		pFile->ReadInitInfo(strAppPath+"set.ini","Fresh","desknum",strTemp,g_iRoomNum,1);
		pFile->ReadInitInfo(strAppPath+"set.ini","Fresh","setnum",strTemp,g_iSetNum,1);
		pFile->ReadInitInfo(strAppPath+"set.ini","Fresh","min",strTemp,g_iMinBalance,1);
		pFile->ReadInitInfo(strAppPath+"set.ini","Fresh","max",strTemp,g_iMaxBalance,1);
		//pFile->CheckInitValue(g_iSetNum,0,3);
		pFile->CheckInitValue(g_iRoomNum,1,99);
		pFile->CheckInitValue(g_iLoseNum,1,60000);
		//pFile->CheckInitValue(g_iWinOrLose,0,1);
		//pFile->CheckInitValue(g_iMinBalance,10,150000000);
		//pFile->CheckInitValue(g_iMaxBalance,10000,10000000000);
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
		pFile->CheckInitValue(g_iCheckGmWaitInterval,2,100);
		SetDlgItemInt(IDC_EDIT_CHECKINTERVAL,g_iCheckGmWaitInterval);
		SetDlgItemText(IDC_EDIT_GMSTR,g_strGmStr);
		m_CheckGM.SetCheck(g_iIsCheckGm);
		m_gmCheckWait.SetCheck(g_iCheckGmWait);
		OnBnClickedCheckGmcheck();
		pFile->ReadInitInfo(strAppPath +"set.ini","Fresh","setpwd",strTemp,g_iIsSetPwd,1);
		pFile->CheckInitValue(g_iIsSetPwd,0,1);
		((CButton *)GetDlgItem(IDC_CHECK_SETPWD))->SetCheck(g_iIsSetPwd);
		
		pFile->ReadInitInfo(strAppPath +"set.ini","Fresh","minmoney",strTemp,g_iMinMoney,1);
		//pFile->CheckInitValue(g_iMinMoney,0,10000);
		SetDlgItemInt(IDC_EDIT_MINMONEY,g_iMinMoney);


		pFile->ReadInitInfo(strAppPath+"set.ini","Fresh","g_iWinnerWinMoney",strTemp,g_iWinnerWinMoney,1);
		pFile->ReadInitInfo(strAppPath+"set.ini","Fresh","g_iSaveLeftMoney",strTemp,g_iSaveLeftMoney,1);
		pFile->ReadInitInfo(strAppPath+"set.ini","Fresh","g_iGetKeepMoney",strTemp,g_iGetKeepMoney,1);
		SetDlgItemInt(IDC_EDIT_WINNERWIN,g_iWinnerWinMoney);
		SetDlgItemInt(IDC_EDIT_SAVELEFT,g_iSaveLeftMoney);
		SetDlgItemInt(IDC_EDIT_GETKEEP,g_iGetKeepMoney);

	}

	g_srcPicPath = strAppPath + SRC_PIN_PATH;
	SetDlgItemInt(IDC_EDIT_MINNUM,g_iMinBalance);
	SetDlgItemInt(IDC_EDIT_MAXNUM,g_iMaxBalance);
	SetDlgItemText(IDC_EDIT_ROOMPWD,g_strRoomPwd);
	SetDlgItemText(IDC_EDIT_EXEPATH,g_strAppPath);
	SetDlgItemInt(IDC_EDIT_DESKNUM,g_iRoomNum);
	SetDlgItemInt(IDC_EDIT_SETNUM,g_iSetNum);
	SetDlgItemInt(IDC_EDIT_LOSENUM,g_iLoseNum);
#ifdef ONLY_LOSER
	//GetDlgItem(IDC_RADIO_WIN)->ShowWindow(SW_HIDE);
	//g_iWinOrLose = 0;
#else
#ifdef ONLY_WINNNER
	//GetDlgItem(IDC_RADIO_LOSE)->ShowWindow(SW_HIDE);
	g_iWinOrLose = 1;
#endif
#endif

	if (g_iWinOrLose == 1)
	{
		OnBnClickedRadioWin();
		SetWindowText("Ӯ�ҿͻ���");
	}
	else
	{
		OnBnClickedRadioLose();
		SetWindowText("��ҿͻ���");
	}
	
	SetPalyType(g_iWinOrLose);
	//SetHook();
	m_iCurrTime = -100000;
	GetDlgItem(IDC_RADIO_WIN)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_EDIT_MINNUM)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT_MAXNUM)->EnableWindow(TRUE);
	SetWindowText("�Ұ����ƿ���30���Զ����Ƴ���");
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
	if (SaveParams()<0)
	{
		return;
	}
	//g_strAppPath = "";
	g_bTimeStop = false;
	g_ExitFlag = 0;
	//SetTimer(1,30000,NULL);
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
		pParam->GetDlgItem(IDC_BTN_START)->EnableWindow(TRUE);
		pParam->GetDlgItem(IDC_BTN_STOP)->EnableWindow(FALSE);
		g_ExitFlag = true;
	}
}

HWND CLoserWindowDlg::GetPlayDesktopWindow()
{
	gl_hwndDeskWindow = NULL;
	EnumChildWindows(NULL,EnumChildProcDesk,(LPARAM)0);
	return NULL;
}
void CLoserWindowDlg::CloseHuaPaiWarnWidow()
{
	//������
	HWND hwnd = ::FindWindow("#32770","������");
	if (hwnd)
	{
		::PostMessage(hwnd,WM_CLOSE,0,0);
		Sleep(10);
	}
}


bool CLoserWindowDlg::GetMoneyFromBank(long iMoney,CString strPwd)
{	
	int iCount = 0;
	CString strShow = "";
	strShow.Format("ȡǮ: %d",iMoney);
	ShowMessage(strShow);
	HWND hwnd = ::FindWindow("#32770","�û�����");
	while(hwnd)
	{
		::PostMessage(hwnd,WM_CLOSE,0,0);
		hwnd = ::FindWindow("#32770","�û�����");
		Sleep(50);
		CloseHuaPaiWarnWidow();
	}

	while (iCount++<10)
	{
		g_find.PostBtnClick(gl_hwndBank);
		Sleep(700);
		HWND hwnd = ::FindWindow("#32770","�û�����");
		if (!hwnd)
		{
			Sleep(100);
			continue;
		}
		Sleep(100);
		//��ȡ�������
		HWND hwndBankMoney = g_find.GetChildWindow(hwnd,5);
		if (!hwndBankMoney)
		{
			Sleep(100);
			continue;
		}
		int iLoop = 0;
		CString strbankMoney = "";
		while(iLoop++ < 6)
		{
			strbankMoney = g_find.GetContrlWindowText(hwndBankMoney);
			if (strbankMoney == "0")
			{
				Sleep(300);
				continue;
			}
			break;
		}
		if (atol(strbankMoney) < iMoney)
		{
			ShowMessage("��������Ѿ�����");

			HWND hwndTemp = ::FindWindow("#32770","�û�����");
			while(hwndTemp)
			{
				::PostMessage(hwndTemp,WM_CLOSE,0,0);
				hwndTemp = ::FindWindow("#32770","�û�����");
				Sleep(50);
				CloseHuaPaiWarnWidow();
			}
			return false;
		}

		CString strMoney = "";
		strMoney.Format("%d",iMoney);
		//7ȷ�� 8,13
		HWND hwndMoney = g_find.GetChildWindow(hwnd,8);
		if (!hwndMoney)
		{
			Sleep(100);
			continue;
		}
		g_find.SetEditText(hwndMoney,strMoney);
		Sleep(30);
		HWND hwndPwd = g_find.GetChildWindow(hwnd,13);
		if (!hwndPwd)
		{
			Sleep(100);
			continue;
		}
		g_find.SetEditText(hwndPwd,strPwd);
		Sleep(30);
		HWND hwndSave = g_find.GetChildWindow(hwnd,11);	//ȡǮ
		if (!hwndSave)
		{
			Sleep(100);
		}
		g_find.PostBtnClick(hwndSave);

		HWND hwndSub = g_find.GetChildWindow(hwnd,7);
		if (!hwndSub)
		{
			Sleep(100);
			continue;
		}
		g_find.PostBtnClick(hwndSub);
		ShowMessage("�ύȡǮ����!");
		Sleep(100);
		int iCountEx = 0;
		while(iCountEx < 10)
		{
			hwnd = ::FindWindow("#32770","������");
			if (hwnd)
			{
				::PostMessage(hwnd,WM_CLOSE,0,0);
				hwnd = ::FindWindow("#32770","�û�����");
				if (hwnd)
				{
					::PostMessage(hwnd,WM_CLOSE,0,0);
					Sleep(20);
				}
				Sleep(50);
				ShowMessage("�ر�ȡǮ�Ի���!");
				return true;
			}
			Sleep(200);
		}
	}
	return true;
}


void CLoserWindowDlg::SaveMoneyToBank(int iMoney,CString strPwd)
{	
	int iCount = 0;
	CString strShow = "";
	strShow.Format("��Ǯ: %d",iMoney);
	ShowMessage(strShow);
	HWND hwnd = ::FindWindow("#32770","�û�����");
	while(hwnd)
	{
		::PostMessage(hwnd,WM_CLOSE,0,0);
		hwnd = ::FindWindow("#32770","�û�����");
		Sleep(50);
		CloseHuaPaiWarnWidow();
	}
	//strPwd = "342523";
	while (iCount++<10)
	{
		g_find.PostBtnClick(gl_hwndBank);
		Sleep(500);
		HWND hwnd = ::FindWindow("#32770","�û�����");
		if (!hwnd)
		{
			Sleep(100);
			continue;
		}

		//��ȡ�������
		HWND hwndBankMoney = g_find.GetChildWindow(hwnd,3);
		if (!hwndBankMoney)
		{
			Sleep(100);
			continue;
		}
		int iLoop = 0;
		CString strbankMoney = "";
		while(iLoop++ < 8)
		{
			strbankMoney = g_find.GetContrlWindowText(hwndBankMoney);
			if (strbankMoney == "0")
			{
				Sleep(300);
				continue;
			}
			break;
		}
		if (atol(strbankMoney) < iMoney)
		{
			ShowMessage("�������е�ֵ�����������");

			HWND hwndTemp = ::FindWindow("#32770","�û�����");
			while(hwndTemp)
			{
				::PostMessage(hwndTemp,WM_CLOSE,0,0);
				hwndTemp = ::FindWindow("#32770","�û�����");
				Sleep(50);
				CloseHuaPaiWarnWidow();
			}
			return;
		}
		CString strMoney = "";
		strMoney.Format("%d",iMoney);
		//7ȷ�� 8,13
		HWND hwndMoney = g_find.GetChildWindow(hwnd,8);
		if (!hwndMoney)
		{
			Sleep(100);
			continue;
		}
		g_find.SetEditText(hwndMoney,strMoney);
		Sleep(30);
		HWND hwndPwd = g_find.GetChildWindow(hwnd,13);
		if (!hwndPwd)
		{
			Sleep(100);
			continue;
		}
		g_find.SetEditText(hwndPwd,strPwd);
		Sleep(30);

		HWND hwndSave = g_find.GetChildWindow(hwnd,10);
		if (hwndSave)
		{
			g_find.PostBtnClick(hwndSave);
		}
		HWND hwndSub = g_find.GetChildWindow(hwnd,7);
		if (!hwndSub)
		{
			Sleep(100);
			continue;
		}
		g_find.PostBtnClick(hwndSub);
		ShowMessage("�ύ��Ǯ����!");
		Sleep(100);
		int iCountEx = 0;
		while(iCountEx < 10)
		{
			hwnd = ::FindWindow("#32770","������");
			if (hwnd)
			{
				::PostMessage(hwnd,WM_CLOSE,0,0);
				hwnd = ::FindWindow("#32770","�û�����");
				if (hwnd)
				{
					::PostMessage(hwnd,WM_CLOSE,0,0);
					Sleep(20);
				}
				Sleep(50);
				ShowMessage("�رմ�Ǯ�Ի���!");
				return;
			}
			Sleep(200);
		}
		
	}
}
void CLoserWindowDlg::QuitPlayWindow()
{
	::PostMessage(gl_hwndPlayWindow,WM_CLOSE,0,0);
	ShowMessage("������Ϣʹ��Ϸ�����˳�");
		
	Sleep(500);
	gl_hwndPlayWindow = NULL;

	int iCount = 0;
	while (!g_ExitFlag && ::FindWindow("#32770",gl_PlayWindowName) != NULL)
	{
		if (iCount++ > 10)
		{
			return;
		}
		//���Ҿ��洰��
		HWND hwnd = ::FindWindow("#32770","Baccarat");
		if (hwnd)
		{
			hwnd = g_find.GetChildWindow(hwnd,1);
			g_find.PostBtnClick(hwnd);
			ShowMessage("ǿ�ƹرմ���1");
		}
		hwnd = ::FindWindow("#32770","��ʾ��Ϣ");
		if (hwnd)
		{
			hwnd = g_find.GetChildWindow(hwnd,1);
			g_find.PostBtnClick(hwnd);
			ShowMessage("ǿ�ƹرմ���2");
		}
		Sleep(300);
	}	
}
//��ע 
void CLoserWindowDlg::Blockbet(int iFlag,int iMoney)
{
	int i500000 = 0;
	int i100000 = 0;
	int i50000 = 0;
	int i10000 = 0;
	int i1000 = 0;
	int i500 = 0;
	int i100 = 0;
	if (iMoney >= 500000)
	{
		i500000 = iMoney / 500000;
		iMoney = iMoney % 500000;
	}

	if (iMoney >= 100000)
	{
		i100000 = iMoney / 100000;
		iMoney = iMoney % 100000;
	}

	if (iMoney >= 50000)
	{
		i50000 = iMoney / 50000;
		iMoney = iMoney % 50000;
	}

	if (iMoney >= 10000)
	{
		i10000 = iMoney / 10000;
		iMoney = iMoney % 10000;
	}

	if (iMoney >= 1000)
	{
		i1000 = iMoney / 1000;
		iMoney = iMoney % 1000;
	}

	if (iMoney >= 500)
	{
		i500 = iMoney / 500;
		iMoney = iMoney % 500;
	}

	if (iMoney >= 100)
	{
		i100 = iMoney / 100;
		iMoney = iMoney % 100;
	}

	//�ƶ������Ŀ������
	CRect rect;
	::GetClientRect(gl_hwndWnd,&rect);
	CWnd::FromHandle(gl_hwndWnd)->ClientToScreen(&rect);
	int iPointY = rect.top + (rect.bottom - rect.top) / 2;
	int iPointX = rect.left + ( rect.right - rect.left)/5;
	if (iFlag == 2)	//��ʾׯ
	{
		iPointX = rect.left + ( rect.right - rect.left)*3/4;
	}

	switch(iFlag)
	{
	case 1:
		
		break;
	case 2:
		
		break;
	case 3:
		iPointX = rect.left + ( rect.right - rect.left)/2;
		break;
	case 4:
		iPointX = rect.left + ( rect.right - rect.left)/2;
		iPointY = rect.top + (rect.bottom - rect.top) * 3 / 5;
		break;
	case 5:
		iPointX = rect.left + ( rect.right - rect.left)/5;
		iPointY = rect.top + (rect.bottom - rect.top) * 3 / 5;
		break;
	default:
		iPointX = rect.left + ( rect.right - rect.left)*3/4;
		iPointY = rect.top + (rect.bottom - rect.top) * 3 / 5;
		break;
	}
	CString strShow = "";
	strShow.Format("%d:%d",iPointX,iPointY);
	ShowMessage(strShow);
	CPoint pt;
	GetCursorPos(&pt);
	SetCursorPos(iPointX,iPointY);
	int i = 0;
	if (i500000 > 0){
		if (::IsWindowEnabled(gl_hwndbtn500000)){
			g_find.PostBtnClick(gl_hwndbtn500000);
			//��Ŀ��������
			for (i=0;i<i500000;i++)	{
				//
				::PostMessage(gl_hwndWnd, WM_LBUTTONDOWN,   MK_LBUTTON, MAKELONG(iPointX,iPointY));
				//Sleep(1);
				::PostMessage(gl_hwndWnd, WM_LBUTTONUP,     MK_LBUTTON, MAKELONG(iPointX,iPointY));
				//Sleep(1);
			}
		}
	}
	if (i100000 > 0){
		if (::IsWindowEnabled(gl_hwndbtn100000)){
			g_find.PostBtnClick(gl_hwndbtn100000);
			//��Ŀ��������
			for (i=0;i<i100000;i++)	{
				::PostMessage(gl_hwndWnd, WM_LBUTTONDOWN,   MK_LBUTTON, MAKELONG(iPointX,iPointY));
				//Sleep(1);
				::PostMessage(gl_hwndWnd, WM_LBUTTONUP,     MK_LBUTTON, MAKELONG(iPointX,iPointY));
				//Sleep(1);
			}
		}
	}
	if (i50000 > 0){
		if (::IsWindowEnabled(gl_hwndbtn50000)){
			g_find.PostBtnClick(gl_hwndbtn50000);
			//��Ŀ��������
			for (i=0;i<i50000;i++)	{
				::PostMessage(gl_hwndWnd, WM_LBUTTONDOWN,   MK_LBUTTON, MAKELONG(iPointX,iPointY));
				//Sleep(1);
				::PostMessage(gl_hwndWnd, WM_LBUTTONUP,     MK_LBUTTON, MAKELONG(iPointX,iPointY));
				//Sleep(1);
			}
		}
	}
	if (i10000 > 0){
		if (::IsWindowEnabled(gl_hwndbtn10000)){
			g_find.PostBtnClick(gl_hwndbtn10000);
			//��Ŀ��������
			for (i=0;i<i10000;i++)	{
				::PostMessage(gl_hwndWnd, WM_LBUTTONDOWN,   MK_LBUTTON, MAKELONG(iPointX,iPointY));
				//Sleep(1);
				::PostMessage(gl_hwndWnd, WM_LBUTTONUP,     MK_LBUTTON, MAKELONG(iPointX,iPointY));
				//Sleep(1);
			}
		}
	}
	if (i1000 > 0){
		if (::IsWindowEnabled(gl_hwndbtn1000)){
			g_find.PostBtnClick(gl_hwndbtn1000);
			//��Ŀ��������
			for (i=0;i<i1000;i++)	{
				::PostMessage(gl_hwndWnd, WM_LBUTTONDOWN,   MK_LBUTTON, MAKELONG(iPointX,iPointY));
				//Sleep(1);
				::PostMessage(gl_hwndWnd, WM_LBUTTONUP,     MK_LBUTTON, MAKELONG(iPointX,iPointY));
				//Sleep(1);
			}
		}
	}
	if (i500 > 0){
		if (::IsWindowEnabled(gl_hwndbtn500)){
			g_find.PostBtnClick(gl_hwndbtn500);
			//��Ŀ��������
			for (i=0;i<i500;i++)	{
				::PostMessage(gl_hwndWnd, WM_LBUTTONDOWN,   MK_LBUTTON, MAKELONG(iPointX,iPointY));
				//Sleep(1);
				::PostMessage(gl_hwndWnd, WM_LBUTTONUP,     MK_LBUTTON, MAKELONG(iPointX,iPointY));
				//Sleep(1);
			}
		}
	}
	if (i100 > 0){
		if (::IsWindowEnabled(gl_hwndbtn100)){
			g_find.PostBtnClick(gl_hwndbtn100);
			//��Ŀ��������
			for (i=0;i<i100;i++)	{
				::PostMessage(gl_hwndWnd, WM_LBUTTONDOWN,   MK_LBUTTON, MAKELONG(iPointX,iPointY));
				//Sleep(1);
				::PostMessage(gl_hwndWnd, WM_LBUTTONUP,     MK_LBUTTON, MAKELONG(iPointX,iPointY));
				//Sleep(1);
			}
		}
	}
	Sleep(100);
	SetCursorPos(pt.x,pt.y);
}
/************************************************************************/
/*  ˢ���̺߳���                                                        */
/************************************************************************/
void CLoserWindowDlg::RunFunc()
{
	CString sendStr = "";
	
	int iLoginError = 0;
	int iRet = -1;
	CWindowFind find;
	CString strTemp = "";
	if (g_hwndTopMost == NULL)
	{
		AfxMessageBox("���ȳ�ʼ����Ϸ!");
		return;
	}
	ShowMessage("��ʼ���ɹ�");
	while (!g_ExitFlag)
	{
		try
		{
			//��ʼ��ʱ��Ҫ�����:����,����,����Ϣ

			//����Ѿ�����Ϸ����,��ֱ�ӽ�����Ϸ�ж�����
			//ͨ���Ѿ���ʼ���õ�HWND������Ϸ
			//������밴ť,ֱ��������Ҫ�Ĵ���

			//�ж��Ƿ��Ѿ���30����
	/*		HWND hwndHappy = ::FindWindow("Button","����30��");
			while (!hwndHappy)
			{
				//��
				Sleep(300);
				hwndHappy = ::FindWindow("Button","����30��");
			}
			g_find.PostBtnClick(hwndHappy);
			Sleep(50);
			GetPlayDesktopWindow();
			if (gl_hwndDeskWindow == NULL)
			{
				ShowMessage("δ���뿪��30��,���½���!");
				Sleep(300);
				continue;
			}
			HWND hwndParent = ::GetParent(gl_hwndDeskWindow);
			gl_hwndGetIn = g_find.GetChildWindow(hwndParent,9);
			gl_hwndBank = g_find.GetChildWindow(hwndParent,8);
			if (!gl_hwndBank || !gl_hwndGetIn)
			{
				ShowMessage("��ȡ���ư�ťʧ��!");
				Sleep(300);
				continue;
			}
*/

			//�����ʱ�������Ϸʧ�ܵĻ�,���¿�ʼ,�˳�,�ٽ���
			gl_hwndPlayWindow = ::FindWindow("#32770",gl_PlayWindowName);
			while (!gl_hwndPlayWindow && !g_ExitFlag)
			{
				g_find.PostBtnClick(gl_hwndGetIn);
				ShowMessage("���Խ�����Ϸ");
				Sleep(1000);
				gl_hwndPlayWindow = ::FindWindow("#32770",gl_PlayWindowName);
			}
			ShowMessage("�ҵ���Ϸ����");
			CloseHuaPaiWarnWidow();
			Sleep(400);
			InitHuapaiPlay();


			//�˴��������Ƿ����趨ֵ,�������,��ͬ��
			int iBalace = GetBalance(gl_hwndMoneyList);
			strTemp.Format("��ǰ���: %d",iBalace);
			ShowMessage(strTemp);
			SetDlgItemInt(IDC_EDIT_MYBALANCE,iBalace);
			
			//��ׯ��
			if ( iBalace > g_iSaveLeftMoney)
			{
				ShowMessage("�������趨ֵ,ͬ�����Ͻ�Ǯ!");
				//ǿ��
				QuitPlayWindow();
				SaveMoneyToBank(iBalace - g_iSaveLeftMoney,g_strRoomPwd);
				continue;;			}
			else if (iBalace < g_iSaveLeftMoney)
			{
				ShowMessage("���С���趨ֵ,ͬ�����Ͻ�Ǯ!");
				//ǿ��
				QuitPlayWindow();
				GetMoneyFromBank(g_iSaveLeftMoney - iBalace,g_strRoomPwd);
				continue;
			}
			

			
			CString strShow = "";

			CString strName = "";
			int iScore = 0;
			int iWinNum = 0;
			int iPalyNum = 0;
			
			int iBalaceWhenGetOn = 0;	//��ׯʱ�����
			long lLastChangeTime = 0;
			int iOldPlayNum = 0;
			int iOldWinNum = 0;
			CString strOldName = "";
			HANDLE hProcess;
			DWORD PID;
			GetWindowThreadProcessId(gl_hwndPlayWindow, &PID);
			gl_DataAddr = 0;			
			while(gl_DataAddr == 0)
			{
				Sleep(100);
				SendMessage(WM_GETGAMEDATA,0,PID);
			}
			long lDataAddr = gl_DataAddr;//m_AddrCall->Hook((long*)&PID);
			hProcess=OpenProcess(PROCESS_ALL_ACCESS,false,PID);
			if (!hProcess)
			{
				ShowMessage("����Ϸ���̿ռ�ʧ��");
				Sleep(500);
				QuitPlayWindow();
				continue;
			}
			ReadPlayData(strName,iScore,iWinNum,iPalyNum,hProcess,lDataAddr);
			lLastChangeTime = GetTickCount();
			SetDlgItemText(IDC_EDIT_WINNERNAME,strName);
			SetDlgItemInt(IDC_EDIT_SCORE,iScore);
			SetDlgItemInt(IDC_EDIT_PLAYNUM,iPalyNum);
			SetDlgItemInt(IDC_EDIT_WINNUM,iWinNum);
			//�ս���Ϸ,��ʼ��һ������
			while(!g_ExitFlag)
			{
				//�����������ׯ
				if(g_bTimeStop)
				{
					ShowMessage("�趨���Զ�ֹͣʱ�䵽,ֹͣ");
					break;
				}
				CloseHuaPaiWarnWidow();
				//���½�����ע����
				//��ʼ���п���
				//
				//����Ϸ��
				//��ʼ����Ϣ
				//�������ׯ�б���,�ж��Ƿ����������ׯ,�������,������
				//���Ǯ���ڶ����������ʾ
				
				HWND hwndTemp = ::FindWindow("#32770",gl_PlayWindowName);
				if (!hwndTemp)
				{
					ShowMessage("��Ϸ�ѹر�,����");
					break;
				}
				if (GetTickCount() - lLastChangeTime > 3 * 60000)
				{
					ShowMessage("�������� 3 ��������Ӧ,����");
					QuitPlayWindow();
					break;
				}
				iOldPlayNum = iPalyNum;
				iOldWinNum = iWinNum;
				strOldName = strName;
				ReadPlayData(strName,iScore,iWinNum,iPalyNum,hProcess,lDataAddr);
				if (iOldPlayNum != iPalyNum)
				{
					lLastChangeTime = GetTickCount();
				}
				
				SetDlgItemText(IDC_EDIT_WINNERNAME,strName);
				SetDlgItemInt(IDC_EDIT_SCORE,iScore);
				SetDlgItemInt(IDC_EDIT_PLAYNUM,iPalyNum);
				SetDlgItemInt(IDC_EDIT_WINNUM,iWinNum);
				//�ж��Ƿ�������ׯ
				if (strName == g_strGmStr)	//���������ׯ
				{
					int iBalace = GetBalance(gl_hwndMoneyList);
					//strShow.Format("��ǰ���: %d",iBalace);
					//ShowMessage(strShow);
					SetDlgItemInt(IDC_EDIT_MYBALANCE,iBalace);
					if (strOldName != strName)	//����ׯ
					{
						iBalaceWhenGetOn = iBalace;
					}
					//ShowMessage("��ǰ��ׯ");
					strShow.Format("��ǰ��ׯ,����: %d,��ǰ���: %d",iPalyNum,iBalace);
					ShowMessage(strShow);
					//����÷ֱ���,˵���н����
					if (iOldWinNum != iWinNum)
					{
						strShow.Format("���ֵ÷�:%d",iWinNum);
						ShowMessage(strShow);
						if (iWinNum + iScore >= g_iWinnerWinMoney)
						{
							ShowMessage("Ӫ�������趨ֵ,��������");
							
							//�˴��ȴ�5��							
							int i=1;
							while(i++<5)
							{
								strShow.Format("%d ����˳���Ϸ,������!",6-i);
								ShowMessage(strShow);
								Sleep(1000);
							}
							QuitPlayWindow();
							//��֤��Ǯ����Ǯ������
							SaveMoneyToBank(iWinNum + iScore + iBalaceWhenGetOn - g_iSaveLeftMoney,g_strRoomPwd);
							i=1;
							while(i++<4)
							{
								strShow.Format("%d ����Խ�����Ϸ!",4-i);
								ShowMessage(strShow);
								Sleep(1000);
							}
							break;
						}
						Sleep(1000);
					}
					//�жϴ�ʱ�Ƿ�һ�ָ���
					//�����,���жϴ�ʱǮ�Ѿ�����100��
					//��������趨ֵ,ǿ�˴�Ǯ

					Sleep(1500);
//					ShowMessage("�ȴ�������ɼƷ�");
					//ѭ���ж�
					continue;
				}
				
				
				int iBalace = GetBalance(gl_hwndMoneyList);
				strShow.Format("��ǰ���: %d",iBalace);
				ShowMessage(strShow);
				SetDlgItemInt(IDC_EDIT_MYBALANCE,iBalace);
				//��⵽���֮ǰ�Լ���ׯ��,����ʱ����ׯ����,�򴥷�ȡǮ���ߴ�Ǯ
				if (strOldName == g_strGmStr)
				{
					//��ׯ��
					if ( iBalace > g_iSaveLeftMoney)
					{
						ShowMessage("��⵽������ׯ��,ͬ�����Ͻ�Ǯ!");
						//ǿ��
						QuitPlayWindow();
						SaveMoneyToBank(iBalace - g_iSaveLeftMoney,g_strRoomPwd);
						break;
					}
					else if (iBalace < g_iSaveLeftMoney)
					{
						ShowMessage("��⵽������ׯ��,ͬ�����Ͻ�Ǯ!");
						//ǿ��
						QuitPlayWindow();
						GetMoneyFromBank(g_iSaveLeftMoney - iBalace,g_strRoomPwd);
						break;
					}
					ShowMessage("��⵽������ׯ��,����������Ҫ��!");
				}

				if (iBalace < g_iMinBalance)
				{					
					//������ʾ
					ShowMessage("����Ѿ������趨,ȥ����ȡǮ!");
					//ǿ��
					QuitPlayWindow();
					GetMoneyFromBank(g_iSaveLeftMoney - iBalace,g_strRoomPwd);
					break;
				}
				if (iBalace >= g_iMaxBalance)
				{					
					//����������
					ShowMessage("�������趨ֵ,��ʼ��������!");
					//ǿ��
					QuitPlayWindow();
					SaveMoneyToBank(iBalace - g_iSaveLeftMoney,g_strRoomPwd);
					break;
				}

				//�ж�����Ƿ��������Լ�ȥ������ׯ,�����,��������ׯ
// 				if (iBalace >= g_iSetNum)	//���ڶ���ʱ��ʼ������ׯ
// 				{
// 
// 					//����������
// 					ShowMessage("����,��ʼ��������!");
// 					//ǿ��
// 					QuitPlayWindow();
// 					SaveMoneyToBank(200,gl_strBankPwd);
// 					break;
// 				}
				
				//ShowMessage()
				//�жϾ����Ƿ�仯 һ���仯,����ע
				if (iOldPlayNum != iPalyNum || strName != strOldName)	//�����仯��
				{
					ShowMessage("��һ����ע��ʼ,��ʼ��ע");
					CString strFlag = "";
					switch(g_iWinOrLose)
					{
					case 1:
						strFlag = "��";
						break;
					case 2:
						strFlag = "ׯ";
						break;
					case 3:
						strFlag = "ƽ";
						break;
					case 4:
						strFlag = "ͬ��ƽ";
						break;
					case 5:
						strFlag = "������";
						break;
					default:
						strFlag = "ׯ����";
						break;
					}
					strShow.Format("�� %s: %d",strFlag,g_iMinMoney);
					ShowMessage(strShow);
					Blockbet(g_iWinOrLose,g_iMinMoney);
					ShowMessage("��ע�ɹ�");
					Sleep(1000);
					//�ж��Լ��Ƿ��ڵȴ��б���
					if (iBalace < g_iSetNum)
					{
						continue;
					}
					ShowMessage("�������趨ֵ,������ׯ!");
					bool bRetEx = IsWaitting(gl_hwndWaitingList);

					/************************************************************************/
					/*int iCount =  ListView_GetItemCount(gl_hwndWaitingList);
					while(iCount++ < 4 && ListView_GetItemCount(gl_hwndWaitingList) < 1)
					{
						Sleep(500);
					}
					bool bRetExEx = false;
					iCount =  ListView_GetItemCount(gl_hwndWaitingList);
					CString strAcc = "";
					for (int i=0;i< iCount;i++)
					{
						strAcc = "";
						strAcc = GetListItemText(gl_hwndWaitingList,i,0);
						ShowMessage(strAcc);
						if (strAcc == g_strGmStr)
						{
							bRetExEx = true;
							break;
						}
					}*/
					/************************************************************************/
					if (!bRetEx)	//��������б���
					{
						if (::IsWindowEnabled(gl_hwndbtnApply))
						{
							if (g_iHaveWinTime >= g_iGetKeepMoney )
							{
								ShowMessage("��ׯ�����Ѿ��ﵽ���ô���,����ֹͣ");
								OnBnClickedBtnStop();
								return;
							}
							g_iHaveWinTime++;
							ShowMessage("������ׯ����!");
							g_find.PostBtnClick(gl_hwndbtnApply);
							Sleep(2050);
						}
						else
						{
							ShowMessage("��ʱ����������ׯ!");
							Sleep(500);
							continue;
						}
					}
					else
					{
						//ShowMessage("�Ѿ�����ׯ�ȴ�������!");
						Sleep(500);
					}					
					continue;
				}
				Sleep(70);
			}
			
		}		
		catch (...)
		{
		}
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
	g_hwndTopMost = ::FindWindow("HallMainWnd","��������");
	if (!g_hwndTopMost)
	{
		g_strAppPathFrame = g_strAppPath.Left(g_strAppPath.ReverseFind('\\'));
		g_strAppPathFrame += "\\Frame.exe";
		//��������
		HINSTANCE iRet = ShellExecute(NULL, "open", g_strAppPath, NULL, NULL, SW_SHOW);
		if ((int)iRet<32)
		{
			ShowMessage("������Ϸʧ��,��������");
			return -3;
		}
		ShowMessage("��������ɹ�");
		Sleep(2000);
		return 2;		//����2��ʾ�ظ���¼
	}
	
	hwnd = find.MyFindWindow("Button","ע�����û�");
	if (hwnd)
	{
		if (hwnd)	//��¼����Ӵ���
		{
			hwnd = ::GetParent(hwnd);
			if (hwnd)
			{
				//hwnd = pParantWnd->GetSafeHwnd();
				//���˺ſ�
				hwndChind = find.GetChildWindow(hwnd,1);	//�˺�ѡ���
				hwndChind = find.GetChildWindow(hwndChind,1);//�˺�����༭��
				//Sleep(40);
				
				find.SetEditText(hwndChind,strUser);
				Sleep(200);

				//�����
				hwndChind = find.GetChildWindow(hwnd,2);
				CString strTEXT = find.GetContrlWindowText(hwndChind);
			//	Sleep(40);
				find.SetEditText(hwndChind,strPwd);
				//��¼��ť
				hwndChind = find.GetChildWindow(hwnd,3);
				Sleep(100);
				::PostMessage(hwndChind,WM_LBUTTONDOWN,0,MAKELONG(1,1));
				Sleep(20);
				::PostMessage(hwndChind,WM_LBUTTONUP,0,MAKELONG(1,1));
				

				Sleep(1500);
				int iCount = 0;
				while (true)
				{
					if (iCount ++ > 15)
					{
						ShowMessage("��¼�쳣!");
						RestartGame();
						return 5;
					}
					hwnd = ::FindWindow("#32770","ϵͳ��Ϣ");
					if (hwnd)
					{
						ShowMessage("�˺Ż������������߱�����!");
						::PostMessage(hwnd,WM_CLOSE,0,0);
						return 5;
					}//�������б�,����ȡֵ,���Ƿ�����Ϣ

					gl_bCloseCheckWindow = false;
					CloseCheckWindow();
					if (gl_bCloseCheckWindow == true)
					{
						ShowMessage("��Ҫ��֤��!");
						return 5;
					}
					hwnd = ::FindWindow("#32770","������ʾ");
					if (hwnd)
					{
						::PostMessage(hwnd,WM_CLOSE,0,0);
					}
					hwnd = find.MyFindWindow("AfxWnd70s","pv");
					if (hwnd)
					{
						if (find.GetChildWindowNum(hwnd) > 2)
						{
							hwnd = find.MyFindWindow("Button","ע�����û�");
							if (!hwnd)
							{
								return 0;//��¼�ɹ�,ִ����һ��
							}						
						}
					}
					Sleep(750);
				}
				return 4;
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
	}
	else
	{
		BackToLoginStatus();
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
	HWND hParant = ::FindWindow("HallMainWnd","��������");
	g_hwndTopMost = hParant;

	HWND hwndWarn = ::FindWindow("#32770","ϵͳ��Ϣ");
	if (hwndWarn)
	{
		::PostMessage(hwndWarn,WM_CLOSE,0,0);
		return -3;
	}
	//CloseWarnWindow();
	HWND hwndForbbid = g_find.MyFindWindowEx("Static","�Բ��������ʺŲ��ܽ���÷���");
	if(hwndForbbid)
	{
		::PostMessage(::GetParent(hwndForbbid),WM_CLOSE,0,0);
		ShowMessage("�˺ű�����");
		return -3;
	}
	hParant = find.MyFindWindow("AfxWnd70s","pv");
	if (hParant)
	{
		hwnd = find.GetChildWindow(hParant,2);
		if (hwnd && find.GetContrlClassName(hwnd) =="SysTreeView32")	
		{
			HTREEITEM hItem = TreeView_GetRoot(hwnd);
			hItem = TreeView_GetChild(hwnd,hItem);
			HTREEITEM hChild = FindItemEx(hwnd,hItem,"�����");
			if (!hChild)
			{
				ShowMessage("û�ҵ������");
				return -1;
			}
			hChild = TreeView_GetChild(hwnd,hChild);
			hChild = FindItemEx(hwnd,hChild,"������");
			if (!hChild)
			{
				ShowMessage("û�ҵ�������");
				return -1;
			}
			hChild = TreeView_GetChild(hwnd,hChild);
			hChild = FindItemEx(hwnd,hChild,g_strGmStr);
			if (!hChild)
			{
				CString strFormat;
				strFormat.Format("û�ҵ�%s",g_strGmStr);
				ShowMessage(strFormat);
				return -1;
			}
			if (gl_bSame && gl_iSameCount < 6)
			{
				//Sleep(1000);
				//return -1;
			}

			//AfxMessageBox("�ҵ�Ŀ�귿��");
			TreeView_EnsureVisible(hwnd,hChild);
			TreeView_Select(hwnd,hChild,TVGN_CARET);
			CRect rect;
			GetTreeItemRect(hwnd,hChild,rect);
			int pX = rect.left+260;
			int pY = rect.top+168;
			Sleep(700);
			//pY = 529;
			CPoint pt;
			GetCursorPos(&pt);
			::SetCursorPos(pX,pY);
			pY = rect.top + 10;
			//pX = rect.left + 10;
			
			::PostMessage(hwnd, WM_LBUTTONDOWN,   MK_LBUTTON, MAKELONG(pX,pY));
			::PostMessage(hwnd, WM_LBUTTONUP,     MK_LBUTTON, MAKELONG(pX,pY));
			::PostMessage(hwnd,WM_LBUTTONDBLCLK,MK_LBUTTON,MAKELONG(pX,pY));
			SetCursorPos(pt.x,pt.y);
			Sleep(1000);
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

	int iLoopCount = g_iCheckGmWaitInterval * 20;
	CloseWarnWindow();
	CWindowFind find;
	CPoint pnt = DestIndexToPointEx(iIndex,iSetIndex);			//һ������
	CString strShow = "";
	pnt.x -= 7;
	pnt.y-=151;
	HWND tempHWND = g_hwndDesk;
	TCHAR   szClassName[256];     
	GetClassName(tempHWND,   szClassName,256);
	ShowMessage(szClassName);
	if ("Afx:00400000:0" == CString(szClassName))
	{//�ҵ�����
		MoveScrollBar(iIndex);
		::PostMessage(tempHWND, WM_LBUTTONDOWN, MK_LBUTTON,MAKELONG(pnt.x,pnt.y));//
		::PostMessage(tempHWND, WM_LBUTTONUP, MK_LBUTTON,MAKELONG(pnt.x,pnt.y));//
		
		//������Ϸ�����Ƿ��Ѿ�����
		Sleep(500);
		int iCount = 0;
		CString strTemp = "";
		g_hwndPlayWindow = NULL;
		while (!g_hwndPlayWindow)
		{
			HWND hwndPwd = ::FindWindow("#32770","����������");//��Ҫ��������
			HWND hwndSub = find.GetChildWindow(hwndPwd,1);
			HWND hwndEdit = find.GetChildWindow(hwndPwd,4);
			find.SetEditText(hwndEdit,g_strRoomPwd);
			find.PostBtnClick(hwndSub);
			//strTemp.Format("������Ϸ�ͻ��� �� ���ӷ���1 - �� %d �� ��",iIndex);
			g_hwndPlayWindow = ::FindWindow("Afx:00400000:b:00010011:00000000:00000000","������");
			if (g_hwndPlayWindow)	//�ҵ�������Ϸ�Ĵ���	��������С,�ƶ�,���һ�ȡ���е�С��ť
			{
				::SetWindowPos(g_hwndPlayWindow,HWND_TOP,0,0,0,0,SWP_NOSIZE|SWP_SHOWWINDOW|~SWP_NOMOVE);
				//::MoveWindow(g_hwndPlayWindow,20,120,500,400,TRUE);
				ShowMessage("���������Ʒ���");
			}
			//�˴��Ҿ����,˵����
			HWND hwndWarn = ::FindWindow("#32770","��ʾ��Ϣ");
			if (hwndWarn)
			{
				HWND hwndNotice = find.GetChildWindow(hwndWarn,4);
				if (find.GetContrlWindowText(hwndNotice) == "�Բ���������Ϸ�Ҳ����������׷����ã�")
				{
					ShowMessage("����׷�����,�˳�");
					find.PostBtnClick(find.GetChildWindow(hwndWarn,1));
					::PostMessage(hwndWarn,WM_CLOSE,0,0);
					return -88;
				}
				else
				{	
					::PostMessage(hwndWarn,WM_CLOSE,0,0);
					return -1;
				}
			}

			iCount++;
			if (iCount > iLoopCount)
			{
				ShowMessage("δ�ҵ��ҵ������Ʒ���,��ȷ�������õ����ź���λ����ȷ");
				return -1;
			}
			Sleep(50);
		}

		//�ҵ����𻨷����,���õ�ע
		//��������õ�Ӯ��,��������30��ע�İ�ť,���֮
		/*if (g_iWinOrLose == 1)
		{
			HWND hwndSet  = NULL;
			iCount = 0;
			while (!hwndSet)
			{
				iCount ++;
				if (iCount > 30)
				{
					ShowMessage("δ�ҵ����ô���,�˳�");
					break;
				}
				hwndSet = ::FindWindow("#32770","�����õ�ע");
				if (hwndSet)
				{
					HWND hwndBtn = NULL;
					strTemp.Format("%d",g_iMinMoney);
					hwndBtn = find.GetSelectChildWindow(hwndSet,strTemp);
					if (hwndBtn)
					{
						strTemp.Format("�ҵ�%d��ע",g_iMinMoney);
						ShowMessage(strTemp);
						find.PostBtnClick(hwndBtn);
						Sleep(200);
					}
					else
					{
// 						strTemp.Format("δ�ҵ�%d��ע,ȡ��һ��",g_iMinMoney);
// 						ShowMessage(strTemp);
// 						hwndBtn = find.GetChildWindow(hwndSet,1);
// 						find.PostBtnClick(hwndBtn);
// 						Sleep(200);
						hwndSet = NULL;
						break;
					}					
				}
				else
				{
					Sleep(50);
				}				
			}
			hwndSet = NULL;
			ShowMessage("δ�ҵ����ô���,Ѱ��ѯ�ʴ���");
			iCount = 0;
			while (!hwndSet)
			{
				iCount ++;
				if (iCount > 100)
				{
					ShowMessage("δ�ҵ�ѯ�ʴ���,�˳�");
					return -1;
				}
				hwndSet = find.MyFindBtnWindowEx("#32770",NULL);
				if (hwndSet)
				{
					::PostMessage(hwndSet,WM_LBUTTONDOWN,0,MAKELONG(1,1));
					Sleep(10);
					::PostMessage(hwndSet,WM_LBUTTONUP,0,MAKELONG(1,1));
					Sleep(10);
				}
				else
				{
					Sleep(50);
				}
			}
		}
		else	//������õ������
		{
			HWND hwndSet  = NULL;
			iCount = 0;
			while (!hwndSet)
			{
				iCount ++;
				if (iCount > 30)
				{
					ShowMessage("δ�ҵ�ѯ�ʴ���,�������ô���");
					break;
				}
				hwndSet = find.MyFindBtnWindowEx("#32770",NULL);
				if (hwndSet)
				{
					::PostMessage(hwndSet,WM_LBUTTONDOWN,0,MAKELONG(1,1));
					Sleep(10);
					::PostMessage(hwndSet,WM_LBUTTONUP,0,MAKELONG(1,1));
					//return 0;
				}
				else
				{
					Sleep(50);
				}
			}
			hwndSet = NULL;
			iCount = 0;
			while (!hwndSet)
			{
				iCount ++;
				if (iCount > 2)
				{
					ShowMessage("δ�ҵ����ô���,�˳�");
					return 0;
				}
				hwndSet = ::FindWindow("#32770","�����õ�ע");
				if (hwndSet)
				{
					HWND hwndBtn = NULL;
					strTemp.Format("%d",g_iMinMoney);
					hwndBtn = find.GetSelectChildWindow(hwndSet,strTemp);
					if (hwndBtn)
					{
						strTemp.Format("�ҵ�%d��ע",g_iMinMoney);
						ShowMessage(strTemp);
						find.PostBtnClick(hwndBtn);
						Sleep(200);
					}
					else
					{
						::PostMessage(hwndSet,WM_CLOSE,0,0);
						return -88;
					}
				}
				else
				{
					Sleep(50);
				}				
			}
		}*/
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
	int iPassTime = 0;
	int iGiveUpTime = 0;
	//hwnd = find.GetChildWindow(g_hwndPlayWindow,4);//find.MyFindWindow("AfxWnd70","",hParant);
	//if (true)
	{
		SetLastWaitCheckTime();
		//1������,����
		/*hwnd800 = find.GetChildWindow(hwnd,6);		//����ע
		hwnd400 = find.GetChildWindow(hwnd,7);
		hwnd200 = find.GetChildWindow(hwnd,8);
		hwnd100 = find.GetChildWindow(hwnd,9);
		hwndGiveUp = find.GetChildWindow(hwnd,4);	//����
		hwndShow = find.GetChildWindow(hwnd,2);		//����
		hwndStart = find.GetChildWindow(hwnd,10);	//��ʼ��Ϸ
		hwndGen = find.GetChildWindow(hwnd,5);		//��ע
		hwndKaipai = find.GetChildWindow(hwnd,3);	//����*/
		int iTimeJuShu = 1;
		int iAddCount = 1;
		bool bPassEnble = false;
		bool bGiveUpEnable = false;
		CloseWarnWindow();
		Sleep(200);
		if (InitPlayData()<0)
		{
			ShowMessage("��ʼ������ʧ��!");
			return 3;
		}
		while(true)
		{
		    CloseWarnWindow();
			if (g_ExitFlag)
			{
				strTemp.Format("��������˳�,�����˳�...");
				ShowMessage(strTemp);
				return 2;
			}
			HWND hParant = ::FindWindow("Afx:00400000:b:00010011:00000000:00000000","������");
			if (!hParant)
			{
				strTemp.Format("��Ϸ�����Ѿ��ر�,�˳�...");
				ShowMessage(strTemp);
				return 3;
			}
			Sleep(100);
			//����Լ��Ƿ��Ѿ��˳�����Ϸ
			if( g_hwndListView2 && GetListItemText(g_hwndListView2,0,1) == "" ) 
			{
				ShowMessage("��ȡ������Ϣʧ��..");
				return 3;
			}
			bPassEnble = false;
			bGiveUpEnable = false;
			int iRet = 0;
			if (iRet < 0) //����,�˳�
			{
				WriteLog("IsBtnEnable return value iRet < 0;",0);
				HWND hwndWarn = ::FindWindow("#32770","������");
				if (hwndWarn)
				{
					HWND hwndChild = g_find.GetChildWindow(hwndWarn,4);
					if (g_find.GetContrlWindowText(hwndChild) == "��ǰ��Ϸ���ڽ����У������ѡ���뿪�����ᰴ���ƴ���ȷ��Ҫ�˳���" )
					{
						hwndChild = g_find.GetChildWindow(hwndWarn,1);
						g_find.PostBtnClick(hwndChild);
						Sleep(2000);
						//int iBalance = GetBalanceEx(g_hwndListView1);
						//g_strBalance.Format("%d",iBalance);						
						//RestartGame();
						iRet = 3;
					}
				}
				return iRet;
			}

			if (iPassTime < 2 && bPassEnble == true)
			{
				//AfxMessageBox("PASS");
				ClickBtnPass();
				ClickBtnPass();
				ClickBtnPass();
				ClickBtnPass();
				iPassTime++;
				Sleep(500);
			}
			else if (bGiveUpEnable == true)
			{
				//AfxMessageBox("SHOW HAND");
				iGiveUpTime++;				
				ClickBtnGiveUp();
				ClickBtnGiveUp();
				ClickBtnGiveUp();
				ClickBtnGiveUp();
				Sleep(500);
			}
			
			Sleep(700);
			if (iGiveUpTime > 0)
			{
				//ǿ���˳�
				::PostMessage(g_hwndPlayWindow,WM_CLOSE,0,0);
				g_hwndPlayWindow = NULL;
				Sleep(20);
				HWND hwndWarn = ::FindWindow("#32770","������");
				if (hwndWarn)
				{
					HWND hwndChild = g_find.GetChildWindow(hwndWarn,4);
					if (g_find.GetContrlWindowText(hwndChild) == "��ǰ��Ϸ���ڽ����У������ѡ���뿪�����ᰴ���ƴ���ȷ��Ҫ�˳���" )
					{
						hwndChild = g_find.GetChildWindow(hwndWarn,1);
						g_find.PostBtnClick(hwndChild);
						Sleep(2000);
						int iBalance = GetBalanceEx(g_hwndListView1);
						g_strBalance.Format("%d",iBalance);
						RestartGame();
						return 0;
					}
				}
			}
			//�ж�
			//�жϿ�ʼ��ť�Ƿ����
			/*if(::IsWindowVisible(hwndStart))//::IsWindowEnabled(hwndStart)		//һ�ֿ�ʼ��ǰ,��ʱ��ĳ��Ƿ����
			{
				//SetLastWaitCheckTime();
				
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
					if (::IsWindowEnabled(hwndGiveUp))
					{
						find.PostBtnClick(hwndGiveUp);
						ShowMessage("����");
						iAddCount = 1;
						Sleep(20*g_iSpeed);
						continue;
					}
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
					if (g_iWinOrLose == 0)
					{
						HWND hwndSet  = NULL;
						hwndSet = find.MyFindBtnWindowEx("#32770",NULL);
						if (hwndSet)
						{
							::PostMessage(hwndSet,WM_LBUTTONDOWN,0,MAKELONG(1,1));
							Sleep(20);
							::PostMessage(hwndSet,WM_LBUTTONUP,0,MAKELONG(1,1));
							Sleep(20);
						}
						else
						{
							hwndSet  = NULL;
							hwndSet = ::FindWindow("#32770","�����õ�ע");
							if (hwndSet && ::IsWindowVisible(hwndSet))
							{
								HWND hwndBtn = NULL;
								hwndBtn = find.GetSelectChildWindow(hwndSet,strTemp);
								if (hwndBtn)
								{
									strTemp.Format("�ҵ�%d��ע",g_iMinMoney);
									ShowMessage(strTemp);
									find.PostBtnClick(hwndBtn);
								}
								else
								{
									::PostMessage(hwndSet,WM_CLOSE,0,0);
									return 0;
								}
							}
						}
					}
					else
					{
						HWND hwndSet  = NULL;
						hwndSet = ::FindWindow("#32770","�����õ�ע");
						if (hwndSet && ::IsWindowVisible(hwndSet))
						{
							HWND hwndBtn = NULL;
							hwndBtn = find.GetSelectChildWindow(hwndSet,strTemp);
							if (hwndBtn)
							{
								strTemp.Format("�ҵ�%d��ע",g_iMinMoney);
								ShowMessage(strTemp);
								find.PostBtnClick(hwndBtn);
							}
							else
							{
								Sleep(3000);
								return -1;
							}
						}
						else
						{
							hwndSet  = NULL;
							hwndSet = find.MyFindBtnWindowEx("#32770",NULL);
							if (hwndSet)
							{
								::SendMessage(hwndSet,WM_LBUTTONDOWN,0,MAKELONG(1,1));
								Sleep(20);
								::SendMessage(hwndSet,WM_LBUTTONUP,0,MAKELONG(1,1));
								Sleep(20);
							}
						}
					}
					Sleep(150);
				}
			}*/
		}
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
	Sleep(150);
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
			//ShowMessage("�ҵ��������ÿ�");
			hwnd = find.GetChildWindow(hParant,29);		//��ס����
			//hChild = find.GetChildWindow(hwnd,12);
			if (hwnd)
			{
				((CButton*)FromHandle(hwnd))->SetCheck(TRUE);
			}
			/*hChild = find.GetChildWindow(hwnd,10);
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
			}		*/	
			hwnd = find.GetChildWindow(hParant,37);
			find.PostBtnClick(hwnd);
			ShowMessage("��ס�����Ѿ�����");
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


void CLoserWindowDlg::WriteOldAcc(RegisterPack &rgPack,int iFlag)
{
	
	try
	{
		CString strAppPath = "";
		CString strContent = "";
		strContent.Format("%s,%s,%d",rgPack.strAcc,rgPack.strPwd,iFlag);
		m_mapOldAcc[rgPack.strAcc+rgPack.strPwd] = 1;
		CUserFile *pFile = CUserFile::GetInstance();
		if ( pFile )
		{
			pFile->GetAppPath(strAppPath);
		}
		strAppPath += "oldacc.txt";
		FILE *fp;
		char szfile[1000]={0};
		sprintf(szfile,strAppPath);
		fp=fopen(szfile,"a+");
		fprintf(fp,"%s\r\n",strContent);
		fclose(fp);
	}
	catch (...)
	{
	}
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
	int iFirstX = 455;
	int iFirstY = 686;
	int iOffSetY = 195;
	int iOffSetX = 180;
    int iDestX = 0;
	int iDestY = 0;
	int iRet= 0;
	int iTemp = 0;
	iTemp = 100-iIndex;
	iDestX = iFirstX - ((iTemp-1) % 3) * iOffSetX;
	//iIndex�仯
	iTemp = iIndex % 9;
	if (iTemp == 7|| iTemp == 8 ||iTemp== 0)
	{
		iDestY = iFirstY;
	}
	else if (iTemp>=4 && iTemp <=6)
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
		iOffSetX = -55;
		iOffSetY = -55;
		break;
	case 2:
		iOffSetX = -0;
		iOffSetY = -110;
		break;
	case 3:
		iOffSetX = 55;
		iOffSetY = -55;
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
	if ("Afx:00400000:0" == CString(szClassName))
	{
		if (iValue == 10)
		{
			::SendMessage(tempHWND,WM_VSCROLL,SB_TOP,1);
		}

		int iOffSet = (10 - iValue) * 294;
		::SendMessage(tempHWND,WM_VSCROLL,MAKELONG(SB_THUMBTRACK,iOffSet),1);
		return;
		//::SendMessage(tempHWND,WM_VSCROLL,SB_BOTTOM,1);
		//�ƶ����ײ�
		switch(iValue)
		{
		case 11:
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
	if (iIndex <=99 && iIndex >= 91)
	{
		iRet = 0;
	}
	else if (iIndex <=90 && iIndex >= 82)
	{
		iRet = 1;
	}
	else if (iIndex <=81 && iIndex >= 73)
	{
		iRet = 2;
	}
	else if (iIndex <=72 && iIndex >= 64)
	{
		iRet = 3;
	}
	else if (iIndex <=63 && iIndex >= 55)
	{
		iRet = 4;
	}
	else if (iIndex <=54 && iIndex >= 46)
	{
		iRet = 5;
	}
	else if (iIndex <=45 && iIndex >= 37)
	{
		iRet = 6;
	}
	else if (iIndex <=36 && iIndex >= 28)
	{
		iRet = 7;
	}
	else if (iIndex <=27 && iIndex >= 19)
	{
		iRet = 8;
	}
	else if (iIndex <=18 && iIndex >= 10)
	{
		iRet = 9;
	}
	else if (iIndex <=9 && iIndex >= 1)
	{
		iRet = 10;
	}
	else
	{
		iRet = 11;
	}
	return iRet;
}
void CLoserWindowDlg::OnBnClickedButton5()
{
	RunFunc();
}

void CLoserWindowDlg::OnBnClickedButton6()
{
	//Se
	

	//��ȡ�ⲿ����һ��ַ��ֵ 



	//gl_hwndPlayWindow = ::FindWindow("#32770",gl_PlayWindowName);
	//InitHuapaiPlay();
	
	//��������
	SaveMoneyToBank(300,"wawawo20");
	return;
// 	HWND hwndForbbid = g_find.MyFindWindowEx("Static","�Բ��������ʺŲ��ܽ���÷���");
// 	
// 	CString strTest = g_find.GetContrlWindowText((HWND)6357944);
// 	AfxMessageBox(strTest);
// 	return;

	g_hwndTopMost = ::FindWindow("HallMainWnd","��������");
	/*CloseCheckWindow();
	//BackToLoginStatus();
	return;

	bool bPassEnable = false;
	bool bGiveUpEnable = false;
	//InitPlayData();
	IsBtnEnable(g_srcPicPath,bPassEnable,bGiveUpEnable);
	return;*/
	CWindowFind find;
	HWND hwnd = find.MyFindWindow(g_strClassName,"");
	HWND hwndparent = ::GetParent(hwnd);
	if(find.GetContrlClassName(hwndparent)  == "AfxWnd42s")
	{
		::PostMessage(m_hWnd,WM_STARTGAME,(WPARAM)hwnd,0);
	}
	int i = 0;
	//int iValue = GetDlgItemInt(IDC_EDIT1);
	//MoveScrollBar(iValue);
}

void CLoserWindowDlg::StartGame(HWND hwnd)
{
	int pX = 275;
	int pY = 540;
	CPoint pt;
	GetCursorPos(&pt);
	::SetCursorPos(pX,pY);
	Sleep(20);
	::PostMessage(hwnd, WM_LBUTTONDOWN, MK_LBUTTON,MAKELONG(pX-10,pY-40 ));//
	Sleep(20);
	::PostMessage(hwnd, WM_LBUTTONUP, MK_LBUTTON,MAKELONG(pX-10,pY-40 ));//
	Sleep(20);
	SetCursorPos(pt.x,pt.y); 
}


LRESULT CLoserWindowDlg::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_STARTGAME:
		{
			HWND hwnd = (HWND)wParam;
			hwnd = g_find.MyFindWindow(g_strClassName,"");
			StartGame(hwnd);
			return 0L;
		}
		break;
	case WM_GETGAMEDATA:
		{
			long lPid = (long)lParam;
			gl_DataAddr = m_AddrCall->Hook(&lPid);
		}
		break;
	}

	return CDialog::DefWindowProc(message, wParam, lParam);
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
	conList->InsertColumn(1,"����",LVCFMT_LEFT,110);
	conList->InsertColumn(2,"����",LVCFMT_LEFT,110);
	conList->InsertColumn(3,"���",LVCFMT_LEFT,80);
	conList->InsertColumn(4,"״̬",LVCFMT_LEFT,50);
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
	//KillTimer(1);
	g_ExitFlag = 1;
	GetDlgItem(IDC_BTN_START)->EnableWindow(TRUE);
	GetDlgItem(IDC_BTN_STOP)->EnableWindow(FALSE);
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
	CString strAcc="";
	CString strPwd="";
	int   lines   =   0;   
	CStdioFile   file;
	CString   cstr;
	CFileException   fe;
	CString strFilePath = "";
	char szFilter[]="Text files(*.txt)|*.txt";
	CFileDialog dlg(1,NULL,NULL,NULL,szFilter); 
	if(dlg.DoModal()==IDOK) 
	{
		strFilePath = dlg.GetPathName();
	}
	if (strFilePath=="")
	{
		ShowMessage("��ѡ���˺��ļ�");
		return;
	}
	if(file.Open(strFilePath,CFile::modeRead,&fe))   
	{ 
		file.SeekToBegin();
		int idx1=0;
		int idx2=0;
		while (file.ReadString(cstr))
		{
			cstr+=",";
			strAcc = "";
			strPwd = "";
			idx2 =cstr.Find(",");
			if (idx2==-1)
			{
				continue;
			}
			strAcc = cstr.Left(idx2);
			cstr = cstr.Right(cstr.GetLength()-idx2-1);
			idx2 =cstr.Find(",");
			if (idx2==-1)
			{
				continue;
			}
			strPwd = cstr.Left(idx2);
			if (strAcc !="" && strPwd != "")
			{
				AddToAccList(strAcc,strPwd);
			}
		}
		file.Close();
	}
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CLoserWindowDlg::AddToAccList(CString strAcc,CString strPwd)
{
	CListCtrl *pList = &m_ListAcc;
	int nCount = pList->GetItemCount();
	CString listIndex = "";
	
	map<CString,int>::iterator it = m_mapOldAcc.find(strAcc+strPwd);
	if (it != m_mapOldAcc.end())
	{
		CString strShow = "";
		strShow.Format("%s �ظ�����",strAcc);
		ShowMessage(strShow);
		return;
	}
	m_mapOldAcc[strAcc+strPwd] = 1;
	listIndex.Format("%d",nCount);
	pList->InsertItem(nCount,"",0);
	pList->SetItemText(nCount,0,listIndex);
	pList->SetItemText(nCount,1,strAcc);
	pList->SetItemText(nCount,2,strPwd);
	RegisterPack rgPack;
	rgPack.strAcc = strAcc;
	rgPack.strPwd = strPwd;
	g_RegisterList.Add(rgPack);
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CLoserWindowDlg::OnBnClickedRadioLose()
{
	SetPalyType(2);
	/*GetDlgItem(IDC_EDIT_LOSENUM)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT_MINNUM)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT_MAXNUM)->EnableWindow(FALSE);*/
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CLoserWindowDlg::OnBnClickedRadioWin()
{
	SetPalyType(1);
	/*GetDlgItem(IDC_EDIT_LOSENUM)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_MINNUM)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_MAXNUM)->EnableWindow(TRUE);*/
}

/************************************************************************/
/* ����������Ϣ                                                         */
/************************************************************************/
int CLoserWindowDlg::SaveParams()
{
// 	if (!g_ExitFlag)
// 	{
// 		ShowMessage("�߳�������,���ܸ�������,����ֹͣ�߳�!");
// 	}
	CString strAppPath = "";
	CString strTemp = "";
	CUserFile *pFile = CUserFile::GetInstance();
	pFile->GetAppPath(strAppPath);
	GetDlgItemText(IDC_EDIT_ROOMPWD,g_strRoomPwd);
	GetDlgItemText(IDC_EDIT_EXEPATH,g_strAppPath);
	GetDlgItemText(IDC_EDIT_GMSTR,g_strGmStr);
	g_iRoomNum = GetDlgItemInt(IDC_EDIT_DESKNUM);
	g_iSetNum = GetDlgItemInt(IDC_EDIT_SETNUM);
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
	g_iWinOrLose = GetPalyType();

	g_strAppPath.Trim();
	g_strRoomPwd.Trim();
	g_strGmStr.Trim();
	if (g_strAppPath == "" || g_strRoomPwd == "" || g_strGmStr == "")
	{
		MessageBox("������������������Ϣ","����");
		return -1;
	}
	
	
	//pFile->CheckInitValue(g_iSetNum,0,3);
	pFile->CheckInitValue(g_iRoomNum,1,99);
	pFile->CheckInitValue(g_iLoseNum,1,60000);
	//pFile->CheckInitValue(g_iWinOrLose,0,1);
	//pFile->CheckInitValue(g_iMinBalance,10,1500000);
	//pFile->CheckInitValue(g_iMaxBalance,10000,100000000);
	pFile->CheckInitValue(g_iStopMinutes,1,100);
	SetDlgItemText(IDC_EDIT_ROOMPWD,g_strRoomPwd);
	SetDlgItemText(IDC_EDIT_EXEPATH,g_strAppPath);
	SetDlgItemInt(IDC_EDIT_DESKNUM,g_iRoomNum);
	SetDlgItemInt(IDC_EDIT_SETNUM,g_iSetNum);
	SetDlgItemInt(IDC_EDIT_LOSENUM,g_iLoseNum);
	SetDlgItemInt(IDC_EDIT_MINNUM,g_iMinBalance);
	SetDlgItemInt(IDC_EDIT_MAXNUM,g_iMaxBalance);


	g_iWinnerWinMoney = GetDlgItemInt(IDC_EDIT_WINNERWIN);
	g_iSaveLeftMoney = GetDlgItemInt(IDC_EDIT_SAVELEFT);
	g_iGetKeepMoney = GetDlgItemInt(IDC_EDIT_GETKEEP);

	
	int iHour = atoi(g_strAppPath.Left(g_strAppPath.Find(':')));
	int iMinute = atoi(g_strAppPath.Mid(g_strAppPath.Find(':')+1));
	if (iHour < 0 || iHour > 23 || iMinute < 0 || iMinute > 59)
	{
		MessageBox("ʱ�����÷Ƿ�","����");
		return -1;	
	}

	g_iIsCheckGm = ((CButton *)GetDlgItem(IDC_CHECK_GMCHECK))->GetCheck();
	g_iIsHidePlayWindow = m_HideWindow.GetCheck();
	pFile->CheckInitValue(g_iIsCheckGm,0,1);
	
	g_iCheckGmWait = ((CButton *)GetDlgItem(IDC_CHECK_GMCHECKWAIT))->GetCheck();
	pFile->CheckInitValue(g_iCheckGmWait,0,1);
	g_iCheckGmWaitInterval = GetDlgItemInt(IDC_EDIT_CHECKINTERVAL);
	pFile->CheckInitValue(g_iCheckGmWaitInterval,2,100);

	g_iIsSetPwd = ((CButton *)GetDlgItem(IDC_CHECK_SETPWD))->GetCheck();
	g_iMinMoney = GetDlgItemInt(IDC_EDIT_MINMONEY);

	g_strAppPathFrame = g_strAppPath.Left(g_strAppPath.ReverseFind('\\'));
	g_strAppPathFrame += "\\Frame.exe";
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
		pFile->WriteInitInfo(strAppPath +"set.ini","Fresh","minmoney","",g_iMinMoney,1);	
		pFile->WriteInitInfo(strAppPath+"set.ini","Fresh","g_iWinnerWinMoney","",g_iWinnerWinMoney,1);
		pFile->WriteInitInfo(strAppPath+"set.ini","Fresh","g_iSaveLeftMoney","",g_iSaveLeftMoney,1);
		pFile->WriteInitInfo(strAppPath+"set.ini","Fresh","g_iGetKeepMoney","",g_iGetKeepMoney,1);
	}
#ifdef ONLY_LOSER
	//GetDlgItem(IDC_RADIO_WIN)->ShowWindow(SW_HIDE);
	//g_iWinOrLose = 0;
#else
#ifdef ONLY_WINNNER
	//GetDlgItem(IDC_RADIO_LOSE)->ShowWindow(SW_HIDE);
	//g_iWinOrLose = 1;
#endif
#endif
	
	return 0;
}
/************************************************************************/
/*  ��������ļ�                                                        */
/************************************************************************/
void CLoserWindowDlg::OnBnClickedBtnFind()
{
	CString strFilePath = "";
	char szFilter[]="Exe Files(*.exe)|*.exe";
	CFileDialog dlg(1,NULL,NULL,NULL,szFilter); 
	if(dlg.DoModal()==IDOK) 
	{
		SetDlgItemText(IDC_EDIT_EXEPATH,dlg.GetPathName());
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
	strTemp.Format("������Ϸ�ͻ��� �� ���ӷ���1 - �� %d �� ��",g_iRoomNum);
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
	//RestartGame();
	//return;
	g_bNeedWaitGotoReLog = true;
	//g_hwndTopMost = ::FindWindow("HallMainWnd","��������");
	HWND hwndEx = g_find.GetChildWindow(g_hwndTopMost,1);
	if (g_find.GetChildWindowNum(hwndEx) < 2)
	{
		hwndEx = g_find.GetChildWindow(g_hwndTopMost,2);
	}
	hwndEx = g_find.GetChildWindow(hwndEx,2);
	g_find.PostBtnClick(hwndEx);
	int iCount = 0;
	while(g_bNeedWaitGotoReLog)
	{
		if (iCount ++ > 50)
		{
			break;
		}
		//�������µ�¼�Ĵ���
		Sleep(300);
		EnumChildWindows(NULL,EnumChildProcReLogin,(LPARAM)0);
	}
	return;
	if (iCount <= 50)
	{
		return;
	}
	//�������ھ���õ�����ID,��������
	try
	{
		DWORD PID;
		HANDLE hProcess;
		GetWindowThreadProcessId(g_hwndTopMost, &PID);
		hProcess=OpenProcess(PROCESS_ALL_ACCESS,false,PID);
		if (hProcess)
		{
			::TerminateProcess(hProcess,0);
			CloseHandle(hProcess);
		}		
	}
	catch (...)
	{
	}	
	Sleep(100);
}

void CLoserWindowDlg::CloseCheckWindow()
{
	
	EnumChildWindows(NULL,EnumChildProcCheckNum,(LPARAM)0);
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
	int iCount = 0;
	while(iCount++ < 15 && ListView_GetItemCount(hwnd) < 1)
	{
		Sleep(500);
	}
	CString strRet = GetListItemText(hwnd,0,1);
	return atoi(strRet);
}

/************************************************************************/
/* ��ѯ�˺����                                                         */
/************************************************************************/
int CLoserWindowDlg::GetBalanceEx(HWND hwnd)
{
	CString strRet = GetListItemText(hwnd,0,2);
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
	//g_hwndListView2 = NULL;
	g_hwndDesk = NULL;

	g_hwndSet = find.GetChildWindow(hwndTopParant,5);//����
	g_hwndBack = find.GetChildWindow(hwndTopParant,4);//�ر����
	hwndTopParant = ::GetParent(hwndTopParant);

	hwndChild = find.GetChildWindow(hwndTopParant,4);
	g_hwndDesk = find.GetChildWindow(hwndChild,1);

	hwndChild = find.GetChildWindow(hwndTopParant,5);
	g_hwndListView1 = find.GetChildWindow(hwndChild,1);

	if (!g_hwndBack || !g_hwndSet || !g_hwndListView1 || !g_hwndDesk)
	{
		g_hwndSet = NULL;
		g_hwndBack = NULL;
		g_hwndListView1 = NULL;
		//g_hwndListView2 = NULL;
		g_hwndDesk = NULL;
		return -1;
	}
	if (find.GetContrlClassName(g_hwndBack) != "Button" ||
		find.GetContrlClassName(g_hwndSet) != "Button" ||
		find.GetContrlClassName(g_hwndListView1) != "SysListView32" ||
		find.GetContrlClassName(g_hwndDesk) != "Afx:00400000:0")
	{
		g_hwndSet = NULL;
		g_hwndBack = NULL;
		g_hwndListView1 = NULL;
		//g_hwndListView2 = NULL;
		g_hwndDesk = NULL;
		return -1;
	}
	return 0;
}
void CLoserWindowDlg::OnBnClickedButton8()
{
	GetPower();
}

void CLoserWindowDlg::ReadPlayData(CString &strName,int &iScore,int &iWinMoney,int &iPlayNum, HANDLE &hProcess,long lDataAddr)
{
	strName = "";
	iScore = 0;
	iWinMoney = 0;
	iPlayNum = 0; 
	SIZE_T size = 0;
	ReadProcessMemory(hProcess, (LPVOID)(lDataAddr + 0x1F58), (LPVOID)&iWinMoney, 4, &size);
	ReadProcessMemory(hProcess, (LPVOID)(lDataAddr + 0x1F48), (LPVOID)&iScore, 4, &size);
	ReadProcessMemory(hProcess, (LPVOID)(lDataAddr + 0x1F46), (LPVOID)&iPlayNum, 2, &size);	
	char ItemBuf[512] = {0};
	short uValue = 0;
	ReadProcessMemory(hProcess, (LPVOID)(lDataAddr + 0x1F44), (LPVOID)&uValue, 2, &size);
	if (uValue >= 0)
	{
		int iValue = 0;
		ReadProcessMemory(hProcess, (LPVOID)(lDataAddr + 0x184), (LPVOID)&iValue, 4, &size);
		int iValueEx = 0;
		ReadProcessMemory(hProcess, (LPVOID)(uValue*4 + iValue), (LPVOID)&iValueEx, 4, &size);
		iValueEx = iValueEx + 0x20;
		memset(ItemBuf,0,512);
		ReadProcessMemory(hProcess, (LPVOID)(iValueEx), (LPVOID)ItemBuf, 512, &size);
		strName = CString(ItemBuf);
	}
	else
	{
		ShowMessage("δ��ȡ����ǰ��ׯ��");
	}
}
void CLoserWindowDlg::OnBnClickedButton9()
{

	int ii = 0x738B;
	SaveParams();
	//int iFlag =  GetPalyType();
	//int iMoney = GetDlgItemInt(IDC_EDIT_MINMONEY);
	gl_hwndPlayWindow = ::FindWindow("#32770",gl_PlayWindowName);
	InitHuapaiPlay();
	//return;
	Blockbet(GetPalyType(),GetDlgItemInt(IDC_EDIT_MINMONEY));
	return;
	InitHuapaiPlay();
	//bool bRetExEx = IsWaitting(gl_hwndWaitingList);
	int iCount =  ListView_GetItemCount(gl_hwndWaitingList);
	while(iCount++ < 4 && ListView_GetItemCount(gl_hwndWaitingList) < 1)
	{
		Sleep(500);
	}
	bool bRetExEx = false;
	iCount =  ListView_GetItemCount(gl_hwndWaitingList);
	CString strAcc = "";
	for (int i=0;i< iCount;i++)
	{
		strAcc = "";
		strAcc = GetListItemText(gl_hwndWaitingList,i,0);
		ShowMessage(strAcc);
		if (strAcc == gl_strRunAcc)
		{
			bRetExEx = true;
			break;
		}
	}
	bool SSS = bRetExEx;
	return;
	DWORD PID;
	HANDLE hProcess;
	gl_hwndPlayWindow = ::FindWindow("#32770",gl_PlayWindowName);
	GetWindowThreadProcessId(gl_hwndPlayWindow, &PID);
	char ItemBuf[512] = {0};
	long lRet = m_AddrCall->Hook((long*)&PID);

	hProcess=OpenProcess(PROCESS_ALL_ACCESS,false,PID);
	if (!hProcess)
	{
		//��ȡ���̾������ʧ��
	}
	else
	{
		int iValue = 0;
		SIZE_T size = 0;
		/*ReadProcessMemory(hProcess, (LPVOID)(lRet + 0x1F58), (LPVOID)&iValue, 4, &size);
		sprintf_s(ItemBuf,"%d",iValue);
		AfxMessageBox(ItemBuf);

		ReadProcessMemory(hProcess, (LPVOID)(lRet + 0x1F48), (LPVOID)&iValue, 4, &size);
		sprintf_s(ItemBuf,"%d",iValue);
		AfxMessageBox(ItemBuf);

		short uValue = 0;
		ReadProcessMemory(hProcess, (LPVOID)(lRet + 0x1F46), (LPVOID)&uValue, 2, &size);
		sprintf_s(ItemBuf,"%d",uValue);
		AfxMessageBox(ItemBuf);
*/
		short uValue = 0;
		ReadProcessMemory(hProcess, (LPVOID)(lRet + 0x1F44), (LPVOID)&uValue, 2, &size);
		if (uValue > 0)
		{
			iValue = 0;
			ReadProcessMemory(hProcess, (LPVOID)(lRet + 0x184), (LPVOID)&iValue, 4, &size);
			int iValueEx = 0;
			ReadProcessMemory(hProcess, (LPVOID)(uValue*4 + iValue), (LPVOID)&iValueEx, 4, &size);
			iValueEx = iValueEx + 0x20;
			memset(ItemBuf,0,512);
			ReadProcessMemory(hProcess, (LPVOID)(iValueEx), (LPVOID)ItemBuf, 512, &size);
			AfxMessageBox(ItemBuf);
		}
		else
		{
			AfxMessageBox("�û�������");
		}
		/*
		UserKey = rPoit(AddrBase + &H1F44, 2)'�û�id		��� UserKey > 0 		rPoit(rPoit(AddrBase + &H184) + UserKey * 4) + &H20
		*/
		ReadProcessMemory(hProcess, (LPVOID)(lRet + 0x1F46), (LPVOID)&uValue, 2, &size);
		sprintf_s(ItemBuf,"%d",uValue);
	//	AfxMessageBox(ItemBuf);
// 		memset(ItemBuf,0,512);
// 		ReadProcessMemory(hProcess, (LPVOID)0x0040C254, ItemBuf, 512, &size);
	//	AfxMessageBox(ItemBuf);
	}
	return;

	int iFlag = ((CButton*)GetDlgItem(IDC_RADIO_LOSE))->GetCheck();
	int iMoney = GetDlgItemInt(IDC_EDIT_MINMONEY);
	InitHuapaiPlay();
	//return;
	Blockbet(iFlag,iMoney);
	return;
	//GetMoneyFromBank(300,"wawawo20");
	//"�ټ�����Ϸ  --  Ver��6.0.1.0");
	//char ItemBuf[512] = {0};
	//DWORD PID;
	//HANDLE hProcess;
	gl_hwndPlayWindow = ::FindWindow("#32770",gl_PlayWindowName);
	GetWindowThreadProcessId(gl_hwndPlayWindow, &PID);
	hProcess=OpenProcess(PROCESS_ALL_ACCESS,false,PID);
	if (!hProcess)
	{
		//��ȡ���̾������ʧ��
	}
	else
	{
		SIZE_T size = 0;
		ReadProcessMemory(hProcess, (LPVOID)0x0040C25C, ItemBuf, 512, &size);
		AfxMessageBox(ItemBuf);
		memset(ItemBuf,0,512);
		ReadProcessMemory(hProcess, (LPVOID)0x0040C254, ItemBuf, 512, &size);
		AfxMessageBox(ItemBuf);

		int ii = 0;
// 		plvitem=(LVITEM*)VirtualAllocEx(hProcess, NULL, sizeof(LVITEM), MEM_COMMIT, PAGE_READWRITE);
// 		pItem=(char*)VirtualAllocEx(hProcess, NULL, 512, MEM_COMMIT, PAGE_READWRITE);
// 
// 		if ((!plvitem)||(!pItem))
// 		{
// 			//�޷������ڴ�
// 		}
// 		else    //��ȡ������
// 		{
// 			lvitem.cchTextMax=512;
// 			lvitem.iSubItem=0; //ProcessName
// 			lvitem.pszText=pItem;
// 			WriteProcessMemory(hProcess, plvitem, &lvitem, sizeof(LVITEM), NULL);
// 			::SendMessage(hwnd,LVM_GETITEMTEXT, (WPARAM)iItem,(LPARAM)plvitem);
// 			ReadProcessMemory(hProcess, pItem, ItemBuf, 512, NULL);
// 		}
	}
}
/************************************************************************/
/*�ر��ȼ�                                                              */
/************************************************************************/
void CLoserWindowDlg::UnloadHook()
{
	if(m_KeyHook.hKeyBoardHook  != NULL)
		UnhookWindowsHookEx(m_KeyHook.hKeyBoardHook);
	if(m_KeyHook.hLowKeyBoardHook != NULL)
		UnhookWindowsHookEx(m_KeyHook.hLowKeyBoardHook);
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
	//m_KeyHook = SetHookEx(m_hWnd);
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
/*������,���ҵ�һ������Ϊ����ֵ�Ľڵ�,����                              */
/************************************************************************/
HTREEITEM CLoserWindowDlg::FindItemEx(HWND hwnd,HTREEITEM hItem,CString strText)   
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
	hItem= TreeView_GetNextItem(hwnd,hItem,TVGN_NEXT);
	if (!hItem)
	{
		return NULL;
	}
	hFind = FindItemEx(hwnd,hItem,strText);
	return hFind;


	HTREEITEM hChild = TreeView_GetChild(hwnd,hItem);
	if(hChild)
	{
		hItem = hChild;   
		hFind = FindItemEx(hwnd,hItem,strText);
		if(hFind)   
		{   
			return hFind;
		}
		else   
		{			
			hItem= TreeView_GetNextItem(hwnd,TreeView_GetParent(hwnd,hItem),TVGN_NEXT);
			if (hItem)
			{
				return FindItemEx(hwnd,hItem,strText);
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
		hFind = FindItemEx(hwnd,hItem,strText);
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
	GetDlgItem(IDC_EDIT_GMSTR)->EnableWindow(TRUE);
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
			pList->SetItemText(i,3,g_strBalance);
			pList->SetItemText(i,4,iFlag==0?"�ɹ�":"ʧ��");
			break;
		}
	}
	if (iFlag == 0)		//д��ˢ����ɵĺ����ļ�
	{
		CString strShow = "";
		strShow.Format("ˢ�����: %s,%s",rgPack.strAcc,g_strBalance);
		WriteLog(strShow,1);
	}
	WriteOldAcc(rgPack, iFlag);
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
//	return 0;
	CInternetSession session;
	CString strTemp = "";
	try
	{
		CStdioFile *pFile = session.OpenURL("http://blog.163.com/shaojun_hnist/blog/static/1387171242011115387766/");
		if (pFile != NULL)
		{
			while(pFile->ReadString(strTemp))
			{
				strTemp.MakeLower();
				if (strTemp.Find("myownpowerhuapaicontrol") !=-1)
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

int CLoserWindowDlg::InitPlayData()
{
	//���Ҵ���
	g_hwndListView2 = NULL;
	g_hwndPlayContrl = NULL;
	g_hwndPlayContrl = g_find.MyFindWindow(g_strClassName,"");
	if (!g_hwndPlayContrl)
	{
		ShowMessage("���Ҵ���ʧ��");
		return -1;
	}
	Sleep(1000);
	//��ȡͼ��
	HWND hwndparent = ::GetParent(g_hwndPlayContrl);
	if(g_find.GetContrlClassName(hwndparent)  == "AfxWnd42s")
	{
		::PostMessage(m_hWnd,WM_STARTGAME,(WPARAM)g_hwndPlayContrl,0);
		/*
		int pX = 275;
		int pY = 540;
		CPoint pt;
		GetCursorPos(&pt);
		::SetCursorPos(pX,pY);
		Sleep(20);
		::PostMessage(g_hwndPlayContrl, WM_LBUTTONDOWN, MK_LBUTTON,MAKELONG(pX-10,pY-40 ));//
		Sleep(20);
		::PostMessage(g_hwndPlayContrl, WM_LBUTTONUP, MK_LBUTTON,MAKELONG(pX-10,pY-40 ));//
		Sleep(20);
		SetCursorPos(pt.x,pt.y);*/
	}
	else
	{
		return -1;
	}
	
	HWND hwnd = g_find.GetChildWindow(g_hwndPlayWindow,4);
	hwnd = g_find.GetChildWindow(hwnd,4);
	hwnd = g_find.GetChildWindow(hwnd,1);
	if (hwnd && g_find.GetContrlClassName(hwnd) == "SysListView32")
	{
		int iCount = ListView_GetItemCount(hwnd);
		if (iCount < 1)
		{
			ShowMessage("��ȡ������Ϣʧ��");
			return -1;
		}
		if( GetListItemText(hwnd,0,1) == "" ) 
		{
			ShowMessage("��ȡ������Ϣʧ��");
			return -1;
		}
		g_hwndListView2 = hwnd;

	}
	return 0;
}

void CLoserWindowDlg::ClickBtnPass()
{
	int pX = 605;
	int pY = 640;
	CPoint pt;
	GetCursorPos(&pt);
	::SetCursorPos(pX,pY);
	Sleep(20);
	::PostMessage(g_hwndPlayContrl, WM_LBUTTONDOWN, MK_LBUTTON,MAKELONG(pX-10,pY-40 ));//
	Sleep(20);
	::PostMessage(g_hwndPlayContrl, WM_LBUTTONUP, MK_LBUTTON,MAKELONG(pX-10,pY-40 ));//
	Sleep(20);
	SetCursorPos(pt.x,pt.y);
}

void CLoserWindowDlg::ClickBtnGiveUp()
{
	int pX = 605;
	int pY = 670;
	CPoint pt;
	GetCursorPos(&pt);
	::SetCursorPos(pX,pY);
	Sleep(20);
	::PostMessage(g_hwndPlayContrl, WM_LBUTTONDOWN, MK_LBUTTON,MAKELONG(pX-10,pY-40 ));//
	Sleep(20);
	::PostMessage(g_hwndPlayContrl, WM_LBUTTONUP, MK_LBUTTON,MAKELONG(pX-10,pY-40 ));//
	Sleep(20);
	SetCursorPos(pt.x,pt.y);
}
/************************************************************************/
/* 
��ʼX:571
��ʼY:
�ֱ�:606  667
��5��һ����                                                                      */
/************************************************************************/
//���Դͼ����в��

void CLoserWindowDlg::RestartGame()
{
	try
	{
		DWORD PID;
		HANDLE hProcess;
		g_hwndTopMost = ::FindWindow("HallMainWnd","��������");
		if (!g_hwndTopMost)
		{
			return;
		}
		//::PostMessage(g_hwndTopMost,WM_CLOSE,0,0);
		//Sleep(300);
// 		if(!::IsWindow(g_hwndTopMost))
// 		{
// 			return;
// 		}
		GetWindowThreadProcessId(g_hwndTopMost, &PID);
		hProcess=OpenProcess(PROCESS_ALL_ACCESS,false,PID);
		if (hProcess)
		{
			::TerminateProcess(hProcess,0);
			CloseHandle(hProcess);
		}		
	}
	catch (...)
	{
	}
}

void CLoserWindowDlg::CloseWarnWindow()
{
	HWND hwndWarn = ::FindWindow("#32770","ϵͳ��Ϣ");
	if (hwndWarn)
	{
		::PostMessage(hwndWarn,WM_CLOSE,0,0);
	}
}

void CLoserWindowDlg::LoadOldAcc()
{
	CString strAppPath = "";
	CUserFile *pFile = CUserFile::GetInstance();
	if ( pFile )
	{
		pFile->GetAppPath(strAppPath);
	}
	CString strFilePath = strAppPath + "oldacc.txt";
	CStdioFile   file;
	CString   cstr;
	CFileException   fe;
	if(file.Open(strFilePath,CFile::modeRead,&fe))   
	{ 
		file.SeekToBegin();
		int idx1=0;
		int idx2=0;
		srand(time(NULL));
		CString strAcc = "";
		CString strPwd = "";
		while (file.ReadString(cstr))
		{
			strAcc = "";
			strPwd = "";
			idx2 =cstr.Find(",");
			if (idx2==-1)
			{
				continue;
			}
			strAcc = cstr.Left(idx2);
			cstr = cstr.Mid(idx2+1);
			idx2 =cstr.Find(",");
			if (idx2==-1)
			{
				continue;
			}
			strPwd = cstr.Left(idx2);
			m_mapOldAcc[strAcc+strPwd] = 1;
		}
	}
}

bool CLoserWindowDlg::IsPlayerOut()
{
	int iCount = 0;
	while(iCount++ < 15 && ListView_GetItemCount(g_hwndListView2) < 1)
	{
		Sleep(500);
	}
	return iCount >= 14;
}
//��ʼʼ����Ϸ״̬
void CLoserWindowDlg::OnBnClickedBtnInit()
{
	g_hwndTopMost = ::FindWindow("HPGamePlaza","��Ϸ����");
	if (!g_hwndTopMost)
	{
		ShowMessage("��Ϸδ����!");
		return;
	}
	
	//GetPlayDesktopWindow();
	gl_hwndDeskWindow = NULL;
	HWND hwndTemp = g_find.GetChildWindow(g_hwndTopMost,30);

	hwndTemp = g_find.GetChildWindow(hwndTemp,20);

	if (g_find.GetContrlClassName(hwndTemp) == "AfxWnd70")
	{
		ShowMessage("�ҵ�����Ϊ��AfxWnd70���Ĵ���");
		//�жϴ��ڴ�С 217x260
		RECT rect;
		::GetWindowRect(hwndTemp,&rect);
		int iWidth = rect.right - rect.left;
		int iHeight = rect.bottom-rect.top;
		CString strShow = "";
		strShow.Format("Width:%d  Height:%d",iWidth,iHeight);
		ShowMessage(strShow);
		gl_hwndDeskWindow = hwndTemp;
		if (g_find.GetChildWindowCount(hwndTemp))
		{
			ShowMessage("���ڴ�С���Ϲ���");
			gl_hwndDeskWindow = hwndTemp;
		}
	}
	if (gl_hwndDeskWindow == NULL)
	{
		ShowMessage("δ���뿪��30��,���½���!");
		//Sleep(300);
		return;
	}
	HWND hwndParent = ::GetParent(gl_hwndDeskWindow);
	gl_hwndGetIn = g_find.GetChildWindow(hwndParent,9);
	gl_hwndBank = g_find.GetChildWindow(hwndParent,8);
	if (!gl_hwndBank || !gl_hwndGetIn)
	{
		ShowMessage("��ȡ���ư�ťʧ��!");
		//Sleep(300);
		return;
	}
	//25
	gl_hwndRoomList = g_find.GetChildWindow(g_hwndTopMost,25);
	ShowMessage("��Ϸ���ݳ�ʼ���ɹ�!");
	gl_hwndPlayWindow = ::FindWindow("#32770",gl_PlayWindowName);
//	g_ExitFlag = false;
	//9  ����
	//8����
}

bool CLoserWindowDlg::InitHuapaiPlay()
{
	//15���Ӵ���
	//13���Ӵ���
	// 11 ��1������ׯ�ĵȴ��б�

	::SetWindowPos(gl_hwndPlayWindow,HWND_TOP,0,0,0,0,SWP_NOSIZE|SWP_SHOWWINDOW|~SWP_NOMOVE);
	::MoveWindow(gl_hwndPlayWindow,45,3,1069,737,TRUE);
	int iOffset1 = 1;
	int iOffset2 = 1;
	HWND hwnd = g_find.GetChildWindow(gl_hwndPlayWindow,3);
	
	gl_hwndMoneyList = g_find.GetChildWindow(hwnd,10);
	hwnd = g_find.GetChildWindow(gl_hwndPlayWindow,4);
	if (!hwnd)
	{
		ShowMessage("��ʼ����Ϸ����ʧ��");
		return false;
	}
	gl_hwndWnd = hwnd;
	int iCount = g_find.GetChildWindowCount(hwnd);
	if (iCount == 13)
	{
		iOffset1 = 0;
		iOffset2 = 0;
	}
	else
	{
		iOffset1 = 1;
		iOffset2 = 2;		
	}
	gl_hwndbtnApply = g_find.GetChildWindow(hwnd,3 + iOffset1 );	//
	gl_hwndbtn500000 = g_find.GetChildWindow(hwnd,4 + iOffset1 );	//
	gl_hwndbtn50000 = g_find.GetChildWindow(hwnd,5 + iOffset1 );	//
	gl_hwndbtn500 = g_find.GetChildWindow(hwnd,6 + iOffset1 );	//
	gl_hwndbtn100000 = g_find.GetChildWindow(hwnd,7 + iOffset1 );	//
	gl_hwndbtn10000 = g_find.GetChildWindow(hwnd,8 + iOffset1 );	//
	gl_hwndbtn1000 = g_find.GetChildWindow(hwnd,9 + iOffset1 );	//
	gl_hwndbtn100 = g_find.GetChildWindow(hwnd,10 + iOffset1 );	//

	HWND hwndChild = g_find.GetChildWindow(hwnd,11 + iOffset2 );	//
	gl_hwndWaitingList = g_find.GetChildWindow(hwndChild,1);
	ShowMessage("��ʼ����Ϸ���ڲ����ɹ�");
	return true;
}

bool CLoserWindowDlg::IsWaitting(HWND hwnd)
{
	int iii = 0;
	//gl_hwndWaitingList

	int iCount = 0;
	iCount =  ListView_GetItemCount(hwnd);
	while(iCount++ < 4 && ListView_GetItemCount(hwnd) < 1)
	{
		Sleep(500);
	}
	bool bRet = false;
	iCount =  ListView_GetItemCount(hwnd);
	CString strAcc = "";
	for (int i=0;i< iCount;i++)
	{
		strAcc = "";
		strAcc = GetListItemText(hwnd,i,0);
		//ShowMessage(strAcc);
		if (strAcc == g_strGmStr)
		{
			CString strShow = "";
			strShow.Format("��ǰ����ׯ�ȴ����е� %d λ",i+1);
			ShowMessage(strShow);
			bRet = true;
			break;
		}
	}
	return bRet;
}
//�Ƿ�����ׯ
bool CLoserWindowDlg::IsBoss()
{
	return false;
}
void CLoserWindowDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (nIDEvent == 1)
	{
// 		int iHour = atoi(g_strAppPath.Left(g_strAppPath.Find(':')));
// 		int iMinute = atoi(g_strAppPath.Mid(g_strAppPath.Find(':')));
// 		CTime dt = CTime::GetCurrentTime();
// 		if( dt.GetHour() >= iHour && dt.GetMinute()>=iMinute)
// 		{
// 			//ʱ�䵽
// 			KillTimer(1);
// 			ShowMessage("��ʱֹͣʱ�䵽,ֹͣ�߳�");
// 			g_bTimeStop = true;
// 			OnBnClickedBtnStop();
// 		}
	}
	CDialog::OnTimer(nIDEvent);
}

void CLoserWindowDlg::OnEnChangeEditPlaynum()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ�������������
	// ���͸�֪ͨ��������д CDialog::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}

void CLoserWindowDlg::OnEnChangeEditSetnum2()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ�������������
	// ���͸�֪ͨ��������д CDialog::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}

void CLoserWindowDlg::OnEnChangeEditScore()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ�������������
	// ���͸�֪ͨ��������д CDialog::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}

void CLoserWindowDlg::OnEnChangeEditMybalance()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ�������������
	// ���͸�֪ͨ��������д CDialog::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}

void CLoserWindowDlg::OnEnChangeEditGetkeep()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ�������������
	// ���͸�֪ͨ��������д CDialog::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}

int CLoserWindowDlg::GetPalyType()
{
	if (((CButton*)GetDlgItem(IDC_RADIO_WIN))->GetCheck()==1)		//��
	{
		return 1;
	}
	if (((CButton*)GetDlgItem(IDC_RADIO_LOSE))->GetCheck())	//ׯ
	{
		return 2;
	}
	if (((CButton*)GetDlgItem(IDC_RADIO_PEASE))->GetCheck())	//ƽ
	{
		return 3;
	}
	if (((CButton*)GetDlgItem(IDC_RADIO_PEASESAME))->GetCheck())//ͬ��ƽ
	{
		return 4;
	}
	if (((CButton*)GetDlgItem(IDC_RADIO_WINLEFT))->GetCheck())//������
	{
		return 5;
	}
	if (((CButton*)GetDlgItem(IDC_RADIO_WINRIGHT))->GetCheck())//ׯ����
	{
		return 6;
	}
	return 6;
}

void CLoserWindowDlg::SetPalyType(int iFlag)
{
	((CButton*)GetDlgItem(IDC_RADIO_WIN))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_RADIO_LOSE))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_RADIO_PEASE))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_RADIO_PEASESAME))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_RADIO_WINLEFT))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_RADIO_WINRIGHT))->SetCheck(FALSE);
	switch(iFlag)
	{
	case 1:
		((CButton*)GetDlgItem(IDC_RADIO_WIN))->SetCheck(TRUE);
		break;
	case 2:
		((CButton*)GetDlgItem(IDC_RADIO_LOSE))->SetCheck(TRUE);
		break;
	case 3:
		((CButton*)GetDlgItem(IDC_RADIO_PEASE))->SetCheck(TRUE);
		break;
	case 4:
		((CButton*)GetDlgItem(IDC_RADIO_PEASESAME))->SetCheck(TRUE);
		break;
	case 5:
		((CButton*)GetDlgItem(IDC_RADIO_WINLEFT))->SetCheck(TRUE);
		break;
	default:
		((CButton*)GetDlgItem(IDC_RADIO_WINRIGHT))->SetCheck(TRUE);
		break;
	}
}

void CLoserWindowDlg::OnBnClickedRadioPease()
{
	SetPalyType(3);
}

void CLoserWindowDlg::OnBnClickedRadioPeasesame()
{
	SetPalyType(4);
}

void CLoserWindowDlg::OnBnClickedRadioWinleft()
{
	SetPalyType(5);
}

void CLoserWindowDlg::OnBnClickedRadioWinright()
{
	SetPalyType(6);
}

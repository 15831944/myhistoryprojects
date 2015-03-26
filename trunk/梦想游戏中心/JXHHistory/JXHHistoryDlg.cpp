// JXHHistoryDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "JXHHistory.h"
#include "JXHHistoryDlg.h"
#include "ListCtrlColore.h"
#include ".\jxhhistorydlg.h"
#include "ADOFile\AdoRecordSet.h"
#include "UserFile.h"
#include <io.h>
#include "datatype.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#include <afxmt.h>
#include <afxtempl.h>
#include "afxinet.h"
using namespace std;


CListCtrl g_ShowResult;		//��ʷ��ʾ�б�
CListCtrlColore g_ShowMsg;			//��Ϣ��ʾ�б�
BOOL g_bExitFlag = TRUE;			//�߳����п���
CCriticalSection g_ShowLock;		//��Ϣ��ʾ��
CString g_dbServer = "jxhrecord.mdb";//���ݿ�
CString g_dbPwd = "";				//���ݿ�����
CString g_strCurrRecord = "";		//��ǰ��Ӯ���
CString g_strCurrRecordList = "";	//��ǰ��ʷ��¼��
int g_iLastRecordID = 0;			//���µļ�¼ID
CString g_strAppPath = "";
CString g_srcPicPath = "srcpic.bmp";
CString g_dstPicPath11 = "dstpic11.bmp";
CString g_dstPicPath12 = "dstpic12.bmp";
CString g_dstPicPath13 = "dstpic13.bmp";
CString g_cmpPicPath1 = "cmppic1.bmp";
CString g_srcPicPath2 = "dstpic2.bmp";
CString g_cmpPicPath2 = "olddstpic2.bmp";
CCriticalSection g_QueryFlagLock;		//��Ϣ��ʾ��
int g_iQueryFlag = 0;
int g_iAutoQueryFlag = 0;

int g_iQuerySleep = 1000;		//ƥ���߳���ͣ���
int g_iGotSleep = 6000;	//��ȡ�߳��ڻ�ȡ������ʱ��ͣ
int g_iGetWaitSleep = 3000;	//��ȡ�̵߳ȴ����

CString g_strClassName = "";
CString g_strWindowName = "";

int g_iMinNum = 0;
void ShowMessageEx(CString strMsg);
void ShowMessage(CString strMsg)
{
	g_ShowLock.Lock();
	ShowMessageEx(strMsg);
	g_ShowLock.Unlock();
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void ShowMessageEx(CString strMsg)
{
	CString msgBuf = strMsg;
	int nCount = g_ShowMsg.GetItemCount();
	if ( nCount >= 100 )
	{
		for (int i=0;i<90;i++)
		{
			g_ShowMsg.DeleteItem(0);
		}
	}
	nCount = g_ShowMsg.GetItemCount();
	CString strShow  = "";
	if (nCount > 0)
	{
		strShow= g_ShowMsg.GetItemText(nCount-1,1);
	}	
	if (msgBuf == strShow)
	{
		g_ShowMsg.SetItemText( nCount-1 , 0, CTime::GetCurrentTime().Format("%Y-%m-%d %H:%M:%S") );
		::PostMessage(g_ShowMsg.m_hWnd,WM_VSCROLL,SB_BOTTOM,0);
		return;
	}
	g_ShowMsg.InsertItem( nCount, "");
	if ( nCount % 2 == 0 )
	{
		g_ShowMsg.SetRowSelectedBgColor(nCount, RGB(0,0,0) , false);
		g_ShowMsg.SetRowTxtColor(nCount , RGB(255,0,0) , false);
	}
	else
	{
		g_ShowMsg.SetRowTxtColor(nCount , RGB(0,0,255) , false);
	}
	g_ShowMsg.SetRowBgColor(nCount, RGB(230,190,220), false);
	g_ShowMsg.SetRowStyle(nCount,  LIS_TXTCOLOR | LIS_BGCOLOR);

	g_ShowMsg.SetItemText( nCount , 0, CTime::GetCurrentTime().Format("%Y-%m-%d %H:%M:%S") );
	g_ShowMsg.SetItemText( nCount , 1, msgBuf );
	::PostMessage(g_ShowMsg.m_hWnd,WM_VSCROLL,SB_BOTTOM,0);
	if (msgBuf != "�ȴ���...")
	{
		//	WriteLog(msgBuf,2);
	}
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


// CJXHHistoryDlg �Ի���



CJXHHistoryDlg::CJXHHistoryDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CJXHHistoryDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CJXHHistoryDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_ShowResult, g_ShowResult);
	DDX_Control(pDX, IDC_LIST_MSG, g_ShowMsg);
}

BEGIN_MESSAGE_MAP(CJXHHistoryDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BTN_BEGIN, OnBnClickedBtnBegin)
	ON_BN_CLICKED(IDC_BTN_STOP, OnBnClickedBtnStop)
	ON_BN_CLICKED(IDC_BTN_QUERY, OnBnClickedBtnQuery)
	ON_BN_CLICKED(IDC_CHECK_AUTOQUERY, OnBnClickedCheckAutoquery)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BTN_EXIT, OnBnClickedBtnExit)
	ON_BN_CLICKED(IDC_BTN_SAVE, OnBnClickedBtnSave)
	ON_BN_CLICKED(IDC_BTN_SHOWALL, OnBnClickedBtnShowall)
	ON_COMMAND(IDR_DELETETHIS, OnDeletethis)
	ON_COMMAND(IDR_DELETEALL, OnDeleteall)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_ShowResult, OnNMRclickListShowresult)
END_MESSAGE_MAP()


// CJXHHistoryDlg ��Ϣ�������

BOOL CJXHHistoryDlg::OnInitDialog()
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
	
#ifndef DEBUG
	if (GetPower() != 0)
	{
		exit(0);
	}
#endif
	::CoInitialize(NULL);
	//////////////////////////////////////////////////////////////////////////
	CUserFile *pFile = CUserFile::GetInstance();
	pFile->GetAppPath(g_strAppPath);
	InitParams();
	g_dbServer = g_strAppPath + g_dbServer;
	//Դͼ��
	g_srcPicPath = g_strAppPath + g_srcPicPath;

	//������Ӯ�ж�ͼ��
	g_dstPicPath11 = g_strAppPath + g_dstPicPath11;
	g_dstPicPath12 = g_strAppPath + g_dstPicPath12;
	g_dstPicPath13 = g_strAppPath + g_dstPicPath13;
	
	//�Ա�ͼ��
	g_cmpPicPath1 = g_strAppPath + g_cmpPicPath1;
	
	//����ͼ��
	g_srcPicPath2 = g_strAppPath + g_srcPicPath2;	
	g_cmpPicPath2 = g_strAppPath + g_cmpPicPath2;

	SetTableStyle();
	InitListCtrl();
	GetDlgItem(IDC_BTN_STOP)->EnableWindow(FALSE);
	SetDlgItemInt(IDC_EDIT_GOTWAIT,g_iGotSleep);
	SetDlgItemInt(IDC_EDIT_GETWAIT,g_iGetWaitSleep);
	SetDlgItemInt(IDC_EDIT_MINNUM,g_iMinNum);
	return TRUE;  // ���������˿ؼ��Ľ��㣬���򷵻� TRUE
}

void CJXHHistoryDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CJXHHistoryDlg::OnPaint() 
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
HCURSOR CJXHHistoryDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
void CJXHHistoryDlg::SetTableStyle()
{
	//CListCtrl* conList = &g_ShowResult;
	DWORD dwSytle=::GetWindowLong(g_ShowResult.m_hWnd,GWL_STYLE);
	// ����Ϊ������ʽ
	SetWindowLong(g_ShowResult.m_hWnd,GWL_STYLE,dwSytle|LVS_REPORT);
	DWORD ExStyle=g_ShowResult.GetExtendedStyle();
	g_ShowResult.SetExtendedStyle(ExStyle|LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
	CImageList image;
	image.Create(1,16, ILC_COLOR, 0, 0);
	g_ShowResult.SetImageList(&image, LVSIL_STATE);
	g_ShowResult.MoveWindow(3,3,725,230);
	g_ShowResult.InsertColumn(0,"���",LVCFMT_CENTER,40);
	g_ShowResult.InsertColumn(1,"��",LVCFMT_LEFT,30);
	g_ShowResult.InsertColumn(2,"��",LVCFMT_LEFT,30);
	g_ShowResult.InsertColumn(3,"��",LVCFMT_LEFT,30);
	//g_ShowResult.InsertColumn(2,"������",LVCFMT_LEFT,50);
	g_ShowResult.InsertColumn(4,"����",LVCFMT_LEFT,550);
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CJXHHistoryDlg::InitListCtrl()
{
	DWORD dwSytle=::GetWindowLong(g_ShowMsg.m_hWnd,GWL_STYLE);
	// ����Ϊ������ʽ
	SetWindowLong(g_ShowMsg.m_hWnd,GWL_STYLE,dwSytle|LVS_REPORT);
	DWORD ExStyle=g_ShowMsg.GetExtendedStyle();
	g_ShowMsg.SetExtendedStyle(ExStyle|LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
	::SendMessage(g_ShowMsg.m_hWnd, LVM_SETEXTENDEDLISTVIEWSTYLE,
		LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);
	g_ShowMsg.MoveWindow(3,293,475,155);
	g_ShowMsg.InsertColumn(0,_T("ʱ��"),LVCFMT_LEFT,125);
	g_ShowMsg.InsertColumn(1,_T("��Ϣ"),LVCFMT_LEFT,320);
}

/************************************************************************/
/*  ��ȡͼ���߳�                                                        */
/************************************************************************/
void CJXHHistoryDlg::ThreadFunc(LPVOID lParam)
{
	CJXHHistoryDlg *pParam = ((CJXHHistoryDlg *)lParam);
	if ( pParam )
	{
		ShowMessage("��ȡͼ���߳̿�ʼ");
		::CoInitialize(NULL);
		pParam->GetFunc(); //ִ���̺߳�����
		::CoUninitialize();
		
		ShowMessage("��ȡͼ���߳��˳�");
		pParam->GetDlgItem(IDC_BTN_BEGIN)->EnableWindow(TRUE);
		pParam->GetDlgItem(IDC_BTN_STOP)->EnableWindow(FALSE);
	}
}
/************************************************************************/
/*  ��ȡͼ���̺߳���                                                    */
/************************************************************************/
void CJXHHistoryDlg::GetFunc()
{
	while (!g_bExitFlag)
	{
		GetRecordPic();
		SplitPic();
		bool bRet = IsNeedToGetRecord();
		if (bRet)
		{
			SplitPicEx();
			g_strCurrRecord = "";
			g_strCurrRecord = GetCurrRecord();			
			SaveHistory();			
			Delay(g_iGotSleep,&g_bExitFlag);
		}
		else		//����δ��,����Ҫ���½��
		{
			ShowMessage("�ȴ���һ�ֵĽ��...");
			Delay(g_iGetWaitSleep,&g_bExitFlag);
		}		
	}
}

/************************************************************************/
/*  ƥ����ʷ�߳�                                                        */
/************************************************************************/
void CJXHHistoryDlg::ThreadQueryFunc(LPVOID lParam)
{
	CJXHHistoryDlg *pParam = ((CJXHHistoryDlg *)lParam);
	if ( pParam )
	{
		ShowMessage("ƥ����ʷ�߳̿�ʼ");
		::CoInitialize(NULL);
		pParam->QueryFunc(); //ִ���̺߳�����
		::CoUninitialize();
		ShowMessage("ƥ����ʷ�߳��˳�");
		pParam->GetDlgItem(IDC_BTN_BEGIN)->EnableWindow(TRUE);
		pParam->GetDlgItem(IDC_BTN_STOP)->EnableWindow(FALSE);
	}
}
/************************************************************************/
/*  ƥ����ʷ�̺߳���                                                        */
/************************************************************************/
void CJXHHistoryDlg::QueryFunc()
{
	while (!g_bExitFlag)
	{
		int iFlag = 0;
		g_QueryFlagLock.Lock();
		iFlag = g_iQueryFlag;
		g_iQueryFlag = 0;
		g_QueryFlagLock.Unlock();

		if (iFlag == 1)		//ִ��ƥ�����
		{
			ShowMessage("�Զ�ƥ����ʷ��ʼ...");
			//ִ��ƥ�����
			ShowRecords();
			ShowMessage("�Զ�ƥ����ʷ����");
		}
		Delay(500,&g_bExitFlag);
	}
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
void CJXHHistoryDlg::OnBnClickedBtnBegin()
{
	AfxBeginThread((AFX_THREADPROC)ThreadFunc,(LPVOID)this);
	AfxBeginThread((AFX_THREADPROC)ThreadQueryFunc,(LPVOID)this);
	g_bExitFlag = false;
	GetDlgItem(IDC_BTN_BEGIN)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_STOP)->EnableWindow(TRUE);	
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CJXHHistoryDlg::OnBnClickedBtnStop()
{
	g_bExitFlag = true;
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CJXHHistoryDlg::OnBnClickedBtnQuery()
{
	ShowMessage("ƥ����ʷ��ʼ...");
	//ִ��ƥ�����
	g_iMinNum = GetDlgItemInt(IDC_EDIT_MINNUM);
	ShowRecords();
	ShowMessage("ƥ����ʷ����");
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CJXHHistoryDlg::OnBnClickedCheckAutoquery()
{
	g_iAutoQueryFlag = ((CButton*)GetDlgItem(IDC_CHECK_AUTOQUERY))->GetCheck();
	GetDlgItem(IDC_BTN_QUERY)->EnableWindow(g_iAutoQueryFlag==1?FALSE:TRUE);
	ShowMessage(g_iAutoQueryFlag==1?"�����Զ�ƥ��":"ȡ���Զ�ƥ��");
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
//������ʷ����
bool CJXHHistoryDlg::SaveHistory()
{
	CAdoConnection adoConn;
	CAdoRecordSet adoRst;
	try
	{
		if(!adoConn.ConnectAccess(g_dbServer,g_dbPwd))
		{
			ShowMessage("�������ݿ�ʧ��");
			return false;	
		}
		adoRst.SetAdoConnection(&adoConn);
		adoRst.SetCursorLocation(adUseClient);
		CString strSQL = "";
		if (g_strCurrRecord == "0")		//�Ǻ�	0��2��1��
		{
			if (g_strCurrRecordList == "")
			{
				ShowMessage("��ʼͳ��");
			}
			ShowMessage("�˾ֺ;�");
			strSQL.Format("update hisrecord set isfull=1,detail=detail+'%s' where [id] = %d",g_strCurrRecord,g_iLastRecordID);
		}
		else
		{
			if (g_strCurrRecord == "1")
			{
				ShowMessage("�˾���Ӯ");
			}
			else
			{
				ShowMessage("�˾ֽ�Ӯ");
			}
			if (g_strCurrRecordList == "")	//��ʼͳ��
			{
				ShowMessage("��ʼͳ��");
				strSQL.Format("insert into hisrecord (isfull,rectime,detail) values(0,'%s','%s')",CTime::GetCurrentTime().Format("%Y-%m-%d %H:%M:%S"),g_strCurrRecord);
			}
			else		//ͳ����
			{
				strSQL.Format("update hisrecord set detail=detail+'%s' where [id] = %d",g_strCurrRecord,g_iLastRecordID);
			}			
		}
		//�������ݿ�
		if ( !adoRst.Open(strSQL,adCmdText) )
		{
			ShowMessage(adoConn.GetLastErrorText());
			ShowMessage("�����ʷʧ��");
			adoRst.Close();
			adoConn.Close();
			return false;
		}
		adoRst.Close();
		if (g_strCurrRecordList == "")//������¼�����,��ȡ���µ�IDֵ
		{
			strSQL.Format("select top 1 [id] from hisrecord order by [id] desc");
			if ( !adoRst.Open(strSQL,adCmdText))
			{
				ShowMessage(adoConn.GetLastErrorText());
				ShowMessage("��ȡ����IDʧ��");
				adoRst.Close();
				adoConn.Close();
				return false;
			}
			//��ȡ����ID
			adoRst.GetCollect("ID",g_iLastRecordID);
			CString strShow = "";
			strShow.Format("��ǰID��:%d",g_iLastRecordID);
			ShowMessage(strShow);
			adoRst.Close();
		}
		g_strCurrRecordList += g_strCurrRecord;
		SetDlgItemText(IDC_EDIT_CURRRECORD,g_strCurrRecordList);
		//g_strCurrRecordList += g_strCurrRecord;//��¼�ۼ�
		if (g_strCurrRecord == "0")
		{
			g_strCurrRecordList = "";
			g_iLastRecordID = 0;
		}
		else
		{
			//֪ͨ��ѯ�߳̿��Խ���ƥ����ʷ��
			g_QueryFlagLock.Lock();			
			if (g_iAutoQueryFlag == 1)
			{
				g_iQueryFlag = 1;
			}
			else
			{
				g_iQueryFlag = 0;
			}
			g_QueryFlagLock.Unlock();
		}
		SetDlgItemText(IDC_EDIT_QUERYSTRING,g_strCurrRecordList);
		adoConn.Close();
		return true;
	}
	catch(...)
	{
		ShowMessage("������ʷ�쳣");
	}
	adoRst.Close();
	adoConn.Close();
	return false;
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
//��ȡͼ����
void CJXHHistoryDlg::GetRecordPic()
{
	//���Ҵ���
	HWND hwnd = ::FindWindow(g_strClassName,g_strWindowName);
	if (!hwnd)
	{
		ShowMessage("���Ҵ���ʧ��");
		return;
	}	
	hwnd = m_myWind.GetChildWindow(hwnd,3);
	if (m_myWind.GetContrlClassName(hwnd) != "#32770")
	{
		ShowMessage("�����Ӵ���ʧ��");
		return;
	}
	//��ȡͼ��
	DeleteFile(g_srcPicPath);
	m_myImg.GetSelectWindowPic(hwnd,g_srcPicPath);
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
//�ж��Ƿ���Ҫ������ʷ��¼
bool CJXHHistoryDlg::IsNeedToGetRecord()
{
	IplImage *Img = NULL;
	Img = cvLoadImage(g_srcPicPath2,0);
	cvThreshold(Img,Img,90,255,CV_THRESH_BINARY);
	for (int i=2;i<Img->width-2;i++)
	{
		CvScalar tempColor = cvGet2D(Img,9,i);
		if (tempColor.val[0] != 255)
		{
			cvReleaseImage(&Img);
			return false;
		}
	}
	ShowMessage("��ʼ��ȡ���...");
	cvReleaseImage(&Img);
	return true;
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
//��ȡ��ǰ��Ӯ��
CString CJXHHistoryDlg::GetCurrRecord()
{
	if (IsSelectResult(1))//��
	{
		return "2";				
	}
	else if (IsSelectResult(2))//��
	{
		return "1";
	}
	else if (IsSelectResult(3))//��
	{
		return "0";
	}
	return "";
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
//�ж��ǲ���ָ�����
bool CJXHHistoryDlg::IsSelectResult(int iFlag)
{
	IplImage *Img = NULL;
	CString strPicPath = "";
	switch(iFlag)
	{
	case 1:strPicPath = g_dstPicPath11;break;
	case 2:strPicPath = g_dstPicPath12;break;
	case 3:strPicPath = g_dstPicPath13;break;
	default:return false;
	}
	Img = cvLoadImage(strPicPath,0);
	if (Img == NULL)
	{
		ShowMessage("���ͼ��ʱ����Դͼ��ʧ��");
		return false;
	}
	cvThreshold(Img,Img,90,255,CV_THRESH_BINARY);
	for (int i=7;i<Img->width-7;i++)
	{
		for (int j=7;j<Img->height-7;j++)
		{
			CvScalar tempColor = cvGet2D(Img,j,i);
			if (tempColor.val[0] != 0 || tempColor.val[1] != 0 || tempColor.val[2] != 0)
			{
				cvReleaseImage(&Img);
				return true;
			}
		}
	}
	cvReleaseImage(&Img);
	return false;
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CJXHHistoryDlg::DeleteFile(CString strPath)
{
	if (_access(strPath,0) == 0)
	{
		CFile::Remove(strPath);
	}
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
//���Դͼ����в��,��ֳ�������ʷ��¼ͼ��
void CJXHHistoryDlg::SplitPic()
{
	IplImage *Img = NULL;	
	Img = cvLoadImage(g_srcPicPath);
	if (Img == NULL)
	{
		ShowMessage("���ͼ��ʱ����Դͼ��ʧ��");
		return;
	}
	SaveSubRectPic(Img,g_srcPicPath2,161,562,10,10);//��������ͼ��
	cvReleaseImage(&Img);
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
//���Դͼ����в��,��ֳ�������ʷ��¼ͼ��
void CJXHHistoryDlg::SplitPicEx()
{
	IplImage *Img = NULL;	
	Img = cvLoadImage(g_srcPicPath);
	if (Img == NULL)
	{
		ShowMessage("���ͼ��ʱ����Դͼ��ʧ��");
		return;
	}
	SaveSubRectPic(Img,g_dstPicPath11,635,615,20,20);//����ͼ��1
	SaveSubRectPic(Img,g_dstPicPath12,635,645,20,20);//����ͼ��2
	SaveSubRectPic(Img,g_dstPicPath13,635,675,20,20);//����ͼ��3
	cvReleaseImage(&Img);
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
//��ȡ�������ͼ�񱣴�
void CJXHHistoryDlg::SaveSubRectPic(IplImage *srcImg,CString strPath,int x,int y,int iWidth,int iHeight)
{
	CvMat *dst;
	CvRect subRect;
	IplImage *dstImg;
	int iii = CV_8UC3;
	dst=cvCreateMat(iWidth,iHeight,CV_8UC3);
	subRect =cvRect(x,y,iWidth,iHeight);
	cvGetSubRect(srcImg,dst,subRect);
	int ii = IPL_DEPTH_8U;
	dstImg=cvCreateImage(cvSize(iWidth,iHeight),srcImg->depth,srcImg->nChannels);
	cvGetImage(dst,dstImg);
	cvSaveImage(strPath,dstImg);
	cvReleaseMat(&dst);
	cvReleaseImage(&dstImg);

}
void CJXHHistoryDlg::OnClose()
{	
	if (g_bExitFlag == false)
	{
		MessageBox("����ֹͣ�߳�","��ʾ");
		return;
	}
	CUserFile *pFile = CUserFile::GetInstance();
	if (pFile)
	{
		delete pFile;
		pFile = NULL;
	}
	CDialog::OnClose();
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CJXHHistoryDlg::OnBnClickedButton1()
{
	SplitPic();
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
//�߳��п�����ʱ�ĺ���
void CJXHHistoryDlg::Delay(int iMiniSecond,BOOL *bExit)
{
	for (int i=0;i<iMiniSecond;i+=500)
	{
		if (*bExit)
		{
			break;
		}
		Sleep( 500 );
	}
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CJXHHistoryDlg::OnBnClickedBtnExit()
{
	PostMessage(WM_CLOSE,0,0);
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CJXHHistoryDlg::OnBnClickedBtnSave()
{
	g_iGotSleep = GetDlgItemInt(IDC_EDIT_GOTWAIT);
	g_iGetWaitSleep = GetDlgItemInt(IDC_EDIT_GETWAIT);
	//g_iQuerySleep = GetDlgItemInt(IDC_EDIT_QUERYWAIT);
	g_iMinNum = GetDlgItemInt(IDC_EDIT_MINNUM);
	CString strTemp = "";
	int iTemp = 0;
	CUserFile *pFile = NULL;
	pFile = CUserFile::GetInstance();
	if (pFile)
	{	
		int iRet = -1;
		CString strAppPath = g_strAppPath;
		strAppPath += MWGATEINITFILE;
		pFile->CheckInitValue(g_iGetWaitSleep,1000,1000000);
		pFile->CheckInitValue(g_iGotSleep,5000,1000000);
		pFile->CheckInitValue(g_iQuerySleep,1000,1000000);
		pFile->WriteInitInfo(strAppPath,"Time","getwait",strTemp,g_iGetWaitSleep,1);
		pFile->WriteInitInfo(strAppPath,"Time","gotwait",strTemp,g_iGotSleep,1);
		pFile->WriteInitInfo(strAppPath,"Time","querywait",strTemp,g_iQuerySleep,1);
		pFile->WriteInitInfo(strAppPath,"QueryNum","min",strTemp,g_iMinNum,1);
	}
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CJXHHistoryDlg::InitParams()
{
	CString strTemp = "";
	int iTemp = 0;
	CUserFile *pFile = NULL;
	pFile = CUserFile::GetInstance();
	if (pFile)
	{	
		int iRet = -1;
		CString strAppPath = g_strAppPath;
		strAppPath += MWGATEINITFILE;
		//���ݿ���Ϣ
		iRet = pFile->ReadInitInfo(strAppPath,"database","dbserver",g_dbServer,iTemp,0);
		if (iRet < 0)
		{
			g_dbServer = "jxhrecord.mdb";
		}
		iRet = pFile->ReadInitInfo(strAppPath,"database","dbpwd",g_dbPwd,iTemp,0);
		if (iRet < 0)
		{
			g_dbPwd = "";
		}
		iRet = pFile->ReadInitInfo(strAppPath,"window","class",g_strClassName,iTemp,0);
		if (iRet < 0)
		{
			g_strClassName = "ClientFrame_CMainFrame";
		}
		iRet = pFile->ReadInitInfo(strAppPath,"window","name",g_strWindowName,iTemp,0);
		if (iRet < 0)
		{
			g_strWindowName = "������Ϸ--�����";
		}
		pFile->ReadInitInfo(strAppPath,"Time","getwait",strTemp,g_iGetWaitSleep,1);
		pFile->ReadInitInfo(strAppPath,"Time","gotwait",strTemp,g_iGotSleep,1);		
		pFile->ReadInitInfo(strAppPath,"Time","querywait",strTemp,g_iQuerySleep,1);
		pFile->ReadInitInfo(strAppPath,"QueryNum","min",strTemp,g_iMinNum,1);
		pFile->CheckInitValue(g_iMinNum,1,1000000);
		pFile->CheckInitValue(g_iGetWaitSleep,1000,1000000);
		pFile->CheckInitValue(g_iGotSleep,5000,1000000);
		pFile->CheckInitValue(g_iQuerySleep,1000,1000000);
	}
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
int CJXHHistoryDlg::QueryHistoryRecord(CString strQuery,CStringList &strResultList)
{
	CAdoConnection adoConn;
	CAdoRecordSet adoRst;
	try
	{
		if(!adoConn.ConnectAccess(g_dbServer,g_dbPwd))
		{
			ShowMessage("�������ݿ�ʧ��");
			return -1;	
		}
		adoRst.SetAdoConnection(&adoConn);
		adoRst.SetCursorLocation(adUseClient);
		CString strSQL = "";
		strSQL.Format("select detail from hisrecord where [id] <>%d and detail like '%%%s%%'",g_iLastRecordID,strQuery);
		//�������ݿ�
		if ( !adoRst.Open(strSQL,adCmdText) )
		{
			ShowMessage(adoConn.GetLastErrorText());
			ShowMessage("ƥ����ʷʧ��");
			adoRst.Close();
			adoConn.Close();
			return -1;
		}
		int iRecordCount = 0;
		CString strTemp = "";
		while(!adoRst.IsEOF())
		{
			strTemp = "";
			adoRst.GetCollect("detail",strTemp);
			if (strTemp != "0")
			{
				strResultList.AddTail(strTemp);
				iRecordCount ++;
			}
			adoRst.MoveNext();
		}
		adoRst.Close();
		adoConn.Close();
		return iRecordCount;
	}
	catch(...)
	{
		ShowMessage("ƥ����ʷ�쳣");
	}
	adoRst.Close();
	adoConn.Close();
	return -1;
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
int CJXHHistoryDlg::GetPower()
{
	CInternetSession session;
	CString strTemp = "";
	try
	{
		CStdioFile *pFile = session.OpenURL("http://blog.csdn.net/TragicGuy/archive/2010/01/08/5157814.aspx");
		if (pFile != NULL)
		{
			while(pFile->ReadString(strTemp))
			{
				strTemp.MakeLower();
				if (strTemp.Find("myownpower111") !=-1)
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
void CJXHHistoryDlg::ShowRecords()
{
	CString strQuery = "";
	GetDlgItemText(IDC_EDIT_QUERYSTRING,strQuery);
	g_ShowResult.DeleteAllItems();
	SetDlgItemText(IDC_EDIT_FIRST,"0");
	SetDlgItemText(IDC_EDIT_SECOND,"0");
	SetDlgItemText(IDC_EDIT_THIRD,"0");
	if (strQuery == "")
	{
		ShowMessage("��ƥ�����ݷǷ�");
		return;
	}
	CStringList strRecords;
	while (true)
	{
		if(strQuery.GetLength() < g_iMinNum)
		{
			ShowMessage("�ȴ�ƥ�䴮����С����С������,�˳�");
			break;
		}
		strRecords.RemoveAll();
		int iRet = QueryHistoryRecord(strQuery,strRecords);
		if (iRet < 0)
		{
			break;
		}
		else if (iRet == 0)
		{
			if (strQuery.GetLength() < 1)
			{
				break;
			}
			strQuery = strQuery.Mid(1);
			SetDlgItemText(IDC_EDIT_QUERYSTRING,strQuery);
			continue;
		}
		else	//ƥ�䵽���
		{
			CString strFormat = "";
			strFormat.Format("Ӧ�� \"%s\" ƥ�䵽%d����¼",strQuery,iRet);
			ShowMessage(strFormat);
			break;
		}
	}
	//�������ʾ
	CString strNum = "";
	for (POSITION Pos_ = strRecords.GetHeadPosition(); Pos_ != NULL;)
	{
		CString strResult = strRecords.GetNext(Pos_);
		int nCount = g_ShowResult.GetItemCount();
		strNum.Format("%d",nCount);
		g_ShowResult.InsertItem( nCount, strNum);
		g_ShowResult.SetItemText(nCount,4,strResult);
	}
	ComputeResult(strQuery);
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
void CJXHHistoryDlg::ComputeResult(CString strDstString)
{
	CString strRecord = "";	
	int iTotalFirst = 0;
	int iTotalSecond = 0;
	int iTotalThird = 0;
	//char charTemp = 0;
	CString strTemp = "";
	int iLen = strDstString.GetLength();
	int iRecordCount = g_ShowResult.GetItemCount();	
	for (int i=0;i<iRecordCount;i++)
	{
		int iFirst = 0;
		int iSecond = 0;
		int iThird = 0;
		strRecord = g_ShowResult.GetItemText(i,4);
		int idx = -1;
		while (true)
		{
			idx = strRecord.Find(strDstString);
			if (idx == -1)
			{
				break;
			}
			if (strRecord.GetLength() <= idx + iLen)
			{
				break;
			}
			strTemp = strRecord.Mid(idx + strDstString.GetLength(),1);
			strRecord = strRecord.Right(strRecord.GetLength()-idx-1);
			switch (atoi(strTemp))
			{
			case 2:
				iFirst ++;
				iTotalFirst++;
				break;
			case 1:
				iSecond++;
				iTotalSecond++;
				break;
			case 0:
				iThird++;
				iTotalThird++;
				break;
			default: break;
			}			
		}
		//��ʾͳ�ƽ��
		strTemp.Format("%d",iFirst);
		g_ShowResult.SetItemText(i,1,strTemp);
		strTemp.Format("%d",iSecond);
		g_ShowResult.SetItemText(i,2,strTemp);
		strTemp.Format("%d",iThird);
		g_ShowResult.SetItemText(i,3,strTemp);
	}
	//��ʾӮ��
	int iTotalCount = iTotalFirst + iTotalSecond + iTotalThird;
	if (iTotalCount == 0)
	{
		SetDlgItemText(IDC_EDIT_FIRST,"0");
		SetDlgItemText(IDC_EDIT_SECOND,"0");
		SetDlgItemText(IDC_EDIT_THIRD,"0");
		return;
	}
	float fFirst = 0.00;
	float fSecond = 0.00;
	float fThird = 0.00;
	fFirst = float(iTotalFirst * 100)/float(iTotalCount);
	fSecond = float(iTotalSecond * 100)/float(iTotalCount);
	fThird = float(iTotalThird * 100)/float(iTotalCount);
	strTemp.Format("%g %%",fFirst);
	SetDlgItemText(IDC_EDIT_FIRST,strTemp);
	strTemp.Format("%g %%",fSecond);
	SetDlgItemText(IDC_EDIT_SECOND,strTemp);
	strTemp.Format("%g %%",fThird);
	SetDlgItemText(IDC_EDIT_THIRD,strTemp);
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CJXHHistoryDlg::OnBnClickedBtnShowall()
{
	g_ShowResult.DeleteAllItems();
	CAdoConnection adoConn;
	CAdoRecordSet adoRst;	
	try
	{
		if(!adoConn.ConnectAccess(g_dbServer,g_dbPwd))
		{
			ShowMessage("�������ݿ�ʧ��");
			return;	
		}
		adoRst.SetAdoConnection(&adoConn);
		adoRst.SetCursorLocation(adUseClient);
		CString strSQL = "";
		strSQL.Format("select [id],detail from hisrecord where [id] <> %d",g_iLastRecordID);
		//�������ݿ�
		if ( !adoRst.Open(strSQL,adCmdText) )
		{
			ShowMessage("��ѯ��ʷʧ��");
			ShowMessage(adoConn.GetLastErrorText());
			adoRst.Close();
			adoConn.Close();
			return;
		}
		int iRecordCount = 0;
		CString strTemp = "";
		CString strNum = "";
		while(!adoRst.IsEOF())
		{
			strTemp = "";
			int iRecordID = 0;
			adoRst.GetCollect("id",iRecordID);
			adoRst.GetCollect("detail",strTemp);			
			if (strTemp != "0")
			{
				int nCount = g_ShowResult.GetItemCount();				
				strNum.Format("%d",iRecordID);
				g_ShowResult.InsertItem( nCount, strNum);
				g_ShowResult.SetItemText(nCount,4,strTemp);
				iRecordCount++;
			}
			adoRst.MoveNext();
		}
		adoRst.Close();
		adoConn.Close();
		strTemp.Format("���ݿ��й���%d����ʷ��¼",iRecordCount);
		ShowMessage(strTemp);
		return ;
	}
	catch(...)
	{
		ShowMessage("��ѯ��ʷ�쳣");
	}
	adoRst.Close();
	adoConn.Close();
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CJXHHistoryDlg::OnDeletethis()
{
	if (g_bExitFlag == false)
	{
		ShowMessage("���ڳ���ֹͣ״̬�½��д˲���");
		return;
	}
	if(MessageBox("��ʷɾ�����޷��ָ�,��ȷ��Ҫɾ����?","����",MB_ICONEXCLAMATION|MB_YESNO)==IDNO)
	{
		return;
	}
	POSITION pos = g_ShowResult.GetFirstSelectedItemPosition();
	if (pos != NULL)
	{
		int nItem = g_ShowResult.GetNextSelectedItem(pos);
		CString strID = g_ShowResult.GetItemText(nItem,0);
		CString strSQL = "";
		strSQL.Format("delete from hisrecord where [ID] = %s",strID);
		ExcuteSQL(strSQL);
		OnBnClickedBtnShowall();
	}
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CJXHHistoryDlg::OnDeleteall()
{
	if (g_bExitFlag == false)
	{
		ShowMessage("���ڳ���ֹͣ״̬�½��д˲���");
		return;
	}
	if(MessageBox("��ʷɾ�����޷��ָ�,��ȷ��Ҫɾ����?","����",MB_ICONEXCLAMATION|MB_YESNO)==IDNO)
	{
		return;
	}
	CString strSQL = "";
	strSQL.Format("delete from hisrecord");
	ExcuteSQL(strSQL);
	OnBnClickedBtnShowall();
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CJXHHistoryDlg::OnNMRclickListShowresult(NMHDR *pNMHDR, LRESULT *pResult)
{
	*pResult = 0;
	POSITION pos = g_ShowResult.GetFirstSelectedItemPosition();
	if (pos != NULL)
	{
		//int nItem = g_ShowResult.GetNextSelectedItem(pos);		
		CMenu menu; 
		menu.LoadMenu(IDR_MENU1); //�������ȶ����ѡ�� 
		CMenu *pmenu = menu.GetSubMenu(0); 
		CPoint pos; 
		GetCursorPos(&pos);
		SetForegroundWindow();
		pmenu->TrackPopupMenu(TPM_LEFTALIGN,pos.x,pos.y,this);		
	}
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
void CJXHHistoryDlg::ExcuteSQL(CString strSQL)
{
	CAdoConnection adoConn;
	CAdoRecordSet adoRst;
	try
	{
		if(!adoConn.ConnectAccess(g_dbServer,g_dbPwd))
		{
			ShowMessage("�������ݿ�ʧ��");
			return;	
		}
		adoRst.SetAdoConnection(&adoConn);
		adoRst.SetCursorLocation(adUseClient);
		//�������ݿ�
		if ( !adoRst.Open(strSQL,adCmdText) )
		{
			ShowMessage("��ѯ��ʷʧ��");
			ShowMessage(adoConn.GetLastErrorText());
			adoRst.Close();
			adoConn.Close();
			return;
		}		
		adoRst.Close();
		adoConn.Close();
		return ;
	}
	catch(...)
	{
		ShowMessage("��ѯ��ʷ�쳣");
	}
	adoRst.Close();
	adoConn.Close();
}
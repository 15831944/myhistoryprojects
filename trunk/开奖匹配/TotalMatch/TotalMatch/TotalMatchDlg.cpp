// TotalMatchDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "TotalMatch.h"
#include "TotalMatchDlg.h"
#include <io.h>
#include <direct.h>
#include <afxmt.h>
#include <afxtempl.h>
#include ".\totalmatchdlg.h"
#include "UserFile.h"
#include <afxmt.h>
#include <afxtempl.h>
#include <map>
#include <atlbase.h>
#include "afxinet.h"
#include <io.h>
#include <odbcinst.h>
#include <afxdb.h>
#include "UserFile.h"
#include <vector>
#define TOTAL_NUMBER 49
#pragma comment(lib,"wininet.lib")
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
int g_iOptType = 0;	//0��ʾ���룬1��ʾ������2��ʾ��ʾ����
CArray<CString, CString> g_strList;
CString g_strPath = "";
bool g_bExitFlag = true;
int g_iTotalNum = 0;
CString g_dbServer = "";
CString g_dbPwd = "";
CString g_strAppPath = "";
CString g_strDateStart = "";
CString g_strDateEnd = "";
int g_iNumLen = 0;	//�������
int g_iRightCheck = 0; //ƥ��ģʽ[1����0��]
map<int,int> g_mapKey;
typedef map<int,int> ResultList;
//map<int[5],int> g_litlist;
ResultList g_ALLData;
ResultList g_ALLDataArray[2];
int g_iDataLocation = 0;
int g_iTotalLenth = 0;
vector<CString> g_vec_ret;
int g_iArray[49];
int g_iNumMatched = 0;
int g_iTotalResult = 1906884;
int g_iMatchArray[1906884][5];

int g_iMatchArrayEx[2][1906884];
// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���
DWORD g_lTotalCount = 1; //��������������
CString g_strMatchNum = "";
#define STR_RIGHT_FLAG "��ƥ��"
#define STR_NORIGHT_FLAG "��ƥ��"

#define STR_RIGHT_SHOW "��ƥ�� [ƥ������N�ھ����ֹ��ĺ������]"
#define STR_NORIGHT_SHOW "��ƥ�� [ƥ��δ���ֹ���]"

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


// CTotalMatchDlg �Ի���



CTotalMatchDlg::CTotalMatchDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTotalMatchDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	::CoInitialize(NULL);
}
CTotalMatchDlg::~CTotalMatchDlg()
{
	CUserFile *pFile = CUserFile::GetInstance();
	if (pFile)
	{
		delete pFile;
		pFile = NULL;
	}
	::CoUninitialize();
}
void CTotalMatchDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_TOTAL, m_list_show);
	DDX_Control(pDX, IDC_LIST_SHOW, m_list_info);
	DDX_Control(pDX, IDC_DATE_START, m_datestart);
	DDX_Control(pDX, IDC_DATE_END, m_dateend);
}

BEGIN_MESSAGE_MAP(CTotalMatchDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BTN_EXIT, OnBnClickedBtnExit)
	ON_BN_CLICKED(IDC_BTN_IMPORT, OnBnClickedBtnImport)
	ON_BN_CLICKED(IDC_BTN_STOP, OnBnClickedBtnStop)
	ON_BN_CLICKED(IDC_BTN_SHOWUNION, OnBnClickedBtnShowunion)
	ON_BN_CLICKED(IDC_BTN_MATCH, OnBnClickedBtnMatch)
	ON_BN_CLICKED(IDC_RADIO_MATCH, OnBnClickedRadioMatch)
	ON_BN_CLICKED(IDC_RADIO_NOMATCH, OnBnClickedRadioNomatch)
END_MESSAGE_MAP()


// CTotalMatchDlg ��Ϣ�������

BOOL CTotalMatchDlg::OnInitDialog()
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
	if (GetPower()<0)
	{
		exit(0);
	}
	SetTableStyle();
	SetTableStyleEx();
	SetWindowText("�н�ƥ��");
	InitParams();
	GetDlgItem(IDC_BTN_SHOWUNION)->EnableWindow(FALSE);
	COleDateTime tCurr = COleDateTime::GetCurrentTime();
	COleDateTimeSpan span; 
	span.SetDateTimeSpan(3,0,0,0);
	COleDateTime olgTime = tCurr-span;
	m_datestart.SetTime(olgTime);
	SetDlgItemText(IDC_RADIO_MATCH,STR_RIGHT_FLAG);
	SetDlgItemText(IDC_RADIO_NOMATCH,STR_NORIGHT_FLAG);
	ShowDbData();
	//����һ���̣߳���ʼ��ƥ��Դ
	g_iOptType = 4;
	g_bExitFlag = false;
	AfxBeginThread((AFX_THREADPROC)ThreadProc, (LPVOID)this);
	return TRUE;  // ���������˿ؼ��Ľ��㣬���򷵻� TRUE
}

void CTotalMatchDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CTotalMatchDlg::OnPaint() 
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
HCURSOR CTotalMatchDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CTotalMatchDlg::ThreadProc(LPVOID lpParam)
{
	CTotalMatchDlg *pWnd = (CTotalMatchDlg *)lpParam;
	pWnd->GetDlgItem(IDC_BTN_IMPORT)->EnableWindow(FALSE);
	pWnd->GetDlgItem(IDC_BTN_MATCH)->EnableWindow(FALSE);
	pWnd->GetDlgItem(IDC_BTN_SHOWUNION)->EnableWindow(FALSE);
	::CoInitialize(NULL);
	pWnd->OperateProc();
	::CoUninitialize();
	pWnd->GetDlgItem(IDC_BTN_IMPORT)->EnableWindow(TRUE);
	pWnd->GetDlgItem(IDC_BTN_MATCH)->EnableWindow(TRUE);
	//pWnd->GetDlgItem(IDC_BTN_SHOWUNION)->EnableWindow(TRUE);
	g_bExitFlag = true;
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
void CTotalMatchDlg::OperateProc()
{
	CString strFormat = "";
	switch(g_iOptType)
	{
	case 1:	//��������
		{	
			CStdioFile   file;
			CString   cstr = "";
			CFileException   fe;			
			int iNum = 0;
			int iImportNum = 0;
			int iSuccNum = 0;
			int iFailNum = 0;
			CString strBefore = "";
			CString strAfter = "";
			CString strAcc = "";
			CString strPwd = "";
			CString strNum = "";
			if(file.Open(g_strPath,CFile::modeRead,&fe))
			{
				file.SeekToBegin();
				ShowMessage("���ڵ���,���Ժ�...");
				g_strList.RemoveAll();
				CAdoConnection adoConn;
				CAdoRecordSet adoRst;
				if(!adoConn.ConnectAccess(g_dbServer,g_dbPwd))
				{
					ShowMessage("�������ݿ�ʧ��");
					return;	
				}
				adoRst.SetAdoConnection(&adoConn);
				adoRst.SetCursorLocation(adUseClient);
				while (file.ReadString(cstr))
				{
					if (g_bExitFlag)
					{
						break;
					}
					cstr.Trim();
					if (cstr != "")
					{
						iImportNum++;
						CString strArray[100] = {""};
						int iArrayLen = 0;
						char charSplit = ' ';
						CString strSplit = "";
						strSplit.Format("%c",charSplit);
						cstr += strSplit;
						iArrayLen = StringSplit(cstr,strArray,charSplit);
						if (iArrayLen != 3)
						{
							//error
							iFailNum++;
							continue;
						}
						//�������ݿ�
						
						int iRet = InsertIntoDb(strArray,adoRst,adoConn);
						if (iRet < 0 )	//ʧ��
						{
							iFailNum++;
						}
						else
						{
							iSuccNum++;
							//��ӵ���ʾ�б�
							int nCount = m_list_show.GetItemCount();
							strNum.Format("%d",nCount);
							m_list_show.InsertItem( nCount,strNum);
							m_list_show.SetItemText(nCount,1,strArray[0]);
							m_list_show.SetItemText(nCount,2,strArray[1]);
							m_list_show.SetItemText(nCount,3,strArray[2]);
							if (iSuccNum % 10 == 0)
							{
								m_list_show.PostMessage(WM_VSCROLL,SB_BOTTOM,0);
							}
						}
					}				
				}
				g_iTotalNum = iNum;
				if (g_bExitFlag)
				{
					strFormat.Format("�ֶ���ֹ����,������%d����¼,�ɹ�%d��,ʧ��%d��",iImportNum,iSuccNum,iFailNum);
				}
				else
				{
					strFormat.Format("�������,������%d����¼,�ɹ�%d��,ʧ��%d��",iImportNum,iSuccNum,iFailNum);
				}
				m_list_show.PostMessage(WM_VSCROLL,SB_BOTTOM,0);
				ShowMessage(strFormat);
				adoConn.Close();
			}
		}
		break;
	case 2:	//ƥ����
		{
			//�ȵõ������
			//�ж��Ƿ����㹻����
			//���˵������Ѿ����ֹ�������[49�������е�]
			//��ʣ������ְ�5�����
			strFormat.Format("ƥ������:%s - ����:%d",g_iRightCheck == 0?STR_NORIGHT_SHOW:STR_RIGHT_SHOW,g_iNumLen);
			ShowMessage(strFormat);
			Sleep(500);
			ShowMessage("���ڻ�ȡƥ������,���Ժ�...");
			CString strSQL = "";
			//strSQL.Format("select  * from RollTable where (rollTime > #%s# or rollTime=#%s#) and (rollTime < #%s# or rollTime=#%s#)",
			//	g_strDateStart,g_strDateStart,g_strDateEnd,g_strDateEnd);
			strSQL.Format("select top %d * from RollTable order by rollTime desc",g_iNumLen);
			CAdoConnection adoConn;
			CAdoRecordSet adoRst;
			if(!adoConn.ConnectAccess(g_dbServer,g_dbPwd))
			{
				ShowMessage("�������ݿ�ʧ��");
				return;	
			}
			adoRst.SetAdoConnection(&adoConn);
			adoRst.SetCursorLocation(adUseClient);
			if (!adoRst.Open(strSQL))
			{
				ShowMessage("��ѯʧ��:"+adoConn.GetLastErrorText());
				adoRst.Close();
				adoConn.Close();
				return;
			}
			ShowMessage("���ڽ���,���Ժ�...");
			InitMapList();
			m_list_show.DeleteAllItems();
			g_vec_ret.clear();
			SetDlgItemText(IDC_STATIC_INFO,"");
			g_iNumMatched = 0;
			for (int i=0;i<49;i++)
			{
				g_iArray[i] = -1;
			}
			CString strRollStr = "";
			int iNum = 0;
			int iCount = 0;
			COleDateTime oleTime;
			CString strNum = "";
			CString strRollEx = "";
			while (!adoRst.IsEOF())
			{
				adoRst.GetCollect("rollTime",oleTime);
				adoRst.GetCollect("rollNum",iNum);
				adoRst.GetCollect("rollStr",strRollStr);
				strRollEx = strRollStr;
				//����strRollStr���в��				
				//��ӵ���ʾ�б�
				int nCount = m_list_show.GetItemCount();
				strNum.Format("%d",nCount);
				m_list_show.InsertItem( nCount,strNum);
				m_list_show.SetItemText(nCount,1,oleTime.Format("%Y-%m-%d"));
				strNum.Format("%d",iNum);
				m_list_show.SetItemText(nCount,2,strNum);
				m_list_show.SetItemText(nCount,3,strRollStr);
				m_list_show.PostMessage(WM_VSCROLL,SB_BOTTOM,0);
				strRollEx.Trim();
				strRollEx.Replace("T","");
				g_vec_ret.push_back(strRollEx);
				CString strArray[100] = {""};
				int iArrayLen = StringSplit(strRollEx +"-",strArray,'-');
				if (iArrayLen != 7)
				{
					continue;
				}
				iCount++;
				for (int i=0;i<iArrayLen;i++)
				{
					g_mapKey[atoi(strArray[i])] = 1;
				}
				adoRst.MoveNext();
			}
			adoRst.Close();
			adoConn.Close();
			//iCount
			
			strFormat.Format("����%d�ڵ���������ѡ������,���ڽ���ƥ��,���Ժ�...",iCount);
			ShowMessage(strFormat);
			strFormat.Format("��ƥ�����������Ĺ�  %d  �ڵ�����",iCount);
			SetDlgItemText(IDC_STATIC_INFO,strFormat);
			if (iCount < 1)
			{
				ShowMessage("û�������������������Ŀ�����¼,��ƥ����");
				return;
			}
			//Sleep(1000);
			//���Ѿ����ڵ����ݹ��˵�
			//��MAP�еõ�δ���ֹ�������
			if (g_iRightCheck == 0)		//��ƥ��
			{
				GetResultNoMatch();
			}
			else		//��ƥ��
			{
				GetResultMatch();
			}
			GetDlgItem(IDC_BTN_SHOWUNION)->EnableWindow(TRUE);
		}
		break;
	case 3:
		{
			//�������
			ShowMessage("���ڵ���,���Ժ�...");
			if (g_iRightCheck == 0)
			{
				OutputNoMatch();
			}
			else
			{
				OutputMatch();
			}
		}
		break;
	case 4:	//ͨ���߳����������е��������
		{
			ShowMessage("��������ƥ��Դ,���Ժ�...");
			for (int i=0;i<49;i++)
			{
				g_iArray[i] = i+1;
			}
			int iNum = 0;
			for (int i=0;i<TOTAL_NUMBER;i++)
			{
				for (int j=i+1;j<TOTAL_NUMBER;j++)
				{
					for (int k=j+1;k<TOTAL_NUMBER;k++)
					{
						for(int m=k+1;m<TOTAL_NUMBER;m++)
						{
							for (int n=m+1;n<TOTAL_NUMBER;n++)
							{								
								g_iMatchArray[iNum][0] = g_iArray[i];
								g_iMatchArray[iNum][1] = g_iArray[j];
								g_iMatchArray[iNum][2] = g_iArray[k];
								g_iMatchArray[iNum][3] = g_iArray[m];
								g_iMatchArray[iNum][4] = g_iArray[n];
								iNum++;
							}
						}
					}
				}
			}
			ShowMessage("����ƥ��Դ���");
		}
		break;
	default:
		break;
	}
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CTotalMatchDlg::OnBnClickedBtnExit()
{
	PostMessage(WM_CLOSE,0,0);
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CTotalMatchDlg::ShowMessage(CString strMsg)
{
	//SetDlgItemText(IDC_INFO_SHOW,strMsg);	
	WriteLog(strMsg);
	int nCount = m_list_info.GetItemCount();
	if (nCount > 500)
	{
		m_list_info.DeleteAllItems();
		nCount = 0;
	}	
	m_list_info.InsertItem( nCount, CTime::GetCurrentTime().Format("%H:%M:%S"));
	m_list_info.SetItemText(nCount,1,strMsg);
	m_list_info.PostMessage(WM_VSCROLL,SB_BOTTOM,0);
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CTotalMatchDlg::OnBnClickedBtnImport()
{
	g_strPath = "";
// 	if(SaveParams()<0)
// 	{
// 		return;
// 	}
	char szFilter[255] = {0};
	CString strPath = "";
	strcpy(szFilter,"Text files(*.txt)|*.txt");
	CFileDialog dlg(1,NULL,NULL,NULL,szFilter); 
	if(dlg.DoModal()==IDOK) 
	{
		strPath = dlg.GetPathName();
	}
	strPath.Trim();
	if (strPath != "")
	{
		g_strPath = strPath;
		//�����߳�
		g_iOptType = 1;
		g_bExitFlag = false;
		AfxBeginThread((AFX_THREADPROC)ThreadProc, (LPVOID)this);	
	}	
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
int CTotalMatchDlg::StringSplit(CString srcStr,CString strArray[],char splitChar)
{
	try
	{
		int idx1 = 0;
		int idx2 = 0;
		int iCount = 0;
		idx2 = srcStr.Find(splitChar, idx1);  //�ҵ�һ����¼ 
		while(idx2 != -1)						//����ҵ�
		{ 
			strArray[iCount] = srcStr.Mid(idx1,idx2 - idx1); //ȡ��һ����¼
			idx1 = idx2 + 1; 
			idx2 = srcStr.Find(splitChar, idx1);  //����һ����¼
			iCount++;
			if (iCount > 99)
			{
				break;
			}
		} 
		return iCount;
	}
	catch (...)
	{
		
	}
	return 0;
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
int CTotalMatchDlg::InsertIntoDb(CString strArray[],CAdoRecordSet &adoRst,CAdoConnection &adoConn)
{
	CString strSQL = "";
	strSQL.Format("insert into RollTable (rollTime,rollNum,rollStr)values('%s',%s,'%s')",strArray[0],strArray[1],strArray[2]);
	//�������ݿ�
	if (!adoRst.Open(strSQL,adCmdText) )
	{
		ShowMessage(adoConn.GetLastErrorText());
		adoRst.Close();		
		return -1;
	}
	adoRst.Close();
	return 0;
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CTotalMatchDlg::InitParams()
{
	CUserFile *pFile = CUserFile::GetInstance();
	CString strAppPath = "";
	int iTemp = 0;
	CString strTemp = "";
	pFile->GetAppPath(strAppPath);
	g_strAppPath = strAppPath;
	pFile->ReadInitInfo(strAppPath+"setex.ini","DataBase","name",g_dbServer,iTemp,0);
	pFile->ReadInitInfo(strAppPath+"setex.ini","DataBase","pwd",g_dbPwd,iTemp,0);
	g_dbServer = strAppPath + g_dbServer;
	pFile->ReadInitInfo(strAppPath+"setex.ini","Set","num",strTemp,g_iNumLen,1);
	pFile->CheckInitValue(g_iNumLen,3,1000);
	pFile->ReadInitInfo(strAppPath+"setex.ini","Set","matchtype",strTemp,g_iRightCheck,1);
	pFile->CheckInitValue(g_iRightCheck,0,1);
	((CButton*)GetDlgItem(IDC_RADIO_MATCH))->SetCheck(g_iRightCheck);
	((CButton*)GetDlgItem(IDC_RADIO_NOMATCH))->SetCheck(!g_iRightCheck);
	SetDlgItemInt(IDC_EDIT_NUMLEN,g_iNumLen);
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CTotalMatchDlg::OnBnClickedBtnStop()
{
	g_bExitFlag = true;
}
/************************************************************************/
/*�������                                                              */
/************************************************************************/
void CTotalMatchDlg::OnBnClickedBtnShowunion()
{
	if (g_bExitFlag == false)
	{
		ShowMessage("�߳�����ִ����,���Ժ��ٽ��е���...");
		return;
	}
	g_iOptType = 3;
	g_bExitFlag = false;
	AfxBeginThread((AFX_THREADPROC)ThreadProc, (LPVOID)this);
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
void CTotalMatchDlg::SetTableStyle()
{
	CListCtrl* conList = &m_list_show;
	DWORD dwSytle=::GetWindowLong(conList->m_hWnd,GWL_STYLE);
	// ����Ϊ������ʽ
	SetWindowLong(conList->m_hWnd,GWL_STYLE,dwSytle|LVS_REPORT);
	DWORD ExStyle=conList->GetExtendedStyle();
	conList->SetExtendedStyle(ExStyle|LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
	CImageList image;
	image.Create(1,16, ILC_COLOR, 0, 0);
	conList->SetImageList(&image, LVSIL_STATE);
	conList->MoveWindow(10,75,450,250);	
	conList->InsertColumn(1,"���",LVCFMT_LEFT,60);
	conList->InsertColumn(2,"����",LVCFMT_LEFT,90);
	conList->InsertColumn(3,"�ں�",LVCFMT_LEFT,70);
	conList->InsertColumn(4,"�н�����",LVCFMT_LEFT,180);
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
void CTotalMatchDlg::SetTableStyleEx()
{
	CListCtrl* conList = &m_list_info;
	DWORD dwSytle=::GetWindowLong(conList->m_hWnd,GWL_STYLE);
	// ����Ϊ������ʽ
	SetWindowLong(conList->m_hWnd,GWL_STYLE,dwSytle|LVS_REPORT);
	DWORD ExStyle=conList->GetExtendedStyle();
	conList->SetExtendedStyle(ExStyle|LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
	CImageList image;
	image.Create(1,16, ILC_COLOR, 0, 0);
	conList->SetImageList(&image, LVSIL_STATE);
	conList->MoveWindow(10,335,450,185);	
	conList->InsertColumn(1,"ʱ��",LVCFMT_LEFT,110);
	conList->InsertColumn(2,"��Ϣ",LVCFMT_LEFT,310);
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CTotalMatchDlg::OnBnClickedBtnMatch()
{
	/*COleDateTime timeDateStart;
	COleDateTime timeDateEnd;
	m_datestart.GetTime(timeDateStart);
	m_dateend.GetTime(timeDateEnd);
	COleDateTimeSpan tSpan;
	tSpan = timeDateEnd - timeDateStart;
	DWORD dDayInterval = tSpan.GetTotalDays() ;
	CString strTemp;*/
	
	if( SetParams() < 0)
	{
		return;
	}
	/*if (dDayInterval < 2)
	{
		strTemp.Format("��ƥ��%d���ڵ�����",dDayInterval);
		ShowMessage(strTemp);
		MessageBox("��ѯ�����ڼ�������������","����");
		return;
	}	
	strTemp.Format("��ƥ��%d���ڵ�����",dDayInterval);
	ShowMessage(strTemp);
	g_strDateStart = timeDateStart.Format("%Y-%m-%d");
	g_strDateEnd = timeDateEnd.Format("%Y-%m-%d");*/
	g_iOptType = 2;
	g_bExitFlag = false;
	AfxBeginThread((AFX_THREADPROC)ThreadProc, (LPVOID)this);
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CTotalMatchDlg::InitMapList()
{
	g_mapKey.clear();
	for (int i=1;i<50;i++)
	{
		g_mapKey[i] = 0;
	}
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
//д��־
void CTotalMatchDlg::WriteLog(CString srcStr)
{
	CTime currTime = CTime::GetCurrentTime();
	try
	{
		CString filepath = "";
		CString destStr = "";
		CString folder = "";
		CString filename = "";
		CString strAppPath = g_strAppPath;
		int iFlag = 1;
		if (iFlag == 0)			//������־
		{
			folder = "log";
			filename.Format("%s_sendlog.txt",currTime.Format("%Y%m%d"));
		}
		else if(iFlag == 1)		//��Ϣ��־
		{
			folder = "log";
			filename.Format("%s_log.txt",currTime.Format("%Y%m%d"));
		}
		else if ( iFlag == 2 )	//������־
		{
			folder = "log";
			filename.Format("%s_bufinfo.txt",currTime.Format("%Y%m%d"));
		}
		else			//�쳣��־ (3)
		{
			folder = "exception";
			filename.Format("%s_exception.txt",currTime.Format("%Y%m%d"));
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
/************************************************************************/
/*                                                                      */
/************************************************************************/
//���Ĭ�ϵ��ļ���
BOOL CTotalMatchDlg::GetDefaultXlsFileName(CString& sExcelFile)
{
	///Ĭ���ļ�����yyyymmddhhmmss.xls
	CString timeStr = "";
	timeStr = CTime::GetCurrentTime().Format("%Y%m%d%H%M%S");
	timeStr = "ƥ�����-����ʱ��["+timeStr+"]";

	sExcelFile =  timeStr + ".txt";
	// prompt the user (with all document templates)
	CFileDialog dlgFile(FALSE,".txt",sExcelFile);
	CString title;
	CString strFilter;

	title = "����";
	strFilter = "TXT �ļ�(*.txt)";
	strFilter += (TCHAR)'\0';   // next string please
	strFilter += _T("*.txt");
	strFilter += (TCHAR)'\0';   // last string
	dlgFile.m_ofn.nMaxCustFilter++;
	dlgFile.m_ofn.nFilterIndex = 1;
	// append the "*.*" all files filter
	CString allFilter;
	VERIFY(allFilter.LoadString(AFX_IDS_ALLFILTER));
	strFilter += allFilter;
	strFilter += (TCHAR)'\0';   // next string please
	strFilter += _T("*.*");
	strFilter += (TCHAR)'\0';   // last string
	dlgFile.m_ofn.nMaxCustFilter++;

	dlgFile.m_ofn.lpstrFilter = strFilter;
	dlgFile.m_ofn.lpstrTitle = title;

	if (dlgFile.DoModal()==IDCANCEL)
	{
		return FALSE; // open cancelled
	}	
	sExcelFile.ReleaseBuffer();
	sExcelFile = dlgFile.GetPathName();
	sExcelFile.MakeLower();
	if (sExcelFile.Right(4) != ".txt")
	{
		sExcelFile+=".txt";
	}
	if (MakeSurePathExists(sExcelFile,true)) 
	{
		if(!DeleteFile(sExcelFile)) 
		{    // delete the file
			AfxMessageBox("�����ļ�ʱ����");
			return FALSE;
		}
	}
	return TRUE;
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
BOOL CTotalMatchDlg::MakeSurePathExists( CString &Path,bool FilenameIncluded)
{
	int Pos=0;
	while((Pos=Path.Find('\\',Pos+1))!=-1)
	{
		CreateDirectory(Path.Left(Pos),NULL);
	}
	if(!FilenameIncluded)
	{
		CreateDirectory(Path,NULL);
	}
	return !_access(Path,0);
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
void CTotalMatchDlg::WriteFileLog(CString strFilePath,CString srcStr)
{
	try
	{
		FILE *fp;
		char szfile[1000]={0};
		sprintf(szfile,strFilePath);
		fp=fopen(szfile,"a+");
		fprintf(fp,"%s\r\n",srcStr);
		fclose(fp);
	}
	catch (...)
	{
	}
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
int CTotalMatchDlg::GetPower()
{
	int i=0;
#ifdef _DEBUG
	return 0;
#endif
	return 0;
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
				if (strTemp.Find("myownpower2010029") !=-1)
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
BOOL CTotalMatchDlg::PreTranslateMessage( MSG* pMsg)
{
	if( pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE)
	{
		return TRUE;      //���ε�Esc��
	}
	if( pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
	{
		return TRUE;      //���ε�Enter��
	}
	return CDialog::PreTranslateMessage( pMsg );
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CTotalMatchDlg::OnBnClickedRadioMatch()
{
	((CButton*)GetDlgItem(IDC_RADIO_MATCH))->SetCheck(TRUE);
	((CButton*)GetDlgItem(IDC_RADIO_NOMATCH))->SetCheck(FALSE);
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CTotalMatchDlg::OnBnClickedRadioNomatch()
{
	((CButton*)GetDlgItem(IDC_RADIO_MATCH))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_RADIO_NOMATCH))->SetCheck(TRUE);
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
int CTotalMatchDlg::SetParams()
{
	g_iRightCheck = ((CButton*)GetDlgItem(IDC_RADIO_MATCH))->GetCheck();
	g_iNumLen = GetDlgItemInt(IDC_EDIT_NUMLEN);
	if (g_iNumLen < 3 && g_iRightCheck == 0)
	{
		ShowMessage("��ƥ��ʱƥ�����������������");
		return -1;
	}
	CUserFile *pFile = CUserFile::GetInstance();
	CString strAppPath = "";
	pFile->GetAppPath(strAppPath);
	pFile->WriteInitInfo(strAppPath+"setex.ini","Set","num","",g_iNumLen,1);
	pFile->WriteInitInfo(strAppPath+"setex.ini","Set","matchtype","",g_iRightCheck,1);	
	return 0;
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CTotalMatchDlg::ShowDbData()
{
	CString strSQL = "";
	//strSQL.Format("select  * from RollTable where (rollTime > #%s# or rollTime=#%s#) and (rollTime < #%s# or rollTime=#%s#)",
	//	g_strDateStart,g_strDateStart,g_strDateEnd,g_strDateEnd);
	strSQL = "select count(*) as TotalCount from RollTable";
	CAdoConnection adoConn;
	CAdoRecordSet adoRst;
	if(!adoConn.ConnectAccess(g_dbServer,g_dbPwd))
	{
		ShowMessage("�������ݿ�ʧ��");
		return;	
	}
	adoRst.SetAdoConnection(&adoConn);
	adoRst.SetCursorLocation(adUseClient);
	if (!adoRst.Open(strSQL))
	{
		ShowMessage("��ѯʧ��:"+adoConn.GetLastErrorText());
		adoRst.Close();
		adoConn.Close();
		return;
	}
	CString strShow = "";
	int iTemp = 0;
	if (!adoRst.IsEOF())
	{
		adoRst.GetCollect("TotalCount",iTemp);		
	}
	strShow.Format("�����ʼ���ɹ�,������ %d ��������¼",iTemp);
	ShowMessage(strShow);
	adoRst.Close();
	adoConn.Close();
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CTotalMatchDlg::GetResultNoMatch()
{
	CString strRollEx = "";
	g_strMatchNum = "";
	CString strFormat = "";
	map<int,int>::iterator it;
	int iNum = 0;
	CString strNum = "";
	for (int i=0;i<49;i++)
	{
		g_iArray[iNum] = -1;
	}
	for(it=g_mapKey.begin();it!=g_mapKey.end();++it)
	{
		if(it->second == g_iRightCheck)
		{
			g_iArray[iNum] = it->first;
			strNum.Format("%d",it->first);
			g_strMatchNum += strNum +",";
			iNum++;
		}
	}
	g_iNumMatched = iNum;
	strFormat.Format("ƥ�����,����%d������%s���ֹ�",g_iNumMatched,g_iRightCheck == 0?"δ":"�Ѿ�");
	ShowMessage(strFormat);
	ShowMessage(g_strMatchNum);
	if (g_iNumMatched < 5 )
	{
		strFormat.Format("%s���ֹ������ָ���С����,��ƥ����",g_iRightCheck == 0?"δ":"�Ѿ�");
		ShowMessage(strFormat);
		return;
	}
	//��������������ĺ�����������ʾ����
	int iMatchKey = 5;
	if (g_iNumMatched-5 > 5)
	{
		iMatchKey = g_iNumMatched - 5;
	}
	g_lTotalCount = 1;
	for (int i=g_iNumMatched;i>iMatchKey;i--)
	{
		g_lTotalCount = g_lTotalCount * i;
	}
	for (int i=1;i<g_iNumMatched - iMatchKey + 1;i++)
	{
		g_lTotalCount = g_lTotalCount / i;
	}
	strFormat.Format("����%d�������������",(int)g_lTotalCount);
	ShowMessage(strFormat);
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
int CTotalMatchDlg::GetResultMatch()
{
	CString strArray[100] = {""};
	int iArray[7] = {0};
	int iRet = 0;
	int iDataLocation = 0;
	int iDataLocation1 = (iDataLocation+1)%2;
	int iTemp = 0;
	g_ALLDataArray[iDataLocation].clear();
	g_ALLDataArray[iDataLocation1].clear();
	CString strRollResult = "";
	CString strFormat = "";
	vector<CString>::iterator it_vec;
	ResultList::iterator it_ret;
	int iResultLen = 0;
	int iNum = 0;
	int iLen1 = 0;
	int iLen2 = 0;
	int iLenen = sizeof(g_iMatchArrayEx[iDataLocation]);
	memset(g_iMatchArrayEx[0],-1,iLenen);
	memset(g_iMatchArrayEx[1],-1,iLenen);
	for (it_vec = g_vec_ret.begin();it_vec !=g_vec_ret.end();it_vec++)		//������������
	{//��ÿһ�ڵ����ݽ��бȶ�
		strRollResult = *it_vec;
		iRet = StringSplit(strRollResult+"-",strArray,'-');
		if (iRet != 7)
		{
			continue;
		}
		for (int i=0;i<7;i++)
		{
			iArray[i] = atoi(strArray[i]);
		}
		//g_ALLDataArray[iDataLocation].clear();		
		memset(g_iMatchArrayEx[iDataLocation],-1,iLenen);
		if (iNum == 0)
		{
			iLen1 = 0;
			for (int i=0;i<g_iTotalResult;i++)
			{				
				if (IsInResult(iArray,g_iMatchArray[i]) == true)	//ƥ�䵽��,������ת��һ��
				{
					g_iMatchArrayEx[iDataLocation][iLen1] = i+1;
					iLen1++;
				}				
			}
		}
		else
		{
			int i = 0;
			iLen1 = 0;
			int iVal = 0;
			while(true)
			{
				iVal = g_iMatchArrayEx[iDataLocation1][i];
				if ( iVal< 1 || i>= g_iTotalResult)
				{
					break;
				}
				if(IsInResult(iArray,g_iMatchArray[iVal-1]) == true)
				{
					g_iMatchArrayEx[iDataLocation][iLen1] = iVal;
					iLen1++;
				}
				i++;
			}
		}		
		iNum++;
		iResultLen = iLen1;
		strFormat.Format("����%d�ֹ��˺�,����%d�����������������",iNum,iResultLen);
		ShowMessage(strFormat);
		if (iResultLen < 1)
		{
			ShowMessage("��ƥ��Ľ��");
			return -1;
		}
		//������
		iDataLocation = (iDataLocation+1)%2;
		iDataLocation1 = (iDataLocation1+1)%2;
	}
	g_iDataLocation = iDataLocation1;
	g_iTotalLenth = iResultLen;
	strFormat.Format("ƥ����ɺ�,����%d�����������������",iResultLen);
	ShowMessage(strFormat);
	return 0;
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
bool CTotalMatchDlg::IsInResult(int iArray1[7],int iArray2[5])
{	
	for (int i=0;i<7;i++)
	{
 		for (int j=0;j<5;j++)
 		{
			if (iArray1[i] == iArray2[j])
			{
				return true;
			}
 		}
	}
	return false;
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CTotalMatchDlg::OutputNoMatch()
{
	try
	{
		CString strFormat = "";
		CString sExcelFile = "";
		if (!GetDefaultXlsFileName(sExcelFile))
		{
			ShowMessage("ȷ��·������.");
			return;
		}
		strFormat.Format("ƥ������:%s - ����:%d",g_iRightCheck == 0?STR_NORIGHT_SHOW:STR_RIGHT_SHOW,g_iNumLen);
		WriteFileLog(sExcelFile,strFormat);
		//WriteFileLog(sExcelFile,"");				
		strFormat.Format("����%d������%s���ֹ�",g_iNumMatched,g_iRightCheck == 0?"δ":"�Ѿ�");
		WriteFileLog(sExcelFile,strFormat);
		WriteFileLog(sExcelFile,"������������������:");
		WriteFileLog(sExcelFile,g_strMatchNum);
		strFormat.Format("����������������ϸ���: %d",g_lTotalCount);
		WriteFileLog(sExcelFile,strFormat);
		WriteFileLog(sExcelFile,"�����������������:");
		WriteFileLog(sExcelFile,"");
		int iNum = 1;
		for (int i=0;i<g_iNumMatched;i++)
		{
			for (int j=i+1;j<g_iNumMatched;j++)
			{
				for (int k=j+1;k<g_iNumMatched;k++)
				{
					for(int m=k+1;m<g_iNumMatched;m++)
					{
						for (int n=m+1;n<g_iNumMatched;n++)
						{
							strFormat.Format("%d: %d-%d-%d-%d-%d",iNum,g_iArray[i],g_iArray[j],g_iArray[k],g_iArray[m],g_iArray[n]);
							WriteFileLog(sExcelFile,strFormat);
							//ShowMessage(strFormat);
							iNum++;
						}
					}
				}
			}
		}
		ShowMessage("�������");		
	}
	catch(...)
	{
		ShowMessage("ִ���쳣,����δ���");
	}
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CTotalMatchDlg::OutputMatch()
{
	try
	{
		CString strFormat = "";
		CString sExcelFile = "";
		if (!GetDefaultXlsFileName(sExcelFile))
		{
			ShowMessage("ȷ��·������.");
			return;
		}
		strFormat.Format("ƥ������:%s - ����:%d",g_iRightCheck == 0?STR_NORIGHT_SHOW:STR_RIGHT_SHOW,g_iNumLen);
		WriteFileLog(sExcelFile,strFormat);			
		//strFormat.Format("����%d������%s���ֹ�",g_iNumMatched,g_iRightCheck == 0?"δ":"�Ѿ�");
		//WriteFileLog(sExcelFile,strFormat);
		//WriteFileLog(sExcelFile,"������������������:");
		//WriteFileLog(sExcelFile,g_strMatchNum);
		strFormat.Format("����������������ϸ���: %d",g_iTotalLenth);
		WriteFileLog(sExcelFile,strFormat);
		WriteFileLog(sExcelFile,"�����������������:");
		WriteFileLog(sExcelFile,"");
		int iNum = 1;
		int i = 0;
		int iVal = 0;
		while(true)
		{
			iVal = g_iMatchArrayEx[g_iDataLocation][i];
			if ( iVal< 1 || i>= g_iTotalResult)
			{
				break;
			}
			iVal--;
			strFormat.Format("%d: %d-%d-%d-%d-%d",i+1,g_iMatchArray[iVal][0],g_iMatchArray[iVal][1],g_iMatchArray[iVal][2],g_iMatchArray[iVal][3],g_iMatchArray[iVal][4]);
			WriteFileLog(sExcelFile,strFormat);
			i++;
		}
		ShowMessage("�������");		
	}
	catch(...)
	{
		ShowMessage("ִ���쳣,����δ���");
	}
}
#include "stdafx.h"
#include "SendLogQuery.h"
#include "SendLogQueryDlg.h"
#include ".\sendlogquerydlg.h"
#include <io.h>
#include <odbcinst.h>
#include <afxdb.h>
#include "InfoShow.h"
#include "ServiceDetail.h"
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


// CSendLogQueryDlg �Ի���
CSendLogQueryDlg::CSendLogQueryDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSendLogQueryDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSendLogQueryDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROGRESSBAR, m_ProgressCtrl);
	DDX_Control(pDX, IDC_LIST_SHOW, m_resultList);
	DDX_Control(pDX, IDC_LIST_SHOW3, m_listtype);
	DDX_Control(pDX, IDC_COMBO_ACCTYPE, m_comboType);
	DDX_Control(pDX, IDC_COMBO_LANGUAGE, m_combo_language);
}

BEGIN_MESSAGE_MAP(CSendLogQueryDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BTN_QUERY, OnBnClickedBtnQuery)
	ON_BN_CLICKED(IDC_BTN_STOP, OnBnClickedBtnStop)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BTN_OUTPORT, OnBnClickedBtnOutport)
	ON_BN_CLICKED(IDC_BTN_ADD, OnBnClickedBtnAdd)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_SHOW3, OnNMDblclkListShow3)
	ON_BN_CLICKED(IDC_BTN_FRESH, &CSendLogQueryDlg::OnBnClickedBtnFresh)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_SHOW, &CSendLogQueryDlg::OnNMRclickListShow)
	ON_COMMAND(IDR_MENU_DELETE, &CSendLogQueryDlg::OnMenuDelete)
	ON_COMMAND(IDR_MENU_EDIT, &CSendLogQueryDlg::OnMenuEdit)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_SHOW, &CSendLogQueryDlg::OnNMDblclkListShow)
	ON_CBN_SELCHANGE(IDC_COMBO_ACCTYPE, &CSendLogQueryDlg::OnCbnSelchangeComboAcctype)
END_MESSAGE_MAP()


// CSendLogQueryDlg ��Ϣ�������

BOOL CSendLogQueryDlg::OnInitDialog()
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

	m_Counter = 50;
	m_ProgressCtrl.SetRange(0,50);
	m_ProgressCtrl.SetPos(0);
	SetTimer(1,50,NULL);
	MoveWindow(0,0,1000,700);
	InitCtrList();
	GetDlgItem(IDC_PROGRESSBAR)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_BTN_OUTPORT)->EnableWindow(FALSE);
	//��ȡ���ݿ�����
	CString strAppPath;
	GetAppPath(strAppPath);
	CString strFilePath = strAppPath + "set.ini";
	char szTemp[1000] = {0};
	::GetPrivateProfileString("database", "dbserver", "",(LPSTR)szTemp,1000, strFilePath);
	m_dbServer = szTemp;

	memset(szTemp,0,1000);
	::GetPrivateProfileString("database", "dbname", "",(LPSTR)szTemp,1000, strFilePath);
	m_dbName = szTemp;

	memset(szTemp,0,1000);
	::GetPrivateProfileString("database", "dbuser", "",(LPSTR)szTemp,1000, strFilePath);
	m_dbUser = szTemp;

	memset(szTemp,0,1000);
	::GetPrivateProfileString("database", "dbpwd", "",(LPSTR)szTemp,1000, strFilePath);
	m_dbPwd = szTemp;
	memset(szTemp,0,1000);
	//::GetPrivateProfileString("database", "dbshortpath", "",(LPSTR)szTemp,1000, strFilePath);
	//m_dbPath = szTemp;
	//m_dbPath = "customer.mdb";
	//m_dbPath = strAppPath + m_dbPath;
	OnBnClickedBtnFresh();
	return TRUE;  // ���������˿ؼ��Ľ��㣬���򷵻� TRUE
}

void CSendLogQueryDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CSendLogQueryDlg::OnPaint() 
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
HCURSOR CSendLogQueryDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
void CSendLogQueryDlg::OnTimer(UINT nIDEvent)
{
	m_Counter++;
	if ( m_Counter <= 50 )
	{
		m_ProgressCtrl.SetPos(m_Counter);	
	}
	else
	{
		m_Counter = 0;
	}	
	CDialog::OnTimer(nIDEvent);
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CSendLogQueryDlg::ThreadProc(LPVOID lpParam)
{
	CSendLogQueryDlg *pWnd = (CSendLogQueryDlg *)((ThreadParams *)lpParam)->pWnd;
	int iFlag = ((ThreadParams *)lpParam)->m_iNo;
	::CoInitialize(NULL);
	pWnd->OperateProc(iFlag);
	::CoUninitialize();	
	delete lpParam;
	lpParam = NULL;
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
void CSendLogQueryDlg::OperateProc(int iFlag)
{
	if (iFlag != 10)
	{
		SetDlgItemText(IDC_INFO_SHOW,"���ڲ�ѯ,���Ժ�...");
		GetDlgItem(IDC_PROGRESSBAR)->ShowWindow(SW_SHOW);
		QuerySendLog(m_strWhere,m_iSendFlag,&(m_ThreadContrl.ThreadExit),m_iCurrPage,m_IsShowDetail);
	}
	else		//������־
	{
		GetDlgItem(IDC_PROGRESSBAR)->ShowWindow(SW_HIDE);
		SetDlgItemText(IDC_INFO_SHOW,"");
		if (m_IsShowDetail)
		{
			ExportToExcel("������־��¼",m_strWhere,m_iSendFlag,&(m_ThreadContrl.ThreadExit),m_iCurrPage,m_IsShowDetail);
		}
		else		//����ÿ����־
		{
//			ExportToExcelEx("ÿ�շ��ͳɹ���ͳ��");
		}
	}

	GetDlgItem(IDC_PROGRESSBAR)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_BTN_QUERY)->EnableWindow(TRUE);
	if ( !m_IsShowDetail )
	{
		GetDlgItem(IDC_BTN_OUTPORT)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_BTN_OUTPORT)->EnableWindow(TRUE);
	}	
	GetDlgItem(IDC_PROGRESSBAR)->ShowWindow(SW_HIDE);
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
void CSendLogQueryDlg::OnBnClickedBtnQuery()
{
	//��֤��ѯ����
	
// 	GetDlgItemText(IDC_DATE_DATESTART,m_strStartDate);
// 	GetDlgItemText(IDC_DATE_DATEEND,m_strEndDate);
// 	CTime startTime = GetCTimeFromString(m_strStartDate + " 00:00:00");
// 	CTime endTime = GetCTimeFromString(m_strEndDate + "  23:59:59");
// 	CTimeSpan timeSpan;
// 	timeSpan = endTime - startTime;
// 	if (timeSpan < 0 )
// 	{
// 		SetDlgItemText(IDC_INFO_SHOW,"��ʼʱ�����С�ڽ���ʱ��");
// 		return;
// 	}
	m_strWhere = " where 1=1";
	CString strTemp = "";
	CString strFormat = "";
	int iTemp = 0;
	GetDlgItemText(IDC_COMBO_ACCTYPE,strTemp);
	strTemp.Trim();
	iTemp = m_typeMap[strTemp];
	//strTemp.Format("%d",iTemp);
	//MessageBox(strTemp);
	if (iTemp != -1 && iTemp != 0)
	{
		strFormat.Format(" and [type] = %d",iTemp);
		m_strWhere += strFormat;
	}

	GetDlgItemText(IDC_COMBO_LANGUAGE,strTemp);
	strTemp.Trim();
	iTemp = m_languageMap[strTemp];

	if (iTemp != -1 && iTemp != 0)
	{
		strFormat.Format(" and [language] = %d",iTemp);
		m_strWhere += strFormat;
	}

	GetDlgItemText(IDC_EDIT_COMNAME,strTemp);
	if (strTemp != "")
	{
		strFormat.Format(" and comname like '%%%s%%'",strTemp);
		m_strWhere += strFormat;
	}	
	strTemp = "";
	GetDlgItemText(IDC_EDIT_LINKMAN,strTemp);
	if (strTemp != "")
	{
		strFormat.Format(" and linkman like '%%%s%%'",strTemp);
		m_strWhere += strFormat;
	}
	strTemp = "";
	GetDlgItemText(IDC_EDIT_LINKPHONE,strTemp);
	if (strTemp != "")
	{
		strFormat.Format(" and linkphone like '%%%s%%'",strTemp);
		m_strWhere += strFormat;
	}
	strTemp = "";
	GetDlgItemText(IDC_EDIT_QQ,strTemp);
	if (strTemp != "")
	{
		strFormat.Format(" and linkqq like '%%%s%%'",strTemp);
		m_strWhere += strFormat;
	}
	strTemp = "";
	GetDlgItemText(IDC_EDIT_ACCOUNT,strTemp);
	if (strTemp != "")
	{
		strFormat.Format(" and account like '%%%s%%'",strTemp);
		m_strWhere += strFormat;
	}

	strTemp = "";
	GetDlgItemText(IDC_EDIT_SALEMAN,strTemp);
	if (strTemp != "")
	{
		strFormat.Format(" and saleman like '%%%s%%'",strTemp);
		m_strWhere += strFormat;
	}
	strTemp = "";
	GetDlgItemText(IDC_EDIT_SENDNUM,strTemp);
	if (strTemp != "")
	{
		strFormat.Format(" and (num > %s or num =%s)",strTemp,strTemp);
		m_strWhere += strFormat;
	}

	/************************************************************************/
	//��ʼ��ѯ
	OperateProc(1);
}


/************************************************************************/
/*                                                                      */
/************************************************************************/
void CSendLogQueryDlg::InitCtrList()
{
	DWORD dwSytle=::GetWindowLong(m_resultList.m_hWnd,GWL_STYLE);
	//����Ϊ������ʽ
	SetWindowLong(m_resultList.m_hWnd,GWL_STYLE,dwSytle|LVS_REPORT);
	DWORD ExStyle=m_resultList.GetExtendedStyle();
	m_resultList.SetExtendedStyle(ExStyle|LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
	::SendMessage(m_resultList.m_hWnd, LVM_SETEXTENDEDLISTVIEWSTYLE,
		LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);
	m_resultList.InsertColumn(0,_T("���"),LVCFMT_LEFT,50);
	m_resultList.InsertColumn(1,_T("��˾��"),LVCFMT_LEFT,110);
	m_resultList.InsertColumn(2,_T("��������"),LVCFMT_LEFT,110);
	m_resultList.InsertColumn(3,_T("ʹ������"),LVCFMT_LEFT,80);
	m_resultList.InsertColumn(4,_T("��ϵ��"),LVCFMT_LEFT,80);
	m_resultList.InsertColumn(5,_T("�绰"),LVCFMT_LEFT,100);
	m_resultList.InsertColumn(6,_T("QQ[Email]"),LVCFMT_LEFT,120);
	m_resultList.InsertColumn(7,_T("�˺�"),LVCFMT_LEFT,60);
	m_resultList.InsertColumn(8,_T("����"),LVCFMT_LEFT,60);
	m_resultList.InsertColumn(9,_T("ҵ��Ա"),LVCFMT_LEFT,80);
	m_resultList.InsertColumn(10,_T("������"),LVCFMT_LEFT,90);
	m_resultList.InsertColumn(11,_T("��������"),LVCFMT_LEFT,100);
	m_resultList.InsertColumn(12,_T("��ע"),LVCFMT_LEFT,200);
	m_resultList.MoveWindow(1,115,991,545);
	//����Ϊ������ʽ
	 dwSytle=::GetWindowLong(m_resultList.m_hWnd,GWL_STYLE);

	//����Ϊ������ʽ
	SetWindowLong(m_listtype.m_hWnd,GWL_STYLE,dwSytle|LVS_REPORT);
	ExStyle=m_listtype.GetExtendedStyle();
	m_listtype.SetExtendedStyle(ExStyle|LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
	::SendMessage(m_listtype.m_hWnd, LVM_SETEXTENDEDLISTVIEWSTYLE,
		LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);
	m_listtype.InsertColumn(0,_T("��������"),LVCFMT_LEFT,150);
	m_listtype.InsertColumn(1,_T("�ͻ�����"),LVCFMT_LEFT,100);
	//m_listtype.InsertColumn(2,_T("���"),LVCFMT_LEFT,60);
	m_listtype.MoveWindow(655,10,336,98);
	GetDlgItem(IDC_PROGRESSBAR)->MoveWindow(1,110,991,3);
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
void CSendLogQueryDlg::QuerySendLog(CString strWhere,int iFlag,BOOL *bExit,int iCurrPage,BOOL bDetail)
{	
	iCurrPage = 1;
	m_resultList.DeleteAllItems();
	bDetail = TRUE;
	//m_showDay.DeleteAllItems();	
	if (bDetail==TRUE)		//���Ҫ��ѯ��ϸ��Ϣ
	{
		//m_showDay.ShowWindow(SW_HIDE);
		m_resultList.ShowWindow(SW_SHOW);
	}
	else		//����ѯ��ϸ��Ϣ
	{	
		//m_showDay.ShowWindow(SW_SHOW);
		m_resultList.ShowWindow(SW_HIDE);
	}
	try
	{
		if ( !m_adoConn.IsOpen() )		//���δ�����Ϸ�����
		{
			if ( !m_adoConn.ConnectSQLServer(m_dbServer,m_dbName,m_dbUser,m_dbPwd))//.ConnectSQLServer( m_dbServer , m_dbName, m_dbUser,m_dbPwd) )
			{
				SetDlgItemText(IDC_INFO_SHOW,"�������ݿ�ʧ��");
				return ; 
			}
			m_adoRecrd.SetAdoConnection(&m_adoConn);
			m_adoRecrd.SetCursorLocation(adUseClient);
		}
		CString strSQL;
		CString strTempWhere;
		CString strTemp;
		int iTemp = 0;
		bool bQuit = false;		//�ֶ��˳�ѭ��
		if ( bDetail != TRUE)		//����ʾ��ϸ��Ϣ
		{			
//			GetDaySucCount(m_strStartDate,m_strEndDate,m_adoConn,m_adoRecrd);
		}
		else	//��ʾ��ϸ��Ϣ
		{
			//strSQL.Format("select * from Acctype left join (Customer left join langType on Customer.language = langType.id) on Customer.type = Acctype.id");
			strSQL.Format("select * from (select * from Customer left join Acctype on Customer.type = Acctype.typeid) as table1 left join langType on table1.[language] = langType.langid %s order by firsttime asc,customerid asc",m_strWhere);

			//strSQL.Format("select * from Customer left join Acctype on Customer.[language] = Acctype.id");

			if( !m_adoRecrd.Open(strSQL,adCmdText))
			{
				m_adoRecrd.Close();
				SetDlgItemText(IDC_INFO_SHOW,"ִ�г���,"+m_adoConn.GetLastErrorText());

				return;
			}
			
			COleDateTime tempTime;
			while( !m_adoRecrd.IsEOF())		//�����¼��Ϊ��
			{
				if ( *bExit == TRUE )
				{
					bQuit = true;
					break;
				}
				int iCount = m_resultList.GetItemCount();
				m_resultList.InsertItem( iCount, "");			
				m_adoRecrd.GetCollect("customerid",iTemp);
				strTemp.Format("%d",iTemp);
				m_resultList.SetItemText( iCount , 0,strTemp);
	
				m_adoRecrd.GetCollect("comname",strTemp);
				m_resultList.SetItemText( iCount , 1,strTemp);

				m_adoRecrd.GetCollect("typename",strTemp);
				m_resultList.SetItemText( iCount , 2,strTemp);

				m_adoRecrd.GetCollect("langName",strTemp);
				m_resultList.SetItemText( iCount , 3,strTemp);

				m_adoRecrd.GetCollect("linkman",strTemp);
				m_resultList.SetItemText( iCount , 4,strTemp);

				m_adoRecrd.GetCollect("linkphone",strTemp);
				m_resultList.SetItemText( iCount , 5,strTemp);

				CString strQQEmail = "";
				m_adoRecrd.GetCollect("linkqq",strTemp);
				strQQEmail = strTemp;
				m_adoRecrd.GetCollect("linkemail",strTemp);
				strTemp.Trim();
				if (strTemp != "")
				{
					strQQEmail += "["+strTemp+"]";
				}				
				m_resultList.SetItemText( iCount ,6,strQQEmail);

				m_adoRecrd.GetCollect("account",strTemp);
				m_resultList.SetItemText( iCount ,7,strTemp);

				m_adoRecrd.GetCollect("password",strTemp);
				m_resultList.SetItemText( iCount ,8,strTemp);

				m_adoRecrd.GetCollect("saleman",strTemp);
				m_resultList.SetItemText( iCount ,9,strTemp);

				m_adoRecrd.GetCollect("num",iTemp);	
				strTemp.Format("%d",iTemp);
				m_resultList.SetItemText( iCount ,10,strTemp);
				
				m_adoRecrd.GetCollect("firsttime",tempTime);			
				m_resultList.SetItemText( iCount ,11,tempTime.Format("%Y-%m-%d"));
				m_adoRecrd.GetCollect("reverse1",strTemp);	
				m_resultList.SetItemText( iCount ,12,strTemp);
				m_adoRecrd.MoveNext();
			}
		}
		if ( bQuit )
		{
			SetDlgItemText(IDC_INFO_SHOW,"�ֶ�ֹͣ,��ѯδ���");
		}
		else
		{
			SetDlgItemText(IDC_INFO_SHOW,"��ѯ���");
		}
		
	}
	catch(...)
	{
		SetDlgItemText(IDC_INFO_SHOW,"ִ���쳣,��ѯδ���");
	}
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CSendLogQueryDlg::OnBnClickedBtnStop()
{
	m_ThreadContrl.ThreadExit = TRUE;
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
CTime CSendLogQueryDlg::GetCTimeFromString(CString strTime)
{
	int   nYear,   nMonth,   nDate,   nHour,   nMin,   nSec;   
	sscanf(strTime,"%d-%d-%d %d:%d:%d",&nYear,&nMonth,&nDate,&nHour,&nMin,&nSec);   
	return CTime(nYear,nMonth,nDate,nHour,nMin,nSec);
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CSendLogQueryDlg::OnClose()
{
	OnBnClickedBtnStop();
	Sleep(500);
	CDialog::OnClose();
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
//����Ӧ�ó�������Ŀ¼������"\"
int CSendLogQueryDlg::GetAppPath(CString &strAppPath)
{
	int iRet = -3;
	try
	{
		TCHAR szBuff[_MAX_PATH];
		VERIFY(::GetModuleFileName(AfxGetApp()->m_hInstance, szBuff, _MAX_PATH));
		strAppPath=szBuff;
		int pos=strAppPath.ReverseFind('\\');
		strAppPath=strAppPath.Left(pos+1);		
		iRet = 0;
	}
	catch (...)
	{
	}
	return iRet;	
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CSendLogQueryDlg::OnBnClickedBtnOutport()
{
	/************************************************************************/
	//��ʼ��ѯ
	ThreadParams *lpAddBufParam = new ThreadParams;
	lpAddBufParam->pWnd = this;
	lpAddBufParam->m_iNo = 10;		//1�ǲ�ѯ 
	m_ThreadContrl.pThread = AfxBeginThread((AFX_THREADPROC)ThreadProc, (LPVOID)lpAddBufParam);
	m_ThreadContrl.ThreadExit = FALSE;
}


/************************************************************************/
/*                                                                      */
/************************************************************************/
void CSendLogQueryDlg::ExportToExcel(CString strTitle,CString strWhere,int iFlag,BOOL *bExit,int iCurrPage,BOOL bDetail)
{
	try
	{
		CDatabase database;
		CString sDriver;
		CString sExcelFile; 
		CString sSql;
		CString tableName = strTitle;

		// �����Ƿ�װ��Excel���� "Microsoft Excel Driver (*.xls)" 
		sDriver = GetExcelDriver();
		if (sDriver.IsEmpty())
		{
			// û�з���Excel����
			SetDlgItemText(IDC_INFO_SHOW,"���Ȱ�װExcel�������ʹ�õ�������");
			return;
		}
		if ( !m_adoConn.IsOpen() )		//���δ�����Ϸ�����
		{
			if ( !m_adoConn.ConnectSQLServer(m_dbServer,m_dbName,m_dbUser,m_dbPwd) )
			{
				SetDlgItemText(IDC_INFO_SHOW,"�������ݿ�ʧ��");
				return ; 
			}
			m_adoRecrd.SetAdoConnection(&m_adoConn);
			m_adoRecrd.SetCursorLocation(adUseClient);
		}
		CString strSQL;		
		//strSQL.Format("select localID,PhoneNo,content,SendTime,FinishTime,ReadStatus,SendStatus,msglen,PhoneCount,FailReason,smstype from win22_SMSendLog_MW %s",strTempWhere);
		strSQL.Format("select * from (select * from Customer left join Acctype on Customer.type = Acctype.typeid) as table1 left join langType on table1.[language] = langType.langid %s order by firsttime asc,customerid asc",m_strWhere);
		if( !m_adoRecrd.Open(strSQL,adCmdText))
		{
			m_adoRecrd.Close();
			SetDlgItemText(IDC_INFO_SHOW,"ִ�г���,����δ���");
			return;
		}

		if ( m_adoRecrd.IsEOF() )
		{
			SetDlgItemText(IDC_INFO_SHOW,"����Ϣ�ɵ���");
			m_adoRecrd.Close();
			return;
		}
		///Ĭ���ļ���
		if (!GetDefaultXlsFileName(sExcelFile))
		{
			return;
		}
		SetDlgItemText(IDC_INFO_SHOW,"���ڽ��е���,���Ժ�...");
		GetDlgItem(IDC_PROGRESSBAR)->ShowWindow(SW_SHOW);
		// �������д�ȡ���ַ���
		sSql.Format("DRIVER={%s};DSN='';FIRSTROWHASNAMES=1;READONLY=FALSE;CREATE_DB=\"%s\";DBQ=%s",sDriver, sExcelFile, sExcelFile);
		COleDateTime tempTime;
		bool bQuit = false;
		CString localID;
		CString comName;
		CString accType;
		CString language;
		CString linkman;
		CString linkphone;
		CString linkqq;
		CString linkemail;
		CString account;
		CString pwd;
		CString sendnum;
		CString accdate;
		CString serverip;
		CString serveraccount;
		CString serverpwd;
		CString strreverse;
		CString saleman;
		CString salemanphone;
		CString salemanqq;
		CString salemanEmail;
		CString strPort;
		int iTemp;
		// �������ݿ� (��Excel����ļ�)
		if( database.OpenEx(sSql,CDatabase::noOdbcDialog) )
		{
			CString strTemp;
			strTemp.Format("��� INT,��˾�� TEXT,�������� TEXT,ʹ������ TEXT,��ϵ�� TEXT,��ϵ�绰 TEXT,QQ TEXT,Email TEXT,ҵ��Ա TEXT,ҵ��Ա�绰 TEXT,ҵ��ԱQQ TEXT,ҵ��ԱEmail TEXT,�˺� TEXT,���� TEXT,������ INT,�������� TEXT,������IP TEXT,�������˿� TEXT,�������˺� TEXT,���������� TEXT,��ע TEXT");
			// ������ṹ			
			sSql = "CREATE TABLE " + tableName + " ( " + strTemp +  " ) ";
			int i = 0;
			database.ExecuteSQL(sSql);			
		}
		else
		{
			SetDlgItemText(IDC_INFO_SHOW,"����EXCEL�ļ�����,����δ���");
			return;
		}
		while( !m_adoRecrd.IsEOF())		//�����¼��Ϊ��
		{
			if ( *bExit == TRUE )
			{
				bQuit = true;
				break;
			}
			m_adoRecrd.GetCollect("customerid",iTemp);
			localID.Format("%d",iTemp);
			m_adoRecrd.GetCollect("comname",comName);
			m_adoRecrd.GetCollect("typename",accType);
			m_adoRecrd.GetCollect("langname",language);
			m_adoRecrd.GetCollect("linkman",linkman);
			m_adoRecrd.GetCollect("linkphone",linkphone);
			m_adoRecrd.GetCollect("linkqq",linkqq);
			m_adoRecrd.GetCollect("linkemail",linkemail);
			m_adoRecrd.GetCollect("account",account);
			m_adoRecrd.GetCollect("password",pwd);
			m_adoRecrd.GetCollect("num",sendnum);
			m_adoRecrd.GetCollect("firsttime",tempTime);
			accdate	= tempTime.Format("%Y-%m-%d");
			m_adoRecrd.GetCollect("serverip",serverip);
			m_adoRecrd.GetCollect("serverport",strPort);
			m_adoRecrd.GetCollect("serveracc",serveraccount);
			m_adoRecrd.GetCollect("serverpwd",serverpwd);
			m_adoRecrd.GetCollect("reverse1",strreverse);
			
			m_adoRecrd.GetCollect("saleman",saleman);
			m_adoRecrd.GetCollect("salemanphone",salemanphone);
			m_adoRecrd.GetCollect("salemanqq",salemanqq);
			m_adoRecrd.GetCollect("salemanemail",salemanEmail);
			sSql.Format("INSERT INTO %s (���,��˾��,��������,ʹ������,��ϵ��,��ϵ�绰,QQ,Email,ҵ��Ա,ҵ��Ա�绰,ҵ��ԱQQ,ҵ��ԱEmail,�˺�,����,������,��������,������IP,�������˿�,�������˺�,����������,��ע)values('%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s')",
				tableName,
				localID,
				comName,
				accType,
				language,
				linkman,
				linkphone,
				linkqq,
				linkemail,
				saleman,
				salemanphone,
				salemanqq,
				salemanEmail,
				account,
				pwd,
				sendnum,
				accdate,
				serverip,
				strPort,
				serveraccount,
				serverpwd,
				strreverse);
			database.ExecuteSQL(sSql);
			Sleep( 5 );
			m_adoRecrd.MoveNext();
		}
		m_adoRecrd.Close();
		if ( bQuit )
		{
			SetDlgItemText(IDC_INFO_SHOW,"�ֶ�ֹͣ,����δ���");
		}
		else
		{
			SetDlgItemText(IDC_INFO_SHOW,"�������");
		}	
	}
	catch(...)
	{
		SetDlgItemText(IDC_INFO_SHOW,"ִ���쳣,����δ���");
		m_adoRecrd.Close();
	}
}



/************************************************************************/
/*                                                                      */
/************************************************************************/
CString CSendLogQueryDlg::GetExcelDriver()
{
	char szBuf[2001];
	WORD cbBufMax = 2000;
	WORD cbBufOut;
	char *pszBuf = szBuf;
	CString sDriver;

	// ��ȡ�Ѱ�װ����������(������odbcinst.h��)
	if (!SQLGetInstalledDrivers(szBuf, cbBufMax, &cbBufOut))
		return "";

	// �����Ѱ�װ�������Ƿ���Excel...
	do
	{
		if (strstr(pszBuf, "Excel") != 0)
		{
			//���� !
			sDriver = CString(pszBuf);
			break;
		}
		pszBuf = strchr(pszBuf, '\0') + 1;
	}
	while (pszBuf[1] != '\0');

	return sDriver;
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
//���Ĭ�ϵ��ļ���
BOOL CSendLogQueryDlg::GetDefaultXlsFileName(CString& sExcelFile)
{
	///Ĭ���ļ�����yyyymmddhhmmss.xls
	CString timeStr = "";
	timeStr = CTime::GetCurrentTime().Format("%Y%m%d%H%M%S");
	timeStr = "�û�����-����ʱ��["+timeStr+"]";

	sExcelFile =  timeStr + ".xls";
	// prompt the user (with all document templates)
	CFileDialog dlgFile(FALSE,".xls",sExcelFile);
	CString title;
	CString strFilter;

	title = "����";
	strFilter = "Excel�ļ�(*.xls)";
	strFilter += (TCHAR)'\0';   // next string please
	strFilter += _T("*.xls");
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
	if (sExcelFile.Right(4) != ".xls")
	{
		sExcelFile+=".xls";
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
BOOL CSendLogQueryDlg::MakeSurePathExists( CString &Path,bool FilenameIncluded)
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
/* ��ӿͻ�                                                             */
/************************************************************************/
void CSendLogQueryDlg::OnBnClickedBtnAdd()
{
	CInfoShow infoShow;
	infoShow.SetParams(m_dbName,m_dbServer,m_dbUser,m_dbPwd,1,0);
	infoShow.DoModal();
}

void CSendLogQueryDlg::OnNMDblclkListShow3(NMHDR *pNMHDR, LRESULT *pResult)
{
	*pResult = 0;
	POSITION pos = m_listtype.GetFirstSelectedItemPosition();
	if (pos == NULL)
	{
		return;
	}
	else
	{
		int nItem = m_listtype.GetNextSelectedItem(pos);
		CString strFlag = m_listtype.GetItemText(nItem,0);
		m_strWhere.Format(" where type = %d",m_typeMap[strFlag]);
	}
	//��ʼ��ѯ
	OperateProc(1);
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
void CSendLogQueryDlg::QueryTypeInfo(CString strWhere,int iFlag,BOOL *bExit,int iCurrPage,BOOL bDetail)
{	
	iCurrPage = 1;
	m_listtype.DeleteAllItems();
	bDetail = TRUE;
	try
	{
		CAdoConnection adoConn;
		CAdoRecordSet adoRst;
		if ( !adoConn.IsOpen() )		//���δ�����Ϸ�����
		{
			if ( !adoConn.ConnectSQLServer(m_dbServer,m_dbName,m_dbUser,m_dbPwd))//.ConnectSQLServer( m_dbServer , m_dbName, m_dbUser,m_dbPwd) )
			{
				SetDlgItemText(IDC_INFO_SHOW,"�������ݿ�ʧ��");
				return ; 
			}
			adoRst.SetAdoConnection(&adoConn);
			adoRst.SetCursorLocation(adUseClient);
		}
		CString strSQL;
		CString strTempWhere;
		CString strTemp;
		int iTemp = 0;
		bool bQuit = false;		//�ֶ��˳�ѭ��


		//�ӽظ����û��ڲ�ͬ�����µĸ���
		if ( bDetail != TRUE)		//����ʾ��ϸ��Ϣ
		{			
			//			GetDaySucCount(m_strStartDate,m_strEndDate,m_adoConn,m_adoRecrd);
		}
		else	//��ʾ��ϸ��Ϣ
		{
			strSQL.Format("select Acctype.*,count(customer.customerid) as totalnum from Acctype left join customer on acctype.typeid=customer.type group by acctype.typeid,typename order by typeid asc");
			if( !adoRst.Open(strSQL,adCmdText))
			{
				adoRst.Close();
				SetDlgItemText(IDC_INFO_SHOW,"ִ�г���,"+adoConn.GetLastErrorText());
				return;
			}
			while( !adoRst.IsEOF())		//�����¼��Ϊ��
			{
				int iCount = m_listtype.GetItemCount();
				m_listtype.InsertItem( iCount, "");			
				adoRst.GetCollect("typeid",iTemp);
				strTemp.Format("%d",iTemp);
				//m_listtype.SetItemText( iCount , 2,strTemp);

				adoRst.GetCollect("typename",strTemp);
				m_listtype.SetItemText( iCount , 0,strTemp);

				adoRst.GetCollect("totalnum",iTemp);
				strTemp.Format("%d",iTemp);
				m_listtype.SetItemText( iCount , 1,strTemp);
				adoRst.MoveNext();
			}
			adoRst.Close();
			adoConn.Close();
		}
	}
	catch(...)
	{
		SetDlgItemText(IDC_INFO_SHOW,"������Ϣ�����쳣");
	}
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CSendLogQueryDlg::OnBnClickedBtnFresh()
{
	BOOL tempBool= FALSE;
	QueTypeAndLanguage();
	QueryTypeInfo("",0,&tempBool,1,TRUE);
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CSendLogQueryDlg::QueTypeAndLanguage()
{
	m_languageMap.clear();
	m_typeMap.clear();
// 	m_comboType.Clear();
// 	m_combo_language.Clear();
	while (m_comboType.GetCount() > 0)
	{
		m_comboType.DeleteString(0);
	}
	while (m_combo_language.GetCount() > 0)
	{
		m_combo_language.DeleteString(0);
	}
	CAdoConnection adoConn;
	CAdoRecordSet adoRst;
	try
	{	if ( !adoConn.IsOpen() )		//���δ�����Ϸ�����
		{
			if ( !adoConn.ConnectSQLServer(m_dbServer,m_dbName,m_dbUser,m_dbPwd))//.ConnectSQLServer( m_dbServer , m_dbName, m_dbUser,m_dbPwd) )
			{
				SetDlgItemText(IDC_INFO_SHOW,"�������ݿ�ʧ��");
				return ; 
			}
			adoRst.SetAdoConnection(&adoConn);
			adoRst.SetCursorLocation(adUseClient);
		}
		CString strSQL;
		CString strTemp = "";
		int iTemp = 0;
		strSQL.Format("select * from Acctype order by typeid asc");
		if( !adoRst.Open(strSQL,adCmdText))
		{
			adoRst.Close();
			SetDlgItemText(IDC_INFO_SHOW,"ִ�г���,"+adoConn.GetLastErrorText());
			adoConn.Close();
			return;
		}
		m_comboType.AddString("��ѡ������");
		m_typeMap["��ѡ������"] = -1;
		while(!adoRst.IsEOF())		//�����¼��Ϊ��
		{
			adoRst.GetCollect("typeid",iTemp);
			adoRst.GetCollect("typename",strTemp);
			m_comboType.AddString(strTemp);
			m_typeMap[strTemp] = iTemp;
			adoRst.MoveNext();
		}
		adoRst.Close();

		strSQL.Format("select * from langType order by langid asc");
		if( !adoRst.Open(strSQL,adCmdText))
		{
			adoRst.Close();
			SetDlgItemText(IDC_INFO_SHOW,"ִ�г���,"+adoConn.GetLastErrorText());
			adoConn.Close();
			return;
		}
		m_combo_language.AddString("��ѡ������");
		m_languageMap["��ѡ������"] = -1;
		while( !adoRst.IsEOF())		//�����¼��Ϊ��
		{
			adoRst.GetCollect("langid",iTemp);
			adoRst.GetCollect("langname",strTemp);
			m_combo_language.AddString(strTemp);
			m_languageMap[strTemp] = iTemp;
			adoRst.MoveNext();
		}
		m_comboType.SetCurSel(0);
		m_combo_language.SetCurSel(0);
	}
	catch(...)
	{
		SetDlgItemText(IDC_INFO_SHOW,"ִ�г���,�����쳣");
	}
	adoRst.Close();
	adoConn.Close();
}

BOOL CSendLogQueryDlg::PreTranslateMessage( MSG* pMsg )
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
void CSendLogQueryDlg::OnNMRclickListShow(NMHDR *pNMHDR, LRESULT *pResult)
{
	POSITION pos = m_resultList.GetFirstSelectedItemPosition();
	if (pos == NULL)
	{
		return;
	}
	else
	{
		int nItem = m_resultList.GetNextSelectedItem(pos);
		CMenu menu; 
		menu.LoadMenu(IDR_MENU1); //�������ȶ����ѡ�� 
		CMenu *pmenu = menu.GetSubMenu(0);
		CPoint pos; 
		GetCursorPos(&pos);
		SetForegroundWindow();
		pmenu->TrackPopupMenu(TPM_LEFTALIGN,pos.x,pos.y,this);
	}
	*pResult = 0;
}

void CSendLogQueryDlg::OnMenuDelete()
{
	POSITION pos = m_resultList.GetFirstSelectedItemPosition();
	int nItem = m_resultList.GetNextSelectedItem(pos);
	CString strID = m_resultList.GetItemText(nItem,0);
	int iRet = MessageBox("���Ҫɾ���ÿͻ���?","��ʾ",MB_ICONQUESTION|MB_YESNO);
	if (iRet != IDYES)
	{
		return;
	}
	CString strSQL = "";
	strSQL.Format("delete from Customer where [customerid]=%s",strID);
	CAdoConnection adoConn;
	CAdoRecordSet adoRst;
	try
	{	
		if ( !adoConn.IsOpen() )		//���δ�����Ϸ�����
		{
			if ( !adoConn.ConnectSQLServer(m_dbServer,m_dbName,m_dbUser,m_dbPwd))//.ConnectSQLServer( m_dbServer , m_dbName, m_dbUser,m_dbPwd) )
			{
				return ; 
			}
			adoRst.SetAdoConnection(&adoConn);
			adoRst.SetCursorLocation(adUseClient);
		}
		if(!adoRst.Open(strSQL,adCmdText))
		{
			adoRst.Close();			
			adoConn.Close();
			MessageBox("ɾ���ͻ���Ϣʧ��"+adoConn.GetLastErrorText(),"������Ϣ");
			return;
		}
		OperateProc(1);
	}
	catch(...)
	{
		MessageBox("ɾ���ͻ���Ϣʧ��","������Ϣ");
	}
	adoRst.Close();			
	adoConn.Close();
}

void CSendLogQueryDlg::OnMenuEdit()
{
	POSITION pos = m_resultList.GetFirstSelectedItemPosition();
	int nItem = m_resultList.GetNextSelectedItem(pos);
	CString strID = m_resultList.GetItemText(nItem,0);
	CInfoShow infoShow;
	infoShow.SetParams(m_dbName,m_dbServer,m_dbUser,m_dbPwd,2,atoi(strID));
	infoShow.DoModal();
}

void CSendLogQueryDlg::OnNMDblclkListShow(NMHDR *pNMHDR, LRESULT *pResult)
{
	*pResult = 0;
	POSITION pos = m_resultList.GetFirstSelectedItemPosition();
	if (pos == NULL)
	{
		return;
	}
	else
	{
		int nItem = m_resultList.GetNextSelectedItem(pos);
		CString strFlag = m_resultList.GetItemText(nItem,0);
		CServiceDetail dlgDetail;
		dlgDetail.m_strID = strFlag;
		dlgDetail.SetParams(m_dbName,m_dbServer,m_dbUser,m_dbPwd,strFlag);
		dlgDetail.DoModal();
	}
}

void CSendLogQueryDlg::OnCbnSelchangeComboAcctype()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}

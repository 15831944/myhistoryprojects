// SendLogQueryDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SendLogQuery.h"
#include "SendLogQueryDlg.h"
#include ".\sendlogquerydlg.h"
#include <io.h>
#include <odbcinst.h>
#include <afxdb.h>
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
	DDX_Control(pDX, IDC_DATE_TIMESTART, m_TimeCtrl);
	DDX_Control(pDX, IDC_DATE_TIMEEND, m_TimeEndCtrl);
	DDX_Control(pDX, IDC_LIST_SHOW_DAY, m_showDay);
}

BEGIN_MESSAGE_MAP(CSendLogQueryDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BTN_QUERY, OnBnClickedBtnQuery)
	ON_BN_CLICKED(IDC_BTN_FIRST, OnBnClickedBtnFirst)
	ON_BN_CLICKED(IDC_BTN_PRE, OnBnClickedBtnPre)
	ON_BN_CLICKED(IDC_BTN_NEXT, OnBnClickedBtnNext)
	ON_BN_CLICKED(IDC_BTN_LAST, OnBnClickedBtnLast)
	ON_BN_CLICKED(IDC_RADIO_SENDDELAY, OnBnClickedRadioSenddelay)
	ON_BN_CLICKED(IDC_RADIO_SENDNOW, OnBnClickedRadioSendnow)
	ON_BN_CLICKED(IDC_RADIO_NOWORDELAY, OnBnClickedRadioNowordelay)
	ON_BN_CLICKED(IDC_RADIO_SUCCESS, OnBnClickedRadioSuccess)
	ON_BN_CLICKED(IDC_RADIO_FAIL, OnBnClickedRadioFail)
	ON_BN_CLICKED(IDC_RADIO_SUCORFIAL, OnBnClickedRadioSucorfial)
	ON_BN_CLICKED(IDC_BTN_STOP, OnBnClickedBtnStop)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BTN_OUTPORT, OnBnClickedBtnOutport)
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

	((CButton*)GetDlgItem(IDC_RADIO_NOWORDELAY))->SetCheck(TRUE);
	((CButton*)GetDlgItem(IDC_RADIO_SUCORFIAL))->SetCheck(TRUE);
	m_TimeCtrl.SetTime(&CTime(2000,1,1,0,0,0,0));
	m_TimeEndCtrl.SetTime(&CTime(2000,1,1,23,59,59,0));
	SetDlgItemInt(IDC_EDIT_NUMONEPAGE,2000);
	GetDlgItem(IDC_PROGRESSBAR)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_BTN_OUTPORT)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_FIRST)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_BTN_PRE)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_BTN_NEXT)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_BTN_LAST)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_INFO_TOTALPAGE)->ShowWindow(SW_HIDE);

	//��ȡ���ݿ�����
	CString strAppPath;
	GetAppPath(strAppPath);
	CString strFilePath = strAppPath + "JinLin.ini";
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
	int iSmsType = ::GetPrivateProfileInt("infotype", "type", 1, strFilePath);
	SetDlgItemInt(IDC_EDIT_SMSTYPE,iSmsType);
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
	if ( !m_IsShowDetail )
	{
		GetDlgItem(IDC_BTN_FIRST)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BTN_PRE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BTN_NEXT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BTN_LAST)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_INFO_TOTALPAGE)->ShowWindow(SW_HIDE);
	}
	else
	{
		GetDlgItem(IDC_BTN_FIRST)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_BTN_PRE)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_BTN_NEXT)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_BTN_LAST)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_INFO_TOTALPAGE)->ShowWindow(SW_SHOW);
	}
	GetDlgItem(IDC_BTN_FIRST)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_PRE)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_NEXT)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_LAST)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_QUERY)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_OUTPORT)->EnableWindow(FALSE);
	if ( iFlag != 10 )
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
			ExportToExcelEx("ÿ�շ��ͳɹ���ͳ��");
		}
	}

	GetDlgItem(IDC_PROGRESSBAR)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_BTN_QUERY)->EnableWindow(TRUE);
	if ( !m_IsShowDetail )
	{
		GetDlgItem(IDC_BTN_OUTPORT)->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_FIRST)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BTN_PRE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BTN_NEXT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BTN_LAST)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_INFO_TOTALPAGE)->ShowWindow(SW_HIDE);
	}
	else
	{
		GetDlgItem(IDC_BTN_OUTPORT)->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_FIRST)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_BTN_PRE)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_BTN_NEXT)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_BTN_LAST)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_INFO_TOTALPAGE)->ShowWindow(SW_SHOW);		
		GetDlgItem(IDC_BTN_FIRST)->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_PRE)->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_NEXT)->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_LAST)->EnableWindow(TRUE);
	}	
	GetDlgItem(IDC_PROGRESSBAR)->ShowWindow(SW_HIDE);
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
void CSendLogQueryDlg::OnBnClickedBtnQuery()
{
	//��֤��ѯ����
	
	GetDlgItemText(IDC_DATE_DATESTART,m_strStartDate);
	GetDlgItemText(IDC_DATE_TIMESTART,m_strStartTime);
	GetDlgItemText(IDC_DATE_DATEEND,m_strEndDate);
	GetDlgItemText(IDC_DATE_TIMEEND,m_strEndTime);
	CTime startTime = GetCTimeFromString(m_strStartDate + " " + m_strStartTime);
	CTime endTime = GetCTimeFromString(m_strEndDate + " " + m_strEndTime);
	CTimeSpan timeSpan;
	timeSpan = endTime - startTime;
	if (timeSpan < 0 )
	{
		SetDlgItemText(IDC_INFO_SHOW,"��ʼʱ�����С�ڽ���ʱ��");
		return;
	}	
	m_iRowOnePage = GetDlgItemInt(IDC_EDIT_NUMONEPAGE);
	if ( m_iRowOnePage < 2000 || m_iRowOnePage > 9999)
	{
		SetDlgItemText(IDC_INFO_SHOW,"ÿҳ��ʾ����������2000-9999֮��");
		return;
	}
	CString strSmsType="";
	GetDlgItemText(IDC_EDIT_SMSTYPE,strSmsType);
	if (strSmsType.Trim() == "")
	{
		SetDlgItemText(IDC_INFO_SHOW,"������Ҫ��ѯ����Ϣ���ͺ�");
		return;
	}
	if ( strSmsType != "0")
	{
		m_strWhere.Format(" where 1=1 and smstype = '%s' and FinishTime > '%s' and FinishTime < '%s' ",strSmsType,startTime.Format("%Y-%m-%d %H:%M:%S"),endTime.Format("%Y-%m-%d %H:%M:%S"));
	}
	else
	{
		m_strWhere.Format(" where 1=1 and FinishTime > '%s' and FinishTime < '%s' ",startTime.Format("%Y-%m-%d %H:%M:%S"),endTime.Format("%Y-%m-%d %H:%M:%S"));
	}
	/************************************************************************/
	CString strTemp = "";
	int iSendType = 0;
	if( ((CButton*)GetDlgItem(IDC_RADIO_SENDNOW))->GetCheck() == 1 )
	{
		iSendType = 1;
	}
	else if( ((CButton*)GetDlgItem(IDC_RADIO_SENDDELAY))->GetCheck() == 1)
	{
		iSendType = 2;
	}
	else
	{
		iSendType = 0;
	}	
	if (iSendType != 0)
	{
		strTemp.Format("and ReadStatus='%d' ",iSendType);
		m_strWhere += strTemp;
	}
	/************************************************************************/
	if( ((CButton*)GetDlgItem(IDC_RADIO_SUCCESS))->GetCheck() == 1 )
	{
		m_iSendFlag = 1;
	}
	else if( ((CButton*)GetDlgItem(IDC_RADIO_FAIL))->GetCheck() == 1)
	{
		m_iSendFlag = 2;
	}
	else
	{
		m_iSendFlag = 0;
	}	
	/************************************************************************/
	//����ʾ��ϸ��Ϣ��־��ֵ
	m_IsShowDetail = ((CButton*)GetDlgItem(IDC_CHECK_SHOWDETAIL))->GetCheck();
	m_iCurrPage = 1;
	/************************************************************************/
	//��ʼ��ѯ
	ThreadParams *lpAddBufParam = new ThreadParams;
	lpAddBufParam->pWnd = this;
	lpAddBufParam->m_iNo = 3;		//1�ǲ�ѯ 
	m_ThreadContrl.pThread = AfxBeginThread((AFX_THREADPROC)ThreadProc, (LPVOID)lpAddBufParam);
	m_ThreadContrl.ThreadExit = FALSE;
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
	m_resultList.InsertColumn(0,_T("ԭ��Ϣ���"),LVCFMT_LEFT,80);
	//m_resultList.InsertColumn(1,_T("ԭ��ϢID"),LVCFMT_LEFT,60);
	m_resultList.InsertColumn(1,_T("Ŀ�����"),LVCFMT_LEFT,100);
	m_resultList.InsertColumn(2,_T("��Ϣ����"),LVCFMT_LEFT,290);
	m_resultList.InsertColumn(3,_T("Ԥ����ʱ��"),LVCFMT_LEFT,130);
	m_resultList.InsertColumn(4,_T("���ʱ��"),LVCFMT_LEFT,130);
	m_resultList.InsertColumn(5,_T("��������"),LVCFMT_LEFT,60);
	m_resultList.InsertColumn(6,_T("��Ϣ����"),LVCFMT_LEFT,60);
	m_resultList.InsertColumn(7,_T("�������"),LVCFMT_LEFT,60);
	m_resultList.InsertColumn(8,_T("����"),LVCFMT_LEFT,40);
	m_resultList.InsertColumn(9,_T("״̬"),LVCFMT_LEFT,50);
	m_resultList.InsertColumn(10,_T("��ע"),LVCFMT_LEFT,400);
	m_resultList.MoveWindow(1,115,991,545);
	dwSytle=::GetWindowLong(m_showDay.m_hWnd,GWL_STYLE);
	//����Ϊ������ʽ
	SetWindowLong(m_showDay.m_hWnd,GWL_STYLE,dwSytle|LVS_REPORT);
	ExStyle=m_showDay.GetExtendedStyle();
	m_showDay.SetExtendedStyle(ExStyle|LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
	::SendMessage(m_showDay.m_hWnd, LVM_SETEXTENDEDLISTVIEWSTYLE,
		LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);
	m_showDay.InsertColumn(0,_T("����"),LVCFMT_LEFT,100);
	m_showDay.InsertColumn(1,_T("���ͳɹ���Ϣ����"),LVCFMT_LEFT,120);
	m_showDay.MoveWindow(1,115,991,545);
	m_showDay.ShowWindow(SW_HIDE);
	GetDlgItem(IDC_BTN_FIRST)->MoveWindow(822,82,35,20);
	GetDlgItem(IDC_BTN_PRE)->MoveWindow(864,82,38,20);
	GetDlgItem(IDC_BTN_NEXT)->MoveWindow(908,82,38,20);
	GetDlgItem(IDC_BTN_LAST)->MoveWindow(950,82,35,20);

	GetDlgItem(IDC_PROGRESSBAR)->MoveWindow(1,110,991,3);
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
void CSendLogQueryDlg::QuerySendLog(CString strWhere,int iFlag,BOOL *bExit,int iCurrPage,BOOL bDetail)
{	
	m_resultList.DeleteAllItems();
	m_showDay.DeleteAllItems();	
	if (bDetail==TRUE)		//���Ҫ��ѯ��ϸ��Ϣ
	{
		m_showDay.ShowWindow(SW_HIDE);
		m_resultList.ShowWindow(SW_SHOW);
	}
	else		//����ѯ��ϸ��Ϣ
	{	
		m_showDay.ShowWindow(SW_SHOW);
		m_resultList.ShowWindow(SW_HIDE);
	}
	try
	{
		if ( !m_adoConn.IsOpen() )		//���δ�����Ϸ�����
		{
			if ( !m_adoConn.ConnectSQLServer( m_dbServer , m_dbName, m_dbUser,m_dbPwd) )
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
		int iTemp;
		bool bQuit = false;		//�ֶ��˳�ѭ��
		switch ( iFlag )
		{
		case 1:		//ֻ��ѯ�ɹ���¼��
			{
				strTempWhere = strWhere + " and SendStatus ='5'";
				strSQL.Format("select (select count(id) from win22_SMSendLog_MW %s) as rows,(select count(id) from win22_SMSendLog_MW %s) as successrows,(select 0) as failrows,(select sum(PhoneCount) from win22_SMSendLog_MW %s) as phoneCount,(select sum(PhoneCount) from win22_SMSendLog_MW %s) as successCount,(select '0' ) as failCount",
					strTempWhere,
					strTempWhere,
					strTempWhere,
					strTempWhere);
				break;
			}
		case 2:		//ֻ��ѯʧ�ܼ�¼��
			{
				strTempWhere = strWhere + " and SendStatus ='4'";
				strSQL.Format("select (select count(id) from win22_SMSendLog_MW %s) as rows,(select count(id) from win22_SMSendLog_MW %s) as failrows,(select 0) as successrows,(select sum(PhoneCount) from win22_SMSendLog_MW %s) as phoneCount,(select sum(PhoneCount) from win22_SMSendLog_MW %s) as failCount,(select '0' ) as successCount",
					strTempWhere,
					strTempWhere,
					strTempWhere,
					strTempWhere);
				break;
			}
		default:	//����
			{
				strTempWhere = strWhere;
				strSQL.Format("select (select count(id) from win22_SMSendLog_MW %s) as rows,(select count(id) from win22_SMSendLog_MW %s and SendStatus='5') as successrows,(select count(id) from win22_SMSendLog_MW %s and SendStatus='4') as failrows,(select sum(PhoneCount) from win22_SMSendLog_MW %s) as phoneCount,(select sum(PhoneCount) from win22_SMSendLog_MW %s and SendStatus='5') as successCount,(select sum(PhoneCount) from win22_SMSendLog_MW %s and SendStatus='4') as failCount",
					strTempWhere,
					strTempWhere,
					strTempWhere,
					strTempWhere,
					strTempWhere,
					strTempWhere);
				break;
			}
		}
		if( !m_adoRecrd.Open(strSQL,adCmdText))
		{
			m_adoRecrd.Close();
			SetDlgItemText(IDC_INFO_SHOW,"ִ�г���,��ѯδ���");
			return;
		}
		m_adoRecrd.GetCollect("rows",m_iTotalRecordCount);
		m_adoRecrd.GetCollect("successrows",m_iTotalSucRecord);
		m_adoRecrd.GetCollect("failrows",m_iTotalFailRecord);

		m_adoRecrd.GetCollect("phoneCount",m_iTotalPhoneCount);
		m_adoRecrd.GetCollect("successCount",m_iTotalSucPhoneCount);
		m_adoRecrd.GetCollect("failCount",m_iTotalFailPhoneCount);
		m_adoRecrd.Close();
		
		strTemp.Format("%d",m_iTotalRecordCount);
		SetDlgItemText(IDC_SHOW_TOTALRECORD,strTemp);
		strTemp.Format("%d",m_iTotalSucRecord);
		SetDlgItemText(IDC_SUCCESSRECORD,strTemp);
		strTemp.Format("%d",m_iTotalFailRecord);
		SetDlgItemText(IDC_FAILRECORD,strTemp);
		strTemp.Format("%d",m_iTotalPhoneCount);
		SetDlgItemText(IDC_TOTALNUM,strTemp);
		strTemp.Format("%d",m_iTotalSucPhoneCount);
		SetDlgItemText(IDC_SUCCESSNUM,strTemp);
		strTemp.Format("%d",m_iTotalFailPhoneCount);
		SetDlgItemText(IDC_FAILNUM,strTemp);
		
		if ( bDetail != TRUE)		//����ʾ��ϸ��Ϣ
		{			
			GetDaySucCount(m_strStartDate,m_strEndDate,m_adoConn,m_adoRecrd);
		}
		else	//��ʾ��ϸ��Ϣ
		{
			m_iTotalPage = m_iTotalRecordCount / m_iRowOnePage + 1;
			if ( m_iTotalRecordCount % m_iRowOnePage == 0 )
			{
				m_iTotalPage--;
			}
			if ( m_iTotalRecordCount == 0 )
			{
				m_iTotalPage = 1;
			}
			strTemp.Format("�� %d ҳ,�� %d ҳ",m_iTotalPage,m_iCurrPage);
			SetDlgItemText(IDC_INFO_TOTALPAGE,strTemp);
			strSQL.Format("select top %d * from win22_SMSendLog_MW %s and id not in (select top %d id from win22_SMSendLog_MW %s order by id asc) order by id asc",
				m_iRowOnePage,strTempWhere,m_iRowOnePage * (iCurrPage-1),strTempWhere);
			if( !m_adoRecrd.Open(strSQL,adCmdText))
			{
				m_adoRecrd.Close();
				SetDlgItemText(IDC_INFO_SHOW,"ִ�г���,��ѯδ���");
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
				m_adoRecrd.GetCollect("localID",strTemp);
				m_resultList.SetItemText( iCount , 0,strTemp);
				//m_resultList.SetItemText( iCount , 1,"--------");

				m_adoRecrd.GetCollect("PhoneNo",strTemp);
				m_resultList.SetItemText( iCount , 1,strTemp);

				m_adoRecrd.GetCollect("content",strTemp);
				m_resultList.SetItemText( iCount , 2,strTemp);

				m_adoRecrd.GetCollect("SendTime",tempTime);
				m_resultList.SetItemText( iCount , 3,tempTime.Format("%Y-%m-%d %H:%M:%S"));

				m_adoRecrd.GetCollect("FinishTime",strTemp);
				m_resultList.SetItemText( iCount , 4,tempTime.Format("%Y-%m-%d %H:%M:%S"));

				m_adoRecrd.GetCollect("ReadStatus",iTemp);
				if ( iTemp == 1)
				{
					strTemp = "��ʱ����";
				}
				else
				{
					strTemp = "��ʱ";
				}
				m_resultList.SetItemText( iCount , 5,strTemp);

				m_adoRecrd.GetCollect("msglen",iTemp);
				strTemp.Format("%d",iTemp);
				m_resultList.SetItemText( iCount , 6,strTemp);

				m_adoRecrd.GetCollect("PhoneCount",iTemp);
				strTemp.Format("%d",iTemp);
				m_resultList.SetItemText( iCount ,7,strTemp);

				m_adoRecrd.GetCollect("smstype",iTemp);
				strTemp.Format("%d",iTemp);
				m_resultList.SetItemText( iCount ,8,strTemp);

				m_adoRecrd.GetCollect("SendStatus",iTemp);			
				if ( iTemp == 5)
				{
					strTemp = "�ɹ�";
					m_resultList.SetItemText( iCount , 9,strTemp);
					m_resultList.SetItemText( iCount , 10,"");
				}
				else
				{
					strTemp = "ʧ��";
					m_resultList.SetItemText( iCount , 9,strTemp);
					m_adoRecrd.GetCollect("FailReason",strTemp);
					m_resultList.SetItemText( iCount , 10,strTemp);
				}

				Sleep( 5 );
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
void CSendLogQueryDlg::OnBnClickedBtnFirst()
{
	if (m_iCurrPage == 1)
	{
		SetDlgItemText(IDC_INFO_SHOW,"�Ѿ�������ǰҳ");
		return;
	}
	m_iCurrPage = 1;
	/************************************************************************/
	//��ʼ��ѯ
	ThreadParams *lpAddBufParam = new ThreadParams;
	lpAddBufParam->pWnd = this;
	lpAddBufParam->m_iNo = 3;		//1�ǲ�ѯ 
	m_ThreadContrl.pThread = AfxBeginThread((AFX_THREADPROC)ThreadProc, (LPVOID)lpAddBufParam);
	m_ThreadContrl.ThreadExit = FALSE;
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
void CSendLogQueryDlg::OnBnClickedBtnPre()
{
	if (m_iCurrPage == 1)
	{
		SetDlgItemText(IDC_INFO_SHOW,"�Ѿ�������ǰҳ");
		return;
	}
	m_iCurrPage--;
	/************************************************************************/
	//��ʼ��ѯ
	ThreadParams *lpAddBufParam = new ThreadParams;
	lpAddBufParam->pWnd = this;
	lpAddBufParam->m_iNo = 3;		//1�ǲ�ѯ 
	m_ThreadContrl.pThread = AfxBeginThread((AFX_THREADPROC)ThreadProc, (LPVOID)lpAddBufParam);
	m_ThreadContrl.ThreadExit = FALSE;
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
void CSendLogQueryDlg::OnBnClickedBtnNext()
{
	if (m_iCurrPage == m_iTotalPage)
	{
		SetDlgItemText(IDC_INFO_SHOW,"�Ѿ�������ĩҳ");
		return;
	}
	m_iCurrPage++;
	/************************************************************************/
	//��ʼ��ѯ
	ThreadParams *lpAddBufParam = new ThreadParams;
	lpAddBufParam->pWnd = this;
	lpAddBufParam->m_iNo = 3;		//1�ǲ�ѯ 
	m_ThreadContrl.pThread = AfxBeginThread((AFX_THREADPROC)ThreadProc, (LPVOID)lpAddBufParam);
	m_ThreadContrl.ThreadExit = FALSE;
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
void CSendLogQueryDlg::OnBnClickedBtnLast()
{
	if (m_iCurrPage == m_iTotalPage)
	{
		SetDlgItemText(IDC_INFO_SHOW,"�Ѿ�������ĩҳ");
		return;
	}
	m_iCurrPage = m_iTotalPage;
	/************************************************************************/
	//��ʼ��ѯ
	ThreadParams *lpAddBufParam = new ThreadParams;
	lpAddBufParam->pWnd = this;
	lpAddBufParam->m_iNo = 3;		//1�ǲ�ѯ 
	m_ThreadContrl.pThread = AfxBeginThread((AFX_THREADPROC)ThreadProc, (LPVOID)lpAddBufParam);
	m_ThreadContrl.ThreadExit = FALSE;
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
void CSendLogQueryDlg::OnBnClickedRadioSenddelay()
{
	((CButton*)GetDlgItem(IDC_RADIO_SENDNOW))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_RADIO_SENDDELAY))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_RADIO_NOWORDELAY))->SetCheck(FALSE);

	((CButton*)GetDlgItem(IDC_RADIO_SENDDELAY))->SetCheck(TRUE);
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CSendLogQueryDlg::OnBnClickedRadioSendnow()
{
	((CButton*)GetDlgItem(IDC_RADIO_SENDNOW))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_RADIO_SENDDELAY))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_RADIO_NOWORDELAY))->SetCheck(FALSE);

	((CButton*)GetDlgItem(IDC_RADIO_SENDNOW))->SetCheck(TRUE);
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CSendLogQueryDlg::OnBnClickedRadioNowordelay()
{
	((CButton*)GetDlgItem(IDC_RADIO_SENDNOW))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_RADIO_SENDDELAY))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_RADIO_NOWORDELAY))->SetCheck(FALSE);

	((CButton*)GetDlgItem(IDC_RADIO_NOWORDELAY))->SetCheck(TRUE);
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CSendLogQueryDlg::OnBnClickedRadioSuccess()
{
	((CButton*)GetDlgItem(IDC_RADIO_SUCCESS))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_RADIO_FAIL))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_RADIO_SUCORFIAL))->SetCheck(FALSE);

	((CButton*)GetDlgItem(IDC_RADIO_SUCCESS))->SetCheck(TRUE);
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CSendLogQueryDlg::OnBnClickedRadioFail()
{
	((CButton*)GetDlgItem(IDC_RADIO_SUCCESS))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_RADIO_FAIL))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_RADIO_SUCORFIAL))->SetCheck(FALSE);

	((CButton*)GetDlgItem(IDC_RADIO_FAIL))->SetCheck(TRUE);
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CSendLogQueryDlg::OnBnClickedRadioSucorfial()
{
	((CButton*)GetDlgItem(IDC_RADIO_SUCCESS))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_RADIO_FAIL))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_RADIO_SUCORFIAL))->SetCheck(FALSE);

	((CButton*)GetDlgItem(IDC_RADIO_SUCORFIAL))->SetCheck(TRUE);
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
			if ( !m_adoConn.ConnectSQLServer( m_dbServer , m_dbName, m_dbUser,m_dbPwd) )
			{
				SetDlgItemText(IDC_INFO_SHOW,"�������ݿ�ʧ��");
				return ; 
			}
			m_adoRecrd.SetAdoConnection(&m_adoConn);
			m_adoRecrd.SetCursorLocation(adUseClient);
		}
		CString strSQL;
		CString strTempWhere;
		switch ( iFlag )
		{
		case 1:		//ֻ��ѯ�ɹ���¼��
			{
				strTempWhere = strWhere + " and SendStatus =\"5\"";
				break;
			}
		case 2:
			{
				strTempWhere = strWhere + " and SendStatus =\"4\"";
				break;
			}
		default:
			{
				strTempWhere = strWhere;
				break;
			}

		}
		strSQL.Format("select localID,PhoneNo,content,SendTime,FinishTime,ReadStatus,SendStatus,msglen,PhoneCount,FailReason,smstype from win22_SMSendLog_MW %s",strTempWhere);

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
		CString PhoneNo;
		CString content;
		CString SendTime;
		CString FinishTime;
		CString ReadStatus;
		CString SendStatus;
		CString msglen;
		CString PhoneCount;
		CString smstype;
		CString FailReason;
		int iTemp;
		// �������ݿ� (��Excel����ļ�)
		if( database.OpenEx(sSql,CDatabase::noOdbcDialog) )
		{
			CString strTemp;
			strTemp.Format("ԭ��Ϣ��� INT,Ŀ����� TEXT,��Ϣ���� TEXT,Ԥ����ʱ�� TEXT,���ʱ�� TEXT,�������� TEXT,״̬ TEXT,��Ϣ���� INT,������� INT,���� INT,��ע TEXT");
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
			m_adoRecrd.GetCollect("localID",localID);
			m_adoRecrd.GetCollect("PhoneNo",PhoneNo);
			m_adoRecrd.GetCollect("content",content);
			m_adoRecrd.GetCollect("SendTime",tempTime);
			SendTime = tempTime.Format("%Y-%m-%d %H:%M:%S");
			m_adoRecrd.GetCollect("FinishTime",tempTime);
			FinishTime = tempTime.Format("%Y-%m-%d %H:%M:%S");
			m_adoRecrd.GetCollect("ReadStatus",iTemp);
			if ( iTemp == 1)
			{
				ReadStatus = "��ʱ����";
			}
			else
			{
				ReadStatus = "��ʱ";
			}
			m_adoRecrd.GetCollect("msglen",iTemp);
			msglen.Format("%d",iTemp);
			m_adoRecrd.GetCollect("PhoneCount",iTemp);
			PhoneCount.Format("%d",iTemp);
			m_adoRecrd.GetCollect("SendStatus",iTemp);			
			if ( iTemp == 5)
			{
				SendStatus = "�ɹ�";
			}
			else
			{
				SendStatus = "ʧ��";
			}
			m_adoRecrd.GetCollect("smstype",iTemp);
			smstype.Format("%d",iTemp);
			m_adoRecrd.GetCollect("FailReason",FailReason);
			sSql.Format("INSERT INTO %s (ԭ��Ϣ���,Ŀ�����,��Ϣ����,Ԥ����ʱ��,���ʱ��,��������,״̬,��Ϣ����,�������,����,��ע)values('%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s')",
				tableName,
				localID,
				PhoneNo,
				content,
				SendTime,
				FinishTime,
				ReadStatus,
				SendStatus,
				msglen,
				PhoneCount,
				smstype,
				FailReason);
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
/* ����ÿ�շ��ͳɹ�����־                                                 */
/************************************************************************/
void CSendLogQueryDlg::ExportToExcelEx(CString strTitle)
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
		///Ĭ���ļ���
		if (!GetDefaultXlsFileName(sExcelFile))
		{
			return;
		}
		SetDlgItemText(IDC_INFO_SHOW,"���ڽ��е���,���Ժ�...");
		GetDlgItem(IDC_PROGRESSBAR)->ShowWindow(SW_SHOW);
		// �������д�ȡ���ַ���
		sSql.Format("DRIVER={%s};DSN='';FIRSTROWHASNAMES=1;READONLY=FALSE;CREATE_DB=\"%s\";DBQ=%s",sDriver, sExcelFile, sExcelFile);
		CString strDay = "";
		CString strSucCount = "";
		
		// �������ݿ� (��Excel����ļ�)
		if( database.OpenEx(sSql,CDatabase::noOdbcDialog) )
		{
			CString strTemp;
			strTemp.Format("���� TEXT,�ɹ��� INT");
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
		for (int i=0;i<m_showDay.GetItemCount();i++)
		{
			strDay = m_showDay.GetItemText(i,0);
			strSucCount = m_showDay.GetItemText(i,1);
			sSql.Format("INSERT INTO %s (����,�ɹ���)values('%s','%s')",
				tableName,
				strDay,
				strSucCount
				);
			database.ExecuteSQL(sSql);
		}		
		SetDlgItemText(IDC_INFO_SHOW,"�������");
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
	if (m_IsShowDetail)
	{
		timeStr.Format("������־%s %s--%s %s",m_strStartDate,m_strStartTime,m_strEndDate,m_strEndTime);
		timeStr.Replace(':','_');
	}
	else
	{
		timeStr.Format("ÿ�շ��ͳɹ���ͳ��%s--%s",m_strStartDate,m_strEndDate);
	}
	
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
/*                                                                      */
/************************************************************************/
void CSendLogQueryDlg::GetDaySucCount(CString strStartDay,CString strEndDay,CAdoConnection &adoConn,CAdoRecordSet &adoRst)
{
	int iYear=0;
	int iMonth=0;
	int iDay=0;
	int iCount = 0;
	CString strSucCount = "";
	CTimeSpan tempSpan;
	int iTotalCount = 0;
	tempSpan = CTime(2009,01,2,0,0,0)-CTime(2009,01,1,0,0,0);
	try
	{
		sscanf(strStartDay,"%d-%d-%d",&iYear,&iMonth,&iDay);
		CTime startTime = CTime(iYear,iMonth,iDay,0,0,0);
		sscanf(strEndDay,"%d-%d-%d",&iYear,&iMonth,&iDay);
		CTime endTime = CTime(iYear,iMonth,iDay,0,0,0);
		CTime tempTime = CTime(startTime);
		CString strSQL = "";
		int iQueryType = GetDlgItemInt(IDC_EDIT_SMSTYPE);
		while(true)
		{
			if (tempTime<startTime || tempTime > endTime)
			{
				break;
			}
			if (iQueryType !=0)
			{
				strSQL.Format("select sum(PhoneCount) as successCount from win22_SMSendLog_MW  where FinishTime >= '%s 00:00:00' and FinishTime <= '%s 23:59:59' and SendStatus='5' and smstype='%d'",
					tempTime.Format("%Y-%m-%d"),tempTime.Format("%Y-%m-%d"),iQueryType);
			}
			else
			{
				strSQL.Format("select sum(PhoneCount) as successCount from win22_SMSendLog_MW  where FinishTime >= '%s 00:00:00' and FinishTime <= '%s 23:59:59' and SendStatus='5'",
					tempTime.Format("%Y-%m-%d"),tempTime.Format("%Y-%m-%d"));
			}
			if (!adoRst.Open(strSQL))
			{
				SetDlgItemText(IDC_INFO_SHOW,"��ѯÿ�ճɹ�����ʧ��");
				break;
			}
			if (adoRst.IsEOF())
			{
				tempTime += tempSpan;
				adoRst.Close();
				continue;
			}
			else
			{
				adoRst.GetCollect("successCount",iCount);
				if (iCount == 0)	//0����ʾ
				{
					tempTime += tempSpan;
					adoRst.Close();
					continue;
				}
				iTotalCount += iCount;
				strSucCount.Format("%d",iCount);
				iCount = m_showDay.GetItemCount();
				m_showDay.InsertItem( iCount, "");
				m_showDay.SetItemText(iCount,0,tempTime.Format("%Y-%m-%d"));
				m_showDay.SetItemText(iCount,1,strSucCount);
				tempTime += tempSpan;
				adoRst.Close();
			}
		}
		iCount = m_showDay.GetItemCount();
		m_showDay.InsertItem( iCount, "");
		m_showDay.SetItemText(iCount,0,"����");
		strSucCount.Format("%d",iTotalCount);
		m_showDay.SetItemText(iCount,1,strSucCount);
	}	
	catch (...)
	{
		SetDlgItemText(IDC_INFO_SHOW,"��ѯÿ�ճɹ������쳣");
	}	
}
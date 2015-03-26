// DetailEdit.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SendLogQuery.h"
#include "DetailEdit.h"

#include "ServiceDetail.h"
// CDetailEdit �Ի���

IMPLEMENT_DYNAMIC(CDetailEdit, CDialog)

CDetailEdit::CDetailEdit(CWnd* pParent /*=NULL*/)
	: CDialog(CDetailEdit::IDD, pParent)
{

}

CDetailEdit::~CDetailEdit()
{
}

void CDetailEdit::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDetailEdit, CDialog)
	ON_BN_CLICKED(IDC_BTN_ADD, &CDetailEdit::OnBnClickedBtnAdd)
	ON_BN_CLICKED(IDC_BTN_SAVE, &CDetailEdit::OnBnClickedBtnSave)
	ON_BN_CLICKED(IDC_BTN_EXIT, &CDetailEdit::OnBnClickedBtnExit)
END_MESSAGE_MAP()


// CDetailEdit ��Ϣ�������

void CDetailEdit::OnBnClickedBtnAdd()
{
	CString strTime;
	CString strContent;
	
	GetDlgItemText(IDC_EDIT_TIME,strTime);
	GetDlgItemText(IDC_EDIT_DETAIL,strContent);

	CString strSQL = "";
	strSQL.Format("insert into servicedetail (customerid,begintime,content) values(%s,'%s','%s')",
				m_strID,
				strTime,
				strContent);
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
			MessageBox("��Ӽ�¼��Ϣʧ��"+adoConn.GetLastErrorText(),"������Ϣ");
			return;
		}
		//MessageBox("��Ӽ�¼��Ϣ�ɹ�","��ʾ");
	}
	catch(...)
	{
		MessageBox("��Ӽ�¼��Ϣʧ��","������Ϣ");
		adoRst.Close();			
		adoConn.Close();
		return;
	}
	adoRst.Close();			
	adoConn.Close();
	((CServiceDetail*)GetParent())->InitData();
	PostMessage(WM_CLOSE,0,0);
}

void CDetailEdit::OnBnClickedBtnSave()
{
	CString strTime;
	CString strContent;

	GetDlgItemText(IDC_EDIT_TIME,strTime);
	GetDlgItemText(IDC_EDIT_DETAIL,strContent);

	CString strSQL = "";
	strSQL.Format("update servicedetail set begintime='%s',content='%s' where [id]=%s",		
		strTime,
		strContent,
		m_strID);
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
			MessageBox("�޸ļ�¼��Ϣʧ��"+adoConn.GetLastErrorText(),"������Ϣ");
			return;
		}
		//MessageBox("��Ӽ�¼��Ϣ�ɹ�","��ʾ");
	}
	catch(...)
	{
		MessageBox("�޸ļ�¼��Ϣʧ��","������Ϣ");
		adoRst.Close();			
		adoConn.Close();
		return;
	}
	adoRst.Close();			
	adoConn.Close();
	((CServiceDetail*)GetParent())->InitData();
	PostMessage(WM_CLOSE,0,0);
}

void CDetailEdit::OnBnClickedBtnExit()
{
	PostMessage(WM_CLOSE,0,0);
}

void CDetailEdit::SetParams(CString strdbName,CString strdbServer,CString strdbUser,CString strPwd,CString strID,int iFlag)
{
	m_dbName = strdbName;
	m_dbServer = strdbServer;
	m_dbUser = strdbUser;
	m_dbPwd = strPwd;
	m_strID = strID;
	m_iFlag = iFlag;
}

BOOL CDetailEdit::OnInitDialog()
{
	CDialog::OnInitDialog();
	HICON m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��
	if (m_iFlag == 0)	//0���
	{
		GetDlgItem(IDC_EDIT_TIME)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_SAVE)->ShowWindow(SW_HIDE);
		SetDlgItemText(IDC_EDIT_TIME,CTime::GetCurrentTime().Format("%Y-%m-%d %H:%M:%S"));
	}
	else
	{
		GetDlgItem(IDC_BTN_ADD)->ShowWindow(SW_HIDE);
		InitData();
	}
	SetWindowText("��������༭");	
	return TRUE;  // ���������˿ؼ��Ľ��㣬���򷵻� TRUE
}

void CDetailEdit::InitData()
{
	CString strSQL = "";
	strSQL.Format("select * from servicedetail where [id]=%s", m_strID);
	CAdoConnection adoConn;
	CAdoRecordSet adoRst;
	try
	{

		if ( !adoConn.IsOpen() )		//���δ�����Ϸ�����
		{
			if ( !adoConn.ConnectSQLServer(m_dbServer,m_dbName,m_dbUser,m_dbPwd))//.ConnectSQLServer( m_dbServer , m_dbName, m_dbUser,m_dbPwd) )
			{
				//SetDlgItemText(IDC_INFO_SHOW,"�������ݿ�ʧ��");
				return ; 
			}
			adoRst.SetAdoConnection(&adoConn);
			adoRst.SetCursorLocation(adUseClient);
		}
		if( !adoRst.Open(strSQL,adCmdText))
		{
			adoRst.Close();			
			return;
		}

		COleDateTime tempTime;
		CString strTemp;
		if( !adoRst.IsEOF())		//�����¼��Ϊ��
		{
			adoRst.GetCollect("begintime",tempTime);
			SetDlgItemText(IDC_EDIT_TIME,tempTime.Format("%Y-%m-%d %H:%M:%S"));

			adoRst.GetCollect("content",strTemp);
			SetDlgItemText(IDC_EDIT_DETAIL,strTemp);
		}
	}
	catch (...)
	{
	}	
	adoRst.Close();
	adoConn.Close();
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
BOOL CDetailEdit::PreTranslateMessage( MSG* pMsg )
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
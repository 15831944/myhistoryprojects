// ServiceDetail.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SendLogQuery.h"
#include "ServiceDetail.h"
#include "DetailEdit.h"

// CServiceDetail �Ի���

IMPLEMENT_DYNAMIC(CServiceDetail, CDialog)

CServiceDetail::CServiceDetail(CWnd* pParent /*=NULL*/)
	: CDialog(CServiceDetail::IDD, pParent)
{

}

CServiceDetail::~CServiceDetail()
{
}

void CServiceDetail::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_SHOW, m_resultList);
}


BEGIN_MESSAGE_MAP(CServiceDetail, CDialog)
	ON_COMMAND(IDR_DETAIL_ADD, &CServiceDetail::OnDetailAdd)
	ON_COMMAND(IDR_DETAIL_EDIT, &CServiceDetail::OnDetailEdit)
	ON_COMMAND(IDR_DETAIL_DELETE, &CServiceDetail::OnDetailDelete)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_SHOW, &CServiceDetail::OnNMRclickListShow)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_SHOW, &CServiceDetail::OnNMDblclkListShow)
END_MESSAGE_MAP()


// CServiceDetail ��Ϣ�������
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CServiceDetail::InitCtrList()
{
	DWORD dwSytle=::GetWindowLong(m_resultList.m_hWnd,GWL_STYLE);
	//����Ϊ������ʽ
	SetWindowLong(m_resultList.m_hWnd,GWL_STYLE,dwSytle|LVS_REPORT);
	DWORD ExStyle=m_resultList.GetExtendedStyle();
	m_resultList.SetExtendedStyle(ExStyle|LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
	::SendMessage(m_resultList.m_hWnd, LVM_SETEXTENDEDLISTVIEWSTYLE,
		LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);
	m_resultList.InsertColumn(1,_T("���"),LVCFMT_LEFT,50);
	m_resultList.InsertColumn(1,_T("ʱ��"),LVCFMT_LEFT,120);
	m_resultList.InsertColumn(2,_T("����ϸ��"),LVCFMT_LEFT,750);
	m_resultList.MoveWindow(10,10,730,525);
}
BOOL CServiceDetail::OnInitDialog()
{
	CDialog::OnInitDialog();
	HICON m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��
	SetWindowText("����������ʾ");
	InitCtrList();
	InitData();
	return TRUE;  // ���������˿ؼ��Ľ��㣬���򷵻� TRUE
}

void CServiceDetail::InitData()
{
	m_resultList.DeleteAllItems();
	CString strSQL = "";
	strSQL.Format("select * from servicedetail where customerid=%s order by begintime desc", m_strID);
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
		int iTemp;
		while( !adoRst.IsEOF())		//�����¼��Ϊ��
		{
			int iCount = m_resultList.GetItemCount();
			m_resultList.InsertItem( iCount, "");	
			adoRst.GetCollect("id",iTemp);
			strTemp.Format("%d",iTemp);
			m_resultList.SetItemText( iCount , 0,strTemp);

			adoRst.GetCollect("begintime",tempTime);
			m_resultList.SetItemText( iCount , 1,tempTime.Format("%Y-%m-%d %H:%M:%S"));

			adoRst.GetCollect("content",strTemp);
			m_resultList.SetItemText( iCount , 2,strTemp);
			adoRst.MoveNext();
		}
	}
	catch (...)
	{
	}	
	adoRst.Close();
	adoConn.Close();
}

void CServiceDetail::SetParams(CString strdbName,CString strdbServer,CString strdbUser,CString strPwd,CString strID)
{
	m_dbName = strdbName;
	m_dbServer = strdbServer;
	m_dbUser = strdbUser;
	m_dbPwd = strPwd;
	m_strID = strID;
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
BOOL CServiceDetail::PreTranslateMessage(MSG* pMsg)
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
void CServiceDetail::OnDetailAdd()
{
	CDetailEdit infoEdit;
	infoEdit.SetParams(m_dbName,m_dbServer,m_dbUser,m_dbPwd,m_strID,0);
	infoEdit.DoModal();	
}

void CServiceDetail::OnDetailEdit()
{
	POSITION pos = m_resultList.GetFirstSelectedItemPosition();
	if (pos == NULL)
	{
		return;
	}
	int nItem = m_resultList.GetNextSelectedItem(pos);
	CString strID = m_resultList.GetItemText(nItem,0);
	CDetailEdit infoEdit;
	infoEdit.SetParams(m_dbName,m_dbServer,m_dbUser,m_dbPwd,strID,1);
	infoEdit.DoModal();
}

void CServiceDetail::OnDetailDelete()
{	
	POSITION pos = m_resultList.GetFirstSelectedItemPosition();
	int nItem = m_resultList.GetNextSelectedItem(pos);
	CString strID = m_resultList.GetItemText(nItem,0);
	int iRet = MessageBox("���Ҫɾ���ü�¼��?","��ʾ",MB_ICONQUESTION|MB_YESNO);
	if (iRet != IDYES)
	{
		return;
	}
	CString strSQL = "";
	strSQL.Format("delete from servicedetail where [id]=%s",strID);
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
			MessageBox("ɾ����¼��Ϣʧ��"+adoConn.GetLastErrorText(),"������Ϣ");
			return;
		}
		InitData();
	}
	catch(...)
	{
		MessageBox("ɾ����¼��Ϣʧ��","������Ϣ");
	}
	adoRst.Close();			
	adoConn.Close();
}

void CServiceDetail::OnNMRclickListShow(NMHDR *pNMHDR, LRESULT *pResult)
{
	POSITION pos = m_resultList.GetFirstSelectedItemPosition();
	int iFlag = 0;
	if(pos == NULL)
	{
		iFlag = 0;		
	}
	else
	{
		iFlag = 1;
	}
	int nItem = m_resultList.GetNextSelectedItem(pos);
	CMenu menu; 
	menu.LoadMenu(IDR_MENU2); //�������ȶ����ѡ�� 
	CMenu *pmenu = menu.GetSubMenu(0);
	if (iFlag == 0)
	{
		pmenu->EnableMenuItem(IDR_DETAIL_EDIT,MF_GRAYED);
		pmenu->EnableMenuItem(IDR_DETAIL_DELETE,MF_GRAYED);
	}
	else
	{
		pmenu->EnableMenuItem(IDR_DETAIL_EDIT,MF_ENABLED);
		pmenu->EnableMenuItem(IDR_DETAIL_DELETE,MF_ENABLED);
	}
	CPoint pt; 
	GetCursorPos(&pt);
	SetForegroundWindow();
	pmenu->TrackPopupMenu(TPM_LEFTALIGN,pt.x,pt.y,this);
	*pResult = 0;
}

void CServiceDetail::OnNMDblclkListShow(NMHDR *pNMHDR, LRESULT *pResult)
{
	*pResult = 0;
	POSITION pos = m_resultList.GetFirstSelectedItemPosition();
	if (pos == NULL)
	{
		return;
	}
	int nItem = m_resultList.GetNextSelectedItem(pos);
	CString strID = m_resultList.GetItemText(nItem,0);
	CDetailEdit infoEdit;
	infoEdit.SetParams(m_dbName,m_dbServer,m_dbUser,m_dbPwd,strID,1);
	infoEdit.DoModal();	
}

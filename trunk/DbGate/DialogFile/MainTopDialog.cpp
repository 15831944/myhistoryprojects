// MainTopDialog.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "..\DbGate.h"
#include "MainTopDialog.h"
#include ".\maintopdialog.h"
#include "../ControlFile/dataType.h"
// CMainTopDialog

IMPLEMENT_DYNCREATE(CMainTopDialog, CFormView)
CListCtrl g_RecvList;
CListCtrl g_SentList;
CListCtrl g_ReportList;
CMainTopDialog::CMainTopDialog()
	: CFormView(CMainTopDialog::IDD)
{
}

CMainTopDialog::~CMainTopDialog()
{	
}

void CMainTopDialog::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB_SHOW, m_TabShow);
	DDX_Control(pDX, IDC_LIST_RECV, g_RecvList);
	DDX_Control(pDX, IDC_LIST_SENT, g_SentList);
	DDX_Control(pDX, IDC_LIST_REPORT, g_ReportList);
}

BEGIN_MESSAGE_MAP(CMainTopDialog, CFormView)
	ON_WM_SIZE()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_SHOW, OnTcnSelchangeTabShow)
END_MESSAGE_MAP()


// CMainTopDialog ���

#ifdef _DEBUG
void CMainTopDialog::AssertValid() const
{
	CFormView::AssertValid();
}

void CMainTopDialog::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

void CMainTopDialog::OnSize(UINT nType, int cx, int cy)
{
	CFormView::OnSize(nType, cx, cy);

	CWnd *pWnd = NULL;
	CRect rectCtrl;

	int iLeft ;
	int iTop ;
	int iWidth ;
	int iHeight;

	CRect rectClient;
	GetClientRect(rectClient);
	iLeft = rectClient.left;
	iTop = rectClient.top;
	iWidth = rectClient.Width();
	iHeight = rectClient.Height();
	if ( iHeight < 50 ) 
	{
		iHeight = 50 ;
	}
	if ( iWidth < 600 ) 
	{
		iWidth = 600 ; 
	}

	//�ƶ�TAB������CTRL
	pWnd = GetDlgItem(IDC_TAB_SHOW);
	if ( pWnd )
	{
		pWnd->MoveWindow(iLeft,iTop,iWidth ,20);
	}

	pWnd = GetDlgItem(IDC_LIST_REPORT);
	if ( pWnd )
	{
		pWnd->MoveWindow(iLeft,iTop+20,iWidth ,iHeight-20);
	}

	pWnd = GetDlgItem(IDC_LIST_RECV);
	if ( pWnd )
	{
		pWnd->MoveWindow(iLeft,iTop+20,iWidth ,iHeight-20);
	}

	pWnd = GetDlgItem(IDC_LIST_SENT);
	if ( pWnd )
	{
		pWnd->MoveWindow(iLeft,iTop+20,iWidth ,iHeight-20);
	}
}


/************************************************************************/
/*                                                                      */
/************************************************************************/
void CMainTopDialog::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	InitReportCtrList();
	InitSentCtrList();
	InitRecvCtrList();	
	InitTabCtrl();
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CMainTopDialog::InitTabCtrl()
{
 	m_TabShow.InsertItem(0,"���Ͷ���");
 	m_TabShow.InsertItem(1,"���ն���");
 	m_TabShow.InsertItem(2,"״̬����");
 	m_TabShow.m_crUnselColour = RGB( 21, 200, 32 );//����ɫ
 	ShowLists( 0 );
}


/************************************************************************/
/*                                                                      */
/************************************************************************/
void CMainTopDialog::ShowLists(int nSel)
{
	g_ReportList.ShowWindow(SW_HIDE);
	g_RecvList.ShowWindow(SW_HIDE);
	g_SentList.ShowWindow(SW_HIDE);
	switch(nSel)
	{
	case 0:
		{
			g_SentList.ShowWindow(SW_SHOW);			
			break;
		}
	case 1:
		{
			g_RecvList.ShowWindow(SW_SHOW);
			break;
		}
	case 2:
		{
			g_ReportList.ShowWindow(SW_SHOW);
			break;
		}
	}	
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
void CMainTopDialog::InitRecvCtrList()
{
	g_RecvList.ShowWindow(SW_HIDE);
	//m_RecList.ShowWindow(SW_HIDE);
	DWORD dwSytle=::GetWindowLong(g_RecvList.m_hWnd,GWL_STYLE);
	//����Ϊ������ʽ
	SetWindowLong(g_RecvList.m_hWnd,GWL_STYLE,dwSytle|LVS_REPORT);
	DWORD ExStyle=g_RecvList.GetExtendedStyle();
	g_RecvList.SetExtendedStyle(ExStyle|LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);

	::SendMessage(g_RecvList.m_hWnd, LVM_SETEXTENDEDLISTVIEWSTYLE,
		LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);

	g_RecvList.InsertColumn(0,_T("����ʱ��"),LVCFMT_LEFT,125);
	g_RecvList.InsertColumn(1,_T("�����ֻ�"),LVCFMT_LEFT,120);
	g_RecvList.InsertColumn(2,_T("��Ϣ����"),LVCFMT_LEFT,350);
	g_RecvList.InsertColumn(3,_T("����ͨ��"),LVCFMT_LEFT,200);
}


/************************************************************************/
/*                                                                      */
/************************************************************************/
void CMainTopDialog::InitSentCtrList()
{
	g_SentList.ShowWindow(SW_HIDE);
	//m_RecList.ShowWindow(SW_HIDE);
	DWORD dwSytle=::GetWindowLong(g_SentList.m_hWnd,GWL_STYLE);
	//����Ϊ������ʽ
	SetWindowLong(g_SentList.m_hWnd,GWL_STYLE,dwSytle|LVS_REPORT);
	DWORD ExStyle=g_SentList.GetExtendedStyle();
	g_SentList.SetExtendedStyle(ExStyle|LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);

	::SendMessage(g_SentList.m_hWnd, LVM_SETEXTENDEDLISTVIEWSTYLE,
		LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);

	g_SentList.InsertColumn(0,_T("����ʱ��"),LVCFMT_LEFT,125);
	g_SentList.InsertColumn(1,_T("��Ϣ��ʼ���"),LVCFMT_LEFT,120);
	g_SentList.InsertColumn(2,_T("�����ֻ�"),LVCFMT_LEFT,150);
	g_SentList.InsertColumn(3,_T("��Ϣ����"),LVCFMT_LEFT,240);
	g_SentList.InsertColumn(4,_T("��Ϣ����"),LVCFMT_LEFT,65);
	g_SentList.InsertColumn(5,_T("�������"),LVCFMT_LEFT,65);
	g_SentList.InsertColumn(6,_T("�Ŷ�"),LVCFMT_LEFT,85);
	g_SentList.InsertColumn(7,_T("����״̬"),LVCFMT_LEFT,165);
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
void CMainTopDialog::InitReportCtrList()
{
	g_ReportList.ShowWindow(SW_HIDE);
	DWORD dwSytle=::GetWindowLong(g_ReportList.m_hWnd,GWL_STYLE);
	// ����Ϊ������ʽ
	SetWindowLong(g_ReportList.m_hWnd,GWL_STYLE,dwSytle|LVS_REPORT);
	DWORD ExStyle=g_ReportList.GetExtendedStyle();
	g_ReportList.SetExtendedStyle(ExStyle|LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
	::SendMessage(g_ReportList.m_hWnd, LVM_SETEXTENDEDLISTVIEWSTYLE,
		LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);
	g_ReportList.InsertColumn(0,_T("����ʱ��"),LVCFMT_LEFT,125);
	g_ReportList.InsertColumn(1,_T("��Ϣ���"),LVCFMT_LEFT,155);
	g_ReportList.InsertColumn(2,_T("״̬"),LVCFMT_LEFT,150);
}
void CMainTopDialog::OnTcnSelchangeTabShow(NMHDR *pNMHDR, LRESULT *pResult)
{
	*pResult = 0;
	int iCurSel = m_TabShow.GetCurSel();
	ShowLists(iCurSel);
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
void AddToRecvListShow(CString strTime,CString orgAddr,CString strContent,CString strDestAddr )
{
	int iCount = g_RecvList.GetItemCount();
	if ( iCount >= 400 )		//ֻ����400����¼
	{
		g_RecvList.DeleteAllItems();
		iCount = 0 ;
	}
	g_RecvList.InsertItem( iCount, "");
	g_RecvList.SetItemText( iCount , 0, strTime );
	g_RecvList.SetItemText( iCount , 1, orgAddr );
	g_RecvList.SetItemText( iCount , 2, strContent );
	g_RecvList.SetItemText( iCount , 3, strDestAddr );
	//g_RecvList.PostMessage(WM_VSCROLL,SB_BOTTOM,0);
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
void AddToSentListShow(CString strMsgID,CString destAddr,CString strContent,int iMsgLen,int iUserNum,int iPhoneType,CString strStatus,int iFlag,CString strOptId )
{
	int iCount = g_SentList.GetItemCount();
	if ( iCount >= 500 )		//ֻ����500����¼
	{
		g_SentList.DeleteAllItems();
		iCount =0 ;
	}
	CString strTemp = "";
	g_SentList.InsertItem( iCount, "");
	g_SentList.SetItemText( iCount , 0, CTime::GetCurrentTime().Format("%Y-%m-%d %H:%M:%S") );
	g_SentList.SetItemText( iCount , 1, strMsgID );
	g_SentList.SetItemText( iCount , 2, destAddr );
	g_SentList.SetItemText( iCount , 3, strContent );
	strTemp.Format("%d",iMsgLen);
	g_SentList.SetItemText( iCount , 4, strTemp );
	strTemp.Format("%d",iUserNum);
	g_SentList.SetItemText( iCount , 5, strTemp );
	switch (iPhoneType)
	{
	case 0:
		if (iFlag==0)
		{
			strTemp = "�ƶ�/��ͨ/����/С��ͨ";
			//strTemp = "�����ֺŶ�";
		}
		else
		{
			strTemp = "�ƶ�";
		}
        break;
	case 1:
		if (iFlag==0)
		{
			strTemp = "�ƶ�/��ͨ/����/С��ͨ";
			//strTemp = "�����ֺŶ�";
		}
		else
		{
			strTemp = "��ͨ";
		}
		break;
	case 2:
		strTemp = "С��ͨ";
		break;
	case 3:
		strTemp = "����";
		break;
	case 6:
		strTemp = "��δ���";
		break;
	default:
		{
			strTemp = "δ֪";
			break;
		}
	}
	g_SentList.SetItemText( iCount , 6, strTemp );
	g_SentList.SetItemText( iCount , 7, strStatus );
	//g_SentList.PostMessage(WM_VSCROLL,SB_BOTTOM,0);
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void AddToRePortListShow( CString strTime,CString strMsgID,CString strState )
{
	int i = 0;
	int iCount = g_ReportList.GetItemCount();
	if ( iCount >= 300 )		//ֻ����300����¼
	{
		g_ReportList.DeleteAllItems();
		iCount = 0 ;
	}
	g_ReportList.InsertItem( iCount, "");
	g_ReportList.SetItemText( iCount , 0, strTime );
	g_ReportList.SetItemText( iCount , 1, strMsgID );
	g_ReportList.SetItemText( iCount , 2, strState );
}
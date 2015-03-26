// MainBottomDialog.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "..\DbGate.h"
#include "MainBottomDialog.h"
#include ".\mainbottomdialog.h"

// CMainBottomDialog

IMPLEMENT_DYNCREATE(CMainBottomDialog, CFormView)
CListCtrl g_DispList;
CMainBottomDialog::CMainBottomDialog()
	: CFormView(CMainBottomDialog::IDD)
{
}

CMainBottomDialog::~CMainBottomDialog()
{
}

void CMainBottomDialog::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_DISP, g_DispList);
}

BEGIN_MESSAGE_MAP(CMainBottomDialog, CFormView)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CMainBottomDialog ���

#ifdef _DEBUG
void CMainBottomDialog::AssertValid() const
{
	CFormView::AssertValid();
}

void CMainBottomDialog::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/************************************************************************/
/*                                                                      */
/************************************************************************/
void CMainBottomDialog::OnSize(UINT nType, int cx, int cy)
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
	pWnd = GetDlgItem(IDC_LIST_DISP);
	if ((NULL != pWnd) && IsWindow(pWnd->GetSafeHwnd()))
	{            
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
		pWnd->MoveWindow(iLeft, iTop, iWidth, iHeight);		
	}	
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
void CMainBottomDialog::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();  
	InitDispCtrList();
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
void CMainBottomDialog::InitDispCtrList()
{
	//g_DispList.ShowWindow(SW_HIDE);
	DWORD dwSytle=::GetWindowLong(g_DispList.m_hWnd,GWL_STYLE);
	// ����Ϊ������ʽ
	SetWindowLong(g_DispList.m_hWnd,GWL_STYLE,dwSytle|LVS_REPORT);
	DWORD ExStyle=g_DispList.GetExtendedStyle();
	g_DispList.SetExtendedStyle(ExStyle|LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);

	::SendMessage(g_DispList.m_hWnd, LVM_SETEXTENDEDLISTVIEWSTYLE,
		LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);

	g_DispList.InsertColumn(0,_T("ʱ��"),LVCFMT_LEFT,125);
	g_DispList.InsertColumn(1,_T("��Ϣ"),LVCFMT_LEFT,470);
}
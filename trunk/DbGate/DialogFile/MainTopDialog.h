#pragma once
#include "afxcmn.h"
#include "../ControlFile/TabSheetEx.h"
// CMainTopDialog ������ͼ

class CMainTopDialog : public CFormView
{
	DECLARE_DYNCREATE(CMainTopDialog)

protected:
	CMainTopDialog();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~CMainTopDialog();

public:
	enum { IDD = IDD_DLG_MAINTOP };
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	void OnInitialUpdate();
	void InitTabCtrl();
	void ShowLists(int nSel);
	void InitReportCtrList();
	void InitSentCtrList();
	void InitRecvCtrList();	
private:
	CTabSheetEx m_TabShow;	
public:	
	afx_msg void OnTcnSelchangeTabShow(NMHDR *pNMHDR, LRESULT *pResult);
};


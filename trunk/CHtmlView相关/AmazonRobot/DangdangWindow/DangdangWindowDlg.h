// DangdangWindowDlg.h : ͷ�ļ�
//

#pragma once
#include "afxcmn.h"
#include "../GoodBase/ThreadBase.h"
#include <vector>
#include <afxmt.h>
using std::vector;


// CDangdangWindowDlg �Ի���
class CDangdangWindowDlg : public CDialog, public CThreadBase
{
// ����
public:
	CDangdangWindowDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_DANGDANGWINDOW_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
	void SetTableStyle();
	void ThreadProc();
public:
	CListCtrl m_CountList;
public:
	afx_msg void OnBnClickedBtnStart();
	CEvent* m_pSubmitEvent;
	vector<int> m_AutoOrderList;
public:
	afx_msg void OnBnClickedBtnSubmit();
public:
	afx_msg void OnBnClickedBtnImport();
public:
	afx_msg void OnClose();
	BOOL PreTranslateMessage( MSG* pMsg);
public:
	afx_msg void OnBnClickedButton2();
public:
	afx_msg void OnBnClickedBtnCode();
};

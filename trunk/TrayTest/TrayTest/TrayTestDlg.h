// TrayTestDlg.h : ͷ�ļ�
//

#pragma once
#include "SystemTray.h"


// CTrayTestDlg �Ի���
class CTrayTestDlg : public CDialog
{
// ����
public:
	CTrayTestDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_TRAYTEST_DIALOG };

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
public:
	afx_msg void OnBnClickedButton1();
	~CTrayTestDlg();
	CSystemTray *m_pTray;
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton5();
	afx_msg void OnBnClickedButton6();
	afx_msg void OnClose();
	afx_msg void OnTrayMenu();
	afx_msg void OnMenuexit();
	afx_msg void OnTestmenu();
	afx_msg void OnMenushow();
};

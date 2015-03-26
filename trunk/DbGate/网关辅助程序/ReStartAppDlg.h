// ReStartAppDlg.h : ͷ�ļ�
//

#pragma once


// CReStartAppDlg �Ի���
class CReStartAppDlg : public CDialog
{
// ����
public:
	CReStartAppDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_RESTARTAPP_DIALOG };

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
	afx_msg void OnBnClickedBtnStart();
	int GetAppPath(CString &strAppPath);
	afx_msg void OnTimer(UINT nIDEvent);
	int ReStartApp(int iFlag);
	afx_msg void OnBnClickedBtnExit();
	void InitParams();
	afx_msg LRESULT OnReInitparam(WPARAM wParam,LPARAM lParam);
	int m_iCheckInterval;
	CString m_strWindowName;
	CString m_strStrAppName;
	int m_iRunStatus;
	int m_iFailTime;
	afx_msg void OnSize(UINT nType, int cx, int cy);
	void CloseErrorWindow();
	static void CloseProcFun(LPVOID lpParam);	//
	afx_msg void OnClose();
	void CloseProc();
};

// TESTTESTDlg.h : ͷ�ļ�
//

#pragma once


// CTESTTESTDlg �Ի���
class CTESTTESTDlg : public CDialog
{
// ����
public:
	CTESTTESTDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_TESTTEST_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;
	HINSTANCE m_hModule;
	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	~CTESTTESTDlg();
	void ThreadProc(int iIndex);
	static void ThreadRun(LPVOID lpParam);
	void ShowMessage(CString strMsg);
};

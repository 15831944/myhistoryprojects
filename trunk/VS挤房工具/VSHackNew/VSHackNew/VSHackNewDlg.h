// VSHackNewDlg.h : ͷ�ļ�
//

#pragma once


// CVSHackNewDlg �Ի���
class CVSHackNewDlg : public CDialog
{
// ����
public:
	CVSHackNewDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_VSHACKNEW_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��
	
	static DWORD WINAPI CloseWarnProc(LPVOID lpParameter);

// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
	void CloseWarnWindow();
};

// AmazonWrapperDlg.h : ͷ�ļ�
//

#pragma once


// CAmazonWrapperDlg �Ի���
class CAmazonWrapperDlg : public CDialog
{
// ����
public:
	CAmazonWrapperDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_AMAZONWRAPPER_DIALOG };

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
	BOOL PreTranslateMessage( MSG* pMsg );
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
};

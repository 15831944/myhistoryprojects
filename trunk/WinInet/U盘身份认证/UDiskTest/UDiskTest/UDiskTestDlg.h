// UDiskTestDlg.h : ͷ�ļ�
//

#pragma once


// CUDiskTestDlg �Ի���
class CUDiskTestDlg : public CDialog
{
// ����
public:
	CUDiskTestDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_UDISKTEST_DIALOG };

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
	afx_msg void OnBnClickedOk();
	void ShowMessage(CString strInfo);
	CString GetDriverInfomation(CString bAlpha,int iGTI_TYPE);
};

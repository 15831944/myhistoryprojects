// SerialNumberDlg.h : ͷ�ļ�
//

#pragma once


// CSerialNumberDlg �Ի���
class CSerialNumberDlg : public CDialog
{
// ����
public:
	CSerialNumberDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_SERIALNUMBER_DIALOG };

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
	afx_msg void OnBnClickedButtonGen();
	afx_msg void OnBnClickedButtonReg();
	afx_msg void OnBnClickedButtonGen2();
};

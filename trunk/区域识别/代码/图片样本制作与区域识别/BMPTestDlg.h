// BMPTestDlg.h : ͷ�ļ�
//

#pragma once
#include "MyImage.h"
// CBMPTestDlg �Ի���
class CBMPTestDlg : public CDialog
{
// ����
public:
	CBMPTestDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_BMPTEST_DIALOG };

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
	int GetRows(CString fileName);
	void GenerateDataFile();
	afx_msg void OnBnClickedBtnDetect();
	afx_msg void OnBnClickedBtnCreattemp();
	afx_msg void OnBnClickedBtnCbat();
	afx_msg void OnBnClickedBtnCvec();
	afx_msg void OnBnClickedBtnTrain();
	afx_msg void OnBnClickedBtnCxml();
};

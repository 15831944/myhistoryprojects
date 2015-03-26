// XlsCtrolDlg.h : ͷ�ļ�
//

#pragma once

#include "XlsCtrolClass.h"
#include "BasicExcel.hpp"
using namespace YExcel;
// CXlsCtrolDlg �Ի���
class CXlsCtrolDlg : public CDialog
{
// ����
public:
	CXlsCtrolDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_XLSCTROL_DIALOG };

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
	afx_msg void OnBnClickedButton3();
	
	vector<ExpressStr> m_vecExpressInfo;
	afx_msg void OnBnClickedButton2();
	CXlsCtrolClass m_XlsCtrol;

	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton5();
	void GenData(vector<ExpressStr> &vecStr);
	afx_msg void OnBnClickedButton6();
};

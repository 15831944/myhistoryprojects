// AmazonWrapperDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"
//#include "../XinHuaLib/XinHuaClass.h"
//#include "../JingDongLib/XinHuaClass.h"
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
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton5();
	void SplitString(CString strSourceEx, CString strSep, CStringArray &Strings);
	afx_msg void OnBnClickedRadio1();
	afx_msg void OnBnClickedRadio2();
public:
	CComboBox m_com_paytype;
public:
	CComboBox m_cob_billcontent;
public:
	CComboBox m_comb_shiptype;
public:
	CComboBox m_combo_sendtime;
};

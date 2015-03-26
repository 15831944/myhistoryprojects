#pragma once
#include "Resource.h"
#include "afxwin.h"
#include <map>
using namespace std;
// CCountGen �Ի���

extern int g_iFreshCount;
class CCountGen : public CDialog
{
	DECLARE_DYNAMIC(CCountGen)

public:
	CCountGen(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CCountGen();
	CListCtrl *m_pList;
// �Ի�������
	enum { IDD = IDD_DLG_SET };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	int GenerateCounts();
	BOOL OnInitDialog();
	BOOL PreTranslateMessage( MSG* pMsg);
	DECLARE_MESSAGE_MAP()
	char RandChar(char inChar);
public:
	afx_msg void OnBnClickedBtnGencount();
	afx_msg void OnBnClickedBtnSave();
	int SaveParams();
	afx_msg void OnBnClickedCheckAutoip2();
	CButton m_AutoCheck;
	CButton m_AutoIP;
	afx_msg void OnBnClickedCheckAutocheckcode2();
	char DoVerify(const char* pszSrc);
	afx_msg void OnBnClickedRadioIpchange1();
	afx_msg void OnBnClickedRadioIpchange2();
	CButton m_ipchange1;
	int m_ipchange2;
	afx_msg void OnBnClickedRadioCheckip1();
	afx_msg void OnBnClickedRadioCheckip2();
	afx_msg void OnBnClickedRadioCheckip3();
	afx_msg void OnBnClickedBtnFind();
	afx_msg void OnBnClickedRadioLongyan();
	afx_msg void OnBnClickedRadioShaowu();
	afx_msg void OnBnClickedRadioPutian();
	afx_msg void OnBnClickedCheckChangemac();
	afx_msg void OnBnClickedButton1();
	void LoadOldAcc();
	map<CString,int> m_mapOldAcc;
};

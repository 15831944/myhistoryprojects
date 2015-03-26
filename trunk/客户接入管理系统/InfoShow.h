#pragma once
#include "afxwin.h"
#include "afxdtctl.h"

// CInfoShow �Ի���

class CInfoShow : public CDialog
{
	DECLARE_DYNAMIC(CInfoShow)

public:
	CInfoShow(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CInfoShow();

// �Ի�������
	enum { IDD = IDD_DIALOG_INFO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnDeletetype();
	BOOL PreTranslateMessage( MSG* pMsg );
	BOOL OnInitDialog();
	int m_iFlag;		//��־λ��1Ϊ��ӣ�2Ϊ�޸ģ�����ΪNOTHING�˳�
	int m_iCustomerID;
public:
	afx_msg void OnBnClickedBtnCancle();
	void QueTypeAndLanguage();
	void SetParams(CString strdbName,CString strdbServer,CString strdbUser,CString strPwd,int iFlag,int iCustomerID);
	void InitControls();
public:
	CComboBox m_comboType;
	CComboBox m_combo_language;
	map<CString, int>  m_typeMap;
	map<CString, int>  m_languageMap;
	//CString m_dbPath;
	CString m_dbServer;		//���ݿ������
	CString m_dbName;		//���ݿ���
	CString m_dbUser;		//��¼�˻�
	CString m_dbPwd;		//��¼����
public:
	afx_msg void OnBnClickedBtnAddtype();
public:
	afx_msg void OnBnClickedBtnSave();
public:
	afx_msg void OnBnClickedBtnAdd();
public:
	afx_msg void OnBnClickedBtnAddlang();
public:
	afx_msg void OnBnClickedBtnDeletelang();
	int GetComboIndex(int iValue,int iFlag);
	void QueType();
	void QueLanguage();
public:
	CDateTimeCtrl m_datePicker;
};

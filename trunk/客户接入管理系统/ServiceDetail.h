#pragma once
#include "afxcmn.h"


// CServiceDetail �Ի���

class CServiceDetail : public CDialog
{
	DECLARE_DYNAMIC(CServiceDetail)

public:
	CServiceDetail(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CServiceDetail();

// �Ի�������
	enum { IDD = IDD_DLG_DETAIL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
	void InitCtrList();
	BOOL OnInitDialog();


public:
	void InitData();
	void SetParams(CString strdbName,CString strdbServer,CString strdbUser,CString strPwd,CString strID);
	BOOL PreTranslateMessage( MSG* pMsg );
	CListCtrl m_resultList;
	CString m_strID;
	CString m_dbServer;		//���ݿ������
	CString m_dbName;		//���ݿ���
	CString m_dbUser;		//��¼�˻�
	CString m_dbPwd;		//��¼����
public:
	afx_msg void OnDetailAdd();
public:
	afx_msg void OnDetailEdit();
public:
	afx_msg void OnDetailDelete();
public:
	afx_msg void OnNMRclickListShow(NMHDR *pNMHDR, LRESULT *pResult);
public:
	afx_msg void OnNMDblclkListShow(NMHDR *pNMHDR, LRESULT *pResult);
};

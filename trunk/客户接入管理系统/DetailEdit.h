#pragma once


// CDetailEdit �Ի���

class CDetailEdit : public CDialog
{
	DECLARE_DYNAMIC(CDetailEdit)

public:
	CDetailEdit(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDetailEdit();

// �Ի�������
	enum { IDD = IDD_DLG_DETAIL_EDIT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnAdd();
public:
	afx_msg void OnBnClickedBtnSave();
public:
	afx_msg void OnBnClickedBtnExit();
	void SetParams(CString strdbName,CString strdbServer,CString strdbUser,CString strPwd,CString strID,int iFlag);
	BOOL OnInitDialog();
	void InitData();
	BOOL PreTranslateMessage( MSG* pMsg );
	CString m_strID;
	CString m_dbServer;		//���ݿ������
	CString m_dbName;		//���ݿ���
	CString m_dbUser;		//��¼�˻�
	CString m_dbPwd;		//��¼����
	int m_iFlag;
};

#pragma once


// CSingleSend �Ի���

class CSingleSend : public CDialog
{
	DECLARE_DYNAMIC(CSingleSend)

public:
	CSingleSend(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CSingleSend();

// �Ի�������
	enum { IDD = IDD_DIALOG_SINGLESEND };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	DECLARE_MESSAGE_MAP()
public:
	BOOL OnInitDialog();
public:
	afx_msg void OnBnClickedBtnSend();
public:
	CString m_Content;
public:
	CString m_strPhones;
public:
	CString m_strPhone;
};

#pragma once


// CCheckCodeInput �Ի���

class CCheckCodeInput : public CDialog
{
	DECLARE_DYNAMIC(CCheckCodeInput)

public:
	CCheckCodeInput(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CCheckCodeInput();

// �Ի�������
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
};

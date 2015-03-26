#pragma once
#include "afxwin.h"
#include "UIHelper.h"
#include "PictureButton\PicButton.h"

// CFolderNameBuilder �Ի���

class CFolderNameBuilder : public CDialog
{
	DECLARE_DYNAMIC(CFolderNameBuilder)

public:
	CFolderNameBuilder(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CFolderNameBuilder();

	CString GetFolderName();

// �Ի�������
	enum { IDD = IDD_DIALOG_NAME };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
private:
	CString m_FolderName;
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedOk();
	CEdit m_EditName;
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);

	CPicButton m_btOK;
	CPicButton m_btCancel;
	CPicButton m_btCancel2;

	virtual BOOL OnInitDialog();
	void SetDlgReign();
	afx_msg void OnBnClickedCancel2();
	afx_msg LRESULT OnNcHitTest(CPoint point);
};

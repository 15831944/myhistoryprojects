#pragma once
#include "afxcmn.h"

// CMainBottomDialog ������ͼ
class CMainBottomDialog : public CFormView
{
	DECLARE_DYNCREATE(CMainBottomDialog)

protected:
	CMainBottomDialog();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~CMainBottomDialog();

public:
	enum { IDD = IDD_DLG_MAINBOTTOM };
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	void InitDispCtrList();
	void OnInitialUpdate();
	
};



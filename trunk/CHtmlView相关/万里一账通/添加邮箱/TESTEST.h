#pragma once


// TESTEST �Ի���

class TESTEST : public CDHtmlDialog
{
	DECLARE_DYNCREATE(TESTEST)

public:
	TESTEST(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~TESTEST();
// ��д
	HRESULT OnButtonOK(IHTMLElement *pElement);
	HRESULT OnButtonCancel(IHTMLElement *pElement);

// �Ի�������
	enum { IDD = IDD_DLG_HTMLDLG, IDH = IDR_HTML_TESTEST };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
	DECLARE_DHTML_EVENT_MAP()
};

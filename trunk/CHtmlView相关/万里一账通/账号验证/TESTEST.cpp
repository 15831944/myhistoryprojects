// TESTEST.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "TESTEST.h"


// TESTEST �Ի���

IMPLEMENT_DYNCREATE(TESTEST, CDHtmlDialog)

TESTEST::TESTEST(CWnd* pParent /*=NULL*/)
	: CDHtmlDialog(TESTEST::IDD, TESTEST::IDH, pParent)
{
}

TESTEST::~TESTEST()
{
}

void TESTEST::DoDataExchange(CDataExchange* pDX)
{
	CDHtmlDialog::DoDataExchange(pDX);
}

BOOL TESTEST::OnInitDialog()
{
	CDHtmlDialog::OnInitDialog();
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

BEGIN_MESSAGE_MAP(TESTEST, CDHtmlDialog)
END_MESSAGE_MAP()

BEGIN_DHTML_EVENT_MAP(TESTEST)
	DHTML_EVENT_ONCLICK(_T("ButtonOK"), OnButtonOK)
	DHTML_EVENT_ONCLICK(_T("ButtonCancel"), OnButtonCancel)
END_DHTML_EVENT_MAP()



// TESTEST ��Ϣ�������

HRESULT TESTEST::OnButtonOK(IHTMLElement* /*pElement*/)
{
	OnOK();
	return S_OK;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

HRESULT TESTEST::OnButtonCancel(IHTMLElement* /*pElement*/)
{
	OnCancel();
	return S_OK;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

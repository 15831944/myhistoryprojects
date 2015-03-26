// SDocEvtHandler.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SDocEvtHandler.h"
#include "mshtmdid.h"


// SDocEvtHandler

IMPLEMENT_DYNAMIC(SDocEvtHandler, CCmdTarget)


SDocEvtHandler::SDocEvtHandler()
{
	EnableAutomation();
	m_pOnClickCallback = NULL;
}

SDocEvtHandler::~SDocEvtHandler()
{
}

BEGIN_MESSAGE_MAP(SDocEvtHandler, CCmdTarget)
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(SDocEvtHandler, CCmdTarget)
  DISP_FUNCTION_ID(SDocEvtHandler,"HTMLELEMENTEVENTS2_ONCLICK",
  DISPID_HTMLELEMENTEVENTS2_ONCLICK, OnClick,
  VT_EMPTY, VTS_DISPATCH)
END_DISPATCH_MAP()

BEGIN_INTERFACE_MAP(SDocEvtHandler, CCmdTarget)
  INTERFACE_PART(SDocEvtHandler,
  DIID_HTMLButtonElementEvents2, Dispatch)
END_INTERFACE_MAP()


void SDocEvtHandler::OnClick(MSHTML::IHTMLEventObjPtr pEvtObj)
{
	MSHTML::IHTMLElementPtr pElement =
		pEvtObj->GetsrcElement(); // �¼������Ķ���Ԫ��

	if (m_pOnClickCallback)
	{
		if (m_pOnClickCallback->IsEnableCallback())
		{
			BOOL bRes = m_pOnClickCallback->call(pEvtObj);
			if (!bRes)
				pEvtObj->put_returnValue(_variant_t(VARIANT_FALSE, VT_BOOL));
		}
	}

	// ��2��ȡ���������ֹת��Ŀ��ҳ��
	//pEvtObj->put_returnValue(_variant_t(VARIANT_FALSE, VT_BOOL));
}

// SDocEvtHandler ��Ϣ�������

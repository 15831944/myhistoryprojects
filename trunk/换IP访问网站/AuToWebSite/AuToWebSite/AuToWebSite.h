// AuToWebSite.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CAuToWebSiteApp:
// �йش����ʵ�֣������ AuToWebSite.cpp
//

class CAuToWebSiteApp : public CWinApp
{
public:
	CAuToWebSiteApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CAuToWebSiteApp theApp;
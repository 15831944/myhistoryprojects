// XlsCtrol.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CXlsCtrolApp:
// �йش����ʵ�֣������ XlsCtrol.cpp
//

class CXlsCtrolApp : public CWinApp
{
public:
	CXlsCtrolApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CXlsCtrolApp theApp;
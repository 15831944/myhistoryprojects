// CheckCodeCatch.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CCheckCodeCatchApp:
// �йش����ʵ�֣������ CheckCodeCatch.cpp
//

class CCheckCodeCatchApp : public CWinApp
{
public:
	CCheckCodeCatchApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CCheckCodeCatchApp theApp;
// MobileBomb.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CMobileBombApp:
// �йش����ʵ�֣������ MobileBomb.cpp
//

class CMobileBombApp : public CWinApp
{
public:
	CMobileBombApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CMobileBombApp theApp;
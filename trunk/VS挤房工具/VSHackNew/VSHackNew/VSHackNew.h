// VSHackNew.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CVSHackNewApp:
// �йش����ʵ�֣������ VSHackNew.cpp
//

class CVSHackNewApp : public CWinApp
{
public:
	CVSHackNewApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CVSHackNewApp theApp;
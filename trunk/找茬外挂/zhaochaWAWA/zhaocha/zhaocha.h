// zhaocha.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error �ڰ������� PCH �Ĵ��ļ�֮ǰ������stdafx.h��
#endif

#include "resource.h"		// ������


// CzhaochaApp:
// �йش����ʵ�֣������ zhaocha.cpp
//

class CzhaochaApp : public CWinApp
{
public:
	CzhaochaApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CzhaochaApp theApp;

// Web1800AutoTester.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CWeb1800AutoTesterApp:
// �йش����ʵ�֣������ Web1800AutoTester.cpp
//

class CWeb1800AutoTesterApp : public CWinApp
{
public:
	CWeb1800AutoTesterApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CWeb1800AutoTesterApp theApp;
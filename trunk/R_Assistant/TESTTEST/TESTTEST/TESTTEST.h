// TESTTEST.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error �ڰ������� PCH �Ĵ��ļ�֮ǰ������stdafx.h��
#endif

#include "resource.h"		// ������


// CTESTTESTApp:
// �йش����ʵ�֣������ TESTTEST.cpp
//

class CTESTTESTApp : public CWinApp
{
public:
	CTESTTESTApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CTESTTESTApp theApp;

// BMPTest.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error �ڰ������� PCH �Ĵ��ļ�֮ǰ������stdafx.h��
#endif

#include "resource.h"		// ������


// CBMPTestApp:
// �йش����ʵ�֣������ BMPTest.cpp
//

class CBMPTestApp : public CWinApp
{
public:
	CBMPTestApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CBMPTestApp theApp;

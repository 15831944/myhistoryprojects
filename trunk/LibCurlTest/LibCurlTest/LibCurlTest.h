// LibCurlTest.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CLibCurlTestApp:
// �йش����ʵ�֣������ LibCurlTest.cpp
//

class CLibCurlTestApp : public CWinApp
{
public:
	CLibCurlTestApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CLibCurlTestApp theApp;
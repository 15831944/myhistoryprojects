// DetectDLLTest.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error �ڰ������� PCH �Ĵ��ļ�֮ǰ������stdafx.h��
#endif

#include "resource.h"		// ������


// CDetectDLLTestApp:
// �йش����ʵ�֣������ DetectDLLTest.cpp
//

class CDetectDLLTestApp : public CWinApp
{
public:
	CDetectDLLTestApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CDetectDLLTestApp theApp;

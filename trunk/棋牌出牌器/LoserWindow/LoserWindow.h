// LoserWindow.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error �ڰ������� PCH �Ĵ��ļ�֮ǰ������stdafx.h��
#endif

#include "resource.h"		// ������


// CLoserWindowApp:
// �йش����ʵ�֣������ LoserWindow.cpp
//

class CLoserWindowApp : public CWinApp
{
public:
	CLoserWindowApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CLoserWindowApp theApp;

// TotalMatch.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error �ڰ������� PCH �Ĵ��ļ�֮ǰ������stdafx.h��
#endif

#include "resource.h"		// ������


// CTotalMatchApp:
// �йش����ʵ�֣������ TotalMatch.cpp
//

class CTotalMatchApp : public CWinApp
{
public:
	CTotalMatchApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CTotalMatchApp theApp;

// TotalCollect.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error �ڰ������� PCH �Ĵ��ļ�֮ǰ������stdafx.h��
#endif

#include "resource.h"		// ������


// CTotalCollectApp:
// �йش����ʵ�֣������ TotalCollect.cpp
//

class CTotalCollectApp : public CWinApp
{
public:
	CTotalCollectApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CTotalCollectApp theApp;

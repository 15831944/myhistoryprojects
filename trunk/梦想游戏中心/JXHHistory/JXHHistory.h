// JXHHistory.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error �ڰ������� PCH �Ĵ��ļ�֮ǰ������stdafx.h��
#endif

#include "resource.h"		// ������


// CJXHHistoryApp:
// �йش����ʵ�֣������ JXHHistory.cpp
//

class CJXHHistoryApp : public CWinApp
{
public:
	CJXHHistoryApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CJXHHistoryApp theApp;

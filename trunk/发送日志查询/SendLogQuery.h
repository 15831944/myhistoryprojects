// SendLogQuery.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error �ڰ������� PCH �Ĵ��ļ�֮ǰ������stdafx.h��
#endif

#include "resource.h"		// ������


// CSendLogQueryApp:
// �йش����ʵ�֣������ SendLogQuery.cpp
//

class CSendLogQueryApp : public CWinApp
{
public:
	CSendLogQueryApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
	BOOL ExitInstance(void);
	CString GetAppPath();
};

extern CSendLogQueryApp theApp;

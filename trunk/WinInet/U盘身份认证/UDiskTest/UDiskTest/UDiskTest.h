// UDiskTest.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CUDiskTestApp:
// �йش����ʵ�֣������ UDiskTest.cpp
//

class CUDiskTestApp : public CWinApp
{
public:
	CUDiskTestApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CUDiskTestApp theApp;
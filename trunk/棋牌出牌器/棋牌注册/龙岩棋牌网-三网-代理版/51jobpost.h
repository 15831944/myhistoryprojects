// 51jobpost.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error �ڰ������� PCH �Ĵ��ļ�֮ǰ������stdafx.h��
#endif

#include "resource.h"		// ������


// CjobpostApp:
// �йش����ʵ�֣������ 51jobpost.cpp
//

class CjobpostApp : public CWinApp
{
public:
	CjobpostApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CjobpostApp theApp;


// PicOperate.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CPicOperateApp:
// �йش����ʵ�֣������ PicOperate.cpp
//

class CPicOperateApp : public CWinApp
{
public:
	CPicOperateApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CPicOperateApp theApp;
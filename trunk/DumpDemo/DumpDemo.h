// DumpDemo.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������

#include "mdump.h"
// CDumpDemoApp:
// �йش����ʵ�֣������ DumpDemo.cpp
//

class CDumpDemoApp : public CWinApp
{
public:
	CDumpDemoApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
private:
	CMiniDumper m_MiniDumper;
};

extern CDumpDemoApp theApp;
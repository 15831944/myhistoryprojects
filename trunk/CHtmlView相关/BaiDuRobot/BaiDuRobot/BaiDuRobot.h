// BaiDuRobot.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CBaiDuRobotApp:
// �йش����ʵ�֣������ BaiDuRobot.cpp
//

class CBaiDuRobotApp : public CWinApp
{
public:
	CBaiDuRobotApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CBaiDuRobotApp theApp;
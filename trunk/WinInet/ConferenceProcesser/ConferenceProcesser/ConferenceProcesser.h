// ConferenceProcesser.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CConferenceProcesserApp:
// �йش����ʵ�֣������ ConferenceProcesser.cpp
//

class CConferenceProcesserApp : public CWinApp
{
public:
	CConferenceProcesserApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()

	bool m_bFromService;
};

extern CConferenceProcesserApp theApp;
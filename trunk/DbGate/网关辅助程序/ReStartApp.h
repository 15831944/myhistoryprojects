// ReStartApp.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error �ڰ������� PCH �Ĵ��ļ�֮ǰ������stdafx.h��
#endif

#include "resource.h"		// ������


// CReStartAppApp:
// �йش����ʵ�֣������ ReStartApp.cpp
//

class CReStartAppApp : public CWinApp
{
public:
	CReStartAppApp();

// ��д
	public:
	virtual BOOL InitInstance();
	HANDLE m_hMutex;
// ʵ��

	DECLARE_MESSAGE_MAP()
	BOOL ExitInstance();
	void WriteLog(CString srcStr,int iFlag);
};

extern CReStartAppApp theApp;

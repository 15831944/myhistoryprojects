// FileTransferTest.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CFileTransferTestApp:
// �йش����ʵ�֣������ FileTransferTest.cpp
//

class CFileTransferTestApp : public CWinApp
{
public:
	CFileTransferTestApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CFileTransferTestApp theApp;
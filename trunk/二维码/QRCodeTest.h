// QRCodeTest.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error �ڰ������� PCH �Ĵ��ļ�֮ǰ������stdafx.h��
#endif

#include "resource.h"		// ������


// CQRCodeTestApp:
// �йش����ʵ�֣������ QRCodeTest.cpp
//

class CQRCodeTestApp : public CWinApp
{
public:
	CQRCodeTestApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CQRCodeTestApp theApp;

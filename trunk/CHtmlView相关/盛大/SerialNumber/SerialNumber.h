// SerialNumber.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error �ڰ������� PCH �Ĵ��ļ�֮ǰ������stdafx.h��
#endif

#include "resource.h"		// ������


// CSerialNumberApp:
// �йش����ʵ�֣������ SerialNumber.cpp
//

class CSerialNumberApp : public CWinApp
{
public:
	CSerialNumberApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CSerialNumberApp theApp;

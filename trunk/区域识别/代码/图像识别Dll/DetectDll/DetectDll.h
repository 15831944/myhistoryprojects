// DetectDll.h : DetectDll DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// ������



// CDetectDllApp
// �йش���ʵ�ֵ���Ϣ������� DetectDll.cpp
//

class CDetectDllApp : public CWinApp
{
public:
	CDetectDllApp();

// ��д
public:
	virtual BOOL InitInstance();
	
	DECLARE_MESSAGE_MAP()
};

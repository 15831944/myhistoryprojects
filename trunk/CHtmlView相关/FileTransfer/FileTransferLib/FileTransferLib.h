// FileTransferLib.h : FileTransferLib DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CFileTransferLibApp
// �йش���ʵ�ֵ���Ϣ������� FileTransferLib.cpp
//

class CFileTransferLibApp : public CWinApp
{
public:
	CFileTransferLibApp();

// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};

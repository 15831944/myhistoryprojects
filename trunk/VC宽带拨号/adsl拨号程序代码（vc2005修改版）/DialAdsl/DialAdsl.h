// DialAdsl.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//
//���������������д���ֽ����뷢����ѧϰ����֮��
//������ķ�������GPLЭ�飬�����ڴ�Э��Ĺ涨��ʹ�ñ�����
//�������ʹ�ñ������;����ʲô���ʻ��������http://digua254909662.blog.163.com/blog/static/74805072200922323951508/edit/
//������ϵ���ߣ�qq<254909662> ��˵�����⣬���˷ǳ�������������

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CDialAdslApp:
// �йش����ʵ�֣������ DialAdsl.cpp
//

class CDialAdslApp : public CWinApp
{
public:
	CDialAdslApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CDialAdslApp theApp;
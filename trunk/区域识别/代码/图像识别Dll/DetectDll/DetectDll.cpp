// DetectDll.cpp : ���� DLL �ĳ�ʼ�����̡�
//

#include "stdafx.h"
#include "DetectDll.h"
#include "MyImage.h"

#define DETECTDLL_API extern "C" _declspec(dllexport)
#include "DetectDllClient.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//
//	ע�⣡
//
//		����� DLL ��̬���ӵ� MFC
//		DLL���Ӵ� DLL ������
//		���� MFC ���κκ����ں�������ǰ��
//		��������� AFX_MANAGE_STATE �ꡣ
//
//		����:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// �˴�Ϊ��ͨ������
//		}
//
//		�˺������κ� MFC ����
//		������ÿ��������ʮ����Ҫ������ζ��
//		��������Ϊ�����еĵ�һ�����
//		���֣������������ж������������
//		������Ϊ���ǵĹ��캯���������� MFC
//		DLL ���á�
//
//		�й�������ϸ��Ϣ��
//		����� MFC ����˵�� 33 �� 58��
//

// CDetectDllApp

BEGIN_MESSAGE_MAP(CDetectDllApp, CWinApp)
END_MESSAGE_MAP()


// CDetectDllApp ����

CDetectDllApp::CDetectDllApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CDetectDllApp ����

CDetectDllApp theApp;


// CDetectDllApp ��ʼ��

BOOL CDetectDllApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}
extern "C" _declspec(dllexport) CPoint GetCenterPoint(CString picPath,CString xmlPath)
{
	CMyImage myImage;
	return myImage.MatchPicture(picPath,xmlPath);

}

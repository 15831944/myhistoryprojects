// SendLogQuery.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "SendLogQuery.h"
#include "SendLogQueryDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSendLogQueryApp

BEGIN_MESSAGE_MAP(CSendLogQueryApp, CWinApp)
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()


// CSendLogQueryApp ����

CSendLogQueryApp::CSendLogQueryApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CSendLogQueryApp ����

CSendLogQueryApp theApp;


/************************************************************************/
/*                                                                      */
/************************************************************************/
BOOL CSendLogQueryApp::InitInstance()
{
	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControls()�����򣬽��޷��������ڡ�
	InitCommonControls();

	CWinApp::InitInstance();

	AfxEnableControlContainer();

	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("������־��ѯ����"));
	::CoInitialize(NULL);
	
	CSendLogQueryDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: �ڴ˷��ô����ʱ�á�ȷ�������ر�
		//�Ի���Ĵ���
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: �ڴ˷��ô����ʱ�á�ȡ�������ر�
		//�Ի���Ĵ���
	}

	// ���ڶԻ����ѹرգ����Խ����� FALSE �Ա��˳�Ӧ�ó���
	// ����������Ӧ�ó������Ϣ�á�
	return FALSE;
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
BOOL CSendLogQueryApp::ExitInstance(void)
{
	return FALSE;
	::CoUninitialize();
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
CString CSendLogQueryApp::GetAppPath()
{
	CString strRet;
	TCHAR szBuff[_MAX_PATH];
	VERIFY(::GetModuleFileName(AfxGetApp()->m_hInstance, szBuff, _MAX_PATH));
	strRet=szBuff;
	int pos=strRet.ReverseFind('\\');
	strRet=strRet.Left(pos);
	return strRet;
}
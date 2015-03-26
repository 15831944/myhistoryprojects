// ConferenceProcesser.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "ConferenceProcesser.h"
#include "ConferenceProcesserDlg.h"
#include "../Util/Service.h"
#include "../Util/mdump.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

const char MutexName [] = "Web1800Service_Win32_Instance_Mutex";

UINT uShowServiceSettingMsg = ::RegisterWindowMessage("ShowWeb1800ServiceSetting");

CService			_Module;
// CConferenceProcesserApp

BEGIN_MESSAGE_MAP(CConferenceProcesserApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CConferenceProcesserApp ����

CConferenceProcesserApp::CConferenceProcesserApp()
{
	
}


// Ψһ��һ�� CConferenceProcesserApp ����

CConferenceProcesserApp theApp;


// CConferenceProcesserApp ��ʼ��

BOOL CConferenceProcesserApp::InitInstance()
{
	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControlsEx()�����򣬽��޷��������ڡ�
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ��������Ϊ��������Ҫ��Ӧ�ó�����ʹ�õ�
	// �����ؼ��ࡣ
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	AfxEnableControlContainer();

	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));

	CConferenceProcesserDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: �ڴ˴����ô����ʱ�á�ȷ�������ر�
		//  �Ի���Ĵ���
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: �ڴ˷��ô����ʱ�á�ȡ�������ر�
		//  �Ի���Ĵ���
	}

	// ���ڶԻ����ѹرգ����Խ����� FALSE �Ա��˳�Ӧ�ó���
	//  ����������Ӧ�ó������Ϣ�á�
	return FALSE;
}


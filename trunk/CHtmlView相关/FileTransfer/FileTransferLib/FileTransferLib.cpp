// FileTransferLib.cpp : ���� DLL �ĳ�ʼ�����̡�
//

#include "stdafx.h"
#include "FileTransferLib.h"
//#include "FileServer.h"
//#include "FileClient.h"
#include "GlobalFun.h"
#include "ThreadShell.h"
#include <map>
using namespace std;
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//
//TODO: ����� DLL ����� MFC DLL �Ƕ�̬���ӵģ�
//		��Ӵ� DLL �������κε���
//		MFC �ĺ������뽫 AFX_MANAGE_STATE ����ӵ�
//		�ú�������ǰ�档
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


// CFileTransferLibApp

BEGIN_MESSAGE_MAP(CFileTransferLibApp, CWinApp)
END_MESSAGE_MAP()


// CFileTransferLibApp ����

CFileTransferLibApp::CFileTransferLibApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CFileTransferLibApp ����

CFileTransferLibApp theApp;


// CFileTransferLibApp ��ʼ��

BOOL CFileTransferLibApp::InitInstance()
{
	CWinApp::InitInstance();

	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}

	AfxInitRichEdit2();

	return TRUE;
}

FileTransferLog gl_logger;
CThreadShell m_ServerManager;
//add by shaojun at 2010-09-28 ͨ����������������ļ�����Ķ���
CThreadShell m_ClientManagerEx[10];
static map<HWND,CThreadShell *> gl_mapClientManager;

//�ͻ��ˣ�����䶯
extern "C" __declspec(dllexport) bool CreateFileServer(int nPort, HWND MSGNotifyWnd, int nQuitMessageID, LPCTSTR strLanguageId, bool bAutoControl)
{
	bool bRes = m_ServerManager.CreateFileServer(nPort, MSGNotifyWnd, nQuitMessageID, strLanguageId, bAutoControl);

	if (gl_pLogger) 
		gl_pLogger->log_info("\r\n\r\nMain proccess CreateFileServer. Port:%d, LanguageId:%s return %d", nPort, strLanguageId, bRes);

	return bRes;
}
//�ͻ��ˣ�����䶯
extern "C" __declspec(dllexport) void StopFileServer(bool bLocal)
{
	if (gl_pLogger) 
		gl_pLogger->log_info("Main proccess StopFileServer.");
	return m_ServerManager.Stop();
}

extern "C" __declspec(dllexport) bool CreateFileClient(char* pServerAddress, int nPort, HWND MSGNotifyWnd, int nQuitMessageID, bool bGetRemoteFile)
{
	CThreadShell *pShell = NULL;
	if (gl_mapClientManager.find(MSGNotifyWnd) != gl_mapClientManager.end())
	{
		pShell = gl_mapClientManager[MSGNotifyWnd];
	}
	else
	{
		pShell = new CThreadShell;
		gl_mapClientManager[MSGNotifyWnd] = pShell;
	}
	if (!pShell)
	{
		if (gl_pLogger) gl_pLogger->log_info("CreateFileClient pShell NULL.");
		return false;
	}
	bool bRes = pShell->CreateFileClient(pServerAddress, nPort, MSGNotifyWnd, nQuitMessageID, bGetRemoteFile);

	if (gl_pLogger) 
		gl_pLogger->log_info("\r\n\r\nMain proccess CreateFileClient. ServerAddress:%s, Port:%d, GetRemoteFile:%d,HWND:%d,return %d", pServerAddress, nPort, bGetRemoteFile, MSGNotifyWnd,bRes);
	return bRes;
}
	
extern "C" __declspec(dllexport) void StopFileClient(bool bLocal,HWND hwnd)
{
	CThreadShell *pShell = NULL;
	map<HWND,CThreadShell *>::iterator it = gl_mapClientManager.find(hwnd);
	if ( it != gl_mapClientManager.end())
	{
		pShell = gl_mapClientManager[hwnd];
		gl_mapClientManager.erase(it);
	}	
	if (!pShell)
	{
		if (gl_pLogger) gl_pLogger->log_info("StopFileClient pShell NULL.");
		return ;
	}
	if (gl_pLogger) gl_pLogger->log_info("Main proccess StopFileClient. HWND:%d",hwnd);
	pShell->Stop();
	delete pShell;
	pShell = NULL;
	return;
}
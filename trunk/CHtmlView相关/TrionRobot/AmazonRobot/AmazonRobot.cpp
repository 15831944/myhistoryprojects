// AmazonRobot.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "AmazonRobot.h"
#include "AmazonRobotDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAmazonRobotApp

BEGIN_MESSAGE_MAP(CAmazonRobotApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()

void InitParams(CString strPath)
{
	char szTemp[2048] = {0};
	CString strNode = "set";

	::GetPrivateProfileString(strNode,"strGoodNos", "",(LPSTR)szTemp,2047, strPath);
	gl_AmazonStr.strGoodNos = szTemp;

	memset(szTemp,0,2048);
	::GetPrivateProfileString(strNode,"strUserID", "",(LPSTR)szTemp,2047, strPath);
	gl_AmazonStr.strUserID = szTemp;

	memset(szTemp,0,2048);
	::GetPrivateProfileString(strNode,"strPwd", "",(LPSTR)szTemp,2047, strPath);
	gl_AmazonStr.strPwd = szTemp;

	memset(szTemp,0,2048);
	::GetPrivateProfileString(strNode,"strProvince", "",(LPSTR)szTemp,2047, strPath);
	gl_AmazonStr.strProvince = szTemp;

	memset(szTemp,0,2048);
	::GetPrivateProfileString(strNode,"strCity", "",(LPSTR)szTemp,2047, strPath);
	gl_AmazonStr.strCity = szTemp;

	memset(szTemp,0,2048);
	::GetPrivateProfileString(strNode,"strTown", "",(LPSTR)szTemp,2047, strPath);
	gl_AmazonStr.strTown = szTemp;

	memset(szTemp,0,2048);
	::GetPrivateProfileString(strNode,"strRecver", "",(LPSTR)szTemp,2047, strPath);
	gl_AmazonStr.strRecver = szTemp;

	memset(szTemp,0,2048);
	::GetPrivateProfileString(strNode,"strAddr", "",(LPSTR)szTemp,2047, strPath);
	gl_AmazonStr.strAddr = szTemp;

	memset(szTemp,0,2048);
	::GetPrivateProfileString(strNode,"strPostCode", "",(LPSTR)szTemp,2047, strPath);
	gl_AmazonStr.strPostCode = szTemp;

	memset(szTemp,0,2048);
	::GetPrivateProfileString(strNode,"strPhone", "",(LPSTR)szTemp,2047, strPath);
	gl_AmazonStr.strPhone = szTemp;

	memset(szTemp,0,2048);
	::GetPrivateProfileString(strNode,"strBillHeader", "",(LPSTR)szTemp,2047, strPath);
	gl_AmazonStr.strBillHeader = szTemp;

	memset(szTemp,0,2048);
	::GetPrivateProfileString(strNode,"strBillContent", "",(LPSTR)szTemp,2047, strPath);
	gl_AmazonStr.strBillContent = szTemp;

	memset(szTemp,0,2048);
	::GetPrivateProfileString(strNode,"strOrderNo", "",(LPSTR)szTemp,2047, strPath);
	gl_AmazonStr.strOrderNo = szTemp;

	gl_AmazonStr.eSendType = (_SendType)::GetPrivateProfileInt(strNode,"eSendType",1, strPath);
	gl_AmazonStr.bBill = ::GetPrivateProfileInt(strNode,"bBill",0, strPath);
	gl_AmazonStr.iShowFlag = ::GetPrivateProfileInt(strNode,"iShowFlag",1, strPath);
	gl_AmazonStr.iDebug = ::GetPrivateProfileInt(strNode,"iDebug",1, strPath);
	gl_AmazonStr.iOpFlag = ::GetPrivateProfileInt(strNode,"iOpFlag",2, strPath);	//Ĭ���µ�
	gl_AmazonStr.eQueryType = (_QueryType)::GetPrivateProfileInt(strNode,"eQueryType",1, strPath);
}
// CAmazonRobotApp ����
//���ָ����վ��COOKIE
void ClearSiteCookie(CString strFlag)
{
	TCHAR szPath[MAX_PATH] = {0};
	if (SHGetSpecialFolderPath(NULL, szPath, CSIDL_COOKIES, FALSE))
	{//�õ�Ŀ¼�������
		char szFind[MAX_PATH]={0};
		WIN32_FIND_DATA FindFileData;
		strcpy(szFind,szPath);
		strcat(szFind,"\\*.txt");
		HANDLE hFind=::FindFirstFile(szFind,&FindFileData);
		if(INVALID_HANDLE_VALUE == hFind)    return;
		while(TRUE)
		{
			if (1)
			{
				CString strPath = "";
				strPath.Format("%s\\%s",szPath,FindFileData.cFileName);
				if ( strPath.Find(strFlag) >=0 )
				{
					CFile::Remove(strPath);
				}
			}
			if(!FindNextFile(hFind,&FindFileData))
			{
				break;
			}
		}
		FindClose(hFind);
	}
}
CAmazonRobotApp::CAmazonRobotApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CAmazonRobotApp ����

CAmazonRobotApp theApp;


// CAmazonRobotApp ��ʼ��

BOOL CAmazonRobotApp::InitInstance()
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

	//ͬһ̨����ͬʱֻ��ִ��һ������
	HANDLE hMutex = CreateMutex(NULL, FALSE, MUTEX_DEFINE);
	if(hMutex != NULL)
	{
		if(GetLastError() == ERROR_ALREADY_EXISTS)
		{
			ReleaseMutex(hMutex);
			CloseHandle(hMutex); 
			hMutex = NULL;
			exit(HAVE_ALREADY_RUN);
			return FALSE;
		}
	}
	else
	{
		return FALSE;
	}
	
	//��ȡ����
	CString strPath = AfxGetApp()->m_lpCmdLine;

 	strPath.Trim();
 	if (strPath != "")
 	{
		InitParams(strPath);
		gl_AmazonStr.iAutoRun = 1;
	}
	else
	{
		gl_AmazonStr.iShowFlag = 1;
		gl_AmazonStr.iAutoRun = 0;
		gl_AmazonStr.iDebug = 1;
// 		GetAppPath(strPath);
// 		InitParams(strPath + AMAZON_CONFIG_FILE);
// 		gl_AmazonStr.iAutoRun = 1;
	}
	//���COOKIE
	ClearSiteCookie("amazon");	
	CAmazonRobotDlg dlg;
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



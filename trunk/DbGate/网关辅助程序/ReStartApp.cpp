// ReStartApp.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "ReStartApp.h"
#include "ReStartAppDlg.h"
#include "UserFile.h"
#include <io.h>
#include <direct.h>
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CReStartAppApp

BEGIN_MESSAGE_MAP(CReStartAppApp, CWinApp)
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()


// CReStartAppApp ����

CReStartAppApp::CReStartAppApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CReStartAppApp ����

CReStartAppApp theApp;


// CReStartAppApp ��ʼ��

BOOL CReStartAppApp::InitInstance()
{
	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControls()�����򣬽��޷��������ڡ�
	CString strAppPath="";
	CUserFile *pFile = CUserFile::GetInstance();
	if ( pFile )
	{
		pFile->GetAppPath(strAppPath);
		if (strAppPath.GetLength() > 16)
		{
			strAppPath = strAppPath.Right(16);
		}
		strAppPath = pFile->StrEncode(strAppPath);
	}
	if (strAppPath == "")
	{
		return FALSE;
	}
	if (strAppPath.GetLength() > 32)
	{
		strAppPath = strAppPath.Right(32);
	}
	strAppPath +="help";
	// ����Ƿ��Ѿ�����Mutex
	// ����Ѿ�����������ֹ���̵�����
	m_hMutex = CreateMutex(NULL, TRUE, strAppPath);	
	if ((m_hMutex != NULL) && (GetLastError() == ERROR_ALREADY_EXISTS)) 
	{
		WriteLog("���������ظ�����",0);
		ReleaseMutex(m_hMutex);
		//MessageBox(NULL, "�ó����Ѿ���������,�벻Ҫ���ж��!", "���ؾ���", MB_OK);
		return FALSE;
	}

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
	SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));

	CReStartAppDlg dlg;
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
BOOL CReStartAppApp::ExitInstance()
{
	CUserFile *pFile = CUserFile::GetInstance();
	if ( pFile )
	{
		delete pFile;
		pFile = NULL;
	}
	return TRUE;
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
//д��־
void CReStartAppApp::WriteLog(CString srcStr,int iFlag)
{

	CTime currTime = CTime::GetCurrentTime();
	try
	{
		CString filepath = "";
		CString destStr = "";
		CString folder = "";
		CString filename = "";
		CString strAppPath = "";
		if (iFlag == 0)	//������־
		{
			folder = "restartlog";
			filename.Format("%s_restartlog.txt",currTime.Format("%Y%m%d"));
		}
		else
		{
			return;
		}
		CUserFile *pFile = CUserFile::GetInstance();
		if ( pFile )
		{
			pFile->GetAppPath(strAppPath);
		}
		CString folderName = "";
		folderName.Format("%s%s",strAppPath,folder);
		if(_access(folderName,0)!=0)
		{
			if(mkdir(folderName) != 0)//����Ŀ¼
			{
				return;
			}
		}					
		destStr.Format("%s	%s",currTime.Format("%Y-%m-%d %H:%M:%S"),srcStr);
		filepath.Format("%s\\%s",folderName,filename);
		FILE *fp;
		char szfile[1000]={0};
		sprintf(szfile,filepath);
		fp=fopen(szfile,"a+");
		fprintf(fp,"%s\r\n",destStr);
		fclose(fp);
	}
	catch (...)
	{
	}
}
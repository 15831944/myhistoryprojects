// 51jobpost.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "51jobpost.h"
#include "51jobpostDlg.h"
#include "RegDlg.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CjobpostApp

BEGIN_MESSAGE_MAP(CjobpostApp, CWinApp)
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()


// CjobpostApp ����

CjobpostApp::CjobpostApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CjobpostApp ����

CjobpostApp theApp;


// CjobpostApp ��ʼ��

BOOL CjobpostApp::InitInstance()
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
	SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));
	gl_strMac = GetMac();
	CString strAppPath = "";
	GetAppPath(strAppPath);
	strAppPath += "set.ini";
	char szTemp[2048] = {0};
	CString strNode = "set";
	::GetPrivateProfileString(strNode,"username", "",(LPSTR)szTemp,2047, strAppPath);
	CString strUser = szTemp;	
	memset(szTemp,0,2048);
	::GetPrivateProfileString(strNode,"password", "",(LPSTR)szTemp,2047, strAppPath);
	CString strPwd = szTemp;
	int iRet = 0;
	if (strUser != "" && strPwd != "")
	{
		//������֤
		iRet = Reg(strUser,strPwd,gl_strMac.c_str());
	}
	if (!iRet)
	{
		CRegDlg dlgReg;
		dlgReg.DoModal();
		iRet = dlgReg.m_iRegSuc;
		if (!iRet)
		{
			return FALSE;
		}
	}
	CjobpostDlg dlg;
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

int CjobpostApp::GetAppPath(CString &strAppPath)
{
	int iRet = -3;
	try
	{
		TCHAR szBuff[_MAX_PATH];
		VERIFY(::GetModuleFileName(AfxGetApp()->m_hInstance, szBuff, _MAX_PATH));
		strAppPath.Format("%s",szBuff);
		int pos=strAppPath.ReverseFind('\\');
		strAppPath=strAppPath.Left(pos+1);		
		iRet = 0;
	}
	catch (...)
	{

	}
	return iRet;	
}
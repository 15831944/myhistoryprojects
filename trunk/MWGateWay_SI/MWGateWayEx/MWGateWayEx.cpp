// MWGateWayEx.cpp : ���� DLL �ĳ�ʼ�����̡�
//

#include "stdafx.h"
#include <afxdllx.h>
#include "ADOFile/AdoRecordSet.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
CRITICAL_SECTION	g_csSend;
CRITICAL_SECTION	g_csRecv;
CRITICAL_SECTION	g_csLog;
char g_szpath[300] = {0};
char g_szdbfile[100] = {0};
char g_szdbpwd[100] = {0};
static AFX_EXTENSION_MODULE MWGateWayExDLL = { NULL, NULL };

extern "C" int APIENTRY
DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	// ���ʹ�� lpReserved���뽫���Ƴ�
	UNREFERENCED_PARAMETER(lpReserved);

	if (dwReason == DLL_PROCESS_ATTACH)
	{
		TRACE0("MWGateWayEx.DLL ���ڳ�ʼ����\n");
		
		// ��չ DLL һ���Գ�ʼ��
		if (!AfxInitExtensionModule(MWGateWayExDLL, hInstance))
			return 0;
		::CoInitialize(NULL);
		// Extension DLL one-time initialization
		char lpszPath[512]={0};
		char temppath[512]={0};
		char *ptr;
		//GetModuleFileName(AfxGetInstanceHandle(),lpszPath,512);
		GetModuleFileName(hInstance,lpszPath,512);
		ptr = lpszPath + strlen(lpszPath);
		while(*ptr != '\\' && *ptr != '/')
		{
			ptr--;
		}
		*ptr = '\0';
		strcpy(g_szpath,lpszPath);
		sprintf(temppath,"%s\\SMEIDll.ini",g_szpath);
		GetPrivateProfileString("database","path","upreport.mdb",g_szdbfile,100,temppath);
		GetPrivateProfileString("database","pwd","",g_szdbpwd,100,temppath);
		InitializeCriticalSection( &g_csSend );
		InitializeCriticalSection( &g_csRecv );
		InitializeCriticalSection( &g_csLog );
		// ���� DLL ���뵽��Դ����
		// ע��: �������չ DLL ��
		//  MFC ���� DLL(�� ActiveX �ؼ�)��ʽ���ӵ�
		//  �������� MFC Ӧ�ó������ӵ�������Ҫ
		//  �����д� DllMain ���Ƴ������������һ��
		//  �Ӵ���չ DLL �����ĵ����ĺ����С�ʹ�ô���չ DLL ��
		//  ���� DLL Ȼ��Ӧ��ʽ
		//  ���øú����Գ�ʼ������չ DLL������
		//  CDynLinkLibrary ���󲻻ḽ�ӵ�
		//  ���� DLL ����Դ���������������ص�
		//  ���⡣

		new CDynLinkLibrary(MWGateWayExDLL);

	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		TRACE0("MWGateWayEx.DLL ������ֹ��\n");
		DeleteCriticalSection( &g_csSend );
		DeleteCriticalSection( &g_csRecv );
		DeleteCriticalSection( &g_csLog );
		::CoUninitialize();
		// �ڵ�����������֮ǰ��ֹ�ÿ�
		AfxTermExtensionModule(MWGateWayExDLL);
	}
	return 1;   // ȷ��
}


void WriteLogMsg(CString strMsg)
{
	FILE *fp =NULL;
	char szfilepath[300] = {0};
	sprintf(szfilepath,"%s\\accessdebug.log",g_szpath);
	EnterCriticalSection( &g_csLog );
	fp = fopen(szfilepath,"a+");
	fprintf(fp,"%s	%s\n",CTime::GetCurrentTime().Format("%Y-%m-%d %H:%M:%S"),strMsg);
	fclose(fp);
	LeaveCriticalSection( &g_csLog );
}

int __stdcall WriteToDatabase(char *szReport,int iCount)
{
	CString strSQL = "";
	CString strShow = "";
	//CString strSQL1 = "";
	int iTrrTimes = 0;
	int iRet = 0;
	strSQL.Format("insert into [reportrecord] ([icount],[content])values(%d,'%s')",iCount,szReport);
	//strSQL1.Format("delete from [reportrecord] where [content]='%s'",szReport);
	CAdoConnection adoConn;	
	try
	{	
		if (!adoConn.ConnectAccess(CString(g_szdbfile),CString(g_szdbpwd)))
		{
			strShow.Format("��������������ݿ�ʧ��:%s,%s",g_szdbfile,g_szdbpwd);
			WriteLogMsg(strShow);
			strShow.Format("%d  %s",iCount,szReport);
			WriteLogMsg(strShow);
			return -1;
		}
		while( iTrrTimes++<2 )
		{
			//adoConn.Execute(strSQL1);
			if(FALSE == adoConn.Execute(strSQL))
			{
				iTrrTimes++;
				WriteLogMsg(adoConn.GetLastErrorText());
				strShow.Format("%d  %s",iCount,szReport);
				WriteLogMsg(strShow);
			}
			else
			{
				break;
			}
		}
		adoConn.Close();
	}
	catch (_com_error e)
	{
		WriteLogMsg("����״̬�����쳣");
		strShow.Format("%d  %s",iCount,szReport);
		WriteLogMsg(strShow);
	}
	catch (...)
	{
		WriteLogMsg("����״̬�����쳣");
		strShow.Format("%d  %s",iCount,szReport);
		WriteLogMsg(strShow);
	}	
	return iRet;
}

int __stdcall ReadFromDatabase(char *szReport,int *iCount)
{
	int iTrrTimes = 0;
	int iRet = 0;
	CString strReport = "";
	CString strShow = "";
	int iID = 0;
	CString strSQL = "select top 1 [ID],[icount],[content] from [reportrecord]";	
	CAdoConnection adoConn;
	CAdoRecordSet adoRst;
	*iCount = 0;
	int iRptCount = 0;
	try
	{	
		if (!adoConn.ConnectAccess(CString(g_szdbfile),CString(g_szdbpwd)))
		{
			strShow.Format("��ȡ�����������ݿ�ʧ��:%s,%s",g_szdbfile,g_szdbpwd);
			WriteLogMsg(strShow);
			return -1;
		}
		adoRst.SetAdoConnection(&adoConn);
		adoRst.SetCursorLocation(adUseClient);
		if (!adoRst.Open(strSQL))
		{
			WriteLogMsg("��ȡ״̬����ʧ��"+adoConn.GetLastErrorText());
			return -1;
		}
		if (!adoRst.IsEOF())
		{
			adoRst.GetCollect("ID",iID);
			adoRst.GetCollect("icount",iRptCount);
			*iCount = iRptCount;
			adoRst.GetCollect("content",strReport);
			strcpy(szReport,strReport);
			iRet = 1;
		}
		adoRst.Close();
		if (iID != 0 )
		{
			strSQL.Format("delete from [reportrecord] where [ID] = %d",iID);
			while( iTrrTimes++<2 )
			{
				if(FALSE == adoConn.Execute(strSQL))
				{
					iTrrTimes++;
					WriteLogMsg(adoConn.GetLastErrorText());
				}
				else
				{
					break;
				}
			}
		}		
		adoConn.Close();
	}
	catch (_com_error e)
	{
		WriteLogMsg("��ȡ״̬�����쳣");
		iRet = -99;
	}
	catch (...)
	{
		iRet = -99;
		WriteLogMsg("��ȡ״̬�����쳣");
	}	
	return iRet;
}

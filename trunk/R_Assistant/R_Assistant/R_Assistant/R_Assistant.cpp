// R_Assistant.cpp : Defines the entry point for the DLL application.
//
#include <string>
#include <windows.h>
#include <time.h>
#include <io.h>
#define  TESTTESTSHOW 
using namespace std;
char g_szExePath[256] = {0};
char g_szPath[256] = {0};

int g_iStartTimeOut = 0;
int g_iCheckTimeOut = 0;
int g_iCheckFileTimeOut = 0;
int g_iLogFlag = 0;
CRITICAL_SECTION g_Section;
CRITICAL_SECTION g_SectionClip;
void WriteLog(const char *szInfo);
BOOL StartProcess(PROCESS_INFORMATION &pi,STARTUPINFO &si);
HWND GetChildWindow(HWND hwnd,int iIndex);
BOOL CopyStringToClipboard(HWND hWnd, LPCTSTR lpszText);

BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
	if (ul_reason_for_call == DLL_PROCESS_ATTACH)
	{
		char szIniPath[256] ={0};
		::GetModuleFileName((HMODULE)hModule,g_szPath,256);
		char *p = strrchr(g_szPath,'\\');
		*(p+1) = '\0';

		sprintf(szIniPath,"%sAssistantSet.ini",g_szPath);
		::GetPrivateProfileString("config","path", "",(LPSTR)g_szExePath,256, szIniPath);
		g_iStartTimeOut = ::GetPrivateProfileInt("config","starttimeout",0, szIniPath);
		g_iCheckTimeOut = ::GetPrivateProfileInt("config","checktimeout",0, szIniPath);
		g_iCheckFileTimeOut = ::GetPrivateProfileInt("config","chekfiletimeout",0, szIniPath);
		g_iLogFlag = ::GetPrivateProfileInt("config","log",0, szIniPath);
		if (g_iStartTimeOut < 20) g_iStartTimeOut = 20;
		if (g_iCheckTimeOut < 20) g_iCheckTimeOut = 20;
		if (g_iCheckFileTimeOut < 20)  g_iCheckFileTimeOut = 20;
		if (g_iStartTimeOut > 1000) g_iStartTimeOut = 1000;
		if (g_iCheckTimeOut > 5000) g_iCheckTimeOut = 5000;
		if (g_iCheckFileTimeOut > 1000)  g_iCheckFileTimeOut = 1000;
		if (strlen(g_szExePath) < 1)
		{
			return FALSE;
		}
		InitializeCriticalSection(&g_Section);
		InitializeCriticalSection(&g_SectionClip);
	}
	else if (DLL_PROCESS_DETACH == ul_reason_for_call)
	{
		DeleteCriticalSection(&g_Section);
		DeleteCriticalSection(&g_SectionClip);
	}
	return TRUE;
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
//ö��������ṹ��
struct   EnumParam
{
	HWND hMainWnd;
	DWORD dwProcessID;
};

BOOL CALLBACK EnumWinProc(HWND hwnd, LPARAM lParam)
{
	DWORD dwID;
	EnumParam* pep = (EnumParam*)lParam;
	GetWindowThreadProcessId(hwnd, &dwID);
	if (dwID == pep->dwProcessID)
	{
		pep->hMainWnd = hwnd;
		char strClassName[256] = {0};
		GetClassName(hwnd,strClassName,sizeof(strClassName));
		if(strcmp(strClassName,"Rgui Workspace")==0)
		return FALSE;
	}
	return  TRUE;
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
//���⺯��
int __stdcall Calculate(const char *szPath)
{
	//�ж��ļ��Ƿ����
	if(_access(szPath,0)!=0)
	{
		return -2;
	}

	PROCESS_INFORMATION    pi; 
	STARTUPINFO            si; 
	EnumParam ep;
	char szInfo[800] = {0};
	char szPathEx[256] = {0};
	DWORD dRet = 0;
	int iRet = -1;
	HWND hwnd = NULL;
	HWND hwndPaste = NULL;
	HWND hwndClient = NULL;
	char szPathExEx[256] = {0};	
	/************************************************************************/
	char szRpath[256] ={0};
	strcpy(szRpath,szPath);
	char *p = strrchr(szRpath,'\\');
	*(p+1) = '\0';
	char szRfilePath[256] = {0};
	sprintf(szRfilePath,"%s%s",szRpath,"gr.csv");
	const int iDataArraylen = 4;
	char szDataFileArray[iDataArraylen][250] = {"forecasts.csv","elasticity.csv","gr.csv","report.csv"};
	char szRet[iDataArraylen] = {0};
	int i=0;
	for(i=0;i<iDataArraylen;i++)
	{
		sprintf(szRfilePath,"%s%s",szRpath,szDataFileArray[i]);
		//ɾ��ԭ�������ļ�
		if (_access(szRfilePath,0)==0)
		{
			try
			{
				BOOL bRet = DeleteFile(szRfilePath);
				if (!bRet)
				{
					sprintf(szInfo,"delete file failed[%d]:%s",GetLastError(),szRfilePath);
				}
				else
				{
					sprintf(szInfo,"delete file succuss:%s",szRfilePath);
				}			
				WriteLog(szInfo);
			}
			catch (...)
			{
				return -5;
			}			
		}
	}
	
	//ƴ��R������Ҫ�������ַ���
	int j = 0;
	while (szPath[i]!='\0')
	{
		if (szPath[i] !='\\')
		{
			szPathExEx[j]=szPath[i];
			i++;
			j++;
		}
		else
		{
			szPathExEx[j]='\\';
			szPathExEx[j+1]='\\';
			i++;
			j+=2;
		}
	}
	sprintf(szPathEx,"source(\"%s\")\r\n",szPathExEx);
	//��������
	BOOL bRet = StartProcess(pi,si);
	if (!bRet)	//��������ʧ��
	{
		dRet = GetLastError();
		sprintf(szInfo,"StartProcess Failed :%d",(int)dRet);
		WriteLog(szInfo);
		return -3;
	}
	//��ʼ���Ŀ������Ƿ���������
	DWORD dwExitCode;
	DWORD dwTimeStart = ::GetTickCount();
	while(true)
	{
		//��������ʱ���˳�
		if (::GetTickCount()-dwTimeStart >g_iStartTimeOut * 1000)
		{
			sprintf(szInfo,"StartProcess Failed TimeOut:%d",(int)g_iStartTimeOut);
			WriteLog(szInfo);
			if (pi.hProcess)
			{
				bRet = TerminateProcess(pi.hProcess,0);
				if (!bRet)
				{
					dRet = GetLastError();
					sprintf(szInfo,"StartProcess TerminateProcess Failed :%d",(int)dRet);
					WriteLog(szInfo);
				}
			}
			iRet = -4;
			return iRet;
		}
		
		if(GetExitCodeProcess(pi.hProcess, &dwExitCode))//���״̬
		{
			if (dwExitCode == STILL_ACTIVE)
			{
				//������������
				ep.hMainWnd = NULL;
				ep.dwProcessID = pi.dwProcessId;
				EnumWindows((WNDENUMPROC)EnumWinProc, (long)&ep);
				if (ep.hMainWnd != NULL)
				{
					//����Ŀ�괰��
#ifdef TESTTESTSHOW
					int iCount = 0;
					while (true)
					{
						iCount++;
						if (iCount>10)
						{
							break;
						}
						if (::ShowWindow(ep.hMainWnd,SW_HIDE))
						{
							break;
						}
						Sleep(100);
					}
					
#endif
					
					//�����Ӵ���
					hwnd = NULL;
					hwnd = GetChildWindow(ep.hMainWnd,1);					
					hwndPaste = GetChildWindow(ep.hMainWnd,2);
					if (hwndPaste)
					{
						hwndPaste = GetChildWindow(hwndPaste,5);
						if (!hwndPaste)
						{
							continue;
						}
						else
						{
							sprintf(szInfo,"Find Paste Button HWND :%d",(int)hwndPaste);
							WriteLog(szInfo);
						}
					}
					else
					{
						continue;
					}
					if (hwnd)
					{
						hwndClient = hwnd;
						hwnd = GetChildWindow(hwnd,1);
					}
					if (hwnd)
					{
						char strClassName[256] = {0};
						char strWindowName[256] = {0};
						GetClassName(hwnd,strClassName,sizeof(strClassName));
						::GetWindowText(hwnd,(LPTSTR)&strWindowName,256);
						if (strcmp(strClassName,"Rgui Document")==0 &&strcmp(strWindowName,"R Console")==0)
						{
							sprintf(szInfo,"Find Rgui Document HWND :%d",(int)hwnd);
							WriteLog(szInfo);
							break;
						}
					}
					
				}				
			}
		}
		else
		{
			dRet = GetLastError();
			sprintf(szInfo,"GetExitCodeProcess Failed :%d",(int)dRet);
			WriteLog(szInfo);
		}
		Sleep(500);
	}
	
	//��д��Դ�ļ��ַ��������н���
	EnterCriticalSection(&g_SectionClip);
	CopyStringToClipboard(NULL,szPathEx);	
	::PostMessage(hwndPaste,WM_LBUTTONDOWN,0,0);
	Sleep(10);
	::PostMessage(hwndPaste,WM_LBUTTONUP,0,0);
	Sleep(10);
	LeaveCriticalSection(&g_SectionClip);
	//���ڽ�������ʼ����Ƿ��Ѿ��������
	HWND hwndResult = NULL;
	bool bIsSuccess = false;
	dwTimeStart = ::GetTickCount();
	/*
	while (!bIsSuccess)
	{
		//R���������㳬ʱ���˳�
		if (::GetTickCount()-dwTimeStart >g_iCheckTimeOut * 1000)
		{
			sprintf(szInfo,"CheckCalculate Failed TimeOut:%d",(int)g_iCheckTimeOut);
			WriteLog(szInfo);
			if (pi.hProcess)
			{
				bRet = TerminateProcess(pi.hProcess,0);
				if (!bRet)
				{
					dRet = GetLastError();
					sprintf(szInfo,"CheckCalculate TerminateProcess Failed :%d",(int)dRet);
					WriteLog(szInfo);
				}
			}
			iRet = -5;
			return iRet;
		}
		Sleep(100);
		if (!hwndClient)
		{
			break;
		}
		//ѭ�������Ӵ��ڣ����ҽ�������Ƿ����
		for (int i=1;i<3;i++)
		{
			hwndResult = GetChildWindow(hwndClient,i);
			if (!hwndResult)
			{
				break;
			}
			char strClassName[256] = {0};
			char strWindowName[256] = {0};
			GetClassName(hwndResult,strClassName,sizeof(strClassName));
			::GetWindowText(hwndResult,(LPTSTR)&strWindowName,256);
			if (strcmp(strClassName,"Rgui Document")==0 &&strcmp(strWindowName,"R Graphics: Device 2 (ACTIVE)")==0)
			{
				sprintf(szInfo,"Find Rgui Graphics HWND :%d,Caculate Success",(int)hwnd);
				WriteLog(szInfo);
				bIsSuccess = true;
				break;
			}
		}		
	}
*/
	//�ɹ���
	bIsSuccess = true;
	iRet = bIsSuccess==true?0:-1;
	//�˴��ж������ļ��Ƿ��Ѿ�����
	dwTimeStart = ::GetTickCount();
	int iFileGenCount = 0;
	memset(szRet,0,iDataArraylen);
	while(true)
	{	
		if (::GetTickCount()-dwTimeStart >g_iCheckFileTimeOut * 1000)
		{
			sprintf(szInfo,"Check Data File Failed TimeOut:%d",(int)g_iCheckFileTimeOut);
			WriteLog(szInfo);
			iRet = -6;
			break;
		}
		for(int i=0;i<iDataArraylen;i++)
		{
			sprintf(szRfilePath,"%s%s",szRpath,szDataFileArray[i]);
			if(szRet[i] == 1)
			{
				continue;
			}
			//��������ļ�
			if (_access(szRfilePath,0)==0)
			{
				sprintf(szInfo,"Check %s File Success",szDataFileArray[i]);
				WriteLog(szInfo);
				szRet[i] = 1;
				iFileGenCount++;
			}
		}
		if(iFileGenCount >= iDataArraylen)
		{
			iRet = 0;
			break;
		}
		Sleep(100);
	}
	
#ifdef TESTTESTSHOW
	//����Ŀ�����
	if (pi.hProcess)
	{
		bRet = FALSE;
		int iCount = 0;
		while (!bRet)
		{
			if (iCount>5)
			{
				break;
			}
			bRet = TerminateProcess(pi.hProcess,0);
			if (!bRet)
			{
				dRet = GetLastError();
				sprintf(szInfo,"TerminateProcess Failed :%d",(int)dRet);
				WriteLog(szInfo);
				iCount++;
			}
			else
			{
				break;
			}
		}		
	}	
#endif	
	//���ؽ��
	return iRet;
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
HWND GetChildWindow(HWND hwnd,int iIndex)
{
	HWND hwndChind = NULL;
	int i = 1;
	hwndChind = GetWindow(hwnd,GW_CHILD);
	while (hwndChind)
	{	
		if (i == iIndex)
		{
			return hwndChind;
		}
		hwndChind = ::GetNextWindow(hwndChind,GW_HWNDNEXT);
		i++;
	}
	return hwndChind;
}
BOOL StartProcess(PROCESS_INFORMATION &pi,STARTUPINFO &si)
{
	ZeroMemory(&si, sizeof(si));
	si.cb =  sizeof(STARTUPINFO); 
	//GetStartupInfo(&si);
	si.dwFlags =STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;;
	si.wShowWindow = SW_HIDE;
// 	si.dwX = -10000;
// 	si.dwY = -10000;
// 	si.dwXSize = 0;
// 	si.dwYSize = 0;
	//return CreateProcess(g_szExePath,NULL,NULL,NULL,false,NORMAL_PRIORITY_CLASS, NULL,NULL,&si,&pi); 	
	return CreateProcess(NULL,g_szExePath,NULL,NULL,TRUE,CREATE_NO_WINDOW, NULL,NULL,&si,&pi); 
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
BOOL CopyStringToClipboard(HWND hWnd, LPCTSTR lpszText)
{
	// ��szText�е����ݸ��Ƶ�������
	char szInfo[800] ={0};
	DWORD dRet = 0;	
	HGLOBAL hglbCopy;
	LPTSTR  lptstrCopy;
	int nlen = strlen(lpszText);
	if (nlen == 0)
	{
		return FALSE;
	}
	if (!::OpenClipboard(hWnd))
	{
		dRet = GetLastError();
		sprintf(szInfo,"OpenClipboard Failed :%d",(int)dRet);
		WriteLog(szInfo);
		return FALSE;
	}
	BOOL bRet = EmptyClipboard();	
	if (!bRet)
	{
		dRet = GetLastError();		
		sprintf(szInfo,"EmptyClipboard Failed :%d",(int)dRet);
		WriteLog(szInfo);
		return FALSE;
	}

	hglbCopy = GlobalAlloc(GMEM_DDESHARE, 
		(nlen + 1) * sizeof(char)); 

	if (hglbCopy == NULL) 
	{ 
		dRet = GetLastError();
		sprintf(szInfo,"GlobalAlloc Failed :%d",(int)dRet);
		WriteLog(szInfo);
		CloseClipboard(); 
		return FALSE; 
	}
	lptstrCopy = (LPTSTR)GlobalLock(hglbCopy); 
	memcpy(lptstrCopy, lpszText, nlen);
	lptstrCopy[nlen] = (TCHAR)0;  //null character
	GlobalUnlock(lptstrCopy);
	SetClipboardData(CF_TEXT, hglbCopy);
	CloseClipboard();
	return TRUE;
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
//д��־
void WriteLog(const char * szInfo)
{
	if (g_iLogFlag == 0)
	{
		return;
	}
	//EnterCriticalSection(&g_Section);
	try
	{
		char szPath[256] = {0};
		char szInfoEx[1000] = {0};
		time_t t = time(NULL);
		string strTime = ctime(&t);
		strTime = strTime.substr(0, strTime.size() - 1);
		sprintf(szInfoEx,"%s\t%s",strTime.c_str(),szInfo);		
		sprintf(szPath,"%sAssistantlog_%d.log",g_szPath,::GetCurrentThreadId());
		FILE *fp;
		fp=fopen(szPath,"a+");
		fprintf(fp,"%s\r\n",szInfoEx);
		fclose(fp);
	}
	catch (...)
	{
	}
	//LeaveCriticalSection(&g_Section);
}
//���������������д���ֽ����뷢����ѧϰ����֮��
//������ķ�������GPLЭ�飬�����ڴ�Э��Ĺ涨��ʹ�ñ�����
//�������ʹ�ñ������;����ʲô���ʻ��������http://digua254909662.blog.163.com/blog/static/74805072200922323951508/edit/
//������ϵ���ߣ�qq<254909662> ��˵�����⣬���˷ǳ�������������

#include "StdAfx.h"
#include "DialManage.h"

typedef int (__stdcall* LPENUMENTRIES)(RASENTRYNAMEA *, int *);
typedef int (__stdcall* LPGETPROPERTY)(char *);
typedef int (__stdcall* LPGETDIALPARAMS)(RASDIALPARAMS *);
typedef int (__stdcall* LPRASDIALUP)(LPRASDIALFUNC, RASDIALPARAMS *, HRASCONN *);
typedef int (__stdcall* LPGETERRORMESSAGE)(UINT, LPSTR, DWORD );
typedef int (__stdcall* LPRESETDIALCONNECT)(HRASCONN *);
typedef int (__stdcall* LPISCONNECTED)(char *);

HRASCONN hConn;

DialManage::DialManage(void)
{
	::RtlZeroMemory(&dialInfo, sizeof(dialInfo) );
	bExistAdsl = FALSE;
	myFindConnectType = FIND_PPPOE;
}

DialManage::~DialManage(void)
{
}

//--------------------------------------
//return 1 denote success 100 denote there is no device type of you find's negative denote a error occur
int DialManage::FindDefaultAdslConnect()
{
	RASENTRYNAMEA * pEntries = new RASENTRYNAMEA;
	int num = -1;

	HINSTANCE hLib = LoadLibrary("DefGetProperty.dll");
	if (NULL == hLib) return -1;
#pragma region //ö�������������
	LPENUMENTRIES lpEnumEntry; 
	lpEnumEntry = (LPENUMENTRIES) GetProcAddress(hLib, "EnumEntries");   
	if (NULL == lpEnumEntry)   
	{   
		::FreeLibrary(hLib);   
		return -2;   
	}  
	int result = lpEnumEntry(pEntries, &num);
	if(result != 1)
	{
		::FreeLibrary(hLib); 
		return -3;
	}
	//result == 1
	//ASSERT( num > 0 );
	if (num <=0)
	{
		AfxMessageBox("û�п��õ�ADSL����");
		return -4;
	}
	result = pEntries ->dwSize;
	delete pEntries;
	pEntries = new RASENTRYNAMEA[num];
	if( NULL == pEntries)
	{
		::FreeLibrary(hLib); 
		return -4;
	}
	result = lpEnumEntry(pEntries, &num);
	if( result != 1 )
	{
		::FreeLibrary(hLib); 
		return -3;
	}
	ASSERT(num >0);
#pragma endregion

#pragma region //Ѱ�ҵ�һ��adsl����
	LPGETPROPERTY lpGetProperty; 
	lpGetProperty = (LPGETPROPERTY) GetProcAddress(hLib, "GetEntryProperty");   
	if (NULL == lpGetProperty)   
	{   
		::FreeLibrary(hLib);   
		return -12;   
	}  
	for(int i=0; i < num; i++)
	{
		result = lpGetProperty( pEntries[i].szEntryName);
		if( myFindConnectType == result)//we find the device which we are find and follow me to get the params
		{
			::RtlZeroMemory(&dialInfo, sizeof(EntryNameInfo) );
			lstrcpy(dialInfo.szEntryName, pEntries[i].szEntryName);
			bExistAdsl = true;
			LPGETDIALPARAMS lpGetDialParams;
			lpGetDialParams = (LPGETDIALPARAMS) GetProcAddress(hLib, "GetEntryDialParams");
			if (NULL == lpGetDialParams)
			{
				::FreeLibrary(hLib);
				return -22;
			}
			result = lpGetDialParams(&dialInfo);
			if( result != 1 )
			{
				AfxMessageBox(_T("û�ҵ����ӵĲ���"));
			}
			//AfxMessageBox(dialInfo.szPassword);
			::FreeLibrary(hLib);
			return 1;
		}
	}
#pragma endregion
	::FreeLibrary(hLib);
	return 100;
}

void DialManage::GetDialInfo(EntryNameInfo *lpDialInfo)
{
	if( NULL != lpDialInfo)
	{
		lstrcpy(lpDialInfo ->szEntryName, dialInfo.szEntryName);
		lstrcpy(lpDialInfo ->szUserName,  dialInfo.szUserName);
		lstrcpy(lpDialInfo ->szPassword,  dialInfo.szPassword);
	}
}

void DialManage::SetDialInfo(EntryNameInfo *lpDialInfo)
{
	if(NULL != lpDialInfo)
	{
		lstrcpy(dialInfo.szPassword, lpDialInfo ->szPassword);
		lstrcpy(dialInfo.szUserName, lpDialInfo ->szUserName);
	}
}

int DialManage::CmdDialConnect(HANDLE &hReadHanle)
{ 
	if( !bExistAdsl )
	{
		return 100;
	}
	HANDLE   hWritePipe;  
	SECURITY_ATTRIBUTES   sat;  
	STARTUPINFO   startupinfo;  
	PROCESS_INFORMATION   pinfo; 
	CHAR buffer[1024];
	CString dialCommand;

	sat.nLength=sizeof(SECURITY_ATTRIBUTES);  
	sat.bInheritHandle=true;  
	sat.lpSecurityDescriptor=NULL;  
	if(!CreatePipe(&hReadHanle,&hWritePipe,&sat,NULL))  
	{    
		return -1;  
	}  
	dialCommand.Format("rasdial %s %s %s", dialInfo.szEntryName, dialInfo.szUserName, dialInfo.szEntryName);

	::ZeroMemory(&startupinfo, sizeof(STARTUPINFO));
	startupinfo.cb=sizeof(STARTUPINFO);  
	GetStartupInfo(&startupinfo);  
	startupinfo.hStdError=hWritePipe;  
	startupinfo.hStdOutput=hWritePipe;  
	startupinfo.dwFlags=STARTF_USESHOWWINDOW   |   STARTF_USESTDHANDLES;  
	startupinfo.wShowWindow=SW_HIDE;  
	if(!CreateProcess(NULL, dialCommand.GetBuffer(), NULL,   NULL,   TRUE,   NULL,   NULL,   NULL,&startupinfo,&pinfo))  
	{  
		return -1;  
	}  
	CloseHandle(hWritePipe);
	return 1;
}

int DialManage::RasDialConnect(LPRASDIALFUNC RasDialFunc)
{
	if( !bExistAdsl )
	{
		return 100;
	}
	LPRASDIALUP lpRasDialUp;
	HINSTANCE hLib = LoadLibrary("DefGetProperty.dll");
	if (NULL == hLib) return -1;

	LPISCONNECTED lpIsConnected;
	lpIsConnected = (LPISCONNECTED) GetProcAddress(hLib, "IsConnected");   
	if (NULL == lpIsConnected)   
	{   
		::FreeLibrary(hLib);   
		return -2;   
	} 
	int iRet = lpIsConnected(dialInfo.szEntryName);
	if( 1 == iRet)
	{
		return 99;
	}

	iRet = RasDialReset();
	if( 1 == iRet)
	{
	}

	hConn = NULL;
	lpRasDialUp = (LPRASDIALUP) GetProcAddress(hLib, "SurrogateDialUP");   
	if (NULL == lpRasDialUp)   
	{   
		::FreeLibrary(hLib);   
		return -2;   
	} 
	iRet = lpRasDialUp(RasDialFunc, &dialInfo, &hConn);
	/*CString cRet;
	cRet.Format(_T("lpRasDialup is:%d"), iRet);
	AfxMessageBox(cRet);*/
	if( 1 == iRet)
	{
		return 1;
	}
	if( iRet > 1 )
	{
		return iRet;
	}
	return -3;
}

int DialManage::RasDialReset()
{
	LPRESETDIALCONNECT lpRasResetConnect;
	HINSTANCE hLib = LoadLibrary("DefGetProperty.dll");
	if (NULL == hLib) return -1;
	lpRasResetConnect = (LPRESETDIALCONNECT) GetProcAddress(hLib, "ResetDialConnect");   
	if (NULL == lpRasResetConnect)   
	{   
		::FreeLibrary(hLib);   
		return -2;   
	}
	ASSERT((&hConn) != NULL);
	int iRet = lpRasResetConnect(&hConn);
	if( 1 == iRet)
	{
		return 1;
	}
	return -3;
}

int DialManage::GetErrorMessage(UINT dwError, LPSTR buffer, DWORD dwSize)
{
	LPGETERRORMESSAGE lpGetErrorMessage;
	HINSTANCE hLib = LoadLibrary("DefGetProperty.dll");
	if (NULL == hLib) return -1;
	lpGetErrorMessage = (LPGETERRORMESSAGE) GetProcAddress(hLib, "GetRasErrorString");   
	if (NULL == lpGetErrorMessage)   
	{   
		::FreeLibrary(hLib);   
		return -2;   
	} 
	int iRet = lpGetErrorMessage(dwError, buffer, dwSize);
	if( iRet != 1)
	{
		return -3;
	}
	return 1;
}
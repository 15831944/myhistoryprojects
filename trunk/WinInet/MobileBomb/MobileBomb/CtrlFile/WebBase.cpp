#include "WebBase.h"
#include "log.h"


log g_objLog;
CWebBase::CWebBase(int nTimeOut)
{
	m_strErrorInfo.clear();
	m_strContent.clear();
	m_strServer.clear();
	m_nTimeOutSecond = 0;
	m_nHttpType = 1;	
	m_nPort = 80;
	m_nTimeOutSecond = nTimeOut;
	m_bNoCookie = false;
	m_hInterSessin = InternetOpen("YuTone_Http_Post", 
		INTERNET_OPEN_TYPE_DIRECT, 
		NULL, 
		NULL, 
		INTERNET_FLAG_NO_CACHE_WRITE);
	if ( NULL == m_hInterSessin )
	{
		SetLastErrorInfo("InternetOpen Error:%d", GetLastError());
	}
	else
	{
		SetTimeOut();
	}
	g_objLog.SetLogPath("c:\\log.txt");
}

CWebBase::~CWebBase(void)
{
	InternetCloseHandle(m_hInterSessin);
}
/*
int CWebBase::DownLoadFile(const char* lpszUrl,const char* lpszSavePath)
{
	return 0;
}
*/
HINTERNET CWebBase::GetHttpConnectionEx(const char * lpszServer,int nPort)
{
	if (lpszServer == NULL)
	{
		lpszServer = m_strServer.c_str();
	}
	if (nPort == 0)
	{
		nPort = m_nPort;
	}
	SetProxy();
	return InternetConnect(m_hInterSessin,
		lpszServer,
		nPort,
		NULL,
		NULL,
		INTERNET_SERVICE_HTTP, 
		INTERNET_FLAG_NO_CACHE_WRITE,
		NULL);
}

// void CWebBase::CloseHttpConnection(HINTERNET &hConnect)
// {
// 	try
// 	{
// 		if (hConnect)
// 		{
// 			InternetCloseHandle(hConnect);
// 		}
// 	}
// 	catch (...)
// 	{
// 	}
// }
/************************************************************************/
/* ����������WEB������                                                  */
/************************************************************************/
BOOL CWebBase::SendRequestEx(HINTERNET hRequest,const char* lpszHeader,const char* lpszPostData,bool bPostFlag)
{
	BOOL bRet = FALSE;
	char *pData = NULL;
	if (bPostFlag)
	{
		pData = (char *)lpszPostData;
	}
	return HttpSendRequest(hRequest,lpszHeader,strlen(lpszHeader),pData,pData == NULL?0:strlen(pData));
}
/************************************************************************/
/* HTTPͨ��                                                             */
/************************************************************************/
int CWebBase::GetData(const char* lpszObject,const char* lpszHeader,const char* lpszServer,int nPort,const char * lpszVerb,const char* lpszData,bool bIsFile,const char* szFilePath)
{
	time_t t = time(NULL);
	//��ȡHTTP����
	HINTERNET hConnect = GetHttpConnectionEx(lpszServer,nPort);
	if (hConnect == NULL)
	{
		SetLastErrorInfo("InternetConnect Error:%d,TimeSpan:%d", GetLastError(),time(NULL)-t);
		return WM_CONNECT_FAILED;
	}
	DWORD dwFlag = GenFlags();
	t = time(NULL);
	//����HTTP����
	HINTERNET hRequest = HttpOpenRequest(hConnect,lpszVerb,lpszObject,NULL,NULL,NULL,dwFlag,NULL);
	if (NULL == hRequest)
	{		
		SetLastErrorInfo("HttpOpenRequest %s Error:%d,TimeSpan:%d",lpszVerb, GetLastError(),time(NULL)-t);
		InternetCloseHandle(hConnect);
		return WM_OPENREQUEST_FAILED;
	}
	t = time(NULL);
	DWORD dwFlagsEx;
	DWORD dwBuffLen = sizeof(hRequest);
	InternetQueryOptionA(hRequest, INTERNET_OPTION_SECURITY_FLAGS, (LPVOID)&dwFlagsEx, &dwBuffLen);
	dwFlagsEx |= SECURITY_IGNORE_ERROR_MASK;
	InternetSetOptionA(hRequest, INTERNET_OPTION_SECURITY_FLAGS, &dwFlagsEx, sizeof(dwFlagsEx) );
	BOOL bRet = SendRequestEx(hRequest,lpszHeader,lpszData,lpszData!=NULL);
	if (!bRet)
	{
		SetLastErrorInfo("HttpSendRequest Error:%d,TimeSpan:%d", GetLastError(),time(NULL)-t);
		InternetCloseHandle(hRequest);
		InternetCloseHandle(hConnect);
		return WM_SENDREQUEST_FAILED;
	}
	t = time(NULL);
	DWORD dwStatus = 0;
	DWORD dwStatusSize = sizeof(dwStatus);
	bRet = HttpQueryInfo(hRequest,HTTP_QUERY_STATUS_CODE | HTTP_QUERY_FLAG_NUMBER,&dwStatus,&dwStatusSize,NULL);
	if (!bRet)
	{
		SetLastErrorInfo("HttpQueryInfo Error:%d,TimeSpan:%d", GetLastError(),time(NULL)-t);
		InternetCloseHandle(hRequest);
		InternetCloseHandle(hConnect);
		return WM_QUERYINFO_FAILED;
	}
	if ( !(dwStatus == HTTP_STATUS_OK ||dwStatus == HTTP_STATUS_MOVED ||
		dwStatus == HTTP_STATUS_REDIRECT ||	dwStatus == HTTP_STATUS_REDIRECT_METHOD) )
	{
		SetLastErrorInfo("RetStatusCode Error,dwStatus:%d,LastError:%d,TimeSpan:%d",dwStatus,GetLastError(),time(NULL)-t);
		InternetCloseHandle(hRequest);
		InternetCloseHandle(hConnect);
		return WM_RETCODE_ERROR;
	}
	char szHttpHeader[10240] = {0};
	DWORD dHeaderLen = sizeof(szHttpHeader);
	bRet = HttpQueryInfo(hRequest,HTTP_QUERY_RAW_HEADERS_CRLF,szHttpHeader,&dHeaderLen,NULL);
	if (!bRet)
	{
		SetLastErrorInfo("HttpQueryInfo Error:%d,TimeSpan:%d", GetLastError(),time(NULL)-t);
		InternetCloseHandle(hRequest);
		InternetCloseHandle(hConnect);
		return WM_QUERYINFO_FAILED;
	}
	m_strHttpHeader = string(szHttpHeader);
	SetSessionCookie();
	t = time(NULL);
	m_strContent.clear();
	DWORD dwReadLen = 0;
	DWORD dwCurrLen = 0;	
	char szBuf[1024] = {0};
	if (szFilePath != NULL && _access(szFilePath,0) == 0)
	{
		remove(szFilePath);
	}
	while (true)
	{
		if (bIsFile)
		{
			bRet = InternetReadFile(hRequest,szBuf,1,&dwCurrLen);
			if ( !bRet || dwCurrLen <= 0)
			{
				break;
			}
			fstream fsFile(szFilePath, ios_base::out | ios_base::app | ios_base::binary);
			fsFile << szBuf[0];
			fsFile.flush();
			fsFile.close();
		}
		else
		{
			ZeroMemory(szBuf,1024);
			bRet = InternetReadFile(hRequest,szBuf,1023,&dwCurrLen);
			if (!bRet)
			{
				SetLastErrorInfo("InternetReadFile Error:%d,TimeSpan:%d", GetLastError(),time(NULL)-t);
				InternetCloseHandle(hRequest);
				InternetCloseHandle(hConnect);
				return WM_READFILE_ERROR;
			}
			if (dwCurrLen == 0)
			{
				break;
			}
			dwReadLen += dwCurrLen;
			m_strContent.append(szBuf);
		}		
	}
	return 0;
}
/************************************************************************/
/* GET��ʽ��ȡ����                                                      */
/************************************************************************/
int CWebBase::GetPage(const char* lpszObject,const char* lpszHeader,const char* lpszServer,int nPort,bool bIsFile,const char* szFilePath)
{
	return GetData(lpszObject,lpszHeader,lpszServer,nPort,"GET",NULL,bIsFile,szFilePath);
}

/************************************************************************/
/* POST��ʽ��ȡ����                                                     */
/************************************************************************/
int CWebBase::PostPage(const char* lpszObject,const char* lpszData,const char* lpszHeader,const char* lpszServer,int nPort,bool bIsFile,const char* szFilePath)
{
	return GetData(lpszObject,lpszHeader,lpszServer,nPort,"POST",lpszData,bIsFile,szFilePath);
}

/************************************************************************/
/* ��InternetSession���ó�ʱ                                            */
/************************************************************************/
void CWebBase::SetTimeOut( )
{
	if (m_nTimeOutSecond < 10)
	{
		m_nTimeOutSecond = 10;
	}
	DWORD dwTime = 3000;//3������ӳ�ʱ
	InternetSetOption(m_hInterSessin,INTERNET_OPTION_CONNECT_TIMEOUT,(LPVOID)&dwTime,sizeof(dwTime));
	//3��ķ��ͳ�ʱ
	InternetSetOption(m_hInterSessin,INTERNET_OPTION_SEND_TIMEOUT,(LPVOID)&dwTime,sizeof(dwTime));
	dwTime = m_nTimeOutSecond * 1500;	//���ճ�ʱ
	InternetSetOption(m_hInterSessin,INTERNET_OPTION_RECEIVE_TIMEOUT,(LPVOID)&dwTime,sizeof(dwTime));
	dwTime = 3000;//3������ݷ��ͳ�ʱ
	InternetSetOption(m_hInterSessin,INTERNET_OPTION_DATA_SEND_TIMEOUT,(LPVOID)&dwTime,sizeof(dwTime));
	dwTime = m_nTimeOutSecond * 1500;	//���ճ�ʱ
	InternetSetOption(m_hInterSessin,INTERNET_OPTION_DATA_RECEIVE_TIMEOUT,(LPVOID)&dwTime,sizeof(dwTime));
	dwTime = 2;//���Դ���
	InternetSetOption(m_hInterSessin,INTERNET_OPTION_CONNECT_RETRIES,(LPVOID)&dwTime,sizeof(dwTime));
}



/************************************************************************/
/*                                                                      */
/************************************************************************/
DWORD CWebBase::GenFlags()
{
	DWORD dwFlags = 0;
	dwFlags = INTERNET_FLAG_NO_CACHE_WRITE | INTERNET_FLAG_NO_AUTO_REDIRECT | INTERNET_FLAG_RELOAD;
	if (m_nHttpType == 0)			//�����HTTPSЭ��
	{
		dwFlags = dwFlags |
			INTERNET_FLAG_SECURE |
			INTERNET_FLAG_IGNORE_CERT_CN_INVALID |
			INTERNET_FLAG_IGNORE_CERT_DATE_INVALID | SECURITY_IGNORE_ERROR_MASK ;
	}
	if(m_bNoCookie)
	{
		dwFlags |= INTERNET_FLAG_NO_COOKIES;
	}
	return dwFlags;
}
/************************************************************************/
/* ���ô�����Ϣ                                                         */
/************************************************************************/
void CWebBase::SetLastErrorInfo(const char* szFormat, ...) 
{ 		
	char szBuf[2048] = {0};
	va_list arg_list;
	va_start (arg_list, szFormat);
	::vsnprintf_s(szBuf, sizeof(szBuf) - 1, _TRUNCATE, szFormat, arg_list);
	m_strErrorInfo = string(szBuf); 
	g_objLog.loginfo(szBuf);
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
BOOL CWebBase::SetSessionCookie()
{
	m_strCookie = "";
	if(!m_bNoCookie) return TRUE;
	string strBefore = "Set-Cookie:";
	int iPos1 = m_strHttpHeader.find(strBefore);
	if (iPos1 < 0)
	{
		return FALSE;
	}
  	int iPos2 = m_strHttpHeader.find("\r\n",iPos1+strBefore.length());
	if (iPos2 < iPos1)
	{
		return FALSE;
	}
	m_strCookie = m_strHttpHeader.substr(iPos1+strBefore.length(),iPos2);
	return TRUE;
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
string CWebBase::GetAppPath()
{
	char szExePath[_MAX_PATH] = {0};
	::GetModuleFileName(NULL, szExePath, _MAX_PATH);	
	char *szFlag = strrchr(szExePath,'\\');
	if (szFlag)
	{
		*szFlag = '\0';
	}
	string strRet = string(szExePath);
	strRet.append("\\");
	return strRet;
}
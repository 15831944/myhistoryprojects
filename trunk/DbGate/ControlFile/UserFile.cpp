#include "StdAfx.h"
#include ".\userfile.h"
#include "dataType.h"
#include <atlbase.h>
#include "ConsolePrinter.h"
#pragma comment(lib, "VERSION.LIB")
CUserFile *CUserFile:: m_Instance=NULL;
CCriticalSection CUserFile::m_slockInstance;
/************************************************************************/
/*                                                                      */
/************************************************************************/
CUserFile::CUserFile(void)
{
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
CUserFile::~CUserFile(void)
{
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
CUserFile * CUserFile::GetInstance()
{
	if(m_Instance==NULL)
	{
		m_slockInstance.Lock();
		if(m_Instance==NULL)
		{
			m_Instance=new CUserFile;
		}
		m_slockInstance.Unlock();
	}
	return m_Instance;
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
int CUserFile::WriteInitInfo(CString strfilePath,CString strnodeName,CString strKeyName,CString strKeyValue,int iKeyValue,int iFlag)
{
	//���ݿ�����
	CString strTemp = "";
	try
	{
		if ( iFlag == 0 )	//�ַ���
		{
			strTemp.Format("%s",strKeyValue);
		}
		else
		{
			strTemp.Format("%d",iKeyValue);
		}
		::WritePrivateProfileString(strnodeName,strKeyName, strTemp, strfilePath);
		return 0;
	}
	catch (...)
	{
		WriteLog("WriteInitInfo�쳣",3);
	}
	return -99;
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
int CUserFile::ReadInitInfo(CString strfilePath,CString strnodeName,CString strKeyName,CString &strKeyValue,int &iKeyValue,int iFlag)
{
	char szTemp[2001] = {0};
	try
	{
		if (iFlag == 0)	//�ַ���
		{
			::GetPrivateProfileString(strnodeName,strKeyName, "",(LPSTR)szTemp,2001, strfilePath);
			strKeyValue.Format("%s",szTemp);
			if (strKeyValue == "")
			{
				return -1;
			}
		}
		else	//����
		{
			iKeyValue = ::GetPrivateProfileInt(strnodeName,strKeyName, -99, strfilePath);
			if (iKeyValue == -99)
			{
				return -1;
			}
		}
		return iFlag;
	}
	catch (...)
	{
		WriteLog("ReadInitInfo�쳣",3);
	}	
	return -99;
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
//����Ӧ�ó�������Ŀ¼������"\"
int CUserFile::GetAppPath(CString &strAppPath)
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
		WriteLog("GetAppPath�쳣",3);
	}
	return iRet;	
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
//����Ӧ�ó�����
int CUserFile::GetAppName(CString &strAppName)
{
	int iRet = -3;
	try
	{
		TCHAR szBuff[_MAX_PATH];
		VERIFY(::GetModuleFileName(AfxGetApp()->m_hInstance, szBuff, _MAX_PATH));
		strAppName.Format("%s",szBuff);
		int pos=strAppName.ReverseFind('\\');
		strAppName=strAppName.Right(strAppName.GetLength()-pos-1);	
		iRet = 0;
	}
	catch (...)
	{
		WriteLog("strAppName�쳣",3);
	}
	return iRet;	
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
void CUserFile::CheckInitValue(int &iValue,int iMin,int iMax)
{
	if (iValue < iMin)
	{
		iValue = iMin;
	}
	else if (iValue > iMax)
	{
		iValue = iMax;
	}
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
//���ܳ�16�����ַ���
CString CUserFile::StrEncode(CString srcStr)
{
	int             i ;
	int             nLen = 0;
	CString         newString = "";
	CString         tempStr = "";
	unsigned char   ucTemp = 0;
	nLen =  srcStr.GetLength( );
	//���ܺ�ת��16����
	for( i = 0; i < nLen; i++ )
	{
		ucTemp = ( unsigned char )srcStr.GetAt(i);
		tempStr.Format("%02X", ucTemp^(i%255) );
		newString +=tempStr;
	}
	return newString;
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
//����16�����ַ���
CString CUserFile::StrDecode(CString srcStr)
{
	CString         newString = "";
	CString         tempStr = "";
	//����16�����ַ���
	int i = 0;
	while( srcStr.GetLength() > 0 ) 
	{ 
		CString temp1 = "0x" + srcStr.Left(2); //ȡ��һ��16�����ִ�
		int ldec = strtol(temp1, NULL, 16); //��16����ת��Ϊ10����
		CString temp = "";
		temp.Format("%c",(unsigned char )ldec^(i%255));
		newString += temp;
		srcStr = srcStr.Right( srcStr.GetLength() - 2);
		i ++;
	} 
	return newString;
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
void CUserFile::SplitMsg(CString srcStr,CString *strArray,CString strSplit,int iLen)
{
	int i = 0;
	int idx = 0;
	srcStr.Replace(strSplit," ");
	srcStr.Trim();
	if ( srcStr == "" )
	{
		return;
	}
	srcStr += " ";	
	while(true)						//����ҵ�
	{
		idx = srcStr.Find(' ',0);  //�ҵ�һ����¼
		if ( idx != -1 )
		{
			strArray[i] = srcStr.Left( idx );
			srcStr = srcStr.Mid(idx + 1);
			i++;
			if ( i >= iLen)
			{
				break;
			}
		}
		else
		{
			break;
		}
	}
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
CString CUserFile::GetFileVersion(CString strFileName)   
{      
	int iVerInfoSize;   
	char *pBuf;   
	CString asVer="";   
	VS_FIXEDFILEINFO *pVsInfo;   
	unsigned int iFileInfoSize = sizeof(VS_FIXEDFILEINFO);
	iVerInfoSize   =   GetFileVersionInfoSize(strFileName,NULL);
	if(iVerInfoSize!= 0)   
	{      
		pBuf = new char[iVerInfoSize];   
		if(GetFileVersionInfo(strFileName,0,iVerInfoSize,pBuf))
		{      
			if(VerQueryValue(pBuf,"\\",(void**)&pVsInfo,&iFileInfoSize))      
			{      
				asVer.Format("%d.%d.%d.%d",HIWORD(pVsInfo->dwFileVersionMS),LOWORD(pVsInfo->dwFileVersionMS),HIWORD(pVsInfo->dwFileVersionLS),LOWORD(pVsInfo->dwFileVersionLS));   
			}      
		}      
		delete[] pBuf;
		pBuf = NULL;
	}
	return "v"+asVer;
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
CString CUserFile::GetCurrentAppPath()   
{   
	char szFile[MAX_PATH];   
	GetModuleFileName(NULL,szFile,MAX_PATH);	
	return CString(szFile);   
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
CString CUserFile::GetModifyTime(CString appname)
{   
	WIN32_FIND_DATA ffd ;   
	HANDLE hFind = FindFirstFile(appname,&ffd);   
	SYSTEMTIME stUTC, stLocal;   
	FileTimeToSystemTime(&(ffd.ftLastWriteTime), &stUTC);   
	SystemTimeToTzSpecificLocalTime(NULL, &stUTC, &stLocal);   
	CString strTmp = "";   
	strTmp.Format("%d-%d-%d,%d:%d", stLocal.wYear,stLocal.wMonth,stLocal.wDay,stLocal.wHour,stLocal.wMinute);   
	return strTmp;   
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
bool CUserFile::DeleteAutoRun(CString strKey)
{
	bool bRet = false;
	CRegKey   regAutoRun;
	char szKey[50] = {0};
	strcpy(szKey,strKey);
	if (ERROR_SUCCESS != regAutoRun.Create(HKEY_CURRENT_USER,"Software\\Microsoft\\Windows\\CurrentVersion\\Run"))
	{
		return false;
	}
	if( ERROR_SUCCESS == regAutoRun.DeleteValue(szKey))
	{
		bRet = true;
	}
	regAutoRun.Close();
	return bRet;
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
bool CUserFile::SetAutoRun(CString strKey)
{
	bool bRet = false;
	CRegKey   regAutoRun;
	char szKey[50] = {0};
	strcpy(szKey,strKey);
	if (ERROR_SUCCESS != regAutoRun.Create(HKEY_CURRENT_USER,"Software\\Microsoft\\Windows\\CurrentVersion\\Run"))
	{
		return false;
	}
	char fileNameBuf[MAX_PATH];   
	long iLen   =   0;   
	iLen = GetModuleFileName(NULL,fileNameBuf,255);//Ĭ�ϵ�ǰ��exe   
	fileNameBuf[iLen] = NULL;   
	if( ERROR_SUCCESS == regAutoRun.SetValue(fileNameBuf,szKey))
	{
		bRet = true;
	}
	regAutoRun.Close();
 	return bRet;
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
bool CUserFile::IsAutoRun(CString strKey)
{
	bool bRet = false;
	CRegKey   regAutoRun;
	char szKey[50] = {0};
	strcpy(szKey,strKey);
	if (ERROR_SUCCESS != regAutoRun.Create(HKEY_CURRENT_USER,"Software\\Microsoft\\Windows\\CurrentVersion\\Run"))
	{
		return false;
	}
	char fileNameBuf[MAX_PATH]={0};   
	long iLen   =   0;
	DWORD dLen = 255;
	char szValue[MAX_PATH]={0};   
	iLen = GetModuleFileName(NULL,fileNameBuf,255);//Ĭ�ϵ�ǰ��exe   
	fileNameBuf[iLen] = NULL;  
	if( ERROR_SUCCESS == regAutoRun.QueryValue(szValue,szKey,&dLen))
	{
		if (strcmp(fileNameBuf,szValue)==0)
		{
			bRet=true; 
		}
	}
	regAutoRun.Close();
	return bRet;
}
#include "stdafx.h"
#include "WebBase.h"
#include <atlrx.h>
#include "zlib\gzdecompress.h"
#include "datatype.h"
using namespace std;
#define TOHEX(x) ((x)>9 ? (x)+55 : (x)+48)
//HTTP ������ GetPage
//����HTTPЭ�����������ת������ѹ
/************************************************************************/
/*                                                                      */
/************************************************************************/
CWebBase::CWebBase( )
{
	m_pConnection=NULL;
	m_nNotDirect = 0;
	m_nElseWhereIP = 0;
	m_iHttpsFlag = 0;
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
CWebBase::~CWebBase()
{
	DeleteObjectExEx();
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
//��������ϴ�������ҳ���URL ,���ϴ��ı����ļ�·��
int  CWebBase::UploadFile(LPCTSTR strURL, LPCTSTR strLocalFileName)
{
	ASSERT(strURL != NULL && strLocalFileName != NULL);

	BOOL bResult = FALSE;
	DWORD dwType = 0;
	CString strServer;
	CString strObject;
	INTERNET_PORT wPort = 0;
	DWORD dwFileLength = 0;
	char * pFileBuff = NULL;

	CHttpConnection * pHC = NULL;
	CHttpFile * pHF = NULL;
	CInternetSession cis;

	bResult =  AfxParseURL(strURL, dwType, strServer, strObject, wPort);
	if(!bResult)
	{
		return FALSE;
	}
	CFile file;
	try
	{
		if(!file.Open(strLocalFileName, CFile::shareDenyNone | CFile::modeRead))
			return FALSE;
		dwFileLength = file.GetLength();
		if(dwFileLength <= 0)
			return FALSE;
		pFileBuff = new char[dwFileLength];
		memset(pFileBuff, 0, sizeof(char) * dwFileLength);
		file.Read(pFileBuff, dwFileLength);

		const int nTimeOut = 5000;
		cis.SetOption(INTERNET_OPTION_CONNECT_TIMEOUT, nTimeOut); //���ӳ�ʱ����
		cis.SetOption(INTERNET_OPTION_CONNECT_RETRIES, 1);  //����1��
		pHC = cis.GetHttpConnection(strServer, wPort);  //ȡ��һ��Http����

		pHF = pHC->OpenRequest(CHttpConnection::HTTP_VERB_POST, strObject);
		if(!pHF->SendRequest(NULL, 0, pFileBuff, dwFileLength))
		{
			delete[]pFileBuff;
			pFileBuff = NULL;
			pHF->Close();
			pHC->Close();
			cis.Close();
			return FALSE;
		}
		DWORD dwStateCode = 0;
		pHF->QueryInfoStatusCode(dwStateCode);

		if(dwStateCode == HTTP_STATUS_OK)
			bResult = TRUE;
	}

	catch(CInternetException * pEx)
	{
		char sz[256] = "";
		pEx->GetErrorMessage(sz, 25);
		CString str;
		str.Format("InternetException occur!\r\n%s", sz);
		AfxMessageBox(str);
	}
	catch(...)
	{
		DWORD dwError = GetLastError();
		CString str;
		str.Format("Unknow Exception occur!\r\n%d", dwError);
		AfxMessageBox(str);
	}

	delete[]pFileBuff;
	pFileBuff = NULL;
	file.Close();
	pHF->Close();
	pHC->Close();
	cis.Close();
	return bResult;
}


/************************************************************************/
/*                                                                      */
/************************************************************************/
int CWebBase::TraditionTOSimple( CString & strHtml )
{
	char *          pszBig5=NULL;          //Big5������ַ�        
	wchar_t *       wszUnicode=NULL;       //Unicode������ַ� 
	char *          pszGbt=NULL;           //Gb����ķ����ַ� 
	char *          pszGbs=NULL;           //Gb����ļ����ַ� 
	CString         sGb;                   //���ص��ַ��� 
	int             iLen=0;                //��Ҫת�����ַ��� 

	//ȡ�ó���
	iLen = strHtml.GetLength();
	if ( iLen <= 0 ) 
	{
		return iLen;
	}
	pszBig5 = strHtml.GetBuffer( iLen);

	//����ת�����ַ��� 
	iLen=MultiByteToWideChar (950, 0, pszBig5, -1, NULL,0) ;
	if( iLen <= 0 ) 
	{
		return iLen;
	}
	//��wszUnicode�����ڴ� 
	wszUnicode = new wchar_t[iLen+1]; 
	if( wszUnicode == NULL )
	{
		return -1;
	}
	//ת��Big5�뵽Unicode�룬ʹ����API����MultiByteToWideChar 
	MultiByteToWideChar (950, 0, pszBig5, -1, wszUnicode,iLen);

	//����ת�����ַ��� 
	iLen=WideCharToMultiByte( 936, 0, (PWSTR) wszUnicode, -1, 
		NULL,0, NULL, NULL) ; 
	//��pszGbt�����ڴ� 
	pszGbt=new char[iLen+1]; 
	if( pszGbt == NULL )
	{
		delete [] wszUnicode; 
		return -1;
	}
	//��pszGbs�����ڴ� 
	pszGbs = new char[iLen+1]; 
	if( pszGbs == NULL )
	{
		delete [] wszUnicode; 
		delete [] pszGbt;
		return -1;
	}
	//ת��Unicode�뵽Gb�뷱�壬ʹ��API����WideCharToMultiByte 
	WideCharToMultiByte( 936, 0, (PWSTR) wszUnicode, -1, 
		pszGbt,iLen, NULL, NULL) ; 

	//ת��Gb�뷱�嵽Gb����壬ʹ��API����LCMapString 
	LCMapString( 0x0804, LCMAP_SIMPLIFIED_CHINESE, 
		pszGbt, -1, pszGbs, iLen); 
	//�ͷ��ڴ�        
	delete [] wszUnicode; 
	delete [] pszGbt;         
	if( HtmlCodeToUniCodeByte( (char *) pszGbs,  iLen) != iLen )
	{
		delete [] pszGbs; 
		return 0;
	}
	strHtml = CString( pszGbs, iLen);
	if( strHtml.GetLength() != iLen )
	{
		delete [] pszGbs; 
		return 0;
	}
	//�ͷ��ڴ� 
	delete [] pszGbs; 
	strHtml.Replace("&#", "");
	strHtml.Replace("^^^;", "");
	//strHtml.Replace("^", "");

	//����Gb������ַ� 
	return strHtml.GetLength();
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
int CWebBase::HtmlCodeToUniCodeByte( char *  pszHtmlcode, int iLen )
{

	int i;
	char * ptemp;
	unsigned short usData;
	char cbuffGbt[8] = {0};
	char cBuffGbs[8] = {0}; 

	if( pszHtmlcode == NULL || iLen <= 0 ) 
	{
		return -1;
	}
	ptemp = pszHtmlcode;
	pszHtmlcode = strstr( pszHtmlcode, "&#") ;
	if( pszHtmlcode == NULL ) 
	{
		return iLen;
	}

	for ( i = 0; i < 4000; i++ )//&#
	{
		pszHtmlcode = strstr( pszHtmlcode, "&#") ;
		if( pszHtmlcode == NULL ) 
		{
			break;
		}
		pszHtmlcode += 2;
		usData = (unsigned short)atoi(pszHtmlcode);
		//ת��Unicode�뵽Gb�뷱�壬ʹ��API����WideCharToMultiByte 
		WideCharToMultiByte ( 936, 0, (PWSTR)&usData, -1, 
			cbuffGbt,2, NULL, NULL) ; 
		//ת��Gb�뷱�嵽Gb����壬ʹ��API����LCMapString 
		LCMapString( 0x0804,LCMAP_SIMPLIFIED_CHINESE, cbuffGbt, -1, 
			cBuffGbs , 2); 
		memcpy( pszHtmlcode , cBuffGbs, 2);
		memset( pszHtmlcode + 2, '^' , 3 );
		pszHtmlcode += 6;
	}
	return iLen;

}

/************************************************************************/
/*                                                                      */
/************************************************************************/
//��������ʽ��ת��
int CWebBase::HtmlCodeToUniCodeByteReg( char *  pszHtmlcode, int iLen )
{

	int     i;
	char  * pcTemp = NULL;
	char *  pcWord = NULL;

	REParseError status;
	CAtlRegExp<>            reurl;
	CAtlREMatchContext<>    mcurl;
	const CAtlREMatchContext<>::RECHAR * szStart = 0;
	const CAtlREMatchContext<>::RECHAR * szEnd = 0;

	unsigned short usData;
	char cbuffGbt[8] = {0};
	char cBuffGbs[8] = {0}; 


	pcTemp = strstr( pszHtmlcode, "&#") ;
	if( pcTemp == NULL ) 
	{
		return iLen;
	}
	status = reurl.Parse( "&#{.*?};");
	if( REPARSE_ERROR_OK != status)
	{
		return iLen;
	}
	for ( i = 0; i < 4000; i++ )
	{
		if (!reurl.Match( pcTemp , &mcurl ) )
		{
			break;
		}
		if( mcurl.m_uNumGroups <= 0 )
		{
			break;
		}
		szStart = NULL;
		szEnd = NULL;
		mcurl.GetMatch( 0, &szStart, &szEnd );
		pcTemp = (char*)szStart;
		if( pcTemp == NULL || szStart == NULL )
		{
			break;
		}
		pcWord = pcTemp;
		//nLength = szEnd - szStart;        
		usData = (unsigned short)atoi(pcWord);
		//ת��Unicode�뵽Gb�뷱�壬ʹ��API����WideCharToMultiByte 
		WideCharToMultiByte ( 936, 0, (PWSTR)&usData, -1, 
			cbuffGbt,2, NULL, NULL) ; 
		//ת��Gb�뷱�嵽Gb����壬ʹ��API����LCMapString 
		LCMapString( 0x0804,LCMAP_SIMPLIFIED_CHINESE, cbuffGbt, -1, 
			cBuffGbs , 2); 
		memcpy( pcWord, cBuffGbs, 2);
		memset( pcWord + 2, '^' , 3 );                        

	}
	return iLen;
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
//��Html����ת��Ϊ����
int CWebBase::HtmlCodeToSimple( CString & strHtmlText )
{

	int             i = 0;
	char *          ptemp = NULL;
	char *          pEnd = NULL;
	unsigned short  usData = 0;
	char cbuffGbt[8] = {0};
	char cBuffGbs[8] = {0}; 

	if( strHtmlText.IsEmpty() ) 
	{
		return 0;
	}
	ptemp = strHtmlText.GetBuffer( 20 );
	ptemp = strstr( ptemp, "&#") ;
	if( ptemp == NULL ) 
	{
		return strHtmlText.GetLength();
	}
	for ( i = 0; i < 4000; i++ )//&#
	{
		ptemp = strstr( ptemp, "&#") ;
		if( ptemp == NULL ) 
		{
			break;
		}
		ptemp += 2;
		usData = (unsigned short)atoi(ptemp);
		if( usData > 255 )
		{                
			//ת��Unicode�뵽Gb�뷱�壬ʹ��API����WideCharToMultiByte 
			WideCharToMultiByte ( 936, 0, (PWSTR)&usData, -1, 
				cbuffGbt,2, NULL, NULL) ; 
			//ת��Gb�뷱�嵽Gb����壬ʹ��API����LCMapString 
			LCMapString( 0x0804,LCMAP_SIMPLIFIED_CHINESE, cbuffGbt, -1, 
				cBuffGbs , 2); 
			memcpy( ptemp, cBuffGbs, 2);
			memset( ptemp + 2, '^' , 3 );
			ptemp += 6;
		}
		else
		{
			//* ptemp = (char)usData ;
			pEnd = strstr( ptemp, ";") ;
			if( pEnd != NULL )
			{
				memset( ptemp , '^' , pEnd - ptemp );
				ptemp = pEnd;
			}
		}
	}
	strHtmlText.Replace("&#", "");
	strHtmlText.Replace("^^^;", "");
	//strHtmlText.Replace("^", "");
	return strHtmlText.GetLength();
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CWebBase::ConvertGBKToUtf8(CString& strGBK) 
{
	int len=MultiByteToWideChar(CP_ACP, 0, (LPCTSTR)strGBK, -1, NULL,0);
	unsigned short * wszUtf8 = new unsigned short[len+1];
	memset(wszUtf8, 0, len * 2 + 2);
	MultiByteToWideChar(CP_ACP, 0, (LPCTSTR)strGBK, -1, (LPWSTR)wszUtf8, len);

	len = WideCharToMultiByte(CP_UTF8, 0, (LPWSTR)wszUtf8, -1, NULL, 0, NULL, NULL); 
	char *szUtf8=new char[len + 1];
	memset(szUtf8, 0, len + 1);
	WideCharToMultiByte (CP_UTF8, 0, (LPWSTR)wszUtf8, -1, szUtf8, len, NULL,NULL);

	strGBK = szUtf8;
	delete[] szUtf8;
	delete[] wszUtf8;
	return ;
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CWebBase::ConvertUtf8ToGBK(CString& strUtf8) 
{
	int len = 0;
	unsigned short * wszGBK = NULL;
	char * szGBKt = NULL;
	char * szGBKs = NULL;

	len = MultiByteToWideChar(CP_UTF8, 0, (LPCTSTR)strUtf8, -1, NULL,0);
	wszGBK = new unsigned short[len+1];
	memset(wszGBK, 0, len * 2 + 2);
	MultiByteToWideChar(CP_UTF8, 0, (LPCTSTR)strUtf8, -1, (LPWSTR)wszGBK, len);

	len = WideCharToMultiByte(CP_ACP, 0, (LPWSTR)wszGBK, -1, NULL, 0, NULL, NULL); 
	szGBKt  =new char[len + 1];
	memset(szGBKt, 0, len + 1);
	WideCharToMultiByte (CP_ACP, 0, (LPWSTR)wszGBK, -1, szGBKt, len, NULL,NULL);

	szGBKs  =new char[len + 1];
	memset(szGBKs, 0, len + 1);
	//ת��Gb�뷱�嵽Gb����壬ʹ��API����LCMapString 
	LCMapString( 0x0804, LCMAP_SIMPLIFIED_CHINESE, szGBKt, -1, szGBKs, len);
	strUtf8 = szGBKs;
	delete[] szGBKs;
	delete[] szGBKt;
	delete[] wszGBK;
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
int CWebBase::SetTimeOut( CInternetSession & internetSession, int  nTimeOutSecond  )
{
	if( nTimeOutSecond < 1 )
	{
		return -1;
	}
	m_Session.SetOption(INTERNET_OPTION_CONNECT_TIMEOUT, 1000 * nTimeOutSecond );   //3������ӳ�ʱ
	m_Session.SetOption(INTERNET_OPTION_SEND_TIMEOUT, 1000);                        // 1��ķ��ͳ�ʱ
	m_Session.SetOption(INTERNET_OPTION_RECEIVE_TIMEOUT, 1500 * nTimeOutSecond );   // 7��Ľ��ճ�ʱ
	m_Session.SetOption(INTERNET_OPTION_DATA_SEND_TIMEOUT, 1200);                   // 1������ݷ��ͳ�ʱ
	m_Session.SetOption(INTERNET_OPTION_DATA_RECEIVE_TIMEOUT, 1500 * nTimeOutSecond );  // 7������ݽ��ճ�ʱ
	m_Session.SetOption(INTERNET_OPTION_CONNECT_RETRIES, 1);                            // 1������
	return 0;
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CWebBase::URL_Encode(  CString  &  strSource )
{
	int  i;
	int  index = 0;
	int  len = 0 ;
	char tmp_chr;
	char * str_output = NULL;

	len = strSource.GetLength() ;
	if( len <= 0 )
	{
		return ;
	}
	str_output  = new char[ len * 4 ];
	for(i = 0; i < len; i++)
	{
		if(     ('A' <= strSource[i] && strSource[i] <= 'Z') 
			||  ('a' <= strSource[i] && strSource[i] <= 'z')
			||  ('0' <= strSource[i] && strSource[i] <= '9') 
			|| strSource[i] == '-' ||  strSource[i] == '_' 
			|| strSource[i] ==  '.'  )
		{
			str_output[index++] = strSource[i];
		}
		else
		{
			str_output[index++] = '%';
			tmp_chr = strSource[i] / 16;
			tmp_chr += tmp_chr > 9 ? 'A' - 10 : '0';
			str_output[index++] = tmp_chr;

			tmp_chr = strSource[i] % 16;
			tmp_chr += tmp_chr > 9 ? 'A' - 10 : '0';
			str_output[index++] = tmp_chr;
		}
	}
	str_output[index++] = '\0';
	strSource = str_output;
	delete [] str_output;
	return  ;
}
inline BYTE toHex(const BYTE &x)
{
	return x > 9 ? x + 55: x + 48;
}
inline BYTE toByte(const BYTE &x)
{
	return x > 57? x - 55: x - 48;
}
CString CWebBase::URLDecode(CString sIn)
{
	CString sOut;
	const int nLen = sIn.GetLength() + 1;
	register LPBYTE pOutTmp = NULL;
	LPBYTE pOutBuf = NULL;
	register LPBYTE pInTmp = NULL;
	LPBYTE pInBuf =(LPBYTE)sIn.GetBuffer(nLen);
	//alloc out buffer
	pOutBuf = (LPBYTE)sOut.GetBuffer(nLen);

	if(pOutBuf)
	{
		pInTmp   = pInBuf;
		pOutTmp = pOutBuf;
		// do encoding
		while (*pInTmp)
		{
			if('%'==*pInTmp)
			{
				pInTmp++;
				*pOutTmp++ = (toByte(*pInTmp)%16<<4) + toByte(*(pInTmp+1))%16;//��4λ+��4λ
				pInTmp++;
			}
			else if('+'==*pInTmp)
				*pOutTmp++ = ' ';
			else
				*pOutTmp++ = *pInTmp;
			pInTmp++;
		}
		*pOutTmp = '\0';
		sOut.ReleaseBuffer();
	}
	sIn.ReleaseBuffer();

	return sOut;
}
CString CWebBase::URLEncode(CString sIn)
{
	CString sOut;
	const int nLen = sIn.GetLength() + 1;
	register LPBYTE pOutTmp = NULL;
	LPBYTE pOutBuf = NULL;
	register LPBYTE pInTmp = NULL;
	LPBYTE pInBuf =(LPBYTE)sIn.GetBuffer(nLen);
	//alloc out buffer
	pOutBuf = (LPBYTE)sOut.GetBuffer(nLen*3);

	if(pOutBuf)
	{
		pInTmp   = pInBuf;
		pOutTmp = pOutBuf;
		// do encoding
		while (*pInTmp)
		{
			if(isalnum(*pInTmp) || '-'==*pInTmp || '_'==*pInTmp || '.'==*pInTmp)
				*pOutTmp++ = *pInTmp;
			else if(isspace(*pInTmp))
				*pOutTmp++ = '+';
			else
			{
				*pOutTmp++ = '%';
				*pOutTmp++ = toHex(*pInTmp>>4);//��4λ
				*pOutTmp++ = toHex(*pInTmp%16);//��4λ
			}
			pInTmp++;
		}
		*pOutTmp = '\0';
		sOut.ReleaseBuffer();
	}
	sIn.ReleaseBuffer();

	return sOut;
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
void CWebBase::URL_Decode( CString  &  strSource )
{
	int i = 0, index = 0;
	int  len = 0;
	int hb, lb;

	char * str_output = NULL;
	len = strSource.GetLength();
	if( len <= 1 )
	{
		return ;
	}
	str_output = new char[len];

	while(i < len)
	{
		if((strSource[i] == '%') && (i + 2 <= len) )
		{
			if('A' <= strSource[i + 1] && strSource[i + 1] <= 'F')
			{
				hb = strSource[i + 1] - 'A' + 10;
			}
			else if('a' <= strSource[i + 1] && strSource[i + 1] <= 'f')
			{
				hb = strSource[i + 1] - 'a' + 10;
			}
			else if('0' <= strSource[i + 1] && strSource[i + 1] <= '9')
			{
				hb = strSource[i + 1] - '0';
			}
			else
			{
				hb = 0;
			}

			if('A' <= strSource[i + 2] && strSource[i + 2] <= 'F')
			{
				lb = strSource[i + 2] - 'A' + 10;
			}
			else if('a' <= strSource[i + 2] && strSource[i + 2] <= 'f')
			{
				lb = strSource[i + 2] - 'a' + 10;
			}
			else if('0' <= strSource[i + 2] && strSource[i + 2] <= '9')
			{
				lb = strSource[i + 2] - '0';
			}
			else
			{
				lb = 0;
			}

			str_output[index] = hb * 16 + lb;
			i += 3;
		}
		else
		{
			str_output[index] = strSource[i];
			i++;
		}

		index++;
	}

	str_output[index] = 0;
	strSource =  str_output;       
	delete [] str_output;
	return;
}

/************************************************************************/
/*��ȡҳ�����֤��ͼƬ                                                  */
/************************************************************************/
int CWebBase::RequestGetPageExExEx(CString & strServer, CString & strObject, 
								 CString & strHead,CString &strError,
								 int iGetType,	//��ʾ��ȡ�ļ���1��ʾ��ȡ��֤��
								 int iIsProxy,	//�Ƿ�ʹ�ô���
								 int iProxyType,//��������
								 CString strProxyIP,//����IP
								 int iProxyPort,//����˿�
								 CString strProxyUser,//�����û���
								 CString strProxyPwd//��������
								 )
{
	CHttpFile * pHttpFile;
	char cBuffer[1024];
	unsigned long StatusCode;
	unsigned long  iReadLen = 0;
	int            nStringLen = 0;
	CString        strTemp = "";	
	WORD            wdPort;
	BOOL result;
	int iRet = -1;
	try
	{
		//��ȡ��������Ϣ
		DWORD dwFlags = GetDwflag();		
		pHttpFile = NULL;
		m_strWorkBuf.Empty();
		//���ó�ʱ
		SetTimeOut(m_Session,g_iTimeOut);
		//��ȡ����
		iRet = GetHttpConnectionEx(strServer,strObject,strHead,strError,iIsProxy,iProxyType,strProxyIP,iProxyPort);
		if (iRet < 0)
		{
			return iRet;
		}
		//���������
		pHttpFile = m_pConnection->OpenRequest( CHttpConnection::HTTP_VERB_GET,
			(LPCTSTR)strObject,NULL, 0,
			NULL, NULL,
			dwFlags );
		if( pHttpFile == NULL )
		{
			strError = "OpenRequestʧ��";
			return -1;
		}
		//�����SOCKET5���������˺�����
		// 		if   (!pHttpFile->SetOption(INTERNET_OPTION_PROXY_USERNAME,strProxyUser,strProxyUser.GetLength()))   
		// 		{   
		// 			return -99;
		// 		}
		// 		if   (!pHttpFile->SetOption(INTERNET_OPTION_PROXY_PASSWORD,strProxyPwd,strProxyPwd.GetLength()))   
		// 		{   
		// 			return -99;   
		// 		}
		//�������󵽷�����		
		iRet = SendRequest(pHttpFile,strError,strHead,"");/*��������*/
		if (iRet < 0)	//����HTTPͷ��Ϣʧ��
		{
			DeleteObjectExEx();
			return iRet; 
		}
		//��ȡ״̬�뼰ͷ��Ϣ
		pHttpFile->QueryInfoStatusCode(StatusCode);
		pHttpFile->QueryInfo(HTTP_QUERY_LOCATION,m_strRedirectUrl); 
		pHttpFile->QueryInfo(HTTP_QUERY_RAW_HEADERS_CRLF,m_strResponHead);
		iReadLen = (unsigned long )pHttpFile->GetLength();
		// �Ƿ��ض���
		if ( StatusCode == HTTP_STATUS_OK ||
			StatusCode == HTTP_STATUS_MOVED ||
			StatusCode == HTTP_STATUS_REDIRECT ||
			StatusCode == HTTP_STATUS_REDIRECT_METHOD)
		{
			//��ȡ����
			if (iGetType == 0)			//��ȡ�ı���Ϣ
			{
				while( iReadLen >= 0 )
				{
					iReadLen = pHttpFile->Read( cBuffer ,1023 );
					if ( iReadLen <= 0)
					{
						break;
					}
					m_strWorkBuf += CString( cBuffer ,iReadLen);
				}
			}
			else		//��ȡ��֤��
			{
				CStdioFile out;
				out.Open(g_strDirect+g_strFileName, CFile::modeCreate | CFile::modeWrite |CFile::typeBinary);
				while(true)
				{
					iReadLen = pHttpFile->Read( cBuffer ,1 );
					if ( iReadLen <= 0)
					{
						break;
					}
					out.Write(cBuffer,1); 
				}
				out.Close();
			}			
		}
		//ɾ��HTTPFILE����
		DeleteObjectEx(pHttpFile);	
		if(!m_strWorkBuf.IsEmpty()) //��ѹ����
		{
			return ConvertData(m_strResponHead,strError,m_strWorkBuf);	
		}
		return 0;
	}
	catch(CInternetException * e)
	{
		DeleteObjectEx(pHttpFile);	
		DeleteObjectExEx();
		e->Delete();
		return -6;
	}
	catch(...)
	{
		strError = "ץȡ��ҳ��Ϣ�쳣";
		DeleteObjectEx(pHttpFile);	
		DeleteObjectExEx();
		return -7;
	}
	return 0;
}
/************************************************************************/
/*��ȡҳ�����֤��ͼƬ                                                  */
/************************************************************************/
int CWebBase::RequestPostPageExExEx(CString & strServer, CString & strObject, 
								  CString & strHead,CString & strPostData,
								  CString &strError,
								  int iGetType,	//��ʾ��ȡ�ļ���1��ʾ��ȡ��֤��
								  int iIsProxy,	//�Ƿ�ʹ�ô���
								  int iProxyType,//��������
								  CString strProxyIP,//����IP
								  int iProxyPort,//����˿�
								  CString strProxyUser,//�����û���
								  CString strProxyPwd//��������
								  )
{
	CHttpFile * pHttpFile;
	char cBuffer[1024];
	unsigned long StatusCode;
	unsigned long  iReadLen = 0;
	int            nStringLen = 0;
	CString        strTemp = "";	
	WORD            wdPort;
	BOOL result;
	int iRet = -1;
	try
	{
		//��ȡ��������Ϣ
		DWORD dwFlags = GetDwflag();
		pHttpFile = NULL;
		m_strWorkBuf.Empty();
		//���ó�ʱ
		SetTimeOut(m_Session, g_iTimeOut);
		//��ȡ����
		iRet = GetHttpConnectionEx(strServer,strObject,strHead,strError,iIsProxy,iProxyType,strProxyIP,iProxyPort);
		if (iRet < 0)
		{
			return iRet;
		}
		//���������
		pHttpFile = m_pConnection->OpenRequest( CHttpConnection::HTTP_VERB_POST,
			(LPCTSTR)strObject,NULL, 0,
			NULL, NULL,
			dwFlags );
		if( pHttpFile == NULL )
		{
			strError = "OpenRequestʧ��";
			return -1;
		}
		//�����SOCKET5���������˺�����
		// 		if   (!pHttpFile->SetOption(INTERNET_OPTION_PROXY_USERNAME,strProxyUser,strProxyUser.GetLength()))   
		// 		{   
		// 			return -99;
		// 		}
		// 		if   (!pHttpFile->SetOption(INTERNET_OPTION_PROXY_PASSWORD,strProxyPwd,strProxyPwd.GetLength()))   
		// 		{   
		// 			return -99;   
		// 		}
		//�������󵽷�����		
		iRet = SendRequest(pHttpFile,strError,strHead,strPostData,1);/*��������*/
		if (iRet < 0)	//����POST����ʧ��
		{
			//������Ӷ���
			DeleteObjectExEx();
			return iRet; 
		}
		//��ȡ״̬�뼰ͷ��Ϣ
		pHttpFile->QueryInfoStatusCode(StatusCode);
		pHttpFile->QueryInfo(HTTP_QUERY_LOCATION,m_strRedirectUrl); 
		pHttpFile->QueryInfo(HTTP_QUERY_RAW_HEADERS_CRLF,m_strResponHead);
		iReadLen = (unsigned long )pHttpFile->GetLength();
		// �Ƿ��ض���
		if ( StatusCode == HTTP_STATUS_OK ||
			StatusCode == HTTP_STATUS_MOVED ||
			StatusCode == HTTP_STATUS_REDIRECT ||
			StatusCode == HTTP_STATUS_REDIRECT_METHOD)
		{
			//��ȡ����
			if (iGetType == 0)			//��ȡ�ı���Ϣ
			{
				while(iReadLen >= 0)
				{
					iReadLen = pHttpFile->Read( cBuffer ,1023 );
					if (iReadLen <= 0)
					{
						break;
					}
					m_strWorkBuf += CString( cBuffer ,iReadLen);
				}
			}
			else		//��ȡ��֤��
			{
				CStdioFile out;
				out.Open(g_strDirect+g_strFileName, CFile::modeCreate | CFile::modeWrite |CFile::typeBinary);
				while(true)
				{
					iReadLen = pHttpFile->Read(cBuffer ,1);
					if ( iReadLen <= 0)
					{
						break;
					}
					out.Write(cBuffer,1); 
				}
				out.Close();
			}			
		}
		//ɾ��HTTPFILE����
		DeleteObjectEx(pHttpFile);	
		if(!m_strWorkBuf.IsEmpty()) //��ѹ����
		{
			//ConvertUtf8ToGBK( m_strWorkBuf );
			return ConvertData(m_strResponHead,strError,m_strWorkBuf);
		}
		return 0;			
	}
	catch(CInternetException * e)
	{
		strError = "ץȡ��ҳ��Ϣ�쳣";
		DeleteObjectEx(pHttpFile);	
		DeleteObjectExEx();
		e->Delete();
		return -6;
	}
	catch(...)
	{
		strError = "ץȡ��ҳ��Ϣ�쳣";
		DeleteObjectEx(pHttpFile);	
		DeleteObjectExEx();
		return -7;
	}
	return 0;
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
int CWebBase::SendRequest(CHttpFile *pHttpFile,CString &strError,CString strHeader,CString strPostData,int iPostFlag)
{
	int iRet = 0;
	BOOL bRet = FALSE;
	try
	{
		if (iPostFlag == 0)
		{
			bRet = pHttpFile->SendRequest(strHeader);
		}
		else
		{
			char szPostData[2048] = {0};
			strncpy(szPostData,strPostData,2048);
			bRet = pHttpFile->SendRequest(strHeader,szPostData,strPostData.GetLength());
		}
		if( bRet == FALSE )
		{
			pHttpFile->Close();
			delete pHttpFile;
			strError = "SendRequestʧ��";
			return -2;
		}
		return 0;
	}
	catch (CInternetException * e )
	{
		if ( pHttpFile )
		{
			pHttpFile->Close();
			delete pHttpFile;
			pHttpFile = NULL;
		}
		strError = "SendRequest�쳣";
		e->Delete();
		return -4; 
	}
	catch (...)
	{
		if ( pHttpFile )
		{
			pHttpFile->Close();
			delete pHttpFile;
			pHttpFile = NULL;
		}
		strError = "SendRequest�쳣";
		return -5;
	}
}
/************************************************************************/
/*��ȡ����                                                            */
/************************************************************************/
int CWebBase::GetHttpConnectionEx(CString & strServer, CString & strObject, 
								CString & strHead,CString &strError,
								int iIsProxy,	//�Ƿ�ʹ�ô���
								int iProxyType,//��������
								CString strProxyIP,//����IP
								int iProxyPort//����˿�
								)
{
	CString strTemp = "";
	WORD    wdPort;
	if ( m_pConnection == NULL )
	{
		if (iIsProxy == 1)		//ʹ�ô���
		{
			INTERNET_PROXY_INFO   proxyinfo;
			try   
			{   
				strTemp.Format("%s:%d",strProxyIP,iProxyPort);
				proxyinfo.dwAccessType = INTERNET_OPEN_TYPE_PROXY;   
				proxyinfo.lpszProxy = strTemp;   
				proxyinfo.lpszProxyBypass = NULL;
				if(!m_Session.SetOption(INTERNET_OPTION_PROXY,(LPVOID)&proxyinfo,sizeof(INTERNET_PROXY_INFO)))   
				{   
					strError = "���ô���ʧ��";
					return -99;
				}
			}
			catch(...)
			{
				strError = "���ô����쳣";
				return -99;
			}
		}
		try
		{
			if (m_iHttpsFlag == 0)
			{
				wdPort = INTERNET_INVALID_PORT_NUMBER;
				int nStringLen = strServer.Find(":");
				if( nStringLen != -1 )
				{
					strTemp  =  strServer.Mid( nStringLen + 1 );
					wdPort = atoi((LPCTSTR)strTemp);
					strTemp = strServer.Mid( 0,  nStringLen  );
					m_pConnection =
						m_Session.GetHttpConnection( strTemp,
						INTERNET_FLAG_KEEP_CONNECTION,
						wdPort,
						NULL, 
						NULL );

				}
				else
				{
					m_pConnection =
						m_Session.GetHttpConnection( strServer,
						INTERNET_FLAG_KEEP_CONNECTION,
						INTERNET_INVALID_PORT_NUMBER,
						NULL, 
						NULL );
				}
			}
			else		//��HTTPSЭ��
			{
				m_pConnection =
					m_Session.GetHttpConnection((LPCTSTR)strServer,
					INTERNET_FLAG_NO_CACHE_WRITE,
					INTERNET_DEFAULT_HTTPS_PORT,
					NULL, 
					NULL);
			}
			if (m_pConnection == NULL)
			{
				strError = "��ȡ����ʧ��";
				return -3;
			}
		}
		catch (...)
		{
			if (m_pConnection)
			{
				m_pConnection->Close();
				delete m_pConnection;
				m_pConnection = NULL;
			}
			strError = "��ȡ�����쳣";
			return -99;
		}	
	}
	return 0;
}

/************************************************************************/
/*//��ѹ����                                                            */
/************************************************************************/
int CWebBase::ConvertData(CString strReponseHead,CString &strError,CString &strData)
{
	int nStringLen = -1;
	strReponseHead.MakeLower();
	//�Ǳ�ѹ�����ģ� Ҫ��ѹ
	if(strReponseHead.Find("gzip" ) != -1) 
	{                        
		nStringLen = strData.GetLength();
		if( gzuncompress(strData, nStringLen ) != 0 )
		{
			strError = "��ѹʧ��";
			return -5;
		}
	}
	if( strReponseHead.Find("utf-8") != -1 && strReponseHead.Find("jpeg" ) == -1 )
	{                        
		ConvertUtf8ToGBK( strData );
	}
	return 0;
}
/************************************************************************/
/*����HTTPSЭ������                                                     */
/************************************************************************/
void CWebBase::SetHttpsType(int iType)
{
	if (iType >= 1)
	{
		m_iHttpsFlag = 1;
	}
	else
	{
		m_iHttpsFlag = 0;
	}
}
/************************************************************************/
/* ɾ�����Ӷ����ͷ���Դ												*/
/************************************************************************/
void CWebBase::DeleteObjectEx(CHttpFile *pHttpFile)
{
	if ( pHttpFile )
	{
		pHttpFile->Close();
		delete pHttpFile;
		pHttpFile = NULL;
	}
}
/************************************************************************/
/* ɾ�����Ӷ����ͷ���Դ												*/
/************************************************************************/
void CWebBase::DeleteObjectExEx()
{
	try
	{
		if (m_pConnection)
		{
			m_pConnection->Close();
			delete m_pConnection;
			m_pConnection = NULL;
		}
	}
	catch (...)
	{
	}
	
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
DWORD CWebBase::GetDwflag()
{
	DWORD dwFlags = 0;
	if (m_iHttpsFlag == 1)			//�����HTTPSЭ��
	{
		dwFlags = INTERNET_FLAG_NO_CACHE_WRITE |
			INTERNET_FLAG_SECURE |
			INTERNET_FLAG_IGNORE_CERT_CN_INVALID |
			INTERNET_FLAG_IGNORE_CERT_DATE_INVALID ;
	}
	else
	{
		dwFlags = INTERNET_FLAG_KEEP_CONNECTION;
	}
	if(m_nNotDirect)
	{
		dwFlags = dwFlags | INTERNET_FLAG_NO_AUTO_REDIRECT;
	}
	return dwFlags;
}
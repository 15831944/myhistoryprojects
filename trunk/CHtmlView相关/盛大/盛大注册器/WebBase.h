#pragma once 
#include "afxinet.h"
#include "afxmt.h"
#include <math.h>
#pragma comment(lib,"wininet.lib")

//HTTP ������ GetPage

class CWebBase 
{

public:
        CWebBase();
        ~CWebBase();
public:

        //�û���Ϣ
        CString                 m_strServerURL;
        CString                 m_strCompany;
        int                     m_nElseWhereIP;

        //������Ϣ
        CInternetSession        m_Session;
        CHttpConnection  *      m_pConnection;

        CString                 m_strLanguage;
        int                     m_nNotDirect; //�Ƿ��ض���

        CMutex                  m_lock;   //�����Ժ�ͬ��
        CEvent                  m_even;   //�Ժ�֪ͨ��

        //���������صĲ�������,����ͷ
        //Ҫ�Ķ����������������ҵ�
        CString                 m_strWorkBuf;
        CString                 m_strResponHead;
        CString                 m_strRedirectUrl;
        CString                 m_strSetCookie;

		int						m_iHttpsFlag;	//�Ƿ�ΪHTTPSЭ��
		int						m_iPostFlag;	//POST��ʽ

public:
		
        //��Ϊ��¼,ˢ��,��ע��Ҫ�õ�,���Է�������
		int RequestGetPageExEx( CInternetSession & internetSession,
			CHttpConnection * & httpConncection,
			CString & strServer, CString & strObject, 
			CString & strHead,CString &strError,
			int iGetType=0,	//��ʾ��ȡ�ļ���1��ʾ��ȡ��֤��
			int iIsProxy=0,	//��������
			int iProxyType=0,	//�Ƿ�ʹ�ô���
			CString strProxyIP="",//����IP
			int iProxyPort=80,//����˿�
			CString strProxyUser="",//�����û���
			CString strProxyPwd=""//��������
			);
		/************************************************************************/
		/*��ȡҳ�����֤��ͼƬ                                                  */
		/************************************************************************/
		int RequestPostPageExEx( CInternetSession & internetSession,
			CHttpConnection * & httpConncection,
			CString & strServer, CString & strObject, 
			CString & strHead,CString & strPostData,
			CString &strError,
			int iGetType=0,	//��ʾ��ȡ�ļ���1��ʾ��ȡ��֤��
			int iIsProxy=0,	//��������
			int iProxyType=0,	//�Ƿ�ʹ�ô���
			CString strProxyIP="",//����IP
			int iProxyPort=80,//����˿�
			CString strProxyUser="",//�����û���
			CString strProxyPwd=""//��������
			);

		//��Ϊ��¼,ˢ��,��ע��Ҫ�õ�,���Է�������
		int RequestGetPageExExEx(
			CString & strServer, CString & strObject, 
			CString & strHead,CString &strError,
			int iGetType=0,	//��ʾ��ȡ�ļ���1��ʾ��ȡ��֤��
			int iIsProxy=0,	//��������
			int iProxyType=0,	//�Ƿ�ʹ�ô���
			CString strProxyIP="",//����IP
			int iProxyPort=80,//����˿�
			CString strProxyUser="",//�����û���
			CString strProxyPwd=""//��������
			);
		/************************************************************************/
		/*��ȡҳ�����֤��ͼƬ                                                  */
		/************************************************************************/
		int RequestPostPageExExEx(
			CString & strServer, CString & strObject, 
			CString & strHead,CString & strPostData,
			CString &strError,
			int iGetType=0,	//��ʾ��ȡ�ļ���1��ʾ��ȡ��֤��
			int iIsProxy=0,	//��������
			int iProxyType=0,	//�Ƿ�ʹ�ô���
			CString strProxyIP="",//����IP
			int iProxyPort=80,//����˿�
			CString strProxyUser="",//�����û���
			CString strProxyPwd=""//��������
			);
        int SetTimeOut( CInternetSession & internetSession,
                int  nTimeOutSecond );
        int  UploadFile(LPCTSTR strURL, LPCTSTR strLocalFileName);
        //Ϊ�˽���Big5,unicode�ı�������,��������������
        int TraditionTOSimple( CString & strHtml );
        int HtmlCodeToSimple( CString & strHtmlText );
        void ConvertGBKToUtf8(CString& strGBK) ;
        void ConvertUtf8ToGBK(CString& strUtf8) ;
        void URL_Encode(  CString  &  strSource );
        void URL_Decode(  CString  &  strSource );
		void SetHttpsType(int iType);
		void DeleteObjectEx(CHttpFile *pHttpFile);
		void DeleteObjectExEx();
		DWORD GetDwflag();
		CString URLEncode(CString sIn);
	private:
        int HtmlCodeToUniCodeByte( char *  pszHtmlcode, int iLen );
        int HtmlCodeToUniCodeByteReg( char *  pszHtmlcode, int iLen );		
		int SendRequest(CHttpFile *pHttpFile,CString &strError,CString strHeader,CString strPostData,int iPostFlag=0);
		int GetHttpConnection(CInternetSession & internetSession, CHttpConnection *&httpConncection, CString & strServer, CString & strObject, CString & strHead,CString &strError, int iIsProxy, /*�Ƿ�ʹ�ô��� */ int iProxyType,/*�������� */ CString strProxyIP,/*����IP */ int iProxyPort/*����˿� */ );
		int ConvertData(CString strReponseHead,CString &strError,CString &strData);
		int GetHttpConnectionEx(CString & strServer, CString & strObject, CString & strHead,CString &strError, int iIsProxy, /*�Ƿ�ʹ�ô��� */ int iProxyType,/*�������� */ CString strProxyIP,/*����IP */ int iProxyPort/*����˿� */ );
		
};



#pragma once
#include "WebBase.h"
#include "ThreadManager.h"
#include "ASCIIConver.h"
#include "afxwin.h"
#include "RasDialMgr.h"
#include "datatype.h"
#include <mshtml.h>
//#include <afxhtml.h>
//#include <atlbase.h>
#include <oleacc.h>
//#include <MsHtmcid.h>
//#pragma comment(lib,"comsupp.lib")
//#pragma comment(lib,"comsuppw.lib")
//#pragma comment(lib,"kernel32.lib")
//51JOB�� WEB POST��
class C51JobWebPost:public CWebBase
{
public:
	C51JobWebPost();
	~C51JobWebPost();
	bool UserRegister();
	static void ThreadFunc(LPVOID lParam);
	CString GetKeyString(CString &srcStr,CString &strBefore,CString &strAfter);
	void RegisterFunc();
	void BeginRegister();
	void ShowMessage(CString strMsg);
	CString NativeToAscii(CString strSrc);
	static void ShowMessageEx(CString strMsg);
	int GetGateWayIP(CString &strIP);
	void SaveToExcel(RegisterPack rgPack);
	BOOL CreatExcelDb(CString dbpath,CString tableName,CString &strError);
	CString GetExcelDriver();
	int WriteToBase(RegisterPack rgPack,int iFlag);
	int GetHtmlCtrl(IHTMLDocument2 *pDoc2);
	IHTMLDocument2* GetDocInterface(HWND hWnd);
	CString GetElementValue(CComPtr<IHTMLElement> pElement);
	int ChangeMacAddr();
	int ReOpenRegWindow();
	CString MacAddrRand();
	int DisableKey(int iKeyNum);
	int GetFirstUsefulKey();
private:
	CThreadManager m_ThreadManager;
	bool m_LoginFlag;
	bool m_OpenFreshPageFlag;
	int m_ExceptionNum;
	int m_FreshTimes;		//��ˢ�´���
	CString m_strHeader;            //����ͷ
	CString m_ResumeIDStr;
	CString m_FreshPageURL;         //ˢ��ҳURL
	CString m_strUserName;
	CString m_strUserPwd;
	int m_TimeInterval;             //ˢ�¼��
	int m_iCount;
	int m_nErrorTime;		//�������
	int m_iCheckFlag;
	
	//���ļ�����Ϣ
	CString m_ChHPNation;           //��ͥ�����
	CString m_ChHPCity;             //��ͥ����
	CString m_ChHPNumber;           //���õ绰����
	CString m_ChFPNation;           //��˾�����
	CString m_ChFPCity;             //��˾����
	CString m_ChFPNumber;           //��˾�ܻ�
	CString m_ChFPExtension;        //�ֻ���
	CString m_ChMPNation;           //�ƶ��绰�����
	CString m_ChMobile;             //�ƶ��绰
	CString m_ChEmailAdd;           //Email
	CString m_ChResumeID;           //����ID
	CString m_ChHelpID;             //����ID

	//Ӣ�ļ�����Ϣ
	CString m_EnHPNation;
	CString m_EnHPCity;
	CString m_EnHPNumber;
	CString m_EnFPNation;
	CString m_EnFPCity;
	CString m_EnFPNumber;
	CString m_EnFPExtension;
	CString m_EnMPNation;
	CString m_EnMobile;
	CString m_EnEmailAdd;
	CString m_EnResumeID;
	CASCIIConver m_Conver;	
	int m_iNeedChageIP;
	int m_iIpSuccNum;		//��IPע��ɹ���
	CString m_strOldIP;		//�ɵ�����IP
	int m_iCodeErrorNum;

	CString m_strVerify;
	CString m_strTrans;
	CString m_strMac;
	int m_iID;
	HWND m_hwnd;
	int m_iIsNeedChangeMac;
};
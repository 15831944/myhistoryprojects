#pragma once
#include "WebBase.h"
#include "ThreadManager.h"
#include "ASCIIConver.h"
#include "afxwin.h"
#include "RasDialMgr.h"
#include "datatype.h"
//51JOB�� WEB POST��
class C51JobWebPost:public CWebBase
{
public:
	C51JobWebPost();
	~C51JobWebPost();
	bool UserRegister(RegisterPack &rgPack);
	static void ThreadFunc(LPVOID lParam);
	CString GetKeyString(CString &srcStr,CString &strBefore,CString &strAfter);
	void RegisterFunc();
	void BeginRegister();
	void ShowMessage(CString strMsg);
	CString NativeToAscii(CString strSrc);
	static void ShowMessageEx(CString strMsg);
	int GetGateWayIP(CString &strIP);
	int GetQQCheckCodeBMP(CString strUrl);
	int StringSplit(CString srcStr,CString strArray[],char splitChar);
	void GenUserName(CString &strName);
	void GenAddress(CString &strProvince,CString &strAddress,CString &strMailCode,CString &strCity);
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
};
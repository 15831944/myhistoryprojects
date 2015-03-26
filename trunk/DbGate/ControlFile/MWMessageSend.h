#pragma once
#include "dataType.h"
#include "TcpClient.h"
#define  USE_DLL___    //�����Ƿ�ͨ����̬�ⷢ��
//�����������
#define  MSG_ERROR_INIT			1
#define  MSG_ERROR_LOGIN		2
#define  MSG_ERROR_SEND			3
#define  MSG_ERROR_RECVMO		4
#define  MSG_ERROR_RECVREPORT	5
#define  MSG_ERROR_KEEPONLINE	6
#define  MSG_ERROR_MODIFYPWD	7
#define  MSG_ERROR_CHECKFEE		8

//��������
#define  OPTYPE_LOGIN			1
#define  OPTYPE_SEND			2
#define  OPTYPE_RECVMO			3
#define  OPTYPE_RECVREPORT		4
#define  OPTYPE_MODIFYPWD		5
#define  OPTYPE_CHECKFEE		6

#define MWGATE_DLL_NAME "EMWGateway.dll"	//��̬����
#define MWGATE_DLL_NAME_SMEIDLL "SMEIDll.dll"	//��̬����

#ifdef USE_DLL___
class CMWMessageSend
#else
class CMWMessageSend : CTcpClient
#endif
{
public:
	CMWMessageSend(void);
	~CMWMessageSend(void);
	int SendMtMessage(LPCTSTR sMsg, LPCTSTR sDestAddrs,int byUserNum,LPCTSTR sSubPort,unsigned char* byMsgID,CString &strError);//����������Ϣ	
	BOOL InitInterface();	//��ʼ���ӿ�
	BOOL ExitInterface();	//�ͷŽӿ�
	void SetParams(CString strServerIP,int iPort,CString strUserID,CString strPwd);	//�����û���Ϣ
	int ConnectAndLogin(CString &strError); //���Ӳ���¼������
	int GetMoMessges(DeliverPackList &recvPackList,CString &strError );  //��������
	int GetMsgReports(ReportPackList &reportPackList,CString &strError );//��״̬����
	int ChangePassword(CString strNewPwd,CString &strError);	//�޸�����
	int GetFee(int &iUsed,int &iLeft,CString &strError);	//��ѯ������ʣ������
	int KeepOnline(CString &strError);		//��������
	void Disconnect();						//�Ͽ�����
	void SetCmdTypeEx(int iCmdType,int iRptFlag);
private:
	int m_sock;		//�������ͨѶ��SOCKET
	HMODULE m_handle;	//���ض�̬��ľ��
	HMODULE m_handle_smeidll;
	char m_sServerIP[20]; //������IP
	int m_iport;			//�������˿�
	char m_sUserID[32+1];	//�û�ID
	char m_sPassWord[32+1];	//�û�����
	/************************************************************************/
	/*                                                                      */
	/************************************************************************/
	CString GetServerErrorStr(int iErrorCode);	//���ط���������
	CString GetLoginErrorStr( int iErrorCode );	//���ص�¼�����ַ���
	CString GetSendMsgErrorStr( int iErrorCode );
	CString GetMoReportErrorMsg(int iErrorCode);
	int AddRePortPack(ReportPackList &reportPackList, CString strMsg);
	int AddDeleverPack(DeliverPackList &recvPackList,CString strMsg);
	void TranMsgIdI64ToChar(unsigned char* MsgId, __int64 IMsgId);
	int TranMsgId(unsigned char *pSrc, char *MsgId);
	/************************************************************************/
	/* ����Ϊ��̬�⺯��                                                      */
	/************************************************************************/
#ifdef USE_DLL___
	typedef int (__stdcall *FnMongateConnect)(const char* pszIP, int iPort,
		const char* pszAccount, const char* pszPwd);
	//typedef int (__stdcall *FnMongateCsSendSms)(int sock, const char* pszMobis, const char* pszMsg,
	//	int iMobiCount, char* pszSN);
	typedef int (__stdcall *FnMongateCsSPSendSms)(int sock, const char* pszMobis,
		const char* pszMsg, int iMobiCount,const char* pszSubPort,char* pszSN);
	typedef void (__stdcall *FnMongateDisconnect)(int sock);
	//typedef int (__stdcall *FnMongateCsGetSms)(int sock, char strMsg[500][255]);
	typedef int (__stdcall *FnMongateCsGetSmsEx)(int sock, char *strMsg);//[1600*500]
	//typedef int (__stdcall *FnMongateSetCmdType)(int iCmdType);
	typedef int (__stdcall *FnMongateQueryBalance)(int sock);
	typedef int (__stdcall *FnMongateQueryUsed)(int sock);
	typedef int (__stdcall *FnMongateTestConn)(int sock);		//ά������
	typedef int (__stdcall *FnMongateCsGetStatusReportEx)(int sock, char *strMsg);
	typedef int (__stdcall *FnMGhexChrTosrcStr)(const char* srcMsg,char* retMsg);
	typedef int (__stdcall *FnMongateChangePwd)(int sock, const char* pszAccount,
		const char* pszOldPwd, const char* pszNewPwd);
	typedef int (__stdcall *FnSetCmdType)(int iCmdType,int iRptFlag);
	
	FnMongateConnect MongateConnect;
	FnMongateCsSPSendSms MongateCsSPSendSms;
	FnMongateDisconnect MongateDisconnect;
	FnMongateCsGetSmsEx MongateCsGetSmsEx;
	FnMongateQueryBalance MongateQueryBalance;
	FnMongateQueryUsed MongateQueryUsed;
	FnMongateCsGetStatusReportEx MongateCsGetStatusReportEx;
	FnMGhexChrTosrcStr MGhexChrTosrcStr;
	FnMongateChangePwd MongateChangePwd;
	FnMongateTestConn MongateTestConn;
	FnSetCmdType	SetCmdType;
#else
#endif
	/////////////////////////////////////////////////////
	//��̬�⺯���ӿ�
	/////////////////////////////////////////////////////
// 	typedef int (FAR __stdcall *_IFInitInterface)(DWORD dwCodeProtocol, DWORD dwDriverProtocol, LPCTSTR pDriverParam);
// 	typedef int (FAR __stdcall *_IFExitInterface)();
// 	typedef int (FAR __stdcall *_Login_R)(LPCTSTR Account, LPCTSTR Password);
// 	typedef int (FAR __stdcall *_SendSms)
// 		(
// 		const char* sOrgAddr,		//Դ��ַ
// 		const char* sPhones,		//�ֻ��ţ����һ�η���100������Ӣ�Ķ��ŷָ�
// 		unsigned char ReportFlag,	//�Ƿ�Ҫ״̬����,1��Ҫ��0����Ҫ
// 		const char *sMsg,		    //��������
// 		unsigned char *sMsgID,		//���ص�msgId
// 		unsigned char* FCS		    //���صĴ�����
// 		);
// 
// 	typedef int (FAR __stdcall *_RecvSms)
// 		(
// 		char *sOrgAddr,			//Դ��ַ
// 		char *sPhone,				//�ֻ���
// 		unsigned char *sMsgID,		//���л�״̬�����msgid
// 		unsigned char *ReportFlag, //��Ϣ��־ 0:״̬���� 1:����
// 		char *sTimeStamp,			//���յ���ʱ�� ��ʽ��2009-10-28 12:12:12	
// 		char *sSubmitdate,			//�ύʱ��   200910281212
// 		char *sDonedate,			//����ʱ��200910281212
// 		char *sErrorCode,			//״���������
// 		char *sMsg				    //������Ϣ
// 		);
// 	_IFInitInterface IFInitInterface = NULL;
// 	_IFExitInterface IFExitInterface = NULL;
// 	_Login_R         Login_R = NULL;
// 	_SendSms         SendSms = NULL;
// 	_RecvSms         RecvSms = NULL;

};
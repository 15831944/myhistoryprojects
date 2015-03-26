#pragma once
#include "mwmessagesend.h"
#include "ADOFile\AdoRecordSet.h"
#include "ASCIIConver.h"
#include "DataBaseCtrl.h"
#include "KeyWord.h"

class CMWMessageSendEx :public CMWMessageSend
{
	typedef struct tagThreadControl
	{
		CWinThread* pThread;
		BOOL ThreadExit;
		int iThreadNO;
	} ThreadControl;
	typedef struct tagThreadParam
	{
		CMWMessageSendEx* pThis;
		int iThreadNO;
	} ThreadParam;

public:
	CMWMessageSendEx(void);
	~CMWMessageSendEx(void);
public:
	void Run();
	void Stop( );
	void AppExit();
	int GetRunStatus();
	void SetRunStatus( int iFlag);
	void CheckFee();
	int IsNeedToReRunOnError();
	int IsNeedToReRunOnTime();
	void ReInitParams();
	void InitParams();
	void SetFrameHwnd(HWND hwnd);
	void AddSingleSend(CString strPhone,CString strContent,CString strSubport);
private:
	/************************************************************************/
	/*                                                                      */
	/************************************************************************/
	bool IsNeedToRecv(unsigned int iDelay);		//�Ƿ���Զ�������
	void SetLastActiveTime( );					//�ϴ�����������ʱ��
	bool IsNeedReConn(unsigned int iDelay);		//�Ƿ���Ҫ����	
	bool IsNeedToRecvReport(unsigned int iDelay);
	bool IsNeedToRecvMo(unsigned int iDelay);
	void SetLastRecvReportTime();
	void SetLastRecvMoTime();

	/************************************************************************/
	/*                                                                      */
	/************************************************************************/
	int MessageOperate(int iOptype,int &iErrorType,CString &strError,KfSubmitPack &mtPack,DeliverPackList &recvList,ReportPackList &reportList);	//�������������

	/************************************************************************/
	/*                                                                      */
	/************************************************************************/
	static void AddSendResultThread(LPVOID lpParam);	//��ӷ��ͽ���߳�
	void AddSendResultProc(int iThreadNO);//��ӷ��ͽ��ִ�к���
	static void MessageSendThreadProc(LPVOID lpParam);		//�����̺߳���
	void MessageSendProc();			//�����߳�ִ�к���

	static void InsertBackThreadProc(LPVOID lpParam);		//�ش��̺߳���
	void InsertBackProc();			//�ش��߳�ִ�к���
	
	static void RecvThreadProc(LPVOID lpParam);		//�����̺߳���
	void RecvProc();			//�����߳�ִ�к���

	void Delay(int iMiniSecond,BOOL *bExit);
	void OnActiveThreadExit(int iFlag);
	/************************************************************************/
	/*                                                                      */
	/************************************************************************/
	void AddKfSubmitPack(KfSubmitPack &submitPack);
	void AddKfSubmitPackEx(KfSubmitPack &submitPack);	//�嵽��ͷ��
	int GetKfSubmitPack(KfSubmitPack &submitPack);
	void AddRealSendPack(KfSubmitPack &sendPack);
	int GetRealSendPack(KfSubmitPack &sendPack);
	/************************************************************************/
	/*                                                                      */
	/************************************************************************/
	void ConvertPhoneAddMessage(KfSubmitPack &mt,int iFlag,CStringList &strIllegalPhones);
	void AddSendPackList( CString &strPhoneList,KfSubmitPack &mt,int iCount,int iSplitLen);
	/************************************************************************/
	/*                                                                      */
	/************************************************************************/
	BOOL ReadWaitMessage(CDataBaseCtrl &dataBaseCtrl);
	/************************************************************************/
	/*                                                                      */
	/************************************************************************/		
	int NextOptJudge(int iErrorCode,int iErrorType);	
	int IsRightSendTime(CString strSendTime,CString strStartTime,CString strEndTime);	
	/************************************************************************/
	/*                                                                      */
	/************************************************************************/
	bool IsOverLastErrorTime(unsigned int iErrorStopTime );
	void SetLastErrorTime();
	int UnionSameSingleMessage(CDataBaseCtrl &dataBaseCtrl);
	int AddSendResult(CDataBaseCtrl &dataCtrl,KfSubmitPack &mt,int iFlag);
	int StringSplit(CString srcStr,CString strArray[],char splitChar);
	void GetMatchMtPack(KfSubmitPack &mt,int iLocalID);
	void LoadKeys();
	bool IsOverLastReconnTime(unsigned int iReconnInterval);
	void AddtoSendHistory(CDataBaseCtrl &dataCtrl,KfSubmitPack &mt,int iFlag,CString strError);
	void WriteBufToFile(KfSubmitPack &mt);
	void ReadBufFromFile();	
	int ComputePhoneType(CString &strPhone);
	void SetStatusInfoEx(int iIndex,int iCount);
private:
	long m_LastRecvMoTime;
	CCriticalSection m_LastRecvMoTimeLock;

	long m_LastRecvReportTime;
	CCriticalSection m_LastRecvReportTimeLock;

	DWORD m_LastActiveTime;		//�ϴ�����ʱ��
	CCriticalSection m_LastActiveTimeLock;	//�ϴ�����ʱ����

	KfSubmitPackList m_SubmitPackList;		//���ͻ���
	CCriticalSection m_SendListLock;		//����������

	KfSubmitPackList m_RealSendList;		//������������
	CCriticalSection m_RealSendLock;		//��������������
	
	KfSubmitPackList m_SendHistoryList;		//������ʷ
	CCriticalSection m_SendHistoryLock;		//������ʷ��

	ThreadControl m_SendThreadContrl;	//�����߳�
	ThreadControl m_AddResultThreadContrl[5];	//���淢�ͽ���߳�
	ThreadControl m_RecvThreadContrl;	//�����߳�
	ThreadControl m_InsertBackContrl;	//�ش��߳�
	int m_iActiveThreadNum;
	CCriticalSection m_activeNumLock;
	bool m_CheckFeeFlag;
	int m_iRunStatus;
	long m_iLastErrorTime;		//���һ�η��ͳ���ʱ��
	int m_iIsSendThreadRun;
	int m_iIsRecvThreadRun;
	CCriticalSection m_IsSendThreadRunLock;
	CCriticalSection m_IsRecvThreadRunLock;
	/************************************************************************/
	//ͨ������	
	int m_iSplitLength;			//�ַ�����ֳ���[�ƶ�]
	int m_iSplitLengthUnicom;			//�ַ�����ֳ���[��ͨ]
	int m_iSplitLengthChinaCom;			//�ַ�����ֳ���[����]
	int m_iSplitLengthChina;			//�ַ�����ֳ���[С��ͨ]
	int m_iPhoneSplitLen;		//����ÿ���������

	CString m_dbSQLServer;		//���ݿ������
	CString m_dbSQLName;		//���ݿ���
	CString m_dbSQLUser;		//��¼�˻�
	CString m_dbSQLPwd;			//��¼����

// 	CString m_dbAccessPath;		//���ݿ��ļ�·��
// 	CString m_dbAccessPwd;		//���ݿ�����
	
	CString m_strAccount;		//��¼�˻�
	CString m_strAccPwd;		//�˻�����

	CString m_strServerIP;		//������IP
	int		m_iServerPort;		//�������˿�

	int m_iSendInterval;		//�����߳�ִ�м��
	int m_iBufNullWaitTime;		//������Ϊ����ͣʱ��
	
	int m_iAddNullWaitTime;		//�����ݿ�Ϊ��ʱ��ͣʱ��
	int m_iUseSubPort;			//�Ƿ�ʹ���Ӷ˿ڱ�־

	CString m_strMobileList;		//�Ŷ�
	CString m_strUnicomList;
	CString m_strTelecomList;
	int		m_iDiffMobile;		//�����ƶ�����ͨ�Ŷ�

	//�Ƿ����״̬���漰���еı�־
	int m_iIsRecvReport;
	int m_iIsRecvMo;
	int m_iRecvMoInterval;		//��������ʱ����
	int m_iRecvReportInterval;	//����״̬����ʱ����

	int m_iAutoReRun;			//�Զ�������(���ݿ�����ʱ���)
	CString m_strRunTime;		//����ʱ�������
	int m_iErrorStopTime;		//����ֹͣʱ��
	int m_iCheckKey;			//���Ƿ��ؼ���

	int m_iIsReconn;			//�Ƿ��Զ�����
	int m_iReconnInterval;		//�Զ��������
	long m_iLastReconnTime;		//�ϴ��Զ�����ʱ��
	int m_iTestConInterval;		//�������Ӽ��
	//��ӷ��ͼ�¼�߳�
	int m_iAddHistThreadNum;	//�����ʷ�߳���
	int m_iAddHistInterval;		//ִ�м��
	int m_iAddHistNullWait;		//����ʷ��Ҫд��ʱ��ͣ
	int m_iNumSendStop;			//����ʷ���¼����*��ʱ��ͣ����
	
	int m_iDbType;			//���ݿ�����
	int m_iDbPort;			//���ݿ�˿ں�,ֻ��MYSQL��ORACLEʱ��Ҫ
	CKeyWord m_keyWord;		//�Ƿ��ؼ��ʼ�����
	map<int,KfSubmitPack>  m_SendMatchList;//��Ϣ�ϲ�����(ƥ��)

	HWND m_hWnd;			//�����ھ��

	CString m_strLastSingleMsg;	//��һ�ε�������Ϣ����
	int m_iSameSingleSendTime;	//������ͬ��Ϣ��������
	int m_iMostSameSingleSendTime;	//��������������ͬ��Ϣ��������
};

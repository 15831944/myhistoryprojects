#pragma once
//״̬����ṹ��
#include <afxmt.h>
#include <afxtempl.h>
#include "ASCIIConver.h"
#define STATUS_RUN_NORMAL 0
#define STATUS_STOP_ERROR 1
#define STATUS_STOP_MANUAL   2
#define STATUS_STOP_OVERRUNTIME 3
#define STATUS_STOP_NORMAL 4

#define USERMSGSTART	 WM_USER + 500
#define USERWINDOWMSG_1  USERMSGSTART + 1
#define USERWINDOWMSG_2  USERMSGSTART + 2
#define USERWINDOWMSG_3  USERMSGSTART + 3
#define USERWINDOWMSG_4  USERMSGSTART + 4
#define USERWINDOWMSG_5  USERMSGSTART + 5
#define USERWINDOWMSG_6  USERMSGSTART + 6
#define USERWINDOWMSG_7  USERMSGSTART + 7
#define USERWINDOWMSG_8  USERMSGSTART + 8
#define USERWINDOWMSG_9  USERMSGSTART + 9
#define USERWINDOWMSG_10 USERMSGSTART + 10
#define USERWINDOWMSG_11 USERMSGSTART + 11
#define USERWINDOWMSG_12 USERMSGSTART + 12

#define USERMSGSHOWINFO USERMSGSTART + 13
#define USERMSGSHOWDISPINFO USERMSGSTART + 14
#define WM_USERMSGSEND		USERMSGSTART + 15
#define WM_USERMSGSTATUS		USERMSGSTART + 85	//״̬����Ϣ��
typedef struct ReportPack
{
	CString strDate;		//��������
	CString strTime;		//����ʱ��
	CString strMsgNo;		//��Ϣ���
	CString strReserve;		//����
	CString strMsgStatus;	//��Ϣ״̬
	CString strErrorCode;	//��Ϣ������	
	ReportPack()
	{
		strDate.Empty();
		strTime.Empty();
		strMsgNo.Empty();
		strReserve.Empty();
		strMsgStatus.Empty();
		strErrorCode.Empty();
	}
}ReportPack;

//������Ϣ�ṹ��
typedef struct DeliverPack
{
	CString strDate;		//��������
	CString strTime;		//����ʱ��
	CString strOrgAddr;		//�ֻ�����
	CString strDestAddr;	//���շ�����
	CString strReserve;		//����
	CString strMsg;			//��Ϣ����
	DeliverPack()
	{
		strDate.Empty();
		strTime.Empty();
		strOrgAddr.Empty();
		strDestAddr.Empty();
		strReserve.Empty();
		strMsg.Empty();
	}
}DeliverPack;

typedef struct KfSubmitPack
{
	int m_iSendType;				//��������     
	int m_iCount;					//���պ�����
	int m_iCountA;					//��ʱ���պ�����
	int m_iPhoneType;				//�����Ŷ�
	int m_iTryTimes;				//���Դ���
	int m_iMsgLen;					//��Ϣ����
	int m_iPrecedence;				//�������ȼ�

	int m_iLocalID;					//����ID(INT��)
	int m_iReverse1;				//�����ֶ�1
	int m_iReverse2;				//�����ֶ�2
	int m_iReverse3;				//�����ֶ�3
	int m_iReverse4;				//�����ֶ�4
	int m_iReverse5;				//�����ֶ�5
	int m_iSendResultFlag;			//���ͽ����0�ɹ���1ʧ��
	CString m_strSendType;			//��������		
	CString m_strUserNumber;		//���շ������б�
	CString m_strUserNumberA;		//��ʱ���շ������б�
	CString m_strPtMsgId;			//��Ϣ���ر�ʶ
	CString m_strSpMsgId;			//��ϢID(���ͳɹ������Ϣ���)
	CString m_strMsg;				//��������
	CString m_strCreatTime;		//��Ϣ����ʱ��
	CString m_strSubTime;			//��Ϣ�ύ�����ݿ�ʱ��
	COleDateTime m_timeSendTime;			//����ʱ��

	CString m_strOperatorID;		//����ԱID
	CString m_strServiceID;			//����ID
	CString m_strTaskName;			//������
	CString m_strLocalID;			//��Ϣ����ID
	CString m_strSubOprationType;	//�Ӳ�������
	CString m_strSubPort;			//�Ӷ˿�

	CString m_strUserId;			//�����ֶ�
	CString m_strMsgStatus;			//�����ֶ�1
	CString m_strRecvStatus;			//�����ֶ�2
	CString m_strMainPort;			//�����ֶ�3
	CString m_strFailReason;			//�����ֶ�4
	CString m_strReverse5;			//�����ֶ�5
	CString m_strReverse6;			//�����ֶ�6
	CString m_strIdList;			//���ڱ���ϲ������ϢID����
	KfSubmitPack()
	{
		m_iSendType = 0;	
		m_iCount = 0;					//���պ�����
		m_iCountA = 0;					//��ʱ���պ�����
		m_iPhoneType = 0;				//�����Ŷ�
		m_iTryTimes = 0;				//���Դ���
		m_iMsgLen = 0;					//��Ϣ����
		m_iPrecedence = 0;				//�������ȼ�

		m_iLocalID = 0;				//�����ֶ�6
		m_iReverse1 = 0;				//�����ֶ�1
		m_iReverse2 = 0;				//�����ֶ�2
		m_iReverse3 = 0;				//�����ֶ�3
		m_iReverse4 = 0;				//�����ֶ�4
		m_iReverse5 = 0;				//�����ֶ�5		
		m_iSendResultFlag = 0;
		m_strSendType.Empty();			//��������
		m_strUserId.Empty();			
		m_strUserNumber.Empty();		//���շ������б�
		m_strUserNumberA.Empty();		//��ʱ���շ������б�
		m_strPtMsgId.Empty();			//��Ϣ���ر�ʶ
		m_strSpMsgId.Empty();			//��Ϣ��ʶ2  //��ϢID(���ͳɹ������Ϣ���)
		m_strMsg.Empty();				//��������
		m_strCreatTime.Empty();			//��Ϣ����ʱ��
		m_strSubTime.Empty();			//��Ϣ�ύ�����ݿ�ʱ��
		//m_timeSendTime.Empty();			//����ʱ��

		m_strOperatorID.Empty();		//����ԱID
		m_strServiceID.Empty();			//����ID
		m_strTaskName.Empty();			//������	
		m_strLocalID.Empty();			//��Ϣ����ID	
		m_strSubOprationType.Empty();	//�Ӳ�������	
		m_strSubPort.Empty();			//�Ӷ˿�

		m_strMsgStatus.Empty();			//�����ֶ�1
		m_strRecvStatus.Empty();			//�����ֶ�2
		m_strMainPort.Empty();			//�����ֶ�3
		m_strFailReason.Empty();			//�����ֶ�4
		m_strReverse5.Empty();			//�����ֶ�5
		m_strReverse6.Empty();			//�����ֶ�6
		m_strIdList.Empty();
	}

}KfSubmitPack;

typedef CArray<DeliverPack, DeliverPack> DeliverPackList;
typedef CArray<ReportPack, ReportPack> ReportPackList;
typedef CArray<KfSubmitPack, KfSubmitPack> KfSubmitPackList;
extern CCriticalSection g_DisListLock;
extern CListCtrl g_DispList;
extern CListCtrl g_RecvList;
extern CListCtrl g_SentList;
extern CListCtrl g_ReportList;
extern int g_iSendBufLen;
extern int g_iIsNeedToLog;
extern int g_iDebugLog;		//д������־��־
extern int g_iServerType;	//����ƽ̨ѡ��0Ϊ��ܰͨƽ̨��1ΪSIƽ̨
extern int g_iReportFlag;
extern CASCIIConver g_Conver;
extern int g_iBalance;	//���
extern CString g_strWindowName;
extern CString g_strVirsion;
extern CString g_strServiceName;
extern HWND g_hwndClose;		//�رմ��ھ��
extern void WriteConsoleInfo(CString srcStr);	//д����̨��Ϣ
extern void WriteLog(CString srcStr,int iFlag);
extern void WriteLogEx(CString srcStr,int iFlag);//д������־
extern void AddToDispListShow(CString showStr,int iFlag=0);		//��ʾ��Ϣ
extern void AddToRecvListShow(CString strTime,CString orgAddr,CString strContent,CString strDestAddr );
extern void AddToSentListShow(CString strMsgID,CString destAddr,CString strContent,int iMsgLen,int iUserNum,int iPhoneType,CString strStatus,int iFlag,CString strOptId );
extern void AddToRePortListShow( CString strTime,CString strMsgID,CString strState );

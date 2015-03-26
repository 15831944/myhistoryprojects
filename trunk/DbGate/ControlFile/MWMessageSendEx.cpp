#include "StdAfx.h"
#include ".\mwmessagesendex.h"
#include "UserFile.h"
#include "../DialogFile/DbGateClose.h"
#include <io.h>
#include <direct.h>

CCriticalSection g_DisListLock;
int g_iIsNeedToLog = 0;
int g_iDebugLog = 0;
int g_iServerType = 0;	//����ƽ̨ѡ��0Ϊ��ܰͨƽ̨��1ΪSIƽ̨
int g_iReportFlag = 0;
CASCIIConver g_Conver;
int g_iSendBufLen = 0;			//���ͻ�������С
CString g_strServiceName = "";
/************************************************************************/
/*                                                                      */
/************************************************************************/
CMWMessageSendEx::CMWMessageSendEx(void)
{
	m_iIsSendThreadRun = 0;
	m_iIsRecvThreadRun = 0;
	m_iPhoneSplitLen = 1000;
	m_iIsRecvReport = 1;
	m_iIsRecvMo = 1;
	m_iDiffMobile = 0;
	SetRunStatus(STATUS_STOP_NORMAL);		//������״̬Ϊֹͣ
	if(InitInterface()== FALSE)
	{
        AfxMessageBox("��ʼ���ӿ�ʧ��,ϵͳ�Ҳ��� EMWGateway.dll");
		exit(0);
	}
	m_iTestConInterval = 0;
	m_iActiveThreadNum = 0;
	int i = 0;
	for (i=0;i<5;i++)
	{
		m_AddResultThreadContrl[i].ThreadExit = TRUE;
	}
	//��ʼ��������ͬ������������
	m_strLastSingleMsg = "";
	m_iSameSingleSendTime = 0;
	m_iMostSameSingleSendTime = 15;
	InitParams();
	//��ʼ���˻���Ϣ
	SetParams(m_strServerIP,m_iServerPort,m_strAccount,m_strAccPwd);
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
CMWMessageSendEx::~CMWMessageSendEx(void)
{
	ExitInterface();
	CUserFile *pFile = NULL;
	pFile = CUserFile::GetInstance();
	if (pFile)
	{
		delete pFile;
		pFile = NULL;
	}
// 	CConsolePrinter *pConsole = CConsolePrinter::Instance();
// 	if (pConsole)
// 	{
// 		delete pConsole;
// 		pConsole = NULL;
// 	}
	Sleep(20);
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
bool CMWMessageSendEx::IsNeedToRecvMo(unsigned int iDelay)
{
	DWORD t2 = 0;
	t2 = ::GetTickCount();
	DWORD tSpan = 0;
	m_LastRecvMoTimeLock.Lock();
	tSpan = t2 - m_LastRecvMoTime;
	m_LastRecvMoTimeLock.Unlock();
	if (tSpan > iDelay * 1000) //iDelay ��
	{
		return true;
	}
	else
	{
		return false;
	}
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
void CMWMessageSendEx::SetLastRecvMoTime()
{
	m_LastRecvMoTimeLock.Lock();
	m_LastRecvMoTime = ::GetTickCount();
	m_LastRecvMoTimeLock.Unlock();
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
bool CMWMessageSendEx::IsNeedToRecvReport(unsigned int iDelay)
{
	DWORD t2 = 0;
	t2 = ::GetTickCount();
	DWORD tSpan = 0;
	m_LastRecvReportTimeLock.Lock();
	tSpan = t2 - m_LastRecvReportTime;
	m_LastRecvReportTimeLock.Unlock();
	if (tSpan > iDelay * 1000) //iDelay ��
	{
		return true;
	}
	else
	{
		return false;
	}
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CMWMessageSendEx::SetLastRecvReportTime()
{
	m_LastRecvReportTimeLock.Lock();
	m_LastRecvReportTime = ::GetTickCount();
	m_LastRecvReportTimeLock.Unlock();
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CMWMessageSendEx::SetLastActiveTime( )
{
	m_LastActiveTimeLock.Lock();
	m_LastActiveTime = ::GetTickCount();
	m_LastActiveTimeLock.Unlock();
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
//DelayΪ�������,�ж��Ƿ���Ҫ��������
bool CMWMessageSendEx::IsNeedReConn(unsigned int iDelay)
{
	try
	{
		DWORD t2 = 0;
		t2 = ::GetTickCount();
		DWORD tSpan = 0;

		m_LastActiveTimeLock.Lock();
		tSpan = t2 - m_LastActiveTime;
		m_LastActiveTimeLock.Unlock();

		if (tSpan > iDelay * 1000 ) //1����
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	catch (...)
	{
	}
	return true;
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
int CMWMessageSendEx::MessageOperate(int iOptype,int &iErrorType,CString &strError,KfSubmitPack &mtPack,DeliverPackList &recvList,ReportPackList &reportList)
{
	int iRet = -99;
	try
	{
		iRet = -99;
		switch (iOptype)
		{
		case OPTYPE_LOGIN:
			{
				iErrorType = MSG_ERROR_KEEPONLINE;
				if (IsNeedReConn(m_iTestConInterval))		//1���Ӽ�������һ��
				{
					iRet = KeepOnline(strError);
					if (iRet < 0)
					{
						Disconnect();
						m_LastActiveTime = ::GetTickCount() - 10000000;
						iErrorType = MSG_ERROR_LOGIN;
						iRet = ConnectAndLogin(strError);
					}
					else
					{
						strError = "�������ӳɹ�";
						//m_CheckFeeFlag = true;
						SetLastActiveTime();
					}
				}
				else
				{
					strError = "���輤������";
					iRet = 1;
				}
				break;
			}
		case OPTYPE_SEND:
			{
				iErrorType = MSG_ERROR_SEND;
				unsigned char szMsgNO[50]= {0};		
				CString strDestAddr = mtPack.m_strUserNumber;
				strDestAddr.Trim();
				strDestAddr.Replace(' ',',');
				if ( m_iUseSubPort == 0 )	//��ʹ���Ӷ˿�
				{
					mtPack.m_strSubPort = "*";
				}
				iRet = SendMtMessage(mtPack.m_strMsg,strDestAddr,mtPack.m_iCount,mtPack.m_strSubPort,szMsgNO,strError);
				if (iRet == 0)	//���ͳɹ�
				{
					mtPack.m_strSpMsgId.Format("%s",szMsgNO);		
				}
				break;
			}
		case OPTYPE_RECVMO:
			{
				iErrorType = MSG_ERROR_RECVMO;
				if ( IsNeedToRecvMo( m_iRecvMoInterval ))
				{
					iRet = GetMoMessges(recvList,strError);
					if (iRet == -300)
					{
						iRet = 10000;
					}
					else
					{
						SetLastRecvMoTime();
					}	
				}
				else
				{
					strError = "����������Ϣδ�������ռ�� -- ������֤";
					iRet = 10000;
				}				
				break;
			}
		case OPTYPE_RECVREPORT:
			{
				iErrorType = MSG_ERROR_RECVREPORT;
				if ( IsNeedToRecvReport( m_iRecvReportInterval ))
				{
					iRet = GetMsgReports(reportList,strError);
					if (iRet == -300)
					{
						iRet = 10000;
					}
					else
					{
						SetLastRecvReportTime();
					}					
				}
				else
				{
					strError = "����״̬����δ�������ռ�� -- ������֤";
					iRet = 10000;
				}				
				break;
			}
		case OPTYPE_CHECKFEE:
			{
				iErrorType = MSG_ERROR_RECVREPORT;
				int iUsed,iLeft;
				iRet = GetFee(iUsed,iLeft,strError);
				break;
			}
		}	
		return iRet;
	}
	catch (...)
	{
		WriteLog("MessageOperate�쳣",3);
		iErrorType = 10000;	//δ֪�쳣
		return -99;
	}
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
//�ش��̺߳���
void CMWMessageSendEx::InsertBackThreadProc(LPVOID lpParam)
{
	CMWMessageSendEx *pThis = (CMWMessageSendEx *)lpParam;
	::CoInitialize(NULL);
	pThis->InsertBackProc(); //ִ���̺߳�����
	::CoUninitialize();
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
//�ش��߳�ִ�к���
void CMWMessageSendEx::InsertBackProc()
{
	AddToDispListShow("������д��ʷ��ɺ��˳�",1);	
	Sleep(300);
	int iRet = 0;
	KfSubmitPack mt;
	int iRunFlag = 1;	
	try
	{
		while (true)
		{
			m_IsSendThreadRunLock.Lock();
			iRunFlag = m_iIsSendThreadRun;
			m_IsSendThreadRunLock.Unlock();
			if (iRunFlag==0)
			{
				break;
			}
			else
			{
				Sleep(500);
			}
		}
		while(true)
		{
			iRet = GetRealSendPack(mt);
			if (iRet == 0)		//��δ���͵ļ�¼�����ļ�
			{
				AddToDispListShow("�����������б���Ϣд���ļ�");
				WriteBufToFile(mt);
			}
			else
			{
				break;
			}
		}
	}
	catch (...)
	{		
	}
	AddToDispListShow("д�ļ��߳��˳�");
	OnActiveThreadExit(2);
	return;
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
//д��ʷ�̺߳���
void CMWMessageSendEx::AddSendResultThread(LPVOID lpParam)
{
	ThreadParam *pParam = (ThreadParam*)lpParam;
	CMWMessageSendEx *pThis = (CMWMessageSendEx *)pParam->pThis;
	int iThreadNo = pParam->iThreadNO;
	::CoInitialize(NULL);
	pThis->AddSendResultProc(iThreadNo); //ִ���̺߳�����
	::CoUninitialize();
	//AfxMessageBox("aa");
	delete pParam;
	pParam = NULL;
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
//д��ʷ���߳�ִ�к���
void CMWMessageSendEx::AddSendResultProc(int iThreadNO)
{
	KfSubmitPack mt;
	CDataBaseCtrl dataBaseCtrl;	//���ݿ��������
 	dataBaseCtrl.InitParams(m_dbSQLServer,m_dbSQLName,m_dbSQLUser,m_dbSQLPwd,m_iDbType,m_iDbPort);
	CString strTemp="";
	int iRet = 0;
	bool bExitFlag = false;
	try
	{	
		Sleep(100*(iThreadNO+1));
		strTemp.Format("д��ʷ���߳�%d����",iThreadNO);
		AddToDispListShow(strTemp);
		while(true)
		{	
			if (m_iAddHistInterval >= 5)
			{
				Delay(m_iAddHistInterval,&m_AddResultThreadContrl[iThreadNO].ThreadExit);
			}			
			if (m_AddResultThreadContrl[iThreadNO].ThreadExit && iRet < 0)
			{
				if(bExitFlag==true)	//���߳��˳�
				{
					break;
				}
				int iRunFlag = 1;
				m_IsSendThreadRunLock.Lock();
				iRunFlag = m_iIsSendThreadRun;
				m_IsSendThreadRunLock.Unlock();
				if (iRunFlag == 0)	//��������߳��Ѿ��˳�
				{
					bExitFlag=true;
				}
				else
				{
					AddToDispListShow("�ȴ������߳��˳�...",1);
					Sleep(500);
				}
			}
			m_SendHistoryLock.Lock();
			try
			{				
				int iSize = m_SendHistoryList.GetSize();
				SetStatusInfoEx(4,iSize);//����
				if (iSize <= 0)	//������������
				{
					iRet = -1;
				}
				else
				{
					iRet = 0;
					mt = m_SendHistoryList.GetAt(0);
					//WriteLogEx("���淢����ʷ��ʼ",50 + iThreadNO);
					if (AddSendResult(dataBaseCtrl,mt,mt.m_iSendResultFlag)<0)//���淢�ͽ��ʧ��
					{
						if (mt.m_iTryTimes > 0)
						{
							strTemp.Format("��%d�α��淢�ͽ��ʧ��[%d]",4-mt.m_iTryTimes,mt.m_iLocalID);
							AddToDispListShow(strTemp);
							m_SendHistoryList.GetAt(0).m_iTryTimes--;
						}
						else
						{
							AddToDispListShow("���淢�ͽ��ʧ��");
							m_SendHistoryList.RemoveAt(0);
						}
					}
					else
					{
						m_SendHistoryList.RemoveAt(0);
					}
					//WriteLogEx("���淢����ʷ����",50 + iThreadNO);
				}		
			}
			catch(...)
			{
				iRet = -1;
				WriteLog("GetSendHistory�쳣",3);
			}
			m_SendHistoryLock.Unlock();
			if (iRet == -1)
			{
				Delay(m_iAddHistNullWait,&m_AddResultThreadContrl[iThreadNO].ThreadExit);
			}
		}
	}
	catch (...)
	{
		WriteLog("AddSendResultProc�쳣",3);
	}
	strTemp.Format("д��ʷ���߳�%d�˳�",iThreadNO);
	AddToDispListShow(strTemp);
	OnActiveThreadExit(1);	
	return;
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CMWMessageSendEx::MessageSendThreadProc(LPVOID lpParam)
{
	CMWMessageSendEx *pThis = (CMWMessageSendEx *)lpParam;
	::CoInitialize(NULL);
	pThis->MessageSendProc(); //ִ���̺߳�����
	::CoUninitialize();
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
//����������Ϣ���������״̬������߳�ִ�к���
void CMWMessageSendEx::MessageSendProc()
{
	CString strTemp = "";
	CString strSend = "";			
	CDataBaseCtrl dataBaseCtrl;	//���ݿ��������	
	DeliverPackList recvList;	//������Ϣ�б�
	ReportPackList reportList;	//״̬�����б�
	int iNextRet = -99;		//��һ����������ֵ
	int iiRet = -99;		//��������շ���ֵ
	int iErrorType = -99;	//��¼���صĴ�������
	CString strError = "";	//��¼���صĴ�����Ϣ
	CStringList strIllegalList;	//�Ƿ������б�
	int iPos = 0;				//�˻�����ѯλ��
	CString strBalance = "";	//�˻����
	int iRet = 0;	//��ȡ�����ṹ��ķ���ֵ
	int i=0;		//ѭ�����Ʊ���
	int iHistLen = 0;//��ʷ���¼��
	KfSubmitPack mt;			//�·��ṹ��
	ReportPack report;
	DeliverPack mo;
	//Delay(100,&m_SendThreadContrl.ThreadExit);
	try
	{	
		m_IsSendThreadRunLock.Lock();
		m_iIsSendThreadRun = 1;
		m_IsSendThreadRunLock.Unlock();
		AddToDispListShow("�����߳�����");
		dataBaseCtrl.InitParams(m_dbSQLServer,m_dbSQLName,m_dbSQLUser,m_dbSQLPwd,m_iDbType,m_iDbPort);
		LoadKeys();	  //���طǷ��ؼ����б�
		if (m_iIsReconn==1)
		{
			strTemp.Format("�����˶�ʱ�Զ�����,�������:%d��",m_iReconnInterval);
			AddToDispListShow(strTemp);
		}
		//strTemp.Format("��������������Ϊ%d��,������������Ϣĩβ��'��'���ٷ�",m_iMostSameSingleSendTime);
		//AddToDispListShow(strTemp);
		//Disconnect();
		m_LastActiveTime = ::GetTickCount() - 10000000;
		m_iLastErrorTime = ::GetTickCount() - 1000000;
		m_iLastReconnTime = ::GetTickCount();
		//����δ������ļ�¼
		ReadBufFromFile();
		int iRunFlag = 0;
		int iSendInterval = 0;
		SetCmdTypeEx(g_iServerType,g_iReportFlag);
		while (true)
		{
			if (m_SendThreadContrl.ThreadExit)
			{
				m_IsRecvThreadRunLock.Lock();
				iRunFlag = m_iIsRecvThreadRun;
				m_IsRecvThreadRunLock.Unlock();
				if (iRunFlag==0)
				{
					break;
				}
				else
				{
					AddToDispListShow("�ȴ������߳��˳�",1);
					Sleep(500);
					continue;
				}
			}
			if (m_iSendInterval >= 10)
			{
				Delay(m_iSendInterval,&m_SendThreadContrl.ThreadExit);//�����߳�ִ�м��
			}
			if (m_iIsReconn==1)//��ʱ����
			{
				if(IsOverLastReconnTime(m_iReconnInterval)==true)	//����ʱ�䵽
				{
					AddToDispListShow("����ʱ�䵽");
					Disconnect();
					m_iLastReconnTime = ::GetTickCount();
					m_LastActiveTime = ::GetTickCount() - 10000000;
					continue;
				}
			}
			/************************************************************************/
			//��¼
			iiRet = MessageOperate(OPTYPE_LOGIN,iErrorType,strError,mt,recvList,reportList);
			iNextRet = NextOptJudge(iiRet,iErrorType);
			if (iNextRet != 0) //��������
			{
				strTemp.Format("��¼ʧ�� [%s]",strError);
				AddToDispListShow(strTemp);
				m_LastActiveTime = ::GetTickCount() - 10000000;
				SetStatusInfoEx(3,-2);//�˻����:δ��ѯ
				Delay(10000,&m_SendThreadContrl.ThreadExit);	//��¼ʧ��,��ͣ10��
				continue;	
			}
			else if (iErrorType == MSG_ERROR_LOGIN)
			{
				SetLastActiveTime();
				AddToDispListShow("��¼�ɹ�");
				AddToDispListShow("�ȴ���ѯ���,���Ժ�...");
				m_CheckFeeFlag = true;
				SetStatusInfoEx(1,1);	//�ѵ�¼
			}
			/************************************************************************/
			//��ѯ���
			if (m_CheckFeeFlag == true && m_SendThreadContrl.ThreadExit == FALSE)
			{
				iiRet = MessageOperate(OPTYPE_CHECKFEE,iErrorType,strError,mt,recvList,reportList);
				AddToDispListShow(strError);
				m_CheckFeeFlag = false;
				iNextRet = NextOptJudge(iiRet,iErrorType);
				if (iNextRet < 0)
				{
					//���öϿ����ӣ���Ҫ���µ�¼
					Disconnect();
					m_LastActiveTime = ::GetTickCount() - 10000000;
					SetStatusInfoEx(1,0);	//δ��¼
					SetStatusInfoEx(3,-2);//�˻����:δ��ѯ
					m_CheckFeeFlag = true;
					continue;
				}
				g_iBalance = iiRet;
				SetStatusInfoEx(3,iiRet);//�˻����
				SetLastActiveTime();
			}
			//ȡ������Ϣ
			if (!ReadWaitMessage(dataBaseCtrl))//�����ݿ��ж�������Ϣ	
			{
				Delay(500,&m_SendThreadContrl.ThreadExit);
				continue;
			}				
			if (IsOverLastErrorTime(m_iErrorStopTime))	//��������˷��ͳ���ʱ��
			{
				/************************************************************************/
				//��ʽ����Ϣ������������				
				m_RealSendLock.Lock();
				iRet = m_RealSendList.GetSize();
				m_RealSendLock.Unlock();
				if (iRet <= 0)	//��������Ϊ��ʱ��ִ�д˲���
				{
					//�ϲ���ͬ������Ϣ,��һ����������
					UnionSameSingleMessage(dataBaseCtrl);
					iRet = GetKfSubmitPack(mt);
					if (iRet == 0)
					{
						mt.m_iTryTimes = 3;	//�ܹ������ط�						
						iiRet = dataBaseCtrl.ConvertSendMessage(mt);//�Ժ��뼰��Ϣ���г����Ϸ���֤
 						if (iiRet <= 0)
 						{
							AddToDispListShow("����ʧ��["+mt.m_strFailReason+"]");
							AddToSentListShow("88888888",mt.m_strUserNumber,mt.m_strMsg,g_Conver.GetMsgULen(mt.m_strMsg),mt.m_iCount,6,mt.m_strFailReason,m_iDiffMobile,mt.m_strOperatorID);
							AddtoSendHistory(dataBaseCtrl,mt,1,mt.m_strFailReason);
							continue;
 						}
						//���Ƿ��ؼ���
						if(m_iCheckKey != 0)	//��Ҫ��ѯ�ؼ���
						{
							iiRet = m_keyWord.KeyWordCheck(mt.m_strMsg,strTemp);
							if (iiRet==0)	//���ڷǷ��ؼ���
							{
								strError.Format("���ڷǷ��ؼ���[%s]",strTemp);
								strTemp.Format("����ʧ��[%s],����[%s]",strError,mt.m_strMsg);
								AddToDispListShow(strTemp);
								AddToSentListShow("88888888",mt.m_strUserNumber,mt.m_strMsg,g_Conver.GetMsgULen(mt.m_strMsg),mt.m_iCount,6,strError,m_iDiffMobile,mt.m_strOperatorID);
								AddtoSendHistory(dataBaseCtrl,mt,1,strError);
								continue;
							}
						}
						//���Ϸ�����Ϣ���������ķ��ͻ���
						ConvertPhoneAddMessage(mt,0,strIllegalList);
						//�����ںŶ��ڵ���Ϣ����ʧ�ܽ��
						int iErrorCount = 0;
						mt.m_strUserNumber = "";
						for (POSITION Pos_ = strIllegalList.GetHeadPosition(); Pos_ != NULL;)
						{
							mt.m_strUserNumber += strIllegalList.GetNext(Pos_)+" ";
							iErrorCount++;
						}
						mt.m_strUserNumber.Trim();
						if (mt.m_strUserNumber != "")
						{
							AddToDispListShow(mt.m_strUserNumber+"���ںŶα���");
							AddToSentListShow("88888888",mt.m_strUserNumber,mt.m_strMsg,g_Conver.GetMsgULen(mt.m_strMsg),iErrorCount,5,"ʧ��[���ںŶα���]",m_iDiffMobile,mt.m_strOperatorID);
							AddtoSendHistory(dataBaseCtrl,mt,1,"���벻�ںŶα���");
						}	
					}
					else if (iRet == -1)
					{
						AddToDispListShow("���ͻ����������Ϣ",1);
						Delay(m_iBufNullWaitTime,&m_SendThreadContrl.ThreadExit);
					}
					else
					{
						AddToDispListShow("��������г���");
						Delay(100,&m_SendThreadContrl.ThreadExit);		
					}
				}
				/************************************************************************/
				//������Ϣ				
				iNextRet = -99;
				iiRet = -99;
				//int iNum = 0;
				while(true)		//��һ�����еķ�����
				{
					m_SendHistoryLock.Lock();
					iHistLen = m_SendHistoryList.GetSize();
					m_SendHistoryLock.Unlock();
					if (iHistLen>m_iNumSendStop)	//�����ʷ���еļ�¼������һ������,��ͣ����
					{
						break;
					}
					iRet = GetRealSendPack(mt);
					if (iRet == 0)	//ȡ���ȴ�������Ϣ
					{
						iiRet = MessageOperate(OPTYPE_SEND,iErrorType,strError,mt,recvList,reportList);
						iErrorType = MSG_ERROR_SEND;
						iNextRet = NextOptJudge(iiRet,iErrorType);
						if (iNextRet == 0)
						{
							SetLastActiveTime();				
							//���˻����
							g_iBalance -= mt.m_iCount;
							SetStatusInfoEx(3,g_iBalance);//�˻����
							AddToSentListShow(mt.m_strSpMsgId,mt.m_strUserNumber,mt.m_strMsg,mt.m_iMsgLen,mt.m_iCount,mt.m_iPhoneType,"�ɹ�",m_iDiffMobile,mt.m_strOperatorID);
							strTemp.Format("������Ϣ�ɹ�,���[%s],����[%s]",mt.m_strSpMsgId,mt.m_strMsg);
							AddToDispListShow(strTemp);
							AddtoSendHistory(dataBaseCtrl,mt,0,"");
						}
						else	//���ͳ�������
						{
							if (iNextRet == -1)
							{
								//�Ͽ�,��Ҫ�ط�
								Disconnect();
								m_LastActiveTime = ::GetTickCount() - 10000000;
								SetStatusInfoEx(1,0);//��¼״̬:�Ͽ�
								SetStatusInfoEx(3,-2);//"�˻����:δ��ѯ"
								//�ط�����
								if (mt.m_iTryTimes > 0)
								{
									strTemp.Format("��%d�η���ʧ��,�ȴ��ط�",4-mt.m_iTryTimes);
									AddToDispListShow(strTemp);
									mt.m_iTryTimes--;
									AddRealSendPack(mt);
								}
								else
								{
									strSend.Format("ʧ��[%s]",strError);					
									AddToSentListShow(mt.m_strSpMsgId,mt.m_strUserNumber,mt.m_strMsg,mt.m_iMsgLen,mt.m_iCount,mt.m_iPhoneType,strSend,m_iDiffMobile,mt.m_strOperatorID);
									strTemp.Format("������Ϣʧ��[%s][�����ط�����]",strError);
									AddToDispListShow(strTemp);
									//��ӷ���ʧ�ܽ����������
									AddtoSendHistory(dataBaseCtrl,mt,1,strError);
								}
								break;
							}
							else if (iNextRet == -2)
							{
								Disconnect();
								m_LastActiveTime = ::GetTickCount() - 10000000;
								SetStatusInfoEx(1,0);//��¼״̬:�Ͽ�
								SetStatusInfoEx(3,-2);//"�˻����:δ��ѯ"
								SetLastErrorTime();	//����������ʱ��
							}
							//-3������

							//��ӷ���ʧ�ܽ����������
							AddtoSendHistory(dataBaseCtrl,mt,1,strError);
							strSend.Format("ʧ��[%s]",strError);		
							AddToSentListShow(mt.m_strSpMsgId,mt.m_strUserNumber,mt.m_strMsg,mt.m_iMsgLen,mt.m_iCount,mt.m_iPhoneType,strSend,m_iDiffMobile,mt.m_strOperatorID);
							strTemp.Format("������Ϣʧ��[%s]",strError);
							AddToDispListShow(strTemp);
							break;
						}
					}
					else		//����Ϣ�ȴ�����
					{
						break;
					}
				}
				//���Ҫ�Ͽ�����,����һ��ѭ��,���β��������к�״̬����
				if (iNextRet == -1 || iNextRet == -2)
				{
					continue;
				}
			}
			else		//���ͳ�����ͣ���
			{
				Delay(500,&m_SendThreadContrl.ThreadExit);	//��ͣ100MS
			}
			if (g_iServerType != 0)		//�������SIƽ̨��ֱ�ӷ��أ����ս�������һ���߳�ִ��
			{
				continue;
			}
			/************************************************************************/		
			//����������Ϣ
			if (m_iIsRecvMo == 1 && m_SendThreadContrl.ThreadExit == FALSE)	//�Ƿ����������Ϣ��־
			{
				iNextRet = -99;
				iiRet = -99;
				recvList.RemoveAll();
				iiRet = MessageOperate(OPTYPE_RECVMO,iErrorType,strError,mt,recvList,reportList);
				iNextRet = NextOptJudge(iiRet,iErrorType);
				if (iNextRet < 0)
				{
					//���öϿ����ӣ���Ҫ���µ�¼
					AddToDispListShow(strError);
					Disconnect();					
					m_LastActiveTime = ::GetTickCount() - 10000000;
					SetStatusInfoEx(1,0);//��¼״̬:�Ͽ�
					SetStatusInfoEx(3,-2);//"�˻����:δ��ѯ"
					continue;
				}
				else		//������յ���������Ϣ
				{
					if (iiRet > 0 && iiRet != 10000)	//��������Ϣ
					{
						//strTemp.Format("�յ�%d��������Ϣ",recvList.GetSize());
						//AddToDispListShow(strTemp);
						//�����յ���Ϣ�������ݿ�
						//WriteLogEx("����������Ϣ��ʼ",2);
						for ( i=0;i<recvList.GetSize();i++)
						{
							mo = recvList.GetAt(i);
							mo.strTime = mo.strDate +" "+ mo.strTime;
							AddToRecvListShow(mo.strTime,mo.strOrgAddr,mo.strMsg,mo.strDestAddr);
							strTemp.Format("������Ϣ:[%s,%s,%s,%s]",mo.strTime,mo.strOrgAddr,mo.strDestAddr,mo.strMsg);
							AddToDispListShow(strTemp,0);
							dataBaseCtrl.AddMoMessage(mo);
						}
						//WriteLogEx("����������Ϣ����",2);
						SetLastActiveTime();
					}
					else if (iiRet != 10000) //��������Ϣ = 0
					{
						AddToDispListShow("��������Ϣ",1);
					}					
				}
			}			
			/************************************************************************/
			//����״̬����
			if (m_iIsRecvReport == 1 && m_SendThreadContrl.ThreadExit == FALSE)	//�Ƿ����������Ϣ��־
			{
				iNextRet = -99;
				iiRet = -99;
				reportList.RemoveAll();
				iiRet = MessageOperate(OPTYPE_RECVREPORT,iErrorType,strError,mt,recvList,reportList);				
				iNextRet = NextOptJudge(iiRet,iErrorType);
				if (iNextRet < 0)
				{
					//���öϿ����ӣ���Ҫ���µ�¼
					AddToDispListShow(strError);
					Disconnect();
					m_LastActiveTime = ::GetTickCount() - 10000000;
					SetStatusInfoEx(1,0);//��¼״̬:�Ͽ�
					SetStatusInfoEx(3,-2);//"�˻����:δ��ѯ"
					continue;
				}
				else		//������յ���״̬����
				{
					if (iiRet > 0 && iiRet != 10000)	//��״̬����
					{
						strTemp.Format("�յ�%d��״̬����",reportList.GetSize());
						AddToDispListShow(strTemp);
						//WriteLogEx("����״̬���濪ʼ",2);
						for ( i=0;i<reportList.GetSize();i++)
						{
							report = reportList.GetAt(i);
							if ( report.strMsgStatus == "0")
							{
								strSend = "�ɹ�";
							}
							else if (report.strMsgStatus == "1")
							{
								strSend.Format("�����ݻ�[%s]",report.strErrorCode);
							}
							else if (report.strMsgStatus == "2")
							{
								strSend.Format("ʧ��[%s]",report.strErrorCode);
							}
							else
							{
								strSend.Format("ʧ��[%s]",report.strErrorCode);
							}
							report.strTime = report.strDate +" "+ report.strTime;
							AddToRePortListShow(report.strTime,report.strMsgNo,strSend);
							dataBaseCtrl.UpdateSendResult(report);
						}
						g_ReportList.PostMessage(WM_VSCROLL,SB_BOTTOM,0);
						//WriteLogEx("����״̬�������",2);
						SetLastActiveTime();
					}
					else if (iiRet != 10000)//��״̬���� = 0
					{
						AddToDispListShow("��״̬����",1);
					}					
				}
			}
		}		
	}
	catch (...)
	{
		AddToDispListShow("�����߳��쳣");
		WriteLog("MessageSendProc�쳣",3);
	}
	Disconnect();
	m_LastActiveTime = ::GetTickCount() - 10000000;
	SetStatusInfoEx(1,0);//��¼״̬:�Ͽ�
	SetStatusInfoEx(3,-2);//"�˻����:δ��ѯ"
	AddToDispListShow("�����߳��˳�");
	m_IsSendThreadRunLock.Lock();
	m_iIsSendThreadRun = 0;
	m_IsSendThreadRunLock.Unlock();
	//���һ���ж��Ƿ������̶߳����˳��ĺ���,ִ�в���	
	OnActiveThreadExit(0);
	return;
}
/************************************************************************/
/*�����̺߳���                                                          */
/************************************************************************/
void CMWMessageSendEx::RecvThreadProc(LPVOID lpParam)
{
	CMWMessageSendEx *pThis = (CMWMessageSendEx *)lpParam;
	::CoInitialize(NULL);
	pThis->RecvProc(); //ִ���̺߳�����
	::CoUninitialize();
}
/************************************************************************/
/*�����߳�ִ�к���                                                      */
/************************************************************************/
void CMWMessageSendEx::RecvProc()
{
	CString strTemp = "";
	CString strSend = "";			
	CDataBaseCtrl dataBaseCtrl;	//���ݿ��������	
	DeliverPackList recvList;	//������Ϣ�б�
	ReportPackList reportList;	//״̬�����б�
	int iNextRet = -99;		//��һ����������ֵ
	int iiRet = -99;		//��������շ���ֵ
	int iErrorType = -99;	//��¼���صĴ�������
	CString strError = "";	//��¼���صĴ�����Ϣ
	int iRet = 0;	//��ȡ�����ṹ��ķ���ֵ
	int i=0;		//ѭ�����Ʊ���
	int iHistLen = 0;//��ʷ���¼��
	KfSubmitPack mt;
	ReportPack report;
	DeliverPack mo;
	try
	{	
		m_IsRecvThreadRunLock.Lock();
		m_iIsRecvThreadRun = 1;
		m_IsRecvThreadRunLock.Unlock();
		AddToDispListShow("�����߳�����");
		dataBaseCtrl.InitParams(m_dbSQLServer,m_dbSQLName,m_dbSQLUser,m_dbSQLPwd,m_iDbType,m_iDbPort);		
		SetCmdTypeEx(g_iServerType,g_iReportFlag);
		bool bRecvMoStop = false;
		bool bRecvRptStop = false;
		while (!m_RecvThreadContrl.ThreadExit)
		{	
			/************************************************************************/		
			//����������Ϣ
			iNextRet = -99;
			iiRet = -99;
			bRecvMoStop = false;
			bRecvRptStop = false;
			recvList.RemoveAll();
			iiRet = MessageOperate(OPTYPE_RECVMO,iErrorType,strError,mt,recvList,reportList);
			iNextRet = NextOptJudge(iiRet,iErrorType);
			if (iiRet > 0 && iiRet != 10000)	//��������Ϣ
			{
				strTemp.Format("�յ�%d��������Ϣ",recvList.GetSize());
				AddToDispListShow(strTemp);
				//�����յ���Ϣ�������ݿ�
				for ( i=0;i<recvList.GetSize();i++)
				{
					mo = recvList.GetAt(i);
					mo.strTime = mo.strDate +" "+ mo.strTime;
					AddToRecvListShow(mo.strTime,mo.strOrgAddr,mo.strMsg,mo.strDestAddr);
					dataBaseCtrl.AddMoMessage(mo);
				}
			}
			else if (iiRet != 10000) //��������Ϣ = 0
			{
				AddToDispListShow("��������Ϣ",1);
			}				
			else
			{
				bRecvMoStop = true;
			}

			/************************************************************************/
			//����״̬����
			iNextRet = -99;
			iiRet = -99;
			reportList.RemoveAll();
			iiRet = MessageOperate(OPTYPE_RECVREPORT,iErrorType,strError,mt,recvList,reportList);				
			iNextRet = NextOptJudge(iiRet,iErrorType);
			if (iiRet > 0 && iiRet != 10000)	//��״̬����
			{
				strTemp.Format("�յ�%d��״̬����",reportList.GetSize());
				AddToDispListShow(strTemp);
				for ( i=0;i<reportList.GetSize();i++)
				{
					report = reportList.GetAt(i);
					if (report.strMsgStatus == "0")
					{
						strSend = "�ɹ�";
					}
					else if (report.strMsgStatus == "1")
					{
						strSend.Format("�����ݻ�[%s]",report.strErrorCode);
					}
					else if (report.strMsgStatus == "2")
					{
						strSend.Format("ʧ��[%s]",report.strErrorCode);
					}
					else
					{
						strSend.Format("ʧ��[%s]",report.strErrorCode);
					}
					report.strTime = report.strDate +" "+ report.strTime;
					AddToRePortListShow(report.strTime,report.strMsgNo,strSend);
					dataBaseCtrl.UpdateSendResult(report);
				}
				g_ReportList.PostMessage(WM_VSCROLL,SB_BOTTOM,0);
			}
			else if (iiRet != 10000)//��״̬���� = 0
			{
				AddToDispListShow("��״̬����",1);
			}
			else
			{
				bRecvRptStop = true;
			}
			if (bRecvMoStop && bRecvRptStop)		//����������к�״̬�����ʱ�䶼û������ͣ500MS
			{
				Delay(500,&m_RecvThreadContrl.ThreadExit);
			}
		}
	}
	catch (...)
	{
		AddToDispListShow("�����߳��쳣");
		WriteLog("RecvProc�쳣",3);
	}
	AddToDispListShow("�����߳��˳�");
	m_IsRecvThreadRunLock.Lock();
	m_iIsRecvThreadRun = 0;
	m_IsRecvThreadRunLock.Unlock();
	//���һ���ж��Ƿ������̶߳����˳��ĺ���,ִ�в���	
	OnActiveThreadExit(0);
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
//�߳��п�����ʱ�ĺ���
void CMWMessageSendEx::Delay(int iMiniSecond,BOOL *bExit)
{
	for (int i=0;i<iMiniSecond;i+=5)
	{
		if (*bExit)
		{
			break;
		}
		Sleep( 5 );
	}
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
//�����ݿ��ж�ȡ�������ŵ����ͻ�����
BOOL CMWMessageSendEx::ReadWaitMessage(CDataBaseCtrl &dataBaseCtrl)
{
	try
	{		
		int iBufLen = 0;
		int iHistLen = 0;
		int iRealLen = 0;
		//�������ͻ���
		m_RealSendLock.Lock();
		iRealLen = m_RealSendList.GetSize();
		m_RealSendLock.Unlock();
		if(iRealLen>0)
		{
			return TRUE;
		}
		//���ͻ���
		m_SendListLock.Lock();
		iBufLen = m_SubmitPackList.GetSize();
		m_SendListLock.Unlock();
		if(iBufLen>0)
		{
			return TRUE;
		}
		//������ʷ
		m_SendHistoryLock.Lock();
		iHistLen = m_SendHistoryList.GetSize();
		m_SendHistoryLock.Unlock();
		if (iHistLen > 0)
		{
			return TRUE;
		}
		//���ȴ�������Ϣ
		int iRet = -3;
		//WriteLogEx("�����ݿ��ȡ������Ϣ��ʼ",2);
		iRet = dataBaseCtrl.ReadWaitMessage(m_SubmitPackList,m_SendListLock);
		//WriteLogEx("�����ݿ��ȡ������Ϣ����",2);
		if (iRet == 0)	//����Ϣ
		{
			Delay(m_iAddNullWaitTime,&m_SendThreadContrl.ThreadExit);
		}
		else if (iRet == -3)	//���ӷ�����ʧ��
		{
			return FALSE;
		}
	}
	catch (...)
	{
		AddToDispListShow("��ȡ�����쳣");
		WriteLog("CMWMessageSendEx::ReadWaitMessage�쳣",3);
	}
	return TRUE;
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
//��ӷ��ͻ���
void CMWMessageSendEx::AddKfSubmitPack(KfSubmitPack &submitPack)
{
	m_SendListLock.Lock();
	try
	{	
		m_SubmitPackList.Add(submitPack);
	}
	catch(...)
	{
		WriteLog("AddKfSubmitPack�쳣",3);
	}
	m_SendListLock.Unlock();
	return;
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
//��ӷ��ͻ���
void CMWMessageSendEx::AddKfSubmitPackEx(KfSubmitPack &submitPack)
{
	m_SendListLock.Lock();
	try
	{	
		m_SubmitPackList.InsertAt(0,submitPack);
	}
	catch(...)
	{
		WriteLog("AddKfSubmitPackEx�쳣",3);
	}
	m_SendListLock.Unlock();
	return;
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
//��ȡ��������,����0��ʾ����,-1��ʾ������,-2��ʾ����
int CMWMessageSendEx::GetKfSubmitPack(KfSubmitPack &submitPack)
{
	int iRet = 0;
	m_SendListLock.Lock();
	try
	{
		int iSize = m_SubmitPackList.GetSize();
		if (iSize == 0)	//������������
		{
			iRet = -1;
		}
		else
		{
			submitPack = m_SubmitPackList.GetAt( 0 );
			m_SubmitPackList.RemoveAt( 0 );
			SetStatusInfoEx(2,iSize-1);//����
			iRet = 0;	//������ȡ������
		}		
	}
	catch(...)
	{
		iRet = -2;		//����
		WriteLog("GetKfSubmitPack�쳣",3);
	}
	m_SendListLock.Unlock();
	return iRet;
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
//��ȡ�ȴ�������Ϣ���з���
int CMWMessageSendEx::GetRealSendPack(KfSubmitPack &sendPack)
{
	int iRet = 0;
	m_RealSendLock.Lock();
	try
	{		
		int iSize = m_RealSendList.GetSize();
		if (iSize == 0)	//������������
		{
			iRet = -1;
		}
		else
		{
			sendPack = m_RealSendList.GetAt( 0 );
			m_RealSendList.RemoveAt( 0 );
			iRet = 0;	//������ȡ������
		}		
	}
	catch(...)
	{
		iRet = -2;		//����
		WriteLog("GetRealSendPack�쳣",3);
	}
	m_RealSendLock.Unlock();
	return iRet;
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
//��ӵȴ����ͻ�������
void CMWMessageSendEx::AddRealSendPack(KfSubmitPack &sendPack)
{
	m_RealSendLock.Lock();
	try
	{		
		m_RealSendList.Add(sendPack);
	}
	catch(...)
	{
		WriteLog("AddRealSendPack�쳣",3);
	}
	m_RealSendLock.Unlock();
	return;
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
//�������̶߳��˳�ʱ�Ĳ���,0��ʾ�����̣߳�1��ʾд��ʷ���߳�
void CMWMessageSendEx::OnActiveThreadExit(int iFlag)
{
	try
	{
		Sleep(200);
		//m_activeNumLock.Lock();
		m_iActiveThreadNum--;
		//m_activeNumLock.Unlock();
		
		//����߳�ȫ��ֹͣ��,���¹�������ʾ
		if (m_iActiveThreadNum == 0)
		{
			int iStatus = GetRunStatus();
			switch (iStatus)
			{
			case STATUS_RUN_NORMAL:
				{
					AddToDispListShow("�����߳���������");
					break;
				}
			case STATUS_STOP_ERROR:
				{
					AddToDispListShow("�������ݿ����,�����߳��Զ�ֹͣ,��������ȷ�����ݿ���Ϣ");
					break;
				}
			case STATUS_STOP_MANUAL:
				{
					//AddToDispListShow("�����߳�ֹͣ");
					break;
				}
			case STATUS_STOP_NORMAL:		//���ø���,�߳�����
				{
					AddToDispListShow("�����߳�����");
					break;
				}
			case STATUS_STOP_OVERRUNTIME:
				{
					AddToDispListShow("������ִ��ʱ���,�߳��Զ�ֹͣ");
					break;
				}
			default:
				{
					AddToDispListShow("�߳��Զ�ֹͣ");
					break;
				}
			}
			if (m_hWnd!=NULL)
			{
				::PostMessage(m_hWnd,WM_USERMSGSEND,0,iStatus);
			}
		}		
		Sleep(200);
		if (g_hwndClose != NULL)
		{
			::PostMessage(g_hwndClose,USERWINDOWMSG_1,0,0);
		}
	}
	catch (...)
	{
		WriteLog("OnActiveThreadExit�쳣",3);
	}	
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
//��ֺ��뼰��Ϣ,���ݺ�������ӵ��������Ͷ�����,flag���������Ƿ����������ͱ�0,���Ƿ���ƥ���1
void CMWMessageSendEx::ConvertPhoneAddMessage(KfSubmitPack &mt,int iFlag,CStringList &strIllegalPhones)
{
	CString strTemp = "";
	int iIndex = 0;
	CString strPhoneTemp = "";
	CString MobilePhoneList = "";	//�ƶ�������
	CString UnicomPhoneList = "";	//��ͨ������
	CString TelcomPhoneList = "";	//С��ͨ������
	int iMobilePhoneNum = 0;
	int iUnicomPhoneNum = 0;
	int iTelcomPhoneNum = 0;
	CString strTelComList = "";	//���ź�����
	int iTelComNumEx = 0;	//���ź������
	int iPhoneType = -1;
	int idx1 = 0;
	int idx2 = 0;
	strIllegalPhones.RemoveAll();
	CString strTempMesg = "";
	CString strOut = "";
	CString strMsg = "";
	//WriteLogEx("��ֺ��뼰��Ϣ������ʼ",3);
	try
	{		
		CString strPhoneList = mt.m_strUserNumber;
		strPhoneList.Trim();
		strPhoneList += " ";
		idx2 = strPhoneList.Find(' ', idx1);  //�ҵ�һ����¼
		while(idx2 != -1)						//����ҵ�
		{
			strPhoneTemp = strPhoneList.Mid(idx1,idx2 - idx1); //ȡ��һ����¼	
			strPhoneTemp.Trim();
			if ( strPhoneTemp == "" )
			{
				//strIllegalPhones.AddTail(strPhoneTemp);
				idx1 = idx2 + 1;
				idx2 = strPhoneList.Find(' ', idx1);  //����һ����¼
				continue;
			}
			iPhoneType = -1;			
			iPhoneType = g_Conver.MobileType(strPhoneTemp);			
			if ( m_iDiffMobile == 0 && iPhoneType >= 0)
			{
// 				if ( iPhoneType == 1 || iPhoneType == 3)	//����ͨ����źŶ���Ϊ�ƶ�
// 				{
// 					iPhoneType = 0;
// 				}
				iPhoneType = 0;			//��������ֺŶ�,�����кŶΰ��ƶ��Ŷε�����ͳһ����
			}
			if ( iPhoneType == 0 )	//�ƶ�����
			{
				iMobilePhoneNum++;
				MobilePhoneList += strPhoneTemp + " ";
			}
			else if ( iPhoneType == 1 ) //��ͨ���� 
			{
				iUnicomPhoneNum++;
				UnicomPhoneList += strPhoneTemp + " ";
			}
			else if (iPhoneType == 2)		//С��ͨ���� 
			{
				iTelcomPhoneNum++;
				TelcomPhoneList += strPhoneTemp + " ";
			}
			else if (iPhoneType == 3)	//���ź���
			{
				iTelComNumEx++;
				strTelComList += strPhoneTemp + " ";
			}
			else		//�������ͳ���
			{
				strIllegalPhones.AddTail(strPhoneTemp);
			}
			idx1 = idx2 + 1;
			idx2 = strPhoneList.Find(' ', idx1);  //����һ����¼
		}

		//�����Ѿ���ֿ�����ʼ�����Ϣ
		strTempMesg = mt.m_strMsg;
		strOut = "";
		strMsg = strTempMesg;
		int iMsgLen = g_Conver.GetMsgULen(strMsg);
		if (iMobilePhoneNum > 0)	//����ƶ�
		{
			if ( iMsgLen <= m_iSplitLength )	//������
			{
				mt.m_iMsgLen = iMsgLen;
				if ( iMobilePhoneNum > 0 )
				{
					mt.m_iPhoneType = 0;
					AddSendPackList( MobilePhoneList,mt,iMobilePhoneNum,m_iPhoneSplitLen );
				}				
			}
			else	//��Ҫ���
			{
				int iSplitLength = m_iSplitLength - 3;
				int nMsgNum = 0;
				nMsgNum = iMsgLen / iSplitLength + 1;
				if ( iMsgLen % iSplitLength == 0 )
				{
					nMsgNum--;
				}
				int i = 0;
				for (i=0;i<nMsgNum;)
				{
					i++;
					int iSplitRet = g_Conver.SplitMsg(strMsg,iSplitLength,strOut);
					if (iSplitRet>=0)
					{
						mt.m_strMsg.Format("%d/%d%s",i,nMsgNum,strOut);
						mt.m_iMsgLen = g_Conver.GetMsgULen(mt.m_strMsg);
						if ( iMobilePhoneNum > 0 )
						{
							mt.m_iPhoneType = 0;
							AddSendPackList( MobilePhoneList,mt,iMobilePhoneNum,m_iPhoneSplitLen );
						}						
					}
				}
			}
		}
		strOut = "";
		strMsg = strTempMesg;
		if (iUnicomPhoneNum > 0)	//�����ͨ����
		{
			if ( iMsgLen <= m_iSplitLengthUnicom )	//������
			{
				mt.m_iMsgLen = iMsgLen;
				mt.m_strMsg = strTempMesg;
				if ( iUnicomPhoneNum > 0 )
				{
					mt.m_iPhoneType = 1;
					AddSendPackList( UnicomPhoneList,mt,iUnicomPhoneNum,m_iPhoneSplitLen );
				}				
			}
			else	//��Ҫ���
			{
				int iSplitLength = m_iSplitLengthUnicom - 3;
				int nMsgNum = 0;
				nMsgNum = iMsgLen / iSplitLength + 1;
				if ( iMsgLen % iSplitLength == 0 )
				{
					nMsgNum--;
				}
				int i = 0;
				for (i=0;i<nMsgNum;)
				{
					i++;
					int iSplitRet = g_Conver.SplitMsg(strMsg,iSplitLength,strOut);
					if (iSplitRet>=0)
					{
						mt.m_strMsg.Format("%d/%d%s",i,nMsgNum,strOut);
						mt.m_iMsgLen = g_Conver.GetMsgULen(mt.m_strMsg);					
						if ( iUnicomPhoneNum > 0 )
						{
							mt.m_iPhoneType = 1;
							AddSendPackList( UnicomPhoneList,mt,iUnicomPhoneNum,m_iPhoneSplitLen );
						}
						
					}
				}
			}
		}
		strOut = "";
		strMsg = strTempMesg;
		if (iTelComNumEx > 0)	//��ֵ���
		{
			if ( iMsgLen <= m_iSplitLengthChinaCom )	//������
			{
				mt.m_iMsgLen = iMsgLen;
				if ( iTelComNumEx > 0 )
				{
					mt.m_iPhoneType = 3;
					AddSendPackList(strTelComList,mt,iTelComNumEx,m_iPhoneSplitLen);
				}			
			}
			else	//��Ҫ���
			{
				int iSplitLength = m_iSplitLengthChinaCom - 3;
				int nMsgNum = 0;
				nMsgNum = iMsgLen / iSplitLength + 1;
				if ( iMsgLen % iSplitLength == 0 )
				{
					nMsgNum--;
				}
				int i = 0;
				for (i=0;i<nMsgNum;)
				{
					i++;
					int iSplitRet = g_Conver.SplitMsg(strMsg,iSplitLength,strOut);
					if (iSplitRet>=0)
					{
						mt.m_strMsg.Format("%d/%d%s",i,nMsgNum,strOut);
						mt.m_iMsgLen = g_Conver.GetMsgULen(mt.m_strMsg);
						if ( iTelComNumEx > 0 )
						{
							mt.m_iPhoneType = 3;
							AddSendPackList( strTelComList,mt,iTelComNumEx,m_iPhoneSplitLen );
						}					
					}
				}
			}
		}
		//���С��ͨ
		strOut = "";
		strMsg = strTempMesg;
		if ( iTelcomPhoneNum > 0 )
		{
			if (iMsgLen <= m_iSplitLengthChina)	//������
			{
				mt.m_iMsgLen = iMsgLen;
				mt.m_strMsg = strTempMesg;
				if (iTelcomPhoneNum > 0)
				{
					mt.m_iPhoneType = 2;
					AddSendPackList(TelcomPhoneList,mt,iTelcomPhoneNum,m_iPhoneSplitLen);
				}				
			}
			else	//��Ҫ���
			{
				int iSplitLength = m_iSplitLengthChina - 3;
				int nMsgNum = 0;
				nMsgNum = iMsgLen / iSplitLength + 1;
				if ( iMsgLen % iSplitLength == 0 )
				{
					nMsgNum--;
				}
				int i = 0;
				for (i=0;i<nMsgNum;)
				{
					i++;
					int iSplitRet = g_Conver.SplitMsg(strMsg,iSplitLength,strOut);
					if (iSplitRet>=0)
					{
						mt.m_strMsg.Format("%d/%d%s",i,nMsgNum,strOut);
						mt.m_iMsgLen = g_Conver.GetMsgULen(mt.m_strMsg);
						if ( iTelcomPhoneNum > 0 )
						{
							mt.m_iPhoneType = 2;
							AddSendPackList(TelcomPhoneList,mt,iTelcomPhoneNum,m_iPhoneSplitLen);
						}					
					}
				}
			}
		}		
	}
	catch (...)
	{
		WriteLog("ConvertPhoneAddMessage�쳣",3);
	}
	//WriteLogEx("��ֺ��뼰��Ϣ��������",3);
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
//��Ժ�����,���Ը��ݴ����������ÿ����������
void CMWMessageSendEx::AddSendPackList( CString &strPhoneList,KfSubmitPack &mt,int iCount,int iSplitLen)
{
	try
	{
		CString strDestAddr = strPhoneList;
		strDestAddr.Trim();
		strDestAddr += " ";
		//CString temp = "";
		int iCalCount = 0;
		if ( iCount > iSplitLen )		//������Ҫ���
		{
			//�ҿո�
			int idx1 = 0;
			int idx2 = 0;
			int i = 0;
			while(true)						//����ҵ�
			{ 
				if ( idx2 != -1 )
				{
					idx2 = strDestAddr.Find(' ', idx1);  //����һ����¼
					if ( idx2 == -1 )
					{
						continue;
					}
					idx1 = idx2 + 1;
					i ++;						
					if ( i == iSplitLen )	//�պ�1000��ʱ��
					{
						//Ŀ���ֻ�������
						mt.m_strUserNumber = strDestAddr.Left(idx2);//15913218097
						//�ֻ��������
						mt.m_iCount = iSplitLen;
						iCalCount += iSplitLen;
						AddRealSendPack( mt );
						strDestAddr = strDestAddr.Mid(idx2+1);
						i = 0;
						idx1 = 0;
						idx2 = 0;
					}
				}
				else
				{
					strDestAddr.Trim();
					//Ŀ���ֻ�������
					if ( strDestAddr != "" )
					{
						mt.m_strUserNumber = strDestAddr;
						//�ֻ��������
						mt.m_iCount = i;
						iCalCount += i;
						AddRealSendPack( mt );
					}				
					break;
				}
			}
		}
		else if (iCount > 0 && iCount <= iSplitLen)	//����������
		{
			//Ŀ���ֻ�������
			mt.m_strUserNumber = strDestAddr; 
			//�ֻ��������
			mt.m_iCount = iCount;
			AddRealSendPack( mt );
		}
		else		//�޺���
		{

		}
	}
	catch (...)
	{
		WriteLog("AddSendPackList�쳣",3);
	}	
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
//��ȡ�����ļ�,��ʼ������
void CMWMessageSendEx::InitParams()
{
	try
	{
		CString strTemp = "";
		int iTemp = 0;
		CUserFile *pFile = NULL;
		pFile = CUserFile::GetInstance();
		if (pFile)
		{	
			int iRet = -1;
			CString strAppPath = "";
			pFile->GetAppPath(strAppPath);
			strAppPath += MWGATEINITFILE;
			//���ݿ���Ϣ
			iRet = pFile->ReadInitInfo(strAppPath,"database","dbserver",m_dbSQLServer,iTemp,0);
			if (iRet < 0)
			{
				m_dbSQLServer = "127.0.0.1";
			}
			iRet = pFile->ReadInitInfo(strAppPath,"database","dbname",m_dbSQLName,iTemp,0);
			if (iRet < 0)
			{
				m_dbSQLName = "test";
			}
			iRet = pFile->ReadInitInfo(strAppPath,"database","dbuser",m_dbSQLUser,iTemp,0);
			if (iRet < 0)
			{
				m_dbSQLUser = "test";
			}
			iRet = pFile->ReadInitInfo(strAppPath,"database","dbpwd",m_dbSQLPwd,iTemp,0);
			if (iRet < 0)
			{
				m_dbSQLPwd = "123456";
			}
			pFile->ReadInitInfo(strAppPath,"database","dbtype",strTemp,m_iDbType,1);			
			pFile->CheckInitValue(m_iDbType,1,4);
			pFile->ReadInitInfo(strAppPath,"database","dbport",strTemp,m_iDbPort,1);			
			pFile->CheckInitValue(m_iDbPort,0,1000000000);
			//�û�������Ϣ
			iRet = pFile->ReadInitInfo(strAppPath,"Userinfo","Account",m_strAccount,iTemp,0);
			if (iRet < 0)
			{
				m_strAccount = "13600000412";
			}
			iRet = pFile->ReadInitInfo(strAppPath,"Userinfo","Pwd",m_strAccPwd,iTemp,0);
			if (iRet < 0)
			{
				m_strAccPwd = "123456";
			}
			//ƽ̨��Ϣ
			iRet = pFile->ReadInitInfo(strAppPath,"Server","Ip",m_strServerIP,iTemp,0);
			if (iRet < 0)
			{
				m_strServerIP = "192.169.1.128";
			}
			iRet = pFile->ReadInitInfo(strAppPath,"Server","Port",strTemp,m_iServerPort,1);
			if (iRet < 0)
			{
				m_iServerPort = 8018;
			}
			//�Ŷ�
			iRet = pFile->ReadInitInfo(strAppPath,"NumberHead","Mobile",m_strMobileList,iTemp,0);
			if (iRet < 0)
			{
				m_strMobileList = "134,135,136,137,138,139,150,151,154,155,157,158,159,188";
			}
			iRet = pFile->ReadInitInfo(strAppPath,"NumberHead","Unicom",m_strUnicomList,iTemp,0);
			if (iRet < 0)
			{
				m_strUnicomList = "130,131,132,133,152,153,156";
			}
			iRet = pFile->ReadInitInfo(strAppPath,"NumberHead","Telecom",m_strTelecomList,iTemp,0);
			if (iRet < 0)
			{
				m_strTelecomList = "116,118,189";
			}
			iRet = pFile->ReadInitInfo(strAppPath,"NumberHead","Differ",strTemp,m_iDiffMobile,1);
			if (iRet < 0)
			{
				m_iDiffMobile = 0;
			}
			//�����߳�����
			iRet = pFile->ReadInitInfo(strAppPath,"Sendthread","Interval",strTemp,m_iSendInterval,1);
			if (iRet < 0)
			{
				m_iSendInterval = 500;
			}
			pFile->CheckInitValue(m_iSendInterval,5,2000);
			iRet = pFile->ReadInitInfo(strAppPath,"Sendthread","Bufnullwait",strTemp,m_iBufNullWaitTime,1);
			if (iRet < 0)
			{
				m_iBufNullWaitTime = 500;
			}
			pFile->CheckInitValue(m_iBufNullWaitTime,5,5000);
			iRet = pFile->ReadInitInfo(strAppPath,"Sendthread","Buflen",strTemp,g_iSendBufLen,1);
			pFile->CheckInitValue(g_iSendBufLen,100,1000);
			iRet = pFile->ReadInitInfo(strAppPath,"Sendthread","Splitlength",strTemp,m_iSplitLength,1);
			if (iRet < 0)
			{
				m_iSplitLength = 62;	//�ƶ�
			}
			pFile->CheckInitValue(m_iSplitLength,30,1000);
			iRet = pFile->ReadInitInfo(strAppPath,"Sendthread","SplitlengthUnicom",strTemp,m_iSplitLengthUnicom,1);
			if (iRet < 0)
			{
				m_iSplitLength =70;		//��ͨ
			}
			pFile->CheckInitValue(m_iSplitLength,30,1000);
			iRet = pFile->ReadInitInfo(strAppPath,"Sendthread","SplitlengthChinacom",strTemp,m_iSplitLengthChinaCom,1);
			if (iRet < 0)
			{
				m_iSplitLength = 70;	//����
			}
			pFile->CheckInitValue(m_iSplitLength,30,1000);
			iRet = pFile->ReadInitInfo(strAppPath,"Sendthread","SplitlengthChinaNum",strTemp,m_iSplitLengthChina,1);
			if (iRet < 0)
			{
				m_iSplitLength = 56;	//С��ͨ
			}
			pFile->CheckInitValue(m_iSplitLength,30,1000);

			//��ӻ�������
			iRet = pFile->ReadInitInfo(strAppPath,"Addbuf","NoMessage",strTemp,m_iAddNullWaitTime,1);
			pFile->CheckInitValue(m_iAddNullWaitTime,500,5000);

			//����ֹͣʱ��
			iRet = pFile->ReadInitInfo(strAppPath,"StopTime","Error",strTemp,m_iErrorStopTime,1);
			if (iRet < 0)
			{
				m_iErrorStopTime = 0;
			}
			pFile->CheckInitValue(m_iErrorStopTime,1,5000);

			//�Ӷ˿�
			iRet = pFile->ReadInitInfo(strAppPath,"SubPort","type",strTemp,m_iUseSubPort,1);
			if (iRet < 0)
			{
				m_iUseSubPort = 1;
			}
			pFile->CheckInitValue(m_iUseSubPort,0,1);

			//��������
			iRet = pFile->ReadInitInfo(strAppPath,"Recv","RecvMo",strTemp,m_iIsRecvMo,1);
			if (iRet < 0)
			{
				m_iIsRecvMo = 1;
			}
			pFile->CheckInitValue(m_iIsRecvMo,0,1);
			iRet = pFile->ReadInitInfo(strAppPath,"Recv","RecvReport",strTemp,m_iIsRecvReport,1);
			if (iRet < 0)
			{
				m_iIsRecvReport = 1;
			}
			pFile->CheckInitValue(m_iIsRecvReport,0,1);

			//����������
			iRet = pFile->ReadInitInfo(strAppPath,"PhoneSplit","PhoneSplit",strTemp,m_iPhoneSplitLen,1);
			if (iRet < 0)
			{
				m_iPhoneSplitLen = 500;
			}
			pFile->CheckInitValue(m_iPhoneSplitLen,1,1000);

			//����ʱ���
			iRet = pFile->ReadInitInfo(strAppPath,"runtime","time",m_strRunTime,iTemp,0);
			if ( iRet < 0)
			{
				m_strRunTime = "08:00--21:00";
			}
			//�ͻ��˼��Ƿ��ؼ���
			iRet = pFile->ReadInitInfo(strAppPath,"keycheck","type",strTemp,m_iCheckKey,1);
			if (iRet == 0)
			{
				m_iCheckKey = 0;
			}
			//���ռ��
			pFile->ReadInitInfo(strAppPath,"Recv","RecvMoInterval",strTemp,m_iRecvMoInterval,1);
			pFile->ReadInitInfo(strAppPath,"Recv","RecvReportInterval",strTemp,m_iRecvReportInterval,1);
			pFile->CheckInitValue(m_iRecvMoInterval,1,100);
			pFile->CheckInitValue(m_iRecvReportInterval,1,100);
			//��ʱ����
			pFile->ReadInitInfo(strAppPath,"Reconn","type",strTemp,m_iIsReconn,1);
			pFile->ReadInitInfo(strAppPath,"Reconn","interval",strTemp,m_iReconnInterval,1);
			pFile->CheckInitValue(m_iIsReconn,0,1);
			pFile->CheckInitValue(m_iReconnInterval,1,100000);

			//д��ʷ���߳�
			pFile->ReadInitInfo(strAppPath,"SaveHistThread","threadnum",strTemp,m_iAddHistThreadNum,1);
			pFile->CheckInitValue(m_iAddHistThreadNum,1,5);
			pFile->ReadInitInfo(strAppPath,"SaveHistThread","interval",strTemp,m_iAddHistInterval,1);
			pFile->CheckInitValue(m_iAddHistInterval,0,5);
			pFile->ReadInitInfo(strAppPath,"SaveHistThread","nullwait",strTemp,m_iAddHistNullWait,1);
			pFile->CheckInitValue(m_iAddHistNullWait,300,1000);
			pFile->ReadInitInfo(strAppPath,"SaveHistThread","numstopsend",strTemp,m_iNumSendStop,1);
			pFile->CheckInitValue(m_iNumSendStop,500,1500);
			//�������Ӽ��			
			pFile->ReadInitInfo(strAppPath,"Testconn","interval",strTemp,m_iTestConInterval,1);
			pFile->CheckInitValue(m_iTestConInterval,60,150);		
			
			//��������������
			pFile->ReadInitInfo(strAppPath,"samesingle","times",strTemp,m_iMostSameSingleSendTime,1);
			pFile->CheckInitValue(m_iMostSameSingleSendTime,5,1000);	
			m_strAccount = pFile->StrDecode(m_strAccount);
			m_strAccPwd = pFile->StrDecode(m_strAccPwd);
			m_dbSQLUser = pFile->StrDecode(m_dbSQLUser);
			m_dbSQLPwd = pFile->StrDecode(m_dbSQLPwd);
			
			pFile->ReadInitInfo(strAppPath,"Server","type",strTemp,g_iServerType,1);
			pFile->ReadInitInfo(strAppPath,"Report","flag",strTemp,g_iReportFlag,1);
			pFile->CheckInitValue(g_iServerType,0,1);
			pFile->CheckInitValue(g_iReportFlag,0,1);
			//��ʼ���Ŷ���Ϣ
			g_Conver.InitNumbers(m_strMobileList,m_strUnicomList,m_strTelecomList);
			int iCmdType = -99;
			pFile->ReadInitInfo(strAppPath,"acctype","cmdtype",strTemp,iCmdType,1);
			if (iCmdType == -99)
			{
				iCmdType = 25;
			}
			pFile->CheckInitValue(iCmdType,10,150);
		}
		else
		{
			AddToDispListShow("��ȡ�����ļ���ָ�����,�����˳�");
			Sleep(200);
			exit(0);
		}
	}
	catch (...)
	{
		WriteLog("InitParams�쳣",3);
	}
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
//���¼��������ļ�,����Ӧ����
void CMWMessageSendEx::ReInitParams()
{
	try
	{
		CString stroldAccount = m_strAccount;
		CString stroldAccPwd = m_strAccPwd;
		CString stroldIp = m_strServerIP;
		int ioldPort = m_iServerPort;
		CString stroldDbServer = m_dbSQLServer;
		CString stroldDbName = m_dbSQLName;
		CString stroldDbUser = m_dbSQLUser;
		CString stroldDbPwd = m_dbSQLPwd;
		int ioldServerType = g_iServerType;
		InitParams( );
		SetCmdTypeEx(g_iServerType,g_iReportFlag);
		if ( //���ݿ���Ϣ
			stroldDbServer != m_dbSQLServer||
			stroldDbName != m_dbSQLName||
			stroldDbUser != m_dbSQLUser||
			stroldDbPwd != m_dbSQLPwd||
			//��������Ϣ
			stroldAccount != m_strAccount||
			stroldAccPwd != m_strAccPwd||
			stroldIp != m_strServerIP||
			ioldPort != m_iServerPort||
			ioldServerType != g_iServerType)	//�����������Ϣ���κθ���,�����߳�
		{
			SetRunStatus(STATUS_STOP_NORMAL);	//����ֹͣ״̬
			Stop();
		}
	}	
	catch (...)
	{
		WriteLog("ReInitParams�쳣",3);
	}	
}
/************************************************************************/
/* ������������Ľ���,������һ���Ĳ�����־                              */
/************************************************************************/
//����0��ʾ�ɹ�,-1�Ͽ�����[�ط�]��-2��ͣ����[���ط�]��-3����ʧ�ܣ����ط�)
int CMWMessageSendEx::NextOptJudge(int iErrorCode,int iErrorType)
{
	if ( iErrorCode >= 0 )
	{
		return 0;
	}
	else
	{
		switch ( iErrorType )
		{
			case MSG_ERROR_KEEPONLINE:
				{
					return -1;	//�Ͽ�����
				}
			case MSG_ERROR_LOGIN:
				{
					return -1;	//�Ͽ�����
				}
			case MSG_ERROR_SEND:		
				{
					switch ( iErrorCode )
					{
					case -101:						
					case -102:					
					case -103:
					case -104:
					case -105:
					case -198:
					case -200:
						{							
							return -1;	//�Ͽ�����,�ط�
						}
					case -10015:
						{
							//strRet = "��������æ";
							return -1;
						}				
					case -10099:
						{
							//strRet = "��������";
							return -1;
						}
					case -10009:
						{
							//strRet = "��������";
							return -1;
						}
					case -10003:
						{
							//strRet = "�û�����";
							return -2;	//��ͣ����,���ط�
						}
					case -10037:
						{
							//strRet = "������ͬ����Ϣ����������ͣ";
							return -2;
						}					
					case -10044:
						{
							//strRet = "���ʺ�û����չ�Ӷ˿�Ȩ��";
							return -2;
						}					
					case -10029:
						{
							//strRet = "���û�û��Ȩ�޴Ӵ�ͨ��������Ϣ";
							return -2;
						}
					case -1:
						{
							//strError = "�����а�����NULLֵ";
							return -3;
							break;
						}
					case -2:
						{
							//strError = "�绰�����������1000";
							return -3;
							break;
						}
					case -10:
						{
							//strError = "���뻺��ռ�ʧ��";
							return -3;
							break;
						}			
					case -11:
						{
							//strError = "�绰�������з������ַ�";
							return -3;
							break;
						}
					case -12:
						{
							//strError = "���쳣�绰����";
							return -3;
							break;
						}
					case -13:
						{
							//strError = "�绰���������ʵ�ʸ��������";
							return -3;
							break;
						}
					case -14:
						{
							//strError = "ʵ�ʺ����������1000";
							return -3;
							break;
						}
					case -10017:
						{
							//strRet = "��ʱ��β�������";
							return -3;
						}
					case -10002:
						{
							//strRet = "�ύ��ʽ����ȷ";
							return -3;
						}
					case -10004:
						{
							//strRet = "�ֻ����벻��ȷ";
							return -3;
						}
					case -10008:
						{
							//strRet = "�˺����Ͳ�֧�ָù���";
							return -3;
						}					
					case -10011:
						{
							//strRet = "��Ϣ���ݳ���";
							return -3;
						}
					case -10012:
						{
							//strRet = "���ܷ�����ͨ����";
							return -3;
						}					
					case -10019:
						{
							//strRet = "�ֻ��б��������";
							return -3;
						}					
					case -10030:
						{
							//strRet = "���ܷ����ƶ�����";
							return -3;
						}
					case -10031:
						{
							//strRet = "�ֻ�����(��)�Ƿ�";
							return -3;
						}
					case -10033:
						{
							//strRet = "�Ƿ��ؼ���";
							return -3;
						}
					case -10035:
						{
							//strRet = "���ܷ��Ͷ���ֻ�";
							return -3;
						}
					case -10036:
						{
							//strRet = "���ܷ��͵����ֻ�";
							return -3;
						}
					case -10051:
						{
							//strRet = "ֻ�ܷ����ƶ�����";
							return -3;
						}
					case -10052:
						{
							//strRet = "ֻ�ܷ�����ͨ����";
							return -3;
							break;
						}
					case -10053:
						{
							//strRet = "ֻ�ܷ��͵���С��ͨ����";
							return -3;
						}
					default:
						{
							return -3;
						}
					}
					break;
				}
			case MSG_ERROR_RECVMO:
				{
					switch (iErrorCode)
					{
					case -101:						
					case -102:					
					case -103:
					case -104:
					case -105:
					case -198:
					case -200:
						{							
							return -1;
						}
					case -10015:
						{
							//strRet = "��������æ";
							return -1;
						}					
					case -10099:
						{
							//strRet = "��������";
							return -1;
						}
					case -10009:
						{
							//strRet = "��������";
							return -1;
						}
					case -300:
						{
							break;
						}
					case -10041:
						{
							//strRet = "��ȡ����MO��Ϣ̫��";
							break;
						}
					case -10042:
						{
							//strRet = "��ȡ״̬����̫��";
							break;
						}
					default:
						{
							return -1;
						}
					}
					break;
				}
			case MSG_ERROR_RECVREPORT:
				{
					switch (iErrorCode)
					{
					case -101:						
					case -102:					
					case -103:
					case -104:
					case -105:
					case -198:
					case -200:
						{							
							return -1;
						}
					case -10015:
						{
							//strRet = "��������æ";
							return -1;
						}					
					case -10099:
						{
							//strRet = "��������";
							return -1;
						}
					case -10009:
						{
							//strRet = "��������";
							return -1;
							break;
						}
					case -300:
						{
							break;
						}
					case -10041:
						{
							//strRet = "��ȡ����MO��Ϣ̫��";
							break;
						}
					case -10042:
						{
							//strRet = "��ȡ״̬����̫��";
							break;
						}
					default:
						{
							return -1;
						}
					}
					break;
				}
			case MSG_ERROR_CHECKFEE:
				{
					switch (iErrorCode)
					{
					case -101:						
					case -102:
					case -103:
					case -104:
					case -105:
					case -198:
					case -200:
						{
							return -1;
						}
					case -10015:
						{
							//strRet = "��������æ";
							return -1;
						}					
					case -10099:
						{
							//strRet = "��������";
							return -1;
						}
					case -10009:
						{
							//strRet = "��������";
							return -1;
						}
					case -10041:
						{
							//strRet = "��ȡ����MO��Ϣ̫��";
							break;
						}
					case -10042:
						{
							//strRet = "��ȡ״̬����̫��";
							break;
						}
					default:
						{
							return -1;
						}
					}
					break;
				}
			default:
				{
					break;
				}
		}
	}
	return 0;
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
//���߳�
void CMWMessageSendEx::Run()
{
	try
	{
		int i = 0;
		InitParams();
		//��ʼ���˻���Ϣ
		SetParams(m_strServerIP,m_iServerPort,m_strAccount,m_strAccPwd);
		//���������߳�
		m_SendThreadContrl.ThreadExit = FALSE;
		m_SendThreadContrl.pThread = AfxBeginThread((AFX_THREADPROC)MessageSendThreadProc,(LPVOID)this);
		m_iActiveThreadNum++;
		if (g_iServerType == 1)
		{
			//���������߳�
			m_RecvThreadContrl.ThreadExit = FALSE;
			m_RecvThreadContrl.pThread = AfxBeginThread((AFX_THREADPROC)RecvThreadProc,(LPVOID)this);
			m_iActiveThreadNum++;
		}		
		if(m_iAddHistThreadNum>5)
		{
			m_iAddHistThreadNum = 5;
		}
		for (i=0;i<m_iAddHistThreadNum;i++)
		{
			if (m_AddResultThreadContrl[i].ThreadExit == TRUE)
			{
				ThreadParam *lpParam = new ThreadParam;
				lpParam->pThis = this;
				lpParam->iThreadNO = i;
				m_AddResultThreadContrl[i].ThreadExit = FALSE;
				m_AddResultThreadContrl[i].pThread = AfxBeginThread((AFX_THREADPROC)AddSendResultThread,(LPVOID)lpParam);
				m_iActiveThreadNum++;
			}
		}
		SetRunStatus(STATUS_RUN_NORMAL);
	}
	catch (...)
	{
		WriteLog("Run�쳣",3);
	}
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
//ֹͣ�����߳�
void CMWMessageSendEx::Stop()
{
	try
	{
		m_RecvThreadContrl.ThreadExit = TRUE;
		m_SendThreadContrl.ThreadExit = TRUE;
		int i=0;
		for (i=0;i<5;i++)
		{
			m_AddResultThreadContrl[i].ThreadExit = TRUE;
		}
	}
	catch (...)
	{
		WriteLog("Stop�쳣",3);
	}	
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
//Ӧ�ó����˳�ʱ�Ĵ���
void CMWMessageSendEx::AppExit()
{
	try
	{
		CString strAppPath="";
		CUserFile *pFile = CUserFile::GetInstance();
		if ( pFile )
		{
			pFile->GetAppPath(strAppPath);
			pFile->WriteInitInfo(strAppPath+MWGATEINITFILE,"window","runstatus","",1,1);
		}
		HWND hwnd  = FindWindow(NULL,"���ظ�������_"+g_strWindowName);
		if (hwnd)
		{
			::PostMessage(hwnd,WM_CLOSE,0,0);
		}
		Sleep(200);
		//�½���һ�����������´������ݿ���߳�
		m_InsertBackContrl.pThread = AfxBeginThread((AFX_THREADPROC)InsertBackThreadProc, (LPVOID)this);
		m_InsertBackContrl.ThreadExit = TRUE;
		m_iActiveThreadNum++;
		if (m_iActiveThreadNum > 0)
		{
			SetRunStatus(STATUS_STOP_MANUAL);
			Stop();		//�ֶ���ֹ�߳�
			CDbGateClose dbClose;
			dbClose.m_ThreadNum = m_iActiveThreadNum;
			dbClose.DoModal();
			Sleep(100);
		}
	}
	catch (...)
	{
		WriteLog("AppExit�쳣",3);
	}	
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
//��ѯ����
void CMWMessageSendEx::CheckFee()
{
	try
	{
		AddToDispListShow("�ȴ���ѯ���,���Ժ�...");
		if (GetRunStatus() == STATUS_RUN_NORMAL)
		{
			m_CheckFeeFlag = true;
		}
		else
		{
			KfSubmitPack mt;
			DeliverPackList recvList;
			ReportPackList reportList;

			int iNextRet = -99;		//��һ����������ֵ
			int iiRet = -99;		//��������շ���ֵ
			int iErrorType = -99;	//��¼���صĴ�������
			CString strError = "";	//��¼���صĴ�����Ϣ
			//InitParams();
			iiRet = MessageOperate( OPTYPE_LOGIN,iErrorType,strError,mt,recvList,reportList);	
			iNextRet = NextOptJudge(iiRet,iErrorType);	
			if ( iNextRet != 0 ) //��������
			{
				AddToDispListShow(strError);
				Disconnect();			
				m_LastActiveTime = ::GetTickCount() - 10000000;	
				SetStatusInfoEx(1,0);	//�Ͽ�
				return;
			}
			iiRet = MessageOperate( OPTYPE_CHECKFEE,iErrorType,strError,mt,recvList,reportList);
			iNextRet = NextOptJudge(iiRet,iErrorType);
			if ( iNextRet != 0 ) //��������
			{
				Disconnect();
				m_LastActiveTime = ::GetTickCount() - 10000000;
				SetStatusInfoEx(1,0);	//�Ͽ�
				AddToDispListShow(strError);
				return;
			}
			AddToDispListShow(strError);			
			SetStatusInfoEx(3,iiRet);			
			m_CheckFeeFlag = false;			
		}
	}
	catch (...)
	{
		WriteLog("CheckFee�쳣",3);
	}	
}

//ȫ�ֺ���
/************************************************************************/
/*                                                                      */
/************************************************************************/
//������ʾ�б�
void AddToDispListShow(CString showStr,int iFlag)
{
	//������־
	if (iFlag == 0)
	{
		WriteLog(showStr,1);
	}
	showStr.Trim();
	g_DisListLock.Lock();
	try
	{		
		int iCount = g_DispList.GetItemCount();
		if ( iCount >= 1000 )		//ֻ����1000����¼
		{
			g_DispList.DeleteAllItems();
			iCount = 0;
		}
		if ( iCount > 0 )
		{
			if ( g_DispList.GetItemText(iCount-1,1) == showStr)
			{
				g_DispList.SetItemText(iCount-1 , 0, CTime::GetCurrentTime().Format("%Y-%m-%d %H:%M:%S") );
			}
			else
			{
				g_DispList.InsertItem(iCount,"");
				g_DispList.SetItemText(iCount , 0, CTime::GetCurrentTime().Format("%Y-%m-%d %H:%M:%S"));
				g_DispList.SetItemText(iCount , 1, showStr);
				g_DispList.PostMessage(WM_VSCROLL,SB_BOTTOM,0);
			}
		}
		else
		{
			g_DispList.InsertItem(iCount,"");
			g_DispList.SetItemText(iCount , 0, CTime::GetCurrentTime().Format("%Y-%m-%d %H:%M:%S"));
			g_DispList.SetItemText(iCount , 1, showStr);
			g_DispList.PostMessage(WM_VSCROLL,SB_BOTTOM,0);
		}		
	}	
	catch (...)
	{
		CString tempStr = "";
		tempStr.Format("AddToDispListShow�쳣,����:%s",showStr);
		WriteLog(tempStr,3);
	}
	g_DisListLock.Unlock();
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
void WriteConsoleInfo(CString srcStr)
{
	//CConsolePrinter::Instance()->print(srcStr);
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
//д��־
void WriteLog(CString srcStr,int iFlag)
{
	//WriteConsoleInfo(srcStr);
	if (g_iIsNeedToLog == 1)
	{
		CTime currTime = CTime::GetCurrentTime();
		try
		{
			CString filepath = "";
			CString destStr = "";
			CString folder = "";
			CString filename = "";
			CString strAppPath = "";
			if (iFlag == 0)			//������־
			{
				folder = "log";
				filename.Format("%s_sendlog.txt",currTime.Format("%Y%m%d"));
			}
			else if(iFlag == 1)		//��Ϣ��־
			{
				folder = "log";
				filename.Format("%s_log.txt",currTime.Format("%Y%m%d"));
			}
			else if ( iFlag == 2 )	//������־
			{
				folder = "log";
				filename.Format("%s_bufinfo.txt",currTime.Format("%Y%m%d"));
			}
			else			//�쳣��־ (3)
			{
				folder = "exception";
				filename.Format("%s_exception.txt",currTime.Format("%Y%m%d"));
			}

			CUserFile *pFile = CUserFile::GetInstance();
			if ( pFile )
			{
				pFile->GetAppPath(strAppPath);
			}
			CString folderName = "";
			folderName.Format("%s%s",strAppPath,folder);
			if(_access(folderName,0)!=0)
			{
				if( mkdir(folderName) != 0 )//����Ŀ¼
				{
					return;
				}
			}					
			destStr.Format("%s	%s",currTime.Format("%Y-%m-%d %H:%M:%S"),srcStr);
			filepath.Format("%s\\%s",folderName,filename);
			FILE *fp;
			char szfile[1000]={0};
			sprintf(szfile,filepath);
			fp=fopen(szfile,"a+");
			fprintf(fp,"%s\r\n",destStr);
			fclose(fp);
		}
		catch (...)
		{
		}
	}
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
//д������־
void WriteLogEx(CString srcStr,int iFlag)
{
	if (g_iDebugLog == 1)		//д������־��־Ϊ��ʱ,д������־
	{
		CTime currTime = CTime::GetCurrentTime();
		try
		{
			CString filepath = "";
			CString destStr = "";
			CString folder = "";
			CString filename = "";
			CString strAppPath = "";
			
			if(iFlag == 1)		//����,��,��,����,����־
			{
				folder = "debuglog";
				filename.Format("%s_netdebug.txt",currTime.Format("%Y%m%d"));
			}
			else if ( iFlag == 2 )	//���ݿ���������־
			{
				folder = "debuglog";
				filename.Format("%s_dbdebug.txt",currTime.Format("%Y%m%d"));
			}
			else if ( iFlag == 5 )	//���ݿ���������־
			{
				folder = "netlog";
				filename.Format("%s_netlog.txt",currTime.Format("%Y%m%d"));
			}
			else if ( iFlag > 45 )		//���淢����ʷ�ļ�¼
			{
				folder = "debuglog";
				filename.Format("%s_dbdebug_thread%d.txt",currTime.Format("%Y%m%d"),iFlag);
			}
			else			//������־ (3)
			{
				folder = "debuglog";
				filename.Format("%s_elsedebug.txt",currTime.Format("%Y%m%d"));
			}

			CUserFile *pFile = CUserFile::GetInstance();
			if ( pFile )
			{
				pFile->GetAppPath(strAppPath);
			}
			CString folderName = "";
			folderName.Format("%s%s",strAppPath,folder);
			if(_access(folderName,0)!=0)
			{
				if( mkdir(folderName) != 0 )//����Ŀ¼
				{
					return;
				}
			}					
			destStr.Format("%s	%s",currTime.Format("%Y-%m-%d %H:%M:%S"),srcStr);
			filepath.Format("%s\\%s",folderName,filename);
			FILE *fp;
			char szfile[1000]={0};
			sprintf(szfile,filepath);
			fp=fopen(szfile,"a+");
			fprintf(fp,"%s\r\n",destStr);
			fclose(fp);
		}
		catch (...)
		{
		}
	}
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CMWMessageSendEx::SetRunStatus( int iFlag )
{
	m_iRunStatus = iFlag;
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
int CMWMessageSendEx::GetRunStatus()
{
	return m_iRunStatus;
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
//�ж��Ƿ�����ȷ������ʱ�䷶Χ��,0��ȷ��1����
int CMWMessageSendEx::IsRightSendTime(CString strSendTime,CString strStartTime,CString strEndTime)
{
	if ( strStartTime == strEndTime )
	{
		return 0;
	}
	int iRet = 1;
	try
	{
		CTime currTime = CTime::GetCurrentTime();
		int iYear,iMonth,iDay;
		iYear = currTime.GetYear();
		iMonth = currTime.GetMonth();
		iDay = currTime.GetDay();
		int iHour,iMinite;
		int iStartHour,iStartMinite;
		int iEndHour,iEndMinite;

		CString strTemp = "";
		strTemp = strSendTime.Right(8);
		iHour = atoi(strTemp.Left(2));
		strTemp = strTemp.Right(5);
		iMinite = atoi(strTemp.Left(2));
		iStartHour = atoi(strStartTime.Left(2));
		iStartMinite = atoi(strStartTime.Right(2));
		iEndHour = atoi(strEndTime.Left(2));
		iEndMinite = atoi(strEndTime.Right(2));
		CTime sendTime,startTime,endTime;
		sendTime = CTime(iYear,iMonth,iDay,iHour,iMinite,0,0);
		startTime = CTime(iYear,iMonth,iDay,iStartHour,iStartMinite,0,0);
		endTime = CTime(iYear,iMonth,iDay,iEndHour,iEndMinite,0,0);
		CTimeSpan timeSpan1;
		CTimeSpan timeSpan2;
		CTimeSpan timeSpan;

		timeSpan1 = sendTime - startTime;		
		timeSpan2 = endTime - sendTime;

		timeSpan = endTime - startTime;
		if ( timeSpan < 0 )			//����ʱ��С�ڿ�ʼʱ��
		{
			if (timeSpan1 >= 0 || timeSpan2 > 0)
			{
				return 0;
			}
			else
			{
				return 1;
			}			
		}
		else if ( timeSpan > 0 )			//����ʱ����ڿ�ʼʱ��
		{
			if (timeSpan1 >= 0 && timeSpan2 > 0)
			{
				return 0;
			}
			else
			{
				return 1;
			}
		}
		else								//����ʱ����ڿ�ʼʱ��
		{
			return 0;
		}	
		iRet = 0;
	}
	catch (...)
	{
		iRet = 1;
		WriteLog("IsRightSendTime�쳣",3);
	}	
	return iRet;
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
//0��Ҫ��1�Ѿ����ڿ�ִ��ʱ���,2���ò���Ҫ [��ʱδʹ��]
int CMWMessageSendEx::IsNeedToReRunOnError()
{
	try
	{
		int iRet = IsRightSendTime(CTime::GetCurrentTime().Format("%Y-%m-%d %H:%M:%S"),m_strRunTime.Left(5),m_strRunTime.Right(5));
		if ( iRet == 0 )
		{
			if ( m_iAutoReRun == 1 )		//�������Ϊ��Ҫ�Զ�����
			{
				if ( GetRunStatus() == STATUS_STOP_ERROR )
				{
					return 0;
				}			
			}
			else
			{
				return 2;
			}
		}
		else
		{
			return 1;
		}		
	}
	catch (...)
	{
		WriteLog("IsNeedToReRunOnError�쳣",3);
	}
	return 0;
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
//�Զ�����������
int CMWMessageSendEx::IsNeedToReRunOnTime()
{
	return IsRightSendTime(CTime::GetCurrentTime().Format("%Y-%m-%d %H:%M:%S"),m_strRunTime.Left(5),m_strRunTime.Right(5));
}


/************************************************************************/
/*                                                                      */
/************************************************************************/
//����������ʱ��
void CMWMessageSendEx::SetLastErrorTime()
{
	m_iLastErrorTime = ::GetTickCount();
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
//�ж��Ƿ��Ѿ��������ͳ���ȴ�ʱ��
bool CMWMessageSendEx::IsOverLastErrorTime(unsigned int iErrorStopTime )
{
	DWORD t2 = 0;
	t2 = ::GetTickCount();
	DWORD tSpan = 0;
	tSpan = t2 - m_iLastErrorTime;
	if (tSpan > iErrorStopTime * 1000) //iDelay ��
	{
		return true;
	}
	else
	{
		AddToDispListShow("���ͳ���ȴ�");
		return false;
	}
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
//�ж��Ƿ��Ѿ��ﵽ����ʱ��
bool CMWMessageSendEx::IsOverLastReconnTime(unsigned int iReconnInterval)
{
	DWORD t2 = 0;
	t2 = ::GetTickCount();
	DWORD tSpan = 0;
	tSpan = t2 - m_iLastReconnTime;
	if (tSpan > iReconnInterval * 1000) //iDelay ��
	{
		return true;
	}
	else
	{
		return false;
	}
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
//�ϲ�������Ϣ,���غϲ��˶�������¼
int CMWMessageSendEx::UnionSameSingleMessage(CDataBaseCtrl &dataBaseCtrl)
{
	int iRet = 0;	
	m_SendMatchList.clear();
	//WriteLogEx("�ϲ�������Ϣ������ʼ",3);
	m_SendListLock.Lock();
	try
	{
		int iSize = m_SubmitPackList.GetSize();
		if (iSize == 0)	//������������
		{
			iRet = -1;
		}
		else
		{
			int i = 0;
			KfSubmitPack mt;
			KfSubmitPack mtSend;
			CString lastMsg = "";
			int iLastPhoneType = -100; //�ϸ��Ŷ�����
			int iUnionCount = 0;
			CString strTempID = "";
			for (i=0;i<iSize;i++)
			{
				mt = m_SubmitPackList.GetAt(i);
				if (lastMsg == "" && iLastPhoneType == -100)	//��һ����Ϣ
				{
					lastMsg = mt.m_strMsg;
					//��Ŷ�����
					iLastPhoneType = 1;
					if (dataBaseCtrl.IsSingleSend(mt.m_strUserNumber))	//�ǵ���
					{
						iLastPhoneType = ComputePhoneType(mt.m_strUserNumber);
						iUnionCount++;
						mt.m_strUserNumber.Trim();
						mtSend = mt;
						m_SendMatchList[mt.m_iLocalID] = mt;
						strTempID.Format("%d",mt.m_iLocalID);
						mtSend.m_strIdList += strTempID;											
					}
				}
				else
				{
					if (mt.m_strMsg == lastMsg && dataBaseCtrl.IsSingleSend(mt.m_strUserNumber)==true)
					{
						if (iLastPhoneType != ComputePhoneType(mt.m_strUserNumber))//��Ŷ�����
						{
							break;
						}
						iUnionCount++;
						mt.m_strUserNumber.Trim();
						mtSend.m_strUserNumber += " "+mt.m_strUserNumber;
						m_SendMatchList[mt.m_iLocalID] = mt;
						strTempID.Format("%d",mt.m_iLocalID);
						mtSend.m_strIdList += " "+strTempID;
						if (iUnionCount >= m_iPhoneSplitLen)
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
			if (iUnionCount > 1)	//����1��ʾΪ�����Ϣ
			{
				for (i=0;i<iUnionCount;i++)
				{
					m_SubmitPackList.RemoveAt(0);
				}
				mtSend.m_iSendType = 100;	//100��ʾ����Ϻ����Ϣ,�Ա㷢�ͺ����ƥ��
				m_SubmitPackList.InsertAt(0,mtSend);
			}
		}
	}
	catch( ... )
	{
		iRet = -2;		//����
		WriteLog("UnionSameSingleMessage�쳣",3);
	}
	m_SendListLock.Unlock();
	//WriteLogEx("�ϲ�������Ϣ��������",3);
	return iRet;
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
//�����ͺ����Ϣ������ʷ�б�
void CMWMessageSendEx::AddtoSendHistory(CDataBaseCtrl &dataCtrl,KfSubmitPack &mt,int iFlag,CString strError)
{
	int i=0;
	CString strPhoneArray[1001] = {""};
	CString strIdArray[1001] = {""};
	CString strMsgID = "";
	CString tempLeft = "";
	CString tempRight = "";
	CString tempStr = "";
	int iRight = 0;
	int tempLen = 0;
	KfSubmitPack mtSave;
	mt.m_strUserNumber.Trim();
	mt.m_strIdList.Trim();
	int iPhoneNum = 0;	
	try
	{
		iPhoneNum= StringSplit(mt.m_strUserNumber+" ",strPhoneArray,' ');
		StringSplit(mt.m_strIdList+" ",strIdArray,' ');
		tempStr = mt.m_strSpMsgId;
		tempStr.Trim();
		tempLen = tempStr.GetLength();
		tempLeft = tempStr.Left(tempLen - 6);
		tempRight = tempStr.Right(6);
		iRight = atoi(tempRight);
		mt.m_iTryTimes = 3;
		mt.m_strFailReason = strError;
		mt.m_strFailReason.Replace("'","''");
		mtSave = mt;
		m_SendHistoryLock.Lock();
		try
		{
			for (i=0;i<iPhoneNum;i++)
			{
				if (iFlag != 0)	//���ʧ��
				{
					strMsgID = "88888888";
				}
				else
				{
					//iRight += i;
					strMsgID.Format("%06d",iRight+i);
					strMsgID = tempLeft + strMsgID;
				}
				if (mt.m_iSendType==100)	//�������Ϻ����Ϣ
				{
					GetMatchMtPack(mtSave,atoi(strIdArray[i]));
					if (mtSave.m_iLocalID <= 0)	//û�л�ȡ��
					{
						mtSave = mt;
					}
				}
				mtSave.m_strFailReason = strError;
				mtSave.m_iSendResultFlag = iFlag;
				mtSave.m_strUserNumber = strPhoneArray[i];
				mtSave.m_strMsg = mt.m_strMsg;
				mtSave.m_strSpMsgId = strMsgID;
				mtSave.m_iCount = 1;
				m_SendHistoryList.Add(mtSave);
			}
		}
		catch (...)
		{
		}	
		m_SendHistoryLock.Unlock();
	}
	catch (...)
	{
	}
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
//��ӷ�����ʷ��
int CMWMessageSendEx::AddSendResult(CDataBaseCtrl &dataCtrl,KfSubmitPack &mt,int iFlag)
{
	return dataCtrl.AddSendResult(mt,iFlag);
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
//�˴����ڲ�ֺ��뼰ƥ��ID��
int CMWMessageSendEx::StringSplit(CString srcStr,CString strArray[],char splitChar)
{
	try
	{
		int idx1 = 0;
		int idx2 = 0;
		int iCount = 0;
		idx2 = srcStr.Find(splitChar, idx1);  //�ҵ�һ����¼
		while(idx2 != -1)						//����ҵ�
		{ 
			strArray[iCount] = srcStr.Mid(idx1,idx2 - idx1); //ȡ��һ����¼
			idx1 = idx2 + 1; 
			idx2 = srcStr.Find(splitChar, idx1);  //����һ����¼
			iCount++;
		} 
		return iCount;
	}
	catch (...)
	{
	}
	return 0;
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
//����ƥ��ĵ�����Ϣ
void CMWMessageSendEx::GetMatchMtPack(KfSubmitPack &mt,int iLocalID)
{
	mt = m_SendMatchList[iLocalID];
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
//���عؼ���
void CMWMessageSendEx::LoadKeys()
{
	CString strTemp = "";
	CString strAppPath = "";
	CUserFile *pFile = CUserFile::GetInstance();
	if (pFile != NULL)
	{
		pFile->GetAppPath(strAppPath);
		strAppPath += KEYWORDFILE;
		//���طǷ��ؼ����б�
		m_keyWord.SetDBInfo(strAppPath);
		int iCount = 0;
		int iRet = m_keyWord.LoadAllKeyWords(iCount);
		if ( iRet == 0 )
		{
			strTemp.Format("�ɹ�����%d���Ƿ��ؼ���",iCount);
			AddToDispListShow(strTemp);
		}
		else
		{
			switch(iRet)
			{
			case -3:
				AddToDispListShow("���طǷ��ؼ����б��쳣");
				break;
			default:
				AddToDispListShow("���طǷ��ؼ����б�ʧ��");
				break;
			}			
		}
	}
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
//����δ����¼
void CMWMessageSendEx::WriteBufToFile(KfSubmitPack &mt)
{
	CString strBuf = "";
	mt.m_strMsg.Replace("'","");
	try
	{
		strBuf.Format("%d'%d'%d'%d'%d'%d'%d'%d'%d'%d'%d'%d'%d'%d'%s'%s'%s'%s'%s'%s'%s'%s'%s'%s'%s'%s'%s'%s'%s'%s'%s'%s'%s'%s'%s'%s",
			mt.m_iSendType,				//��������
			mt.m_iCount,				//���պ�����
			mt.m_iCountA,				//��ʱ���պ�����
			mt.m_iPhoneType,			//�����Ŷ�
			mt.m_iTryTimes,				//���Դ���
			mt.m_iMsgLen,				//��Ϣ����
			mt.m_iPrecedence,			//�������ȼ�
			mt.m_iLocalID,				//����ID(INT��)
			mt.m_iReverse1,				//�����ֶ�1
			mt.m_iReverse2,				//�����ֶ�2
			mt.m_iReverse3,				//�����ֶ�3
			mt.m_iReverse4,				//�����ֶ�4
			mt.m_iReverse5,				//�����ֶ�5
			mt.m_iSendResultFlag,		//���ͽ����0�ɹ���1ʧ��
			mt.m_strSendType,			//��������
			mt.m_strUserNumber,			//���շ������б�
			mt.m_strUserNumberA,		//��ʱ���շ������б�
			mt.m_strPtMsgId,			//��Ϣ���ر�ʶ
			mt.m_strSpMsgId,			//��ϢID(���ͳɹ������Ϣ���)
			mt.m_strMsg,				//��������
			mt.m_strCreatTime,			//��Ϣ����ʱ��
			mt.m_strSubTime,			//��Ϣ�ύ�����ݿ�ʱ��
			mt.m_strOperatorID,			//����ԱID
			mt.m_strServiceID,			//����ID
			mt.m_strTaskName,			//������
			mt.m_strLocalID,			//��Ϣ����ID
			mt.m_strSubOprationType,	//�Ӳ�������
			mt.m_strSubPort,			//�Ӷ˿�
			mt.m_strUserId,				//�����ֶ�
			mt.m_strMsgStatus,			//�����ֶ�1
			mt.m_strRecvStatus,			//�����ֶ�2
			mt.m_strMainPort,			//�����ֶ�3
			mt.m_strFailReason,			//�����ֶ�4
			mt.m_strReverse5,			//�����ֶ�5
			mt.m_strReverse6,			//�����ֶ�6
			mt.m_strIdList);			//���ڱ���ϲ������ϢID����)
		FILE *fp;
		char szfile[1000]={0};
		CString strAppPath = "";
		CUserFile *pFile = CUserFile::GetInstance();
		if ( pFile )
		{
			pFile->GetAppPath(strAppPath);
			sprintf(szfile,strAppPath+"\\"+BUFSAVEFILE);
			fp=fopen(szfile,"a+");
			fprintf(fp,"%s\r\n",pFile->StrEncode(strBuf));
			fclose(fp);
		}		
	}
	catch (...)
	{
	}	
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
//��δ���͵ļ�¼����
void CMWMessageSendEx::ReadBufFromFile()
{
	CString strBuf = "'";
	CString m_strFileName = "";
	KfSubmitPack mt;
	CString strArray[100] = {""};
	try
	{
		CString strAppPath = "";
		CUserFile *pFile = CUserFile::GetInstance();
		if ( pFile )
		{
			pFile->GetAppPath(strAppPath);			
		}
		else
		{
			return;
		}
		m_strFileName = strAppPath+"\\"+BUFSAVEFILE;
		if(_access(m_strFileName,0)!=0)
		{
			return;
		}
		
		CStdioFile   file;
		CString   cstr ="";
		CFileException   fe;
		if(file.Open(m_strFileName,CFile::modeRead,&fe))   
		{ 
			file.SeekToBegin();
			while (file.ReadString(cstr))
			{
				cstr.Trim();
				if (cstr != "")
				{
					cstr = pFile->StrDecode(cstr);
					StringSplit(cstr,strArray,'\'');
					mt.m_iSendType=atoi(strArray[0]);			//��������
					mt.m_iCount=atoi(strArray[1]);				//���պ�����
					mt.m_iCountA=atoi(strArray[2]);				//��ʱ���պ�����
					mt.m_iPhoneType=atoi(strArray[3]);			//�����Ŷ�
					mt.m_iTryTimes=atoi(strArray[4]);			//���Դ���
					mt.m_iMsgLen=atoi(strArray[5]);				//��Ϣ����
					mt.m_iPrecedence=atoi(strArray[6]);			//�������ȼ�
					mt.m_iLocalID=atoi(strArray[7]);			//����ID(INT��)
					mt.m_iReverse1=atoi(strArray[8]);			//�����ֶ�1
					mt.m_iReverse2=atoi(strArray[9]);			//�����ֶ�2
					mt.m_iReverse3=atoi(strArray[10]);			//�����ֶ�3
					mt.m_iReverse4=atoi(strArray[11]);			//�����ֶ�4
					mt.m_iReverse5=atoi(strArray[12]);			//�����ֶ�5
					mt.m_iSendResultFlag=atoi(strArray[13]);	//���ͽ����0�ɹ���1ʧ��
					mt.m_strSendType=strArray[14];			//��������
					mt.m_strUserNumber=strArray[15];		//���շ������б�
					mt.m_strUserNumberA=strArray[16];		//��ʱ���շ������б�
					mt.m_strPtMsgId=strArray[17];			//��Ϣ���ر�ʶ
					mt.m_strSpMsgId=strArray[18];			//��ϢID(���ͳɹ������Ϣ���)
					mt.m_strMsg=strArray[19];				//��������
					mt.m_strCreatTime=strArray[20];			//��Ϣ����ʱ��
					mt.m_strSubTime=strArray[21];			//��Ϣ�ύ�����ݿ�ʱ��
					mt.m_strOperatorID=strArray[22];		//����ԱID
					mt.m_strServiceID=strArray[23];			//����ID
					mt.m_strTaskName=strArray[24];			//������
					mt.m_strLocalID=strArray[25];			//��Ϣ����ID
					mt.m_strSubOprationType=strArray[26];	//�Ӳ�������
					mt.m_strSubPort=strArray[27];			//�Ӷ˿�
					mt.m_strUserId=strArray[28];			//�����ֶ�
					mt.m_strMsgStatus=strArray[29];			//�����ֶ�1
					mt.m_strRecvStatus=strArray[30];		//�����ֶ�2
					mt.m_strMainPort=strArray[31];			//�����ֶ�3
					mt.m_strFailReason=strArray[32];		//�����ֶ�4
					mt.m_strReverse5=strArray[33];			//�����ֶ�5
					mt.m_strReverse6=strArray[34];			//�����ֶ�6
					mt.m_strIdList=strArray[35];
					mt.m_timeSendTime = COleDateTime::GetCurrentTime();
					if (mt.m_iCount > 0)
					{
						AddRealSendPack(mt);
					}					
				}
			}
			file.Close();
		}
		file.Remove(m_strFileName);
		AddToDispListShow("�ɹ����ļ����ص�������Ϣ");
	}
	catch (...)
	{
	}	
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
void CMWMessageSendEx::SetFrameHwnd(HWND hwnd)
{
	m_hWnd = hwnd;
}

/************************************************************************/
/*���㵥����Ϣ�������Ŷ�                                                  */
/************************************************************************/
int CMWMessageSendEx::ComputePhoneType(CString &strPhone)
{
	int iPhoneType = -1;
	strPhone.Trim();
	iPhoneType = g_Conver.MobileType(strPhone);			
	if (m_iDiffMobile == 0)
	{
		if ( iPhoneType == 1 || iPhoneType == 3)	//����ͨ����źŶ���Ϊ�ƶ�
		{
			iPhoneType = 0;
		}
	}
	return iPhoneType;
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
void CMWMessageSendEx::AddSingleSend(CString strPhone,CString strContent,CString strSubport)
{
	try
	{
		KfSubmitPack mt;
		mt.m_iCount = 1;
		mt.m_strUserNumber = strPhone;
		mt.m_strMsg = strContent;
		COleDateTime currTime = COleDateTime::GetCurrentTime();
		mt.m_strSubTime = currTime.Format("%y-%m-%d %H:%M:%S");
		mt.m_strCreatTime = mt.m_strSubTime;
		mt.m_timeSendTime = currTime;
		mt.m_strSubPort = strSubport;
		mt.m_strPtMsgId = "88888888";
		mt.m_strSpMsgId = "88888888";
		mt.m_iLocalID = 0;
		m_SubmitPackList.Add(mt);
	}
	catch (...)
	{
	}	
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
//ͨ��������Ϣ����״̬����Ϣ
void CMWMessageSendEx::SetStatusInfoEx(int iIndex,int iCount)
{
	try
	{
		if (m_hWnd != NULL)
		{
			::PostMessage(m_hWnd,WM_USERMSGSTATUS,iIndex,iCount);
		}
		else
		{
			WriteLog("����״̬����Ϣʱ���Ϊ��",3);
		}
	}
	catch (...)
	{
		WriteLog("SetStatusInfoEx�쳣",3);
	}	
}
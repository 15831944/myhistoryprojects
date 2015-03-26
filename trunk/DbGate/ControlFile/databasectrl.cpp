#include "stdafx.h"
#include ".\databasectrl.h"
#include "UserFile.h"
#include <io.h>
/************************************************************************/
/*                                                                      */
/************************************************************************/
CDataBaseCtrl::CDataBaseCtrl(void)
{
	int i=0;
	for ( i=0;i<35;i++ )
	{
		m_iReadIndex[i] = -1;
		m_iSucAddResultIndex[i] = -1;
		m_iFailAddResultIndex[i] = -1;
		m_iSucCleanIndex[i] = -1;
		m_iFailCleanIndex[i] = -1;
		m_iAddResultIndex[i] = -1;
		m_iAddResultParamLen[i] = 0;
		m_strReadParams[i].Empty();
		m_strAddResultParams[i].Empty();
		m_strSucAddResultParams[i].Empty();
		m_strFailAddResultParams[i].Empty();
		m_strSucCleanParams[i].Empty();
		m_strFailCleanParams[i].Empty();
	}
	for ( i=0;i<7;i++ )
	{
		m_iUpdateIndex[i] = -1;
		m_iAddMoIndex[i] = -1;
		m_iAddmoParamLen[i] = 0;
		m_iUpdateParamLen[i] = 0;
		m_strUpdateParams[i].Empty();
		m_strAddmoParams[i].Empty();
	}

	m_dbServer = "127.0.0.1";
	m_dbName = "DB_CustomSMS";
	m_dbUser = "sa";
	m_dbPwd = "123456";
	m_strPhoneFilePath = "C:\\EIE\\was\\upfile\\tempfile\\";
	m_strSplitString = " ";
	m_iOptFlag = 0;
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
CDataBaseCtrl::~CDataBaseCtrl(void)
{
	m_adoConnect.Close();
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
void CDataBaseCtrl::InitParams(CString strdbServer,CString strDbName,CString strDbUser,CString strDbPwd,int iDbType,int iDbPort)
{
	m_dbServer = strdbServer;
	m_dbName = strDbName;
	m_dbUser = strDbUser;
	m_dbPwd = strDbPwd;
	m_idbType = iDbType;
	m_idbPort = iDbPort;
	ReadSqlParams();
	InitArray(m_strReadIndexList,1);
	InitArray(m_strSucAddResultIndexList,2);
	InitArray(m_strFailAddResultIndexList,3);
	InitArray(m_strSucCleanIndexList,4);
	InitArray(m_strFailCleanIndexList,5);
	InitArray(m_strAddMoIndexList,6);
	InitArray(m_strUpdateIndexList,7);
	InitArray(m_strAddResultStoreIndexList,8);
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
//��ȡ���ݿ�ִ�����������Ϣ
void CDataBaseCtrl::ReadSqlParams()
{
	CString strTemp = "";
	int iTemp = 0;
	CUserFile *pFile = NULL;
	pFile = CUserFile::GetInstance();
	CString strFurtherName = "";
	if (pFile)
	{	
		int iRet = -1;
		CString strAppPath = "";
		pFile->GetAppPath(strAppPath);
		CString strIniFile = strAppPath + DBSETINITFILE;
		//���ݿ���Ϣ
		pFile->ReadInitInfo(strIniFile,"opttype","type",strTemp,m_iOptFlag,1);
		if ( m_iOptFlag == 1 )
		{
			strFurtherName = "normal";
		}
		else if ( m_iOptFlag == 2)
		{
			strFurtherName = "own";
		}
		else if ( m_iOptFlag == 3 )	//�洢����
		{
			strFurtherName = "store";
		}
		else		//Ĭ�ϵ�����1
		{
			strFurtherName = "normal";
		}
		
		//��ȡ
		pFile->ReadInitInfo(strIniFile,"db","read"+strFurtherName,m_strReadSql,iTemp,0);
		pFile->ReadInitInfo(strIniFile,"db","readindex"+strFurtherName,m_strReadIndexList,iTemp,0);
		//���ͳɹ��󱣴�
		pFile->ReadInitInfo(strIniFile,"db","sucadd"+strFurtherName,m_strSucAddResultSql,iTemp,0);
		pFile->ReadInitInfo(strIniFile,"db","sucaddindex"+strFurtherName,m_strSucAddResultIndexList,iTemp,0);
		//����ʧ�ܺ���
		pFile->ReadInitInfo(strIniFile,"db","failadd"+strFurtherName,m_strFailAddResultSql,iTemp,0);
		pFile->ReadInitInfo(strIniFile,"db","failaddindex"+strFurtherName,m_strFailAddResultIndexList,iTemp,0);
		//���ͳɹ����������Ϣ
		pFile->ReadInitInfo(strIniFile,"db","succlean"+strFurtherName,m_strSucCleanSql,iTemp,0);
		pFile->ReadInitInfo(strIniFile,"db","succleanindex"+strFurtherName,m_strSucCleanIndexList,iTemp,0);		
		//����ʧ�ܴ��������Ϣ
		pFile->ReadInitInfo(strIniFile,"db","failclean"+strFurtherName,m_strFailCleanSql,iTemp,0);
		pFile->ReadInitInfo(strIniFile,"db","failcleanindex"+strFurtherName,m_strFailCleanIndexList,iTemp,0);
		//״̬���洦��
		pFile->ReadInitInfo(strIniFile,"db","update"+strFurtherName,m_strUpdateSql,iTemp,0);			pFile->ReadInitInfo(strIniFile,"db","updateindex"+strFurtherName,m_strUpdateIndexList,iTemp,0);
		//������Ϣ����
		pFile->ReadInitInfo(strIniFile,"db","addmo"+strFurtherName,m_strAddMoSql,iTemp,0);		
		pFile->ReadInitInfo(strIniFile,"db","addmoindex"+strFurtherName,m_strAddMoIndexList,iTemp,0);
		
		//���ͱ���(ר��Ϊ�洢��������)
		pFile->ReadInitInfo(strIniFile,"db","add"+strFurtherName,m_strAddResultStoreName,iTemp,0);
		pFile->ReadInitInfo(strIniFile,"db","addindex"+strFurtherName,m_strAddResultStoreIndexList,iTemp,0);
		m_strReadSql = pFile->StrDecode(m_strReadSql);
		m_strReadIndexList = pFile->StrDecode(m_strReadIndexList);
		///////////////////////////////////////////////////////////
		m_strAddResultStoreName = pFile->StrDecode(m_strAddResultStoreName);
		m_strAddResultStoreIndexList = pFile->StrDecode(m_strAddResultStoreIndexList);
		///////////////////////////////////////////////////////////
		m_strSucAddResultSql = pFile->StrDecode(m_strSucAddResultSql);
		m_strSucAddResultIndexList = pFile->StrDecode(m_strSucAddResultIndexList);

		m_strFailAddResultSql = pFile->StrDecode(m_strFailAddResultSql);
		m_strFailAddResultIndexList = pFile->StrDecode(m_strFailAddResultIndexList);

		m_strSucCleanSql = pFile->StrDecode(m_strSucCleanSql);
		m_strSucCleanIndexList = pFile->StrDecode(m_strSucCleanIndexList);
		m_strFailCleanSql = pFile->StrDecode(m_strFailCleanSql);
		m_strFailCleanIndexList = pFile->StrDecode(m_strFailCleanIndexList);

		m_strUpdateSql = pFile->StrDecode(m_strUpdateSql);	
		m_strUpdateIndexList = pFile->StrDecode(m_strUpdateIndexList);	

		m_strAddMoSql = pFile->StrDecode(m_strAddMoSql);	
		m_strAddMoIndexList = pFile->StrDecode(m_strAddMoIndexList);
		
		m_strReadSql.Replace("{","");
		m_strReadSql.Replace("}","");
		m_strSucAddResultSql.Replace("{","");
		m_strSucAddResultSql.Replace("}","");
		m_strFailAddResultSql.Replace("{","");
		m_strFailAddResultSql.Replace("}","");
		m_strSucCleanSql.Replace("{","");
		m_strSucCleanSql.Replace("}","");
		m_strFailCleanSql.Replace("{","");
		m_strFailCleanSql.Replace("}","");
		m_strUpdateSql.Replace("{","");
		m_strUpdateSql.Replace("}","");
		m_strAddMoSql.Replace("{","");
		m_strAddMoSql.Replace("}","");
    	m_strMsgStatus[0] = "-1";
		m_strMsgStatus[1] = "-1";
		m_iMsgStatus[0] = -99;
		m_iMsgStatus[1] = -99;
		m_strRecvStatus[0] = "-1";
		m_strRecvStatus[1] = "-1";
		m_strRecvStatus[2] = "-1";
		m_strRecvStatus[3] = "-1";
		m_strRecvStatus[4] = "-1";
		m_iRecvStatus[0] = -99;
		m_iRecvStatus[1] = -99;
		m_iRecvStatus[2] = -99;
		m_iRecvStatus[3] = -99;
		m_iRecvStatus[4] = -99;
		//�ָ�������
		pFile->ReadInitInfo(strIniFile,"spliter","char",m_strSplitString,iTemp,0);
		//��ȡ״̬ӳ������
		CString strTempStatus = "";
		pFile->ReadInitInfo(strIniFile,"status","send",strTempStatus,iTemp,0);
		pFile->SplitMsg(strTempStatus,m_strMsgStatus,",",2);
		int i=0;
		for (i=0;i<2;i++)
		{
			m_iMsgStatus[i] = atoi(m_strMsgStatus[i]);
		}
		strTempStatus.Empty();
		pFile->ReadInitInfo(strIniFile,"status","recv",strTempStatus,iTemp,0);
		pFile->SplitMsg(strTempStatus,m_strRecvStatus,",",5);
		for ( i=0;i<5;i++)
		{
			m_iRecvStatus[i] = atoi(m_strRecvStatus[i]);
		}
		strIniFile = strAppPath + MWGATEINITFILE;
		pFile->ReadInitInfo(strIniFile,"phonefile","path",m_strPhoneFilePath,iTemp,0);
		if (m_strPhoneFilePath == "")
		{
			m_strPhoneFilePath = "C:\\EIE\\was\\upfile\\tempfile\\";
		}
		pFile->ReadInitInfo(strIniFile,"database","charset",m_strCharSet,iTemp,0);		
	}
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
int CDataBaseCtrl::ReadWaitMessage(KfSubmitPackList &sendBufList,CCriticalSection &sendListLock)
{
	try
	{		
		if (!ConnectDataBase(m_adoConnect,m_adoRcrd,m_idbType))
		{
			AddToDispListShow("�����߳��������ݿ�ʧ��["+m_adoConnect.GetLastErrorText()+"]");
			return -3;
		}		
		if (m_iOptFlag == 3) //�洢����
		{
			CAdoCommand adoCmd;
			adoCmd.SetConnection(&m_adoConnect);
			adoCmd.SetCommandText(m_strReadSql);
			int iRet = -1;
			if (m_idbType == 2)		//oracle���ݿ�
			{
				adoCmd.SetCommandText("{call "+m_strReadSql+"()}");
				m_adoRcrd.SetRecordSetPtr(adoCmd.Execute(iRet,adCmdStoredProc|adCmdUnspecified));
			}
			else
			{
				m_adoRcrd.SetRecordSetPtr(adoCmd.Execute(iRet));
			}
			if (iRet == -1)
			{
				AddToDispListShow(m_adoConnect.GetLastErrorText());
				return -1;
			}
		}
		else				//�Ǵ洢����
		{
			CString strSQL = "";
			strSQL.Format(m_strReadSql,g_iSendBufLen);
			if(!m_adoRcrd.Open(strSQL,adCmdText))
			{
				WriteLog("��ȡ�ȴ�������Ϣ����",1);
				AddToDispListShow(m_adoConnect.GetLastErrorText());
				m_adoRcrd.Close();
				Sleep(2000);			
				return -2;
			}
		}		
		//���û�����ݣ�ֱ�ӷ���
		if(m_adoRcrd.IsEOF())
		{
			m_adoRcrd.Close();				
			return 0;
		}
		CString strTemp = "";		//������
		int iTemp = 0;
		COleDateTime tempTime = COleDateTime::GetCurrentTime();
		KfSubmitPack submitPack;
		int iIndex = 0;
		BOOL bRet = FALSE;
		while(!m_adoRcrd.IsEOF())			//�����¼��Ϊ��
		{
			for (iIndex = 0; iIndex < 35; iIndex++)
			{
				if (m_iReadIndex[iIndex] == -1)
				{
					break;
				}
				switch (m_iReadIndex[iIndex])
				{
				case 1:			//m_iSendType
					{
						bRet = m_adoRcrd.GetCollect(m_strReadParams[iIndex],iTemp);
						submitPack.m_iSendType = iTemp;
						break;
					}
				case 2:			//m_iCount
					{
						bRet = m_adoRcrd.GetCollect(m_strReadParams[iIndex],iTemp);
						submitPack.m_iCount = iTemp;
						break;
					}
				case 3:			//m_iCountA
					{
						bRet = m_adoRcrd.GetCollect(m_strReadParams[iIndex],iTemp);
						submitPack.m_iCountA = iTemp;
						break;
					}
				case 4:			//m_iPhoneType
					{
						bRet = m_adoRcrd.GetCollect(m_strReadParams[iIndex],iTemp);
						submitPack.m_iPhoneType = iTemp;
						break;
					}
				case 5:			//m_iTryTimes
					{
						bRet = m_adoRcrd.GetCollect(m_strReadParams[iIndex],iTemp);
						submitPack.m_iTryTimes = iTemp;
						break;
					}
				case 6:			//m_iMsgLen
					{
						bRet = m_adoRcrd.GetCollect(m_strReadParams[iIndex],iTemp);
						submitPack.m_iMsgLen = iTemp;
						break;
					}
				case 7:			//m_iPrecedence
					{
						bRet = m_adoRcrd.GetCollect(m_strReadParams[iIndex],iTemp);
						submitPack.m_iMsgLen = iTemp;
						break;
					}
				case 8:			//m_iLocalID
					{
						bRet = m_adoRcrd.GetCollect(m_strReadParams[iIndex],iTemp);
						submitPack.m_iLocalID = iTemp;
						break;
					}
				case 9:			//m_iReverse1
					{
						bRet = m_adoRcrd.GetCollect(m_strReadParams[iIndex],iTemp);
						submitPack.m_iReverse1 = iTemp;
						break;
					}
				case 10:		//m_iReverse2
					{
						bRet = m_adoRcrd.GetCollect(m_strReadParams[iIndex],iTemp);
						submitPack.m_iReverse2 = iTemp;
						break;
					}
				case 11:		//m_iReverse3
					{
						bRet = m_adoRcrd.GetCollect(m_strReadParams[iIndex],iTemp);
						submitPack.m_iReverse3 = iTemp;
						break;
					}
				case 12:		//m_iReverse4
					{
						bRet = m_adoRcrd.GetCollect(m_strReadParams[iIndex],iTemp);
						submitPack.m_iReverse4 = iTemp;
						break;
					}
				case 13:		//m_iReverse5
					{
						bRet = m_adoRcrd.GetCollect(m_strReadParams[iIndex],iTemp);
						submitPack.m_iReverse5 = iTemp;
						break;
					}
				case 14:		//m_strSendType
					{
						bRet = m_adoRcrd.GetCollect(m_strReadParams[iIndex],strTemp);
						strTemp.Trim();
						submitPack.m_strSendType = strTemp;
						break;
					}
				case 15:		//m_strUserNumber
					{
						bRet = m_adoRcrd.GetCollect(m_strReadParams[iIndex],strTemp);
						strTemp.Trim();
						submitPack.m_strUserNumber = strTemp;
						break;
					}
				case 16:		//m_strUserNumberA
					{
						bRet = m_adoRcrd.GetCollect(m_strReadParams[iIndex],strTemp);
						strTemp.Trim();
						submitPack.m_strUserNumber = strTemp;
						break;
					}
				case 17:		//m_strPtMsgId
					{
						bRet = m_adoRcrd.GetCollect(m_strReadParams[iIndex],strTemp);
						strTemp.Trim();
						submitPack.m_strPtMsgId = strTemp;
						break;
					}
				case 18:		//m_strSpMsgId
					{
						bRet = m_adoRcrd.GetCollect(m_strReadParams[iIndex],strTemp);
						strTemp.Trim();
						submitPack.m_strSpMsgId = strTemp;
						break;
					}
				case 19:		//m_strMsg
					{
						bRet = m_adoRcrd.GetCollect(m_strReadParams[iIndex],strTemp);
						strTemp.Trim();
						submitPack.m_strMsg = strTemp;
						break;
					}
				case 20:		//m_strCreatTime
					{
						bRet = m_adoRcrd.GetCollect(m_strReadParams[iIndex],tempTime);
						submitPack.m_strCreatTime = tempTime.Format("%Y-%m-%d %H:%M:%S");
						break;
					}
				case 21:		//m_strSubTime
					{
						bRet = m_adoRcrd.GetCollect(m_strReadParams[iIndex],tempTime);
						submitPack.m_strSubTime = tempTime.Format("%Y-%m-%d %H:%M:%S");
						break;
					}
				case 22:		//m_timeSendTime
					{
						submitPack.m_timeSendTime = COleDateTime::GetCurrentTime();//.Format("%Y-%m-%d %H:%M:%S");
						break;
					}
				case 23:		//m_strOperatorID
					{
						bRet = m_adoRcrd.GetCollect(m_strReadParams[iIndex],strTemp);
						strTemp.Trim();
						submitPack.m_strOperatorID = strTemp;
						break;
					}
				case 24:		//m_strServiceID
					{
						bRet = m_adoRcrd.GetCollect(m_strReadParams[iIndex],strTemp);
						strTemp.Trim();
						submitPack.m_strServiceID = strTemp;
						break;
					}
				case 25:		//m_strTaskName
					{
						bRet = m_adoRcrd.GetCollect(m_strReadParams[iIndex],strTemp);
						strTemp.Trim();
						submitPack.m_strTaskName = strTemp;
						break;
					}
				case 26:		//m_strLocalID
					{
						bRet = m_adoRcrd.GetCollect(m_strReadParams[iIndex],strTemp);
						strTemp.Trim();
						submitPack.m_strLocalID = strTemp;
						break;
					}
				case 27:		//m_strSubOprationType
					{
						bRet = m_adoRcrd.GetCollect(m_strReadParams[iIndex],strTemp);
						strTemp.Trim();
						submitPack.m_strSubOprationType = strTemp;
						break;
					}
				case 28:		//m_strSubPort
					{
						bRet = m_adoRcrd.GetCollect(m_strReadParams[iIndex],strTemp);
						strTemp.Trim();
						submitPack.m_strSubPort = strTemp;
						break;
					}
				case 29:		//m_strUserId
					{
						bRet = m_adoRcrd.GetCollect(m_strReadParams[iIndex],strTemp);
						strTemp.Trim();
						submitPack.m_strUserId = strTemp;
						break;
					}
				case 30:		//m_strReverse1
					{
						bRet = m_adoRcrd.GetCollect(m_strReadParams[iIndex],strTemp);
						strTemp.Trim();
						submitPack.m_strMsgStatus = strTemp;
						break;
					}
				case 31:		//m_strReverse2
					{
						bRet = m_adoRcrd.GetCollect(m_strReadParams[iIndex],strTemp);
						strTemp.Trim();
						submitPack.m_strRecvStatus = strTemp;
						break;
					}
				case 32:		//m_strMainPort
					{
						bRet = m_adoRcrd.GetCollect(m_strReadParams[iIndex],strTemp);
						strTemp.Trim();
						submitPack.m_strMainPort = strTemp;
						break;
					}
				case 33:		//m_strFailReason
					{
						bRet = m_adoRcrd.GetCollect(m_strReadParams[iIndex],strTemp);
						strTemp.Trim();
						submitPack.m_strFailReason = strTemp;
						break;
					}
				case 34:		//m_strReverse5
					{
						bRet = m_adoRcrd.GetCollect(m_strReadParams[iIndex],strTemp);
						submitPack.m_strReverse5 = strTemp;
						break;
					}
				case 35:		//m_strReverse6
					{
						bRet = m_adoRcrd.GetCollect(m_strReadParams[iIndex],strTemp);
						strTemp.Trim();
						submitPack.m_strReverse6 = strTemp;
						break;
					}
				default:
					{
						break;
					}
				}
				if (bRet==FALSE)
				{
					AddToDispListShow(m_strReadParams[iIndex]+"�ֶη���ʧ��,����ȷ�����ֶ���,���ָ����ݿ��в�ѯ��һ��");
					break;
				}
			}
			if (bRet==TRUE)
			{
				submitPack.m_timeSendTime = COleDateTime::GetCurrentTime();
				submitPack.m_iTryTimes = 3;
				submitPack.m_strSpMsgId = "88888888";
				submitPack.m_strPtMsgId = "88888888";
				//��ӵ�������
				sendListLock.Lock();
				sendBufList.Add(submitPack);
				sendListLock.Unlock();			
				strTemp.Format("����������Ϣ:%d|%s|%s|%d",submitPack.m_iLocalID,submitPack.m_strUserNumber,submitPack.m_strMsg,submitPack.m_iSendType);
				WriteLog(strTemp,2);
			}
			m_adoRcrd.MoveNext();//����һ����¼��
		}
		m_adoRcrd.Close();
	}
	catch (...)
	{
		WriteLog("CDataBaseCtrl::ReadWaitMessage�쳣",3);
		AddToDispListShow(m_adoConnect.GetLastErrorText());
		m_adoRcrd.Close();	
	}
	return 1;
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
int CDataBaseCtrl::AddMoMessage(DeliverPack &mo)
{
	try
	{		
		if (!ConnectDataBase(m_adoConnect,m_adoRcrd,m_idbType))
		{
			AddToDispListShow("�����߳��������ݿ�ʧ��["+m_adoConnect.GetLastErrorText()+"]");
			return -3;			
		}
		if (m_iOptFlag == 3) //�洢����
		{
			try
			{
				CAdoCommand adoCmd;
				adoCmd.SetConnection(&m_adoConnect);
				adoCmd.SetCommandText(m_strAddMoSql);				
				int iCount = InitMoStoreParams(adoCmd,mo);				
				int iRet = -1;
				if (m_idbType == 2)		//oracle���ݿ�
				{
					CString strStore = "{call "+m_strAddMoSql+"("; //)}
					for (int i=0;i<iCount;i++)
					{
						strStore+="?,";
					}
					strStore = strStore.Left(strStore.GetLength()-1);
					strStore += ")}";
					adoCmd.SetCommandText(strStore);
					adoCmd.Execute(iRet,adCmdStoredProc|adCmdUnspecified);
				}
				else
				{
					adoCmd.Execute(iRet);
				}
				if (iRet == -1)
				{
					AddToDispListShow(m_adoConnect.GetLastErrorText());
					return -1;
				}
				return 0;
			}
			catch (...)
			{
				//AddToDispListShow(m_adoConnect.GetLastErrorText());
			}
			return -1;
		}
		CString strSQL = "";
		CString strArray[7] = {""};
		GetMoString(mo,strArray);
		strSQL.Format(m_strAddMoSql,
			strArray[0],
			strArray[1],
			strArray[2],
			strArray[3],
			strArray[4],
			strArray[5],
			strArray[6]);
		if (!m_adoRcrd.Open(strSQL,adCmdText))
		{
			AddToDispListShow(m_adoConnect.GetLastErrorText());
			AddToDispListShow("����������Ϣʧ��");
			m_adoRcrd.Close();
			Sleep(2000);
			return -2;
		}
		m_adoRcrd.Close();
		return 0;
	}
	catch(...)
	{
		WriteLog("CDataBaseCtrl::AddMoMessage�쳣",3);
		AddToDispListShow(m_adoConnect.GetLastErrorText());
		m_adoRcrd.Close();
	}
	return -1;
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
int CDataBaseCtrl::UpdateSendResult(ReportPack &report)
{
	try
	{		
		if (!ConnectDataBase(m_adoConnect,m_adoRcrd,m_idbType))
		{
			AddToDispListShow("�����߳��������ݿ�ʧ��["+m_adoConnect.GetLastErrorText()+"]");
			return -3;			
		}
		if ( m_iOptFlag == 3 )  //�洢����
		{
			try
			{
				CAdoCommand adoCmd;
				adoCmd.SetConnection(&m_adoConnect);
				adoCmd.SetCommandText(m_strUpdateSql);
				int iCount = InitReportStoreParams(adoCmd,report);				
				int iRet = -1;
				if (m_idbType == 2)		//oracle���ݿ�
				{
					CString strStore = "{call "+m_strUpdateSql+"("; //)}
					for (int i=0;i<iCount;i++)
					{
						strStore+="?,";
					}
					strStore = strStore.Left(strStore.GetLength()-1);
					strStore += ")}";
					adoCmd.SetCommandText(strStore);
					adoCmd.Execute(iRet,adCmdStoredProc|adCmdUnspecified);
				}
				else
				{
					adoCmd.Execute(iRet);
				}
				if ( iRet == -1 )
				{
					AddToDispListShow(m_adoConnect.GetLastErrorText());
					return -2;
				}
				return 0;
			}
			catch (...)
			{
			}
			return -2;
		}
		/************************************************************************/
		CString strSQL = "";
		CString strArray[7] = {""};
		GetReportString(report,strArray);
		strSQL.Format(m_strUpdateSql,
							strArray[0],
							strArray[1],
							strArray[2],
							strArray[3],
							strArray[4],
							strArray[5],
							strArray[6]);
		if (!m_adoRcrd.Open(strSQL,adCmdText))
		{
			AddToDispListShow(m_adoConnect.GetLastErrorText());
			AddToDispListShow("���·��ͽ��ʧ��");
			m_adoRcrd.Close();
			Sleep(2000);			
			return -2;
		}
		m_adoRcrd.Close();
		return 0;
	}
	catch(...)
	{
		WriteLog("UpdateSendResult�쳣",3);
		AddToDispListShow(m_adoConnect.GetLastErrorText());
		m_adoRcrd.Close();
	}
	return -1;
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
//���淢�ͽ�����Դ�������д���
int CDataBaseCtrl::AddSendResult(KfSubmitPack &mt,int iFlag)
{
	if ( iFlag < 0 || iFlag > 1)
	{
		AfxMessageBox("����Խ��");
		exit(0);
	}
	try
	{	
		if (!ConnectDataBase(m_adoConnect,m_adoRcrd,m_idbType))
		{
			AddToDispListShow("д��ʷ���߳��������ݿ�ʧ��["+m_adoConnect.GetLastErrorText()+"]");
			return -3;
		}
		
		if ( m_iOptFlag == 3 )	//�洢����
		{
			try
			{
				CAdoCommand adoCmd;
				adoCmd.SetConnection(&m_adoConnect);
				adoCmd.SetCommandText(m_strAddResultStoreName);			
				int iCount = InitAddResultStoreParams(adoCmd,mt,iFlag);
				int iRet = -1;
				if (m_idbType == 2)		//oracle���ݿ�
				{
					CString strStore = "{call "+m_strAddResultStoreName+"("; //)}
					for (int i=0;i<iCount;i++)
					{
						strStore+="?,";
					}
					strStore = strStore.Left(strStore.GetLength()-1);
					strStore += ")}";
					adoCmd.SetCommandText(strStore);
					adoCmd.Execute(iRet,adCmdStoredProc|adCmdUnspecified);
				}
				else
				{
					adoCmd.Execute(iRet);
				}

				if (iRet == -1)
				{
					AddToDispListShow(m_adoConnect.GetLastErrorText());
					return -1;
				}				
				return 0;
			}
			catch (...)
			{				
			}
			return -1;
		}
		mt.m_strMsgStatus = m_strMsgStatus[iFlag];
		mt.m_strRecvStatus = m_strRecvStatus[iFlag];
		CString strSQL = "";
		CString strSQL1 = "";
		CString strSQL2 = "";
		strSQL1 = m_strSucAddResultSql;
		// 		if ( iFlag == 0 ) //�ɹ�
		// 		{			
		// 			strSQL2 = m_strSucCleanSql;
		// 		}
		// 		else
		// 		{
		// 			strSQL2 = m_strFailCleanSql;
		// 		}
		strSQL2 = m_strSucCleanSql;
		//strSQL = MakeSqlString(mt,strSQL1,3,iFlag);//���淢�ͽ��
		strSQL = MakeSqlString(mt,strSQL1,3,0);	//���淢�ͽ��
		//AfxMessageBox(strSQL);
		if ( !m_adoRcrd.Open(strSQL,adCmdText) )
		{
			AddToDispListShow(m_adoConnect.GetLastErrorText());
			AddToDispListShow("��ӷ��ͽ��ʧ��");
		}
		m_adoRcrd.Close();
		//strSQL = MakeSqlString(mt,strSQL2,2,iFlag);	//���������Ϣ
		strSQL = MakeSqlString(mt,strSQL2,2,0);	//���������Ϣ
		//AfxMessageBox(strSQL);
		if ( !m_adoRcrd.Open(strSQL,adCmdText) )
		{
			AddToDispListShow(m_adoConnect.GetLastErrorText());
			AddToDispListShow("���������Ϣʧ��");
			m_adoRcrd.Close();
			Sleep(2000);			
			return -2;
		}
		m_adoRcrd.Close();
		return 0;
	}
	catch( ... )
	{
		WriteLog("CDataBaseCtrl::AddSendResult�쳣",3);
		AddToDispListShow(m_adoConnect.GetLastErrorText());
		m_adoRcrd.Close();
	}
	return -1;
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
//1�Ǵ���ȴ�������Ϣ,2�Ǳ�����
CString CDataBaseCtrl::MakeSqlString(KfSubmitPack &mt,CString strSqlSrc,int iFlag,int iSucFalg)
{
	CString strArry[35] = {""};
	GetMtString(mt,strArry,iFlag,iSucFalg);
	CString strRetSQL = "";
	strRetSQL.Format(strSqlSrc,
						strArry[0],
						strArry[1],
						strArry[2],
						strArry[3],
						strArry[4],
						strArry[5],
						strArry[6],
						strArry[7],
						strArry[8],
						strArry[9],
						strArry[10],
						strArry[11],
						strArry[12],
						strArry[13],
						strArry[14],
						strArry[15],
						strArry[16],
						strArry[17],
						strArry[18],
						strArry[19],
						strArry[20],
						strArry[21],
						strArry[22],
						strArry[23],
						strArry[24],
						strArry[25],
						strArry[26],
						strArry[27],
						strArry[28],
						strArry[29],
						strArry[30],
						strArry[31],
						strArry[32],
						strArry[33],
						strArry[34]);
	return strRetSQL;
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CDataBaseCtrl::GetReportString(ReportPack &report,CString strArry[7])
{
	try
	{
		int i = 0;
		int iIndexFlag = 0;
		for ( i=0;i<7;i++ )
		{
			iIndexFlag = m_iUpdateIndex[i];
			switch ( iIndexFlag )
			{
			case -1:
				{
					return;
				}
			case 1:
				{
					strArry[i] = report.strDate;
					break;
				}
			case 2:
				{
					strArry[i] = report.strTime;
					break;
				}
			case 3:
				{
					strArry[i] = report.strMsgNo;
					break;
				}
			case 4:
				{
					strArry[i] = report.strErrorCode;//strReserve;
					if ( report.strMsgStatus =="0" )	//���ͳɹ�
					{
						strArry[i].Format("��[%s]���ճɹ�;",report.strTime);
					}
					else if ( report.strMsgStatus == "1" )
					{
						strArry[i].Format("��[%s]�����ݻ�;",report.strTime);	//�����ݻ�
					}
					else
					{
						strArry[i].Format("��[%s]����ʧ��[%s];",report.strTime,report.strErrorCode);
					}
					strArry[i].Replace("'","''");
					break;
				}
			case 5:
				{	
					if ( report.strMsgStatus =="0" )	//���ͳɹ�
					{
						strArry[i] = m_strRecvStatus[2];
					}
					else if (  report.strMsgStatus == "1" )
					{
						strArry[i] = m_strRecvStatus[3];	//�����ݻ�				
					}
					else
					{
						strArry[i] = m_strRecvStatus[1];   //����ʧ��					
					}
					break;
				}
			case 6:		//������
				{
					strArry[i] = report.strErrorCode;
					break;
				}
			default:
				{
					return;
				}
			}
		}
	}
	catch (...)
	{
	}	
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
void CDataBaseCtrl::GetMoString(DeliverPack &mo,CString strArry[7])
{
	try
	{
		int i = 0;
		int iIndexFlag = 0;
		for ( i=0;i<7;i++ )
		{
			iIndexFlag = m_iAddMoIndex[i];
			switch ( iIndexFlag )
			{
			case -1:
				{
					return;
				}
			case 1:
				{
					strArry[i] = mo.strDate;
					break;
				}
			case 2:
				{
					strArry[i] = mo.strTime;
					break;
				}
			case 3:
				{
					strArry[i] = mo.strOrgAddr;
					break;
				}
			case 4:
				{
					strArry[i] = mo.strDestAddr;
					break;
				}
			case 5:
				{	
					strArry[i] = mo.strReserve;
					break;
				}
			case 6:
				{
					strArry[i] = mo.strMsg;
					strArry[i].Replace("'","''");
					break;
				}
			default:
				{
					return;
				}
			}
		}
	}
	catch (...)
	{
	}
}

/************************************************************************/
/* 09-07-24 09:42 ����ӽ��������ȴ����͵�SQLͳһ,�����ֳɹ���ʧ��    */
/************************************************************************/
//0,1�ɹ�,2�쳣
int CDataBaseCtrl::GetMtString(KfSubmitPack &mt,CString strArry[35],int iFlag,int iSucFlag)
{
	try
	{
		switch ( iFlag )
		{
		case 2:			//��ȡ
		case 3:			//��ӷ��ͽ��
			{
				int i = 0;
				int iIndexFlag = 0;
				for (i=0;i<35;i++)
				{
					if (iSucFlag == 0) //���ͳɹ�
					{
						if (iFlag == 2)
						{
							iIndexFlag = m_iSucCleanIndex[i];
						}
						else
						{
							iIndexFlag = m_iSucAddResultIndex[i];
						}

					}
					else		//����ʧ��
					{
						if ( iFlag == 2 )
						{
							//iIndexFlag = m_iSucCleanIndex[i];
							iIndexFlag = m_iFailCleanIndex[i];
						}
						else
						{
							//iIndexFlag = m_iSucAddResultIndex[i];
							iIndexFlag = m_iFailAddResultIndex[i];
						}
					}
					switch ( iIndexFlag )
					{
					case -1:
						{
							return 1;
						}
					case 1:			//m_iSendType
						{
							strArry[i].Format("%d",mt.m_iSendType);
							break;
						}
					case 2:			//m_iCount
						{
							strArry[i].Format("%d",mt.m_iCount);
							break;
						}
					case 3:			//m_iCountA
						{
							strArry[i].Format("%d",mt.m_iCountA);
							break;
						}
					case 4:			//m_iPhoneType
						{
							strArry[i].Format("%d",mt.m_iPhoneType);
							break;
						}
					case 5:			//m_iTryTimes
						{
							strArry[i].Format("%d",mt.m_iTryTimes);
							break;
						}
					case 6:			//m_iMsgLen
						{
							strArry[i].Format("%d",mt.m_iMsgLen);
							break;
						}
					case 7:			//m_iPrecedence
						{
							strArry[i].Format("%d",mt.m_iPrecedence);
							break;
						}
					case 8:			//m_iLocalID
						{
							strArry[i].Format("%d",mt.m_iLocalID);
							break;
						}
					case 9:			//m_iReverse1
						{
							strArry[i].Format("%d",mt.m_iReverse1);
							strArry[i].Replace("'","''");
							break;
						}
					case 10:		//m_iReverse2
						{
							strArry[i].Format("%d",mt.m_iReverse2);
							strArry[i].Replace("'","''");
							break;
						}
					case 11:		//m_iReverse3
						{
							strArry[i].Format("%d",mt.m_iReverse3);
							break;
						}
					case 12:		//m_iReverse4
						{
							strArry[i].Format("%d",mt.m_iReverse4);
							break;
						}
					case 13:		//m_iReverse5
						{
							strArry[i].Format("%d",mt.m_iReverse5);
							break;
						}
					case 14:		//m_strSendType
						{
							strArry[i].Format("%s",mt.m_strSendType);
							break;
						}
					case 15:		//m_strUserNumber
						{
							strArry[i].Format("%s",mt.m_strUserNumber);
							break;
						}
					case 16:		//m_strUserNumberA
						{
							strArry[i].Format("%s",mt.m_strUserNumberA);
							break;
						}
					case 17:		//m_strPtMsgId
						{
							strArry[i].Format("%s",mt.m_strPtMsgId);
							break;
						}
					case 18:		//m_strSpMsgId
						{
							strArry[i].Format("%s",mt.m_strSpMsgId);
							break;
						}
					case 19:		//m_strMsg
						{
							strArry[i].Format("%s",mt.m_strMsg);
							strArry[i].Replace("'","''");					
							break;
						}
					case 20:		//m_strCreatTime
						{
							strArry[i].Format("%s",mt.m_strCreatTime);
							break;
						}
					case 21:		//m_strSubTime
						{
							strArry[i].Format("%s",mt.m_strSubTime);
							break;
						}
					case 22:		//m_timeSendTime
						{
							strArry[i].Format("%s",mt.m_timeSendTime.Format("%Y-%m-%d %H:%M:%S"));
							break;
						}
					case 23:		//m_strOperatorID
						{
							strArry[i].Format("%s",mt.m_strOperatorID);
							break;
						}
					case 24:		//m_strServiceID
						{
							strArry[i].Format("%s",mt.m_strServiceID);
							break;
						}
					case 25:		//m_strTaskName
						{
							strArry[i].Format("%s",mt.m_strTaskName);
							break;
						}
					case 26:		//m_strLocalID
						{
							strArry[i].Format("%s",mt.m_strLocalID);
							break;
						}
					case 27:		//m_strSubOprationType
						{
							strArry[i].Format("%s",mt.m_strSubOprationType);
							break;
						}
					case 28:		//m_strSubPort
						{
							strArry[i].Format("%s",mt.m_strSubPort);
							break;
						}
					case 29:		//m_strUserId
						{
							strArry[i].Format("%s",mt.m_strUserId);
							break;
						}
					case 30:		//m_strMsgStatus
						{
							strArry[i].Format("%s",mt.m_strMsgStatus);
							break;
						}
					case 31:		//m_strRecvStatus
						{
							strArry[i].Format("%s",mt.m_strRecvStatus);
							break;
						}
					case 32:		//m_strMainPort
						{
							strArry[i].Format("%s",mt.m_strMainPort);
							break;
						}
					case 33:		//m_strFailReason
						{
							strArry[i].Format("%s",mt.m_strFailReason);
							strArry[i].Replace("'","''");
							break;
						}
					case 34:		//m_strReverse5
						{
							strArry[i].Format("%s",mt.m_strReverse5);
							strArry[i].Replace("'","''");
							break;
						}
					case 35:		//m_strReverse6
						{
							strArry[i].Format("%s",mt.m_strReverse6);
							strArry[i].Replace("'","''");
							break;
						}
					default:
						{
							return 1;
						}
					}				
				}
			}
		default:
			{
				break;
			}
		}
		return 0;
	}
	catch ( ... )
	{
		return 2;
	}	
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
//1�Ƕ�ȡ,2�ǳɹ�����,3��ʧ�ܱ���,4�ɹ�����,5ʧ������,6����,7״̬����,8������(�洢����)
void CDataBaseCtrl::InitArray(CString strSrcIndex,int iFlag)
{
	int i = 0;
	int iPos = 0;
	int iIdx = 0;
	int iIndx1 = 0;
	CString strTemp = "";
	CString strTempLeft = "";
	try
	{
		while ( true )
		{
			strTempLeft.Empty();
			strTemp.Empty();
			iPos = strSrcIndex.Find(',');
			iIdx = strSrcIndex.Find(';');
			if( iPos != -1 && iIdx != -1 )
			{
				strTempLeft = strSrcIndex.Left(iPos);
				strTemp = strSrcIndex.Mid( iPos+1,iIdx - iPos - 1 );
				strTempLeft.Trim();
				strTemp.Trim();
				switch( iFlag )
				{
				case 1:			//��ȡ
					{
						if ( strTemp == "" || strTempLeft == "" )
						{
							break;
						}
						if ( i >= 35 )
						{
							break;
						}
						m_iReadIndex[i] = atoi(strTemp);
						m_strReadParams[i] = strTempLeft;
						i++;
						break;
					}

				case 2:		//�ɹ�����
					{
						if ( strTemp == "" || strTempLeft == "" )
						{
							break;
						}
						if ( i >= 35 )
						{
							break;
						}
						m_iSucAddResultIndex[i] = atoi(strTemp);
						m_strSucAddResultParams[i] = strTempLeft;
						i++;
						break;
					}

				case 3:		//ʧ�ܱ���
					{
						if ( strTemp == "" || strTempLeft == "" )
						{
							break;
						}
						if ( i >= 35 )
						{
							break;
						}
						m_iFailAddResultIndex[i] = atoi(strTemp);
						m_strFailAddResultParams[i] = strTempLeft;
						i++;
						break;
					}

				case 4:		//�ɹ�����
					{
						if ( strTemp == "" || strTempLeft == "" )
						{
							break;
						}
						if ( i >= 35 )
						{
							break;
						}
						m_iSucCleanIndex[i] = atoi(strTemp);
						m_strSucCleanParams[i] = strTempLeft;
						i++;
						break;
					}	
				case 5:		//ʧ������
					{
						if ( strTemp == "" || strTempLeft == "" )
						{
							break;
						}
						if ( i >= 35 )
						{
							break;
						}
						m_iFailCleanIndex[i] = atoi(strTemp);
						m_strFailCleanParams[i] = strTempLeft;
						i++;
						break;
					}
				case 6:		//����
					{
						if ( strTemp == "" || strTempLeft == "" )
						{
							break;
						}
						if ( i >= 7 )
						{
							break;
						}
						m_iAddMoIndex[i] = atoi(strTemp);
						m_strAddmoParams[i] = strTempLeft;
						if ( m_iOptFlag == 3 )		//����Ǵ洢����	
						{
							iIndx1 = strTempLeft.Find('[');
							m_strAddmoParams[i] = strTempLeft.Left(iIndx1);
							strTempLeft = strTempLeft.Mid(iIndx1);
							strTempLeft.Replace("[","");
							strTempLeft.Replace("]","");
							m_iAddmoParamLen[i] = atoi(strTempLeft);
						}
						i++;
						break;
					}	
				case 7:		//״̬����
					{
						if ( strTemp == "" || strTempLeft == "" )
						{
							break;
						}
						if ( i >= 7 )
						{
							break;
						}
						m_iUpdateIndex[i] = atoi(strTemp);
						m_strUpdateParams[i] = strTempLeft;
						if ( m_iOptFlag == 3 )
						{
							iIndx1 = strTempLeft.Find('[');
							m_strUpdateParams[i] = strTempLeft.Left(iIndx1);
							strTempLeft = strTempLeft.Mid(iIndx1);
							strTempLeft.Replace("[","");
							strTempLeft.Replace("]","");
							m_iUpdateParamLen[i] = atoi(strTempLeft);
						}
						i++;
						break;
					}
				case 8:		//����
					{
						if ( strTemp == "" || strTempLeft == "" )
						{
							break;
						}
						if ( i >= 35 )
						{
							break;
						}
						m_iAddResultIndex[i] = atoi(strTemp);
						m_strAddResultParams[i] = strTempLeft;
						if ( m_iOptFlag == 3 )
						{
							iIndx1 = strTempLeft.Find('[');
							m_strAddResultParams[i] = strTempLeft.Left(iIndx1);
							strTempLeft = strTempLeft.Mid(iIndx1);
							strTempLeft.Replace("[","");
							strTempLeft.Replace("]","");
							m_iAddResultParamLen[i] = atoi(strTempLeft);
						}
						i++;
						break;
					}
				default:
					{
						break;
					}
				}				
				strSrcIndex = strSrcIndex.Mid(iIdx+1);
			}
			else
			{
				break;
			}
		}
	}
	catch ( ... )
	{
	}	
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
int CDataBaseCtrl::ConvertSendMessage(KfSubmitPack &mt)
{
	try
	{
		int iRet = 0;
		CString strDestAddr = mt.m_strUserNumber;
		int iSendType = mt.m_iSendType;
		//ǰ�ڴ���,�ŵ�һ��MFC��̬����,���ش�����Ϣ
		/************************************************************************/		

		//���ݷ�����������
		//////////////////////////////////////////////////////////////////////100Ϊ�����Ϣ
		strDestAddr.Replace(m_strSplitString," ");
		//ת������ָ���
		iRet = ConvertPhoneList(strDestAddr);
		if (iRet == -1)			//�����ļ�δ�ҵ�
		{
			mt.m_strFailReason="�����ļ�δ�ҵ�";
			mt.m_strUserNumber = "13888888888";
			mt.m_iCount = -1;
			return 0;
		}
		else if (iRet == -99)		//�쳣
		{
			mt.m_strFailReason = "�����쳣";
			mt.m_iCount = -1;
			return 0;
		}
		else if (strDestAddr == "")	//����Ϊ��,����
		{
			mt.m_strFailReason = "����Ϊ��,����";
			mt.m_iCount = -1;
			return 0;
		}
		mt.m_strMsg.Trim();
		if (mt.m_strMsg == "")
		{
			mt.m_strFailReason = "����Ϣ,������";
			mt.m_iCount = iRet;
			return 0;
		}
		mt.m_strUserNumber = strDestAddr;
		mt.m_iCount = iRet;
		return 1;
	}
	catch (...)
	{
		mt.m_strFailReason = "�����쳣";
		WriteLog("ConvertSendMessage�쳣",3);
	}
	return 0;
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
//ת�������ַ���,����-1��ʾ�����ļ�δ�ҵ�
int CDataBaseCtrl::ConvertPhoneList( CString &strDestAddr )
{
	try
	{
		//����Ǻ����ļ�·��������ļ����������
		int iCount = 1;
		strDestAddr.Trim();
		if ( strDestAddr.Find(".txt") != -1 ) //��ʾ�Ǻ����ļ�
		{
			//���ļ�
			CString fileName = m_strPhoneFilePath + strDestAddr;
			if ( _access(fileName,0) != 0 )
			{
				//�ļ�������
				strDestAddr = "";
				WriteLog("�����ļ�������["+fileName+"]",1);
				AddToDispListShow("�����ļ�������["+fileName+"]");
				return -1;
			}
			iCount = 0;
			strDestAddr = "";
			CStdioFile   file;
			CString   cstr = "";
			CFileException   fe;
			if(file.Open(fileName,CFile::modeRead,&fe))   
			{ 
				file.SeekToBegin();
				while (file.ReadString(cstr))
				{
					cstr.Trim();
					if (cstr == "")
					{
						continue;
					}
					strDestAddr += cstr + " "; 
					iCount++;
				}
				file.Close();
			}
			strDestAddr.Trim();
			return iCount;
		}
		else		//�����ַ���
		{
			int i = 0;
			int istrLen = strDestAddr.GetLength();
			for ( i = 0;i<istrLen;i++)
			{
				if ( strDestAddr.GetAt( i ) == ' ')
				{
					iCount++;
				}
			}
			return iCount;
		}
	}	
	catch (...)
	{
		WriteLog("CDataBaseCtrl::ConvertPhoneList�쳣",3);
		return -99;
	}
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
//ת�������ַ���,����-1��ʾ�����ļ�δ�ҵ�
bool CDataBaseCtrl::IsSingleSend( CString strDestAddr )
{
	try
	{
		//����Ǻ����ļ�·��������ļ����������
		strDestAddr.Trim();
		if ( strDestAddr.Find(".txt") != -1 ) //��ʾ�Ǻ����ļ�
		{
			//���ļ�
			return false;	//�����ļ�Ⱥ��
		}
		else		//�����ַ���
		{
			int iCount = 0;
			int i = 0;
			strDestAddr += " ";
			int istrLen = strDestAddr.GetLength();
			for (i = 0;i<istrLen;i++)
			{
				if (strDestAddr.GetAt(i) == ' ')
				{
					iCount++;
					if (iCount>1)
					{
						return false;
					}
				}
			}
			return true;
		}
	}	
	catch (...)
	{
		WriteLog("IsSingleSend�쳣",3);
		return false;
	}
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
//����-1��ʾ�ļ�δ�ҵ�,
int CDataBaseCtrl::GetSendArray(CArray <CString, CString> &strSendList,CString strSrc)
{
	try
	{
		//���ļ�
		CString fileName = m_strPhoneFilePath + strSrc;
		if ( _access(fileName,0) != 0 )
		{
			strSendList.RemoveAll();
			AddToDispListShow("���ͨ�����ļ�������");
			return 0;
		}
		int 	iCount = 0;
		CStdioFile   file;
		CString   cstr = "";
		CFileException   fe;
		if(file.Open(fileName,CFile::modeRead,&fe))   
		{ 
			file.SeekToBegin();
			while (file.ReadString(cstr))
			{
				strSendList.Add(cstr);
				iCount++;
			}
			file.Close();
		}
		return iCount;
	}	
	catch (...)
	{
		WriteLog("CDataBaseCtrl::ConvertPhoneList�쳣",3);
		return -99;
	}
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
int CDataBaseCtrl::InitMoStoreParams(CAdoCommand &adoCmd,DeliverPack &mo)
{
	try
	{
		int i = 0;
		int iIndexFlag = 0;
		int iRet = 0;
		for ( i=0;i<7;i++)
		{
			iIndexFlag = m_iAddMoIndex[i];
			if ( iIndexFlag == -1 )
			{
				return iRet;
			}
			CAdoParameter adoParam;
			adoParam.SetDirection(adParamInput);
			adoParam.SetName(m_strAddmoParams[i]);
			adoParam.SetSize(m_iAddmoParamLen[i]);
			switch (iIndexFlag)
			{
			case 1:
				{
					break;
				}
			case 2:		//����ʱ��
				{
					adoParam.SetType(adVarChar);   
					adoParam.SetValue(mo.strTime);
					adoCmd.Append(adoParam.GetParameter());
					iRet++;
					break;
				}
			case 3:		//�ֻ���
				{
					adoParam.SetType(adVarChar);
					adoParam.SetValue(mo.strOrgAddr);
					adoCmd.Append(adoParam.GetParameter());
					iRet++;
					break;
				}
			case 4:		//ͨ����
				{
					adoParam.SetType(adVarChar);
					adoParam.SetValue(mo.strDestAddr);
					adoCmd.Append(adoParam.GetParameter());
					iRet++;
					break;
				}
			case 5:		//�����ֶ�
				{	
					break;
				}
			case 6:				//��Ϣ����
				{
					mo.strMsg.Replace("'","''");
					adoParam.SetType(adVarChar);
					adoParam.SetValue(mo.strMsg);
					adoCmd.Append(adoParam.GetParameter());
					iRet++;
					break;
				}
			default:
				{
					return iRet;
				}
			}
		}
		return iRet;
	}
	catch (...)
	{
	}
	return 0;
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
int CDataBaseCtrl::InitReportStoreParams(CAdoCommand &adoCmd,ReportPack &report)
{
	try
	{
		int i = 0;
		int iIndexFlag = 0;
		int iRet = 0;
		CString strTemp = "";
		for ( i=0;i<7;i++)
		{
			iIndexFlag = m_iUpdateIndex[i];
			if ( iIndexFlag == -1 )
			{
				return iRet;
			}
			CAdoParameter adoParam;
			adoParam.SetDirection(adParamInput);
			adoParam.SetName(m_strUpdateParams[i]);
			adoParam.SetSize(m_iUpdateParamLen[i]);
			switch (iIndexFlag)
			{
			case 1:
				{
					break;
				}
			case 2:			//����ʱ��
				{
					adoParam.SetType(adVarChar);
					adoParam.SetValue(report.strTime);
					adoCmd.Append(adoParam.GetParameter());
					iRet++;
					break;
				}
			case 3:			//��Ϣ����
				{
					adoParam.SetType(adVarChar);
					adoParam.SetValue(report.strMsgNo);
					adoCmd.Append(adoParam.GetParameter());
					iRet++;
					break;
				}
			case 4:			//���Ի��Ľ��ղ�����Ϣ
				{
					if ( report.strMsgStatus =="0" )	//���ͳɹ�
					{
						strTemp.Format("��[%s]���ճɹ�;",report.strTime);
					}
					else if ( report.strMsgStatus == "1" )
					{
						strTemp.Format("��[%s]�����ݻ�;",report.strTime);	//�����ݻ�
					}
					else
					{
						strTemp.Format("��[%s]����ʧ��[%s];",report.strTime,report.strErrorCode);
					}
					strTemp.Replace("'","''");
					adoParam.SetType(adVarChar);
					adoParam.SetValue(strTemp);
					adoCmd.Append(adoParam.GetParameter());
					iRet++;
					break;
				}
			case 5:			//�˴�ӳ�����״̬
				{
					adoParam.SetType(adInteger);
					if (report.strMsgStatus =="0")	//���ͳɹ�
					{
						adoParam.SetValue(m_iRecvStatus[2]);
					}
					else if (report.strMsgStatus == "1")	//�ݻ�
					{
						adoParam.SetValue(m_iRecvStatus[3]);
					}
					else		//ʧ��
					{
						adoParam.SetValue(m_iRecvStatus[1]);
					}								
					adoCmd.Append(adoParam.GetParameter());
					iRet++;
					break;
				}
			case 6:			//������
				{
					adoParam.SetType(adVarChar);
					adoParam.SetValue(report.strErrorCode);
					adoCmd.Append(adoParam.GetParameter());
					iRet++;
					break;
				}
			default:
				{
					return iRet;
				}
			}
		}
		return iRet;
	}
	catch (...)
	{
	}
	return 0;
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
int CDataBaseCtrl::InitAddResultStoreParams(CAdoCommand &adoCmd,KfSubmitPack &mt,int iFlag)
{
	try
	{
		int i = 0;
		int iIndexFlag = 0;
		CString strTemp = "";
		int iRet = 0;
		for ( i=0;i<35;i++)
		{
			iIndexFlag = m_iAddResultIndex[i];
			if (iIndexFlag == -1)
			{
				return iRet;
			}
			CAdoParameter adoParam;
			adoParam.SetDirection(adParamInput);
			adoParam.SetName(m_strAddResultParams[i]);
			adoParam.SetSize(m_iAddResultParamLen[i]);
			switch (iIndexFlag)
			{
			case 1:			//m_iSendType
				{
					adoParam.SetType(adInteger);
					adoParam.SetValue(mt.m_iSendType);
					adoCmd.Append(adoParam.GetParameter());
					iRet++;
					break;
				}
			case 2:			//m_iCount
				{
					adoParam.SetType(adInteger);
					adoParam.SetValue(mt.m_iCount);
					adoCmd.Append(adoParam.GetParameter());
					iRet++;
					break;
				}
			case 3:			//m_iCountA
				{
					adoParam.SetType(adInteger);
					adoParam.SetValue(mt.m_iCountA);
					adoCmd.Append(adoParam.GetParameter());
					iRet++;
					break;
				}
			case 4:			//m_iPhoneType
				{
					adoParam.SetType(adInteger);
					adoParam.SetValue(mt.m_iPhoneType);
					adoCmd.Append(adoParam.GetParameter());
					iRet++;
					break;
				}
			case 5:			//m_iTryTimes
				{
					adoParam.SetType(adInteger);
					adoParam.SetValue(mt.m_iTryTimes);
					adoCmd.Append(adoParam.GetParameter());
					iRet++;
					break;
				}
			case 6:			//m_iMsgLen
				{
					adoParam.SetType(adInteger);
					adoParam.SetValue(mt.m_iMsgLen);
					adoCmd.Append(adoParam.GetParameter());
					iRet++;
					break;
				}
			case 7:			//m_iPrecedence
				{
					adoParam.SetType(adInteger);
					adoParam.SetValue(mt.m_iPrecedence);
					adoCmd.Append(adoParam.GetParameter());
					iRet++;
					break;
				}
			case 8:			//m_iLocalID
				{
					adoParam.SetType(adInteger);
					adoParam.SetValue(mt.m_iLocalID);
					adoCmd.Append(adoParam.GetParameter());
					iRet++;
					break;
				}
			case 9:			//m_iReverse1
				{
					adoParam.SetType(adInteger);
					adoParam.SetValue(mt.m_iReverse1);
					adoCmd.Append(adoParam.GetParameter());
					iRet++;
					break;
				}
			case 10:		//m_iReverse2
				{
					adoParam.SetType(adInteger);
					adoParam.SetValue(mt.m_iReverse2);
					adoCmd.Append(adoParam.GetParameter());
					iRet++;
					break;
				}
			case 11:		//m_iReverse3
				{
					adoParam.SetType(adInteger);
					adoParam.SetValue(mt.m_iReverse3);
					adoCmd.Append(adoParam.GetParameter());
					iRet++;
					break;
				}
			case 12:		//m_iReverse4
				{
					adoParam.SetType(adInteger);
					adoParam.SetValue(mt.m_iReverse4);
					adoCmd.Append(adoParam.GetParameter());
					iRet++;
					break;
				}
			case 13:		//m_iReverse5
				{
					adoParam.SetType(adInteger);
					adoParam.SetValue(mt.m_iReverse5);
					adoCmd.Append(adoParam.GetParameter());
					iRet++;
					break;
				}
			case 14:		//m_strSendType
				{
					adoParam.SetType(adVarChar);
					adoParam.SetValue(mt.m_strSendType);
					adoCmd.Append(adoParam.GetParameter());
					iRet++;
					break;
				}
			case 15:		//m_strUserNumber
				{
					adoParam.SetType(adVarChar);
					adoParam.SetValue(mt.m_strUserNumber.Left(20));
					adoCmd.Append(adoParam.GetParameter());
					iRet++;
					break;
				}
			case 16:		//m_strUserNumberA
				{
					adoParam.SetType(adVarChar);
					adoParam.SetValue(mt.m_strUserNumberA);
					adoCmd.Append(adoParam.GetParameter());
					iRet++;
					break;
				}
			case 17:		//m_strPtMsgId
				{
					adoParam.SetType(adVarChar);
					adoParam.SetValue(mt.m_strPtMsgId);
					adoCmd.Append(adoParam.GetParameter());
					iRet++;
					break;
				}
			case 18:		//m_strSpMsgId
				{
					adoParam.SetType(adVarChar);
					adoParam.SetValue(mt.m_strSpMsgId);
					adoCmd.Append(adoParam.GetParameter());
					iRet++;
					break;
				}
			case 19:		//m_strMsg
				{
					mt.m_strMsg.Replace("'","''");
					adoParam.SetType(adVarChar);
					adoParam.SetValue(mt.m_strMsg);
					adoCmd.Append(adoParam.GetParameter());
					iRet++;
					break;
				}
			case 20:		//m_strCreatTime
				{
					adoParam.SetType(adVarChar);
					adoParam.SetValue(mt.m_strCreatTime);
					adoCmd.Append(adoParam.GetParameter());
					iRet++;
					break;
				}
			case 21:		//m_strSubTime
				{
					adoParam.SetType(adVarChar);
					adoParam.SetValue(mt.m_strSubTime);
					adoCmd.Append(adoParam.GetParameter());
					iRet++;
					break;
				}
			case 22:		//m_timeSendTime
				{
					adoParam.SetType(adVarChar);
					adoParam.SetValue(mt.m_timeSendTime.Format("%Y-%m-%d %H:%M:%S"));
					adoCmd.Append(adoParam.GetParameter());
					iRet++;
					break;
				}
			case 23:		//m_strOperatorID
				{
					adoParam.SetType(adVarChar);
					adoParam.SetValue(mt.m_strOperatorID);
					adoCmd.Append(adoParam.GetParameter());
					iRet++;
					break;
				}
			case 24:		//m_strServiceID
				{
					adoParam.SetType(adVarChar);
					adoParam.SetValue(mt.m_strServiceID);
					adoCmd.Append(adoParam.GetParameter());
					iRet++;
					break;
				}
			case 25:		//m_strTaskName
				{
					adoParam.SetType(adVarChar);
					adoParam.SetValue(mt.m_strTaskName);
					adoCmd.Append(adoParam.GetParameter());
					iRet++;
					break;
				}
			case 26:		//m_strLocalID
				{
					adoParam.SetType(adVarChar);
					adoParam.SetValue(mt.m_strLocalID);
					adoCmd.Append(adoParam.GetParameter());
					iRet++;
					break;
				}
			case 27:		//m_strSubOprationType
				{
					adoParam.SetType(adVarChar);
					adoParam.SetValue(mt.m_strSubOprationType);
					adoCmd.Append(adoParam.GetParameter());
					iRet++;
					break;
				}
			case 28:		//m_strSubPort
				{
					adoParam.SetType(adVarChar);
					adoParam.SetValue(mt.m_strSubPort);
					adoCmd.Append(adoParam.GetParameter());
					iRet++;
					break;
				}
			case 29:		//m_strUserId
				{
					adoParam.SetType(adVarChar);
					adoParam.SetValue(mt.m_strUserId);
					adoCmd.Append(adoParam.GetParameter());
					iRet++;
					break;
				}
			case 30:		//m_strMsgStatus
				{
					adoParam.SetType(adInteger);
					adoParam.SetValue(m_iMsgStatus[iFlag]);
					adoCmd.Append(adoParam.GetParameter());
					iRet++;
					break;
				}
			case 31:		//m_strRecvStatus
				{
					adoParam.SetType(adInteger);
					adoParam.SetValue(m_iRecvStatus[iFlag]);
					adoCmd.Append(adoParam.GetParameter());
					iRet++;
					break;
				}
			case 32:		//m_strMainPort
				{
					adoParam.SetType(adVarChar);
					adoParam.SetValue(mt.m_strMainPort);
					adoCmd.Append(adoParam.GetParameter());
					iRet++;
					break;
				}
			case 33:		//m_strFailReason
				{
					mt.m_strFailReason.Replace("'","''");
					adoParam.SetType(adVarChar);
					adoParam.SetValue(mt.m_strFailReason);
					adoCmd.Append(adoParam.GetParameter());
					iRet++;
					break;
				}
			case 34:		//m_strReverse5
				{
					mt.m_strReverse5.Replace("'","''");
					adoParam.SetType(adVarChar);
					adoParam.SetValue(mt.m_strReverse5);
					adoCmd.Append(adoParam.GetParameter());
					iRet++;
					break;
				}
			case 35:		//m_strReverse6
				{
					mt.m_strReverse6.Replace("'","''");
					adoParam.SetType(adVarChar);
					adoParam.SetValue(mt.m_strReverse6);
					adoCmd.Append(adoParam.GetParameter());
					iRet++;
					break;
				}
			default:
				{
					return iRet;
				}
			}
		}
		return iRet;
	}
	catch (...)
	{
	}
	return 0;
}
/************************************************************************/
/*iFlag��ʶ�������������ݿ�,����������Դ                                  */
/************************************************************************/
BOOL CDataBaseCtrl::ConnectDataBase(CAdoConnection &adoConn,CAdoRecordSet &adoRst,int iFlag)
{
	try
	{
		if (adoConn.IsOpen())		//���δ�����Ϸ�����
		{
			return TRUE;
		}
		//iFlag = 1;			//����ֻ����SQL SERVER
		BOOL bRet = FALSE;
		switch (iFlag)
		{
		case 1:		//����SQLSERVER 
			{
				bRet = adoConn.ConnectSQLServer(m_dbServer,m_dbName,m_dbUser,m_dbPwd);
				break;
			}
		case 2:		//����ORACLE 
			{
				bRet = adoConn.ConnectToOracle(m_dbServer,m_dbName,m_dbUser,m_dbPwd);
				break;
			}
		case 3:		//����MYSQL 
			{
				if (m_idbPort <= 0)
				{
					bRet = adoConn.ConnectMysql(m_dbServer,m_dbName,m_dbUser,m_dbPwd,m_strCharSet);
				}
				else
				{
					bRet = adoConn.ConnectMysql(m_dbServer,m_dbName,m_dbUser,m_dbPwd,m_strCharSet,m_idbPort);
				}
				break;
			}
		case 4:		//����ACCESS 
			{
				bRet = adoConn.ConnectAccess(m_dbServer,m_dbPwd);
				break;
			}
		default:
			{
				break;
			}
		}
		if (bRet == TRUE)
		{
			adoRst.SetAdoConnection(&adoConn);
			adoRst.SetCursorLocation(adUseClient);
		}
		return bRet;
	}
	catch (...)
	{
	}
	return FALSE;
}
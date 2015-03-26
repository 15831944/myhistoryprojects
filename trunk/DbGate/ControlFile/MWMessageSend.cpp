#include "stdafx.h"
#include ".\mwmessagesend.h"

/************************************************************************/
/*                                                                      */
/************************************************************************/
CMWMessageSend::CMWMessageSend(void)
{
	m_sock=-1;//-1 δ��ʼ�� ��ʼ�� 0 ��½ >0
	m_handle = NULL;
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
CMWMessageSend::~CMWMessageSend(void)
{
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
//����������Ϣ
int CMWMessageSend::SendMtMessage(LPCTSTR sMsg, LPCTSTR sDestAddrs,int byUserNum,LPCTSTR sSubPort,unsigned char* byMsgID,CString &strError)
{
	char pszSN[50]={0};
	int iret = -99;
	const char *pszMobis;
	const char *pszMsg;
	char pszSubPort[30]="*";
	WriteLogEx("����������Ϣ������ʼ",1);
	try
	{
		if( m_sock < 0 )
		{
			strError = "�ӿ���δ��ʼ��";
			return -999;	//δ��ʼ��
		}
		if (strlen(sSubPort) > 0)
		{
			strncpy(pszSubPort,sSubPort,30);
		}
		pszMobis = sDestAddrs;
		pszMsg = sMsg;
		WriteLogEx("��̬�ⷢ����Ϣ��ʼ",1);
		try
		{
			iret = MongateCsSPSendSms(m_sock,pszMobis,pszMsg,byUserNum,pszSubPort,pszSN);	
			if(iret > 0)
			{
				strError = "���ͳɹ�";
				CString strNewID = "";
				strNewID.Format("%s",pszSN);
				if (strNewID == "")
				{
					strcpy((char*)byMsgID,pszSN);
				}
				else if (strNewID.Left(1) == "-")
				{
					unsigned char szOldNo[50] = {0};
					char szNewNo[50] = {0};
					__int64 iiii =_atoi64(pszSN);
					TranMsgIdI64ToChar(szOldNo,iiii);
					TranMsgId(szOldNo,szNewNo);
					//strNewID.Format("%s",szNewNo);
					strcpy((char*)byMsgID,szNewNo);
				}
				else
				{
					strcpy((char*)byMsgID,pszSN);
				}
				iret =  0;		//�ɹ�
			}
			else
			{
				strError = GetSendMsgErrorStr(iret);
			}
		}
		catch(...)
		{
			strError = "��̬�ⷢ����Ϣ�쳣";
			iret = -99;
		}		
		WriteLogEx("��̬�ⷢ����Ϣ����"+strError,1);
	}
	catch(...)
	{
		WriteLog("SendMtMessage�쳣",3);
		strError = "������Ϣ�쳣����";
		iret = -99;		//�쳣
	}
	WriteLogEx("����������Ϣ��������,"+strError,1);
	return iret;
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
//�ͷŽӿ�
BOOL CMWMessageSend::ExitInterface()
{	
	try
	{
		if(m_sock < 0)
		{
			return TRUE;
		}
		MongateDisconnect(m_sock);
		if (m_handle)
		{
			FreeLibrary(m_handle);
			m_handle = NULL;
		}
		m_sock = -1;
		return TRUE;
	}	
	catch (...)
	{
		WriteLog("CMWMessageSend::ExitInterface�쳣",3);
	}
	return FALSE;
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
//���ò���
void CMWMessageSend::SetParams(CString strServerIP,int iPort,CString strUserID,CString strPwd)
{
	memset(m_sServerIP,0,sizeof(m_sServerIP));
	m_iport = 8018;
	memset(m_sUserID,0,sizeof(m_sUserID));
	memset(m_sPassWord,0,sizeof(m_sPassWord));
	strncpy(m_sServerIP,strServerIP,20);
	m_iport = iPort;
	strncpy(m_sUserID,strUserID,33);
	strncpy(m_sPassWord,strPwd,33);
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
//��ʼ���ӿ�
BOOL CMWMessageSend::InitInterface()
{
#ifdef USE_DLL___
	WriteLogEx("��ʼ����̬�⺯����ʼ",1);
	if(!m_handle)
	{
		TCHAR szBuff[_MAX_PATH];
		VERIFY(::GetModuleFileName(AfxGetApp()->m_hInstance, szBuff, _MAX_PATH));
		CString strAppPath="";
		strAppPath.Format("%s",szBuff);
		int pos=strAppPath.ReverseFind('\\');
		strAppPath=strAppPath.Left(pos+1);

		m_handle = LoadLibrary(strAppPath + MWGATE_DLL_NAME);
		if(!m_handle)
		{
			return FALSE;
		}
		MongateConnect = (FnMongateConnect)GetProcAddress(m_handle, "MongateConnect");
		MongateCsSPSendSms  = (FnMongateCsSPSendSms)GetProcAddress(m_handle, "MongateCsSPSendSms");	
		MongateCsGetSmsEx  = (FnMongateCsGetSmsEx)GetProcAddress(m_handle, "MongateCsGetSmsEx");
		MongateDisconnect  = (FnMongateDisconnect)GetProcAddress(m_handle, "MongateDisconnect");
		MongateQueryBalance = (FnMongateQueryBalance)GetProcAddress(m_handle,"MongateQueryBalance");
		MongateQueryUsed	= (FnMongateQueryUsed)GetProcAddress(m_handle,"MongateQueryUsed");
		MGhexChrTosrcStr = (FnMGhexChrTosrcStr)GetProcAddress(m_handle,"MGhexChrTosrcStr");
		MongateCsGetStatusReportEx = (FnMongateCsGetStatusReportEx)GetProcAddress(m_handle,"MongateCsGetStatusReportEx");
		MongateChangePwd = (FnMongateChangePwd)GetProcAddress(m_handle,"MongateChangePwd");
		MongateTestConn = (FnMongateTestConn)GetProcAddress(m_handle,"MongateTestConn");
		SetCmdType = (FnSetCmdType)GetProcAddress(m_handle,"SetServerType");
		
	}
	m_sock = 0;	//��ʼ���ӿں�SOCKET��Ϊ0�Ա�ʶ
	WriteLogEx("��ʼ����̬�⺯������",1);
#endif
// 	m_handle_smeidll = LoadLibrary(strAppPath + MWGATE_DLL_NAME_SMEIDLL);
// 	if(m_handle_smeidll == 0x0)	
// 	{
// 		return FALSE;
// 	}
// 	IFInitInterface = (_IFInitInterface)GetProcAddress(m_handle_smeidll,"IFInitInterface");
// 	IFExitInterface = (_IFExitInterface)GetProcAddress(m_handle_smeidll,"IFExitInterface");
// 	Login_R = (_Login_R)GetProcAddress(m_handle_smeidll,"Login_R");
// 	SendSms = (_SendSms)GetProcAddress(m_handle_smeidll,"SendSms");
// 	RecvSms = (_RecvSms)GetProcAddress(m_handle_smeidll,"RecvSms");
// 	m_sock = 0;
	return TRUE;
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
//���Ӳ���¼������
int CMWMessageSend::ConnectAndLogin(CString &strError)
{
	WriteLogEx("���Ӳ���¼������������ʼ",1);
	int iret = -99;
	try
	{
		if(m_sock < 0)
		{
			strError = "�ӿ���δ��ʼ��";
			return -999;	//δ��ʼ��
		}
		if (CString(m_sServerIP) == "" || CString(m_sUserID) == "")
		{
			strError = "�û���Ϣ��δ����";
			return -2;
		}
		iret = MongateConnect(m_sServerIP, m_iport,m_sUserID, m_sPassWord);
		m_sock = iret;
		if(iret < 1) //��½ʧ��
		{
			strError = GetLoginErrorStr(iret);
			m_sock = 0;
		}
		else
		{
			strError = "��¼�ɹ�";
		}		
	}
	catch (...)
	{
		WriteLog("CMWMessageSend::ConnectAndLogin�쳣",3);
		strError = "��¼�쳣";
		iret = -99;		
	}
	WriteLogEx("���Ӳ���¼��������������,"+strError,1);
	return iret;
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
//���ݴ���ŷ��ش�����Ϣ
CString CMWMessageSend::GetServerErrorStr(int iErrorCode)
{
	CString strRet = "";
	switch ( iErrorCode )
	{
	case -10001:
		{
			strRet = "�û���½���ɹ�";
			break;
		}
	case -10002:
		{
			strRet = "�ύ��ʽ����ȷ";
			break;
		}
	case -10003:
		{
			strRet = "�û�����";
			break;
		}
	case -10004:
		{
			strRet = "�ֻ����벻��ȷ";
			break;
		}
	case -10005:
		{
			strRet = "�Ʒ��û��ʺŴ���";
			break;
		}
	case -10006:
		{
			strRet = "�Ʒ��û������";
			break;
		}
	case -10007:
		{
			strRet = "�˺��Ѿ���ͣ��";
			break;
		}
	case -10008:
		{
			strRet = "�˺����Ͳ�֧�ָù���";
			break;
		}
	case -10009:
		{
			strRet = "��������";
			break;
		}
	case -10010:
		{
			strRet = "��ҵ���벻��ȷ";
			break;
		}
	case -10011:
		{
			strRet = "��Ϣ���ݳ���";
			break;
		}
	case -10012:
		{
			strRet = "���ܷ�����ͨ����";
			break;
		}
	case -10013:
		{
			strRet = "����ԱȨ�޲���";
			break;
		}
	case -10014:
		{
			strRet = "���ʴ��벻��ȷ";
			break;
		}
	case -10015:
		{
			strRet = "��������æ";
			break;
		}
	case -10016:
		{
			strRet = "��ҵȨ�޲���";
			break;
		}
	case -10017:
		{
			strRet = "��ʱ��β�������";
			break;
		}
	case -10018:
		{
			strRet = "�������û����������";
			break;
		}
	case -10019:
		{
			strRet = "�ֻ��б��������";
			break;
		}
	case -10021:
		{
			strRet = "û�п�ͣ��Ȩ��";
			break;
		}
	case -10022:
		{
			strRet = "û��ת���û����͵�Ȩ��";
			break;
		}
	case -10023:
		{
			strRet = "û���޸��û����������̵�Ȩ��";
			break;
		}
	case -10024:
		{
			strRet = "�������û����������";
			break;
		}
	case -10025:
		{
			strRet = "����Ա��½�����������";
			break;
		}
	case -10026:
		{
			strRet = "����Ա����ֵ���û�������";
			break;
		}
	case -10027:
		{
			strRet = "����Աû�г�ֵ������Ȩ��";
			break;
		}
	case -10028:
		{
			strRet = "���û�û��ת�����ܳ�ֵ";
			break;
		}
	case -10029:
		{
			strRet = "���û�û��Ȩ�޴Ӵ�ͨ��������Ϣ";
			break;
		}
	case -10030:
		{
			strRet = "���ܷ����ƶ�����";
			break;
		}
	case -10031:
		{
			strRet = "�ֻ�����(��)�Ƿ�";
			break;
		}
	case -10032:
		{
			strRet = "�û�ʹ�õķ��ʴ������";
			break;
		}
	case -10033:
		{
			strRet = "�Ƿ��ؼ���";
			break;
		}
	case -10035:
		{
			strRet = "���ܷ��Ͷ���ֻ�";
			break;
		}
	case -10036:
		{
			strRet = "���ܷ��͵����ֻ�";
			break;
		}
	case -10037:
		{
			strRet = "������ͬ����Ϣ����������ͣ";
			break;
		}
	case -10038:
		{
			strRet = "������Ȩ�޲���";
			break;
		}
	case -10040:
		{
			strRet = "�ظ�ע��";
			break;
		}
	case -10041:
		{
			strRet = "��ȡ����MO��Ϣ̫��";
			break;
		}
	case -10042:
		{
			strRet = "��ȡ״̬����̫��";
			break;
		}
	case -10043:
		{
			strRet = "���������ʺŵ�����½��";
			break;
		}
	case -10044:
		{
			strRet = "���ʺ�û����չ�Ӷ˿�Ȩ��";
			break;
		}
	case -10045:
		{
			strRet = "��Ϣ�����С�ھɱ�Ŵ���";
			break;
		}
	case -10046:
		{
			strRet = "��Ϣ����Ŵ��ھɱ�Ŵ���";
			break;
		}
	case -10047:
		{
			strRet = "�޸��������";
			break;
		}
	case -10048:
		{
			strRet = "���Ż��������";
			break;
		}
	case -10051:
		{
			strRet = "ֻ�ܷ����ƶ�����";
			break;
		}
	case -10052:
		{
			strRet = "ֻ�ܷ�����ͨ����";
			break;
		}
	case -10053:
		{
			strRet = "ֻ�ܷ��͵���С��ͨ����";
			break;
		}
	case -10054:
		{
			strRet = "��ʱ��Ų�����";
			break;
		}
	case -10099:
		{
			strRet = "��������";
			break;
		}
	default:
		{
			strRet = "";
			break;
		}
	}
	return strRet;
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
//���ص�¼����
CString CMWMessageSend::GetLoginErrorStr( int iErrorCode )
{
	CString strError = "";
	switch ( iErrorCode )
	{
	case -1:
		{
			strError = "�����а�����NULLֵ";
			break;
		}
	case -2:
		{
			strError = "�Ƿ��ĵ�½�ʺŻ�����";
			break;
		}
	case -199:
		{
			strError = "��ȡԶ��������Ϣʧ��";
			break;
		}			
	case -198:
		{
			strError = "����socketʧ��";
			break;
		}
	case -197:
		{
			strError = "����ʧ��";
			break;
		}
	case -196:
		{
			strError = "�ȴ�ƽ̨��������ȷ����Ϣ��ʱ";
			break;
		}
	case -195:
		{
			strError = "�ȴ�����������ʧ��";
			break;
		}
	default:
		{
			strError = "�������ӵ�¼����";
			break;
		}
	}
	if ( iErrorCode < -10000 )	//���������ش���
	{
		strError = GetServerErrorStr( iErrorCode );
	}
	return strError;
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
//���ط�����Ϣ������Ϣ
CString CMWMessageSend::GetSendMsgErrorStr( int iErrorCode )
{
	CString strError = "";
	switch ( iErrorCode )
	{
	case -1:
		{
			strError = "�����а�����NULLֵ";
			break;
		}
	case -2:
		{
			strError = "�绰�����������1000";
			break;
		}
	case -10:
		{
			strError = "���뻺��ռ�ʧ��";
			break;
		}			
	case -11:
		{
			strError = "�绰�������з������ַ�";
			break;
		}
	case -12:
		{
			strError = "���쳣�绰����";
			break;
		}
	case -13:
		{
			strError = "�绰���������ʵ�ʸ��������";
			break;
		}
	case -14:
		{
			strError = "ʵ�ʺ����������1000";
			break;
		}
	case -101:
		{
			strError = "������Ϣ�ȴ���ʱ";
			break;
		}
	case -102:
		{
			strError = "���ͻ������Ϣʧ��";
			break;
		}
	case -103:
		{
			strError = "������Ϣ��ʱ";
			break;
		}
	case -104:
		{
			strError = "������Ϣʧ��";
			break;
		}
	case -105:
		{
			strError = "������Ϣʱ������Ϣʧ��[-105],���������";
		}
	case -198:
		{
			strError = "��������ʧ��";
			break;
		}
	case -200:
		{
			strError = "��������";
			break;
		}
	default:
		{
			strError = "";
			break;
		}
	}
	if ( iErrorCode < -10000 )	//���������ش���
	{
		strError = GetServerErrorStr( iErrorCode);
	}
	return strError;
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
//���ض�ȡ���л�״̬���������Ϣ
CString CMWMessageSend::GetMoReportErrorMsg(int iErrorCode)
{
	CString strError = "";
	switch ( iErrorCode )
	{
	case -1:
		{
			strError = "�����а�����NULLֵ";
			break;
		}
	case -101:
		{
			strError = "������Ϣ�ȴ���ʱ";
			break;
		}
	case -102:
		{
			strError = "������Ϣʧ��";
			break;
		}
	case -103:
		{
			strError = "������Ϣ��ʱ";
			break;
		}
	case -104:
		{
			strError = "������Ϣʧ��";
			break;
		}
	case -105:
		{
			strError = "������Ϣʧ��[-105],���������";			
			break;
		}
	case -198:
		{
			strError = "��������ʧ��";
			break;
		}
	case -200:
		{
			strError = "��������";
			break;
		}
	case -300:
		{
			strError = "��ȡʱ��������";
			break;
		}
	default:
		{
			strError = "";
			break;
		}
	}
	if ( iErrorCode < -10000 )	//���������ش���
	{
		strError = GetServerErrorStr( iErrorCode);
	}
	return strError;
}


/************************************************************************/
/*                                                                      */
/************************************************************************/
//����������Ϣ
int CMWMessageSend::GetMoMessges(DeliverPackList &recvPackList,CString &strError )
{
	WriteLogEx("��ȡ������Ϣ������ʼ",1);
	int iRet = -99;
	CString strTemp = "";
	try
	{
		if( m_sock < 0 )
		{
			strError = "�ӿ���δ��ʼ��";
			return -999;	//δ��ʼ��
		}
		char szMsg[30000]={0};	//
		char tokens[]=";";	//������֮��ķָ���
		char *szDeliverMsg = NULL; 		
		//��״̬����
		WriteLogEx("��̬�����������Ϣ������ʼ",1);
		iRet = MongateCsGetSmsEx(m_sock,szMsg);
		strTemp.Format("��̬�����������Ϣ��������,����ֵ:%d",iRet);
		WriteLogEx(strTemp,1);
		if ( iRet == 0 )
		{
			strError = "��������Ϣ";
			//return 0; //��ʾ��������Ϣ
		}
		else if (iRet > 0)	//��������Ϣ,���ص���������Ϣ����
		{
			//���д���
			szDeliverMsg=strtok(szMsg,tokens);
			while (szDeliverMsg!=NULL)
			{
				//AddRePortPack(reportPackList,szReport);				
				strTemp.Format("%s",szDeliverMsg);
				AddDeleverPack(recvPackList,strTemp);
				szDeliverMsg = strtok(NULL,tokens);
			}
			strError.Format("��ȡ��%d��������Ϣ",iRet);
		}
		else if ( iRet < 0 )	//���ճ���,���ش�����Ϣ
		{
			strError = "��������ʱ����:"+GetMoReportErrorMsg(iRet);
			WriteLogEx(strError,1);
		}		
	}
	catch (...)
	{
		WriteLog("CMWMessageSend::GetMoMessges�쳣",3);
		strError = "��ȡ������Ϣ�쳣";
		iRet = -99;		
	}
	WriteLogEx("��ȡ������Ϣ��������",1);
	return iRet;
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
//����״̬����
int CMWMessageSend::GetMsgReports(ReportPackList &reportPackList,CString &strError )
{
	WriteLogEx("��ȡ״̬���溯����ʼ",1);
	int iRet = -99;
	CString strTemp = "";
	try
	{
		if( m_sock < 0 )
		{
			strError = "�ӿ���δ��ʼ��";
			return -999;	//δ��ʼ��
		}
		char szMsg[30000]={0};	//
		char tokens[]=";";	//������֮��ķָ���
		char *szReport=NULL; 
		
		//��״̬����
		WriteLogEx("��̬�����״̬���溯����ʼ",1);
		iRet = MongateCsGetStatusReportEx(m_sock,szMsg);
		strTemp.Format("��̬�����״̬���溯������,����ֵ:%d",iRet);
		WriteLogEx(strTemp,1);
		if (iRet == 0)
		{
			strError = "��״̬����";
			//return 0; //��ʾ��״̬����
		}
		else if (iRet > 0)	//��״̬����,���ص���״̬��������
		{
			//���д���
			
			szReport=strtok(szMsg,tokens);
			while (szReport!=NULL)
			{
				strTemp.Format("%s",szReport);
				AddRePortPack(reportPackList,strTemp);
				szReport = strtok(NULL,tokens);
			}
			strError.Format("��ȡ��%d��״̬����",iRet);
		}
		else if (iRet < 0)	//���ճ���,���ش�����Ϣ
		{
			strError = "����״̬����ʱ����:"+GetMoReportErrorMsg(iRet);
			WriteLogEx(strError,1);
		}		
	}
	catch (...)
	{
		WriteLog("CMWMessageSend::GetMsgReports�쳣",3);
		strError = "��ȡ״̬�����쳣";
		iRet = -99;		
	}
	WriteLogEx("��ȡ״̬���溯������",1);
	return iRet;
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
int CMWMessageSend::AddRePortPack(ReportPackList &reportPackList, CString strMsg)
{
	/* 2009-05-20,17:06:05,05201706054587,*,0,DILEVRD */
	CString strMessage = strMsg+",";
	CString szstrMsg = "";
	int iCount=0;
	ReportPack tempPack;
	try
	{
		//szstrMsg=strtok(strTempMsg,tokens);
		int index = 0;
		index = strMessage.Find(',');
		while (index != -1)
		{
			szstrMsg = strMessage.Left(index);
			strMessage = strMessage.Mid(index + 1);
            iCount++;
			switch ( iCount )
			{
			case 1:		//����
				{
					tempPack.strDate = szstrMsg;
					break;
				}
			case 2:		//ʱ��
				{
					tempPack.strTime = szstrMsg;
					break;
				}
			case 3:		//���
				{
					if(szstrMsg.GetLength() == 0)
					{
						tempPack.strMsgNo = "";
						break;
					}
					if (szstrMsg.Left(1) == "-")
					{
						unsigned char szOldNo[50] = {0};
						char szNewNo[50] = {0};
						__int64 iiii =_atoi64(szstrMsg);
						TranMsgIdI64ToChar(szOldNo,iiii);
						TranMsgId(szOldNo,szNewNo);
						tempPack.strMsgNo.Format("%s",szNewNo);
					}
					else
					{
						//strcpy((char*)byMsgID,pszSN);
						tempPack.strMsgNo = szstrMsg;
					}
					break;
				}
			case 4:		//����
				{
					tempPack.strReserve = szstrMsg;
					break;
				}
			case 5:		//״̬
				{
					tempPack.strMsgStatus = szstrMsg;
					break;
				}
			case 6:		//������
				{
					tempPack.strErrorCode = szstrMsg;
					break;
				}
			default:
				{
					break;
				}
			}
			index = strMessage.Find(',');
		}
		reportPackList.Add(tempPack);
		return 0;
	}
	catch (...)
	{
		WriteLog("CMWMessageSend::AddRePortPack�쳣",3);
		return -1;
	}	
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
int CMWMessageSend::AddDeleverPack(DeliverPackList &recvPackList,CString strMsg)
{
	/* 2009-05-20,17:06:05,05201706054587,*,0,DILEVRD */
	CString strMessage = strMsg+",";
	CString szstrMsg = "";
	int iCount=0;
	DeliverPack tempPack;
	char szSrc[1000] = {0};
	char szDst[1000] = {0};
	try
	{
		//szstrMsg=strtok(strTempMsg,tokens);
		int index = 0;
		index = strMessage.Find(',');
		while (index != -1)
		{
			szstrMsg = strMessage.Left(index);
			strMessage = strMessage.Mid(index + 1);
			iCount++;
			switch (iCount)
			{
			case 1:		//����
				{
					tempPack.strDate = szstrMsg;
					break;
				}
			case 2:		//ʱ��
				{
					tempPack.strTime = szstrMsg;
					break;
				}
			case 3:		//�����ֻ�����
				{
					tempPack.strOrgAddr = szstrMsg;
					break;
				}
			case 4:		//���պ���
				{
					tempPack.strDestAddr = szstrMsg;
					break;
				}
			case 5:		//����
				{
					tempPack.strReserve = szstrMsg;
					break;
				}
			case 6:
				{
					memset(szSrc,0,1000);
					memset(szDst,0,1000);
					strncpy(szSrc,szstrMsg,1000);
					MGhexChrTosrcStr(szSrc,szDst);
					szstrMsg.Format("%s",szDst);
					tempPack.strMsg.Format("%s",szDst);
					break;
				}
			default:
				{
					break;
				}
			}
			index = strMessage.Find(',');
		}
		recvPackList.Add(tempPack);
		return 0;
	}
	catch (...)
	{
		WriteLog("CMWMessageSend::AddDeleverPack�쳣",3);
		return -1;
	}
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
//�޸�����
int CMWMessageSend::ChangePassword(CString strNewPwd,CString &strError)
{
	if (strNewPwd.GetLength() < 1)
	{
		strError = "�����벻��Ϊ��";
		return -203;
	}
	if( m_sock < 0 )
	{
		strError = "�ӿ���δ��ʼ��";
		return -999;	//δ��ʼ��
	}
	int iRet = -99;
	char szNewPassWord[32+1] = {0};	//�û�����
	try
	{
		strncpy(szNewPassWord,strNewPwd,33);
		//iRet = MongateChangePwd(m_sock,m_sUserID,m_sPassWord,szNewPassWord);
		if ( iRet > 0)	//�޸ĳɹ�
		{
			strError.Format("�޸�����ɹ�%s",strNewPwd);
		}
		else 
		{
			int iErrorCode = iRet;
			switch ( iErrorCode )
			{
			case -1:
				{
					strError = "�����а�����NULLֵ";
					break;
				}
			case -101:
				{
					strError = "������Ϣ�ȴ���ʱ";
					break;
				}
			case -102:
				{
					strError = "������Ϣʧ��";
					break;
				}
			case -103:
				{
					strError = "������Ϣ��ʱ";
					break;
				}
			case -104:
				{
					strError = "������Ϣʧ��";
					break;
				}
			case -105:
				{
					strError = "�޸�����ʱ������Ϣʧ��[-105],���������";
					break;
				}
			case -198:
				{
					strError = "��������ʧ��";
					break;
				}
			case -200:
				{
					strError = "��������";
					break;
				}
			default:
				{
					strError = "";
					break;
				}
			}
			if ( iErrorCode < -10000 )	//���������ش���
			{
				strError = GetServerErrorStr( iErrorCode );
			}
		}
		return iRet;
	}
	catch (...)
	{
		WriteLog("CMWMessageSend::ChangePassword�쳣",3);
		strError = "�޸������쳣";
		return -99;
	}
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
//��ѯ������ʣ������
int CMWMessageSend::GetFee(int &iUsed,int &iLeft,CString &strError)
{
	WriteLogEx("��ѯ������ʼ",1);
	int iRet1 = -99;
	int iRet2 = -99;
	CString strShow = "";
	if( m_sock < 0 )
	{
		strError = "�ӿ���δ��ʼ��";
		WriteLogEx("��ѯ��������,"+strError,1);
		return -999;	//δ��ʼ��
	}
	try
	{
		WriteLogEx("��̬���ѯ������ʼ",1);
		iRet1 = MongateQueryBalance(m_sock);		
		strShow.Format("%d",iRet1);
		WriteLogEx("��̬���ѯ��������,����ֵ:"+strShow,1);
		if (iRet1 >= 0)		//ֻ�е�����ѯ�ɹ�,��ȥ��ѯ�Ѿ�ʹ�õ�����
		{
			iRet2 = MongateQueryUsed(m_sock);
		}
		if (iRet1 >= 0)	//��ѯ�ɹ�
		{
			iLeft = iRet1;
			iUsed = iRet2;
			strError.Format("����%d��,ʣ��%d��",iUsed,iLeft);
		}
		else	//��ѯʧ��
		{
			strError ="��ѯ������:";
			if (iRet1 < -10000)
			{
				strError +=GetServerErrorStr(iRet1);				
			}
			else
			{
				switch (iRet1)
				{
				case -1:
					{
						strError += "�����а�����NULLֵ";
						break;
					}
				case -101:
					{
						strError += "������Ϣ�ȴ���ʱ";
						break;
					}
				case -102:
					{
						strError += "������Ϣʧ��";
						break;
					}
				case -103:
					{
						strError += "������Ϣ��ʱ";
						break;
					}
				case -104:
					{
						strError += "������Ϣʧ��";
						break;
					}
				case -105:
					{
						strError = "������������Ϣʧ��[-105],���������";
						break;
					}
				case -198:
					{
						strError += "��������ʧ��";
						break;
					}
				case -200:
					{
						strError += "��������";
						break;
					}
				default:
					{
						strError += "��������";
						break;
					}
				}				
			}			
		}			
	}
	catch (...)
	{
		WriteLog("CMWMessageSend::GetFee�쳣",3);
		strError = "��ѯ������ʣ�������쳣";
		iRet1 =  -99;
	}
	WriteLogEx("��ѯ��������,"+strError,1);
	return iRet1;
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
int CMWMessageSend::KeepOnline(CString &strError)
{
	WriteLogEx("ά�����Ӻ�����ʼ",1);
	int iRet = -99;
	if( m_sock < 0 )
	{
		strError = "�ӿ���δ��ʼ��";
		WriteLogEx("ά�����Ӻ�������,"+strError,1);
		return -999;	//δ��ʼ��
	}
	try
	{
		iRet = MongateTestConn(m_sock);
		if (iRet > 0)
		{
			strError = "��������";
		}
		else
		{
			switch (iRet)
			{
			case -1:
				{
					strError = "�����а�����NULLֵ";
					break;
				}
			case -101:
				{
					strError = "������Ϣ�ȴ���ʱ";
					break;
				}
			case -102:
				{
					strError = "������Ϣʧ��";
					break;
				}
			case -103:
				{
					strError = "������Ϣ��ʱ";
					break;
				}
			case -104:
				{
					strError = "������Ϣʧ��";
					break;
				}
			case -105:
				{
					strError = "ά�����Ӻ���������Ϣʧ��[-105],���������";
					break;
				}
			case -198:
				{
					strError = "��������ʧ��";
					break;
				}
			case -200:
				{
					strError = "�������ӳ���";
					break;
				}
			case -300:
				{
					strError = "��ȡʱ��������";
					break;
				}
			default:
				{
					strError = GetServerErrorStr(iRet);
					break;
				}
			}	
		}
	}
	catch (...)
	{
		WriteLog("CMWMessageSend::KeepOnline�쳣",3);
		strError = "���������쳣";
		iRet = -99;
	}
	WriteLogEx("ά�����Ӻ�������,"+strError,1);
	return iRet;
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
void CMWMessageSend::Disconnect()
{
	if (g_iServerType==0)
	{
		if (m_sock<=0)
		{
			return;
		}
	}	
	WriteLogEx("�Ͽ����Ӻ�����ʼ",1);
	try
	{
		MongateDisconnect(m_sock);
		m_sock = 0;		
	}
	catch (...)
	{
		WriteLog("CMWMessageSend::Disconnect�쳣",3);
	}
	WriteLogEx("�Ͽ����Ӻ�������",1);
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CMWMessageSend::SetCmdTypeEx(int iCmdType,int iRptFlag)
{
#ifndef USE_DLL___
	SetCmdType(iCmdType);
#else
	SetCmdType(iCmdType,iRptFlag);
#endif
}


/************************************************************************/
/*                                                                      */
/************************************************************************/
int CMWMessageSend::TranMsgId(unsigned char *pSrc, char *MsgId)
{
	unsigned int nMon	= 0;
	unsigned int nDay	= 0;
	unsigned int nHour	= 0;
	unsigned int nMin	= 0;
	unsigned int nSec	= 0;
	unsigned int nGate	= 0;
	unsigned int nNo	= 0;
	unsigned char *p	= NULL;

	//��תΪ64bit��������λ
	__int64 j = 0;
	p = pSrc;
	j |= (__int64)(*(p))	<< 56;
	j |= (__int64)(*(++p))	<< 48;
	j |= (__int64)(*(++p))	<< 40;
	j |= (__int64)(*(++p))	<< 32;
	j |= (__int64)(*(++p))	<< 24;
	j |= (__int64)(*(++p))	<< 16;
	j |= (__int64)(*(++p))	<< 8;
	j |= (__int64)(*(++p));

	nMon  = (unsigned int)(j >> 60) & 0x0f;            //4  bit
	nDay  = (unsigned int)(j >> 55) & 0x1f;           //5  bit
	nHour = (unsigned int)(j >> 50) & 0x1f;           //5  bit
	nMin  = (unsigned int)(j >> 44) & 0x3f;           //6  bit
	nSec  = (unsigned int)(j >> 38) & 0x3f;           //6  bit
	nGate = (unsigned int)(j >> 20) & 0x03ffff;       //18 bit,���ֵ9999
	nNo   = (unsigned int)(j)		& 0x0fffff;       //20 bit,���ֵ999999
	sprintf(MsgId, "%02d%02d%02d%02d%02d%06d%06d", nMon, nDay, nHour, nMin, nSec, nGate, nNo);
	return 0;
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CMWMessageSend::TranMsgIdI64ToChar(unsigned char* MsgId, __int64 IMsgId)
{
	/*
	__int64 j = IMsgId;
	MsgId[0] = (unsigned char)((j >> 56) & 0xff);
	MsgId[1] = (unsigned char)((j >> 48) & 0xff);
	MsgId[2] = (unsigned char)((j >> 40) & 0xff);
	MsgId[3] = (unsigned char)((j >> 32) & 0xff);
	MsgId[4] = (unsigned char)((j >> 24) & 0xff);
	MsgId[5] = (unsigned char)((j >> 16) & 0xff);
	MsgId[6] = (unsigned char)((j >> 8 ) & 0xff);
	MsgId[7] = (unsigned char)((j) & 0xff);
	return;
	*/
	__int64 j = IMsgId;
	MsgId[0] = (unsigned char)((j >> 56) & 0xff);
	MsgId[1] = (unsigned char)((j >> 48) & 0xff);
	MsgId[2] = (unsigned char)((j >> 40) & 0xff);
	MsgId[3] = (unsigned char)((j >> 32) & 0xff);
	MsgId[4] = (unsigned char)((j >> 24) & 0xff);
	MsgId[5] = (unsigned char)((j >> 16) & 0xff);
	MsgId[6] = (unsigned char)((j >>  8) & 0xff);
	MsgId[7] = (unsigned char)(j & 0xff);
	return;
}
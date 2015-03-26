#include "stdafx.h"
#include ".\tcpclient.h"
#include "dataType.h"
CTcpClient::CTcpClient(void)
{
	if (!AfxSocketInit())
	{
		AfxMessageBox("��ʼ��SOCKETʧ��");
		exit(0);
	}
	memset(m_Os,0,sizeof(m_Os));
	strcpy(m_Os,"WIN2K");
	memset(m_sVersion,0,sizeof(m_sVersion));
	strcpy(m_sVersion,"2005");
	m_iCmdType = 25;
	m_iOvertime = 90;
}

CTcpClient::~CTcpClient(void)
{
}
/************************************************************************/
/* ������������ݵļ�⺯��[�Ƿ�ɷ�,�Ƿ������ݿɶ�                        */
/************************************************************************/
int CTcpClient::IsCanSendRecv(SOCKET fd, int sec, int flags)//flags==READ ���ȴ�
{
	fd_set rwset;
	struct timeval timeout;
	int iret;
	char buffer[10]={0};
	FD_ZERO(&rwset);
	FD_SET(fd, &rwset);
	timeout.tv_sec = sec;
	timeout.tv_usec = 0;  
	if (flags == RECV)		//0Ϊ��
	{
		iret=(select(0, &rwset, NULL, NULL, &timeout));
		if (iret == -1)
		{
			return iret;
		}
		if (!FD_ISSET(fd, &rwset ))
		{
			return -1;
		}
		else
		{
			return 1;
		}
	}
	else if (flags == SEND)   //1Ϊд
	{ 
		iret=(select(0, NULL, &rwset, NULL, &timeout));
		if (iret ==-1)
		{
			return iret;
		}
		if (!FD_ISSET(fd, &rwset ))
		{
			return -1;
		}
		else
		{
			return 1;
		}
	}
	else
	{
		iret=-1; 
	}
	return iret;
}

/************************************************************************/
/* ��¼�������ĺ���                                                      */
/************************************************************************/
int CTcpClient::MongateConnect(const char* pszIP, int iPort, 
							 const char* pszAccount, const char* pszPwd)
{
	char buf[100] ={0},reg[100]={0};
	int cbRecv = 0;
	int cbSend = 0;
	struct hostent* pHostInfo = NULL;
	struct sockaddr_in svrAddr;
	SOCKET sock = 0;
	CString strTemp = "";
	try
	{
		if (pszIP == NULL || pszAccount == NULL || pszPwd == NULL)
		{
			return -1;//��������У�������NULLֵ
		}
		if (strlen(pszAccount) > 64 || strlen(pszPwd) > 64)
		{
			return -2;//�Ƿ����ʺŻ�����
		}
		pHostInfo = gethostbyname(pszIP);//�����������ֺ͵�ַ��Ϣ��hostent�ṹָ��
		if (pHostInfo == NULL)
		{
			pHostInfo = gethostbyaddr(pszIP, strlen(pszIP), AF_INET);
			if (pHostInfo == NULL)
			{
				return -199;//��ȡԶ��������Ϣʧ��,ԭ��:IP����ʧ�ܻ���Ч��IP,�����粻ͨ
			}
		}
		//����Զ������������
		svrAddr.sin_family = AF_INET;
		memcpy(&svrAddr.sin_addr, pHostInfo->h_addr_list[0], pHostInfo->h_length);
		svrAddr.sin_port = htons((unsigned short)iPort);

		sock = socket(AF_INET, SOCK_STREAM, 0);
		if (sock == -1)
		{
			return -198;//socket����ʧ��  
		}
		if (connect(sock, (struct sockaddr*)&svrAddr, sizeof(svrAddr)) == -1)
		{
			MongateDisconnect(sock);
			return -197;//connectʧ��
		}	
		if (IsCanSendRecv(sock, m_iOvertime, RECV) == 1)
		{
			cbRecv = recv(sock, buf, sizeof(buf), 0);
			if (cbRecv < 1)
			{
				MongateDisconnect(sock);
				return -195;
			}
		}
		else
		{
			MongateDisconnect(sock);
			return -196;//connect�ɹ�,�ȴ�ƽ̨��������ȷ����Ϣ��ʱ 
		}	  
		if (!(buf[0] == 'O' && buf[1] == 'K'))	//�ȴ�����������ʧ��
		{
			MongateDisconnect(sock);
			return -195;
		}
		memset(buf,0,sizeof(buf));
		sprintf(buf,"ZZ%06ld,20,*,*,*,%d,%s,%s,%s,%s",strlen(pszAccount)+strlen(pszPwd)+33,
			m_iCmdType,m_sVersion,m_Os,pszAccount,pszPwd);//��½ָ��
		buf[8] = ','; 
		if (IsCanSendRecv(sock, m_iOvertime, SEND) ==1)
		{
			cbSend = send(sock, buf, strlen(buf), 0);
			if (cbSend < 1)
			{
				MongateDisconnect(sock);
				return -102;//unexpected error
			}
		}
		else
		{
			MongateDisconnect(sock);
			return -101;
		}
		strTemp.Format("%s",buf);
		WriteLogEx(strTemp,5);
		memset(buf, 0, sizeof(buf));
		if (IsCanSendRecv(sock, m_iOvertime, RECV) == 1)
		{
			cbRecv = recv(sock, buf, sizeof(buf),0);			
			if (cbRecv<1)
			{
				MongateDisconnect(sock);
				return -104;
			}
			buf[cbRecv] = 0;
			strTemp.Format("%s",buf);
			WriteLogEx(strTemp,5);
		}    
		else
		{
			MongateDisconnect(sock);
			return -103; //����ƽ̨���ص�½��֤��Ϣ��ʱ
		} 
		if (buf[0] == 'E' && buf[1] == 'R')
		{
			MongateDisconnect(sock);
			return -10000 - (atoi(buf + 3)); //����ƽ̨������Ϣ
		}
		else if (buf[0] == 'O' && buf[1] == 'K')
		{
			return sock;
		}	
		else
		{
			MongateDisconnect(sock);
			return -200;//unexpected error
		}
	}
	catch (...)
	{
		MongateDisconnect(sock);
	}
	return -200;
}

/************************************************************************/
/* ����״̬���溯��                                                      */
/************************************************************************/
int CTcpClient::MongateCsGetStatusReportEx(SOCKET sock, char *strMsg)
{
	int cbRecv = 0;
	int cbSend = 0;
	int count = 0;
	int len = 0;
	int iret = 0;
	int msgTotalLen = 0;
	int msgLen = 0;
	char recvbuf[10000]={0};
	char tempbuf[1001]={0};
	char buf[255]= {0};	
	bool bFlag=true;
	CString strTemp = "";
	if (sock<=0) 
	{
		return -198;
	}	
	try
	{
		memset(strMsg,0,sizeof(strMsg));
		//����������������Ի�ȡ״̬����
		if (IsCanSendRecv(sock, m_iOvertime, SEND) == 1)
		{
			cbSend = send(sock, "ZZ000019,33,1,*,*,*",19, 0);
			if(cbSend<1)
			{
				return -102;
			}
		}		
		else
		{
			return -101;
		}	
		WriteLogEx("״̬����:ZZ000019,33,1,*,*,*",5);
		while(bFlag)
		{
			memset(tempbuf,0,sizeof(tempbuf));
			if (IsCanSendRecv(sock, m_iOvertime, RECV) == 1)
			{
				cbRecv = recv(sock, tempbuf, 1000, 0);				
				if (cbRecv<1) 
				{
					iret=-104;
					bFlag=false;
					break;
				}
				tempbuf[cbRecv]=0;
				strTemp.Format("%s",tempbuf);
				WriteLogEx(strTemp,5);
				msgTotalLen+=cbRecv;
				if (msgTotalLen<10000)//�����ַ���
				{
					strcat(recvbuf,tempbuf);
				}
				else
				{
					iret = -105;	//����ʧ��
					bFlag=false;
					break;
				}				
				if (tempbuf[0] == 'E' && tempbuf[1] == 'R')
				{
					iret= -10000 - (atoi(tempbuf + 3));//����ƽ̨������Ϣ
					bFlag=false;
					break;
				}
				else if (tempbuf[0] == 'O' && tempbuf[1] == 'K')
				{
					iret = 0;//û����Ϣ
					bFlag=false;
					break;
				}
				else if(tempbuf[0] == 'Z' && tempbuf[1] == 'Z')
				{
					if (msgTotalLen>=8) 
					{
						char Head[10]={0};
						memcpy(Head,tempbuf+2,6);
						msgLen=atoi(Head);
					}
					else
					{
						continue;
					}
					if (msgTotalLen<msgLen)
					{
						continue;
					}
					else
					{
						char temp[50]={0};
						const char cFlag[]=",";
						int iCount=0,iNumber=0;
						char* token;
						memcpy(temp,recvbuf,25);
						token = strtok(temp, cFlag);
						while( token != NULL )
						{
							iCount++;
							if (5==iCount) 
							{
								iret=atoi(token);
								break;
							}
							token = strtok(NULL, cFlag);
						}					
						memcpy(strMsg,recvbuf+20,strlen(recvbuf)-20);
						bFlag=false;
						break;
					}
				}
				else
				{
					if (msgTotalLen<msgLen) 
					{
						continue;
					}
					else
					{
						char temp[50]={0};
						const char cFlag[]=",";
						int iCount=0,iNumber=0;
						char* token=NULL;
						memcpy(temp,recvbuf,25);
						token = strtok(temp, cFlag);
						while( token != NULL )
						{
							iCount++;
							if (5==iCount) 
							{
								iret=atoi(token);
								break;
							}
							token = strtok( NULL, cFlag);
						}
						memcpy(strMsg,recvbuf+20,strlen(recvbuf)-20);
						token=NULL;
						bFlag=false;
						break;
					}
				}
			}
			else
			{
				iret=-103;
				bFlag=false;
				break;
			}
		}
		//strTemp.Format("%s",recvbuf);
		//WriteLogEx(strTemp,5);
		return iret;
	}
	catch (...)
	{
	}
	WriteLogEx("����״̬�����쳣",5);
	return -200;
}
/************************************************************************/
/* ����������Ϣ����                                                      */
/************************************************************************/
int CTcpClient::MongateCsGetSmsEx(SOCKET sock, char *strMsg)
{
	int cbRecv = 0;
	int cbSend = 0;
	int len = 0;
	int iret = 0;
	int count = 0;
	int iTotalLen = 0,iMsgLen = 0;
	bool bFlag = true;
	char recvbuf[20480]={0},tempbuf[1001]={0};
	CString strTemp = "";
	try
	{
		if (sock<=0) 
		{
			return -198;
		}
		//WSANOTINITIALISED 
		memset(strMsg,0,sizeof(strMsg));
		if (IsCanSendRecv(sock, m_iOvertime, SEND) == 1)
		{
			cbSend = send(sock, "ZZ000019,31,2,*,*,*", 19, 0);
			if (cbSend<1) 
			{
				return -102;
			}
		}
		else
		{
			return -101;
		}		
		WriteLogEx("������Ϣ:ZZ000019,31,2,*,*,*",5);
		while (bFlag)
		{
			memset(tempbuf, 0, sizeof(tempbuf));
			if (IsCanSendRecv(sock, m_iOvertime, RECV) == 1)
			{
				cbRecv = recv(sock, tempbuf, 500, 0);			
				if (cbRecv<1)
				{
					iret = -104;
					bFlag = false;
					break;
				}				
				tempbuf[cbRecv]=0;
				strTemp.Format("%s",tempbuf);
				WriteLogEx(strTemp,5);
				iTotalLen+=cbRecv;
				if (iTotalLen<20480)//ֻ�е��Ѿ����յ������ϱ��ν��յ��ֽ���С�ڵ���***ʱ,�������ַ���
				{
					strcat(recvbuf,tempbuf);
				}
				else
				{
					iret = -105;	//����ʧ��
					bFlag=false;
					break;
				}
				if (tempbuf[0] == 'E' && tempbuf[1] == 'R')
				{
					iret = -10000 - (atoi(tempbuf + 3));//����ƽ̨������Ϣ
					bFlag = false;
					break;
				}			
				else if (tempbuf[0] == 'O' && tempbuf[1] == 'K')
				{
					iret = 0;//û����Ϣ
					bFlag=false;
					break;
				}			
				else if (tempbuf[0] == 'Z' || tempbuf[1] == 'Z')
				{
					if (iTotalLen>=8)
					{
						char Head[10]={0};
						memcpy(Head,tempbuf+2,6);
						iMsgLen=atoi(Head);
					}
					else
					{
						continue;
					}
					if (iTotalLen<iMsgLen) 
					{
						continue;
					}
					else
					{
						int iCount=0;
						int ilen=0;
						char *p=NULL;
						iCount=atoi(recvbuf+16);
						p=strstr(recvbuf+16,",");
						ilen=strlen(p+1);
						memcpy(strMsg,p+1,strlen(p)-1);
						strMsg[ilen-2]='\0';
						iret=iCount;
						p=NULL;
						bFlag=false;
						break;
					}				
				}
				else
				{
					if (iTotalLen<iMsgLen) 
					{
						continue;
					}
					else
					{
						int iCount=0;
						int ilen = 0;
						char *p=NULL;
						iCount=atoi(recvbuf+16);
						p=strstr(recvbuf+16,",");
						ilen=strlen(p+1);
						memcpy(strMsg,p+1,strlen(p)-1);
						strMsg[ilen-2]='\0';
						iret=iCount;
						p=NULL;
						bFlag=false;
						break;
					}
				}
			}
			else
			{
				iret = -103;
				bFlag = false;
			}
		}
// 		strTemp.Format("%s",recvbuf);
// 		WriteLogEx(strTemp,5);
		return iret;
	}
	catch (...)
	{
	}
	WriteLogEx("����������Ϣ�쳣",5);
	return -200;
}

/************************************************************************/
/* ά�����Ӻ���                                                          */
/************************************************************************/
int CTcpClient::MongateTestConn(SOCKET sock)
{
	int cbRecv = 0;
	int cbSend = 0;
	char buf[50] = {0};
	CString strTemp = "";
	try
	{
		if(sock<=0)
		{
			return -198;
		}
		if (IsCanSendRecv(sock, m_iOvertime, SEND) ==1)
		{
			cbSend = send(sock, "ZZ000018,100,*,*,*", 18, 0);
			if (cbSend < 1)
			{
				return -102;
			}
		}
		else
		{
			return -101; //�ȴ����ͳ�ʱ
		}
		WriteLogEx("ά������:ZZ000018,100,*,*,*",5);
		if (IsCanSendRecv(sock, m_iOvertime, RECV) ==1)
		{
			cbRecv = recv(sock, buf, sizeof(buf), 0);
			if (cbRecv<1)
			{
				return -104;
			}
			buf[cbRecv] = 0;
			strTemp.Format("%s",buf);
			WriteLogEx(strTemp,5);
		}
		else
		{
			return -103;//�ȴ����ճ�ʱ
		}
		buf[sizeof(buf) - 1] = '\0';
		if ((buf[1] == 'K' && buf[0] == 'O'))
		{
			return 1;
		}
		else if (buf[0] == 'E' && buf[1] == 'R')
		{
			return -10000 - atoi(buf+3);
		}
		else
		{
			return -200;//���ӶϿ�
		}
	}
	catch (...)
	{
	}
	return -200;
}

/************************************************************************/
/* ��ѯ����                                                          */
/************************************************************************/
int CTcpClient::MongateQueryBalance(SOCKET sock)
{
	int cbRecv = 0;
	int cbSend = 0;
	char* p = NULL;
	char buf[27]={0};
	CString strTemp = "";
	try
	{
		if(sock<=0)
		{
			return -198;
		}
		if (IsCanSendRecv(sock, m_iOvertime, SEND) ==1)
		{
			cbSend = send(sock, "ZZ000011,28", 11, 0);
			if (cbSend<1)
			{
				return -102;
			}
		}
		else
		{
			return -101;//��ʱ
		}
		WriteLogEx("�����:ZZ000011,28",5);
		if (IsCanSendRecv(sock, m_iOvertime, RECV) ==1)
		{
			cbRecv = recv(sock, buf, sizeof(buf), 0);
			if (cbRecv < 1)
			{
				return -104;//����ʧ��
			}
		}
		else
		{
			return -103;//��ʱ	
		}
		buf[sizeof(buf) - 1]='\0';
		strTemp.Format("%s",buf);
		WriteLogEx(strTemp,5);

		if (buf[0] == 'O' && buf[1] == 'K')
		{
			char* token=NULL;
			const char cFlag[]=",";
			int iFlag=0;
			int iret =0;
			token=strtok(buf,cFlag);
			while(token != NULL)
			{
				iFlag++;
				switch(iFlag) 
				{
				case 2:
					iret=atoi(token);
					break;
				case 3:
					m_iSendNumber=atoi(token);
					break;
				default:
					break;
				}
				token = strtok( NULL,cFlag);			
			}
			token=NULL;
			return iret;
		}
		else if (buf[0] == 'E' && buf[1] == 'R')
		{
			return -10000 - atoi(buf+3);//
		}
		else
		{
			return -200;
		}
	}
	catch (...)
	{
	}
	return -200;
}
/************************************************************************/
/* ��ѯ�Ѿ�ʹ�õ�����                                                    */
/************************************************************************/
int CTcpClient::MongateQueryUsed(SOCKET sock)
{
	return m_iSendNumber;
}
/************************************************************************/
/* �Ͽ��������������                                                    */
/************************************************************************/
void CTcpClient::MongateDisconnect(SOCKET sock)
{
	
#ifdef WIN32
	shutdown(sock, 2);
	closesocket(sock);
#else
	close(sock);
#endif
}
/************************************************************************/
/* ������Ϣ����                                                          */
/************************************************************************/
int CTcpClient::MongateCsSPSendSms(SOCKET sock, const char* pszMobis,const char* pszMsg, int iMobiCount,const char* pszSubPort,char* pszSN)
{
	int cbRecv = 0;
	int cbSend = 0;
	int iFlag = 0;
	int iRecvTotalLen = 0;
	char* buf = NULL;
	char strNMsg[4 * MAX_SMS_LENGTH + 1] = {0};  
	int ret = -11;
	long tStart,tEnd;
	CString strTemp = "";
	if (sock<=0) 
	{
		return -198;
	}
	if (pszMobis == NULL || pszMsg == NULL)
	{
		return -1;
	}
	if (iMobiCount > 1000)
	{
		return -2;
	}
	memset(pszSN,0,sizeof(pszSN));
	srcStrTohexChr(pszMsg,strNMsg);//������Ϣ����
	strNMsg[4 * MAX_SMS_LENGTH] = '\0';
	buf = (char*)malloc(1024 + 4 * MAX_SMS_LENGTH + iMobiCount * 17);
	if (buf == NULL)
	{
		return -10;
	}
	try
	{
		memset(buf,0,sizeof(buf));
		while (true)
		{		  
			if(iMobiCount==1) //�ֿ�������Ⱥ��
			{
				if (!(IsNumeric(pszMobis))) 
				{
					ret =  -11;//
					break;
				}
				sprintf(buf+8,",23,*,*,*,%s,0,%d,%d,%s,%s", pszSubPort,iMobiCount, strlen(strNMsg),strNMsg, pszMobis);
			}
			else
			{
				iFlag=TelsCheck(pszMobis,iMobiCount);
				if (1==iFlag) 
				{
					sprintf(buf+8,",25,*,*,*,%s,0,%d,%d,%s,%s",pszSubPort,iMobiCount, strlen(strNMsg),strNMsg, pszMobis);
				}
				else
				{
					ret=iFlag;
					break;
				}
			}
			sprintf(buf, "ZZ%06d", strlen(buf+8)+ 8);
			buf[8] = ',';    
			if (IsCanSendRecv(sock, m_iOvertime, SEND) == 1)
			{
				cbSend = send(sock, buf, strlen(buf), 0);
				if(cbSend<1)
				{
					ret = -102;
					break;
				}
			}
			else
			{
				ret = -101;//
				break;
			}
			strTemp.Format("%s",buf);
			WriteLogEx(strTemp,5);
			tStart=tEnd=time(NULL);
			memset(strNMsg,0,sizeof(strNMsg));
			while(tEnd-tStart<m_iOvertime)		//����һ���ĳ�ʱʱ��
			{
				if(strstr(strNMsg,"\r\n"))
				{
					break;
				}
				memset(buf, 0, 50);
				if (IsCanSendRecv(sock, m_iOvertime, RECV) == 1)
				{
					cbRecv = recv(sock, buf, 30, 0); //�������21
					if (cbRecv<1)
					{
						ret = -104;
						break;
					}				
					buf[cbRecv]=0;
					strTemp.Format("%s",buf);
					WriteLogEx(strTemp,5);
					iRecvTotalLen += cbRecv;
					if (iRecvTotalLen < 4096)
					{
						strcat(strNMsg,buf);		//�޸�
					}
					else
					{
						ret = -105;
						break;
					}
				}
				else
				{
					ret = -103;
					break;
				}
				tEnd=time(NULL);
			}
			if (cbRecv < 1)
			{
				ret = -104;
				break;
			}
			memset(buf, 0, 50);
			strncpy(buf,strNMsg,49);
			if (buf[0] == 'O' && buf[1] == 'K')
			{
				strcpy(pszSN, buf+3);
				pszSN[strlen(buf+3)-2] = '\0';
				pszSN[strlen(buf+3)-1] = '\0';
				ret = 1;
				break;
			}
			else if (buf[0] == 'E' && buf[1] == 'R')
			{
				ret = -10000 - (atoi(buf + 3)); //����ƽ̨������Ϣ
				break;
			}
			else
			{
				ret = -200;
				break;		
			}
		}
	}
	catch (...)
	{
		WriteLogEx("������Ϣ�쳣",5);
	}	
	free(buf); 
	buf = NULL;
	return ret;
}

/************************************************************************/
/*���ܷ�����Ϣ���ݺ���                                                    */
/************************************************************************/
void CTcpClient::srcStrTohexChr(const char* strMsg,char* retstr)
{
	char strTemp[3]={0};
	char strASCII[4 * MAX_SMS_LENGTH + 1]={0};
	char pbuf[MAX_SMS_LENGTH + 1]={0};
	int len,i;
	if (strMsg == NULL || retstr == NULL)
	{
		return;
	}
	strcpy(pbuf,strMsg);
	*retstr = 0;
	len=strlen(pbuf);
	for(i = 0; i<len; i++)
	{		
		sprintf(strTemp,"%02X", pbuf[i]&0xff);		
		strcat(strASCII,strTemp);
	}
	len=strlen(strASCII);
	strASCII[len]='\0';
	memcpy(retstr,strASCII,len);
	retstr[len]='\0';
	return;
} 

/************************************************************************/
/*���ܺ���                                                               */
/************************************************************************/
char* CTcpClient::hexChrTosrcStr(const char* strMsg,char* retstr)
{	
	char strTemp[3]={0},strTemp1[3]={0};
	char strASCII[MAX_SMS_LENGTH + 1]={0};
	int i=0,ihex=0;
	int ilen=strlen(strMsg)/2;
	if (strMsg==NULL) 
	{
		return NULL;
	}
	if(strstr(strMsg,"\r\n")>0)
	{
		ilen=strlen(strMsg)/2 - 1;//ȥ��β���Ļس��ͻ���
	}
	for(i = 0; i<ilen*2; i++)
	{
		memset(strTemp1,0,3);
		sprintf(strTemp1,"%c%c",strMsg[i],strMsg[i+1]);
		i++;
		sscanf(strTemp1, "%x", &ihex);
		sprintf(strTemp, "%c", ihex);
		strASCII[(i+1)/2-1]=strTemp[0];		
	}
	strASCII[ilen]='\0';
	strcpy(retstr,strASCII);
	return retstr;
}

int CTcpClient::code_convert(char *from_charset,char *to_charset,const char *inbuf,int inlen,char *outbuf,int outlen)
{
	//strcpy(outbuf,inbuf);
	return 0;
}
/************************************************************************/
/* �ж��ֻ������Ƿ�Ϊ����                                                 */
/************************************************************************/
bool CTcpClient::IsNumeric(const char* pszMobi)
{
	char *p=(char *)pszMobi;
	if (p == NULL)
	{
		return false;
	}
	while (*p != 0)
	{
		if (*p < 48 || *p > 57)
		{
			return false;
		}
		p++;
	}
	return true;
}
/************************************************************************/
/* ����ֻ������Ƿ�Ϸ�                                                   */
/************************************************************************/
int  CTcpClient::TelsCheck(const char* pTels,int iTelNum)
{
	char * token=NULL;
	int iNumber=0;
	int iret=1;
	char *temp=NULL;
	const char cFlag[]=",";
	int kk=0;
	if (pTels==NULL||iTelNum==0) 
	{
		return -15;
	}
	temp=(char*)malloc(strlen(pTels)+1);
	if (temp==NULL) 
	{
		return -10;
	}
	memset(temp,0,sizeof(temp));
	kk=strlen(pTels);
	memcpy(temp,pTels,strlen(pTels));
	temp[kk]='\0';
	token=strtok(temp,cFlag);
	while (token != NULL) 
	{
		iNumber++;
		if (!(IsNumeric(token))) 
		{
			iret=-11;
			break;
		}
		if (strlen(token)>15) 
		{
			iret=-12;
			break;
		}
		token = strtok( NULL,cFlag);
	}
	if (iret==-11||iret==-12) 
	{
		free(temp);
		token=NULL;
		temp=NULL;
		return iret;
	}
	if (iNumber!=iTelNum) 
	{
		iret=-13;
	}
	else if (iNumber>1000) 
	{
		iret=-14;
	}
	free(temp);
	token=NULL;
	temp=NULL;
	return iret;
}
/************************************************************************/
/* ����Ľ��ܺ���                                                        */
/************************************************************************/
int CTcpClient::MGhexChrTosrcStr(const char* srcMsg,char* retMsg)
{
	char *p=NULL;
	if (srcMsg==NULL) 
	{
		return -1;
	}
	p=hexChrTosrcStr(srcMsg,retMsg);
	if (p==NULL) 
	{
		return -1;
	}
	else
	{
		return 1;
	}
}
/************************************************************************/
/* �����˺�����                                                          */
/************************************************************************/
void CTcpClient::SetCmdType(int iCmdType)
{
	m_iCmdType = iCmdType;
}
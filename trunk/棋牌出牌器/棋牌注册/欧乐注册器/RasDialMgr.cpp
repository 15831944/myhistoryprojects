#include "StdAfx.h"
#include "datatype.h"

CRasDialMgr::CRasDialMgr(void)
{
	m_hRscon = NULL;
}

CRasDialMgr::~CRasDialMgr(void)
{
}

int CRasDialMgr::DialRas(CString &strResult)
{
	DWORD iRet= RasDialA(NULL,NULL,&m_rdParams,0L,NULL,&m_hRscon);
	if (iRet == 0)
	{
		strResult = "���ųɹ�";
	}
	else
	{
		m_hRscon = NULL;
		strResult = "���ӳ���";
	}
	return iRet;
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
int CRasDialMgr::StopRas(CString &strResult)
{
	//ö������
	DWORD iRet = 0;
	if (m_hRscon != NULL)
	{
		iRet= RasHangUp(m_hRscon);
		if (iRet==0)
		{
			strResult = "�����ѶϿ�";
			m_hRscon = NULL;
			return iRet;
		}
		else
		{
			strResult = "�Ͽ�����ʧ��";
		}		
	}	
	DWORD iCount = sizeof(RASCONN);
	RASCONN rasConn;
	rasConn.dwSize = 0x19c;
	DWORD iLen = 0;
	iRet = RasEnumConnections(&rasConn,&iCount,&iLen);
	if (iRet == 0 && iLen > 0)
	{
		for (int i=0;i<iLen;i++)
		{
			iRet= RasHangUp(rasConn.hrasconn);
			if (iRet==0)
			{
				strResult = "�����ѶϿ�";
				rasConn.hrasconn = NULL;				
			}
			else
			{
				strResult = "�Ͽ����ӳ���";
			}
		}
	}
	else
	{
		strResult = "������";
	}
	m_hRscon = NULL;
	return iRet;
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CRasDialMgr::SetParams(CString strAcc,CString strPwd,CString strConnName)
{
	m_rdParams.dwSize=sizeof(RASDIALPARAMSA);
	strcpy(m_rdParams.szEntryName,strConnName); //������������Ĳ�������
	m_rdParams.szPhoneNumber[0]='\0';
	m_rdParams.szCallbackNumber[0]='\0';
	strcpy(m_rdParams.szUserName,strAcc);
	strcpy(m_rdParams.szPassword,strPwd);
	m_rdParams.szDomain[0]='\0';
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
int CRasDialMgr::GetGateWayIP(CString &strIP)
{
	RASPPPIP rip;
	ZeroMemory(&rip,sizeof(rip));
	//RASPROJECTION rasprj;
// 	RASPROJECTION
// 	{
// 		RASP_Amb = 0x10000,
// 			RASP_PppNbf = 0x803F,
// 			RASP_PppIpx = 0x802B,
// 			RASP_PppIp = 0x8021,
// #if (WINVER >= 0x500)
// 			RASP_PppCcp = 0x80FD,
// #endif
// 			RASP_PppLcp = 0xC021,
// 			RASP_Slip = 0x20000
// 	};
	DWORD size;
	size = 40;
	rip.dwSize = 40;
	if((RasGetProjectionInfo(m_hRscon,RASP_PppIp,&rip,&size)) != 0)		//��ȡ���ź�Ĺ���IP
	{
		strIP = "";
		return -1;
	}
	else
	{
		strIP.Format("%s",rip.szIpAddress);
	}
	return 0;
}
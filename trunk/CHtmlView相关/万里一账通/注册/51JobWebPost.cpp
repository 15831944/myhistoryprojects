#include "StdAfx.h"
#include ".\51jobwebpost.h"
#include "UserMessagePump.h"
#include <io.h>
#include <odbcinst.h>
#include <afxdb.h>
#include <direct.h>
#include "UserFile.h"
#include "ADOFile/AdoRecordSet.h"
#include "WindowFind.h"
#include <Iphlpapi.h>
#pragma comment(lib,"Iphlpapi.lib")
HANDLE  g_hEvent = NULL;			//�¼����
CString g_strCheckCode = "";
bool g_ExitFlag = true;
bool g_ExitFlagDialog = true;
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
int g_iOptFlag = -1;
int g_iOptedFlagEx = 0;			//�Ѿ�����õĲ���
int g_iOldWidthAdd = -1;
void ShowMessageExEx(CString strMsg);
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
void AddToShowList(RegisterPack rgPack)
{
	CString strStauts = "";
	int nCount = g_AccList.GetItemCount();
	for ( int i = 0;i<nCount;i++)
	{
		if (g_AccList.GetItemText(i,1) == rgPack.strAcc&&g_AccList.GetItemText(i,7) != "�ɹ�")
		{
			g_AccList.SetItemText(i,6,rgPack.strPhone);
			g_AccList.SetItemText(i,7,rgPack.strMemo);
			break;
		}		
	}
	CString strLog = "";
	strLog.Format("%s,%s,,,,,",rgPack.strAcc,rgPack.strPwd);
	WriteLog(strLog,rgPack.iStatus);
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
void ShowMessageExEx(CString strMsg)
{
	WriteLog(strMsg,1);
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
			filename.Format("%s_success.txt",currTime.Format("%Y%m%d"));
		}
		else if(iFlag == 1)		//��Ϣ��־
		{
			folder = "log";
			filename.Format("%s_log.txt",currTime.Format("%Y%m%d"));
		}
		else if ( iFlag == 2 )	//������־
		{
			folder = "log";
			filename.Format("%s_fail.txt",currTime.Format("%Y%m%d"));
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
		if (iFlag == 1)
		{
			destStr.Format("%s	%s",CTime::GetCurrentTime().Format("%Y-%m-%d %H:%M:%S"),srcStr);
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
C51JobWebPost::C51JobWebPost()
{
    //��ʼ����Ϣ
    m_nErrorTime = 0;
	m_LoginFlag = false;
	m_OpenFreshPageFlag = false;
	m_ExceptionNum = 0;
	m_FreshTimes = 0;
	switch (g_iRegWebFlag)
	{
	case 0:
		m_strServerURL = "www.7ple.com";
		break;
	case 1:
		m_strServerURL = "www.7pmi.com";
		break;
	case 2:
	default:
		m_strServerURL = "www.7psw.com";
		break;
	}	
	m_TimeInterval = 3;
	m_strHeader = "Content-Type: application/x-www-form-urlencoded";
	m_strHeader += "\r\nAccept:image/gif, image/x-xbitmap, image/jpeg, image/pjpeg, application/x-shockwave-flash, application/vnd.ms-excel, application/vnd.ms-powerpoint, application/msword, application/x-silverlight, */*";
	m_strHeader += "\r\nAccept-Language:zh-cn";
	m_strHeader += "\r\nAccept-Encoding:gzip, deflate";//
	m_strHeader += "\r\nCache-Control:no-cache";
	m_strHeader += "\r\nUser-Agent:Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1; SV1; .NET CLR 1.1.4322; .NET CLR 2.0.50727)";
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
//�û���¼
bool C51JobWebPost::UserRegister()
{
	try
	{
		switch (g_iRegWebFlag)
		{
		case 0:
			m_strServerURL = "www.7ple.com";
			break;
		case 1:
			m_strServerURL = "www.7pmi.com";
			break;
		case 2:
		default:
			m_strServerURL = "www.7psw.com";
			break;
		}
		CString strError = "";
		CString sendStr = "";
		ShowMessage("���ڻ�ȡע��ҳ��...");
		CString objectlink = "";
		switch (g_iRegWebFlag)
		{
		case 0:
			objectlink = "/reg.jhtml?siteID=11";
			break;
		case 1:
			objectlink = "/reg.jhtml?siteID=11";
			break;
		case 2:
		default:
			objectlink = "/reg.jhtml?siteID=18";
			break;
		}
		CString strHeaders = m_strHeader;
		//int iRet = RequestGetPage(m_Session,m_pConnection,m_strServerURL,objectlink,strHeaders);
		int iRet = RequestGetPageExExEx(m_strServerURL,objectlink,strHeaders,
			strError);//"148.233.239.24",3128,"","",0);
		if ( iRet != 0 )
		{
			TRACE("��ȡҳ�����");
			ShowMessage("��ȡע��ҳ��ʧ��");
			m_nErrorTime ++;
			DeleteObjectExEx();
			return false;
		}
		m_strWorkBuf.MakeLower();
		//AfxMessageBox(m_strWorkBuf.Right(m_strWorkBuf.GetLength() - 200));
		//����Ƿ�򿪼�������ҳ
		int iPos = m_strWorkBuf.Find("�һ�������");
		if (iPos >= 0)
		{
			ShowMessage("�ɹ���ȡע��ҳ,���ڻ�ȡ��֤��...");
		}
		else
		{
			return false;
		}
		CString postStr = "";
		CString strShow = "";		
		while(!g_ExitFlag)
		{
			try
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
				/************************************************************************/
				/* �˴��Ȼ�ȡע��ҳ�����Ϣ,�Ա����                                    */
				/************************************************************************/
				//HWND hwnd = ::FindWindow("IEFrame","������ - ע���ʺ� - Microsoft Internet Explorer");
				m_hwnd = ::FindWindow("#32770","�ʺ�ע��");
				char str[128] = {0};
				HWND hwnd = NULL;
				CWindowFind find;
				//hwnd = find.MyFindWindow("Internet Explorer_Server","",hwnd);
				hwnd = find.GetChildWindow(m_hwnd,1);
				hwnd = find.GetChildWindow(hwnd,1);
				hwnd = find.GetChildWindow(hwnd,1);
				hwnd = find.GetChildWindow(hwnd,1);
				GetClassName(hwnd,str,sizeof(str));//��ȡ���� 
				if(strcmp(str,"Internet Explorer_Server")==0)
				{
					IHTMLDocument2 *pDoc = GetDocInterface(hwnd);
					GetHtmlCtrl(pDoc);
				}
				rgPack.strIp = g_strCurrIP;
				m_iCheckFlag = 0;
				g_strCheckCode = "";
				if (m_iCheckFlag == 0)
				{
					//ɾ���ɵ���֤��
					if (_access(g_strDirect+g_strFileName,0) == 0)
					{
						CFile::Remove(g_strDirect+g_strFileName);
					}
					ShowMessage("���ڻ�ȡ��֤��...");
					m_iCheckFlag = 0;
					objectlink = "/RegRandomCodeCtrl";
					//strHeaders = m_strHeader + "\r\nReferer:http://www.dajiale365.com/reg.shtml";
					iRet = RequestGetPageExExEx(m_strServerURL,objectlink,strHeaders,strError,1);
					if (iRet != 0)
					{
						ShowMessage("��ȡ��֤��ʧ��");
						m_nErrorTime ++;
						DeleteObjectExEx();
						return false;
					}
					else
					{
						ShowMessage("��ȡ��֤��ɹ�");	
						strShow.Format("����ע��:%s %s",rgPack.strAcc,rgPack.strPwd);
						ShowMessage(strShow);
					}
				}		
				while(!g_ExitFlag)
				{
					if ( g_strCheckCode != "")
					{
						break;
					}
					Sleep(100);
				}
				if (g_ExitFlag)
				{
					DeleteObjectExEx();
					return false;
				}
				CString strFailReason = "";
				CString strCheckCode = g_strCheckCode;
				if (strCheckCode == "r")
				{
					continue;
				}
				objectlink = "/regAction.jhtml";

				switch (g_iRegWebFlag)
				{
				case 0:
					strHeaders = m_strHeader + "\r\nReferer:http://www.7ple.com/reg.jhtml?siteID=11";
					break;
				case 1:
					strHeaders = m_strHeader + "\r\nReferer:http://www.7pmi.com/reg.jhtml?siteID=11";
					break;
				case 2:
				default:
					strHeaders = m_strHeader + "\r\nReferer:http://www.7psw.com/reg.jhtml?siteID=18";
					break;
				}	
				
				//strHeaders += "\r\nx-requested-with: XMLHttpRequest";
				//strHeaders += "\r\nContent-Type: application/x-www-form-urlencoded";
				CString strTime = CTime::GetCurrentTime().Format("%m%d%H%M%S");
				//strCheckCode = NativeToAscii(strCheckCode);
				//strCheckCode = "ȷѧװ��";
				//ConvertGBKToUtf8(strCheckCode);
				m_strMac.MakeUpper();
				switch (g_iRegWebFlag)
				{
				case 0:
					ConvertGBKToUtf8(strCheckCode);
					strCheckCode = URLEncode(strCheckCode);
					postStr.Format("username=%s&password=%s&repassword=%s&sex=1&myhead=1&email=%s&siteID=11&7pmiReferrerUrl=&7pmiLocalUrl=&selfinfo=&machineserial=%s&transactionid=%s&verify=%s&ptvodaccounts=&introducer=%s&checkcode=%s&agree=on",rgPack.strAcc,rgPack.strPwd,rgPack.strPwd,rgPack.strMail,m_strMac,m_strTrans,m_strVerify,rgPack.strNickName,strCheckCode);
					break;
				case 1:
					postStr.Format("username=%s&password=%s&repassword=%s&sex=1&myhead=1&email=%s&siteID=11&7pmiReferrerUrl=&7pmiLocalUrl=&selfinfo=&machineserial=%s&transactionid=%s&verify=%s&ptvodaccounts=&introducer=%s&checkcode=%s&agree=on",rgPack.strAcc,rgPack.strPwd,rgPack.strPwd,rgPack.strMail,m_strMac,m_strTrans,m_strVerify,rgPack.strNickName,strCheckCode);
					break;
				case 2:
				default:
					ConvertGBKToUtf8(strCheckCode);
					strCheckCode = URLEncode(strCheckCode);
					postStr.Format("username=%s&password=%s&repassword=%s&sex=1&myhead=1&email=%s&siteID=18&7pmiReferrerUrl=&7pmiLocalUrl=&selfinfo=&machineserial=%s&transactionid=%s&verify=%s&ptvodaccounts=&introducer=%s&checkcode=%s&agree=on",rgPack.strAcc,rgPack.strPwd,rgPack.strPwd,rgPack.strMail,m_strMac,m_strTrans,m_strVerify,rgPack.strNickName,strCheckCode);
					break;
				}
				iRet = RequestPostPageExExEx(m_strServerURL,objectlink,strHeaders,postStr,strError,0);//
				if ( iRet != 0 )
				{
					TRACE("POST����");
					m_nErrorTime ++;
					DeleteObjectExEx();
					return false;
				}
				m_strWorkBuf.MakeLower();				
				iPos = m_strWorkBuf.Find("ע��ɹ�");
				if ( iPos >= 0)
				{
                    ShowMessage("ע��ɹ�");
					rgPack.iStatus = 1;
					rgPack.strMemo = "�ɹ�";
					strShow.Format("%s,%s,",rgPack.strAcc,rgPack.strPwd);
					WriteLog(strShow,0);
					//����ע��ɹ����˺Ž����ݿ�
					iRet = WriteToBase(rgPack,g_iRegWebFlag);
					if (iRet != 0)
					{
						WriteLog(strShow,3);
					}
					m_iIpSuccNum++;
					if (m_iIpSuccNum >= g_iIpRegNum)
					{
						if (g_iAotuChangeIp)
						{
							ShowMessage("��һIPע�����Ѵ�����,�����л�IP...");
						}
						if (g_iIsChangeMac)
						{
							ShowMessage("��һMAC��ַע�����Ѵ�����,�����л�MAC��ַ...");
						}
						m_iNeedChageIP = 1;
						m_iIpSuccNum = 0;
						m_iIsNeedChangeMac = 1;
					}
				}
				else
				{//ע��ʧ��						
					CString strBefore = "<div align=\"center\"><font color='red'>";
					CString strAfter = "</font></div>";
					strFailReason = GetKeyString(m_strWorkBuf,strBefore,strAfter);
					ShowMessage("ע��ʧ��:	"+strFailReason);
					rgPack.iStatus = 2;
					rgPack.strMemo = strFailReason;
				}
				AddToShowList(rgPack);
				if (g_iDeleteFlag == 1)
				{
					if (strFailReason.Find("��֤���������")<0)
					{
						DeleteRegisterPack(rgPack);												
					}
					else
					{
						m_iCodeErrorNum ++;
						if (m_iCodeErrorNum >= g_iCodeErrorNum)
						{
							ShowMessage("��֤����������������,�������˺ŵ�ע��");
							DeleteRegisterPack(rgPack);
							m_iCodeErrorNum = 0;
						}
					}
				}
				else
				{
					DeleteRegisterPack(rgPack);
				}
#ifdef DEBUG
				m_iNeedChageIP = 1;
#endif
				if (m_iNeedChageIP == 1)		//��IP
				{
					DeleteObjectExEx();
					return false;
				}
				if (m_iIsNeedChangeMac == 1)		//��MAC��ַ
				{
					DeleteObjectExEx();
					return false;
				}
			}
			catch (...)
			{
				DeleteObjectExEx();
				return false;
			}			
		}
		return false;  
	}
	catch (...)
	{
	}
	DeleteObjectExEx();
	return false;            
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void C51JobWebPost::ThreadFunc(LPVOID lParam)
{
     C51JobWebPost *pParam = ((C51JobWebPost *)lParam);
     if ( pParam )
     {
		 //�������
        pParam->RegisterFuncEx();
		pParam->ShowMessage("�߳��˳�");
     }
     
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void C51JobWebPost::RegisterFuncEx()
{
	while (!g_ExitFlag)
	{
		RegisterPack rgPack;
		int iRet = GetRegisterPack(rgPack);
		if (iRet == -1)
		{
			ShowMessage("�뵼���˺�...");
			Sleep(700);
			continue;
		}
		else if (iRet == -2)
		{
			Sleep(2000);
			continue;
		}
		iRet = AddEmail(rgPack);
		switch (iRet)
		{
		case  -3:			
			DeleteRegisterPack(rgPack);
			AddToShowList(rgPack);
			break;
		case -1:
			break;
		case 0:
			DeleteRegisterPack(rgPack);
			AddToShowList(rgPack);
			break;
		default:
			break;
		}
	}
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void C51JobWebPost::RegisterFunc()
{
	m_iNeedChageIP = 0;
	m_iIpSuccNum = 0;
	CString sendStr = "";
	ShowMessage("��ʼ���ɹ�");
	if (g_iAotuChangeIp == 1)
	{
		sendStr.Format("�Զ���IP,��һIPע����:%d",g_iIpRegNum);
		ShowMessage(sendStr);
	}	
	if (g_iAotuCheckCode==1)
	{
		ShowMessage("�Զ�ʶ����֤��");
	}
	m_strOldIP = "";
	while (!g_ExitFlag)
	{
		if (g_RegisterList.GetSize() <= 0)
		{
			Sleep(500);
			continue;
		}
		
		m_iIpSuccNum = 0;
		m_iNeedChageIP = 1;
		int iErrorCount = 0;
		int iSameIpCount = 0;
		if (!g_iAotuChangeIp)		//����Ҫ�Զ���IP
		{
			//����MAC��ַ
			if (m_iIsNeedChangeMac)
			{
				m_iIsNeedChangeMac = ChangeMacAddr();
				if (m_iIsNeedChangeMac == 1)
				{
					ShowMessage("�޸�MAC��ַʧ��");
				}
				else
				{
					m_iIsNeedChangeMac = 0;
				}
			}
			else
			{
				ShowMessage("�Ѿ�����MAC��ַ");				
			}
		}
		while (m_iNeedChageIP && !g_ExitFlag && g_iAotuChangeIp == 1)	//�����Ҫ��IP
		{
			m_iNeedChageIP = 1;
			g_RasMgr.StopRas(sendStr);
			ShowMessage(sendStr);
			//����MAC��ַ
			if (m_iIsNeedChangeMac == 1)
			{
				 m_iIsNeedChangeMac = ChangeMacAddr();
				 if (m_iIsNeedChangeMac == 1)
				 {
					 ShowMessage("�޸�MAC��ַʧ��");
				 }
				 else
				 {
					 m_iIsNeedChangeMac = 0;
				 }
			}
			else
			{
				 ShowMessage("�Ѿ�����MAC��ַ");
			}
			int iRet =  g_RasMgr.DialRas(sendStr);			
			ShowMessage(sendStr);
			if (iRet == 0)		//���²��ųɹ�����ȡ�µ�IP
			{
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
			iErrorCount ++;
			if (iErrorCount>=10)
			{
				ShowMessage("��������ʧ�ܴ�������,��ȷ������ʹ��ADSL���Ų���������ȷ");
				return;
			}
			Sleep(500);
		}
		m_iIsNeedChangeMac = 0;
		m_iNeedChageIP = 0;
		if (!g_ExitFlag)
		{
			UserRegister();
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
/*                                                                      */
/************************************************************************/
void C51JobWebPost::SaveToExcel(RegisterPack rgPack)
{
	CAdoConnection adoConn;
	CString strError;
	//CreatExcelDb("c:\\aaa.xls","hello",strError);
// 	CAdoConnection adoConn;
// 	CAdoRecordSet adoRst;
// 	try
// 	{
// 		if (!adoConn.IsOpen())
// 		{
// 			if(adoConn.ConnectExcel("c:\\aa.xml"))
// 			{
// 				adoRst.SetAdoConnection(&adoConn);
// 				adoRst.SetCursorLocation(adUseClient);
// 			}
// 			else
// 			{
// 				//������
// 				
// 				ShowMessage("��Excel�ļ�ʧ��"+adoConn.GetLastErrorText());
// 				return;
// 			}
// 		}
// 		CString strSQL
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
/************************************************************************/
/*                                                                      */
/************************************************************************/
int C51JobWebPost::WriteToBase(RegisterPack rgPack,int iFlag)
{
	int iRet = -1;
	CAdoConnection adoConn;
	CAdoRecordSet adoRst;
	try
	{
		if ( !adoConn.IsOpen() )		//���δ�����Ϸ�����
		{
			CString strAppPath = "";
			CUserFile::GetInstance()->GetAppPath(strAppPath);
			if ( !adoConn.ConnectAccess(strAppPath+"7pRegister.mdb",""))
			{
				ShowMessage("�������ݿ�ʧ��");
				return -1; 
			}
			adoRst.SetAdoConnection(&adoConn);
			adoRst.SetCursorLocation(adUseClient);
		}
		CString strSQL;		
		strSQL.Format("insert into [Users] ([userid],[userpwd],flag)values('%s','%s',%d,)",rgPack.strAcc,rgPack.strPwd,iFlag);
		if( !adoRst.Open(strSQL,adCmdText))
		{
			iRet = -1;
			ShowMessage("�����˺���Ϣʧ��");
		}
		else
		{
			iRet = 0;
		}
		adoRst.Close();
		adoConn.Close();
		return iRet;
	}
	catch (...)
	{
		adoRst.Close();
		adoConn.Close();
		ShowMessage("�����˺���Ϣ�쳣");
	}
	return -1;
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
		//CString strValue = "onfocus";
		//BSTR bstrValue = strName.AllocSysString();
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
				//CComPtr<IHTMLImgElement> pImg;
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
					//pElement->
// 					CString strValue = "";
// 					if (varValue.vt == VT_BSTR)
// 					{
// 						strValue.Format ("%s",(const char*)(_bstr_t)(_variant_t)varValue);
// 					}
// 					strValue.MakeLower();
// 					ShowMessage(strValue);
// 					if (strID == "imgReg")
// 					{
// 						ShowMessage("�ҵ�ע�ᰴť");
// 						//m_pRegBtn = pElement;
// 						iFindCount ++;
// 						//pElement->click();
// 					}
// 					else if (strID == "introducer")
// 					{
// 						ShowMessage("�ҵ��Ƽ��������");
// 						//pElement->setAttribute(bstrValue,"aa",0);
// 						//pElement->
// 						//pUserIntroman = (CComPtr<IHTMLInputTextElement>)pElement;
// 						iFindCount ++;
// 					}
					if (strID == "machineserial")		//email name repassword password
					{
						//ShowMessage("�ҵ�MAC��ַ������");
						m_strMac = GetElementValue(pElement);
						ShowMessage(m_strMac);
						//iFindCount ++;
					}
// 					else if (strID == "username")		
// 					{
// 						ShowMessage("�ҵ�USERNAME");
// 						iFindCount ++;
// 					}
// 					else if (strID == "userpwd")
// 					{
// 						ShowMessage("�ҵ�����");
// 						iFindCount ++;
// 					}
// 					else if (strID == "repassword" || strValue == "repassword")
// 					{
// 						ShowMessage("�ҵ�����ȷ��");
// 						iFindCount ++;
// 					}
// 					else if (strID == "checkcode" || strValue == "checkcode")
// 					{
// 						ShowMessage("�ҵ���֤��");
// 						iFindCount ++;
// 					}
					else if (strID == "transactionid")
					{
						//�ҵ�TRANS
						//ShowMessage("�ҵ�TRANS");
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
		strRet.Format ("%s",(const char*)(_bstr_t)(_variant_t)varValue);
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
/************************************************************************/
/*                                                                      */
/************************************************************************/
int C51JobWebPost::AddEmail(RegisterPack &rgPack)
{
	CString strMsg = "";
	//strMsg.Format("%s,%s,%s,%s,%s,%s,%s",rgPack.strAcc,rgPack.strPwd,rgPack.strMail,rgPack.strIntroMan);
	m_rgPack = rgPack;
	SendAddMessage(0,strMsg);
	g_strCheckCode = "";
	g_iOptedFlagEx = 100;
	while(true)		//�ȴ���֤�뱣��ɹ�
	{
		if (g_ExitFlag)
		{
			return -5;
		}
		if (g_iOptedFlagEx <= 1)
		{
			break;
		}
		Sleep(500);
	}
	switch (g_iOptedFlagEx)
	{
	case -2:
		{
			ShowMessageExEx("��ӹ����г��ִ���,���¿�ʼ");
			return -1;
			break;
		}
	case -3:
		{
			ShowMessageExEx("�˻����������,�������˺�");
			rgPack.strMemo = "�˻����������";
			rgPack.iStatus = 2;
			return -3;
			break;
		}
	case -8:
		{
			return -1;
			break;
		}
	case -7:
		{
			rgPack.strMemo = "����δͨ����֤";
			rgPack.iStatus = 2;
			return -3;
			break;
		}
	}
	//�ȴ���֤������	
	while(true)
	{
		if (g_ExitFlag)
		{
			return -5;
		}
		if (g_strCheckCode != "")
		{
			break;
		}
		Sleep(500);
	}
	//����֤�봫����Ӵ���
	
	g_iOptedFlagEx = 100;
	SendAddMessage(1,g_strCheckCode);
	while(true)		//�ȴ����ڷ��ص�¼��֤��Ϣ
	{
		if (g_ExitFlag)
		{
			return -5;
		}
		if (g_iOptedFlagEx < 1)
		{
			break;
		}
		Sleep(500);
	}
	g_strCheckCode = "";
	switch (g_iOptedFlagEx)
	{
	case 0:
		ShowMessageExEx("��ӳɹ�");
		rgPack.strMemo = "�ɹ�";
		rgPack.iStatus = 0;
		return 0;
		break;
	case -2:
		{
			ShowMessageExEx("��ӹ����г��ִ���,���¿�ʼ");
			return -1;
			break;
		}
	case -3:
		{
			//ShowMessageExEx("��д��Ϣ����");
			rgPack.strMemo = "��д��Ϣ����";
			rgPack.iStatus = 2;
			return -3;
			break;
		}
	case -4:
		{
			//ShowMessageExEx("��д��Ϣ����");
			rgPack.strMemo = "�Ѿ�ע��һ��ͨ";
			rgPack.iStatus = 2;
			return -3;
			break;
		}
	case -8:
		{
			return -1;
			break;
		}
	case -7:
		{
			rgPack.strMemo = "����δͨ����֤";
			rgPack.iStatus = 2;
			return -3;
			break;
		}
	}
// 	if (g_iOptedFlagEx == 2)
// 	{
// 		while(true)		//�ȴ����ڷ��ص�¼��֤��Ϣ
// 		{
// 			if (g_ExitFlag)
// 			{
// 				return -5;
// 			}
// 			if (g_strCheckCode != "")
// 			{
// 				break;
// 			}
// 			Sleep(500);
// 		}
// 	}
// 	SendAddMessage(5,g_strCheckCode);
// 	while(true)		//�����ֻ���
// 	{
// 		if (g_ExitFlag)
// 		{
// 			return -5;
// 		}
// 		if (g_iOptedFlagEx <= 2)
// 		{
// 			break;
// 		}
// 		Sleep(500);
// 	}
// 	g_strCheckCode = "";
// 	switch (g_iOptedFlagEx)
// 	{
// 	case 0:
// 		ShowMessageExEx("�ֻ���֤���Ѿ�����,�뾡����д�ֻ���֤��");
// 		break;
// 	case -2:
// 		{
// 			ShowMessageExEx("��ӹ����г��ִ���,���¿�ʼ");
// 			return -1;
// 			break;
// 		}	
// 	}
// 	if (g_iOptedFlagEx == 2)
// 	{
// 		while(true)		//�����ֻ���֤��
// 		{
// 			if (g_ExitFlag)
// 			{
// 				return -5;
// 			}
// 			if (g_strCheckCode != "")
// 			{
// 				break;
// 			}
// 			Sleep(500);
// 		}
// 	}
// 	SendAddMessage(6,g_strCheckCode);//�����ֻ���
// 	while(true)		//�ȴ����ڷ��ص�¼��֤��Ϣ
// 	{
// 		if (g_ExitFlag)
// 		{
// 			return -5;
// 		}
// 		if (g_iOptedFlagEx < 1)
// 		{
// 			break;
// 		}
// 		Sleep(500);
// 	}
// 	g_strCheckCode = "";
// 	switch (g_iOptedFlagEx)
// 	{
// 	case 0:
// 		ShowMessageExEx("ע��ɹ�");
// 		rgPack.strMemo = "ע��ɹ�";
// 		rgPack.iStatus = 0;
// 		return 0;
// 		break;
// 	case -2:
// 		{
// 			ShowMessageExEx("��ӹ����г��ִ���,���¿�ʼ");
// 			return -1;
// 			break;
// 		}
// 	case -3:
// 		{
// 			ShowMessageExEx("�˻����������,�������˺�");
// 			rgPack.strMemo = "�˻����������";
// 			rgPack.iStatus = 2;
// 			return -3;
// 			break;
// 		}
// 	case -8:
// 		{
// 			return -1;
// 			break;
// 		}
// 	case -7:
// 		{
// 			rgPack.strMemo = "�˻���Ϣ����";
// 			rgPack.iStatus = 2;
// 			return -3;
// 			break;
// 		}
// 	}
	return 0;
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
void C51JobWebPost::SendAddMessage(int iFlag,CString strMsg)
{
	strMsg.Format("%d,%s",iFlag,strMsg);
	CUserMessagePump *pMsgPump = NULL;
	pMsgPump = CUserMessagePump::GetInstance();
	if (pMsgPump)
	{
		pMsgPump->AddToMessagePump(USERWINDOWMSG_2,strMsg);
	}
}
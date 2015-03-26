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
#include "MD5.h"
#include <string>
using namespace std;
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
CString g_strGotNum = "";
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
	bool isExist = false;
	for ( int i = 0;i<nCount;i++)
	{
		if (g_AccList.GetItemText(i,2) == rgPack.strAcc&&g_AccList.GetItemText(i,3) == rgPack.strPwd)
		{
			isExist = true;
			g_AccList.SetItemText(i,4,rgPack.strMemo);
			g_AccList.SetItemText(i,1,rgPack.strIp);
			break;
		}		
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
			filename.Format("%s_succes.txt",currTime.Format("%Y%m%d"));
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
		if (iFlag == 0 || iFlag == 2)
		{
			destStr = srcStr;
		}
		else
		{
			destStr.Format("%s	%s",currTime.Format("%Y-%m-%d %H:%M:%S"),srcStr);
		}
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
//�û���¼
bool C51JobWebPost::UserRegister()
{
	try
	{
		m_strServerURL = "www.eku8.com";
		CString strError = "";
		CString sendStr = "";
		ShowMessage("���ڻ�ȡ����ҳ��...");
		//����Ҫ����ID�ź�MD5���ܺ������
		CString objectlink = "/JH_UserConfig/DayPay.Aspx?UserID=4974619&PassWord=9ab50b8e55bdf45adb12de33a2279273";
		CString strHeaders = m_strHeader;
		int iRet = RequestGetPageExExEx(m_strServerURL,objectlink,strHeaders,strError);
		if ( iRet != 0 )
		{
			TRACE("��ȡҳ�����");
			ShowMessage("��ȡ����ҳ��ʧ��");
			m_nErrorTime ++;
			DeleteObjectExEx();
			return false;
		}
		//m_strWorkBuf.MakeLower();
		//AfxMessageBox(m_strWorkBuf.Right(m_strWorkBuf.GetLength() - 200));
		//����Ƿ�򿪼�������ҳ
		int iPos = m_strWorkBuf.Find("��������ͼ��ʾ���ĸ�����");
		if (iPos >= 0)
		{
			ShowMessage("�ɹ���ȡ����ҳ,���ڻ�ȡ��֤��...");
		}
		else
		{
			return false;
		}
		//�˴���Ҫ��ȡ��VIEWSTATEֵ
		CString strViewState = "";
		CString strValidate = "";
		CString strBefore = "id=\"__VIEWSTATE\" value=\"";
		CString strAfter = "\" />";
		CString postStr = "";
		CString strShow = "";
		strViewState = GetKeyString(m_strWorkBuf,strBefore,strAfter);
		strBefore = "id=\"__EVENTVALIDATION\" value=\"";
		strAfter = "\" />";
		strValidate = GetKeyString(m_strWorkBuf,strBefore,strAfter);

		strViewState = URLEncode(strViewState);
		strValidate = URLEncode(strValidate);
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
				/* ������֤�ַ���                                                       */
// 				/************************************************************************/
// 				iRet = GetFirstUsefulKey();
// 				if (iRet != 0)
// 				{	
// 					Sleep(1000);
// 					continue;
// 				}
// 				m_strMac.MakeUpper();
// 				ShowMessage("ģ����֤��Ϣ:"+m_strMac + "," + m_strTrans +","+m_strVerify);
				/************************************************************************/
				/* �˴��Ȼ�ȡע��ҳ�����Ϣ,�Ա����                                    */
				/************************************************************************/
				//HWND hwnd = ::FindWindow("IEFrame","������ - ע���ʺ� - Microsoft Internet Explorer");
				CString strTime = CTime::GetCurrentTime().Format("%m%d%H%M%S");
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
					objectlink.Format("/JH_UserConfig/FundCode.aspx?rnd111=61992366%s?",strTime);
					strHeaders = m_strHeader + "\r\nReferer:http://www.eku8.com/JH_UserConfig/DayPay.Aspx?UserID=4974619&PassWord=9ab50b8e55bdf45adb12de33a2279273";
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
				strHeaders = m_strHeader + "\r\nReferer:http://qipai.jl.cncmax.cn/chongzhi/Dxpay.aspx";				
				rgPack.strMail = "";
				CString strUserTag = "ctl00%24ContentPlaceHolder1%24UserName";
				CString strReUserTag = "ctl00%24ContentPlaceHolder1%24UserName2";
				CString strPwdTag = "ctl00%24ContentPlaceHolder1%24CardPwd";
				CString strCardTag = "ctl00%24ContentPlaceHolder1%24CardNum";
				CString strCheckCodeTag = "ctl00%24ContentPlaceHolder1%24txtyzm";
				CString strBtnId = "ctl00%24ContentPlaceHolder1%24ImageButton1";
				postStr.Format("__EVENTTARGET=&__EVENTARGUMENT=&__VIEWSTATE=%s&__EVENTVALIDATION=%s&%s=%s&%s=%s&%s=%s&%s=%s&%s=%s&%s.x=82&%s.y=22",
					strViewState,
					strValidate,
					strUserTag,
					rgPack.strNickName,
					strReUserTag,
					rgPack.strNickName,
					strCardTag,
					rgPack.strAcc,
					strPwdTag,
					rgPack.strPwd,
					strCheckCodeTag,
					strCheckCode,
					strBtnId,
					strBtnId
					);
				
				iRet = RequestPostPageExExEx(m_strServerURL,objectlink,strHeaders,postStr,strError,0);//
				if ( iRet != 0 )
				{
					TRACE("POST����");
					m_nErrorTime ++;
					DeleteObjectExEx();
					return false;
				}
				//m_strWorkBuf.MakeLower();				
				iPos = m_strWorkBuf.Find("ע��ɹ�");
				if ( iPos >= 0)
				{
                    ShowMessage("ע��ɹ�");
					rgPack.iStatus = 1;
					rgPack.strMemo = "�ɹ�";
					strShow.Format("%s,%s,",rgPack.strAcc,rgPack.strPwd);
					WriteLog(strShow,0);
					m_iIpSuccNum++;
					if (m_iIpSuccNum >= g_iIpRegNum)
					{
						if (g_iAotuChangeIp)
						{
							ShowMessage("��һIPע�����Ѵ�����,�����л�IP...");
						}
						ShowMessage("��һM��֤����ע�����Ѵ�����,�����л���֤����...");
						DisableKey(m_iID);
						m_iNeedChageIP = 1;
						m_iIpSuccNum = 0;
						m_iIsNeedChangeMac = 1;
						//continue;
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
					//�˴��ҵ������Ѿ��ﵽ�����ַ�������ȡ��һ��������
					if (strFailReason.Find("��ע��̫��") != -1)
					{
						DisableKey(m_iID);
					}
					m_iIpSuccNum = 0;
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
//�û���¼
int C51JobWebPost::UserRegisterEx(RegisterPack &rgPack)
{
	try
	{
		m_strServerURL = "www.eku8.com";
		CString strError = "";
		CString sendStr = "";
		CString strBefore = "";
		CString strAfter = "";
		int iPos = -1;
		CString strFailReason = "";

		ShowMessage("���ڻ�ȡ����ҳ��...");
		string strRetMD5 = "";
		char szPwd[50] = {0};				
		strcpy(szPwd,rgPack.strPwd);
		strRetMD5 = MD5Encode(szPwd);
		CString strObjectLink = "";
		strObjectLink.Format("/JH_UserConfig/DayPay.Aspx?UserID=%s&PassWord=%s",rgPack.strAcc,strRetMD5.c_str());
		strRetMD5.clear();
		CString objectlink = strObjectLink;
		CString strHeaders = m_strHeader;
		int iRet = RequestGetPageExExEx(m_strServerURL,objectlink,strHeaders,strError);
		if ( iRet != 0 )
		{
			TRACE("��ȡҳ�����");
			ShowMessage("��ȡ����ҳ��ʧ��");
			m_nErrorTime ++;
			DeleteObjectExEx();
			return -1;
		}
		g_strGotNum = "";
		ShowMessage("��ȡ����ҳ�����,��ʼ����...");
		//�˴���ȡ�Ѿ���ȡ�����Ƿ��ܼ�����ȡ����Ϣ
		if (m_strWorkBuf.Find("��ȡ�û���Ϣʧ��")>=0)
		{
			strFailReason = "��ϷID�����������";
			return ErrorInfoShow(rgPack,strFailReason);
		}
		//��ȡ����ʧ��
		if (m_strWorkBuf.Find("��ȡ����ʧ��")>=0)
		{
			strFailReason = "��ϷID��������Ƿ�";
			return ErrorInfoShow(rgPack,strFailReason);
		}
		//�˴����������˺ŷǷ���Ϣ

		g_strGotNum = "0";		//�Ѿ���ȡ��
		strBefore = "<span id=\"lb_NowTake\" class=\"font\" style=\"color:#C2273A;\">";
		strAfter = "</span>";
		g_strGotNum = GetKeyString(m_strWorkBuf,strBefore,strAfter);
		if (g_strGotNum.Trim(),g_strGotNum == "")
		{
			g_strGotNum = "0";
		}
		if (g_strGotNum == "3")	//��ʾ�Ѿ���ȡ���
		{	
			ShowMessage("���˺��Ѿ���ȡ�����λ���");
			return ErrorInfoShow(rgPack,"���˺��Ѿ���ȡ�����λ���");
		}
// 		if (m_strWorkBuf.Find("�����ڵ�������Ϸʱ��û�ﵽ��ȡ��׼") >= 0)
// 		{
// 			ShowMessage("�����ڵ�������Ϸʱ��û�ﵽ��ȡ��׼");
// 			return ErrorInfoShow(rgPack,"�����ڵ�������Ϸʱ��û�ﵽ��ȡ��׼");
// 		}
		if (m_strWorkBuf.Find("��������ͼ��ʾ���ĸ�����") >= 0)
		{
			ShowMessage("�ɹ���ȡ����ҳ,���ڻ�ȡ��֤��...");
		}
		else
		{
			ShowMessage("��������ҳ�����[δ�ҵ���ʶ]");
			return -1;
		}
		//�˴�����ͬ����ʾֵ���ж��Ƿ���Խ��л�ȡ����

		//�˴���Ҫ��ȡ��VIEWSTATEֵ
		CString strViewState = "";
		CString strValidate = "";
		strBefore = "id=\"__VIEWSTATE\" value=\"";
		strAfter = "\" />";
		CString postStr = "";
		CString strShow = "";
		strViewState = GetKeyString(m_strWorkBuf,strBefore,strAfter);
		strBefore = "id=\"__EVENTVALIDATION\" value=\"";
		strAfter = "\" />";
		strValidate = GetKeyString(m_strWorkBuf,strBefore,strAfter);
		strViewState = URLEncode(strViewState);
		strValidate = URLEncode(strValidate);		
		CString strRand = "";
		CString strTime = CTime::GetCurrentTime().Format("%m%d%H%M%S");
		rgPack.strIp = g_strCurrIP;
		m_iCheckFlag = 0;
		g_strCheckCode = "";
		int iRand = rand()%100;
		if (m_iCheckFlag == 0)
		{
			//ɾ���ɵ���֤��
			if (_access(g_strDirect+g_strFileName,0) == 0)
			{
				CFile::Remove(g_strDirect+g_strFileName);
			}
			ShowMessage("���ڻ�ȡ��֤��...");
			m_iCheckFlag = 0;
			objectlink.Format("/JH_UserConfig/FundCode.aspx?rnd111=619923%d%s?",iRand,strTime);
			strHeaders = m_strHeader + "\r\nReferer:http://www.eku8.com"+strObjectLink;
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
				strShow.Format("���ڻ�ȡ����:%s %s",rgPack.strAcc,rgPack.strPwd);
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
			return -1;
		}
		
		CString strCheckCode = g_strCheckCode;
		if (strCheckCode == "r")
		{
			DeleteObjectExEx();
			return -1;		//������
		}
		if (strCheckCode.GetLength() != 4)
		{
			DeleteObjectExEx();
			ShowMessage("��֤�볤�ȴ���,���»�ȡ...");
			return -1;
		}		
		objectlink = strObjectLink;	
		strHeaders = m_strHeader + "\r\nReferer:http://www.eku8.com"+strObjectLink;
		rgPack.strMail = "";
		CString strBtnName = "%C1%EC%C8%A1%B1%BE%B4%CE%D2%DA%BF%E1%BB%F9%BD%F0";
		postStr.Format("__EVENTTARGET=&__EVENTARGUMENT=&__VIEWSTATE=%s&__EVENTVALIDATION=%s&txt_Code=%s&bt_submit=%s",
			strViewState,
			strValidate,
			strCheckCode,
			strBtnName);
		iRet = RequestPostPageExExEx(m_strServerURL,objectlink,strHeaders,postStr,strError,0);//
		if ( iRet != 0 )
		{
			TRACE("POST����");
			m_nErrorTime ++;
			DeleteObjectExEx();
			return -1;
		}
		
		iPos = m_strWorkBuf.Find("��ϲ");
		if ( iPos >= 0)
		{
			ShowMessage("��ֵ�ɹ�");
			rgPack.iStatus = 1;
			rgPack.strMemo = "�ɹ�";
			strShow.Format("%s,%s,%s",rgPack.strAcc,rgPack.strPwd,rgPack.strNickName);
			WriteLog(strShow,0);			
		}
		else
		{//��ֵʧ��						
			strBefore = "<span id=\"lb_prompt\" class=\"font\" style=\"color:#C2273A;\">";
			strAfter = "</span>";
			strError = GetKeyString(m_strWorkBuf,strBefore,strAfter);
			strFailReason = GetKeyString(m_strWorkBuf,strBefore,strAfter);
			//�õ��Ѿ���ȡ�Ĵ���
			g_strGotNum = "-";		//�Ѿ���ȡ��
			strBefore = "<span id=\"lb_NowTake\" class=\"font\" style=\"color:#C2273A;\">";
			strAfter = "</span>";
			g_strGotNum = GetKeyString(m_strWorkBuf,strBefore,strAfter);
			DeleteObjectExEx();
			iRet =  ErrorInfoShow(rgPack,strFailReason);
			return iRet;
		}
		DeleteObjectExEx();
		return 0;
	}
	catch (...)
	{
	}
	DeleteObjectExEx();
	return -1;            
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
	g_strDirect = "c:\\";
	g_strFileName = "checkcodeold.bmp";
	CString strOldAcc = "";
	m_iNeedChageIP = 0;
	m_iIpSuccNum = 0;
	CString sendStr = "";
	CString strError = "";
	ShowMessage("��ʼ���ɹ�");
	srand(time(NULL));
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
		//�ж�ˢ��ʱ���Ƿ���
		iRet = UserRegisterEx(rgPack);
		switch (iRet)
		{
		case  -2:			
			DeleteRegisterPack(rgPack);
			AddToShowList(rgPack);
			sendStr.Format("%s,%s,%s -- %s",rgPack.strAcc,rgPack.strPwd,rgPack.strNickName,rgPack.strMemo);
			WriteLog(sendStr,2);
			break;
		case -1:
			break;
		case -3:
			DeleteRegisterPack(rgPack);
			break;
		case 0:
			DeleteRegisterPack(rgPack);
			//AddRegisterPack(rgPack);
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
int C51JobWebPost::GetFirstUsefulKey()
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
			if ( !adoConn.ConnectAccess(strAppPath+"machineInfo.mdb",""))
			{
				ShowMessage("�������ݿ�ʧ��");
				return -1; 
			}
			adoRst.SetAdoConnection(&adoConn);
			adoRst.SetCursorLocation(adUseClient);
		}
		CString strSQL;		
		strSQL.Format("select top 1 * from [machine] where datediff('d',lastusetime,now()) > 0 order by [ID] asc");
		if(!adoRst.Open(strSQL,adCmdText))
		{
			iRet = -1;
			ShowMessage("��ȡ��ֵ��Ϣʧ��");
		}
		else
		{
			if (!adoRst.IsEOF())
			{
				adoRst.GetCollect("ID",m_iID);
				adoRst.GetCollect("machineserial",m_strMac);
				adoRst.GetCollect("transactionid",m_strTrans);
				adoRst.GetCollect("verify",m_strVerify);
				iRet = 0;
			}
			else
			{
				ShowMessage("�����Ѿ�û�п��õļ�ֵ��Ϣ����������ɳ���");
				iRet = -1;
			}
		}
		adoRst.Close();
		adoConn.Close();
		return iRet;
	}
	catch (...)
	{
		adoRst.Close();
		adoConn.Close();
		ShowMessage("��ȡ��ֵ��Ϣ�쳣");
	}
	return -1;
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
int C51JobWebPost::DisableKey(int iKeyNum)
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
			if ( !adoConn.ConnectAccess(strAppPath+"machineInfo.mdb",""))
			{
				ShowMessage("�������ݿ�ʧ��");
				return -1; 
			}
			adoRst.SetAdoConnection(&adoConn);
			adoRst.SetCursorLocation(adUseClient);
		}
		CString strSQL;		
		strSQL.Format("update [machine] set [lastusetime]=now() where [ID]=%d",iKeyNum);
		if(!adoRst.Open(strSQL,adCmdText))
		{
			iRet = -1;
			ShowMessage("ɾ��������Ϣʧ��");
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
		ShowMessage("ɾ��������Ϣ�쳣");
	}
	return -1;
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void C51JobWebPost::WriteBufLog(CString strPath,CString strData)
{
	CFile file;
	file.Open(strPath,CFile::modeCreate|CFile::modeWrite);
	file.Write(strData,strData.GetLength());
	file.Close();
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
int C51JobWebPost::ErrorInfoShow(RegisterPack &rgPack,CString strFailReason)
{
	ShowMessage("��ȡʧ��:	"+strFailReason);
	rgPack.iStatus = 2;
	rgPack.strMemo = strFailReason;
	rgPack.strIp = g_strGotNum;		//��ʾ�Ѿ���ȡ�Ĵ��� 
	if (rgPack.strIp == "")
	{
		rgPack.strIp = "-";
	}	
	//��ϷID��������Ƿ�
	if (strFailReason.Find("��ϷID��������Ƿ�")>=0)
	{
		return -2;
	}
	//��ϷID�����������
	if (strFailReason.Find("��ϷID�����������")>=0)
	{
		return -2;
	}
	if (strFailReason.Find("���˺��Ѿ���ȡ�����λ���")>=0)
	{
		return -2;
	}
	if (strFailReason.Find("�����ڵ�������Ϸʱ��û�ﵽ��ȡ��׼")>=0)
	{
		return -2;
	}
	//����δ�ﵽ��ȡ�ڿ���������
	if (strFailReason.Find("����δ�ﵽ��ȡ�ڿ���������")>=0)
	{
		return -2;
	}
	//��֤�����벻��ȷ
	if (strFailReason.Find("��֤�����벻��ȷ")>=0)
	{
		return -1;
	}
	//�������ĸ�������֤��
	if (strFailReason.Find("�������ĸ�������֤��")>=0)
	{
		return -1;
	}
	if (strFailReason.Find("�û���������")>=0)
	{
		return -2;
	}
	if (strFailReason.Find("���Ų�����")>=0)
	{
		return -2;
	}
	if (strFailReason.Find("����")>=0)
	{
		return -2;
	}
	//���ʺ�û�н��й���Ϸ
	if (strFailReason.Find("û�н��й���Ϸ")>=0)
	{
		return -2;
	}			
	return -2;
}
#include "StdAfx.h"
#include ".\51jobwebpost.h"
#include "UserMessagePump.h"
#include <io.h>
#include <direct.h>
#include "UserFile.h"

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
		if (g_AccList.GetItemText(i,1) == rgPack.strAcc&&g_AccList.GetItemText(i,5) != "��")
		{
			isExist = true;
			g_AccList.SetItemText(i,5,strStauts);
			g_AccList.SetItemText(i,6,rgPack.strMemo);
			g_AccList.SetItemText(i,8,rgPack.strIp);
			break;
		}		
	}
	if (!isExist)
	{
		CString listIndex = "";
		listIndex.Format("%d",nCount);
		g_AccList.InsertItem(nCount,"",0);
		g_AccList.SetItemText(nCount,0,listIndex);
		g_AccList.SetItemText(nCount,1,rgPack.strAcc);
		g_AccList.SetItemText(nCount,2,rgPack.strPwd);
		g_AccList.SetItemText(nCount,3,rgPack.strMail);
		g_AccList.SetItemText(nCount,4,rgPack.strIntroMan);
		g_AccList.SetItemText(nCount,5,strStauts);
		g_AccList.SetItemText(nCount,6,rgPack.strMemo);
		g_AccList.SetItemText(nCount,7,rgPack.strNickName);
		g_AccList.SetItemText(nCount,8,rgPack.strIp);
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
	m_strServerURL = "www.sdo.com";
	m_TimeInterval = 3;
	m_strHeader = "Content-Type: application/x-www-form-urlencoded";
	m_strHeader += "\r\nAccept:image/gif, image/x-xbitmap, image/jpeg, image/pjpeg, application/x-shockwave-flash, application/vnd.ms-excel, application/vnd.ms-powerpoint, application/msword, application/x-silverlight, */*";
	//m_strHeader += "\r\nAccept:*/*";
	m_strHeader += "\r\nAccept-Language:zh-cn";
	m_strHeader += "\r\nAccept-Encoding:gzip, deflate";//
	//m_strHeader += "\r\nCache-Control:no-cache";
	m_strHeader += "\r\nUser-Agent:Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1; SV1; Foxy/1; Foxy/1; .NET CLR 1.1.4322; .NET CLR 2.0.50727; .NET CLR 3.0.04506.648; .NET CLR 3.5.21022)";
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
bool C51JobWebPost::UserRegister(RegisterPack &rgPack)
{
	CString g_strUserId = rgPack.strAcc;			//����
	CString g_strUserpwd = rgPack.strPwd;			//�˺�
	CString g_strPhone = rgPack.strPhone;		//��֤�ֻ�
	CString g_strReCommendMobile = rgPack.strIntroMan;	//�Ƽ����ֻ�
	rgPack.strMemo = "[��ʼ��¼]";
	try
	{
		CString strError = "";
		CString sendStr = "";
		CString strOjbStrings = "";
		ShowMessage("���ڻ�ȡ��¼ҳ��...");
		CString objectlink = "/loginPT.asp";
		CString strHeaders = m_strHeader;
		CString postStr = "";
		CString strShow = "";
		int iRet = -1;
		m_strServerURL = "www.sdo.com";
		m_iPostFlag = 0;
 		iRet = RequestGetPageExExEx(m_strServerURL,objectlink,strHeaders,
			strError);//"148.233.239.24",3128,"","",0);
		CString strCookies = m_strSetCookie;
		if ( iRet != 0 )
		{
			TRACE("��ȡҳ�����");
			ShowMessage("��ȡ��¼ҳ��ʧ��");
			rgPack.iStatus = 0;
			DeleteObjectExEx();
			return false;
		}		
		int iPos = m_strWorkBuf.Find("location.href = \"http://www.sdo.com");
		if (iPos < 0)
		{
			ShowMessage("���Ի�ȡ��¼ҳʧ��!");
			rgPack.iStatus = 0;
			return false;
		}
		CString strBefore11 = "location.href = \"http://www.sdo.com";
		CString strAfter11 = "\";";
		objectlink = GetKeyString(m_strWorkBuf,strBefore11,strAfter11);
		iRet = RequestGetPageExExEx(m_strServerURL,objectlink,strHeaders,strError,0);
		strCookies = m_strSetCookie;
		if (iRet != 0)
		{
			ShowMessage("��ȡ������Ϣʧ��");
			m_nErrorTime ++;
			DeleteObjectExEx();
			rgPack.iStatus = 0;
			return false;
		}
		CString strLt = "";
		CString strChallenge = "";
		//CString strError = "";
		strBefore11 = "<input type=\"hidden\" name=\"lt\" value=\"";
		strAfter11 = "\" />";
		strLt = GetKeyString(m_strWorkBuf,strBefore11,strAfter11);
		strBefore11 = "name=\"challenge\" value=\"";
		strAfter11 = "\" />";
		strChallenge = GetKeyString(m_strWorkBuf,strBefore11,strAfter11);
		if (strChallenge == "" || strLt == "")
		{
			ShowMessage("���Ի�ȡ��¼ҳʧ��!");
			rgPack.iStatus = 0;
			DeleteObjectExEx();
			return false;
		}
		CString strTemp = "";
		strTemp = "http://"+m_strServerURL + objectlink;
		strHeaders += "\r\nReferer:"+strTemp;
		strLt.Replace(":","%3A");
		postStr.Format("lt=%s&service=&challenge=%s&cardno=&ekeytype=&request_QueryString=&ecardpos=&err=8",strLt,strChallenge);
		objectlink = "/loginPT.asp";
		iRet = RequestPostPageExExEx(m_strServerURL,objectlink,strHeaders,postStr,strError,0);
		strCookies = m_strSetCookie;
		strBefore11 = "action=\"https://cas.sdo.com/CAS/Login.Validate.Account?";
		strAfter11 = "\">";
		objectlink = GetKeyString(m_strWorkBuf,strBefore11,strAfter11);
		strBefore11 = "name=\"lt\" value=\"";
		strAfter11 = "\" />";
		URL_Encode(objectlink);
		objectlink = "/CAS/Login.Validate.Account?service=http://www.sdo.com/center/index.asp?cas=1";
		strLt = GetKeyString(m_strWorkBuf,strBefore11,strAfter11);
		//strLt.Replace(":","%3A");
		postStr.Format("warn=false&lt=%s&_eventId=submit&idtype=0&gamearea=0&gametype=0&username=%s&password=%s&ekey=&challenge=",strLt,g_strUserId
			,g_strUserpwd);
		ShowMessage("��ȡ��¼ҳ�ɹ�,���ڵ�¼...");
		DeleteObjectExEx();
		strHeaders = m_strHeader;
		strHeaders += "\r\nReferer:http://www.sdo.com/loginPT.asp";
		m_strServerURL = "cas.sdo.com";
		SetHttpsType(1);		
		iRet = RequestPostPageExExEx(m_strServerURL,objectlink,strHeaders,postStr,strError,0);
		SetHttpsType(0);
		strHeaders = m_strHeader;
		strBefore11 = "location.href	=	\"http://www.sdo.com";
		strAfter11 = "\";";
		objectlink = GetKeyString(m_strWorkBuf,strBefore11,strAfter11);
		m_strServerURL = "www.sdo.com";
		DeleteObjectExEx();
		iRet = RequestGetPageExExEx(m_strServerURL,objectlink,strHeaders,strError);
		objectlink = "/Partner/MobileDeposit.aspx";
		if (m_strWorkBuf.Find("�ҵĸ�����Ϣ") > 0)
		{
			ShowMessage("��¼�ɹ�,������ת������ҳ...");
		}
		else
		{
			ShowMessage("��¼ʧ��,�������˺�");
			rgPack.strMemo += ",[��¼ʧ��,�������˺�]";
			rgPack.iStatus = 2;
			return false;
		}
		DeleteObjectExEx();
		m_strServerURL = "jf.sdo.com";
		strHeaders = m_strHeader;
		strHeaders += "\r\nReferer:http://jf.sdo.com/";		
		objectlink = "/LoginFrame.aspx";
		iRet = RequestGetPageExExEx(m_strServerURL,objectlink,strHeaders,strError);
		strBefore11 = "var path    =   \"http://jf.sdo.com";
		strAfter11 = "\";";
		objectlink = GetKeyString(m_strWorkBuf,strBefore11,strAfter11);
		iRet = RequestGetPageExExEx(m_strServerURL,objectlink,strHeaders,strError);
		if (m_strWorkBuf.Find("���Ļ������") > 0)
		{
			ShowMessage("�ɹ���ת������ҳ,���ڼ����ֻ���֤ҳ...");
		}
		objectlink = "/Partner/MobileDeposit.aspx";
    	iRet = RequestGetPageExExEx(m_strServerURL,objectlink,strHeaders,strError);
		CString strViewState = "";
		CString strValidDate = "";
		if (m_strWorkBuf.Find("����ʢ��ͨ��֤�˺�") > 0)
		{
			ShowMessage("�����ֻ���֤ҳ�ɹ�����ʼ�����ֻ���֤");
		}
		else if (m_strWorkBuf.Find("���ѽ��й��ֻ���֤") > 0)
		{
			ShowMessage("�Ѿ�ͨ���ֻ���֤����ת�������޸�ҳ");
			rgPack.strMemo += ",[�Ѿ�ͨ���ֻ���֤����ת�������޸�ҳ]";
			goto GETUSERDATAPAGE;
		}
		else
		{
			ShowMessage("�����ֻ���֤ҳʧ�ܣ���ת�������޸�ҳ");
			rgPack.strMemo += ",[�����ֻ���֤ҳʧ��]";
			goto GETUSERDATAPAGE;
		}		
		strBefore11 = "<input type=\"hidden\" name=\"__VIEWSTATE\" id=\"__VIEWSTATE\" value=\"";
		strAfter11 = "\" />";
		strViewState = GetKeyString(m_strWorkBuf,strBefore11,strAfter11);
		strBefore11 = "name=\"__EVENTVALIDATION\" id=\"__EVENTVALIDATION\" value=\"";
		strAfter11 = "\" />";
		strValidDate = GetKeyString(m_strWorkBuf,strBefore11,strAfter11);
		objectlink = "/csharpwrapper/Score.Web.AjaxMethod,JF.sdo.com.ashx?_method=SendMobileSms&_session=rw";
		strHeaders = m_strHeader;
		strHeaders += "\r\nReferer:http://jf.sdo.com/Partner/MobileDeposit.aspx";		
		postStr.Format("mobile=%s\nuserId=%s\nrecmobile=\n",g_strPhone,g_strUserId);
		m_iPostFlag = 1;
		iRet = RequestPostPageExExEx(m_strServerURL,objectlink,strHeaders,postStr,strError,0);
		m_iPostFlag = 0;
		if (m_strWorkBuf.Trim() == "0")
		{
			ShowMessage("��֤��Ϣ�ѷ���,����ղ���������,�����ֻ���֤�������ַ�'R'������ť");
			g_strCheckCode = "";
			while(!g_ExitFlag)
			{
				if (g_strCheckCode != "")
				{
					break;
				}
				Sleep(100);
			}
			if (g_ExitFlag)		//�����˳�
			{
				DeleteObjectExEx();
				rgPack.iStatus = 0;
				return false;
			}
			if (g_strCheckCode.MakeLower() == "r")
			{
				ShowMessage("�ֶ������ֻ���֤");
				rgPack.strMemo += ",[�ֶ������ֻ���֤]";
			}
			else
			{
				ShowMessage("�ֻ���֤���Ѿ�����,������֤...");
				//׼��POST������
				URL_Encode(strViewState);
				URL_Encode(strValidDate);
				CString strPostPre = "__EVENTTARGET=&__EVENTARGUMENT=&__VIEWSTATE=%s&__EVENTVALIDATION=%s&_ctl0:ContentPlaceHolder1:mobileDeposit:TB_UserId=%s&_ctl0:ContentPlaceHolder1:mobileDeposit:TB_Mobile=%s&_ctl0:ContentPlaceHolder1:mobileDeposit:TB_SmsCode=%s&_ctl0:ContentPlaceHolder1:mobileDeposit:TB_ReCommendMobile=%s&_ctl0:ContentPlaceHolder1:mobileDeposit:btnSubmit.x=23&_ctl0:ContentPlaceHolder1:mobileDeposit:btnSubmit.y=9";
				objectlink = "/Partner/MobileDeposit.aspx";				
				postStr.Format(strPostPre,strViewState,strValidDate,g_strUserId,g_strPhone,g_strCheckCode,g_strReCommendMobile);
				postStr.Replace(":","%3A");
				//URL_Encode(postStr);
				strHeaders = m_strHeader;
				strHeaders += "\r\nReferer:http://jf.sdo.com/Partner/MobileDeposit.aspx";
				iRet = RequestPostPageExExEx(m_strServerURL,objectlink,strHeaders,postStr,strError,0);				
				if(iRet < 0)
				{
					ShowMessage("�����ֻ���֤��Ϣ���������ֻ���֤");	
					rgPack.strMemo += ",[�����ֻ���֤��Ϣ���������ֻ���֤]";
				}
				else
				{
					if (m_strWorkBuf.Find("��֤�ֻ��ɹ�") > 0)
					{
						ShowMessage("��֤�ֻ��ɹ�");
						rgPack.strMemo += ",[��֤�ֻ��ɹ�]";
					}
					else
					{
						strBefore11 = "id=\"_ctl0_ContentPlaceHolder1_mobileDeposit_lblCheckMobileMsg\" style=\"color:Red;\">";
						strAfter11 = "</span></td>";
						CString strErrorInfo = "";
						strErrorInfo.Format("[%s]",GetKeyString(m_strWorkBuf,strBefore11,strAfter11));
						ShowMessage(strErrorInfo);
						ShowMessage("�����ֻ���֤,����������д");
						rgPack.strMemo += ",[" + strErrorInfo+"]";
					}
				}
			}
		}
		else
		{
			if (m_strWorkBuf.GetLength() > 10)
			{
				m_strWorkBuf = m_strWorkBuf.Left(10);
			}
			m_strWorkBuf.Trim();
			switch (atoi(m_strWorkBuf))
			{
			case -1:
				m_strWorkBuf = "('�����ֻ���ʽ����')";
				break;
			case -2:
				m_strWorkBuf = "('�Ƽ����ֻ���ʽ����')";
				break;
			case -5:
				m_strWorkBuf = "('�Ƽ��˲������Լ�')";
				break;
			case -3:
				m_strWorkBuf = "('���ѳ�����һСʱ�ڷ���������֤�������')";
				break;
			case -35001:
				m_strWorkBuf = "('���ֻ��Ż��û����͹�����')";
				break;
			case -35006:
				m_strWorkBuf = "('���Ƽ�����ͨ���Ƽ���ʽ�͹�����')";
				break;
			case -35008:
				m_strWorkBuf = "('���Ƽ����ֻ�δ����֤')";
				break;
			default:
				m_strWorkBuf = "('ϵͳ�쳣�����Ժ�����')";
					break;
			}
			ShowMessage(m_strWorkBuf);
			rgPack.strMemo += ",[" + m_strWorkBuf + "]";
			ShowMessage("�����ֻ���֤,����������д");
		}
		
		//����������������д
GETUSERDATAPAGE:
		if (g_ExitFlag)		//�����˳�
		{
			DeleteObjectExEx();
			rgPack.iStatus = 0;
			return false;
		}
		ShowMessage("���ڻ�ȡ�����޸�ҳ...");
		objectlink = "/Partner/UpdateUserInfoDeposit.aspx";
		strHeaders = m_strHeader;
		//strHeaders += "\r\nReferer:http://jf.sdo.com/Partner/MobileDeposit.aspx";		
		//postStr.Format("mobile=%s\nuserId=%s\nrecmobile=","15219212360","junbrave22");
		iRet = RequestGetPageExExEx(m_strServerURL,objectlink,strHeaders,strError);
		if (m_strWorkBuf.Find("��������������") > 0)
		{
			ShowMessage("�ֻ���֤δͨ��,���������޸�");
			rgPack.strMemo += ",[�ֻ���֤δͨ��,���������޸�]";
		}
		else if (m_strWorkBuf.Find("�����˸�����Ϣ") > 0)
		{
			ShowMessage("�Ѿ���ӹ�����,���������޸�");
			rgPack.strMemo += ",[�Ѿ���ӹ�����]";
		}
		else if (m_strWorkBuf.Find("��ϲ������֤�ֻ��ɹ�") > 0)	//��������ҳ��ɹ�����ʼ��д����
		{
			//�˴����������޸�
			strHeaders += "\r\nReferer:http://jf.sdo.com/Partner/UpdateUserInfoDeposit.aspx";
			ShowMessage("���������޸�ҳ�ɹ�,�����ύ����...");
			CString strPhone = "021-50504740";
			CString strEmail = rgPack.strMail;
			CString strUserName = "";
			GenUserName(strUserName);
			CString strIdentity = rgPack.strIdentity;
			CString strProvince = "";
			CString strCity = "";
			CString strMailCode = "";
			CString strAddress = "";
			strBefore11 = "name=\"__VIEWSTATE\" id=\"__VIEWSTATE\" value=\"";
			strAfter11 = "\" />";
			strViewState = GetKeyString(m_strWorkBuf,strBefore11,strAfter11);
			strBefore11 = "name=\"__EVENTVALIDATION\" id=\"__EVENTVALIDATION\" value=\"";
			strAfter11 = "\" />";
			strValidDate = GetKeyString(m_strWorkBuf,strBefore11,strAfter11);			
			GenAddress(strProvince,strAddress,strMailCode,strCity);
			strViewState.Trim();
			strValidDate.Trim();
			strViewState = URLEncode(strViewState);
			strValidDate = URLEncode(strValidDate);
			strProvince = URLEncode(strProvince);
			strUserName = URLEncode(strUserName);
			strCity = URLEncode(strCity);
			strAddress = URLEncode(strAddress);
			CString strPostPre = "__EVENTTARGET=&__EVENTARGUMENT=&__VIEWSTATE=%s&__EVENTVALIDATION=%s&_ctl0:ContentPlaceHolder1:userDeposit:TB_UserName=%s&_ctl0:ContentPlaceHolder1:userDeposit:TB_UserId=%s&_ctl0:ContentPlaceHolder1:userDeposit:TB_FromIndex=fromMall&_ctl0:ContentPlaceHolder1:userDeposit:TB_IdentityCard=%s&_ctl0:ContentPlaceHolder1:userDeposit:TB_Mobile=%s&_ctl0:ContentPlaceHolder1:userDeposit:TB_Telephone=%s&_ctl0:ContentPlaceHolder1:userDeposit:TB_LinkEmail=%s&_ctl0:ContentPlaceHolder1:userDeposit:DDL_Province=%s&_ctl0:ContentPlaceHolder1:userDeposit:TB_City=%s&_ctl0:ContentPlaceHolder1:userDeposit:TB_Address=%s&_ctl0:ContentPlaceHolder1:userDeposit:TB_Zip=%s&_ctl0:ContentPlaceHolder1:userDeposit:btnConfirm.x=15&_ctl0:ContentPlaceHolder1:userDeposit:btnConfirm.y=5";
			postStr.Format(strPostPre,strViewState,strValidDate,
				strUserName,g_strUserId,strIdentity,
				g_strPhone,	strPhone,strEmail,
				strProvince,strCity,strAddress,
				strMailCode);			
			postStr.Replace(":","%3A");
			if (g_ExitFlag)		//�����˳�
			{
				DeleteObjectExEx();
				rgPack.iStatus = 0;
				return false;
			}
			iRet = RequestPostPageExExEx(m_strServerURL,objectlink,strHeaders,postStr,strError,0);
			strBefore11 = "name=\"__VIEWSTATE\" id=\"__VIEWSTATE\" value=\"";
			strAfter11 = "\" />";
			strViewState = GetKeyString(m_strWorkBuf,strBefore11,strAfter11);
			strBefore11 = "name=\"__EVENTVALIDATION\" id=\"__EVENTVALIDATION\" value=\"";
			strAfter11 = "\" />";
			strValidDate = GetKeyString(m_strWorkBuf,strBefore11,strAfter11);
			strViewState = URLEncode(strViewState);
			strValidDate = URLEncode(strValidDate);
			strPostPre = "__EVENTTARGET=&__EVENTARGUMENT=&__VIEWSTATE=%s&__EVENTVALIDATION=%s&_ctl0%3AContentPlaceHolder1%3AuserDeposit%3AbtnSubmit.x=56&_ctl0%3AContentPlaceHolder1%3AuserDeposit%3AbtnSubmit.y=11";
			strPostPre.Replace("%3A",":");
			postStr.Format(strPostPre,strViewState,strValidDate);
			postStr.Replace(":","%3A");
			if (g_ExitFlag)		//�����˳�
			{
				DeleteObjectExEx();
				rgPack.iStatus = 0;
				return false;
			}
			iRet = RequestPostPageExExEx(m_strServerURL,objectlink,strHeaders,postStr,strError,0);
			if (m_strWorkBuf.Find("���Ƹ������ϳɹ�") > 0)
			{
				ShowMessage("���Ƹ������ϳɹ�,��ʼ���������ƹ�Աҳ��...");
				rgPack.strMemo += ",[���Ƹ������ϳɹ�]";
			}
			else
			{
				ShowMessage("���Ƹ�������ʧ��,�����޸�����ҳ,��ʼ���������ƹ�Աҳ��...");
				rgPack.strMemo += ",[���Ƹ�������ʧ��]";
			}
			//��Ҫ�Ĳ����Ѿ�ȫ������
		}
		else	//���������޸�ҳʧ��
		{
			ShowMessage("���������޸�ҳʧ��,���������޸�");	
		}
		if (g_ExitFlag)		//�����˳�
		{
			DeleteObjectExEx();
			rgPack.iStatus = 0;
			return false;
		}
		//�����ƹ�Ա
		m_iCodeErrorNum = 0;
		ShowMessage("���ڻ�ȡ�����ƹ�Աҳ��...");
		objectlink = "/Activity/FenhongLevelUp.aspx";
		strHeaders = m_strHeader;
		strHeaders += "\r\nReferer:http://jf.sdo.com/FreeScoreIndex.aspx";
		iRet = RequestGetPageExExEx(m_strServerURL,objectlink,strHeaders,strError);
		CFile file1;
		file1.Open("c:\\44444.txt",CFile::modeCreate|CFile::modeWrite);
		file1.Write(m_strWorkBuf,m_strWorkBuf.GetLength());
		file1.Close();
		
		if (m_strWorkBuf.Find("���Ѿ���ʢ���ƹ�Ա") > 0)
		{
			ShowMessage("���˺��Ѿ����ƹ�Ա,�˳��˺�...");
			//�˳��˺�
			rgPack.strMemo += ",[�Ѿ����ƹ�Ա]";
			rgPack.iStatus = 1;
			DeleteObjectExEx();
			return false;
		}
		else if (m_strWorkBuf.Find("��������֤�룺") > 0)
		{
			strBefore11 = "name=\"__VIEWSTATE\" id=\"__VIEWSTATE\" value=\"";
			strAfter11 = "\" />";
			strViewState = GetKeyString(m_strWorkBuf,strBefore11,strAfter11);
			strBefore11 = "name=\"__EVENTVALIDATION\" id=\"__EVENTVALIDATION\" value=\"";
			strAfter11 = "\" />";
			strValidDate = GetKeyString(m_strWorkBuf,strBefore11,strAfter11);
			ShowMessage("��ȡ�����ƹ�Աҳ��ɹ�,���ڼ�����֤��...");
			while(!g_ExitFlag)
			{
				//ɾ���ɵ���֤��
				if (_access(g_strDirect+g_strFileName,0) == 0)
				{
					CFile::Remove(g_strDirect+g_strFileName);
				}
				ShowMessage("���ڻ�ȡ��֤��...");
				g_strCheckCode = "";
				strHeaders = m_strHeader;
				strHeaders += "\r\nReferer:http://jf.sdo.com/Activity/FenhongLevelUp.aspx";
				objectlink.Format("/Activity/newvalidatecode1.aspx?0.410849410%s",CTime::GetCurrentTime().Format("%H%M%S"));
				if (g_ExitFlag)		//�����˳�
				{
					DeleteObjectExEx();
					rgPack.iStatus = 0;
					return false;
				}
				iRet = RequestGetPageExExEx(m_strServerURL,objectlink,strHeaders,strError,1);
				if (iRet != 0)
				{
					ShowMessage("��ȡ��֤��ʧ��,����");
					rgPack.strMemo += ",[��ȡ��֤��ʧ��]";
					return false;
				}
				else
				{
					ShowMessage("��ȡ��֤��ɹ�");
					ShowMessage("��������֤��");
					while(!g_ExitFlag)
					{
						if (g_strCheckCode != "")
						{
							break;
						}
						Sleep(100);
					}
					if (g_ExitFlag)		//�����˳�
					{
						DeleteObjectExEx();
						rgPack.iStatus = 0;
						return false;
					}
					CString strFailReason = "";
					CString strCheckCode = g_strCheckCode;
					if (strCheckCode == "r")
					{
						continue;
					}										
					strViewState.Trim();
					strValidDate.Trim();
					strViewState = URLEncode(strViewState);
					strValidDate = URLEncode(strValidDate);
					postStr.Format("__EVENTTARGET=&__EVENTARGUMENT=&__VIEWSTATE=%s&__EVENTVALIDATION=%s&ValidTxt=%s",strViewState,strValidDate,strCheckCode);
					postStr += "&btnLevelUp=%CE%D2%D2%D1%D4%C4%B6%C1%B2%A2%CD%AC%D2%E2%C9%CF%CA%F6%D0%AD%D2%E9%A3%AC%C8%B7%C8%CF%C9%FD%BC%B6%CE%AA%CA%A2%B4%F3%CD%C6%B9%E3%D4%B1";
					objectlink = "/Activity/FenhongLevelUp.aspx";
					if (g_ExitFlag)		//�����˳�
					{
						DeleteObjectExEx();
						rgPack.iStatus = 0;
						return false;
					}
					iRet = RequestPostPageExExEx(m_strServerURL,objectlink,strHeaders,postStr,strError,0);//
					CFile file;
					file.Open("c:\\test9999.txt",CFile::modeCreate|CFile::modeWrite);
					file.Write(m_strWorkBuf,m_strWorkBuf.GetLength());
					file.Close();
					if (iRet != 0)
					{
						ShowMessage("������֤������ʧ��,����");
						DeleteObjectExEx();
						rgPack.iStatus = 2;
						rgPack.strMemo += ",[������֤������ʧ��]";
						return false;
					}
					if (m_strWorkBuf.Find("���ɹ�����Ϊʢ���ƹ�Ա") > 0)
					{
						ShowMessage("�����ƹ�Ա�ɹ�");
						rgPack.iStatus = 1;
						rgPack.strMemo += ",[�����ƹ�Ա�ɹ�]";
					}
					else
					{//ע��ʧ��
						//<script type='text/javascript'>alert('������֤�����');
						strBefore11 = "<script type='text/javascript'>alert('";
						strAfter11 = "');";
						strFailReason = GetKeyString(m_strWorkBuf,strBefore11,strAfter11);
						ShowMessage("�����ƹ�Աʧ��:	"+strFailReason);
						rgPack.iStatus = 2;
						rgPack.strMemo += ","+strFailReason;
					}
					if (g_iDeleteFlag == 1)
					{
						if (strFailReason.Find("������֤�����")>0)
						{
							m_iCodeErrorNum ++;
							if (m_iCodeErrorNum >= g_iCodeErrorNum)
							{
								ShowMessage("��֤����������������,�������˺�");
								m_iCodeErrorNum = 0;
								break;
							}
						}
						else
						{
							DeleteObjectExEx();
							return false;
						}
					}
					else
					{
						DeleteObjectExEx();
						return false;
					}
				}
			}
		}
		else
		{
			ShowMessage("��ȡ�����ƹ�Աҳ��ʧ��,�˳����˺�...");
			rgPack.strMemo += ",[��ȡ�����ƹ�Աҳ��ʧ��]";
		}
		//�˳��˺�
		DeleteObjectExEx();
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
        pParam->RegisterFunc();
		pParam->ShowMessage("�߳��˳�");
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
		m_iIpSuccNum = 0;
		m_iNeedChageIP = 1;
		int iErrorCount = 0;
		int iSameIpCount = 0;
		while (m_iNeedChageIP && !g_ExitFlag && g_iAotuChangeIp == 1)	//�����Ҫ��IP
		{
			m_iNeedChageIP = 1;
			g_RasMgr.StopRas(sendStr);
			ShowMessage(sendStr);
			Sleep(300);
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
		m_iNeedChageIP = 0;
		if (!g_ExitFlag)
		{
			SetHttpsType(0);
			UserRegister(rgPack);
			switch (rgPack.iStatus)
			{
			case 0:			//ѭ��
				break;
			case 1:			//ʧ��
				AddToShowList(rgPack);
				DeleteRegisterPack(rgPack);
				break;
			case 2:			//�ɹ�
				AddToShowList(rgPack);
				DeleteRegisterPack(rgPack);
				break;
			default:
				break;
			}
			//�˳���¼
			CString strError = "";
			CString objectlink = "/jfindex.aspx?logout=true";
			CString strHeaders = m_strHeader;
			CString postStr = "";
			CString strShow = "";
			int iRet = -1;
			m_iPostFlag = 0;
			DeleteObjectExEx();
			iRet = RequestGetPageExExEx(m_strServerURL,objectlink,strHeaders,strError);
			DeleteObjectExEx();
			m_strServerURL = "cas.sdo.com:8443";
			SetHttpsType(1);
			objectlink = "/cas/logout?url=http://jf.sdo.com";
			iRet = RequestGetPageExExEx(m_strServerURL,objectlink,strHeaders,strError);
			DeleteObjectExEx();
			m_strServerURL = "www.sdo.com";
			SetHttpsType(0);
			objectlink = "/logout_iframe.asp?url=http%3a%2f%2fjf.sdo.com";
			iRet = RequestGetPageExExEx(m_strServerURL,objectlink,strHeaders,strError);
			DeleteObjectExEx();
			//������ֵ,�����Ϣ��ʾ
			strShow.Format("%s,%s,",rgPack.strAcc,rgPack.strPwd,rgPack.strNickName,rgPack.strIntroMan);
			WriteLog(strShow,0);
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
	}
	return -1;
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
int C51JobWebPost::GetQQCheckCodeBMP(CString strUrl)
{
	try
	{
		CString strTemp = "";
		char cBuffer[1024] = {0};
		CStdioFile *pFile = m_Session.OpenURL(strUrl);
		int iReadLen = 0;
		if (pFile != NULL)
		{
			CStdioFile out;
			out.Open(g_strDirect+g_strFileName, CFile::modeCreate | CFile::modeWrite |CFile::typeBinary);
			while(true)
			{
				iReadLen = pFile->Read( cBuffer ,1 );
				if ( iReadLen <= 0)
				{
					break;
				}
				out.Write(cBuffer,1); 
			}
			out.Close();
			pFile->Close();
			delete pFile;
			pFile = NULL;
			return 0;
		}
	}
	catch (CInternetException* e)
	{
		int i = 0;
	}
	
	return -1;
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
int C51JobWebPost::StringSplit(CString srcStr,CString strArray[],char splitChar)
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
void C51JobWebPost::GenUserName(CString &strName)
{
	CString strFamilyName[78] = {"л","��","��","��","��","��","��","��","��","��","Ф","��","��","Ԭ","��","��","��","��","��","��","Ҷ","��","��","κ","��","��","��","��","Ҧ","¬","��","��","��","̷","½","��","��","��","ʯ","��","��","��","Τ","��","��","��","��","��","��","��","��","��","��","Ѧ","��","��","��","��","��","ʷ","��","��","��","��","ë","��","��","��","��","Ǯ","��","��","��","��","Ī","��","��","��"};
	CString strLastName[78] = {"л","��","��","��","��","��","��","��","��","��","Ф","��","��","Ԭ","��","��","��","��","��","��","Ҷ","��","��","κ","��","��","��","��","Ҧ","¬","��","��","��","̷","½","��","��","��","ʯ","��","��","��","Τ","��","��","��","��","��","��","��","��","��","��","Ѧ","��","��","��","��","��","ʷ","��","��","��","��","ë","��","��","��","��","Ǯ","��","��","��","��","Ī","��","��","��"};;
	int iFamilyName = 0;
	int iLastName = 0;
	srand(time(NULL));
	iFamilyName = rand() % 78;
	iLastName = rand() % 78;
	strName.Format("%s%s",strFamilyName[iFamilyName],strLastName[iLastName]);
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void C51JobWebPost::GenAddress(CString &strProvince,CString &strAddress,CString &strMailCode,CString &strCity)
{
	CString strProvinceArray[15] = {"����ʡ","����","������",
		"����ʡ","����ʡ","�㶫ʡ",
		"����","����ʡ","����ʡ",
		"�ӱ�ʡ","����ʡ","������ʡ",
		"����ʡ","����ʡ","����ʡ"};
	CString strAddressArray[15] = {"����ʡ�Ϸ��й�·11��",
						"���Ÿ�ƽ·1057��",
						"�����й�·1001��",
						"����ʡ���ݴ���·101��",
						"����ʡ����������·88��",
						"�㶫ʡ��������ɽ���614��301��",
						"���������й��·35��",
						"����ʡ�����г����·28��",
						"����ʡ����������һ·99��",
						"�ӱ�ʡʯ��ׯ������·55��",
						"����ʡ֣����֣��·81��",
						"������ʡ�������й���·98��",
						"����ʡ�������ƽ·87��",
						"����ʡ����������¥��1024��",
						"����ʡ�����м���·34��"
								};
	CString strCityArray[15] = {"�Ϸ���",
		"����",
		"������",
		"����",
		"������",
		"������",
		"������",
		"������",
		"������",
		"ʯ��ׯ��",
		"֣����",
		"��������",
		"�����",
		"������",
		"������"
	};
	CString strCodeArray[15] = {"315004",
		"564824",
		"524654",
		"112547",
		"256548",
		"518000",
		"619222",
		"651248",
		"201547",
		"616584",
		"232564",
		"313658",
		"424565",
		"414005",
		"515007"
	};
	srand(time(NULL));
	int iNum = rand() % 15;
	strProvince.Format("%s",strProvinceArray[iNum]);
	strCity.Format("%s",strCityArray[iNum]);
	strAddress.Format("%s",strAddressArray[iNum]);
	strMailCode.Format("%s",strCodeArray[iNum]);
}
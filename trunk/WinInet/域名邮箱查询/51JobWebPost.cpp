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
#include "CountGen.h"
#pragma comment(lib,"Iphlpapi.lib")
HANDLE  g_hEvent = NULL;			//事件句柄
CString g_strCheckCode = "";
bool g_ExitFlag = true;
RegisterPackList g_RegisterList;
int g_iDeleteFlag = 0;
int g_iShowWidth = 0;
int g_iShowHeight = 0;
int g_iAotuChangeIp = 0;
int g_iAotuCheckCode = 0;
int g_iIpRegNum = 0;
int g_iIpChangeType = 0;		//IP切换方式
int g_iCheckIpFlag = 0;			//0不检测IP是否变化，1通过RAS检测IP变化，2通过POST方式检测
int g_iReCheckIpCount = 0;		//在一次拨号成功后重新检测IP是否变化的总次数
int g_iTimeOut = 0;				//超时
int g_iCheckCodeType = 0;		//验证码自动检测配置，是否为测试
int g_iCodeErrorNum = 0;		//验证码验证错误重试次数
CRasDialMgr g_RasMgr;
CString g_strCurrIP = "";
CString g_strDirect = "";		//图像保存目录
CString g_strFileName = "";
int g_iRegWebFlag = 1;
int g_iIsChangeMac =0;

extern int g_iFreshCount;

/************************************************************************/
/*                                                                      */
/************************************************************************/
//添加发送缓存
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
//添加发送缓存
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
//获取发送数据,返回0表示正常,-1表示无数据,-2表示出错
int GetRegisterPack(RegisterPack &submitPack)
{
	int iRet = 0;
	try
	{
		int iSize = g_RegisterList.GetSize();
		if (iSize == 0)	//缓冲区无数据
		{
			iRet = -1;
		}
		else
		{
			submitPack = g_RegisterList.GetAt( 0 );			
			iRet = 0;	//正常获取到数据
		}		
	}
	catch(...)
	{
		iRet = -2;		//出错
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

void WriteEmail(CString strEmail)
{
	CString strAppPath = "";
	CUserFile *pFile = CUserFile::GetInstance();
	if ( pFile )
	{
		pFile->GetAppPath(strAppPath);
	}
	CString strFilePath = strAppPath + "域名联系邮箱.txt";
	CString strContent = strEmail;
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
void AddToShowList(RegisterPack rgPack)
{
	CString strStauts = "";
	switch (rgPack.iStatus)
	{
	case 0:
		break;
	case 1:
		strStauts = "√";
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
	if ( iIndex >=0 && iIndex < nCount )
	{
		g_AccList.SetItemText(iIndex,3,rgPack.strMemo);
		g_AccList.SetItemText(iIndex,2,rgPack.strMail);
	}
	
	if (rgPack.strMail != "")
	{
		WriteEmail(rgPack.strMail);
	}
	//写入OLDACC文件
	if (rgPack.strMemo != "查询失败")
	{
		WriteOldFile(rgPack);
	}	
}


/************************************************************************/
/*                                                                      */
/************************************************************************/
//写日志
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
		if (iFlag == 0)			//注册成功列表
		{
			folder = "log";
			filename.Format("%s_succes.txt",currTime.Format("%Y%m%d"));
		}
		else if(iFlag == 1)		//信息日志
		{
			folder = "log";
			filename.Format("%s_log.txt",currTime.Format("%Y%m%d"));
		}
		else if ( iFlag == 2 )	//缓冲日志
		{
			folder = "log";
			filename.Format("%s_fail.txt",currTime.Format("%Y%m%d"));
		}
		else if ( iFlag == 3 )	//缓冲日志
		{
			folder = "log";
			filename.Format("%s_insertErrorAcc.txt",currTime.Format("%Y%m%d"));
		}
		else			//异常日志 (3)
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
			if( mkdir(folderName) != 0 )//创建目录
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
    //初始化信息
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
	g_hEvent = CreateEvent(NULL, FALSE, TRUE, NULL);	//创建事件对象
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
//用户登录
bool C51JobWebPost::UserRegister()
{
	try
	{
		m_strServerURL = "qipai.jl.cncmax.cn";
		CString strError = "";
		CString sendStr = "";
		ShowMessage("正在获取充值页面...");
		CString objectlink = "/chongzhi/Dxpay.aspx";
		CString strHeaders = m_strHeader;
		int iRet = RequestGetPageExExEx(m_strServerURL,objectlink,strHeaders,strError);
		if ( iRet != 0 )
		{
			TRACE("获取页面出错");
			ShowMessage("获取充值页面失败");
			m_nErrorTime ++;
			DeleteObjectExEx();
			return false;
		}
		//m_strWorkBuf.MakeLower();
		//AfxMessageBox(m_strWorkBuf.Right(m_strWorkBuf.GetLength() - 200));
		//检查是否打开简历管理页
		int iPos = m_strWorkBuf.Find("请按照卡密码填写正确的密码");
		if (iPos >= 0)
		{
			ShowMessage("成功获取充值页,正在获取验证码...");
		}
		else
		{
			return false;
		}
		//此处需要获取到VIEWSTATE值
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
				/* 更换验证字符串                                                       */
// 				/************************************************************************/
// 				iRet = GetFirstUsefulKey();
// 				if (iRet != 0)
// 				{	
// 					Sleep(1000);
// 					continue;
// 				}
// 				m_strMac.MakeUpper();
// 				ShowMessage("模拟验证信息:"+m_strMac + "," + m_strTrans +","+m_strVerify);
				/************************************************************************/
				/* 此处先获取注册页面的信息,以备填充                                    */
				/************************************************************************/
				//HWND hwnd = ::FindWindow("IEFrame","棋牌乐 - 注册帐号 - Microsoft Internet Explorer");
				CString strTime = CTime::GetCurrentTime().Format("%m%d%H%M%S");
				rgPack.strIp = g_strCurrIP;
				m_iCheckFlag = 0;
				g_strCheckCode = "";
				if (m_iCheckFlag == 0)
				{
					//删除旧的验证码
					if (_access(g_strDirect+g_strFileName,0) == 0)
					{
						CFile::Remove(g_strDirect+g_strFileName);
					}
					ShowMessage("正在获取验证码...");
					m_iCheckFlag = 0;
					objectlink.Format("/yzm/Yzm.aspx?rnd111=61992366%s?",strTime);
					//strHeaders = m_strHeader + "\r\nReferer:http://www.dajiale365.com/reg.shtml";
					iRet = RequestGetPageExExEx(m_strServerURL,objectlink,strHeaders,strError,1);
					if (iRet != 0)
					{
						ShowMessage("获取验证码失败");
						m_nErrorTime ++;
						DeleteObjectExEx();
						return false;
					}
					else
					{
						ShowMessage("获取验证码成功");	
						strShow.Format("正在注册:%s %s",rgPack.strAcc,rgPack.strPwd);
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
					TRACE("POST出错");
					m_nErrorTime ++;
					DeleteObjectExEx();
					return false;
				}
				//m_strWorkBuf.MakeLower();				
				iPos = m_strWorkBuf.Find("注册成功");
				if ( iPos >= 0)
				{
                    ShowMessage("注册成功");
					rgPack.iStatus = 1;
					rgPack.strMemo = "成功";
					strShow.Format("%s,%s,",rgPack.strAcc,rgPack.strPwd);
					WriteLog(strShow,0);
					m_iIpSuccNum++;
					if (m_iIpSuccNum >= g_iIpRegNum)
					{
						if (g_iAotuChangeIp)
						{
							ShowMessage("单一IP注册数已达上限,正在切换IP...");
						}
						ShowMessage("单一M验证数据注册数已达上限,正在切换验证数据...");
						DisableKey(m_iID);
						m_iNeedChageIP = 1;
						m_iIpSuccNum = 0;
						m_iIsNeedChangeMac = 1;
						//continue;
					}
				}
				else
				{//注册失败						
					CString strBefore = "<div align=\"center\"><font color='red'>";
					CString strAfter = "</font></div>";
					strFailReason = GetKeyString(m_strWorkBuf,strBefore,strAfter);
					ShowMessage("注册失败:	"+strFailReason);
					rgPack.iStatus = 2;
					rgPack.strMemo = strFailReason;
					//此处找到单机已经达到上限字符串，获取下一组特征码
					if (strFailReason.Find("您注册太快") != -1)
					{
						DisableKey(m_iID);
					}
					m_iIpSuccNum = 0;
				}
				AddToShowList(rgPack);
				if (g_iDeleteFlag == 1)
				{
					if (strFailReason.Find("验证码输入错误")<0)
					{
						DeleteRegisterPack(rgPack);												
					}
					else
					{
						m_iCodeErrorNum ++;
						if (m_iCodeErrorNum >= g_iCodeErrorNum)
						{
							ShowMessage("验证码错误次数到达上限,跳过此账号的注册");
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
				if (m_iNeedChageIP == 1)		//换IP
				{
					DeleteObjectExEx();
					return false;
				}
				if (m_iIsNeedChangeMac == 1)		//换MAC地址
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
//用户登录
int C51JobWebPost::UserRegisterEx(RegisterPack &rgPack)
{
	try
	{
		//http://whois.www.net.cn/whois/api_whois_full?host=mh0.com&web_server=whois.godaddy.com&_=1301889655453
		m_strServerURL = "whois.www.net.cn";
		m_strServerURL = "who.godaddy.com";
		CString strError = "";
		CString sendStr = "";
		CString strShowEx = "";
		strShowEx.Format("正在查询 %s 的邮箱...",rgPack.strAcc);
		ShowMessage(strShowEx);
		CString objectlink = "";
		objectlink.Format("/whois/api_whois_full?host=%s&web_server=whois.godaddy.com&_=1301889655453",rgPack.strAcc);
		objectlink.Format("/whois.aspx?k=PGdD/xuI/TY=&domain=%s&prog_id=GoDaddy",rgPack.strAcc);
		CString strHeaders = m_strHeader;
		//strShowEx.Format("\r\nReferer:http://whois.www.net.cn/whois/domain/%s",rgPack.strAcc);
		strHeaders += strShowEx;//"\r\nReferer:http://whois.www.net.cn/whois/domain/mh0.com";

		int iRet = RequestGetPageExExEx(m_strServerURL,objectlink,strHeaders,strError);
		if ( iRet != 0 || g_iContrlFlag == 1)
		{
			ShowMessage("查询失败");
			m_nErrorTime ++;
			if (m_nErrorTime > 5)
			{
				m_nErrorTime = 0;
				DeleteObjectExEx();
				return -2;
			}
			DeleteObjectExEx();
			return -1;
		}
		
		//解析邮箱
		if (m_nErrorTime > 5)
		{
			m_nErrorTime = 0;
			DeleteObjectExEx();
			return -2;
		}
		
		//判断,如果有提示说输入验证码,则输入
		if (m_strWorkBuf.Find("ctl00_cphContent_txtAccessCode")>0)
		{
			ShowMessage("需要输入验证码");
			//加载验证码
			//<img id="whoisverify_ctl00_cphcontent_ctlcaptcha_CaptchaImage" alt="CAPTCHA code image" src="LanapCaptcha.aspx?get=image&c=whoisverify_ctl00_cphcontent_ctlcaptcha&t=5bb2e628f2334a809fe081deb9dfe256&s=nynjih2k0tdoqvt0sm45mzbg" complete="complete"/>
			//获取验证码地址
			m_strWorkBuf.Replace("'","\"");
			CString strTemp = m_strWorkBuf.Mid(m_strWorkBuf.Find("<img id=\"whoisverify_ctl00_cphcontent_ctlcaptcha_CaptchaImage"));
			CString strBefore = "src=\"";
			CString strAfter = "\"";
			CString strCodeUrl = "/" + GetKeyString(strTemp,strBefore,strAfter);

			CUserFile *pFile = CUserFile::GetInstance();
			if ( pFile )
			{
				pFile->GetAppPath(g_strDirect);
			}
			while(!g_ExitFlag)
			{
				g_strCheckCode = "";
				g_strFileName = "code.bmp"; //CTime::GetCurrentTime().Format("%m%d%H%M%S")+".bmp";
				
				//删除旧的验证码
				if (_access(g_strDirect+g_strFileName,0) == 0)
				{
					CFile::Remove(g_strDirect+g_strFileName);
				}
				ShowMessage("正在获取验证码...");
				m_iCheckFlag = 0;
				objectlink.Format("/ckcode.php?rnd111=619923%d%s?",0,"");
				objectlink = strCodeUrl;
				//strHeaders = m_strHeader + "\r\nReferer:http://www.dajiale365.com/reg.shtml";
				iRet = RequestGetPageExExEx(m_strServerURL,objectlink,strHeaders,strError,1);
				if (iRet != 0)
				{
					ShowMessage("获取验证码失败");
					m_nErrorTime ++;
					DeleteObjectExEx();
					return -1;
				}
				else
				{
					ShowMessage("获取验证码成功");	
					ShowMessage("请输入验证码");	
					// 						strShow.Format("正在充值:%s %s %s",rgPack.strNickName,rgPack.strAcc,rgPack.strPwd);
					// 						ShowMessage(strShow);
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
//				g_strCheckCode = "333";
				CString strFailReason = "";
				CString strCheckCode = g_strCheckCode;
				if (strCheckCode == "r")
				{
					g_strCheckCode = "";
					DeleteObjectExEx();
					continue;		//重来过
				}
				break; 
			}
			//得到了验证码,进行验证
			//http://who.godaddy.com/whoisverify.aspx?_=1301996119187&jsonRequest=true&Request=whoisVerify&AccessCode=YWGMEH9&Domain=dfa.com&ContextID=GoDaddy
			objectlink.Format("/whoisverify.aspx?_=1301996119187&jsonRequest=true&Request=whoisVerify&AccessCode=%s&Domain=%s&ContextID=GoDaddy",g_strCheckCode,rgPack.strAcc);
			iRet = RequestGetPageExExEx(m_strServerURL,objectlink,strHeaders,strError);
			if ( iRet != 0 || g_iContrlFlag == 1)
			{
				ShowMessage("提交验证码失败");
				m_nErrorTime ++;
				if (m_nErrorTime > 5)
				{
					m_nErrorTime = 0;
					DeleteObjectExEx();
					return -2;
				}
				DeleteObjectExEx();
				return -1;
			}
			if (m_strWorkBuf.Find("redirectUrl") < 0)
			{
				ShowMessage("验证码错误");
				DeleteObjectExEx();
				return -1;
			}
			m_strWorkBuf.Replace("'","\"");
			strBefore = "\":\"";
			strAfter = "\"";
			objectlink = "/"+GetKeyString(m_strWorkBuf,strBefore,strAfter);
			iRet = RequestGetPageExExEx(m_strServerURL,objectlink,strHeaders,strError);
			if ( iRet != 0 || g_iContrlFlag == 1)
			{
				ShowMessage("查询域名邮箱失败");
				m_nErrorTime ++;
				if (m_nErrorTime > 5)
				{
					m_nErrorTime = 0;
					DeleteObjectExEx();
					return -2;
				}
				DeleteObjectExEx();
				return -1;
			}
		}
		//:
		int iPos = m_strWorkBuf.Find("Registered through");
		if (iPos < 0)
		{
			ShowMessage("无此域名信息");
			rgPack.strMemo = "无此域名信息";	
			DeleteObjectExEx();
			return 0;
		}
		iPos = m_strWorkBuf.Find("Administrative Contact:");
		if (iPos >= 0)
		{
			ShowMessage("成功解析邮箱");
			rgPack.strMemo = "成功";
			iPos = m_strWorkBuf.Find("@");			
			CString strTemp = m_strWorkBuf.Left(m_strWorkBuf.Find("<br>",iPos));
			strTemp.Trim();
			strTemp = strTemp.Mid(strTemp.ReverseFind(' '));
			strTemp.Trim();
			rgPack.strMail = strTemp;
		}
		else
		{
			rgPack.strMemo = "无邮箱";			
		}
		DeleteObjectExEx();
		return 0;

		CString strBefore = "type=\"hidden\" name=\"comeurl\" value=\"";
		CString strAfter = "\">";
		CString strComeurl = GetKeyString(m_strWorkBuf,strBefore,strAfter);

		strBefore = "type=\"hidden\" name=\"key\" value=\"";
		strAfter = "\">";
		CString strKey = GetKeyString(m_strWorkBuf,strBefore,strAfter);

		strBefore = "type=\"hidden\" name=\"t\" value=\"";
		strAfter = "\">";
		CString strTflag = GetKeyString(m_strWorkBuf,strBefore,strAfter);

		CString postStr = "";
		CString strShow = "";
		CString strRand = "";

		CString strTime = CTime::GetCurrentTime().Format("%m%d%H%M%S");
		rgPack.strIp = g_strCurrIP;
		m_iCheckFlag = 0;
		g_strCheckCode = "";
		int iRand = rand()%100;
		
		if (g_ExitFlag)
		{
			DeleteObjectExEx();
			return -1;
		}
		CString strFailReason = "";
		CString strCheckCode = g_strCheckCode;

		objectlink = "/user/login.php";
		strHeaders = m_strHeader + "\r\nReferer:http://www.jj.cn/user/login.php?comeurl=http%3A%2F%2Fmy.jj.cn%2Fcoffer.php";				
		rgPack.strMail = "";
		
		postStr.Format("comeurl=%s&isShowCode=0&key=%s&act=login&t=%s&username=%s&password=%s",
			"http%3A%2F%2Fmy.jj.cn%2Fcoffer.php",
			strKey,
			strTflag,
			rgPack.strAcc,
			rgPack.strPwd
			);
		iRet = RequestPostPageExExEx(m_strServerURL,objectlink,strHeaders,postStr,strError,0);//
		if ( iRet != 0 )
		{
			TRACE("POST出错");
			m_nErrorTime ++;
			DeleteObjectExEx();
			return false;
		}
		//WriteBufLog("c:\\test.txt",m_strWorkBuf);	
		iPos = m_strWorkBuf.Find("http-equiv='refresh'");
		if ( iPos >= 0)
		{
			ShowMessage("登录成功,正在获取金币数...");					
		}
		else
		{
			ShowMessage("登录失败");
			m_nErrorTime ++;
			DeleteObjectExEx();
			rgPack.iStatus = 0;
			rgPack.strMemo = "登录失败";
			return -2;
		}

		DeleteObjectExEx();
		//查询数据
		m_strServerURL = "my.jj.cn";
		objectlink = "/coffer.php";
		strHeaders = m_strHeader + "\r\nUA-CPU:x86";				
		

		rgPack.strMail = "";
		iRet = RequestGetPageExExEx(m_strServerURL,objectlink,strHeaders,strError);
		if ( iRet != 0 )
		{
			TRACE("获取页面出错");
			ShowMessage("获取金钱页面失败");
			m_nErrorTime ++;
			DeleteObjectExEx();
			return -1;
		}		
		iPos = m_strWorkBuf.Find("账户金币数");
		if ( iPos < 0)
		{
			ShowMessage("获取金币数失败");
			m_nErrorTime ++;
			DeleteObjectExEx();
			rgPack.iStatus = 0;
			rgPack.strMemo = "失败";
			strShow.Format("%s,%s,%s",rgPack.strAcc,rgPack.strPwd,rgPack.strNickName);
			WriteLog(strShow,0);
			return -2;
		}
		else
		{
			//账户金币数:<span>193</span>
			//保险箱金币数:<span>21</span>
			strBefore = "账户金币数:<span>";
			strAfter = "</span>";
			rgPack.strMoney = GetKeyString(m_strWorkBuf,strBefore,strAfter);
			strBefore = "保险箱金币数:<span>";
			strAfter = "</span>";
			if (m_strWorkBuf.Find("保险箱金币数")<0)
			{
				rgPack.strBank = "无保险箱";
			}
			else 
			{
				rgPack.strBank = GetKeyString(m_strWorkBuf,strBefore,strAfter);
			}
			ShowMessage("获取金币数成功");
			DeleteObjectExEx();
			rgPack.iStatus = 1;
			rgPack.strMemo = "成功";
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
		pParam->ShowMessage("线程退出");
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
	ShowMessage("初始化成功");
	srand(time(NULL));
	int iRunCount = 0;
	while (!g_ExitFlag)
	{
		RegisterPack rgPack;
		int iRet = GetRegisterPack(rgPack);
		if (iRet == -1)
		{
			ShowMessage("请导入域名...");
			Sleep(700);
			continue;
		}
		else if (iRet == -2)
		{
			Sleep(2000);
			continue;
		}
		m_iIpSuccNum = 0;

		int iErrorCount = 0;
		int iSameIpCount = 0;
		while (m_iNeedChageIP && !g_ExitFlag && g_iAotuChangeIp == 1)	//如果需要换IP
		{
					
			m_iNeedChageIP = 1;
			g_RasMgr.StopRas(sendStr);
			Sleep(1000);
			ShowMessage(sendStr);

			int iRet = g_RasMgr.DialRas(sendStr);
			//iRet = bRet==true?0:1; //g_RasMgr.DialRas(sendStr);		
			//	ShowMessage(sendStr);
			if (iRet == 0)		//重新拨号成功，获取新的IP
			{
				ShowMessage("拨号成功");
				g_iCheckIpFlag = 3;
				if (g_iCheckIpFlag == 2)////POST检测
				{
					int iRet = -1;
					int iErrorNum = 0;
					while (iRet == -1)
					{
						iRet = GetGateWayIP(g_strCurrIP);
						if (iRet == -1)
						{
							iErrorNum++;
							sendStr.Format("获取本机Ip失败 %d 次,重试...",iErrorNum);
							ShowMessage(sendStr);							
							if (iErrorNum >= 3)
							{
								ShowMessage("获取本机Ip失败次数超过限制,跳过此操作...");
								g_strCurrIP = m_strOldIP;
								break;
							}
							Sleep(300);
						}
						else
						{
							ShowMessage("检测到本机Ip: "+g_strCurrIP);
						}
					}
					if (g_strCurrIP == m_strOldIP)
					{
						m_iNeedChageIP = 1;
						iSameIpCount ++;
						if (iSameIpCount < g_iReCheckIpCount)
						{
							sendStr.Format("Ip未更换,重复拨号[%d]...",iSameIpCount);
							ShowMessage(sendStr);
							Sleep(300);
							continue;
						}
						sendStr.Format("重复拨号次数[%d]超过限制仍未更换Ip,跳过此操作...",g_iReCheckIpCount);
						ShowMessage(sendStr);
						iSameIpCount = 0;
					}
					else	//IP更换成功
					{
						m_strOldIP = g_strCurrIP;
					}
				}
				else if (g_iCheckIpFlag == 1)//RAS检测
				{
					int iRet = -1;
					int iErrorNum = 0;
					while (iRet == -1)
					{
						iRet = g_RasMgr.GetGateWayIP(g_strCurrIP);
						if (iRet == -1)
						{
							iErrorNum++;
							sendStr.Format("获取本机Ip失败 %d 次,重试...",iErrorNum);
							ShowMessage(sendStr);
							if (iErrorNum >= 3)
							{
								ShowMessage("获取本机Ip失败次数超过限制,跳过此操作...");
								g_strCurrIP = m_strOldIP;
								break;
							}
							Sleep(300);
						}
						else
						{
							ShowMessage("检测到本机Ip: "+g_strCurrIP);
						}
					}
					if (g_strCurrIP == m_strOldIP)
					{
						m_iNeedChageIP = 1;
						iSameIpCount ++;
						if (iSameIpCount < g_iReCheckIpCount)
						{
							sendStr.Format("Ip未更换,重复拨号[%d]...",iSameIpCount);
							ShowMessage(sendStr);
							Sleep(500);
							continue;
						}
						sendStr.Format("重复拨号次数[%d]超过限制仍未更换Ip,跳过此操作...",g_iReCheckIpCount);
						ShowMessage(sendStr);

						iSameIpCount = 0;
					}
					else	//IP更换成功
					{
						m_strOldIP = g_strCurrIP;
					}
				}
				else{}//不检测
				iSameIpCount = 0;
				m_iNeedChageIP = 0;
				iErrorCount = 0;
				break;
			}
			else
			{
				ShowMessage("失败");
			}
			iErrorCount ++;
			if (iErrorCount>=10)
			{
				ShowMessage("拨号连接失败次数过多,请确认你的ADSL信息设置正确");
				return;
			}
			Sleep(500);
		}
		//判断刷新时间是否到了
		iRet = UserRegisterEx(rgPack);
		switch (iRet)
		{
		case  -2:			
			DeleteRegisterPack(rgPack);
			rgPack.strMemo = "查询失败,换IP";
			AddToShowList(rgPack);
			//重新拨号
			m_iNeedChageIP = 1;
			m_iIpSuccNum = 0;
// 			sendStr.Format("%s,%s,%s -- %s",rgPack.strAcc,rgPack.strPwd,rgPack.strNickName,rgPack.strMemo);
// 			WriteLog(sendStr,2);
			break;
		case -1:
			break;
		case -3:
			DeleteRegisterPack(rgPack);
			break;
		case 0:
			iRunCount++;
			DeleteRegisterPack(rgPack);
			//AddRegisterPack(rgPack);
			AddToShowList(rgPack);
			m_iIpSuccNum++;
			if (m_iIpSuccNum >= g_iIpRegNum)
			{
				ShowMessage("单IP查询数达到上限,换IP");
				//重新拨号
				m_iNeedChageIP = 1;
				m_iIpSuccNum = 0;
			}
			break;
		default:
			break;
		}
		if (iRet != -2 && iRet != -1 )
		{
			ShowMessage("查询等待.");
			Sleep(g_iFreshCount * 1000);
		}
		else
		{
			Sleep(500);
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
		if (strTemp.Find("您的ip地址是：[") !=-1)
		{
			CString strBefore = "您的ip地址是：[";
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
// 				//创建它
// 				
// 				ShowMessage("打开Excel文件失败"+adoConn.GetLastErrorText());
// 				return;
// 			}
// 		}
// 		CString strSQL
}

/************************************************************************/
/*       创建Excel数据库                                                */
/************************************************************************/
BOOL C51JobWebPost::CreatExcelDb(CString dbpath,CString tableName,CString &strError)
{
	if (_access(dbpath,0)==0)	//存在
	{
		strError = "数据库已存在";
		return TRUE;
	}
	CDatabase database;
	CString sDriver;
	CString sExcelFile; 
	CString sSql;
	//检索是否安装有Excel驱动 "Microsoft Excel Driver (*.xls)" 
	sDriver = GetExcelDriver();
	if (sDriver.IsEmpty())
	{
		// 没有发现Excel驱动
		strError = "请先安装Excel软件才能使用导出功能";
		return FALSE;
	}
	// 创建进行存取的字符串
	sSql.Format("DRIVER={%s};DSN='';FIRSTROWHASNAMES=1;READONLY=FALSE;CREATE_DB=\"%s\";DBQ=%s",sDriver, sExcelFile, dbpath);
	// 创建数据库 (既Excel表格文件)
	if( database.OpenEx(sSql,CDatabase::noOdbcDialog) )
	{
		CString strTemp;
		strTemp.Format("账号 TEXT,密码 TEXT,推荐人 TEXT,邮箱 TEXT,网址 TEXT,时间 TEXT");
		// 创建表结构			
		sSql = "CREATE TABLE " + tableName + " ( " + strTemp +  " ) ";
		database.ExecuteSQL(sSql);
		return TRUE;
	}
	else
	{
		strError = "创建EXCEL数据库失败";
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

	// 获取已安装驱动的名称(涵数在odbcinst.h里)
	if (!SQLGetInstalledDrivers(szBuf, cbBufMax, &cbBufOut))
		return "";

	// 检索已安装的驱动是否有Excel...
	do
	{
		if (strstr(pszBuf, "Excel") != 0)
		{
			//发现 !
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
		if ( !adoConn.IsOpen() )		//如果未连接上服务器
		{
			CString strAppPath = "";
			CUserFile::GetInstance()->GetAppPath(strAppPath);
			if ( !adoConn.ConnectAccess(strAppPath+"7pRegister.mdb",""))
			{
				ShowMessage("连接数据库失败");
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
			ShowMessage("保存账号信息失败");
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
		ShowMessage("保存账号信息异常");
	}
	return -1;
}

IHTMLDocument2* C51JobWebPost::GetDocInterface(HWND hWnd) 
{
	// 我们需要显示地装载OLEACC.DLL,这样我们才知道有没有安装MSAA
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
	else{//如果没有安装MSAA
		AfxMessageBox(_T("请您安装Microsoft Active Accessibility"));
	}
	//CoUninitialize();
	return pDoc2;
}
/************************************************************************/
/* 得到MAC地址等信息                                                    */
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
						//找到VERIFY
						//ShowMessage("找到verify");
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
	//目标窗口已经关闭，更新MAC地址
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
			ShowMessage("MAC地址修改器未启动！");
		}
		iCount++;
	}
	if (!hwnd)
	{
		ShowMessage("MAC地址修改器未启动！");
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
	//重新打开注册窗口
	CWindowFind find;
	m_hwnd = NULL;
	m_hwnd = ::FindWindow("#32770","帐号注册");
	while(m_hwnd)
	{
		::SendMessage(m_hwnd,WM_CLOSE,0,0);
		Sleep(50);
		m_hwnd = ::FindWindow("#32770","帐号注册");
		if (m_hwnd == NULL)
		{
			break;
		}		
	}
	//关闭后
	HWND hwnd = find.MyFindWindow("Static","游戏帐号：");
	if (hwnd)	//登录框的子窗口
	{
		hwnd = ::GetParent(hwnd);
		if (hwnd)
		{
			//hwnd = pParantWnd->GetSafeHwnd();
			//找账号框
			HWND hwndChind = find.GetChildWindow(hwnd,6);	//账号选择框
			Sleep(40);
			hwndChind = find.GetChildWindow(hwndChind,1);//账号输入编辑框
			//注册按钮
			hwndChind = find.GetChildWindow(hwnd,9);
			Sleep(40);
			find.PostBtnClick(hwndChind);
		}
	}
	Sleep(100);
	m_hwnd = NULL;
	m_hwnd = ::FindWindow("#32770","帐号注册");
	//判断程序是否已经启动
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
		if ( !adoConn.IsOpen() )		//如果未连接上服务器
		{
			CString strAppPath = "";
			CUserFile::GetInstance()->GetAppPath(strAppPath);
			if ( !adoConn.ConnectAccess(strAppPath+"machineInfo.mdb",""))
			{
				ShowMessage("连接数据库失败");
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
			ShowMessage("读取键值信息失败");
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
				ShowMessage("当天已经没有可用的键值信息，请调用生成程序");
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
		ShowMessage("读取键值信息异常");
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
		if ( !adoConn.IsOpen() )		//如果未连接上服务器
		{
			CString strAppPath = "";
			CUserFile::GetInstance()->GetAppPath(strAppPath);
			if ( !adoConn.ConnectAccess(strAppPath+"machineInfo.mdb",""))
			{
				ShowMessage("连接数据库失败");
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
			ShowMessage("删除机器信息失败");
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
		ShowMessage("删除机器信息异常");
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
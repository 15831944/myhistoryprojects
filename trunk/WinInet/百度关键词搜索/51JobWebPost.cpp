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
#include <atlrx.h>
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

CString g_strCurrIP = "";
CString g_strDirect = "";		//ͼ�񱣴�Ŀ¼
CString g_strFileName = "";
int g_iRegWebFlag = 1;
int g_iIsChangeMac =0;

int g_iTimeStart = 0;
int g_iTimeEnd = 0;
int g_iTimeDelay = 0;
int g_iFreshDelay = 0;
int g_iHelpDelay = 0;	//�������������֮��ļ��

int g_iIsNeedLogin = 1;
int g_iUtfFlag = 0;
/************************************************************************/
/*                                                                      */
/************************************************************************/
//��ӷ��ͻ���
/************************************************************************/
/*                                                                      */
/************************************************************************/
void ShowMessage(CString strMsg)
{
	CUserMessagePump *pMsgPump = NULL;
	pMsgPump = CUserMessagePump::GetInstance();
	if (pMsgPump)
	{
		pMsgPump->AddToMessagePump(USERWINDOWMSG_1,strMsg);
	}
}

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
	g_AccList.InsertItem(nCount,"");
	strStauts.Format("%d",nCount);
	g_AccList.SetItemText(nCount,0,strStauts);	
	g_AccList.SetItemText(nCount,1,rgPack.strAcc);
	strStauts.Format("%d",rgPack.iStatus);
	g_AccList.SetItemText(nCount,2,strStauts);
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
	//m_strHeader += "\r\nAccept:image/gif, image/x-xbitmap, image/jpeg, image/pjpeg, application/x-shockwave-flash, application/vnd.ms-excel, application/vnd.ms-powerpoint, application/msword, application/x-silverlight, */*";
	m_strHeader += "\r\nAccept:*/*";
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
	return true;     
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
		ShowMessage("�߳��˳�");
	}
	CoUninitialize();
     
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void C51JobWebPost::RegisterFunc()
{
	CString strOldAcc = "";
	g_iIsNeedLogin = 1;
	m_iNeedChageIP = 0;
	m_iIpSuccNum = 0;
	CString sendStr = "";
	CString strError = "";
	ShowMessage("��ʼ���ɹ�");
	int iCurrTime = 0;
	int iOldCurrTime = 0;
	//g_iTimeDelay ��Ϊ��������
	while (!g_ExitFlag)
	{
		RegisterPack rgPack;
		int iRet = GetRegisterPack(rgPack);
		if (iRet == -1)
		{
			ShowMessage("���");
			return;
		}
		else if (iRet == -2)
		{
			ShowMessage("��ȡ�˺ų���...");
			Sleep(2000);
			continue;
		}
		if (rgPack.iStatus > g_iFreshDelay)
		{
			ShowMessage("������ȵ����趨ֵ,���");
			return;
		}
		//�����ж�,����Ѿ��ﵽҪ��Ĳ��,���,�˳�
		iRet = GetKeyWords(rgPack.strAcc,rgPack.iStatus,strError);
		if (iRet < 0)
		{
            continue;
		}
		g_RegisterList.RemoveAt(0);
	}
	DeleteObjectExEx();
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
CString C51JobWebPost::GetKeyStringEx(CString srcStr,CString &strBefore,CString &strAfter)
{
	int iPosBefore = 0;
	int iPosAfter = 0;
	iPosAfter = srcStr.Find(strAfter);
	srcStr = srcStr.Left(iPosAfter);
	iPosBefore = srcStr.ReverseFind('"');
	if (iPosBefore < 0 || iPosAfter < 0)
	{
		return "";
	}
	else
	{
		return srcStr.Mid(iPosBefore+1);
	}
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
CString C51JobWebPost::GetKeyStringExEx(CString srcStr,CString &strBefore,CString &strAfter)
{
	int iPosBefore = 0;
	int iPosAfter = 0;
	iPosAfter = srcStr.Find(strAfter);
	srcStr = srcStr.Left(iPosAfter);
	iPosBefore = srcStr.ReverseFind('=');
	if (iPosBefore < 0 || iPosAfter < 0)
	{
		return "";
	}
	else
	{
		return srcStr.Mid(iPosBefore+1);
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
//-1��ѭ��,-2��ѭ��
int C51JobWebPost::HelpSubject(CString strUserName,CString strPwd,CString strUrl,CString strSubContent,int iDelay,CString &strError)
{
	try
	{
		CString postStr = "";
		CString strShow = "";
		int iRet = -1;
		INTERNET_PORT iPort = 80;
		DWORD dType = AFX_INET_SERVICE_HTTP;
		CString strSubUrl = "";
		BOOL iiRet = AfxParseURL(strUrl,dType,m_strServerURL,strSubUrl,iPort);
		if (iiRet == 0)
		{
			ShowMessage("����URLʧ��,�˳�");
			return -3;
		}
		CString strCmpUrl = "";
		strCmpUrl = "bbs.0595bbs.cn";		//Ȫ����̳
		strCmpUrl = "bbs.etaicang.com";		//̫����̳
		strCmpUrl = "www.18lou.net";		//18¥
		strCmpUrl = "www.chinayk.com";		//�й�Ӫ����̳
		strCmpUrl = "www.0574bbs.com";		//��Ҧ��̳
		strCmpUrl = "bbs.zxip.com";			//��Ϫ��̳
		strCmpUrl = m_strServerURL;
		//strCmpUrl = "bbs.0595bbs.cn";		//Ȫ����̳
		if (strCmpUrl == "www.chinayk.com")
		{
			g_iUtfFlag = 1;
		}
		else
		{
			g_iUtfFlag = 0;
		}
		if (strCmpUrl != m_strServerURL)
		{
			ShowMessage("��̳��ַ����,�˳�");
			return -3;
		}
		CString strFailReason = "";
		CString strFlag = "";
		CString sendStr = "";
		CString objectlink = "/login.php";
		CString strHeaders = m_strHeader;
		if (g_iIsNeedLogin == 1)
		{
			ShowMessage("��ȡ��¼ҳ��...");			
			iRet = RequestGetPageExExEx(m_strServerURL,objectlink,strHeaders,strError);
			if (iRet < 0)
			{
				ShowMessage("��ȡ��¼ҳ��ʧ��");
				m_nErrorTime ++;
				DeleteObjectExEx();
				return -1;
			}
			int iPos = m_strWorkBuf.Find("Cookie ��Ч��:");			
			if (iPos >= 0)
			{
				ShowMessage("�ɹ���ȡ��¼ҳ...");
			}
			else if (m_strWorkBuf.Find("���Ѿ�Ϊ��Ա���,�벻Ҫ�ظ���¼") > 0)	//��Ҫ�˳���¼
			{
				objectlink = "/index.php";
				Sleep(iDelay);
				iRet = RequestGetPageExExEx(m_strServerURL,objectlink,strHeaders,strError);
				CString strBefore = "";
				CString strAfter = "\" hidefocus=\"true\">�˳�</a>";
				objectlink = GetKeyStringEx(m_strWorkBuf,strBefore,strAfter);
				Sleep(iDelay);
				iRet = RequestGetPageExExEx(m_strServerURL,objectlink,strHeaders,strError);
				return -1;
			}
			else 
			{
				ShowMessage("��ȡ��¼ҳʧ��");
				return -2;
			}
			m_iCheckFlag = 0;
			g_strCheckCode = "";				
			if (g_ExitFlag)
			{
				DeleteObjectExEx();
				return -2;
			}			
			CString strCheckCode = g_strCheckCode;
			CString struuidCode = "";
			CString strAppId = "";
			objectlink = "/login.php?";
			strHeaders = m_strHeader;
			CString strReffers = "";
			strReffers.Format("\r\nReferer:http://%s/login.php",m_strServerURL);
			strHeaders += strReffers;
			postStr.Format("forward=&jumpurl=&step=2&lgt=0&pwuser=%s&pwpwd=%s&hideid=0&cktime=0&submit=%B5%C7%C2%BC",
				strUserName,
				strPwd
				);
			Sleep(iDelay);
			iRet = RequestPostPageExExEx(m_strServerURL,objectlink,strHeaders,postStr,strError,0);//
			if ( iRet != 0 )
			{
				m_nErrorTime ++;
				DeleteObjectExEx();
				return -2;
			}
			m_strWorkBuf.MakeLower();
			CFile file;
			file.Open("c:\\sdfd.txt",CFile::modeCreate|CFile::modeWrite);
			file.Write(m_strWorkBuf,m_strWorkBuf.GetLength());
			file.Close();
			iPos = m_strWorkBuf.Find("���Ѿ�˳����¼");
			int iPos11 = m_strWorkBuf.Find(strUserName);
			int iPos2 = m_strWorkBuf.Find("��½�ɹ�");
			if ( iPos > 0 || iPos2 > 0 || iPos11 > 0)
			{
			}
			else if (m_strWorkBuf.Find("���Ѿ�Ϊ��Ա���") > 0)	//��Ҫ�˳���¼
			{
				objectlink = "/index.php";
				Sleep(iDelay);
				iRet = RequestGetPageExExEx(m_strServerURL,objectlink,strHeaders,strError);
				CString strBefore = "";
				CString strAfter = "\" hidefocus=\"true\">�˳�</a>";
				objectlink = GetKeyStringEx(m_strWorkBuf,strBefore,strAfter);
				Sleep(iDelay);
				iRet = RequestGetPageExExEx(m_strServerURL,objectlink,strHeaders,strError);
				return -1;
			}
			else
			{
				CString strBefore = "";
				CString strAfter = "\" hidefocus=\"true\">�˳�</a>";

				ShowMessage("��¼��̳ʧ��");
				strBefore = "<span class=\"f14\">";
				if (m_strWorkBuf.Find(strBefore) >=0)
				{
					strAfter = "</span><br />";
					strError = GetKeyString(m_strWorkBuf,strBefore,strAfter);
					ShowMessage(strError);
				}
				DeleteObjectExEx();
				if (m_strWorkBuf.Find("�������") >=0 || m_strWorkBuf.Find("������") >= 0) 
				{
					return -3;
				}
				return -2;
			}				
			ShowMessage("��¼��̳�ɹ�");
			g_iIsNeedLogin = 0;
		}
		ShowMessage("��ʼ��ȡ����ҳ��...");
		CString strTime = CTime::GetCurrentTime().Format("%m%d%H%M%S");
		try
		{				
			//���ؽ�����Ҫ��������
			objectlink = strSubUrl;
			Sleep(iDelay);
			iRet = RequestGetPageExExEx(m_strServerURL,objectlink,strHeaders,strError);
			if (iRet < 0)
			{
				ShowMessage("��ȡ����ҳ��ʧ��");
				m_nErrorTime ++;
				LogOut();
				DeleteObjectExEx();
				return -1;
			}
			CFile file1;
			file1.Open("c:\\333333331.txt",CFile::modeCreate|CFile::modeWrite);
			file1.Write(m_strWorkBuf,m_strWorkBuf.GetLength());
			//file1.Write(m_szWorkBuf,m_iszWorkBufLen);
			file1.Close();
			int iPos = strSubUrl.ReverseFind('/');
			strSubUrl = strSubUrl.Mid(iPos+1);
			if (m_strWorkBuf.Find(strSubUrl) < 0)
			{
				ShowMessage("��ȡ����ҳ��ʧ��");
				m_nErrorTime ++;
				LogOut();
				DeleteObjectExEx();
				return -2;
			}
			ShowMessage("��ȡ����ҳ��ɹ�");
			
			//AfxMessageBox(m_strWorkBuf.Right(100));
			int iiiFlag = 0;
			objectlink = "/post.php?";
			CString strTid = "";
			CString strVerify = "";
			CString strHexie = "";
			CString strTitle = "";
			CString strContent = "";
			CString strFid = "";
			CString strBefore = "";
			CString strAfter = "\" name=\"tid\" />";
			strAfter = "\" name=\"_hexie";
			int iiiiPos = m_strWorkBuf.Find("_hexie");
			if ( iiiiPos< 0)
			{
				ShowMessage("��ȡ������ʽ�б�");
				iiiFlag = 1;
			}
			if (iiiFlag == 0)
			{
				strAfter = "\" name=\"tid\" />";\
				strTid = GetKeyStringEx(m_strWorkBuf,strBefore,strAfter);
				strAfter = "\" name=\"_hexie";
				strHexie = GetKeyStringEx(m_strWorkBuf,strBefore,strAfter);
				strAfter = "\" name=\"fid\" />";
				strFid = GetKeyStringEx(m_strWorkBuf,strBefore,strAfter);
				strBefore = "name=\"verify\" value=\"";
				strAfter = "\" />";
				strVerify = GetKeyString(m_strWorkBuf,strBefore,strAfter);
				strBefore = "name=\"atc_title\" value=\"";
				strAfter = "\" ";
				strTitle = GetKeyString(m_strWorkBuf,strBefore,strAfter);
			}
			else
			{
				strAfter = " name=tid";
				strTid = GetKeyStringExEx(m_strWorkBuf,strBefore,strAfter);
				strAfter = " name=_hexie";
				strHexie = GetKeyStringExEx(m_strWorkBuf,strBefore,strAfter);
				strAfter = " name=fid";
				strFid = GetKeyStringExEx(m_strWorkBuf,strBefore,strAfter);
				strBefore = "name=tid> <INPUT type=hidden value=";
				strAfter = " ";
				strVerify = GetKeyString(m_strWorkBuf,strBefore,strAfter);
				strBefore = "name=\"atc_title\" value=\"";
				strAfter = " name=atc_title";
				strTitle = GetKeyStringExEx(m_strWorkBuf,strBefore,strAfter);
			}
			if (strHexie == "" || strFid == "")
			{
				ShowMessage("�Ҳ�������");
				g_iIsNeedLogin = 1;
				DeleteObjectExEx();
				return -1;
			}
			/*
			<input type="hidden" value="192" name="fid" />
			<input type="hidden" value="167729" name="tid" />
			<input type="hidden" name="verify" value="eb4ee27e" />
			<input type="hidden" value="cn0zz" name="_hexie" />
			*/
			
			
			strSubContent = URLEncode(strSubContent);
			strTitle = URLEncode(strTitle);
			postStr.Format("atc_usesign=1&atc_convert=1&atc_autourl=1&step=2&action=reply&fid=%s&tid=%s&verify=%s&_hexie=%s&atc_title=%s&atc_content=%s&attachment_1=&atc_desc1=",
				strFid,
				strTid,
				strVerify,
				strHexie,
				strTitle,
				strSubContent);
			Sleep(iDelay);
			CString strRefer = "";
			strRefer.Format("\r\nReferer:%s",strUrl);
			strHeaders += strRefer;
			iRet = RequestPostPageExExEx(m_strServerURL,objectlink,strHeaders,postStr,strError,0);//
			if ( iRet != 0 )
			{
				m_nErrorTime ++;
				LogOut();
				DeleteObjectExEx();
				return -1;
			}
			CFile file;
			file.Open("c:\\sdfd111.txt",CFile::modeCreate|CFile::modeWrite);
			file.Write(m_strWorkBuf,m_strWorkBuf.GetLength());
			file.Close();
			if (m_iStatusCode != 302)
			{
				if (m_strWorkBuf.Find("������ϵ�����������б�") > 0)
				{
					ShowMessage("�����ɹ�");
					LogOut();
					DeleteObjectExEx();
					return 0;
				}
				strBefore = "<span class=\"f14\">";
				if (m_strWorkBuf.Find(strBefore) >=0)
				{
					strAfter = "</span><br />";
					strError = GetKeyString(m_strWorkBuf,strBefore,strAfter);
					ShowMessage(strError);
					LogOut();
					DeleteObjectExEx();
					return -2;
				}
// 				ShowMessage("����ʧ��");
// 				DeleteObjectExEx();
// 				return -2;
			}			
			ShowMessage("�����ɹ�");
			LogOut();
			DeleteObjectExEx();
			return 0;
		}
		catch (...)
		{
			LogOut();
			DeleteObjectExEx();
			return -1;
		}			
	}
	catch (...)
	{
	}
	LogOut();
	DeleteObjectExEx();
	return -1;    
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
CString C51JobWebPost::RandContent()
{
	int iCount = g_ContentList.GetSize();
	if (iCount <= 0)
	{
		return "δ��������,�����Ĭ������";
	}
	int iValue = rand() % iCount;
	while (iValue >= iCount)
	{
		iValue --;
	}
	if (iValue < 0)
	{
		return "δ��������,�����Ĭ������";
	}
	return g_ContentList.GetAt(iValue);
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
//�ж��Ƿ��Ѿ��������ͳ���ȴ�ʱ��
bool C51JobWebPost::IsOverLastTime(unsigned int iErrorStopTime,long lastTime)
{
	DWORD t2 = 0;
	t2 = ::GetTickCount();
	DWORD tSpan = 0;
	tSpan = t2 - lastTime;
	if (tSpan > iErrorStopTime * 1000) //iDelay ��
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
void C51JobWebPost::LogOut()
{	
// 	CString strHeaders = m_strHeader;
// 	CString postStr = "";
// 	CString strError = "";
// 	CString strTid = "";
// 	CString strVerify = "";
// 	CString strBefore = "";
// 	CString strAfter = "";
// 	CString objectlink  = "";
// 	objectlink = "/index.php";
// 	int iRet = RequestGetPageExExEx(m_strServerURL,objectlink,strHeaders,strError);
// 	strHeaders += "\r\nReferer:http://bbs.0595bbs.cn/index.php";
// 	strBefore = "name=\"verify\" value=\"";
// 	strAfter = "\" />";
// 	strVerify = GetKeyString(m_strWorkBuf,strBefore,strAfter);	
// 	objectlink.Format("/login.php?action=quit&verify=%s",strVerify);
// 	iRet = RequestGetPageExExEx(m_strServerURL,objectlink,strHeaders,strError);
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
int C51JobWebPost::GetKeyWords(CString strKey,int iParent,CString &strError)
{
	m_strServerURL = "www.baidu.com";
	CString strFailReason = "";
	CString strFlag = "";
	CString sendStr = "";
	CString objectlink = "";
	objectlink.Format("/s?wd=%s",URLEncode(strKey));
	CString strHeaders = m_strHeader;
	int iRet = -1;
	CFile file;
	ShowMessage("��������...");
	iRet = RequestGetPageExExEx(m_strServerURL,objectlink,strHeaders,strError);
	if (iRet < 0)
	{
		DeleteObjectExEx();
		return -1;
	}	
	int iPos = m_strWorkBuf.Find("�������</td><td rowspan=");	
	if (iPos<0)
	{
		ShowMessage("�˹ؼ���û���������,����!");
		DeleteObjectExEx();
		return 0;
	}
	m_strWorkBuf = m_strWorkBuf.Right(m_strWorkBuf.GetLength() - iPos);
	iPos = m_strWorkBuf.Find("<table");
	m_strWorkBuf = m_strWorkBuf.Right(m_strWorkBuf.GetLength() - iPos - 6);
	CString strBefore = "<td";
	CString strEnd = "</table>";
	m_strWorkBuf = GetKeyString(m_strWorkBuf,strBefore,strEnd);
	CString strTempEx = "";
	while(true)
	{
		iPos = m_strWorkBuf.Find("<a");
		if (iPos < 0)
		{
			break;
		}
		m_strWorkBuf = m_strWorkBuf.Right(m_strWorkBuf.GetLength() - iPos);
		strBefore = "\">";
		strEnd = "</a>";
		strTempEx = GetKeyString(m_strWorkBuf,strBefore,strEnd);
		strTempEx.Trim();
		ShowMessage(strTempEx);
		if (g_keyMap[strTempEx] == 1)
		{
			ShowMessage("�ؼ����ظ�,����");
		}
		else
		{
			RegisterPack rgPack;
			rgPack.iStatus = iParent + 1;
			rgPack.strAcc = strTempEx;
			g_RegisterList.Add(rgPack);
			g_keyMap[strTempEx] = 1;
			//��ʾ
			AddToShowList(rgPack);
		}		
		iPos = m_strWorkBuf.Find(strBefore);
		if (iPos < 0)
		{
			break;
		}
		m_strWorkBuf = m_strWorkBuf.Right(m_strWorkBuf.GetLength() - iPos);
	}
	DeleteObjectExEx();
	return 0;
}
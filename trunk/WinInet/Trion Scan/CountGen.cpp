// CountGen.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "CountGen.h"
#include ".\countgen.h"
#include "UserMessagePump.h"
#include "datatype.h"
#include "UserFile.h"
// CCountGen �Ի���

IMPLEMENT_DYNAMIC(CCountGen, CDialog)
CCountGen::CCountGen(CWnd* pParent /*=NULL*/)
	: CDialog(CCountGen::IDD, pParent)
{
}

CCountGen::~CCountGen()
{
}
map<CString,int> gl_mapListAcc;
void CCountGen::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECK_AUTOCHECKCODE2, m_AutoCheck);
	DDX_Control(pDX, IDC_CHECK_AUTOIP2, m_AutoIP);
}

BEGIN_MESSAGE_MAP(CCountGen, CDialog)
	ON_BN_CLICKED(IDC_BTN_GENCOUNT, OnBnClickedBtnGencount)
	ON_BN_CLICKED(IDC_BTN_SAVE, OnBnClickedBtnSave)
	ON_BN_CLICKED(IDC_CHECK_AUTOIP2, OnBnClickedCheckAutoip2)
	ON_BN_CLICKED(IDC_CHECK_AUTOCHECKCODE2, OnBnClickedCheckAutocheckcode2)
	ON_BN_CLICKED(IDC_RADIO_IPCHANGE1, OnBnClickedRadioIpchange1)
	ON_BN_CLICKED(IDC_RADIO_IPCHANGE2, OnBnClickedRadioIpchange2)
	ON_BN_CLICKED(IDC_RADIO_CHECKIP1, OnBnClickedRadioCheckip1)
	ON_BN_CLICKED(IDC_RADIO_CHECKIP2, OnBnClickedRadioCheckip2)
	ON_BN_CLICKED(IDC_RADIO_CHECKIP3, OnBnClickedRadioCheckip3)
	ON_BN_CLICKED(IDC_BTN_FIND, OnBnClickedBtnFind)
	ON_BN_CLICKED(IDC_RADIO_LONGYAN, OnBnClickedRadioLongyan)
	ON_BN_CLICKED(IDC_RADIO_SHAOWU, OnBnClickedRadioShaowu)
	ON_BN_CLICKED(IDC_RADIO_PUTIAN, OnBnClickedRadioPutian)
	ON_BN_CLICKED(IDC_CHECK_CHANGEMAC, OnBnClickedCheckChangemac)
	ON_BN_CLICKED(IDC_BUTTON1, &CCountGen::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON3, &CCountGen::OnBnClickedButton3)
END_MESSAGE_MAP()


BOOL CCountGen::PreTranslateMessage( MSG* pMsg)
{
	if( pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE)
	{
		return TRUE;      //���ε�Esc��
	}
	if( pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
	{
		return TRUE;      //���ε�Enter��
	}

	return CDialog::PreTranslateMessage( pMsg );
}

BOOL CCountGen::OnInitDialog()
{
	CDialog::OnInitDialog();
	HICON hIcon;
	hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	SetIcon(hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(hIcon, FALSE);		// ����Сͼ��
	
	::SetWindowPos(m_hWnd,HWND_TOP,0,0,475,280,~SWP_NOSIZE & SWP_SHOWWINDOW|SWP_NOMOVE);
	int generateNum = 0;
	CString strAccType = "";
	CString strPwdType = "";
	CString strMailType = "";
	CString strIntroMan = "";
	int iTemp = 0;
	//��ȡ����
	CString strAppPath = "";
	CString strTemp = "";
	CUserFile *pFile = CUserFile::GetInstance();
	if (pFile)
	{
		pFile->GetAppPath(strAppPath);
		pFile->ReadInitInfo(strAppPath+"set.ini","Register","strAccType",strAccType,iTemp,0);
		pFile->ReadInitInfo(strAppPath+"set.ini","Register","strPwdType",strPwdType,iTemp,0);
		pFile->ReadInitInfo(strAppPath+"set.ini","Register","strMailType",strMailType,iTemp,0);
		pFile->ReadInitInfo(strAppPath+"set.ini","Register","genNum",strTemp,generateNum,1);
		pFile->CheckInitValue(generateNum,1,1000000);		
		pFile->ReadInitInfo(strAppPath+"set.ini","Register","type",strTemp,g_iDeleteFlag,1);
		pFile->CheckInitValue(g_iDeleteFlag,0,1);

		pFile->ReadInitInfo(strAppPath+"set.ini","Register","strIntro",strIntroMan,iTemp,0);
		pFile->ReadInitInfo(strAppPath+"set.ini","Register","width",strTemp,g_iShowWidth,1);
		pFile->ReadInitInfo(strAppPath+"set.ini","Register","height",strTemp,g_iShowHeight,1);
		pFile->CheckInitValue(g_iShowWidth,1,10000);
		pFile->CheckInitValue(g_iShowHeight,1,10000);
		pFile->ReadInitInfo(strAppPath+"set.ini","Register","autochangeip",strTemp,g_iAotuChangeIp,1);
		pFile->ReadInitInfo(strAppPath+"set.ini","Register","autocheck",strTemp,g_iAotuCheckCode,1);
		pFile->ReadInitInfo(strAppPath+"set.ini","Register","ipregnum",strTemp,g_iIpRegNum,1);
		pFile->CheckInitValue(g_iAotuChangeIp,0,1);
		pFile->CheckInitValue(g_iAotuCheckCode,0,1);
		pFile->CheckInitValue(g_iIpRegNum,1,100000000);
		m_AutoCheck.SetCheck(g_iAotuCheckCode);
		m_AutoIP.SetCheck(g_iAotuChangeIp);		
		SetDlgItemInt(IDC_EDIT_IPREGNUM,g_iIpRegNum);
		GetDlgItem(IDC_EDIT_IPREGNUM)->EnableWindow(g_iAotuChangeIp);
		CString strRasName = "";
		CString strRasAcc = "";
		CString strRasPwd = "";		
		pFile->ReadInitInfo(strAppPath+"set.ini","Register","rasname",strRasName,iTemp,0);
		pFile->ReadInitInfo(strAppPath+"set.ini","Register","rasacc",strRasAcc,iTemp,0);
		pFile->ReadInitInfo(strAppPath+"set.ini","Register","raspwd",strRasPwd,iTemp,0);
		SetDlgItemText(IDC_EDIT_RASNAME,strRasName);
		SetDlgItemText(IDC_EDIT_RASACC,strRasAcc);
		SetDlgItemText(IDC_EDIT_RASPWD,strRasPwd);

		pFile->ReadInitInfo(strAppPath+"set.ini","Register","ipchangetype",strTemp,g_iIpChangeType,1);
		pFile->CheckInitValue(g_iIpChangeType,0,1);
		pFile->ReadInitInfo(strAppPath+"set.ini","Register","timeout",strTemp,g_iTimeOut,1);
		pFile->CheckInitValue(g_iTimeOut,1,50);
		pFile->ReadInitInfo(strAppPath+"set.ini","Register","recheckipnum",strTemp,g_iReCheckIpCount,1);
		pFile->CheckInitValue(g_iReCheckIpCount,0,50);
		pFile->ReadInitInfo(strAppPath+"set.ini","Register","checkipfalg",strTemp,g_iCheckIpFlag,1);
		pFile->CheckInitValue(g_iCheckIpFlag,0,2);
		SetDlgItemInt(IDC_EDIT_TIMEOUT,g_iTimeOut);
		SetDlgItemInt(IDC_EDIT_CHECKIPCHANGENUM,g_iReCheckIpCount);
		if (g_iIpChangeType == 0)
		{
			OnBnClickedRadioIpchange1();
		}
		else
		{
			OnBnClickedRadioIpchange2();
		}
		switch (g_iCheckIpFlag)
		{
		case 1:
			OnBnClickedRadioCheckip1();
			break;
		case 2:
			OnBnClickedRadioCheckip2();
			break;
		case 0:
		default:
			OnBnClickedRadioCheckip3();
			break;
		}
		OnBnClickedCheckAutoip2();

		pFile->ReadInitInfo(strAppPath+"set.ini","Register","codechecktype",strTemp,g_iCheckCodeType,1);
		pFile->ReadInitInfo(strAppPath+"set.ini","Register","codeerrornum",strTemp,g_iCodeErrorNum,1);
		pFile->CheckInitValue(g_iCheckCodeType,0,1);
		pFile->CheckInitValue(g_iCodeErrorNum,1,100);
		((CButton*)GetDlgItem(IDC_CHECK_TEST))->SetCheck(g_iCheckCodeType);
		SetDlgItemInt(IDC_EDIT_CODEERRORNUM,g_iCodeErrorNum);
		OnBnClickedCheckAutocheckcode2();

		int iCheck = 0;
		CString strEailTail = "";
		CString strNickName = "";
		pFile->ReadInitInfo(strAppPath+"set.ini","Register","nicknametype",strTemp,iCheck,1);
		pFile->CheckInitValue(iCheck,0,1);		
		pFile->ReadInitInfo(strAppPath+"set.ini","Register","emailtail",strEailTail,iTemp,0);		
		pFile->ReadInitInfo(strAppPath+"set.ini","Register","nickname",strNickName,iTemp,0);
		SetDlgItemText(IDC_EDIT_MAILTAIL,strEailTail);
		SetDlgItemText(IDC_EDIT_NICKNAME,strNickName);
		((CButton*)GetDlgItem(IDC_CHECK_NAMEADD))->SetCheck(iCheck);
		CString strDirect = "";
		CString strFileName = "";		
		pFile->ReadInitInfo(strAppPath+"set.ini","Register","direct",strDirect,iTemp,0);		
		pFile->ReadInitInfo(strAppPath+"set.ini","Register","filename",strFileName,iTemp,0);
		if (strFileName == "")
		{
			strFileName = "checkcode.bmp";
		}
		if (strDirect == "")
		{
			strDirect = "c:\\";
		}
		SetDlgItemText(IDC_EDIT_PICSAVENAME,strFileName);
		SetDlgItemText(IDC_EDIT_PICPATH,strDirect);
		
		pFile->ReadInitInfo(strAppPath+"set.ini","Register","webflag",strTemp,g_iRegWebFlag,1);
		pFile->CheckInitValue(g_iRegWebFlag,0,2);
		if (g_iRegWebFlag == 0)
		{
			OnBnClickedRadioLongyan();
		}
		else if (g_iRegWebFlag == 1)
		{
			OnBnClickedRadioPutian();
		}
		else
		{
			OnBnClickedRadioShaowu();
		}
		pFile->ReadInitInfo(strAppPath+"set.ini","Register","macchange",strTemp,g_iIsChangeMac,1);
		pFile->CheckInitValue(g_iIsChangeMac,0,1);
		((CButton*)GetDlgItem(IDC_CHECK_CHANGEMAC))->SetCheck(g_iIsChangeMac);
		
		pFile->ReadInitInfo(strAppPath+"set.ini","Register","shutstop",strTemp,g_iRedialStop,1);
		pFile->CheckInitValue(g_iRedialStop,1,20000);
		SetDlgItemInt(IDC_EDIT_SHUTSTOP,g_iRedialStop);
		
	}
	SetDlgItemText(IDC_EDIT_ACC,strAccType);
	SetDlgItemText(IDC_EDIT_PWD,strPwdType);
	SetDlgItemText(IDC_EDIT_MAILACC,strMailType);
	SetDlgItemText(IDC_EDIT_INTROMAN,strIntroMan);
	SetDlgItemInt(IDC_EDIT_COUNTNUM,generateNum);
	SetDlgItemInt(IDC_EDIT_SHOWWIDTH,g_iShowWidth);
	SetDlgItemInt(IDC_EDIT_SHOWHEIGHT,g_iShowHeight);
	((CButton*)GetDlgItem(IDC_CHECK_CHECKERROR))->SetCheck(g_iDeleteFlag);
	return TRUE;  // ���������˿ؼ��Ľ��㣬���򷵻� TRUE
}

//�����˺�
void CCountGen::OnBnClickedBtnGencount()
{
	if(GenerateCounts()<0)
	{
		return;
	}
	CString msgBuf	 = "�˺����ɳɹ�";
	CUserMessagePump *pMsgPump = NULL;
	pMsgPump = CUserMessagePump::GetInstance();
	if (!pMsgPump)
	{
		return;
	}
	pMsgPump->AddToMessagePump(USERWINDOWMSGSUCCESS,msgBuf);
	PostMessage(WM_CLOSE,0,0);
}


/*********************************
 *��ʾ��ĸ
 ?��ʾ����
 ����Ϊ0 (�˺Ž������»��ߣ��Ҳ����ڵ�һλ)
 ���뾡���������ּ���ĸ
****************************************/
int CCountGen::GenerateCounts()
{
	CString strAccType = "";
	CString strPwdType = "";
	CString strMailType = "";
	CString strTemp = "";
	CString strAcc = "";
	CString strPwd = "";
	CString strMail = "";
	CString strIntroMan = "";
	CString strIntroManEx = "";
	CString strNickNameEx = "";
	int generateNum = 0;	
	CString mailtype = "@china.com.cn";
	GetDlgItemText(IDC_EDIT_ACC,strAccType);
	GetDlgItemText(IDC_EDIT_PWD,strPwdType);
	GetDlgItemText(IDC_EDIT_MAILACC,strMailType);
	generateNum = GetDlgItemInt(IDC_EDIT_COUNTNUM);
	g_iShowWidth = GetDlgItemInt(IDC_EDIT_SHOWWIDTH);
	g_iShowHeight = GetDlgItemInt(IDC_EDIT_SHOWHEIGHT);
	GetDlgItemText(IDC_EDIT_INTROMAN,strIntroMan);
	strAccType.Trim();
	strPwdType.Trim();
	strMailType.Trim();
	strIntroMan.Trim();
	int iCheck = ((CButton*)GetDlgItem(IDC_CHECK_NAMEADD))->GetCheck();
	GetDlgItemText(IDC_EDIT_MAILTAIL,mailtype);
	CString strNickName = "";
	GetDlgItemText(IDC_EDIT_NICKNAME,strNickName);
	strNickName.Trim();
	mailtype.Trim();	
	//��������
	if (SaveParams() == -1)
	{
		return -1;
	}
	CString tempStr = "";		//��ʱ����
	srand(time(NULL));
	for (int i=0;i< generateNum;)
	{
		char inChar;
		char outChar;
		int iLen = 0;
		strAcc = "";
		strPwd = "";
		strMail = "";
		strIntroManEx = "";
		strNickNameEx = "";
		int j = 0;
		iLen = strIntroMan.GetLength();
		for (j=0;j<iLen;j++)		//����֤
		{
			inChar = strIntroMan.GetAt(j);
			outChar = RandChar(inChar);
			tempStr.Format("%c",outChar);
			strIntroManEx += tempStr;
		}
		strTemp.Format("%c",DoVerify(strIntroManEx));
		if (strTemp.MakeLower() =="x")
		{
			continue;
		}
		strIntroManEx += strTemp;

		iLen = strAccType.GetLength();
		for (j=0;j<iLen;j++)		//�˺�
		{
			inChar = strAccType.GetAt(j);
			outChar = RandChar(inChar);
			tempStr.Format("%c",outChar);
			strAcc += tempStr;
		}
		iLen = strPwdType.GetLength();
		for (j=0;j<iLen;j++)		//����
		{

			inChar = strPwdType.GetAt(j);
			outChar = RandChar(inChar);
			tempStr.Format("%c",outChar);
			strPwd += tempStr;
		}
		iLen = strMailType.GetLength();
		for (j=0;j<iLen;j++)		//����
		{

			inChar = strMailType.GetAt(j);
			outChar = RandChar(inChar);
			tempStr.Format("%c",outChar);
			strMail += tempStr;
		}
		strMail += mailtype;
		
		iLen = strNickName.GetLength();
		for (j=0;j<iLen;j++)		//����
		{

			inChar = strNickName.GetAt(j);
			outChar = RandChar(inChar);
			tempStr.Format("%c",outChar);
			strNickNameEx += tempStr;
		}
		if (iCheck == 1)
		{
			strNickNameEx = strAcc + strNickNameEx;
		}		
		RegisterPack rgPack;
		rgPack.strAcc = strAcc;					//�˺�
		rgPack.strPwd = strPwd;					//����
		rgPack.strMail = strMail;				//����
		rgPack.strNickName = strNickNameEx;		//�ǳ�
		rgPack.strIntroMan = strIntroManEx;		//����֤��
		AddRegisterPack(rgPack);
		int nCount = m_pList->GetItemCount();
		CString listIndex = "";
		listIndex.Format("%d",nCount);
		m_pList->InsertItem(nCount,"",0);
		m_pList->SetItemText(nCount,0,listIndex);
		m_pList->SetItemText(nCount,1,strAcc);
		m_pList->SetItemText(nCount,2,strPwd);
		m_pList->SetItemText(nCount,3,strMail);
		m_pList->SetItemText(nCount,4,strIntroManEx);
		m_pList->SetItemText(nCount,7,strNickNameEx);
		m_pList->SetItemText(nCount,8,"");
		i++;
	}
	return 0;
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
char CCountGen::RandChar(char inChar)
{
	char outChar;
	if (inChar == '?')	//����
	{
		outChar = rand() % 10 + 48;
	}
	else if (inChar == '*') //��ĸ
	{
		int num = rand() % 26;
		outChar = num + 97;
	}
	else
	{
		int num = (int)inChar;
		if ((num >= 48 && num <=57)   ||
			(num >= 65 && num <=90) ||
			(num >= 97 && num <=122)||
			inChar == '_')
		{
			outChar = inChar;
		}
		else
		{
			outChar = 48;
		}
	}
	return outChar;
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CCountGen::OnBnClickedBtnSave()
{
	if (SaveParams() < 0)
	{
		return;
	}	
	PostMessage(WM_CLOSE,0,0);
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
int CCountGen::SaveParams()
{
	CString strAccType = "";
	CString strPwdType = "";
	CString strMailType = "";

	CString strAcc = "";
	CString strPwd = "";
	CString strMail = "";
	CString strIntroMan = "";	
	int generateNum = 0;
	CString mailtype = "@china.com.cn";
	GetDlgItemText(IDC_EDIT_ACC,strAccType);
	GetDlgItemText(IDC_EDIT_PWD,strPwdType);
	GetDlgItemText(IDC_EDIT_MAILACC,strMailType);
	generateNum = GetDlgItemInt(IDC_EDIT_COUNTNUM);
	g_iShowWidth = GetDlgItemInt(IDC_EDIT_SHOWWIDTH);
	g_iShowHeight = GetDlgItemInt(IDC_EDIT_SHOWHEIGHT);
	GetDlgItemText(IDC_EDIT_INTROMAN,strIntroMan);
	strAccType.Trim();
	strPwdType.Trim();
	strMailType.Trim();
	strIntroMan.Trim();
	CString strNickName = "";
	GetDlgItemText(IDC_EDIT_NICKNAME,strNickName);
	GetDlgItemText(IDC_EDIT_MAILTAIL,mailtype);
	strNickName.Trim();
	mailtype.Trim();
	if (strAccType == "" || strPwdType == "" || strMailType == "" || mailtype=="" )
	{
		MessageBox("���������ɸ�ʽ��Ϣ","����");
		return -1;
	}
	if (strIntroMan.GetLength() != 17)
	{
		MessageBox("����д����֤��ǰ17λ","����");
		return -1;
	}
	CString strDirect = "";
	CString strFileName = "";
	GetDlgItemText(IDC_EDIT_PICPATH,strDirect);
	GetDlgItemText(IDC_EDIT_PICSAVENAME,strFileName);
	strDirect.Trim();
	strFileName.Trim();
	if (strFileName == BMPFILE_PATH)
	{
		MessageBox("����д�����ļ���","����");
		return -1;
	}
	CString strRasName = "";
	CString strRasAcc = "";
	CString strRasPwd = "";
	GetDlgItemText(IDC_EDIT_RASNAME,strRasName);
	GetDlgItemText(IDC_EDIT_RASACC,strRasAcc);
	GetDlgItemText(IDC_EDIT_RASPWD,strRasPwd);
	g_RasMgr.SetParams(strRasAcc,strRasPwd,strRasName);
	g_iDeleteFlag = ((CButton*)GetDlgItem(IDC_CHECK_CHECKERROR))->GetCheck();
	g_iIpRegNum = GetDlgItemInt(IDC_EDIT_IPREGNUM);
	//��������
	CString strAppPath = "";
	CString strTemp = "";
	CUserFile *pFile = CUserFile::GetInstance();
	if (pFile)
	{
		pFile->GetAppPath(strAppPath);
		pFile->WriteInitInfo(strAppPath+"set.ini","Register","strAccType",strAccType,0,0);
		pFile->WriteInitInfo(strAppPath+"set.ini","Register","strPwdType",strPwdType,0,0);
		pFile->WriteInitInfo(strAppPath+"set.ini","Register","strMailType",strMailType,0,0);
		pFile->WriteInitInfo(strAppPath+"set.ini","Register","strIntro",strIntroMan,0,0);
		pFile->WriteInitInfo(strAppPath+"set.ini","Register","genNum","",generateNum,1);
		pFile->WriteInitInfo(strAppPath+"set.ini","Register","type","",g_iDeleteFlag,1);
		pFile->WriteInitInfo(strAppPath+"set.ini","Register","width","",g_iShowWidth,1);
		pFile->WriteInitInfo(strAppPath+"set.ini","Register","height","",g_iShowHeight,1);
		pFile->WriteInitInfo(strAppPath+"set.ini","Register","autochangeip",strTemp,g_iAotuChangeIp,1);
		pFile->WriteInitInfo(strAppPath+"set.ini","Register","autocheck",strTemp,g_iAotuCheckCode,1);
		pFile->WriteInitInfo(strAppPath+"set.ini","Register","ipregnum",strTemp,g_iIpRegNum,1);
		pFile->WriteInitInfo(strAppPath+"set.ini","Register","rasname",strRasName,0,0);
		pFile->WriteInitInfo(strAppPath+"set.ini","Register","rasacc",strRasAcc,0,0);
		pFile->WriteInitInfo(strAppPath+"set.ini","Register","raspwd",strRasPwd,0,0);
		//��ȡIP��ⷽʽ
		int iCheck = 0;
		iCheck = ((CButton*)GetDlgItem(IDC_RADIO_IPCHANGE2))->GetCheck();

		g_iIpChangeType = iCheck;		
        g_iTimeOut = GetDlgItemInt(IDC_EDIT_TIMEOUT);
		g_iReCheckIpCount = GetDlgItemInt(IDC_EDIT_CHECKIPCHANGENUM);
		iCheck = ((CButton*)GetDlgItem(IDC_RADIO_CHECKIP1))->GetCheck();
		if (iCheck == 1)
		{
			g_iCheckIpFlag = 1;
		}
		else if (((CButton*)GetDlgItem(IDC_RADIO_CHECKIP2))->GetCheck()==1)
		{
			g_iCheckIpFlag = 2;
		}
		else if (((CButton*)GetDlgItem(IDC_RADIO_CHECKIP3))->GetCheck()==1)
		{
			g_iCheckIpFlag = 0;
		}
		else
		{
			g_iCheckIpFlag = 0;
		}
		pFile->WriteInitInfo(strAppPath+"set.ini","Register","ipchangetype",strTemp,g_iIpChangeType,1);
		pFile->WriteInitInfo(strAppPath+"set.ini","Register","timeout",strTemp,g_iTimeOut,1);
		pFile->WriteInitInfo(strAppPath+"set.ini","Register","recheckipnum",strTemp,g_iReCheckIpCount,1);
		pFile->WriteInitInfo(strAppPath+"set.ini","Register","checkipfalg",strTemp,g_iCheckIpFlag,1);
		g_iCheckCodeType = ((CButton*)GetDlgItem(IDC_CHECK_TEST))->GetCheck();
		g_iCodeErrorNum = GetDlgItemInt(IDC_EDIT_CODEERRORNUM);
		pFile->WriteInitInfo(strAppPath+"set.ini","Register","codechecktype",strTemp,g_iCheckCodeType,1);
		pFile->WriteInitInfo(strAppPath+"set.ini","Register","codeerrornum",strTemp,g_iCodeErrorNum,1);
		iCheck = ((CButton*)GetDlgItem(IDC_CHECK_NAMEADD))->GetCheck();
		pFile->WriteInitInfo(strAppPath+"set.ini","Register","nicknametype",strTemp,iCheck,1);
		CString strEailTail = "";
		GetDlgItemText(IDC_EDIT_MAILTAIL,strEailTail);
		pFile->WriteInitInfo(strAppPath+"set.ini","Register","emailtail",strEailTail,0,0);
		CString strNickName = "";
		GetDlgItemText(IDC_EDIT_NICKNAME,strNickName);
		pFile->WriteInitInfo(strAppPath+"set.ini","Register","nickname",strNickName,0,0);		
		pFile->WriteInitInfo(strAppPath+"set.ini","Register","direct",strDirect,0,0);		
		pFile->WriteInitInfo(strAppPath+"set.ini","Register","filename",strFileName,0,0);
		iCheck = ((CButton*)GetDlgItem(IDC_RADIO_LONGYAN))->GetCheck();
		if (iCheck == 1)
		{
			g_iRegWebFlag = 0;
		}
		else if (((CButton*)GetDlgItem(IDC_RADIO_PUTIAN))->GetCheck()==1)
		{
			g_iRegWebFlag = 1;
		}
		else if (((CButton*)GetDlgItem(IDC_RADIO_SHAOWU))->GetCheck()==1)
		{
			g_iRegWebFlag = 2;
		}
		else
		{
			g_iRegWebFlag = 2;
		}
		pFile->WriteInitInfo(strAppPath+"set.ini","Register","webflag","",g_iRegWebFlag,1);
		g_strDirect = strDirect;
		g_strFileName = strFileName;
		g_iIsChangeMac = ((CButton*)GetDlgItem(IDC_CHECK_CHANGEMAC))->GetCheck();
		pFile->WriteInitInfo(strAppPath+"set.ini","Register","macchange","",g_iIsChangeMac,1);
		g_iRedialStop = GetDlgItemInt(IDC_EDIT_SHUTSTOP);
		pFile->CheckInitValue(g_iRedialStop,1,20000);
		pFile->WriteInitInfo(strAppPath+"set.ini","Register","shutstop","",g_iRedialStop,1);
	}
	return 0;
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CCountGen::OnBnClickedCheckAutoip2()
{
	g_iAotuChangeIp = m_AutoIP.GetCheck();
	int iCheck = ((CButton*)GetDlgItem(IDC_RADIO_IPCHANGE1))->GetCheck();
	GetDlgItem(IDC_RADIO_IPCHANGE1)->EnableWindow(g_iAotuChangeIp);
	GetDlgItem(IDC_RADIO_IPCHANGE2)->EnableWindow(g_iAotuChangeIp);
	GetDlgItem(IDC_EDIT_IPREGNUM)->EnableWindow(g_iAotuChangeIp);
	GetDlgItem(IDC_EDIT_RASNAME)->EnableWindow(g_iAotuChangeIp&&iCheck);
	GetDlgItem(IDC_EDIT_RASACC)->EnableWindow(g_iAotuChangeIp&&iCheck);
	GetDlgItem(IDC_EDIT_RASPWD)->EnableWindow(g_iAotuChangeIp&&iCheck);
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CCountGen::OnBnClickedCheckAutocheckcode2()
{
	g_iAotuCheckCode = m_AutoCheck.GetCheck();
	GetDlgItem(IDC_CHECK_TEST)->EnableWindow(g_iAotuCheckCode);
}
/************************************************************************/
/*�������һλУ����                                                    */
/************************************************************************/
char CCountGen::DoVerify(const char* pszSrc)
{
	int iS = 0;
	int iW[]={7, 9, 10, 5, 8, 4, 2, 1, 6, 3, 7, 9, 10, 5, 8, 4, 2};
	static char szVerCode[]="10X98765432";
	int i;
	for(i=0;i<17;i++)
	{
		iS += (int)(pszSrc[i]-'0') * iW[i];
	}
	int iY = iS%11;
	return szVerCode[iY];
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CCountGen::OnBnClickedRadioIpchange1()
{
	((CButton*)GetDlgItem(IDC_RADIO_IPCHANGE1))->SetCheck(1);
	((CButton*)GetDlgItem(IDC_RADIO_IPCHANGE2))->SetCheck(0);
	GetDlgItem(IDC_EDIT_RASNAME)->EnableWindow(1&&g_iAotuChangeIp);
	GetDlgItem(IDC_EDIT_RASACC)->EnableWindow(1&&g_iAotuChangeIp);
	GetDlgItem(IDC_EDIT_RASPWD)->EnableWindow(1&&g_iAotuChangeIp);
	GetDlgItem(IDC_BUTTON3)->EnableWindow(0);
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CCountGen::OnBnClickedRadioIpchange2()
{
	((CButton*)GetDlgItem(IDC_RADIO_IPCHANGE1))->SetCheck(0);
	((CButton*)GetDlgItem(IDC_RADIO_IPCHANGE2))->SetCheck(1);	
	GetDlgItem(IDC_BUTTON3)->EnableWindow(1);
	GetDlgItem(IDC_EDIT_RASACC)->EnableWindow(0);
	GetDlgItem(IDC_EDIT_RASPWD)->EnableWindow(0);
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CCountGen::OnBnClickedRadioCheckip1()
{
	((CButton*)GetDlgItem(IDC_RADIO_CHECKIP1))->SetCheck(1);
	((CButton*)GetDlgItem(IDC_RADIO_CHECKIP2))->SetCheck(0);
	((CButton*)GetDlgItem(IDC_RADIO_CHECKIP3))->SetCheck(0);
	GetDlgItem(IDC_EDIT_CHECKIPCHANGENUM)->EnableWindow(1);
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CCountGen::OnBnClickedRadioCheckip2()
{
	((CButton*)GetDlgItem(IDC_RADIO_CHECKIP1))->SetCheck(0);
	((CButton*)GetDlgItem(IDC_RADIO_CHECKIP2))->SetCheck(1);
	((CButton*)GetDlgItem(IDC_RADIO_CHECKIP3))->SetCheck(0);
	GetDlgItem(IDC_EDIT_CHECKIPCHANGENUM)->EnableWindow(1);
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CCountGen::OnBnClickedRadioCheckip3()
{
	((CButton*)GetDlgItem(IDC_RADIO_CHECKIP1))->SetCheck(0);
	((CButton*)GetDlgItem(IDC_RADIO_CHECKIP2))->SetCheck(0);
	((CButton*)GetDlgItem(IDC_RADIO_CHECKIP3))->SetCheck(1);
	GetDlgItem(IDC_EDIT_CHECKIPCHANGENUM)->EnableWindow(0);
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CCountGen::OnBnClickedBtnFind()
{
	CString sFolderPath;
	BROWSEINFO bi;
	char Buffer[MAX_PATH]; //��ʼ����ڲ���bi��ʼ
	bi.hwndOwner = NULL;
	bi.pidlRoot = NULL;
	bi.pszDisplayName = Buffer; //�˲�����ΪNULL������ʾ�Ի���
	bi.lpszTitle = "ѡ��·��";
	bi.ulFlags = BIF_RETURNONLYFSDIRS;
	bi.lpfn = NULL;
	bi.iImage = 0;   //��ʼ����ڲ���bi����
	LPITEMIDLIST pIDList = SHBrowseForFolder(&bi);//������ʾѡ��Ի���
	if(pIDList)
	{
		SHGetPathFromIDList(pIDList, Buffer);
		//ȡ���ļ���·����Buffer��
		sFolderPath.Format("%s",Buffer);//��·��������һ��CString������
		if (sFolderPath.GetLength() > 0)
		{
			if (sFolderPath.Right(1) != "\\")
			{
				sFolderPath +="\\";
			}
		}
	}
	SetDlgItemText(IDC_EDIT_PICPATH,sFolderPath);
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CCountGen::OnBnClickedRadioLongyan()
{
	((CButton*)GetDlgItem(IDC_RADIO_LONGYAN))->SetCheck(1);
	((CButton*)GetDlgItem(IDC_RADIO_SHAOWU))->SetCheck(0);
	((CButton*)GetDlgItem(IDC_RADIO_PUTIAN))->SetCheck(0);
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CCountGen::OnBnClickedRadioShaowu()
{
	((CButton*)GetDlgItem(IDC_RADIO_LONGYAN))->SetCheck(0);
	((CButton*)GetDlgItem(IDC_RADIO_SHAOWU))->SetCheck(1);
	((CButton*)GetDlgItem(IDC_RADIO_PUTIAN))->SetCheck(0);
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CCountGen::OnBnClickedRadioPutian()
{
	((CButton*)GetDlgItem(IDC_RADIO_LONGYAN))->SetCheck(0);
	((CButton*)GetDlgItem(IDC_RADIO_SHAOWU))->SetCheck(0);
	((CButton*)GetDlgItem(IDC_RADIO_PUTIAN))->SetCheck(1);
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CCountGen::OnBnClickedCheckChangemac()
{
}

void CCountGen::OnBnClickedButton1()
{
	CString strAcc="";
	CString strPwd="";
	CString strPhone = "";
	GetDlgItemText(IDC_EDIT_PICSAVENAME,strPhone);
	strPhone.Trim();
	if (strPhone == "")
	{
		MessageBox("��ѯʱ��������Ϊ��,��һ����д!");
		return;
	}
	//g_iFreshCount = atoi(strPhone);	//ʱ����
	if (SaveParams() < 0)
	{
		return;
	}
	if (g_iAotuChangeIp== 1 && gl_ProxyStruList.GetCount() < 1)
	{
		MessageBox("��ѡ�����л�����,���ȵ���VPN�����б�");
		return;
	}
	CString strRecommandPhone = "";
	int   lines   =   0;   
	CStdioFile   file;
	CString   cstr;
	CFileException   fe;
	CString strFilePath = "";
	char szFilter[]="Text files(*.txt)|*.txt";
	CFileDialog dlg(1,NULL,NULL,NULL,szFilter); 
	if(dlg.DoModal()==IDOK) 
	{
		strFilePath = dlg.GetPathName();
	}
	if (strFilePath=="")
	{
		return;
	}
	if(file.Open(strFilePath,CFile::modeRead,&fe))   
	{ 
		file.SeekToBegin();
		int idx1=0;
		int idx2=0;
		srand(time(NULL));
		while (file.ReadString(cstr))
		{
			cstr.Replace("     ",",");
			cstr.Replace("    ",",");
			cstr.Replace("   ",",");
			cstr.Replace("  ",",");
			cstr.Replace(" ",",");
			cstr.Replace("\t\t",",");
			cstr.Replace("\t",",");
			cstr+=",";
			strAcc = "";
			strPwd = "";
			//strPhone = "";
			strRecommandPhone = "";
			idx2 =cstr.Find(",");
			if (idx2==-1)
			{
				continue;
			}
			strAcc = cstr.Left(idx2);
			cstr = cstr.Right(cstr.GetLength()-idx2-1);
			idx2 =cstr.Find(",");
			if (idx2==-1)
			{
				continue;
			}
			strPwd = cstr.Left(idx2);

			map<CString,int>::iterator it = gl_mapOldAcc.find(strAcc);
			if (it != gl_mapOldAcc.end())
			{
				CString strShow = "";
				strShow.Format("%s,%s �Ѿ��Դ��˺���������",strAcc,strPwd);
				CUserMessagePump *pMsgPump = NULL;
				pMsgPump = CUserMessagePump::GetInstance();
				if (!pMsgPump)
				{
					return;
				}
				pMsgPump->AddToMessagePump(USERWINDOWMSG_1,strShow);
				continue;
			}
			//�Ѿ��ҵ�������Ҫ������
			if (strAcc !="")
			{
				int nCount = m_pList->GetItemCount();
				CString listIndex = "";
				int iIsExist = 0;
				map<CString,int>::iterator it = gl_mapListAcc.find(strAcc);
				if (it != gl_mapListAcc.end())
				{
					CString strShow = "";
					strShow.Format("%s,%s �ظ�����",strAcc,strPwd);
					CUserMessagePump *pMsgPump = NULL;
					pMsgPump = CUserMessagePump::GetInstance();
					if (!pMsgPump)
					{
						return;
					}
					pMsgPump->AddToMessagePump(USERWINDOWMSG_1,strShow);
					continue;
				}


				if (iIsExist == 1)
				{
					continue;
				}
				gl_mapListAcc[strAcc] = nCount;
				RegisterPack rgPack;
				rgPack.strAcc = strAcc;					//����
				rgPack.strPwd = strPwd;					//����
				rgPack.strNickName = strPhone;			//��ֵ�˺�
				rgPack.strMail = "";
				AddRegisterPack(rgPack);
				nCount = m_pList->GetItemCount();
				listIndex.Format("%d",nCount);
				m_pList->InsertItem(nCount,"",0);
				m_pList->SetItemText(nCount,0,listIndex);
				m_pList->SetItemText(nCount,1,strAcc);
				m_pList->SetItemText(nCount,2,strPwd);							
			}
		}
		file.Close();
	}
	::PostMessage(gl_hwnd,WM_USER + 111,0,0);
	PostMessage(WM_CLOSE,0,0);
}

void CCountGen::OnBnClickedButton3()
{
	CString strAcc="";
	CString strPwd="";
	CString strPhone = "";
	GetDlgItemText(IDC_EDIT_PICSAVENAME,strPhone);
	strPhone.Trim();
	if (strPhone == "")
	{
		MessageBox("��ѯʱ��������Ϊ��,��һ����д!");
		return;
	}
	//g_iFreshCount = atoi(strPhone);	//ʱ����
	if (SaveParams() < 0)
	{
		return;
	}
	CString strRecommandPhone = "";
	int   lines   =   0;   
	CStdioFile   file;
	CString   cstr;
	CFileException   fe;
	CString strFilePath = "";
	char szFilter[]="Text files(*.txt)|*.txt";
	CFileDialog dlg(1,NULL,NULL,NULL,szFilter); 
	if(dlg.DoModal()==IDOK) 
	{
		strFilePath = dlg.GetPathName();
	}
	if (strFilePath=="")
	{
		return;
	}
	if(file.Open(strFilePath,CFile::modeRead,&fe))   
	{ 
		file.SeekToBegin();
		int idx1=0;
		int idx2=0;
		srand(time(NULL));
		while (file.ReadString(cstr))
		{
			cstr.Replace("     ",",");
			cstr.Replace("    ",",");
			cstr.Replace("   ",",");
			cstr.Replace("  ",",");
			cstr.Replace(" ",",");
			cstr.Replace("\t\t",",");
			cstr.Replace("\t",",");
			cstr+=",";
			strAcc = "";
			strPwd = "";
			//strPhone = "";
			strRecommandPhone = "";
			idx2 =cstr.Find(",");
			if (idx2==-1)
			{
				continue;
			}
			CString strServer = cstr.Left(idx2);
			cstr = cstr.Right(cstr.GetLength()-idx2-1);
			idx2 =cstr.Find(",");
			if (idx2==-1)
			{
				continue;
			}
			strAcc = cstr.Left(idx2);
			cstr = cstr.Right(cstr.GetLength()-idx2-1);
			idx2 =cstr.Find(",");

			if (idx2==-1)
			{
				continue;
			}
			strPwd = cstr.Left(idx2);
			if (strServer !="")
			{
				VpnInfoStr objStr;
				objStr.strServer = strServer;
				objStr.strUser = strAcc;
				objStr.strPwd = strPwd;
				gl_ProxyStruList.Add(objStr);
			}
		}
		file.Close();
	}
	CString strShow = "";
	strShow.Format("���� %d ������",gl_ProxyStruList.GetCount());
	CUserMessagePump *pMsgPump = NULL;
	pMsgPump = CUserMessagePump::GetInstance();
	if (!pMsgPump)
	{
		return;
	}
	pMsgPump->AddToMessagePump(USERWINDOWMSG_1,strShow);
}
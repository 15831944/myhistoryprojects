// AmazonWrapperDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "AmazonWrapper.h"
#include "AmazonWrapperDlg.h"
#include <direct.h>
#include "../AmazonLib/AmazonClass.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���


class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CAmazonWrapperDlg �Ի���




CAmazonWrapperDlg::CAmazonWrapperDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAmazonWrapperDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CAmazonWrapperDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_PAY, m_com_paytype);
	DDX_Control(pDX, IDC_COMBO_INVOICE, m_cob_billcontent);
	DDX_Control(pDX, IDC_COMBO_SHIPTYPE, m_comb_shiptype);
	DDX_Control(pDX, IDC_COMBO_SENDTIME, m_combo_sendtime);
}

BEGIN_MESSAGE_MAP(CAmazonWrapperDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON1, &CAmazonWrapperDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CAmazonWrapperDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CAmazonWrapperDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CAmazonWrapperDlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, &CAmazonWrapperDlg::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_RADIO1, &CAmazonWrapperDlg::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO2, &CAmazonWrapperDlg::OnBnClickedRadio2)
END_MESSAGE_MAP()


// CAmazonWrapperDlg ��Ϣ�������

BOOL CAmazonWrapperDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}


	GoodOrderStru objStr;
	objStr.strUserID = "58357601@qq.com";
	objStr.strPwd = "shizisen861225";


	CString strPath = "c:\\test";
	if (_access(strPath,0) != 0)
	{
		mkdir(strPath);
	}
	objStr.strUserID = "zisen";
	objStr.strPwd = "5880186";
	objStr.strRecver = "�۾�";
	objStr.strAddr = "�������򳤹��Ӵ�";
	objStr.strProvince = "����";
	objStr.strCity = "������";
	objStr.strTown = "��ɽ��";
	objStr.strPostCode = "632725";
	objStr.strPhone = "15019212360";
	objStr.bBill = true;
	objStr.strBillHeader = "��㿩";
	objStr.strBillContent = "�ҵ�";
	objStr.eSendTime = NightOrOther;
	objStr.eQueryType = Last30;
	objStr.iShowFlag = 1;
	objStr.strOrderNo = "C02-3760340-2099800";

	objStr.strUserID = "test33223@qq.com";
	objStr.strPwd = "123456";
	objStr.strRecver = "�ź�";
	objStr.strAddr = "ܽ����· 368�� bobo���³� 2��2501��";

//#define BEIJING_ADDR
#ifdef BEIJING_ADDR
	objStr.strProvince = "����";
	objStr.strCity = "������";
	objStr.strTown = "������";
	objStr.strPostCode = "100029";
#else
	objStr.strProvince = "�㶫";
	objStr.strCity = "������";
	objStr.strTown = "��ɽ��";
	objStr.strPostCode = "518025";
#endif

	//����    ��ɳ��   ������   ܽ����· 368�� bobo���³� 2��2501��  B003RCK84Q
	/*
	objStr.strProvince = "����";
	objStr.strCity = "��ɳ��";
	objStr.strTown = "������";
	objStr.strPostCode = "518025";
	*/
	objStr.strPhone = "18980889086";
	objStr.strPayType = "֧����";
	SetDlgItemText(IDC_EDITORDERNO,objStr.strOrderNo);
	SetDlgItemText(IDC_EDITUSER,objStr.strUserID);
	SetDlgItemText(IDC_EDITPWD,objStr.strPwd);
	SetDlgItemText(IDC_EDITNO,objStr.strGoodNos);
	SetDlgItemText(IDC_EDITNAME,objStr.strRecver);
	SetDlgItemText(IDC_EDITPHONE,objStr.strPhone);
	SetDlgItemText(IDC_EDITADDR,objStr.strAddr);
	SetDlgItemText(IDC_EDITPRO,objStr.strProvince);
	SetDlgItemText(IDC_EDITCITY,objStr.strCity);
	SetDlgItemText(IDC_EDITTOWN,objStr.strTown);
	SetDlgItemText(IDC_EDITPOSTCODE,objStr.strPostCode);
	SetDlgItemText(IDC_EDITHEADER,objStr.strBillHeader);
	SetDlgItemText(m_cob_billcontent.GetDlgCtrlID(), objStr.strBillContent);
	SetDlgItemText(m_com_paytype.GetDlgCtrlID(), objStr.strPayType);
	SetDlgItemText(IDC_EDIT_EMAIL,"shaojun.hnist@gmail.com");
	SetDlgItemText(IDC_EDITNO,"B003RCK84Q");//0140623361";//B002YGSOS8//1594483078
	((CButton*)GetDlgItem(IDC_CHECKBILL))->SetCheck(objStr.bBill);
	SetDlgItemInt(IDC_EDITTYPE,objStr.eSendTime);
	SetDlgItemInt(IDC_EDITQUERYTYPE,objStr.eQueryType);
	((CButton*)GetDlgItem(IDC_CHECKSHOW))->SetCheck(objStr.iShowFlag);

	((CButton*)GetDlgItem(IDC_CHECKDEBUG))->SetCheck(objStr.iDebug);
	//OnBnClickedRadio1();

	m_cob_billcontent.MoveWindow(235, 261, 100, 250);
	m_com_paytype.MoveWindow(70, 228, 100, 250);

	m_comb_shiptype.MoveWindow(70, 200, 100, 250);
	m_combo_sendtime.MoveWindow(235, 200, 100, 250);

	vector<CString> vecData;
	CFunction::InitInfo(AMAZON_INVOICE_CONTENT, vecData);
	vector<CString>::iterator it;
	for (it = vecData.begin();it != vecData.end();it++)
	{
		m_cob_billcontent.AddString(*it);
	}
	CFunction::InitInfo(AMAZON_PAY_INFO, vecData);
	for (it = vecData.begin();it != vecData.end();it++)
	{
		m_com_paytype.AddString(*it);
	}
	m_comb_shiptype.AddString("ƽ��");
	m_comb_shiptype.AddString("EMS");
	m_comb_shiptype.AddString("���");
	m_comb_shiptype.AddString("�Ӽ����");

	m_combo_sendtime.AddString("ֻ�������ͻ�");
	m_combo_sendtime.AddString("ֻ˫����,�����ͻ�");
	m_combo_sendtime.AddString("���ϻ�����ʱ��");
	m_combo_sendtime.AddString("�κ�ʱ��");
	
	m_combo_sendtime.SetCurSel(0);
	m_comb_shiptype.SetCurSel(2);
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��
	((CButton*)GetDlgItem(IDC_CHECK_TOGETHER))->SetCheck(TRUE);
	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
BOOL CAmazonWrapperDlg::PreTranslateMessage( MSG* pMsg )
{
	if( pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE)
	{
		return TRUE;      //���ε�Esc��
	}
	if( pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
	{
		return TRUE;      //���ε�Enter��
	}
	return CDialog::PreTranslateMessage(pMsg);
}

void CAmazonWrapperDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CAmazonWrapperDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ��������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù����ʾ��
//
HCURSOR CAmazonWrapperDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CAmazonWrapperDlg::OnBnClickedButton1()
{
	GoodOrderStru objStr;
	CString strGoods = "";
	GetDlgItemText(IDC_EDITUSER,objStr.strUserID);
	GetDlgItemText(IDC_EDITPWD,objStr.strPwd);
	GetDlgItemText(IDC_EDITNO,strGoods);
	GetDlgItemText(IDC_EDITNAME,objStr.strRecver);
	GetDlgItemText(IDC_EDITPHONE,objStr.strPhone);
	GetDlgItemText(IDC_EDITADDR,objStr.strAddr);
	GetDlgItemText(IDC_EDITPRO,objStr.strProvince);
	GetDlgItemText(IDC_EDITCITY,objStr.strCity);
	GetDlgItemText(IDC_EDITTOWN,objStr.strTown);
	GetDlgItemText(IDC_EDITPOSTCODE, objStr.strPostCode);
	GetDlgItemText(IDC_EDITHEADER, objStr.strBillHeader);
	GetDlgItemText(IDC_EDITCONTENT,objStr.strBillContent);
	objStr.bBill = ((CButton*)GetDlgItem(IDC_CHECKBILL))->GetCheck() == TRUE?true:false;
	objStr.iShowFlag = ((CButton*)GetDlgItem(IDC_CHECKSHOW))->GetCheck();
	objStr.iDebug = ((CButton*)GetDlgItem(IDC_CHECKDEBUG))->GetCheck();
	objStr.bCanNormalSend = ((CButton*)GetDlgItem(IDC_CHECKNORMAL))->GetCheck();
	objStr.bUseGiftCard = ((CButton*)GetDlgItem(IDC_CHECK_USEGIFTCARD))->GetCheck();
	objStr.eShipType = (_ShipType)(m_comb_shiptype.GetCurSel() + 1);
	objStr.eSendTime = (_SendTime)(m_combo_sendtime.GetCurSel() + 1);
	GetDlgItemText(IDC_EDIT_CLAIMCODE,objStr.strClaimCode);
	GetDlgItemText(IDC_EDIT_REMARK,objStr.strSendRemark);

	GetDlgItemText(IDC_COMBO_PAY,objStr.strPayType);
	GetDlgItemText(IDC_COMBO_INVOICE,objStr.strBillContent);
	objStr.strPayType.Trim();
	objStr.strBillContent.Trim();	
	if (objStr.strPayType == "")
		objStr.strPayType = "֧����";
	if (objStr.strBillContent == "")
		objStr.strBillContent = "�ҵ�";

	objStr.bShipWaitType = ((CButton*)GetDlgItem(IDC_CHECK_TOGETHER))->GetCheck();
	//IDC_CHECKDEBUG
	
	objStr.mapGoodNos.clear();
	strGoods += ";";
	int iPos = strGoods.Find(";");
	while(iPos >= 0)
	{
		CString strRet = strGoods.Left(iPos);
		strRet.Trim();
		if (strRet != "")
		{
			objStr.mapGoodNos[strRet] = 1;
		}
		strGoods = strGoods.Mid(iPos + 1);
		iPos = strGoods.Find(";");
	}
	if(objStr.mapGoodNos.size() < 1)
	{
		SetDlgItemText(IDC_EDITRESULT,"����д�������");
		return;
	}
	CAmazonClass objAmazon;
	CGoodOrderBase *pGood = &objAmazon;
	int iRet = pGood->Order(&objStr);
	SetDlgItemText(IDC_EDITRESULT,pGood->GetRetString(iRet));
}

void CAmazonWrapperDlg::OnBnClickedButton2()
{
	GoodOrderStru objStr;
	GetDlgItemText(IDC_EDITUSER,objStr.strUserID);
	GetDlgItemText(IDC_EDITPWD,objStr.strPwd);
	GetDlgItemText(IDC_EDITNO,objStr.strGoodNos);
	GetDlgItemText(IDC_EDITNAME,objStr.strRecver);
	GetDlgItemText(IDC_EDITPHONE,objStr.strPhone);
	GetDlgItemText(IDC_EDITADDR,objStr.strAddr);
	GetDlgItemText(IDC_EDITPRO,objStr.strProvince);
	GetDlgItemText(IDC_EDITCITY,objStr.strCity);
	GetDlgItemText(IDC_EDITTOWN,objStr.strTown);
	GetDlgItemText(IDC_EDITPOSTCODE,objStr.strPostCode);
	GetDlgItemText(IDC_EDITHEADER,objStr.strBillHeader);
	GetDlgItemText(IDC_EDITCONTENT,objStr.strBillContent);
	objStr.bBill = ((CButton*)GetDlgItem(IDC_CHECKBILL))->GetCheck() == TRUE?true:false;
	objStr.eSendTime = (_SendTime)GetDlgItemInt(IDC_EDITTYPE);
	objStr.iShowFlag = ((CButton*)GetDlgItem(IDC_CHECKSHOW))->GetCheck();
	objStr.iDebug = ((CButton*)GetDlgItem(IDC_CHECKDEBUG))->GetCheck();
	objStr.eQueryType = (_QueryType)GetDlgItemInt(IDC_EDITQUERYTYPE);;//
	//IDC_CHECKDEBUG	
// 	CAmazonWraper objWraper;
// 	int iRet = objWraper.Query(objStr);
// 	if (iRet == 0)
// 	{
// 		SetDlgItemText(IDC_EDITRESULT,"�ɹ�");
// 		return;
// 	}
// 	SetDlgItemInt(IDC_EDITRESULT,iRet);
}

void CAmazonWrapperDlg::OnBnClickedButton3()
{
	GoodOrderStru objStr;
	GetDlgItemText(IDC_EDITUSER,objStr.strUserID);
	GetDlgItemText(IDC_EDITPWD,objStr.strPwd);
	GetDlgItemText(IDC_EDITNO,objStr.strGoodNos);
	GetDlgItemText(IDC_EDITNAME,objStr.strRecver);
	GetDlgItemText(IDC_EDITPHONE,objStr.strPhone);
	GetDlgItemText(IDC_EDITADDR,objStr.strAddr);
	GetDlgItemText(IDC_EDITPRO,objStr.strProvince);
	GetDlgItemText(IDC_EDITCITY,objStr.strCity);
	GetDlgItemText(IDC_EDITTOWN,objStr.strTown);
	GetDlgItemText(IDC_EDITPOSTCODE,objStr.strPostCode);
	GetDlgItemText(IDC_EDITHEADER,objStr.strBillHeader);
	GetDlgItemText(IDC_EDITCONTENT,objStr.strBillContent);
	GetDlgItemText(IDC_EDITORDERNO,objStr.strOrderNo);
	objStr.bBill = ((CButton*)GetDlgItem(IDC_CHECKBILL))->GetCheck() == TRUE?true:false;
	objStr.eSendTime = (_SendTime)GetDlgItemInt(IDC_EDITTYPE);
	objStr.iShowFlag = ((CButton*)GetDlgItem(IDC_CHECKSHOW))->GetCheck();
	objStr.iDebug = ((CButton*)GetDlgItem(IDC_CHECKDEBUG))->GetCheck();
	objStr.eQueryType = (_QueryType)GetDlgItemInt(IDC_EDITQUERYTYPE);;//
	//IDC_CHECKDEBUG	
// 	CAmazonWraper objWraper;
// 	int iRet = objWraper.ChangePayType(objStr);
// 	if (iRet == 0)
// 	{
// 		SetDlgItemText(IDC_EDITRESULT,"�ɹ�");
// 		return;
// 	}
//	SetDlgItemInt(IDC_EDITRESULT,iRet);
}

void CAmazonWrapperDlg::OnBnClickedButton4()
{

	/*
	long jsBeginTick = DateTime.Parse("1970-1-1").Ticks;
	DateTime dt2 = DateTime.Now.ToLocalTime();

	long dt2Ticks = dt2.ToUniversalTime().Ticks;
	long dt2JsTicks = (dt2Ticks - jsBeginTick) / (1000 * 10);
	*/

	/*
	XinHuaOrderStr objStr;
	GetDlgItemText(IDC_EDITUSER,objStr.strUserID);
	GetDlgItemText(IDC_EDITPWD,objStr.strPwd); 
	XinHuaClass objXin;
	int iRet = objXin.Query(objStr);
	if (iRet == 0)
	{
		SetDlgItemText(IDC_EDITRESULT,"�ɹ�");
		return;
	}
	SetDlgItemInt(IDC_EDITRESULT,iRet);*/
}

void CAmazonWrapperDlg::OnBnClickedButton5()
{
	GoodOrderStru objStr;
	GetDlgItemText(IDC_EDITUSER,objStr.strUserID);
	GetDlgItemText(IDC_EDITPWD,objStr.strPwd);
	GetDlgItemText(IDC_EDITNO,objStr.strGoodNos);
	GetDlgItemText(IDC_EDITNAME,objStr.strRecver);
	GetDlgItemText(IDC_EDITPHONE,objStr.strPhone);
	GetDlgItemText(IDC_EDITADDR,objStr.strAddr);
	GetDlgItemText(IDC_EDITPRO,objStr.strProvince);
	GetDlgItemText(IDC_EDITCITY,objStr.strCity);
	GetDlgItemText(IDC_EDITTOWN,objStr.strTown);
	GetDlgItemText(IDC_EDITPOSTCODE,objStr.strPostCode);
	GetDlgItemText(IDC_EDITHEADER,objStr.strBillHeader);
	GetDlgItemText(IDC_EDITCONTENT,objStr.strBillContent);
	GetDlgItemText(IDC_EDITORDERNO,objStr.strOrderNo);
	
	objStr.mapGoodNos["B004CRTQ20"] = 1;

	CString strNo = "B004CRTQ20";
	//GetDlgItemText(IDC_EDITNO,strNo);
	CAmazonClass objAmazon;
	CGoodOrderBase *pGood = &objAmazon;
	//pGood->AddToCard(strNo , 1);
	//pGood->Login("shaojun@qq.com", "123456");
	pGood->Order(&objStr);
}



void CAmazonWrapperDlg::SplitString(CString strSourceEx, CString strSep, CStringArray &Strings)
{
	if(strSourceEx.IsEmpty())
		return;
	Strings.RemoveAll();
	int index = strSourceEx.Find(strSep);
	while(index >= 0)
	{
		CString strTemp = strSourceEx.Left(index);
		Strings.Add(strTemp);
		strSourceEx = strSourceEx.Right(strSourceEx.GetLength() - index - 1);
		index = strSourceEx.Find(strSep);
	}
	Strings.Add(strSourceEx);
}
void CAmazonWrapperDlg::OnBnClickedRadio1()
{
	((CButton*)GetDlgItem(IDC_RADIO1))->SetCheck(TRUE);
	((CButton*)GetDlgItem(IDC_RADIO2))->SetCheck(FALSE);
	SetDlgItemText(IDC_EDITNO,"1724439,1619590");
}

void CAmazonWrapperDlg::OnBnClickedRadio2()
{
	((CButton*)GetDlgItem(IDC_RADIO1))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_RADIO2))->SetCheck(TRUE);
	SetDlgItemText(IDC_EDITNO,"9787530963852|23.00,9787807609254|25.00");
}



// AmazonWrapperDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "AmazonWrapper.h"
#include "AmazonWrapperDlg.h"
#include "AmazonWraper.h"
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
}

BEGIN_MESSAGE_MAP(CAmazonWrapperDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON1, &CAmazonWrapperDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CAmazonWrapperDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CAmazonWrapperDlg::OnBnClickedButton3)
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


	AmazonOrderStru objStr;
	objStr.strUserID = "58357601@qq.com";
	objStr.strPwd = "shizisen861225";
	objStr.strRecver = "�۾�";
	objStr.strAddr = "�������򳤹��Ӵ�";
	objStr.strProvince = "����";
	objStr.strCity = "������";
	objStr.strTown = "��ɽ��";
	objStr.strPostCode = "414005";
	objStr.strPhone = "15019212360";
	objStr.bBill = true;
	objStr.strBillHeader = "��㿩";
	objStr.strBillContent = "�ҵ�";
	objStr.eSendType = NightOrOther;
	objStr.eQueryType = Last30;
	objStr.iShowFlag = 1;
	objStr.strOrderNo = "C02-3760340-2099800";
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
	SetDlgItemText(IDC_EDITCONTENT,objStr.strBillContent);
	SetDlgItemText(IDC_EDITNO,"0140623361,1594483078");//0140623361";//B002YGSOS8//1594483078
	((CButton*)GetDlgItem(IDC_CHECKBILL))->SetCheck(objStr.bBill);
	SetDlgItemInt(IDC_EDITTYPE,objStr.eSendType);
	SetDlgItemInt(IDC_EDITQUERYTYPE,objStr.eQueryType);
	((CButton*)GetDlgItem(IDC_CHECKSHOW))->SetCheck(objStr.iShowFlag);

	((CButton*)GetDlgItem(IDC_CHECKDEBUG))->SetCheck(objStr.iDebug);
	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

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
	AmazonOrderStru objStr;
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
	objStr.eSendType = (_SendType)GetDlgItemInt(IDC_EDITTYPE);
	objStr.iShowFlag = ((CButton*)GetDlgItem(IDC_CHECKSHOW))->GetCheck();
	objStr.iDebug = ((CButton*)GetDlgItem(IDC_CHECKDEBUG))->GetCheck();
	//IDC_CHECKDEBUG
	if(objStr.strGoodNos == "")
	{
		SetDlgItemText(IDC_EDITRESULT,"����д�������");
		return;
	}
	CAmazonWraper objWraper;
	int iRet = objWraper.Order(objStr);
	if (iRet == 0)
	{
		SetDlgItemText(IDC_EDITRESULT,"�ɹ�");
		return;
	}
	SetDlgItemInt(IDC_EDITRESULT,iRet);	
}

void CAmazonWrapperDlg::OnBnClickedButton2()
{
	AmazonOrderStru objStr;
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
	objStr.eSendType = (_SendType)GetDlgItemInt(IDC_EDITTYPE);
	objStr.iShowFlag = ((CButton*)GetDlgItem(IDC_CHECKSHOW))->GetCheck();
	objStr.iDebug = ((CButton*)GetDlgItem(IDC_CHECKDEBUG))->GetCheck();
	objStr.eQueryType = (_QueryType)GetDlgItemInt(IDC_EDITQUERYTYPE);;//
	//IDC_CHECKDEBUG	
	CAmazonWraper objWraper;
	int iRet = objWraper.Query(objStr);
	if (iRet == 0)
	{
		SetDlgItemText(IDC_EDITRESULT,"�ɹ�");
		return;
	}
	SetDlgItemInt(IDC_EDITRESULT,iRet);
}

void CAmazonWrapperDlg::OnBnClickedButton3()
{
	AmazonOrderStru objStr;
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
	objStr.eSendType = (_SendType)GetDlgItemInt(IDC_EDITTYPE);
	objStr.iShowFlag = ((CButton*)GetDlgItem(IDC_CHECKSHOW))->GetCheck();
	objStr.iDebug = ((CButton*)GetDlgItem(IDC_CHECKDEBUG))->GetCheck();
	objStr.eQueryType = (_QueryType)GetDlgItemInt(IDC_EDITQUERYTYPE);;//
	//IDC_CHECKDEBUG	
	CAmazonWraper objWraper;
	int iRet = objWraper.ChangePayType(objStr);
	if (iRet == 0)
	{
		SetDlgItemText(IDC_EDITRESULT,"�ɹ�");
		return;
	}
	SetDlgItemInt(IDC_EDITRESULT,iRet);
}

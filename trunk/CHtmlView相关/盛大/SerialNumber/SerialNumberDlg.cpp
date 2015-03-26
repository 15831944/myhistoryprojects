// SerialNumberDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SerialNumber.h"
#include "SerialNumberDlg.h"
#include ".\serialnumberdlg.h"
#include <afxmt.h>
#include <atlbase.h>
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define  __CLIENT__

/************************************************************************/
/*                                                                      */
/************************************************************************/
CString GetGenNum()
{
	char szName[255] = {0};
	DWORD dwSerialNum = 0;
	DWORD dwMaxNum = 0;
	DWORD dwSysFlag = 0;
	char szSysName[255] = {0};
	DWORD dwSysNameSize = 255;	
	BOOL bRet = GetVolumeInformation("c:\\",szName,255,&dwSerialNum,&dwMaxNum,&dwSysFlag,szSysName,dwSysNameSize);
	CString strRet = "";
	strRet.Format("%s%u%s",szName,dwSerialNum,szSysName);
	return strRet;
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
//���ܳ�16�����ַ���
CString StrEncode(CString srcStr)
{
	int             i ;
	int             nLen;
	CString         newString = "";
	CString         tempStr;
	unsigned char   ucTemp;
	nLen =  srcStr.GetLength( );

	//���ܺ�ת��16����

	for( i = 0; i < nLen; i++ )
	{
		ucTemp = ( unsigned char )srcStr.GetAt(i);
		tempStr.Format("%02X#", ucTemp^i );
		newString +=tempStr;
	}
	return newString;
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
//����16�����ַ���
CString StrDecode(CString srcStr)
{
	int             nLen;
	CString         newString = "";
	CString         tempStr;
	unsigned char   ucTemp;

	//����16�����ַ���
	int idx1 = 0; 
	int idx2 = 0;
	int i = 0;
	idx2 = srcStr.Find('#', idx1);  //����һ��ʮ�����������Կո��ֿ� 
	while(idx2 != -1) 
	{ 
		CString temp1 = "0x" + srcStr.Mid(idx1,idx2); //ȡ��һ��16�����ִ�
		int ldec = strtol(temp1, NULL, 16); //��16����ת��Ϊ10����
		CString temp;
		temp.Format("%c",(unsigned char )ldec^i);
		newString += temp;
		idx1 = idx2 + 1; 
		idx2 = srcStr.Find('#', idx1);  //����һ��16���������Կո��ֿ�
		i ++;
	} 
	return newString;

}
// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���
/************************************************************************/
/* ���ܺ���                                                             */
/************************************************************************/
CString strEnCodeEx(CString strSrc)
{
	INT i = 0;
	INT j = 0;
	INT64 fSeedA = 56789; ///     ����
	INT64 fSeedB = 54667; ///     ����
	INT64 fKey = 6968875; //      Կ��
	INT64 iKey = fKey;
	CString strGet = strSrc;
	CString strRet = "";
	CString strTemp = "";
	CHAR szRet[1000] = {0};
	for (i=0;i<strGet.GetLength();i++)
	{
		szRet[i] = (CHAR)((BYTE)strGet.GetAt(i) ^ (iKey >> 8));
		iKey = ((BYTE)szRet[i]+iKey)*fSeedA+fSeedB;
	}
	strGet.Format("%s",szRet);
	strRet = "";
	for (i=0;i<strGet.GetLength();i++)
	{
		j = (int)strGet.GetAt(i);
		if (j < 0)
		{
			j+=256;
		}		
		strTemp.Format("%c",(CHAR)(65+(j/26)));
		strRet += strTemp;
		strTemp.Format("%c",(CHAR)(65+(j%26)));
		strRet += strTemp;
	}
	return strRet;
}
/************************************************************************/
/* ���ܺ���                                                             */
/************************************************************************/
CString strDeCodeEx(CString strSrc)
{
	int i = 0;
	int j = 0;
	CString strGet;
	strGet = strSrc;
	INT64 fSeedA = 56789; ///     ����   ,
	INT64 fSeedB = 54667; ///     ����   ,
	INT64 fKey = 6968875; //     Կ��
	INT64 iKey = fKey;
	char szResult[1000]={0};
	CString strResult = "";
	CString strTemp = "";
	int index = 0;
	for (i=0;i<strGet.GetLength()/2;i++)
	{
		index = 2*i;
		j = ((int)strGet.GetAt(index)-65)*26;
		index = 2*i+1;
		j += (int)strGet.GetAt(index)-65;
		if (j>128)
		{
			j-=256;
		}
		strTemp.Format("%c",(char)j);
		strResult += strTemp;
	}
	strGet = strResult;
	for (i=0;i<strGet.GetLength();i++)
	{
		szResult[i]=(char)((byte)strGet.GetAt(i)^(iKey >> 8));
		iKey = ((BYTE)strGet.GetAt(i)+iKey)*fSeedA+fSeedB;
	}
	strResult.Format("%s",szResult);
	return strResult;
}

bool IsPowered()
{
	CString strSerial = strEnCodeEx(StrEncode(strEnCodeEx(StrEncode(GetGenNum()))));	
	bool bRet = false;
	CRegKey   regAutoRun;
	char szKey[50] = {0};
	strcpy(szKey,"IsPowered");
	if (ERROR_SUCCESS != regAutoRun.Create(HKEY_CURRENT_USER,"Software\\Microsoft\\Windows\\CurrentVersion"))
	{
		return false;
	}
	DWORD dLen = 5000;
	char szValue[5000]={0}; 
	if( ERROR_SUCCESS == regAutoRun.QueryValue(szValue,szKey,&dLen))
	{
		if (CString(szValue)==strSerial)
		{
			bRet=true; 
		}
	}
	regAutoRun.Close();
	return bRet;
}


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


// CSerialNumberDlg �Ի���



CSerialNumberDlg::CSerialNumberDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSerialNumberDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSerialNumberDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CSerialNumberDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_GEN, OnBnClickedButtonGen)
	ON_BN_CLICKED(IDC_BUTTON_REG, OnBnClickedButtonReg)
	ON_BN_CLICKED(IDC_BUTTON_GEN2, OnBnClickedButtonGen2)
END_MESSAGE_MAP()


// CSerialNumberDlg ��Ϣ�������

BOOL CSerialNumberDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ��\������...\���˵�����ӵ�ϵͳ�˵��С�

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

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

#ifdef  __CLIENT__
	((CEdit *)(GetDlgItem(IDC_EDIT_OLD)))->SetReadOnly(TRUE);   
	GetDlgItem(IDC_BUTTON_GEN2)->ShowWindow(SW_HIDE);
	SetWindowText("����ע��");
#else
	GetDlgItem(IDC_BUTTON_GEN)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_BUTTON_REG)->ShowWindow(SW_HIDE);
	((CEdit *)(GetDlgItem(IDC_EDIT_NEW)))->SetReadOnly(TRUE); 
	SetWindowText("���к�����");
#endif
	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	
	return TRUE;  // ���������˿ؼ��Ľ��㣬���򷵻� TRUE
}

void CSerialNumberDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CSerialNumberDlg::OnPaint() 
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
HCURSOR CSerialNumberDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CSerialNumberDlg::OnBnClickedButtonGen()
{
	SetDlgItemText(IDC_EDIT_OLD,strEnCodeEx(StrEncode(GetGenNum())));
}


/************************************************************************/
/*                                                                      */
/************************************************************************/
void CSerialNumberDlg::OnBnClickedButtonReg()
{
#ifdef  __CLIENT__
#else
	return;
#endif

	CString strNew = "";
	CString strOld = strEnCodeEx(StrEncode(strEnCodeEx(StrEncode(GetGenNum()))));
	GetDlgItemText(IDC_EDIT_NEW,strNew);
	if (strNew == strOld)	//ͨ����֤
	{
		//дע���
		bool bRet = false;
		CRegKey   regAutoRun;
		char szKey[5000] = {0};
		char szValue[5000] = {0};
		strcpy(szKey,"IsPowered");
		strcpy(szValue,strNew);
		if (ERROR_SUCCESS != regAutoRun.Create(HKEY_CURRENT_USER,"Software\\Microsoft\\Windows\\CurrentVersion"))
		{
			MessageBox("ע��ʧ��","��ʾ");
			return;
		}
		if( ERROR_SUCCESS == regAutoRun.SetValue(szValue,szKey))
		{
			bRet = true;
		}
		regAutoRun.Close();
		MessageBox("��ϲ��ע��ɹ�","��ʾ");
		PostMessage(WM_CLOSE,0,0);
		return;
	}
	MessageBox("ע��ʧ��","��ʾ");
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CSerialNumberDlg::OnBnClickedButtonGen2()
{
#ifdef  __CLIENT__
	return;
#endif
	CString strSrc = "";
	GetDlgItemText(IDC_EDIT_OLD,strSrc);
	CString strDest = strEnCodeEx(StrEncode(strSrc));
	SetDlgItemText(IDC_EDIT_NEW,strDest);
}

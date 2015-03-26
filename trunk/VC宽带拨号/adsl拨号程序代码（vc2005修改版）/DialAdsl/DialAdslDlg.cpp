// DialAdslDlg.cpp : ʵ���ļ�
//
//���������������д���ֽ����뷢����ѧϰ����֮��
//������ķ�������GPLЭ�飬�����ڴ�Э��Ĺ涨��ʹ�ñ�����
//�������ʹ�ñ������;����ʲô���ʻ��������http://digua254909662.blog.163.com/blog/static/74805072200922323951508/edit/
//������ϵ���ߣ�qq<254909662> ��˵�����⣬���˷ǳ�������������

#include "stdafx.h"
#include "DialAdsl.h"
#include "DialAdslDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


CListBox CDialAdslDlg::dialStatusListBox;
CONNNECT_STATUS CDialAdslDlg::dialStatus = MYADSL_INIALTED;
DialManage* CDialAdslDlg::lpDialManage = NULL;

// CDialAdslDlg �Ի���




CDialAdslDlg::CDialAdslDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDialAdslDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CDialAdslDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LISTDIALSTATUS, dialStatusListBox);
}

BEGIN_MESSAGE_MAP(CDialAdslDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_DIAL, &CDialAdslDlg::OnBnClickedDial)
	ON_WM_NCHITTEST()
	ON_BN_CLICKED(IDOK, &CDialAdslDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDialAdslDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_CLOSE, &CDialAdslDlg::OnBnClickedClose)
	ON_BN_CLICKED(IDC_MINI, &CDialAdslDlg::OnBnClickedMini)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CDialAdslDlg ��Ϣ�������

BOOL CDialAdslDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
#pragma region //���ð�ťͼƬ
	m_btnTest.LoadBitmaps(IDB_DIALMAP);
	m_btnTest.SubclassDlgItem(IDC_DIAL,this);
	m_btnTest.SizeToContent();
	//m_btnTest.AutoLoad(IDC_DIAL, this);IDB_MIN IDB_CLOSE
	m_btnMin.LoadBitmaps(IDB_MIN);
	m_btnMin.SubclassDlgItem(IDC_MINI,this);
	m_btnMin.SizeToContent();

	m_btnClose.LoadBitmaps(IDB_CLOSE);
	m_btnClose.SubclassDlgItem(IDC_CLOSE,this);
	m_btnClose.SizeToContent();
#pragma endregion

	dialStatus = MYADSL_INIALTED;

#pragma region //��ȡϵͳ�е�һ��adsl���ӵ�����
	lpDialManage = new DialManage();
	ASSERT( NULL != lpDialManage );
	if( 1 != lpDialManage ->FindDefaultAdslConnect())
	{
		AfxMessageBox(_T("ϵͳ���Ҳ���adsl����"));
		return TRUE;
	}
	lpDialManage ->GetDialInfo(&dialInformation);
	AfxGetMainWnd() ->SetDlgItemTextA(IDC_EDITUSERNAME, dialInformation.szUserName);
	AfxGetMainWnd() ->SetDlgItemTextA(IDC_EDITPASSWORD, dialInformation.szPassword);
#pragma endregion

	//HBITMAP hBitmap;
	////����ͼƬID
	//hBitmap =(HBITMAP)LoadImage(AfxGetInstanceHandle(),
	//	MAKEINTRESOURCE(IDB_BITMAP1),
	//	IMAGE_BITMAP,0,0,
	//	LR_LOADMAP3DCOLORS);
	////���ָ��̬�ؼ���ָ��
	//CStatic *pStatic=(CStatic *)GetDlgItem(IDC_PICTURE);//�ؼ�ID
	////���þ�̬�ؼ�����ʽ��ʹ�����ʹ��λͼ������λ����ʾʹ����
	//pStatic->ModifyStyle(0xF,SS_BITMAP|SS_CENTERIMAGE);
	////���þ�̬�ؼ���ʾλͼ
	//pStatic->SetBitmap(hBitmap); 

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CDialAdslDlg::SetBitmap(UINT uBmpResource, UINT uCtrlResource)
{  
	HBITMAP hBitmap;  
	CStatic *pStatic = (CStatic*)GetDlgItem(uCtrlResource);  
	hBitmap = (HBITMAP)LoadImage(  
		AfxGetInstanceHandle(),    
		MAKEINTRESOURCE(uBmpResource),    
		IMAGE_BITMAP,    
		0,    
		0,    
		LR_LOADMAP3DCOLORS);  
	pStatic->ModifyStyle(0xF,   SS_BITMAP|SS_CENTERIMAGE);  
	pStatic->SetBitmap(hBitmap);  
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CDialAdslDlg::OnPaint()
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

	if( MYADSL_FAILED == dialStatus )
	{
		lpDialManage ->RasDialReset();
		dialStatus = MYADSL_INIALTED;
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù����ʾ��
//
HCURSOR CDialAdslDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CDialAdslDlg::OnBnClickedDial()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//CString buffer;
	//CHAR buffer[1024];
	//ConnectByCmd();
	CHAR buffer[128];
	::RtlZeroMemory(buffer, 128);
	AfxGetMainWnd() ->GetDlgItemTextA(IDC_EDITPASSWORD, buffer, 127);
	lstrcpy(dialInformation.szPassword, buffer);
	::RtlZeroMemory(buffer, 128);
	AfxGetMainWnd() ->GetDlgItemTextA(IDC_EDITUSERNAME, buffer, 127);
	lstrcpy(dialInformation.szUserName, buffer);
	lpDialManage ->SetDialInfo(&dialInformation);

	ConnectByRas();
	//AfxMessageBox("hello");
}


// ��ק
LRESULT CDialAdslDlg::OnNcHitTest(CPoint pt)
{
	CRect rc;
	GetClientRect(&rc);
	ClientToScreen(&rc);
	return rc.PtInRect(pt) ? HTCAPTION : CDialog::OnNcHitTest(pt);
}

void CDialAdslDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	AfxGetMainWnd()->ShowWindow(SW_SHOWMINIMIZED);
	//OnOK();
}

void CDialAdslDlg::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	OnCancel();
}

void CDialAdslDlg::OnBnClickedClose()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	OnCancel();
}

void CDialAdslDlg::OnBnClickedMini()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	AfxGetMainWnd()->ShowWindow(SW_SHOWMINIMIZED);
}

HBRUSH CDialAdslDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  �ڴ˸��� DC ���κ�����
	if(nCtlColor == CTLCOLOR_STATIC)
	{
		return (HBRUSH)::GetStockObject(HOLLOW_BRUSH);   
	}

	// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
	return hbr;
}

void CDialAdslDlg::ConnectByRas()
{
	if(NULL == lpDialManage)
	{
		return;
	}
	dialStatusListBox.ResetContent();
	int iRet = lpDialManage ->RasDialConnect(RasDialFunc);
	if( 1 == iRet)
	{
		CString temp;
		temp.Format(_T("�������ӵ� %s...���Ժ�"), dialInformation.szEntryName);
		dialStatusListBox.AddString(temp);
		return;
	}
	if(iRet == 99)
	{
		CString temp;
		temp.Format(_T("�Ѿ����ӳɹ�"));
		dialStatusListBox.AddString(temp);
		return;
	}
	if(iRet == 100)
	{
		CString temp;
		temp.Format(_T("��Ǹ��ϵͳ��û�ҵ���������"));
		dialStatusListBox.AddString(temp);
		return;
	}
	if( iRet > 1)
	{
		CString temp;
		lpDialManage ->GetErrorMessage(iRet, temp.GetBuffer(128), 127);
		//AfxMessageBox(temp.GetBuffer());
		dialStatusListBox.AddString(temp);
		temp.Format(_T("���Ժ����ԣ���Ϣһ������"));
		dialStatusListBox.AddString(temp);
		
		//lpDialManage ->RasDialReset();
	}
}

void CDialAdslDlg::ConnectByCmd()
{
	CString strBuffer;
	HANDLE hReadHandle;
	DWORD byteRead;
	if(NULL == lpDialManage)
	{
		return;
	}
	int listIndex = 0;
	int result = lpDialManage ->CmdDialConnect(hReadHandle);
	if( 100 == result)
	{
		AfxMessageBox(_T("ϵͳ��û���ҵ�����"));
		return;
	}
	dialStatusListBox.InsertString(listIndex, _T("�����������Ժ�..."));
	Invalidate();
	if( 1 == result )
	{
		while(true)  
		{  
			strBuffer = "";
			if(ReadFile(hReadHandle,strBuffer.GetBuffer(1024),1023,&byteRead,NULL)==NULL)  
				break;  
			int index = strBuffer.Find(_T("..."), 0);
			if( index > 0 )
			{
				AfxMessageBox(_T("finded ..."));
				AfxMessageBox(strBuffer.GetBuffer());
				CString tempStr;
				tempStr = strBuffer.Mid(0, 14 + 3);
				listIndex++;
				dialStatusListBox.InsertString(listIndex, tempStr);
				int lateIndex;
				tempStr.Format("%d", index);
				AfxMessageBox(tempStr);
				/*while(index > 0)
				{
					lateIndex = strBuffer.Find(_T("..."), index + 3 );
					if(lateIndex > 0)
					{
						AfxMessageBox(_T("while ..."));
						tempStr = strBuffer.Mid(index+3, lateIndex - index);
						listIndex++;
						dialStatusListBox.InsertString(listIndex, tempStr);
					}
					index = lateIndex;
				}*/
				//dialStatusListBox.AddString(_T("\n"));
			}
		}  
	}
}
VOID WINAPI CDialAdslDlg::RasDialFunc(UINT unMsg, RASCONNSTATE rasconnstate, DWORD dwError)
{
	//AfxMessageBox(_T("arrived RasDialFunc"));
	if (unMsg != WM_RASDIALEVENT) return; 
	if( MYADSL_INIALTED == dialStatus)
	{
		dialStatus = MYADSL_DIALING;
	}
	if(dwError != 0)
	{
		ASSERT( MYADSL_DIALING == dialStatus );
		dialStatus = MYADSL_FAILED;
		CHAR buffer[1024];
		lpDialManage ->GetErrorMessage(dwError, buffer, 1023);
		//AfxMessageBox(buffer);
		dialStatusListBox.AddString(buffer);
		return;
	}
	CHAR buffer[128];
	if( RASCS_Authenticate == rasconnstate)
	{
		lstrcpy(buffer, _T("������֤�û���������...") );
		dialStatusListBox.AddString(buffer);
		return;
	}
	if( RASCS_Projected == rasconnstate)
	{
		lstrcpy(buffer, _T("����������ע�����ļ����") );
		dialStatusListBox.AddString(buffer);
		return;
	}
	if( RASCS_Connected == rasconnstate)
	{
		lstrcpy(buffer, _T("�ɹ�����") );
		dialStatusListBox.AddString(buffer);
		dialStatus = MYADSL_CONNECTED;
		return;
	}
	return VOID();
}

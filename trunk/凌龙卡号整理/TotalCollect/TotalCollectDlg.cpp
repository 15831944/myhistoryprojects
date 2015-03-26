// TotalCollectDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "TotalCollect.h"
#include "TotalCollectDlg.h"
#include ".\totalcollectdlg.h"
#include <io.h>
#include <atlbase.h>
#include "afxinet.h"
#include <io.h>
#include <odbcinst.h>
#include <afxdb.h>
#include "UserFile.h"
#pragma comment(lib,"wininet.lib")
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

typedef struct _DetailStruct
{
	CString strKey;
	CString strPosStr;
}DetailStruct;

// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���
int g_iOptType = 0;	//0��ʾ���룬1��ʾ������2��ʾ��ʾ����
CArray<CString, CString> g_strList;
CString g_strPath = "";
bool g_bExitFlag = true;
int g_iTotalNum = 0;
CString g_strTemplate = "900,800,700,600,009,008,007,006,090,080,070,060";
CString g_strSelectTail = "";
CArray<DetailStruct, DetailStruct> g_DetailStructList;
CArray<DetailStruct, DetailStruct>  g_UnionList;		//���ڳ���
map<CString, int> g_keyMap;
int g_iSelectPos = -1;
CString g_strBeforeCard = "";
CString g_strAfterCard = "";
CString g_strBeforePwd = "";
CString g_strAfterPwd = "";
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


// CTotalCollectDlg �Ի���



CTotalCollectDlg::CTotalCollectDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTotalCollectDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTotalCollectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_TOTAL, m_list_total);
	DDX_Control(pDX, IDC_LIST_DETAIL, m_list_detail);
	DDX_Control(pDX, IDC_LIST_DETAIL2, m_listResult);
}

BEGIN_MESSAGE_MAP(CTotalCollectDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BTN_IMPORT, OnBnClickedBtnImport)
	ON_BN_CLICKED(IDC_BTN_EXIT, OnBnClickedBtnExit)
	ON_BN_CLICKED(IDC_BTN_STOP, OnBnClickedBtnStop)
	ON_BN_CLICKED(IDC_BTN_ANANYZE, OnBnClickedBtnAnanyze)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_TOTAL, OnNMDblclkListTotal)
	ON_BN_CLICKED(IDC_BTN_SHOWUNION, OnBnClickedBtnShowunion)
	ON_BN_CLICKED(IDC_BTN_IMPORT2, OnBnClickedBtnImport2)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CTotalCollectDlg ��Ϣ�������

BOOL CTotalCollectDlg::OnInitDialog()
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
// 	if (GetPower() != 0)
// 	{
// 		exit(0);
// 	}
	InitParams();
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��
	SetTableStyle();
	SetTableStyleEx();
	SetTableStyleExEx();
	return TRUE;  // ���������˿ؼ��Ľ��㣬���򷵻� TRUE
}

void CTotalCollectDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CTotalCollectDlg::OnPaint() 
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
HCURSOR CTotalCollectDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CTotalCollectDlg::SetTableStyle()
{
	CListCtrl* conList = &m_list_total;
	DWORD dwSytle=::GetWindowLong(conList->m_hWnd,GWL_STYLE);
	// ����Ϊ������ʽ
	SetWindowLong(conList->m_hWnd,GWL_STYLE,dwSytle|LVS_REPORT);
	DWORD ExStyle=conList->GetExtendedStyle();
	conList->SetExtendedStyle(ExStyle|LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
	CImageList image;
	image.Create(1,16, ILC_COLOR, 0, 0);
	conList->SetImageList(&image, LVSIL_STATE);
	conList->MoveWindow(7,35,200,440);
	conList->InsertColumn(1,"���",LVCFMT_LEFT,80);
	conList->InsertColumn(2,"�ַ���",LVCFMT_LEFT,90);
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CTotalCollectDlg::SetTableStyleExEx()
{
	CListCtrl* conList = &m_listResult;
	DWORD dwSytle=::GetWindowLong(conList->m_hWnd,GWL_STYLE);
	// ����Ϊ������ʽ
	SetWindowLong(conList->m_hWnd,GWL_STYLE,dwSytle|LVS_REPORT);
	DWORD ExStyle=conList->GetExtendedStyle();
	conList->SetExtendedStyle(ExStyle|LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
	CImageList image;
	image.Create(1,16, ILC_COLOR, 0, 0);
	conList->SetImageList(&image, LVSIL_STATE);
	conList->MoveWindow(9,83,510,390);
	conList->InsertColumn(1,"���",LVCFMT_LEFT,70);
	conList->InsertColumn(2,"����",LVCFMT_LEFT,120);
	conList->InsertColumn(3,"����",LVCFMT_LEFT,120);
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CTotalCollectDlg::SetTableStyleEx()
{
	DWORD dwSytle=::GetWindowLong(m_list_detail.m_hWnd,GWL_STYLE);
	// ����Ϊ������ʽ
	SetWindowLong(m_list_detail.m_hWnd,GWL_STYLE,dwSytle|LVS_REPORT);
	DWORD ExStyle=m_list_detail.GetExtendedStyle();
	m_list_detail.SetExtendedStyle(ExStyle|LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
	CImageList image;
	image.Create(1,16,ILC_COLOR,0,0);
	m_list_detail.SetImageList(&image, LVSIL_STATE);
	m_list_detail.MoveWindow(217,35,200,440);
	m_list_detail.InsertColumn(1,"���",LVCFMT_LEFT,70);
	m_list_detail.InsertColumn(2,"ְҵ",LVCFMT_LEFT,90);
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CTotalCollectDlg::OnBnClickedBtnImport()
{
	g_strPath = "";
	if(SaveParams()<0)
	{
		return;
	}
	char szFilter[255] = {0};
	CString strPath = "";
	strcpy(szFilter,"Text files(*.txt)|*.txt");
	CFileDialog dlg(1,NULL,NULL,NULL,szFilter); 
	if(dlg.DoModal()==IDOK) 
	{
		strPath = dlg.GetPathName();
	}
	strPath.Trim();
	if (strPath != "")
	{
		g_strPath = strPath;
		//�����߳�
		g_iOptType = 0;
		g_bExitFlag = false;
		AfxBeginThread((AFX_THREADPROC)ThreadProc, (LPVOID)this);		
	}	
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CTotalCollectDlg::OnBnClickedBtnExit()
{
	if (g_bExitFlag = false)
	{
		g_bExitFlag = true;
		Sleep(500);
	}
	PostMessage(WM_CLOSE,0,0);
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CTotalCollectDlg::ThreadProc(LPVOID lpParam)
{
	CTotalCollectDlg *pWnd = (CTotalCollectDlg *)lpParam;
	pWnd->GetDlgItem(IDC_BTN_IMPORT)->EnableWindow(FALSE);
	pWnd->GetDlgItem(IDC_BTN_ANANYZE)->EnableWindow(FALSE);
	pWnd->GetDlgItem(IDC_BTN_SHOWUNION)->EnableWindow(FALSE);
	pWnd->OperateProc();
	pWnd->GetDlgItem(IDC_BTN_IMPORT)->EnableWindow(TRUE);
	pWnd->GetDlgItem(IDC_BTN_ANANYZE)->EnableWindow(TRUE);
	pWnd->GetDlgItem(IDC_BTN_SHOWUNION)->EnableWindow(TRUE);
	g_bExitFlag = true;
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CTotalCollectDlg::OperateProc()
{
	CString strFormat = "";
	switch(g_iOptType)
	{
	case 0:	//����
		{	
			//m_list_total.DeleteAllItems();
			//m_list_detail.DeleteAllItems();
			CStdioFile   file;
			CString   cstr = "";
			CFileException   fe;			
			int iNum = 0;
			CString strBefore = "";
			CString strAfter = "";
			CString strAcc = "";
			CString strPwd = "";
			if(file.Open(g_strPath,CFile::modeRead,&fe))   
			{
				file.SeekToBegin();
				SetDlgItemText(IDC_EDIT_NOTICE,"���ڵ���,���Ժ�...");
				g_strList.RemoveAll();
				while (file.ReadString(cstr))
				{
					if (g_bExitFlag)
					{
						break;
					}
					cstr.Trim();
					if (cstr != "")
					{
						strBefore = g_strBeforeCard;
						strAfter = g_strAfterCard;
						strAcc = GetKeyString(cstr,strBefore,strAfter);
						strBefore = g_strBeforePwd;
						strAfter = g_strAfterPwd;
						strPwd = GetKeyString(cstr,strBefore,strAfter);
						int nCount = m_listResult.GetItemCount();
						bool bExist = false;
						for (int i=0;i<nCount;i++)
						{
							if (m_listResult.GetItemText(i,1)==strAcc)
							{
								bExist = true;
							}
						}
						if (bExist)
						{
							continue;
						}
						strFormat.Format("%d",nCount);
						m_listResult.InsertItem( nCount,strFormat);
						m_listResult.SetItemText(nCount,1,strAcc);
						m_listResult.SetItemText(nCount,2,strPwd);
						iNum++;
					}				
				}
				g_iTotalNum = iNum;
				if (g_bExitFlag)
				{
					strFormat.Format("�ֶ���ֹ����,������%d����¼��Ϣ",iNum);
				}
				else
				{
					strFormat.Format("�������,������%d����¼��Ϣ",iNum);
				}
				SetDlgItemText(IDC_EDIT_NOTICE,strFormat);
			}
		}
		break;
	case 5:
		{
			CStdioFile   file;
			CString   cstr = "";
			CFileException   fe;			
			int iNum = 0;
			if(file.Open(g_strPath,CFile::modeRead,&fe))   
			{
				file.SeekToBegin();
				SetDlgItemText(IDC_EDIT_NOTICE,"���ڵ���,���Ժ�...");
				g_strList.RemoveAll();
				while (file.ReadString(cstr))
				{
					if (g_bExitFlag)
					{
						break;
					}
					cstr.Trim();
					if (cstr != "")
					{
						//g_strList.Add(cstr);
						int nCount = m_list_detail.GetItemCount();
						bool bExist = false;
						for (int i=0;i<nCount;i++)
						{
							if (m_list_detail.GetItemText(i,1)==cstr)
							{
								bExist = true;
							}
						}
						if (bExist)
						{
							continue;
						}
						strFormat.Format("%d",nCount);
						m_list_detail.InsertItem( nCount,strFormat);
						m_list_detail.SetItemText(nCount,1,cstr);
						iNum++;
					}				
				}
				g_iTotalNum = iNum;
				if (g_bExitFlag)
				{
					strFormat.Format("�ֶ���ֹ����,������%d����¼��Ϣ",iNum);
				}
				else
				{
					strFormat.Format("�������,������%d����¼��Ϣ",iNum);
				}
				SetDlgItemText(IDC_EDIT_NOTICE,strFormat);
			}
		}
		break;
	case 1://����
		{
			g_keyMap.clear();
			CString strAddr = "";
			CString strJob = "";
			int iaddrNum = m_list_total.GetItemCount();
			int iJobNum = m_list_detail.GetItemCount();
			int iNum = 0;
			for (int i=0;i<iaddrNum;i++)
			{
				if (g_bExitFlag)
				{
					break;
				}
				strAddr = m_list_total.GetItemText(i,1);
				for (int j=0;j<iJobNum;j++)
				{
					if (g_bExitFlag)
					{
						break;
					}
					strJob = m_list_detail.GetItemText(j,1);
					if (g_keyMap[strAddr+strJob] == 1)
					{
						continue;
					}
					else
					{
						g_keyMap[strAddr+strJob] = 1;
					}
					int nCount = m_listResult.GetItemCount();
					strFormat.Format("%d",nCount);
					m_listResult.InsertItem( nCount,strFormat);
					m_listResult.SetItemText(nCount,1,strAddr);
					m_listResult.SetItemText(nCount,2,strJob);
					iNum++;
				}
			}
			if (g_bExitFlag)
			{
				strFormat.Format("�ֶ���ֹ�ϲ�,���ϲ�%d����¼��Ϣ",iNum);
			}
			else
			{
				strFormat.Format("�������,���ϲ�%d����¼��Ϣ",iNum);
			}
			SetDlgItemText(IDC_EDIT_NOTICE,strFormat);
		}
		break;
	case 2://����
		{
			m_list_detail.DeleteAllItems();
			SetDlgItemText(IDC_EDIT_NOTICE,"������ʾ����,���Ժ�...");
			CString strPosList = g_DetailStructList.GetAt(g_iSelectPos).strPosStr;
			CString strLeft = "";
			CString strRight = "";
			CString strPos = "";
			CString strResult = "";
			int idx1 = 0;
			int idx2 = -1;
			while (true)
			{
				if (g_bExitFlag)
				{
					break;
				}
				idx2 = strPosList.Find(',',idx1);
				if (idx2 == -1)
				{
					break;
				}
				strPos = strPosList.Mid(idx1,idx2-idx1);
				strResult = g_strList.GetAt(atoi(strPos));
				int nCount = m_list_detail.GetItemCount();
				strFormat.Format("%d",nCount);
				m_list_detail.InsertItem( nCount,strFormat);
				m_list_detail.SetItemText(nCount,1,strResult.Left(strResult.GetLength() - 5));
				m_list_detail.SetItemText(nCount,2,strResult.Right(5));
				m_list_detail.SetItemText(nCount,3,strPos);
				idx1 = idx2 + 1;
			}
			if (g_bExitFlag)
			{
				strFormat.Format("�ֶ���ֹ��ʾ����");
			}
			else
			{
				strFormat.Format("��ʾ�������");	
			}
			SetDlgItemText(IDC_EDIT_NOTICE,strFormat);
		}
		break;
	case 3:			//������ʾ
		{
			//�������
			try
			{
				CString sExcelFile = "";
				if (!GetDefaultXlsFileName(sExcelFile))
				{
					return;
				}
				CString strContent = "";
				int iCount = m_listResult.GetItemCount();
				for (int i=0;i<iCount;i++)
				{
					strContent.Format("%s,%s,,,,",m_listResult.GetItemText(i,1),m_listResult.GetItemText(i,2));
					WriteFileLog(sExcelFile,strContent);
				}				
				SetDlgItemText(IDC_EDIT_NOTICE,"�������");		
			}
			catch(...)
			{
				SetDlgItemText(IDC_EDIT_NOTICE,"ִ���쳣,����δ���");
			}
		}
		break;
	default:
		break;
	}
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CTotalCollectDlg::OnBnClickedBtnStop()
{
	g_bExitFlag = true;
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
bool CTotalCollectDlg::IsRightData(CString strData)
{
	if (g_strTemplate.Find(strData) != -1)	//�����ַ�
	{
		return true;
	}
	CString strLeft = strData.Left(1);
	CString strMid = strData.Mid(1,1);
	CString strRight = strData.Right(1);
	int iResult = atoi(strLeft) + atoi(strMid) + atoi(strRight);
	if (iResult < 6 || iResult > 21)
	{
		return true;
	}
	return false;

}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CTotalCollectDlg::OnBnClickedBtnAnanyze()
{
	if (g_bExitFlag == false)
	{
		SetDlgItemText(IDC_EDIT_NOTICE,"�߳�����ִ����,���Ժ��ٽ��з���...");
		return;
	}
	g_iOptType = 1;
	g_bExitFlag = false;
	AfxBeginThread((AFX_THREADPROC)ThreadProc, (LPVOID)this);
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CTotalCollectDlg::OnNMDblclkListTotal(NMHDR *pNMHDR, LRESULT *pResult)
{
	*pResult = 0;
	g_strSelectTail = "";
	if (g_bExitFlag == false)
	{
		SetDlgItemText(IDC_EDIT_NOTICE,"�߳�����ִ����,���Ժ��ٽ��з���...");
		return;
	}
	POSITION pos = m_list_total.GetFirstSelectedItemPosition();
	if (pos != NULL)
	{
		int nItem = m_list_total.GetNextSelectedItem(pos);		
		g_strSelectTail = m_list_total.GetItemText(nItem,0);
		g_iSelectPos = nItem;
		g_iOptType = 2;
		g_bExitFlag = false;
		AfxBeginThread((AFX_THREADPROC)ThreadProc, (LPVOID)this);
	}
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CTotalCollectDlg::AddToResultList(CString strTail,CString strSrcPos)
{
	int iPos = g_keyMap[strTail];
	if (iPos == 0)
	{
		iPos = m_list_total.GetItemCount();
		g_keyMap[strTail] = iPos + 1;
		//���
		m_list_total.InsertItem(iPos,"");
		m_list_total.SetItemText(iPos,0,strTail);
		m_list_total.SetItemText(iPos,1,"1");
		DetailStruct detailStr;
		detailStr.strKey = strTail;
		detailStr.strPosStr = strSrcPos + ",";
		g_DetailStructList.Add(detailStr);
	}
	else
	{
		iPos = iPos - 1;
		//����
		CString strCount = m_list_total.GetItemText(iPos,1);
		strCount.Format("%d",atoi(strCount) + 1);
		m_list_total.SetItemText(iPos,1,strCount);
		g_DetailStructList.GetAt(iPos).strPosStr += strSrcPos + ",";
	}
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
int CTotalCollectDlg::GetPower()
{
	CInternetSession session;
	CString strTemp = "";
	try
	{
		CStdioFile *pFile = session.OpenURL("http://blog.csdn.net/TragicGuy/archive/2010/01/08/5157814.aspx");
		if (pFile != NULL)
		{
			while(pFile->ReadString(strTemp))
			{
				strTemp.MakeLower();
				if (strTemp.Find("myownpower444") !=-1)
				{
					pFile->Close();
					delete pFile;
					pFile = NULL;
					return 0;
				}
			}

			if (pFile == NULL)
			{
				pFile->Close();
				delete pFile;
				pFile = NULL;
			}
		}
	}
	catch (...)
	{
	}
	return -1;
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CTotalCollectDlg::OnBnClickedBtnShowunion()
{
	if (g_bExitFlag == false)
	{
		SetDlgItemText(IDC_EDIT_NOTICE,"�߳�����ִ����,���Ժ��ٽ��з���...");
		return;
	}
	g_iOptType = 3;
	g_bExitFlag = false;
	AfxBeginThread((AFX_THREADPROC)ThreadProc, (LPVOID)this);
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
int CTotalCollectDlg::GetCountTotal(CString strSrc)
{
	int iCount = 0;
	int iLen = strSrc.GetLength();
	for (int i=0;i<iLen;i++)
	{
		if (strSrc.GetAt(i)==',')
		{
			iCount++;
		}
	}
	return iCount;
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CTotalCollectDlg::OnBnClickedBtnImport2()
{
	g_strPath = "";
	char szFilter[255] = {0};
	CString strPath = "";
	strcpy(szFilter,"Text files(*.txt)|*.txt");
	CFileDialog dlg(1,NULL,NULL,NULL,szFilter); 
	if(dlg.DoModal()==IDOK) 
	{
		strPath = dlg.GetPathName();
	}
	strPath.Trim();
	if (strPath != "")
	{
		g_strPath = strPath;
		//�����߳�
		g_iOptType = 5;		//��������2
		g_bExitFlag = false;
		AfxBeginThread((AFX_THREADPROC)ThreadProc, (LPVOID)this);		
	}
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
//���Ĭ�ϵ��ļ���
BOOL CTotalCollectDlg::GetDefaultXlsFileName(CString& sExcelFile)
{
	///Ĭ���ļ�����yyyymmddhhmmss.xls
	CString timeStr = "";
	timeStr = CTime::GetCurrentTime().Format("%Y%m%d%H%M%S");
	timeStr = "��������-����ʱ��["+timeStr+"]";

	sExcelFile =  timeStr + ".txt";
	// prompt the user (with all document templates)
	CFileDialog dlgFile(FALSE,".txt",sExcelFile);
	CString title;
	CString strFilter;

	title = "����";
	strFilter = "TXT �ļ�(*.txt)";
	strFilter += (TCHAR)'\0';   // next string please
	strFilter += _T("*.txt");
	strFilter += (TCHAR)'\0';   // last string
	dlgFile.m_ofn.nMaxCustFilter++;
	dlgFile.m_ofn.nFilterIndex = 1;
	// append the "*.*" all files filter
	CString allFilter;
	VERIFY(allFilter.LoadString(AFX_IDS_ALLFILTER));
	strFilter += allFilter;
	strFilter += (TCHAR)'\0';   // next string please
	strFilter += _T("*.*");
	strFilter += (TCHAR)'\0';   // last string
	dlgFile.m_ofn.nMaxCustFilter++;

	dlgFile.m_ofn.lpstrFilter = strFilter;
	dlgFile.m_ofn.lpstrTitle = title;

	if (dlgFile.DoModal()==IDCANCEL)
	{
		return FALSE; // open cancelled
	}	
	sExcelFile.ReleaseBuffer();
	sExcelFile = dlgFile.GetPathName();
	sExcelFile.MakeLower();
	if (sExcelFile.Right(4) != ".txt")
	{
		sExcelFile+=".txt";
	}
	if (MakeSurePathExists(sExcelFile,true)) 
	{
		if(!DeleteFile(sExcelFile)) 
		{    // delete the file
			AfxMessageBox("�����ļ�ʱ����");
			return FALSE;
		}
	}
	return TRUE;
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
BOOL CTotalCollectDlg::MakeSurePathExists( CString &Path,bool FilenameIncluded)
{
	int Pos=0;
	while((Pos=Path.Find('\\',Pos+1))!=-1)
	{
		CreateDirectory(Path.Left(Pos),NULL);
	}
	if(!FilenameIncluded)
	{
		CreateDirectory(Path,NULL);
	}
	return !_access(Path,0);
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
CString CTotalCollectDlg::GetExcelDriver()
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
CString CTotalCollectDlg::GetKeyString(CString &srcStr,CString &strBefore,CString &strAfter)
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
void CTotalCollectDlg::WriteFileLog(CString strFilePath,CString srcStr)
{
	try
	{
		FILE *fp;
		char szfile[1000]={0};
		sprintf(szfile,strFilePath);
		fp=fopen(szfile,"a+");
		fprintf(fp,"%s\r\n",srcStr);
		fclose(fp);
	}
	catch (...)
	{
	}
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CTotalCollectDlg::OnClose()
{
	CUserFile *pFile = CUserFile::GetInstance();
	if (pFile)
	{
		delete pFile;
		pFile = NULL;
	}
	CDialog::OnClose();
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CTotalCollectDlg::InitParams()
{
	CUserFile *pFile = CUserFile::GetInstance();
	CString strAppPath = "";
	int iTemp = 0;
	pFile->GetAppPath(strAppPath);
	pFile->ReadInitInfo(strAppPath+"setex.ini","Register","beforecard",g_strBeforeCard,iTemp,0);
	pFile->ReadInitInfo(strAppPath+"setex.ini","Register","aftercard",g_strAfterCard,iTemp,0);
	pFile->ReadInitInfo(strAppPath+"setex.ini","Register","beforepwd",g_strBeforePwd,iTemp,0);
	pFile->ReadInitInfo(strAppPath+"setex.ini","Register","afterpwd",g_strAfterPwd,iTemp,0);
	SetDlgItemText(IDC_EDIT_CARDPRE,g_strBeforeCard);
	SetDlgItemText(IDC_EDIT_CARDNEXT,g_strAfterCard);
	SetDlgItemText(IDC_EDIT_PWDPRE,g_strBeforePwd);
	SetDlgItemText(IDC_EDIT_PWDNEXT,g_strAfterPwd);
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
int CTotalCollectDlg::SaveParams()
{
	CUserFile *pFile = CUserFile::GetInstance();
	CString strAppPath = "";
	int iTemp = 0;
	GetDlgItemText(IDC_EDIT_CARDPRE,g_strBeforeCard);
	GetDlgItemText(IDC_EDIT_CARDNEXT,g_strAfterCard);
	GetDlgItemText(IDC_EDIT_PWDPRE,g_strBeforePwd);
	GetDlgItemText(IDC_EDIT_PWDNEXT,g_strAfterPwd);
	if (g_strBeforeCard == "" || g_strAfterCard =="" || g_strBeforePwd == "" || g_strAfterPwd =="")
	{
		MessageBox("��������Ϊ��","����");
		return -1;
	}
	pFile->GetAppPath(strAppPath);
	pFile->WriteInitInfo(strAppPath+"setex.ini","Register","beforecard",g_strBeforeCard,iTemp,0);
	pFile->WriteInitInfo(strAppPath+"setex.ini","Register","aftercard",g_strAfterCard,iTemp,0);
	pFile->WriteInitInfo(strAppPath+"setex.ini","Register","beforepwd",g_strBeforePwd,iTemp,0);
	pFile->WriteInitInfo(strAppPath+"setex.ini","Register","afterpwd",g_strAfterPwd,iTemp,0);	
	return 0;
}

// PicOperateDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "PicOperate.h"
#include "PicOperateDlg.h"
#include "afxdialogex.h"
#include <cv.h>
#include <cxcore.h>
#include <highgui.h>
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
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

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CPicOperateDlg �Ի���




CPicOperateDlg::CPicOperateDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CPicOperateDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CPicOperateDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CPicOperateDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_MERGE, &CPicOperateDlg::OnBnClickedButtonMerge)
END_MESSAGE_MAP()


// CPicOperateDlg ��Ϣ�������

BOOL CPicOperateDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
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

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CPicOperateDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CPicOperateDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
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
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CPicOperateDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CPicOperateDlg::MergePicture(IplImage* pImg1, IplImage* pImg2, IplImage* pDsg, bool bVirtical)
{
	IplImage* image3 = cvCreateImage( cvSize(MAX(image1->width,image2->width),image1->height + image2->height),IPL_DEPTH_8U,3);
	CvRect rect=cvRect(0,0,image1->width,image1->height);
	cvSetImageROI(image3,rect);
	cvCopy(image1,image3);

	cvResetImageROI(image3);
	rect=cvRect(0,image1->height,image2->width,image2->height);
	cvSetImageROI(image3,rect);
	cvCopy(image2,image3);
	cvResetImageROI(image3);
	cvNamedWindow("image3");
	cvShowImage("image3",image3);
}

void CPicOperateDlg::OnBnClickedButtonMerge()
{
	

	//����
	CString sFolderPath;
	BROWSEINFO bi;
	char Buffer[MAX_PATH]; //��ʼ����ڲ���bi��ʼ
	bi.hwndOwner = NULL;
	bi.pidlRoot = NULL;
	bi.pszDisplayName = Buffer; //�˲�����ΪNULL������ʾ�Ի���
	bi.lpszTitle = "��ѡ��ͼƬ����Ŀ¼";
	bi.ulFlags = BIF_RETURNONLYFSDIRS;
	bi.lpfn = NULL;
	bi.iImage = 0;   //��ʼ����ڲ���bi����
	LPITEMIDLIST pIDList = SHBrowseForFolder(&bi);//������ʾѡ��Ի���
	if(pIDList)
	{
		SHGetPathFromIDList(pIDList, Buffer);
		//ȡ���ļ���·����Buffer��
		sFolderPath = Buffer;//��·��������һ��CString������
	}
	CFileFind finder;
	CString findName = sFolderPath + "\\*.bmp";
	BOOL findFlag = finder.FindFile(findName);
	bool bFirstPic = true;
	while (findFlag)
	{
		findFlag = finder.FindNextFile();
		if (finder.IsDots())
			continue;
		if(bFirstPic)
		{

		}
		else
		{

		}
		bFirstPic = false;
		CString sFileName = finder.GetFileName();
	}
	finder.Close();

	CString strDstName = "";
	
	
}

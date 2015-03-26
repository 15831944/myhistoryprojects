// BMPTestDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "BMPTest.h"
#include "BMPTestDlg.h"
#include ".\bmptestdlg.h"
#include <cv.h>
#include <cxcore.h>
#include <highgui.h>
#include <io.h>
#include   <iostream>   
#include   <fstream>

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


// CBMPTestDlg �Ի���



CBMPTestDlg::CBMPTestDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CBMPTestDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CBMPTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CBMPTestDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP

	ON_BN_CLICKED(IDC_BTN_DETECT, OnBnClickedBtnDetect)
	ON_BN_CLICKED(IDC_BTN_CreatTemp, OnBnClickedBtnCreattemp)
	ON_BN_CLICKED(IDC_BTN_CBAT, OnBnClickedBtnCbat)
	ON_BN_CLICKED(IDC_BTN_CVEC, OnBnClickedBtnCvec)
	ON_BN_CLICKED(IDC_BTN_TRAIN, OnBnClickedBtnTrain)
	ON_BN_CLICKED(IDC_BTN_CXML, OnBnClickedBtnCxml)
END_MESSAGE_MAP()


// CBMPTestDlg ��Ϣ�������

BOOL CBMPTestDlg::OnInitDialog()
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

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	SetDlgItemText(IDC_INFO,"1: ��C�̸�Ŀ¼�½�һ��ΪOpenCVTrain���ļ���\r\n2: ��bin�ļ���COPY�����ļ�����\r\n3: ��OpenCVTrain�н���һ����Ϊdata���ļ���\r\n4: ����������׼��\r\n5: ����ѵ���ű�������\r\n6: ���������ļ�\r\n7: ����������ѵ��\r\n8: ����XMLģ���ļ�\r\n\r\n���в������谴˳��ִ��.");
	return TRUE;  // ���������˿ؼ��Ľ��㣬���򷵻� TRUE
}

void CBMPTestDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CBMPTestDlg::OnPaint() 
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
HCURSOR CBMPTestDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


//������
void CBMPTestDlg::OnBnClickedBtnDetect()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	CString findName = sFolderPath + "\\*.jpg";
	BOOL findFlag = finder.FindFile(findName);
	while (findFlag)
	{
		findFlag = finder.FindNextFile();
		if (finder.IsDots())
			continue;

		CString sFileName = finder.GetFileName();
		CMyImage myImg;
		CPoint detectPoint = myImg.MatchPicture(sFolderPath+"\\"+sFileName,"c:\\OpenCVTrain\\data.xml");
		CString aa;
		aa.Format("%d,%d",detectPoint.x,detectPoint.y);
		MessageBox(aa);
	}
	finder.Close();
	
}

//��������ͼƬ����������
void CBMPTestDlg::OnBnClickedBtnCreattemp()
{
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
	CString findName = sFolderPath + "\\*.jpg";
	BOOL findFlag = finder.FindFile(findName);
	while (findFlag)
	{
		
		findFlag = finder.FindNextFile();
		if (finder.IsDots())
		continue;
		
		CString sFileName = finder.GetFileName();
		CMyImage myImg;
		myImg.MackTemplate(sFolderPath+"\\"+sFileName);
		
	}
	finder.Close();
}

void CBMPTestDlg::GenerateDataFile()
{

	CString infopath = "C:\\OpenCVTrain\\info.dat";     //�����������ļ�
	CFile file;
	if (_access(infopath,0)==0) //�ļ�����
	{
		file.Remove(infopath);		
	}
	file.Open(infopath,CFile::modeCreate|CFile::modeReadWrite);
	
	CString sFolderPath = "C:\\OpenCVTrain\\Template";
	CFileFind finder;
	CString findName = sFolderPath + "\\*.jpg";
	BOOL findFlag = finder.FindFile(findName);
	while (findFlag)
	{

		findFlag = finder.FindNextFile();
		if (finder.IsDots())
			continue;
		CString sFileName = finder.GetFileName();
		CString infoStr;
		infoStr.Format("Template\\%s 1 0 0 %d %d \r\n",sFileName,40,40);
		file.SeekToEnd();
		file.Write(infoStr,infoStr.GetLength());
	}
	file.Close();
	finder.Close();

	
	infopath = "C:\\OpenCVTrain\\bg.txt";     //�����������ļ�

	if (_access(infopath,0)==0) //�ļ�����
	{
		file.Remove(infopath);		
	}
	file.Open(infopath,CFile::modeCreate|CFile::modeReadWrite);

	sFolderPath = "C:\\OpenCVTrain\\offTemplate";
	findName = sFolderPath + "\\*.jpg";
	findFlag = finder.FindFile(findName);
	while (findFlag)
	{

		findFlag = finder.FindNextFile();
		if (finder.IsDots())
			continue;
		CString sFileName = finder.GetFileName();
		CString infoStr;
		infoStr.Format("offTemplate\\%s\r\n",sFileName);
		file.SeekToEnd();
		file.Write(infoStr,infoStr.GetLength());
	}
	file.Close();
	finder.Close();
}


//�������ɵ�����ͼƬ����ѵ���ű�
void CBMPTestDlg::OnBnClickedBtnCbat()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	//����run.bat

	GenerateDataFile();
	CFile file;
	CString fileName;
	CString infoStr;
	int infoRows = 0;
	int bgRows = 0;
	fileName = "C:\\OpenCVTrain\\info.dat";
	if (_access(fileName,0)!=0) //�ļ�������
	{
		MessageBox("���������ļ�������");
		return;
	}
	
	infoRows = GetRows(fileName);

	fileName = "C:\\OpenCVTrain\\run.bat";     //�����������ļ�
	if (_access(fileName,0)==0) //�ļ�����
	{
		file.Remove(fileName);		
	}
	file.Open(fileName,CFile::modeCreate|CFile::modeReadWrite);
	file.SeekToEnd();
	infoStr.Format("c:\\OpenCVTrain\\bin\\CreateSamples.exe  -vec c:\\OpenCVTrain\\data.vec -info c:\\OpenCVTrain\\info.dat -num %d -w 40 -h 40",infoRows);
	file.Write(infoStr,infoStr.GetLength());
	file.Close();


	//����train.bat
	fileName = "C:\\OpenCVTrain\\bg.txt"; 
	if (_access(fileName,0)!=0) //�ļ�������
	{
		MessageBox("�����������ļ�������");
		return;
	}
	bgRows = GetRows(fileName);
	fileName = "C:\\OpenCVTrain\\train.bat";     //�����������ļ�
	if (_access(fileName,0)==0) //�ļ�����
	{
		file.Remove(fileName);		
	}
	file.Open(fileName,CFile::modeCreate|CFile::modeReadWrite);
	file.SeekToEnd();
	infoStr.Format("c:\\OpenCVTrain\\bin\\haartraining.exe -mem 1000 -data c:\\OpenCVTrain\\data -vec c:\\OpenCVTrain\\data.vec -bg c:\\OpenCVTrain\\bg.txt -npos %d -nneg %d -w 40 -h 40",infoRows,bgRows);
	file.Write(infoStr,infoStr.GetLength());
	file.Close();
	
	//����saveXML.bat
	fileName = "C:\\OpenCVTrain\\saveXML.bat";     //�����������ļ�
	if (_access(fileName,0)==0) //�ļ�����
	{
		file.Remove(fileName);		
	}
	file.Open(fileName,CFile::modeCreate|CFile::modeReadWrite);
	file.SeekToEnd();
	infoStr.Format("c:\\OpenCVTrain\\bin\\haarconv.exe c:\\OpenCVTrain\\data c:\\OpenCVTrain\\data.xml 40 40");
	file.Write(infoStr,infoStr.GetLength());
	file.Close();
	MessageBox("���ɽű��ɹ�");

}


//��һ���ı��ļ��л�ȡ����
int CBMPTestDlg::GetRows(CString fileName)
{
	int   lines   =   0;   
	CStdioFile   file;
	CString   cstr;
	CFileException   fe;
	if(file.Open(fileName,CFile::modeRead,&fe))   
	{ 
		file.SeekToBegin();
		while (file.ReadString(cstr))
		{
			lines ++;
		}
		file.Close();
	}
	return lines;
}


//���������ļ�
void CBMPTestDlg::OnBnClickedBtnCvec()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	system("c:\\OpenCVTrain\\run.bat");
}	


//ѵ������
void CBMPTestDlg::OnBnClickedBtnTrain()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	system("c:\\OpenCVTrain\\train.bat");
}


//����XMLģ������
void CBMPTestDlg::OnBnClickedBtnCxml()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	system("c:\\OpenCVTrain\\saveXML.bat");
}

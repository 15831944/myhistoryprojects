// UDiskTestDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "UDiskTest.h"
#include "UDiskTestDlg.h"
#include <setupapi.h>
#include <WinIoCtl.h>
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define GDI_VOLUMENAME 0
#define GDI_VOLUMESERIALNUMBER 1
#define GDI_VOLUMEFILESYSTEM 2
#define GDI_VOLUMETYPE 3
#define GDI_VOLUMESIZE 4
#define GDI_VOLUMEFREESIZE 5
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


// CUDiskTestDlg �Ի���




CUDiskTestDlg::CUDiskTestDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CUDiskTestDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CUDiskTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CUDiskTestDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, &CUDiskTestDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CUDiskTestDlg ��Ϣ�������

BOOL CUDiskTestDlg::OnInitDialog()
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

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CUDiskTestDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CUDiskTestDlg::OnPaint()
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
HCURSOR CUDiskTestDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}
/*
BOOL GetDisksProperty(HANDLE hDevice, PSTORAGE_DEVICE_DESCRIPTOR pDevDesc)
{
	STORAGE_PROPERTY_QUERY    Query;    
	DWORD dwOutBytes;                
	BOOL bResult;                    

	Query.PropertyId = StorageDeviceProperty;
	Query.QueryType = PropertyStandardQuery;

	bResult = ::DeviceIoControl(hDevice,            
		IOCTL_STORAGE_QUERY_PROPERTY,            
		&Query, sizeof(STORAGE_PROPERTY_QUERY),    
		pDevDesc, pDevDesc->Size,                
		&dwOutBytes,                            
		(LPOVERLAPPED)NULL);                    

	return bResult;
}*/

#define MEDIA_INFO_SIZE    sizeof(GET_MEDIA_TYPES)+15*sizeof(DEVICE_MEDIA_INFO)

BOOL GetDriveGeometry(const TCHAR * filename, DISK_GEOMETRY * pdg)
{
	HANDLE hDevice;         // �豸���
	BOOL bResult;           // DeviceIoControl�ķ��ؽ��
	GET_MEDIA_TYPES *pmt;   // �ڲ��õ����������
	DWORD dwOutBytes;       // ������ݳ���

	// ���豸
	hDevice = ::CreateFile(filename,           // �ļ���
		GENERIC_READ,                          // ������Ҫ����
		FILE_SHARE_READ | FILE_SHARE_WRITE,    // ����ʽ
		NULL,                                  // Ĭ�ϵİ�ȫ������
		OPEN_EXISTING,                         // ������ʽ
		0,                                     // ���������ļ�����
		NULL);                                 // �������ģ���ļ�

	if (hDevice == INVALID_HANDLE_VALUE)
	{
		// �豸�޷���...
		return FALSE;
	}

	// ��IOCTL_DISK_GET_DRIVE_GEOMETRYȡ���̲���
	bResult = ::DeviceIoControl(hDevice,       // �豸���
		IOCTL_DISK_GET_DRIVE_GEOMETRY,         // ȡ���̲���
		NULL, 0,                               // ����Ҫ��������
		pdg, sizeof(DISK_GEOMETRY),            // ������ݻ�����
		&dwOutBytes,                           // ������ݳ���
		(LPOVERLAPPED)NULL);                   // ��ͬ��I/O

	// ���ʧ�ܣ�����IOCTL_STORAGE_GET_MEDIA_TYPES_EXȡ�������Ͳ���
	if (!bResult)
	{
		pmt = (GET_MEDIA_TYPES *)new BYTE[MEDIA_INFO_SIZE];

		bResult = ::DeviceIoControl(hDevice,    // �豸���
			IOCTL_STORAGE_GET_MEDIA_TYPES_EX,   // ȡ�������Ͳ���
			NULL, 0,                            // ����Ҫ��������
			pmt, MEDIA_INFO_SIZE,               // ������ݻ�����
			&dwOutBytes,                        // ������ݳ���
			(LPOVERLAPPED)NULL);                // ��ͬ��I/O

		if (bResult)
		{
			// ע�⵽�ṹDEVICE_MEDIA_INFO���ڽṹDISK_GEOMETRY�Ļ����������
			// Ϊ�򻯳�����memcpy�������¶�����ֵ��䣺
			// pdg->MediaType = (MEDIA_TYPE)pmt->MediaInfo[0].DeviceSpecific.DiskInfo.MediaType;
			// pdg->Cylinders = pmt->MediaInfo[0].DeviceSpecific.DiskInfo.Cylinders;
			// pdg->TracksPerCylinder = pmt->MediaInfo[0].DeviceSpecific.DiskInfo.TracksPerCylinder;
			// ... ...
			::memcpy(pdg, pmt->MediaInfo, sizeof(DISK_GEOMETRY));
		}

		delete pmt;
	}

	// �ر��豸���
	::CloseHandle(hDevice);

	return (bResult);

}
void CUDiskTestDlg::ShowMessage(CString strInfo)
{
	AfxMessageBox(strInfo);
}
void CUDiskTestDlg::OnBnClickedOk()
{
	CString strDisk = "";
	GetDlgItemText(IDC_EDIT1,strDisk);
	CString strRet = GetDriverInfomation(strDisk,GDI_VOLUMESERIALNUMBER);
	AfxMessageBox(strRet);
	TCHAR szBuf[100];
	memset(szBuf,0,100);
	DWORD len = GetLogicalDriveStrings(sizeof(szBuf)/sizeof(TCHAR),szBuf);

	for (TCHAR* s = szBuf; *s; s += _tcslen(s)+1)
	{
		LPCTSTR sDrivePath = s;
		UINT uType = GetDriveType(sDrivePath);
		if (uType == DRIVE_REMOVABLE)
		{
			DISK_GEOMETRY dg;
			TCHAR szPath[100] = _T("\\\\.\\");
			::_tcscat(szPath,sDrivePath);
			int nSize = ::_tcslen(szPath);
			szPath[nSize-1] = '\0';
			BOOL bRetVal = GetDriveGeometry(szPath,&dg);

			if(dg.MediaType == RemovableMedia)
			{
				//dg.Cylinders
			}
		}
		
	}
	/*
	CString strDisk = "";
	HANDLE hDevice = CreateFile(strDisk, GENERIC_READ,
		FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, NULL, NULL);
	CString str = "";
	if (hDevice != INVALID_HANDLE_VALUE)
	{

		PSTORAGE_DEVICE_DESCRIPTOR pDevDesc = (PSTORAGE_DEVICE_DESCRIPTOR)new BYTE[sizeof(STORAGE_DEVICE_DESCRIPTOR) + 512 - 1];

		pDevDesc->Size = sizeof(STORAGE_DEVICE_DESCRIPTOR) + 512 - 1;

		if(GetDisksProperty(hDevice, pDevDesc))
		{
			szMoveDiskName[k] = chFirstDriveFromMask(temp);
			szMoveDiskName[0]=k;
			k++;
			/*ShowMessage("\r\n�ҵ�U�̣�����:\r\n");
			p= (char*)pDevDesc;
			str = (pDevDesc->VendorIdOffset ? &p[pDevDesc->VendorIdOffset]:"(NULL)");
			ShowMessage(str);

			ShowMessage("\r\n��ƷID:\r\n");
			str=(pDevDesc->ProductIdOffset ? &p[pDevDesc->ProductIdOffset]:"(NULL)");
			ShowMessage(str);
			ShowMessage("\r\n��Ʒ�汾:\r\n");
			str = (pDevDesc->ProductRevisionOffset ? &p[pDevDesc->ProductRevisionOffset] : "(NULL)");
			ShowMessage(str);
			ShowMessage("\r\n���к�:\r\n");
			str = (pDevDesc->SerialNumberOffset ? &p[pDevDesc->SerialNumberOffset] : "(NULL)");
			ShowMessage(str);
		}

		delete pDevDesc;
		CloseHandle(hDevice);
	}
	*/
}
/*
bool Test()
{
	char   USBSN[23];

	TCHAR   buf[MAX_PATH]   =   {0};
	TCHAR   VID[MAX_PATH]   =   {0};
	TCHAR   PID[MAX_PATH]   =   {0};
	TCHAR   SN[MAX_PATH]   =   {0};
	DWORD   nSize   =   0;
	HDEVINFO   hDevInfo;
	SP_DEVINFO_DATA   DeviceInfoData;
	hDevInfo   =   SetupDiGetClassDevs((LPGUID)0,   0,   0,   (DIGCF_PRESENT|DIGCF_DEVICEINTERFACE));
	if   (hDevInfo   ==   INVALID_HANDLE_VALUE)
	{
		//   no   found   USB   device
		return   false;
	}
	DeviceInfoData.cbSize   =   sizeof(SP_DEVINFO_DATA);
	for   (int   j=0;SetupDiEnumDeviceInfo(hDevInfo,   j,   &DeviceInfoData);j++)
	{
		if   (   !SetupDiGetDeviceInstanceId(hDevInfo,   &DeviceInfoData,   buf,   sizeof(buf),   &nSize)   )
			continue;


		for   (int   i=0;   i <4;   i++)
		{
			VID[i]   =   buf[8+i];
		}

		for   (int i=0;   i <4;   i++)
		{
			PID[i]   =   buf[17+i];  
		}


		for   (int i=0;   i <20;   i++)
		{
			USBSN[i]   =   buf[22+i];
		}
	}
	SetupDiDestroyDeviceInfoList(hDevInfo);
	return true;
}
*/
/************************************************************
//�������ƣ�GetDriverInfomation
//�������ܣ��õ�����������Ϣ
//����1��bAlpha - BYTE�ͣ��������Ĵ���A-Z(a-z)
//����2��iGTI_TYPE - int����Ҫ��ȡ����������Ϣ����
// GDI_VOLUMENAME �õ�����������
// GDI_VOLUMESERIALNUMBER �õ����������к�
// GDI_VOLUMEFILESYSTEM �õ��������ļ�ϵͳ
// GDI_VOLUMETYPE �õ�����������
// GDI_VOLUMESIZE �õ��������ܴ�С
// GDI_VOLUMEFREESIZE �õ�������ʣ���С
//����ֵ��CString�ͣ���Ҫ�õ�����������Ϣ���ַ������
//���ӣ��õ�C�̵�����GetDriverInfomation('C',GDI_VOLUMENAME)
************************************************************/

CString CUDiskTestDlg::GetDriverInfomation(CString bAlpha,int iGTI_TYPE)
{
CString strResult = _T("");
{
/**********��ȡ���������֡����кź��ļ�ϵͳ����**********/
CString strRootPathName;
strRootPathName.Format(_T("%s:\\"),bAlpha);
LPCTSTR lpRootPathName = strRootPathName;
LPTSTR lpVolumeNameBuffer = new char[_MAX_FNAME];
DWORD nVolumeNameSize = _MAX_FNAME;
DWORD nVolumeSerialNumber = 0;//������������Чʱ���ж�
DWORD nMaximumComponentLength;
DWORD nFileSystemFlags;
LPTSTR lpFileSystemNameBuffer = new char[20];//�ļ�ϵͳ(NTFS,FAT)����ж���õĺ���
DWORD nFileSystemNameSize = 20;
GetVolumeInformation(
lpRootPathName,
lpVolumeNameBuffer,
nVolumeNameSize,
&nVolumeSerialNumber, 
&nMaximumComponentLength,
&nFileSystemFlags,
lpFileSystemNameBuffer,
nFileSystemNameSize);

/**********��ȡ���������Ͳ���**********/
CString strDriveType;

/**********��ȡ�������ܴ�С��ʣ���С����**********/
LPCTSTR lpDirectoryName = new char[2];
lpDirectoryName = (LPCTSTR)strRootPathName.Mid(0,2);
_ULARGE_INTEGER FreeBytesAvailable,TotalNumberOfBytes,TotalNumberOfFreeBytes ;

__int64 iVolumeSize = 0,iVolumeFreeSize = 0;

GetDiskFreeSpaceEx(strRootPathName.Mid(0,2),&FreeBytesAvailable,&TotalNumberOfBytes,&TotalNumberOfFreeBytes );
iVolumeSize = TotalNumberOfBytes.QuadPart / 1024 / 1024;
iVolumeFreeSize = FreeBytesAvailable.QuadPart / 1024 / 1024;

/**********���ݲ����ó���Ӧ����������Ϣ**********/
switch(iGTI_TYPE)
{
case GDI_VOLUMENAME:
if (lpVolumeNameBuffer != NULL)
strResult.Format(_T("������ %c ������Ϊ:%s."),bAlpha,lpVolumeNameBuffer);
else
strResult.Format(_T("������ %c ������Ϊ:%s."),bAlpha,lpVolumeNameBuffer);
//strResult.Format(_T("��ȡ����������ʧ��!"));
break;
case GDI_VOLUMESERIALNUMBER:
if (nVolumeSerialNumber != 0)
strResult.Format(_T("%X"),nVolumeSerialNumber);
else
strResult.Format(_T(""));
break;
case GDI_VOLUMEFILESYSTEM:
if (lpFileSystemNameBuffer != NULL)
strResult.Format(_T("������ %c ���ļ�ϵͳΪ:%s."),bAlpha,lpFileSystemNameBuffer);
else
strResult.Format(_T("��ȡ�������ļ�ϵͳʧ��!"));
break;
case GDI_VOLUMESIZE:
if (iVolumeSize != 0)
strResult.Format(_T("������ %c ���ܴ�СΪ:%.2fGB."),bAlpha,(float)iVolumeSize/1024);
else
strResult.Format(_T("��ȡ�������ܴ�Сʧ��!"));
break;
case GDI_VOLUMEFREESIZE:
if (iVolumeFreeSize != 0)
strResult.Format(_T("������ %c ��ʣ���СΪ:%.2fGB."),bAlpha,(float)iVolumeFreeSize/1024);
else
strResult.Format(_T("��ȡ������ʣ���Сʧ��!"));
break;
case GDI_VOLUMETYPE:
switch(GetDriveType(lpRootPathName))
{
case DRIVE_UNKNOWN:
strDriveType = _T("δ֪����!");
break;
case DRIVE_NO_ROOT_DIR:
strResult = _T("��ȡ����������ʱ�������ô���!");
return strResult;
case DRIVE_REMOVABLE:
strDriveType = _T("���ƶ�����");
break;
case DRIVE_FIXED:
strDriveType = _T("Ӳ��");
break;
case DRIVE_REMOTE:
strDriveType = _T("Զ���豸");
break;
case DRIVE_CDROM:
strDriveType = _T("CD");
break;
case DRIVE_RAMDISK:
strDriveType = _T("RAM");
break;
default:
strResult = _T("δ֪����!");
return strResult;
}
strResult.Format(_T("������ %c ������Ϊ:%s."),bAlpha,strDriveType);
break;
default:
strResult = _T("��ȡ��������Ϣʱ�������ô���!");
break;
}

}
return strResult;
}
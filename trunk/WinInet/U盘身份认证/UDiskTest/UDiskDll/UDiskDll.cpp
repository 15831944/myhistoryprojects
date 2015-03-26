// UDiskDll.cpp : ���� DLL Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "UDiskDll.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Ψһ��Ӧ�ó������

CWinApp theApp;

using namespace std;

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	int nRetCode = 0;

	// ��ʼ�� MFC ����ʧ��ʱ��ʾ����
	if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0))
	{
		// TODO: ���Ĵ�������Է���������Ҫ
		_tprintf(_T("����: MFC ��ʼ��ʧ��\n"));
		nRetCode = 1;
	}
	else
	{
		// TODO: �ڴ˴�ΪӦ�ó������Ϊ��д���롣
	}

	return nRetCode;
}


CString GetDriverInfomation(char* bAlpha)
{
	int iGTI_TYPE = GDI_VOLUMESERIALNUMBER;
	CString strResult = _T("");
	{
		/**********��ȡ���������֡����кź��ļ�ϵͳ����**********/
		CString strRootPathName;
		strRootPathName.Format(_T("%s:\\"),bAlpha);
		if (GetDriveType(strRootPathName) != DRIVE_REMOVABLE)
		{
			return "";
		}
		LPCTSTR lpRootPathName = strRootPathName;
		LPTSTR lpVolumeNameBuffer = new char[_MAX_FNAME];
		DWORD nVolumeNameSize = _MAX_FNAME;
		DWORD nVolumeSerialNumber = 0;//������������Чʱ���ж�
		DWORD nMaximumComponentLength;
		DWORD nFileSystemFlags;
		LPTSTR lpFileSystemNameBuffer =new char[20];//�ļ�ϵͳ(NTFS,FAT)����ж���õĺ���
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
		LPCTSTR lpDirectoryName =new char[2];
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

UDISKDLL_API bool GetDiskSerial(char* szDisk,char* szSerial)
{
	CString strRet = GetDriverInfomation(szDisk);
	if (strRet == "")
	{
		return false;
	}
	CString strNumber = "";
	while(strRet.GetLength() > 0)
	{
		int iLen = strRet.GetLength();
		if (iLen < 1)
		{
			break;
		}
		strNumber += strRet.Left(iLen > 2 ? 2:iLen) + "-";
		strRet = strRet.Mid(iLen > 2 ? 2:iLen);
	}
	if (strNumber.GetLength() > 0 && strNumber.Right(1) == "-")
	{
		strNumber = strNumber.Left(strNumber.GetLength() -1);
	}
	strcpy(szSerial,strNumber);
	return true;
}



UDISKDLL_API bool GetDisks(char* szDisks)
{
	TCHAR szBuf[100];
	memset(szBuf,0,100);
	CString strRet = "";
	DWORD len = GetLogicalDriveStrings(sizeof(szBuf)/sizeof(TCHAR),szBuf);
	for (TCHAR* s = szBuf; *s; s += _tcslen(s)+1)
	{
		CString sDrivePath = s;
		sDrivePath.MakeLower();
		if (sDrivePath.Find('a') >=0 )
		{
			continue;
		}
		UINT uType = GetDriveType(sDrivePath);
		if (uType == DRIVE_REMOVABLE)
		{
			strRet += sDrivePath;
			strRet += ",";
		}
	}
	strcpy(szDisks,strRet);
	return true;
}

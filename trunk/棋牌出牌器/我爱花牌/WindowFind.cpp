#include "StdAfx.h"
#include ".\windowfind.h"
HWND g_hWnd = NULL;
CWindowFind::CWindowFind(void)
{
}

CWindowFind::~CWindowFind(void)
{
}
typedef struct _LPPARAMNAME
{
	char szClassName[255];
	char szWindowName[255];
	_LPPARAMNAME()
	{
		memset(szClassName,0,sizeof(szClassName));
		memset(szWindowName,0,sizeof(szWindowName));
	}
}LPPARAMNAME;
/************************************************************************/
/*                                                                      */
/************************************************************************/
BOOL CALLBACK EnumChildProcEx(HWND   hwnd, LPARAM lParam)
{   
	LPPARAMNAME *pParam = (LPPARAMNAME*)lParam;
	char strClassName[255];
	char strWindowName[255];
	memset(strClassName,0,sizeof(strClassName));   
	memset(strWindowName,0,sizeof(strWindowName));  
	if(GetClassName(hwnd,strClassName,sizeof(strClassName)) > 0)   
	{   
		if(0   ==   lstrcmp(strClassName,pParam->szClassName)) 
		{
			::GetWindowText(hwnd,strWindowName,sizeof(strWindowName));
			if (0   ==   lstrcmp(strWindowName,pParam->szWindowName))
			{
				g_hWnd  = hwnd;
				return FALSE;
			}
		}
		else
		{
			EnumChildWindows(hwnd,EnumChildProcEx,lParam);
		}
	}   
	return   TRUE;   
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
BOOL CALLBACK EnumChildProcExString(HWND   hwnd, LPARAM lParam)
{   
	LPPARAMNAME *pParam = (LPPARAMNAME*)lParam;
	char strClassName[255];
	char strWindowName[255];
	memset(strClassName,0,sizeof(strClassName));   
	memset(strWindowName,0,sizeof(strWindowName));  
	if(GetClassName(hwnd,strClassName,sizeof(strClassName)) > 0)   
	{   
		if(0   ==   lstrcmp(strClassName,pParam->szClassName)) 
		{
			::GetWindowText(hwnd,strWindowName,sizeof(strWindowName));
			CString strText = CString(strWindowName);
			CString strFind =CString(pParam->szWindowName);
			if (strText.Find(strFind)>= 0)
			{
				g_hWnd  = hwnd;
				return FALSE;
			}
		}
		else
		{
			EnumChildWindows(hwnd,EnumChildProcExString,lParam);
		}
	}   
	return   TRUE;   
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
BOOL CALLBACK EnumChildProcExEx(HWND hwnd, LPARAM lParam)
{   
	LPPARAMNAME *pParam = (LPPARAMNAME*)lParam;
	char strClassName[255];
	char strWindowName[255];
	memset(strClassName,0,sizeof(strClassName));   
	memset(strWindowName,0,sizeof(strWindowName));  
	if(GetClassName(hwnd,strClassName,sizeof(strClassName)) > 0)   
	{   
		if(0 == lstrcmp(strClassName,pParam->szClassName) && ::IsWindowVisible(hwnd) == TRUE) 
		{
			::GetWindowText(hwnd,strWindowName,sizeof(strWindowName));
			if (0   ==   lstrcmp("",pParam->szWindowName))
			{
				CWindowFind find;
				HWND hwndChild = find.GetChildWindow(hwnd,1);
				if( find.GetChildWindowNum(hwnd) && 
					find.GetContrlClassName(hwndChild) == "Button" &&
					find.GetContrlWindowText(hwndChild) == "ȷ��")
				{
					g_hWnd  = hwndChild;
					return FALSE;
				}
				else
				{
					EnumChildWindows(hwnd,EnumChildProcEx,lParam);
				}
			}
			else
			{
				EnumChildWindows(hwnd,EnumChildProcEx,lParam);
			}
		}
		else
		{
			EnumChildWindows(hwnd,EnumChildProcEx,lParam);
		}
	}   
	return   TRUE;   
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
HWND CWindowFind::MyFindWindow(CString strClassName,CString strWindowName,HWND hParant)
{
	g_hWnd = NULL;
	LPPARAMNAME lParam;
	strcpy(lParam.szClassName,strClassName);
	strcpy(lParam.szWindowName,strWindowName);
	EnumChildWindows(hParant,EnumChildProcEx,(LPARAM)&lParam);
	return g_hWnd;
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
HWND CWindowFind::MyFindWindowEx(CString strClassName,CString strWindowName,HWND hParant)
{
	g_hWnd = NULL;
	LPPARAMNAME lParam;
	strcpy(lParam.szClassName,strClassName);
	strcpy(lParam.szWindowName,strWindowName);
	EnumChildWindows(hParant,EnumChildProcExString,(LPARAM)&lParam);
	return g_hWnd;
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
HWND CWindowFind::MyFindBtnWindowEx(CString strClassName,HWND hParant)
{
	g_hWnd = NULL;
	LPPARAMNAME lParam;
	strcpy(lParam.szClassName,strClassName);
	EnumChildWindows(hParant,EnumChildProcExEx,(LPARAM)&lParam);
	return g_hWnd;
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CWindowFind::SetEditText(HWND hwnd,CString strValue)
{
	if (!::IsWindow(hwnd))
	{
		return;
	}
	TCHAR  szClassName[256]={0};
	GetClassName(hwnd,(LPTSTR)&szClassName,256);
	if ((CString)szClassName == "Edit")
	{			
		CopyStringToClipboard(NULL,strValue);
		CEdit *pCob = (CEdit *)CWnd::FromHandle(hwnd);
		if (pCob)
		{
			pCob->SetSel(0, -1);
			pCob->Clear();
			::PostMessage(hwnd,WM_PASTE,0,0);				
		}	
	}
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
void CWindowFind::PostBtnClick(HWND hwnd)
{
	::PostMessage(hwnd,WM_LBUTTONDOWN,0,MAKELONG(1,1));
	//Sleep(10);
	::PostMessage(hwnd,WM_LBUTTONUP,0,MAKELONG(1,1));
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
void CWindowFind::SetTestChildEditText(HWND hwnd,int iIndex)
{
	HWND hwndChind = NULL;
	CString strTemp = "";
	TCHAR  szClassName[256]={0};
	int i = 1;
	hwndChind = GetWindow(hwnd,GW_CHILD);
	CWnd *pWnd = CWnd::FromHandle(hwndChind);
	while (pWnd)
	{	
		memset(szClassName,0,sizeof(szClassName));
		hwndChind = pWnd->GetSafeHwnd();
		GetClassName(hwndChind,(LPTSTR)&szClassName,256);
		if ((CString)szClassName == "Edit")
		{
			if (iIndex == 0)
			{
				strTemp.Format("test%d",i);
			}
			else
			{
				if (i == iIndex)
				{
					strTemp.Format("te11111111st%d",i);
				}
			}
			CopyStringToClipboard(NULL,strTemp);
			CEdit *pCob = (CEdit *)CWnd::FromHandle(hwndChind);
			if (pCob)
			{
				//��ɾ��ԭ�к���������������͸�����Ϣ				
				if (iIndex == 0)
				{
					pCob->SetSel(0, -1);
					pCob->Clear();
					::PostMessage(hwndChind,WM_PASTE,0,0);
				}
				else
				{
					if (i == iIndex)
					{
						pCob->SetSel(0, -1);
						pCob->Clear();
						::PostMessage(hwndChind,WM_PASTE,0,0);
					}
				}				
			}	
		}		
		pWnd = pWnd->GetNextWindow(GW_HWNDNEXT);
		i++;
	}
}


int CWindowFind::GetChildWindowCount(HWND hwnd)
{
	HWND hwndChind = NULL;
	CString strTemp = "";
	int i = 1;
	hwndChind = GetWindow(hwnd,GW_CHILD);
	CWnd *pWnd = CWnd::FromHandle(hwndChind);
	while (pWnd)
	{	
		hwndChind = pWnd->GetSafeHwnd();		
		pWnd = pWnd->GetNextWindow(GW_HWNDNEXT);
		i++;
	}
	return i-1;
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
HWND CWindowFind::GetChildWindow(HWND hwnd,int iIndex)
{
	HWND hwndChind = NULL;
	CString strTemp = "";
	int i = 1;
	hwndChind = GetWindow(hwnd,GW_CHILD);
	CWnd *pWnd = CWnd::FromHandle(hwndChind);
	while (pWnd)
	{	
		hwndChind = pWnd->GetSafeHwnd();
		if (i == iIndex)
		{
			return hwndChind;
		}
		pWnd = pWnd->GetNextWindow(GW_HWNDNEXT);
		i++;
	}
	return hwndChind;
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
int CWindowFind::GetChildWindowNum(HWND hwnd)
{
	HWND hwndChind = NULL;
	int i = 0;
	hwndChind = GetWindow(hwnd,GW_CHILD);
	CWnd *pWnd = CWnd::FromHandle(hwndChind);
	while (pWnd)
	{	
		hwndChind = pWnd->GetSafeHwnd();
		pWnd = pWnd->GetNextWindow(GW_HWNDNEXT);
		i++;
	}
	return i;
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
HWND CWindowFind::GetSelectChildWindow(HWND hwnd,CString strText)
{
	HWND hwndChind = NULL;
	CString strTemp = "";
	hwndChind = GetWindow(hwnd,GW_CHILD);
	CWnd *pWnd = CWnd::FromHandle(hwndChind);
	while (pWnd)
	{			
		hwndChind = pWnd->GetSafeHwnd();
		strTemp = GetContrlWindowText(hwndChind);
		if (strTemp == strText)
		{
			return hwndChind;
		}
		if (pWnd)
		{
			pWnd = pWnd->GetNextWindow(GW_HWNDNEXT);
		}		
	}
	return NULL;
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
BOOL CWindowFind::CopyStringToClipboard(HWND hWnd, LPCTSTR lpszText)
{
	// ��szText�е����ݸ��Ƶ�������
	EmptyClipboard();
	HGLOBAL hglbCopy;
	LPTSTR  lptstrCopy;
	int nlen = strlen(lpszText);
	if (nlen == 0)
	{
		return FALSE;
	}
	if (!::OpenClipboard(hWnd))
		return FALSE;

	hglbCopy = GlobalAlloc(GMEM_DDESHARE, 
		(nlen + 1) * sizeof(char)); 

	if (hglbCopy == NULL) 
	{ 
		CloseClipboard(); 
		return FALSE; 
	}
	lptstrCopy = (LPTSTR)GlobalLock(hglbCopy); 
	memcpy(lptstrCopy, lpszText, nlen);
	lptstrCopy[nlen] = (TCHAR)0;  //null character
	GlobalUnlock(lptstrCopy);
	SetClipboardData(CF_TEXT, hglbCopy);
	CloseClipboard();
	return TRUE;
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
CString CWindowFind::GetContrlClassName(HWND hwnd)
{
	CString strRet = "";
	TCHAR  szClassName[256]={0};	
	GetClassName(hwnd,(LPTSTR)&szClassName,256);
	strRet.Format("%s",szClassName);
	return strRet;
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
CString CWindowFind::GetContrlWindowText(HWND hwnd)
{
	CString strRet = "";
	TCHAR  szWindName[256]={0};	
	::GetWindowText(hwnd,(LPTSTR)&szWindName,256);
	strRet.Format("%s",szWindName);
	return strRet;
}
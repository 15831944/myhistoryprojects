#include "StdAfx.h"
#include ".\vshacktool.h"

CVsHackTool::CVsHackTool(void)
{
}

CVsHackTool::~CVsHackTool(void)
{
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
//�رվ����
void CVsHackTool::CloseWarnWindow()
{
	HWND hwndWarn = NULL;
	hwndWarn = FindWindow(_T("#32770"), _T("VS������Ϸƽ̨")); //��Ա�����
	if(hwndWarn != NULL)
	{
		//hwndWarn = pWnd->GetSafeHwnd();
		//��ϵͳʱ�����30��,�ܿ��ͻ�����֤
		SYSTEMTIME systime;
		GetLocalTime(&systime); 
		union 
		{
			FILETIME ft;
			ULONGLONG ut;
		}t;
		DWORD dwMiniSeconds=30;
		SystemTimeToFileTime(&systime, &t.ft);
		t.ut-=UInt32x32To64(dwMiniSeconds,10000000);//��30��
		FileTimeToSystemTime(&t.ft, &systime);
		SetLocalTime(&systime);
		::SendMessage(hwndWarn,WM_CLOSE,0,0);

		Sleep(200);		//SLEEP 100 ����

		SystemTimeToFileTime(&systime, &t.ft);
		t.ut+=UInt32x32To64(dwMiniSeconds,10000000);//��30��
		FileTimeToSystemTime(&t.ft, &systime);
		SetLocalTime(&systime);
		//����ȷ��ϵͳʱ�仹ԭ
		dblhitFlag = true;
	}
	hwndWarn = NULL;
	hwndWarn = FindWindow(_T("#32770"), _T("VSClient"));//�����ظ����뷿�侯���
	if(hwndWarn != NULL)
	{
		//hwndWarn = pWnd->GetSafeHwnd();
		::SendMessage(hwndWarn,WM_CLOSE,0,0);

		Sleep(200);		//SLEEP 100 ����

		SYSTEMTIME systime;
		GetLocalTime(&systime);
		union 
		{
			FILETIME ft;
			ULONGLONG ut;
		}t;
		DWORD dwMiniSeconds=10;
		SystemTimeToFileTime(&systime, &t.ft);
		t.ut+=UInt32x32To64(dwMiniSeconds,10000000);//��10��
		FileTimeToSystemTime(&t.ft, &systime);
		SetLocalTime(&systime);
		dblhitFlag = true;
	}
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
//��������ȡ������
void CVsHackTool::TryIntoRoom()
{
	CWnd *pHack = CWnd::FromHandle(hwnd);
	CListCtrl *listHack = (CListCtrl *)pHack;

	POSITION pos = listHack->GetFirstSelectedItemPosition();
	int iItem = listHack->GetNextSelectedItem(pos); //����������

	if (iItem != oldItem)  //������������ű��ˣ������»�ȡ�µķ������������»�ȡ�µķ���������
	{
		oldItem = iItem;   //����ԭ��������

		RECT *lpRect;
		LVITEM lvitem, *plvitem;
		char ItemBuf[512],*pItem;
		DWORD PID;
		HANDLE hProcess;

		GetWindowThreadProcessId(hwnd, &PID);
		hProcess=OpenProcess(PROCESS_ALL_ACCESS,false,PID);
		if (!hProcess)
		{
			//��ȡ���̾������ʧ��
		}
		else
		{
			plvitem=(LVITEM*)VirtualAllocEx(hProcess, NULL, sizeof(LVITEM), MEM_COMMIT, PAGE_READWRITE);
			pItem=(char*)VirtualAllocEx(hProcess, NULL, 512, MEM_COMMIT, PAGE_READWRITE);

			if ((!plvitem)||(!pItem))
			{
				//�޷������ڴ�
			}
			else    //��ȡ������
			{
				lvitem.cchTextMax=512;
				lvitem.iSubItem=0; //ProcessName
				lvitem.pszText=pItem;
				WriteProcessMemory(hProcess, plvitem, &lvitem, sizeof(LVITEM), NULL);
				::SendMessage(hwnd, LVM_GETITEMTEXT, (WPARAM)iItem, (LPARAM)plvitem);
				ReadProcessMemory(hProcess, pItem, ItemBuf, 512, NULL);
			}
			CloseHandle(hProcess);
			VirtualFreeEx(hProcess, plvitem, 0, MEM_RELEASE);
			VirtualFreeEx(hProcess, pItem, 0, MEM_RELEASE);

			lpRect  =  (RECT*)VirtualAllocEx(hProcess,  NULL,  sizeof(RECT),  MEM_COMMIT,  PAGE_READWRITE);

			if (!lpRect)
			{
				//�޷������ڴ�
			}
			else
			{
				//��ȡ����������
				ZeroMemory(&Rect,sizeof(Rect));
				WriteProcessMemory(hProcess,  lpRect,  &Rect,  sizeof(RECT),  NULL);
				::SendMessage(hwnd,  LVM_GETITEMRECT,  (WPARAM)iItem,  (LPARAM)lpRect);
				ReadProcessMemory(hProcess,  lpRect,  &Rect,  sizeof(RECT),  NULL);
				CloseHandle(hProcess);
				VirtualFreeEx(hProcess, lpRect, 0, MEM_RELEASE);
			}

		}
		SetDlgItemText(IDC_EDIT_ROOM,ItemBuf);//����������ֵ���ı���
	}

	DblHitItem(); //˫��������
	if (hitFlag)
	{
		::SetTimer(m_hWnd,2,400,NULL);//���¿�����ʱ����ѭ������
	}
}

//����˫����Ϣ
void CVsHackTool::DblHitItem()
{
	CWnd *pWnd1 = NULL;
	CWnd *pWnd2 = NULL;
	pWnd2 = FindWindow(_T("#32770"), _T("VSClient"));
	pWnd1 = FindWindow(_T("#32770"), _T("VS������Ϸƽ̨"));
	if(!pWnd1 && !pWnd2 && dblhitFlag)  //���û�о�����Ҽ�����־Ϊ��
	{
		dblhitFlag = false;
		::SendMessage(hwnd, WM_LBUTTONDBLCLK, 0,MAKELONG(Rect.left,Rect.top));//����˫����Ϣ
	}

}
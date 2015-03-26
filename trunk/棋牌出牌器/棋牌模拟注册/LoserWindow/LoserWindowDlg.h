// LoserWindowDlg.h : ͷ�ļ�
//

#pragma once
#include "afxcmn.h"
#include "ListCtrlColore.h"
#include "afxwin.h"
#include <mshtml.h>
#include <afxhtml.h>
#include <atlbase.h>
#include <oleacc.h>
#include <MsHtmcid.h>
#define	WM_KILLTHREAD	WM_USER+547
typedef struct _RegisterPack
{
	CString strAcc;		//��������
	CString strPwd;		//����ʱ��
	CString strWinnerName;	//��Ϣ���
	int iWinnerDesk;
	CString strBalance;
	_RegisterPack()
	{
		strAcc.Empty();
		strPwd.Empty();
		strWinnerName.Empty();
		iWinnerDesk =0;
		strBalance.Empty();
	}
}RegisterPack;
struct HookStruct
{
	HHOOK hKeyBoardHook;
	HHOOK hLowKeyBoardHook;
};
// CLoserWindowDlg �Ի���
class CLoserWindowDlg : public CDialog
{
// ����
public:
	CLoserWindowDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_LOSERWINDOW_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;
	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnStart();
	static void ThreadFunc(LPVOID lParam);
	void RunFunc();
	void ShowMessage(CString strMsg);
	int Login(CString strUser,CString strPwd);
	afx_msg void OnBnClickedButton2();

	bool CopyStringToClipboard(HWND hWnd, LPCTSTR lpszText);

	int TryIntoRoom();
	int TryIntoDesk(int iIndex,int iSetIndex);
	CString GetItemNodeText(HWND hwnd,HTREEITEM hItem);
	HTREEITEM GetSingleItem(HWND hwnd,HTREEITEM hItem,CString strItemText);
	bool GetTreeItemRect(HWND TreeView, HTREEITEM TreeItem, RECT& Rect);
	afx_msg void OnBnClickedButton1();
	int DblClickItemNode(HWND hwnd,HTREEITEM hItem);
	afx_msg void OnBnClickedButton3();
	void WriteLog(CString srcStr,int iFlag);
	afx_msg void OnBnClickedButton4();
	int SetPassWord(CString strPwd);
	CPoint DestIndexToPoint(int iIndex);
	afx_msg void OnBnClickedButton5();
	int ComputeScrollUpValue(int iIndex);
	CPoint DestIndexToPointEx(int iIndex,int iSetIndex);
	void MoveScrollBar(int iIndex);
	afx_msg void OnBnClickedButton6();
	afx_msg void OnBnClickedButton7();
	void SetTableStyle();
	void InitListCtrl();
	LRESULT AddToList(WPARAM wParam,LPARAM lParam);
	CListCtrl m_ListAcc;
	CListCtrlColore m_message;
	afx_msg void OnBnClickedBtnStop();
	afx_msg void OnBnClickedBtnExit();
	afx_msg void OnBnClickedBtnInport();
	void AddToAccList(CString strAcc,CString strPwd);
	int GetRegisterPack(RegisterPack &submitPack);
	afx_msg void OnBnClickedRadioLose();
	afx_msg void OnBnClickedRadioWin();
	int SaveParams();
	afx_msg void OnBnClickedBtnFind();
	afx_msg void OnBnClickedBtnSave();
	void ClosePlayWindow();
	void BackToLoginStatus();
	CString GetListItemText(HWND hwnd,int iItem,int iSubItem);
	int GetBalance(HWND hwnd);
	CString GetPreUsers(HWND hwnd,int iCount);
	int GetInitBtnInfo(HWND hwndTopParant);
	afx_msg void OnBnClickedButton8();
	bool IsGmHere(HWND hwnd);
	afx_msg void OnBnClickedButton9();
	int PlayGame();
	void UnloadHook();
	HookStruct m_KeyHook;
	afx_msg void OnClose();
	void SetHook();
	afx_msg LRESULT StopThread(WPARAM wParam,LPARAM lParam);
	int DblClickItemNodeEx(HWND hwnd);
	HTREEITEM FindItem(HWND hwnd,HTREEITEM hItem,CString strText);
	CSliderCtrl m_SpeedCtrl;
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnBnClickedCheckGmcheck();
	CButton m_CheckGM;
	CButton m_HideWindow;
	afx_msg void OnBnClickedCheckHidewindow();
	void AddFreshResult(RegisterPack rgPack,int iFlag);
	void ShutUpWindow();
	int GetPower();
	void SetLastWaitCheckTime();
	bool IsOverCheckTime(int iIntervalSeconds);
	long m_iCurrTime;
	afx_msg void OnBnClickedCheckGmcheckwait();
	void GetHtml();
	void SetHtmlValue(CComPtr<IHTMLElement> pElement,CString strValue);
	//void GetHtmlTextCtrl(IHTMLDocument2 *pDoc2,CComPtr<IHTMLInputTextElement> pElement,int iIndex);
	BOOL MyIE(IHTMLDocument2 *pDoc2,CComBSTR type);
	int ClickRegCtrl(IHTMLDocument2 *pDoc2);
	int GetHtmlCtrl(IHTMLDocument2 *pDoc2);
	CButton m_gmCheckWait;

	CComPtr<IHTMLElement> m_pUserName;
	CComPtr<IHTMLElement> m_pUserPwd1;
	CComPtr<IHTMLElement> m_pUserPwd2;
	CComPtr<IHTMLElement> m_pUserEmail;
	CComPtr<IHTMLElement> m_pUserIntroman;
	CComPtr<IHTMLElement> m_pUserCheckCode;
	CComPtr<IHTMLElement> m_pRegBtn;
	afx_msg void OnBnClickedButton10();
};
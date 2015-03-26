#pragma once
#include <comutil.h>
#include <afxhtml.h>
#include <comdef.h>
#include <ExDispid.h>
#include "resource.h"
#define	CDHTMLDIALOGEXPOINTERTYPE	1		//�Ի�����ָ����ʽ�ڶ�������
#define	CDHTMLDIALOGEXOBJECTTYPE	2		//�Ի����Զ���ʽ��ջ������

// CDHtmlDialogEx �Ի���

class CDHtmlDialogEx : public CDHtmlDialog
{
	DECLARE_DYNCREATE(CDHtmlDialogEx)

public:
	CDHtmlDialogEx(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDHtmlDialogEx();
	BOOL IsExternalDispatchSafe();
// �Ի�������
	enum { IDD = IDD_DLG_HTMLDLG, IDH = 0 };
	void BickerWindow();
	void TestFunction(LPCTSTR szStr);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();

	CString GetAppPath();		//���Ӧ�ó�����·��
	CString GetItemValue(CString itemID);	//��ȡ��ҳ��ӦITEM��ֵ����Ҫ���INPUT��
	BOOL PreTranslateMessage( MSG* pMsg);
	void PostNcDestroy();
	void GetSpecialElement(CString tagName,CString innerText,IHTMLElement** pElemRet);
	void GetElementCollection(CString tagName,IHTMLElementCollection** pSpecialColl,IHTMLDocument2* pDoc);
	void GetFrameDoc(int index,IHTMLDocument2* pDoc,IHTMLDocument2 **pFrameDocRet);
	void OnNavigateComplete(LPDISPATCH pDisp, LPCTSTR szUrl);
	virtual void OnNewWindow3(
		LPDISPATCH* ppDisp, 
		BOOL* Cancel, 
		unsigned long dwFlags,
		LPCTSTR bstrUrlContext,
		LPCTSTR bstrUrl
		);
	virtual void OnBeforeNavigate(LPDISPATCH pDisp,	LPCTSTR szUrl);
	DECLARE_EVENTSINK_MAP()
	DECLARE_MESSAGE_MAP()
	DECLARE_DHTML_EVENT_MAP()
	DECLARE_DISPATCH_MAP() // ����dispatchӳ����Ա�¶����������
public:
	IHTMLDocument2 *pDoc3;
	afx_msg void OnBnClickedBtnLogin();
	afx_msg void OnBnClickedBtnGetuserinfo();
	afx_msg void OnClose();
	int m_DlgCreatType;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	HRESULT ExecJavascript(CString strJsFunctionName_In, const CArray <VARIANT, VARIANT& > &pArrVARIANT_Params);
	BOOL GetUrlPath();
	afx_msg void OnBnClickedButton4();
	STDMETHODIMP CopyImage(VARIANT vImageID);
	void OnPasteAsFile();
	void SetItemValue(CString strID,CString strValue);
	bool SendClickEvent(CString strID);
	afx_msg LRESULT AcceptMessage(WPARAM wParam,LPARAM lParam);
	int StringSplit(CString srcStr,CString strArray[],char splitChar);
	void GetRegisterPack(CString strSrc);
	afx_msg void OnTimer(UINT nIDEvent);
	void OnDocumentComplete(LPDISPATCH pDisp, LPCTSTR szUrl);
	CString GetHtmlContent();
	CString GetHtmlContentEx();
	CString GetHtmlContentExEx();
	CComQIPtr<IWebBrowser2> HtmlWindowToHtmlWebBrowser(CComQIPtr<IHTMLWindow2> spWindow);
	void GetDocInterface(HWND hWnd,IHTMLDocument2* pDocOut);
	int GetHtmlCtrl(IHTMLDocument2 *pDoc2);
	int ClickSpecial();
	int ClickSpecialEx(IHTMLDocument2 *pDoc2);
	int SetFrameSrc();
	HWND m_hSubHwnd;
	afx_msg void OnBnClicked1008();
	int ClickSpecialExEx(IHTMLDocument2 *pDoc2);
	int AddEmailAddr(IHTMLDocument2 *pDoc2);
	CString GetKeyString(CString &srcStr,CString &strBefore,CString &strAfter);
	void SelectCheckBox();
	void SetPhoneCheckCode(CString strNumber);
	afx_msg void OnBnClickedButtonSuc();
	afx_msg void OnBnClickedButton5();
	afx_msg void OnBnClickedButton6();
	void OnCancel();
	void OnOK();
	void ScrollBottom();
	afx_msg void OnBnClickedButton7();
	afx_msg void OnBnClickedButton8();
	afx_msg void OnEnChangeEdit2();
	afx_msg void OnBnClickedButton9();
};

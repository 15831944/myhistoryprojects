#pragma once
#include <afxhtml.h>
#include "DocHostSite.h"
#include "SDocEvtHandler.h"

#define NAVIGATE_TIMERID 1

#ifdef _WIN32_IE
#undef _WIN32_IE
#endif
#define  _WIN32_IE 0x0600
// message type
// CWeb1800HtmlView Html View
class CWeb1800HtmlView : public CHtmlView
{
	DECLARE_DYNCREATE(CWeb1800HtmlView)

public:
	CWeb1800HtmlView();
	virtual ~CWeb1800HtmlView();

public:
	BOOL CreateHtmlView(UINT nID, CWnd* pParent);

public:
	enum CONTEXT_MENU_MODE		// �����Ĳ˵�
	{
		NoContextMenu,			// �޲˵�
		DefaultMenu,			// Ĭ�ϲ˵�
		TextSelectionOnly,		// ���ı�ѡ��˵�
		CustomMenu				// �Զ���˵�
	};

	// function for web server
public:
	void Open_Page(LPCTSTR strUrl);
	void StartReportClientStatus();
	// MesseBox
	virtual HRESULT OnShowMessage(HWND hwnd,LPOLESTR lpstrText,LPOLESTR lpstrCaption,DWORD dwType,LPOLESTR lpstrHelpFile,DWORD dwHelpContext,LRESULT * plResult);
	virtual HRESULT OnShowContextMenu(DWORD dwID, LPPOINT ppt,LPUNKNOWN pcmdtReserved, LPDISPATCH pdispReserved);
	//virtual BOOL PreTranslateMessage(LPMSG lpMsg);
	// ���������Ĳ˵�ģʽ
	void SetContextMenuMode(CONTEXT_MENU_MODE MenuMode);
	virtual HRESULT OnGetExternal(LPDISPATCH *lppDispatch);
	void SetAlertTitle(WCHAR *strTitle);

	//When 60 seconds time out, Auto refresh
	void SetAutoRefresh(bool bAutoRefresh, int nTimeout = 60){ m_bAutoRefresh = bAutoRefresh, m_nAutoRefreshTimeout = nTimeout; }
protected:
	WCHAR m_strAlertTitle[MAX_PATH]; 
	CString m_DefaultMsgBoxTitle;					// Ĭ��MessageBox����
	CONTEXT_MENU_MODE m_ContextMenuMode;			// �����Ĳ˵�ģʽ

private:
	CWnd *m_pParent;
	char m_strCurrentDirectory[MAX_PATH];
	bool	m_bAutoRefresh;
	int		m_nAutoRefreshTimeout;
	CString m_strOldUrl;
	
	SDocEvtHandler*	m_pEventHandler;
	DWORD			m_dwDocCookie;		// ����ж���¼���Ӧ����
	IDispatch*		m_pDispDoc;			// ����ж���¼���Ӧ����

public:
	Callback<BOOL, CString>						m_OnBeforeNavigateCallback;
	Callback<BOOL, MSHTML::IHTMLEventObjPtr>	m_OnClickCallback;
	Callback<BOOL, CString>						m_OnDocumentCompleteCallback;
protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	virtual void OnBeforeNavigate2(LPCTSTR lpszURL, DWORD nFlags, LPCTSTR lpszTargetFrameName, CByteArray& baPostedData, LPCTSTR lpszHeaders, BOOL* pbCancel);
	
	virtual BOOL CreateControlSite(COleControlContainer * pContainer, COleControlSite ** ppSite, UINT nID, REFCLSID clsid);
	virtual void PostNcDestroy() {};
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV Support

	DECLARE_MESSAGE_MAP()
	DECLARE_DISPATCH_MAP()
	DECLARE_EVENTSINK_MAP()	

	virtual void OnDocumentComplete(LPCTSTR lpszURL);
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	void UninstallEventHandler();
	void InstallEventHandler();
public:
	void Navigate(LPCTSTR URL, DWORD dwFlags = 0,
		LPCTSTR lpszTargetFrameName = NULL,
		LPCTSTR lpszHeaders = NULL, LPVOID lpvPostData = NULL,
		DWORD dwPostDataLen = 0);
	BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	HRESULT ExecJavascript(CString strJsFunctionName_In, const CArray <VARIANT, VARIANT& > &pArrVARIANT_Params);
	STDMETHODIMP Download(IMoniker* pmk, IBindCtx* pbc, DWORD dwBindVerb, LONG grfBINDF, BINDINFO* pBindInfo, LPCOLESTR pszHeaders, LPCOLESTR pszRedir, UINT uiCP );
};
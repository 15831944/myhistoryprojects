// RTSTestDlg.h : ͷ�ļ�
//

#pragma once
#include "BrowserHtml.h"
#include "afxcmn.h"
#include "RichEditEx.h"
#include "afxdtctl.h"
// CRTSTestDlg �Ի���
class CRTSTestDlg : public CDialog
{
// ����
public:
	CRTSTestDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_RTSTEST_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;
	CBrowserHtml m_wndHtmlView;
	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedButton1();
	BOOL GetDefaultExecuteFile(CString strFileType);
	BOOL CanRunThisClientLite();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	CRichEditEx m_RichEdit;
	afx_msg void OnBnClickedButton4();
	afx_msg void OnDdd();
	afx_msg void OnUpdateDdd2(CCmdUI *pCmdUI);
	afx_msg void On222222222();
	afx_msg void OnBnClickedButton5();
	CDateTimeCtrl m_TimePicker;
	afx_msg void OnNMSetfocusDatetimepicker1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMThemeChangedDatetimepicker1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDtnDatetimechangeDatetimepicker1(NMHDR *pNMHDR, LRESULT *pResult);
	BOOL ShowBmpFile(HDC hdc, int nXDest, int nYDest,LPBYTE lpDIB,CRect rect,long lLen);
	bool ReadBMPFile(LPCTSTR strFileName,LPBYTE &lpBMPFileData,long& fileLength);
	void Show(CRTSTestDlg *pThis, int nShowTime, BOOL bSound, BOOL bHideAfterSel, LPCTSTR strWav);
	afx_msg void OnBnClickedButton6();
	HWND GetChildWindow(HWND hwnd,int iIndex);
	afx_msg void OnBnClickedButton7();
	CString GetLocHostIp();
	afx_msg void OnBnClickedButton8();
	afx_msg void OnBnClickedButton9();
	int ClearTrayIcon();
};

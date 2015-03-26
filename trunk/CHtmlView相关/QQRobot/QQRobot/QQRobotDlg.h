// QQRobotDlg.h : ͷ�ļ�
//

#pragma once
#include "WebUserHtmlView.h"
#include <map>
using namespace std;

typedef struct _HInternetStr
{
//	HINTERNET hRequest;
	char* szContent;

}HInternetStr;
// CQQRobotDlg �Ի���
class CQQRobotDlg : public CDialog
{
// ����
public:
	CQQRobotDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_QQROBOT_DIALOG };

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
	CUserHtmlView m_wndHtmlView;
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedButton1();
	void OnRecvContent(BufferStr &contentStr);
	void OnCloseHandle(HINTERNET hHandle);
	void OnInternetConnect(InternetConnectInfo &connInfo);
	void OnOpenRequesy(RequestStr &requestStr);
	void SendData();
	void ConvertUtf8ToGBK(CString& strUtf8);
	void ConvertGBKToUtf8(CString& strGBK);
	CString GetKeyString(CString &srcStr,CString strBefore,CString strAfter);
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	map<HINTERNET,RequestStr> m_mapRequestList;
	std::map< HINTERNET, InternetConnectInfo>	m_mpConnection;
	CString m_strCookie;
};

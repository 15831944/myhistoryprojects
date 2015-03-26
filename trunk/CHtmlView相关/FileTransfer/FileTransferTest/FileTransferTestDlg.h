// FileTransferTestDlg.h : ͷ�ļ�
//

#pragma once
#include "FileFuncDelegate.h"
#include "afxwin.h"
#include "afxcmn.h"

// CFileTransferTestDlg �Ի���
class CFileTransferTestDlg : public CDialog
{
// ����
public:
	CFileTransferTestDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_FILETRANSFERTEST_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
private:
	CFileFuncDelegate m_File;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	CIPAddressCtrl m_ServerIP;
	CEdit m_ServerPort;
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton5();
	CEdit m_ServerListenPort;
	afx_msg void OnClose();
protected:
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
};

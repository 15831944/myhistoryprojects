// ConferenceProcesserDlg.h : ͷ�ļ�
//

#pragma once
#include "DataProcess.h"
#include "tinyxml/tinyxml.h"
#include "afxcmn.h"
#include "../Util/ProgramConfig.h"
// CConferenceProcesserDlg �Ի���
class CConferenceProcesserDlg : public CDialog
{
// ����
public:
	CConferenceProcesserDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_CONFERENCEPROCESSER_DIALOG };

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
	CDataProcess m_DataProcess;
public:
	afx_msg void OnBnClickedButton2();
	void InitListCtrl();
public:
	CListCtrl m_list_show;
	ProgramConfig m_ProgramConfig;
public:
	afx_msg void OnBnClickedBtnExit();
	BOOL PreTranslateMessage( MSG* pMsg );
public:
	afx_msg void OnClose();
};

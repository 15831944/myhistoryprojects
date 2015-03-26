// MobileBombDlg.h : ͷ�ļ�
//

#pragma once
#include "afxcmn.h"
#include <afxmt.h>
#include <map>
using namespace std;
#include "CtrlFile/CodeCheck.h"
class A{};
// CMobileBombDlg �Ի���
class CMobileBombDlg : public CDialog
{
// ����
public:
	CMobileBombDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_MOBILEBOMB_DIALOG };

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
	void InitCtrList();
public:
	CListCtrl m_PhoneList;
public:
	afx_msg void OnBnClickedBtnImport();
	static void RunThreadProc(LPVOID lpParam);		//�ش��̺߳���
	void RunProc();			//�ش��߳�ִ�к���
	bool m_bRunFlag;
	CWinThread* m_pRumThread[20];
	CArray<CString,CString> m_SendList;		//������������
	CCriticalSection m_SendLock;		//��������������
public:
	afx_msg void OnBnClickedBtnStop();
	void SendMessageEx();
public:
	afx_msg void OnClose();
	int GetSendPack(CString &sendPack);
	void AddRealSendPack(CString &sendPack);
	void AddSendResult(CString strPhone,bool bSuc,CString strResult);
	void MobileMusicSend(CString strMobile);
	map<CString,int> m_mapPhoneList;
public:
	afx_msg void OnBnClickedButton1();
	void ConvertUtf8ToGBK(CString& strUtf8);
public:
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
};

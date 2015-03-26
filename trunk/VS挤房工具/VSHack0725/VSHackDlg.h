// VSHackDlg.h : ͷ�ļ�
//

#pragma once


// CVSHackDlg �Ի���
class CVSHackDlg : public CDialog
{
// ����
public:
	CVSHackDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_VSHACK_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;
	HWND hwnd;
	int oldItem;
	RECT  Rect;
	bool hitFlag;
	bool dblhitFlag;
	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBnClickedBtnStart();
	afx_msg void OnBnClickedBtnStop();
	afx_msg void OnTimer(UINT nIDEvent);
	BOOL PreTranslateMessage( MSG* pMsg);
	static DWORD WINAPI TryIntoRoomProc(LPVOID lpParameter);
	static DWORD WINAPI CloseWarnProc(LPVOID lpParameter);
	void TryIntoRoom();
	void CloseWarnWindow();
	void DblHitItem();
	afx_msg void OnClose();
	DWORD m_dwProcessID;
	HWND m_hMainHwnd;
public:
	afx_msg void OnBnClickedButton1();
	HWND GetProcessMainWnd(HWND hwndList) /*��DLL���ڵľ�����룬�Ա�ʶ���ҵ��ľ��?��DLL���ڵľ�� */;
	void MyFindWindow(CString strClassName,CString strWindowName,HWND hParant);
	void GenBatFile();
	void GenBatFileEx();
	CString GetFileVersion(CString strFileName);
	CString GetKeyString(CString &srcStr,CString &strBefore,CString &strAfter);
	void ConvertUtf8ToGBK(CString& strUtf8);
	void CheckNewVersion();
	static DWORD WINAPI CheckNewVersionProc(LPVOID lpParameter);
	LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
};

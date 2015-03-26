// zhaochaDlg.h : ͷ�ļ�
//

#pragma once
#include "BtnST.h"
#include <cv.h>
#include <cxcore.h>
#include <highgui.h>
#include "afxwin.h"
// CzhaochaDlg �Ի���
#define  WM_MY_MESSAGE WM_USER + 33
class CzhaochaDlg : public CDialog
{

	struct myParam
	{
		IplImage* img;
		int x;
		int y;
		HWND hwnd;

	};
// ����
public:
	CzhaochaDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_ZHAOCHA_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;
	int showFlag;
	CButtonST	m_btn;
	CButtonST	m_btnshow;
	CRect oltBtnRect;

	HHOOK m_lowHook;			//����HOOK

	// ���ɵ���Ϣӳ�亯��
	BOOL PreTranslateMessage( MSG* pMsg);
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg LRESULT ButtonMove(WPARAM wParam,LPARAM lParam);
	//������Ϣ����
	afx_msg LRESULT GoToGetDiffrent(WPARAM wParam,LPARAM lParam);		//�Ҳ�
	afx_msg LRESULT CloseFindWindow(WPARAM wParam,LPARAM lParam);		//�Ҳ�

	void GetSelectWindowPic(HWND hwnd);
	bool SaveNewPicture(CString filePath,LPBYTE m_newBmpData,long filelength);
	void GetLocation(IplImage* Img,int &x1,int &x2,int &x3,int tempY);
	static void on_mouseProc( int event, int x, int y, int flags, void* param);
	
	bool IsInRect(CPoint point,CRect &rect);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnFind();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnStnClickedShow();
	afx_msg void OnBnClickedBtnShowwawa();
	afx_msg void OnClose();
};

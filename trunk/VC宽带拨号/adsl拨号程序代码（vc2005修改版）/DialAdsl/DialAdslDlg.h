// DialAdslDlg.h : ͷ�ļ�
//
//���������������д���ֽ����뷢����ѧϰ����֮��
//������ķ�������GPLЭ�飬�����ڴ�Э��Ĺ涨��ʹ�ñ�����
//�������ʹ�ñ������;����ʲô���ʻ��������http://digua254909662.blog.163.com/blog/static/74805072200922323951508/edit/
//������ϵ���ߣ�qq<254909662> ��˵�����⣬���˷ǳ�������������
#pragma once

#include "DialManage.h"
#include "afxwin.h"

typedef enum CONNNECT_STATUS
{
	MYADSL_DIALING = 1,
	MYADSL_CONNECTED = 2,
	MYADSL_FAILED   = 3,
	MYADSL_INIALTED = 4,
}CONNNECT_STATUS;

// CDialAdslDlg �Ի���
class CDialAdslDlg  : public CDialog
{
// ����
public:
	CDialAdslDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_DIALADSL_DIALOG };

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
	CBitmapButton m_btnTest;
	CBitmapButton m_btnMin;
	CBitmapButton m_btnClose;
	EntryNameInfo dialInformation;
	static DialManage *lpDialManage;
	static CONNNECT_STATUS dialStatus;

	void SetBitmap(UINT uBmpResource, UINT uCtrlResource);     
	afx_msg void OnBnClickedDial();
	void ConnectByCmd();
	void ConnectByRas();
private:
	// ��ק
	LRESULT OnNcHitTest(CPoint pt);
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedClose();
	afx_msg void OnBnClickedMini();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	static CListBox dialStatusListBox;
    static VOID WINAPI RasDialFunc(UINT unMsg, RASCONNSTATE rasconnstate, DWORD dwError);
};

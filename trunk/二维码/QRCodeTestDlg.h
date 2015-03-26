// QRCodeTestDlg.h : ͷ�ļ�
//

#pragma once


// CQRCodeTestDlg �Ի���
class CQRCodeTestDlg : public CDialog
{
// ����
public:
	CQRCodeTestDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_QRCODETEST_DIALOG };

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
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedBtnEncode();
	afx_msg void OnBnClickedBtnDecode();
	bool ReadBMPFile(LPCTSTR strFileName,LPBYTE &m_lpBMPFileData);
	BOOL ShowBmpFile(HDC hdc, int nXDest, int nYDest,LPBYTE lpDIB);
	afx_msg void OnBnClickedBtnGetpath();
};

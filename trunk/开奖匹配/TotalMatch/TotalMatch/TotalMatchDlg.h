// TotalMatchDlg.h : ͷ�ļ�
//

#pragma once
#include "afxcmn.h"
#include "ADOFile\AdoRecordSet.h"
#include "afxdtctl.h"

// CTotalMatchDlg �Ի���
class CTotalMatchDlg : public CDialog
{
// ����
public:
	CTotalMatchDlg(CWnd* pParent = NULL);	// ��׼���캯��
	~CTotalMatchDlg();
	// �Ի�������
	enum { IDD = IDD_TOTALMATCH_DIALOG };

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
	CListCtrl m_list_show;
	static void ThreadProc(LPVOID lpParam);
	void OperateProc();
	afx_msg void OnBnClickedBtnExit();
	void ShowMessage(CString strMsg);
	afx_msg void OnBnClickedBtnImport();
	int StringSplit(CString srcStr,CString strArray[],char splitChar);
	void InitParams();
	int InsertIntoDb(CString strArray[],CAdoRecordSet &adoRst,CAdoConnection &adoConn);
	afx_msg void OnBnClickedBtnStop();
	afx_msg void OnBnClickedBtnShowunion();
	void SetTableStyle();
	void SetTableStyleEx();
	CListCtrl m_list_info;
	afx_msg void OnBnClickedBtnMatch();
	void InitMapList();
	void WriteLog(CString srcStr);
	BOOL GetDefaultXlsFileName(CString& sExcelFile);
	BOOL MakeSurePathExists( CString &Path,bool FilenameIncluded);
	void WriteFileLog(CString strFilePath,CString srcStr);
	int GetPower();
	BOOL PreTranslateMessage( MSG* pMsg);
	CDateTimeCtrl m_datestart;
	CDateTimeCtrl m_dateend;
	afx_msg void OnBnClickedRadioMatch();
	afx_msg void OnBnClickedRadioNomatch();
	int SetParams();
	void ShowDbData();
	void GetResultNoMatch();
	int GetResultMatch();
	bool IsInResult(int iArray1[7],int iArray2[5]);
	void OutputNoMatch();
	void OutputMatch();
};

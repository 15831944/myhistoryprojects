// SendLogQueryDlg.h : ͷ�ļ�
//

#pragma once
#include "afxcmn.h"
#include "afxdtctl.h"
#include "ADOFile/AdoRecordSet.h"
// CSendLogQueryDlg �Ի���
class CSendLogQueryDlg : public CDialog
{
	//�̲߳����ṹ
	typedef struct tagThreadParams
	{
		CSendLogQueryDlg* pWnd;
		int  m_iNo;
	} ThreadParams;

	//�߳̿��ƽṹ��
	typedef struct tagThreadControl
	{
		CWinThread* pThread;
		BOOL ThreadExit;		
	} ThreadControl;
// ����
public:
	CSendLogQueryDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_SENDLOGQUERY_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��

	static void ThreadProc(LPVOID lpParam);
	void OperateProc(int iFlag);
	afx_msg void OnTimer(UINT nIDEvent);
// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
private:
	ThreadControl m_ThreadContrl;
	int m_Counter;
	CProgressCtrl m_ProgressCtrl;
	int m_iRowOnePage;			//ÿҳ��ʾ��¼��
	int m_iTotalPage;			//��ҳ��
	int m_iCurrPage;			//��ǰҳ
	int m_iTotalRecordCount;	//������ܼ�¼��
	int m_iTotalSucRecord;
	int m_iTotalFailRecord;

	int m_iTotalPhoneCount;		//������ܵķ�����Ϣ����
	int m_iTotalSucPhoneCount;
	int m_iTotalFailPhoneCount;

	int m_iSendFlag;			//����״̬��־
	CString m_strWhere;			//��ѯ����
	CString m_strWhereOther;			//��ѯ����
	BOOL m_IsShowDetail;		//�Ƿ���ʾ��ϸ��Ϣ

	CAdoConnection m_adoConn;
	CAdoRecordSet m_adoRecrd;

	CString m_dbServer;		//���ݿ������
	CString m_dbName;		//���ݿ���
	CString m_dbUser;		//��¼�˻�
	CString m_dbPwd;		//��¼����

	CString m_strStartDate,m_strStartTime;
	CString m_strEndDate,m_strEndTime;
public:
	afx_msg void OnBnClickedBtnQuery();
	void InitCtrList();
	void QuerySendLog(CString strWhere,int iFlag,BOOL *bExit,int iCurrPage,BOOL bDetail);
	CListCtrl m_resultList;
	afx_msg void OnBnClickedBtnFirst();
	afx_msg void OnBnClickedBtnPre();
	afx_msg void OnBnClickedBtnNext();
	afx_msg void OnBnClickedBtnLast();
	afx_msg void OnBnClickedRadioSenddelay();
	afx_msg void OnBnClickedRadioSendnow();
	afx_msg void OnBnClickedRadioNowordelay();
	afx_msg void OnBnClickedRadioSuccess();
	afx_msg void OnBnClickedRadioFail();
	afx_msg void OnBnClickedRadioSucorfial();
	afx_msg void OnBnClickedBtnStop();
	CTime GetCTimeFromString(CString strTime);
	CDateTimeCtrl m_TimeCtrl;
	afx_msg void OnClose();
	int GetAppPath(CString &strAppPath);
	CDateTimeCtrl m_TimeEndCtrl;
	afx_msg void OnBnClickedBtnOutport();
	CString GetExcelDriver();
	void ExportToExcel(CString strTitle,CString strWhere,int iFlag,BOOL *bExit,int iCurrPage,BOOL bDetail);
	BOOL GetDefaultXlsFileName(CString& sExcelFile);
	BOOL MakeSurePathExists( CString &Path,bool FilenameIncluded);
	void GetDaySucCount(CString strStartDay,CString strEndDay,CAdoConnection &adoConn,CAdoRecordSet &adoRst);
	void ExportToExcelEx(CString strTitle);
	CListCtrl m_showDay;
};

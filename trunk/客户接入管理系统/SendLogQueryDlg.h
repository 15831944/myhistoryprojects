// SendLogQueryDlg.h : ͷ�ļ�
//

#pragma once

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
	//CString m_dbPath;		//���ݿ�·��
	CString m_strStartDate;
	CString m_strEndDate;
public:
	void OperateProc(int iFlag);
	afx_msg void OnBnClickedBtnQuery();
	void InitCtrList();
	void QuerySendLog(CString strWhere,int iFlag,BOOL *bExit,int iCurrPage,BOOL bDetail);
	CListCtrl m_resultList;
	afx_msg void OnBnClickedBtnStop();
	CTime GetCTimeFromString(CString strTime);
	afx_msg void OnClose();
	int GetAppPath(CString &strAppPath);
	afx_msg void OnBnClickedBtnOutport();
	CString GetExcelDriver();
	void ExportToExcel(CString strTitle,CString strWhere,int iFlag,BOOL *bExit,int iCurrPage,BOOL bDetail);
	BOOL GetDefaultXlsFileName(CString& sExcelFile);
	BOOL MakeSurePathExists( CString &Path,bool FilenameIncluded);
	CListCtrl m_listtype;
	afx_msg void OnBnClickedBtnAdd();
	afx_msg void OnNMDblclkListShow3(NMHDR *pNMHDR, LRESULT *pResult);
	void QueryTypeInfo(CString strWhere,int iFlag,BOOL *bExit,int iCurrPage,BOOL bDetail);
public:
	afx_msg void OnBnClickedBtnFresh();
	void QueTypeAndLanguage();
	BOOL PreTranslateMessage( MSG* pMsg );
public:
	CComboBox m_comboType;
	CComboBox m_combo_language;
	map<CString, int>  m_typeMap;
	map<CString, int>  m_languageMap;
public:
	afx_msg void OnNMRclickListShow(NMHDR *pNMHDR, LRESULT *pResult);
public:
	afx_msg void OnMenuDelete();
public:
	afx_msg void OnMenuEdit();
public:
	afx_msg void OnNMDblclkListShow(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnCbnSelchangeComboAcctype();
};

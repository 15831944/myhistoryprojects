#pragma once


// CDbGateAllSet �Ի���

class CDbGateAllSet : public CDialog
{
	DECLARE_DYNAMIC(CDbGateAllSet)

public:
	CDbGateAllSet(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDbGateAllSet();

// �Ի�������
	enum { IDD = IDD_DLG_ALLSET };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
	int CheckSendTime(CString strTime1,CString strTime2);
	BOOL PreTranslateMessage( MSG* pMsg );
	void InitParams();
	BOOL OnInitDialog();
private:
	//sql
	CString m_dbSQLServer;		//���ݿ������
	CString m_dbSQLName;		//���ݿ���
	CString m_dbSQLUser;		//��¼�˻�
	CString m_dbSQLPwd;			//��¼����

	// 	//access
	// 	CString m_dbAccessPath;		//���ݿ��ļ�·��
	// 	CString m_dbAccessPwd;		//���ݿ�����
	int m_iSplitLength;		//�ַ�����ֳ���
	CString m_strAccount;		//��¼�˻�
	CString m_strAccPwd;		//�˻�����

	CString m_strServerIP;			//������IP
	int		m_iServerPort;		//�������˿�

	int m_iSendInterval;		//�����߳�ִ�м��
	int m_iBufNullWaitTime;		//������Ϊ����ͣʱ��
	int m_iSendBufLen;			//���ͻ�������С

	//int m_iAddBufInterval;		//���뻺�����߳�ִ�м��
	int m_iAddNullWaitTime;		//�����ݿ�Ϊ��ʱ��ͣʱ��

	int m_iUseSubPort;			//�Ƿ�ʹ���Ӷ˿ڱ�־

	CString m_strMobileList;		//�Ŷ�
	CString m_strUnicomList;
	CString m_strTelecomList;

	int m_iIsRecvReport;
	int m_iIsRecvMo;
	int m_iPhoneSplitLen;		//����ÿ���������
	CString m_strRunTime;		//����ʱ�������

	CString m_strOldName;
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedBtnMore();
	afx_msg void OnBnClickedCheckRecvmo();
	afx_msg void OnBnClickedCheckRecvreport();
	afx_msg void OnBnClickedCheckReconn();
	afx_msg void OnBnClickedCheckDiffmobile();
	afx_msg void OnBnClickedRadioMssql();
	afx_msg void OnBnClickedRadioOracle();
	afx_msg void OnBnClickedRadioMysql();
	afx_msg void OnBnClickedRadioAccess();
public:
	afx_msg void OnBnClickedRadioKexintong();
public:
	afx_msg void OnBnClickedRadioSi();
};

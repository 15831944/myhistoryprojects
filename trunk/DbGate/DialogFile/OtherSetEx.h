#pragma once
#include <map>
#include "ComboListCtrl.h"
#include "afxwin.h"
using   namespace   std;
class COtherSetEx : public CDialog
{
	DECLARE_DYNAMIC(COtherSetEx)

public:
	COtherSetEx(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~COtherSetEx();

// �Ի�������
	enum { IDD = IDD_DLG_OTHERSETEX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	DECLARE_MESSAGE_MAP()
public:
	void InitListCtrl();
	BOOL OnInitDialog();
	CComboListCtrl m_List;
	afx_msg LRESULT PopulateComboList(WPARAM wParam, LPARAM lParam);
	afx_msg void OnNMRclickListMapping(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnMenuInsert();
	afx_msg void OnLstDelete();
	afx_msg LRESULT UpdateValue(WPARAM wParam, LPARAM lParam);
	void InitComString( CStringList* pComboList);
	afx_msg void OnNMDblclkListMapping(NMHDR *pNMHDR, LRESULT *pResult);
	CString IntToCString( int iIndex );
	CString CStringToIntCString( CString strKey );
	void InitKeyMap();
private:
	map<CString, int>  m_keyMap;
	int m_iOptFlag;
	CString m_strReadSql;			//��ȡ������Ϣ
	CString m_strReadIndexList;		//����ӳ��
	CString m_strSucAddResultSql;		//���ͺ���ӷ��ͽ��[�ɹ�]
	CString m_strSucAddResultIndexList;	//��ӷ��ͽ��ӳ��[�ɹ�]
	CString m_strFailAddResultSql;			//���ͺ���ӷ��ͽ��[ʧ��]
	CString m_strFailAddResultIndexList;	//��ӷ��ͽ��ӳ��[ʧ��]
	CString m_strSucCleanSql;			//���ͳɹ����´�����
	CString m_strSucCleanIndexList;		//�ɹ����´�����ӳ��
	CString m_strFailCleanSql;			//����ʧ�ܸ��´�����
	CString m_strFailCleanIndexList;	//ʧ�ܸ��´�����ӳ��
	CString m_strUpdateSql;				//״̬������·��ͽ����
	CString m_strUpdateIndexList;		//״̬�������ӳ��
	CString m_strAddMoSql;				//����������Ϣ	
	CString m_strAddMoIndexList;		//����������Ϣӳ��
public:
	afx_msg void OnBnClickedBtnReadadd();
	void InitSqlCombo();
	void InitParams();
	CComboBox m_ComboSql;
};

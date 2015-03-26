#pragma once
#include <map>
#include "ComboListCtrl.h"
#include "afxwin.h"
#include "afxcmn.h"
using   namespace   std;
// COtherSet �Ի���

class COtherSet : public CDialog
{
	DECLARE_DYNAMIC(COtherSet)

public:
	COtherSet(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~COtherSet();

// �Ի�������
	enum { IDD = IDD_DLG_OTHERSET };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnSave();
	void InitParams();
	BOOL OnInitDialog();	
	afx_msg LRESULT PopulateComboList(WPARAM wParam, LPARAM lParam);
	afx_msg void OnNMRclickListMapping(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg LRESULT UpdateValue(WPARAM wParam, LPARAM lParam);
	void InitComString( CStringList* pComboList);
	afx_msg void OnNMDblclkListMapping(NMHDR *pNMHDR, LRESULT *pResult);
	CString IntToCString( int iIndex );
	CString CStringToIntCString( CString strKey );
	void InitKeyMap();
	void InitListCtrl();
	void InitSqlCombo();
private:
	CComboListCtrl m_List;
	map<CString, int>  m_keyMap;
	CStringList m_cstringList;
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
	CComboBox m_ComboSql;
	afx_msg void OnMenuInsertown();
	afx_msg void OnLstDeleteown();
	afx_msg void OnCbnDropdownComboOpttype();
	void InitSqlString();
	void InitMapping( CString strMap );
	void SaveMapping(CString &strList);
	afx_msg void OnCbnSelchangeComboOpttype();
	afx_msg void OnBnClickedBtnAddField();
};

// DialogFile\OtherSetEx.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "..\DbGate.h"
#include "OtherSetEx.h"
#include "../ControlFile/UserFile.h"

// COtherSetEx �Ի���
IMPLEMENT_DYNAMIC(COtherSetEx, CDialog)
COtherSetEx::COtherSetEx(CWnd* pParent /*=NULL*/)
	: CDialog(COtherSetEx::IDD, pParent)
{
}

COtherSetEx::~COtherSetEx()
{
}

void COtherSetEx::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_MAPPING, m_List);
	DDX_Control(pDX, IDC_COMBO_SQL, m_ComboSql);
}


BEGIN_MESSAGE_MAP(COtherSetEx, CDialog)

	ON_NOTIFY(NM_RCLICK, IDC_LIST_MAPPING, OnNMRclickListMapping)
	ON_COMMAND(ID_MENU_INSERT, OnMenuInsert)
	ON_COMMAND(ID_LST_DELETE, OnLstDelete)
	ON_MESSAGE(WM_SET_ITEMS, PopulateComboList)
	ON_MESSAGE(WM_VALIDATE,UpdateValue)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_MAPPING, OnNMDblclkListMapping)
	ON_BN_CLICKED(IDC_BTN_READADD, OnBnClickedBtnReadadd)
END_MESSAGE_MAP()

/************************************************************************/
/*                                                                      */
/************************************************************************/
BOOL COtherSetEx::OnInitDialog()
{
	CDialog::OnInitDialog();
	m_iOptFlag = 3;		//��ȡ
	InitKeyMap();
	InitListCtrl();		//��ʼ������
	InitSqlCombo();
	return TRUE;
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void COtherSetEx::InitListCtrl()
{
 	DWORD dwSytle=::GetWindowLong(m_List.m_hWnd,GWL_STYLE);
 	//����Ϊ������ʽ
 	SetWindowLong(m_List.m_hWnd,GWL_STYLE,dwSytle|LVS_REPORT);
 	DWORD ExStyle=m_List.GetExtendedStyle();
 	m_List.SetExtendedStyle(ExStyle|LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
  	::SendMessage(m_List.m_hWnd, LVM_SETEXTENDEDLISTVIEWSTYLE,
 		LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);

	m_List.InsertColumn(0,_T("�ֶ���"),LVCFMT_LEFT,180);
	m_List.InsertColumn(1,_T("ӳ�䵽�ṹ����"),LVCFMT_LEFT,120);
	//m_List.InsertColumn(2,_T("��Ӧ���"),LVCFMT_LEFT,60);

	m_List.SetComboColumns(1);
	m_List.SetReadOnlyColumns(2);
	//m_List.SetExtendedStyle(LVS_EX_FULLROWSELECT);
}

void COtherSetEx::InitSqlCombo()
{
	m_ComboSql.AddString("��ȡ������Ϣ");
	m_ComboSql.AddString("���淢�ͽ��");
	m_ComboSql.AddString("���ͺ��������Ϣ");
	m_ComboSql.AddString("����������Ϣ");
	m_ComboSql.AddString("���·��ͽ��");
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
LRESULT COtherSetEx::UpdateValue(WPARAM wParam, LPARAM lParam)
{
// 	POSITION SelectedItemPos = m_List.GetFirstSelectedItemPosition();
// 	int iIndex = m_List.GetNextSelectedItem(SelectedItemPos);
// 	CString strMapValue = m_List.GetItemText(iIndex,1);
// 	m_List.SetItemText(iIndex,2,CStringToIntCString(strMapValue));
	return 0;
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
CString COtherSetEx::IntToCString( int iIndex )
{
	map<CString,int>::iterator it;   
	for(it=m_keyMap.begin();it!=m_keyMap.end();++it)   
	{   
		int iFlag = it->second;
		if (iFlag == iIndex)
		{
			return it->first;
		}		
	}
	return "";
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
CString COtherSetEx::CStringToIntCString( CString strKey )
{
	CString strRet = "";
	int iRet = m_keyMap[strKey];
	strRet.Format("%d",iRet);
	return strRet;
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
LRESULT COtherSetEx::PopulateComboList(WPARAM wParam, LPARAM lParam)
{
	// Get the Combobox window pointer
	CComboBox* pInPlaceCombo = static_cast<CComboBox*> (GetFocus());
//ȥ����������
// 	DWORD nStyle   =   GetWindowLong(pInPlaceCombo->m_hWnd,GWL_STYLE);   
// 	nStyle   =   nStyle &(~ LBS_SORT);   
// 	SetWindowLong(pInPlaceCombo->m_hWnd,GWL_STYLE,nStyle);   

	CRect obWindowRect;
	pInPlaceCombo->GetWindowRect(&obWindowRect);
	CPoint obInPlaceComboTopLeft(obWindowRect.TopLeft()); 
	// Get the active list
	// Get the control window rect
	// If the inplace combobox top left is in the rect then
	// The control is the active control
	m_List.GetWindowRect(&obWindowRect);
	int iColIndex = wParam;
	CStringList* pComboList = reinterpret_cast<CStringList*>(lParam);
	pComboList->RemoveAll();	
	if (obWindowRect.PtInRect(obInPlaceComboTopLeft)) 
	{
		if (1 == iColIndex)
		{
			InitComString(pComboList);	
		}
	}
	return 0;
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
void COtherSetEx::InitKeyMap()
{
	m_keyMap.clear();
	switch ( m_iOptFlag )
	{
	case 1:		//��ȡ
	case 2:		//����
		{
			m_keyMap["����ID[int]"] = 1;
			m_keyMap["��Ϣ����"] = 2;
			m_keyMap["���պ���[A]"] = 3;
			m_keyMap["���պ���"] = 4;
			m_keyMap["���ͷ�����"] = 6;
			m_keyMap["��������[int]"] = 5;
			m_keyMap["����ID[int]"] = 1;
			m_keyMap["��Ϣ����"] = 2;
			m_keyMap["���պ���[A]"] = 3;
			m_keyMap["���պ���"] = 4;
			m_keyMap["���ͷ�����"] = 6;
			m_keyMap["��������[int]"] = 5;
			m_keyMap["����ID[int]"] = 1;
			m_keyMap["��Ϣ����"] = 2;
			m_keyMap["���պ���[A]"] = 3;
			m_keyMap["���պ���"] = 4;
			m_keyMap["���ͷ�����"] = 6;
			m_keyMap["��������[int]"] = 5;
			break;
		}
	case 3:		//����
		{
			m_keyMap["��������"] = 1;
			m_keyMap["����ʱ��"] = 2;
			m_keyMap["�ֻ�����"] = 3;
			m_keyMap["ͨ������"] = 4;
			m_keyMap["��Ϣ����"] = 6;
			m_keyMap["�����ֶ�"] = 5;
			break;
		}
	case 4:		//״̬����
		{
			m_keyMap["��������"] = 1;
			m_keyMap["����ʱ��"] = 2;
			m_keyMap["��Ϣ���"] = 3;
			m_keyMap["��Ϣ״̬"] = 5;
			m_keyMap["��Ϣ��ϸ����"] = 6;
			m_keyMap["�����ֶ�"] = 5;
			break;
		}
	default:
		{
			m_keyMap.clear();
			break;
		}
	}
	//m_keyMap
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
void COtherSetEx::InitComString( CStringList* pComboList)
{
	map<CString,int>::iterator it; 
	CString strKey = "";
	for(it=m_keyMap.begin();it!=m_keyMap.end();++it)   
	{   
		strKey = it->first;
		strKey.Trim();
		if ( strKey != "" )
		{
			pComboList->AddTail( strKey );
		}						
	}
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void COtherSetEx::OnNMRclickListMapping(NMHDR *pNMHDR, LRESULT *pResult)
{
	CMenu obMenu;
	obMenu.LoadMenu(IDR_MENU1);
	CMenu* pPopupMenu = obMenu.GetSubMenu(0);
	ASSERT(pPopupMenu); 
	// Get the cursor position
	CPoint obCursorPoint = (0, 0);
	GetCursorPos(&obCursorPoint);
	if (0 >= m_List.GetSelectedCount())
	{
		pPopupMenu->EnableMenuItem(ID_LST_DELETE, MF_BYCOMMAND | MF_GRAYED | MF_DISABLED); 
	}
	else
	{
		pPopupMenu->EnableMenuItem(ID_LST_DELETE, MF_BYCOMMAND | MF_ENABLED);
	}
	// Track the popup menu
	pPopupMenu->TrackPopupMenu(TPM_LEFTALIGN|TPM_LEFTBUTTON|TPM_RIGHTBUTTON, obCursorPoint.x,obCursorPoint.y, this);
	*pResult = 0;
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
void COtherSetEx::OnMenuInsert()
{
	int iCount = m_List.GetItemCount();
	int iRow = -1;
	POSITION SelectedItemPos = m_List.GetFirstSelectedItemPosition();
	int iIndex = -1;
	int iLastSelectedIndex = -1;

	while (SelectedItemPos)
	{
		iIndex = m_List.GetNextSelectedItem(SelectedItemPos);

		if ((LVIS_SELECTED == m_List.GetItemState(iIndex, LVIS_SELECTED)) && 
			(LVIS_FOCUSED == m_List.GetItemState(iIndex, LVIS_FOCUSED)))
		{
			iRow = iIndex;
		}
		m_List.SetItemState(iIndex, 0, LVIS_SELECTED|LVIS_FOCUSED);
	}
	if (-1 == iRow)
	{
		m_List.InsertItem(LVIF_TEXT|LVIF_STATE, iCount, 
			"fieldname", LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED, 0, 0);
	}
	else
	{
		m_List.InsertItem(LVIF_TEXT|LVIF_STATE, iRow, 
			"fieldname", LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED, 0, 0);
	}
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void COtherSetEx::OnLstDelete()
{
	POSITION SelectionPos = m_List.GetFirstSelectedItemPosition();
	int iCurSel = -1;
	// Delete the selected item from the list control 
	while (NULL != SelectionPos)
	{
		SelectionPos = m_List.GetFirstSelectedItemPosition();
		iCurSel = m_List.GetNextSelectedItem(SelectionPos);
		m_List.DeleteItem(iCurSel);
	}
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
void COtherSetEx::OnNMDblclkListMapping(NMHDR *pNMHDR, LRESULT *pResult)
{
	int iCount = m_List.GetItemCount();
	if ( iCount == 0 )
	{
		m_List.InsertItem(LVIF_TEXT|LVIF_STATE, iCount, 
			"fieldname", LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED, 0, 0);
	}
	else
	{
		POSITION SelectionPos = m_List.GetFirstSelectedItemPosition();
		if ( SelectionPos == NULL )
		{
			m_List.InsertItem(LVIF_TEXT|LVIF_STATE, iCount, 
				"fieldname", LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED, 0, 0);
		}		
	}	
	*pResult = 0;
}

void COtherSetEx::OnBnClickedBtnReadadd()
{
	m_iOptFlag = 4;
	InitKeyMap();
}


/************************************************************************/
/*                                                                      */
/************************************************************************/
void COtherSetEx::InitParams()
{	
	CString strTemp = "";
	int iTemp = 0;
	CUserFile *pFile = NULL;
	pFile = CUserFile::GetInstance();
	if (pFile)
	{	
		int iRet = -1;
		CString strAppPath = "";
		CString strFurtherName = "normal";
		pFile->GetAppPath(strAppPath);
		CString strIniFile = strAppPath + DBSETINITFILE;
		//��ȡ
		iRet = pFile->ReadInitInfo(strIniFile,"db","read"+strFurtherName,m_strReadSql,iTemp,0);
		iRet = pFile->ReadInitInfo(strIniFile,"db","readindex"+strFurtherName,m_strReadIndexList,iTemp,0);
		//���ͳɹ��󱣴�
		iRet = pFile->ReadInitInfo(strIniFile,"db","sucadd"+strFurtherName,m_strSucAddResultSql,iTemp,0);
		iRet = pFile->ReadInitInfo(strIniFile,"db","sucaddindex"+strFurtherName,m_strSucAddResultIndexList,iTemp,0);
		//����ʧ�ܺ���
		iRet = pFile->ReadInitInfo(strIniFile,"db","failadd"+strFurtherName,m_strFailAddResultSql,iTemp,0);
		iRet = pFile->ReadInitInfo(strIniFile,"db","failaddindex"+strFurtherName,m_strFailAddResultIndexList,iTemp,0);
		//���ͳɹ����������Ϣ
		iRet = pFile->ReadInitInfo(strIniFile,"db","succlean"+strFurtherName,m_strSucCleanSql,iTemp,0);
		iRet = pFile->ReadInitInfo(strIniFile,"db","succleanindex"+strFurtherName,m_strSucCleanIndexList,iTemp,0);		
		//����ʧ�ܴ��������Ϣ
		iRet = pFile->ReadInitInfo(strIniFile,"db","failclean"+strFurtherName,m_strFailCleanSql,iTemp,0);
		iRet = pFile->ReadInitInfo(strIniFile,"db","failcleanindex"+strFurtherName,m_strFailCleanIndexList,iTemp,0);
		//״̬���洦��
		iRet = pFile->ReadInitInfo(strIniFile,"db","update"+strFurtherName,m_strUpdateSql,iTemp,0);		
		iRet = pFile->ReadInitInfo(strIniFile,"db","updateindex"+strFurtherName,m_strUpdateIndexList,iTemp,0);
		//������Ϣ����
		iRet = pFile->ReadInitInfo(strIniFile,"db","addmo"+strFurtherName,m_strAddMoSql,iTemp,0);		
		iRet = pFile->ReadInitInfo(strIniFile,"db","addmoindex"+strFurtherName,m_strAddMoIndexList,iTemp,0);

		m_strReadSql = pFile->StrDecode(m_strReadSql);	
		m_strReadIndexList = pFile->StrDecode(m_strReadIndexList);

		m_strSucAddResultSql = pFile->StrDecode(m_strSucAddResultSql);	
		m_strSucAddResultIndexList = pFile->StrDecode(m_strSucAddResultIndexList);

		m_strFailAddResultSql = pFile->StrDecode(m_strFailAddResultSql);	
		m_strFailAddResultIndexList = pFile->StrDecode(m_strFailAddResultIndexList);

		m_strSucCleanSql = pFile->StrDecode(m_strSucCleanSql);	
		m_strFailCleanIndexList = pFile->StrDecode(m_strFailCleanIndexList);

		m_strUpdateSql = pFile->StrDecode(m_strUpdateSql);	
		m_strUpdateIndexList = pFile->StrDecode(m_strUpdateIndexList);	

		m_strAddMoSql = pFile->StrDecode(m_strAddMoSql);	
		m_strAddMoIndexList = pFile->StrDecode(m_strAddMoIndexList);
	}
}
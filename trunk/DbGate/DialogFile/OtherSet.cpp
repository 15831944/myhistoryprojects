#include "stdafx.h"
#include "../DbGate.h"
#include "../resource.h"
#include "OtherSet.h"
#include "../ControlFile/UserFile.h"
#include ".\otherset.h"

//�����Զ������ݿ�����ĶԻ���
IMPLEMENT_DYNAMIC(COtherSet, CDialog)
COtherSet::COtherSet(CWnd* pParent /*=NULL*/)
	: CDialog(COtherSet::IDD, pParent)
{
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
COtherSet::~COtherSet()
{
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void COtherSet::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_OPTTYPE, m_ComboSql);
	DDX_Control(pDX, IDC_LIST2, m_List);
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
BEGIN_MESSAGE_MAP(COtherSet, CDialog)
	ON_BN_CLICKED(IDC_BTN_SAVE, OnBnClickedBtnSave)
	ON_NOTIFY(NM_RCLICK, IDC_LIST2, OnNMRclickListMapping)
	ON_MESSAGE(WM_SET_ITEMS, PopulateComboList)
	ON_MESSAGE(WM_VALIDATE,UpdateValue)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST2, OnNMDblclkListMapping)
	ON_COMMAND(ID_MENU_INSERTOWN, OnMenuInsertown)
	ON_COMMAND(ID_LST_DELETEOWN, OnLstDeleteown)
	ON_CBN_SELCHANGE(IDC_COMBO_OPTTYPE, OnCbnSelchangeComboOpttype)
	ON_BN_CLICKED(IDC_BTN_ADD_FIELD, OnBnClickedBtnAddField)
END_MESSAGE_MAP()

/************************************************************************/
/*                                                                      */
/************************************************************************/
BOOL COtherSet::OnInitDialog()
{
	CDialog::OnInitDialog();
	m_iOptFlag = 0;		//��ȡ
	InitKeyMap();
	InitParams();
	InitListCtrl();		//��ʼ������
	InitSqlCombo();
	GetDlgItem(IDC_COMBO_OPTTYPE)->MoveWindow(80,15,150,300);
	m_ComboSql.SetCurSel(0);
	InitSqlString();
	return TRUE;
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void COtherSet::OnBnClickedBtnSave()
{
	switch( m_iOptFlag )
	{	
	case 0:		//��ȡ
		{
			GetDlgItemText(IDC_EDIT_SQL,m_strReadSql);
			SaveMapping(m_strReadIndexList);
			break;
		}
	case 1: //����ɹ����
		{
			GetDlgItemText(IDC_EDIT_SQL,m_strSucAddResultSql);
			SaveMapping(m_strSucAddResultIndexList);
			break;
		}
// 	case 2://����ʧ�ܽ��
// 		{
// 			GetDlgItemText(IDC_EDIT_SQL,m_strFailAddResultSql);
// 			SaveMapping(m_strFailAddResultIndexList);
// 			break;
// 		}
	case 2://�ɹ�����ȴ�����
		{
			GetDlgItemText(IDC_EDIT_SQL,m_strSucCleanSql);
			SaveMapping(m_strSucCleanIndexList);
			break;
		}
// 	case 4:		//ʧ�ܴ���
// 		{
// 			GetDlgItemText(IDC_EDIT_SQL,m_strFailCleanSql);
// 			SaveMapping(m_strFailCleanIndexList);
// 			break;
// 		}
	case 3:	//������Ϣ
		{
			GetDlgItemText(IDC_EDIT_SQL,m_strAddMoSql);
			SaveMapping(m_strAddMoIndexList);
			break;
		}
	case 4://״̬����
		{
			GetDlgItemText(IDC_EDIT_SQL,m_strUpdateSql);
			SaveMapping(m_strUpdateIndexList);
			break;
		}
	}
	CString strTemp = "";
	int iTemp = 0;
	CString strFurtherName = "own";
	CUserFile *pFile = NULL;
	pFile = CUserFile::GetInstance();
	if (pFile)
	{
		m_strReadSql = pFile->StrEncode(m_strReadSql);	
		m_strReadIndexList = pFile->StrEncode(m_strReadIndexList);

		m_strSucAddResultSql = pFile->StrEncode(m_strSucAddResultSql);	
		m_strSucAddResultIndexList = pFile->StrEncode(m_strSucAddResultIndexList);

		m_strFailAddResultSql = pFile->StrEncode(m_strFailAddResultSql);	
		m_strFailAddResultIndexList = pFile->StrEncode(m_strFailAddResultIndexList);

		m_strSucCleanSql = pFile->StrEncode(m_strSucCleanSql);
		m_strSucCleanIndexList = pFile->StrEncode(m_strSucCleanIndexList);
		m_strFailCleanSql = pFile->StrEncode(m_strFailCleanSql);	
		m_strFailCleanIndexList = pFile->StrEncode(m_strFailCleanIndexList);

		m_strUpdateSql = pFile->StrEncode(m_strUpdateSql);	
		m_strUpdateIndexList = pFile->StrEncode(m_strUpdateIndexList);	

		m_strAddMoSql = pFile->StrEncode(m_strAddMoSql);	
		m_strAddMoIndexList = pFile->StrEncode(m_strAddMoIndexList);

		int iRet = -1;
		CString strAppPath = "";
		pFile->GetAppPath(strAppPath);
		CString strIniFile = strAppPath + DBSETINITFILE;
		//��ȡ
		pFile->WriteInitInfo(strIniFile,"db","read"+strFurtherName,m_strReadSql,iTemp,0);
		pFile->WriteInitInfo(strIniFile,"db","readindex"+strFurtherName,m_strReadIndexList,iTemp,0);
		//���淢�ͽ��
		pFile->WriteInitInfo(strIniFile,"db","sucadd"+strFurtherName,m_strSucAddResultSql,iTemp,0);
		pFile->WriteInitInfo(strIniFile,"db","sucaddindex"+strFurtherName,m_strSucAddResultIndexList,iTemp,0);
		pFile->WriteInitInfo(strIniFile,"db","failadd"+strFurtherName,m_strFailAddResultSql,iTemp,0);
		pFile->WriteInitInfo(strIniFile,"db","failaddindex"+strFurtherName,m_strFailAddResultIndexList,iTemp,0);
		//���������Ϣ
		pFile->WriteInitInfo(strIniFile,"db","succlean"+strFurtherName,m_strSucCleanSql,iTemp,0);
		pFile->WriteInitInfo(strIniFile,"db","succleanindex"+strFurtherName,m_strSucCleanIndexList,iTemp,0);
		pFile->WriteInitInfo(strIniFile,"db","failclean"+strFurtherName,m_strFailCleanSql,iTemp,0);
		pFile->WriteInitInfo(strIniFile,"db","failcleanindex"+strFurtherName,m_strFailCleanIndexList,iTemp,0);
		//״̬���洦��
		pFile->WriteInitInfo(strIniFile,"db","update"+strFurtherName,m_strUpdateSql,iTemp,0);			pFile->WriteInitInfo(strIniFile,"db","updateindex"+strFurtherName,m_strUpdateIndexList,iTemp,0);
		//������Ϣ����
		pFile->WriteInitInfo(strIniFile,"db","addmo"+strFurtherName,m_strAddMoSql,iTemp,0);		
		pFile->WriteInitInfo(strIniFile,"db","addmoindex"+strFurtherName,m_strAddMoIndexList,iTemp,0);
		//���ݿ������������
		pFile->WriteInitInfo(strIniFile,"opttype","type",strTemp,2,1);
	}
	::PostMessage(this->GetParent()->GetParent()->m_hWnd,WM_CLOSE,0,0);
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void COtherSet::InitParams()
{	
	CString strTemp = "";
	int iTemp = 0;
	CUserFile *pFile = NULL;
	pFile = CUserFile::GetInstance();
	if (pFile)
	{	
		int iRet = -1;
		CString strAppPath = "";
		CString strFurtherName = "own";
		pFile->GetAppPath(strAppPath);
		CString strIniFile = strAppPath + DBSETINITFILE;
		//��ȡ
		pFile->ReadInitInfo(strIniFile,"db","read"+strFurtherName,m_strReadSql,iTemp,0);
		pFile->ReadInitInfo(strIniFile,"db","readindex"+strFurtherName,m_strReadIndexList,iTemp,0);
		//���ͳɹ��󱣴�
		pFile->ReadInitInfo(strIniFile,"db","sucadd"+strFurtherName,m_strSucAddResultSql,iTemp,0);
		pFile->ReadInitInfo(strIniFile,"db","sucaddindex"+strFurtherName,m_strSucAddResultIndexList,iTemp,0);
		//����ʧ�ܺ���
		pFile->ReadInitInfo(strIniFile,"db","failadd"+strFurtherName,m_strFailAddResultSql,iTemp,0);
		pFile->ReadInitInfo(strIniFile,"db","failaddindex"+strFurtherName,m_strFailAddResultIndexList,iTemp,0);
		//���ͳɹ����������Ϣ
		pFile->ReadInitInfo(strIniFile,"db","succlean"+strFurtherName,m_strSucCleanSql,iTemp,0);
		pFile->ReadInitInfo(strIniFile,"db","succleanindex"+strFurtherName,m_strSucCleanIndexList,iTemp,0);		
		//����ʧ�ܴ��������Ϣ
		pFile->ReadInitInfo(strIniFile,"db","failclean"+strFurtherName,m_strFailCleanSql,iTemp,0);
		pFile->ReadInitInfo(strIniFile,"db","failcleanindex"+strFurtherName,m_strFailCleanIndexList,iTemp,0);
		//״̬���洦��
		pFile->ReadInitInfo(strIniFile,"db","update"+strFurtherName,m_strUpdateSql,iTemp,0);			pFile->ReadInitInfo(strIniFile,"db","updateindex"+strFurtherName,m_strUpdateIndexList,iTemp,0);
		//������Ϣ����
		pFile->ReadInitInfo(strIniFile,"db","addmo"+strFurtherName,m_strAddMoSql,iTemp,0);		
		pFile->ReadInitInfo(strIniFile,"db","addmoindex"+strFurtherName,m_strAddMoIndexList,iTemp,0);

		m_strReadSql = pFile->StrDecode(m_strReadSql);	
		m_strReadIndexList = pFile->StrDecode(m_strReadIndexList);

		m_strSucAddResultSql = pFile->StrDecode(m_strSucAddResultSql);	
		m_strSucAddResultIndexList = pFile->StrDecode(m_strSucAddResultIndexList);

		m_strFailAddResultSql = pFile->StrDecode(m_strFailAddResultSql);	
		m_strFailAddResultIndexList = pFile->StrDecode(m_strFailAddResultIndexList);

		m_strSucCleanSql = pFile->StrDecode(m_strSucCleanSql);
		m_strSucCleanIndexList = pFile->StrDecode(m_strSucCleanIndexList);
		m_strFailCleanSql = pFile->StrDecode(m_strFailCleanSql);	
		m_strFailCleanIndexList = pFile->StrDecode(m_strFailCleanIndexList);

		m_strUpdateSql = pFile->StrDecode(m_strUpdateSql);	
		m_strUpdateIndexList = pFile->StrDecode(m_strUpdateIndexList);	

		m_strAddMoSql = pFile->StrDecode(m_strAddMoSql);	
		m_strAddMoIndexList = pFile->StrDecode(m_strAddMoIndexList);
	}
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void COtherSet::InitComString( CStringList* pComboList)
{
	for (POSITION Pos_ = m_cstringList.GetHeadPosition(); Pos_ != NULL;)
	{
		pComboList->AddTail(m_cstringList.GetNext(Pos_));
	}
	return;	
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void COtherSet::OnNMRclickListMapping(NMHDR *pNMHDR, LRESULT *pResult)
{
	CMenu obMenu;
	obMenu.LoadMenu(IDR_MENU2);
	CMenu* pPopupMenu = obMenu.GetSubMenu(0);
	ASSERT(pPopupMenu); 
	// Get the cursor position
	CPoint obCursorPoint = (0, 0);
	GetCursorPos(&obCursorPoint);
	if (0 >= m_List.GetSelectedCount())
	{
		pPopupMenu->EnableMenuItem(ID_LST_DELETEOWN, MF_BYCOMMAND | MF_GRAYED | MF_DISABLED); 
	}
	else
	{
		pPopupMenu->EnableMenuItem(ID_LST_DELETEOWN, MF_BYCOMMAND | MF_ENABLED);
	}
	// Track the popup menu
	pPopupMenu->TrackPopupMenu(TPM_LEFTALIGN|TPM_LEFTBUTTON|TPM_RIGHTBUTTON, obCursorPoint.x,obCursorPoint.y, this);
	*pResult = 0;
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
void COtherSet::OnNMDblclkListMapping(NMHDR *pNMHDR, LRESULT *pResult)
{
	*pResult = 0;
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
void COtherSet::InitKeyMap()
{
	m_keyMap.clear();
	m_cstringList.RemoveAll();
	switch ( m_iOptFlag )
	{
	case 0:		//��ȡ
	case 1:		//����
	case 2:		//����ȴ�������Ϣ
	{
			m_keyMap["����ID[int]"] = 8;//			
			m_keyMap["��Ϣ����"] = 19;//
			m_keyMap["���պ���[Ԥ����]"] = 16;//
			m_keyMap["���պ���"] = 15;//
			m_keyMap["���ͷ�����"] = 6;
			//m_keyMap["��������[int]"] = 1;	//
			m_keyMap["���Դ���"] = 5;		//
			m_keyMap["��Ϣ����"] = 6;		//
			m_keyMap["�������ȼ�"] = 7;		//	
			m_keyMap["�����ֶ�1[int]"] = 9;//
			m_keyMap["�����ֶ�2[int]"] = 10;//
			m_keyMap["�����ֶ�3[int]"] = 11;//
			m_keyMap["�����ֶ�4[int]"] = 12;//
			m_keyMap["�����ֶ�5[int]"] = 13;//
			m_keyMap["��������[string]"] = 14;
			m_keyMap["��Ϣ���ر�ʶ"] = 17;//
			m_keyMap["��ϢID"]	= 18;
			m_keyMap["��Ϣ����ʱ��"] = 20;//
			m_keyMap["��Ϣ�ύʱ��"] = 21;//
			m_keyMap["��Ϣ����ʱ��"] = 22;//
			m_keyMap["����ԱID"] = 23;//
			m_keyMap["����ID"] = 24;
			m_keyMap["������"] = 25;
			m_keyMap["����ID[string]"] = 26;
			m_keyMap["�ӷ�������"] = 27;
			m_keyMap["�Ӷ˿�"] = 28;
			m_keyMap["�û�ID"] = 29;
			m_keyMap["���ŷ���״̬"] = 30;
			m_keyMap["���Ž���״̬"] = 31;
			m_keyMap["���˿�"] = 32;
			m_keyMap["ʧ��ԭ��"] = 33;
			m_keyMap["�����ֶ�1[string]"] = 34;
			m_keyMap["�����ֶ�2[string]"] = 35;	
			m_keyMap["���պ�����"] = 3;

			m_cstringList.AddTail("����ID[int]");
			m_cstringList.AddTail("����ID[string]");//26;
			m_cstringList.AddTail("��Ϣ����");//19;//
			//m_cstringList.AddTail("���պ���[Ԥ����]");//16;//
			m_cstringList.AddTail("���պ���");//15;//
			m_cstringList.AddTail("���պ�����");//3;
			m_cstringList.AddTail("���ͷ�����");//6;			
			m_cstringList.AddTail("�������ȼ�");//7;	//
			//m_cstringList.AddTail("��������[int]");//1;	//
			m_cstringList.AddTail("��������[string]");//14;
			m_cstringList.AddTail("�ӷ�������");//27;
			m_cstringList.AddTail("��ϢID");// 18;
			m_cstringList.AddTail("��Ϣ���ر�ʶ");//17;//		
			m_cstringList.AddTail("��Ϣ����ʱ��");//20;//
			m_cstringList.AddTail("��Ϣ�ύʱ��");//21;//
			m_cstringList.AddTail("��Ϣ����ʱ��");//22;//
			m_cstringList.AddTail("����ԱID");//23;//
			m_cstringList.AddTail("����ID");//24;
			m_cstringList.AddTail("������");//25;						
			m_cstringList.AddTail("�û�ID");//29;
			m_cstringList.AddTail("���ŷ���״̬");//30;
			m_cstringList.AddTail("���Ž���״̬");//31;
			m_cstringList.AddTail("���Դ���");//5;		//
			m_cstringList.AddTail("��Ϣ����");//6;		//
			m_cstringList.AddTail("�Ӷ˿�");//28;
			m_cstringList.AddTail("���˿�");//32;
			m_cstringList.AddTail("ʧ��ԭ��");//33;
			m_cstringList.AddTail("�����ֶ�1[int]");//9;//
			m_cstringList.AddTail("�����ֶ�2[int]");//10;//
			m_cstringList.AddTail("�����ֶ�3[int]");//11;//
			m_cstringList.AddTail("�����ֶ�4[int]");//12;//
			m_cstringList.AddTail("�����ֶ�5[int]");//13;//
			m_cstringList.AddTail("�����ֶ�1[string]");//34;
			m_cstringList.AddTail("�����ֶ�2[string]");//35;			
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
			//m_cstringList.AddTail("��������");
			m_cstringList.AddTail("����ʱ��");
			m_cstringList.AddTail("�ֻ�����");
			m_cstringList.AddTail("ͨ������");
			//m_cstringList.AddTail("���ͷ�����");
			m_cstringList.AddTail("��Ϣ����");
			m_cstringList.AddTail("�����ֶ�");
			break;
		}
	case 4:		//״̬����
		{
			m_keyMap["��������"] = 1;
			m_keyMap["����ʱ��"] = 2;
			m_keyMap["��Ϣ���"] = 3;
			m_keyMap["��Ϣ״̬"] = 5;
			m_keyMap["��Ϣ��ϸ����"] = 6;
			m_keyMap["�����ֶ�"] = 4;
			//m_cstringList.AddTail("��������");
			m_cstringList.AddTail("����ʱ��");
			m_cstringList.AddTail("��Ϣ���");
			m_cstringList.AddTail("��Ϣ״̬");
			m_cstringList.AddTail("��Ϣ��ϸ����");
			m_cstringList.AddTail("�����ֶ�");
			break;
		}
	default:
		{
			m_keyMap.clear();
			break;
		}
	}
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
void COtherSet::InitListCtrl()
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
	m_List.SetComboColumns(1);
	m_List.SetReadOnlyColumns(1);
	//m_List.SetExtendedStyle(LVS_EX_FULLROWSELECT);
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void COtherSet::InitSqlCombo()
{
	m_ComboSql.AddString("��ȡ������Ϣ");
	m_ComboSql.AddString("���淢�ͽ��");
	//m_ComboSql.AddString("���淢�ͽ��[ʧ��]");
	m_ComboSql.AddString("���������Ϣ");
	//m_ComboSql.AddString("����[ʧ��]���������Ϣ");
	m_ComboSql.AddString("����������Ϣ");
	m_ComboSql.AddString("���·��ͽ��");
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
LRESULT COtherSet::UpdateValue(WPARAM wParam, LPARAM lParam)
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
CString COtherSet::IntToCString( int iIndex )
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
CString COtherSet::CStringToIntCString( CString strKey )
{
	CString strRet = "";
	int iRet = m_keyMap[strKey];
	strRet.Format("%d",iRet);
	return strRet;
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
LRESULT COtherSet::PopulateComboList(WPARAM wParam, LPARAM lParam)
{
	// Get the Combobox window pointer
	CComboBox* pInPlaceCombo = static_cast<CComboBox*> (GetFocus());
	//ȥ����������
	// 	DWORD nStyle   =   GetWindowLong(pInPlaceCombo->m_hWnd,GWL_STYLE);   
	// 	nStyle   =   nStyle &(~ LBS_SORT);   
	// 	SetWindowLong(pInPlaceCombo->m_hWnd,GWL_STYLE,nStyle);   

	CRect obWindowRect;
	pInPlaceCombo->GetWindowRect(&obWindowRect);
	//pInPlaceCombo->MoveWindow(obWindowRect.left,obWindowRect.top,obWindowRect.Width(),500);
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
void COtherSet::OnMenuInsertown()
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
void COtherSet::OnLstDeleteown()
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
void COtherSet::SaveMapping(CString &strList)
{
	strList.Empty();
	CString strTemp = "";
	int iCount = m_List.GetItemCount();
	for ( int i=0;i<iCount;i++)
	{
		strTemp = m_List.GetItemText(i,0);
		strTemp.Trim();
		if ( strTemp != "" )
		{
			strList += strTemp+",";
			strTemp = CStringToIntCString(m_List.GetItemText(i,1));
			strList +=strTemp+";";
		}		
	}
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void COtherSet::InitMapping( CString strMap )
{
	//strMap += ";";
	CString strTemp = "";
	int iPos = 0 ;
	int idx = 0;
	m_List.DeleteAllItems();
	int iCount = 0;
	while( true )
	{
		try
		{
			iPos = strMap.Find( ';' );
			if ( iPos != -1 )
			{
				strTemp = strMap.Left( iPos );
				idx = strTemp.Find(',');
				iCount = m_List.GetItemCount();
				m_List.InsertItem(iCount,strTemp.Left(idx));
				m_List.SetItemText( iCount,1,IntToCString(atoi(strTemp.Mid(idx+1))) );
				strMap = strMap.Mid(iPos+1);
			}
			else
			{
				break;
			}
		}
		catch (...)
		{
		}		
	}
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void COtherSet::InitSqlString()
{
	switch( m_iOptFlag )
	{	
		case 0:		//��ȡ
			{
				SetDlgItemText(IDC_EDIT_SQL,m_strReadSql);
				InitMapping(m_strReadIndexList);
				break;
			}
		case 1: //����ɹ����
			{
				SetDlgItemText(IDC_EDIT_SQL,m_strSucAddResultSql);
				InitMapping(m_strSucAddResultIndexList);
				break;
			}
// 		case 2://����ʧ�ܽ��
// 			{
// 				SetDlgItemText(IDC_EDIT_SQL,m_strFailAddResultSql);
// 				InitMapping(m_strFailAddResultIndexList);
// 				break;
// 			}
		case 2://�ɹ�����ȴ�����
			{
				SetDlgItemText(IDC_EDIT_SQL,m_strSucCleanSql);
				InitMapping(m_strSucCleanIndexList);
				break;
			}
// 		case 4:		//ʧ�ܴ���
// 			{
// 				SetDlgItemText(IDC_EDIT_SQL,m_strFailCleanSql);
// 				InitMapping(m_strFailCleanIndexList);
// 				break;
// 			}
		case 3:	//������Ϣ
			{
				SetDlgItemText(IDC_EDIT_SQL,m_strAddMoSql);
				InitMapping(m_strAddMoIndexList);
				break;
			}
		case 4://״̬����
			{
				SetDlgItemText(IDC_EDIT_SQL,m_strUpdateSql);
				InitMapping(m_strUpdateIndexList);
				break;
			}
	}
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void COtherSet::OnCbnSelchangeComboOpttype()
{
	switch(m_iOptFlag)
	{	
	case 0: //��ȡ
		{
			GetDlgItemText(IDC_EDIT_SQL,m_strReadSql);
			SaveMapping(m_strReadIndexList);
			break;
		}
	case 1: //����ɹ����
		{
			GetDlgItemText(IDC_EDIT_SQL,m_strSucAddResultSql);
			SaveMapping(m_strSucAddResultIndexList);
			break;
		}
// 	case 2: //����ʧ�ܽ��
// 		{
// 			GetDlgItemText(IDC_EDIT_SQL,m_strFailAddResultSql);
// 			SaveMapping(m_strFailAddResultIndexList);
// 			break;
// 		}
	case 2: //�ɹ�����ȴ�����
		{
			GetDlgItemText(IDC_EDIT_SQL,m_strSucCleanSql);
			SaveMapping(m_strSucCleanIndexList);
			break;
		}
// 	case 4:	//ʧ�ܴ���
// 		{
// 			GetDlgItemText(IDC_EDIT_SQL,m_strFailCleanSql);
// 			SaveMapping(m_strFailCleanIndexList);
// 			break;
// 		}
	case 3:	//������Ϣ
		{
			GetDlgItemText(IDC_EDIT_SQL,m_strAddMoSql);
			SaveMapping(m_strAddMoIndexList);
			break;
		}
	case 4://״̬����
		{
			GetDlgItemText(IDC_EDIT_SQL,m_strUpdateSql);
			SaveMapping(m_strUpdateIndexList);
			break;
		}
	}
	m_iOptFlag = m_ComboSql.GetCurSel();
	InitKeyMap();
	InitSqlString();	
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
void COtherSet::OnBnClickedBtnAddField()
{
	CString strSql = "";
	GetDlgItemText(IDC_EDIT_SQL,strSql);
	int idx1 = 0;
	int idx2 = 0;
	CString strTemp = "";
	int iIndex = 0;
	m_List.DeleteAllItems();
	while(true)
	{
		idx1 = strSql.Find('{');
		idx2 = strSql.Find('}');
		if ( idx1 != -1 && idx2 != -1)
		{
			strTemp = strSql.Mid(idx1+1,idx2-idx1-1);
			strSql = strSql.Mid(idx2 + 1);
			int iCount = m_List.GetItemCount();
			m_List.InsertItem(iCount,strTemp);
		}
		else
		{
			break;
		}
	}
}

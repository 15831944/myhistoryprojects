#include "stdafx.h"
#include "../DbGate.h"
#include "StroeSet.h"
#include ".\stroeset.h"
#include "../ControlFile/UserFile.h"
//�����Զ������ݿ�����ĶԻ���
IMPLEMENT_DYNAMIC(CStroeSet, CDialog)
CStroeSet::CStroeSet(CWnd* pParent /*=NULL*/)
: CDialog(CStroeSet::IDD, pParent)
{
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
CStroeSet::~CStroeSet()
{
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CStroeSet::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_OPTTYPE, m_ComboSql);
	DDX_Control(pDX, IDC_LIST2, m_List);
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
BEGIN_MESSAGE_MAP(CStroeSet, CDialog)
	ON_BN_CLICKED(IDC_BTN_SAVE, OnBnClickedBtnSave)
	ON_NOTIFY(NM_RCLICK, IDC_LIST2, OnNMRclickListMapping)
	ON_MESSAGE(WM_SET_ITEMS, PopulateComboList)
	ON_MESSAGE(WM_VALIDATE,UpdateValue)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST2, OnNMDblclkListMapping)
	ON_COMMAND(ID_MENU_INSERTOWN, OnMenuInsertown)
	ON_COMMAND(ID_LST_DELETEOWN, OnLstDeleteown)
	ON_CBN_SELCHANGE(IDC_COMBO_OPTTYPE, OnCbnSelchangeComboOpttype)
END_MESSAGE_MAP()

/************************************************************************/
/*                                                                      */
/************************************************************************/
BOOL CStroeSet::OnInitDialog()
{
	CDialog::OnInitDialog();
	m_iOptFlag = 0;		//��ȡ
	InitKeyMap();
	InitParams();
	InitListCtrl();		//��ʼ������
	InitSqlCombo();
	GetDlgItem(IDC_COMBO_OPTTYPE)->MoveWindow(80,15,150,300);
	m_iOptFlag = 0;
	m_ComboSql.SetCurSel(0);
	InitSqlString();
	return TRUE;
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CStroeSet::OnBnClickedBtnSave()
{
	switch( m_iOptFlag )
	{	
	case 0:		//��ȡ
		{
			GetDlgItemText(IDC_EDIT_SQL,m_strRead);
			SaveMapping(m_strReadIndexList);
			break;
		}
	case 1: //������
		{
			GetDlgItemText(IDC_EDIT_SQL,m_strAddResult);
			SaveMapping(m_strAddResultIndexList);
			break;
		}
	case 2:	//������Ϣ
		{
			GetDlgItemText(IDC_EDIT_SQL,m_strAddMo);
			SaveMapping(m_strAddMoIndexList);
			break;
		}
	case 3://״̬����
		{
			GetDlgItemText(IDC_EDIT_SQL,m_strUpdate);
			SaveMapping(m_strUpdateIndexList);
			break;
		}
	}
	CString strTemp = "";
	int iTemp = 0;
	CString strFurtherName = "store";
	CUserFile *pFile = NULL;
	pFile = CUserFile::GetInstance();
	if (pFile)
	{
		m_strRead = pFile->StrEncode(m_strRead);	
		m_strReadIndexList = pFile->StrEncode(m_strReadIndexList);

		m_strAddResult = pFile->StrEncode(m_strAddResult);	
		m_strAddResultIndexList = pFile->StrEncode(m_strAddResultIndexList);

		m_strUpdate = pFile->StrEncode(m_strUpdate);	
		m_strUpdateIndexList = pFile->StrEncode(m_strUpdateIndexList);	

		m_strAddMo = pFile->StrEncode(m_strAddMo);	
		m_strAddMoIndexList = pFile->StrEncode(m_strAddMoIndexList);

		int iRet = -1;
		CString strAppPath = "";
		pFile->GetAppPath(strAppPath);
		CString strIniFile = strAppPath + DBSETINITFILE;
		//��ȡ
		pFile->WriteInitInfo(strIniFile,"db","read"+strFurtherName,m_strRead,iTemp,0);
		pFile->WriteInitInfo(strIniFile,"db","readindex"+strFurtherName,m_strReadIndexList,iTemp,0);
		//���淢�ͽ��
		pFile->WriteInitInfo(strIniFile,"db","add"+strFurtherName,m_strAddResult,iTemp,0);
		pFile->WriteInitInfo(strIniFile,"db","addindex"+strFurtherName,m_strAddResultIndexList,iTemp,0);
		//״̬���洦��
		pFile->WriteInitInfo(strIniFile,"db","update"+strFurtherName,m_strUpdate,iTemp,0);		
		pFile->WriteInitInfo(strIniFile,"db","updateindex"+strFurtherName,m_strUpdateIndexList,iTemp,0);
		//������Ϣ����
		pFile->WriteInitInfo(strIniFile,"db","addmo"+strFurtherName,m_strAddMo,iTemp,0);		
		pFile->WriteInitInfo(strIniFile,"db","addmoindex"+strFurtherName,m_strAddMoIndexList,iTemp,0);
		//���ݿ������������
		pFile->WriteInitInfo(strIniFile,"opttype","type",strTemp,3,1);
	}
	::PostMessage(this->GetParent()->GetParent()->m_hWnd,WM_CLOSE,0,0);
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CStroeSet::InitParams()
{	
	CString strTemp = "";
	int iTemp = 0;
	CUserFile *pFile = NULL;
	pFile = CUserFile::GetInstance();
	if (pFile)
	{	
		int iRet = -1;
		CString strAppPath = "";
		CString strFurtherName = "store";
		pFile->GetAppPath(strAppPath);
		CString strIniFile = strAppPath + DBSETINITFILE;
		//��ȡ
		pFile->ReadInitInfo(strIniFile,"db","read"+strFurtherName,m_strRead,iTemp,0);
		pFile->ReadInitInfo(strIniFile,"db","readindex"+strFurtherName,m_strReadIndexList,iTemp,0);
		//���ͳɹ��󱣴�
		pFile->ReadInitInfo(strIniFile,"db","add"+strFurtherName,m_strAddResult,iTemp,0);
		pFile->ReadInitInfo(strIniFile,"db","addindex"+strFurtherName,m_strAddResultIndexList,iTemp,0);
		
		//״̬���洦��
		pFile->ReadInitInfo(strIniFile,"db","update"+strFurtherName,m_strUpdate,iTemp,0);		
		pFile->ReadInitInfo(strIniFile,"db","updateindex"+strFurtherName,m_strUpdateIndexList,iTemp,0);
		//������Ϣ����
		pFile->ReadInitInfo(strIniFile,"db","addmo"+strFurtherName,m_strAddMo,iTemp,0);		
		pFile->ReadInitInfo(strIniFile,"db","addmoindex"+strFurtherName,m_strAddMoIndexList,iTemp,0);
		
		m_strRead = pFile->StrDecode(m_strRead);
		m_strReadIndexList = pFile->StrDecode(m_strReadIndexList);

		m_strAddResult = pFile->StrDecode(m_strAddResult);	
		m_strAddResultIndexList = pFile->StrDecode(m_strAddResultIndexList);

		m_strUpdate = pFile->StrDecode(m_strUpdate);	
		m_strUpdateIndexList = pFile->StrDecode(m_strUpdateIndexList);	

		m_strAddMo = pFile->StrDecode(m_strAddMo);	
		m_strAddMoIndexList = pFile->StrDecode(m_strAddMoIndexList);
	}
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CStroeSet::InitComString( CStringList* pComboList)
{
	for (POSITION Pos_ = m_cstringList.GetHeadPosition(); Pos_ != NULL;)
	{
		pComboList->AddTail(m_cstringList.GetNext(Pos_));
	}
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CStroeSet::OnNMRclickListMapping(NMHDR *pNMHDR, LRESULT *pResult)
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
void CStroeSet::OnNMDblclkListMapping(NMHDR *pNMHDR, LRESULT *pResult)
{
	*pResult = 0;
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
void CStroeSet::InitKeyMap()
{
	m_keyMap.clear();
	m_cstringList.RemoveAll();
	switch ( m_iOptFlag )
	{
	case 0:	//��ȡ
	case 1:	//���ͽ��
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
	case 2:		//����
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
			m_cstringList.AddTail("���ͷ�����");
			m_cstringList.AddTail("��Ϣ����");
			m_cstringList.AddTail("�����ֶ�");
			break;
		}
	case 3:		//״̬����
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
void CStroeSet::InitListCtrl()
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
void CStroeSet::InitSqlCombo()
{
	m_ComboSql.AddString("��ȡ������Ϣ");
	m_ComboSql.AddString("���淢�ͽ��");
	m_ComboSql.AddString("����������Ϣ");
	m_ComboSql.AddString("���·��ͽ��");
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
LRESULT CStroeSet::UpdateValue(WPARAM wParam, LPARAM lParam)
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
CString CStroeSet::IntToCString( int iIndex )
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
CString CStroeSet::CStringToIntCString( CString strKey )
{
	CString strRet = "";
	int iRet = m_keyMap[strKey];
	strRet.Format("%d",iRet);
	return strRet;
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
LRESULT CStroeSet::PopulateComboList(WPARAM wParam, LPARAM lParam)
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
void CStroeSet::OnMenuInsertown()
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
void CStroeSet::OnLstDeleteown()
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
void CStroeSet::SaveMapping(CString &strList)
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
void CStroeSet::InitMapping( CString strMap )
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
void CStroeSet::InitSqlString()
{
	switch( m_iOptFlag )
	{	
	case 0:		//��ȡ
		{
			SetDlgItemText(IDC_EDIT_SQL,m_strRead);
			InitMapping(m_strReadIndexList);
			break;
		}
	case 1: //������
		{
			SetDlgItemText(IDC_EDIT_SQL,m_strAddResult);
			InitMapping(m_strAddResultIndexList);
			break;
		}	
	case 2:	//������Ϣ
		{
			SetDlgItemText(IDC_EDIT_SQL,m_strAddMo);
			InitMapping(m_strAddMoIndexList);
			break;
		}
	case 3://״̬����
		{
			SetDlgItemText(IDC_EDIT_SQL,m_strUpdate);
			InitMapping(m_strUpdateIndexList);
			break;
		}
	}
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CStroeSet::OnCbnSelchangeComboOpttype()
{
	switch( m_iOptFlag )
	{	
	case 0: //��ȡ
		{
			GetDlgItemText(IDC_EDIT_SQL,m_strRead);
			SaveMapping(m_strReadIndexList);
			break;
		}
	case 1: //����ɹ����
		{
			GetDlgItemText(IDC_EDIT_SQL,m_strAddResult);
			SaveMapping(m_strAddResultIndexList);
			break;
		}	
	case 2:	//������Ϣ
		{
			GetDlgItemText(IDC_EDIT_SQL,m_strAddMo);
			SaveMapping(m_strAddMoIndexList);
			break;
		}
	case 3://״̬����
		{
			GetDlgItemText(IDC_EDIT_SQL,m_strUpdate);
			SaveMapping(m_strUpdateIndexList);
			break;
		}
	}
	m_iOptFlag = m_ComboSql.GetCurSel();
	InitKeyMap();
	InitSqlString();	
}



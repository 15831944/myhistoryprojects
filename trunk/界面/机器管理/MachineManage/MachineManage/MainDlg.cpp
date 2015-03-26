#include "StdAfx.h"
#include "MainDlg.h"
#include <windows.h>
#if !defined(UNDER_CE)
#include <shellapi.h>
#endif
#include "..\DuiLibWrapper\ListEx.h"

#include "AppListControl.h"
CMainDlg::CMainDlg(void)
{
}


CMainDlg::~CMainDlg(void)
{
}


void CMainDlg::OnClick(TNotifyUI& msg)
{
	CControlUI* pControl = msg.pSender;
	if(pControl->IsEqual("removeitem"))
	{
		m_appListUI->Remove(pControl->GetParent()->GetParent());
		int nnn = 0;
		return;
	}
	else if(pControl->IsEqual("iconshow"))
	{
		//Ӧ�ó���ĸ�ѡ������������ѡ�� 
		CCheckBoxUI* pCheck = (CCheckBoxUI*)m_PaintManager.FindSubControlByName(pControl->GetParent(), "isuse");
		if (pCheck)
		{
			pCheck->SetCheck(!pCheck->GetCheck());
		}
		return;
	}
	else if (pControl->IsEqual("savetreenode"))
	{
		//����༭�ڵ����ƻ����Ҷ�ӽڵ�
		EndEditOrAddTreeNode();
		return;
	}
	else if(pControl->IsEqual("beginadd") || pControl->IsEqual("beginedit"))
	{
		//��ʾ�༭�ڵ������� 
		BeginEditOrAddTreeNode(pControl);
		return;
	}
	else if (pControl->IsEqual("adddirectory"))
	{
		//���Ŀ¼
		CDuiString strDirectryName = GetDlgItemText("editdirectory");
		CTreeViewUI* pTree = static_cast<CTreeViewUI*>(m_PaintManager.FindControl(_T("tree")));
		
		CTreeNodeUI* pNodeAdd = new CTreeNodeUI;
		pNodeAdd->SetAttribute(_T("folderattr"),_T("padding=\"0,1,0,0\" width=\"36\" height=\"16\" normalimage=\"file='treeview_b.png' source='0,0,36,16'\" hotimage=\"file='treeview_b.png' source='36,0,72,16'\" selectedimage=\"file='treeview_a.png' source='0,0,36,16' selectedhotimage=\"file='treeview_a.png' source='36,0,72,16'\""));
		pNodeAdd->SetItemText(strDirectryName);
		pNodeAdd->SetExpand(true);
		m_pNodeRoot->AddChildNode(pNodeAdd);
		
	}
	else if (pControl->IsEqual("selall"))
	{
		int nCount = m_userList->GetCount();
		for (int i = nCount - 1; i >=0 ; i--)
		{
			CControlUI *pLine = m_userList->GetItemAt(i);
			CCheckBoxUI* pCheckLine = (CCheckBoxUI*)m_PaintManager.FindSubControlByName(pLine, "selme");
			if (pCheckLine)
			{
				//�˴��Ƚ���֣�Ҫȡȫѡ�ķ�������ȷ��ֵ 
				pCheckLine->SetCheck(!((CCheckBoxUI*)pControl)->GetCheck());
			}
		}
	}
	else if (pControl->IsEqual("deletesel"))
	{
		//�����б����ɾ�� 
		int nCount = m_userList->GetCount();
		for (int i = nCount - 1; i >=0 ; i--)
		{
			CControlUI *pLine = m_userList->GetItemAt(i);
			CCheckBoxUI* pCheckLine = (CCheckBoxUI*)m_PaintManager.FindSubControlByName(pLine, "selme");
			if (pCheckLine && pCheckLine->GetCheck())
			{
				m_userList->RemoveAt(i);
			}
		}
	}
	else if (pControl->IsEqual("edititem"))
	{
		StartEditItem(pControl);
		return;
	}
	else if (pControl->IsEqual("deleteitem") || pControl->IsEqual("canceladd"))
	{
		CListContainerElementUI *pElement = (CListContainerElementUI *)GetLineContainer(pControl);
		if (pElement->GetUserData() == "fromedititem")
		{
			//���ڱ༭ĳһ����ȡ������ԭ 
			SaveItemData(pControl, true);
			return;
		}
		int nIndex = m_userList->GetItemIndex(pElement);		
		m_userList->RemoveAt(nIndex);
		return;
	}
	else if (pControl->IsEqual("additem"))
	{
		StartAddUserInfo(pControl);
		return;
	}
	else if (pControl->IsEqual("saveitem"))
	{
		SaveItemData(pControl);
		return;
	}
	else if (pControl->IsEqual("hostmachine"))
	{
		::MessageBox(m_hWnd, "������ť���������Ҫ����������", "��ʾ", 0);
		return;
	}
	else if (pControl->IsEqual("database"))
	{
		::MessageBox(m_hWnd, "���ݿⰴť���������Ҫ����������", "��ʾ", 0);
		return;
	}
	else if (pControl->IsEqual("netmachine"))
	{
		::MessageBox(m_hWnd, "�����豸��ť���������Ҫ����������", "��ʾ", 0);
		return;
	}
	__super::OnClick(msg);
}

CControlUI* CMainDlg::GetTreeNode(CControlUI* pControl)
{
	CControlUI *pNode = NULL;
	while (pControl != NULL && pNode == NULL)
	{
		pNode = (CControlUI *)pControl->GetInterface(DUI_CTR_TREENODE);
		if (pNode != NULL)
		{
			break;
		}
		pControl = pControl->GetParent();
	}
	return pNode;
}

void CMainDlg::Notify(TNotifyUI& msg)
{
	if (_tcsicmp(msg.sType, _T("TreeNodeMove")) == 0 )
	{
		
		CTreeNodeUI* oldParent = (CTreeNodeUI*)msg.wParam;
		CTreeNodeUI* pNodeMoved =  (CTreeNodeUI*)msg.lParam;
		//ͨ��pNodeMoved���Ի�ȡ���µĸ��ڵ� 
		CTreeNodeUI* pNewParent = pNodeMoved->GetParentNode();
		//�˴�����pNodeMoved�Ƿ���Ҷ�ӽڵ㣬������δ������ݵĴ洢���� 
		if (pNodeMoved->IsLeaf())
		{
			CDuiString strShow = "";
			strShow.Format("Ҷ�ӽڵ� %s �� %s �ƶ����� %s ��", pNodeMoved->GetItemText().GetData(), oldParent->GetItemText().GetData(), pNewParent->GetItemText().GetData());
			::MessageBox(NULL, strShow, "��ʾ", 0);
		}
		
		//���ڵ��ƶ���� 
		return;
	}
	//�ڴ˴���Ӧ�б��˫���������˴����ڵ��˫����LIST�ĵ���������� 
	if (_tcsicmp(msg.sType, _T("itemactivate")) == 0 )
	{
		CControlUI* pNode = GetTreeNode(msg.pSender);
		if (pNode != NULL)
		{
			CTreeNodeUI* pTreeNode = (CTreeNodeUI*)pNode;
			//�ڴ˴��������ڵ��˫��
			if (!pTreeNode->IsLeaf())	//�������Ҷ�ӽڵ㣬����Ҫ�����Ҳ���б���ʾ  
			{
				::MessageBox(NULL, pTreeNode->GetItemText().GetData(), "��ʾ", 0);
			}
			return;
		}
		CControlUI* pEditContrl = m_PaintManager.FindSubControlByName(msg.pSender, "edituserid");
		if (pEditContrl)
		{
			return;
		}
		CControlUI* pLabel = m_PaintManager.FindSubControlByName(msg.pSender, "userid");
		if (!pLabel)
		{
			return;
		}
		CDuiString struserid = m_PaintManager.FindSubControlByName(msg.pSender, "userid")->GetText();
		CDuiString strpwd = m_PaintManager.FindSubControlByName(msg.pSender, "pwd")->GetText();
		CDuiString strprotocol = m_PaintManager.FindSubControlByName(msg.pSender, "protocol")->GetText();
		CDuiString strtype = m_PaintManager.FindSubControlByName(msg.pSender, "type")->GetText();
		CDuiString strShow = "";
			
		CDuiString strSelectedApp = " ";
		int nAppCount = m_appListUI->GetCount();
		for (int i=0; i< nAppCount; i++)
		{
		    CCheckBoxUI* pCheck = (CCheckBoxUI*)m_PaintManager.FindSubControlByName(m_appListUI->GetItemAt(i), "isuse");
			if (pCheck->GetCheck())
			{
				strSelectedApp += pCheck->GetText() + "\r\n ";
			}			
		}
		strShow.Format("ѡ�е���Ϣ���£�\r\n�˺ţ�%s\r\n���룺%s\r\nЭ�飺%s\r\n���ͣ�%s\r\nѡ�е�Ӧ�ó���\r\n%s", 
			struserid.GetData(), strpwd.GetData(), strprotocol.GetData(), strtype.GetData(), strSelectedApp.GetData());
		MessageBox(m_hWnd, strShow, "��ʾ", 0);	
	}
	
	__super::Notify(msg);
}
void CMainDlg::OnPrepare(TNotifyUI& msg)
{
	__super::OnPrepare(msg);

	::DragAcceptFiles(this->m_hWnd, true);  

	//����������ݣ���ӵ��б��� 
	for (int i = 0; i < 8; i++)
	{
		UserInfo info;
		info.strip.Format("192.168.2.11%d", i);
		info.struserid.Format("userid%d", i);
		info.strpwd = "123";
		info.strprotocol = i%2==0 ? "HTTP" : "TCP";
		info.strtype = i%3==0 ? "��ҵӦ��" : "���˲�Ʒ";
		m_vecUserList.push_back(info);
	}
	m_userList = static_cast<CListUIEx*>(m_PaintManager.FindControl(_T("listex")));
		
	for (int i = 0; i < m_vecUserList.size(); ++i)
	{
		CDialogBuilder builder;
		CListContainerElementUI* pLine = (CListContainerElementUI*)(builder.Create(_T("single_list_item.xml"), (UINT)0, this));

		if( pLine != NULL ) 
		{   
			SetListItemData(pLine, m_vecUserList[i].struserid, m_vecUserList[i].strpwd, m_vecUserList[i].strprotocol, m_vecUserList[i].strtype);
			m_userList->InsertItem(m_userList->GetCount(), 21, pLine);
		}
	}
	

	//���Դ��룬����������ͼ�����������У���ʾ����
	char szPath[MAX_PATH] = {0};
	GetModuleFileName(NULL, szPath, MAX_PATH);
	m_appListUI = (AppListUI*)m_PaintManager.FindControl(_T("applist"));
	m_appListUI->AddApplication(szPath);

	//��ʼ��������ʾ 
	CTreeViewUI* pTree = static_cast<CTreeViewUI*>(m_PaintManager.FindControl(_T("tree")));
	CControlUI* pControl = m_PaintManager.FindControl(_T("drag"));
	pTree->m_pDragingCtrl = pControl;

	CTreeNodeUI* pNodeRoot = new CTreeNodeUI;
	pNodeRoot->SetAttribute(_T("folderattr"),_T("padding=\"0,1,0,0\" width=\"36\" height=\"16\" normalimage=\"file='treeview_b.png' source='0,0,36,16'\" hotimage=\"file='treeview_b.png' source='36,0,72,16'\" selectedimage=\"file='treeview_a.png' source='0,0,36,16' selectedhotimage=\"file='treeview_a.png' source='36,0,72,16'\""));
	pTree->Add(pNodeRoot);
	CDuiString strNode = "";
	strNode.Format("�����б�");
	pNodeRoot->SetItemText(strNode);
	pNodeRoot->SetExpand(true);
	m_pNodeRoot = pNodeRoot;
	for (int i = 1; i < 7; i++)
	{
		int nn = pNodeRoot->GetCountChild();
		CTreeNodeUI* pNodeParent = new CTreeNodeUI;
		pNodeRoot->AddChildNode(pNodeParent);
		pNodeParent->SetName(strNode);
		pNodeParent->SetAttribute(_T("folderattr"),_T("padding=\"0,1,0,0\" width=\"36\" height=\"16\" normalimage=\"file='treeview_b.png' source='0,0,36,16'\" hotimage=\"file='treeview_b.png' source='36,0,72,16'\" selectedimage=\"file='treeview_a.png' source='0,0,36,16' selectedhotimage=\"file='treeview_a.png' source='36,0,72,16'\""));
		CDuiString strNode = "";
		strNode.Format("Ŀ¼%d", i);
		pNodeParent->SetItemText(strNode);
		pNodeParent->SetName(strNode);
		if (i % 4 == 0)
		{
			pNodeParent->SetExpand(true);
		}
		for (int j = 1; j< 7; j++)
		{
			CTreeNodeUI* pNodeChild = new CTreeNodeUI(true);	//��ʾ��Ҷ�ӽڵ� 
			CDuiString strNode = "";
			strNode.Format("%d_%d", i, j);
			pNodeChild->SetItemText(strNode);
			pNodeChild->SetName(strNode);
			pNodeParent->AddChildNode(pNodeChild);
		}
	}
}
CControlUI* CMainDlg::CreateControl(LPCTSTR pstrClass)
{
	if( _tcscmp(pstrClass, _T("AppList")) == 0 ) 
		return new AppListUI;
	return NULL;
}

void CMainDlg::SetListItemData( CControlUI* pLine, CDuiString struserid, CDuiString strpwd, CDuiString strprotocol, CDuiString strtype) 
{
	m_PaintManager.FindSubControlByName(pLine, "userid")->SetText(struserid);;
	m_PaintManager.FindSubControlByName(pLine, "pwd")->SetText(strpwd);;
	m_PaintManager.FindSubControlByName(pLine, "protocol")->SetText(strprotocol);;
	m_PaintManager.FindSubControlByName(pLine, "type")->SetText(strtype);
}

int CMainDlg::GetComboItemIndexFromString(CDuiString strItemText)
{
	if (strItemText == "HTTP")
	{
		return 0;
	}
	if (strItemText == "TCP")
	{
		return 1;
	}
	if (strItemText == "��ҵӦ��")
	{
		return 0;
	}
	if (strItemText == "���˲�Ʒ")
	{
		return 1;
	}
}

void CMainDlg::SetEditListItemData( CControlUI* pLine, CDuiString struserid, CDuiString strpwd, CDuiString strprotocol, CDuiString strtype) 
{
	CControlUI * pControl = m_PaintManager.FindSubControlByName(pLine, "edituserid");
	pControl->SetText(struserid);
	pControl->SetUserData(struserid);

	pControl = m_PaintManager.FindSubControlByName(pLine, "editpwd");
	pControl->SetText(strpwd);
	pControl->SetUserData(strpwd);

    CComboUI* pCombo = (CComboUI*)m_PaintManager.FindSubControlByName(pLine, "editprotocol");
	pCombo->SelectItem(GetComboItemIndexFromString(strprotocol));
	pCombo->SetUserData(strprotocol);

	pCombo = (CComboUI*)m_PaintManager.FindSubControlByName(pLine, "edittype");
	pCombo->SelectItem(GetComboItemIndexFromString(strtype));
	pCombo->SetUserData(strtype);
}


LRESULT CMainDlg::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{	
	switch(uMsg)
	{
		
		case WM_DROPFILES:
			OnDropFile((HDROP)wParam);
			break;

	}
	return __super::HandleMessage(uMsg, wParam, lParam);
}

LRESULT CMainDlg::OnDropFile(HDROP hDropInfo)
{
	TCHAR szFileName[MAX_PATH];  
	DragQueryFile(hDropInfo, 0, szFileName, MAX_PATH);  
	DragFinish(hDropInfo);  
	m_appListUI->AddApplication(szFileName);
	return 0L;
}

void CMainDlg::StartAddUserInfo( CControlUI* pControl )
{
	int i = m_userList->GetCount();
	CDialogBuilder builder;
	CListContainerElementUI* pLine = (CListContainerElementUI*)(builder.Create(_T("single_list_item_add.xml"),(UINT)0, this));
	if( pLine != NULL ) 
	{
		m_userList->InsertItem(i, 23, pLine);
	}
}

CControlUI* CMainDlg::GetLineContainer(CControlUI* pControl)
{
	CControlUI *pLineUI = NULL;
	while (pControl != NULL && pLineUI == NULL)
	{
		pLineUI = (CControlUI *)pControl->GetInterface(DUI_CTR_LISTCONTAINERELEMENT);
		if (pLineUI != NULL)
		{
			break;
		}
		pControl = pControl->GetParent();
	}
	return pLineUI;
}

void CMainDlg::SaveItemData( CControlUI* pControl, bool bCancelEdit )
{
	CControlUI *pParentCtrl = GetLineContainer(pControl);
	if (pParentCtrl == NULL)
	{
		return;
	}
	
	CDuiString struserid = "";
	CDuiString strpwd = "";
	CDuiString strprotocol = "";
	CDuiString strtype = "";
	if (bCancelEdit)
	{
		struserid = m_PaintManager.FindSubControlByName(pParentCtrl, "edituserid")->GetUserData();;
		strpwd = m_PaintManager.FindSubControlByName(pParentCtrl, "editpwd")->GetUserData();;
		strprotocol = m_PaintManager.FindSubControlByName(pParentCtrl, "editprotocol")->GetUserData();;
		strtype = m_PaintManager.FindSubControlByName(pParentCtrl, "edittype")->GetUserData();
	}
	else
	{
		//�˴���ȡҪ��ӵ���Ϣ
		struserid = m_PaintManager.FindSubControlByName(pParentCtrl, "edituserid")->GetText();;
		strpwd = m_PaintManager.FindSubControlByName(pParentCtrl, "editpwd")->GetText();;
		strprotocol = m_PaintManager.FindSubControlByName(pParentCtrl, "editprotocol")->GetText();;
		strtype = m_PaintManager.FindSubControlByName(pParentCtrl, "edittype")->GetText();
		if (struserid == "" || strprotocol == "" || strpwd == "" || strtype == "")
		{
			::MessageBox(m_hWnd, "���������������Ϣ", "����", 0);
			return;
		}
	}
	
	//�˴�Ӧ��Ҫ���洢���ݿ�Ȳ���
	int nIndex = m_userList->GetItemIndex(pParentCtrl);	
	m_userList->RemoveAt(nIndex);
	CDialogBuilder builder;
	CListContainerElementUI* pLine = (CListContainerElementUI*)(builder.Create(_T("single_list_item.xml"), (UINT)0, this));
	if( pLine != NULL ) 
	{
		SetListItemData(pLine, struserid, strpwd, strprotocol, strtype);
		m_userList->InsertItem(nIndex, 21, pLine);
	}	
}

void CMainDlg::StartEditItem( CControlUI* pControl )
{
	CControlUI *pParentCtrl = GetLineContainer(pControl);
	if (pParentCtrl == NULL)
	{
		return;
	}

	CDuiString struserid = m_PaintManager.FindSubControlByName(pParentCtrl, "userid")->GetText();
	CDuiString strpwd = m_PaintManager.FindSubControlByName(pParentCtrl, "pwd")->GetText();
	CDuiString strprotocol = m_PaintManager.FindSubControlByName(pParentCtrl, "protocol")->GetText();
	CDuiString strtype = m_PaintManager.FindSubControlByName(pParentCtrl, "type")->GetText();
	//��ȡ����ص�����  
	int nIndex = m_userList->GetItemIndex(pParentCtrl);	
	m_userList->RemoveAt(nIndex);
	CDialogBuilder builder;
	CListContainerElementUI* pLine = (CListContainerElementUI*)(builder.Create(_T("single_list_item_add.xml"), (UINT)0, this));
	if( pLine != NULL ) 
	{
		pLine->SetUserData("fromedititem");
		SetEditListItemData(pLine, struserid, strpwd, strprotocol, strtype);
		m_userList->InsertItem(nIndex, 23, pLine);
	}
}

void CMainDlg::BeginEditOrAddTreeNode( CControlUI* pControl )
{
	CTreeViewUI* pTree = static_cast<CTreeViewUI*>(m_PaintManager.FindControl(_T("tree")));
	RECT rt = pTree->m_pNodeModifyOrAdd->GetPos();
	RECT rtItem = pTree->m_pNodeModifyOrAdd->GetItemButton()->GetPos();

	//���������Ӧ����ʾ��λ�ã������Ϊ�����ʣ����Ե����˴� 
	RECT rtPos;
	rtPos.left = rtItem.left;
	rtPos.top = rt.top;
	rtPos.bottom = rt.bottom;
	rtPos.right = rtPos.left + 150;
	CControlUI* pControlModify = m_PaintManager.FindControl("modifyoradd");

	//ͨ��UserData����¼�Ǳ༭��������¼����Ա��������
	if (pControl->IsEqual("beginedit"))
	{
		pControlModify->SetUserData("edit");
		m_PaintManager.FindControl(_T("edittext"))->SetText(pTree->m_pNodeModifyOrAdd->GetItemText());
		m_PaintManager.FindControl("savetreenode")->SetText("����");
	}
	else 
	{
		m_PaintManager.FindControl(_T("edittext"))->SetText("");
		pControlModify->SetUserData("add");
		m_PaintManager.FindControl("savetreenode")->SetText("���");
	}
	pControlModify->SetVisible(true);
	pControlModify->SetPos(rtPos);

	pControlModify = m_PaintManager.FindControl("treemenu");
	pControlModify->SetVisible(false);
}

void CMainDlg::EndEditOrAddTreeNode()
{
	CTreeViewUI* pTree = static_cast<CTreeViewUI*>(m_PaintManager.FindControl(_T("tree")));

	CControlUI* pControlModify = m_PaintManager.FindControl("modifyoradd");
	//ͨ��UserData��ȷ���Ǳ༭��������¼�
	CDuiString strUserData = pControlModify->GetUserData();
	CDuiString strText = m_PaintManager.FindControl(_T("edittext"))->GetText();
	if (strUserData == "edit")	//���������޸�
	{
		pTree->m_pNodeModifyOrAdd->SetItemText(strText);
	}
	else if (strUserData == "add")	//ȷ�����Ҷ�ӽڵ� 
	{
		CTreeNodeUI* pNodeAdd = new CTreeNodeUI(true);
		pNodeAdd->SetItemText(strText);
		pTree->m_pNodeModifyOrAdd->AddChildNode(pNodeAdd);
	}
	pControlModify->SetVisible(false);
}
// SortListCtrl.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "FileTransferLib.h"
#include "SortListCtrl.h"


// CSortListCtrl

IMPLEMENT_DYNAMIC(CSortListCtrl, CListCtrl)

CSortListCtrl::CSortListCtrl()
{
	m_nSortedCol = 1;
}

CSortListCtrl::~CSortListCtrl()
{

}


BEGIN_MESSAGE_MAP(CSortListCtrl, CListCtrl)
END_MESSAGE_MAP()



// CSortListCtrl ��Ϣ�������



#pragma once


// CSortListCtrl

class CSortListCtrl : public CListCtrl
{
	DECLARE_DYNAMIC(CSortListCtrl)

public:
	CSortListCtrl();
	virtual ~CSortListCtrl();

protected:
	DECLARE_MESSAGE_MAP()
public:
	BOOL m_fAsc;		//�Ƿ�˳������
	int m_nSortedCol;	//��ǰ�������
};



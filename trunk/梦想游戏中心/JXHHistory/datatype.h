#pragma once
extern CListCtrl g_ShowResult;		//��ʷ��ʾ�б�
extern CListCtrlColore g_ShowMsg;			//��Ϣ��ʾ�б�
extern BOOL g_bExitFlag;			//�߳����п���
extern CCriticalSection g_ShowLock;		//��Ϣ��ʾ��
extern CString g_dbServer;//���ݿ�
extern CString g_dbPwd;				//���ݿ�����
extern CString g_strCurrRecord;		//��ǰ��Ӯ���
extern CString g_strCurrRecordList;	//��ǰ��ʷ��¼��
extern int g_iLastRecordID;			//���µļ�¼ID
extern CString  g_strAppPath;
extern CString  g_srcPicPath;
extern CString  g_dstPicPath11;
extern CString  g_dstPicPath12;
extern CString  g_dstPicPath13;
extern CString g_cmpPicPath1;
extern CString g_srcPicPath2;
extern CString g_cmpPicPath2;
extern void ShowMessageEx(CString strMsg);
extern CCriticalSection g_QueryFlagLock;		//ƥ�������
extern int g_iQueryFlag;
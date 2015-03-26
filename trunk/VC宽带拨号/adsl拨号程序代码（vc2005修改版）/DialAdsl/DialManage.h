//���������������д���ֽ����뷢����ѧϰ����֮��
//������ķ�������GPLЭ�飬�����ڴ�Э��Ĺ涨��ʹ�ñ�����
//�������ʹ�ñ������;����ʲô���ʻ��������http://digua254909662.blog.163.com/blog/static/74805072200922323951508/edit/
//������ϵ���ߣ�qq<254909662> ��˵�����⣬���˷ǳ�������������
#pragma once
#include <windows.h>
#include <Ras.h>

struct EntryNameInfo
{
	CHAR szEntryName[ RAS_MaxEntryName + 1 ];
	CHAR szUserName[ UNLEN + 1 ];
	CHAR szPassword[ PWLEN + 1 ];
};

typedef enum CONNECT_TYPE
{
	FIND_MODEM = 1,
	FIND_VPN   = 2,
	FIND_PPPOE = 3,
	FIND_ISDN  = 4,
	FIND_X25   = 5,
}CONNECT_TYPE;

//#define FIND_MY_DEVICE FIND_VPN

typedef VOID (__stdcall* LPRASDIALFUNC)(UINT, RASCONNSTATE, DWORD);

class DialManage
{
public:
	DialManage(void);
public:
	~DialManage(void);
public:
	void SetFindDefaultType(CONNECT_TYPE conType);
	int FindDefaultAdslConnect();
	void GetDialInfo(EntryNameInfo *lpDialInfo);
	void SetDialInfo(EntryNameInfo *lpDialInfo);
	int CmdDialConnect(HANDLE &hReadHanle);
	int RasDialConnect(LPRASDIALFUNC RasDialFunc);
	int RasDialReset();
	int GetErrorMessage(UINT dwError, LPSTR buffer, DWORD dwSize);
private:
	RASDIALPARAMS dialInfo;
	BOOL bExistAdsl;
	CONNECT_TYPE myFindConnectType;
};

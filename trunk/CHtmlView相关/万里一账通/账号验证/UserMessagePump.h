#pragma once
#include <afxmt.h>
#include <list>
using namespace std;

#define USERMSGSTART	 WM_USER + 500
#define USERWINDOWMSG_1  USERMSGSTART + 1
#define USERWINDOWMSG_2  USERMSGSTART + 2
#define USERWINDOWMSG_3  USERMSGSTART + 3
#define USERWINDOWMSG_4  USERMSGSTART + 4
#define USERWINDOWMSG_5  USERMSGSTART + 5
#define USERWINDOWMSG_6  USERMSGSTART + 6
#define USERWINDOWMSG_7  USERMSGSTART + 7
#define USERWINDOWMSG_8  USERMSGSTART + 8
#define USERWINDOWMSG_9  USERMSGSTART + 9
#define USERWINDOWMSG_10 USERMSGSTART + 10
#define USERWINDOWMSG_11 USERMSGSTART + 11
#define USERWINDOWMSG_12 USERMSGSTART + 12

#define USERWINDOWMSGSUCCESS  USERMSGSTART + 111		//����ע��ɹ�
#define USERWINDOWMSGWAITCHECKCODE   USERMSGSTART + 222		//�ȴ�������֤��
#define USERWINDOWMSGGOONREG		 USERMSGSTART + 333		//��֤�����룬����ע��



class CUserMessagePump
{
        struct _MessagePumpStr
        {
                HWND hwnd;
                CString msgString;
                UINT DstWindowID;
                _MessagePumpStr()
                {
                        hwnd = NULL;
                        msgString.Empty();
                        DstWindowID = 0;
                }
        };

        struct _MessageHWND
        {
                HWND hwnd;
                UINT DstWindowID;
                _MessageHWND()
                {
                        hwnd = NULL;
                        DstWindowID = 0;
                }
        };

private:
        static CUserMessagePump * m_Instance;          //����ָ��
        static CCriticalSection m_slockInstance;       //ȫ����,double-check Lock��
public:
        static CUserMessagePump * GetInstance();
        CUserMessagePump(void);
        ~CUserMessagePump(void);
        void SetHwnd(UINT windowID,HWND hwnd);
        void AddToMessagePump(UINT windowID,CString msgStr);		//������Ϣ
        void GetPumpMessage(UINT windowID,HWND hwnd,CString &msgStr);		//ȡ��Ϣ
private:
        list<_MessagePumpStr>   m_listMsg ;		//��Ϣ��
        CCriticalSection        m_lockMsg;
        list<_MessageHWND>	m_listHwnd ;	//��ϢĿ�괰�ھ����
        CCriticalSection        m_lockHwnd;
};

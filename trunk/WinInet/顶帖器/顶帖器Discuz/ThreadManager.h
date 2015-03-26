#pragma once
#include <afxmt.h>
#define			ALWAYSWAITTYPE     -2
#define			ONETIMEJUSTTYPE    -1
#define			ALWAYSWORKTYPE      0

class CThreadManager
{
public:
	CThreadManager(void);
	~CThreadManager(void);

	typedef void (*WorkFuncProc)(void * param);			//�����̺߳���
	

	void StartThread();								//�������߳�
	void StopThread();								//�����߳�
	void ThreadDoWork();							//ʹ�߳��������빤��״̬

	void Set(WorkFuncProc funcProc, void * lparameter, int cycleTime);		//�����̺߳�����ַ��������ִ������
	void SetWorkFunction( WorkFuncProc funcProc );							//�����̺߳�����ַ
	void SetWorkParameter( void * lparameter);								//�����̺߳�������
	void SetCycleTime( int cycleTime );										//�����߳�ִ������
	
	void SuspendThread();	//�����̣߳�����m_CycleTime��־Ϊ ALWAYSWAIT��
	void ResumeThread();	//�ָ��̣߳�����m_CycleTime��־Ϊ֮ǰ���õ�m_OldCycleTime,������ThreadDoWok����������
	unsigned long GetThreadId();											//��ȡ�߳�ID

protected:
	enum CycleTimeEnum{ ALWAYSWAIT = -2 , ONETIMEJUST = -1 , ALWAYSWORK = 0 };			//ִ������

	enum ThreadCmd{ NONE = 0 , STOP = 1 };

	static unsigned __stdcall WorkThread( void * lpParameter );	//�̹߳�������

	void WorkThreadProc();

	ThreadCmd m_ThreadCmd;			//�߳��Ƿ������־
	
	HANDLE   m_hThread;			//�߳̾��	
	HANDLE   m_hEvent;			//�¼����

	unsigned int m_ThreadId;		//�߳�ID
	
	int      m_CycleTime;			//ִ������
	int      m_OldCycleTime;		//�ɵ�ִ������,���ڻ�ԭ
	void *   m_lpParameter;			//�̺߳�������
	WorkFuncProc m_FunctionProc;		//�̺߳�����ַ

	CCriticalSection m_CycleTimeSection;		//�����趨��
};	

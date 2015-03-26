#pragma once

class CThreadBase 
{
public:
	CThreadBase();
	virtual ~CThreadBase();

public:
	bool StartWorking();      //��ʼִ��
	HANDLE StartWorkingEx();
	void StopWorking(HANDLE threadHandle = NULL, DWORD dwWait = 3000);			//����ִ��
	void MarkTheadToStop();   //��ͣ�߳�
	virtual void ThreadProc(); //OVERWRITE
	virtual void OnThreadStop(); 
	void SetRunning(bool bRunning = true);
	inline bool IsRunning(){ return m_bRunning; }

	//CString		m_DebugID;
protected:
	static	UINT WINAPI StartThread(LPVOID pParam);
	void ThreadLoop();

public:
	HANDLE	m_hThread;
	bool	m_bRunning;
public:
	UINT	m_uiThreadId;
protected:
};
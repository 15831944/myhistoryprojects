#include "StdAfx.h"
#include <process.h>
#include ".\threadmanager.h"

/************************************************************************/
/*                                                                      */
/************************************************************************/
//���캯��,��ʼ���̱߳�ʶ
CThreadManager::CThreadManager(void)
{
        m_ThreadCmd = NONE;										//�߳��Ƿ������ʶ

        m_hThread = NULL;										//�߳̾��
        m_hEvent = CreateEvent(NULL, FALSE, TRUE, NULL);		//�����¼�����

        m_ThreadId = 0;							//�߳�ID

        m_CycleTime = ONETIMEJUST;				//һ��ִ���߳�
        m_FunctionProc = NULL;					//������ַΪ��
        m_lpParameter = NULL;					//�̺߳�������Ϊ��
}


/************************************************************************/
/*                                                                      */
/************************************************************************/
//����,����߳�δ����������֮
CThreadManager::~CThreadManager(void)
{
        StopThread();
        CloseHandle(m_hEvent);
}


/************************************************************************/
/*                                                                      */
/************************************************************************/
//����һ�����߳�
void CThreadManager::StartThread()
{
        if ( m_ThreadId !=0 )		//����߳��Ѿ�������ֱ�ӷ���
        {
                return;
        }
        m_ThreadCmd = NONE;			//�������߳̽�����־Ϊ������
        m_hThread = (HANDLE)_beginthreadex(NULL,0, WorkThread, (void *)this, 0, &m_ThreadId);	//�����߳�

        TRACE("�����߳�%d\r\n",m_ThreadId);
}


/************************************************************************/
/*                                                                      */
/************************************************************************/
//�����߳�
void CThreadManager::StopThread()
{
        if ( m_ThreadId == 0 )		//����߳�δ������ֱ�ӷ���
        {
                return;
        }
        m_ThreadCmd = STOP;			//�����߳̽�����־Ϊ����

        ThreadDoWork();				//�����߳�ִ��
        WaitForSingleObject(m_hThread, 3000);	//�ȴ��߳̽���
        if ( m_ThreadId != 0)
        {
                TRACE("δ�ȵ��߳��Զ�����,ǿ�ƽ�����\r\n");                   
		m_ThreadCmd = NONE;
		m_ThreadId = 0;
		::TerminateThread(m_hThread,STILL_ACTIVE);
		m_hThread = NULL;
        }
	m_ThreadCmd = NONE;
	m_ThreadId = 0;
	m_hThread = NULL;

}



/************************************************************************/
/*                                                                      */
/************************************************************************/
//ʹ�߳��������빤��״̬
void CThreadManager::ThreadDoWork()
{
        if ( m_hThread == 0 )
        {
                StartThread();
                SetEvent(m_hEvent);
        }
        else
        {
                SetEvent(m_hEvent);
        }
}


/************************************************************************/
/*                                                                      */
/************************************************************************/
//�����߳�
void CThreadManager::SuspendThread()
{
        if ( m_ThreadId == 0 )
        {
                return;
        }
        TRACE("�����߳�%d\r\n",m_ThreadId);
        ::SuspendThread(m_hThread);
}


/************************************************************************/
/*                                                                      */
/************************************************************************/
//�ָ��߳�
void CThreadManager::ResumeThread()
{
        if ( m_ThreadId == 0 )
        {
                return;
        }
        TRACE("�ָ��߳�%d\r\n",m_ThreadId);
        ::ResumeThread(m_hThread);
}



/************************************************************************/
/*                                                                      */
/************************************************************************/
//�߳�ִ�й���
unsigned CThreadManager::WorkThread( void * lpParameter )
{
        CThreadManager * pThis = (CThreadManager*) lpParameter;
        if ( pThis == NULL )
        {
                return -1;		//������Ч
        }
        pThis->WorkThreadProc();
        return 0;			//�˳�
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
//�߳�ִ�й���
void CThreadManager::WorkThreadProc()
{
        while( true )		//ѭ��ִ��
        {
                if ( m_ThreadCmd == STOP )				//����̼߳�������
                {
                        TRACE("�߳�%d����\r\n",m_ThreadId);
                        m_ThreadCmd = NONE;
                        m_ThreadId = 0;
                        m_hThread = NULL;
                        return;			//�߳���ֹ
                }
                if ( m_FunctionProc != NULL )			//ִ���������̹߳���
                {
                        try
                        {          
                                ( *m_FunctionProc )( m_lpParameter );
                        }
                        catch ( ... )
                        {
                                //.......�쳣
                        }
                }

                int  cycleFlag = m_CycleTime;

                if ( cycleFlag == ONETIMEJUST )			//����߳̽�Ϊһ����ִ���߳�
                {
                        m_ThreadCmd = NONE;
                        m_ThreadId = 0;
                        m_hThread = NULL;
                        return;			//�����˳�
                }

                if ( cycleFlag > ALWAYSWORK )		//���������־Ϊ��ʱִ��
                {
                        WaitForSingleObject( m_hEvent, m_CycleTime );
                }
                if ( cycleFlag <= ALWAYSWAIT )		//���������־Ϊִ��һ�κ�ȴ�
                {
                        WaitForSingleObject( m_hEvent , INFINITE );
                }

        }
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
//�����̺߳�����ַ��������ִ������
void CThreadManager::Set( WorkFuncProc funcProc, void * lparameter, int cycleTime )
{
        m_FunctionProc  = funcProc;
        m_lpParameter	= lparameter;
        m_CycleTime = cycleTime;
}


/************************************************************************/
/*                                                                      */
/************************************************************************/
//�����̺߳�����ַ
void CThreadManager::SetWorkFunction( WorkFuncProc funcProc )
{
        m_FunctionProc = funcProc;
}


/************************************************************************/
/*                                                                      */
/************************************************************************/
//�����̺߳�������
void CThreadManager::SetWorkParameter( void * lparameter)
{
        m_lpParameter = lparameter;
}


/************************************************************************/
/*                                                                      */
/************************************************************************/
//��������
void CThreadManager::SetCycleTime( int cycleTime )
{
        m_CycleTime = cycleTime;
}


/************************************************************************/
/*                                                                      */
/************************************************************************/
//��ȡ�߳�ID
unsigned long CThreadManager::GetThreadId()
{
        return m_ThreadId;
}
#pragma once
#include "ADOFile/AdoRecordSet.h"
#include "ADOFile/AdoCommand.h"
#include "dataType.h"
#include "ASCIIConver.h"

class CDataBaseCtrl
{
public:
	CDataBaseCtrl(void);
	~CDataBaseCtrl(void);
	void InitParams(CString strdbServer,CString strDbName,CString strDbUser,CString strDbPwd,int iDbType,int iDbPort);
	void ReadSqlParams();
	int AddMoMessage(DeliverPack &mo);
	int UpdateSendResult(ReportPack &report);
	int ReadWaitMessage(KfSubmitPackList &sendBufList,CCriticalSection &sendListLock);
	int AddSendResult(KfSubmitPack &mt,int iFlag);
	void InitArray(CString strSrcIndex,int iFlag);	
	int ConvertSendMessage(KfSubmitPack &mt);
	bool IsSingleSend( CString strDestAddr );
private:
	int GetSendArray(CArray <CString, CString> &strSendList,CString strSrc);
	int ConvertPhoneList( CString &strDestAddr );
	int GetMtString(KfSubmitPack &mt,CString strArry[35],int iFlag,int iSucFlag);
	void GetReportString(ReportPack &report,CString strArry[7]);
	void GetMoString(DeliverPack &mo,CString strArry[7]);
	CString MakeSqlString(KfSubmitPack &mt,CString strSqlSrc,int iFlag,int iSucFalg);
	int InitMoStoreParams(CAdoCommand &adoCmd,DeliverPack &mo);
	int InitReportStoreParams(CAdoCommand &adoCmd,ReportPack &report);
	int InitAddResultStoreParams(CAdoCommand &adoCmd,KfSubmitPack &mt,int iFlag);	
	BOOL ConnectDataBase(CAdoConnection &adoConn,int iFlag);	//�������ݿ�
	BOOL ConnectDataBase(CAdoConnection &adoConn,CAdoRecordSet &adoRst,int iFlag);
private:
	CString m_strPhoneFilePath;			//strPath
	CAdoConnection m_adoConnect;		//��ȡ��������
	CAdoRecordSet  m_adoRcrd;			//��ȡ���ݼ�¼��
	CAdoCommand m_adoCmd;				//�洢���̶���
	CString m_dbServer;		//���ݿ������
	CString m_dbName;		//���ݿ���
	CString m_dbUser;		//��¼�˻�
	CString m_dbPwd;		//��¼����
	int m_idbType;
	int m_idbPort;

	int m_iOptFlag;		   //���ݿ⽻����ʽ,2�Զ���,3�洢����

	CString m_strReadSql;				//��ȡ������Ϣ
	CString m_strReadIndexList;			//����ӳ��
	CString m_strSucAddResultSql;		//���ͺ���ӷ��ͽ��[�ɹ�]
	CString m_strSucAddResultIndexList;	//��ӷ��ͽ��ӳ��[�ɹ�]
	CString m_strFailAddResultSql;		//���ͺ���ӷ��ͽ��[ʧ��]
	CString m_strFailAddResultIndexList;//��ӷ��ͽ��ӳ��[ʧ��]
	CString m_strSucCleanSql;			//���ͳɹ����´�����
	CString m_strSucCleanIndexList;		//�ɹ����´�����ӳ��
	CString m_strFailCleanSql;			//����ʧ�ܸ��´�����
	CString m_strFailCleanIndexList;	//ʧ�ܸ��´�����ӳ��
	CString m_strUpdateSql;				//״̬������·��ͽ����
	CString m_strUpdateIndexList;		//״̬�������ӳ��
	CString m_strAddMoSql;				//����������Ϣ	
	CString m_strAddMoIndexList;		//����������Ϣӳ��

	CString m_strAddResultStoreName;	//�洢����(���淢����ʷ)
	CString m_strAddResultStoreIndexList;	//�洢����(���淢����ʷ���)
	
	//CString m_strReadNumName;			//�洢�����ж�ȡʱ����Ĳ�����

	int m_iReadIndex[35];
	int m_iSucAddResultIndex[35];
	int m_iFailAddResultIndex[35];
	int m_iSucCleanIndex[35];
	int m_iFailCleanIndex[35];
	int m_iUpdateIndex[7];
	int m_iAddMoIndex[7];
	int m_iAddResultIndex[35];

	CString m_strReadParams[35];
	CString m_strAddResultParams[35];
	CString m_strSucAddResultParams[35];
	CString m_strFailAddResultParams[35];
	CString m_strSucCleanParams[35];
	CString m_strFailCleanParams[35];

	CString m_strUpdateParams[7];
	CString m_strAddmoParams[7];

	int m_iUpdateParamLen[7];
	int m_iAddmoParamLen[7];
	int m_iAddResultParamLen[35];

	CString m_strSplitString;			//�ָ����ַ���
	
	CString m_strMsgStatus[2];			//��Ϣ����״̬	��һλ��ʾ�ɹ���״̬,�ڶ�λ��ʾʧ�ܵ�״̬
	int m_iMsgStatus[2];
	CString m_strRecvStatus[5];		//ӳ���Ľ���״̬
	int m_iRecvStatus[5];

	CString m_strCharSet;			//ָ�����ݿ��ַ���
};

// MWGateway.h
// �ļ�˵��: �������� ˫������ �ͷ����� ��̬�� ����ͷ�ļ�
//   �汾��: 1.2.0.0
//     ʱ��: 2006-03-01
#define USE_SMEIDLL_DLL_
#ifdef WIN32
  #pragma once
#else
  #define SOCKET int
  #define __stdcall
#endif


char* hexChrTosrcStr(const char* strMsg,char* retstr);
void srcStrTohexChr(const char* strMsg,char* retstr);
int __stdcall SetUserType(const char* Os,int SoftType,const char* Version,const char* sPwd);
//---------------------------------------------------------------------------
// ͨ�÷���ֵ˵��
//   ����ִ�гɹ�������ֵΪ0������������ͨ��Ϊ1
//   ����ִ��ʧ�ܣ�����ֵΪ������
//   ������ֵС�� -100 ���� -200�����ʾ��Ԥ�ڵĴ��󣬼������˲�Ӧ�ò����Ĵ���
//       �����ô���ʱ����ر����Ӻ���������
//   ������ֵС�� -10000������ֵ�ľ���ֵ��ȥ10000����Ϊƽ̨�ķ���ֵ

//---------------------------------------------------------------------------
// ������Ϣ˵��
//   1.��ע�⣬������Ϣ����һЩ�ؼ��ֱ�ƽ̨�г��������޷��ύ�ɹ�
//   2.���͵���Ϣ������Ϊ���ֽڱ��룬��Ϣ������󳤶�Ϊ68���֣�һ��Ӣ���ַ�Ҳ
//     ��һ����


//---------------------------------------------------------------------------
//                                ***����***
// MongateConnect
// ����˵����
//        pszIP: [in] ������IP��ͨ��Ϊ"server1.montnets.com"����Ϊ"61.242.89.115"
//        iPort: [in] �������˿ڣ�ͨ��Ϊ 8018
//   pszAccount: [in] ��½�ʻ���
//       pszPwd: [in] ��½�ʻ�����
// ����ֵ��
//  ��ֵ: ��½�ɹ�����ֵΪsocket���
//    -1: const char*, char* �����Ĳ����У�������NULLֵ
//    -2: �Ƿ����ȵĵ�½�ʺţ�������
//    -3: ��ȡԶ��������Ϣʧ��,ԭ��:IP����ʧ�ܻ���Ч��IP,�����粻ͨ
//    -4: socket����ʧ��
//    -5: connectʧ��
//    -6: connect�ɹ�,�ȴ�ƽ̨��������ȷ����Ϣ��ʱ
//    -7: ����ƽ̨���ص�½��֤��Ϣ��ʱ
//    -8: ������û���������
//---------------------------------------------------------------------------
int __stdcall MongateConnect(const char* pszIP, int iPort
                             , const char* pszAccount, const char* pszPwd);

//---------------------------------------------------------------------------
//                            ***���͵�����Ϣ***
// MongateSendSms
// ����˵����
//      sock: [in] ���Ӿ�����þ����MongateConnect����
//   pszMobi: [in] ���շ��ֻ���
//    pszMsg: [in] ��Ϣ����
//   ����ֵ:
//      1: ������Ϣ�ɹ��ύ��ƽ̨
//     -1: const char*, char* �����Ĳ����У�������NULLֵ
//     -2: �Ƿ��ֻ���
//     -3: �ȴ����ͳ�ʱ
//     -4: �ȴ����ճ�ʱ
//---------------------------------------------------------------------------
int __stdcall MongateSendSms(SOCKET sock, const char* pszMobi,
                             const char* pszMsg);
int __stdcall MongateSendSmsBeta(SOCKET sock, const char* pszMobi,
                             const char* pszMsg, char* pszSN);

//---------------------------------------------------------------------------
//                             ***��������***
// MongateTestConn
// ����˵��:
//   sock: [in] ���Ӿ��
//  ����ֵ:
//     1: ��sock��Ч
//    -1: �ȴ����ͳ�ʱ
//    -2: �ȴ����ճ�ʱ
//    -3: ��sock��Ч
//---------------------------------------------------------------------------
int __stdcall MongateTestConn(SOCKET sock);
int __stdcall MongateTestConnHttp(SOCKET sock);

//---------------------------------------------------------------------------
//                             ***��ѯ���***
// MongateQueryBalance
// ����˵��:
//   sock: [in] ���Ӿ��
// ����ֵ:
//    >=0: Ϊ���
//     -1: �ȴ����ͳ�ʱ
//     -2: �ȴ����ճ�ʱ
//---------------------------------------------------------------------------
int __stdcall MongateQueryBalance(SOCKET sock);

//---------------------------------------------------------------------------
//                        ***��ѯ�Ѿ�ʹ������***
// MongateQueryUsed
// ����˵��:
//   sock: [in] ���Ӿ��
// ����ֵ:
//    >=0: Ϊ�Ѿ�ʹ������
//     -1: �ȴ����ͳ�ʱ
//     -2: �ȴ����ճ�ʱ
//---------------------------------------------------------------------------
int __stdcall MongateQueryUsed(SOCKET sock);

//---------------------------------------------------------------------------
//                            ***��ֵ***
// MongateRecharge
// ����˵��:
//        sock: [in] ���Ӿ��
//   pszCardNo: [in] ��ֵ������
//  pszCardPwd: [in] ��ֵ������
// ����ֵ:
//   >0: Ϊ�ʻ�ʣ������
//   -1: const char*, char* �����Ĳ����У�������NULLֵ
//   -2: ��ֵ�����Ż����볤����Ч
//   -3: �ȴ����ͳ�ʱ
//   -4: �ȴ����ճ�ʱ
//---------------------------------------------------------------------------
int __stdcall MongateRecharge(SOCKET sock, const char* pszCardNo,
                              const char* pszCardPwd);

//---------------------------------------------------------------------------
//                            ***�޸�����***
// MongateChangePwd
// ����˵��:
//         sock: [in] ���Ӿ��
//   pszAccount: [in] �ʺ�
//    pszOldPwd: [in] �ʺŵ�ǰ����
//    pszNewPwd: [in] �ʺ�������
// ����ֵ:
//    1: �ɹ�
//   -1: const char*, char* �����Ĳ����У�������NULLֵ
//   -2: �û��������볤�ȷǷ�
//   -3: �ȴ����ͳ�ʱ
//   -4: �ȴ����ճ�ʱ
//---------------------------------------------------------------------------
int __stdcall MongateChangePwd(SOCKET sock, const char* pszAccount,
                               const char* pszOldPwd, const char* pszNewPwd);

//---------------------------------------------------------------------------
//                            ***�Ͽ�����***
// MongateDisconnect
// ����˵��:
//    sock: [in] ���Ӿ��
// ����ֵ: ��
//---------------------------------------------------------------------------
void __stdcall MongateDisconnect(SOCKET sock);

//---------------------------------------------------------------------------
//                         ***��ֵ���أ���ȡ������Ϣ***
// MongateVasGetSms
// ����˵��:
//     sock: [in]  ���Ӿ��
//   strMsg: [out] ����������Ϣ������
// ����ֵ:
//    0-10: Ϊ��ƽ̨����
//      -1: strMsgΪNULL
//      -2: �ȴ����ͳ�ʱ
//      -3: �ȴ����ճ�ʱ
//---------------------------------------------------------------------------
int __stdcall MongateVasGetSms(SOCKET sock, char strMsg[500][255]);


//---------------------------------------------------------------------------
//                      ***��ֵ���أ�������Ϣ***
// MongateVasSendSms
// ����˵��:
//          sock: [in] ���Ӿ��
//    pszFeeCode: [in] ���ʴ���
//  pszServiceNo: [in] �������,������ʱ�Ķ˿ں�
//   pszOperCode: [in] ������(�ֽв�����)
//     pszLinkID: [in] linkid
//      pszMobis: [in] ���շ��ֻ����� ��:"13500000000,13600000000"
//        pszMsg: [in] ��������
//    iMobiCount: [in] �����ֻ��������е��ֻ�����
// ����ֵ:
//    1: �ɹ�
//   -1: const char*, char* �����Ĳ����У�������NULLֵ
//   -2: pszMobis�ֻ������� ���ϸ�
//   -3: �ȴ����ͳ�ʱ
//   -4: �ȴ����ճ�ʱ
//  -10: ����ѿռ�ʧ��
//---------------------------------------------------------------------------
int __stdcall MongateVasSendSms(SOCKET sock, const char* pszFeeCode,
                                 const char* pszServiceNo,
                                 const char* pszOperCode,
                                 const char* pszLinkID,
                                 const char* pszMobis,
                                 const char* pszMsg, int iMobiCount);								 
int __stdcall MongateVasSendSmsBeta(SOCKET sock, const char* pszFeeCode,
                                 const char* pszServiceNo,
                                 const char* pszOperCode,
                                 const char* pszLinkID,
                                 const char* pszMobis,
                                 const char* pszMsg, int iMobiCount,
								 char* pszSN); 
//---------------------------------------------------------------------------
//                         ***�ͷ����أ���ȡ������Ϣ***
// MongateGetCsSms
// ����˵��:
//     sock: [in]  ���Ӿ��
//   strMsg: [out] ����������Ϣ������
// ����ֵ:
//    0-10: Ϊ��ƽ̨����
//      -1: strMsgΪNULL
//      -2: �ȴ����ͳ�ʱ
//      -3: �ȴ����ճ�ʱ
//---------------------------------------------------------------------------
int __stdcall MongateCsGetSms(SOCKET sock, char strMsg[500][255]);
int __stdcall MongateCsGetSmsEx(SOCKET sock, char *strMsg);


//---------------------------------------------------------------------------
//                       ***�ͷ����أ�������Ϣ***
// MongateCsSendSms
// ����˵��:
//        sock: [in] ���Ӿ��
//    pszMobis: [in] �ֻ������� ��:"13500000000,13600000000"
//      pszMsg: [in] ��Ϣ����
//  iMobiCount: [in] �ֻ���������,�ֻ��Ÿ���
// ����ֵ:
//    1: �ɹ�
//   -1: const char*, char* �����Ĳ����У�������NULLֵ
//   -2: pszMobis�ֻ������� ���ϸ�
//   -3: �ȴ����ͳ�ʱ
//   -4: �ȴ����ճ�ʱ
//  -10: ����ѿռ�ʧ��
//---------------------------------------------------------------------------
int __stdcall MongateKxtSend(SOCKET sock, const char* pszMobis,
							   const char* pszMsg, int iMobiCount, char* pszSN);
int __stdcall MongateCsSendSms(SOCKET sock, const char* pszMobis,
                            const char* pszMsg, int iMobiCount, char* pszSN);

//---------------------------------------------------------------------------
//                         ***�ͷ����أ�ȡ״̬����***
// MongateCsGetStatusReport
// ����˵��:
//     sock: [in]  ���Ӿ��
//   strMsg: [out] ���ڴ��״̬��Ϣ
// ����ֵ:
//     1: �ɹ�
//    -1: strMsgΪNULL
//    -2: �ȴ����ͳ�ʱ
//    -3: �ȴ����ճ�ʱ
//---------------------------------------------------------------------------
int __stdcall MongateCsGetStatusReport(SOCKET sock, char strMsg[500][255]);

int __stdcall MongateCsGetStatusReportEx(SOCKET sock, char *strMsg);


//---------------------------------------------------------------------------
//                       ***��ȡ��̬��汾***
// MongateGetVer
// win�汾��,��鿴dll�����еİ汾�ż���
//---------------------------------------------------------------------------
const char* __stdcall MongateGetVer();
void TestMsg(const char* mystr);


//---------------------------------------------------------------------------
//                       ***�ͷ����أ��Ӷ˿ڷ�����Ϣ***
// MongateCsSPSendSms
// ����˵��:
//        sock: [in] ���Ӿ��
//    pszMobis: [in] �ֻ������� ��:"13500000000,13600000000"
//      pszMsg: [in] ��Ϣ����
//  iMobiCount: [in] �ֻ���������,�ֻ��Ÿ���
//   psSubPort: [in] �ͷ��Ӷ˿ڱ��
//     msgPkid: [in] �û��Զ�����Ϣ����ţ�Ĭ��Ϊ(*)�ݲ�����
//       pszSN: [out] ��Ϣ���
// ����ֵ:
//    1: �ɹ�
//   -1: const char*, char* �����Ĳ����У�������NULLֵ
//   -2: pszMobis�ֻ������� ���ϸ�
//   -3: �ȴ����ͳ�ʱ
//   -4: �ȴ����ճ�ʱ
//  -10: ����ѿռ�ʧ��
//---------------------------------------------------------------------------
int __stdcall MongateCsSPSendSms(SOCKET sock, const char* pszMobis,
   const char* pszMsg, int iMobiCount, const char* pszSubPort,char* pszSN);

int __stdcall MongateCsSPSendSmsHttp(SOCKET sock, const char* pszMobis,
	const char* pszMsg, int iMobiCount, const char* pszSubPort,char* pszSN);
   //int msgPkid,char* pszSN);
//---------------


/*-------------------------------------20070326 fq---------------------------------------------*/

//---------------------------------------------------------------------------
//                      ***��ʱ��Ϣ����***
//����˵��:
//	sock:[in] ���Ӿ��
//  iType:[in] ��ʱ����,0 ��ʾ'ֻ��һ��',1��ʾ'ÿ�췢��',2'��ʾÿ�ܷ���',5��ʾ'ÿ�·���'
//	pBeginDate: [in]��ʼ����,��ʱ��ʼִ�е����ڡ���ʽΪ2006-01-01
//	pEndDate��[in]�������ڣ���ʱ����ִ�е����ڡ���ʽΪ2006-01-01
//  iFlag��[in]��ʱ����, ��̬������
//       ��ֻ��һ�Ρ���ʱ��Ϊ��ʱ����:��ʽ20060101
//       ��ÿ�췢�͡���ʱ��Ϊ0
//       ��ÿ�ܷ��͡���ʱ��Ϊ4�ֽڵ����������,���ҵ���ÿbit����һ��, 1Ϊѡ��
//                         ����,0Ϊ������.�硰0011111��Ϊ��һ�����巢�ͣ�����
//                         �ղ�����
//       ��ÿ�·��͡���ʱ��Ϊ4�ֽ�����ת�����ַ�����������ַ���,���ҵ���ÿbit
//                         λ����һ��,1Ϊѡ�з���,0Ϊ������
//                         ������ơ�01111111111111111111111111111111��,ת��ʮ
//                         ����λ2��31�η���31�춼���ͣ�ת��ֵ0Ϊ��ĩ���͡�
//  Time��[in]��ʱʱ��: ��ʽΪ15:30:01
//  iMobiCount��[in] �ֻ���������,�ֻ��Ÿ���
//  pszMsg��[in] ��Ϣ����
//  pTelNos��[in] ���շ��ֻ����� ��:"13500000000,13600000000"
//---------------------------------------------------------------------------
int __stdcall MongateCsSendTime(SOCKET sock,
								int iType,
                                const char* pBeginDate,
                                const char* pEndDate,
                                int  iFlag,
                                const char* Time, 
								int iMobiCount,
								const char* pszMsg,
								const char* pTelNos);	

int __stdcall MongateCsSendTimeHttp(SOCKET sock,
								int iType,
                                const char* pBeginDate,
                                const char* pEndDate,
                                int  iFlag,
                                const char* Time, 
								int iMobiCount,
								const char* pszMsg,
								const char* pTelNos);	

//---------------------------------------------------------------------------
//                      ***���¶�ʱ��Ϣ����***
//����˵��:
//	sock:[in] ���Ӿ��
//  iNumber:[in] ��ʱ���
//  iType:[in] ��ʱ����,0 ��ʾ'ֻ��һ��',1��ʾ'ÿ�췢��',2'��ʾÿ�ܷ���',5��ʾ'ÿ�·���'
//	pBeginDate: [in]��ʼ����,��ʱ��ʼִ�е����ڡ���ʽΪ2006-01-01
//	pEndDate��[in]�������ڣ���ʱ����ִ�е����ڡ���ʽΪ2006-01-01
//  iFlag��[in]��ʱ����, ��̬������
//       ��ֻ��һ�Ρ���ʱ��Ϊ��ʱ����:��ʽ20060101
//       ��ÿ�췢�͡���ʱ��Ϊ0
//       ��ÿ�ܷ��͡���ʱ��Ϊ4�ֽڵ����������,���ҵ���ÿbit����һ��, 1Ϊѡ��
//                         ����,0Ϊ������.�硰0011111��Ϊ��һ�����巢�ͣ�����
//                         �ղ�����
//       ��ÿ�·��͡���ʱ��Ϊ4�ֽ�����ת�����ַ�����������ַ���,���ҵ���ÿbit
//                         λ����һ��,1Ϊѡ�з���,0Ϊ������
//                         ������ơ�01111111111111111111111111111111��,ת��ʮ
//                         ����λ2��31�η���31�춼���ͣ�ת��ֵ0Ϊ��ĩ���͡�
//  Time��[in]��ʱʱ��: ��ʽΪ15:30:01
//  iMobiCount��[in] �ֻ���������,�ֻ��Ÿ���
//  pszMsg��[in] ��Ϣ����
//  pTelNos��[in] ���շ��ֻ����� ��:"13500000000,13600000000"
//---------------------------------------------------------------------------
int __stdcall MongateCsUpdateSendTime(SOCKET sock,
									  int iNumber,
								      int iType,
                                      const char* pBeginDate,
                                      const char* pEndDate,
                                      int  iFlag,
                                      const char* Time, 
								      int iMobiCount,
								      const char* pszMsg,
								      const char* pTelNos);

int __stdcall MongateCsUpdateSendTimeHttp(SOCKET sock,
									  int iNumber,
								      int iType,
                                      const char* pBeginDate,
                                      const char* pEndDate,
                                      int  iFlag,
                                      const char* Time, 
								      int iMobiCount,
								      const char* pszMsg,
								      const char* pTelNos);



//---------------------------------------------------------------------------
//                      ***ɾ��ָ����ʱ��Ϣ***
//����˵��:
//	sock:[in] ���Ӿ��
//  iNum:[in] ��ʱ���
//---------------------------------------------------------------------------
int __stdcall MongateCsDelSendTime(SOCKET sock,
								   int iNum);

int __stdcall MongateCsDelSendTimeHttp(SOCKET sock,
								   int iNum);
	
//---------------------------------------------------------------------------
//                      ***ɾ������ָ����ʱ��Ϣ***
//����˵��:
//	sock:[in] ���Ӿ��
//---------------------------------------------------------------------------
int __stdcall MongateCsDelAllSendTime(SOCKET sock);

int __stdcall MongateCsDelAllSendTimeHttp(SOCKET sock);

//---------------------------------------------------------------------------
//                      ***��ȡ��ʱ��������***
//����˵��:
//	sock:[in] ���Ӿ��
//  iNumber:[in] ��ʱ���
//  iType:[in] ��ȡ����: Ϊ��ȡ���ͣ�1Ϊ���ڸö�ʱ��ţ�2Ϊ���ڸö�ʱ���,����������һ
//	        ��ֻ���ز��ֶ�ʱ��������
//  pStrMessage:[out] ��ȡ������������Ϣ
//---------------------------------------------------------------------------
int __stdcall MongateCsReadSendTime(SOCKET sock,
									int iNumber,
								    int iType,
									char strMsg[10][1024]
									);

int __stdcall MongateCsReadSendTimeEx(SOCKET sock,int iNumber,int iType,char *strMsg);

int __stdcall MongateCsReadSendTimeHttp(SOCKET sock,
									int iNumber,
								    int iType,
									char strMsg[10][1024]
									);
int __stdcall MongateCsReadSendTimeHttpEx(SOCKET sock,int iNumber,int iType,char *strMsg);
//---------------------------------------------------------------------------
//                      ***���¶�ʱ�����б�***
//����˵��:
//	sock:[in] ���Ӿ��
//  iNumber:[in] ��ʱ���
//  iNum:[in] Ϊ�����б��к����ʵ�ʸ���
//  pTelNos:[in] �����б�: 1~1000������(���Ϻ������),�м���Ӣ�Ķ��ŷָ�
//---------------------------------------------------------------------------
int __stdcall MongateCsUpdateTelListSendTime(SOCKET sock,
									int iNumber,
								    int iNum,
									const char* pTelNos);

int __stdcall MongateCsUpdateTelListSendTimeHttp(SOCKET sock,
									int iNumber,
								    int iNum,
									const char* pTelNos);

//---------------------------------------------------------------------------
//                      ***��ȡ��ʱ�����б�***
//����˵��:
//	sock:[in] ���Ӿ��
//  iNumber:[in] ��ʱ���
//  pStrMessage:[out] ��ȡ������������Ϣ
//---------------------------------------------------------------------------
int __stdcall MongateCsReadTelListSendTime(SOCKET sock,
									      int iNumber,
										  char* pStrMessage);

int __stdcall MongateCsReadTelListSendTimeHttp(SOCKET sock,
									      int iNumber,
										  char* pStrMessage);

/*-----------------------------------20070326 fq-------------------------------------------*/
int __stdcall MongateTestMsg(SOCKET sock, const char* pStrMessage);
int __stdcall MongateTestMsgEx(SOCKET sock,const char* pStrMessage,char *keywords);
int __stdcall MongateTestMsgHttp(SOCKET sock,const char* pStrMessage);
int __stdcall MongateTestMsgExHttp(SOCKET sock,const char* pStrMessage,char *keyword);
int __stdcall MGhexChrTosrcStr(const char* srcMsg,char* retMsg);
int __stdcall MGQueryAccount(SOCKET sock ,const char * pReserve,char* pReStr);
/*******************************************************************************************/

int __stdcall MongateConnectHttp(const char* pszIP, int iPort,
   const char* pszAccount, const char* pszPwd);
int __stdcall MongateQueryUsedHttp(SOCKET sock);
int __stdcall MongateQueryBalanceHttp(SOCKET sock);
int __stdcall MongateCsSendSmsHttp(SOCKET sock, const char* pszMobis,
	const char* pszMsg, int iMobiCount, char* pszSN);
int __stdcall MongateKxtSendHttp(SOCKET sock, const char* pszMobis,
	const char* pszMsg, int iMobiCount, char* pszSN);
int __stdcall MongateCsGetSmsHttp(SOCKET sock, char strMsg[500][255]);
int __stdcall MongateCsGetSmsHttpEx(SOCKET sock, char *strMsg);
int __stdcall MongateChangePwdHttp(SOCKET sock, const char* pszAccount,
  const char* pszOldPwd, const char* pszNewPwd);
int __stdcall MongateCsGetStatusReportHttp(SOCKET sock, char strMsg[500][255]);
int __stdcall MongateCsGetStatusReportHttpEx(SOCKET sock, char *strMsg);
int __stdcall MongateRechargeHttp(SOCKET sock, const char* pszCardNo,
  const char* pszCardPwd);
//int __stdcall MongateTestConn(SOCKET sock);
int __stdcall MongateConnectSocks4Proxy(const char* pszProxyIP,int iProxyPort,
  const char* pszIP,int iPort,const char* pszAccount,const char* pszPwd);
int __stdcall MongateConnectSocks5Proxy( const char* pszProxyIP,
  int iProxyPort,const char* pszProxyAccount,const char* pszProxyPwd,
  const char* pszIP,int iPort,const char* pszAccount,const char* pszPwd);
int __stdcall MongateConnectHttpProxy(const char* pszProxyIP,
  int iProxyPort,const char* pszProxyAccount,const char* pszProxyPwd,
  const char* pszIP, int iPort, const char* pszAccount, const char* pszPwd);
int __stdcall MongateSetCmdType(int iCmdType); //���õ�½����
int __stdcall MongateSetOverTime(int iOverTime); //�������糬ʱʱ��

int __stdcall MongateCsReadInform(SOCKET sock,
								  int iType,
								  const char* sNumber,
								  const char* reserve1,
								  const char * reserve2,
								  const char * reserve3,
								  char *strMsg);
void TranMsgIdI64ToChar(unsigned char* MsgId, __int64 IMsgId);
int TranMsgId(unsigned char *pSrc, char *MsgId);
void TranSmitMsgID(unsigned char *pSrc, char *MsgId);

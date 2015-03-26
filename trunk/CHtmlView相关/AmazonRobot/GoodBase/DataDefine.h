#ifndef  _ORDER_DEFINE__
#define  _ORDER_DEFINE__
#include <map>
using std::map;
//#include "stdafx.h"
#define MUTEX_DEFINE "_amazonrobot_yutone_copyright"
#define AMAZON_CONFIG_FILE "amazonset.ini"
#define AMAZON_ROBOTNAME "AmazonRobot.exe"
#define FLAG_STRING "source=eqifa|140382|1|"
#define AMAZON_TITILE "%E4%BA%9A%E9%A9%AC%E9%80%8A%E7%BD%91%E7%AB%99"
#define HAVE_ALREADY_RUN  555	//ֻ������һ������
#define USER_OR_PWD_WRONG  666	//�˺Ż���������� [��ʱû��]
#define USER_TIMEOUT  777		//��ʱ
#define ORDER_SUCESS  888		//�µ��ɹ�[��ʱû��]
#define ORDER_ERROR   999		//�µ�ʧ��
#define ORDER_DUPLICATE 1111	//�ظ��µ�
#define LOGIN_FAILED 2222		//��¼ʧ��
#define PARAM_ERROR 3333		//��������
#define ADD_TOCARTERROR 4444	//��ӵ����ﳵʧ��
#define PROCEEDTOCHECKOUT_ERROR 5555	//����Ʒ�����ʧ��
#define LOGINPAGE_ERROR 6666	//���ص�¼ҳ��ʧ��
#define GETSNDADDRINPUT_ERROR 7777	//������д��ַҳ��ʧ��
#define SETADDR_ERROR 8888		//��д��ַʧ��
#define GETQUERY_ERROR 9999 //���ض���ҳ��ʧ��

#define GETCHANGE_ERROR 11111 //�����޸�֧����ʽҳ��ʧ��
#define CHANGE_ERROR 22222 //�����޸�֧����ʽҳ��ʧ��

#define NONORMAL -1 //������
#define NOEMS 1 //��EMS
#define NOEXPRESS 2 //�޿��
#define NOSUPEREXPRESS 3 //�޳������
#define CONFIG_ERROR	4
#define PAYTYPE_ERROR	5	//֧��������Ч
#define BILL_CONTENT_ERROR 6	//��Ʊ���ʹ�
#define NOSUCHPAYTYPE	  7		/*�������͵�ַ����MPOS���ֽ�����ͷ�Χ��.��ѡ����ͻ���ʽ��ƽ�ʻ�����ؿ�ר�ݣ�EMS��*/

#define ONLY_ALL_COMPLETE 8	//ֻ��ѡ��\"�ȴ�������Ʒ����һ�𷢻�

#define NOT_ENUGH_GOOD 9	//����Ʒ�Ĺ�����������
#define ORDER_TIMEOUT 10	//����Ʒ�Ĺ�����������
#define ORDER_UNKNOWN_STATE 11
#define PARAM_EXCEPTION 12
#define NO_GIFT_CARD 13
#define LOW_GIFTCARD_BALANCE 14
#define BE_SET_QUICK_ORDER 15

#define BOOKNOTFOUND 44444 //
#define NOMATCHPRICEBOOK 55555 //

#define ADDR_ERROR 66666 //
#define SEND_TYPE_ERROR 77777
#define PAY_TYPE_ERROR 88888
enum _SendTime		
{
	NoneSendTime = 0,
	WorkDayOnly,
	HolidayOnly,
	NightOrOther,
	AddDays
};

enum _ShipType		
{
	Normal = 1,
	Ems,
	Express,
	SuperExpress,
	Scheduled
};

enum _QueryType
{
	Last30 = 1,
	Months6,
	Year2011
};

class GoodOrderStru
{
public:
	CString strGoodNos;	//��ƷID�ַ���
	map<CString, int> mapGoodNos;
	CString strUserID;
	CString strPwd;
	CString strProvince;
	CString strCity;
	CString strTown;	
	CString strRecver;
	CString strAddr;
	CString strPostCode;
	CString strPhone;
	
	_ShipType eShipType;
	BOOL bCanNormalSend;
	BOOL bShipWaitType;	//0��������, 1�Ȼ����һ������
	_SendTime eSendTime;
	CString strSendRemark;
	CString strPayType;	//���ʽID
	CString strPayTypeEx;	//��������

	bool bBill;
	CString strBillHeader;
	CString strBillContent;

	CString strClaimCode;	//�Ż���
	BOOL bUseGiftCard;
	int iShowFlag; //�Ƿ���ʾ
	int iAutoRun;
	int iDebug;	//����ǵ���״̬,��ʧ����,���˳�����,�ɹ۲�
	int iOpFlag;	//1��ʾ��ѯ��2��ʾ�µ�,3�޸�֧����ʽΪ�й�����
	_QueryType eQueryType;	

	
	
	
	CString strOrderNo;	//�����޸Ķ���֧����ʽΪ�ʾ�
	//_PayType ePayType;
	GoodOrderStru()
	{
		bCanNormalSend = FALSE;
		bShipWaitType = TRUE;
		eShipType = Express;
		eSendTime = NightOrOther;
		strPayType = "֧����";
		bBill = false;
		bUseGiftCard = FALSE;
	}
};

typedef struct _BookInfo
{
	CString strNo;	//��Ż�����Ʒ��,����iType����,���iType��1,���ʾ��Ʒ��,�����0,���ʾ���
	float fPrice;	
}BookInfo;

#endif

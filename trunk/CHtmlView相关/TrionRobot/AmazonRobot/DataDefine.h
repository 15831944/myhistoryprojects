#pragma 

#define MUTEX_DEFINE "_amazonrobot_yutone_copyright"
#define AMAZON_CONFIG_FILE "amazonset.ini"
#define AMAZON_ROBOTNAME "AmazonRobot.exe"
#define FLAG_STRING "source=eqifa|140382|1|"
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
enum _SendType
{
	WorkDayOnly = 1,
	HolidayOnly,
	NightOrOther,
	AddDays
};
enum _QueryType
{
	Last30 = 1,
	Months6,
	Year2011
};

typedef struct _AmazonOrderStru
{
	CString strGoodNos;	//��ƷID�ַ���
	CString strUserID;
	CString strPwd;
	CString strProvince;
	CString strCity;
	CString strTown;	
	CString strRecver;
	CString strAddr;
	CString strPostCode;
	CString strPhone;
	_SendType eSendType;
	bool bBill;
	CString strBillHeader;
	CString strBillContent;
	int iShowFlag; //�Ƿ���ʾ
	int iAutoRun;
	int iDebug;	//����ǵ���״̬,��ʧ����,���˳�����,�ɹ۲�
	int iOpFlag;	//1��ʾ��ѯ��2��ʾ�µ�,3�޸�֧����ʽΪ�й�����
	_QueryType eQueryType;

	CString strOrderNo;	//�����޸Ķ���֧����ʽΪ�ʾ�
	//_PayType ePayType;
}AmazonOrderStru;
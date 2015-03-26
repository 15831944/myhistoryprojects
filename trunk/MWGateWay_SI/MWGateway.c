// MWGateway.cpp
//
#define bool unsigned char
#define true 1
#define false 0
#include <assert.h>
//��ͨ������ MongateCsSPSendSms �� 61ָ��
#define CsSPSend61 1
//{--------------------------------------------------------------------------
#ifdef WIN32
#  define MW_GATEWAY_VERSION "MWGateway.dll Version 3.4.3.0"
#  include <stdio.h>
#  include "locale.h" 
#  include <string.h>
#  include <assert.h>
#  include <time.h>
#  include <Winsock2.h>
#  include "MWGateway.h"
#  include "montnets_MWGateway.h"
#  pragma warning(disable : 4127)
#else
//---------------------------------------------------------------------------
#  define MW_GATEWAY_VERSION "libMWGateway.so.3.4.3.0"
#  include <stdio.h>
#  include <stdlib.h>
#  include <string.h>
#  include <assert.h>
#  include <time.h>
#  include <sys/socket.h>
//#include <sys/types.h>
#  include <netinet/in.h>
#  include <arpa/inet.h>
#  include <netdb.h>
#  include <unistd.h>
#  include "MWGateway.h"
#  include "montnets_MWGateway.h"
#endif //endif WINDOWS
#ifdef _DEBUG
#undef _DEBUG
#endif
#include "Des.h"
//}--------------------------------------------------------------------------
//}--------------------------------------------------------------------------
//{{
//************************************//
//��ֵ�˿�ת�ͷ�
//************************************//

#define MAXUNISERIAL 100000
struct Z_UNISERIAL{ //save Unicom serial number
	long phno; //�绰����
	char szSerial[21]; //ƥ����
	int day; //����
};
struct Z_UNISERIAL g_UniSerial[MAXUNISERIAL]={0};
long g_UniSerPos =0;
char vasport[36]={0};
//MongateCsGetSmsEx -> MongateVasGetSms
//MongateCsGetSms -> MongateVasGetSms
//MongateCsSPSendSms -> MongateVasSendSms

int vassend = 0;//�Ƿ����ֱ������ֵͨ��

//}}
#define MAX_SMS_LENGTH  136  //��������β��'\0'
int g_iOvertime=45;
long g_lastquery=0;
int g_iReportSpace =0;   //����״̬���淵�صļ��ʱ��
int g_iLastTick = 0;      //�����ȡ״̬��������ʱ��
int g_iMOSpace =0;       //�����ȡ������Ϣ�ļ��ʱ��
//int g_iLastMOTick = 0;    //�����ȡ������Ϣ�����ʱ��
int g_iSendNumber = 0;    //�ݴ淢������ʹ��
char g_Os[10]="WIN2K";        //����ϵͳ�汾
int g_SoftType=0;         //������� 2����  1��ܰͨ
char g_sVersion[6]="0300";   //�汾��
int g_iCmdType=25;        //������������
int g_ConnectType = 0;     //��������
char mpath[255]={0};
static time_t g_tHistory[9];
char Server[50]={0},proxyEncode[100]={0};//������ip
int httpproxy=0;

int g_iMoPackNum = 0;		//����������Ϣ���������
int g_iReportPackNum = 0;	//����״̬������������

int g_iMoNum = 0;		//������Ϣ����
int g_iReportNum = 0;	//״̬���������

char g_szpath[255] = {0};

char szMoMsg[500][255] = {0};
char szRpt[500][255] = {0};

int g_iMoNumEx = 0;		//������Ϣ����
int g_iReportNumEx = 0;	//״̬���������
char szMoMsgEx[500*255] = {0};
char szRptEx[500*255] = {0};

HINSTANCE g_handle = 0x0;
HINSTANCE g_handle1 = 0x0;
char g_spgate[50] = {0};	//ͨ����

CRITICAL_SECTION	g_csReport;
#ifdef USE_SMEIDLL_DLL_
/////////////////////////////////////////////////////
//��̬�⺯���ӿ�
/////////////////////////////////////////////////////
typedef int (FAR __stdcall *_IFInitInterface)(DWORD dwCodeProtocol, DWORD dwDriverProtocol, LPCTSTR pDriverParam);
typedef int (FAR __stdcall *_IFExitInterface)();
typedef int (FAR __stdcall *_Login_R)(LPCTSTR Account, LPCTSTR Password);
typedef int (FAR __stdcall *_SendSms)
(
 const char* sOrgAddr,		//Դ��ַ
 const char* sPhones,		//�ֻ��ţ����һ�η���100������Ӣ�Ķ��ŷָ�
 unsigned char ReportFlag,	//�Ƿ�Ҫ״̬����,1��Ҫ��0����Ҫ
 const char *sMsg,		    //��������
 unsigned char *sMsgID,		//���ص�msgId
 unsigned char* FCS		    //���صĴ�����
 );

typedef int (FAR __stdcall *_RecvSms)
(
 char *sOrgAddr,			//Դ��ַ
 char *sPhone,				//�ֻ���
 unsigned char *sMsgID,		//���л�״̬�����msgid
 unsigned char *ReportFlag, //��Ϣ��־ 0:״̬���� 1:����
 char *sTimeStamp,			//���յ���ʱ�� ��ʽ��2009-10-28 12:12:12	
 char *sSubmitdate,			//�ύʱ��   200910281212
 char *sDonedate,			//����ʱ��200910281212
 char *sErrorCode,			//״���������
 char *sMsg				    //������Ϣ
 );

typedef int (FAR __stdcall *_WriteToDatabase)
(
	char *szReport,			//״̬���� 
	int iCount
);
typedef int (FAR __stdcall *_ReadFromDatabase)
(
 char *szReport,			//״̬���� 
 int *iCount
);

_IFInitInterface IFInitInterface = NULL;
_IFExitInterface IFExitInterface = NULL;
_Login_R         Login_R = NULL;
_SendSms         SendSms = NULL;
_RecvSms         RecvSms = NULL;
_WriteToDatabase WriteToDatabase = NULL;
_ReadFromDatabase ReadFromDatabase = NULL;
#endif

//---------------------------------------------------------------------------
//{--------------------------------------------------------------------------
//ͨ��ʱ����漴��Ȩ
char timeEncry[200]={0};
//ע���������
char EncodeID[256]={"{{MW}}"};
#define LINKID_TMP_FILENAME "\\phones.data"
#ifdef WIN32
BOOL APIENTRY DllMain( HANDLE hModule, 
                      DWORD  ul_reason_for_call, 
                      LPVOID lpReserved )
{
	FILE *fp;
	int i=0,size,iret;
	WORD wVersionRequested;
	WSADATA wsaData;
	char *ptr,tmp[255]={0};
	char szpath[255] = {0};
	hModule;
	lpReserved; //avoid warning
	//GetCurrentDirectory(255,mpath);
	time(&(g_tHistory[0]));
	if (strlen(g_Os)==0) {
		memset(g_Os,0,sizeof(g_Os));
		memcpy(g_Os,"WIN2K",5);
	}
	if (strlen(g_sVersion)==0) {
		memset(g_sVersion,0,sizeof(g_sVersion));
		memcpy(g_sVersion,"0300",4);
	}
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:  
		wVersionRequested = MAKEWORD( 2, 0 ); 
		if ( WSAStartup( wVersionRequested, &wsaData ) != 0 )
		{ 
			return false;
		}
//		InitializeCriticalSection( &g_csReport);
		GetModuleFileName(hModule,mpath,255);
		ptr = mpath + strlen(mpath);
		while(*ptr != '\\' && *ptr != '/')
		{
			ptr--;
		}
		*ptr = '\0';
		//��ȡ����	
		strcat(tmp,mpath);
		strcpy(szpath,tmp);	
		strcpy(g_szpath,szpath);
		strcat(tmp,"\\MWGateway.ini");
		GetPrivateProfileString("Port","VasPort","",vasport,20,tmp);
		strcat(szpath,"\\SMEIDll.ini");
		GetPrivateProfileString("Port","num","",g_spgate,30,szpath);
		g_iMoPackNum = GetPrivateProfileInt("Num","mo",30,szpath);
		g_iReportPackNum = GetPrivateProfileInt("Num","report",30,szpath);
		if (g_iMoPackNum > 350)
		{
			g_iMoPackNum =350;
		}
		if (g_iMoPackNum < 5)
		{
			g_iMoPackNum = 5;
		}
		if (g_iReportPackNum > 350)
		{
			g_iReportPackNum =350;
		}
		if (g_iReportPackNum < 5)
		{
			g_iReportPackNum = 5;
		}
		//��ȡlinkid�ļ�
		strcpy(tmp,mpath);
		strcat(tmp,LINKID_TMP_FILENAME);
		i= 0;
		fp = fopen(tmp,"rb");
		if(fp)
		{
			size = sizeof(struct Z_UNISERIAL);
			do
			{
				iret = fread(g_UniSerial+i,size,1,fp);
				i++;
			}while(!feof(fp) && i <MAXUNISERIAL && iret >0);
			fclose(fp);
		}

#ifdef USE_SMEIDLL_DLL_
	//	DeleteCriticalSection(&g_csReport);
		memset(szpath,0,sizeof(szpath));
		sprintf(szpath,"%s\\SMEIDll.dll",g_szpath);
		g_handle = LoadLibrary(szpath);//
		if(g_handle == 0x0)	
		{
			//MessageBox(NULL,"δ�ҵ���̬��SMEIDll.dll !","����!",0);
			return false;
		}
		IFInitInterface = (_IFInitInterface)GetProcAddress(g_handle,"IFInitInterface");
		IFExitInterface = (_IFExitInterface)GetProcAddress(g_handle,"IFExitInterface");
		Login_R = (_Login_R)GetProcAddress(g_handle,"Login_R");
		SendSms = (_SendSms)GetProcAddress(g_handle,"SendSms");
		RecvSms = (_RecvSms)GetProcAddress(g_handle,"RecvSms");
		memset(szpath,0,sizeof(szpath));
		sprintf(szpath,"%s\\MWGateWayEx.dll",g_szpath);
		g_handle1 = LoadLibrary(szpath);//
		if(g_handle1 == 0x0)	
		{
			//MessageBox(NULL,"δ�ҵ���̬��SMEIDll.dll !","����!",0);
			return false;
		}
		WriteToDatabase = (_WriteToDatabase)GetProcAddress(g_handle1,"WriteToDatabase");
		ReadFromDatabase = (_ReadFromDatabase)GetProcAddress(g_handle1,"ReadFromDatabase");		
#endif
		break;
	case DLL_PROCESS_DETACH:
		//��������
		strcpy(tmp,mpath);
		strcat(tmp,LINKID_TMP_FILENAME);
		fp = fopen(tmp,"wb+");
		i =0;
		if(fp)
		{
			iret =1;
			while(g_UniSerial[i].phno>0&&iret >0)
			{
				iret = fwrite(g_UniSerial+i,sizeof(struct Z_UNISERIAL),1,fp);
				i++;
			}
			fflush(fp);
			fclose(fp);
		}
		if(g_handle)FreeLibrary(g_handle);
		if(g_handle1)FreeLibrary(g_handle1);
		WSACleanup();
		break;
	default:
		break;
	}
	return true;
}

#endif

enum TWaitFlag
{
	RECV, SEND
};

//*****************************************************//
//              ���������������Ϣ�ṹ
//*****************************************************//
struct sc4Req 
{ 
	char VN; 
	char CD; 
	unsigned short Port; 
	unsigned long IPAddr; 
	char other[1]; 
};

struct sc4Ans 
{ 
	char VN; 
	char CD; 
}; 
  
struct sock5req1 
{ 
	char Ver; 
	char nMethods; 
	char Methods[255]; 
}; 

struct sock5ans1 
{ 
	char Ver; 
	char Method; 
}; 

struct sock5req2 
{ 
	char Ver; 
	char Cmd; 
	char Rsv; 
	char Atyp; 
	char other[1]; 
}; 

struct sock5ans2 
{ 
	char Ver; 
	char Rep; 
	char Rsv; 
	char Atyp; 
	char other[1]; 
}; 

struct authreq 
{ 
	char Ver; 
	char Ulen; 
	char Name[255]; 
	char PLen; 
	char Pass[255]; 
}; 

struct authans 
{ 
	char Ver; 
	char Status; 
};
long GetFileLen()
{
	FILE *fp =NULL;
	char szfilepath[300] = {0};
	char szfilepathold[300] = {0};
	time_t tt;
	struct tm *t;
	long flen = 0;
	sprintf(szfilepath,"%s\\reportfile.txt",g_szpath);
	//EnterCriticalSection( &cs );
	fp = fopen(szfilepath,"r");
	if (fp == NULL)
	{
		return 0;
	}
	fseek(fp,0L,SEEK_END); /* ��λ���ļ�ĩβ */
	flen=ftell(fp); /* �õ��ļ���С */
	fclose(fp);
	if (flen >1024)
	{
		time(&tt);
		t = localtime(&tt);
		sprintf(szfilepathold,"%s\\reportfile-%04d-%02d-%02d-%02d-%02d.txt",g_szpath,t->tm_year+1900,t->tm_mon+1,t->tm_mday,t->tm_hour,t->tm_min);
		rename(szfilepath,szfilepathold);
	}
	return flen;
}
/************************************************************************/
/* д״̬������־�ļ�                                                   */
/************************************************************************/
void WriteRptToFile(char *strRpt)
{
	FILE *fp =NULL;
	char szfilepath[300] = {0};
	sprintf(szfilepath,"%s\\reportfile.txt",g_szpath);
	//EnterCriticalSection( &cs );
	fp = fopen(szfilepath,"a+");
	fprintf(fp,"%s\n",strRpt);
	fclose(fp);
	//LeaveCriticalSection( &cs );
}

/************************************************************************/
/* д״̬������־�ļ�                                                   */
/************************************************************************/
void WriteRptToFileEx(char *strRpt)
{
	FILE *fp =NULL;
	char szfilepath[300] = {0};
	sprintf(szfilepath,"%s\\reportfile.txt",g_szpath);
	remove(szfilepath);	
	//EnterCriticalSection( &cs );
	fp = fopen(szfilepath,"a+");
	fprintf(fp,"%s\n",strRpt);
	fclose(fp);
	//LeaveCriticalSection( &cs );
}

/************************************************************************/
/* ����1��ʾ���ճɹ�������0��ʾû����Ϣ                                  */
/************************************************************************/
int GetMoRptMessage()
{
	//2008-01-23,15:43:34,15986756631,10657302056780408,*,��Ϣ����1
	//2008-01-23,15:43:34,0518153837115735,*,0,DELIVRD
	char sOrgAddr[21 + 1] = {0};
	char sPhone[22] = {0};
	unsigned char sMsgID[50] = {0};
	unsigned char ReportFlag;
	char sTimeStamp[25] = {0};
	char sSubmitdate[20] = {0};
	char sDonedate[20] = {0};
	char sErrorCode[20] = {0};
	char sMsg[1024] = {0};
	int  iRet = 0;
	char szDate[20] = {0};
	char szTime[20] = {0};
	int iTimes = 0;
	char * token=NULL;
	const char Flag[]=" ";
//	char szString[255] = {0};
	while (g_iMoNum<g_iMoPackNum && g_iReportNum < g_iReportPackNum + 20)
	{
		iRet = RecvSms
			(
			sOrgAddr,       //Դ��ַ
			sPhone,         //�ֻ���
			sMsgID,			//���л�״̬�����msgid
			&ReportFlag,	//��Ϣ��־ 0:״̬���� 1:����
			sTimeStamp,     //���յ���ʱ�� ��ʽ��2009-10-28 12:12:12
			sSubmitdate,    //�ύʱ��200910281212
			sDonedate,      //����ʱ��200910281212
			sErrorCode,     //״���������
			sMsg            //������Ϣ
			);
		if (iRet == 0)		//���ճɹ�
		{
			//���ʱ��
			iTimes = 0;
			token = NULL;
			token=strtok(sTimeStamp,Flag);
			while (token != NULL)
			{	
				if (iTimes > 1)
				{
					break;
				}				
				if (iTimes == 0)
				{
					strcpy(szDate,token);
				}
				else
				{
					strcpy(szTime,token);
				}
				token = strtok( NULL,Flag);
				iTimes++;
			}

			if (ReportFlag == 0) //������Ϣ
			{
				sprintf(szMoMsg[g_iMoNum],"%s,%s,%s,%s,*,%s",szDate,szTime,sPhone,sOrgAddr,sMsg);
				//strcpy(strMsg[iMoCount],szString);
				g_iMoNum++;	
			}
			else if (ReportFlag == 1)	//״̬����
			{
				char szMsgIdNew[50] = {0};
				TranSmitMsgID(sMsgID,szMsgIdNew);
				sprintf(szRpt[g_iReportNum],"%s,%s,%s,*,%d,%s",szDate,szTime,szMsgIdNew,strcmp(sErrorCode,"DELIVRD")==0?0:2,sErrorCode);
				g_iReportNum++;
			}
			else
			{
				break;
			}
		}
		else
		{
			break;
		}
	}
	return g_iMoNum;
}

//shaojun add
/************************************************************************/
/* ����1��ʾ���ճɹ�������0��ʾû����Ϣ                                  */
/************************************************************************/
int GetMoRptMessageEx()
{
	//2008-01-23,15:43:34,13800000000,10657302056780408,*,��Ϣ����1
	//2008-01-23,15:43:34,051815315986756631837115735,*,0,DELIVRD
	char sOrgAddr[21 + 1] = {0};
	char sPhone[22] = {0};
	unsigned char sMsgID[50] = {0};
	unsigned char ReportFlag;
	char sTimeStamp[25] = {0};
	char sSubmitdate[20] = {0};
	char sDonedate[20] = {0};
	char sErrorCode[20] = {0};
	char sMsg[1024] = {0};
	char sMsgEx[1024] = {0};
	int  iRet = 0;
	char szDate[20] = {0};
	char szTime[20] = {0};
	char szString[255] = {0};
	int iTimes = 0;
	char * token=NULL;
	const char Flag[]=" ";
	while (g_iMoNumEx<g_iMoPackNum && g_iReportNumEx<g_iReportPackNum + 20)
	{
		iRet = RecvSms
			(
			sOrgAddr,       //Դ��ַ
			sPhone,         //�ֻ���
			sMsgID,			//���л�״̬�����msgid
			&ReportFlag,	//��Ϣ��־ 0:״̬���� 1:����
			sTimeStamp,     //���յ���ʱ�� ��ʽ��2009-10-28 12:12:12
			sSubmitdate,    //�ύʱ��200910281212
			sDonedate,      //����ʱ��200910281212
			sErrorCode,     //״���������
			sMsg            //������Ϣ
			);
		if (iRet == 0)		//���ճɹ�
		{
			//���ʱ��
			iTimes = 0;
			token = NULL;
			token=strtok(sTimeStamp,Flag);
			while (token != NULL)
			{	
				if (iTimes > 1)
				{
					break;
				}
				if (iTimes == 0)
				{
					strcpy(szDate,token);
				}
				else
				{
					strcpy(szTime,token);
				}
				token = strtok( NULL,Flag);
				iTimes++;
			}

			if (ReportFlag == 0) //������Ϣ
			{
				//����Ϣ���ݽ��м��ܴ���
				sMsg[200] = '\0';
				srcStrTohexChr(sMsg,sMsgEx);
				if (g_iMoNumEx != 0)
				{
					sprintf(szString,";%s,%s,%s,%s,*,%s",szDate,szTime,sPhone,sOrgAddr,sMsgEx);
				}
				else
				{
					sprintf(szString,"%s,%s,%s,%s,*,%s",szDate,szTime,sPhone,sOrgAddr,sMsgEx);
				}
				strcat(szMoMsgEx,szString);
				g_iMoNumEx++;	
			}
			else if (ReportFlag == 1)	//״̬����
			{
				char szMsgIdNew[50] = {0};
				TranSmitMsgID(sMsgID,szMsgIdNew);
				if (g_iReportNumEx != 0)
				{
					sprintf(szString,";%s,%s,%s,*,%d,%s",szDate,szTime,szMsgIdNew,strcmp(sErrorCode,"DELIVRD")==0?0:2,sErrorCode);
				}
				else
				{
					sprintf(szString,"%s,%s,%s,*,%d,%s",szDate,szTime,szMsgIdNew,strcmp(sErrorCode,"DELIVRD")==0?0:2,sErrorCode);
				}
				strcat(szRptEx,szString);
				g_iReportNumEx++;
			}
			else
			{
				break;
			}
		}
		else
		{
			break;
		}
	}
	return g_iMoNumEx+g_iReportNumEx;
}
//***************************************************************//
//                    ȷ�ϵ绰�����Ƿ�������
//***************************************************************//
bool IsNumeric(const char* pszMobi)
{
	char *p=(char *)pszMobi;
	if (p == NULL)
		return false;
	while (*p != 0)
	{
		if (*p < 48 || *p > 57)
			return false;
		p++;
	}
	return true;
}

//***************************************************************//
//             ��ʵ�绰�������������б��Ƿ���ȷ
//***************************************************************//
int  TelsCheck(const char* pTels,int iTelNum)
{
	char * token=NULL;
	int iNumber=0;
	int iret=1;
	char *temp=NULL;
	const char cFlag[]=",";
	int kk=0;
	if (pTels==NULL||iTelNum==0) {
		return -15;
	}
	temp=(char*)malloc(strlen(pTels)+1);
	if (temp==NULL) {
		return -10;
	}
	memset(temp,0,sizeof(temp));
	kk=strlen(pTels);
	memcpy(temp,pTels,strlen(pTels));
	temp[kk]='\0';
	token=strtok(temp,cFlag);
	while (token != NULL) {
		iNumber++;
		if (!(IsNumeric(token))) {
			iret=-11;
			break;
		}
		if (strlen(token)>15) {
			iret=-12;
			break;
		}
		token = strtok( NULL,cFlag);
	}
	if (iret==-11||iret==-12) {
		free(temp);
		token=NULL;
		temp=NULL;
		return iret;
	}
	if (iNumber!=iTelNum) {
		iret=-13;
	}
	else if (iNumber>1000) {
		iret=-14;
	}
	free(temp);
	token=NULL;
	temp=NULL;
	return iret;
}

//***************************************************************//
//                  ���ܷ�����Ϣ���ݺ���
//***************************************************************//
void srcStrTohexChr(const char* strMsg,char* retstr)
{
	char strTemp[3]={0};
	char strASCII[1024]={0},temp[1000]={0};
	int len,i;
	if (strMsg==NULL) {
		return;
	}
	len=strlen(strMsg);
    for(i = 0; i<len; i++)
	{		
		sprintf(strTemp,"%02X", strMsg[i]&0xff);		
		strcat(strASCII,strTemp);
	}
	len=strlen(strASCII);
	strASCII[len]='\0';
	memcpy(retstr,strASCII,len);
	retstr[len]='\0';
	return;
} 

//***************************************************************//
//                  ���ܺ���
//***************************************************************//
char* hexChrTosrcStr(const char* strMsg,char* retstr)
{	
	char strTemp[3]={0},strTemp1[3]={0},temp[1000]={0};
	char strASCII[255]={0};
	int i=0,ihex=0;
	int ilen=strlen(strMsg)/2;
	if (strMsg==NULL) {
		return NULL;
	}
	if(strstr(strMsg,"\r\n")>0){ilen=strlen(strMsg)/2 - 1;}//ȥ��β���Ļس��ͻ���
    for(i = 0; i<ilen*2; i++)
	{
		memset(strTemp1,0,3);
		sprintf(strTemp1,"%c%c",strMsg[i],strMsg[i+1]);
		i++;
		sscanf(strTemp1, "%x", &ihex);
		sprintf(strTemp, "%c", ihex);
		strASCII[(i+1)/2-1]=strTemp[0];		
	}
	strASCII[ilen]='\0';
	strcpy(retstr,strASCII);
	return retstr;
}


//***************************************************************//
//                  ������ܷ�����Ϣ���ݺ���
//***************************************************************//
char* convertMOMsg(const char* srcMsg,char* retStr)
{
	char srctemp[255]={0},strTemp[255]={0};
	char *p=NULL;
	int i=4;
	if(srcMsg==0 || retStr==0){return 0;}
	p=strstr(srcMsg,",");
	while (i>0) {
		if(p==0)return 0;
		p=strstr(p+1,",");
		i--;
	}
	strncpy(srctemp,srcMsg,p-srcMsg+1);
	strcat(srctemp,hexChrTosrcStr(p+1,strTemp));
	strcpy(retStr,srctemp);
	p=NULL;
	return retStr;	
}

//***************************************************************//
//                  ���HTTPͷ����
//***************************************************************//
void MakeHttpHead(const char* szTcpCmd, char* szHttpCmd)
{	
	char* TempHttpbuff=NULL;
	char buf[100]={0};
	int ilen=0;
	TempHttpbuff = (char*)malloc(1024 + strlen(szTcpCmd));
	if (TempHttpbuff==NULL) {
		return;
	}
	memset(TempHttpbuff,0,sizeof(TempHttpbuff));
	if(0 == httpproxy)
	{//ֱ����http/tcp
	sprintf( TempHttpbuff,"POST /kfhttpgate.asp?kfline=%s HTTP/1.1\r\n%s%s%s%s%s",
		szTcpCmd,
		"Accept: image/gif, image/x-xbitmap, image/jpeg, image/pjpeg,\
application/vnd.ms-excel, application/msword,application/x-shockwave-flash, */*\r\n\
Accept-Language: zh-cn\r\nAccept-Encoding: gzip, deflate\r\n\
User-Agent: Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.2; \
Mister X; .NET CLR 1.1.4322; .NET CLR 2.0.50727)\r\n",
"Host: ",Server,"\r\n",
"Connection: Keep-Alive\r\n\r\n");
	}
	else
	{//��http����/tpc
	if(strlen(proxyEncode)>0)
	{
		sprintf(buf,"Proxy-Authorization: Basic %s\r\n",proxyEncode);
	}
	sprintf( TempHttpbuff,"POST http://%s/kfhttpgate.asp?kfline=%s HTTP/1.1\r\n%s%s%s%s%s%s",
		Server,
		szTcpCmd,
		"Accept: image/gif, image/x-xbitmap, image/jpeg, image/pjpeg,\
application/vnd.ms-excel, application/msword,application/x-shockwave-flash, */*\r\n\
Accept-Language: zh-cn\r\nAccept-Encoding: gzip, deflate\r\n\
User-Agent: Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.2; \
Mister X; .NET CLR 1.1.4322; .NET CLR 2.0.50727)\r\nConnection: Keep-Alive\r\n",
"Host: ",Server,"\r\n",buf,"\r\n");
	}
	strcpy(szHttpCmd,TempHttpbuff);
	free(TempHttpbuff);
	TempHttpbuff=NULL;
}

//***************************************************************//
//                  ȥ��HTTPͷ����
//***************************************************************//
bool GetHttpRet(const char *pStr,char* szRet)
{
//	char* result=NULL;
	char *pt1=NULL, *pt2=NULL;
	int iLen;
//	memset(szRet,0,sizeof(szRet));
	pt1 = strstr(pStr, "<resultline>");
	pt2 = strstr(pStr, "</resultline>");
	if (pStr==NULL) {
		return false;
	}
	if (pt1 == NULL || pt2 == NULL)
	{
		return false;
	}
	pt1 +=12;
	*pt2 = '\0';
	iLen=(pt2-pt1)-1;
//	result=(char*)malloc(iLen+1);
	strncpy(szRet,pt1,iLen);
	szRet[iLen]='\0';
//  free(result);
//	result=NULL;
	pt1=NULL;
	pt2=NULL;
	return true;
} 
//************************************//
//��ֵ�˿�ת�ͷ�
//************************************//
void SaveUnicomSerialNo(char *csPhone,char *csSerial)//������ͨ���к�
{
	char tmp[30]={0};
	long phno=0;
	int iDay=0;
	time_t t;
	struct tm *ptm;
	int i;
	if(strlen(csPhone)!=11) return;
	//if(atoi(csPhone.Left(3))>=135) return;
	strcpy(tmp,csPhone+2);
	phno = atol(tmp);

	time(&t);
	ptm = localtime(&t);
	iDay = ptm->tm_mday;
	//find, delete , finded save return
	for(i=0;i<g_UniSerPos;i++){
		if(g_UniSerial[i].day==iDay){
			if(g_UniSerial[i].phno==phno){
				memset(tmp,0,sizeof(tmp));
				if(strlen(csSerial) >= 20)
					strncpy(tmp,csSerial,20);
				else
					strcpy(tmp,csSerial);
				sprintf(g_UniSerial[i].szSerial,"%s",tmp);
				return;
			}
		}
		while(g_UniSerial[i].day!=iDay){
			g_UniSerPos--;
			if(i<g_UniSerPos){
				memcpy(&g_UniSerial[i],&g_UniSerial[g_UniSerPos],sizeof(struct Z_UNISERIAL));
			} else {
				break;
			}
		}
	}

	//not find save return;
	if(g_UniSerPos<MAXUNISERIAL){
		g_UniSerial[g_UniSerPos].day=iDay;
		g_UniSerial[g_UniSerPos].phno=phno;
		//g_UniSerial[g_UniSerPos].serial=atol(csSerial);
		memset(tmp,0,sizeof(tmp));
		if(strlen(csSerial) >= 20)
			strncpy(tmp,csSerial,20);
		else
			strcpy(tmp,csSerial);
		sprintf(g_UniSerial[g_UniSerPos].szSerial,"%s",tmp);
		g_UniSerPos++;
	}
}

char *GetUnicomSerialNo(char *csPhone) //ȡ����ͨ����ƥ����
{
	static char csResult[36]={0};
	char tmp[30]={0};
	long phno;
	time_t t;
	struct tm *ptm;	
	int iDay;
	int i;
	strcpy(tmp,csPhone+2);
	phno = atol(tmp);

	time(&t);
	ptm = localtime(&t);
	iDay = ptm->tm_mday;
	for(i=0;i<g_UniSerPos;i++){
		if(g_UniSerial[i].day==iDay){
			if(g_UniSerial[i].phno==phno){
				//csResult.Format("%ld",g_UniSerial[i].serial);
				strcpy(csResult,g_UniSerial[i].szSerial);
				if(strlen(csResult)==0) csResult[0]='\0';
				break;
			}
		}
		while(g_UniSerial[i].day!=iDay){
			g_UniSerPos--;
			if(i<g_UniSerPos){
				memcpy(&g_UniSerial[i],&g_UniSerial[g_UniSerPos],sizeof(struct Z_UNISERIAL));
			} else {
				break;
			}
		}
	}	
	return csResult;
}
//***************************************************************//
//                  Base64���뺯��
//***************************************************************//
unsigned char* Base64Encode(const char *str,int length)   
{ 
	static char   base64_table[]={
		'A','B','C','D','E','F','G','H','I','J','K','L','M',       
		'N','O','P','Q','R','S','T','U','V','W','X','Y','Z',       
		'a','b','c','d','e','f','g','h','i','j','k','l','m',       
		'n','o','p','q','r','s','t','u','v','w','x','y','z',       
		'0','1','2','3','4','5','6','7','8','9','+','/','\0'       
	};   
	static char base64_pad='=';       
	unsigned const char *current=(unsigned const char*)str;
	int i=0;       
	unsigned char *result=(unsigned char*)malloc(((length+3-length%3)*4/3+1)*sizeof(char));       
	while(length>2){/*keep going until we have less than 24 bits*/     
		result[i++]=base64_table[current[0]>>2];   
		result[i++]=base64_table[((current[0]& 0x03)<<4)+(current[1]>>4)];       
		result[i++]=base64_table[((current[1]& 0x0f)<<2)+(current[2]>>6)];       
		result[i++]=base64_table[current[2]& 0x3f];       
		current+= 3;       
		length-= 3;  /*we just handle 3 octets of data*/       
	}       
	/*now deal with the tail end of things*/       
	if(length !=0){       
		result[i++] = base64_table[current[0]>> 2];       
		if(length>1){       
			result[i++]=base64_table[((current[0]& 0x03)<<4)+(current[1]>>4)];       
			result[i++]=base64_table[(current[1]&  0x0f)<<2];
			result[i++]=base64_pad;     
		}     
		else{
	  		result[i++]= base64_table[(current[0]& 0x03) <<4];
			result[i++]= base64_pad;     
			result[i++]= base64_pad;     
		}     
	}     
	result[i]='\0';
	return result;     
}     

//***************************************************************//
//                  socket���շ��ͺ���
//***************************************************************//
int WaitSomeTime(SOCKET fd, int sec, enum TWaitFlag flags)//flags==READ ���ȴ�
{
	fd_set rwset;
	struct timeval timeout;
	int iret;
	char buffer[10]={0};
	FD_ZERO(&rwset);
	FD_SET(fd, &rwset);
	timeout.tv_sec = sec;
	timeout.tv_usec = 0;  
	if (flags == RECV)
	{
		iret=(select(fd+1, &rwset, NULL, NULL, &timeout));
	}
	else if (flags == SEND)
	{ 
		iret=(select(fd+1, NULL, &rwset, NULL, &timeout));
	}
	else
	{
		iret=-1; 
	}
	return iret;
}

//***************************************************************//
//                 �����û����ͺ���
//***************************************************************//
int __stdcall SetUserType(const char* Os,int SoftType,const char* Version,const char* sPwd)
{
	int iret = -3;
	if (Os==NULL||Version==NULL||sPwd==NULL) {
		return -1;
	}
	if (strlen(Os)!=5||strlen(Version)!=4) 
	{
		iret =-2;
		return iret;
	}
	switch(SoftType) 
	{
	case 0:
		break;
	case 1:
		if (strcmp(sPwd,"rszgsw4_MW")==0) 
		{			
			memset(g_Os,0,sizeof(g_Os));
			memcpy(g_Os,Os,strlen(Os));
			memset(g_sVersion,0,sizeof(Version));
			memcpy(g_sVersion,Version,strlen(Version));
			g_iCmdType=24;
			g_SoftType=SoftType;
			iret = 0;
		}
		break;
	case 2:
		if (strcmp(sPwd,"wsjqwh5_MG")==0) {
			memset(g_Os,0,sizeof(g_Os));
			memcpy(g_Os,Os,strlen(Os));
			memset(g_sVersion,0,sizeof(Version));
			memcpy(g_sVersion,Version,strlen(Version));
			g_iCmdType=19;
			g_SoftType=SoftType;
			iret = 0;
		}
		break;
	default:
		break;
	}
	return iret;
}

//***************************************************************//
//                 HTTP���Ӻ���
//***************************************************************//
int __stdcall MongateConnectHttp(const char* pszIP, int iPort, 
  const char* pszAccount, const char* pszPwd)
{
	char buf[1024]={0},httpbuf[1025]={0},reg[100]={0};
	int cbRecv = 0;
	int cbSend = 0;
	struct hostent* pHostInfo = NULL;
	struct sockaddr_in svrAddr;
	SOCKET sock = 0;
	g_ConnectType=1;
	//�������
	if (pszIP == NULL || pszAccount == NULL || pszPwd == NULL)
		return -1;

	if (strlen(pszAccount) > 32 || strlen(pszPwd) > 32)
		return -2;//�Ƿ����ʺŻ�����
	strcpy(Server,pszIP);
	httpproxy = 0;
	strcpy(proxyEncode,"");
// 	sprintf(reg,"+%s+%d+",pszAccount,g_iCmdType);
// 	//srcStrTohexChr(reg,buf);
// 	MD5String(buf,reg);
// 	memset(reg,0,33);
// 	strncpy(reg,EncodeID+18,32);
// 	if(strcmp(buf,reg) != 0)
// 	{//δ��Ȩ
// 		return 0;
// 	}
// 	sprintf(reg,",%d,%s,%s,",g_iCmdType,"888888",pszAccount);
// //	srcStrTohexChr(buff,buf);
// 	MD5String(buf,reg);
// 	memset(reg,0,33);
// 	strncpy(reg,EncodeID+115,32);
// 	if(strcmp(buf,reg) != 0)
// 	{//δ��Ȩ
// 		return 0;
// 	} 
	//get host infomation and set svrAddr
	pHostInfo = gethostbyname(pszIP);
	if (pHostInfo == NULL)
	{
		pHostInfo = gethostbyaddr(pszIP, strlen(pszIP), AF_INET);
		if (pHostInfo == NULL)
		{
			return -3;//��ȡԶ��������Ϣʧ��,ԭ��:IP����ʧ�ܻ���Ч��IP,�����粻ͨ
		}
	}
	svrAddr.sin_family = AF_INET;
	memcpy(&svrAddr.sin_addr, pHostInfo->h_addr_list[0], pHostInfo->h_length);
	svrAddr.sin_port = htons((unsigned short)iPort);
  
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == -1)
		return -4;//socket����ʧ��
  
	if (connect(sock, (struct sockaddr*)&svrAddr, sizeof(svrAddr)) == -1)
		return -5;//connectʧ��  
	//ZZ�����,20(��������),*,*,*,�������,����汾,����ϵͳ�汾,��½�ʺ�,��
	sprintf(buf,"ZZ%06ld,20,*,*,*,%d,%s,%s,%s,%s",strlen(pszAccount)+strlen(pszPwd)+33,
			g_iCmdType,g_sVersion,g_Os,pszAccount,pszPwd);//��½ָ��
	buf[8] = ',';    
	MakeHttpHead(buf,httpbuf); 
	if (WaitSomeTime(sock, g_iOvertime, SEND) ==1)
	{
		cbSend = send(sock, httpbuf, strlen(httpbuf), 0);
	}
	else{
		return -101;
	}
	if (cbSend < 1)
		return -102;//�ύ����ʧ��
  
	memset(buf, 0, sizeof(buf));
	if (WaitSomeTime(sock, g_iOvertime, RECV) == 1)
		cbRecv = recv(sock, buf, sizeof(buf),0);
	else
		return -103; //����ƽ̨���ص�½��֤��Ϣ��ʱ
	if (cbRecv<1) {
		return -104;
	}
	memset(httpbuf,0,sizeof(httpbuf));
	GetHttpRet(buf,httpbuf);
	if (httpbuf[0] == 'E' && httpbuf[1] == 'R')
		return -10000 - (atoi(httpbuf + 3)); //����ƽ̨������Ϣ   
	else if (httpbuf[0] == 'O' && httpbuf[1] == 'K')
		return sock;
	else
		return -200;//���շ��������س���
}

//***************************************************************//
//                        ��ʣ������
//***************************************************************//
int __stdcall MongateQueryBalanceHttp(SOCKET sock)
{
	int cbRecv = 0;
	int cbSend = 0;
	char* p = NULL,*p1= NULL;
	char buf[1024]={0},httpbuf[1024]={0};  
	if (sock<=0) {
		return -198;
	}
	if (WaitSomeTime(sock, g_iOvertime, SEND) ==1)
	{
		MakeHttpHead("ZZ000011,28",httpbuf);  
		cbSend = send(sock, httpbuf, strlen(httpbuf), 0);
		if (cbSend<1) {
			return -102;
		}
	}
	else
		return -101;//��ʱ
  
	if (WaitSomeTime(sock, g_iOvertime, RECV) ==1){
		cbRecv = recv(sock, buf, sizeof(buf), 0);
		if (cbRecv < 1 )
			return -104;//���ͻ��߽���ʧ��  
	}
	else
		return -103;//��ʱ  
	
	memset(httpbuf,0,sizeof(httpbuf));
	GetHttpRet(buf,httpbuf);  
	memset(buf,0,sizeof(buf));
	strcpy(buf,httpbuf);      
	if (buf[1] == 'K' && buf[0] == 'O')
	{
		char* token=NULL;
		const char cFlag[]=",";
		int iFlag=0;
		int iret =0;
		token=strtok(buf,cFlag);
		while(token != NULL)
		{
			iFlag++;
			switch(iFlag) {
			case 2:
				iret=atoi(token);
				break;
			case 3:
				g_iSendNumber=atoi(token);
				break;
			default:
				break;
			}
			token = strtok( NULL,cFlag);			
		}
		token=NULL;
		return iret;
	}
	else if (buf[0] == 'E' && buf[1] == 'R')
		return -10000 - atoi(buf+3);//
	else
		return -200;
}

//***************************************************************//
//                      ��ѯ�Ѿ�ʹ�õ�����
//***************************************************************//
int __stdcall MongateQueryUsedHttp(SOCKET sock)
{
	return g_iSendNumber;
}

//***************************************************************//
//                      ����SOCKET�Ƿ���Ч
//***************************************************************//
int __stdcall MongateTestConnHttp(SOCKET sock)
{
	int cbRecv = 0;
	int cbSend = 0;
	char* p1 = NULL;
	char* p2 = NULL;
	char buf[1024]={0},httpbuf[1025]={0};
	long lnow=time(NULL);
	if (sock<=0) {
		return -198;
	}
	if(g_lastquery==0) 
		g_lastquery=lnow;
	else{
		if(lnow-g_lastquery<30){
			return -300;
		}else{
			g_lastquery=lnow;
		}
	}
	if (WaitSomeTime(sock, g_iOvertime, SEND) ==1)
	{    
		MakeHttpHead("ZZ000018,100,*,*,*",httpbuf);  
		cbSend = send(sock, httpbuf, strlen(httpbuf), 0);
		if (cbSend < 1)
			return -102;//���ͻ��߽���ʧ�� 
	}
	else
		return -101;  
	if (WaitSomeTime(sock, g_iOvertime, RECV) ==1)
	{
		cbRecv = recv(sock, buf, sizeof(buf), 0);
		if (cbRecv < 1 )
			return -104;//���ͻ��߽���ʧ��
	}
	else
		return -103;
	  
	buf[cbRecv]='\0';
	memset(httpbuf,0,sizeof(httpbuf));
	GetHttpRet(buf,httpbuf);  
	memset(buf,0,sizeof(buf));
	strcpy(buf,httpbuf);    
	if (buf[1] == 'K' && buf[0] == 'O')  
		return 1;
	else if (buf[0] == 'E' && buf[1] == 'R')
		return -10000 - atoi(buf+3);
	else
		return -200;
}

//***************************************************************//
//                     �޸������ʻ�����HTTP
//***************************************************************//
int __stdcall MongateChangePwdHttp(SOCKET sock, const char* pszAccount,
  const char* pszOldPwd, const char* pszNewPwd)
{
	int cbRecv = 0;
	int cbSend = 0;
	char buf[1024]={0},httpbuf[1025]={0};
	if (sock<=0) {
		return -198;
	}
	if (pszAccount == NULL || pszOldPwd == NULL || pszNewPwd == NULL)
		return -1;
	if (strlen(pszOldPwd) > 6 || strlen(pszNewPwd) > 6)
		return -2; //�û��������볤�ȷǷ�
	//ZZ�����,29,*(����),*(����),�û��ʺ�,������,������.
	sprintf(buf + 8, ",29,*,*,%s,%s,%s", pszAccount, pszOldPwd, pszNewPwd);
	sprintf(buf, "ZZ%06d", strlen(buf + 8) + 8);
	buf[8] = ',';  

	if (WaitSomeTime(sock, g_iOvertime, SEND) ==1){    
		MakeHttpHead(buf,httpbuf);  
		cbSend = send(sock, httpbuf, strlen(httpbuf), 0);
		if (cbSend < 1)
			return -102;
	}
	else
		return -101;//


	if (WaitSomeTime(sock, g_iOvertime, RECV) ==1)  
	{
		cbRecv = recv(sock, buf, sizeof(buf), 0);  
		if (cbRecv < 1)
			return -104;
	}
	else
		return -103;

	memset(httpbuf,0,sizeof(httpbuf));
	GetHttpRet(buf,httpbuf);  
	memset(buf,0,sizeof(buf));
	strcpy(buf,httpbuf);    
  
	if (buf[0] == 'O' && buf[1] == 'K')
		return 1;
	else if (buf[0] == 'E' && buf[1] == 'R')
		return -10000 - atoi(buf+3);
	else
		return -200;
}


//***************************************************************//
//                      �ʻ���ֵ����HTTP
//***************************************************************//
int __stdcall MongateRechargeHttp(SOCKET sock, const char* pszCardNo,
                              const char* pszCardPwd)
{
	int cbRecv = 0;
	int cbSend = 0;
	int len = 0;
	char* p1 = NULL;
	char* p2 = NULL;
	char buf[1024]={0},httpbuf[1025]={0};
  
	if (sock<=0) {
		return -198;
	}
	if (pszCardNo == NULL || pszCardPwd == NULL)
		return -1;
	if (strlen(pszCardNo) != 6 || strlen(pszCardPwd) != 6)
		return -2;//��Ч�Ŀ��Ż�����
  
	sprintf(buf + 8, ",%s,%s", pszCardNo, pszCardPwd);
	sprintf(buf, "UC%06d", strlen(buf + 8) + 8);
	buf[8] = ',';
  
	if (WaitSomeTime(sock, g_iOvertime, SEND) ==1){
		MakeHttpHead(buf,httpbuf);  
		cbSend = send(sock, httpbuf, strlen(httpbuf), 0);
		if (cbSend < 1)
			return -102; 
	}
	else
		return -101;//���͵ȴ���ʱ

	memset(buf, 0, sizeof(buf));
	if (WaitSomeTime(sock, g_iOvertime, RECV) == 1)
	{
		cbRecv = recv(sock, buf, sizeof(buf), 0);
		if (cbRecv < 1)
			return -104;  
	}
	else
		return -103;//�ȴ�ƽ̨����ȷ����Ϣ��ʱ
	
	buf[sizeof(buf) - 1] = '\0';  
	memset(httpbuf,0,sizeof(httpbuf));
	GetHttpRet(buf,httpbuf);  
	memset(buf,0,sizeof(buf));
	strcpy(buf,httpbuf);
  
	if (buf[1] == 'K' && buf[0] == 'O' && len > 2)
	{
		if ((p1 = strstr(buf, ",")) == NULL){
			p1=NULL;
			return -151;
		}
		if ((p2 = strstr(p1 + 1, ",")) == NULL)
		{
			p1=NULL;
			p2=NULL;
			return -152;
		}
		return atoi(p2 + 1);//����ʣ������
	}
	else if(buf[0] == 'E' && buf[1] == 'R')
		return -10000 - atoi(buf+3);//
	else
		return -200;//	
}

//***************************************************************//
//                      �ͷ����ط��ͺ���HTTP
//***************************************************************//
int __stdcall MongateCsSendSmsHttp(SOCKET sock, const char* pszMobis,
  const char* pszMsg, int iMobiCount, char* pszSN)
{
	int cbRecv = 0;
	int cbSend = 0;
	int iFlag=0;
	char* buf = NULL;
	char* httpbuf=NULL;
	char strMsg[MAX_SMS_LENGTH + 1] = {0};
	char strNMsg[512]={0};
	int ret = -11;
	char tmpstr[1024]={0};
	memset(pszSN,0,sizeof(pszSN));
	if (sock<=0) {
		return -198;
	}
	if (pszMobis == NULL || pszMsg == NULL)
		return -1;
	if (iMobiCount > 1000)
		return -2;
	srcStrTohexChr(pszMsg,strNMsg);
	strNMsg[511] = '\0'; 
	buf = (char*)malloc(712 + iMobiCount * 17);
	httpbuf = (char*)malloc(712 + iMobiCount * 17);
	if ((buf == NULL)||(httpbuf==NULL))  return -10;   
	memset(buf,0,sizeof(buf));
	memset(httpbuf,0,sizeof(httpbuf));
	while (true)
	{
#ifdef CsSPSend61
		iFlag=TelsCheck(pszMobis,iMobiCount);
		if (1==iFlag) {
			sprintf(buf+8,",61,*,*,*,*,0,%d,%d,%s,%s", iMobiCount, strlen(strNMsg), strNMsg, pszMobis);
		}
		else{
			ret=iFlag;
			break;
		}
#else
		if(iMobiCount==1) //�ֿ�������Ⱥ��
		{
			if (!(IsNumeric(pszMobis))) {
				ret =  -11;//
				break;
			}
			if (g_SoftType==1) {
				sprintf(buf+8,",21,*,*,*,*,0,%d,%d,%s,%s", iMobiCount, strlen(strNMsg),strNMsg, pszMobis);
			}
			else{
				sprintf(buf+8,",23,*,*,*,*,0,%d,%d,%s,%s", iMobiCount, strlen(strNMsg),strNMsg, pszMobis);
			}
		}
		else
		{
			iFlag=TelsCheck(pszMobis,iMobiCount);
			if (1==iFlag) {
				if (g_SoftType==1) {
					sprintf(buf+8,",21,*,*,*,*,0,%d,%d,%s,%s", iMobiCount, strlen(strNMsg), strNMsg, pszMobis);
				}
				else{
					sprintf(buf+8,",25,*,*,*,*,0,%d,%d,%s,%s", iMobiCount, strlen(strNMsg), strNMsg, pszMobis);
				}
			}
			else{
				ret=iFlag;
				break;
			}		  		 	  
		}
#endif

		sprintf(buf, "ZZ%06d", strlen(buf+8) + 8);	  
		buf[8] = ','; 
	  	if (WaitSomeTime(sock, g_iOvertime, SEND) == 1)
		{
			MakeHttpHead(buf,httpbuf);//20070413 fq
			cbSend = send(sock, httpbuf, strlen(httpbuf), 0);
			if (cbSend < 1) {
				ret = -102;//
				break;
			}
		}
		else
		{		  
			ret = -101;
			break;
		} 
		if (WaitSomeTime(sock, g_iOvertime, RECV) == 1)
		{
			cbRecv = recv(sock, tmpstr, sizeof(tmpstr), 0); //�������21
			if (cbRecv<1) {
				ret = -104;//
				break;
			}	
		}
		else
		{
			ret = -103;//
			break;
		}
		tmpstr[cbRecv]=0;
		memset(httpbuf,0,sizeof(httpbuf));
		GetHttpRet(tmpstr,httpbuf); 
		memset(buf,0,sizeof(buf));
		strcpy(buf,httpbuf);	  
		if ((buf[0] == 'O' && buf[1] == 'K')||strstr(buf, "OK,") != NULL)
		{
			strcpy(pszSN, buf+3);
			pszSN[strlen(buf+3)-2] = '\0';
			pszSN[strlen(buf+3)-1] = '\0';
			ret = 1;//
			break;
		}
		else if (buf[0] == 'E' && buf[1] == 'R')
		{
			ret = -10000 - (atoi(buf + 3)); //����ƽ̨������Ϣ
			break;
		}
		else
		{
			ret = -200;
			break;
		}
	}
	free(buf); buf = NULL;
	free(httpbuf); httpbuf = NULL;
	return ret;
}

int __stdcall MongateKxtSendHttp(SOCKET sock, const char* pszMobis,
  const char* pszMsg, int iMobiCount, char* pszSN)
{
	int cbRecv = 0;
	int cbSend = 0;
	int iFlag=0;
	char* buf = NULL;
	char* httpbuf=NULL;
	char strMsg[MAX_SMS_LENGTH + 1] = {0};
	char strNMsg[512]={0};
	int ret = -11;
	char tmpstr[1024]={0};
	if (sock<=0) {
		return -198;
	}
	if (pszMobis == NULL || pszMsg == NULL)
		return -1;
	if (iMobiCount > 1000)
		return -2;

	memset(pszSN,0,sizeof(pszSN));
	srcStrTohexChr(pszMsg,strNMsg);
	strNMsg[511] = '\0'; 

	buf = (char*)malloc(712 + iMobiCount * 17);
	httpbuf = (char*)malloc(712 + iMobiCount * 17);
	if ((buf == NULL)||(httpbuf==NULL))  return -10; 
  
	memset(buf,0,sizeof(buf));
	memset(httpbuf,0,sizeof(httpbuf));
	while (true)
	{
		iFlag=TelsCheck(pszMobis,iMobiCount);
		if (1==iFlag) {
			sprintf(buf+8,",21,*,*,*,*,0,%d,%d,%s,%s", iMobiCount, strlen(strNMsg), strNMsg, pszMobis);
		}
		else{
			ret=iFlag;
			break;
		}		  		 	  
		sprintf(buf, "ZZ%06d", strlen(buf+8) + 8);	  
		buf[8] = ',';	    
	  
		if (WaitSomeTime(sock, g_iOvertime, SEND) == 1)
		{
			MakeHttpHead(buf,httpbuf);//20070413 fq
			cbSend = send(sock, httpbuf, strlen(httpbuf), 0);
		}
		else
		{		  
			ret = -101;
			break;
		} 
		if (WaitSomeTime(sock, g_iOvertime, RECV) == 1)
			cbRecv = recv(sock, tmpstr, sizeof(tmpstr), 0); //�������21		  
		else
		{
			ret = -103;//
			break;
		}
		if (cbSend < 1 || cbRecv < 1)
		{
			if (cbSend < 1) {
				ret = -102;//
			}
			else{
				ret = -104;//
			}		  
			break;
		}

		tmpstr[cbRecv]=0;
		memset(httpbuf,0,sizeof(httpbuf));
		GetHttpRet(tmpstr,httpbuf); 
		memset(buf,0,sizeof(buf));
		strcpy(buf,httpbuf);	  
		if ((buf[0] == 'O' && buf[1] == 'K')||strstr(buf, "OK,") != NULL)
		{
			strcpy(pszSN, buf+3);
			pszSN[strlen(buf+3)-2] = '\0';
			pszSN[strlen(buf+3)-1] = '\0';
			ret = 1;//
			break;
		}
		else if (buf[0] == 'E' && buf[1] == 'R')
		{
			ret = -10000 - (atoi(buf + 3)); //����ƽ̨������Ϣ
			break;
		}
		else
		{
			ret = -200;
			break;
		}
	}
	free(buf); buf = NULL;
	free(httpbuf); httpbuf = NULL;
	return ret;
}
//�ͷ�Э�����������Ϣ
int __stdcall MongateCsGetSmsHttp(SOCKET sock, char strMsg[500][255])
{
	int cbRecv = 0;
	int cbSend = 0;
	int iret = 0;
	bool bFlag = true;
	int icount = 0;
	char retbuf[20000]={0},temp[2049] = {0},retbufHttp[20000]={0};
	char httpbuf[1025]={0};
	time_t tCourrent;
	time(&tCourrent);
	if (sock<=0) {
		return -198;
	}
	if(((int)difftime(tCourrent,g_tHistory[1]) < (g_iMOSpace+1))&&(g_iMOSpace!=0))
	{
		return -300;
	}
	else{
		g_tHistory[1]=tCourrent;
	}
	if (WaitSomeTime(sock, g_iOvertime, SEND) == 1){
		MakeHttpHead("ZZ000019,31,2,*,*,*",httpbuf);  
		cbSend = send(sock, httpbuf, strlen(httpbuf), 0);
	}
	else
	{
		return -101;
	}
	if (cbSend<1) {
		return -102;
	}
	while(bFlag){
		memset(temp, 0, sizeof(temp));
		if (WaitSomeTime(sock, g_iOvertime, RECV) == 1)
		{
			cbRecv = recv(sock, temp, 2048, 0);
			if (cbRecv<1) {
				iret = -104;
				bFlag = false;
			}
			temp[cbRecv]=0;
			strcat(retbufHttp,temp);
			if (strstr(retbufHttp,"</kfsms-http>")!=NULL)
			{
				GetHttpRet(retbufHttp,retbuf); 
				if (retbuf[0] == 'E' && retbuf[1] == 'R')
				{
					iret = -10000 - (atoi(retbuf + 3));//����ƽ̨������Ϣ
					bFlag = false;
					break;
				}				
				else if (strstr(retbuf, "OK,0") != NULL)
				{
					char temp[20]={0};
					int iFlag = 0;
					char * token=NULL;
					const char Flag[]=",";
					memcpy(temp,retbuf,20);
					token=strtok(temp,Flag);
					while (token != NULL) {
						iFlag++;					
						if (3==iFlag) {
							g_iMOSpace=atoi(token);
						}
						token = strtok( NULL,Flag);
					}
					iret = 0;//û����Ϣ	
					token=NULL;
					bFlag=false;
				}				
				else if (retbuf[0] == 'Z' || retbuf[1] == 'Z')
				{
					const char cFlag[]=";";
					int iCount=0;
					char* token=NULL;
					token = strtok(retbuf+20, cFlag);
					while( token != NULL )
					{
						char tempMsg[256]={0};
						convertMOMsg(token,tempMsg);
						memcpy(strMsg[iCount],tempMsg,strlen(tempMsg));
						strMsg[iCount][strlen(tempMsg)] = '\0';
						iCount++;
						token = strtok( NULL, cFlag);
					}
					iret=iCount;
					token=NULL;
					bFlag=false;					
				}
				else{
					iret = -10;
					bFlag = false;
				}
			}
			else
			{
				continue;
			}
		}
		else
		{
			iret = -103;
			bFlag = false;
		}		
	}
	return iret;
}


int __stdcall MongateCsGetSmsHttpEx(SOCKET sock, char *strMsg)
{
	int cbRecv = 0;
	int cbSend = 0;
	int iret = 0;
	bool bFlag = true;
	int icount = 0;
	char retbuf[20000]={0},temp[2049] = {0},retbufHttp[20000]={0};
	char httpbuf[1025]={0}; 
	time_t tCourrent;
	time(&tCourrent);
	if (sock<=0) {
		return -198;
	}
	if(((int)difftime(tCourrent,g_tHistory[2]) < (g_iMOSpace+1))&&(g_iMOSpace!=0))
	{
		return -300;
	}
	else{
		g_tHistory[2]=tCourrent;
	}
	if (WaitSomeTime(sock, g_iOvertime, SEND) == 1){
		MakeHttpHead("ZZ000019,31,2,*,*,*",httpbuf);  
		cbSend = send(sock, httpbuf, strlen(httpbuf), 0);
	}
	else
	{
		return -101;
	}
	if (cbSend<1) {
		return -102;
	}
	while(bFlag){
		memset(temp, 0, sizeof(temp));
		if (WaitSomeTime(sock, g_iOvertime, RECV) == 1)
		{
			cbRecv = recv(sock, temp, 2048, 0);
			if (cbRecv<1) {
				iret = -104;
				bFlag = false;
			}
			temp[cbRecv]=0;
			strcat(retbufHttp,temp);
			if (strstr(retbufHttp,"</kfsms-http>")!=NULL)
			{
				GetHttpRet(retbufHttp,retbuf); 
				if (retbuf[0] == 'E' && retbuf[1] == 'R')
				{
					iret = -10000 - (atoi(retbuf + 3));//����ƽ̨������Ϣ
					bFlag = false;
				}
				
				else if (strstr(retbuf, "OK,0") != NULL)
				{
					char temp[20]={0};
					int iFlag = 0;
					char * token=NULL;
					const char Flag[]=",";
					memcpy(temp,retbuf,20);
					token=strtok(temp,Flag);
					while (token != NULL) {
						iFlag++;					
						if (3==iFlag) {
							g_iMOSpace=atoi(token);
						}
						token = strtok( NULL,Flag);
					}
					token=NULL;
					iret = 0;//û����Ϣ				
					bFlag=false;
				}
				
				else if (retbuf[0] == 'Z' || retbuf[1] == 'Z')
				{
					int iCount=0;
					int ilen=0;
					char *p;
					iCount=atoi(retbuf+16);
					p=strstr(retbuf+16,",");
					ilen=strlen(p+1);
					memcpy(strMsg,p+1,strlen(p+1));
					strMsg[ilen-2]='\0';
					iret=iCount;
					bFlag=false;					
				}
				else{
					iret = -10;
					bFlag = false;
				}
			}
			else
			{
				continue;
			}
		}
		else
		{
			iret = -103;
			bFlag = false;
		}		
	}
	return iret;
}

int __stdcall MongateCsGetStatusReportHttp(SOCKET sock, char strMsg[500][255])
{
	int cbRecv = 0;
	int cbSend = 0;
	int iret = 0;
	bool bFlag = true;
	int icount = 0;
	char retbuf[20000]={0},temp[2049] = {0},retbufHttp[20000]={0};
	char buf[1025]={0},httpbuf[1025]={0}; 
	time_t tCourrent;
	time(&tCourrent);
	if (sock<=0) {
		return -198;
	}
	if(((int)difftime(tCourrent,g_tHistory[3]) < (g_iReportSpace+1))&&(g_iReportSpace!=0))
	{
		return -300;
	}
	else{
		g_tHistory[3]=tCourrent;
	}
	if (WaitSomeTime(sock, g_iOvertime, SEND) == 1){
		MakeHttpHead("ZZ000019,33,1,*,*,*",httpbuf);  
		cbSend = send(sock, httpbuf, strlen(httpbuf), 0);
		if (cbSend<1) {
			iret = -102;
			bFlag = false;
		}
	}
	else
	{
		iret = -101;
		bFlag = false;
	}	
	while(bFlag){
		memset(temp, 0, sizeof(temp));
		if (WaitSomeTime(sock, g_iOvertime, RECV) == 1)
		{
			cbRecv = recv(sock, temp, 2048, 0);
			if (cbRecv<1) {
				iret = -104;				
			}
			temp[cbRecv]=0;
			strcat(retbufHttp,temp);
			if (strstr(retbufHttp,"</kfsms-http>")!=NULL) 
			{
				GetHttpRet(retbufHttp,retbuf);
				if (retbuf[0] == 'E' && retbuf[1] == 'R')
				{
					iret = -10000 - (atoi(buf + 3));//����ƽ̨������Ϣ
					bFlag = false;
				}				
				if (strstr(retbuf, "OK,0") != NULL)
				{
					char temp[20]={0};
					int iFlag = 0;
					char * token=NULL;
					const char Flag[]=",";
					memcpy(temp,retbuf,20);
					token=strtok(temp,Flag);
					while (token != NULL) {
						iFlag++;					
						if (3==iFlag) {
							g_iReportSpace=atoi(token);
						}
						token = strtok( NULL,Flag);
					}
					iret = 0;//û����Ϣ	
					token=NULL;
					bFlag=false;
				}				
				if (retbuf[0] == 'Z' || retbuf[1] == 'Z')
				{
					const char cFlag[]=";";
					int iCount=0;
					char* token=NULL;
					token = strtok(retbuf+20, cFlag);
					while( token != NULL )
					{
						memcpy(strMsg[iCount],token,strlen(token));
						strMsg[iCount][strlen(token)] = '\0';
						iCount++;
						token = strtok( NULL, cFlag);
					}
					iret=iCount;
					token=NULL;
					bFlag=false;
				}
			}
			else{
				continue;
			}
		}
		else
		{
			iret = -103;
			bFlag=false;
		}			
	}
	return iret;
}

int __stdcall MongateCsGetStatusReportHttpEx(SOCKET sock, char *strMsg)
{
	int cbRecv = 0;
	int cbSend = 0;
	int iret = 0;
	bool bFlag = true;
	int icount = 0;
	char retbuf[20000]={0},temp[2049] = {0},retbufHttp[20000]={0};
	char httpbuf[1025]={0};
	time_t tCourrent;
	time(&tCourrent);
	if (sock<=0) {
		return -198;
	}
	if(((int)difftime(tCourrent,g_tHistory[4]) < (g_iReportSpace+1))&&(g_iReportSpace!=0))
	{
		return -300;
	}
	else{
		g_tHistory[4]=tCourrent;
	}
	if (WaitSomeTime(sock, g_iOvertime, SEND) == 1){
		MakeHttpHead("ZZ000019,33,1,*,*,*",httpbuf);  
		cbSend = send(sock, httpbuf, strlen(httpbuf), 0);
		if (cbSend<1) {
			iret = -102;
			bFlag = false;
		}
	}
	else
	{
		iret = -101;
		bFlag = false;
	}	
	while(bFlag){
		memset(temp, 0, sizeof(temp));
		if (WaitSomeTime(sock, g_iOvertime, RECV) == 1)
		{
			cbRecv = recv(sock, temp, 2048, 0);
			if (cbRecv<1) {
				iret = -104;				
			}
			temp[cbRecv]=0;
			strcat(retbufHttp,temp);
			if (strstr(retbufHttp,"</kfsms-http>")!=NULL) 
			{
				GetHttpRet(retbufHttp,retbuf);
				if (retbuf[0] == 'E' && retbuf[1] == 'R')
				{
					iret = -10000 - (atoi(retbuf + 3));//����ƽ̨������Ϣ
					bFlag = false;
				}
				
				if (strstr(retbuf, "OK,0") != NULL)
				{
					char temp[20]={0};
					int iFlag = 0;
					char * token=NULL;
					const char Flag[]=",";
					memcpy(temp,retbuf,20);
					token=strtok(temp,Flag);
					while (token != NULL) {
						iFlag++;					
						if (3==iFlag) {
							g_iReportSpace=atoi(token);
						}
						token = strtok( NULL,Flag);
					}
					iret = 0;//û����Ϣ	
					token=NULL;
					bFlag=false;
				}				
				if (retbuf[0] == 'Z' || retbuf[1] == 'Z')
				{
					char temp[50]={0};
					const char cFlag[]=",";
					int iCount=0,iNumber=0;
					char* token=NULL;
					memcpy(temp,retbuf,25);
					token = strtok(temp, cFlag);
					while( token != NULL )
					{
						iCount++;
						if (5==iCount) {
							iret=atoi(token);
							break;
						}
						token = strtok( NULL, cFlag);
					}					
					memcpy(strMsg,retbuf+20,strlen(retbuf)-20);
					token=NULL;
					bFlag=false;
				}
			}
			else{
				continue;
			}
		}
		else
		{
			iret = -103;
			bFlag=false;
		}			
	}
	return iret;
}
int __stdcall MongateSetCmdType(int iCmdType) //���õ�½����
{
	g_iCmdType=iCmdType;
	return g_iCmdType;
}

int __stdcall MongateSetOverTime(int iOverTime) //�������糬ʱʱ��
{
	g_iOvertime=iOverTime;
	return g_iOvertime;
}
//�ڴ˴��ı����
//---------------------------------------------------------------------------
// ���� shaojun modify at 11-06
int __stdcall MongateConnect(const char* pszIP, int iPort, 
	const char* pszAccount, const char* pszPwd)
{
	char buf[100] ={0},reg[100]={0};
	int cbRecv = 0;
	int cbSend = 0;
	int iRet = 0;
	char szParam[100] = {0};
	struct hostent* pHostInfo = NULL;
	struct sockaddr_in svrAddr;
	SOCKET sock = 120;
	//�������
	g_ConnectType=0;
	if (pszIP == NULL || pszAccount == NULL || pszPwd == NULL)
		return -1;//��������У�������NULLֵ
	if (strlen(pszAccount) > 32 || strlen(pszPwd) > 32)	

		return -2;//�Ƿ����ʺŻ�����
// 	sprintf(reg,"+%s+%d+",pszAccount,g_iCmdType);//58
// 	//srcStrTohexChr(reg,buf);
// 	MD5String(buf,reg);
// 	memset(reg,0,33);
// 	strncpy(reg,EncodeID+18,32);
// 	if(strcmp(buf,reg) != 0)
// 	{//δ��Ȩ
// 		if(strstr(timeEncry,"\r\n")==NULL)
// 			return 0;
// 	}
// 	sprintf(reg,",%d,%s,%s,",g_iCmdType,"888888",pszAccount);
// //	srcStrTohexChr(buff,buf);
// 	MD5String(buf,reg);
// 	memset(reg,0,33);
// 	strncpy(reg,EncodeID+115,32);
// 	if(strcmp(buf,reg) != 0)
// 	{//δ��Ȩ
// 		if(strstr(timeEncry,"\r\n\r\n")==NULL)
// 			return 0;
// 	} 
	pHostInfo = gethostbyname(pszIP);//�����������ֺ͵�ַ��Ϣ��hostent�ṹָ��
	if (pHostInfo == NULL)
	{
		pHostInfo = gethostbyaddr(pszIP, strlen(pszIP), AF_INET);
		if (pHostInfo == NULL)
		{
		  return -199;//��ȡԶ��������Ϣʧ��,ԭ��:IP����ʧ�ܻ���Ч��IP,�����粻ͨ
		}
	}

#ifdef USE_SMEIDLL_DLL_
	if (strlen(pszAccount) > 6 || strlen(pszPwd) > 32)		
	{
		return -10001;//�Ƿ����ʺŻ�����
	}
	sprintf(szParam,"%s %d 5000",pszIP,iPort);
	iRet = IFInitInterface(5,1,szParam);
	if(iRet == 0 || iRet == 1)		//���ӳɹ�
	{
		iRet = Login_R(pszAccount,pszPwd);
		if (iRet == 0 || iRet == 1)
		{
			return 288;
		}
		else		//��¼ʧ��
		{
			IFExitInterface();
			return -10001;
			//ת������ŷ���
		}
	}
	else
	{
		return -197;
		//ת������ŷ���
	}
	return -197;
#endif
	//����Զ������������
	svrAddr.sin_family = AF_INET;
	memcpy(&svrAddr.sin_addr, pHostInfo->h_addr_list[0], pHostInfo->h_length);
	svrAddr.sin_port = htons((unsigned short)iPort);
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == -1)
		return -198;//socket����ʧ��  
	if (connect(sock, (struct sockaddr*)&svrAddr, sizeof(svrAddr)) == -1)
		return -197;//connectʧ��
	if (WaitSomeTime(sock, g_iOvertime, RECV) == 1)
		cbRecv = recv(sock, buf, sizeof(buf), 0);
	else
		return -196;//connect�ɹ�,�ȴ�ƽ̨��������ȷ����Ϣ��ʱ   
	if (!(buf[0] == 'O' && buf[1] == 'K'))
		return -195;//�ȴ�����������ʧ��
	sprintf(buf,"ZZ%06ld,20,*,*,*,%d,%s,%s,%s,%s",strlen(pszAccount)+strlen(pszPwd)+33,
			g_iCmdType,g_sVersion,g_Os,pszAccount,pszPwd);//��½ָ��
	buf[8] = ','; 
	if (WaitSomeTime(sock, g_iOvertime, SEND) ==1)
	{
		cbSend = send(sock, buf, strlen(buf), 0);
	}
	else{
		return -101;
	}
	if (cbSend < 1)
		return -102;//unexpected error

	memset(buf, 0, sizeof(buf));
	if (WaitSomeTime(sock, g_iOvertime, RECV) == 1)
	{
		cbRecv = recv(sock, buf, sizeof(buf),0);
	}    
	else
	{
		return -103; //����ƽ̨���ص�½��֤��Ϣ��ʱ
	}    
	if(cbRecv<1){
		return -104;
	}
	if (buf[0] == 'E' && buf[1] == 'R'){
		return -10000 - (atoi(buf + 3)); //����ƽ̨������Ϣ
	}
	else if (buf[0] == 'O' && buf[1] == 'K')
		return sock;
	else
		return -200;//unexpected error
}

//---------------------------------------------------------------------------
// ���͵�����Ϣ
int __stdcall MongateSendSms(SOCKET sock, const char* pszMobi,
							 const char* pszMsg)                             
{
	char arrSN[50] = {0}; 
	int iret;
	if (sock<=0) {
		return -198;
	}
	iret = MongateCsSendSms(sock, pszMobi, pszMsg, 1, arrSN);
	return iret;
}

//---------------------------------------------------------------------------
// ����������Ч��
int __stdcall MongateTestConn(SOCKET sock)
{
	int cbRecv = 0;
	int cbSend = 0;
	char buf[50] = {0};
	long lnow=time(NULL);
	if(sock<=0){
		return -200;
	}
	return 1;
// 	if(g_lastquery==0) 
// 		g_lastquery=lnow;
// 	else{
// 		if(lnow-g_lastquery<30){
// //			g_lastquery=lnow;
// 			return -300;
// 		}else{
// 			g_lastquery=lnow;
// 		}
// 	}
	if (WaitSomeTime(sock, g_iOvertime, SEND) ==1)
		cbSend = send(sock, "ZZ000018,100,*,*,*", 18, 0);
	else
		return -101; //�ȴ����ͳ�ʱ
  
	if (WaitSomeTime(sock, g_iOvertime, RECV) ==1)
		cbRecv = recv(sock, buf, sizeof(buf), 0);
	else
		return -103;//�ȴ����ճ�ʱ
  
	if ( cbSend < 1 || cbRecv < 1)
		return -102;//�ȴ�����������ʧ��
  
	buf[sizeof(buf) - 1] = '\0';
	if ((buf[1] == 'K' && buf[0] == 'O'))
		return 1;
	else if (buf[0] == 'E' && buf[1] == 'R')
		return -10000 - atoi(buf+3);
	else
		return -200;//���ӶϿ�
}
//---------------------------------------------------------------------------
// ��ʣ������
int __stdcall MongateQueryBalance(SOCKET sock)
{
	int cbRecv = 0;
	int cbSend = 0;
	char* p = NULL;
	char buf[27]={0};
	if(sock<=0){
		return -198;
	}
	return 9999999;
	if (WaitSomeTime(sock, g_iOvertime, SEND) ==1)
		cbSend = send(sock, "ZZ000011,28", 11, 0);
	else
		return -101;//��ʱ
  
	if (WaitSomeTime(sock, g_iOvertime, RECV) ==1)
		cbRecv = recv(sock, buf, sizeof(buf), 0);
	else
		return -103;//��ʱ
  
	if (cbRecv < 1 || cbSend < 1)
		return -102;//���ͻ��߽���ʧ��
  
	buf[sizeof(buf) - 1]='\0';
	if (buf[0] == 'O' && buf[1] == 'K')
	{
		char* token=NULL;
		const char cFlag[]=",";
		int iFlag=0;
		int iret =0;
		token=strtok(buf,cFlag);
		while(token != NULL)
		{
			iFlag++;
			switch(iFlag) {
			case 2:
				iret=atoi(token);
				break;
			case 3:
				g_iSendNumber=atoi(token);
				break;
			default:
				break;
			}
			token = strtok( NULL,cFlag);			
		}
		token=NULL;
		return iret;
	}
	else if (buf[0] == 'E' && buf[1] == 'R')
		return -10000 - atoi(buf+3);//
	else
		return -200;
}
//---------------------------------------------------------------------------
// ��ѯ�Ѿ�ʹ�õ�����
int __stdcall MongateQueryUsed(SOCKET sock)
{
	return g_iSendNumber;
}
//---------------------------------------------------------------------------
// �ʻ���ֵ
int __stdcall MongateRecharge(SOCKET sock, const char* pszCardNo,
                              const char* pszCardPwd)
{
	int cbRecv = 0;
	int cbSend = 0;
	int len = 0;
	char* p1 = NULL;
	char* p2 = NULL;
	char buf[50] = {0};  
	if(sock<=0){
		return -198;
	}
	if (pszCardNo == NULL || pszCardPwd == NULL)
		return -1;
	if (strlen(pszCardNo) != 6 || strlen(pszCardPwd) != 6)
		return -2;//��Ч�Ŀ��Ż�����
  
	sprintf(buf + 8, ",%s,%s", pszCardNo, pszCardPwd);
	sprintf(buf, "UC%06d", strlen(buf + 8) + 8);
	buf[8] = ',';
  
	if (WaitSomeTime(sock, g_iOvertime, SEND) ==1)
		cbSend = send(sock, buf, strlen(buf), 0);
	else
		return -101;//���͵ȴ���ʱ
	memset(buf, 0, sizeof(buf));
	if (WaitSomeTime(sock, g_iOvertime, RECV) == 1)
		cbRecv = recv(sock, buf, sizeof(buf), 0);
	else
		return -103;//�ȴ�ƽ̨����ȷ����Ϣ��ʱ
	if (cbRecv < 1 || cbSend < 1)
		return -101;
  
	buf[sizeof(buf) - 1] = '\0';
	len = strlen(buf);
	if (buf[len - 1] == 'K' && buf[len - 2] == 'O' && len > 2)
	{
		if ((p1 = strstr(buf, ",")) == NULL)
		{
			p1=NULL;
			return -101;
		}
		if ((p2 = strstr(p1 + 1, ",")) == NULL)
		{
			p1=NULL;
			p2=NULL;
			return -102;
		}
		else
		{
			*p2 = '\0';
		}    
		return atoi(p1 + 1);//����ʣ������
	}
	else if(buf[0] == 'E' && buf[1] == 'R')
		return -10000 - atoi(buf+3);//
	else
		return -200;//
}

//---------------------------------------------------------------------------
// �޸�����
int __stdcall MongateChangePwd(SOCKET sock, const char* pszAccount,
  const char* pszOldPwd, const char* pszNewPwd)
{
	int cbRecv = 0;
	int cbSend = 0;
	char buf[1024] = {0};
	if(sock<=0){
		return -198;
	}
	if (pszAccount == NULL || pszOldPwd == NULL || pszNewPwd == NULL)
	{
		return -1;
	}
	if (strlen(pszOldPwd) > 6|| strlen(pszNewPwd) > 6)
	{
		return -2; //�û��������볤�ȷǷ�
	}
	sprintf(buf + 8, ",29,*,*,%s,%s,%s", pszAccount, pszOldPwd, pszNewPwd);
	sprintf(buf, "ZZ%06d", strlen(buf + 8) + 8);
	buf[8] = ',';

	if (WaitSomeTime(sock, g_iOvertime, SEND) ==1)
		cbSend = send(sock, buf, strlen(buf),0);
	else
		return -101;//
	memset(buf,0,sizeof(buf));
	if (WaitSomeTime(sock, g_iOvertime, RECV) ==1)
		cbRecv = recv(sock, buf, sizeof(buf), 0);
	else
		return -103;
  
	if (cbRecv < 1 || cbSend < 1)
		return -102;
  
	if (buf[0] == 'O' && buf[1] == 'K')
		return 1;
	else if (buf[0] == 'E' && buf[1] == 'R')
		return -10000 - atoi(buf+3);
	else
		return -200;
}
//---------------------------------------------------------------------------
// �Ͽ�����
void __stdcall MongateDisconnect(SOCKET sock)
{
	char recvbuf[500 * 255] = {0};
	int i = 0;
	if (g_iReportNum > 0)
	{
		//д�ļ�����
		for (i=0;i<g_iReportNum;i++)
		{
			strcat(recvbuf,szRpt[i]);
			if (i<g_iReportNum-1)
			{
				strcat(recvbuf,";");
			}
			memset(szRpt[i],0,255);
		}
		WriteToDatabase(recvbuf,g_iReportNum);
		g_iReportNum = 0;
	}

	if (g_iReportNumEx > 0)
	{
		//д�ļ�����
		WriteToDatabase(szRptEx,g_iReportNumEx);
		memset(szMoMsgEx,0,sizeof(0));
		g_iReportNumEx = 0;
	}
	IFExitInterface();
	return;
#ifdef WIN32
  shutdown(sock, 2);
  closesocket(sock);  
#else
  close(sock);
#endif
}
//---------------------------------------------------------------------------
// ������Ϣ
int __stdcall MongateVasGetSms(SOCKET sock, char strMsg[500][255])
{
	int cbRecv = 0;
	int cbSend = 0;
	int len = 0;
	int count = 0;
	char buf[512] = {0},tmp[512],hexmsg[512],*p=0x0;
	char bufphone[15]={0},bufcode[20]={0};
	int buflen1,buflen2;
	int i;
	if(sock<=0){
		return -198;
	}
	if (WaitSomeTime(sock, g_iOvertime, SEND) == 1)
		cbSend = send(sock, "ZD000010,0", 10, 0);
	else
		return -101;

	memset(buf, 0, sizeof(buf));
	if (WaitSomeTime(sock, g_iOvertime, RECV) == 1)
		cbRecv = recv(sock, buf, 9, 0);
	else
		return -103;
	if (cbSend<0||cbRecv<0) {
		return -102;
	}   
	if (buf[0] == 'E' && buf[1] == 'R')
		return -10000 - (atoi(buf + 3));//����ƽ̨������Ϣ
  
	if (strstr(buf, "OK,0") != NULL)
		return 0;//û����Ϣ
    
	if (buf[0] != 'Z' || buf[1] != 'D')
		return -10;

	for (count = 0; count < 501; count++) //��count<11��������10����ΪҪ��ED����
	{
		if (count != 0)
		{
			memset(buf, 0, sizeof(buf));
			if (WaitSomeTime(sock, g_iOvertime, RECV) == 1)
				cbRecv = recv(sock, buf, 9, 0);
			else
				return -103;//
		}  
		if ( (buf[0] == 'E' && buf[1] == 'D') || count >= 1000)
			break;

		buf[8] = '\0';    //ȥ������
		len = atoi(buf+2);//�õ���Ϣ����
		if (WaitSomeTime(sock, g_iOvertime, SEND) ==1)
			cbRecv = recv(sock, buf, len - 9, 0);
		else
			return -103;
		buf[strlen(buf)-1]='\0';//ȥ���س��뻻��
		strcpy(tmp, buf);//���Ƶ�������,������λ�������Ϣ����
		p = tmp;
		i=0;
		buflen1 = buflen2 = 0;
		while(*p && i < 4)//�ҵ���4������
		{
			if(i == 2 && *p != ','){//���ҵ�2�����ţ����ƺ���
				bufphone[buflen1++]=*p;//copy phone
			}
			if(*p == ','){i++;}
			p++;
		}
		//ZD000067,2008-10-09,11:30:33,13164724370,91601858,0,36482612,����
		//����

		*p=0x0;//�ж��ַ���
		strcpy(strMsg[count],tmp);
		strcat(strMsg[count],"*,");
		while(*(++p) && i < 6)//�ҵ���6������
		{
			if(i == 5 && *p != ',')//���ҵ�5�����ţ�����ƥ����
			{
				bufcode[buflen2++]=*p;
			}
			if(*p == ','){i++;}
		}
		SaveUnicomSerialNo(bufphone,bufcode);

		memset(hexmsg,0,sizeof(hexmsg));
		srcStrTohexChr(p,hexmsg);//����
		strcat(strMsg[count],hexmsg);
	}
	return count;
}
char * EncryptString(char *code)
{///����������ܴ�
	//ͨ��ʱ����漴��Ȩ
	SYSTEMTIME t;
	int rnd = rand();
	GetSystemTime(&t);
	sprintf(timeEncry,"%4d-%2d-%2d;%2d:%2d:%2d;%d;%d",t.wYear,t.wMonth,t.wDay,t.wHour,t.wMinute,t.wSecond,t.wMilliseconds,rnd);
	strcpy(code,timeEncry);
	return code;
}
int CheckEncryptString(char *code)
{//��֤�����ַ���
	char buf[1000]={0};
	if(strcmp(timeEncry,"")==0)return -806;
	MD5String(buf,timeEncry);
	if(strcmp(code,buf)==0)
	{
		strcat(timeEncry,"\r\n\r\n");
		return 1;
	}
	else return 0;
}
//shaojun modify
int __stdcall MongateCsGetSmsEx(SOCKET sock, char *strMsg)
{
	//recv=-1
	int cbRecv = 0;
	int cbSend = 0;
	int len = 0;
	int iret = 0;
	int count = 0;
	int iTotalLen = 0,iMsgLen = 0;
	bool bFlag = true;
	char recvbuf[20480]={0},tempbuf[1001]={0};
	char rev[500][255]={0};
	int i;
	char *p = NULL;
	time_t tCourrent;
	time(&tCourrent);
	if (sock<=0) {
		return -198;
	}
	iret = GetMoRptMessageEx();	//�������л�״̬���溯��
	if (g_iMoNumEx > 0)
	{
		strcpy(strMsg,szMoMsgEx);
		memset(szMoMsgEx,0,sizeof(szMoMsgEx));
	}
	count = g_iMoNumEx;
	g_iMoNumEx = 0;
//	EnterCriticalSection(&g_csReport);
	if (g_iReportNumEx > g_iReportPackNum)
	{
		WriteToDatabase(szRptEx,g_iReportNumEx);
		memset(szRptEx,0,sizeof(0));
		g_iReportNumEx = 0;
	}
//	LeaveCriticalSection(&g_csReport);
	return count;	//���ؽ��յ���������Ϣ����

	strncpy(tempbuf,vasport,4);
	tempbuf[4] ='\0';
	if(0==strcmp(tempbuf,"9160"))
	{//9160�˿�
		iret = MongateVasGetSms(sock,rev);
		for(i=0;i < iret;i++)
		{
			strcat(recvbuf,rev[i]);
			strcat(recvbuf,";");
		}
		recvbuf[strlen(recvbuf)-1]='\0';
		strcpy(strMsg,recvbuf);
		return iret;
	}
	//WSANOTINITIALISED 
	memset(strMsg,0,sizeof(strMsg));
	if(((int)difftime(tCourrent,g_tHistory[5]) < (g_iMOSpace+1))&&(g_iMOSpace!=0))
	{
		return -300;
	}
	else{
		g_tHistory[5]=tCourrent;
	}
	if (WaitSomeTime(sock, g_iOvertime, SEND) == 1)
	{
		cbSend = send(sock, "ZZ000019,31,2,*,*,*", 19, 0);
	}
	else
	{
		return -101;
	}
	if (cbSend<1) {
		return -102;
	}
	while (bFlag) {
		memset(tempbuf, 0, sizeof(tempbuf));
		if (WaitSomeTime(sock, g_iOvertime, RECV) == 1)
		{
			cbRecv = recv(sock, tempbuf, 500, 0);
			if (cbRecv<1) {
				iret = -104;
				bFlag = false;
				break;
			}
			tempbuf[cbRecv]=0;
			iTotalLen+=cbRecv;
			strcat(recvbuf,tempbuf);
			if (tempbuf[0] == 'E' && tempbuf[1] == 'R')
			{
				iret = -10000 - (atoi(tempbuf + 3));//����ƽ̨������Ϣ
				bFlag = false;
			}			
			else if (tempbuf[0] == 'O' && tempbuf[1] == 'K')
			{
				char temp[20]={0};
				int iFlag = 0;
				char * token=NULL;
				const char Flag[]=",";
				memcpy(temp,tempbuf,20);
				token=strtok(temp,Flag);
				while (token != NULL) {
					iFlag++;					
					if (3==iFlag) {
						g_iMOSpace=atoi(token);
					}
					token = strtok( NULL,Flag);
				}
				iret = 0;//û����Ϣ
				token=NULL;
				bFlag=false;
			}			
			else if (tempbuf[0] == 'Z' || tempbuf[1] == 'Z')
			{
				if (iTotalLen>=8) {
					char Head[10]={0};
					memcpy(Head,tempbuf+2,6);
					iMsgLen=atoi(Head);
				}
				else
				{
					continue;
				}
				if (iTotalLen<iMsgLen) 
				{
					continue;
				}
				else
				{
					int iCount=0;
					int ilen=0;
					char *p=NULL;
					iCount=atoi(recvbuf+16);
					p=strstr(recvbuf+16,",");
					ilen=strlen(p+1);
					memcpy(strMsg,p+1,strlen(p)-1);
					strMsg[ilen-2]='\0';
					iret=iCount;
					p=NULL;
					bFlag=false;
				}				
			}
			else
			{
				if (iTotalLen<iMsgLen) 
				{
					continue;
				}
				else
				{
					int iCount=0;
					int ilen = 0;
					char *p=NULL;
					iCount=atoi(recvbuf+16);
					p=strstr(recvbuf+16,",");
					ilen=strlen(p+1);
					memcpy(strMsg,p+1,strlen(p)-1);
					strMsg[ilen-2]='\0';
					iret=iCount;
					p=NULL;
					bFlag=false;
				}
			}
		}
		else
		{
			iret = -103;
			bFlag = false;
		}
	}
	return iret;
}
//shaojun
int __stdcall MongateCsGetSms(SOCKET sock, char strMsg[500][255])
{
	int i=0;
	int cbRecv = 0;
	int cbSend = 0;
	int len = 0;
	int iret = 0;
	int count = 0;
	int iTotalLen = 0,iMsgLen = 0;
	bool bFlag = true;
	char recvbuf[20480]={0},tempbuf[1001]={0};
	time_t tCourrent;
	time(&tCourrent);
	if (sock<=0) {
		return -198;
	}
	iret = GetMoRptMessage();	//�������л�״̬���溯��
	for (i=0;i<g_iMoNum;i++)
	{
		strcpy(strMsg[i],szMoMsg[i]);
		memset(szMoMsg[i],0,255);
	}
	count = g_iMoNum;
	g_iMoNum = 0;
//	EnterCriticalSection(&g_csReport);
	if (g_iReportNum > g_iReportPackNum)
	{
		//����
		for (i=0;i<g_iReportNum;i++)
		{
			strcat(recvbuf,szRpt[i]);
			if (i<g_iReportNum-1)
			{
				strcat(recvbuf,";");
			}
			memset(szRpt[i],0,255);
		}
		WriteToDatabase(recvbuf,g_iReportNum);
		g_iReportNum = 0;
	}
//	LeaveCriticalSection( &g_csReport );
	return count;	//���ؽ��յ���������Ϣ����

	if(((int)difftime(tCourrent,g_tHistory[6]) < (g_iMOSpace+1))&&(g_iMOSpace!=0))
	{
		return -300;
	}
	else{
		g_tHistory[6]=tCourrent;
	}	
	if(vassend)
	{//����ֵͨ��
		iret = MongateVasGetSms(sock,strMsg);
		return iret;
	}
	if (WaitSomeTime(sock, g_iOvertime, SEND) == 1)
	{
		cbSend = send(sock, "ZZ000019,31,2,*,*,*", 19, 0);
	}
	else
	{
		return -101;
	}
	if (cbSend<1) {
		return -102;
	}

	while (bFlag) {
		memset(tempbuf, 0, sizeof(tempbuf));
		if (WaitSomeTime(sock, g_iOvertime, RECV) == 1)
		{
			cbRecv = recv(sock, tempbuf, 1000, 0);
			if (cbRecv<1) {
				iret = -104;
				bFlag = false;
				break;
			}
			tempbuf[cbRecv]=0;
			iTotalLen+=cbRecv;
			strcat(recvbuf,tempbuf);
			if (tempbuf[0] == 'E' && tempbuf[1] == 'R')
			{
				iret = -10000 - (atoi(tempbuf + 3));//����ƽ̨������Ϣ
				bFlag = false;
			}			
			if (strstr(tempbuf, "OK,0") != NULL)
			{
				char temp[20]={0};
				int iFlag = 0;
				char * token=NULL;
				const char Flag[]=",";
				memcpy(temp,tempbuf,20);
				token=strtok(temp,Flag);
				while (token != NULL) {
					iFlag++;					
					if (3==iFlag) {
						g_iMOSpace=atoi(token);
					}
					token = strtok( NULL,Flag);
				}
				iret = 0;//û����Ϣ	
				token=NULL;
				bFlag=false;
			}
			if (tempbuf[0] == 'Z' || tempbuf[1] == 'Z')
			{
				if (iTotalLen>=8) {
					char Head[10]={0};
					memcpy(Head,tempbuf+2,6);
					iMsgLen=atoi(Head);
				}
				else
				{
					continue;
				}
				if (iTotalLen<iMsgLen) 
				{
					continue;
				}
				else
				{
					const char cFlag[]=";";
					int iCount=0;
					char* token=NULL;
					token = strtok(recvbuf+20, cFlag);
					while( token != NULL )
					{
						char tempMsg[256]={0};
						convertMOMsg(token,tempMsg);
						memcpy(strMsg[iCount],tempMsg,strlen(tempMsg));
						strMsg[iCount][strlen(tempMsg)] = '\0';
						iCount++;
						token = strtok( NULL, cFlag);
					}
					iret=iCount;
					token=NULL;
					bFlag=false;
				}				
			}
			else
			{
				if (iTotalLen<iMsgLen) 
				{
					continue;
				}
				else
				{
					const char cFlag[]=";";
					int iCount=0;
					char* token=NULL;
					token = strtok(recvbuf+20, cFlag);
					while( token != NULL )
					{
						char tempMsg[256]={0};
						convertMOMsg(token,tempMsg);
						memcpy(strMsg[iCount],tempMsg,strlen(tempMsg));
						strMsg[iCount][strlen(token)] = '\0';
						iCount++;
						token = strtok( NULL, cFlag);
					}
					iret=iCount;
					token=NULL;
					bFlag=false;
				}
			}
		}
		else
		{
			return -103;
		}
	}
	return iret;
}

// ������ֵ��Ϣ
//����1�����Ӿ���������ͱ�����MongateConnect�����ķ���ֵ��
//����2�����ʴ���
//����3���������,������ʱ�Ķ˿ں�
//����4��������(�ֽв�����)
//����5������ƥ����
//����6�������ֻ� ��:"13500000000,13600000000"
//����7����Ϣ����
//����8�������ֻ��ĸ���
//����ֵ:
//     1: �ɹ�
//    -1: ����Ĳ����У�������NULLֵ
// -3,-4: �������������ʱ
//   -10: ����ѿռ�ʧ��
int __stdcall MongateVasSendSms(SOCKET sock, const char* pszFeeCode,
  const char* pszServiceNum, const char* pszOperCode, const char* pszLinkID,
  const char* pszMobis, const char* pszMsg, int iMobiCount)
{
	int cbRecv = 0;
	int cbSend = 0;
	char* buf = NULL;
	char strMsg[MAX_SMS_LENGTH * 4 + 1] = {0};
	int ret = -11;
	char tmpstr[21]={0},tmp[1600]={0};
	if((pszLinkID!=NULL)&&(strlen(pszLinkID)>1)) strncpy(tmpstr,pszLinkID,20);
	else
	{//����
		strncpy(tmp,pszMobis,11);
		strncpy(tmpstr,GetUnicomSerialNo(tmp),20);
	}
	if (sock<=0) {
		return -198;
	}
	if (pszFeeCode == NULL || pszServiceNum == NULL || pszOperCode == NULL
	  || pszMobis == NULL || pszMsg == NULL)
 		return -1;

	if (iMobiCount > 1000)
		return -2;

//	strncpy(strMsg, pszMsg, MAX_SMS_LENGTH);
//	strMsg[MAX_SMS_LENGTH] = '\0';

 
	buf = (char*)malloc(255 + iMobiCount * 17);
	if (buf == NULL)
		return -10;

	while (true)
	{
		////////ͨ��ѡ��
		srcStrTohexChr(pszMsg,strMsg);
		strncpy(buf,pszServiceNum,4);
		buf[4]='\0';
		if(strcmp(buf,"9160")!=0)
		{//����ͨ��
			//ZZ000073,51,*,*,121336,Link8888,11********,9160101,0,1,4,3131,13800000000
			sprintf(buf + 8, ",51,*,*,%s,%s,%s,%s,0,%d,%d,%s,%s",pszFeeCode,tmpstr, pszOperCode,pszServiceNum, iMobiCount, strlen(strMsg), strMsg, pszMobis);
				//tmpstr, pszOperCode, iMobiCount, strlen(strMsg), strMsg, pszMobis);
			sprintf(buf, "ZZ%06d", strlen(buf + 8) + 8);
			buf[8] = ',';
		}
		else
		{//9160ͨ��
			strcpy(strMsg,pszMsg);
			sprintf(buf + 8, ",%s,%s,%s,%s,%d,%d,%s,%s", pszFeeCode, pszServiceNum,
				//pszLinkID, pszOperCode, iMobiCount, strlen(strMsg), strMsg, pszMobis);
				tmpstr, pszOperCode, iMobiCount, strlen(strMsg), strMsg, pszMobis);
			sprintf(buf, "ZE%06d", strlen(buf + 8) + 8);
			buf[8] = ',';
		}
 		if (WaitSomeTime(sock, g_iOvertime, SEND) == 1)
			cbSend = send(sock, buf, strlen(buf), 0);
		else
		{
			ret = -101;//
			break;
		}    
		memset(buf, 0, 50);
		if (WaitSomeTime(sock, g_iOvertime, RECV) == 1)
			cbRecv = recv(sock, buf, 30, 0); //�������21
		else
		{
			ret = -103;//
			break;
		}
    
		if (cbSend < 1 || cbRecv < 1)
		{
			ret = -102;//
			break;
		}
    
		if (buf[0] == 'O' && buf[1] == 'K')
		{      
			ret = 1;//
			break;
		}
		else if (buf[0] == 'E' && buf[1] == 'R')
		{
			ret = -10000 - (atoi(buf + 3)); //����ƽ̨������Ϣ
			break;
		}
		else
		{
			ret = -200;
			break;
		}
	}
	free(buf);
	buf = NULL;
	return ret;
}
//---------------------------------------------------------------------------
// ������ֵ��Ϣ
int __stdcall MongateVasSendSmsBeta(SOCKET sock, const char* pszFeeCode,
  const char* pszServiceNum, const char* pszOperCode, const char* pszLinkID,
  const char* pszMobis, const char* pszMsg, int iMobiCount,char* pszSN)
{
	int cbRecv = 0;
	int cbSend = 0;
	char* buf = NULL;
	char strMsg[MAX_SMS_LENGTH + 1] = {0};
	int ret = -11;
	char tmpstr[100]={0};
	if (sock<=0) {
		return -198;
	}
	if (pszFeeCode == NULL || pszServiceNum == NULL || pszOperCode == NULL
		|| pszLinkID == NULL || pszMobis == NULL || pszMsg == NULL)
		return -1;

	if (iMobiCount > 1000)
		return -2;
	strncpy(strMsg, pszMsg, MAX_SMS_LENGTH);
	strMsg[MAX_SMS_LENGTH] = '\0';

	buf = (char*)malloc(255 + iMobiCount * 17);
	if (buf == NULL)
		return -10;
	while (true)
	{
		sprintf(buf + 8, ",%s,%s,%s,%s,%d,%d,%s,%s", pszFeeCode, pszServiceNum,
			pszLinkID, pszOperCode, iMobiCount, strlen(strMsg), strMsg, pszMobis);
		sprintf(buf, "ZE%06d", strlen(buf + 8) + 8);
		buf[8] = ',';  
 		if (WaitSomeTime(sock, g_iOvertime, SEND) == 1)
			cbSend = send(sock, buf, strlen(buf), 0);
		else
		{
			ret = -101;//
			break;
		}
 		memset(buf, 0, 50);
		if (WaitSomeTime(sock, g_iOvertime, RECV) == 1)
			cbRecv = recv(sock, buf, 30, 0); //�������21
		else
		{
			ret = -103;//
			break;
		}
    
		if (cbSend < 1 || cbRecv < 1)
		{
			ret = -102;//
			break;
		}    
		if (buf[0] == 'O' && buf[1] == 'K')
		{      
			ret = 1;//
			strcpy(pszSN, buf+3);
			pszSN[strlen(buf+3)-2] = '\0';
			pszSN[strlen(buf+3)-1] = '\0';
			break;
		}
		else if (buf[0] == 'E' && buf[1] == 'R')
		{
			ret = -10000 - (atoi(buf + 3)); //����ƽ̨������Ϣ
			break;
		}
		else
		{
			ret = -200;
			break;
		}
	}
	free(buf); buf = NULL;
	return ret;
}

//---------------------------------------------------------------------------
// �ͷ��Ӷ˿ڷ���
//ZZ�����,25(��������),*(����),*,*,�Ӷ˿�,��Ϣ�����,�ֻ�����,��Ϣ����,��Ϣ����,�����б�
//ZZ�����,23(��������),*(����),*,*,�Ӷ˿�,��Ϣ�����,�ֻ�����,��Ϣ����,��Ϣ����,�����б�
int __stdcall MongateCsSPSendSms(SOCKET sock, const char* pszMobis,
	const char* pszMsg, int iMobiCount,const char* pszSubPort,char* pszSN)
{
	int cbRecv = 0;
	int cbSend = 0;
	int iFlag = 0;
	char* buf = NULL;
	char strNMsg[5120] = {0}; 
	char sendport[50]={0};
	int ret = -11;
	char buffer[10]={0};
	long tStart,tEnd;
	int iRet = 0;
	char szSpgateEx[50] = {0};
	byte FCS = 0;
	char szMsgIdOld[50] = {0};
	char szMsgIdNew[50] = {0};
	if (sock<=0) {
		return -198;
	}
	if (pszMobis == NULL || pszMsg == NULL)
		return -1;
#ifdef USE_SMEIDLL_DLL_
	if (iMobiCount > 100)
	{
		return -2;
	}
	if (strcmp(pszSubPort,"*")==0)	//���Ӷ˿�
	{
		strcpy(szSpgateEx,g_spgate);
	}
	else
	{
		sprintf(szSpgateEx,"%s%s",g_spgate,pszSubPort);
	}
	iRet = SendSms(szSpgateEx,pszMobis,1,pszMsg,szMsgIdOld,&FCS);
	TranSmitMsgID(szMsgIdOld,szMsgIdNew);
	strcpy(pszSN,szMsgIdNew);
	//////////////////////////////////////////////////////////////////////////////
	//�˴�������ֵת���������ϲ�
	if (iRet == 0 && FCS == 0)		//���ͳɹ�
	{
		return 1;
	}
	else if (0 == iRet && 0 != FCS)
	{
		//���Ų��Ϸ��������ط�
		if (9 == FCS || 169 == FCS || 10 == FCS|| 69 == FCS || 13 == FCS || 6 == FCS)
		{
			return -10002;
		}
		//�������������β����Ϣһ��
		else if (8 == FCS)
		{
			//������
			Sleep(200);
			return -10009; //��������
		}
		//Ƿ��
		else if(208 == FCS || 210 == FCS || 206 == FCS)
		{
			return -10003; //����
		}		
		else	//�ɹ�
		{
			return 1;
		}	
	}
	else
	{
		//���Ų��Ϸ��������ط�
		if (9 == FCS || 169 == FCS || 10 == FCS|| 69 == FCS || 13 == FCS || 6 == FCS)
		{
			return -10002;	//���Ϸ�
		}
		else if (iRet ==13)
		{
			return -10002;
		}
		//������
		else if (1 == iRet || 255 == iRet)
		{
			return -101;
		}
		//Ƿ��
		else if(208 == FCS || 210 == FCS || 206 == FCS)
		{
			return -10003;
		}
		//�������������β����Ϣһ��
		else if (8 == FCS)
		{
			//������
			Sleep(200);
			return -10099;
		}
		//����
		else
		{
			return -10099;
		}
	}
///////////////////////////////////////////////////////////////////////////////
#endif

	if (iMobiCount > 1000)
	return -2;
	if((pszSubPort!=NULL&&strcmp(pszSubPort,"")!=0&&strcmp(vasport,"")!=0)||vassend)//strcmp(pszSubPort,"-1")
	{//��ֵ�˿�
		//��ȡ�˿�����
		//if(iMobiCount > 100)return -10029;//��Ȩ��
		strcpy(sendport,vasport);
		strncpy(buffer,sendport,4);
		buffer[4] ='\0';
		if(strcmp(buffer,"9160"))
		{//������9160�˿�
			if(strcmp(pszSubPort,"-1")==0)
			{
				ret = MongateCsSendSms(sock,pszMobis,pszMsg,iMobiCount,pszSN);
				return ret;
			}
			else
				strcpy(sendport,pszSubPort);
			//�����߿ͷ��Ӷ˿�
		}else
		{
			if(strcmp(pszSubPort,"-1")!=0)
			{//9160�˿�
				strcat(sendport,pszSubPort);
			}
			ret = MongateVasSendSms(sock,"121336",sendport,"00********","", pszMobis, pszMsg, iMobiCount);
			strcpy(pszSN,"1000000000");
			return ret;
		}
	}
	memset(pszSN,0,sizeof(pszSN));
	srcStrTohexChr(pszMsg,strNMsg);//������Ϣ����
	strNMsg[511] = '\0'; 
	buf = (char*)malloc(1024 + iMobiCount * 17);
	if (buf == NULL)
		return -10;
	memset(buf,0,sizeof(buf));
	while (true)
	{
#ifdef CsSPSend61
		iFlag=TelsCheck(pszMobis,iMobiCount);
		if (1==iFlag) {
			sprintf(buf+8,",61,*,*,*,%s,0,%d,%d,%s,%s",pszSubPort,iMobiCount, strlen(strNMsg),strNMsg, pszMobis);
		}
		else
		{
			ret=iFlag;
			break;
		}
#else
		if(iMobiCount==1) //�ֿ�������Ⱥ��
		{
			if (!(IsNumeric(pszMobis))) {
				ret =  -11;//
				break;
			}
			sprintf(buf+8,",23,*,*,*,%s,0,%d,%d,%s,%s", pszSubPort,iMobiCount, strlen(strNMsg),strNMsg, pszMobis);
		}
		else
		{
			iFlag=TelsCheck(pszMobis,iMobiCount);
			if (1==iFlag) {
				sprintf(buf+8,",25,*,*,*,%s,0,%d,%d,%s,%s",pszSubPort,iMobiCount, strlen(strNMsg),strNMsg, pszMobis);
			}
			else
			{
				ret=iFlag;
				break;
			}
		}
#endif
		sprintf(buf, "ZZ%06d", strlen(buf+8)+ 8);
		buf[8] = ',';
		if (WaitSomeTime(sock, g_iOvertime, SEND) == 1)
			cbSend = send(sock, buf, strlen(buf), 0);
		else
		{
			ret = -101;//
			break;
		}
		tStart=tEnd=time(NULL);
		memset(strNMsg,0,sizeof(strNMsg));
		while(tEnd-tStart<g_iOvertime){	
			if(strstr(strNMsg,"\r\n"))break;
			memset(buf, 0, 50);
			if (WaitSomeTime(sock, g_iOvertime, RECV) == 1){
				cbRecv = recv(sock, buf, 30, 0); //�������21
				strcat(strNMsg,buf);
			}
			else
			{
				ret = -103;
				break;
			}
			tEnd=time(NULL);
		}
		if (cbSend < 1 || cbRecv < 1)
		{
			ret = -102;
			break;
		}
		memset(buf, 0, 50);
		strncpy(buf,strNMsg,49);
		if (buf[0] == 'O' && buf[1] == 'K')
		{
			strcpy(pszSN, buf+3);
			pszSN[strlen(buf+3)-2] = '\0';
			pszSN[strlen(buf+3)-1] = '\0';
			ret = 1;
			break;
		}
		else if (buf[0] == 'E' && buf[1] == 'R')
		{
			ret = -10000 - (atoi(buf + 3)); //����ƽ̨������Ϣ
			break;
		}
		else
		{
			ret = -200;
			break;		
		}
	}
	free(buf); buf = NULL;
	return ret;
}

int __stdcall MongateCsSPSendSmsHttp(SOCKET sock, const char* pszMobis,
		const char* pszMsg, int iMobiCount,const char* pszSubPort,char* pszSN)
{
	int cbRecv = 0;
	int cbSend = 0;
	int iFlag = 0;
	char* buf = NULL;
	char* httpbuf=NULL;
	char strNMsg[512] = {0}; 
	char strRecv[1024]={0};
	char sendport[50]={0};
	int ret = -11;
	if (sock<=0) {
		return -198;
	}
	if (pszMobis == NULL || pszMsg == NULL)
		return -1;
	if (iMobiCount > 1000)
		return -2;

	memset(pszSN,0,sizeof(pszSN));
	srcStrTohexChr(pszMsg,strNMsg);//������Ϣ����
	strNMsg[511] = '\0'; 
	buf = (char*)malloc(1024 + iMobiCount * 17);
	httpbuf = (char*)malloc(1224 + iMobiCount * 17);
	if (buf == NULL||httpbuf == NULL)
		return -10;
	memset(buf,0,sizeof(buf));
	memset(httpbuf,0,sizeof(httpbuf));
	while (true)
	{
#ifdef CsSPSend61
		iFlag=TelsCheck(pszMobis,iMobiCount);
		if (1==iFlag) {
			sprintf(buf+8,",61,*,*,*,%s,0,%d,%d,%s,%s",pszSubPort,
				iMobiCount, strlen(strNMsg),strNMsg, pszMobis);
		}
		else{
			ret=iFlag;
			break;
		}
#else
		if(iMobiCount==1) //�ֿ�������Ⱥ��
		{
			if (!(IsNumeric(pszMobis))) {
				ret =  -11;//
				break;
			}
			sprintf(buf+8,",23,*f,*,*,%s,0,%d,%d,%s,%s", pszSubPort,
			iMobiCount, strlen(strNMsg),strNMsg, pszMobis);
		}
		else
		{
			iFlag=TelsCheck(pszMobis,iMobiCount);
			if (1==iFlag) {
				sprintf(buf+8,",25,*,*,*,%s,0,%d,%d,%s,%s",pszSubPort,
					iMobiCount, strlen(strNMsg),strNMsg, pszMobis);
			}
			else{
				ret=iFlag;
				break;
			}
		}
#endif
		sprintf(buf, "ZZ%06d", strlen(buf+8) + 8);
		buf[8] = ',';
		sprintf(buf, "ZZ%06d", strlen(buf+8)+ 8);
		buf[8] = ',';    
		if (WaitSomeTime(sock, g_iOvertime, SEND) == 1)
		{
			MakeHttpHead(buf,httpbuf);//20070413 fq
			cbSend = send(sock, httpbuf, strlen(httpbuf), 0);
		}
		else
		{
			ret = -101;//
			break;
		}
		if (WaitSomeTime(sock, g_iOvertime, RECV) == 1){
				cbRecv = recv(sock, strRecv, 1024, 0); //�������21
		}
		else
		{
			ret = -103;
			break;
		}
		if (cbSend < 1 || cbRecv < 1)
		{
			ret = -102;
			break;
		}

		strRecv[cbRecv]=0;
		memset(httpbuf,0,sizeof(httpbuf));
		GetHttpRet(strRecv,httpbuf); 
		memset(buf,0,sizeof(buf));
		strcpy(buf,httpbuf);	
		if (buf[0] == 'O' && buf[1] == 'K')
		{
			strcpy(pszSN, buf+3);
			pszSN[strlen(buf+3)-2] = '\0';
			pszSN[strlen(buf+3)-1] = '\0';
			ret = 1;
			break;
		}
		else if (buf[0] == 'E' && buf[1] == 'R')
		{
			ret = -10000 - (atoi(buf + 3)); //����ƽ̨������Ϣ
			break;
		}
		else
		{
			ret = -200;
			break;
		}
	}
	free(buf); buf = NULL;
	free(httpbuf); httpbuf = NULL;
	return ret;
}
//---------------------------------------------------------------------------
// �ͷ����ط���
int __stdcall MongateKxtSend(SOCKET sock, const char* pszMobis,
							   const char* pszMsg, int iMobiCount, char* pszSN)
{
	int cbRecv = 0;
	int cbSend = 0;
	int iFlag=0;
	char* buf = NULL;
	char strMsg[512] = {0};
	int ret = -1;
	char tmpstr[100]={0}; 
	char strNMsg[512]={0};
	//������
	if (sock<=0) {
		return -198;
	}
	if (pszMobis == NULL || pszMsg == NULL)
		return -1;
	if (iMobiCount > 1000)
		return -2;
	memset(pszSN,0,sizeof(pszSN));
	srcStrTohexChr(pszMsg,strNMsg);
	strncpy(strMsg, strNMsg, strlen(strNMsg));
	strMsg[MAX_SMS_LENGTH] = '\0';
	buf = (char*)malloc(512 + iMobiCount * 17);
	if (buf == NULL)
		return -10;
	memset(buf,0,sizeof(buf));
	while (true)
	{
		iFlag=TelsCheck(pszMobis,iMobiCount);
		if (1==iFlag) {
			sprintf(buf+8,",21,*,*,*,*,0,%d,%d,%s,%s", iMobiCount, strlen(strMsg), strMsg, pszMobis);
		}
		else{
			ret=iFlag;
			break;
		}			
		sprintf(buf, "ZZ%06d", strlen(buf+8) + 8);
		buf[8] = ',';	
		if (WaitSomeTime(sock, g_iOvertime, SEND) == 1)
			cbSend = send(sock, buf, strlen(buf), 0);
		else
		{
			ret = -101;//
			break;
		}
		memset(buf, 0, 50);
		if (WaitSomeTime(sock, g_iOvertime, RECV) == 1)
			cbRecv = recv(sock, buf, 50, 0); 
		else
		{
			ret = -103;//
			break;
		}
		if (cbSend < 1 || cbRecv < 1)
		{
			ret = -102;//
			break;
		}
		if (buf[0] == 'O' && buf[1] == 'K')
		{
			strcpy(pszSN, buf+3);
			pszSN[strlen(buf+3)-2] = '\0';
			pszSN[strlen(buf+3)-1] = '\0';
			ret = 1;//
			break;
		}
		else if (buf[0] == 'E' && buf[1] == 'R')
		{
			ret = -10000 - (atoi(buf + 3)); //����ƽ̨������Ϣ
			break;
		}
		else
		{
			ret = -200;
			break;
		}
	}
	free(buf); buf = NULL;
	return ret;
}

int __stdcall MongateCsSendSms(SOCKET sock, const char* pszMobis,
  const char* pszMsg, int iMobiCount, char* pszSN)
{
	int cbRecv = 0;
	int cbSend = 0;
	int iFlag=0;
	char* buf = NULL;
	char strMsg[512] = {0};
	int ret = -9;
	char tmpstr[100]={0}; 
	char strNMsg[512]={0};
	int len;
//�������SMEIDLL��̬��
#ifdef USE_SMEIDLL_DLL_
	return MongateCsSPSendSms(sock,pszMobis,pszMsg,iMobiCount,"*",pszSN);
#endif
  //������
	if(sock<=0){
		return -198;
	}
	if (pszMobis == NULL || pszMsg == NULL)
		return -1;
	if (iMobiCount > 1000)
		return -2;
  
	memset(pszSN,0,sizeof(pszSN));
	len=strlen(pszMsg);
	srcStrTohexChr(pszMsg,strNMsg);
	len=strlen(strNMsg);
	memcpy(strMsg,strNMsg,len);
	strMsg[len] = '\0';
	buf = (char*)malloc(512 + iMobiCount * 17);
	if (buf == NULL)
		return -10;
	memset(buf,0,sizeof(buf));
	while (true)
	{
#ifdef CsSPSend61
		iFlag=TelsCheck(pszMobis,iMobiCount);
		if (1==iFlag) {
			sprintf(buf+8,",61,*,*,*,*,0,%d,%d,%s,%s", iMobiCount, strlen(strMsg), strMsg, pszMobis);
		}
		else{
			ret=iFlag;
			break;
		}	
#else
		if(iMobiCount==1) //�ֿ�������Ⱥ��
		{
			if (!(IsNumeric(pszMobis))) {
				ret =  -11;//
				break;
			}
			if (g_SoftType==1) {
				sprintf(buf+8,",21,*,*,*,*,0,%d,%d,%s,%s", iMobiCount, strlen(strMsg),strMsg, pszMobis);
			}
			else{
				sprintf(buf+8,",23,*,*,*,*,0,%d,%d,%s,%s", iMobiCount, strlen(strMsg),strMsg, pszMobis);
			}
		}			
		else
		{
			iFlag=TelsCheck(pszMobis,iMobiCount);
			if (1==iFlag) {
				if (g_SoftType==1) {
					sprintf(buf+8,",21,*,*,*,*,0,%d,%d,%s,%s", iMobiCount, strlen(strMsg), strMsg, pszMobis);
				}
				else
				{
					sprintf(buf+8,",25,*,*,*,*,0,%d,%d,%s,%s", iMobiCount, strlen(strMsg), strMsg, pszMobis);
				}
			}
			else{
				ret=iFlag;
				break;
			}			
		}
#endif
		sprintf(buf, "ZZ%06d", strlen(buf+8) + 8);
		buf[8] = ',';	
		if (WaitSomeTime(sock, g_iOvertime, SEND) == 1)
			cbSend = send(sock, buf, strlen(buf), 0);
		else
		{
			ret = -101;//
			
			break;
		}
		memset(buf, 0, 50);
		if (WaitSomeTime(sock, g_iOvertime, RECV) == 1)
			cbRecv = recv(sock, buf, 50, 0); 
		else
		{
			ret = -103;//
			break;
		}
		if (cbSend < 1 || cbRecv < 1)
		{
			ret = -102;//
			break;
		}
		if (buf[0] == 'O' && buf[1] == 'K')
		{
			strcpy(pszSN, buf+3);
			pszSN[strlen(buf+3)-2] = '\0';
			pszSN[strlen(buf+3)-1] = '\0';
			ret = 1;//
			break;
		}
		else if (buf[0] == 'E' && buf[1] == 'R')
		{
			ret = -10000 - (atoi(buf + 3)); //����ƽ̨������Ϣ
			break;
		}
		else
		{
			ret = -200;
			break;
		}
	}
	free(buf); buf = NULL;
	return ret;
}

int __stdcall MongateSendSmsBeta(SOCKET sock, const char* pszMobis,
  const char* pszMsg,char* pszSN)
{
	int cbRecv = 0;
	int cbSend = 0;
	char* buf = NULL;
	char strMsg[MAX_SMS_LENGTH + 1] = {0};
	int ret = -11;
	char tmpstr[100]={0};
	int iMobiCount=1;
	if (sock<=0) {
		return -198;
	}
	if (pszMobis == NULL || pszMsg == NULL )
		return -1;

	if (iMobiCount > 1000)
		return -2;
	memset(pszSN,0,sizeof(pszSN));
	strncpy(strMsg, pszMsg, MAX_SMS_LENGTH);
	strMsg[MAX_SMS_LENGTH] = '\0';
	buf = (char*)malloc(255 + iMobiCount * 17);
	if (buf == NULL)
		return -10;

	while (true)
	{
		sprintf(buf+8,",11,*,*,*,%d,%d,%s,%s", iMobiCount, strlen(strMsg),
		strMsg, pszMobis);   
		sprintf(buf, "ZZ%06d", strlen(buf+8) + 8);
		buf[8] = ',';
		if (WaitSomeTime(sock, g_iOvertime, SEND) == 1)
			cbSend = send(sock, buf, strlen(buf), 0);
		else
		{
			ret = -101;//
			break;
		}    
		memset(buf, 0, 50);
		if (WaitSomeTime(sock, g_iOvertime, RECV) == 1)
			cbRecv = recv(sock, buf, 30, 0); //�������21
		else
		{
			ret = -103;//
			break;
		}
    
		if (cbSend < 1 || cbRecv < 1)
		{
			ret = -102;//
			break;
		}
    
		if (buf[0] == 'O' && buf[1] == 'K')
		{     
			ret = 1;//
			strcpy(pszSN, buf+3);
			pszSN[strlen(buf+3)-2] = '\0';
			pszSN[strlen(buf+3)-1] = '\0';
			break;
		}
		else if (buf[0] == 'E' && buf[1] == 'R')
		{
			ret = -10000 - (atoi(buf + 3)); //����ƽ̨������Ϣ
			break;
		}
		else
		{
			ret = -200;
			break;
		}
	}
	free(buf); buf = NULL;
	return ret;
}

//---------------------------------------------------------------------------
// �ͷ����ؽ���״̬����
int __stdcall MongateCsGetStatusReport(SOCKET sock, char strMsg[500][255])
{
	int cbRecv = 0;
	int cbSend = 0;
	int count = 0;
	int len = 0;
	int iret = 0;
	int msgTotalLen = 0;
	int msgLen = 0;
	char recvbuf[20000]={0};
	char tempbuf[1001]={0};
	char buf[255]= {0};	
	bool bFlag=true;
	int i =0;
	int iRptCount;
	time_t tCourrent;
	time(&tCourrent);
	if (sock<=0) {
		return -198;
	}
	if (g_iReportNum<1)
	{
		ReadFromDatabase(recvbuf,&iRptCount);
		if (iRptCount > 0)			//�����ݿ��ж�ȡ��״̬����
		{
			//��״̬������н���
			const char cFlag[]=";";
			int iCount=0;
			char* token=NULL;
			token = strtok(recvbuf+10, cFlag);
			while( token != NULL )
			{
				memcpy(strMsg[iCount],token,strlen(token));
				strMsg[iCount][strlen(token)] = '\0';
				iCount++;
				token = strtok( NULL, cFlag);
			}
			iret=iCount;
			token=NULL;
			bFlag=false;
			return iRptCount;
		}
	}
	iret = GetMoRptMessage();	//�������л�״̬���溯��
	for (i=0;i<g_iReportNum;i++)
	{
		strcpy(strMsg[i],szRpt[i]);
		memset(szRpt[i],0,255);
	}
	count = g_iReportNum;
	g_iReportNum = 0;
	return count;	//���ؽ��յ���״̬��������

	if(((int)difftime(tCourrent,g_tHistory[7]) < (g_iReportSpace+1))&&(g_iReportSpace!=0))
	{
		return -300;
	}
	else{
		g_tHistory[7]=tCourrent;
	}
	if (WaitSomeTime(sock, g_iOvertime, SEND) == 1)
	{
		cbSend = send(sock, "ZZ000019,33,1,*,*,*",19, 0);
	}
	else
	{
		return -101;
	}
	if(cbSend<1)
	{
		return -102;
	}
	while(bFlag)
	{
		memset(tempbuf,0,sizeof(tempbuf));
		if (WaitSomeTime(sock, g_iOvertime, RECV) == 1)
		{
			cbRecv = recv(sock, tempbuf, 1000, 0);
			if (cbRecv<1) {
				iret=-104;
				bFlag=false;
			}
			msgTotalLen+=cbRecv;
			if (cbRecv==1000) {
				tempbuf[1000]='\0';
			}
			tempbuf[cbRecv]=0;
			strcat(recvbuf,tempbuf);
			if (tempbuf[0] == 'E' && tempbuf[1] == 'R')
			{
				iret= -10000 - (atoi(tempbuf + 3));//����ƽ̨������Ϣ
				bFlag=false;
			}  
			else if (tempbuf[0] == 'O' && tempbuf[1] == 'K')
			{	
				char temp[20]={0};
				int iFlag = 0;
				char * token=NULL;
				const char Flag[]=",";
				memcpy(temp,tempbuf,20);
				token=strtok(temp,Flag);
				while (token != NULL) {
					iFlag++;					
					if (3==iFlag) {
						g_iReportSpace=atoi(token);
					}
					token = strtok( NULL,Flag);
				}
				iret = 0;//û����Ϣ
				token=NULL;
				bFlag=false;
			} 
			else if(tempbuf[0] == 'Z' && tempbuf[1] == 'Z')
			{
				if (msgTotalLen>=8) {
					char Head[10]={0};
					memcpy(Head,tempbuf+2,6);
					msgLen=atoi(Head);
				}
				else
				{
					continue;
				}
				if (msgTotalLen<msgLen) 
				{
					continue;
				}
				else
				{
					const char cFlag[]=";";
					int iCount=0;
					char* token;
					token = strtok(recvbuf+20, cFlag);
					while( token != NULL )
					{
						memcpy(strMsg[iCount],token,strlen(token));
						strMsg[iCount][strlen(token)] = '\0';
						iCount++;
						token = strtok( NULL, cFlag);
					}
					iret=iCount;
					bFlag=false;
				}
			}
			else
			{
				if (msgTotalLen<msgLen) 
				{
					continue;
				}
				else
				{
					const char cFlag[]=";";
					int iCount=0;
					char* token=NULL;
					token = strtok(recvbuf+20, cFlag);
					while( token != NULL )
					{
						memcpy(strMsg[iCount],token,strlen(token));
						strMsg[iCount][strlen(token)] = '\0';
						iCount++;
						token = strtok( NULL, cFlag);
					}
					iret=iCount;
					token=NULL;
					bFlag=false;
				}
			}
		}
		else
		{
			iret=-103;
			bFlag=false;
		}
	}
	return iret;
}

int __stdcall MongateCsGetStatusReportEx(SOCKET sock, char *strMsg)
{
	int cbRecv = 0;
	int cbSend = 0;
	int count = 0;
	int len = 0;
	int iret = 0;
	int msgTotalLen = 0;
	int msgLen = 0;
	char recvbuf[20000]={0};
	char tempbuf[1001]={0};
	char buf[255]= {0};	
	bool bFlag=true;
	time_t tCourrent;
	int iCount = 0;
	int iRptCount = 0;
	char szfilepath[300] = {0};
	time(&tCourrent);
	if (sock<=0) {
		return -198;
	}
	memset(strMsg,0,sizeof(strMsg));
	if (g_iReportNumEx<1)
	{
		ReadFromDatabase(szRptEx,&iRptCount);
		if(iRptCount>0)		//�ӵ���
		{
			strcpy(strMsg,szRptEx);
			memset(szRptEx,0,sizeof(szRptEx));
			g_iReportNumEx = 0;
			return iRptCount;
		}
	}
	iret = GetMoRptMessageEx();	//�������л�״̬���溯��
	if (g_iReportNumEx > 0)
	{
		strcpy(strMsg,szRptEx);
		memset(szRptEx,0,sizeof(szRptEx));
	}
	count = g_iReportNumEx;
	g_iReportNumEx = 0;
	return count;	//���ؽ��յ���״̬��������
	
	if(((int)difftime(tCourrent,g_tHistory[8]) < (g_iReportSpace+1))&&(g_iReportSpace!=0))
	{
		return -300;
	}
	else{
		g_tHistory[8]=tCourrent;
	}
	if (WaitSomeTime(sock, g_iOvertime, SEND) == 1)
	{
		cbSend = send(sock, "ZZ000019,33,1,*,*,*",19, 0);
	}		
	else
	{
		return -101;
	}
	if(cbSend<1)
	{
		return -102;
	}
	while(bFlag)
	{
		memset(tempbuf,0,sizeof(tempbuf));
		if (WaitSomeTime(sock, g_iOvertime, RECV) == 1)
		{
			cbRecv = recv(sock, tempbuf, 1000, 0);
			if (cbRecv<1) {
				iret=-104;
				bFlag=false;
			}
			msgTotalLen+=cbRecv;
			if (cbRecv==1000) {
				tempbuf[1000]='\0';
			}
			strcat(recvbuf,tempbuf);
			if (tempbuf[0] == 'E' && tempbuf[1] == 'R')
			{
				iret= -10000 - (atoi(tempbuf + 3));//����ƽ̨������Ϣ
				bFlag=false;
			}  
			else if (tempbuf[0] == 'O' && tempbuf[1] == 'K')
			{	
				char temp[20]={0};
				int iFlag = 0;
				char * token=NULL;
				const char Flag[]=",";
				memcpy(temp,tempbuf,20);
				token=strtok(temp,Flag);
				while (token != NULL) {
					iFlag++;					
					if (3==iFlag) {
						g_iReportSpace=atoi(token);
					}
					token = strtok( NULL,Flag);
				}
				iret = 0;//û����Ϣ
				token=NULL;
				bFlag=false;
			} 
			else if(tempbuf[0] == 'Z' && tempbuf[1] == 'Z')
			{
				if (msgTotalLen>=8) {
					char Head[10]={0};
					memcpy(Head,tempbuf+2,6);
					msgLen=atoi(Head);
				}
				else
				{
					continue;
				}
				if (msgTotalLen<msgLen) 
				{
					continue;
				}
				else
				{
					char temp[50]={0};
					const char cFlag[]=",";
					int iCount=0,iNumber=0;
					char* token;
					memcpy(temp,recvbuf,25);
					token = strtok(temp, cFlag);
					while( token != NULL )
					{
						iCount++;
						if (5==iCount) {
							iret=atoi(token);
							break;
						}
						token = strtok( NULL, cFlag);
					}					
					memcpy(strMsg,recvbuf+20,strlen(recvbuf)-20);
					bFlag=false;
				}
			}
			else
			{
				if (msgTotalLen<msgLen) 
				{
					continue;
				}
				else
				{
					char temp[50]={0};
					const char cFlag[]=",";
					int iCount=0,iNumber=0;
					char* token=NULL;
					memcpy(temp,recvbuf,25);
					token = strtok(temp, cFlag);
					while( token != NULL )
					{
						iCount++;
						if (5==iCount) {
							iret=atoi(token);
							break;
						}
						token = strtok( NULL, cFlag);
					}
					memcpy(strMsg,recvbuf+20,strlen(recvbuf)-20);
					token=NULL;
					bFlag=false;
				}
			}
		}
		else
		{
			iret=-103;
			bFlag=false;
		}
	}
	return iret;
}
//---------------------------------------------------------------------------

const char* __stdcall MongateGetVer()
{
    return MW_GATEWAY_VERSION;
}
//---------------------------------------------------------------------------
//ͨ��Socks4��ʽ���� */
int __stdcall MongateConnectSocks4Proxy(const char* pszProxyIP,int iProxyPort,
  const char* pszIP,int iPort,const char* pszAccount,const char* pszPwd)
{
	char buf[1024]={0},httpbuf[1025]={0},reg[100]={0};
	int cbRecv = 0;
	int cbSend = 0;
	struct hostent* pHostInfo = NULL;
	struct sockaddr_in svrAddr;
	struct sc4Req* m_proxyreq; 
	struct sc4Ans* m_proxyans; 
	SOCKET sock = 0;  

	g_ConnectType=0;
  //�������
	if (pszProxyIP == NULL || pszIP == NULL || pszAccount == NULL || pszPwd == NULL)
		return -1;//������Ϣ���п�ֵ

	if (strlen(pszAccount) > 32 || strlen(pszPwd) > 32)
		return -2;//�Ƿ����ʺŻ�����
// 	sprintf(reg,"+%s+%d+",pszAccount,g_iCmdType);
// 	//srcStrTohexChr(reg,buf);
// 	MD5String(buf,reg);
// 	memset(reg,0,33);
// 	strncpy(reg,EncodeID+18,32);
// 	if(strcmp(buf,reg) != 0)
// 	{//δ��Ȩ
// 		return 0;
// 	}
// 	sprintf(reg,",%d,%s,%s,",g_iCmdType,"888888",pszAccount);
// //	srcStrTohexChr(buff,buf);
// 	MD5String(buf,reg);
// 	memset(reg,0,33);
// 	strncpy(reg,EncodeID+115,32);
// 	if(strcmp(buf,reg) != 0)
// 	{//δ��Ȩ
// 		return 0;
// 	} 
  //get host infomation and set svrAddr
	pHostInfo = gethostbyname(pszProxyIP);
	if (pHostInfo == NULL)
	{
		pHostInfo = gethostbyaddr(pszProxyIP, strlen(pszProxyIP), AF_INET);
		if (pHostInfo == NULL)
		{
			return -3;//��ȡԶ��������Ϣʧ��,ԭ��:IP����ʧ�ܻ���Ч��IP,�����粻ͨ
		}
	}

	svrAddr.sin_family = AF_INET;
	memcpy(&svrAddr.sin_addr, pHostInfo->h_addr_list[0], pHostInfo->h_length);
	svrAddr.sin_port = htons((unsigned short)iProxyPort);
  
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == -1)   
		return -4;//socket����ʧ��
  
	if (connect(sock,(struct sockaddr*)&svrAddr, sizeof(svrAddr)) ==-1)
		return -5;//���Ӵ��������ʧ��

	memset(buf,0,sizeof(buf)); 
	m_proxyreq = (struct sc4Req *)buf; 
	m_proxyreq->VN = 4; 
	m_proxyreq->CD = 1; 
	m_proxyreq->Port = ntohs((unsigned short)iPort);
	m_proxyreq->IPAddr = inet_addr(pszIP);
	if (WaitSomeTime(sock, g_iOvertime, SEND) ==1)
	{
		cbSend = send(sock, buf, 9, 0);
	}
	else{
	  return -101;
	}
	if (cbSend < 1){	  
		MongateDisconnect(sock);
		return -102;//��������ͨ���������ӷ���������
	}

	m_proxyans = (struct sc4Ans *)buf;     
	memset(buf, 0, sizeof(buf));
	if (WaitSomeTime(sock, g_iOvertime, RECV) == 1)
		cbRecv = recv(sock, buf, 100,0);	  
	else{
		MongateDisconnect(sock);
		return -103; //���մ�����Ӧ���ӷ������������
	}
	if (cbRecv<1) {
		MongateDisconnect(sock);
		return -104; 
	}
  
	if(m_proxyans->VN != 0 || m_proxyans->CD != 90){	  
		MongateDisconnect(sock);
		return -6; //ͨ������������վ���ɹ�
	}

//	memset(buf, 0, sizeof(buf));
//	if (WaitSomeTime(sock, g_iOvertime, RECV) == 1)
//		cbRecv = recv(sock, buf, sizeof(buf), 0);
//	else{
//		MongateDisconnect(sock);
//		return -104;//connect�ɹ�,�ȴ�ƽ̨��������ȷ����Ϣ��ʱ  
//	}
//
//	if (!(buf[0] == 'O' && buf[1] == 'K'))
//		return -7;//�ȴ�����������ʧ��

	memset(buf, 0, sizeof(buf));
	sprintf(buf,"ZZ%06ld,20,*,*,*,%d,%s,%s,%s,%s",strlen(pszAccount)+strlen(pszPwd)+33,
		 g_iCmdType,g_sVersion,g_Os,pszAccount,pszPwd);//��½ָ��
	buf[8] = ','; 
	if (WaitSomeTime(sock, g_iOvertime, SEND) ==1)
	{
		cbSend = send(sock, buf, strlen(buf), 0);
	}
	else{
		return -105;
	}
	if (cbSend < 1){
		MongateDisconnect(sock);
		return -106;//���͵�½ָ�����
	}

	memset(buf, 0, sizeof(buf));
	if (WaitSomeTime(sock, g_iOvertime, RECV) == 1)
		cbRecv = recv(sock, buf, sizeof(buf),0);
	else{
		MongateDisconnect(sock);
		return -107; //����ƽ̨���ص�½��֤��Ϣ��ʱ
	}
	if (cbRecv<1) {
		MongateDisconnect(sock);
		return -108; //����ƽ̨���ص�½��֤��Ϣʧ��
	}

	if (buf[0] == 'E' && buf[1] == 'R'){ 
		return -10000 - (atoi(buf + 3)); //����ƽ̨������Ϣ
	}
	else if (buf[0] == 'O' && buf[1] == 'K')
		return sock;
	else{
		MongateDisconnect(sock);
		return -200;//����ƽ̨������֤��Ϣ����
	}	
}

//ͨ��Socks5��ʽ���� 
int __stdcall MongateConnectSocks5Proxy( const char* pszProxyIP,
  int iProxyPort,const char* pszProxyAccount,const char* pszProxyPwd,
  const char* pszIP,int iPort,const char* pszAccount,const char* pszPwd)
{
	char buf[1024]={0},httpbuf[1025]={0},reg[100]={0};
	int cbRecv = 0;
	int cbSend = 0;
	struct hostent* pHostInfo = NULL;
	struct sockaddr_in svrAddr;
	struct sock5req1 * m_proxyreq1; 
	struct sock5req2 * m_proxyreq2; 
	struct sock5ans1 * m_proxyans1; 
	struct sock5ans2 * m_proxyans2; 
	int nUserLen = strlen(pszProxyAccount); 
	int nPassLen = strlen(pszProxyPwd); 
	unsigned long tmpLong = inet_addr(pszIP); 
	unsigned short port = ntohs((unsigned short)iPort); 
	struct authans *m_authans; 
	SOCKET sock = 0;
  
	g_ConnectType=0;
  //�������
	if (pszProxyIP == NULL || pszIP == NULL ||pszProxyAccount == NULL
	  || pszProxyPwd == NULL || pszAccount == NULL|| pszPwd == NULL)
		return -1;// ��������У�������NULLֵ

	if (strlen(pszAccount) > 32 || strlen(pszPwd) > 32)
		return -2;//�Ƿ����ʺŻ�����
// 	sprintf(reg,"+%s+%d+",pszAccount,g_iCmdType);
// 	//srcStrTohexChr(reg,buf);
// 	MD5String(buf,reg);
// 	memset(reg,0,33);
// 	strncpy(reg,EncodeID+18,32);
// 	if(strcmp(buf,reg) != 0)
// 	{//δ��Ȩ
// 		return 0;
// 	}
// 	sprintf(reg,",%d,%s,%s,",g_iCmdType,"888888",pszAccount);
// //	srcStrTohexChr(buff,buf);
// 	MD5String(buf,reg);
// 	memset(reg,0,33);
// 	strncpy(reg,EncodeID+115,32);
// 	if(strcmp(buf,reg) != 0)
// 	{//δ��Ȩ
// 		return 0;
// 	} 
  //get host infomation and set svrAddr
	pHostInfo = gethostbyname(pszProxyIP);
	if (pHostInfo == NULL)
	{
		pHostInfo = gethostbyaddr(pszProxyIP, strlen(pszProxyIP), AF_INET);
		if (pHostInfo == NULL)
		{
			return -3;//��ȡԶ��������Ϣʧ��,ԭ��:IP����ʧ�ܻ���Ч��IP,�����粻ͨ
		}
	}
	svrAddr.sin_family = AF_INET;
	memcpy(&svrAddr.sin_addr, pHostInfo->h_addr_list[0], pHostInfo->h_length);
	svrAddr.sin_port = htons((unsigned short)iProxyPort);
  
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == -1)   return -4;//socket����ʧ��
  
	if (connect(sock,(struct sockaddr*)&svrAddr, sizeof(svrAddr)) ==-1)
		return -5;//���Ӵ��������ʧ��
  
	m_proxyreq1 = (struct sock5req1 *)buf; 
	m_proxyreq1->Ver = 5; 
	m_proxyreq1->nMethods = 2; 
	m_proxyreq1->Methods[0] = 0; 
	m_proxyreq1->Methods[1] = 2; 
  //ClientSock.Send(buf,4); 
	if (WaitSomeTime(sock, g_iOvertime, SEND) ==1)
	{
		cbSend = send(sock, buf, 4, 0);
	}
	else
	{
		return -101;
	}
	if (cbSend < 1){
	  MongateDisconnect(sock);
	  return -102;//��һ����������������������
	}

	m_proxyans1 = (struct sock5ans1 *)buf; 
	memset(buf, 0, sizeof(buf));
  //ClientSock.Receive(buf,600); 
	if (WaitSomeTime(sock, g_iOvertime, RECV) == 1)
		cbRecv = recv(sock, buf, 100,0);  
	else{
		MongateDisconnect(sock);
		return -103; //���մ����������һ����Ӧ����
	}
	if (cbRecv<1) {
		return -104;
	}
  
	if(m_proxyans1->Ver != 5||(m_proxyans1->Method!=0 && m_proxyans1->Method!=2)) 	
	{ 	
		MongateDisconnect(sock);
		return -6; //�����������֧��socket5
	} 
	if(m_proxyans1->Method == 2) 	
	{ 	  
		memset(buf,0,sizeof(buf));
		sprintf((char *)buf,"  %s %s",pszProxyAccount,pszProxyPwd);
		buf[0]=1;
		buf[1]=nUserLen;
		buf[2+nUserLen]=nPassLen;
		cbSend = send(sock, buf,strlen(buf), 0);
		if (cbSend < 1){
			MongateDisconnect(sock);
			return -110;//��֤��������������������
		}
		m_authans = (struct authans *)buf; 
		memset(buf,0,sizeof(buf)); 
		if (WaitSomeTime(sock, g_iOvertime, RECV) == 1)
			cbRecv = recv(sock, buf, 100,0);  
		else{
			MongateDisconnect(sock);
			return -111; ///��֤��������������������
		}

		if(m_authans->Ver != 1 || m_authans->Status != 0) 
		{ 
			MongateDisconnect(sock);		  
			return -112;  //����������û���֤ʧ��
		} 
	} 
  
	m_proxyreq2 = (struct sock5req2 *)buf; 
	m_proxyreq2->Ver = 5; 
	m_proxyreq2->Cmd = 1; 
	m_proxyreq2->Rsv = 0; 
	m_proxyreq2->Atyp = 1;
	memcpy(m_proxyreq2->other,&tmpLong,4); 
	memcpy(m_proxyreq2->other+4,&port,2); 
	cbSend = send(sock, buf,sizeof(struct sock5req2)+5, 0);
	if (cbSend < 1){
		  MongateDisconnect(sock);
		  return -113;//��֤��������������������
	}
		
	m_proxyans2 = (struct sock5ans2 *)buf; 	
	memset(buf,0,sizeof(buf)); 	
	if (WaitSomeTime(sock, g_iOvertime, RECV) == 1)
		cbRecv = recv(sock, buf, 100,0);  
	else{
		MongateDisconnect(sock);
		return -114; ///��֤��������������������
	}

	if(m_proxyans2->Ver != 5 || m_proxyans2->Rep != 0) 
	{ 
		MongateDisconnect(sock);
		return -115;//ͨ������������վ���ɹ�!	
	} 		
	
	memset(buf, 0, sizeof(buf));
	if (WaitSomeTime(sock, g_iOvertime, RECV) == 1)
		cbRecv = recv(sock, buf, sizeof(buf), 0);
	else{
		MongateDisconnect(sock);
		return -116;//connect�ɹ�,�ȴ�ƽ̨��������ȷ����Ϣ��ʱ  
	}
	
	if (!(buf[0] == 'O' && buf[1] == 'K')){
		MongateDisconnect(sock);
		return -117;//�ȴ�����������ʧ��
	}


	sprintf(buf,"ZZ%06ld,20,*,*,*,%d,%s,%s,%s,%s",strlen(pszAccount)+strlen(pszPwd)+33,
		 g_iCmdType,g_sVersion,g_Os,pszAccount,pszPwd);//��½ָ��
	buf[8] = ','; 
	if (WaitSomeTime(sock, g_iOvertime, SEND) ==1)
	{
		cbSend = send(sock, buf, strlen(buf), 0);
	}
	else
	{
		return -105;
	}
	if (cbSend < 1){
		MongateDisconnect(sock);
		return -106;//���͵�½ƽָ̨�����
	}

	memset(buf, 0, sizeof(buf));
	if (WaitSomeTime(sock, g_iOvertime, RECV) == 1)
		cbRecv = recv(sock, buf, sizeof(buf),0);
	else{
		MongateDisconnect(sock);
		return -107; //����ƽ̨���ص�½��֤��Ϣ��ʱ
	}
	if (cbRecv<1) {
		return -108;
	}

	if (buf[0] == 'E' && buf[1] == 'R'){ 	
		MongateDisconnect(sock);
		return -10000 - (atoi(buf + 3)); //����ƽ̨������Ϣ
	}
	else if (buf[0] == 'O' && buf[1] == 'K')
		return sock;
	else{
		MongateDisconnect(sock);
		return -200;//����ƽ̨������֤��Ϣ����
	}	
}
int __stdcall MongateConnectHttpProxy(const char* pszProxyIP,
  int iProxyPort,const char* pszProxyAccount,const char* pszProxyPwd,
  const char* pszIP, int iPort, const char* pszAccount, const char* pszPwd)
{
	char buf[1024]={0},httpbuf[1025]={0},reg[100]={0};
	int cbRecv = 0;
	int cbSend = 0;
	struct hostent* pHostInfo = NULL;
	struct sockaddr_in svrAddr;
	SOCKET sock = 0;
  
	g_ConnectType=1;
  //�������
	if (pszProxyIP == NULL || pszIP == NULL || pszAccount == NULL || pszPwd == NULL)
		return -1;//��������У�������NULLֵ

	if (strlen(pszAccount) > 32 || strlen(pszPwd) > 32)
		return -2;//�Ƿ����ʺŻ�����
// 	sprintf(reg,"+%s+%d+",pszAccount,g_iCmdType);
// 	//srcStrTohexChr(reg,buf);
// 	MD5String(buf,reg);
// 	memset(reg,0,33);
// 	strncpy(reg,EncodeID+18,32);
// 	if(strcmp(buf,reg) != 0)
// 	{//δ��Ȩ
// 		return 0;
// 	}
// 	sprintf(reg,",%d,%s,%s,",g_iCmdType,"888888",pszAccount);
// //	srcStrTohexChr(buff,buf);
// 	MD5String(buf,reg);
// 	memset(reg,0,33);
// 	strncpy(reg,EncodeID+115,32);
// 	if(strcmp(buf,reg) != 0)
// 	{//δ��Ȩ
// 		return 0;
// 	} 
  //get host infomation and set svrAddr
	strcpy(Server,pszIP);
	httpproxy = 1;
	pHostInfo = gethostbyname(pszProxyIP);
	if (pHostInfo == NULL)
	{
		pHostInfo = gethostbyaddr(pszProxyIP, strlen(pszProxyIP), AF_INET);
		if (pHostInfo == NULL)
		{
			return -3;//��ȡԶ��������Ϣʧ��,ԭ��:IP����ʧ�ܻ���Ч��IP,�����粻ͨ
		}
	}
	svrAddr.sin_family = AF_INET;
	memcpy(&svrAddr.sin_addr, pHostInfo->h_addr_list[0], pHostInfo->h_length);
	svrAddr.sin_port = htons((unsigned short)iProxyPort);
  
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == -1)   
		return -4;//socket����ʧ��
  
	if (connect(sock,(struct sockaddr*)&svrAddr, sizeof(svrAddr)) ==-1)
		return -5;//���Ӵ��������ʧ��	 
// 	if(haspwd == 0)
// 	{	
// 		sprintf( buf, "CONNECT %s:%d%s%s%s%s%s",
// 			pszIP,iPort,
// 			" HTTP/1.1",	
//          "\r\nAccept: */*\r\n",			
//   		"Content-Type: text/html\r\n",
//             "Proxy-Connection: Keep-Alive\r\n",
//             "Content-length: 0\r\n\r\n");
// 	}
// 	else
// 	{
// 		sprintf(httpbuf,"%s:%s",pszProxyAccount,pszProxyPwd);
// 		sprintf( buf, "CONNECT %s:%d%s%s%s%s%s%s%s",
// 	        pszIP,iPort,
// 			" HTTP/1.1\r\n",
//             "Proxy-Authorization: Basic ",
// 			Base64Encode(httpbuf,strlen(httpbuf)),
//             "\r\nAccept: */*\r\n",
//             "Content-Type: text/html\r\n",
//             "Proxy-Connection: Keep-Alive\r\n",
//             "Content-length: 0\r\n\r\n");
// 	}
// 	if (WaitSomeTime(sock, g_iOvertime, SEND) ==1)
// 	{
// 		cbSend = send(sock, buf, strlen(buf), 0);
// 	}
// 	else{
// 		return -101;
// 	}
// 	
// 	if (cbSend < 1){
// 		MongateDisconnect(sock);
// 		return -102;//���ʹ����������������ʧ��
// 	}
//   
// 	memset(buf, 0, sizeof(buf));
// 	if (WaitSomeTime(sock, g_iOvertime, RECV) == 1)
// 		cbRecv = recv(sock, buf, sizeof(buf),0);
// 	else{
// 		MongateDisconnect(sock);
// 		return -103;//���մ����������֤��Ϣ��ʱ
// 	} 
// 	if (cbRecv<1) {
// 		MongateDisconnect(sock);
// 		return -104;
// 	}
// 
// 	if(strstr(buf, "HTTP/1.0 200 Connection established") == NULL) //���Ӳ��ɹ� 
// 	{
// 		MongateDisconnect(sock);
// 		return -104;//ͨ������������վ���ɹ�
// 	}
	if(strlen(pszProxyAccount)>0)
	{
		sprintf(httpbuf,"%s:%s",pszProxyAccount,pszProxyPwd);
		strcpy(proxyEncode,Base64Encode(httpbuf,strlen(httpbuf)));//������֤����
	}
	else
	{
		strcpy(proxyEncode,"");
	}
	memset(buf, 0, sizeof(buf));
	sprintf(buf,"ZZ%06ld,20,*,*,*,%d,%s,%s,%s,%s",strlen(pszAccount)+strlen(pszPwd)+33,
		 g_iCmdType,g_sVersion,g_Os,pszAccount,pszPwd);//��½ָ��
	buf[8] = ',';    
  
	MakeHttpHead(buf,httpbuf);  
	if (WaitSomeTime(sock, g_iOvertime, SEND) ==1)
	{
		cbSend = send(sock, httpbuf, strlen(httpbuf), 0);
	}
	else{
		return -105;
	}
	if (cbSend < 1){
		MongateDisconnect(sock);
		return -106; //������ƽ̨�ύ����ʧ��
	}
  
	memset(buf, 0, sizeof(buf));
	if (WaitSomeTime(sock, g_iOvertime, RECV) == 1)
		cbRecv = recv(sock, buf, sizeof(buf),0);
	else{
		MongateDisconnect(sock);
		return -107; //����ƽ̨���ص�½��֤��Ϣ��ʱ
	}
	if (cbRecv<1) {
		MongateDisconnect(sock);
		return -108; //����ƽ̨���ص�½��֤��Ϣʧ��
	}
	memset(httpbuf,0,sizeof(httpbuf));
	GetHttpRet(buf,httpbuf);
	if (httpbuf[0] == 'E' && httpbuf[1] == 'R'){
		MongateDisconnect(sock);
		return -10000 - (atoi(httpbuf + 3)); //����ƽ̨������Ϣ
	}
	else if (httpbuf[0] == 'O' && httpbuf[1] == 'K')
		return sock;
	else{
		MongateDisconnect(sock);
		return -200;//���շ���������ʧ��
	}
}
//*************
//* Java �ӿ� *
//*************
void JNU_ThrowByName(JNIEnv *env, const char *name, const char *msg) 
{ 
  jclass cls;
  
  cls = (*env)->FindClass(env,name); 
  // if cls is NULL, an exception has already been thrown
  if (cls != NULL)
  { 
    (*env)->ThrowNew(env,cls, msg); 
  } 
  // free the local ref
  (*env)->DeleteLocalRef(env,cls); 
} 

jstring JNU_NewStringNative(JNIEnv *env, const char *str) 
{ 

  jclass jcls_str;
  jmethodID jmethod_str;
  jstring result; 
  jbyteArray bytes;
  int len; 
  //result=WindowsTojstring(env,str);
  //return result;  
  if (str==NULL)
  { 
    return NULL; 
  } 
  jcls_str = (*env)->FindClass(env,"java/lang/String");   
  jmethod_str = (*env)->GetMethodID(env, jcls_str, "<init>", "([B)V"); 
  
  bytes = 0;   
  if ((*env)->EnsureLocalCapacity(env,2) < 0)
  { 	
    return NULL; // out of memory error
  }   
  len = strlen(str); 
  bytes = (*env)->NewByteArray(env,len); 
    if (bytes != NULL)
  { 
    (*env)->SetByteArrayRegion(env,bytes, 0, len, (jbyte *)str); 	
    result = (jstring)(*env)->NewObject(env,jcls_str, jmethod_str, bytes); 	
    (*env)->DeleteLocalRef(env,bytes); 	
    return result; 
  } // else fall through  
  return NULL; 
} 

//---------------------------------------------------------------------------
//ʹ��char *JNU_GetStringNativeChars()��õ�ָ�������Ҫ��ʽ��free(). 
char *JNU_GetStringNativeChars(JNIEnv *env, jstring jstr) 
{ 
  jbyteArray bytes = 0; 
  jthrowable exc; 
  char *result = 0; 
  jclass jcls_str;
  jmethodID MID_String_getBytes;
  if ((*env)->EnsureLocalCapacity(env,2) < 0)
  { 
    return 0; // out of memory error
  } 
  jcls_str = (*env)->FindClass(env,"java/lang/String"); 
  MID_String_getBytes = (*env)->GetMethodID(env,jcls_str, "getBytes", "()[B"); 
  
  bytes = (jbyteArray)(*env)->CallObjectMethod(env,jstr, MID_String_getBytes); 
  exc = (*env)->ExceptionOccurred(env); 
  if (!exc)
  { 
    jint len = (*env)->GetArrayLength(env,bytes); 
    result = (char *)malloc(len + 1); 
    if (result == 0)
    { 
      JNU_ThrowByName(env, "java/lang/OutOfMemoryError", 0); 
      (*env)->DeleteLocalRef(env,bytes); 
      return 0; 
    } 
    (*env)->GetByteArrayRegion(env,bytes, 0, len, (jbyte *)result); 
    result[len] = 0; // NULL-terminate
  }
  else
  { 
    (*env)->DeleteLocalRef(env,exc); 
  } 
  (*env)->DeleteLocalRef(env,bytes); 
  return (char*)result; 
}

//---------------------------------------------------------------------------
//http�ӿ�
JNIEXPORT jint JNICALL Java_montnets_MWGateway_ConnectHttp(JNIEnv * env,
  jclass cls, jstring strIP, jint iPort, jstring strAccount, jstring strPwd)
{
  char* pszIP = JNU_GetStringNativeChars(env, strIP);
  char* pszAccount = JNU_GetStringNativeChars(env, strAccount);
  char* pszPwd = JNU_GetStringNativeChars(env, strPwd);
  int ret = MongateConnectHttp(pszIP, iPort, pszAccount, pszPwd);
  
  free(pszIP);
  free(pszAccount);
  free(pszPwd);
  return ret;
}

//---------------------------------------------------------------------------
//
JNIEXPORT jint JNICALL Java_montnets_MWGateway_TestConnHttp
(JNIEnv *env, jclass cls, jint sock)
{
  return MongateTestConnHttp(sock);
}
//---------------------------------------------------------------------------
//
JNIEXPORT jint JNICALL Java_montnets_MWGateway_QueryBalanceHttp(JNIEnv * env,
  jclass cls, jint sock)
{
  return MongateQueryBalanceHttp(sock);
}

//---------------------------------------------------------------------------
//
JNIEXPORT jint JNICALL Java_montnets_MWGateway_QueryUsedHttp
(JNIEnv *env, jclass cls, jint sock)
{
  return MongateQueryUsedHttp(sock);
}

//---------------------------------------------------------------------------
//
JNIEXPORT jint JNICALL Java_montnets_MWGateway_RechargeHttp
(JNIEnv *env, jclass cls, jint sock, jstring strCardNo, jstring strCardPwd)
{
  char* pszCardNo = JNU_GetStringNativeChars(env, strCardNo);
  char* pszCardPwd = JNU_GetStringNativeChars(env, strCardPwd);
  int ret = MongateRechargeHttp(sock, pszCardNo, pszCardPwd);
  free(pszCardNo);
  free(pszCardPwd);
  return ret;
}
//---------------------------------------------------------------------------
//
JNIEXPORT jint JNICALL Java_montnets_MWGateway_ChangePwdHttp(JNIEnv *env, jclass cls,
  jint sock, jstring strAccount, jstring strOldPwd, jstring strNewPwd)
{
  char* pszAccount = JNU_GetStringNativeChars(env, strAccount);
  char* pszOldPwd = JNU_GetStringNativeChars(env, strOldPwd);
  char* pszNewPwd = JNU_GetStringNativeChars(env, strNewPwd);
  int ret = MongateChangePwdHttp(sock, pszAccount, pszOldPwd, pszNewPwd);

  free(pszAccount);
  free(pszOldPwd);
  free(pszNewPwd);
  return ret;
}


//---------------------------------------------------------------------------
//
JNIEXPORT jobjectArray JNICALL Java_montnets_MWGateway_CsGetSmsHttp
  (JNIEnv * env, jclass cls, jint sock)
{
  int res = -99;
  int i = 0;
  char arrMsg[500][255];// = {0};
  char arrState[20] = {0};
  jstring strMsg;
  jobjectArray retVal;  
  for(i=0;i<500;i++)
	  memset(arrMsg[i],0,sizeof(arrMsg[i]));  
  res = MongateCsGetSmsHttp(sock, (char(*)[255])arrMsg);  
  if (res < 0)
  {
    retVal = (*env)->NewObjectArray(env, 1, (*env)->FindClass(env,"java/lang/String"), 0);
    sprintf(arrState, "ER,%d", res);
    strMsg = JNU_NewStringNative(env, arrState);
    (*env)->SetObjectArrayElement(env, retVal, 0, strMsg);
  }
  else if (res == 0)
  {	
    retVal = (*env)->NewObjectArray(env, 1, (*env)->FindClass(env,"java/lang/String"), 0);
    sprintf(arrState,"%s","OK,0");	
    strMsg = JNU_NewStringNative(env, arrState);	
    (*env)->SetObjectArrayElement(env, retVal, 0, strMsg);	
  }
  else
  {	
    retVal = (*env)->NewObjectArray(env, res, (*env)->FindClass(env,"java/lang/String"), 0);
    //��һ����ϢΪ״̬���ڶ�����ʼ���ǽ��յ�����Ϣ
    for (i = 0; i < res; i++)
    {
      strMsg = JNU_NewStringNative(env, arrMsg[i]);
	  (*env)->SetObjectArrayElement(env, retVal, i, strMsg);
      //(*env)->SetObjectArrayElement(env, retVal, i + 1, strMsg);
    }
  }  
  return retVal;
}


//---------------------------------------------------------------------------
JNIEXPORT jstring JNICALL Java_montnets_MWGateway_CsGetSmsHttpEx
(JNIEnv * env, jclass cls, jint sock)
{
	int res = -99;
	char arrMsg[20480] = {0};
	res = MongateCsGetSmsHttpEx(sock, arrMsg);
	if (res > 0)
		return JNU_NewStringNative(env, arrMsg); 
	else
	{
		sprintf(arrMsg, "Err,%d", res);
		return JNU_NewStringNative(env, arrMsg);
	}
}


//---------------------------------------------------------------------------

JNIEXPORT jstring JNICALL Java_montnets_MWGateway_CsSendSmsHttp
  (JNIEnv * env, jclass cls, jint sock, jstring strMobis, jstring strMsg,
  jint iMobiCount)
{
  char* pszMobis = JNU_GetStringNativeChars(env, strMobis);
  char* pszMsg = JNU_GetStringNativeChars(env, strMsg);
  char arrSN[50] = {0};

  int ret = MongateCsSendSmsHttp(sock, pszMobis, pszMsg, iMobiCount, arrSN);
  free(pszMobis);
  free(pszMsg);
  
  if (ret > 0)
   return JNU_NewStringNative(env, arrSN); 
  else
  {
    sprintf(arrSN, "Err,%d", ret);
    return JNU_NewStringNative(env, arrSN);
  }
}

//---------------------------------------------------------------------------
//
JNIEXPORT jobjectArray JNICALL Java_montnets_MWGateway_CsGetStatusReportHttp
(JNIEnv *env, jclass cls, jint sock)
{
  int res = -99;
  int i = 0;
  char arrReport[500][255] = {0};
  char arrState[20] = {0};
  jstring strMsg;
  jobjectArray retVal;  
  res = MongateCsGetStatusReportHttp(sock, arrReport);
  
  //test
  /*retVal = (*env)->NewObjectArray(env, 1, (*env)->FindClass(env,"java/lang/String"), 0);
    sprintf(arrState, "Test,%d", res);
    strMsg = JNU_NewStringNative(env, arrState);
    (*env)->SetObjectArrayElement(env, retVal, 0, strMsg); //test
  return retVal;*/

  if (res < 0)
  {
    retVal = (*env)->NewObjectArray(env, 1, (*env)->FindClass(env,"java/lang/String"), 0);
    sprintf(arrState, "ER,%d", res);
    strMsg = JNU_NewStringNative(env, arrState);
    (*env)->SetObjectArrayElement(env, retVal, 0, strMsg);
  }
  else if (res == 0)
  {
    retVal =  (*env)->NewObjectArray(env, 1, (*env)->FindClass(env,"java/lang/String"), 0);
    sprintf(arrState, "OK,0");
    strMsg = JNU_NewStringNative(env, arrState);
    (*env)->SetObjectArrayElement(env, retVal, 0, strMsg);
  }
  else
  {
    retVal  = (*env)->NewObjectArray(env, res, (*env)->FindClass(env,"java/lang/String"), 0);    
    for (i = 0; i < res; i++)
    {
      strMsg = JNU_NewStringNative(env, arrReport[i]);
	  (*env)->SetObjectArrayElement(env, retVal, i, strMsg);    
    }
  }

  return retVal;
}

//--------------------------------------------------------------------------
JNIEXPORT jstring JNICALL Java_montnets_MWGateway_CsGetStatusReportHttpEx
(JNIEnv *env, jclass cls, jint sock)
{
  int res = -99;
  int i = 0;
  char arrReport[20480] = {0};
  res = MongateCsGetStatusReportHttpEx(sock, arrReport);
  if (res > 0)
	  return JNU_NewStringNative(env, arrReport); 
  else
  {
	  sprintf(arrReport, "Err,%d", res);
	  return JNU_NewStringNative(env, arrReport);
  }	
}

//--------------------------------------------------------------------------
JNIEXPORT jint JNICALL Java_montnets_MWGateway_Connect(JNIEnv * env,
  jclass cls, jstring strIP, jint iPort, jstring strAccount, jstring strPwd)
{
  char* pszIP = JNU_GetStringNativeChars(env, strIP);
  char* pszAccount = JNU_GetStringNativeChars(env, strAccount);
  char* pszPwd = JNU_GetStringNativeChars(env, strPwd);
  int ret = MongateConnect(pszIP, iPort, pszAccount, pszPwd);
  
  free(pszIP);
  free(pszAccount);
  free(pszPwd);
  return ret;
}
//---------------------------------------------------------------------------
//
JNIEXPORT jint JNICALL Java_montnets_MWGateway_SendSms
(JNIEnv *env, jclass cls, jint sock, jstring strMobi, jstring strMsg)
{
  char* pszMobi = JNU_GetStringNativeChars(env, strMobi);
  char* pszMsg  = JNU_GetStringNativeChars(env, strMsg);
  char arrSN[50] = {0};
  int iMobiCount=1;
  int ret = MongateCsSendSms(sock, pszMobi, pszMsg, iMobiCount, arrSN);
  //int ret = MongateCsSendSms(sock, pszMobi, pszMsg);
  
  free(pszMobi);
  free(pszMsg);
  return ret;
}
//---------------------------------------------------------------------------
//
JNIEXPORT jint JNICALL Java_montnets_MWGateway_TestConn
(JNIEnv *env, jclass cls, jint sock)
{
  return MongateTestConn(sock);
}
//---------------------------------------------------------------------------
//
JNIEXPORT jint JNICALL Java_montnets_MWGateway_QueryBalance(JNIEnv * env,
  jclass cls, jint sock)
{
  return MongateQueryBalance(sock);
}

//---------------------------------------------------------------------------
//
JNIEXPORT jint JNICALL Java_montnets_MWGateway_QueryUsed
(JNIEnv *env, jclass cls, jint sock)
{
  return MongateQueryUsed(sock);
}

//---------------------------------------------------------------------------
//
JNIEXPORT jint JNICALL Java_montnets_MWGateway_Recharge
(JNIEnv *env, jclass cls, jint sock, jstring strCardNo, jstring strCardPwd)
{
  char* pszCardNo = JNU_GetStringNativeChars(env, strCardNo);
  char* pszCardPwd = JNU_GetStringNativeChars(env, strCardPwd);
  int ret = MongateRecharge(sock, pszCardNo, pszCardPwd);
  free(pszCardNo);
  free(pszCardPwd);
  return ret;
}
//---------------------------------------------------------------------------
//
JNIEXPORT jint JNICALL Java_montnets_MWGateway_ChangePwd(JNIEnv *env, jclass cls,
  jint sock, jstring strAccount, jstring strOldPwd, jstring strNewPwd)
{
  char* pszAccount = JNU_GetStringNativeChars(env, strAccount);
  char* pszOldPwd = JNU_GetStringNativeChars(env, strOldPwd);
  char* pszNewPwd = JNU_GetStringNativeChars(env, strNewPwd);
  int ret = MongateChangePwd(sock, pszAccount, pszOldPwd, pszNewPwd);

  free(pszAccount);
  free(pszOldPwd);
  free(pszNewPwd);
  return ret;
}
//---------------------------------------------------------------------------
//
JNIEXPORT void JNICALL Java_montnets_MWGateway_Disconnect
  (JNIEnv * env, jclass cls, jint sock)
{
   MongateDisconnect(sock);
}

//---------------------------------------------------------------------------
//
JNIEXPORT jobjectArray JNICALL Java_montnets_MWGateway_CsGetSms
  (JNIEnv * env, jclass cls, jint sock)
{
  int res = -99;
  int i = 0;
  char arrMsg[500][255];// = {0};
  char arrState[20] = {0};
  jstring strMsg;
  jobjectArray retVal;  
  for(i=0;i<500;i++)memset(arrMsg[i],0,sizeof(arrMsg[i]));
  res = MongateCsGetSms(sock, (char(*)[255])arrMsg);
  if (res < 0)
  {
    retVal = (*env)->NewObjectArray(env, 1, (*env)->FindClass(env,"java/lang/String"), 0);
    sprintf(arrState, "ER,%d", res);
    strMsg = JNU_NewStringNative(env, arrState);
    (*env)->SetObjectArrayElement(env, retVal, 0, strMsg);
  }
  else if (res == 0)
  {	
    retVal = (*env)->NewObjectArray(env, 1, (*env)->FindClass(env,"java/lang/String"), 0);
    sprintf(arrState,"%s","OK,0");
	
    strMsg = JNU_NewStringNative(env, arrState);
		
    (*env)->SetObjectArrayElement(env, retVal, 0, strMsg);	
  }
  else
  {	
    retVal = (*env)->NewObjectArray(env, res, (*env)->FindClass(env,"java/lang/String"), 0);
    //��һ����ϢΪ״̬���ڶ�����ʼ���ǽ��յ�����Ϣ
    for (i = 0; i < res; i++)
    {
      strMsg = JNU_NewStringNative(env, arrMsg[i]);
	  (*env)->SetObjectArrayElement(env, retVal, i, strMsg);
      //(*env)->SetObjectArrayElement(env, retVal, i + 1, strMsg);
    }
  }  
  return retVal;
}

JNIEXPORT jstring JNICALL Java_montnets_MWGateway_CsGetSmsEx
(JNIEnv * env, jclass cls, jint sock)
{
	int res = -99;
	char arrMsg[20480]= {0};
	memset(arrMsg,0,sizeof(arrMsg));
	res = MongateCsGetSmsEx(sock, arrMsg);
	if (res >= 0)
		return JNU_NewStringNative(env, arrMsg); 
	else
	{
		sprintf(arrMsg, "Err,%d", res);
		return JNU_NewStringNative(env, arrMsg);
	}
}
//---------------------------------------------------------------------------
JNIEXPORT jobjectArray JNICALL Java_montnets_MWGateway_VasGetSms
(JNIEnv *env, jclass cls, jint sock)
{
  int res = -99;
  int i = 0;
  char arrMsg[500][255] = {0};
  char arrState[20] = {0};
  jstring strMsg;
  jobjectArray retVal;
  
  res = MongateVasGetSms(sock, (char(*)[255])arrMsg);
  
  if (res < 0)
  {
    retVal = (*env)->NewObjectArray(env, 1, (*env)->FindClass(env,"java/lang/String"), 0);
    sprintf(arrState, "ER,%d", res);
    strMsg = JNU_NewStringNative(env, arrState);
    (*env)->SetObjectArrayElement(env, retVal, 0, strMsg);
  }
  else if (res == 0)
  {
    retVal = (*env)->NewObjectArray(env, 1, (*env)->FindClass(env,"java/lang/String"), 0);
    sprintf(arrState, "OK,0");
    strMsg = JNU_NewStringNative(env, arrState);
    (*env)->SetObjectArrayElement(env, retVal, 0, strMsg);
  }
  else
  {
    retVal = (*env)->NewObjectArray(env, res, (*env)->FindClass(env,"java/lang/String"), 0);
    //��һ����ϢΪ״̬���ڶ�����ʼ���ǽ��յ�����Ϣ
    //sprintf(arrState, "OK,%0d", res);
    //strMsg = JNU_NewStringNative(env, arrState);
    //(*env)->SetObjectArrayElement(env, retVal, 0, strMsg);
    
    for (i = 0; i < res; i++)
    {
      strMsg = JNU_NewStringNative(env, arrMsg[i]);
	  (*env)->SetObjectArrayElement(env, retVal, i, strMsg);
      //(*env)->SetObjectArrayElement(env, retVal, i + 1, strMsg);
    }
  }
  
  return retVal;
}
//---------------------------------------------------------------------------
//
JNIEXPORT jint JNICALL Java_montnets_MWGateway_VasSendSms(JNIEnv *env, jclass cls,
  jint sock, jstring strFeeCode,jstring strServiceNo, jstring strOperCode,
  jstring strLinkID, jstring strMobis, jstring strMsg, jint iMobiCount)
{
  char* pszFeeCode = JNU_GetStringNativeChars(env, strFeeCode);
  char* pszServiceNo = JNU_GetStringNativeChars(env, strServiceNo);
  char* pszOperCode = JNU_GetStringNativeChars(env, strOperCode);
  char* pszLinkID = JNU_GetStringNativeChars(env, strLinkID);
  char* pszMobis = JNU_GetStringNativeChars(env, strMobis);
  char* pszMsg = JNU_GetStringNativeChars(env, strMsg);
  int res = MongateVasSendSms(sock, pszFeeCode, pszServiceNo, pszOperCode,
    pszLinkID, pszMobis, pszMsg, iMobiCount);
  free(pszFeeCode);
  free(pszServiceNo);
  free(pszOperCode);
  free(pszLinkID);
  free(pszMobis);
  free(pszMsg);
  return res;
}

//---------------------------------------------------------------------------
// 
JNIEXPORT jstring JNICALL Java_montnets_MWGateway_CsSendSms
  (JNIEnv * env, jclass cls, jint sock, jstring strMobis, jstring strMsg,
  jint iMobiCount)
{
  char* pszMobis = JNU_GetStringNativeChars(env, strMobis);
  char* pszMsg = JNU_GetStringNativeChars(env, strMsg);
  char arrSN[50] = {0};
  int ret = MongateCsSendSms(sock, pszMobis, pszMsg, iMobiCount, arrSN);
  free(pszMobis);
  free(pszMsg);
  
  if (ret > 0)
   return JNU_NewStringNative(env, arrSN); 
  else
  {
    sprintf(arrSN, "Err,%d", ret);
    return JNU_NewStringNative(env, arrSN);
  }
}
//---------------------------------------------------------------------------
JNIEXPORT jstring JNICALL Java_montnets_MWGateway_MGhexChrTosrcStr
(JNIEnv * env, jclass cls,jstring strMsg)
{
	char* pszMsg = JNU_GetStringNativeChars(env, strMsg);
	char arrSN[255] = {0};
	
	int ret = MGhexChrTosrcStr(pszMsg, arrSN);
	free(pszMsg);	
	if (ret > 0)
		return JNU_NewStringNative(env, arrSN); 
	else
	{
		sprintf(arrSN, "Err,%d", ret);
		return JNU_NewStringNative(env, arrSN);
	}
}
//---------------------------------------------------------------------------

JNIEXPORT jobjectArray JNICALL Java_montnets_MWGateway_CsGetStatusReport
(JNIEnv *env, jclass cls, jint sock)
{
  int res = -99;
  int i = 0;
  char arrReport[500][255] = {0};
  char arrState[20] = {0};
  jstring strMsg;
  jobjectArray retVal;

  res = MongateCsGetStatusReport(sock, arrReport);
  if (res < 0)
  {
    retVal = (*env)->NewObjectArray(env, 1, (*env)->FindClass(env,"java/lang/String"), 0);
    sprintf(arrState, "ER,%d", res);
    strMsg = JNU_NewStringNative(env, arrState);
    (*env)->SetObjectArrayElement(env, retVal, 0, strMsg);
  }
  else if (res == 0)
  {
    retVal =  (*env)->NewObjectArray(env, 1, (*env)->FindClass(env,"java/lang/String"), 0);
    sprintf(arrState, "OK,0");
    strMsg = JNU_NewStringNative(env, arrState);
    (*env)->SetObjectArrayElement(env, retVal, 0, strMsg);
  }
  else
  {
    retVal  = (*env)->NewObjectArray(env, res, (*env)->FindClass(env,"java/lang/String"), 0);
    //sprintf(arrState, "OK,%0d", res);
    //strMsg = JNU_NewStringNative(env, arrState);
    //(*env)->SetObjectArrayElement(env, retVal, 0, strMsg);
    for (i = 0; i < res; i++)
    {
      strMsg = JNU_NewStringNative(env, arrReport[i]);
	  (*env)->SetObjectArrayElement(env, retVal, i, strMsg);
      //(*env)->SetObjectArrayElement(env, retVal, i + 1, strMsg);
    }
  }
  return retVal;
}
//---------------------------------------------------------------------------
JNIEXPORT jstring JNICALL Java_montnets_MWGateway_CsGetStatusReportEx
(JNIEnv *env, jclass cls, jint sock)
{
	int res = -99;
	int i = 0;
	char arrReport[20480] = {0};
	memset(arrReport,0,sizeof(arrReport));
	res = MongateCsGetStatusReportEx(sock, arrReport);
	if (res > 0)
		return JNU_NewStringNative(env, arrReport); 
	else
	{
		sprintf(arrReport, "Err,%d", res);
		return JNU_NewStringNative(env, arrReport);
	}	
}
//---------------------------------------------------------------------------
//
JNIEXPORT jstring JNICALL Java_montnets_MWGateway_GetVer(JNIEnv *env, jclass cls)
{
  return JNU_NewStringNative(env, MongateGetVer());
}

JNIEXPORT jstring JNICALL Java_montnets_MWGateway_CsSendTime
(JNIEnv * env, jclass cls, jint sock, jint iType,jstring sBeginDate, jstring sEndDate,
 jint iFlag,jstring sTime, jint iMobiCount,jstring sszMsg, jstring sTelNos)
{
	char* pBeginDate = JNU_GetStringNativeChars(env, sBeginDate);
	char* pEndDate = JNU_GetStringNativeChars(env, sEndDate);
	char* pTime = JNU_GetStringNativeChars(env, sTime);
	char* pszMsg = JNU_GetStringNativeChars(env, sszMsg);
	char* pTelNos = JNU_GetStringNativeChars(env, sTelNos);
	char arrSN[50]={0};
	int ret = MongateCsSendTime(sock,iType,pBeginDate,pEndDate,iFlag,pTime,iMobiCount,pszMsg,pTelNos);
	free(pBeginDate);
	free(pEndDate);
	free(pTime);
	free(pszMsg);
	free(pTelNos);
	sprintf(arrSN, "Err,%d", ret);
	return JNU_NewStringNative(env, arrSN); 
}
JNIEXPORT jstring JNICALL Java_montnets_MWGateway_CsSendTimeHttp
(JNIEnv * env, jclass cls, jint sock, jint iType,jstring sBeginDate, jstring sEndDate,
 jint iFlag,jstring sTime, jint iMobiCount,jstring sszMsg, jstring sTelNos)
{
	char* pBeginDate = JNU_GetStringNativeChars(env, sBeginDate);
	char* pEndDate = JNU_GetStringNativeChars(env, sEndDate);
	char* pTime = JNU_GetStringNativeChars(env, sTime);
	char* pszMsg = JNU_GetStringNativeChars(env, sszMsg);
	char* pTelNos = JNU_GetStringNativeChars(env, sTelNos);
	char arrSN[50]={0};
	int ret = MongateCsSendTimeHttp(sock,iType,pBeginDate,pEndDate,iFlag,pTime,iMobiCount,pszMsg,pTelNos);
	free(pBeginDate);
	free(pEndDate);
	free(pTime);
	free(pszMsg);
	free(pTelNos);
	sprintf(arrSN, "Err,%d", ret);
	return JNU_NewStringNative(env, arrSN); 
}

JNIEXPORT jstring JNICALL Java_montnets_MWGateway_CsUpdateSendTime
(JNIEnv * env, jclass cls, jint sock, jint iNumber,jint iType,jstring sBeginDate, jstring sEndDate,
 jint iFlag,jstring sTime, jint iMobiCount,jstring sszMsg, jstring sTelNos)
{
	char* pBeginDate = JNU_GetStringNativeChars(env, sBeginDate);
	char* pEndDate = JNU_GetStringNativeChars(env, sEndDate);
	char* pTime = JNU_GetStringNativeChars(env, sTime);
	char* pszMsg = JNU_GetStringNativeChars(env, sszMsg);
	char* pTelNos = JNU_GetStringNativeChars(env, sTelNos);
	char arrSN[50]={0};
	int ret = MongateCsUpdateSendTime(sock,iNumber,iType,pBeginDate,pEndDate,iFlag,pTime,iMobiCount,pszMsg,pTelNos);
	free(pBeginDate);
	free(pEndDate);
	free(pTime);
	free(pszMsg);
	free(pTelNos);
	sprintf(arrSN, "Err,%d", ret);
	return JNU_NewStringNative(env, arrSN); 
}
JNIEXPORT jstring JNICALL Java_montnets_MWGateway_CsUpdateSendTimeHttp
(JNIEnv * env, jclass cls, jint sock, jint iNumber,jint iType,jstring sBeginDate, jstring sEndDate,
 jint iFlag,jstring sTime, jint iMobiCount,jstring sszMsg, jstring sTelNos)
{
	char* pBeginDate = JNU_GetStringNativeChars(env, sBeginDate);
	char* pEndDate = JNU_GetStringNativeChars(env, sEndDate);
	char* pTime = JNU_GetStringNativeChars(env, sTime);
	char* pszMsg = JNU_GetStringNativeChars(env, sszMsg);
	char* pTelNos = JNU_GetStringNativeChars(env, sTelNos);
	char arrSN[50]={0};
	int ret = MongateCsUpdateSendTimeHttp(sock,iNumber,iType,pBeginDate,pEndDate,iFlag,pTime,iMobiCount,pszMsg,pTelNos);
	free(pBeginDate);
	free(pEndDate);
	free(pTime);
	free(pszMsg);
	free(pTelNos);
	sprintf(arrSN, "Err,%d", ret);
	return JNU_NewStringNative(env, arrSN); 
}

JNIEXPORT jstring JNICALL Java_montnets_MWGateway_CsReadSendTimeEx
(JNIEnv * env, jclass cls, jint sock, jint iNumber,jint iType,jstring strMsg)
{
	char* pstrMsg = JNU_GetStringNativeChars(env, strMsg);
	char arrSN[50]={0};
	int ret = MongateCsReadSendTimeEx(sock,iNumber,iType,pstrMsg);
	free(pstrMsg);
	if (ret > 0)
		return JNU_NewStringNative(env, pstrMsg); 
	else
	{
		sprintf(arrSN, "Err,%d", ret);
		return JNU_NewStringNative(env, arrSN);
	}	 
}
JNIEXPORT jstring JNICALL Java_montnets_MWGateway_CsReadSendTimeHttpEx
(JNIEnv * env, jclass cls, jint sock, jint iNumber,jint iType,jstring strMsg)
{
	char* pstrMsg = JNU_GetStringNativeChars(env, strMsg);
	char arrSN[50]={0};
	int ret = MongateCsReadSendTimeHttpEx(sock,iNumber,iType,pstrMsg);
	free(pstrMsg);
	if (ret > 0)
		return JNU_NewStringNative(env, pstrMsg); 
	else
	{
		sprintf(arrSN, "Err,%d", ret);
		return JNU_NewStringNative(env, arrSN);
	}
}

JNIEXPORT jstring JNICALL Java_montnets_MWGateway_CsUndateTelListSendTime
(JNIEnv * env, jclass cls, jint sock, jint iNumber,jint iNum,jstring sTelNos)
{
	char* pTelNos = JNU_GetStringNativeChars(env, sTelNos);
	char arrSN[50]={0};
	int ret = MongateCsUpdateTelListSendTime(sock,iNumber,iNum,pTelNos);
	free(pTelNos);
	if (ret > 0)
		return JNU_NewStringNative(env, pTelNos); 
	else
	{
		sprintf(arrSN, "Err,%d", ret);
		return JNU_NewStringNative(env, arrSN);
	}	 
}
JNIEXPORT jstring JNICALL Java_montnets_MWGateway_CsUndateTelListSendTimeHttp
(JNIEnv * env, jclass cls, jint sock, jint iNumber,jint iNum,jstring sTelNos)
{
	char* pTelNos = JNU_GetStringNativeChars(env, sTelNos);
	char arrSN[50]={0};
	int ret = MongateCsUpdateTelListSendTimeHttp(sock,iNumber,iNum,pTelNos);
	free(pTelNos);
	if (ret > 0)
		return JNU_NewStringNative(env, pTelNos); 
	else
	{
		sprintf(arrSN, "Err,%d", ret);
		return JNU_NewStringNative(env, pTelNos);
	}
}

JNIEXPORT jstring JNICALL Java_montnets_MWGateway_MGQueryAccount
(JNIEnv * env, jclass cls, jint sock,jstring sReserve,jstring sReStr)
{
	char* pReserve = JNU_GetStringNativeChars(env, sReserve);
	char* pReStr = JNU_GetStringNativeChars(env, sReStr);
	char arrSN[50]={0};
	int ret = MGQueryAccount(sock ,pReserve,pReStr);
	free(pReserve);
	if (ret > 0)
		return JNU_NewStringNative(env, pReStr); 
	else
	{
		sprintf(arrSN, "Err,%d", ret);
		return JNU_NewStringNative(env, pReStr);
	}	 
}
#ifdef WIN32
void TestMsg(const char* mystr) 
{ 
	FILE *fp=NULL; 
	//setlocale(LC_ALL, "Chinese_China"); 
	//wchar_t *lpString=L"��ô����"; 
	fp=fopen("C:\\mw_test.txt","at+"); 	
	fprintf(fp,"%s\r\n",mystr); 	
	fclose(fp); 	
}
#endif


//---------------------------------------------------------------------------
// 
JNIEXPORT jstring JNICALL Java_montnets_MWGateway_CsSPSendSms
  (JNIEnv * env, jclass cls, jint sock, jstring strMobis, jstring strMsg,
  jint iMobiCount, jstring strSubPort)//, jint iMsgPkid)
{
  char* pszMobis = JNU_GetStringNativeChars(env, strMobis);
  char* pszMsg = JNU_GetStringNativeChars(env, strMsg);
  char* pszSubPort = JNU_GetStringNativeChars(env, strSubPort);
  char arrSN[50] = {0};

  int ret = MongateCsSPSendSms(sock, pszMobis, pszMsg, iMobiCount, 
	  pszSubPort, arrSN);
	  //pszSubPort, iMsgPkid, arrSN);
  free(pszMobis);
  free(pszMsg);
  
  if (ret > 0)
   return JNU_NewStringNative(env, arrSN); 
  else
  {
    sprintf(arrSN, "Err,%d", ret);
    return JNU_NewStringNative(env, arrSN);
  }
}

//-------------------------------Proxy����-------------------------------
JNIEXPORT jint JNICALL Java_montnets_MWGateway_ConnectSocks4Proxy(JNIEnv * env,
  jclass cls, jstring strProxyIP, jint iProxyPort,jstring strIP, jint iPort, 
  jstring strAccount, jstring strPwd)
{
  char* pszProxyIP = JNU_GetStringNativeChars(env, strProxyIP);
  char* pszIP = JNU_GetStringNativeChars(env, strIP);
  char* pszAccount = JNU_GetStringNativeChars(env, strAccount);
  char* pszPwd = JNU_GetStringNativeChars(env, strPwd);
  //int ret = MonGateConnectSocksFour();
  int ret = MongateConnectSocks4Proxy(pszProxyIP,iProxyPort,
	  pszIP, iPort, pszAccount, pszPwd);  
  free(pszProxyIP);
  free(pszIP);
  free(pszAccount);
  free(pszPwd);
  return ret;
}

JNIEXPORT jint JNICALL Java_montnets_MWGateway_ConnectSocks5Proxy(JNIEnv * env,
  jclass cls, jstring strProxyIP,jint iProxyPort,jstring strProxyAccount, 
  jstring strProxyPwd,jstring strIP,jint iPort,jstring strAccount, jstring strPwd)
{
  char* pszProxyIP = JNU_GetStringNativeChars(env, strProxyIP);
  char* pszProxyAccount = JNU_GetStringNativeChars(env, strProxyAccount);
  char* pszProxyPwd = JNU_GetStringNativeChars(env, strProxyPwd);
  char* pszIP = JNU_GetStringNativeChars(env, strIP);
  char* pszAccount = JNU_GetStringNativeChars(env, strAccount);
  char* pszPwd = JNU_GetStringNativeChars(env, strPwd);
  int ret = MongateConnectSocks5Proxy(
	  pszProxyIP,iProxyPort,pszProxyAccount,pszProxyPwd, 
	  pszIP, iPort, pszAccount, pszPwd);  
  free(pszProxyIP);
  free(pszProxyAccount);
  free(pszProxyPwd);
  free(pszIP);
  free(pszAccount);
  free(pszPwd);
  return ret;
}

JNIEXPORT jint JNICALL Java_montnets_MWGateway_ConnectHttpProxy(JNIEnv * env,
  jclass cls, jstring strProxyIP,jint iProxyPort,jstring strProxyAccount, 
  jstring strProxyPwd,jstring strIP,jint iPort,jstring strAccount, jstring strPwd)
{
  char* pszProxyIP = JNU_GetStringNativeChars(env, strProxyIP);
  char* pszProxyAccount = JNU_GetStringNativeChars(env, strProxyAccount);
  char* pszProxyPwd = JNU_GetStringNativeChars(env, strProxyPwd);
  char* pszIP = JNU_GetStringNativeChars(env, strIP);
  char* pszAccount = JNU_GetStringNativeChars(env, strAccount);
  char* pszPwd = JNU_GetStringNativeChars(env, strPwd);
  int ret = MongateConnectHttpProxy(
	  pszProxyIP,iProxyPort,pszProxyAccount,pszProxyPwd, 
	  pszIP, iPort, pszAccount, pszPwd);  
  free(pszProxyIP);
  free(pszProxyAccount);
  free(pszProxyPwd);
  free(pszIP);
  free(pszAccount);
  free(pszPwd);
  return ret;
}

JNIEXPORT jint JNICALL Java_montnets_MWGateway_SetOverTime
  (JNIEnv *env, jclass cls, jint iOvertime)
{
  return MongateSetOverTime(iOvertime);
}

//20070326 fq
//��ʱ��Ϣ����
int __stdcall MongateCsSendTime(SOCKET sock,int iType,const char* pBeginDate,const char* pEndDate,
                                int  iFlag,const char* Time, int iMobiCount,const char* pszMsg,
								const char* pTelNos)	
{
	int cbRecv = 0;
    int cbSend = 0;
	int iretFlag = 0;
	int ret= 1;
    char* buf = NULL;
	char strNMsg[512]={0};
    char tmpstr[100]={0};
	char cNumber[15]={0};
	if (sock<=0) {
		return -198;
	}
    if (pTelNos == NULL || pszMsg == NULL || Time == NULL)
		return -1;
    if (iMobiCount > 1000)
		return -2;

	srcStrTohexChr(pszMsg,strNMsg);//������Ϣ����
    strNMsg[511] = '\0';
    buf = (char*)malloc(1536 + iMobiCount * 17);  
	memset(buf,0,sizeof(buf));
    if (buf == NULL)
		return -10;
	iretFlag=TelsCheck(pTelNos,iMobiCount);
	if (1!=iretFlag) {
		return iretFlag;
	}

	sprintf(buf+8,",41,%d,*,*,*,%s,%s,%d,%s,%d,%d,%s,%s",iType,pBeginDate,pEndDate, iFlag,Time,iMobiCount, strlen(strNMsg),
               strNMsg, pTelNos);
    sprintf(buf, "ZZ%06d", strlen(buf+8) + 8);
        buf[8] = ',';	
    
    if (WaitSomeTime(sock, g_iOvertime, SEND) == 1)
	{
		cbSend = send(sock, buf, strlen(buf), 0);
	}
    else
	{
		free(buf);
        return -101;
	}
	memset(buf, 0, 50);
    if (WaitSomeTime(sock, g_iOvertime, RECV) == 1)
	{
         cbRecv = recv(sock, buf, 30, 0);
	}
    else
	{
		free(buf);
        return -103;
	}
    if (cbSend < 1 || cbRecv < 1)
	{
		free(buf);
		return -102;
	}    
    if (buf[0] == 'O' && buf[1] == 'K')
	{
		if (memcpy(cNumber,buf+3,strlen(buf)-3)) {
			ret=atoi(cNumber);	
		}
		free(buf);
		return ret;
	}
    else if (buf[0] == 'E' && buf[1] == 'R')
	{		
        ret = -10000 - (atoi(buf + 3)); //����ƽ̨������Ϣ
		free(buf);
        return ret;
	}
    else
	{
		free(buf);
        return -200;
	}
}

int __stdcall MongateCsSendTimeHttp(SOCKET sock,int iType,const char* pBeginDate,const char* pEndDate,
                                int  iFlag,const char* Time, int iMobiCount,const char* pszMsg,
								const char* pTelNos)	
{
	int cbRecv = 0;
    int cbSend = 0;
    char* buf = NULL,*httpbuf=NULL;
	char strNMsg[1024]={0};
    int ret = -11;
    char tmpstr[1024]={0}; 
	char cNumber[10]={0};

	if (sock<=0) {
		return -198;
	}
    if (pTelNos == NULL || pszMsg == NULL || Time == NULL)
		return -1;
    if (iMobiCount > 1000)
		return -2;
	srcStrTohexChr(pszMsg,strNMsg);//������Ϣ����
    strNMsg[1023] = '\0';
    buf = (char*)malloc(1736 + iMobiCount * 17);  //
	httpbuf = (char*)malloc(1937 + iMobiCount * 17);  //
    if (buf == NULL||httpbuf==NULL)
		return -10;
	sprintf(buf+8,",41,%d,*,*,*,%s,%s,%d,%s,%d,%d,%s,%s",iType,pBeginDate,pEndDate, iFlag,Time,iMobiCount, strlen(strNMsg),
				   strNMsg, pTelNos);
	sprintf(buf, "ZZ%06d", strlen(buf+8) + 8);
	buf[8] = ',';	
    
	if (WaitSomeTime(sock, g_iOvertime, SEND) == 1)
	{
		MakeHttpHead(buf,httpbuf);//20070413 fq
		cbSend = send(sock, httpbuf, strlen(httpbuf), 0);
	}
	else
	{
		free(buf);
		free(httpbuf);
		return -101;
	}
//	memset(buf,0,sizeof(buf));
	if (WaitSomeTime(sock, g_iOvertime, RECV) == 1)
	{
		cbRecv = recv(sock, tmpstr,sizeof(tmpstr), 0);
	}
	else
	{
		free(buf);
		free(httpbuf);
		return -103;
	}
	if (cbSend < 1 || cbRecv < 1)
	{
		free(buf);
		free(httpbuf);
		return -102;
	}
    tmpstr[cbRecv]=0;
	memset(httpbuf,0,sizeof(httpbuf));
	GetHttpRet(tmpstr,httpbuf); 
	memset(buf,0,sizeof(buf));
	strcpy(buf,httpbuf);	  
	if (buf[0] == 'O' && buf[1] == 'K')
	{
		if (memcpy(cNumber,buf+3,strlen(buf)-3)) {
			ret=atoi(cNumber);	
		}		
		free(buf);
		free(httpbuf);
		return ret;
	}
	else if (buf[0] == 'E' && buf[1] == 'R')
	{
		ret = -10000 - (atoi(buf + 3)); //����ƽ̨������Ϣ
		free(buf);
		free(httpbuf);
		return ret;
	 }
	else
	{
		free(buf);
		free(httpbuf);
		return -200;
	}
}
//���¶�ʱ��Ϣ����
int __stdcall MongateCsUpdateSendTime(SOCKET sock, int iNumber, int iType, const char* pBeginDate,
                                      const char* pEndDate, int  iFlag, const char* Time, int iMobiCount,
								      const char* pszMsg, const char* pTelNos)
{
	int cbRecv = 0;
    int cbSend = 0;
    char* buf = NULL;
	char strNMsg[1024]={0};
	char recvbuf[50]={0};
	if (sock<=0) {
		return -198;
	}
    if (pTelNos == NULL || pszMsg == NULL || Time == NULL)
			return -1;
    if (iMobiCount > 1000)
			return -2;
	srcStrTohexChr(pszMsg,strNMsg);
    strNMsg[1023] = '\0';
    buf = (char*)malloc(1700 + iMobiCount * 17);  
    if (buf == NULL)
         return -10;
	sprintf(buf+8,",42,%d,%d,*,*,*,%s,%s,%d,%s,%d,%d,%s,%s",iNumber,iType,pBeginDate,pEndDate, iFlag,Time,iMobiCount, strlen(strNMsg),
               strNMsg, pTelNos);
    sprintf(buf, "ZZ%06d", strlen(buf + 8) + 8);
    buf[8] = ',';
    if (WaitSomeTime(sock, g_iOvertime, SEND) ==1)
	{
       cbSend = send(sock, buf, strlen(buf),0);
	}
    else
	{
		free(buf);
		buf=NULL;
        return -101;
	}
	memset(buf, 0, 50);
    if (WaitSomeTime(sock, g_iOvertime, RECV) ==1)
	{
	    cbRecv = recv(sock, recvbuf, sizeof(recvbuf), 0);
	}
	else
	{
		free(buf);
		buf=NULL;
	    return -103;
	}	  
	if (cbRecv < 1 || cbSend < 1)
	{
		free(buf);
		buf=NULL;
	    return -102;
	}
	if (recvbuf[0] == 'O' && recvbuf[1] == 'K')
	{
		free(buf);
		buf=NULL;
		return 1;
	}
	else if (recvbuf[0] == 'E' && recvbuf[1] == 'R')
	{
		free(buf);
		buf=NULL;
	    return -10000 - atoi(buf+3);
	}
	else
	{
		free(buf);
		buf=NULL;
	    return -200;
	}
}

int __stdcall MongateCsUpdateSendTimeHttp(SOCKET sock, int iNumber, int iType, const char* pBeginDate,
                                      const char* pEndDate, int  iFlag, const char* Time, int iMobiCount,
								      const char* pszMsg, const char* pTelNos)
{
	int cbRecv = 0;
    int cbSend = 0;
    char* buf = NULL,*httpbuf=NULL;
	char strNMsg[1024]={0};
	char strRecv[2024]={0};
	if (sock<=0) {
		return -198;
	}
    if (pTelNos == NULL || pszMsg == NULL || Time == NULL)
			return -1;
    if (iMobiCount > 1000)
			return -2;
	srcStrTohexChr(pszMsg,strNMsg);
    strNMsg[1023] = '\0';
    buf = (char*)malloc(1500 + iMobiCount * 17);  
	httpbuf = (char*)malloc(1537 + iMobiCount * 17);
    if (buf == NULL||httpbuf==NULL)
         return -10;
	memset(buf,0,sizeof(buf));
	memset(httpbuf,0,sizeof(httpbuf));
	sprintf(buf+8,",42,%d,%d,*,*,*,%s,%s,%d,%s,%d,%d,%s,%s",iNumber,iType,pBeginDate,pEndDate, iFlag,Time,iMobiCount, strlen(strNMsg),
               strNMsg, pTelNos);
    sprintf(buf, "ZZ%06d", strlen(buf + 8) + 8);
    buf[8] = ',';
    if (WaitSomeTime(sock, g_iOvertime, SEND) ==1)
	{
		MakeHttpHead(buf,httpbuf);//20070413 fq
		cbSend = send(sock, httpbuf, strlen(httpbuf), 0);
    }
    else
	{
		free(buf); buf = NULL;
		free(httpbuf); httpbuf = NULL;
        return -3;
	}
    if (WaitSomeTime(sock, g_iOvertime, RECV) ==1)
	{
       cbRecv = recv(sock, strRecv, sizeof(strRecv), 0);
	}
    else
	{
		free(buf); buf = NULL;
		free(httpbuf); httpbuf = NULL;
        return -4;
	}
  
    if (cbRecv < 1 || cbSend < 1)
	{
		free(buf); buf = NULL;
		free(httpbuf); httpbuf = NULL;
        return -101;
	}
	strRecv[cbRecv]=0;
	memset(httpbuf,0,sizeof(httpbuf));
	GetHttpRet(strRecv,httpbuf); 
	memset(buf,0,sizeof(buf));
	strcpy(buf,httpbuf);
    if (buf[0] == 'O' && buf[1] == 'K')
	{
		free(buf); buf = NULL;
		free(httpbuf); httpbuf = NULL;
		return 1;
	}
    else if (buf[0] == 'E' && buf[1] == 'R')
	{
		free(buf); buf = NULL;
		free(httpbuf); httpbuf = NULL;
        return -10000 - atoi(buf+3);
	}
    else
	{
		free(buf); buf = NULL;
		free(httpbuf); httpbuf = NULL;
        return -102;
	}
}
//ɾ��ָ����ʱ��Ϣ
int __stdcall MongateCsDelSendTime(SOCKET sock,int iNum)
{
	int cbRecv = 0;
    int cbSend = 0;
    char buf[50] = {0};
    
    sprintf(buf + 8, ",43,%d,*,*,*", iNum);
    sprintf(buf, "ZZ%06d", strlen(buf + 8) + 8);
    buf[8] = ',';

    if (WaitSomeTime(sock, g_iOvertime, SEND) ==1)
        cbSend = send(sock, buf, strlen(buf),0);
    else
		return -101;//
	memset(buf, 0, 50);
    if (WaitSomeTime(sock, g_iOvertime, RECV) ==1)
		cbRecv = recv(sock, buf, sizeof(buf), 0);
    else
		return -103;  
    if (cbRecv < 1 || cbSend < 1)
		return -102;  
    if (buf[0] == 'O' && buf[1] == 'K')
		return 1;
    else if (buf[0] == 'E' && buf[1] == 'R')
		return -10000 - atoi(buf+3);
    else
		return -200;
}


int __stdcall MongateCsDelSendTimeHttp(SOCKET sock,int iNum)
{
	int cbRecv = 0;
    int cbSend = 0;
	char buf[1024]={0},httpbuf[1025]={0};
    
    sprintf(buf + 8, ",43,%d,*,*,*", iNum);
    sprintf(buf, "ZZ%06d", strlen(buf + 8) + 8);
    buf[8] = ',';

    if (WaitSomeTime(sock, g_iOvertime, SEND) ==1)
	{
		MakeHttpHead(buf,httpbuf);  
        cbSend = send(sock, httpbuf, strlen(httpbuf), 0);
	}
    else
		return -3;//
	memset(buf, 0, sizeof(buf));
    if (WaitSomeTime(sock, g_iOvertime, RECV) ==1)
		cbRecv = recv(sock, buf, sizeof(buf), 0);
    else
		return -4;  
    if (cbRecv < 1 || cbSend < 1)
		return -101; 
	memset(httpbuf,0,sizeof(httpbuf));
	GetHttpRet(buf,httpbuf);  
	memset(buf,0,sizeof(buf));
	strcpy(buf,httpbuf);
    if (buf[0] == 'O' && buf[1] == 'K')
		return 1;
    else if (buf[0] == 'E' && buf[1] == 'R')
		return -10000 - atoi(buf+3);
    else
		return -200;
}
//ɾ������ָ����ʱ��Ϣ
int __stdcall MongateCsDelAllSendTime(SOCKET sock)
{
	int cbRecv = 0;
    int cbSend = 0;
    char buf[35] = {0};
    if (sock<=0) {
		return -198;
    }
    sprintf(buf + 8, ",44,*,*,*");
    sprintf(buf, "ZZ%06d", strlen(buf + 8) + 8);
    buf[8] = ',';
    if (WaitSomeTime(sock, g_iOvertime, SEND) ==1)
        cbSend = send(sock, buf, strlen(buf),0);
    else
		return -101;//
    if (WaitSomeTime(sock, g_iOvertime, RECV) ==1)
		cbRecv = recv(sock, buf, sizeof(buf), 0);
    else
		return -103;
  
    if (cbRecv < 1 || cbSend < 1)
		return -102;  
    if (buf[0] == 'O' && buf[1] == 'K')
		return 1;
    else if (buf[0] == 'E' && buf[1] == 'R')
		return -10000 - atoi(buf+3);
    else
		return -200;
}

int __stdcall MongateCsDelAllSendTimeHttp(SOCKET sock)
{
	int cbRecv = 0;
    int cbSend = 0;
    char buf[1024]={0},httpbuf[1025]={0};
    if (sock<=0) {
		return -198;
    }
    sprintf(buf + 8, ",44,*,*,*");
    sprintf(buf, "ZZ%06d", strlen(buf + 8) + 8);
    buf[8] = ',';
    if (WaitSomeTime(sock, g_iOvertime, SEND) ==1)
	{
        MakeHttpHead(buf,httpbuf);  
        cbSend = send(sock, httpbuf, strlen(httpbuf), 0);
	}
    else
		return -101;//
	memset(buf, 0, sizeof(buf));
    if (WaitSomeTime(sock, g_iOvertime, RECV) ==1)
		cbRecv = recv(sock, buf, sizeof(buf), 0);
    else
		return -103;
  
    if (cbRecv < 1 || cbSend < 1)
		return -102;  
	memset(httpbuf,0,sizeof(httpbuf));
	GetHttpRet(buf,httpbuf);  
	memset(buf,0,sizeof(buf));
	strcpy(buf,httpbuf);
    if (buf[0] == 'O' && buf[1] == 'K')
		return 1;
    else if (buf[0] == 'E' && buf[1] == 'R')
		return -10000 - atoi(buf+3);
    else
		return -200;
}

//��ȡ��ʱ��������
int __stdcall MongateCsReadSendTime(SOCKET sock,int iNumber,int iType,char strMsg[10][1024])
{
	int cbRecv = 0;
	int cbSend = 0;
    int len = 0;
    int count = 0;
	char TempstrMsg[10][255];
	char sTemp[255]={0};
    char Tempbuf[10240]={0};	
	char buf[50] = {0};
	if (sock<=0) {
		return -198;
	}
    sprintf(buf + 8, ",45,%d,%d",iNumber,iType);
    sprintf(buf, "ZZ%06d", strlen(buf + 8) + 8);
    buf[8] = ',';
    if (WaitSomeTime(sock, g_iOvertime, SEND) == 1)
       cbSend = send(sock, buf, strlen(buf), 0);
    else
       return -101;
    memset(Tempbuf, 0, sizeof(Tempbuf));
    if (WaitSomeTime(sock, g_iOvertime, RECV) == 1)
       cbRecv = recv(sock, Tempbuf,10240,0);
    else
       return -103; 
	if(cbRecv < 1 || cbSend < 1)
	{
		return -102; 
	}
    if (Tempbuf[0] == 'E' && Tempbuf[1] == 'R')
	{
       return -10000 - (atoi(Tempbuf + 3));//����ƽ̨������Ϣ
	}  
    else if (strstr(Tempbuf, "OK,0") != NULL)
	{
	    return 0;//û����Ϣ	 
	}
    else if(strstr(Tempbuf, "ZZ") != NULL)
	{
		char cFlag1[]=",",cFlag2[]=";",cFlag[]="ZZ";
		char* token=NULL;
		char* pLeave=NULL;
		char  retstr[255]={0};
		int i,iLength;
		if(Tempbuf==NULL)
		{
			return -5;
		}
		iLength=strlen(Tempbuf);
		pLeave=strstr(Tempbuf,cFlag);
		if(pLeave==NULL)
		{
			memset(Tempbuf, 0, sizeof(Tempbuf));
			if (WaitSomeTime(sock, g_iOvertime, RECV) == 1)
				cbRecv = recv(sock, Tempbuf,10000, 0);
			else
				return -103; 
			iLength=strlen(Tempbuf);
		    pLeave=strstr(Tempbuf,cFlag);
		}
		pLeave=pLeave+20;
		token=strtok(pLeave,cFlag2);
		while( token != NULL&&count<10)
		{
  		  strcpy(TempstrMsg[count],token);
		  count++;
		  token = strtok( NULL, cFlag2 );
 		}

		for(i=0;i<count;i++)
		{
			strcpy(strMsg[i],TempstrMsg[i]);
		}

		for(i=0;i<count;i++)
		{
		   int iFlag=1;
		   token = strtok( strMsg[i], cFlag1);
		   strcpy(sTemp,token);
		   while( token != NULL)
		   {
			  iFlag++;
			  token = strtok( NULL, cFlag1 );
			  if(token!=NULL&&iFlag>5&&iFlag!=8&&iFlag!=10)
			  {
				  if(iFlag==11)
				  {					  
                       token=hexChrTosrcStr(token,retstr);					  
				  }
				  strcat(sTemp,",");
				  strcat(sTemp,token);				  
			  }				  
		   }
		   memset(strMsg[i], 0, 255);
		   strcpy(strMsg[i],sTemp);
		}
		token=NULL;
		pLeave=NULL;
		return 1;//����Ϣ	
	}
	else{
		return -200;
	}
}

int __stdcall MongateCsReadSendTimeEx(SOCKET sock,int iNumber,int iType,char *strMsg)
{
	int cbRecv = 0;
	int cbSend = 0;
    int len = 0;
    int count = 0;
	int iret = -1;
	int iTotalLen=0;
	int iMsgLen=0;
	bool bFlag=true;
	char recvbuf[20480]={0};
    char Tempbuf[3000]={0};	
	char buf[256] = {0};
	if (sock<=0) {
		return -198;
	}
	memset(strMsg,0,sizeof(strMsg));
    sprintf(buf + 8, ",45,%d,%d",iNumber,iType);
    sprintf(buf, "ZZ%06d", strlen(buf + 8) + 8);
    buf[8] = ',';

    if (WaitSomeTime(sock, g_iOvertime, SEND) == 1)
       cbSend = send(sock, buf, strlen(buf), 0);
    else
       return -101;

	while (bFlag) {
		memset(Tempbuf, 0, sizeof(Tempbuf));
		if (WaitSomeTime(sock, g_iOvertime, RECV) == 1)
		{
			cbRecv = recv(sock, Tempbuf, 3000, 0);
			if (cbRecv<1) {
				iret = -104;
				bFlag = false;
			}
			Tempbuf[cbRecv]=0;
			iTotalLen+=cbRecv;
			strcat(recvbuf,Tempbuf);
			if (Tempbuf[0] == 'E' && Tempbuf[1] == 'R')
			{
				iret = -10000 - (atoi(Tempbuf + 3));//����ƽ̨������Ϣ
				bFlag = false;
			}			
			else if (Tempbuf[0] == 'O' && Tempbuf[1] == 'K')
			{
				char temp[20]={0};
				int iFlag = 0;
				char * token=NULL;
				const char Flag[]=",";
				memcpy(temp,Tempbuf,strlen(Tempbuf));
				token=strtok(temp,Flag);
				while (token != NULL) {
					iFlag++;					
					if (3==iFlag) {
						g_iMOSpace=atoi(token);
					}
					token = strtok( NULL,Flag);
				}
				iret = 0;//û����Ϣ	
				token=NULL;
				bFlag=false;
			}			
			else if (Tempbuf[0] == 'Z' || Tempbuf[1] == 'Z')
			{
				if (iTotalLen>=8) {
					char Head[10]={0};
					memcpy(Head,Tempbuf+2,6);
					iMsgLen=atoi(Head);
				}
				else
				{
					continue;
				}
				if (iTotalLen<iMsgLen) 
				{
					continue;
				}
				else
				{
					int iCount=0;
					int ilen=0;
					char *p=NULL;
					int i=4;
					p=strstr(recvbuf,",");
					while (i>0) {
						i--;
						p=strstr(p+1,",");
						if (i==1) {
							iCount=atoi(p+1);
						}						
					}
					ilen=strlen(p+1);
					memcpy(strMsg,p+1,strlen(p+1));
					strMsg[ilen-2]='\0';
					iret=iCount;
					bFlag=false;
				}				
			}
			else
			{
				if (iTotalLen<iMsgLen) 
				{
					continue;
				}
				else
				{
					int iCount=0;
					int ilen = 0;
					char *p=NULL;
					int i=4;
					p=strstr(recvbuf,",");
					while (i>0) {
						i--;
						p=strstr(p+1,",");
						if (i==1) {
							iCount=atoi(p+1);
						}						
					}
					ilen=strlen(p+1);
					memcpy(strMsg,p+1,strlen(p)-1);
					strMsg[ilen-2]='\0';
					iret=iCount;
					bFlag=false;
				}
			}
		}
		else
		{
			iret = -103;
			bFlag = false;
		}
	}
	return iret;
}

int __stdcall MongateCsReadSendTimeHttp(SOCKET sock,int iNumber,int iType,char strMsg[10][1024])
{
	int cbRecv = 0;
	int cbSend = 0;
    int len = 0;
    int count = 0;
	char TempstrMsg[10][1024];
	char sTemp[255]={0};
    char Tempbuf[1525] = {0};	
	char buf[1525] = {0};
	if (sock<=0) {
		return -198;
	}
    sprintf(buf + 8, ",45,%d,%d",iNumber,iType);
    sprintf(buf, "ZZ%06d", strlen(buf + 8) + 8);
    buf[8] = ',';
    if (WaitSomeTime(sock, g_iOvertime, SEND) == 1)
	{
		MakeHttpHead(buf,Tempbuf);
        cbSend = send(sock, Tempbuf, strlen(Tempbuf), 0);
	}
    else
       return -3;
    memset(buf, 0, sizeof(buf));
    if (WaitSomeTime(sock, g_iOvertime, RECV) == 1)
       cbRecv = recv(sock, buf,sizeof(buf), 0);
    else
       return -4; 
	memset(Tempbuf,0,sizeof(Tempbuf));
	GetHttpRet(buf,Tempbuf);  
	memset(buf,0,sizeof(buf));
	strcpy(buf,Tempbuf);

    if (buf[0] == 'E' && buf[1] == 'R')
	{
       return -10000 - (atoi(Tempbuf + 3));//����ƽ̨������Ϣ
	}  
    else if (strstr(buf, "OK") != NULL)
	{
	    return 0;//û����Ϣ	 
	}
	else if(cbRecv < 1 || cbSend < 1)
	{
		return -101; 
	}
    else
	{
		char cFlag1[]=",",cFlag2[]=";",cFlag[]="ZZ";
		char* token=NULL;
		char* pLeave=NULL;
		char  retstr[255]={0};
		int i,iLength;
		iLength=strlen(buf);
		pLeave=strstr(buf,cFlag);		
		pLeave=pLeave+20;
		token=strtok(pLeave,cFlag2);
		while( token != NULL&&count<10)
		{
  		  strcpy(TempstrMsg[count],token);
		  count++;
		  token = strtok( NULL, cFlag2 );
 		}
		for(i=0;i<count;i++)
		{
			strcpy(strMsg[i],TempstrMsg[i]);
		}
		for(i=0;i<count;i++)
		{
		   int iFlag=1;
		   token = strtok( strMsg[i], cFlag1);
		   strcpy(sTemp,token);
		   while( token != NULL )
		   {
			  iFlag++;
			  token = strtok( NULL, cFlag1 );
			  if(token!=NULL&&iFlag>5&&iFlag!=8&&iFlag!=10)
			  {
				  if(iFlag==11)
				  {					  
                       token=hexChrTosrcStr(token,retstr);					  
				  }
				  strcat(sTemp,",");
				  strcat(sTemp,token);				  
			  }				  
		   }
		   memset(strMsg[i], 0, 1024);
		   strcpy(strMsg[i],sTemp);
		}
		token=NULL;
		pLeave=NULL;
		return 1;//����Ϣ	
	}
}

int __stdcall MongateCsReadSendTimeHttpEx(SOCKET sock,int iNumber,int iType,char *strMsg)
{
	int cbRecv = 0;
	int cbSend = 0;
    int len = 0;
    int count = 0;
	int iret = -1;
	int iTotalLen=0;
	int iMsgLen=0;
	bool bFlag=true;
	char recvbuf[20480]={0};
	char retbuf[20480]={0};
    char Tempbuf[3000]={0};	
	char buf[256] = {0};
	char httpbuf[1024]={0};
	if (sock<=0) {
		return -198;
	}
	memset(strMsg,0,sizeof(strMsg));
    sprintf(buf + 8, ",45,%d,%d",iNumber,iType);
    sprintf(buf, "ZZ%06d", strlen(buf + 8) + 8);
    buf[8] = ',';

    if (WaitSomeTime(sock, g_iOvertime, SEND) == 1)
	{
		MakeHttpHead(buf,httpbuf); 
		cbSend = send(sock, httpbuf, strlen(httpbuf), 0);
	}
    else
       return -101;

	while (bFlag) {
		memset(Tempbuf, 0, sizeof(Tempbuf));
		if (WaitSomeTime(sock, g_iOvertime, RECV) == 1)
		{
			cbRecv = recv(sock, Tempbuf, 3000, 0);
			if (cbRecv<1) {
				iret = -104;
				bFlag = false;
			}
			Tempbuf[cbRecv]=0;
			iTotalLen+=cbRecv;
			strcat(recvbuf,Tempbuf);
			if (strstr(recvbuf,"</kfsms-http>")!=NULL) 
			{
				GetHttpRet(recvbuf,retbuf);
				if (retbuf[0] == 'E' && retbuf[1] == 'R')
				{
					iret = -10000 - (atoi(retbuf + 3));//����ƽ̨������Ϣ
					bFlag = false;
				}				
				if (strstr(retbuf, "OK,0") != NULL)
				{
					char temp[20]={0};
					int iFlag = 0;
					char * token;
					const char Flag[]=",";
					memcpy(temp,retbuf,20);
					token=strtok(temp,Flag);
					while (token != NULL) {
						iFlag++;					
						if (3==iFlag) {
							g_iReportSpace=atoi(token);
						}
						token = strtok( NULL,Flag);
					}
					iret = 0;//û����Ϣ				
					bFlag=false;
				}				
				if (retbuf[0] == 'Z' || retbuf[1] == 'Z')
				{
					int iCount=0;
					int ilen=0;
					char *p=NULL;
					int i=4;
					p=strstr(retbuf,",");
					while (i>0) {
						i--;
						p=strstr(p+1,",");
						if (i==1) {
							iCount=atoi(p+1);
						}						
					}
					ilen=strlen(p+1);
					memcpy(strMsg,p+1,strlen(p)-1);
					strMsg[ilen-2]='\0';
					iret=iCount;
					bFlag=false;
				}
			}
			else{
				continue;
			}
		}
		else
		{
			iret = -103;
			bFlag=false;
		}			
	}
	return iret;
}

//���¶�ʱ�����б�
int __stdcall MongateCsUpdateTelListSendTime(SOCKET sock,int iNumber,int iNum,const char* pTelNos)
{
	int cbRecv = 0;
    int cbSend = 0;
    char buf[512] = {0};
	if (sock<=0) {
		return -198;
	}
	if (pTelNos == NULL)
		return -1;
    sprintf(buf + 8, ",46,%d,*,*,%d,%s",iNumber,iNum,pTelNos);
    sprintf(buf, "ZZ%06d", strlen(buf + 8) + 8);
    buf[8] = ',';
    if (WaitSomeTime(sock, g_iOvertime, SEND) ==1)
	{
		cbSend = send(sock, buf, strlen(buf), 0);
	}
    else
		return -101;//
	memset(buf, 0, sizeof(buf));
    if (WaitSomeTime(sock, g_iOvertime, RECV) ==1)
		cbRecv = recv(sock, buf,sizeof(buf), 0);
    else
		return -103;
  
    if (cbRecv < 1 || cbSend < 1)
		return -102;
    if (buf[0] == 'O' && buf[1] == 'K')
		return 1;
    else if (buf[0] == 'E' && buf[1] == 'R')
		return -10000 - atoi(buf+3);
    else
		return -200;	
}

//���¶�ʱ�����б�
int __stdcall MongateCsUpdateTelListSendTimeHttp(SOCKET sock,int iNumber,int iNum,const char* pTelNos)
{
	int cbRecv = 0;
    int cbSend = 0;
	char* buf = NULL,*httpbuf=NULL; 
	if (sock<=0) {
		return -198;
	}
	if (pTelNos == NULL)
		return -1;

	buf = (char*)malloc(1500 +iNum*17);  
	httpbuf = (char*)malloc(1537 + iNum * 17);
    if (buf == NULL||httpbuf==NULL)
         return -10;
	memset(buf,0,sizeof(buf));
	memset(httpbuf,0,sizeof(httpbuf));
    sprintf(buf + 8, ",46,%d,*,*,%d,%s",iNumber,iNum,pTelNos);
    sprintf(buf, "ZZ%06d", strlen(buf + 8) + 8);
    buf[8] = ',';

    if (WaitSomeTime(sock, g_iOvertime, SEND) ==1)
	{
		MakeHttpHead(buf,httpbuf);//20070413 fq
		cbSend = send(sock, httpbuf, strlen(httpbuf), 0);
	}
    else
		return -3;//
	memset(buf, 0, sizeof(buf));
    if (WaitSomeTime(sock, g_iOvertime, RECV) ==1)
		cbRecv = recv(sock, buf,sizeof(buf), 0);
    else
		return -4;
  
    if (cbRecv < 1 || cbSend < 1)
		return -101;
	buf[cbRecv]=0;
	memset(httpbuf,0,sizeof(httpbuf));
	GetHttpRet(buf,httpbuf); 
	memset(buf,0,sizeof(buf));
	strcpy(buf,httpbuf);
    if (buf[0] == 'O' && buf[1] == 'K')
		return 1;
    else if (buf[0] == 'E' && buf[1] == 'R')
		return -10000 - atoi(buf+3);
    else
		return -102;
	
}

//��ȡ��ʱ�����б�
int __stdcall MongateCsReadTelListSendTime(SOCKET sock,int iNumber,char* pStrMessage)
{
	int cbRecv = 0;
	int cbSend = 0;
    int len = 0;
    int count = 0;
    char Tempbuf[20000] = {0};
	char Tempbuf1[10000]={0};
	char sTemp[255]={0};
	char buf[15] = {0};
	int TotalLen=0;
	int MsgLen=0; 
	int iret = 0;
	if (sock<=0) {
		return -198;
	}
	memset(pStrMessage,0,sizeof(pStrMessage));
    sprintf(buf + 8, ",47,%d",iNumber);
    sprintf(buf, "ZZ%06d", strlen(buf + 8) + 8);
    buf[8] = ',';
    if (WaitSomeTime(sock, g_iOvertime, SEND) == 1)
       cbSend = send(sock, buf, strlen(buf), 0);
    else
       return -101;

	if (cbSend < 1) {
		return -102;
	}
    memset(Tempbuf, 0, sizeof(Tempbuf));	
	do {
		if (WaitSomeTime(sock, g_iOvertime, RECV) == 1)
		{
			memset(Tempbuf1, 0, sizeof(Tempbuf1));
			cbRecv = recv(sock, Tempbuf1,10000, 0);
			if (cbRecv==0) {
				iret = -104;
				break; 
			}
			TotalLen+=cbRecv;
			strcat(Tempbuf,Tempbuf1);
			if (Tempbuf[0] == 'E' && Tempbuf[1] == 'R')
			{
				iret = -10000 - (atoi(Tempbuf + 3));
				break; //����ƽ̨������Ϣ
			}  
			else if (Tempbuf[0] == 'O' && Tempbuf[1] == 'K')
			{	   
				iret = 0;//û����Ϣ	
				break;
			} 
			else if(Tempbuf[0] == 'Z' && Tempbuf[1] == 'Z')
			{
				if (TotalLen>=8) {
					char Head[10]={0};
					memcpy(Head,Tempbuf+2,6);
					MsgLen=atoi(Head);
				}
				else
				{
					continue;
				}
				if (TotalLen<MsgLen) {
					continue;
				}
				else
				{
					char  cFlag[]=",";
					char* token=NULL;
					int iFlag=1;
					token = strtok(Tempbuf, cFlag);
					while( token != NULL )
					{
						iFlag++;
						token = strtok( NULL, cFlag);
						if(token!=NULL&&iFlag>5)
						{
							if(iFlag==6)
							{
								strcat(sTemp,token);					
							}
							else
							{
								strcat(sTemp,",");
								strcat(sTemp,token);
							}
						}
						memset(pStrMessage, 0, 255);
						strcpy(pStrMessage,sTemp);
					}
					token=NULL;
				}
				iret= 1;//����Ϣ	
				break;
			}
			else
			{
				iret= -200;
				break;
			}
		}
		else
		{
			iret = -103;
			break;
		}
	} while(1);
	return iret;
}


//��ȡ��ʱ�����б�
int __stdcall MongateCsReadTelListSendTimeHttp(SOCKET sock,int iNumber,char* pStrMessage)
{
	int cbRecv = 0;
	int cbSend = 0;
    int len = 0;
    int count = 0;
    char Tempbuf[20000] = {0};
	char Tempbuf1[10000]={0};
	char sTemp[255]={0};	
	char buf[15] = {0};
	char httpbuf[20000];   
	if (sock<=0) {
		return -198;
	}
	memset(pStrMessage,0,sizeof(pStrMessage));
    sprintf(buf + 8, ",47,%d",iNumber);
    sprintf(buf, "ZZ%06d", strlen(buf + 8) + 8);
    buf[8] = ',';
    if (WaitSomeTime(sock, g_iOvertime, SEND) == 1)
	{	
		MakeHttpHead(buf,httpbuf);                      //20070413 fq
		cbSend = send(sock, httpbuf, strlen(httpbuf), 0);
	}
    else
       return -101;
    memset(Tempbuf, 0, sizeof(Tempbuf));
	do {				
		if (WaitSomeTime(sock, g_iOvertime, RECV) == 1)
		{
			memset(Tempbuf1, 0, sizeof(Tempbuf1));
			cbRecv = recv(sock, Tempbuf1,10000, 0);	
			strcat(Tempbuf,Tempbuf1);
		}			
		else
			return -103;
		if (strstr(Tempbuf1, "</kfsms-http>")==NULL) {
			continue;
		}
		else{
			memset(httpbuf,0,sizeof(httpbuf));
			GetHttpRet(Tempbuf,httpbuf); 
			memset(Tempbuf,0,sizeof(Tempbuf));
			strcpy(Tempbuf,httpbuf);   
			if (Tempbuf[0] == 'E' && Tempbuf[1] == 'R')
			{
				return -10000 - (atoi(Tempbuf + 3));//����ƽ̨������Ϣ
			}  
			else if (strstr(Tempbuf, "OK,0") != NULL)
			{	   
				return 0;//û����Ϣ	 
			} 
			else if (cbRecv < 1 || cbSend < 1)
				return -102; 
			else
			{
				char  cFlag[]=",";
				char * token=NULL;
				int iFlag=1;
				token = strtok(Tempbuf, cFlag);
				while( token != NULL )
				{
					iFlag++;
					token = strtok( NULL, cFlag);
					if(token!=NULL&&iFlag>5)
					{
						if(iFlag==6)
						{
							strcat(sTemp,token);					
						}
						else
						{
							strcat(sTemp,",");
							strcat(sTemp,token);
						}
					}
					memset(pStrMessage, 0, 255);
					strcpy(pStrMessage,sTemp);
					token=NULL;
				}	   
				return 1;//����Ϣ			
			}
		}		
	} while(1);    
}

/**********************************************************************************/
//08-11-21�޸�
//MongateTestMsgEx
//���ܣ�����Ƿ񺬹ؼ��֣����عؼ���
//���룺sock ���Ӿ��;pStrMessage ������Ϣ;keywords����ؼ��� 
//���������1Ϊ�޹ؼ���;-10033Ϊ�йؼ���(ͨ��keywords����);����ֵΪ��������
//
int __stdcall MongateTestMsgEx(SOCKET sock,const char* pStrMessage,char *keywords)
{
	int cbRecv = 0;
	int cbSend = 0;
    int len = 0;
    int count = 0;
	char buf[2024] = {0};
	char strMsg[1920]={0};
	char recvBuf[4000]={0};
	if (sock<=0) {
		return -198;
	}
	if(pStrMessage == NULL || keywords == NULL || strlen(pStrMessage)==0)
	{
		return -1;
	}
	if (strlen(pStrMessage)>960) {
		return -2; 
	}
	return 1;
	srcStrTohexChr(pStrMessage,strMsg);
    
    sprintf(buf + 8, ",57,*,*,*,%d,%s",strlen(strMsg),strMsg);
    sprintf(buf, "ZZ%06d", strlen(buf + 8) + 8);
    buf[8] = ',';

    if (WaitSomeTime(sock, g_iOvertime, SEND) == 1)
       cbSend = send(sock, buf, strlen(buf), 0);
    else
       return -101;
    if (WaitSomeTime(sock, g_iOvertime, RECV) == 1)
       cbRecv = recv(sock, recvBuf,sizeof(recvBuf), 0);
    else
       return -103;
	if (cbRecv < 1 || cbSend < 1)
		return -102;

    if (recvBuf[0] == 'O' && recvBuf[1] == 'K')
		return 1;
    else if (recvBuf[0] == 'E' && recvBuf[1] == 'R')
	{
		memset(buf,0,sizeof(buf));
		memset(strMsg,0,sizeof(strMsg));
		if(recvBuf[2]==',')
		{//ȡ�ؼ���
			strncpy(buf,recvBuf+3,1500);
			strtok(buf,",");
			strcpy(strMsg,strtok(NULL,","));
			memset(buf,0,sizeof(buf));
			hexChrTosrcStr(strMsg,buf);
			strcpy(keywords,buf);
		}
		return -10000 - atoi(recvBuf+3);
	}
    else
		return -200;
}
int __stdcall MongateTestMsg(SOCKET sock,const char* pStrMessage)
{
	int cbRecv = 0;
	int cbSend = 0;
    int len = 0;
    int count = 0;
	char buf[2024] = {0};
	char strMsg[1920]={0};
	char recvBuf[2000]={0};
	if (sock<=0) {
		return -198;
	}
	if (strlen(pStrMessage)==0) {
		return -1;
	}
	if (strlen(pStrMessage)>960) {
		return -2;
	}
	srcStrTohexChr(pStrMessage,strMsg);
    
    sprintf(buf + 8, ",27,*,*,*,%d,%s",strlen(strMsg),strMsg);
    sprintf(buf, "ZZ%06d", strlen(buf + 8) + 8);
    buf[8] = ',';

    if (WaitSomeTime(sock, g_iOvertime, SEND) == 1)
       cbSend = send(sock, buf, strlen(buf), 0);
    else
       return -101;
    if (WaitSomeTime(sock, g_iOvertime, RECV) == 1)
       cbRecv = recv(sock, recvBuf,sizeof(recvBuf), 0);
    else
       return -103;
	if (cbRecv < 1 || cbSend < 1)
		return -102;

    if (recvBuf[0] == 'O' && recvBuf[1] == 'K')
		return 1;
    else if (recvBuf[0] == 'E' && recvBuf[1] == 'R')
	{
		return -10000 - atoi(recvBuf+3);
	}
    else
		return -200;
}
int __stdcall MongateTestMsgHttp(SOCKET sock,const char* pStrMessage)
{
	int cbRecv = 0;
	int cbSend = 0;
    int len = 0;
    int count = 0;
	char buf[1024] = {0};
	char httpbuf[2024]={0};
	char strMsg[1024]={0};
	if (sock<=0) {
		return -198;
	}
	if (strlen(pStrMessage)==0) {
		return -1;
	}
	if (strlen(pStrMessage)>960) {
		return -2;
	}
    srcStrTohexChr(pStrMessage,strMsg);
    sprintf(buf + 8, ",27,*,*,*,%d,%s",strlen(strMsg),strMsg);
    sprintf(buf, "ZZ%06d", strlen(buf + 8) + 8);
    buf[8] = ',';

    if (WaitSomeTime(sock, g_iOvertime, SEND) == 1)
	{
		MakeHttpHead(buf,httpbuf);//20070413 fq
		cbSend = send(sock, httpbuf, strlen(httpbuf), 0);
	   }
    else
       return -101;
	memset(buf,0,sizeof(buf));
    if (WaitSomeTime(sock, g_iOvertime, RECV) == 1)
       cbRecv = recv(sock, buf,500, 0);
    else
       return -103;
	if (cbRecv < 1 || cbSend < 1)
		return -102;

	memset(httpbuf,0,sizeof(httpbuf));
	GetHttpRet(buf,httpbuf); 
	memset(buf,0,sizeof(buf));
	strcpy(buf,httpbuf); 
    if (buf[0] == 'O' && buf[1] == 'K')
		return 1;
    else if (buf[0] == 'E' && buf[1] == 'R')
	{
		return -10000 - atoi(buf+3);
	}
    else
		return -200;
}
int __stdcall MongateTestMsgExHttp(SOCKET sock,const char* pStrMessage,char *keyword)
{
	int cbRecv = 0;
	int cbSend = 0;
    int len = 0;
    int count = 0;
	char buf[1024] = {0};
	char httpbuf[2024]={0};
	char strMsg[1024]={0};
	if (sock<=0) {
		return -198;
	}
	if(pStrMessage == NULL || keyword == NULL || strlen(pStrMessage) == 0)
	{
		return -1;
	}
	if (strlen(pStrMessage)>960) {
		return -2;
	}
    srcStrTohexChr(pStrMessage,strMsg);
    sprintf(buf + 8, ",57,*,*,*,%d,%s",strlen(strMsg),strMsg);
    sprintf(buf, "ZZ%06d", strlen(buf + 8) + 8);
    buf[8] = ',';

    if (WaitSomeTime(sock, g_iOvertime, SEND) == 1)
	{
		MakeHttpHead(buf,httpbuf);//20070413 fq
		cbSend = send(sock, httpbuf, strlen(httpbuf), 0);
	   }
    else
       return -101;
	memset(buf,0,sizeof(buf));
    if (WaitSomeTime(sock, g_iOvertime, RECV) == 1)
       cbRecv = recv(sock, buf,500, 0);
    else
       return -103;
	if (cbRecv < 1 || cbSend < 1)
		return -102;

	memset(httpbuf,0,sizeof(httpbuf));
	GetHttpRet(buf,httpbuf); 
	memset(buf,0,sizeof(buf));
	strcpy(buf,httpbuf); 
    if (buf[0] == 'O' && buf[1] == 'K')
		return 1;
    else if (buf[0] == 'E' && buf[1] == 'R')
	{
		memset(strMsg,0,sizeof(strMsg));
		if(buf[2]==',')
		{//ȡ�ؼ���
			//strncpy(httpbuf,buf+3,1500);
			strtok(buf+3,",");
			strcpy(httpbuf,strtok(NULL,","));
			hexChrTosrcStr(httpbuf,strMsg);
			strcpy(keyword,strMsg);
		}
		return -10000 - atoi(buf+3);
	}
    else
		return -200;
	
}
/**********************************************************************************/

int __stdcall MGhexChrTosrcStr(const char* srcMsg,char* retMsg)
{
	char *p=NULL;
	if (srcMsg==NULL) {
		return -1;
	}
	p=hexChrTosrcStr(srcMsg,retMsg);
	if (p==NULL) {
		return -1;
	}
	else{
		return 1;
	}
}

int __stdcall MGQueryAccount(SOCKET sock ,const char * pReserve,char* pReStr)
{
	int cbRecv = 0;
	int cbSend = 0;
	char sendbuf[30]={0};
	char sendbufhttp[10240]={0};
	char buf[10240] = {0};	
	if (sock<=0) {
		return -198;
	}
	memset(pReStr,0,sizeof(pReStr));
	if (WaitSomeTime(sock, g_iOvertime, SEND) ==1)
	{
		if (0==g_ConnectType) {
			cbSend = send(sock, "ZZ000019,26,0,*,*,*", 19, 0);
		}
		else
		{
			MakeHttpHead("ZZ000019,26,0,*,*,*",sendbufhttp);
			cbSend = send(sock, sendbufhttp, strlen(sendbufhttp), 0);
		}		
	}
	else
		return -101; //�ȴ����ͳ�ʱ
		
	if (WaitSomeTime(sock, g_iOvertime, RECV) ==1)
		cbRecv = recv(sock, buf, sizeof(buf), 0);
	else
		return -103;//�ȴ����ճ�ʱ
		
	if ( cbSend < 1 || cbRecv < 1)
		return -102;//�ȴ�����������ʧ��
	
	if (1==g_ConnectType) {
		buf[cbRecv]='\0';
		memset(sendbufhttp,0,sizeof(sendbufhttp));
		GetHttpRet(buf,sendbufhttp);  
		memset(buf,0,sizeof(buf));
		strcpy(buf,sendbufhttp);
	}
	else{
		buf[cbRecv] = '\0';
	}	
	if ((buf[1] == 'Z' && buf[0] == 'Z'))
	{
		char *p=NULL;
		int i=1;
		p=strstr(buf,",");
		while (i>0) {
			p=strstr(p+1,",");
			i--;
		}
		memcpy(pReStr,p+1,strlen(p)-1);
		return 1;
	}
	else if (buf[0] == 'E' && buf[1] == 'R')
	{
		return -10000 - atoi(buf+3);
	}
	else
	{
		return -200;//���ӶϿ�
	}
}

int __stdcall MongateCsReadInform(SOCKET sock,int iType,const char* sNumber,const char* reserve1,const char * reserve2,const char * reserve3,char *strMsg)
{
	int cbRecv = 0;
	int cbSend = 0;
	int iret = -200;
	int iTotalLen=0;
	int iMsgLen=0;
	bool bFlag=true;
	char recvbuf[20480]={0};
    char Tempbuf[3000]={0};	
	char buf[256] = {0};
	char buftttp[1024]={0};
	if (sock<=0) {
		return -198;
	}
	if (sNumber==NULL||strlen(sNumber)==0) {
		return -1;
	}
	memset(strMsg,0,sizeof(strMsg));
    sprintf(buf + 8, ",38,%d,%s,%s,%s,%s",iType,sNumber,reserve1,reserve2,reserve3);
    sprintf(buf, "ZZ%06d", strlen(buf + 8) + 8);
    buf[8] = ',';

    if (WaitSomeTime(sock, g_iOvertime, SEND) == 1)
	{
		if (0==g_ConnectType) {
			cbSend = send(sock, buf, strlen(buf), 0);
		}
		else{
			MakeHttpHead(buf,buftttp);
			cbSend = send(sock, buftttp, strlen(buftttp), 0);			
		}       
	}
    else
       return -101;

	if (0==g_ConnectType) {	
		while (bFlag) {
			memset(Tempbuf, 0, sizeof(Tempbuf));
			if (WaitSomeTime(sock, g_iOvertime, RECV) == 1)
			{
				cbRecv = recv(sock, Tempbuf, 3000, 0);
				if (cbRecv<1) {
					iret = -104;
					bFlag = false;
				}
				Tempbuf[cbRecv]=0;
				iTotalLen+=cbRecv;
				strcat(recvbuf,Tempbuf);
				if (Tempbuf[0] == 'E' && Tempbuf[1] == 'R')
				{
					iret = -10000 - (atoi(Tempbuf + 3));//����ƽ̨������Ϣ
					bFlag = false;
				}			
				else if (Tempbuf[0] == 'O' && Tempbuf[1] == 'K')
				{
	//				char temp[20]={0};
	//				int iFlag = 0;
	//				char * token;
	//				const char Flag[]=",";
	//				memcpy(temp,Tempbuf,strlen(Tempbuf));
	//				token=strtok(temp,Flag);
	//				while (token != NULL) {
	//					iFlag++;					
	//					if (3==iFlag) {
	//						g_iMOSpace=atoi(token);
	//					}
	//					token = strtok( NULL,Flag);
	//				}
					iret = 0;//û����Ϣ				
					bFlag=false;
				}			
				else if (Tempbuf[0] == 'Z' || Tempbuf[1] == 'Z')
				{
					if (iTotalLen>=8) {
						char Head[10]={0};
						memcpy(Head,Tempbuf+2,6);
						iMsgLen=atoi(Head);
					}
					else
					{
						continue;
					}
					if (iTotalLen<iMsgLen) 
					{
						continue;
					}
					else
					{
						int iCount=0;
						int ilen=0;
						char *p=NULL;
						int i=4;
						p=strstr(recvbuf,",");
						while (i>0) {
							i--;
							p=strstr(p+1,",");
							if (i==1) {
								iCount=atoi(p+1);
							}						
						}
						ilen=strlen(p+1);
						memcpy(strMsg,p+1,strlen(p+1));
						strMsg[ilen-2]='\0';
						iret=iCount;
						bFlag=false;
					}				
				}
				else
				{
					if (iTotalLen<iMsgLen) 
					{
						continue;
					}
					else
					{
						int iCount=0;
						int ilen = 0;
						char *p=NULL;
						int i=4;
						p=strstr(recvbuf,",");
						while (i>0) {
							i--;
							p=strstr(p+1,",");
							if (i==1) {
								iCount=atoi(p+1);
							}						
						}
						ilen=strlen(p+1);
						memcpy(strMsg,p+1,strlen(p)-1);
						strMsg[ilen-2]='\0';
						iret=iCount;
						bFlag=false;
					}
				}
			}
			else
			{
				iret = -103;
				bFlag = false;
			}
		}
	}
	else{
		char retbuf[20480]={0};
		while (bFlag) {
			memset(Tempbuf, 0, sizeof(Tempbuf));
			if (WaitSomeTime(sock, g_iOvertime, RECV) == 1)
			{
				cbRecv = recv(sock, Tempbuf, 3000, 0);
				if (cbRecv<1) {
					iret = -104;
					bFlag = false;
				}
				Tempbuf[cbRecv]=0;
				iTotalLen+=cbRecv;
				strcat(recvbuf,Tempbuf);
				if (strstr(recvbuf,"</kfsms-http>")!=NULL) 
				{
					GetHttpRet(recvbuf,retbuf);
					if (retbuf[0] == 'E' && retbuf[1] == 'R')
					{
						iret = -10000 - (atoi(retbuf + 3));//����ƽ̨������Ϣ
						bFlag = false;
					}				
					if (strstr(retbuf, "OK,0") != NULL)
					{
//						char temp[20]={0};
//						int iFlag = 0;
//						char * token;
//						const char Flag[]=",";
//						memcpy(temp,retbuf,20);
//						token=strtok(temp,Flag);
//						while (token != NULL) {
//							iFlag++;					
//							if (3==iFlag) {
//								g_iReportSpace=atoi(token);
//							}
//							token = strtok( NULL,Flag);
//						}
						iret = 0;//û����Ϣ				
						bFlag=false;
					}				
					if (retbuf[0] == 'Z' || retbuf[1] == 'Z')
					{
						int iCount=0;
						int ilen=0;
						char *p=NULL;
						int i=4;
						p=strstr(retbuf,",");
						while (i>0) {
							i--;
							p=strstr(p+1,",");
							if (i==1) {
								iCount=atoi(p+1);
							}						
						}
						ilen=strlen(p+1);
						memcpy(strMsg,p+1,strlen(p)-1);
						strMsg[ilen-2]='\0';
						iret=iCount;
						bFlag=false;
					}
				}
				else{
					continue;
				}
			}
			else
			{
				iret = -103;
				bFlag=false;
			}			
		}		
	}
	return iret;
}


/************************************************************************/
/*                                                                      */
/************************************************************************/
int TranMsgId(unsigned char *pSrc, char *MsgId)
{
	unsigned int nMon	= 0;
	unsigned int nDay	= 0;
	unsigned int nHour	= 0;
	unsigned int nMin	= 0;
	unsigned int nSec	= 0;
	unsigned int nGate	= 0;
	unsigned int nNo	= 0;
	unsigned char *p	= NULL;
	
	//��תΪ64bit��������λ
	__int64 j = 0;
	p = pSrc;
	j |= (__int64)(*(p))	<< 56;
	j |= (__int64)(*(++p))	<< 48;
	j |= (__int64)(*(++p))	<< 40;
	j |= (__int64)(*(++p))	<< 32;
	j |= (__int64)(*(++p))	<< 24;
	j |= (__int64)(*(++p))	<< 16;
	j |= (__int64)(*(++p))	<< 8;
	j |= (__int64)(*(++p));
	
	nMon  = (unsigned int)(j >> 60) & 0x0f;            //4  bit
	nDay  = (unsigned int)(j >> 55) & 0x1f;           //5  bit
	nHour = (unsigned int)(j >> 50) & 0x1f;           //5  bit
	nMin  = (unsigned int)(j >> 44) & 0x3f;           //6  bit
	nSec  = (unsigned int)(j >> 38) & 0x3f;           //6  bit
	nGate = (unsigned int)(j >> 20) & 0x03ffff;       //18 bit,���ֵ9999
	nNo   = (unsigned int)(j)		& 0x0fffff;       //20 bit,���ֵ999999
	sprintf(MsgId, "%02d%02d%02d%02d%02d%06d%06d", nMon, nDay, nHour, nMin, nSec, nGate, nNo);
	return 0;
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void TranMsgIdI64ToChar(unsigned char* MsgId, __int64 IMsgId)
{
/*
__int64 j = IMsgId;
MsgId[0] = (unsigned char)((j >> 56) & 0xff);
MsgId[1] = (unsigned char)((j >> 48) & 0xff);
MsgId[2] = (unsigned char)((j >> 40) & 0xff);
MsgId[3] = (unsigned char)((j >> 32) & 0xff);
MsgId[4] = (unsigned char)((j >> 24) & 0xff);
MsgId[5] = (unsigned char)((j >> 16) & 0xff);
MsgId[6] = (unsigned char)((j >> 8 ) & 0xff);
MsgId[7] = (unsigned char)((j) & 0xff);
return;
	*/
	__int64 j = IMsgId;
	MsgId[0] = (unsigned char)((j >> 56) & 0xff);
	MsgId[1] = (unsigned char)((j >> 48) & 0xff);
	MsgId[2] = (unsigned char)((j >> 40) & 0xff);
	MsgId[3] = (unsigned char)((j >> 32) & 0xff);
	MsgId[4] = (unsigned char)((j >> 24) & 0xff);
	MsgId[5] = (unsigned char)((j >> 16) & 0xff);
	MsgId[6] = (unsigned char)((j >>  8) & 0xff);
	MsgId[7] = (unsigned char)(j & 0xff);
	return;
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void TranSmitMsgID(unsigned char *pSrc, char *MsgId)
{
	unsigned char szOldNo[50] = {0};
	char szNewNo[50] = {0};
	__int64 iiii =_atoi64(pSrc);
	int iLen = 0;
	memset(MsgId,0,sizeof(MsgId));
	TranMsgIdI64ToChar(szOldNo,iiii);
	TranMsgId(szOldNo,szNewNo);
	iLen = strlen(szNewNo);
	if (iLen>19)
	{
		memcpy(MsgId,szNewNo+iLen-19,19);
	}
	else
	{
		strcpy(MsgId,szNewNo);
	}
}

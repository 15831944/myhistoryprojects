#ifndef  _CAMAZONCLASS_DEFINE__
#define  _CAMAZONCLASS_DEFINE__
#include "../GoodBase/DataDefine.h"
#include "../GoodBase/GoodOrderBase.h"
#include "../GoodBase/Function.h"
#include "AmazonEncode.h"
#define AMAZON_INVOICE_CONTENT "<option value='1' >ͼ��</option><option value='2' >����</option><option value='3' >��Ϸ</option><option value='4' >���</option><option value='5' >����</option><option value='6' >�칫��Ʒ</option><option value='7' >ϴ����Ʒ</option><option value='8' >��ױƷ</option><option value='9' >������Ʒ</option><option value='10' >�ͱ���Ʒ</option><option value='11' >��װ</option><option value='12' >���</option><option value='13' >��Ʒ</option><option value='14' >�ֻ�</option><option value='15' >�ҵ�</option><option value='16' >���</option><option value='17' >�����Ʒ</option><option value='18' >������Ʒ</option><option value='19' >����</option><option value='20' >Ьѥ</option><option value='21' >�ӱ�</option><option value='22' >ʳƷ</option><option value='23' >�̲�</option>"
#define AMAZON_PAY_INFO "<option value='cash'>�ֽ�</option><option value='mpos'>POS��ˢ��</option><option value='70'>֧����</option><option value='80'>�Ƹ�ͨ�˻�</option><option value='53'>���Ż�Ա�˻�</option><option value='4'>�����е��֧�����Ķ���</option><option value='11'>��������</option><option value='19'>�й���������</option><option value='30'>�й�ũҵ����</option><option value='31'>�й���������</option><option value='32'>�㶫��չ����</option><option value='33'>�й�����</option><option value='34'>��ҵ����</option><option value='35'>�ֶ���չ����</option><option value='37'>ƽ������</option><option value='39'>��������</option><option value='40'>��������</option><option value='41'>��������</option><option value='42'>�й���ͨ����</option><option value='43'>�й�����</option><option value='44'>����ʵҵ����</option><option value='45'>���ڷ�չ����</option>"
class CAmazonClass : public CGoodOrderBase , protected CAmazonEncode
{
public:
	CAmazonClass(void);
public:
	~CAmazonClass(void);

	virtual bool Login(CString strUser, CString strPwd, CString strCheckCode = "");
	virtual bool AddToCard(CString strGoodNo, int nGoodNum);
	int DoOrder(GoodOrderStru* pGoodOrderStr);
	virtual CString GetRetString(int iCode);
private:
	bool AddAddress(GoodOrderStru* pGoodOrderStr, int& iRetCode);
	CString GetLocationUrl();
	void EncodeAddrInfo(GoodOrderStru *pGoodOrderStr);
	bool AddAddressEx(CString& strRet, GoodOrderStru* pGoodOrderStr, int& iRetCode);
	bool SelectShipType(GoodOrderStru* pGoodOrderStr, int& iRetCode);
	bool SelectPayType(GoodOrderStru* pGoodOrderStr, int& iRetCode);
	bool Submit(GoodOrderStru* pGoodOrderStr, int& iRetCode);
	
	bool CheckConfigInfo(GoodOrderStru* pGoodStr, int& iRetCode);
	CString GetPayType(CString strPayId);
	CString GetWaitType(_SendTime eType);	
	void DeleteAddress();	
	bool GetShipData(CString &strSrc, CString& strValue, CString strFlag, CString& strRet, CString strOrderNo);
	void ClearCard();
	bool GetAllHiddenValue(CString strSrc, CString& strRet);
	string m_strHeader;
	string m_strServer;
	CString m_strLastQueryUrl;
	map<CString, CString> m_mapInvoiceCategoryID;
	map<CString, CString> m_mappaymentMethodId;
	CString m_strBillContent;
	CString m_strPayType;
	CString m_strpaymentMethodType;
	
	bool m_bNeedMoreInfo;
	CString m_strOrderNoEx;
	CString m_strItemList;

};
#endif

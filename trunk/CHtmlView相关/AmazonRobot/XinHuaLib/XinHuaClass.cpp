#include "StdAfx.h"
#include "XinHuaClass.h"

XinHuaClass::XinHuaClass(void)
{
	CString strAppPath = "";
	GetAppPath(strAppPath);
	m_strPicPath.append((LPCTSTR)strAppPath);
	m_strPicPath.append("checkcode.bmp");
	m_strPicPath = "C:\\2.raw";
	m_strHeader = "Content-Type:application/x-www-form-urlencoded";
	m_strHeader.append("\r\nUser-Agent:Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.2; SV1; .NET CLR 1.1.4322; .NET CLR 2.0.50727; .NET CLR 3.0.04506.30; .NET CLR 3.0.04506.648)");
	m_strHeader.append("\r\nAccept: */*");
	m_strHeader.append("\r\nConnection: Keep-Alive");
	InitProvinceInfo();	
	m_strListPrice = "";
	m_strSalePrice = "";
	m_bClearCart = false;
}
inline BYTE toHex(const BYTE &x)
{
	return x > 9 ? x + 55: x + 48;
}
XinHuaClass::~XinHuaClass(void)
{
}
bool XinHuaClass::Login(CString strUser,CString strPwd)
{
	int iCount = 0;
	while(iCount++ < 4)
	{
		bool bRet = DoLogin(strUser,strPwd);
		if (bRet)
		{
			return true;
		}
	}
	return false;
}

bool XinHuaClass::DoLogin(CString strUser,CString strPwd)
{
	string strServer = "www.xinhuabookstore.com";
	string strHeader = m_strHeader;
	srand(time(NULL));
	int iRet  = -1;
	int iLoopCount = 0;
	CString strFailReason = "";
	CString strObject = "";
	strObject.Format("/customer/login.jsp");
	time_t t;
	time(&t);
	strHeader.append("\r\nReferer:http://www.xinhuabookstore.com/customer/login.jsp");
	strObject.Format("/customer/getValidateNumber.htm?d=%d000",(long)t);
	iRet = m_webbase.GetPage(strObject,strHeader.c_str(),strServer.c_str(),80,true,m_strPicPath.c_str());
	if (iRet < 0)
	{
		return false;
	}	
	strObject = "/customer/login.htm";
	string strCode = m_objCheck.CheckCode(m_strPicPath);
	CString strData = "";
	ConvertGBKToUtf8(strUser);
	strData.Format("registername=%s&password=%s&testcode=%s&x=0&y=0",strUser,strPwd,strCode.c_str());
	iRet = m_webbase.PostPage(strObject,strData,strHeader.c_str(),strServer.c_str());
	CString strRet = CString(m_webbase.GetContentInfo());
	if (strRet.Find("The URL has moved") >=0 )
	{
		time(&t);
		strObject.Format("/customer/shoppingcart/clear.htm?timestamp=%d000",(long)t);		
		if (m_bClearCart)
		{
			//��չ��ﳵ
			m_webbase.GetPage(strObject,strHeader.c_str(),strServer.c_str());			
		}
		return true;
	}	
	return false;
}

int XinHuaClass::Query(XinHuaOrderStr &orderStr)
{
	m_bClearCart = false;
	if (!Login(orderStr.strUserID,orderStr.strPwd))
	{
		return LOGIN_FAILED;
	}
	return DoQuery(orderStr);
}
int XinHuaClass::DoQuery(XinHuaOrderStr &orderStr)
{
	//http://www.xinhuabookstore.com/customer/order/list.htm?pagination.pageSize=2000&pagination.skipSize=10&pagination.currentPage=1
	string strServer = "www.xinhuabookstore.com";
	string strHeader = m_strHeader;
	srand(time(NULL));
	int iRet  = -1;
	CString strObject = "";	
	CString strData = "";
	time_t t;
	int iCount = 0;
	//strHeader.append("\r\nReferer:http://www.xinhuabookstore.com/customer/shoppingcart/shoppingcart.jsp");
	strObject = "/customer/order/list.htm?pagination.pageSize=10000&pagination.skipSize=10&pagination.currentPage=1";
	iRet = m_webbase.GetPage(strObject,strHeader.c_str(),strServer.c_str());
	if (iRet < 0)
	{
		return -1;
	}
	CString strRet = CString(m_webbase.GetContentInfo());
	ConvertUtf8ToGBK(strRet);
	if (strRet.Find("�ҵĶ�����") < 0)
	{
		return GETQUERY_ERROR;
	}
	//���������еı����
	CString strFlag = "view.htm?orderId=";
	CStringArray array;
	CString strNo = "";
	CString strContent = "";
	while(true)
	{
		if( strRet.Find(strFlag) < 0 )
		{
			break;
		}
		strNo = GetKeyString(strRet,strFlag,"\"");
		strObject.Format("/customer/order/view.htm?orderId=%s",strNo);
		int iCount = 0;
		while(iCount++<3)
		{
			iRet = 	m_webbase.GetPage(strObject,strHeader.c_str(),strServer.c_str());
			if (iRet == 0)
			{
				break;
			}
		}
		strContent = CString(m_webbase.GetContentInfo());
		ConvertUtf8ToGBK(strContent);
		AnanyzerGood(strNo, strContent);
		strRet = strRet.Mid(strRet.Find(strFlag)+strFlag.GetLength());
	}

	return 0;
}

void XinHuaClass::AnanyzerGood(CString strNo,CString& strContent)
{
	strContent.Replace("\r\n"," ");
	strContent.Replace("\r"," ");
	strContent.Replace("\n"," ");
	strContent.MakeLower();
	strContent.Replace("\t"," ");
	strContent.Replace("      "," ");
	CString strFlag = "������";
	CString strRecordInfo = strNo;
	CString strTemp = strContent.Mid(strContent.Find(strFlag));
	CString strStatus = GetKeyString(strTemp,"(",")");		//״̬
	strStatus.Trim();
	CString strOrderTime = GetKeyString(strContent,"�µ�ʱ�䣺</strong>","<");
	strOrderTime.Trim();
	strRecordInfo += "\r\n" + strOrderTime;
	strRecordInfo += "\r\n" + strStatus;
	if (strStatus == "ȫ���ѷ�")
	{
		strFlag = "����ʱ��";
		strTemp = strContent.Mid(strContent.Find(strFlag));
		CString strSendTime = GetKeyString(strTemp,"</strong>","</td>");
		strSendTime.Trim();
		strRecordInfo += "\r\n" + strSendTime;
		//���͹�˾��</STRONG> ��ͨ��� 		<STRONG>�˵��ţ�</STRONG> 368778164603 <
		strFlag = "���͹�˾";
		strTemp = strContent.Mid(strContent.Find(strFlag)+strFlag.GetLength());
		CString strCompany = GetKeyString(strTemp,"</strong>","<");
		strFlag = "�˵���";
		strTemp = strTemp.Mid(strTemp.Find(strFlag)+strFlag.GetLength());
		CString strSendNo = GetKeyString(strTemp,">","<");
		strCompany.Trim();
		strSendNo.Trim();
		strRecordInfo += "\r\n" + strCompany;
		strRecordInfo += "\t" + strSendNo;
	}
	else if (strStatus == "��ȡ��")
	{
		//ȡ��ʱ��: 2011-04-07 20:53:35  ȡ��ԭ�� �ͻ�ȡ�� 
		strFlag = "ȡ��ʱ��";
		strTemp = strContent.Mid(strContent.Find(strFlag));
		CString strCancelTime = GetKeyString(strTemp,"</strong>","</td>");
		strCancelTime.Trim();
		strRecordInfo += "\r\n" + strCancelTime;
		strFlag = "ȡ��ԭ��";
		strTemp = strTemp.Mid(strTemp.Find(strFlag));
		CString strCancelReason = GetKeyString(strTemp,"</strong>","</td>");
		strCancelReason.Trim();
		strRecordInfo += "\r\n" + strCancelReason;
	}
	/*�� �� �ˣ� 
		�ż��� <BR>�ջ���ַ�� �й� ������ ������ �������·14�� �⽻�칫��¥2-102A �� 100600 <BR>��ϵ�绰�� 
		13691040531 </TD>*/
	strFlag = "�� �� ��";
	strTemp = strContent.Mid(strContent.Find(strFlag));
	strTemp = strTemp.Left(strTemp.Find("</td>"));
	strTemp.Replace("<br>","\r\n");
	strTemp.Replace("<br />","\r\n");
	strTemp.Trim();
	strRecordInfo += "\r\n" + strTemp;
	//�ͻ���ʽ��</STRONG> <BR>����ͻ����� . 	�ͻ�ʱ�䲻�� </TD>
	strFlag = "�ͻ���ʽ��";
	strTemp = strContent.Mid(strContent.Find(strFlag));
	CString strSendType = GetKeyString(strTemp,"</strong>","</td>");
	strSendType.Trim();
	strSendType.Replace("<br>","\t");
	strSendType.Replace("<br />","\t");
	strRecordInfo += "\r\n" + strSendType;

	//���ʽ��</STRONG> <BR>����ת��
	strFlag = "���ʽ��";
	strTemp = strContent.Mid(strContent.Find(strFlag));
	CString strPayType = GetKeyString(strTemp,"</strong>","</td>");
	strPayType.Trim();
	strPayType.Replace("<br>","");
	strPayType.Replace("<br />","");
	strRecordInfo += "\r\n" + strPayType;
	//ȱ������ʽ��</STRONG> �����е���Ʒ
	strFlag = "ȱ������ʽ";
	strTemp = strContent.Mid(strContent.Find(strFlag));
	CString strDeault = GetKeyString(strTemp,"</strong>","</td>");
	strDeault.Trim();
	strDeault.Replace("<br>","");
	strDeault.Replace("<br />","");
	strRecordInfo += "\r\nȱ������ʽ: " + strDeault;
	//�Ƿ���Ҫ��Ʊ
	strFlag = "�Ƿ���Ҫ��Ʊ";
	strTemp = strContent.Mid(strContent.Find(strFlag));
	CString strBill = GetKeyString(strTemp,"</strong>","</td>");
	strBill.Trim();
	strBill.Replace("<br>","");
	strBill.Replace("<br />","");
	strRecordInfo += "\r\n�Ƿ���Ҫ��Ʊ: " + strBill;
	strRecordInfo += "\r\n/************************************************************************/";
	strFlag = "/product/";
	//strTemp��ʼ
	while (true)
	{
		if (strTemp.Find(strFlag) < 0)
		{
			break;
		}
		CString strGoodInfo = GetKeyString(strTemp,strFlag,"</tr>");
		//����
		CString strTempEx = GetKeyString(strGoodInfo,">","<");
		strTempEx.Trim();
		strRecordInfo +="\r\n" + strTempEx;
		//�۸�
		strTempEx = "<td";
		strGoodInfo = strGoodInfo.Mid(strGoodInfo.Find(strTempEx)+strTempEx.GetLength());
		strTempEx = GetKeyString(strGoodInfo,">","<");
		strTempEx.Trim();
		strRecordInfo +="\r\n" + strTempEx;
		//�ۿ�
		strTempEx = "<td";
		strGoodInfo = strGoodInfo.Mid(strGoodInfo.Find(strTempEx)+strTempEx.GetLength());
		strTempEx = GetKeyString(strGoodInfo,">","<");
		strTempEx.Trim();
		strRecordInfo +="\r\n" + strTempEx;
		//����
		strTempEx = "<td";
		strGoodInfo = strGoodInfo.Mid(strGoodInfo.Find(strTempEx)+strTempEx.GetLength());
		strTempEx = GetKeyString(strGoodInfo,">","<");
		strTempEx.Trim();
		strRecordInfo +="\r\n" + strTempEx;
		//С��
		strTempEx = "<td";
		strGoodInfo = strGoodInfo.Mid(strGoodInfo.Find(strTempEx)+strTempEx.GetLength());
		strTempEx = GetKeyString(strGoodInfo,">","<");
		strTempEx.Trim();
		strRecordInfo +="\r\n" + strTempEx;
		//
		strRecordInfo += "\r\n/************************************************************************/";
		strTemp = strTemp.Mid(strTemp.Find(strFlag) + strFlag.GetLength());
	}
	CString strFile = "";
	strFile.Format("C:\\test\\%s.txt",strNo);

	CFile file;
	file.Open(strFile,CFile::modeCreate | CFile::modeWrite);
	file.Write(strRecordInfo,strRecordInfo.GetLength());
	file.Close();


	//��ȡ��
	//ȫ���ѷ�
	//���յ�
	

}

int XinHuaClass::Order(XinHuaOrderStr &orderStr)
{
	m_bClearCart = true;
	if (!Login(orderStr.strUserID,orderStr.strPwd))
	{
		return LOGIN_FAILED;
	}
	CArray<BookInfo,BookInfo>& bookInfo = orderStr.bookInfo;
	bool bRet = false;
	int iRet = -1;
	for (int i=0;i<bookInfo.GetCount();i++)
	{
		int iCount = 0;
		if (orderStr.iType == 0)	
		{
			iRet = -1;
			while(iRet < 0 && iCount ++ < 3)
			{
				iRet = AddToCart(bookInfo.GetAt(i).strNo,bookInfo.GetAt(i).fPrice);
			}
			
		}
		else	//ͨ����Ʒ����µ�
		{
			iRet = -1;
			while(iRet < 0 && iCount ++ < 3)
			{
				iRet = AddToCart(bookInfo.GetAt(i).strNo);
			}
			
		}
		if (iRet != 0)
		{
			return iRet;
		}
	}
	//���Ž���ȷ�϶���
    return DoOrder(orderStr);
}

int XinHuaClass::DoOrder(XinHuaOrderStr &orderStr)
{
	string strServer = "www.xinhuabookstore.com";
	string strHeader = m_strHeader;
	srand(time(NULL));
	int iRet  = -1;
	CString strObject = "";	
	CString strData = "";
	time_t t;
	int iCount = 0;
	strHeader.append("\r\nReferer:http://www.xinhuabookstore.com/customer/shoppingcart/shoppingcart.jsp");
	strObject = "/customer/checkout/consignee.htm";
	iRet = m_webbase.PostPage(strObject,"",strHeader.c_str(),strServer.c_str());
	if (iRet < 0)
	{
		return -1;
	}
	//<input type="hidden" value="-449216821" id="randomCode" name="randomCode"/>
	CString strRet = CString(m_webbase.GetContentInfo());
	int iPos = strRet.Find("randomCode");
	CString strTemp = strRet.Left(iPos);
	iPos = strTemp.ReverseFind('<');
	strTemp = strRet.Mid(iPos);
	CString strRandomCode = GetKeyString(strTemp,"value=\"","\"");
	//http://www.xinhuabookstore.com/customer/shoppingcart/shoppingcart.jsp
	//���빺������
	/************************************************************************/
	//�ջ�����Ϣ
	map<CString,CString>::iterator it = m_mapProvince.find(orderStr.strProvince);
	if (it == m_mapProvince.end())
	{
		return ADDR_ERROR;
	}
	CString strCountryNo = "23";
	CString strProvinceNo = it->second;
	CString strCityNo = "";
	CString strAreaNo = "";
	iRet = GetSubNo(strProvinceNo,orderStr.strCity,strCityNo);
	if (iRet != 0)
	{
		return iRet;
	}
	iRet = GetSubNo(strCityNo,orderStr.strTown,strAreaNo);
	if (iRet != 0)
	{
		return iRet;
	}
	time(&t);
	//POST�ύ����ȡ��ַ����
	ConvertGBKToUtf8(orderStr.strRecver);
	ConvertGBKToUtf8(orderStr.strAddr);
	
	strData.Format("orderTemplate.id=&orderTemplate.consignee=%s&orderTemplate.country.id=%s&orderTemplate.province.id=%s&orderTemplate.city.id=%s&orderTemplate.district.id=%s&orderTemplate.address=%s&orderTemplate.zipCode=%s&orderTemplate.mail=%s&orderTemplate.phone=%s&timestamp=%d000",
		orderStr.strRecver,
		strCountryNo,strProvinceNo,strCityNo,strAreaNo,
		orderStr.strAddr,
		orderStr.strPostCode,
		orderStr.strEmail,
		orderStr.strPhone,(long)t);
	strObject = "/customer/checkout/opearateConsignee.htm";
	strHeader = m_strHeader;
	strHeader.append("\r\nReferer:http://www.xinhuabookstore.com/customer/checkout/consignee.htm");
	//strHeader.append("\r\nx-requested-with:XMLHttpRequest");
	//strHeader.append("\r\nUA-CPU:x86");
	//strHeader.append("\r\nAccept-Language:zh-cn");
	iRet = m_webbase.PostPage(strObject,strData,strHeader.c_str(),strServer.c_str());
	strRet = CString(m_webbase.GetContentInfo());
	ConvertUtf8ToGBK(strRet);
	if (strRet.Find("�ɹ�") < 0)
	{
		return ADDR_ERROR;
	}	
	//�жϣ��Ƿ���ȷ�������õ���ַ��Ž��б���
	CString strAddrNo = GetKeyString(strRet,":",",");

	//���ͷ�ʽ
	/************************************************************************/
	time(&t);
	strObject.Format("/customer/checkout/deliverySetting.htm?areaId=%s&listPrice=%s&salePrice=%s&timestamp=%d000",
		strAreaNo,
		m_strListPrice,
		m_strSalePrice,
		(long)t);
	iRet = m_webbase.GetPage(strObject,strHeader.c_str(),strServer.c_str());
	strRet = CString(m_webbase.GetContentInfo());
	ConvertUtf8ToGBK(strRet);
	CString strdeliveryTypeId = "";
	CString strFee = "";	
	int iSendType = (int)orderStr.eSendType;
	if (iSendType > 2)
	{
		iSendType = 0;
	}
	time(&t);
	if (!orderStr.bCanNormalSend && strRet.Find("����ͻ�����") < 0 )
	{
		return NOSPEEDSEND;
	}
	if (strRet.Find("����ͻ�����") > 0)
	{		
		CString strTemp = strRet.Mid(strRet.Find("}"));
		strFee = GetKeyString(strTemp,"fee\":",",");
		strdeliveryTypeId = GetKeyString(strTemp,"id\":",",");
		strObject.Format("/customer/checkout/paymentSetting.htm?areaId=%s&orderTemplateId=%s&deliveryTypeId=%s&deliveryFee=%s&deliveryTime=%d&timestamp=%d000",
			strAreaNo,strAddrNo,strdeliveryTypeId,strFee,iSendType,(long)t);
	}
	else
	{
		CString strTemp = strRet.Mid(strRet.Find("}"));
		strFee = GetKeyString(strTemp,"fee\":",",");
		strdeliveryTypeId = GetKeyString(strTemp,"id\":",",");
		strObject.Format("/customer/checkout/paymentSetting.htm?areaId=%s&orderTemplateId=%s&deliveryTypeId=%s&deliveryFee=%s&deliveryTime=2&timestamp=%d000",
			strAreaNo,strAddrNo,strdeliveryTypeId,strFee,(long)t);
	}
	iRet = m_webbase.GetPage(strObject,strHeader.c_str(),strServer.c_str());
	if (iRet < 0)
	{
		return -1;
	}
	strRet = CString(m_webbase.GetContentInfo());
	ConvertUtf8ToGBK(strRet);
	if( strRet.Find("���ڹ˿Ϳ���ͨ��ȫ���κ�һ������") < 0 )
	{
		return ORDER_ERROR;
	}
	//���ʽ
	/************************************************************************/
	CString strPayType = "15"; //�������л�Ʊת�˷�ʽ
	//���ʽֻȡһ��
	/*
	[{"description":"�������ʾֻ��е�\"��ʽ\"��ѡ��\"�ֽ�\"��\"�տ�������\"����д���Ĵ��������ߵ����������޹�˾ ����\"���ʻ������ʻ�\"����д��100434268700010001��ͬʱҪ��д���Ľ�����������Ʒ���ڻ����������ʻ��󷢳���ע����������ʾֻ��ʱ������������ʾֻ�����;����ע�������š�<a href=\"\/help\/281\/411\" target=\"_blank\">�鿴�ʾֻ��ʹ�ð���<\/a>","id":3,"index":0,"logo":null,"name":"�ʾֻ��","type":0},
	{"description":"���ڹ˿Ϳ���ͨ��ȫ���κ�һ�����У����Ĵ��������ߵ����������޹�˾�Ľ������п������˻��������е�㣬����ʱ��һ��Ϊ�԰���������֮����2-7�������գ�����������Ʒ���ڻ����������ʻ��󷢳����������Ĵ��������ߵ����������޹�˾���������гɶ��»�֧���ʺţ�51001870836051517375 ע������������е��ʱ����������ڵ�㵥����;����ע�������š�","id":4,"index":0,"logo":null,"name":"����ת��","type":0},
	{"description":"","id":6,"index":0,"logo":"\/customer\/checkout\/images\/bank\/bank_004-2.gif","name":"��������","type":1},
	{"description":"","id":9,"index":0,"logo":"\/customer\/checkout\/images\/bank\/y_zfb.gif","name":"֧����","type":1},
	{"description":null,"id":22,"index":0,"logo":null,"name":"����֧��","type":1},
	{"description":null,"id":26,"index":0,"logo":null,"name":"֧��������","type":1},
	{"description":null,"id":33,"index":0,"logo":"\/customer eckout\/images\/bank\/y_cft.gif","name":"�Ƹ�ͨ","type":1}]
	*/
	time(&t);
	strObject.Format("/customer/checkout/payment_save_json.htm?orderTemplateId=%s&refundmentMeans=1&paymentId=%s&timestamp=%d000",strAddrNo,strPayType,(long)t);
	iRet = m_webbase.GetPage(strObject,strHeader.c_str(),strServer.c_str());
	if (iRet < 0)
	{
		return -1;
	}
	strRet = CString(m_webbase.GetContentInfo());
	ConvertUtf8ToGBK(strRet);
	if (strRet.Find("orderTemplateId") < 0)
	{
		return ORDER_ERROR;
	}
	CString strPayNo = GetKeyString(strRet,"orderTemplateId\":","}");
	CString strResHead = CString(m_webbase.GetCookie().c_str());
	
	strData.Format("orderTemplateId=%s&useAdvanceAccount=true&excludeUnion=0&randomCode=%s&presentId=&x=55&y=24",strPayNo,strRandomCode);
	strObject = "/customer/checkout/submit.htm";

	iRet = m_webbase.PostPage(strObject,strData,strHeader.c_str(),strServer.c_str());
	strRet = CString(m_webbase.GetContentInfo());
	ConvertUtf8ToGBK(strRet);
	if(strRet.Find("?orderId=") < 0)
	{
		return ORDER_ERROR;
	}
	return 0;
}

int XinHuaClass::GetSubNo(CString strParantNo,CString strKey,CString &strNo)
{
	strNo = "";
	string strServer = "www.xinhuabookstore.com";
	string strHeader = m_strHeader;
	srand(time(NULL));
	int iRet  = -1;
	CString strObject = "";	
	time_t t;
	int iCount = 0;	
	time(&t);
	strObject.Format("/js/area/area_json.jsp?id=%s",strParantNo);
	strHeader.append("\r\nReferer:http://www.xinhuabookstore.com/customer/checkout/consignee.htm");
	iCount = 0;
	iRet  = -1;
	while( iRet < 0 && iCount++ < 3 )
	{
		iRet = m_webbase.GetPage(strObject,strHeader.c_str(),strServer.c_str());		
	}
	if (iRet < 0)
	{
		return -1;
	}
	CString strRet = CString(m_webbase.GetContentInfo());
	ConvertUtf8ToGBK(strRet);
	if (strRet.Find(strKey) < 0)
	{
		return ADDR_ERROR;
	}
	CString strTemp = strRet.Left(strRet.Find(strKey));
	strTemp = strTemp.Mid(strTemp.ReverseFind('{'));
	strNo = GetKeyString(strTemp,":",",");
	if (strNo == "")
	{
		return ADDR_ERROR;
	}
	return 0;
}

int XinHuaClass::AddToCart(CString strGoodNo)
{
	string strServer = "www.xinhuabookstore.com";
	string strHeader = m_strHeader;
	srand(time(NULL));
	int iRet  = -1;
	int iLoopCount = 0;
	CString strFailReason = "";
	CString strObject = "";	
	time_t t;
	time(&t);
	strObject.Format("/customer/shoppingcart/add.htm?commodityId=%s&quantity=1&timestamp=%d000&flag=true",strGoodNo,(long)t);
	//�����Ʒ�����ﳵ
	iRet = m_webbase.GetPage(strObject,strHeader.c_str(),strServer.c_str());
	if (iRet < 0)
	{
		return -1;
	}
	CString strRet = CString(m_webbase.GetContentInfo());
	ConvertUtf8ToGBK(strRet);
	if (strRet.Find("\"listPrice\":") >=0 )
	{
		//�õ�listPrice��realPrice
		strRet = strRet.Mid(strRet.ReverseFind(']'));
		m_strListPrice = GetKeyString(strRet,"\"listPrice\":",",");
		m_strSalePrice = GetKeyString(strRet,"\"salePrice\":",",");
		return 0;
	}
	return -1;
}

int XinHuaClass::AddToCart(CString strBookNo,float fPrice)
{
	//������źͼ۸��ҵ���Ʒ���,�Աȵõ��۸�һ�µ���Ʒ���,��ӽ����ﳵ
	//http://www.xinhuabookstore.com/search/?keyword=9787530963852
	string strServer = "www.xinhuabookstore.com";
	string strHeader = m_strHeader;
	srand(time(NULL));
	int iRet  = -1;
	int iLoopCount = 0;
	CString strFailReason = "";
	CString strObject = "";	
	strObject.Format("/search/?keyword=%s",strBookNo);		
	iRet = m_webbase.GetPage(strObject,strHeader.c_str(),strServer.c_str());
	if (iRet < 0)
	{
		return -1;
	}
	CString strRet = CString(m_webbase.GetContentInfo());
	//�������۸񣬵õ���ȵ��ٵõ���Ʒ���
	//���ۣ�23.00Ԫ
	ConvertUtf8ToGBK(strRet);
	CString strTemp = strRet;
	CString strPrice = "";
	CString strFlag = "����";
	int iPos = -1;
	CString strNo = "";
	while(true)
	{
		iPos = strTemp.Find(strFlag);
		if (iPos < 0)
		{
			//δ�ҵ���Ʒ
			return BOOKNOTFOUND;
		}		
		strPrice = GetKeyString(strRet,"���ۣ�","Ԫ");
		if (atof(strPrice) == fPrice)
		{
			//ȡ����Ʒ���add_to_cart(1724439, 
			strNo = GetKeyString(strRet,"add_to_cart(",",");
			break;
		}
		strTemp = strTemp.Mid(iPos + strFlag.GetLength());
	}
	if(strNo == "")
	{
		//δ�ҵ��۸�ƥ�����Ʒ
		return NOMATCHPRICEBOOK;
	}
	time_t t;
	time(&t);
	strObject.Format("/customer/shoppingcart/add.htm?commodityId=%s&quantity=1&timestamp=%d000&flag=true",strNo,(long)t);
	//�����Ʒ�����ﳵ
	iRet = m_webbase.GetPage(strObject,strHeader.c_str(),strServer.c_str());
	if (iRet < 0)
	{
		return -1;
	}
	strRet = CString(m_webbase.GetContentInfo());
	ConvertUtf8ToGBK(strRet);
	if (strRet.Find("\"listPrice\":") >=0 )
	{
		//�õ�listPrice��realPrice
		strRet = strRet.Mid(strRet.ReverseFind(']'));
		m_strListPrice = GetKeyString(strRet,"\"listPrice\":",",");
		m_strSalePrice = GetKeyString(strRet,"\"salePrice\":",",");
		return 0;
	}
	return -1;
}

int XinHuaClass::GetAppPath(CString &strAppPath)
{
	int iRet = -3;
	try
	{
		TCHAR szBuff[_MAX_PATH];
		VERIFY(::GetModuleFileName(AfxGetApp()->m_hInstance, szBuff, _MAX_PATH));
		strAppPath.Format("%s",szBuff);
		int pos=strAppPath.ReverseFind('\\');
		strAppPath=strAppPath.Left(pos+1);		
		iRet = 0;
	}
	catch (...)
	{

	}
	return iRet;	
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
CString XinHuaClass::GetKeyString(CString &srcStr,CString strBefore,CString strAfter)
{
	int iPosBefore = 0;
	int iPosAfter = 0;
	iPosBefore = srcStr.Find(strBefore) + strBefore.GetLength();
	if (iPosBefore < strBefore.GetLength())
	{
		return "";
	}
	iPosAfter = srcStr.Find(strAfter,iPosBefore);
	if (iPosBefore < 0 || iPosAfter < 0 || iPosAfter - iPosBefore < 0 )
	{
		return "";
	}
	else
	{
		return srcStr.Mid(iPosBefore,iPosAfter - iPosBefore);
	}
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void XinHuaClass::ConvertUtf8ToGBK(CString& strUtf8) 
{
	int len = 0;
	unsigned short * wszGBK = NULL;
	char * szGBKt = NULL;
	char * szGBKs = NULL;

	len = MultiByteToWideChar(CP_UTF8, 0, (LPCTSTR)strUtf8, -1, NULL,0);
	wszGBK = new unsigned short[len+1];
	memset(wszGBK, 0, len * 2 + 2);
	MultiByteToWideChar(CP_UTF8, 0, (LPCTSTR)strUtf8, -1, (LPWSTR)wszGBK, len);

	len = WideCharToMultiByte(CP_ACP, 0, (LPWSTR)wszGBK, -1, NULL, 0, NULL, NULL); 
	szGBKt  =new char[len + 1];
	memset(szGBKt, 0, len + 1);
	WideCharToMultiByte (CP_ACP, 0, (LPWSTR)wszGBK, -1, szGBKt, len, NULL,NULL);

	szGBKs  =new char[len + 1];
	memset(szGBKs, 0, len + 1);
	//ת��Gb�뷱�嵽Gb����壬ʹ��API����LCMapString 
	LCMapString( 0x0804, LCMAP_SIMPLIFIED_CHINESE, szGBKt, -1, szGBKs, len);
	strUtf8 = szGBKs;
	delete[] szGBKs;
	delete[] szGBKt;
	delete[] wszGBK;
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void XinHuaClass::ConvertGBKToUtf8(CString& strGBK) 
{
	int len=MultiByteToWideChar(CP_ACP, 0, (LPCTSTR)strGBK, -1, NULL,0);
	unsigned short * wszUtf8 = new unsigned short[len+1];
	memset(wszUtf8, 0, len * 2 + 2);
	MultiByteToWideChar(CP_ACP, 0, (LPCTSTR)strGBK, -1, (LPWSTR)wszUtf8, len);

	len = WideCharToMultiByte(CP_UTF8, 0, (LPWSTR)wszUtf8, -1, NULL, 0, NULL, NULL); 
	char *szUtf8=new char[len + 1];
	memset(szUtf8, 0, len + 1);
	WideCharToMultiByte (CP_UTF8, 0, (LPWSTR)wszUtf8, -1, szUtf8, len, NULL,NULL);

	strGBK = szUtf8;
	delete[] szUtf8;
	delete[] wszUtf8;
	return ;
}
void XinHuaClass::InitProvinceInfo()
{
	CString strData = "{'id':152,'name':'������'},{'id':177,'name':'�����'},{'id':174,'name':'�Ϻ���'},{'id':183,'name':'������'},	{'id':165,'name':'����ʡ'},{'id':182,'name':'�㽭ʡ'},{'id':155,'name':'�㶫ʡ'},{'id':156,'name':'����'},{'id':153,'name':'����ʡ'},{'id':162,'name':'����ʡ'},{'id':163,'name':'����ʡ'},{'id':175,'name':'�Ĵ�ʡ'},{'id':171,'name':'ɽ��ʡ'},{'id':150,'name':'����ʡ'},{'id':160,'name':'����ʡ'},{'id':166,'name':'����ʡ'},{'id':159,'name':'�ӱ�ʡ'},{'id':172,'name':'ɽ��ʡ'},{'id':157,'name':'����ʡ'},{'id':181,'name':'����ʡ'},{'id':154,'name':'����ʡ'},{'id':180,'name':'�½�'},{'id':161,'name':'������ʡ'},{'id':169,'name':'����'},{'id':173,'name':'����ʡ'},{'id':178,'name':'����'},{'id':168,'name':'���ɹ�'},{'id':164,'name':'����ʡ'},{'id':170,'name':'�ຣʡ'},{'id':167,'name':'����ʡ'},{'id':158,'name':'����ʡ'},{'id':179,'name':'���'},{'id':151,'name':'����'},{'id':176,'name':'̨��'},";
	CString strProvince = "";
	CString strNo = "";
	while(true)
	{
		CString strTemp = strData.Left(strData.Find("}"));		
		if (strTemp.GetLength() < 3)
		{
			break;
		}
		strData = strData.Mid(strData.Find("}") + 1);
		strProvince = GetKeyString(strTemp,"name':'","'");
		strNo = GetKeyString(strTemp,"'id':",",");
		m_mapProvince[strProvince] = strNo;
	}	
}

int XinHuaClass::Test(XinHuaOrderStr &orderStr)
{
	string strServer = "www.xinhuabookstore.com";
	string strHeader = m_strHeader;
	srand(time(NULL));
	int iRet  = -1;
	int iLoopCount = 0;
	CString strFailReason = "";
	CString strObject = "";	
	time_t t;
	time(&t);
	strObject.Format("/js/area/area_json.jsp?id=23");
	//�����Ʒ�����ﳵ
	iRet = m_webbase.GetPage(strObject,strHeader.c_str(),strServer.c_str());
	if (iRet < 0)
	{
		return -1;
	}
	CString strRet = CString(m_webbase.GetContentInfo());
	ConvertUtf8ToGBK(strRet);
	if (strRet.Find("\"listPrice\":") >=0 )
	{
		return 0;
	}
	return -1;	
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
CString XinHuaClass::URLEncode(CString sIn)
{
	CString sOut;
	const int nLen = sIn.GetLength() + 1;
	register LPBYTE pOutTmp = NULL;
	LPBYTE pOutBuf = NULL;
	register LPBYTE pInTmp = NULL;
	LPBYTE pInBuf =(LPBYTE)sIn.GetBuffer(nLen);
	//alloc out buffer
	pOutBuf = (LPBYTE)sOut.GetBuffer(nLen*3);

	if(pOutBuf)
	{
		pInTmp   = pInBuf;
		pOutTmp = pOutBuf;
		// do encoding
		while (*pInTmp)
		{
			if(isalnum(*pInTmp) || '-'==*pInTmp || '_'==*pInTmp || '.'==*pInTmp)
				*pOutTmp++ = *pInTmp;
			else if(isspace(*pInTmp))
				*pOutTmp++ = '+';
			else
			{
				*pOutTmp++ = '%';
				*pOutTmp++ = toHex(*pInTmp>>4);//��4λ
				*pOutTmp++ = toHex(*pInTmp%16);//��4λ
			}
			pInTmp++;
		}
		*pOutTmp = '\0';
		sOut.ReleaseBuffer();
	}
	sIn.ReleaseBuffer();

	return sOut;
}

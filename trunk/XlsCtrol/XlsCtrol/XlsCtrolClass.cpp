#include "StdAfx.h"
#include "XlsCtrolClass.h"


//#define TEST_TEST
CXlsCtrolClass::CXlsCtrolClass(void)
{
}

CXlsCtrolClass::~CXlsCtrolClass(void)
{
}

//Ҳ�ǵ���
bool CXlsCtrolClass::ImportEx(CString strFile,vector<ExpressStr> &vecStr)
{
	CDatabase database;
	CString sSql;
	CString sItem1, sItem2;
	CString sDriver;
	CString sDsn;
	CString sFile = strFile;             // ������ȡ��Excel�ļ���
	vecStr.clear();
	// �����Ƿ�װ��Excel���� "Microsoft Excel Driver (*.xls)" 
	sDriver = GetExcelDriver();
	if (sDriver.IsEmpty())
	{
		return false;
	}
	sDsn.Format("ODBC;DRIVER={%s};DSN='';DBQ=%s", sDriver, sFile);
	TRY
	{
		// �����ݿ�(��Excel�ļ�)
		database.Open(NULL, false, false, sDsn);
		int iSheetCount =1;
		while (true)
		{
			CRecordset adoRst(&database);

			// ���ö�ȡ�Ĳ�ѯ���.
			sSql = "SELECT * from ";
			sSql.Format("SELECT * from [Sheet%d$]",iSheetCount);
			// ִ�в�ѯ���
			try
			{
				adoRst.Open(CRecordset::forwardOnly, sSql, CRecordset::readOnly);
			}			
			catch (CException* e)
			{
				int ii = 0;
				char strMessage[MAX_PATH];
				e->GetErrorMessage(strMessage, sizeof(strMessage) - 1);
				e->Delete();
				CString strError = CString(strMessage);
				if (strError.Find("����һ����Ч���ơ���ȷ������������Ч���ַ�����") >=0 )
				{
					//�������Ѿ�������
					break;
				}
			}

			if (adoRst.IsEOF())		//�Ѿ�Ϊ����
			{
				break;
			}
			ExpressStr ExpressInfo;
			BookStr bookStr;
			CString strTemp = "";
			//CString strColumnName = adoRst.GetFieldName(0);
			bool bColumnFlag = false;//�Ƿ��Ѿ���ȡ������
			bool bNewStruInfo = false;
			bool bInfoEnd = false;
			bool bReadBookInfo = false;
			while (!adoRst.IsEOF())
			{
				if (bInfoEnd)
				{
					vecStr.push_back(ExpressInfo);
					bNewStruInfo = false;
					bInfoEnd = false;
					ExpressInfo.vecBookInfo.clear();
					bReadBookInfo = false;
				}
				adoRst.GetFieldValue((short)0,strTemp);
				if (strTemp != "")			//OrderNo
				{
					adoRst.GetFieldValue((short)2,ExpressInfo.strOrderNo);
					bNewStruInfo = true;
					adoRst.MoveNext();
					continue;
				}
				strTemp = "";
				adoRst.GetFieldValue((short)1,strTemp);		
				if (strTemp == "")		//One Record End
				{
					bInfoEnd = true;
					adoRst.MoveNext();
					continue;
				}
				else if (strTemp == "ISBN")	//Book Info
				{
					bReadBookInfo = true;
					adoRst.MoveNext();
					continue;
				}
				else if (bReadBookInfo)		// add book info 
				{
					bookStr.strISBN = strTemp;
					adoRst.GetFieldValue((short)2,bookStr.strName);
					adoRst.GetFieldValue((short)3,bookStr.strPress);
					adoRst.GetFieldValue((short)4,bookStr.strPrice);
					adoRst.GetFieldValue((short)5,bookStr.strNum);
					adoRst.GetFieldValue((short)6,bookStr.strSellPrice);
					ExpressInfo.vecBookInfo.push_back(bookStr);
				}
				else if (strTemp == "������ַ")
				{
					adoRst.GetFieldValue((short)2,ExpressInfo.strAddr);
				}
				else if (strTemp == "�� ϵ ��")
				{
					adoRst.GetFieldValue((short)2,ExpressInfo.strRecver);
				}
				else if (strTemp == "��ϵ��ʽ")
				{
					adoRst.GetFieldValue((short)2,ExpressInfo.strPhone);
				}
				else if (strTemp == "��    ע")
				{
					adoRst.GetFieldValue((short)2,ExpressInfo.strMemo);
				}
				else if (strTemp == "����ʱ��")
				{
					adoRst.GetFieldValue((short)2,ExpressInfo.strPayTime);
				}
				else if (strTemp == "�������")
				{
					adoRst.GetFieldValue((short)2,ExpressInfo.strExpressCompany);
				}
				else if (strTemp == "��ݵ���")
				{
					adoRst.GetFieldValue((short)2,ExpressInfo.strExpressNo);
				}
				adoRst.MoveNext();
			}
			if (ExpressInfo.vecBookInfo.size()>0)
			{
				vecStr.push_back(ExpressInfo);
			}
			iSheetCount++;
		}		
		// �ر����ݿ�
		database.Close();
	}
	CATCH(CDBException, e)
	{
		// ���ݿ���������쳣ʱ...
		AfxMessageBox("���ݿ����: " + e->m_strError);
	}
	END_CATCH;

	return true;
}

bool CXlsCtrolClass::GetTables(CString strFile,CStringArray &strTableArray)
{
	CAdoConnection adoConn;
	if (!adoConn.IsOpen())
	{
		if(adoConn.ConnectExcel(strFile))
		{
			adoConn.OpenSchema(strTableArray);
			adoConn.Close();
			return true;
		}
		else
		{
			return false;
		}
	}	
}
//���
bool CXlsCtrolClass::Add(CString strFile,vector<ExpressStr> &vecStr)
{
	return Export(strFile,vecStr,true);
}
//����
bool CXlsCtrolClass::Export(CString strFile,vector<ExpressStr> &vecStr,bool bAppend)
{
	CDatabase database;
	CString sDriver;
	CString sExcelFile = strFile; 
	CString sSql;	
	
	CString tableName = "Sheet1";

	if (!bAppend)
	{
		if (_access(strFile,0) ==0)
		{
			CFile::Remove(strFile);
		}
	}	
	// �����Ƿ�װ��Excel���� "Microsoft Excel Driver (*.xls)" 
	sDriver = GetExcelDriver();
	if (sDriver.IsEmpty())
	{		
		return false;
	}

	CStringArray strTableArray;
	GetTables(strFile,strTableArray);

	// �������д�ȡ���ַ���
	sSql.Format("DRIVER={%s};DSN='';FIRSTROWHASNAMES=1;READONLY=FALSE;CREATE_DB=\"%s\";DBQ=%s",sDriver, sExcelFile, sExcelFile);
	CString strTemp;
	int iTemp;
	// �������ݿ� (��Excel����ļ�)
	if( !database.OpenEx(sSql,CDatabase::noOdbcDialog))	
	{		
		return false;
	}
	int iCount = 1;
	int iNodeCount = 0;
	int iSheetCount = 1;
	if (!bAppend)
	{
		iSheetCount = 1;
	}
	else //�õ�����Ŀǰ�Ѿ����ڵı���Ŀ
	{
		iSheetCount = strTableArray.GetSize();
	}	
	vector<ExpressStr>::iterator it;
	bool bTableChecked = false;
	bool bExist = false;
	tableName.Format("Sheet%d$",iSheetCount);
	int iSize = vecStr.size();

	for (it=vecStr.begin();it != vecStr.end();)
	{
		if (!bTableChecked)
		{
			bExist = false;
			tableName.Format("Sheet%d",iSheetCount);
			for (int i=strTableArray.GetCount()-1;i>=0;i--)
			{
				if (strTableArray.GetAt(i) == tableName)
				{
					bExist = true;
					bTableChecked = true;
					break;
				}
			}			
		}
		if (!bExist)
		{
			strTemp.Format("a TEXT,b TEXT,c TEXT,d TEXT,e TEXT,f TEXT,g TEXT");
			// ������ṹ			
			sSql = "CREATE TABLE [" + tableName + "] ( " + strTemp +  " ) ";
			int i = 0;
			database.ExecuteSQL(sSql);
			strTableArray.Add(tableName);
			bTableChecked = true;
			bExist = true;
		}
#ifdef TEST_TEST
		if(iNodeCount++ >= 10000)
			break;
#endif
		if (iCount + it->vecBookInfo.size() + 12 >= 32767)
		{

			iCount = 0;
			bTableChecked = false;
			iSheetCount++;
			continue;
		}
		//memcpy(&expressStr,&it,sizeof(expressStr));
		try
		{
			database.BeginTrans();
			sSql.Format("INSERT INTO %s ([a],[b],[c],[d],[e],[f],[g])values('%s','%s','%s','%s','%s','%s','%s')",tableName,it->strOrderNo,"�������",it->strOrderNo,"","","","");
			database.ExecuteSQL(sSql);
			sSql.Format("INSERT INTO %s ([a],[b],[c],[d],[e],[f],[g])values('%s','%s','%s','%s','%s','%s','%s')",tableName,it->strOrderNo,"������ַ",it->strAddr,"","","","");
			database.ExecuteSQL(sSql);
			sSql.Format("INSERT INTO %s ([a],[b],[c],[d],[e],[f],[g])values('%s','%s','%s','%s','%s','%s','%s')",tableName,it->strOrderNo,"�� ϵ ��",it->strRecver,"","","","");
			database.ExecuteSQL(sSql);
			sSql.Format("INSERT INTO %s ([a],[b],[c],[d],[e],[f],[g])values('%s','%s','%s','%s','%s','%s','%s')",tableName,it->strOrderNo,"��ϵ��ʽ",it->strPhone,"","","","");
			database.ExecuteSQL(sSql);
			sSql.Format("INSERT INTO %s ([a],[b],[c],[d],[e],[f],[g])values('%s','%s','%s','%s','%s','%s','%s')",tableName,it->strOrderNo,"��    ע",it->strMemo,"","","","");
			database.ExecuteSQL(sSql);
			sSql.Format("INSERT INTO %s ([a],[b],[c],[d],[e],[f],[g])values('%s','%s','%s','%s','%s','%s','%s')",tableName,it->strOrderNo,"����ʱ��",it->strPayTime,"","","","");
			database.ExecuteSQL(sSql);
			sSql.Format("INSERT INTO %s ([a],[b],[c],[d],[e],[f],[g])values('%s','%s','%s','%s','%s','%s','%s')",tableName,it->strOrderNo,"�������",it->strExpressCompany,"","","","");
			database.ExecuteSQL(sSql);
			sSql.Format("INSERT INTO %s ([a],[b],[c],[d],[e],[f],[g])values('%s','%s','%s','%s','%s','%s','%s')",tableName,it->strOrderNo,"��ݵ���",it->strExpressNo,"","","","");
			database.ExecuteSQL(sSql);
			sSql.Format("INSERT INTO %s ([a],[b],[c],[d],[e],[f],[g])values('%s','%s','%s','%s','%s','%s','%s')",tableName,it->strOrderNo,"ISBN","����","������","ԭ��","����","�ۼ�");
			database.ExecuteSQL(sSql);
			vector<BookStr>::iterator itEx;
			for (itEx=it->vecBookInfo.begin();itEx!=it->vecBookInfo.end();itEx++)
			{
				sSql.Format("INSERT INTO %s ([a],[b],[c],[d],[e],[f],[g])values('%s','%s','%s','%s','%s','%s','%s')",tableName,"",itEx->strISBN,itEx->strName,itEx->strPress,itEx->strPrice,itEx->strNum,itEx->strSellPrice);
				database.ExecuteSQL(sSql);
				iCount++;
			}
			sSql.Format("INSERT INTO %s ([a],[b],[c],[d],[e],[f],[g])values('%s','%s','%s','%s','%s','%s','%s')",tableName,"","","","","","","");
			database.ExecuteSQL(sSql);
			database.CommitTrans();
			iCount+=10;			
		}		
		catch (CException* e)
		{
			database.Rollback();
			sSql.Format("delete from [%s] where [a] = '%s'",tableName,it->strOrderNo);
			database.ExecuteSQL(sSql);
			bTableChecked = false;
			iSheetCount++;			
		}
#ifndef TEST_TEST
		it++;
#endif
	}
	return true;
}


/************************************************************************/
/*                                                                      */
/************************************************************************/
CString CXlsCtrolClass::GetExcelDriver()
{
	char szBuf[2001];
	WORD cbBufMax = 2000;
	WORD cbBufOut;
	char *pszBuf = szBuf;
	CString sDriver;

	// ��ȡ�Ѱ�װ����������(������odbcinst.h��)
	if (!SQLGetInstalledDrivers(szBuf, cbBufMax, &cbBufOut))
		return "";

	do
	{
		if (strstr(pszBuf, "Excel") != 0)
		{
			//���� !
			sDriver = CString(pszBuf);
			break;
		}
		pszBuf = strchr(pszBuf, '\0') + 1;
	}
	while (pszBuf[1] != '\0');

	return sDriver;
}

//����
bool CXlsCtrolClass::Import(CString strFile,vector<ExpressStr> &vecStr)
{
	CAdoConnection adoConn;
	CAdoRecordSet adoRst;
	vecStr.clear();
	bool bRet = false;
	if (!adoConn.IsOpen())
	{
		if(adoConn.ConnectExcel(strFile))
		{
			adoRst.SetAdoConnection(&adoConn);
			adoRst.SetCursorLocation(adUseClient);
		}
		else
		{
			return false;
		}
	}
	int iSheetCount = 1;
	while (true)
	{
	
		CString strSQL = "SELECT * from [Sheet1$]";
		strSQL.Format("SELECT * from [Sheet%d$]",iSheetCount);

		
		if( !adoRst.Open(strSQL, adCmdText))	//�Ҳ�����
		{
			//if (strError.Find("����һ����Ч���ơ���ȷ������������Ч���ַ�����") >=0 )
			adoRst.Close();
			break;
		}
		CString strFormat = "";
		CString strPhone = "";
		int nFields  = adoRst.GetFieldsCount();
		if (nFields != 7)
		{
			//AfxMessageBox("�ļ���ʽ����ȷ");
			adoRst.Close();
			break;
		}
		ExpressStr ExpressInfo;
		BookStr bookStr;
		CString strTemp = "";
		CString strColumnName = adoRst.GetFieldName(0);
		bool bColumnFlag = false;//�Ƿ��Ѿ���ȡ������
		bool bNewStruInfo = false;
		bool bInfoEnd = false;
		bool bReadBookInfo = false;
		while (!adoRst.IsEOF())
		{
			if (bInfoEnd)
			{
				vecStr.push_back(ExpressInfo);
				bNewStruInfo = false;
				bInfoEnd = false;
				ExpressInfo.vecBookInfo.clear();
				bReadBookInfo = false;
			}			
			strTemp = "";
			adoRst.GetCollect((long)1,strTemp);		
			if (strTemp == "")		//One Record End
			{
				bInfoEnd = true;
				adoRst.MoveNext();
				continue;
			}
			else if (strTemp == "ISBN")	//Book Info
			{
				bReadBookInfo = true;
				adoRst.MoveNext();
				continue;
			}
			else if (bReadBookInfo)		// add book info 
			{
				bookStr.strISBN = strTemp;
				adoRst.GetCollect((long)2,bookStr.strName);
				adoRst.GetCollect((long)3,bookStr.strPress);
				adoRst.GetCollect((long)4,bookStr.strPrice);
				adoRst.GetCollect((long)5,bookStr.strNum);
				adoRst.GetCollect((long)6,bookStr.strSellPrice);
				ExpressInfo.vecBookInfo.push_back(bookStr);
			}
			else if (strTemp == "�������")
			{
				adoRst.GetCollect((long)2,ExpressInfo.strOrderNo);
			}
			else if (strTemp == "������ַ")
			{
				adoRst.GetCollect((long)2,ExpressInfo.strAddr);
			}
			else if (strTemp == "�� ϵ ��")
			{
				adoRst.GetCollect((long)2,ExpressInfo.strRecver);
			}
			else if (strTemp == "��ϵ��ʽ")
			{
				adoRst.GetCollect((long)2,ExpressInfo.strPhone);
			}
			else if (strTemp == "��    ע")
			{
				adoRst.GetCollect((long)2,ExpressInfo.strMemo);
			}
			else if (strTemp == "����ʱ��")
			{
				adoRst.GetCollect((long)2,ExpressInfo.strPayTime);
			}
			else if (strTemp == "�������")
			{
				adoRst.GetCollect((long)2,ExpressInfo.strExpressCompany);
			}
			else if (strTemp == "��ݵ���")
			{
				adoRst.GetCollect((long)2,ExpressInfo.strExpressNo);
			}						
			adoRst.MoveNext();
		}
		if (ExpressInfo.vecBookInfo.size()>0)
		{
			vecStr.push_back(ExpressInfo);
		}
		adoRst.Close();
		bRet = true;
		iSheetCount++;
	}
	adoConn.Close();
	return bRet;
}

//�޸�
bool CXlsCtrolClass::Modify(CString strFile,ExpressStr &expressStr)
{
	//�Ȼ�����еı�
	CStringArray strArray;
	GetTables(strFile,strArray);
	if (strArray.GetCount() < 1)
	{
		return false;
	}
	CAdoConnection adoConn;
	CAdoRecordSet adoRst;
	if (!adoConn.IsOpen())
	{
		if(adoConn.ConnectExcel(strFile))
		{
			adoRst.SetAdoConnection(&adoConn);
			adoRst.SetCursorLocation(adUseClient);
		}
		else
		{
			return false;
		}
	}
	//��ȡ�����ڵı�
	CString strTable = "";
	CString strSQL = "update [Sheet1$] set [a]='1'";
	for (int i=0;i<strArray.GetCount();i++)
	{
		strSQL.Format("select count(*) from [%s$] where [a]='%s'",strArray.GetAt(i),expressStr.strOrderNo);
		if( !adoRst.Open(strSQL, adCmdText) )
		{
			//CString strError = adoConn.GetLastErrorText();
			adoRst.Close();
			break;
		}
		if (!adoRst.IsEOF())
		{
			CString strTemp ="";
			adoRst.GetCollect((long)0,strTemp);
			if (atol(strTemp) > 0)
			{
				strTable = strArray.GetAt(i);
				adoRst.Close();
				break;
			}			
		}
		adoRst.Close();
	}
	if (strTable == "")
	{
		adoRst.Close();
		adoConn.Close();
		return false;
	}
	//�õ��������ڵı�
	//��ʼ�޸�
	strSQL.Format("select * from [%s$] where [a]='%s'",strTable,expressStr.strOrderNo);

	if( !adoRst.Open(strSQL, adCmdText) )
	{
		CString strError = adoConn.GetLastErrorText();
		adoRst.Close();
		return false;
	}
	CString strTemp ="";	
	while(!adoRst.IsEOF())
	{
		adoRst.GetCollect((long)1,strTemp);
		if (strTemp == "������ַ")
		{
			adoRst.PutCollect((long)2,expressStr.strAddr);
		}
		else if (strTemp == "�� ϵ ��")
		{
			adoRst.PutCollect((long)2,expressStr.strRecver);
		}
		else if (strTemp == "��ϵ��ʽ")
		{
			adoRst.PutCollect((long)2,expressStr.strPhone);
		}
		else if (strTemp == "��    ע")
		{
			adoRst.PutCollect((long)2,expressStr.strMemo);
		}
		else if (strTemp == "����ʱ��")
		{
			adoRst.PutCollect((long)2,expressStr.strPayTime);
		}
		else if (strTemp == "�������")
		{
			adoRst.PutCollect((long)2,expressStr.strExpressCompany);
		}
		else if (strTemp == "��ݵ���")
		{
			adoRst.PutCollect((long)2,expressStr.strExpressCompany);
		}
		adoRst.MoveNext();
	}
	adoRst.UpdateBatch();
	adoRst.Close();
	adoConn.Close();
	return true;
}

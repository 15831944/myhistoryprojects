#include "StdAfx.h"
#include ".\keyword.h"
#include <io.h>
CKeyWord::CKeyWord(void)
{
}

CKeyWord::~CKeyWord(void)
{
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
//0�ɹ�,1ʧ��,2�ؼ����Ѿ�����
int CKeyWord::AddKeyWord(CString strKeyWord)
{
// 	strKeyWord.MakeLower();
// 	if ( IsInKeyMap( strKeyWord ) )
// 	{
// 		return 2;	//�Ѿ�����
// 	}
// 	FILE *fp;
// 	char szfile[1000];
// 	sprintf(szfile,m_strFileName);
// 	fp=fopen(szfile,"a+");
// 	fprintf(fp,"%s\r\n",strKeyWord);
// 	fclose(fp);
// 	AddtoKeyMap(strKeyWord);
	return 0;
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
//1�ɹ�,0�Ѿ�����
int CKeyWord::AddtoKeyMap(CString strKeyWord)
{
	strKeyWord.MakeLower();
	try
	{
		if ( m_keyWordMap[strKeyWord] == 1)
		{
			return 0;
		}
		m_keyWordMap[strKeyWord] = 1;	//1Ϊ����
		return 1;
	}	
	catch (...)
	{
		return 2;
	}	
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
//�жϹؼ����Ƿ��Ѿ���MAP��
bool CKeyWord::IsInKeyMap(CString strKeyWord)
{
	strKeyWord.MakeLower();
	bool existFlag = false;
	if (m_keyWordMap[strKeyWord] == 1)				//����
	{
		existFlag = true;
	}
	else					//����
	{
		existFlag = false;
	}
	return existFlag;
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
//0������-1�쳣,1û�зǷ��ؼ���
int CKeyWord::KeyWordCheck(CString strSRC,CString &resultStr)
{
	resultStr = "";
	map<CString,int>::iterator it;   
	CString tempStr = "";
	int idx = 0;
	try
	{
		for(it=m_keyWordMap.begin();it!=m_keyWordMap.end();++it)
		{
			tempStr = it->first;
			idx = strSRC.Find(tempStr,0);  //�ҵ�һ����¼
			if(idx != -1)//����ҵ�
			{
				resultStr = tempStr;
				return 0;
			}				
		}
		return 1;
	}
	catch (...)
	{
	}
	return -1;
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
//0�ɹ�,-1ʧ��,-2�������ݿ�ʧ�ܣ�-3�쳣,-4û�йؼ���
int CKeyWord::LoadAllKeyWords(int &iCount)
{
	iCount = 0;
	m_keyWordMap.clear();
	try
	{
		if(_access(m_strFileName,0)!=0)
		{
			return -1;
		}
		CStdioFile   file;
		CString   cstr ="";
		CFileException   fe;
		if(file.Open(m_strFileName,CFile::modeRead,&fe))   
		{ 
			file.SeekToBegin();
			while (file.ReadString(cstr))
			{
				cstr.Trim();
				if (cstr != "")
				{
					if ( AddtoKeyMap(cstr) == 1)
					{
						iCount ++;
					}
				}
			}
			file.Close();
		}
		if ( iCount > 0 )
		{
			return 0;
		}
		return -4;
	}
	catch(...)
	{
		return -3;
	}
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
//��ȡ�ַ�������
int CKeyWord::GetMsgULen(CString Msg)
{
	if (Msg.GetLength() > 10000)
	{
		return -1;
	}
	wchar_t wszTemp[10000] = {0};
	try
	{
		MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, Msg, strlen(Msg), wszTemp, sizeof(wszTemp));
		int len = wcslen(wszTemp);
		if (len < 0)
		{
			return -3;
		}
		return len;
	}
	catch(...)
	{
	}
	return -9;
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
//��ȡ���ѯ��ƥ��Ĺؼ���
int CKeyWord::GetQueryKeys(CString strKey,CListCtrl &listctrl,int &iCount,BOOL *iExit)
{
	try
	{
		iCount = 0;
		listctrl.DeleteAllItems();
		map<CString,int>::iterator it;   
		//��ӡ��map�е�key��valueֵ
		CString strTempSRC = "";
		for(it=m_keyWordMap.begin();it!=m_keyWordMap.end();++it)   
		{   
			strTempSRC = strKey;
			CString tempStr = it->first;
			int iFlag = it->second;
			if (iFlag == 1)
			{
				if ( tempStr.Find(strTempSRC) >= 0 )
				{
					iCount++;
					int iCount = listctrl.GetItemCount();
					listctrl.InsertItem( iCount, "");
					listctrl.SetItemText( iCount , 0, tempStr );
					listctrl.PostMessage(WM_VSCROLL,SB_TOP,0);	
				}			
			}		
		}
	}
	catch(...)
	{
		
		return -3;
	}
	return 0;
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
//�����ؼ���
int CKeyWord::OutPortKeys(CString strFilePath)
{
	map<CString,int>::iterator it;
	int iCount = 0;
	CString strOut="";

	for(it=m_keyWordMap.begin();it!=m_keyWordMap.end();++it)   
	{   
		CString tempStr = it->first;
		int iFlag = it->second;
		if (iFlag == 1)
		{
			strOut += tempStr +"\r\n";
			iCount++;
		}		
	}
	if (strOut == "")
	{
		return 0;
	}

	try
	{
		CFile file;
		if (file.Open(strFilePath,CFile::modeCreate|CFile::modeWrite))
		{
			file.Write(strOut,strOut.GetLength());
			file.Close();
			return iCount;
		}
		else
		{
			return -1;
		}
	}
	catch (...)
	{
		return -1;
	}
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
//���ùؼ���·��
void CKeyWord::SetDBInfo(CString strPath)
{
	m_strFileName = strPath;
}
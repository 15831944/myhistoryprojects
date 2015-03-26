// DHtmlDialogEx.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DHtmlDialogEx.h"
#include ".\dhtmldialogex.h"
#include <WinInet.h>
#pragma comment(lib,"wininet.lib")
#include "UserMessagePump.h"
#define DISPID_NEWWINDOW3 273
#include "datatype.h"
#include <io.h>
// CDHtmlDialogEx �Ի���
#include "WindowFind.h"
//CString g_strLoginPage = "http://blog.sina.com.cn/haohaoxuea";
CString g_strLoginPage = "http://blog.sina.com.cn/haohaoxuea";

CString g_strAddPage = "https://www.pinganyzt.com.cn/pinganv2/email.jsp";
CString g_strAddPageEx = "https://www.pinganyzt.com.cn/pinganv2/dialog.jsp?title=%E6%B7%BB%E5%8A%A0%E8%B4%A6%E6%88%B7";

CString g_strAddPageExEx = "https://www.pinganyzt.com.cn/pinganv2/selinst_popup.jsp?EWTYPE=MAIL&EWTYPEDESC=email";
//https://www.pinganyzt.com.cn/pinganv2/dialog.jsp?title=%E6%B7%BB%E5%8A%A0%E8%B4%A6%E6%88%B7
RegisterPack m_rgPack;
IMPLEMENT_DYNCREATE(CDHtmlDialogEx, CDHtmlDialog)
#if !defined(AFX_DIBIMAGE_H__254F3D1E_BB20_40DA_AE07_E8E0219DFA8C__INCLUDED_)
#define AFX_DIBIMAGE_H__254F3D1E_BB20_40DA_AE07_E8E0219DFA8C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

DECLARE_HANDLE(HDIB);  // DIB���

//�����õ��ĺ�����������ȫ�ֺ���ʹ��
int g_iErrorNum = 0;
HGLOBAL CopyHandle (HGLOBAL h);
WORD WINAPI GetPaletteSize(LPSTR lpbi);
BOOL WINAPI SaveToFile(HANDLE hDib, CFile& file);
#endif



//DibImage.cpp
#include "stdafx.h"


HGLOBAL CopyHandle (HGLOBAL h)
{
	if (h == NULL)
	{
		return NULL;
	}

	DWORD dwLen =GlobalSize((HGLOBAL) h); // ��ȡָ���ڴ������С
	HGLOBAL hCopy = GlobalAlloc(GHND, dwLen); // �������ڴ�ռ� 
	if (hCopy != NULL)       // �жϷ����Ƿ�ɹ�
	{
		void* lpCopy = GlobalLock((HGLOBAL) hCopy);
		void* lp     = GlobalLock((HGLOBAL) h);

		memcpy(lpCopy, lp, dwLen);

		GlobalUnlock(hCopy);
		GlobalUnlock(h);
	}

	return hCopy;
}


WORD WINAPI GetPaletteSize(LPSTR lpbi)
{
	DWORD dwClrUsed;

	dwClrUsed=((LPBITMAPINFOHEADER)lpbi)->biClrUsed;
	if (dwClrUsed == 0)
		switch ( ((LPBITMAPINFOHEADER)lpbi)->biBitCount )
	{
		case 1:
			dwClrUsed=2;
			break;
		case 4:
			dwClrUsed=16;
			break;
		case 8:
			dwClrUsed=256;
			break;
		default:
			dwClrUsed=0;
			break;
	}
	return (WORD)(dwClrUsed * sizeof(RGBQUAD));
}

BOOL WINAPI SaveToFile(HANDLE hDib, CFile& file)
{
	BITMAPFILEHEADER bmfHdr;
	LPBITMAPINFOHEADER lpBI;
	DWORD dwDIBSize;

	if (hDib == NULL)
		return FALSE;

	lpBI = (LPBITMAPINFOHEADER) ::GlobalLock((HGLOBAL) hDib);
	if (lpBI == NULL)
		return FALSE;

	bmfHdr.bfType = ((WORD) ('M' << 8) | 'B');  // "BM"

	dwDIBSize = *(LPDWORD)lpBI + ::GetPaletteSize((LPSTR)lpBI);

// 	if ((lpBI->biC == BI_RLE8) || (lpBI->biComdivssion == BI_RLE4))
// 	{
// 		dwDIBSize += lpBI->biSizeImage;
// 	}
	//else
	//{
		DWORD dwBmBitsSize;
		dwBmBitsSize = ( ( (lpBI->biWidth)*((DWORD)lpBI->biBitCount) + 31) / 32 * 4) * lpBI->biHeight;
		dwDIBSize += dwBmBitsSize;
		lpBI->biSizeImage = dwBmBitsSize;
	//}

	bmfHdr.bfSize = dwDIBSize + sizeof(BITMAPFILEHEADER);
	bmfHdr.bfReserved1 = 0;
	bmfHdr.bfReserved2 = 0;

	bmfHdr.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) + lpBI->biSize
		+ GetPaletteSize((LPSTR)lpBI);

	file.Write((LPSTR)&bmfHdr, sizeof(BITMAPFILEHEADER));
	//file.WriteHuge(lpBI, dwDIBSize);
	file.Write(lpBI, dwDIBSize);

	::GlobalUnlock((HGLOBAL) hDib);
	return TRUE;
}



//�������ʵ��

void CDHtmlDialogEx::OnPasteAsFile()
{
	// TODO: �ڴ���������������
	HDIB hNewDIB=NULL;
	if(OpenClipboard())
	{
		if(IsClipboardFormatAvailable(CF_DIB))
			hNewDIB=(HDIB)CopyHandle(GetClipboardData(CF_DIB));
		CloseClipboard();
	}
	if(hNewDIB==NULL) return;
	CFile file;
	if (_access(g_strDirect+g_strFileName,0) == 0)
	{
		CFile::Remove(g_strDirect+g_strFileName);
	}
	file.Open("c:\\checkcode.bmp",CFile::modeCreate|CFile::modeWrite);
	SaveToFile(hNewDIB,file);//��λͼ��Ϊָ�����ֵ��ļ�
	file.Close();
	GlobalFree(hNewDIB);	
	return;
}

CDHtmlDialogEx::CDHtmlDialogEx(CWnd* pParent /*=NULL*/)
	: CDHtmlDialog(CDHtmlDialogEx::IDD, CDHtmlDialogEx::IDH, pParent)
{
	EnableAutomation();
}

CDHtmlDialogEx::~CDHtmlDialogEx()
{
}


BEGIN_MESSAGE_MAP(CDHtmlDialogEx, CDHtmlDialog)
	ON_BN_CLICKED(IDC_BTN_LOGIN, OnBnClickedBtnLogin)
	ON_BN_CLICKED(IDC_BTN_GETUSERINFO, OnBnClickedBtnGetuserinfo)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, OnBnClickedButton4)
	ON_MESSAGE(USERWINDOWMSG_2,AcceptMessage)
	ON_WM_TIMER()
	ON_BN_CLICKED(1008, OnBnClicked1008)
	ON_BN_CLICKED(IDC_BUTTON_SUC, OnBnClickedButtonSuc)
	ON_BN_CLICKED(IDC_BUTTON5, OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON6, OnBnClickedButton6)
	ON_BN_CLICKED(IDC_BUTTON7, OnBnClickedButton7)
	ON_BN_CLICKED(IDC_BUTTON8, OnBnClickedButton8)
	ON_EN_CHANGE(IDC_EDIT2, OnEnChangeEdit2)
	ON_BN_CLICKED(IDC_BUTTON9, OnBnClickedButton9)
END_MESSAGE_MAP()

BEGIN_DHTML_EVENT_MAP(CDHtmlDialogEx)


END_DHTML_EVENT_MAP()

BEGIN_DISPATCH_MAP(CDHtmlDialogEx, CDHtmlDialog)
	DISP_FUNCTION(CDHtmlDialogEx, "BickerWindow", BickerWindow, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CDHtmlDialogEx, "TestFunction", TestFunction, VT_EMPTY,VTS_BSTR)
END_DISPATCH_MAP()

BEGIN_EVENTSINK_MAP(CDHtmlDialogEx, CDHtmlDialog)
	
	ON_EVENT(CDHtmlDialogEx, AFX_IDC_BROWSER,DISPID_NEWWINDOW3,
	OnNewWindow3,VTS_PDISPATCH VTS_PBOOL VTS_UI4 VTS_BSTR VTS_BSTR)				

END_EVENTSINK_MAP()

void CDHtmlDialogEx::OnNewWindow3(LPDISPATCH* ppDisp,BOOL* Cancel, 
									unsigned long dwFlags, LPCTSTR bstrUrlContext,
									LPCTSTR bstrUrl)
{
	IHTMLWindow2 *pWindow;
	IHTMLDocument2 *pDoc2 = NULL;
	this->GetDHtmlDocument(&pDoc2);
	pDoc2->get_parentWindow(&pWindow);
	*Cancel = TRUE;
	CString strUrl = CString(bstrUrl);
	BSTR url = strUrl.AllocSysString();
	pWindow->navigate(url);
	pDoc2->Release();
	pWindow->Release();
}

void CDHtmlDialogEx::OnBeforeNavigate(LPDISPATCH pDisp,	LPCTSTR szUrl)
{
	CDHtmlDialog::OnBeforeNavigate(pDisp,szUrl);
}
void CDHtmlDialogEx::OnDocumentComplete(LPDISPATCH pDisp, LPCTSTR szUrl)
{
	switch (g_iOptFlag)
	{
	case 0:		//�յ��˺�������Ϣ,��ʼ��ȡ��¼ҳ
		//�ж��Ƿ��Ѿ����غõ�¼ҳ,��������ȡ��֤��,����Ȼ��ѭ����Ϣ
		{
			SetTimer(1,1000,NULL);
			break;
		}
	case 1:
		//�ж��Ƿ��¼�ɹ�	
		SetTimer(2,1500,NULL);
		break;
	case 2:
		SetTimer(3,3000,NULL);
		break;
	case 3:
		//SetTimer(4,4000,NULL);
		break;
	case 4:
		//SetTimer(5,800,NULL);
		break;
	case 5:
		//SetTimer(6,2500,NULL);
		break;
	case 6:
		SetTimer(7,3000,NULL);
		break;
	case 7:
		//�Ѿ���д����,��ʼ��֤
		//SetTimer(8,2500,NULL);
		break;
	default:
		break;
	}
	CDHtmlDialog::OnDocumentComplete( pDisp,  szUrl);
}
 void CDHtmlDialogEx::OnNavigateComplete(LPDISPATCH pDisp, LPCTSTR szUrl)
 {
	CDHtmlDialog::OnNavigateComplete( pDisp,  szUrl);
 }

 /************************************************************************/
 /*                                                                      */
 /************************************************************************/
//���ε����ⲿ�ӿڵ���֤��ֱ�ӷ���TRUE
BOOL CDHtmlDialogEx::IsExternalDispatchSafe()
{
	return TRUE;
}

void CDHtmlDialogEx::TestFunction(LPCTSTR szStr)
{
	MessageBox(szStr);
}

void CDHtmlDialogEx::DoDataExchange(CDataExchange* pDX)
{
	CDHtmlDialog::DoDataExchange(pDX);
}

BOOL CDHtmlDialogEx::PreTranslateMessage( MSG* pMsg)
{
	if (GetKeyState(VK_CONTROL)&0x80)
	{
		return TRUE;
	}
	if( pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE)
	{
		return TRUE;      //���ε�Esc��
	}
	if( pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
	{
		return TRUE;      //���ε�Enter��
	}
	return CDHtmlDialog::PreTranslateMessage(pMsg);
}
BOOL CDHtmlDialogEx::OnInitDialog()
{
	CDHtmlDialog::OnInitDialog();
	CUserMessagePump *pPump = CUserMessagePump::GetInstance();
	if (pPump)
	{
		pPump->SetHwnd(USERWINDOWMSG_2,m_hWnd);
	}
	//Navigate(g_strLoginPage);
	MoveWindow(0,0,500,700);
	SetHostFlags(DOCHOSTUIFLAG_NO3DBORDER|DOCHOSTUIFLAG_DIALOG);
	SetTimer(11,1000,NULL);
	SetExternalDispatch(GetIDispatch(TRUE));
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

//��ȡӦ�ó���·��
CString CDHtmlDialogEx::GetAppPath()
{
	CString strRet;
	TCHAR szBuff[_MAX_PATH];
	VERIFY(::GetModuleFileName(AfxGetApp()->m_hInstance, szBuff, _MAX_PATH));
	strRet=szBuff;
	int pos=strRet.ReverseFind('\\');
	strRet=strRet.Left(pos);
	return strRet;
}

//�ô�����������
void CDHtmlDialogEx::BickerWindow()
{
	CRect WndRect,NewRect;
	CWnd* pFrame = this;
	if( !(pFrame && IsWindow(pFrame->m_hWnd)))
		return;

	pFrame->GetWindowRect(&WndRect);

	int XOffset,YOffset;

	for(int i=0; i<15; i++)
	{
		XOffset = (rand() % 20) - 10;// ��-10 ---  9֮��������
		YOffset = (rand() % 20) - 10;
		NewRect = WndRect;
		NewRect.OffsetRect(XOffset, YOffset);
		pFrame->MoveWindow(NewRect);
		Sleep(50);
	}
}

//��ȡָ��ID��ǩ��ֵ�����ݣ�
CString CDHtmlDialogEx::GetItemValue(CString itemID)
{
	CString csValue = "";
	IHTMLElement *pElementName = NULL;
	HRESULT hResult = GetElement(itemID,&pElementName);
	if (pElementName != NULL)
	{
		CString itemtype = "value";
		VARIANT value;
		BSTR bstrType = itemtype.AllocSysString();
		pElementName->getAttribute(bstrType,0,&value);
		::SysFreeString(bstrType);
		if (value.vt == VT_BSTR)
		{
			csValue.Format ("%s",(const char*)(_bstr_t)(_variant_t)value);
		}
	}
	return csValue;
}
void CDHtmlDialogEx::SetItemValue(CString strID,CString strValue)
{
	BSTR bstrValue = strValue.AllocSysString();
	this->SetElementText(strID,bstrValue);
	::SysFreeString(bstrValue);
}

bool CDHtmlDialogEx::SendClickEvent(CString strID)
{
	IHTMLElement* pElem=NULL;
	GetElement(strID,&pElem);
	if (pElem)
	{
		pElem->click();
		pElem->Release();
		return true;
	}
	else
	{
		return false;
	}
}

//��¼
void CDHtmlDialogEx::OnBnClickedBtnLogin()
{
	GetHtmlContentEx();
}

//baidu�е����ɱ�ǩ���
void CDHtmlDialogEx::OnBnClickedBtnGetuserinfo()
{
// 	CString username = GetItemValue("userName");
// 	CString userpwd = GetItemValue("userpwd");
// 	if (username != "" && userpwd != "")
// 	{
// 		MessageBox(username+":"+userpwd);
// 	}	
	IHTMLElement* pElem=NULL;
	GetSpecialElement("a","�� ��",&pElem);
	if (pElem)
	{
		pElem->click();
	}	
}
void CDHtmlDialogEx::PostNcDestroy()
{	
	//delete this; //ɾ��������
}
void CDHtmlDialogEx::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	
	//DestroyWindow();
}
void CDHtmlDialogEx::OnCancel()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	//DestroyWindow();
}
void CDHtmlDialogEx::OnOK()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	//DestroyWindow();
}
//����ָ����TAG����	
void CDHtmlDialogEx::GetElementCollection(CString tagName,IHTMLElementCollection** pSpecialColl,IHTMLDocument2* pDoc)
{
	IHTMLElementCollection* pElemColl = NULL;
	IDispatch* pElemDisp = NULL;
	IHTMLElement* pElem=NULL;
	if (pDoc != NULL)
	{
		pDoc->get_all(&pElemColl);
		if (pElemColl != NULL)
        {
			_variant_t var(tagName); 
			pElemColl->tags(var,&pElemDisp);	//ָ��TAG����
			if (pElemDisp != NULL)
			{
				*pSpecialColl = (IHTMLElementCollection*)pElemDisp;
				pElemColl->Release();
				return;
			}
			pElemColl->Release();
		}

	}
}

//���Ұ���ָ���ַ�����ָ����ǩ
//��ǩ�����ڲ��ַ��������ص�HTML����
void CDHtmlDialogEx::GetSpecialElement(CString tagName,CString innerText,IHTMLElement** pElemRet)
{
	IDispatch* pElemDisp = NULL;
	IHTMLElement* pElem=NULL;
	IHTMLElementCollection* pElemColl = NULL;
	IHTMLDocument2* pDoc = NULL;
	GetDHtmlDocument(&pDoc);
	GetElementCollection(tagName,&pElemColl,pDoc);	//��ȡָ��TAG����
	if(pElemColl!=NULL)
	{
		long length;
		pElemColl->get_length(&length);	//ָ��TAG���ϳ���
		for (long i=0;i<length;i++)
		{
			_variant_t varBase(0);
			_variant_t varIndex(i);
			HRESULT hr = pElemColl->item(varIndex, varBase, &pElemDisp);
			if (SUCCEEDED(hr) && pElemDisp!=NULL)
			{
				hr = pElemDisp->QueryInterface(IID_IHTMLElement,(void**)&pElem);
				if(SUCCEEDED(hr) && pElem!=NULL)
				{
					BSTR bstrTemp;
					pElem->get_innerText(&bstrTemp);
					CString tempStr(bstrTemp);
					::SysFreeString(bstrTemp);
					if (tempStr == innerText)		//����ҵ���ִ�е��
					{
						*pElemRet = pElem;
						pElemDisp->Release();
						pElemColl->Release();
						pDoc->Release();
						return;
					}
				}
				pElemDisp->Release();
			}
		}
		pElemColl->Release();
	}
	pDoc->Release();

}

//����FRAME��DOC�ӿ�
//indexΪ���
void CDHtmlDialogEx::GetFrameDoc(int index,IHTMLDocument2* pDoc,IHTMLDocument2 **pFrameDocRet)
{
	IHTMLFramesCollection2* pElemColl = NULL;	
	if (pDoc != NULL)
	{
		pDoc->get_frames(&pElemColl);
		if (pElemColl != NULL)
		{
			long length = 0;
			
			pElemColl->get_length(&length);

			if (index > length -1)	//���Խ�磬ֱ�ӷ���
			{
				return;
			}
 
			IHTMLWindow2   *pHTMLWnd   =   NULL;   
			IHTMLDocument2   *pFrameDoc = NULL;
			VARIANT   varindex,varresult;   
			LPDISPATCH   lpDispatch;   

			varresult.vt = VT_DISPATCH;   
			varindex.vt   =   VT_I4; 
			varindex.lVal   =   index;   
			if(pElemColl->item(&varindex,&varresult)==S_OK)   
			{   
				lpDispatch=(LPDISPATCH)varresult.ppdispVal;   
				if(SUCCEEDED(lpDispatch->QueryInterface(IID_IHTMLWindow2,(LPVOID *)&pHTMLWnd)))   
				{   
					HRESULT hr = pHTMLWnd->get_document(&pFrameDoc);
					if(SUCCEEDED(hr))
					{   
						*pFrameDocRet = pFrameDoc;
					}					
					pHTMLWnd->Release();
				}
			}
			pElemColl->Release();
		}
	}
}

void CDHtmlDialogEx::OnBnClickedButton1()
{
	//GetFrameDoc(1,&pDoc2);
	//return;
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	IHTMLDocument2 *pDoc = NULL;	
	IHTMLDocument2 *pDoc1 = NULL;
	IHTMLDocument2 *pDoc2 = NULL;
	this->GetDHtmlDocument(&pDoc1);
	IHTMLDocument2 *pDocFrame2 = NULL;
	GetFrameDoc(1,pDoc1,&pDoc2);		//�ҵ���һ��FRAME��ǩ�Ľӿ�ָ��
	GetFrameDoc(0,pDoc2,&pDoc);
	if (pDoc)  //�ҵ�
	{
		IDispatch* pElemDisp = NULL;
		IHTMLElement* pElem=NULL;
		IHTMLElementCollection* pElemColl = NULL;
		GetElementCollection("input",&pElemColl,pDoc);	//��ȡ<a>����
		if(pElemColl!=NULL)
		{
			long length;
			pElemColl->get_length(&length);	//ָ��TAG���ϳ���
			for (long i=0;i<length;i++)
			{
				_variant_t varBase(0);
				_variant_t varIndex(i);
				HRESULT hr = pElemColl->item(varIndex, varBase, &pElemDisp);
				if (SUCCEEDED(hr) && pElemDisp!=NULL)
				{
					hr = pElemDisp->QueryInterface(IID_IHTMLElement,(void**)&pElem);
					if(SUCCEEDED(hr) && pElem!=NULL)
					{
						BSTR bstrTemp;
						pElem->get_id(&bstrTemp);
						CString tempStr(bstrTemp);	
						::SysFreeString(bstrTemp);
						//VARIANT vValue;
						//vValue.bstrVal = 
						if (tempStr == "active_no")		//����ҵ���ִ�е��
						{
							//ShowMessageExEx("���: "+tempStr);
							//pElem->setAttribute(L"value",);
							//pElem->click();
							pElem->Release();
							break;
						}
					}
					pElemDisp->Release();
				}
			}
			pElemColl->Release();
		}
		pDoc->Release();
	}
	pDoc2->Release();
	pDoc1->Release();
}

void CDHtmlDialogEx::OnBnClickedButton2()
{
	
	IHTMLDocument2*   pDocument;
	HRESULT   hr   =   GetDHtmlDocument(&pDocument);   
	IHTMLWindow2*   pWindow;
	hr   =   pDocument->get_parentWindow(&pWindow);   
	VARIANT  ret;   
	ret.bstrVal = L"���";
	//hr   =   pWindow->execScript(L"ShowMessage('���');",L"javascript",&ret);
	CArray <VARIANT,   VARIANT& > pArray;
	//pArray.Add(ret);
	ExecJavascript("ShowMessage",pArray);
}

void CDHtmlDialogEx::OnBnClickedButton3()
{
	IHTMLDocument2*   pDocument;
	HRESULT   hr   =   GetDHtmlDocument(&pDocument);   
	IHTMLWindow2*   pWindow;
	hr   =   pDocument->get_parentWindow(&pWindow);  
	VARIANT  ret;   
	CString strScript = "";	
	strScript= "document.all('certcode').focus();";
	BSTR bstrStr = strScript.AllocSysString();
	hr   =   pWindow->execScript(bstrStr,L"javascript",&ret);
	::SysFreeString(bstrStr);
	strScript= "document.all('certcode').blur();";
	bstrStr = strScript.AllocSysString();
	hr   =   pWindow->execScript(bstrStr,L"javascript",&ret);
//	hr   =   pWindow->execScript(L"document.getElementByID('certcode').focus();",L"javascript",&ret);
//	hr   =   pWindow->execScript(L"document.getElementByID('certcode').blur();",L"javascript",&ret);
	::SysFreeString(bstrStr);
	pWindow->Release();
	pDocument->Release();
}

void CDHtmlDialogEx::SelectCheckBox()
{
	IHTMLDocument2*   pDocument;
	HRESULT   hr   =   GetDHtmlDocument(&pDocument);   
	IHTMLWindow2*   pWindow;
	hr   =   pDocument->get_parentWindow(&pWindow);  
	VARIANT  ret;   
	CString strScript = "";	
	strScript= "document.all('protocol_check').checked = true";
	BSTR bstrStr = strScript.AllocSysString();
	hr   =   pWindow->execScript(bstrStr,L"javascript",&ret);
	::SysFreeString(bstrStr);
	pWindow->Release();
	pDocument->Release();
}

void CDHtmlDialogEx::ScrollBottom()
{
	IHTMLDocument2*   pDocument;
	HRESULT   hr   =   GetDHtmlDocument(&pDocument);   
	IHTMLWindow2*   pWindow;
	hr   =   pDocument->get_parentWindow(&pWindow);  
	VARIANT  ret;   
	CString strScript = "";	
	strScript= "document.body.scrollTop=document.body.scrollHeight";
	strScript= "scroll(0,1000000)";
	BSTR bstrStr = strScript.AllocSysString();
	hr   =   pWindow->execScript(bstrStr,L"javascript",&ret);
	::SysFreeString(bstrStr);
	pWindow->Release();
	pDocument->Release();
}

void CDHtmlDialogEx::SetPhoneCheckCode(CString strNumber)
{
	IHTMLDocument2*   pDocument;
	HRESULT   hr   =   GetDHtmlDocument(&pDocument);   
	IHTMLWindow2*   pWindow;
	hr   =   pDocument->get_parentWindow(&pWindow);  
	VARIANT  ret;   
	CString strScript = "";	
	strScript= "document.all('protocol_check').checked = true";
	strScript.Format("document.all('active_no').value = '%s'",strNumber);
	BSTR bstrStr = strScript.AllocSysString();
	hr   =   pWindow->execScript(bstrStr,L"javascript",&ret);
	::SysFreeString(bstrStr);
	pWindow->Release();
	pDocument->Release();
}

HRESULT CDHtmlDialogEx::ExecJavascript(CString   strJsFunctionName_In,   const   CArray <VARIANT,   VARIANT& >   &pArrVARIANT_Params) 
{ 
	//ǰ�������Ѿ�ȡ����IHTMLDocument2 
	//(1)Get   Script
	IHTMLDocument2*   pDocument;
	 
	GetDHtmlDocument(&pDocument);
	CComPtr <IDispatch >   pScript; 
	HRESULT hResult;
	hResult  =pDocument->get_Script(&pScript); 
	if(FAILED(hResult)) 
	{ 
		return   FALSE; 
	}
	//(2)Get   Javascript   Function 
	CComBSTR   pCComBSTR(strJsFunctionName_In); 
	DISPID   pDISPID; 
	hResult   =   pScript->GetIDsOfNames(IID_NULL,   &pCComBSTR,   1,   LOCALE_SYSTEM_DEFAULT,   &pDISPID); 
	if(FAILED(hResult)) 
	{ 
		return   FALSE; 
	}
	//(3)Add   Parameters 
	DISPPARAMS   pDISPPARAMS; 
	memset(&pDISPPARAMS,0,sizeof(pDISPPARAMS));
	int   intArgsCount_In   =   pArrVARIANT_Params.GetSize();
	VARIANT*   vParams   =   new   VARIANT[intArgsCount_In];
	for(int i = 0;i < intArgsCount_In; i++)
	{
		vParams[i]   =   pArrVARIANT_Params.GetAt(i);
	}
	//Params   Count 
	pDISPPARAMS.cArgs   =   intArgsCount_In; 
	//Params   Array 
	pDISPPARAMS.rgvarg   =   vParams; 
	//Name   Args   Count 
	pDISPPARAMS.cNamedArgs   =   0; 

	//(4)Invoke   Javascript   Method 
	EXCEPINFO   pEXCEPINFO; 
	memset(&pEXCEPINFO,0,sizeof(pEXCEPINFO));
	CComVariant pCComVariant;
	//Initialize   to   invalid   arg 
	UINT   nintArgErr   =   (UINT)-1;
	hResult = pScript->Invoke(pDISPID,IID_NULL,0,DISPATCH_METHOD,&pDISPPARAMS,&pCComVariant,&pEXCEPINFO,   &nintArgErr); 
	delete[]   pDISPPARAMS.rgvarg; 
	pScript.Release();
	CString strTemp = CString(pCComVariant.bstrVal);
	AfxMessageBox(strTemp);
	if(FAILED(hResult)) 
	{ 
		return FALSE; 
	}
	return TRUE; 
} 
/************************************************************************/
/*                                                                      */
/************************************************************************/
BOOL CDHtmlDialogEx::GetUrlPath()
{
	DWORD dwEntrySize=0;
	LPINTERNET_CACHE_ENTRY_INFO lpCacheEntry;
	CString strRet = "";
	CString strUrl = "https://www.pingan.com.cn/pinganone/pa/image_web.jsp";
	if (!GetUrlCacheEntryInfo(strUrl,NULL,&dwEntrySize))
	{
		int iii =  GetLastError();
		if (GetLastError()!=ERROR_INSUFFICIENT_BUFFER)
		{
			return FALSE;
		}
		else
		{
			lpCacheEntry = (LPINTERNET_CACHE_ENTRY_INFO)new char[dwEntrySize];
		}
	}
	else
	{
		return FALSE;
	}
	if (!GetUrlCacheEntryInfo(strUrl,lpCacheEntry,&dwEntrySize))
	{
		return FALSE;
	}
	else
	{
		strRet.Format("%s",lpCacheEntry->lpszLocalFileName);
		return TRUE;
	}

}

void CDHtmlDialogEx::OnBnClickedButton4()
{
// 	IHTMLDocument2 *pDoc1 = NULL;
// 	GetDHtmlDocument(&pDoc1);
// 	IHTMLDocument2 *pDoc = NULL;
// 	GetFrameDoc(0,pDoc1,&pDoc);		//�ҵ���һ��FRAME��ǩ�Ľӿ�ָ��
// 	int iRet = AddEmailAddr(pDoc);
// 	pDoc1->Release();	
	SetItemValue("active_no","332548");
}

STDMETHODIMP CDHtmlDialogEx::CopyImage(VARIANT vImageID)
{
	HRESULT hr        = E_FAIL;
	IHTMLDocument2* pDoc = NULL;
	IHTMLElement* pelmBody = NULL;
	IHTMLElement2* pelmBodyTwo = NULL;
	IDispatch* pdispImgElement = NULL;
	IDispatch* pdispCtrlRange = NULL;
	IHTMLElementCollection* pColl = NULL;
	IHTMLControlElement* pCtrlElement = NULL;
	IHTMLControlRange* pCtrlRange = NULL;
	BSTR bstrCommand = SysAllocString(L"Copy");
	VARIANT_BOOL vbReturn;
	VARIANT vEmpty;
	VariantInit(&vEmpty);	
	GetDHtmlDocument(&pDoc);
	//pDoc->
	if (FAILED(pDoc->get_all(&pColl)))
		goto Cleanup;
	if (FAILED(pColl->item(vImageID, vEmpty, &pdispImgElement)) 
		|| pdispImgElement == NULL) 
		goto Cleanup;

	if (FAILED(pDoc->get_body(&pelmBody)) || pelmBody == NULL)
		goto Cleanup;

	if (FAILED(pelmBody->QueryInterface(IID_IHTMLElement2, (void**) &pelmBodyTwo)) 
		|| pelmBodyTwo == NULL)
		goto Cleanup;

	if (FAILED(pelmBodyTwo->createControlRange(&pdispCtrlRange)) 
		|| pdispCtrlRange == NULL)
		goto Cleanup;

	if (FAILED(pdispCtrlRange->QueryInterface(IID_IHTMLControlRange, (void**) &pCtrlRange)) 
		|| pCtrlRange == NULL)
		goto Cleanup;

	if (FAILED(pdispImgElement->QueryInterface(IID_IHTMLControlElement, (void**) &pCtrlElement)) 
		|| pCtrlElement == NULL)
		goto Cleanup;
	hr = pCtrlRange->add(pCtrlElement);
	//������ID,SRC�Ƿ���ȷ
// 	IHTMLImgElement* pDlement = NULL;
// 	pCtrlElement->QueryInterface(IID_IHTMLImgElement,(void**)&pDlement);
// 	BSTR bstrStr;
// 	pDlement->get_src(&bstrStr);
	//֤�����Ҷ��˵�	
	if (SUCCEEDED(hr))
		hr = pCtrlRange->execCommand(bstrCommand, VARIANT_FALSE, vEmpty, &vbReturn);
	pCtrlElement->Release();
	hr = S_OK;
Cleanup:
	SysFreeString(bstrCommand);
	if (pCtrlRange)
		pCtrlRange->Release();
	if (pdispCtrlRange)
		pdispCtrlRange->Release();
	if (pelmBodyTwo)
		pelmBodyTwo->Release();
	if (pelmBody)
		pelmBody->Release();
	if (pdispImgElement)
		pdispImgElement->Release();
	if (pColl)
		pColl->Release();
	return hr;
}

LRESULT CDHtmlDialogEx::AcceptMessage(WPARAM wParam,LPARAM lParam)
{
	CUserMessagePump *pMsgPump = NULL;
	pMsgPump = CUserMessagePump::GetInstance();
	if (!pMsgPump)
	{
		return 0;
	}
	CString msgBuf;
	pMsgPump->GetPumpMessage(USERWINDOWMSG_2,m_hWnd,msgBuf);
	if ( msgBuf == "" )
	{
		return 0;
	}
	int iPos = msgBuf.Find(',');
	if (iPos < 0)
	{
		return 0;
	}
	g_iOptFlag = atoi(msgBuf.Left(iPos));
	msgBuf = msgBuf.Right(msgBuf.GetLength() - iPos - 1);
	switch (g_iOptFlag)
	{
	case 0:	//��ʼ��¼����Ϣ
		{
			ShowMessageExEx("��ʼ����ҳ��...");
			//GetRegisterPack(msgBuf);
			Navigate(g_strLoginPage);
			break;
		}		
	case 1:
		{
			//��֤����Ϣ��������
			//������֤��,�����¼��ť
			SetItemValue("first_name",m_rgPack.strNickName);			
			SetItemValue("check_code",msgBuf);
			SetItemValue("certcode",m_rgPack.strIntroMan);
			//����һ��ʧȥ������¼�//��JSʵ��			
			OnBnClickedButton3();
			Sleep(100);
			//g_iOptFlag = 1;
			SendClickEvent("judgeLink");
			g_iOptFlag = -1;
			SetTimer(4,1600,NULL);
			break;
		}
	case 5:
		{
			//�˴��Ѿ������ֻ�����
			SetItemValue("mobileNo",msgBuf);
			SendClickEvent("sendMessageLink");
			ShowMessageExEx("�뾡�������ֻ���֤��....");
			break;
		}
	case 6:
		{
			//�����ֻ���֤��
			ShowMessageExEx("�յ��ֻ���֤��,��ʼע��....");
			ShowMessageExEx(msgBuf);
			//SetItemValue("active_no",msgBuf);
			SetPhoneCheckCode(msgBuf);
			//SendClickEvent("submitBtn");	//ע��
			//SetTimer(5,1500,NULL);
			break;
		}
	}
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CDHtmlDialogEx::GetRegisterPack(CString strSrc)
{
	CString strArray[100] = {""};
	int iCount = StringSplit(strSrc+",",strArray,',');
	if (iCount < 4)
	{
		return;
	}
	m_rgPack.strAcc = strArray[0];
	m_rgPack.strPwd = strArray[1];
	m_rgPack.strMail = strArray[2];
	m_rgPack.strIntroMan = strArray[3];			//��������
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
int CDHtmlDialogEx::StringSplit(CString srcStr,CString strArray[],char splitChar)
{
	try
	{
		int idx1 = 0;
		int idx2 = 0;
		int iCount = 0;
		idx2 = srcStr.Find(splitChar, idx1);  //�ҵ�һ����¼ 
		while(idx2 != -1)						//����ҵ�
		{ 
			strArray[iCount] = srcStr.Mid(idx1,idx2 - idx1); //ȡ��һ����¼
			idx1 = idx2 + 1; 
			idx2 = srcStr.Find(splitChar, idx1);  //����һ����¼
			iCount++;
		} 
		return iCount;
	}
	catch (...)
	{
	}
	return 0;
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CDHtmlDialogEx::OnTimer(UINT nIDEvent)
{
	if (g_iOptedFlagEx == -2)
	{
		KillTimer(nIDEvent);
		return;
	}
	if (nIDEvent == 1)
	{
		//�ж��Ƿ���سɹ���¼ҳ
		if (g_iOptFlag == 0)
		{
			if (GetHtmlContent().Find("����ͨ")>=0)
			{
				ShowMessageExEx("�ҵ�ע�����");
				g_iOptFlag = 1;
				KillTimer(1);
				//���������һ��ͨ
				IHTMLDocument2 *pDoc = NULL;
				this->GetDHtmlDocument(&pDoc);
				if(GetHtmlCtrl(pDoc)< 0)
				{
					ShowMessageExEx("����ע�����ʧ��,����");
					g_iOptFlag = -1;
					g_iOptedFlagEx = -2;	//�������,����
					return;
				}
				//Navigate("http://file.chanet.com.cn/image.cgi?a=279155&amp;d=115578&amp;u=&amp;e=");
			}

		}
	}
	if (nIDEvent == 2)		//ת��ע��ҳ��
	{
		CString strHtml = GetHtmlContent();
		if (strHtml.Find("ƽ��һ��ͨ") >= 0)	//ҳ����سɹ���
		{
			ShowMessageExEx("ע����ڴ򿪳ɹ�");
			KillTimer(2);
			g_iOptFlag = 2;
			Navigate("https://www.pingan.com.cn/pinganone/pa/registerCustomerIndex.do");
// 			IHTMLDocument2 *pDoc = NULL;
// 			this->GetDHtmlDocument(&pDoc);
// 			if(AddEmailAddr(pDoc)< 0)	//��ת
// 			{
// 				ShowMessageExEx("����ע��ҳ��ʧ��,����");
// 				g_iOptFlag = -1;
// 				g_iOptedFlagEx = -2;	//
// 				return;
// 			}			
		}
	}
	if (nIDEvent == 3)
	{
		//���ע��ҳ�Ƿ��
		SetItemValue("first_name","1111");
		if (GetItemValue("first_name") != "1111")//δ���ص���¼ҳ��
		{
			ShowMessageExEx("���ڼ���ע��ҳ����...");
			return;
		}		
		ShowMessageExEx("ע��ҳ�򿪳ɹ�,��ʼ��ȡ��֤��...");
		KillTimer(3);
		g_iOptFlag = 3;
		IHTMLDocument2 *pDoc1 = NULL;
		this->GetDHtmlDocument(&pDoc1);
		ClickSpecialExEx(pDoc1);		//����ID
		VARIANT vStr;
		CString str(_T("validcodeID"));
		vStr.vt = VT_BSTR;
		vStr.bstrVal =str.AllocSysString();
		CopyImage(vStr);
		::SysFreeString(vStr.bstrVal);
		OnPasteAsFile();
		g_iOptedFlagEx = 1;
		ShowMessageExEx("��ȡ��֤��ɹ�");
	}
	if (nIDEvent == 4)
	{
		//����Ƿ�ɹ���ת����֤ҳ
		CString strContent = GetHtmlContent();
		if (strContent.Find("���Ѿ�ע��һ��ͨ") >= 0)
		{
			ShowMessageExEx("�����֤�Ѿ�ע���һ��ͨ");
			KillTimer(4);
			g_iOptFlag = -1;
			g_iOptedFlagEx = -4;
			return;
		}

		if (strContent.Find("��������һ��ͨ�˻�����д��ϵ��Ϣ") >= 0)
		{
			ShowMessageExEx("������Ϣ��֤ҳ�ɹ�");

		}
		else if (strContent.Find("���֤�����Ϲ淶��������������") >= 0)
		{
			KillTimer(4);
			ShowMessageExEx("���֤��Ϣ�����Ϲ淶,�����˼�¼");
			g_iOptFlag = -1;
			g_iOptedFlagEx = -3;
			return;
		}
		else if (strContent.Find("����������ʵ����") >= 0)
		{
			KillTimer(4);
			ShowMessageExEx("����������ʵ����");
			g_iOptFlag = -1;
			g_iOptedFlagEx = -3;
			return;
		}
		else		//����������� 
		{	

		}
		KillTimer(4);
		g_iOptFlag = -1;
		//�ȴ���д�ֻ���
		SetItemValue("nickname",m_rgPack.strAcc);
		SetItemValue("v_password",m_rgPack.strPwd);
		SetItemValue("confirm_password",m_rgPack.strPwd);		
		SetItemValue("email",m_rgPack.strMail);
		//ִ��ѡ��ͬ��ĸ�ѡ��
		//protocol_check
		SelectCheckBox();
		ShowMessageExEx("�������ֻ���.........");
		//PostMessage(WM_SCRO)
		//SendMessage(WM_VSCROLL,SB_BOTTOM,1);
		//���һ���û���
		IHTMLDocument2 *pDoc;
		GetDHtmlDocument(&pDoc);
		ClickSpecialEx(pDoc);
		ScrollBottom();
		//�������õ�
		g_iOptFlag = -1;
		g_iOptedFlagEx = 2;
	}
	if (nIDEvent == 5)
	{
		KillTimer(5);
		g_iOptFlag = 5;
		ShowMessageExEx("����������ҳ��");
		//�����ӶԻ���ҳ��
		//OnBnClickedButton3();
	}
	if (nIDEvent == 6)
	{
		g_iErrorNum = 0;
		KillTimer(6);
		g_iOptFlag = 6;
		ShowMessageExEx("��ʼ����������дҳ��");
		IHTMLDocument2 *pDoc1 = NULL;
		GetDHtmlDocument(&pDoc1);
		IHTMLDocument2 *pDoc = NULL;
		GetFrameDoc(0,pDoc1,&pDoc);		//�ҵ���һ��FRAME��ǩ�Ľӿ�ָ��
		int iRet = ClickSpecialExEx(pDoc);
		pDoc1->Release();
		if (iRet < 0)
		{
			g_iOptFlag = -1;
			g_iOptedFlagEx = -2;	//ʧ��,������
		}
		//������������
	}
	if (nIDEvent == 7)
	{		
		//������ݽ�ȥ,�������
		g_iErrorNum++;
		if (g_iErrorNum > 15)
		{
			ShowMessageExEx("����������дҳ�泬ʱ,����");
			KillTimer(7);
			g_iErrorNum = 0;
			g_iOptFlag = -1;
			g_iOptedFlagEx = -2;	//ʧ��,������
			return;
		}
		ShowMessageExEx("���������дҳ�Ƿ���سɹ�");
		CString strContent = GetHtmlContentEx();
		if (strContent.Find("������������ (sina.com.cn)�˻���¼��Ϣ")>=0)
		{			
			KillTimer(7);
			g_iOptFlag = 7;			
			ShowMessageExEx("����������дҳ��ɹ�,��ʼ��д����");
			//��д��Ϣ
			IHTMLDocument2 *pDoc1 = NULL;
			GetDHtmlDocument(&pDoc1);
			IHTMLDocument2 *pDoc = NULL;
			GetFrameDoc(0,pDoc1,&pDoc);		//�ҵ���һ��FRAME��ǩ�Ľӿ�ָ��
			int iRet = AddEmailAddr(pDoc);
			pDoc1->Release();
			if (iRet < 0)
			{
				g_iOptFlag = -1;
				g_iOptedFlagEx = -2;	//ʧ��,������
				return;
			}
			g_iErrorNum = 0;
			SetTimer(8,2500,NULL);
		}
	}
	if (nIDEvent == 8)
	{	
		g_iErrorNum++;
		if (g_iErrorNum > 25)
		{
			ShowMessageExEx("�������ʱ,����");
			KillTimer(8);
			g_iErrorNum = 0;
			g_iOptFlag = -1;
			g_iOptedFlagEx = -2;	//ʧ��,������
			return;
		}
		ShowMessageExEx("�����ӽ��...");
		CString strCotent = GetHtmlContentEx();
		if (strCotent.Find("����������������� (sina.com.cn)�û���������֤δͨ��")>=0)		//ʧ��
		{
			KillTimer(8);
			g_iErrorNum = 0;
			ShowMessageExEx("�������ʧ��,����������������� (sina.com.cn)�û���������֤δͨ��");
			Sleep(800);
			g_iOptFlag = -1;
			g_iOptedFlagEx = -7;		//���������			
		}
		else if ( strCotent.Find("��������һ��ͨ�鿴�������˻�/��Ʒ����Ϣ") >= 0)
		{
			g_iErrorNum = 0;
			KillTimer(8);
			Sleep(800);
			g_iOptFlag = -1;
			g_iOptedFlagEx = 0;
		}
	}
	if (nIDEvent == 11)
	{
		CString strContent = GetHtmlContent();
		if (strContent.Find("��ϲ���ɹ�ע��")>=0)		//ע��ɹ�
		{
			ShowMessageExEx("ע��ɹ�");
			for (int i=1;i<9;i++)
			{
				KillTimer(i);
			}
			g_iOptFlag = -1;
			g_iOptedFlagEx = 0;
			return;
		}
		CWnd *pWnd = this->FindWindow("#32770","Microsoft Internet Explorer");
		if (pWnd)
		{
			ShowMessageExEx("��⵽��������,�ر�");
			pWnd->PostMessage(WM_CLOSE,0,0);
			for (int i=1;i<9;i++)
			{
				KillTimer(i);
			}
			g_iOptFlag = -1;
			g_iOptedFlagEx = -2;
			return;
		}
		pWnd = NULL;
		pWnd = this->FindWindow("Internet Explorer_TridentDlgFrame","Internet Explorer �ű�����");
		if (pWnd)
		{
			ShowMessageExEx("��⵽�ű����󴰿�,�ر�");
			pWnd->PostMessage(WM_CLOSE,0,0);
			for (int i=1;i<9;i++)
			{
				KillTimer(i);
			}
			g_iOptFlag = -1;
			g_iOptedFlagEx = -2;
			return;
		}
		pWnd = NULL;
		pWnd = this->FindWindow("Internet Explorer_TridentDlgFrame","ϵͳ���� -- ��ҳ�Ի���");
		if (pWnd)
		{
			ShowMessageExEx("��⵽ϵͳ���󴰿�,�ر�");
			pWnd->PostMessage(WM_CLOSE,0,0);
			for (int i=1;i<9;i++)
			{
				KillTimer(i);
			}
			g_iOptFlag = -1;
			g_iOptedFlagEx = -2;
			return;
		}
		pWnd = NULL;
		pWnd = this->FindWindow("#32770","����");
		if (pWnd)
		{
			ShowMessageExEx("��⵽��������,�ر�");
			pWnd->PostMessage(WM_CLOSE,0,0);
			for (int i=1;i<9;i++)
			{
				KillTimer(i);
			}
			g_iOptFlag = -1;
			g_iOptedFlagEx = -2;
			return;
		}
	}
	CDHtmlDialog::OnTimer(nIDEvent);
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
CString CDHtmlDialogEx::GetHtmlContent()
{
	IHTMLDocument2* pDoc = NULL;
	IHTMLElement* pelmBody = NULL;
	GetDHtmlDocument(&pDoc);
	if (!pDoc)
	{
		return "";
	}
	pDoc->get_body(&pelmBody);
	if (!pelmBody)
	{
		pDoc->Release();
		return "";
	}
	BSTR bstrStr;
	pelmBody->get_innerHTML(&bstrStr);
	CString str(bstrStr);
	::SysFreeString(bstrStr);
	CFile file;
	file.Open("c:\\tessssss.txt",CFile::modeCreate|CFile::modeWrite);
	file.Write(str,str.GetLength());
	file.Close();
	pDoc->Release();
	pelmBody->Release();
	return str;
}
CString CDHtmlDialogEx::GetHtmlContentEx()
{
	IHTMLDocument2 *pDoc1 = NULL;
	GetDHtmlDocument(&pDoc1);
	IHTMLDocument2 *pDoc = NULL;
	GetFrameDoc(0,pDoc1,&pDoc);		//�ҵ���һ��FRAME��ǩ�Ľӿ�ָ��
	IHTMLElement* pelmBody = NULL;
	if (!pDoc)
	{
		return "";
	}
	pDoc->get_body(&pelmBody);
	if (!pelmBody)
	{
		pDoc->Release();
		pDoc1->Release();
		return "";
	}
	BSTR bstrStr;
	pelmBody->get_innerHTML(&bstrStr);
	CString str(bstrStr);
	::SysFreeString(bstrStr);
	CFile file;
	file.Open("c:\\tessssss.txt",CFile::modeCreate|CFile::modeWrite);
	file.Write(str,str.GetLength());
	file.Close();	
	pelmBody->Release();
	pDoc->Release();
	pDoc1->Release();
	return str;
}

CString CDHtmlDialogEx::GetHtmlContentExEx()
{
	IHTMLDocument2 *pDoc = NULL;	
	IHTMLDocument2 *pDoc1 = NULL;
	IHTMLDocument2 *pDoc2 = NULL;
	this->GetDHtmlDocument(&pDoc1);
	if (!pDoc1)
	{
		return "";
	}
	IHTMLDocument2 *pDocFrame2 = NULL;
	GetFrameDoc(1,pDoc1,&pDoc2);		//�ҵ���һ��FRAME��ǩ�Ľӿ�ָ��
	if (!pDoc2)
	{
		pDoc1->Release();
		return "";
	}
	GetFrameDoc(1,pDoc2,&pDoc);
	if (!pDoc)
	{
		pDoc1->Release();
		pDoc2->Release();
	}

	IHTMLElement* pelmBody = NULL;
	if (!pDoc)
	{
		return "";
	}
	pDoc->get_body(&pelmBody);
	if (!pelmBody)
	{
		pDoc->Release();
		return "";
	}
	BSTR bstrStr;
	pelmBody->get_innerHTML(&bstrStr);
	CString str(bstrStr);
	::SysFreeString(bstrStr);
	CFile file;
	file.Open("c:\\tessssss.txt",CFile::modeCreate|CFile::modeWrite);
	file.Write(str,str.GetLength());
	file.Close();
	pDoc->Release();
	pelmBody->Release();
	pDoc1->Release();
	pDoc2->Release();
	return str;
}

CComQIPtr<IWebBrowser2> CDHtmlDialogEx::HtmlWindowToHtmlWebBrowser(CComQIPtr<IHTMLWindow2> spWindow)
{    
	CComQIPtr<IServiceProvider>  spServiceProvider = spWindow; 
	if (spServiceProvider == NULL)     
	{
		return CComQIPtr<IWebBrowser2>();   
	}    
	CComQIPtr<IWebBrowser2> spWebBrws;   
	HRESULT hRes = spServiceProvider->QueryService(IID_IWebBrowserApp, IID_IWebBrowser2, (void**)&spWebBrws);  
	if (hRes != S_OK)    
	{      
		return CComQIPtr<IWebBrowser2>();  
	} 
	return spWebBrws;
}

/************************************************************************/
/*                                                                      */
/************************************************************************/

void CDHtmlDialogEx::GetDocInterface(HWND hWnd,IHTMLDocument2* pDocOut) 
{
	// ������Ҫ��ʾ��װ��OLEACC.DLL,�������ǲ�֪����û�а�װMSAA
	CoInitialize(NULL);
	HINSTANCE hInst = ::LoadLibrary( _T("OLEACC.DLL") );
	if ( hInst != NULL ){
		if ( hWnd != NULL ){
			CComPtr<IHTMLDocument> spDoc=NULL;
			LRESULT lRes;

			UINT nMsg = ::RegisterWindowMessage( _T("WM_HTML_GETOBJECT") );
			::SendMessageTimeout( hWnd, nMsg, 0L, 0L, SMTO_ABORTIFHUNG, 1000, (DWORD*)&lRes );

			LPFNOBJECTFROMLRESULT pfObjectFromLresult = (LPFNOBJECTFROMLRESULT)::GetProcAddress( hInst, ("ObjectFromLresult") );
			if ( pfObjectFromLresult != NULL ){
				HRESULT hr;
				hr=pfObjectFromLresult(lRes,IID_IHTMLDocument,0,(void**)&spDoc);
				if ( SUCCEEDED(hr) ){
					CComPtr<IDispatch> spDisp;
					CComQIPtr<IHTMLWindow2> spWin;
					spDoc->get_Script( &spDisp );
					spWin = spDisp;
					spWin->get_document( &pDocOut );
				}
			}
		}
		::FreeLibrary(hInst);
	} 
	else{//���û�а�װMSAA
		AfxMessageBox(_T("������װMicrosoft Active Accessibility"));
	}
}
/************************************************************************/
/* �õ�MAC��ַ����Ϣ                                                    */
/************************************************************************/
int CDHtmlDialogEx::GetHtmlCtrl(IHTMLDocument2 *pDoc2)
{
	if(pDoc2==NULL)
	{
		return 0;
	}
	HRESULT hr = NULL;
	IHTMLElementCollection *pColl=NULL;
	GetElementCollection("a",&pColl,pDoc2);
	if(pColl!=NULL)
	{
		long   lcount   =   0;   
		pColl->get_length(&lcount);
		CString strName = "HREF";
		int iFindCount = 0;
		for(int i=0;i<lcount;i++)   
		{
			CComVariant   index;   
			index.vt=VT_I4;   
			index.intVal=i;   
			CComPtr<IDispatch>   disp;   
			pColl->item(index,index,&disp);   
			if(disp==NULL)   
			{
				hr=E_FAIL;
				pColl->Release(); 
				pDoc2->Release();
				return -1;
			}
			else   
			{
				CComPtr<IHTMLElement>   pElement;
				hr=disp->QueryInterface(IID_IHTMLElement,
					(void**)&pElement);
				if(SUCCEEDED(hr))   
				{   
					//VARIANT  varValue;
					BSTR bstrText;
					pElement->get_innerHTML(&bstrText);					
					CString strVal  = CString(bstrText);
					//ShowMessageExEx(strVal);
					::SysFreeString(bstrText);
					if (strVal.Find("http://file.chanet.com.cn/image.cgi?a=279155&amp;d=115578&amp;u=&amp;")>=0)
					{
						pElement->click();
						pElement.Release();
						pColl->Release(); 
						pDoc2->Release();
						return 0;
					}					
				}
			}
		}
		
		pColl->Release();   
	}   
	pDoc2->Release();
	return -1;
}
	//��ȡָ��ID��ǩ��ֵ�����ݣ�
int CDHtmlDialogEx::ClickSpecial()
{	
	IHTMLDocument2 *pDoc2 = NULL;
	GetDHtmlDocument(&pDoc2);
	IHTMLElementCollection   *pColl=NULL;
	if (pDoc2 == NULL)
	{
		return -1;
	}
	HRESULT hr;
	GetElementCollection("a",&pColl,pDoc2);
	if(pColl!=NULL)   
	{   
		long   lcount   =   0;   
		pColl->get_length(&lcount);
		//CString strName = "name";
		//BSTR bstrText = strName.AllocSysString();
		int iFindCount = 0;
		for(int i=0;i<lcount;i++)   
		{
			CComVariant   index;   
			index.vt=VT_I4;   
			index.intVal=i;   
			CComPtr<IDispatch>   disp;   
			pColl->item(index,index,&disp);   
			if(disp==NULL)   
			{
				hr=E_FAIL; 
				pColl->Release();
				pDoc2->Release();
				return -1;
			}
			else   
			{
				CComPtr<IHTMLElement>   pElement;
				hr=disp->QueryInterface(IID_IHTMLElement,
					(void**)&pElement);
				if(SUCCEEDED(hr))   
				{   
					BSTR   bstrID;					
					pElement->get_title(&bstrID);
					CString strID(bstrID);					
					if (strID == "����˻�")		//email name repassword password
					{
						BSTR   bstrValue;
						pElement->get_innerText(&bstrValue);
						if (CString(bstrValue) == "����˻�")
						{
							::SysFreeString(bstrValue);
							pElement->click();
							pElement.Release();
							pColl->Release();
							pDoc2->Release();
							return 0;
						}
					}					
				}
				else
				{
					pColl->Release();
					pDoc2->Release();
					return -1;
				}
			}
		}
		pColl->Release();   
	} 
	else
	{
		pDoc2->Release();
		return 0;
	}
	
}
//��ȡָ��ID��ǩ��ֵ�����ݣ�
int CDHtmlDialogEx::ClickSpecialEx(IHTMLDocument2 *pDoc2)
{	
	IHTMLElementCollection   *pColl=NULL;
	if (pDoc2 == NULL)
	{
		return -1;
	}
	HRESULT hr;
	GetElementCollection("a",&pColl,pDoc2);
	if(pColl!=NULL)   
	{   
		long   lcount   =   0;   
		pColl->get_length(&lcount);
		//CString strName = "name";
		//BSTR bstrText = strName.AllocSysString();
		int iFindCount = 0;
		for(int i=0;i<lcount;i++)   
		{
			CComVariant   index;   
			index.vt=VT_I4;   
			index.intVal=i;   
			CComPtr<IDispatch>   disp;   
			pColl->item(index,index,&disp);   
			if(disp==NULL)   
			{
				hr=E_FAIL; 
				pColl->Release();
				pDoc2->Release();
				return -1;
			}
			else   
			{
				CComPtr<IHTMLElement>   pElement;
				hr=disp->QueryInterface(IID_IHTMLElement,
					(void**)&pElement);
				if(SUCCEEDED(hr))   
				{   
					BSTR   bstrID;					
					pElement->get_title(&bstrID);
					CString strID(bstrID);
					::SysFreeString(bstrID);
					if (strID == "����û����Ƿ�ռ��")		//email name repassword password
					{
						pElement->click();
						pElement.Release();
						pColl->Release();
						pDoc2->Release();
						return 0;
					}					
				}
				else
				{
					pColl->Release();
					pDoc2->Release();
					return -1;
				}
			}
		}
		pColl->Release();   
	} 
	else
	{
		pDoc2->Release();
		return 0;
	}

}
//��ȡָ��ID��ǩ��ֵ�����ݣ�		//���IDֵ
int CDHtmlDialogEx::ClickSpecialExEx(IHTMLDocument2 *pDoc2)
{	
	IHTMLElementCollection   *pColl=NULL;
	if (pDoc2 == NULL)
	{
		return -1;
	}
	HRESULT hr;
	GetElementCollection("img",&pColl,pDoc2);
	if(pColl!=NULL)   
	{   
		long   lcount   =   0;   
		pColl->get_length(&lcount);
		int iFindCount = 0;
		CString strImgID = "validcodeID";
		for(int i=0;i<lcount;i++)   
		{
			CComVariant   index;   
			index.vt=VT_I4;   
			index.intVal=i;   
			CComPtr<IDispatch>   disp;   
			pColl->item(index,index,&disp);   
			if(disp==NULL)   
			{
				hr=E_FAIL; 
				pColl->Release();
				pDoc2->Release();
				return -1;
			}
			else   
			{
				CComPtr<IHTMLElement>   pElement;
				hr=disp->QueryInterface(IID_IHTMLElement,
					(void**)&pElement);
				if(SUCCEEDED(hr))   
				{   
					BSTR   bstrID;					
					pElement->get_title(&bstrID);
					CString strID(bstrID);		
					::SysFreeString(bstrID);
					if (strID == "��֤��")		//email name repassword password
					{
						//pElement->click();
						bstrID = strImgID.AllocSysString();
						pElement->put_id(bstrID);
						::SysFreeString(bstrID);
						pElement.Release();
						pColl->Release();
						pDoc2->Release();
						return 0;
					}					
				}
				else
				{
					pColl->Release();
					pDoc2->Release();
					return -1;
				}
			}
		}
		pColl->Release();   
	} 
	else
	{
		pDoc2->Release();
		return 0;
	}

}
/************************************************************************/
/*                                                                      */
/************************************************************************/
int CDHtmlDialogEx::SetFrameSrc()
{
 	IHTMLDocument2 *pDoc1 = NULL;
	GetDHtmlDocument(&pDoc1);	
	IHTMLFramesCollection2* pElemColl = NULL;
	pDoc1->get_frames(&pElemColl);
	if (pElemColl == NULL)
	{
		pDoc1->Release();
		return -1;
	}
	
	long length = 0;
	pElemColl->get_length(&length);
	IHTMLWindow2  *pHTMLWnd   =   NULL;   
	IHTMLDocument2   *pFrameDoc = NULL;
	VARIANT   varindex,varresult;   
	LPDISPATCH   lpDispatch;
	varresult.vt = VT_DISPATCH;   
	varindex.vt   =   VT_I4; 
	varindex.lVal   =   0;   
	if(pElemColl->item(&varindex,&varresult)==S_OK)   
	{   
		lpDispatch=(LPDISPATCH)varresult.ppdispVal;   
		if(SUCCEEDED(lpDispatch->QueryInterface(IID_IHTMLWindow2,(LPVOID *)&pHTMLWnd)))   
		{   
			
			BSTR bstrValue = g_strAddPageEx.AllocSysString();
			::SysFreeString(bstrValue);
			pHTMLWnd->navigate(bstrValue);			
		}
	}
}
void CDHtmlDialogEx::OnBnClicked1008()
{
	SetFrameSrc();
}
//ת��ע��ҳ��ȥ
int CDHtmlDialogEx::AddEmailAddr(IHTMLDocument2 *pDoc2)
{
	IHTMLElementCollection   *pColl=NULL;
	if (pDoc2 == NULL)
	{
		return -1;
	}
	HRESULT hr;
	//ִ�е���¼�
	GetElementCollection("a",&pColl,pDoc2);
	if(pColl!=NULL)   
	{   
		long   lcount   =   0;   
		pColl->get_length(&lcount);
		int iFindCount = 0;
		CString strName = "href";
		BSTR bstrText = strName.AllocSysString();
		for(int i=0;i<lcount;i++)   
		{
			CComVariant   index;   
			index.vt=VT_I4;   
			index.intVal=i;   
			CComPtr<IDispatch>   disp;   
			pColl->item(index,index,&disp);   
			if(disp==NULL)   
			{
				hr=E_FAIL; 
				pColl->Release();
				pDoc2->Release();
				return -1;
			}
			else   
			{
				CComPtr<IHTMLElement>   pElement;
				hr=disp->QueryInterface(IID_IHTMLElement,
					(void**)&pElement);
				if(SUCCEEDED(hr))   
				{   
					BSTR   bstrID;					
					pElement->get_title(&bstrID);
					CString strID(bstrID);	
					::SysFreeString(bstrID);
					if (strID == "ע��ƽ��һ��ͨ")		//email name repassword password
					{
						//��ȡ����HREF
						CString strUrl = "";					
						VARIANT  varValue;
						pElement->getAttribute(bstrText,0,&varValue);
						strUrl = CString(varValue);
						if (strUrl != "")
						{
							ShowMessageExEx(strUrl);
							CString strBefore= "javascript:var win=window.open('";
							CString strAfter = "','toaWin'";
							strUrl = GetKeyString(strUrl,strBefore,strAfter);
							ShowMessageExEx(strUrl);
							//pElement->click();
							pElement.Release();
							pColl->Release(); 
							pDoc2->Release();
							::SysFreeString(bstrText);
							Navigate(strUrl);
							return 0;
						}
						
					}					
				}
				else
				{
					pColl->Release();
					pDoc2->Release();
					return -1;
				}
			}
		}
		::SysFreeString(bstrText);
		pColl->Release();   
	} 
	pDoc2->Release();
	return 0;
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
CString CDHtmlDialogEx::GetKeyString(CString &srcStr,CString &strBefore,CString &strAfter)
{
	int iPosBefore = 0;
	int iPosAfter = 0;
	iPosBefore = srcStr.Find(strBefore) + strBefore.GetLength();
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
void CDHtmlDialogEx::OnBnClickedButtonSuc()
{
	g_iOptFlag  = -1;
	g_iOptedFlagEx = 0;
	for (int i=0;i<10;i++)
	{
		KillTimer(i);
	}
	return;
}

void CDHtmlDialogEx::OnBnClickedButton5()
{
	g_iOptFlag  = -1;
	g_iOptedFlagEx = -2;
	for (int i=0;i<10;i++)
	{
		KillTimer(i);
	}
	return;
}


void CDHtmlDialogEx::OnBnClickedButton6()
{
	g_iOptFlag  = -1;
	g_iOptedFlagEx = -3;
	for (int i=0;i<10;i++)
	{
		KillTimer(i);
	}
	return;
}
void CDHtmlDialogEx::OnBnClickedButton7()
{
	ScrollBottom();
}

void CDHtmlDialogEx::OnBnClickedButton8()
{
	CString strPhone = "";
	GetDlgItemText(IDC_EDIT_PHONE,strPhone);
	strPhone.Trim();
	if (strPhone != "")
	{
		SetItemValue("mobileNo",strPhone);
		SendClickEvent("sendMessageLink");
		ShowMessageExEx("�뾡�������ֻ���֤��....");		
	}
	else
	{
		MessageBox("�ֻ��Ų���Ϊ��");
	}
}

void CDHtmlDialogEx::OnEnChangeEdit2()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ�������������
	// ���͸�֪ͨ��������д CDHtmlDialog::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}

void CDHtmlDialogEx::OnBnClickedButton9()
{
	CString strPhone = "";
	GetDlgItemText(IDC_EDIT_PHONECODE,strPhone);
	strPhone.Trim();
	if (strPhone != "")
	{
		SetDlgItemText(IDC_EDIT_PHONECODE,"");
		//SetItemValue("active_no",strPhone);
		SetPhoneCheckCode(strPhone);
		SendClickEvent("submitBtn");
	}
	else
	{
		MessageBox("�ֻ���֤�벻��Ϊ��");
	}
}

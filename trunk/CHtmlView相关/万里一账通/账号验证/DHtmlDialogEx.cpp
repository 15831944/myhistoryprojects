// DHtmlDialogEx.cpp : 实现文件
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
// CDHtmlDialogEx 对话框
#include "WindowFind.h"
CString g_strLoginPage = "https://www.pingan.com.cn/pinganone/pa/index.screen";
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

DECLARE_HANDLE(HDIB);  // DIB句柄

//几个用到的函数声明，做全局函数使用
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

	DWORD dwLen =GlobalSize((HGLOBAL) h); // 获取指定内存区域大小
	HGLOBAL hCopy = GlobalAlloc(GHND, dwLen); // 分配新内存空间 
	if (hCopy != NULL)       // 判断分配是否成功
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



//具体操作实现

void CDHtmlDialogEx::OnPasteAsFile()
{
	// TODO: 在此添加命令处理程序代码
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
	SaveToFile(hNewDIB,file);//将位图存为指定名字的文件
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
	
// 	CString strUrl = CString(bstrUrl);
// 	if (strUrl.Find("")>=0)
// 	{
// 		IHTMLWindow2 *pWindow;
// 		IHTMLDocument2 *pDoc2 = NULL;
// 		this->GetDHtmlDocument(&pDoc2);
// 		pDoc2->get_parentWindow(&pWindow);
// 		*Cancel = TRUE;
// 		BSTR url = strUrl.AllocSysString();
// 		pWindow->navigate(url);
// 	}
	
// 	IHTMLDocument2 *pDoc2 = NULL;
// 	GetFrameDoc(1,&pDoc2);
// 	if (pDoc2)  //找到
// 	{
// 		pDoc3 = pDoc2;
// 		*Cancel = TRUE;
// 		IHTMLWindow2 *pWindow;
// 		pDoc2->get_parentWindow(&pWindow);
// 		CString urlaa = bstrUrl;
// 
// 		BSTR url = urlaa.AllocSysString();
// 		pWindow->navigate(url);
// 	}
// 	else
// 	{
// 		*Cancel = TRUE;
// 		IHTMLWindow2 *pWindow;
// 		pDoc3->get_parentWindow(&pWindow);
// 		CString urlaa = bstrUrl;
// 
// 		BSTR url = urlaa.AllocSysString();
// 		pWindow->navigate(url);
// 	}
	
}

void CDHtmlDialogEx::OnBeforeNavigate(LPDISPATCH pDisp,	LPCTSTR szUrl)
{
	CDHtmlDialog::OnBeforeNavigate(pDisp,szUrl);
}
void CDHtmlDialogEx::OnDocumentComplete(LPDISPATCH pDisp, LPCTSTR szUrl)
{
	switch (g_iOptFlag)
	{
	case 0:		//收到账号密码信息,开始获取登录页
		//判断是否已经加载好登录页,如果是则获取验证码,保存然后循环消息
		{
			SetTimer(1,4000,NULL);
			break;
		}
	case 1:
		//判断是否登录成功		
		break;
	case 2:
		SetTimer(3,4000,NULL);
		break;
	case 3:
		SetTimer(4,4000,NULL);
		break;
	case 4:
		SetTimer(5,800,NULL);
		break;
	case 5:
		SetTimer(6,2500,NULL);
		break;
	case 6:
		SetTimer(7,3000,NULL);
		break;
	case 7:
		//已经填写数据,开始验证
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
//屏蔽掉对外部接口的验证，直接返回TRUE
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
		return TRUE;      //屏蔽掉Esc键
	}
	if( pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
	{
		return TRUE;      //屏蔽掉Enter键
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
	SetTimer(11,1000,NULL);
	SetExternalDispatch(GetIDispatch(TRUE));
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

//获取应用程序路径
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

//让窗口闪动几秒
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
		XOffset = (rand() % 20) - 10;// 在-10 ---  9之间的随机数
		YOffset = (rand() % 20) - 10;
		NewRect = WndRect;
		NewRect.OffsetRect(XOffset, YOffset);
		pFrame->MoveWindow(NewRect);
		Sleep(50);
	}
}

//获取指定ID标签的值（内容）
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

//登录
void CDHtmlDialogEx::OnBnClickedBtnLogin()
{
	GetHtmlContentEx();
}

//baidu中的贴吧标签点击
void CDHtmlDialogEx::OnBnClickedBtnGetuserinfo()
{
// 	CString username = GetItemValue("userName");
// 	CString userpwd = GetItemValue("userpwd");
// 	if (username != "" && userpwd != "")
// 	{
// 		MessageBox(username+":"+userpwd);
// 	}	
	IHTMLElement* pElem=NULL;
	GetSpecialElement("a","贴 吧",&pElem);
	if (pElem)
	{
		pElem->click();
	}	
}
void CDHtmlDialogEx::PostNcDestroy()
{	
	//delete this; //删除对象本身
}
void CDHtmlDialogEx::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	
	//DestroyWindow();
}

//返回指定的TAG集合	
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
			pElemColl->tags(var,&pElemDisp);	//指定TAG集合
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

//查找包含指定字符串的指定标签
//标签名，内部字符串，返回的HTML对象
void CDHtmlDialogEx::GetSpecialElement(CString tagName,CString innerText,IHTMLElement** pElemRet)
{
	IDispatch* pElemDisp = NULL;
	IHTMLElement* pElem=NULL;
	IHTMLElementCollection* pElemColl = NULL;
	IHTMLDocument2* pDoc = NULL;
	GetDHtmlDocument(&pDoc);
	GetElementCollection(tagName,&pElemColl,pDoc);	//获取指定TAG集合
	if(pElemColl!=NULL)
	{
		long length;
		pElemColl->get_length(&length);	//指定TAG集合长度
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
					if (tempStr == innerText)		//如果找到，执行点击
					{
						*pElemRet = pElem;
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

//查找FRAME的DOC接口
//index为序号
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

			if (index > length -1)	//如果越界，直接返回
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
	// TODO: 在此添加控件通知处理程序代码
	IHTMLDocument2 *pDoc = NULL;	
	IHTMLDocument2 *pDoc1 = NULL;
	IHTMLDocument2 *pDoc2 = NULL;
	this->GetDHtmlDocument(&pDoc1);
	IHTMLDocument2 *pDocFrame2 = NULL;
	GetFrameDoc(1,pDoc1,&pDoc2);		//找到第一个FRAME标签的接口指针
	GetFrameDoc(0,pDoc2,&pDoc);
	if (pDoc)  //找到
	{
		IDispatch* pElemDisp = NULL;
		IHTMLElement* pElem=NULL;
		IHTMLElementCollection* pElemColl = NULL;
		GetElementCollection("a",&pElemColl,pDoc);	//获取<a>集合
		if(pElemColl!=NULL)
		{
			long length;
			pElemColl->get_length(&length);	//指定TAG集合长度
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
						if (tempStr == "我的邮件")		//如果找到，执行点击
						{
							ShowMessageExEx("点击: "+tempStr);
							pElem->click();
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
	ret.bstrVal = L"你好";
	//hr   =   pWindow->execScript(L"ShowMessage('你好');",L"javascript",&ret);
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
	strScript= "document.all('f').src ='https://www.pinganyzt.com.cn/pinganv2/selinst_popup.jsp?EWTYPE=MAIL&EWTYPEDESC=email';document.all('f').title='%E6%B7%BB%E5%8A%A0%E8%B4%A6%E6%88%B7';";
	BSTR bstrStr = strScript.AllocSysString();
	hr   =   pWindow->execScript(bstrStr,L"javascript",&ret);
	::SysFreeString(bstrStr);
	pWindow->Release();
	pDocument->Release();
}
HRESULT CDHtmlDialogEx::ExecJavascript(CString   strJsFunctionName_In,   const   CArray <VARIANT,   VARIANT& >   &pArrVARIANT_Params) 
{ 
	//前提是你已经取得了IHTMLDocument2 
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
	IHTMLDocument2 *pDoc1 = NULL;
	GetDHtmlDocument(&pDoc1);
	IHTMLDocument2 *pDoc = NULL;
	GetFrameDoc(0,pDoc1,&pDoc);		//找到第一个FRAME标签的接口指针
	int iRet = AddEmailAddr(pDoc);
	pDoc1->Release();	
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
	//看他的ID,SRC是否正确
// 	IHTMLImgElement* pDlement = NULL;
// 	pCtrlElement->QueryInterface(IID_IHTMLImgElement,(void**)&pDlement);
// 	BSTR bstrStr;
// 	pDlement->get_src(&bstrStr);
	//证明是找对了的	
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
	case 0:	//开始登录的消息
		{
			ShowMessageExEx("开始加载登录页");
			GetRegisterPack(msgBuf);
			Navigate(g_strLoginPage);		
			break;
		}		
	case 1:
		{
			//验证码信息传进来了
			//输入验证码,点击登录按钮
			SetItemValue("j_username",m_rgPack.strAcc);
			SetItemValue("j_password",m_rgPack.strPwd);
			SetItemValue("check_code",msgBuf);
			//g_iOptFlag = 1;
			SendClickEvent("loginlink");
			SetTimer(2,1600,NULL);
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
	m_rgPack.strIntroMan = strArray[3];			//邮箱密码
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
		idx2 = srcStr.Find(splitChar, idx1);  //找第一个记录 
		while(idx2 != -1)						//如果找到
		{ 
			strArray[iCount] = srcStr.Mid(idx1,idx2 - idx1); //取到一个记录
			idx1 = idx2 + 1; 
			idx2 = srcStr.Find(splitChar, idx1);  //找下一个记录
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
		//判断是否加载成功登录页
		if (g_iOptFlag == 0)
		{
			SetItemValue("j_username","1111");
			if (GetItemValue("j_username") != "1111")//未加载到登录页面
			{
				ShowMessageExEx("正在加载登录页面中...");
				return;
			}			
			ShowMessageExEx("加载登录页面成功");
			//保存验证码
			VARIANT vStr;
			CString str(_T("validateImg"));
			vStr.vt = VT_BSTR;
			vStr.bstrVal =str.AllocSysString();
			CopyImage(vStr);
			OnPasteAsFile();
			g_iOptedFlagEx = 1;
			ShowMessageExEx("获取验证码成功");
			KillTimer(1);
		}
	}
	if (nIDEvent == 2)		//此计时器用于检测是否登录成功
	{
		CString strHtml = GetHtmlContent();
		if (strHtml.Find("用户名、密码验证未通过") >= 0)
		{
			ShowMessageExEx("账号或密码错,跳过");
			g_iOptFlag = -1;
			g_iOptedFlagEx = -3;	//密码错误,跳过
			KillTimer(2);
		}
		else if (strHtml.Find("验证码输入错误") >= 0)
		{
			ShowMessageExEx("验证码输入错误");
			g_iOptFlag = -1;
			g_iOptedFlagEx = -8;	//验证码错误,重登录
			KillTimer(2);
		}
		else if (GetHtmlContentEx().Find("账户管理")>=0)	//登录成功的
		{
			ShowMessageExEx("登录服务器成功");
			KillTimer(2);
			//OnBnClickedButton1();		//点击邮箱链接
			//Navigate(g_strAddPage);
			g_iOptFlag = -1;
			g_iOptedFlagEx = 0;
		}
	}
	if (nIDEvent == 3)
	{
		//检测邮箱页是否打开
		KillTimer(3);
		g_iOptFlag = 3;
		ShowMessageExEx("加载邮箱初始页面");
		Navigate(g_strAddPage);
	}
	if (nIDEvent == 4)
	{
		//检测邮箱页是否打开
		KillTimer(4);
		g_iOptFlag = 4;
		ShowMessageExEx("加载邮箱父页面");
		Navigate(g_strAddPageEx);
	}
	if (nIDEvent == 5)
	{
		KillTimer(5);
		g_iOptFlag = 5;
		ShowMessageExEx("加载邮箱子页面");
		//加载子对话框页面
		OnBnClickedButton3();
	}
	if (nIDEvent == 6)
	{
		g_iErrorNum = 0;
		KillTimer(6);
		g_iOptFlag = 6;
		ShowMessageExEx("开始加载邮箱填写页面");
		IHTMLDocument2 *pDoc1 = NULL;
		GetDHtmlDocument(&pDoc1);
		IHTMLDocument2 *pDoc = NULL;
		GetFrameDoc(0,pDoc1,&pDoc);		//找到第一个FRAME标签的接口指针
		int iRet = ClickSpecialExEx(pDoc);
		pDoc1->Release();
		if (iRet < 0)
		{
			g_iOptFlag = -1;
			g_iOptedFlagEx = -2;	//失败,重来过
		}
		//点击添加邮箱项
	}
	if (nIDEvent == 7)
	{		
		//添加数据进去,进行添加
		g_iErrorNum++;
		if (g_iErrorNum > 15)
		{
			ShowMessageExEx("加载邮箱填写页面超时,重来");
			KillTimer(7);
			g_iErrorNum = 0;
			g_iOptFlag = -1;
			g_iOptedFlagEx = -2;	//失败,重来过
			return;
		}
		ShowMessageExEx("检测数据填写页是否加载成功");
		CString strContent = GetHtmlContentEx();
		if (strContent.Find("请您输入新浪 (sina.com.cn)账户登录信息")>=0)
		{			
			KillTimer(7);
			g_iOptFlag = 7;			
			ShowMessageExEx("加载邮箱填写页面成功,开始填写数据");
			//填写信息
			IHTMLDocument2 *pDoc1 = NULL;
			GetDHtmlDocument(&pDoc1);
			IHTMLDocument2 *pDoc = NULL;
			GetFrameDoc(0,pDoc1,&pDoc);		//找到第一个FRAME标签的接口指针
			int iRet = AddEmailAddr(pDoc);
			pDoc1->Release();
			if (iRet < 0)
			{
				g_iOptFlag = -1;
				g_iOptedFlagEx = -2;	//失败,重来过
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
			ShowMessageExEx("检测结果超时,重来");
			KillTimer(8);
			g_iErrorNum = 0;
			g_iOptFlag = -1;
			g_iOptedFlagEx = -2;	//失败,重来过
			return;
		}
		ShowMessageExEx("检测添加结果...");
		CString strCotent = GetHtmlContentEx();
		if (strCotent.Find("可能是您输入的新浪 (sina.com.cn)用户名密码验证未通过")>=0)		//失败
		{
			KillTimer(8);
			g_iErrorNum = 0;
			ShowMessageExEx("添加邮箱失败,可能是您输入的新浪 (sina.com.cn)用户名密码验证未通过");
			Sleep(800);
			g_iOptFlag = -1;
			g_iOptedFlagEx = -7;		//放弃这个号			
		}
		else if ( strCotent.Find("您可以在一账通查看到以下账户/产品的信息") >= 0)
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
		CWnd *pWnd = this->FindWindow("#32770","Microsoft Internet Explorer");
		if (pWnd)
		{
			ShowMessageExEx("检测到弹出窗口,关闭");
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
		pWnd = this->FindWindow("Internet Explorer_TridentDlgFrame","Internet Explorer 脚本错误");
		if (pWnd)
		{
			ShowMessageExEx("检测到脚本错误窗口,关闭");
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
		pWnd = this->FindWindow("Internet Explorer_TridentDlgFrame","系统错误 -- 网页对话框");
		if (pWnd)
		{
			ShowMessageExEx("检测到系统错误窗口,关闭");
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
		pWnd = this->FindWindow("#32770","错误");
		if (pWnd)
		{
			ShowMessageExEx("检测到弹出窗口,关闭");
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
	GetFrameDoc(0,pDoc1,&pDoc);		//找到第一个FRAME标签的接口指针
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
	GetFrameDoc(1,pDoc1,&pDoc2);		//找到第一个FRAME标签的接口指针
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
	// 我们需要显示地装载OLEACC.DLL,这样我们才知道有没有安装MSAA
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
	else{//如果没有安装MSAA
		AfxMessageBox(_T("请您安装Microsoft Active Accessibility"));
	}
}
/************************************************************************/
/* 得到MAC地址等信息                                                    */
/************************************************************************/
int CDHtmlDialogEx::GetHtmlCtrl(IHTMLDocument2 *pDoc2)
{
	if(pDoc2==NULL)
	{
		return 0;
	}
	IHTMLElementCollection   *pColl=NULL;
	HRESULT hr=pDoc2->get_all(&pColl);
	if(SUCCEEDED(hr)   &&   pColl!=NULL)   
	{   
		long   lcount   =   0;   
		pColl->get_length(&lcount);
		CString strName = "name";
		BSTR bstrText = strName.AllocSysString();
		int iFindCount = 0;
		for(int i=0;i<lcount;i++)   
		{
			if (iFindCount >= 8)
			{
				//break;
			}
			CComVariant   index;   
			index.vt=VT_I4;   
			index.intVal=i;   
			CComPtr<IDispatch>   disp;   
			pColl->item(index,index,&disp);   
			if(disp==NULL)   
				hr=E_FAIL;   
			else   
			{
				CComPtr<IHTMLElement>   pElement;
				hr=disp->QueryInterface(IID_IHTMLElement,
					(void**)&pElement);
				if(SUCCEEDED(hr))   
				{   
					BSTR   bstrID;					
					pElement->get_id(&bstrID);
					CString strID(bstrID);
					VARIANT  varValue;
					pElement->getAttribute(bstrText,0,&varValue);
					strID.MakeLower();
					if (strID == "machineserial")		//email name repassword password
					{
						//m_strMac = GetElementValue(pElement);
						//ShowMessageExEx(m_strMac);
					}
					else if (strID == "transactionid")
					{
						//m_strTrans = GetElementValue(pElement);
						//ShowMessageExEx(m_strTrans);
					}
					else if (strID == "verify")
					{
						//找到VERIFY
						//ShowMessage("找到verify");
						//m_strVerify = GetElementValue(pElement);
						//ShowMessageExEx(m_strVerify);
						break;
					}
				}
			}
		}
		::SysFreeString(bstrText);
		pColl->Release();   
	}   
	pDoc2->Release();
	return 0;
}
	//获取指定ID标签的值（内容）
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
					if (strID == "添加账户")		//email name repassword password
					{
						BSTR   bstrValue;
						pElement->get_innerText(&bstrValue);
						if (CString(bstrValue) == "添加账户")
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
//获取指定ID标签的值（内容）
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
					if (strID == "添加账户")		//email name repassword password
					{
						BSTR   bstrValue;
						pElement->get_innerText(&bstrValue);
						if (CString(bstrValue) == "添加账户")
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
//获取指定ID标签的值（内容）
int CDHtmlDialogEx::ClickSpecialExEx(IHTMLDocument2 *pDoc2)
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
					pElement->get_id(&bstrID);
					CString strID(bstrID);		
					::SysFreeString(bstrID);
					if (strID == "pos4")		//email name repassword password
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
			pHTMLWnd->navigate(bstrValue);
			::SysFreeString(bstrValue);
// 			HRESULT hr = pHTMLWnd->get_document(&pFrameDoc);
// 			if(SUCCEEDED(hr))
// 			{   
// 				*pFrameDocRet = pFrameDoc;
// 			}
// 			pHTMLWnd->Release();
		}
	}
	//pElemColl->Release();


// 
// 	IHTMLElement* pelmBody = NULL;
// 	if (!pDoc)
// 	{
// 		return "";
// 	}
// 	pDoc->get_body(&pelmBody);
// 	if (!pelmBody)
// 	{
// 		pDoc->Release();
// 		return "";
// 	}
// 	BSTR bstrStr;
// 	pelmBody->get_innerHTML(&bstrStr);
// 	CString str(bstrStr);
// 	CFile file;
// 	file.Open("c:\\tessssss.txt",CFile::modeCreate|CFile::modeWrite);
// 	file.Write(str,str.GetLength());
// 	file.Close();
// 	pDoc->Release();
// 	pelmBody->Release();
// 	return str;
}
void CDHtmlDialogEx::OnBnClicked1008()
{
	SetFrameSrc();
}

//添加邮箱
int CDHtmlDialogEx::AddEmailAddr(IHTMLDocument2 *pDoc2)
{
	IHTMLElementCollection   *pColl=NULL;
	if (pDoc2 == NULL)
	{
		return -1;
	}
	HRESULT hr;
	GetElementCollection("input",&pColl,pDoc2);
	if(pColl!=NULL)   
	{   
		long   lcount   =   0;   
		pColl->get_length(&lcount);
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
					CString strPos = "";
					strPos.Format("%d",i);
					if (i == 1)
					{
						strPos = m_rgPack.strMail;
					}
					else if (i == 2)
					{
						strPos = m_rgPack.strIntroMan;
					}
					if (i==1 || i== 2)
					{
						VARIANT vtStr;
						vtStr.vt = VT_BSTR;
						vtStr.bstrVal = strPos.AllocSysString();					
						pElement->setAttribute(L"value",vtStr);	
						pElement.Release();
						::SysFreeString(vtStr.bstrVal);
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
		//检测值是否已经写入

		pColl->Release();   
	}
	
	//preSendToWebServer();
// 	IHTMLWindow2*   pWindow;
// 	hr   =   pDoc2->get_parentWindow(&pWindow);   
// 	VARIANT  ret;   
// 	hr   =   pWindow->execScript(L"preSendToWebServer();return false;",L"javascript",&ret);
// 	pWindow->Release();
	//执行点击事件
	GetElementCollection("a",&pColl,pDoc2);
	if(pColl!=NULL)   
	{   
		long   lcount   =   0;   
		pColl->get_length(&lcount);
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
					if (strID == "添加")		//email name repassword password
					{
						pElement->click();
						pElement.Release();
						pColl->Release();
						pDoc2->Release();
						ShowMessageExEx("开始提交邮箱验证...");
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
void CDHtmlDialogEx::AddImgCtrlID()
{
	IHTMLDocument2*   pDocument;
	HRESULT   hr   =   GetDHtmlDocument(&pDocument);   
	IHTMLWindow2*   pWindow;
	hr   =   pDocument->get_parentWindow(&pWindow);   
	VARIANT  ret;   
	CString strScript = "";	
	strScript= "document.all('f').src ='https://www.pinganyzt.com.cn/pinganv2/selinst_popup.jsp?EWTYPE=MAIL&EWTYPEDESC=email';document.all('f').title='%E6%B7%BB%E5%8A%A0%E8%B4%A6%E6%88%B7';";
	BSTR bstrStr = strScript.AllocSysString();
	hr   =   pWindow->execScript(bstrStr,L"javascript",&ret);
	::SysFreeString(bstrStr);
	pWindow->Release();
	pDocument->Release();
}
// CodeCheck.cpp : ���� DLL �ĳ�ʼ�����̡�
//

#include "stdafx.h"
#include <afxdllx.h>
#include <cv.h>
#include <cxcore.h>
#include <highgui.h>
#include "include/AspriseOCR.h"
#pragma comment(lib,"cxcore.lib")
#pragma comment(lib,"cv.lib")
#pragma comment(lib,"highgui.lib")
#pragma comment(lib,"lib\\AspriseOCR.lib")
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

static AFX_EXTENSION_MODULE CodeCheckDLL = { NULL, NULL };
int __stdcall MatchCheckCode(char *szPath, char *szCode,int iThreadShod);
extern "C" int APIENTRY
DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	// ���ʹ�� lpReserved���뽫���Ƴ�
	UNREFERENCED_PARAMETER(lpReserved);

	if (dwReason == DLL_PROCESS_ATTACH)
	{
		TRACE0("CodeCheck.DLL ���ڳ�ʼ����\n");
		
		// ��չ DLL һ���Գ�ʼ��
		if (!AfxInitExtensionModule(CodeCheckDLL, hInstance))
			return 0;

		// ���� DLL ���뵽��Դ����
		// ע��: �������չ DLL ��
		//  MFC ���� DLL(�� ActiveX �ؼ�)��ʽ���ӵ�
		//  �������� MFC Ӧ�ó������ӵ�������Ҫ
		//  �����д� DllMain ���Ƴ������������һ��
		//  �Ӵ���չ DLL �����ĵ����ĺ����С�ʹ�ô���չ DLL ��
		//  ���� DLL Ȼ��Ӧ��ʽ
		//  ���øú����Գ�ʼ������չ DLL������
		//  CDynLinkLibrary ���󲻻ḽ�ӵ�
		//  ���� DLL ����Դ���������������ص�
		//  ���⡣

		new CDynLinkLibrary(CodeCheckDLL);

	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		TRACE0("CodeCheck.DLL ������ֹ��\n");

		// �ڵ�����������֮ǰ��ֹ�ÿ�
		AfxTermExtensionModule(CodeCheckDLL);
	}
	return 1;   // ȷ��
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
CString GetRetString(char *szSrc)
{
	char num = 0;
	CString strSrc = "";
	CString strRet = "";
	CString strTemp = "";
	strSrc.Format("%s",szSrc);
	for (int i= 0;i<strSrc.GetLength();i++)
	{
		num = strSrc.GetAt(i);
		if ((num >= 48 && num <=57) ||
			(num >= 65 && num <=90) ||
			(num >= 97 && num <=122))
		{
			strTemp.Format("%c",num);
			if (strTemp == "B")
			{
				strTemp = "8";
			}
			else if (strTemp == "b")
			{
				strTemp = "6";
			}
			else if (strTemp == "O" || strTemp == "o")
			{
				strTemp = "0";
			}
			else if (strTemp == "S" || strTemp == "s")
			{
				strTemp = "5";
			}
			else if (strTemp == "g")
			{
				strTemp = "9";
			}
			else if (strTemp == "l")
			{
				strTemp = "1";
			}
			strRet += strTemp;
		}
	}
	return strRet;	
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
int __stdcall MatchCheckCode(char *szPath, char *szCode,int iThreadShod)
{
	try
	{
		IplImage *m_Source_Pic = NULL;
		m_Source_Pic = cvLoadImage(szPath);
		if (!m_Source_Pic)
		{
			strcpy(szCode,"can not open the image");
			return -2;	//�Ҳ���
		}
		IplImage * RedChannel   = cvCreateImage( cvGetSize(m_Source_Pic), 8, 1);
		IplImage * GreenChannel  = cvCreateImage( cvGetSize(m_Source_Pic), 8, 1);
		//IplImage * BlueChannel = cvCreateImage( cvGetSize(m_Source_Pic), 8, 1);
		//cvSetImageCOI(m_Source_Pic,1); 
		//cvCopy(m_Source_Pic,BlueChannel); //��ȡ��ɫ
		cvSetImageCOI(m_Source_Pic,2);
		cvCopy(m_Source_Pic,GreenChannel);  //��ȡ��ɫ
		cvSetImageCOI(m_Source_Pic,3);
		cvCopy(m_Source_Pic,RedChannel); //��ȡ��ɫ
		cvAdd(RedChannel,GreenChannel,RedChannel);
		cvThreshold(RedChannel,RedChannel,iThreadShod,255,CV_THRESH_BINARY);
		strcat(szPath,"2.bmp");
		cvSaveImage(szPath,RedChannel);
		char *szCodeDll = OCR(szPath,-1);
		CString strRet = GetRetString(szCodeDll);
		strcpy(szCode,strRet);
		cvReleaseImage(&m_Source_Pic);
		cvReleaseImage(&RedChannel);
		cvReleaseImage(&GreenChannel);
		//cvReleaseImage(&BlueChannel);
		return 0;
	}
	catch (...)
	{
		strcpy(szCode,"excption");
	}
	return -1;
}


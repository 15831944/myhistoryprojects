// 51jobpostDlg.h : ͷ�ļ�
//

#pragma once
#include "UserMessagePump.h"
#include <io.h>
#include "afxcmn.h"
#include "51JobWebPost.h"
#include "ListCtrlColore.h"
#include "afxwin.h"
#include "BtnST.h"
#include <gdiplus.h>
#include "afxwin.h"
#include <shlwapi.h>
#include <atlbase.h>
//#include "include\AspriseOCR.h"
using namespace Gdiplus;
#pragma comment(lib,"gdiplus.lib")
/*#pragma comment(lib,"lib\\AspriseOCR.lib")*/
// CjobpostDlg �Ի���
// #import "QpmiOcr.dll" //rename_namespace("OCRXX")
// using namespace QpmiOcr;
#import "CodeCheck.dll" //rename_namespace("OCRXX")
using namespace CodeCheck;
class CjobpostDlg : public CDialog
{
// ����
public:
	CjobpostDlg(CWnd* pParent = NULL);	// ��׼���캯��
        ~CjobpostDlg();
// �Ի�������
	enum { IDD = IDD_JOBPOST_DIALOG };
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
        C51JobWebPost m_JobPost;
        HICON m_hIcon;
        CString m_filename;     //��¼��Ϣ�����ļ�
        CListCtrlColore m_message;    //��Ϣ��ʾ��
	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
        void InitListCtrl();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
       	afx_msg HCURSOR OnQueryDragIcon();
        afx_msg LRESULT AddToList(WPARAM wParam,LPARAM lParam);
		BOOL PreTranslateMessage( MSG* pMsg);
	DECLARE_MESSAGE_MAP()
public:
        afx_msg void OnBnClickedBtnStart();        
		void SetTableStyle();
		void OnBnClickedBtnSet();
		CButtonST m_btn;
		afx_msg void OnBnClickedBtnCheckcode();
		BOOL ShowBmpFile(HDC hdc, int nXDest, int nYDest,LPBYTE lpDIB,CRect rect);
		bool ReadBMPFile(LPCTSTR strFileName,LPBYTE &m_lpBMPFileData,long& fileLength);
		void ShowCheckCode();
		BOOL GetImageCLSID(const WCHAR* format, CLSID* pCLSID);
		BOOL TranstToBMP(CString srcPath,CString destPath);
		GdiplusStartupInput m_gdiplusStartupInput;
		ULONG_PTR m_pGdiToken;
		LPBYTE m_pByte;
		afx_msg void OnBnClickedBtnRegiste();
		afx_msg void OnNMRclickListCount(NMHDR *pNMHDR, LRESULT *pResult);
		afx_msg void OnMenuRegister();
		afx_msg void OnBnClickedBtnStop();
		afx_msg void OnClose();
		HINSTANCE m_handle;
		_ClassCheck *m_pOcr;
		afx_msg void OnBnClickedButton2();
		CString GetRetString(CString strSrc);
		void LoadBMP(CString &sPath);
		CComQIPtr<IPicture> m_picture;
};

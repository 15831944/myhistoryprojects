#include "StdAfx.h"
#include "CBitmapConvert.h"
#define WIDTHBYTES(bits)    (((bits) + 31) / 32 * 4)
CCBitmapConvert::CCBitmapConvert(void)
{
	GdiplusStartup(&m_pGdiToken,&m_gdiplusStartupInput,NULL);
}

CCBitmapConvert::~CCBitmapConvert(void)
{
	GdiplusShutdown(m_pGdiToken);
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
BOOL CCBitmapConvert::GetImageCLSID(const WCHAR* format, CLSID* pCLSID)
{
	UINT num = 0;
	UINT size = 0;
	ImageCodecInfo* pImageCodecInfo = NULL;
	GetImageEncodersSize(&num, &size);
	if(size == 0)
	{
		return FALSE;
	}
	pImageCodecInfo = (ImageCodecInfo *)malloc((size));
	if(pImageCodecInfo == NULL)
		return FALSE;
	GetImageEncoders(num, size, pImageCodecInfo);
	// Find for the support of format for image in the windows
	for(UINT i = 0; i < num; ++i)
	{
		if( wcscmp(pImageCodecInfo[i].MimeType, format) == 0)
		{
			*pCLSID = pImageCodecInfo[i].Clsid;
			free(pImageCodecInfo);
			return TRUE;
		}
	}
	free(pImageCodecInfo);
	return FALSE;
}
/************************************************************************/
/*                                                                      */
/************************************************************************/

BOOL CCBitmapConvert::TranstToBMP(string srcPath,string destPath)
{
	remove(destPath.c_str());
	WCHAR wCH[255] = {0};
	WCHAR wCH1[255] = {0};
	wstring wstrPath = s2ws(srcPath);
	wcscpy(wCH,wstrPath.c_str());
	Image img(wCH);//�����ͼƬ������������ʽ	
	CLSID pngClsid;
	GetImageCLSID(L"image/bmp", &pngClsid);//�����ͼƬ������������ʽ���˴�ת��ΪBMP��ʽ
	wstrPath = s2ws(destPath);
	wcscpy(wCH1, wstrPath.c_str());
	img.Save(wCH1, &pngClsid, NULL);
	return TRUE;
}

std::wstring CCBitmapConvert::s2ws(const std::string& s) 
{ 
	int len; 
	int slength = (int)s.length() + 1; 
	len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);  
	wchar_t* buf = new wchar_t[len]; 
	MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len); 
	std::wstring r(buf); 
	delete[] buf; 
	return r; 
} 

string CCBitmapConvert::GenBmpPicPath(string strSrcPath)
{
	int iPos = strSrcPath.rfind('.');
	return strSrcPath.append(".bmp");
}

string CCBitmapConvert::GenBinaryPicPath(string strSrcPath)
{
	int iPos = strSrcPath.rfind('.');
	strSrcPath.insert(iPos,"binary");
	return strSrcPath;
}


/***************************************************************************
*   ��������   
*   OSTUThreshold()   
*   ����   
*         LPSTR   lpDIBBits           -   ָ��ԴDIBͼ��ָ��   
*         LONG     lWidth                 -   Դͼ����(������)   
*         LONG     lHeight               -   Դͼ��߶�(������)   
*   ����ֵ   
*         BOOL                                 -   ����ɹ�   ����TRUE   ,   ����   ����FALSE��   
*   ˵��   
*         �ú������ô�򷨽�����ֵ�ָ�   ��ֵ��   
***************************************************************************/   

int CCBitmapConvert::OSTUThreshold(LPBYTE   lpDIBBits,   LONG   lWidth,   LONG   lHeight)   
{   
	//   ָ��Դͼ������ָ��   
	LPSTR   lpSrc;   

	//   ָ�򻺴�ͼ��ָ��   
	LPSTR   lpDst;   

	//   ָ�򻺴�ͼ������ָ��   
	LPSTR   lpNewDIBBits;   
	HLOCAL   hNewDIBBits;   

	//   ѭ������   
	int   i,   j,   t;   

	//   ���ڼ�������������м����   
	long   lP1,   lS1,   lP2,   lS2;   

	//   ����ֵ   
	unsigned   char   pixel;   

	//   �Ҷ�ֱ��ͼ����   
	long   lHistogram[256];   

	//   ��ֵ,   ���Ҷ�ֵ����С�Ҷ�ֵ,   ���������ƽ���Ҷ�ֵ   
	unsigned   char   iThreshold,   iNewThreshold,   iMaxGrayValue,   iMinGrayValue,   iMean1GrayValue,   iMean2GrayValue;   

	//     ǰ������ռͼ�����,   ��������ռͼ�����   
	double   w0,w1;   

	//   ����   
	double   G,   tempG=0;   

	//   ͼ��ÿ��ռ�ֽ���   
	LONG   lLineBytes;   

	//   ��ʱ�����ڴ�,   �Ա�����ͼ��   
	hNewDIBBits   =   LocalAlloc(LHND,   lWidth   *   lHeight);   

	if   (hNewDIBBits   ==   NULL)     
	{   
		//�����ڴ�ʧ��   
		return   FALSE;   
	}   

	//   �����ڴ�   
	lpNewDIBBits   =   (char   *)   LocalLock(hNewDIBBits);   

	//   ��ʼ���·�����ڴ�,   �趨��ʼֵΪ255   
	lpDst   =   (char   *) lpNewDIBBits;   
	memset(lpDst,   (BYTE)255,   lWidth   *   lHeight);   

	lLineBytes   =   WIDTHBYTES(lWidth   *   8);   

	for(i=   0;   i   <   256;   i++)
	{   
		lHistogram[i]   =   0;   
	}   

	//   ��ûҶ�ֱ��ͼ���Ҷ����ֵ�ͻҶ���Сֵ   
	iMaxGrayValue   =   0;   
	iMinGrayValue   =   255;   
	for(i   =   0;   i   <   lWidth;   i++)   
	{   
		for(j   =   0;   j   <   lHeight;   j++)   
		{   
			lpSrc   =   (char   *)   lpDIBBits   +   lLineBytes   *   j   +   i;   
			pixel   =   (unsigned   char)   *lpSrc;   
			lHistogram[pixel]++;   

			//   �޸����Ҷ�ֵ����С�Ҷ�ֵ   
			if   (iMinGrayValue   >   pixel)   
			{   
				iMinGrayValue   =   pixel;   
			}   
			if   (iMaxGrayValue   <   pixel)   
			{   
				iMaxGrayValue   =   pixel;   
			}   
		}   
	}   

	//   ����t,   ѡȡ�����ֵ   
	for(t   =   iMinGrayValue;   t   <   iMaxGrayValue   ;   t++)   
	{   
		iNewThreshold   =   t;   
		lP1   =   0;   
		lS1   =   0;   
		lP2   =   0;   
		lS2   =   0;   

		//     ��ǰ��,�������������ƽ���Ҷ�ֵ,   ������ռ����   
		for(i   =   iMinGrayValue;   i   <=   iNewThreshold;   i++)   
		{   
			lP1   +=   lHistogram[i]   *   i;   
			lS1   +=   lHistogram[i];   
		}   
		iMean1GrayValue   =   (unsigned   char)   (lP1/lS1);   
		w0   =   (double)   (lS1)   /   (lWidth   *   lHeight);   
		for(i   =   iNewThreshold   +   1;   i   <=   iMaxGrayValue;   i++)   
		{   
			lP2   +=   lHistogram[i]   *   i;   
			lS2   +=   lHistogram[i];   
		}   
		iMean2GrayValue   =   (unsigned   char)   (lP2/lS2);   
		w1   =   1   -   w0;   

		//   ������䷽��   
		G   =   (double)   w0   *   w1     
			*   (iMean1GrayValue   -   iMean2GrayValue)   *   (iMean1GrayValue   -   iMean2GrayValue);   
		if   (G   >   tempG)     
		{   
			tempG   =   G;   
			iThreshold   =   iNewThreshold;   
		}   
	}   

	//   ������ֵ��ͼ���ֵ��   
	for(i   =   0;   i   <   lWidth;   i++)   
	{   
		for(j   =   0;   j   <   lHeight;   j++)   
		{   
			lpSrc   =   (char   *)   lpDIBBits   +   lLineBytes   *   j   +   i;   
			lpDst   =   (char   *)   lpNewDIBBits   +   lLineBytes   *   j   +   i;   
			pixel   =   (unsigned   char)   *lpSrc;   
			if   (pixel   <=   iThreshold)     
			{   
				*lpDst   =   (unsigned   char)0;   
			}   
			else   
			{   
				*lpDst   =   (unsigned   char)   255;   
			}   
		}   
	}   

	//   ����ͼ��   
	memcpy(lpDIBBits,   lpNewDIBBits,   lWidth   *   lHeight);
	//delete lpDst;
	//lpDst = NULL;
	//   �ͷ��ڴ�   
	LocalUnlock(hNewDIBBits);   
	LocalFree(hNewDIBBits);   

	//   ����   
	return   iThreshold;   
}



/***************************************************************************   
*   ��������   
*   ReadBMPFile  
*   ����   
*        LPCTSTR strFileName         -   ͼ���·��   

*   ����ֵ   
*         LPBYTE                     -����ͼ�����������
*   ˵��   
*         �ú��������ļ���ȡ�ķ�����ȡͼ���������   
***************************************************************************/ 
bool CCBitmapConvert::ReadBMPFile(LPCTSTR strFileName,LPBYTE &m_lpBMPFileData,long& fileLength,long &width,long &height)
{
	
	CFile BitmapFile;
	BOOL blOpen=BitmapFile.Open(strFileName, CFile::modeRead,NULL);
	if( !blOpen )
	{
		//CString str = "���ļ�ʧ�ܣ�";
		m_lpBMPFileData = NULL;
		return false;
	}

	unsigned long FileLength=BitmapFile.GetLength();
	fileLength = FileLength;
	m_lpBMPFileData = new BYTE[FileLength];

	ASSERT( m_lpBMPFileData!=NULL );
	BitmapFile.Read(m_lpBMPFileData,FileLength);
	
	if ( ((LPBITMAPFILEHEADER)m_lpBMPFileData)->bfType != 0x4D42 )
	{
		//CString str = "��֧�ֳ�BMP֮����ļ���";
		delete m_lpBMPFileData;
        m_lpBMPFileData = NULL;
		return false;;
	}
	LPBITMAPINFOHEADER lpBMPInfoHeader=(LPBITMAPINFOHEADER) (m_lpBMPFileData + sizeof(BITMAPFILEHEADER));
	width = lpBMPInfoHeader->biWidth;
	height = lpBMPInfoHeader->biHeight;
	return true;
}	

/************************************************************************* 
* 
* �������ƣ� 
* ConvertToGrayScale() 
* 
* ����: 
* LPSTR lpDIB - ָ��ԴDIBͼ��ָ�� 
* 
* ����ֵ: 
* bool - �ɹ�����true�����򷵻�false�� 
* 
* ˵��: 
* �ú�����24λ���ɫͼת����256���Ҷ�ͼ 
* ����Ϣͷ��ʼ
************************************************************************/ 
bool CCBitmapConvert::ConvertToGreyPicture( LPBYTE  pSource ) 
{ 

	LPSTR lpDIB = NULL;

	LPSTR   lpDIBBits = NULL; //ָ��DIB�����ص�ָ�� 
	LPSTR   lpNewDIBBits = NULL;
	LONG    lLineBytes = 0; 
	LONG    nWidth = 0; 
	LONG    nHeight = 0;
	LONG    nGreyLineBytes = 0; 
	int     nBitCount = 0; 
	LPBITMAPINFOHEADER pInforHead;

	unsigned char * lpSrc = NULL; //ָ��ԭͼ�����ص��ָ�� 
	unsigned char * lpdest = NULL; //ָ��Ŀ��ͼ�����ص��ָ��


	unsigned char  * ired = NULL;
	unsigned char  * igreen = NULL;
	unsigned char  * iblue = NULL; 
	long i = 0;
	long j =0; //ѭ������
	RGBQUAD * lpRGBquad = NULL; 

	if( pSource == NULL )
	{
		return false;
	}
	
	lpDIB = (LPSTR)pSource + sizeof(BITMAPFILEHEADER);
	pInforHead = (BITMAPINFOHEADER *)(lpDIB);
	nBitCount = pInforHead->biBitCount;

	nWidth= pInforHead->biWidth; //DIB ���
	nHeight = pInforHead->biHeight; //DIB �߶�

	if ( nBitCount == 8  )
	{
		//	lpDIBBits = (LPSTR)lpDIB + sizeof(BITMAPINFOHEADER) + 256 * 4;//ָ��DIB����
		//����Ҫ����
		return true;
	}

	lpRGBquad = (RGBQUAD *)&lpDIB[sizeof(BITMAPINFOHEADER)]; //INFOHEADER��Ϊ��ɫ�� 

	if ( nBitCount == 24  )
	{
		lLineBytes = WIDTHBYTES( nWidth * 8 * 3 ); 
	}
	else
	{
		lLineBytes = WIDTHBYTES( nWidth * 8  ); 
	}

	nGreyLineBytes = WIDTHBYTES( nWidth * 8 );
	long m_nLineBytes = nGreyLineBytes;

	//�ҶȻ��ȴ�������λͼ����
	lpdest= new BYTE[ nHeight * nGreyLineBytes];
	memset(lpdest , 0xff,  nHeight * nGreyLineBytes );

	lpDIBBits = (LPSTR)lpDIB + sizeof(BITMAPINFOHEADER);//ָ��DIB����

	
	for( i = 0; i < nHeight; i++)
	{
		for(j = 0; j < nWidth * 3; j += 3 )
			{ 
				ired = (unsigned char*)lpDIBBits + lLineBytes * i + j + 2; 
				igreen = (unsigned char*)lpDIBBits + lLineBytes * i + j + 1; 
				iblue = (unsigned char*)lpDIBBits + lLineBytes * i + j ; 
				lpdest[ i * nGreyLineBytes +  j / 3] = (unsigned char)((*ired)*0.299 + (*igreen)*0.587 + (*iblue)*0.114); 
			}

	}
	

	//��Ҫ���������飺1���޸�INFOHEADER 2�����ӵ�ɫ�� 3���޸�ԭͼ��Ҷ�ֵ 

	LPBITMAPINFOHEADER lpBMPInfoHeader;
	LPBITMAPFILEHEADER lpBMPFileHeader;
	lpBMPInfoHeader = (LPBITMAPINFOHEADER)lpDIB;
	lpBMPInfoHeader->biBitCount = 8; 
	lpBMPInfoHeader->biSizeImage = nGreyLineBytes *  nHeight; 
	lpBMPFileHeader = (LPBITMAPFILEHEADER)pSource; 
	lpBMPFileHeader->bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + 256 * 4;
	lpBMPFileHeader->bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + lpBMPInfoHeader->biSizeImage ;
	
	//���ûҶȵ�ɫ�� 
	for(i = 0;i < 256;i++) 
	{ 
		lpRGBquad[i].rgbRed = (unsigned char)i;
		lpRGBquad[i].rgbGreen = (unsigned char)i;
		lpRGBquad[i].rgbBlue = (unsigned char)i;
		lpRGBquad[i].rgbReserved = 0; 
	} 

	lpNewDIBBits = lpDIB + sizeof(BITMAPINFOHEADER) + 256 * 4;
	
	//�޸ĻҶ�ֵ 
	for(i = 0;i < nHeight; i++) 
	{
		for(j = 0;j < nGreyLineBytes; j++)
		{ 
			lpSrc = (unsigned char*)lpNewDIBBits + nGreyLineBytes * i+ j ; 
			*lpSrc = lpdest[ i * nGreyLineBytes + j] ; 
		} 
	}

	delete lpdest;
	lpdest = NULL;
	return true;
} 

//λͼ��ʽ����ͼƬ�Ĳ�������
bool CCBitmapConvert::SaveNewPicture(CString filePath,LPBYTE m_newBmpData,long filelength)
{
	CFile file;
	if( !file.Open( filePath, CFile::modeWrite|CFile::modeCreate) )
		return false;
	file.Write(m_newBmpData,filelength);
		return true;
}
#include "StdAfx.h"
#include ".\myimage.h"
#include <direct.h>
#include <io.h>
#define WIDTHBYTES(i) ((i+31)/32*4)
CMyImage::CMyImage(void)
{
	    
	GdiplusStartup(&gdiplusToken,&gdiplusStartupInput, NULL);
}

CMyImage::~CMyImage(void)
{
	GdiplusShutdown(gdiplusToken);
}

//��ͼƬ��Ϣת����BMP��ʽ����
BOOL CMyImage::TranstToBMP(CString srcPath,CString destPath)
{
	
	
	WCHAR* wCH = new WCHAR[srcPath.GetLength()];
	wcscpy(wCH,srcPath.AllocSysString());

	Image img(wCH);//�����ͼƬ������������ʽ
	CLSID pngClsid;
	GetImageCLSID(L"image/bmp", &pngClsid);//�����ͼƬ������������ʽ���˴�ת��ΪBMP��ʽ

	WCHAR* wCH1 = new WCHAR[destPath.GetLength()];
	wcscpy(wCH1,   destPath.AllocSysString());
	img.Save(wCH1, &pngClsid, NULL);
	
	delete []wCH;
	delete []wCH1;
	wCH = NULL;
	wCH1 = NULL;

	
	return TRUE;
}

/////////
BOOL CMyImage::GetImageCLSID(const WCHAR* format, CLSID* pCLSID)
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
		//MimeType: Depiction for the program image
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

//��ʾλͼ����
BOOL CMyImage::ShowBmpFile(HDC hdc, int nXDest, int nYDest,LPBYTE lpDIB)
{
	if( lpDIB == NULL)
	{
		return FALSE;
	}
	LPBITMAPINFOHEADER lpBMPInfoHeader=(LPBITMAPINFOHEADER) (lpDIB + sizeof(BITMAPFILEHEADER));
	BITMAPINFO * lpBMPInfo =(BITMAPINFO *)(lpDIB + sizeof(BITMAPFILEHEADER));

	SetDIBitsToDevice(hdc,nXDest,nYDest,lpBMPInfoHeader->biWidth,lpBMPInfoHeader->biHeight,
			0,0,0,lpBMPInfoHeader->biHeight,lpDIB+((BITMAPFILEHEADER *)lpDIB)->bfOffBits,lpBMPInfo,DIB_RGB_COLORS);
		
	return TRUE;
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

int CMyImage::OSTUThreshold(LPBYTE   lpDIBBits,   LONG   lWidth,   LONG   lHeight)   
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
	delete lpDst;
	lpDst = NULL;
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
bool CMyImage::ReadBMPFile(LPCTSTR strFileName,LPBYTE &m_lpBMPFileData,long& fileLength)
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
	return true;
}	

//��GDI+�У��й��ַ��Ĳ�������ȫ������WCHAR����     
//�ú����ǽ���ͳ�ַ�������ת�� 
WCHAR*  CMyImage::ToWChar(char * str)     
{     
	static   WCHAR   buffer[1024];
	wcsset(buffer,0);
	MultiByteToWideChar(CP_ACP,0,str,strlen(str),buffer,1024);
	return   buffer;     
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
bool CMyImage::ConvertToGreyPicture( LPBYTE  pSource ) 
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
bool CMyImage::SaveNewPicture(CString filePath,LPBYTE m_newBmpData,long filelength)
{
	CFile file;
	if( !file.Open( filePath, CFile::modeWrite|CFile::modeCreate) )
		return false;
	file.Write(m_newBmpData,filelength);
		return true;
}

//��ֵ�˲�
void CMyImage::ClearNoise(LPBYTE m_BmpData)
{
	long lWidth = 0;
	long lHeight = 0;

	if (m_BmpData == NULL)
	{
		return ;
	}

	lWidth = ((LPBITMAPINFOHEADER)(m_BmpData +sizeof(BITMAPFILEHEADER)))->biWidth;
	lHeight = ((LPBITMAPINFOHEADER)(m_BmpData +sizeof(BITMAPFILEHEADER)))->biHeight;

	long lLineBytes = WIDTHBYTES(lWidth   *   8);
	long totalNum = lWidth*lHeight;
	LPBYTE lpBitmapData = new BYTE[totalNum];

	::memcpy(lpBitmapData,(LPBYTE)m_BmpData + ((BITMAPFILEHEADER *)m_BmpData)->bfOffBits,totalNum);

	BYTE byteArea[9] = {' '};
	BYTE tempByte = 0;
	 for (long i = 1;i < lWidth - 1; i++ )
		for (long j = 1; j < lHeight - 1; j++)
		{
			byteArea[0] = *(lpBitmapData + lLineBytes *(j - 1) + i - 1);
            byteArea[1] = *(lpBitmapData + lLineBytes *(j - 1) + i);
			byteArea[2] = *(lpBitmapData + lLineBytes *(j - 1) + i + 1);
			byteArea[3] = *(lpBitmapData + lLineBytes * j + i - 1);
			byteArea[4] = *(lpBitmapData + lLineBytes * j + i);
			byteArea[5] = *(lpBitmapData + lLineBytes * j + i + 1);
			byteArea[6] = *(lpBitmapData + lLineBytes *(j + 1) + i - 1);
			byteArea[7] = *(lpBitmapData + lLineBytes *(j - 1) + i);
			byteArea[8] = *(lpBitmapData + lLineBytes *(j + 1) + i + 1);
			int totalNum = 0;
			for (int k = 0; k<5; k++)
				for (int m = k+1;m< 9;m++)
				{
					if (byteArea[m] > byteArea[k])
					{
						tempByte  = byteArea[m];
						byteArea[m] = byteArea[k];
						byteArea[k] = tempByte;

					}
				}
			
			 tempByte = *(lpBitmapData + lLineBytes * j + i);	
			 if(tempByte == byteArea[0] || tempByte == byteArea[9])
			 {
				 *(lpBitmapData + lLineBytes * j + i) = byteArea[4];
			 }
			
		}

		::memcpy((LPBYTE)m_BmpData + ((BITMAPFILEHEADER *)m_BmpData)->bfOffBits,lpBitmapData,totalNum);
		delete lpBitmapData;
		lpBitmapData = NULL;
		return;	
}

//��IplImage*���͵�ͼ��������ʾ������ָ��λ�ô�
void CMyImage::ShowToDC(IplImage* imgShow,HWND m_hwnd,int startX,int startY)
{
	CImage CImgShow;
	CImgShow.CopyOf(imgShow,-1);
	CImgShow.DrawToHDC(::GetDC(m_hwnd), &CRect(startX,startY,imgShow->width+startX,imgShow->height+startY));
}


/************************************************************************/
/*                                                                      */
/************************************************************************/
//��ȡ��ϷͼƬ����
int CMyImage::GetSelectWindowPic(HWND hwnd,CString strSavePath)
{
	CBitmap bitmap;
	LPBYTE pDst = NULL;
	CDC *pDC;
	pDC = CDC::FromHandle(::GetDC(hwnd));
	if (!pDC)
	{
		return -1;
	}
	CRect rc;
	CWnd::FromHandle(hwnd)->GetClientRect(&rc);

	int BitPerPixel = pDC->GetDeviceCaps(BITSPIXEL);//�����ɫģʽ

	int Width = rc.Width();//pDC->GetDeviceCaps(HORZRES);
	int Height = rc.Height();//pDC->GetDeviceCaps(VERTRES);

	CDC memDC;//�ڴ�DC
	memDC.CreateCompatibleDC(pDC);

	CBitmap memBitmap, *oldmemBitmap;//��������Ļ���ݵ�bitmap
	memBitmap.CreateCompatibleBitmap(pDC, Width, Height);

	oldmemBitmap = memDC.SelectObject(&memBitmap);//��memBitmapѡ���ڴ�DC
	memDC.BitBlt(0, 0, Width, Height, pDC, 0, 0, SRCCOPY);//������Ļͼ���ڴ�DC

	//���´��뱣��memDC�е�λͼ���ļ�
	BITMAP bmp;
	memBitmap.GetBitmap(&bmp);//���λͼ��Ϣ

	BITMAPFILEHEADER bfh = {0};//λͼ�ļ�ͷ
	bfh.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);//��λͼ���ݵ�ƫ����
	bfh.bfSize = bfh.bfOffBits + bmp.bmWidthBytes * bmp.bmHeight;//�ļ��ܵĴ�С
	bfh.bfType = (WORD)0x4d42;

	BITMAPINFOHEADER bih = {0};//λͼ��Ϣͷ
	bih.biBitCount = bmp.bmBitsPixel;//ÿ�������ֽڴ�С
	bih.biCompression = BI_RGB;
	bih.biHeight = bmp.bmHeight;//�߶�
	bih.biPlanes = 1;
	bih.biSize = sizeof(BITMAPINFOHEADER);
	bih.biSizeImage = bmp.bmWidthBytes * bmp.bmHeight;//ͼ�����ݴ�С
	bih.biWidth = bmp.bmWidth;//���

	BYTE *p = new BYTE[bmp.bmWidthBytes * bmp.bmHeight];//�����ڴ汣��λͼ����
	GetDIBits(memDC.m_hDC, (HBITMAP) memBitmap.m_hObject, 0, Height, p,
		(LPBITMAPINFO) &bih, DIB_RGB_COLORS);//��ȡλͼ����

	pDst = new BYTE[bfh.bfOffBits + bmp.bmWidthBytes * bmp.bmHeight];
	memcpy(pDst,&bfh,sizeof(BITMAPFILEHEADER));
	memcpy(pDst +sizeof(BITMAPFILEHEADER),&bih,sizeof(BITMAPINFOHEADER));
	memcpy(pDst+bfh.bfOffBits,p,bmp.bmWidthBytes * bmp.bmHeight);

	SaveNewPicture(strSavePath,pDst,bfh.bfOffBits+bmp.bmWidthBytes * bmp.bmHeight);
	delete []p;
	delete []pDst;
	memDC.SelectObject(oldmemBitmap);
	return 0;
}
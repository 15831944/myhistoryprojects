#include "StdAfx.h"
#include ".\opencvcheck.h"
#define WIDTHBYTES(i) ((i+31)/32*4)
COpenCVCheck::COpenCVCheck(void)
{
	myPicPath = "C:\\tempOpenCVfile.bmp";
}

COpenCVCheck::~COpenCVCheck(void)
{
}

void COpenCVCheck::OpenCVBinary(CString fileName)
{
	CvScalar colors[] = {{255,255,255},{0,0,0}};
	IplImage* pImg; //����IplImageָ��
	if((pImg = cvLoadImage(fileName, 0)) != 0)
	{
		IplImage* dst = NULL;
		dst=cvCreateImage(cvSize(pImg->width,pImg->height),IPL_DEPTH_8U,1);
		//cvThreshold(pImg,dst,185,255,CV_THRESH_BINARY);
		cvAdaptiveThreshold(pImg,dst,255,CV_ADAPTIVE_THRESH_MEAN_C,CV_THRESH_BINARY,5,3);//��ֵ��

		ReverseColor(dst);
		for (int kk = 0;kk<2;kk++)   //ȥ��
		{
			CvSeq *contours;
			CvMemStorage* storage = cvCreateMemStorage(0);

			cvFindContours( dst, storage, &contours, sizeof(CvContour), CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, cvPoint(0,0) );
			//�˺����Ժ�ɫΪ����ɫ
			while(contours)
			{
				//approximate contour with accuracy proportional
				CvSeq* result = cvApproxPoly( contours, sizeof(CvContour),
					storage,CV_POLY_APPROX_DP, 3, 1);
				//to filter the noisy contour
				if(fabs(cvContourArea(result,CV_WHOLE_SEQ)) < 2)
				{
					if (result->total > 0)
					{
						for(int  i = 0; i < (result ? result->total : 0); i++)
						{
							CvRect* r = (CvRect*)cvGetSeqElem(result,i);
							cvSet2D(dst,r->y,r->x,colors[1]);

						}

					}
				}
				contours = contours->h_next;
			}	
		}
		ReverseColor(dst);
		ClearNoise(dst);
		cvSaveImage(fileName,dst);
		cvReleaseImage(&dst);
		cvReleaseImage(&pImg);
	}
}


//��һ��ͨ������ȥ��������(OPENCV)
void COpenCVCheck::ClearNoise(IplImage *pImg)
{
	CvScalar colors[] = {{255,255,255},{0,0,0}};
	for (int i=1;i<pImg->height-1;i++)
		for (int j=1;j<pImg->width-1;j++)
		{
			CvScalar tempScalar = cvGet2D(pImg,i,j);
			if (tempScalar.val[0] == 0)
			{
				int x;
				int y;
				int round_black_num = GetNearBlackNum(pImg,x,y,i,j);
				if (round_black_num < 1)
				{
					cvSet2D(pImg,i,j,colors[0]);
				}
				if (round_black_num == 1)
				{
					int m;
					int n;
					if (GetNearBlackNum(pImg,m,n,x,y) == 1)
					{
						cvSet2D(pImg,i,j,colors[0]);
						cvSet2D(pImg,x,y,colors[0]);
					}
				}
			}
			
		}
}

int COpenCVCheck::GetNearBlackNum(IplImage * pImg,int &x,int &y,int i,int j)
{
	int round_black_num = 0;  //�ڵ��ܱߺڵ����
	if (cvGet2D(pImg,i-1,j-1).val[0] == 0)
	{
		round_black_num++;
		x = i-1;
		y = j-1;
	}
	if (cvGet2D(pImg,i,j-1).val[0] == 0)
	{
		round_black_num++;
		x = i;
		y = j-1;
	}
	if (cvGet2D(pImg,i+1,j-1).val[0] == 0)
	{
		round_black_num++;
		x = i+1;
		y = j-1;
	}
	if (cvGet2D(pImg,i-1,j).val[0] == 0)
	{
		round_black_num++;
		x = i-1;
		y = j;
	}
	if (cvGet2D(pImg,i+1,j).val[0] == 0)
	{
		round_black_num++;
		x = i+1;
		y = j;
	}
	if (cvGet2D(pImg,i-1,j+1).val[0] == 0)
	{
		round_black_num++;
		x = i-1;
		y = j+1;
	}
	if (cvGet2D(pImg,i,j+1).val[0] == 0)
	{
		round_black_num++;
		x = i;
		y = j+1;
	}
	if (cvGet2D(pImg,i+1,j+1).val[0] == 0)
	{
		round_black_num++;
		x = i+1;
		y = j+1;
	}
	return round_black_num;
}

//��һ��ͨ������ȥ��������
void COpenCVCheck::ClearNoise(LPBYTE pDIB)
{
	long lWidth = ((LPBITMAPINFOHEADER)(pDIB +sizeof(BITMAPFILEHEADER)))->biWidth;
	long lHeight = ((LPBITMAPINFOHEADER)(pDIB +sizeof(BITMAPFILEHEADER)))->biHeight;

	long lLineBytes = WIDTHBYTES(lWidth   *   8);
	long totalNum = lWidth * lHeight;
	LPBYTE lpBitmapData = pDIB + ((BITMAPFILEHEADER *)pDIB)->bfOffBits;
	for (int i=1;i<lWidth-1;i++)
		for (int j=1;j<lHeight-1;j++)
		{
			unsigned char* temp = lpBitmapData + j*lLineBytes + i;
			
			if (*temp == 0) //����˵�Ϊ�ڣ���ô�ж����ǲ���������
			{
				int x;
				int y;
				int round_black_num = GetNearBlackNum(lpBitmapData,x,y,i,j,lLineBytes);
				if (round_black_num < 1)
				{
					*temp = 255;
				}
				if (round_black_num == 1)
				{
					int m;
					int n;
					if (GetNearBlackNum(lpBitmapData,m,n,x,y,lLineBytes) == 1)
					{
						*temp = 255;
						*(lpBitmapData + y*lLineBytes + x) = 255;
					}
				}

			}
			
		}
}

int COpenCVCheck::GetNearBlackNum(LPBYTE lpBits,int &x,int &y,int i,int j,long lLineBytes)
{
	int round_black_num = 0;  //�ڵ��ܱߺڵ����
	if (*(lpBits + (j-1)*lLineBytes + i-1) == 0)   //i-1,j-1
	{
		round_black_num++;
		x = i-1;
		y = j-1;
	}
	if (*(lpBits + (j-1)*lLineBytes + i) == 0)   // i,j-1
	{
		round_black_num++;
		x = i;
		y = j-1;
	}
	if (*(lpBits + (j-1)*lLineBytes + i+1) == 0)  // i+1,j-1
	{
		round_black_num++;
		x = i+1;
		y = j-1;
	}
	if (*(lpBits + j*lLineBytes + i-1) == 0)		// i-1,j
	{
		round_black_num++;
		x = i-1;
		y = j;
	}
	if (*(lpBits + j*lLineBytes + i+1) == 0)		//i+1,j
	{
		round_black_num++;
		x = i+1;
		y = j;
	}
	if (*(lpBits + (j+1)*lLineBytes + i-1) == 0)   //i-1,j+1
	{
		round_black_num++;
		x = i-1;
		y = j+1;
	}
	if (*(lpBits + (j+1)*lLineBytes + i) == 0)	//i,j+1
	{
		round_black_num++;
		x = i;
		y = j+1;
	}
	if (*(lpBits + (j+1)*lLineBytes + i+1) == 0)	//i+1,j+1
	{
		round_black_num++;
		x = i+1;
		y = j+1;
	}
	return round_black_num;
}
void COpenCVCheck::GetBinaryData(LPBYTE pSourse,LPBYTE &m_newBMP)
{
	SaveNewPicture(myPicPath,pSourse,GetPicLength(pSourse));
	OpenCVBinary(myPicPath);
	ReadBMPFile(myPicPath,m_newBMP);
}

//λͼ��ʽ����ͼƬ�Ĳ�������
bool COpenCVCheck::SaveNewPicture(CString filePath,LPBYTE m_newBmpData,long filelength)
{
	CFile file;
	if( !file.Open( filePath, CFile::modeWrite|CFile::modeCreate))
		return false;
	file.Write(m_newBmpData,filelength);
	return true;
}

//��ȡͼƬ�ĸ߶Ⱥ�ռ���ֽڿ��
int COpenCVCheck::GetPicLength(LPBYTE pSource)
{
	int nHeight;
	int nWidth;
	int nGreyLineBytes;
	LPBYTE lpDIB = NULL;
	LPBITMAPFILEHEADER pFileHead;
	LPBITMAPINFOHEADER pInfoHead;
	if( pSource == NULL )
	{
		return 0;
	}
	pFileHead = (BITMAPFILEHEADER *)pSource;
	lpDIB = pSource + sizeof(BITMAPFILEHEADER);
	pInfoHead = (BITMAPINFOHEADER *)(lpDIB);

	nWidth= pInfoHead->biWidth; //DIB ���
	nHeight = pInfoHead->biHeight; //DIB �߶�
	nGreyLineBytes = WIDTHBYTES( nWidth * 8 );
	return pFileHead->bfOffBits + nGreyLineBytes * nHeight;
}


void COpenCVCheck::ReverseColor(IplImage* pImg)
{
	CvScalar colors[] = {{255,255,255},{0,0,0}};
	for (int i=0;i<pImg->height;i++)
		for (int j=0;j<pImg->width;j++)
		{
			CvScalar tempScalar = cvGet2D(pImg,i,j);
			if (tempScalar.val[0] == 0)
			{
				cvSet2D(pImg,i,j,colors[0]);
			}
			else
			{
				cvSet2D(pImg,i,j,colors[1]);
			}
		}
}

//��ʾλͼ����
BOOL COpenCVCheck::ShowBmpFile(HDC hdc, int nXDest, int nYDest,LPBYTE lpDIB)
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
*   ReadBMPFile  
*   ����   
*        LPCTSTR strFileName         -   ͼ���·��

*   ����ֵ   
*         LPBYTE                     -����ͼ�����������
*   ˵��   
*         �ú��������ļ���ȡ�ķ�����ȡͼ���������   
***************************************************************************/ 
bool COpenCVCheck::ReadBMPFile(LPCTSTR strFileName,LPBYTE &m_lpBMPFileData)
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

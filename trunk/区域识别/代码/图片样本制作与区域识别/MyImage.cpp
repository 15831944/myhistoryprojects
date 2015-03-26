#include "StdAfx.h"
#include ".\myimage.h"
#include <direct.h>
#include <io.h>
#define WIDTHBYTES(i) ((i+31)/32*4)
CMyImage::CMyImage(void)
{

}

CMyImage::~CMyImage(void)
{

}

/***************************************************************************   
*   ��������   
*   MatchPicture  
*   ����   
*       CString imgPath         -   ͼ���·��  
*       CString xmlPath         -   ģ��·��

*   ����ֵ   
*         CPoint   -����ƥ��������������꣬����ⲻ���򷵻أ�0��0��
*   ˵��   
*         �ú�������ͼƬ����ʶ�� 
***************************************************************************/ 
CPoint CMyImage::MatchPicture(CString imgPath,CString xmlPath)
{
	CPoint centerPoint;
	
	CvHaarClassifierCascade* cascade = 0;
	cascade = (CvHaarClassifierCascade*)cvLoad(xmlPath); //���������ļ�
	if( !cascade )
	{
		return CPoint(0,0);
	}

	IplImage* image = cvLoadImage( imgPath,1); //���صȴ�����ͼ��
	if(image)
	{
		centerPoint = Detect_and_Draw(image,cascade,0);   //Ŀ����
		cvWaitKey(0);
		cvReleaseImage(&image);
	}
	else
	{
		return CPoint(0,0);
	}
	cvReleaseHaarClassifierCascade( &cascade);  //�ͷ�
	return centerPoint;
}

//���ͼƬ�����������ĵ�����
CPoint CMyImage::Detect_and_Draw(IplImage* img,CvHaarClassifierCascade* cascade,int do_pyramids)
{
	IplImage* small_image = img;
	CvMemStorage* storage = cvCreateMemStorage(0); //������̬�ڴ�
	CvSeq* faces;
	int i, scale = 1;
	if( do_pyramids )
	{
		small_image = cvCreateImage( cvSize(img->width/2,img->height/2), IPL_DEPTH_8U, 3 );
		//���� cvPyrDown ʹ�� Gaussian �������ֽ������ͼ�����²�����
		//������������ͼ����ָ���˲������о����Ȼ��ͨ���ܾ�ż�������������²���ͼ��
		cvPyrDown( img, small_image, CV_GAUSSIAN_5x5 );
		scale = 2;
	}
	
	CvPoint center;
	if( cascade )
	{
		//���
		faces = cvHaarDetectObjects( small_image, cascade, storage, 1.2, 2, CV_HAAR_DO_CANNY_PRUNING );
		int radius = 200;
		if (faces->total > 0)
		{
				for( i = 0; i < (faces ? faces->total : 0); i++ )
				{
					CvRect* r = (CvRect*)cvGetSeqElem( faces, i );
					
					int tempRadiu = cvRound((r->width + r->height)*0.25*scale);
					if (tempRadiu < radius)  //ȡ�뾶��С��Ϊ���ĵ�
 					{
						radius = tempRadiu;
						center.x = cvRound((r->x + r->width*0.5)*scale);
						center.y = cvRound((r->y + r->height*0.5)*scale);
					}
					cvRectangle(small_image,cvPoint(r->x,r->y),cvPoint(r->x+r->width,r->y+r->height),cvScalar(255,255,255),1);
				}
		}
		else
		{
			center.x = 0;
			center.y = 0;
		}
	}
	else
	{
		center.x = 0;
		center.y = 0;
	}
	cvNamedWindow("Image");
	cvShowImage("Image",small_image);
	cvWaitKey(0); //�ȴ�����

	cvDestroyWindow( "Image" );//���ٴ���
	if( small_image != img )
	cvReleaseImage( &small_image );

	return CPoint(center.x,center.y);
}

//��������
void CMyImage::MackTemplate(CString srcPath)
{

	IplImage* pImg; //����IplImageָ��
	if((pImg = cvLoadImage(srcPath, 1)) != 0 )
	{
		cvNamedWindow( "Image", 1 );//��������
		cvShowImage("Image", pImg );//��ʾͼ��
		
		myParam *myparam = new myParam;
		myparam->img = pImg;
		myparam->drawFlag = false;
		myparam->finishFlag = false;
		cvSetMouseCallback("Image",on_mouseProc,(void*)myparam);  //�������ڹ���

		cvWaitKey(0); //�ȴ�����

		cvDestroyWindow( "Image" );//���ٴ���
		cvReleaseImage( &pImg ); //�ͷ�ͼ��
		delete myparam;
		myparam = NULL;
	}
}

//��ȡ�������������Ĵ��ڹ���,������������
void CMyImage::on_mouseProc( int event, int x, int y, int flags, void* param)
{
	
	myParam *myparam = (myParam*)param;
	//�����������
	if(myparam->drawFlag==false && myparam->finishFlag == false && event==CV_EVENT_LBUTTONDOWN)
	{
		myparam->drawFlag = true;
	}
	
	if(myparam->drawFlag==true && myparam->finishFlag == false && event==CV_EVENT_MOUSEMOVE)
	{
		IplImage *tempImage = 0;
		tempImage = cvCreateImage(cvGetSize(myparam->img), IPL_DEPTH_8U, 1);
		tempImage = cvCloneImage(myparam->img);

        cvRectangle(tempImage,cvPoint(x,y),cvPoint(x+40,y+40),cvScalar(255,255,255),1);
		cvShowImage("Image",tempImage);
	}

	//�Ҽ����������������
	if(myparam->drawFlag==true && myparam->finishFlag == false && event==CV_EVENT_RBUTTONDOWN)
	{
		CFileStatus   rStatus;
		CString dirPath = "C:\\OpenCVTrain\\Template";
		if(!CFile::GetStatus(dirPath,rStatus)) //Ŀ¼������
		{
			mkdir(dirPath);
		}

		CvMat *dst;
		dst=cvCreateMat(40,40,CV_8UC1);
		CvRect subRect;

		subRect=cvRect(x,y,40,40);
		cvGetSubRect(myparam->img,dst,subRect);
		IplImage *dst1;
		dst1=cvCreateImage(cvSize(40,40),IPL_DEPTH_8U,1);
		cvGetImage(dst,dst1);
		CString filePath;
		CString fileName;
		fileName.Format("%d%d%s.jpg",x,y,CTime::GetCurrentTime().Format("%Y%m%d%H%M%S"));//����������
		filePath = dirPath + "\\" + fileName;
		cvSaveImage(filePath,dst1);

		// ��Ŀ���������ɰ�ɫ���ٽ�����ͼ����Ϊ����ͼ����������
		CvPoint  curve1[]={x,y,  x+40,y,  x+42,y+40,  x,y+40}; 
		CvPoint* curveArr[1]={curve1};
		int  nCurvePts[1]={4};
		int  nCurves=1;

		cvFillPoly(myparam->img,curveArr,nCurvePts,nCurves,cvScalar(255,255,255));


		CString diroffPath = "C:\\OpenCVTrain\\offTemplate";
		if(!CFile::GetStatus(diroffPath,rStatus)) //Ŀ¼������
		{
			mkdir(diroffPath);
		}

		CString bgFilePath;
		CString bgFileName;
		bgFileName.Format("%d%d%s.jpg",x,y,CTime::GetCurrentTime().Format("%Y%m%d%H%M%S"));//���渺����
		bgFilePath = diroffPath + "\\" + fileName;
		cvSaveImage(bgFilePath,myparam->img);

		myparam->drawFlag = false;
		myparam->finishFlag = true;

	}


}

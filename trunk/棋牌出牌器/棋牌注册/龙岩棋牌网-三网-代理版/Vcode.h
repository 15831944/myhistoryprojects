//���뺯����̬���ļ�
#pragma comment(lib, "Vcode.lib")

extern "C"
{
//����һ���µ���֤���
	int _stdcall LoadDe(char* strFilePath,char* strFilePath2);

//ȡ��֤�����Ϣ
	int _stdcall DeInfo(char* Info);

//ȡ��֤��������Ϣ
	int _stdcall DeError(char* Info);


//PicPath:��֤��ͼƬ��URL
	int _stdcall GetUrlVcode(char* PicPath,int Index,char* result);


//PicPath:����ֻ�ǰ�����֤��URL��һ���������ı�
	int _stdcall GetWebVcode(void* WebHWnd,char* PicPath,int Index,char* result);


	int _stdcall GetFileVcode(char* PicName,int Index,char* result);


	int _stdcall GetByteVcode(void* ByteData,int ByteLength,int Index,char* result);

	int _stdcall GetRecNum(void* ImageDate,int ImageLength,int BYTEflag,char* result);
}


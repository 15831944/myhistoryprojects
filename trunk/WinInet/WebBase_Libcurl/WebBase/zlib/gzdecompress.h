#include <string>
using std::string;

//ʹ��ʱ,�����������ÿռ�, ��ѹ������ݾͷ�������
//int gzuncompress(char* filename,char*&buf,int &len);

int gzuncompress( CString & strBuffer, int & nlen);
int gzuncompress( char* strBuffer, int & nlen, string& strRet);
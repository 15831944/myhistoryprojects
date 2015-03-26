// ���� ifdef ���Ǵ���ʹ�� DLL �������򵥵�
// ��ı�׼�������� DLL �е������ļ��������������϶���� UDISKDLL_EXPORTS
// ���ű���ġ���ʹ�ô� DLL ��
// �κ�������Ŀ�ϲ�Ӧ����˷��š�������Դ�ļ��а������ļ����κ�������Ŀ���Ὣ
// UDISKDLL_API ������Ϊ�Ǵ� DLL ����ģ����� DLL ���ô˺궨���
// ������Ϊ�Ǳ������ġ�
#ifdef UDISKDLL_EXPORTS
#define UDISKDLL_API __declspec(dllexport)
#else
#define UDISKDLL_API __declspec(dllimport)
#endif
#define GDI_VOLUMENAME 0
#define GDI_VOLUMESERIALNUMBER 1
#define GDI_VOLUMEFILESYSTEM 2
#define GDI_VOLUMETYPE 3
#define GDI_VOLUMESIZE 4
#define GDI_VOLUMEFREESIZE 5
// �����Ǵ� UDiskDll.dll ������
class UDISKDLL_API CUDiskDll {
public:
	CUDiskDll(void);
	// TODO: �ڴ�������ķ�����
};

extern UDISKDLL_API int nUDiskDll;

UDISKDLL_API int fnUDiskDll(void);
CString GetDriverInfomation(char* bAlpha);
extern "C" UDISKDLL_API bool GetDiskSerial(char* szDisk,char* szSerial);
extern "C" UDISKDLL_API bool GetDisks(char* szDisks);
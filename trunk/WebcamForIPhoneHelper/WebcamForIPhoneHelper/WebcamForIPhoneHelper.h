// ���� ifdef ���Ǵ���ʹ�� DLL �������򵥵�
// ��ı�׼�������� DLL �е������ļ��������������϶���� WEBCAMFORIPHONEHELPER_EXPORTS
// ���ű���ġ���ʹ�ô� DLL ��
// �κ�������Ŀ�ϲ�Ӧ����˷��š�������Դ�ļ��а������ļ����κ�������Ŀ���Ὣ
// WEBCAMFORIPHONEHELPER_API ������Ϊ�Ǵ� DLL ����ģ����� DLL ���ô˺궨���
// ������Ϊ�Ǳ������ġ�
#ifdef WEBCAMFORIPHONEHELPER_EXPORTS
#define WEBCAMFORIPHONEHELPER_API __declspec(dllexport)
#else
#define WEBCAMFORIPHONEHELPER_API __declspec(dllimport)
#endif

// �����Ǵ� WebcamForIPhoneHelper.dll ������
class WEBCAMFORIPHONEHELPER_API CWebcamForIPhoneHelper {
public:
	CWebcamForIPhoneHelper(void);
	// TODO: �ڴ�������ķ�����
};

extern "C" __declspec(dllexport) int CapturePictrue(char* strDefaultPath, char* strPath);

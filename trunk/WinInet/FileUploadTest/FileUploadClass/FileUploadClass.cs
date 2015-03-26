using System;
using System.Collections.Generic;
using System.Text;
using System.Threading;
using System.IO;
namespace FileUploadClass
{
    //-1��ʾ��ȡʧ��
    //-2��ʾ�ļ�������
    public interface IBack
    {
        void NoticeProcess(int iCurr, string strUrl);
    }

    public class FileUpload
    {
        public IBack m_CallBack = null;   //�������ûص�����
        public Thread m_tThread;
        private string m_strPath;
        private static string m_strPostUrl;
        ClassHttpPost m_webPost;
        public FileUpload(IBack obj)
        {
            m_CallBack = obj;
        }
        public void SetUrl(string strUrl)
        {
            m_strPostUrl = strUrl;
        }
        public void Start(string strPath)
        {
            m_strPath = strPath;
            ThreadStart myThreadStart = new ThreadStart(FunProc);
            m_tThread = new Thread(myThreadStart);
            m_tThread.Start();
            if (m_CallBack != null)
            {
                m_CallBack.NoticeProcess(0,"");
            }
        }
        public void Stop()
        {
            if (m_tThread != null)
            {
                m_tThread.Abort();
            }
        }
        private void FunProc()
        {
            //ȡ���ļ���
            //��һ�Σ����ļ�������ȥ��ȡ���������ӵ�ַ�������������ļ����Ƿ���ڣ������ļ���
            //�ڶ��μ���������ʵ�ʷ��͵��ļ�������ȥ�����������ݱ�ʶ�����ļ�ƫ�Ʊ����ļ�            
            string strPath = m_strPath;
            FileStream fs = new FileStream(strPath, FileMode.Open);
            if (fs == null)
            {
                m_CallBack.NoticeProcess(-1,"");
                return;
            }
            string strUrl = "";
            m_webPost = new ClassHttpPost();
            long iTotalLen = fs.Length;
            long iOperatedLen = 0;
            long iMaxReadLen = 102400;
            long iLeftLen = fs.Length;
            int iIndex = strPath.LastIndexOf('\\');
            string strFileName = strPath.Substring(iIndex+1);
            strFileName = "1" + strFileName;
            byte[] bt = System.Text.Encoding.UTF8.GetBytes(strFileName);
            string strRet = CaculateFile(bt);
            if (strRet.IndexOf("ok") < 0)
            {
                //ʧ��
                m_CallBack.NoticeProcess(-2, "");
            }
            strUrl = strRet.Substring(strRet.IndexOf(',') + 1);
            while (iLeftLen > 0)
            {
                int iRead = (int)(iLeftLen > iMaxReadLen ? iMaxReadLen : iLeftLen);
                iRead++;
                byte[] bData = new byte[iRead];
                int iOffset = iRead - 1;
                fs.Read(bData, 1, iOffset);
                bData[0] = (byte)'2';
                strRet = CaculateFile(bData);
                if (strRet != "0")
                {
                    m_CallBack.NoticeProcess(-2, "");
                    break;
                }
                iOperatedLen += iOffset;
                iLeftLen -= iOffset;
                if (m_CallBack != null) m_CallBack.NoticeProcess((int)(100 * iOperatedLen / iTotalLen), strUrl);
            }
            fs.Close();
        }

        private string CaculateFile(byte[] bData)
        {
            return m_webPost.UpLoadFile(m_strPostUrl, bData);
            
            string strFlag = System.Text.Encoding.UTF8.GetString(bData, 0, 1);
            if (strFlag == "1")
            {
                string strName = System.Text.Encoding.UTF8.GetString(bData, 1, bData.Length - 1);
            }
            else
            {
                FileStream streamFile = new FileStream("c:\\test.dat", FileMode.OpenOrCreate);
                streamFile.Seek(0, SeekOrigin.End);
                streamFile.Write(bData, 1, bData.Length-1);
                streamFile.Dispose();
            }
            return "1";
        }
    }
}

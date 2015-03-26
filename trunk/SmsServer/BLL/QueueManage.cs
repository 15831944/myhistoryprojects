using System;
using System.Data;
using System.Messaging;
namespace SmsServer.BLL
{
    public class QueueManage<T>
    {
        /// 
        /// ���Ͷ��󵽶����� 
        /// 
        /// �������ƣ���Ϊ����������һ��Ӧ����Ӧ�ò��ı�ģ��������д�������ļ��� 
        /// Ҫ����ȥ�Ķ��� 
        public static void SendQueue(string strPath, T sq)
        {
            EnsureQueueExists(strPath);
            MessageQueue mqSend = new MessageQueue(strPath, false);            
            mqSend.Send(sq);
        }

        /// 
        /// �����У�������в����ڣ����� 
        /// 
        /// �������� 
        public static void EnsureQueueExists(string strPath)
        {
            if (!MessageQueue.Exists(strPath))
            {
                MessageQueue.Create(strPath);
                MessageQueue mqTemp = new MessageQueue(strPath);
                mqTemp.SetPermissions("Everyone", MessageQueueAccessRights.FullControl);
            }
        }

        /// 
        /// �Ӷ�����ȡ�������б� 
        /// 
        /// �������� 
        public static System.Collections.ArrayList GetMessage(string strPath)
        {
            MessageQueue mq = new MessageQueue(strPath, false);
            mq.Formatter = new XmlMessageFormatter(new Type[] { typeof(T) });
            Message[] arrM = mq.GetAllMessages();
            mq.Close();            
            System.Collections.ArrayList al = new System.Collections.ArrayList();
            foreach (Message m in arrM)
            {
                al.Add(m.Body);
            }
            return al;
        }

        //��ȡָ����Ϣ���еĳ���
        public static int GetQueueLen(string strPath)
        {
            MessageQueue mq = new MessageQueue(strPath, false);
            int iNum = 0;
            MessageEnumerator msgEnum = mq.GetMessageEnumerator2();
            mq.Close();
            while(msgEnum.MoveNext())
            {
                iNum++;
            }            
            return iNum;
        }
        //�ж϶��г����Ƿ񵽴�һ������
        public static bool IsOverSelectLen(string strPath,int iLength)
        {
            try
            {
                MessageQueue mq = new MessageQueue(strPath, false);
                int iNum = 0;
                MessageEnumerator msgEnum = mq.GetMessageEnumerator2();
                mq.Close();
                while (msgEnum.MoveNext())
                {
                    iNum++;
                    if (iNum >= iLength)
                    {
                        return true;
                    }
                }
                return false;
            }
            catch (System.Exception e)
            {
                return false;
            }
           
        }
        //��ȡһ����Ϣ
        public static T GetSingleMessage(string strPath)
        {
            //SmsLogTemp.WriteDataBaseLog(strPath);
            MessageQueue mq = new MessageQueue(strPath, false);
            mq.Formatter = new XmlMessageFormatter(new Type[] { typeof(T) });
            try
            {
                Message messageObj = mq.Receive(new TimeSpan(0, 0,0,SmsSetIni.ReadQueueTimeout,0));  //10��Ķ�ȡ��ʱ����
                mq.Close();
                return (T)messageObj.Body;
            }
            catch(Exception ee)
            {
                //SmsLogTemp.WriteDataBaseLog(ee.Message.ToString());
                mq.Close();
                return default(T);
            }
        }
    }
}
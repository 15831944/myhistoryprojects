using System;
using System.Data;
using System.Text;
using NBearLite;//�����������
using NBear.Mapping;
namespace SmsServer.DAL
{
	/// <summary>
	/// ���ݷ�����SmsTaskB��
	/// </summary>
	public class SmsTaskB
	{
		public SmsTaskB()
		{}
		#region  ��Ա����

        /// <summary>
        /// ����һ������
        /// </summary>
        public int Add(SmsServer.Model.SmsTaskB model)
        {
            return 0;
        }
        /// <summary>
        /// ����һ������
        /// </summary>
        public void Update(SmsServer.Model.SmsTaskB model)
        {

        }

        /// <summary>
        /// ɾ��һ������
        /// </summary>
        public void Delete(long ID)
        {

        }
        /// <summary>
        /// �õ�һ������ʵ��
        /// </summary>
        public SmsServer.Model.SmsTaskB GetModel(long ID)
        {

            return null;
        }

        /// <summary>
        /// ��������б�
        /// </summary>
        public DataSet GetList(string strWhere)
        {
            return null;
        }

        /// <summary>
        /// ���ǰ��������
        /// </summary>
        public DataSet GetList(int Top, string strWhere, string filedOrder)
        {
            return null;
        }

		#endregion  ��Ա����
	}
}


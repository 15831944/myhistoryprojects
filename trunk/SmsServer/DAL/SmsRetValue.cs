using System;
using System.Data;
using System.Text;
using NBearLite;//�����������
using NBear.Mapping;
namespace SmsServer.DAL
{
	/// <summary>
	/// ���ݷ�����SmsRetValue��
	/// </summary>
	public class SmsRetValue
	{
		public SmsRetValue()
		{}
		#region  ��Ա����

		/// <summary>
		/// �Ƿ���ڸü�¼
		/// </summary>
		public bool Exists(string strSpgate,string strRet)
		{
            return false;
		}

		/// <summary>
		/// ����һ������
		/// </summary>
		public int Add(SmsServer.Model.SmsRetValue model)
		{
            return 0;
		}
		/// <summary>
		/// ����һ������
		/// </summary>
		public void Update(SmsServer.Model.SmsRetValue model)
		{
			
		}

		/// <summary>
		/// ɾ��һ������
		/// </summary>
		public void Delete(int ID)
		{
			
		}

		/// <summary>
		/// �õ�һ������ʵ��
		/// </summary>
		public SmsServer.Model.SmsRetValue GetModel(int ID)
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
		public DataSet GetList(int Top,string strWhere,string filedOrder)
		{
            return null;
		}
		#endregion  ��Ա����
	}
}


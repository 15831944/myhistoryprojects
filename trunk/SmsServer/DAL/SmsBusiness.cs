using System;
using System.Data;
using System.Text;
using NBearLite;//�����������
namespace SmsServer.DAL
{
	/// <summary>
	/// ���ݷ�����SmsBusiness��
	/// </summary>
	public class SmsBusiness
	{
		public SmsBusiness()
		{}
		#region  ��Ա����

        public static DataSet GetBusinessList()
        {
            return MyDataBase.db.Select(SmsPlatForm.SmsBusiness, SmsPlatForm.SmsBusiness.BusinessID, SmsPlatForm.SmsBusiness.BusinessName).ToDataSet();
        }
		#endregion  ��Ա����
	}
}


using System;
using System.Data;
using System.Collections.Generic;
using SmsServer.Model;
namespace SmsServer.BLL
{
	/// <summary>
	/// ҵ���߼���SmsBusiness ��ժҪ˵����
	/// </summary>
	public class SmsBusiness
	{
		public SmsBusiness()
		{}
		#region  ��Ա����

		/// <summary>
		/// �õ����ID
		/// </summary>
        public static string GetBusinessList()
        {
            DataSet ds = SmsServer.DAL.SmsBusiness.GetBusinessList();
            return JSonClass.DataTableToJsonEx(ds.Tables[0], ds.Tables[0].Rows.Count);
        }
		#endregion  ��Ա����
	}
}


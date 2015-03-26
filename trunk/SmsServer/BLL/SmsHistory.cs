using System;
using System.Data;
using System.Collections.Generic;
using SmsServer.Model;
namespace SmsServer.BLL
{
	/// <summary>
	/// ҵ���߼���SmsHistory ��ժҪ˵����
	/// </summary>
	public class SmsHistory
	{
		private readonly SmsServer.DAL.SmsHistory dal=new SmsServer.DAL.SmsHistory();
		public SmsHistory()
		{}

		#region  ��Ա����

		/// <summary>
		/// ����һ������
		/// </summary>
		public bool Add(Model.SmsTaskA model,int iResult,string strSpMsgID,string strStatusStr)
		{
            model.PhoneList = model.PhoneList.Substring(0, model.PhoneList.Length - 1);
            model.FeeNum = model.MobileNum * model.SingleMobileFee +
                            model.UnicomNum * model.SingleUnicomFee +
                            model.TelecomNum * model.SingleTelecomFee +
                            model.ChinaNum * model.SingleChinaFee;
            return dal.Add(model, iResult,strSpMsgID, strStatusStr);
		}
		/// <summary>
		/// ɾ��һ������
		/// </summary>
		public void Delete(long ID)
		{
			
			dal.Delete(ID);
		}
		public string GetHistoryList(int iStart,int iLimit)
        {
            int iTotalCount = 0;
            DataSet ds = dal.GetHistoryList(iStart, iLimit, ref iTotalCount);
            return JSonClass.DataTableToJsonEx(ds.Tables[0], iTotalCount);
        }
		#endregion  ��Ա����
	}
}


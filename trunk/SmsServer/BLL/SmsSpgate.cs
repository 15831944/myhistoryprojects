using System;
using System.Data;
using System.Collections.Generic;
using SmsServer.Model;
using Newtonsoft.Json;
using System.Collections;
using System.Text;
namespace SmsServer.BLL
{
	/// <summary>
	/// ҵ���߼���SmsSpgate ��ժҪ˵����
	/// </summary>
	public class SmsSpgate
	{
		private readonly SmsServer.DAL.SmsSpgate dal=new SmsServer.DAL.SmsSpgate();
		public SmsSpgate()
		{}
		#region  ��Ա����
		/// <summary>
		/// �Ƿ���ڸü�¼
		/// </summary>
		public bool Exists(string GateNumber)
		{
			return dal.Exists(GateNumber);
		}
        public bool ExistsEx(string GateNumber)
        {
            string strSql = string.Format("select count(*) from SmsSpgate where GateNumber = '{0}'",GateNumber);
            int iRet = dal.RetCount(strSql);
            return iRet > 0 ? true : false;
        }
		/// <summary>
		/// ����һ������
		/// </summary>
        public bool Add(string strJson, out string strout)
		{
            SmsServer.Model.SmsSpgate model = (SmsServer.Model.SmsSpgate)JavaScriptConvert.DeserializeObject(strJson, typeof(SmsServer.Model.SmsSpgate));
            if (dal.ExistsExEx(model))
            {
                strout = "ͨ���Ż�ͨ�����Ѵ���";
                return false;
            }
            strout = "���ͨ��ʧ��";
            //�Ѳ����ֶ�ƥ���            
            UpdateSendParams(model);
            return dal.Add(model);
		}

		/// <summary>
        /// ����һ������
		/// </summary>
		public bool Update(string strJson,out string strout)
		{
            SmsServer.Model.SmsSpgate model = (SmsServer.Model.SmsSpgate)JavaScriptConvert.DeserializeObject(strJson, typeof(SmsServer.Model.SmsSpgate));
            if (dal.ExistsEx(model))
            {
                strout = "ͨ�����Ѿ�����";
                return false;
            }
            strout = "����ͨ��ʧ��";
            //�Ѳ����ֶ�ƥ���
            UpdateSendParams(model);            
            bool bRet = dal.Update(model);
            if (bRet)
            {
                DataCache.DeleteCacheSpgate(model.GateNumber);
            }
            //�������
            return bRet;
		}

		/// <summary>
		/// ɾ����������
		/// </summary>
		public bool Delete(string[] strArray)
		{
            bool bRet = dal.Delete(strArray);
            //�������
            if (bRet)
            {
                foreach (string strNumber in strArray)
                {
                    DataCache.DeleteCacheSpgate(strNumber);
                }
            }
            return bRet;
		}
        /// <summary>
        /// 
        /// </summary>
        /// <param name="strSpgate"></param>
        /// <returns></returns>
        public string GetSelectSpgate(string strSpgate)
        {
            DataSet ds = dal.GetSelectSpgate(strSpgate);
            if (ds.Tables[0].Rows.Count > 0)
            {
                string strJson = "{success:true,data:";
                strJson += JSonClass.DataTableToJson(ds.Tables[0]) + "}";
                return strJson;
            }
            else
            {
                return "{success:true,errorInfo:'�޴�ͨ������'}";
            }
        }
        /// <summary>
        /// 
        /// </summary>
        /// <param name="iStart"></param>
        /// <param name="iLimit"></param>
        /// <param name="iType"></param>
        /// <param name="iBusiness"></param>
        /// <param name="strSpgate"></param>
        /// <returns></returns>
        public string GetSpgateList(int iStart, int iLimit, int iType,int iBusiness, string strSpgate)
        {
            int iTotalCount = 0;
            DataSet ds = dal.GetList(iStart, iLimit, iType,iBusiness, strSpgate, ref iTotalCount);
            //�Ƴ������ֶ�
            int iCount =  ds.Tables[0].Columns.Count;
            for (int i=16;i<iCount;i++)
            {
                ds.Tables[0].Columns.RemoveAt(16);
            }            
            return JSonClass.DataTableToJsonEx(ds.Tables[0], iTotalCount);
        }
        /// <summary>
        /// 
        /// </summary>
        /// <param name="strUserID"></param>
        /// <returns></returns>
        public string GetSpgate(string strUserID)
        {
            DataSet ds = dal.GetSpgate(strUserID);
            foreach (DataRow row in ds.Tables[0].Rows)
            {
                row["name"] = row["name"].ToString() + row["value"] + "[" + ReturnPortString(row["gatetype"].ToString()) + "]";
            }           
            return JSonClass.DataTableToJsonEx(ds.Tables[0], ds.Tables[0].Rows.Count);
        }
        /// <summary>
        /// 
        /// </summary>
        /// <param name="strUserID"></param>
        /// <returns></returns>
        public string GetSpgateEx(string strUserID)
        {
            string strSpgates = string.Empty;
            DataSet ds = dal.GetSpgateEx(strUserID, out strSpgates);
            string[] strArray = strSpgates.Split(',');
            Hashtable powerDt = new Hashtable();
            for (int i = 0; i < strArray.Length; i++)
            {
                powerDt.Add(strArray[i], 1);
            }
            int iCount = ds.Tables[0].Rows.Count;
            ds.Tables[0].Columns.Add("checked");
            foreach (DataRow row in ds.Tables[0].Rows)
            {
                if (powerDt.Contains(row["id"].ToString()))
                {
                    row["checked"] = 1;
                }
                else
                {
                    row["checked"] = 0;
                }
                row["text"] = row["text"].ToString() + ReturnPortString(row["gatetype"].ToString()) + "[" + row["id"] + "]";
            }
            ds.Tables[0].Columns.Remove("gatetype");
            Hashtable hashDt = new Hashtable();
            hashDt.Add(3, 1);
            return JSonClass.DataTableToJson(ds.Tables[0], hashDt);
        }
        /// <summary>
        /// 
        /// </summary>
        /// <param name="strSpgate"></param>
        /// <returns></returns>
        public string GetSpgateFields(string strSpgate)
        {
             
            DataSet ds = dal.GetSpgateFields(strSpgate);
            if (ds.Tables[0].Rows.Count > 0)
            {                
                string strJson = "{success:true,data:";
                strJson += JSonClass.DataTableToJson(ds.Tables[0]) + "}";
                return strJson;
            }
            else
            {
                return "{success:true,errorInfo:'�޴�ͨ������'}";
            }
        }
        
		/// <summary>
		/// �õ�һ������ʵ��
		/// </summary>
		public Model.SmsSpgate GetModel(string GateNumber)
		{
            //�ȴӻ�����ȡ��ȡ���򷵻أ�ȡ����������ݿ���ȡ
            return dal.GetModel(GateNumber);
		}	

		/// <summary>
		/// ��������б�
		/// </summary>
		public DataSet GetList(string strWhere)
		{
			return dal.GetList(strWhere);
		}
		/// <summary>
		/// ���ǰ��������
		/// </summary>
		public DataSet GetList(int Top,string strWhere,string filedOrder)
		{
			return dal.GetList(Top,strWhere,filedOrder);
		}
        private string ReturnPortString(string strType)
        {
            switch (strType)
            {
                case "1":
                    return "ȫ��";
                case "2":
                    return "����";
                case "3":
                    return "�ƶ�";
                case "4":
                    return "��ͨ";
                case "5":
                    return "����";
                case "6":
                    return "С��ͨ";
                default:
                    return "δ֪�Ŷ�";
            }
        }
        /// <summary>
        /// ����ͨ���Ĳ����ַ���
        /// </summary>
        /// <param name="model"></param>
        private void UpdateSendParams(SmsServer.Model.SmsSpgate model)
        {
            StringBuilder objBuilder = new StringBuilder();
            objBuilder.Append(model.UseridParamString + "=" + model.UserID + "&");
            objBuilder.Append(model.PwdParamString + "=" + model.UserPwd + "&");
            objBuilder.Append(model.MobileParamString + "={0}&");
            objBuilder.Append(model.MsgParamString + "={1}&");
            objBuilder.Append(model.PhoneNumParamString + "={2}&");
            objBuilder.Append(model.SubportParamString + "={3}&");
            objBuilder.Append(model.GateNoParamString + "={4}");
            model.SendParams = objBuilder.ToString();
//             model.UseridParamString = "";
//             model.PwdParamString = "";
//             model.UserID = "";
//             model.UserPwd = "";
        }
		#endregion  ��Ա����
	}
}


using System;
using System.Data;
using System.Collections.Generic;
using SmsServer.Model;
using Newtonsoft.Json;
namespace SmsServer.BLL
{
	/// <summary>
	/// ҵ���߼���SmsManager ��ժҪ˵����
	/// </summary>
	public class SmsManager
	{
		private readonly SmsServer.DAL.SmsManager dal=new SmsServer.DAL.SmsManager();
		public SmsManager()
		{}
		#region  ��Ա����
		/// <summary>
		/// �Ƿ���ڸü�¼
		/// </summary>
		public bool Exists(string ManagerID)
		{
			return dal.Exists(ManagerID);
		}
        public bool ManagerLogin(string strID,string strPwd)
        {
            Model.SmsManager objMgr = new Model.SmsManager();
            objMgr.ManagerID = strID;
            objMgr.ManagerPwd = strPwd;
            return dal.Exists(objMgr);
        }
        /// <summary>
        /// �ж��Ƿ�����Ӧ��Ȩ��
        /// </summary>
        /// <param name="strPower"></param>
        /// <param name="iPower"></param>
        /// <returns></returns>
        public static int HasPower(string strFile,object objSession,out string strout)
        {
            try
            {
                if (object.Equals(objSession, null))
                {
                    strout = "δ��¼���¼��ʱ";
                    return -1;
                }
                string strManagerID = objSession.ToString();
                if (SmsServer.DAL.SmsManager.HasPower(strFile, strManagerID))
                {
                    strout = "";
                    return 0;
                }
                strout = "�Ƿ�����";
                return -2;      //��Ȩ��
            }
            catch (System.Exception e)
            {
                strout = "�������쳣";
                return -2;
            }
            
        }
		/// <summary>
		/// ����һ������
		/// </summary>
		public bool Add(string strJson,out string strout)
		{
            SmsServer.Model.SmsManager model = (SmsServer.Model.SmsManager)JavaScriptConvert.DeserializeObject(strJson, typeof(SmsServer.Model.SmsManager));
            if (dal.Exists(model.ManagerID))
            {
                strout = "���˺��Ѵ���,����������ID��";
                return false;
            }
            strout = "���ʧ��";
            return dal.Add(model);
		}

		/// <summary>
		/// ����һ������
		/// </summary>
		public bool Update(string strJson)
		{
            SmsServer.Model.SmsManager model = (SmsServer.Model.SmsManager)JavaScriptConvert.DeserializeObject(strJson, typeof(SmsServer.Model.SmsManager));            
			return dal.Update(model);
		}
        /// <summary>
        /// ɾ����������
        /// </summary>
        public bool Delete(string[] ManagerIDArray)
        {
            return dal.Delete(ManagerIDArray);
        }
		/// <summary>
		/// ɾ��һ������
		/// </summary>
		public bool Delete(string ManagerID)
		{
			return dal.Delete(ManagerID);
		}
        /// <summary>
        /// ����Ȩ��
        /// </summary>
        /// <param name="strUserID"></param>
        /// <param name="strPower"></param>
        /// <returns></returns>
        public bool SetPower(string strUserID, string strPower)
        {
            return dal.SetPower(strUserID, strPower);
        }
        /// <summary>
        /// �޸Ĺ���Ա����
        /// </summary>
        /// <param name="strJson"></param>
        /// <returns></returns>
        public bool ModifyPwd(string strJson,string strUserID,out string strErrorInfo)
        {
            SmsServer.Model.SmsManager model = (SmsServer.Model.SmsManager)JavaScriptConvert.DeserializeObject(strJson, typeof(SmsServer.Model.SmsManager));
            model.ManagerID = strUserID;
            //�ж������Ƿ���ȷ
            if (!dal.Exists(model))
            {
                strErrorInfo = "ԭ�������,�޸�ʧ��";
                return false;
            }
            strErrorInfo = "�޸�����ʧ��";
            return dal.ModifyPwd(model);
        }
        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        public string GetManagerList(int iStart,int iLimit,int iType,string strUserID)
        {
            int iTotalCount = 0;
            DataSet ds = dal.GetList(iStart, iLimit, iType, strUserID, ref iTotalCount);
            return JSonClass.DataTableToJsonEx(ds.Tables[0], iTotalCount);
        }
		/// <summary>
		/// �õ�һ������ʵ��
		/// </summary>
		public SmsServer.Model.SmsManager GetModel(string ManagerID,int ID)
		{
			
			return dal.GetModel(ManagerID,ID);
		}
        /// <summary>
        /// ���ع���ԱJSON����
        /// </summary>
        /// <param name="strUserID"></param>
        /// <returns></returns>
        public string GetSelectManager(string strUserID)
        {
            DataSet ds = dal.GetSelectManager(strUserID);
            ds.Tables[0].Columns.Remove("ManagerPwd");
            string strJson = "{success:true,data:";
            strJson += JSonClass.DataTableToJson(ds.Tables[0]) + "}";
            return strJson;
        }
        public bool SetSpgates(string strUserID, string strPorts)
        {
            return dal.SetSpgates(strUserID, strPorts);
        }
		/// <summary>
		/// �õ�һ������ʵ�壬�ӻ����С�
		/// </summary>
		public SmsServer.Model.SmsManager GetModelByCache(string ManagerID,int ID)
		{
            return null;
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
		/// <summary>
		/// ��������б�
		/// </summary>
		public List<SmsServer.Model.SmsManager> GetModelList(string strWhere)
		{
			DataSet ds = dal.GetList(strWhere);
			return DataTableToList(ds.Tables[0]);
		}
		/// <summary>
		/// ��������б�
		/// </summary>
		public List<SmsServer.Model.SmsManager> DataTableToList(DataTable dt)
		{
			List<SmsServer.Model.SmsManager> modelList = new List<SmsServer.Model.SmsManager>();
			int rowsCount = dt.Rows.Count;
			if (rowsCount > 0)
			{
				SmsServer.Model.SmsManager model;
				for (int n = 0; n < rowsCount; n++)
				{
					model = new SmsServer.Model.SmsManager();
					if(dt.Rows[n]["ID"].ToString()!="")
					{
						model.ID=int.Parse(dt.Rows[n]["ID"].ToString());
					}
					model.ManagerID=dt.Rows[n]["ManagerID"].ToString();
					model.ManagerPwd=dt.Rows[n]["ManagerPwd"].ToString();
					if(dt.Rows[n]["ManagerType"].ToString()!="")
					{
						model.ManagerType=int.Parse(dt.Rows[n]["ManagerType"].ToString());
					}
					model.Power=dt.Rows[n]["Power"].ToString();
					model.UserName=dt.Rows[n]["UserName"].ToString();
					model.CompanyName=dt.Rows[n]["CompanyName"].ToString();
					model.Phone=dt.Rows[n]["Phone"].ToString();
					model.Address=dt.Rows[n]["Address"].ToString();
					model.ExtraInfo=dt.Rows[n]["ExtraInfo"].ToString();
					modelList.Add(model);
				}
			}
			return modelList;
		}

		/// <summary>
		/// ��������б�
		/// </summary>
		public DataSet GetAllList()
		{
			return GetList("");
		}

		/// <summary>
		/// ��������б�
		/// </summary>
		//public DataSet GetList(int PageSize,int PageIndex,string strWhere)
		//{
			//return dal.GetList(PageSize,PageIndex,strWhere);
		//}

		#endregion  ��Ա����
	}
}


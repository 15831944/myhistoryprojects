using System;
using System.Data;
using System.Collections.Generic;

using SmsServer.Model;
namespace SmsServer.BLL
{
	/// <summary>
	/// ҵ���߼���SmsLoginLog ��ժҪ˵����
	/// </summary>
	public class SmsLoginLog
	{
		private readonly SmsServer.DAL.SmsLoginLog dal=new SmsServer.DAL.SmsLoginLog();
		public SmsLoginLog()
		{}
		#region  ��Ա����
		/// <summary>
		/// �Ƿ���ڸü�¼
		/// </summary>
		public bool Exists(long ID)
		{
		//	return dal.Exists(ID);
            return false;
        }

		/// <summary>
		/// ����һ������
		/// </summary>
		public int  Add(SmsServer.Model.SmsLoginLog model)
		{
			return dal.Add(model);
		}

		/// <summary>
		/// ����һ������
		/// </summary>
		public void Update(SmsServer.Model.SmsLoginLog model)
		{
			dal.Update(model);
		}

		/// <summary>
		/// ɾ��һ������
		/// </summary>
		public void Delete(long ID)
		{
			
			dal.Delete(ID);
		}

		/// <summary>
		/// �õ�һ������ʵ��
		/// </summary>
		public SmsServer.Model.SmsLoginLog GetModel(long ID)
		{
			
			return dal.GetModel(ID);
		}

		/// <summary>
		/// �õ�һ������ʵ�壬�ӻ����С�
		/// </summary>
		public SmsServer.Model.SmsLoginLog GetModelByCache(long ID)
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
		public List<SmsServer.Model.SmsLoginLog> GetModelList(string strWhere)
		{
			DataSet ds = dal.GetList(strWhere);
			return DataTableToList(ds.Tables[0]);
		}
		/// <summary>
		/// ��������б�
		/// </summary>
		public List<SmsServer.Model.SmsLoginLog> DataTableToList(DataTable dt)
		{
			List<SmsServer.Model.SmsLoginLog> modelList = new List<SmsServer.Model.SmsLoginLog>();
			int rowsCount = dt.Rows.Count;
			if (rowsCount > 0)
			{
				SmsServer.Model.SmsLoginLog model;
				for (int n = 0; n < rowsCount; n++)
				{
					model = new SmsServer.Model.SmsLoginLog();
					//model.ID=dt.Rows[n]["ID"].ToString();
					if(dt.Rows[n]["LoginTime"].ToString()!="")
					{
						model.LoginTime=DateTime.Parse(dt.Rows[n]["LoginTime"].ToString());
					}
					model.LoginIp=dt.Rows[n]["LoginIp"].ToString();
					model.LoginUser=dt.Rows[n]["LoginUser"].ToString();
					model.LoginPwd=dt.Rows[n]["LoginPwd"].ToString();
					model.LoginResult=dt.Rows[n]["LoginResult"].ToString();
					if(dt.Rows[n]["LoginType"].ToString()!="")
					{
						model.LoginType=int.Parse(dt.Rows[n]["LoginType"].ToString());
					}
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


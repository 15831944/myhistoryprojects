using System;
using System.Data;
using System.Collections.Generic;
using SmsServer.Model;
namespace SmsServer.BLL
{
	/// <summary>
	/// ҵ���߼���SmsOprateLog ��ժҪ˵����
	/// </summary>
	public class SmsOprateLog
	{
		private readonly SmsServer.DAL.SmsOprateLog dal=new SmsServer.DAL.SmsOprateLog();
		public SmsOprateLog()
		{}
		#region  ��Ա����
		/// <summary>
		/// �Ƿ���ڸü�¼
		/// </summary>
		public bool Exists(long ID)
		{
            return false;
		}

		/// <summary>
		/// ����һ������
		/// </summary>
		public int  Add(SmsServer.Model.SmsOprateLog model)
		{
			return dal.Add(model);
		}

		/// <summary>
		/// ����һ������
		/// </summary>
		public void Update(SmsServer.Model.SmsOprateLog model)
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
		public SmsServer.Model.SmsOprateLog GetModel(long ID)
		{
			
			return dal.GetModel(ID);
		}

		/// <summary>
		/// �õ�һ������ʵ�壬�ӻ����С�
		/// </summary>
		public SmsServer.Model.SmsOprateLog GetModelByCache(long ID)
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
		public List<SmsServer.Model.SmsOprateLog> GetModelList(string strWhere)
		{
			DataSet ds = dal.GetList(strWhere);
			return DataTableToList(ds.Tables[0]);
		}
		/// <summary>
		/// ��������б�
		/// </summary>
		public List<SmsServer.Model.SmsOprateLog> DataTableToList(DataTable dt)
		{
			List<SmsServer.Model.SmsOprateLog> modelList = new List<SmsServer.Model.SmsOprateLog>();
			int rowsCount = dt.Rows.Count;
			if (rowsCount > 0)
			{
				SmsServer.Model.SmsOprateLog model;
				for (int n = 0; n < rowsCount; n++)
				{
					model = new SmsServer.Model.SmsOprateLog();
					//model.ID=dt.Rows[n]["ID"].ToString();
					model.OpraterType=dt.Rows[n]["OpraterType"].ToString();
					model.OpraterID=dt.Rows[n]["OpraterID"].ToString();
					model.OprationInfo=dt.Rows[n]["OprationInfo"].ToString();
					if(dt.Rows[n]["OprateTime"].ToString()!="")
					{
						model.OprateTime=DateTime.Parse(dt.Rows[n]["OprateTime"].ToString());
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


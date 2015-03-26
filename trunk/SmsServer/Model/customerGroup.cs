using System;
namespace SmsServer.Model
{
	/// <summary>
	/// ʵ����customerGroup ��(����˵���Զ���ȡ���ݿ��ֶε�������Ϣ)
	/// </summary>
	[Serializable]
	public class customerGroup
	{
		public customerGroup()
		{}
		#region Model
		private int _group_id;
		private int _detail_id;
		private string _ref_user;
		private string _group_name;
		private string _group_desc;
		private string _remark;
		/// <summary>
		/// Ⱥ��ID
		/// </summary>
		public int group_id
		{
			set{ _group_id=value;}
			get{return _group_id;}
		}
		/// <summary>
		/// ID
		/// </summary>
		public int detail_id
		{
			set{ _detail_id=value;}
			get{return _detail_id;}
		}
		/// <summary>
		/// ���õ��û�
		/// </summary>
		public string ref_user
		{
			set{ _ref_user=value;}
			get{return _ref_user;}
		}
		/// <summary>
		/// Ⱥ������
		/// </summary>
		public string group_name
		{
			set{ _group_name=value;}
			get{return _group_name;}
		}
		/// <summary>
		/// Ⱥ������
		/// </summary>
		public string group_desc
		{
			set{ _group_desc=value;}
			get{return _group_desc;}
		}
		/// <summary>
		/// ��ע
		/// </summary>
		public string remark
		{
			set{ _remark=value;}
			get{return _remark;}
		}
		#endregion Model

	}
}


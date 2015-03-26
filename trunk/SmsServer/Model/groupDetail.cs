using System;
namespace SmsServer.Model
{
	/// <summary>
	/// ʵ����groupDetail ��(����˵���Զ���ȡ���ݿ��ֶε�������Ϣ)
	/// </summary>
	[Serializable]
	public class groupDetail
	{
		public groupDetail()
		{}
		#region Model
		private int _detail_id;
		private int _group_id;
		private string _iphone;
		private string _iname;
		private string _idesc;
		private string _remark;
		/// <summary>
		/// ID
		/// </summary>
		public int detail_id
		{
			set{ _detail_id=value;}
			get{return _detail_id;}
		}
		/// <summary>
		/// ����Ⱥ��ID
		/// </summary>
		public int group_id
		{
			set{ _group_id=value;}
			get{return _group_id;}
		}
		/// <summary>
		/// Ⱥ���Ա�ֻ�����
		/// </summary>
		public string iphone
		{
			set{ _iphone=value;}
			get{return _iphone;}
		}
		/// <summary>
		/// Ⱥ���Ա����
		/// </summary>
		public string iname
		{
			set{ _iname=value;}
			get{return _iname;}
		}
		/// <summary>
		/// Ⱥ���Ա����
		/// </summary>
		public string idesc
		{
			set{ _idesc=value;}
			get{return _idesc;}
		}
		/// <summary>
		/// �����ֶ�
		/// </summary>
		public string remark
		{
			set{ _remark=value;}
			get{return _remark;}
		}
		#endregion Model

	}
}


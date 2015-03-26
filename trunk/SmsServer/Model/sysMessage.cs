using System;
namespace SmsServer.Model
{
	/// <summary>
	/// ʵ����sysMessage ��(����˵���Զ���ȡ���ݿ��ֶε�������Ϣ)
	/// </summary>
	[Serializable]
	public class sysMessage
	{
		public sysMessage()
		{}
		#region Model
		private int _msg_id;
		private int _msg_type;
		private string _msg_title;
		private string _msg_content;
		private DateTime _pub_date;
		private string _publisher;
		private DateTime? _avi_date;
		private string _recivers;
		/// <summary>
		/// ��ϢID
		/// </summary>
		public int msg_id
		{
			set{ _msg_id=value;}
			get{return _msg_id;}
		}
		/// <summary>
		/// ��Ϣ����(0:���桢1:֪ͨ)
		/// </summary>
		public int msg_type
		{
			set{ _msg_type=value;}
			get{return _msg_type;}
		}
		/// <summary>
		/// ��Ϣ����
		/// </summary>
		public string msg_title
		{
			set{ _msg_title=value;}
			get{return _msg_title;}
		}
		/// <summary>
		/// ��Ϣ����
		/// </summary>
		public string msg_content
		{
			set{ _msg_content=value;}
			get{return _msg_content;}
		}
		/// <summary>
		/// ��������
		/// </summary>
		public DateTime pub_date
		{
			set{ _pub_date=value;}
			get{return _pub_date;}
		}
		/// <summary>
		/// ������
		/// </summary>
		public string publisher
		{
			set{ _publisher=value;}
			get{return _publisher;}
		}
		/// <summary>
		/// ��Ч����
		/// </summary>
		public DateTime? avi_date
		{
			set{ _avi_date=value;}
			get{return _avi_date;}
		}
		/// <summary>
		/// ������
		/// </summary>
		public string recivers
		{
			set{ _recivers=value;}
			get{return _recivers;}
		}
		#endregion Model

	}
}


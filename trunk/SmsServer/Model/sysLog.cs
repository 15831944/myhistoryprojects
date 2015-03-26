using System;
namespace SmsServer.Model
{
	/// <summary>
	/// ʵ����sysLog ��(����˵���Զ���ȡ���ݿ��ֶε�������Ϣ)
	/// </summary>
	[Serializable]
	public class sysLog
	{
		public sysLog()
		{}
		#region Model
		private int _log_id;
		private int _log_type;
		private string _log_detail;
		private DateTime _log_time;
		private string _operatorid;
		private string _op_module;
		/// <summary>
		/// ��־ID
		/// </summary>
		public int log_id
		{
			set{ _log_id=value;}
			get{return _log_id;}
		}
		/// <summary>
		/// ��־����(0:�쳣��־��1:������־)
		/// </summary>
		public int log_type
		{
			set{ _log_type=value;}
			get{return _log_type;}
		}
		/// <summary>
		/// ��־����
		/// </summary>
		public string log_detail
		{
			set{ _log_detail=value;}
			get{return _log_detail;}
		}
		/// <summary>
		/// ��¼ʱ��
		/// </summary>
		public DateTime log_time
		{
			set{ _log_time=value;}
			get{return _log_time;}
		}
		/// <summary>
		/// ������
		/// </summary>
		public string operatorID
		{
			set{ _operatorid=value;}
			get{return _operatorid;}
		}
		/// <summary>
		/// ����ģ��
		/// </summary>
		public string op_module
		{
			set{ _op_module=value;}
			get{return _op_module;}
		}
		#endregion Model

	}
}


using System;
namespace SmsServer.Model
{
	/// <summary>
	/// ʵ����SmsRetValue ��(����˵���Զ���ȡ���ݿ��ֶε�������Ϣ)
	/// </summary>
	[Serializable]
	public class SmsRetValue
	{
		public SmsRetValue()
		{}
		#region Model
		private int _id;
		private string _spgateno;
		private string _retstring;
		private int _mappingstatus;
		private int _matchtype;
		private int _needresend;
		private string _errorinfo;
		/// <summary>
		/// 
		/// </summary>
		public int ID
		{
			set{ _id=value;}
			get{return _id;}
		}
		/// <summary>
		/// 
		/// </summary>
		public string SpgateNo
		{
			set{ _spgateno=value;}
			get{return _spgateno;}
		}
		/// <summary>
		/// 
		/// </summary>
		public string RetString
		{
			set{ _retstring=value;}
			get{return _retstring;}
		}
		/// <summary>
		/// 
		/// </summary>
		public int MappingStatus
		{
			set{ _mappingstatus=value;}
			get{return _mappingstatus;}
		}
		/// <summary>
		/// 
		/// </summary>
		public int MatchType
		{
			set{ _matchtype=value;}
			get{return _matchtype;}
		}
		/// <summary>
		/// 
		/// </summary>
		public int NeedReSend
		{
			set{ _needresend=value;}
			get{return _needresend;}
		}
		/// <summary>
		/// 
		/// </summary>
		public string ErrorInfo
		{
			set{ _errorinfo=value;}
			get{return _errorinfo;}
		}
		#endregion Model

	}
}


using System;
namespace SmsServer.Model
{
	/// <summary>
	/// ʵ����MapScriptDetail ��(����˵���Զ���ȡ���ݿ��ֶε�������Ϣ)
	/// </summary>
	[Serializable]
	public class MapScriptDetail
	{
		public MapScriptDetail()
		{}
		#region Model
		private long _id;
		private string _mapid;
		private int _maptype;
		private string _srcvalue;
		private int _mapretvalue;
		private int _retusevalue;
		private string _retstring;
		private int? _charnum;
		private string _splitchar;
		/// <summary>
		/// 
		/// </summary>
		public long id
		{
			set{ _id=value;}
			get{return _id;}
		}
		/// <summary>
		/// 
		/// </summary>
		public string MapID
		{
			set{ _mapid=value;}
			get{return _mapid;}
		}
		/// <summary>
		/// ƥ�����[1:����;2:С��;3:����;4:����;5:��߼�λ����;6�ұ߼�λ����;7:��߼�λ������;8:�ָ�����ߵ�������;9:�ָ�����ߵ���������]
		/// </summary>
		public int MapType
		{
			set{ _maptype=value;}
			get{return _maptype;}
		}
		/// <summary>
		/// �õ��жϵıȽ�ֵ
		/// </summary>
		public string SrcValue
		{
			set{ _srcvalue=value;}
			get{return _srcvalue;}
		}
		/// <summary>
		/// ӳ��������ϲ�ķ���ֵ
		/// </summary>
		public int MapRetValue
		{
			set{ _mapretvalue=value;}
			get{return _mapretvalue;}
		}
		/// <summary>
		/// �ж��Ƿ���Ҫ�ط��ķ���ֵ
		/// </summary>
		public int RetUseValue
		{
			set{ _retusevalue=value;}
			get{return _retusevalue;}
		}
		/// <summary>
		/// �������ϲ����ϸ������Ϣ
		/// </summary>
		public string RetString
		{
			set{ _retstring=value;}
			get{return _retstring;}
		}
		/// <summary>
		/// ��߻����ұ�ȡ��λ��
		/// </summary>
		public int? CharNum
		{
			set{ _charnum=value;}
			get{return _charnum;}
		}
		/// <summary>
		/// �ָ��ַ���
		/// </summary>
		public string SplitChar
		{
			set{ _splitchar=value;}
			get{return _splitchar;}
		}
		#endregion Model

	}
}


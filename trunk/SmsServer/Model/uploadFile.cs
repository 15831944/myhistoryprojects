using System;
namespace SmsServer.Model
{
	/// <summary>
	/// ʵ����uploadFile ��(����˵���Զ���ȡ���ݿ��ֶε�������Ϣ)
	/// </summary>
	[Serializable]
	public class uploadFile
	{
		public uploadFile()
		{}
		#region Model
		private string _dir_name;
		private string _dir_alias;
		private DateTime _upload_date;
		private string _uploader;
		private string _mobile_file;
		private int _mobile_num;
		private string _unicom_file;
		private int _unicom_num;
		private string _telecom_file;
		private int _telecom_num;
		private string _china_file;
		private int _china_num;
		/// <summary>
		/// ʵ�ʴ���ڷ������ϵ��ļ�������
		/// </summary>
		public string dir_name
		{
			set{ _dir_name=value;}
			get{return _dir_name;}
		}
		/// <summary>
		/// �ͻ��������ļ��б���
		/// </summary>
		public string dir_alias
		{
			set{ _dir_alias=value;}
			get{return _dir_alias;}
		}
		/// <summary>
		/// �ϴ�ʱ��
		/// </summary>
		public DateTime upload_date
		{
			set{ _upload_date=value;}
			get{return _upload_date;}
		}
		/// <summary>
		/// �ϴ���
		/// </summary>
		public string uploader
		{
			set{ _uploader=value;}
			get{return _uploader;}
		}
		/// <summary>
		/// �ƶ��洢·��
		/// </summary>
		public string mobile_file
		{
			set{ _mobile_file=value;}
			get{return _mobile_file;}
		}
		/// <summary>
		/// �ƶ���������
		/// </summary>
		public int mobile_num
		{
			set{ _mobile_num=value;}
			get{return _mobile_num;}
		}
		/// <summary>
		/// ��ͨ�洢·��
		/// </summary>
		public string unicom_file
		{
			set{ _unicom_file=value;}
			get{return _unicom_file;}
		}
		/// <summary>
		/// ��ͨ��������
		/// </summary>
		public int unicom_num
		{
			set{ _unicom_num=value;}
			get{return _unicom_num;}
		}
		/// <summary>
		/// ���Ŵ洢·��
		/// </summary>
		public string telecom_file
		{
			set{ _telecom_file=value;}
			get{return _telecom_file;}
		}
		/// <summary>
		/// ���ź�������
		/// </summary>
		public int telecom_num
		{
			set{ _telecom_num=value;}
			get{return _telecom_num;}
		}
		/// <summary>
		/// С��ͨ�洢·��
		/// </summary>
		public string china_file
		{
			set{ _china_file=value;}
			get{return _china_file;}
		}
		/// <summary>
		/// С��ͨ��������
		/// </summary>
		public int china_num
		{
			set{ _china_num=value;}
			get{return _china_num;}
		}
		#endregion Model

	}
}


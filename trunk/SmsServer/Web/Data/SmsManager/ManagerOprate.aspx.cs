using System;
using System.Data;
using System.Configuration;
using System.Collections;
using System.Web;
using System.Web.Security;
using System.Web.UI;
using System.Web.UI.WebControls;
using System.Web.UI.WebControls.WebParts;
using System.Web.UI.HtmlControls;

namespace SmsServer.Web.Data.SmsManager
{
    public partial class ManagerOprate : System.Web.UI.Page
    {
        SmsServer.BLL.SmsManager manager = new SmsServer.BLL.SmsManager();
        protected void Page_Load(object sender, EventArgs e)
        {
            //Ȩ�޼��
            string strout = string.Empty;
            if (SmsServer.BLL.SmsManager.HasPower(GetType().Name, Session["admin"], out strout) != 0)
            {
                Response.Write("{success:false,errorInfo:'" + strout + "'}");
                return;
            }
            //��һ��type ���ж������,�༭,ɾ��
            try
            {
                if (object.Equals(null, Request.Form["type"]))  //û�����Ϳ��Ʋ���
                {
                    Response.Write("{success:false,errorInfo:'�Ƿ�����'}");
                    return;
                }
                string strType = Request.Form["type"].ToString();
                if (strType == "0")     //���
                {
                    AddManager();
                    return;
                }
                else if (strType == "1")    //�༭
                {
                    ModifyManager();
                    return;
                }
                else if (strType == "2")    //ɾ��
                {
                    DeleteUser();
                    return;
                }
                else if (strType == "3")    //��ȡָ������Ա����Ϣ
                {
                    GetManagerData();
                }
                else
                {
                    Response.Write("{success:false,errorInfo:'�Ƿ�����'}");
                    return;
                }
            }
            catch (System.Exception ee)
            {
                Response.Write("{success:false,errorInfo:'�������쳣'}");
                return;
            }
        }
        /// <summary>
        /// ɾ��ѡ�й���Ա
        /// </summary>
        public void DeleteUser()
        {
            if (object.Equals(null, Request.Form["userid"]))
            {
                Response.Write("{success:false,errorInfo:'�Ƿ�����'}");
                return;
            }
            string strUserIDList = Request.Form["userid"].ToString();
            int iIndex = strUserIDList.LastIndexOf(',');
            if (iIndex > 0)
            {
                strUserIDList = strUserIDList.Substring(0, iIndex);
            }
            else
            {
                Response.Write("{success:false,errorInfo:'δѡ��ɾ����'}");
                return;
            }
            string[] strArray = strUserIDList.Split(',');
            //������ݿ�
            if (manager.Delete(strArray))
            {
                Response.Write("{success:true}");
            }
            else //ʧ��
            {
                Response.Write("{success:false,errorInfo:'ɾ��ʧ��'}");
            }     
        }
        /// <summary>
        /// ��ȡ����Ա��Ϣ
        /// </summary>
        public void GetManagerData()
        {
            if (object.Equals(null, Request.Form["userid"]))
            {
                Response.Write("{success:false,errorInfo:'�Ƿ�����'}");
                return;
            }
            string strUserID = Request.Form["userid"].ToString();
            if (strUserID == "")
            {
                Response.Write("{success:false,errorInfo:'�Ƿ�����'}");
                return;
            }
            string strTest =  manager.GetSelectManager(strUserID);
            Response.Write(strTest);
        }
        /// <summary>
        /// �޸��˻���Ϣ
        /// </summary>
        public void ModifyManager()
        {
            if (object.Equals(null, Request.Form["jdata"]))
            {
                Response.Write("{success:false,errorInfo:'�Ƿ�����'}");
                return;
            }
            string strJson = Request.Form["jdata"].ToString();
            if (manager.Update(strJson))
            {
                Response.Write("{success:true}");
            }
            else //ʧ��
            {
                Response.Write("{success:false,errorInfo:'�޸�ʧ��'}");
            }
        }
        /// <summary>
        /// ����˻���Ϣ
        /// </summary>
        public void AddManager()
        {
            if (object.Equals(null, Request.Form["jdata"]))
            {
                Response.Write("{success:false,errorInfo:'�Ƿ�����'}");
                return;
            }
            string strJson = Request.Form["jdata"].ToString();
            string strout = "";
            if (manager.Add(strJson,out strout))
            {
                Response.Write("{success:true}");
            }
            else //ʧ��
            {
                Response.Write("{success:false,errorInfo:'" + strout + "'}");
            }
        }
        
    }
}

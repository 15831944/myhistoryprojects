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
    public partial class PowerSet : System.Web.UI.Page
    {
        protected void Page_Load(object sender, EventArgs e)
        {
            //Ȩ�޼��
            string strout = string.Empty;
            if (SmsServer.BLL.SmsManager.HasPower(GetType().Name, Session["admin"], out strout) != 0)
            {
                Response.Write("{success:false,errorInfo:'" + strout + "'}");
                return;
            }

            //��һ��type ���ж��ǻ�ȡֵ��������ֵ
            try
            {
                if (object.Equals(null, Request.Form["type"]))  //û�����Ϳ��Ʋ���
                {
                    Response.Write("{success:false,errorInfo:'�Ƿ�����'}");
                    return;
                }
                string strType = Request.Form["type"].ToString();
                if (strType == "1")     //����
                {
                    SetPower();
                    return;
                }
                else if (strType == "0")    //��ȡ
                {
                    GetPowerTree();
                    return;
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
        /// ��ȡȨ��
        /// </summary>
        public void GetPowerTree()
        {
            int iID = 0;
            string strUserID = "";
            if (object.Equals(null, Request.Form["id"]))
            {
                Response.Write("{success:false}");
                return;
            }
            else if (object.Equals(null, Request.Form["userid"]))
            {
                Response.Write("{success:false}");
                return;
            }
            else
            {
                try
                {
                    string strID = Request.Form["id"].ToString();
                    iID = Int32.Parse(strID);
                    strUserID = Request.Form["userid"].ToString();
                }
                catch
                {
                    Response.Write("{success:false}");
                    return;
                }
            }
            SmsServer.BLL.TreeNode node = new SmsServer.BLL.TreeNode();
            string strJson = node.GetTreeNodeEx(strUserID, iID);
            strJson = strJson.Replace("\"leaf\":1", "\"leaf\":true");
            strJson = strJson.Replace("\"leaf\":0", "\"leaf\":false");
            strJson = strJson.Replace("\"checked\":1", "\"checked\":true");
            strJson = strJson.Replace("\"checked\":0", "\"checked\":false");
            Response.Write(strJson);
        }
        /// <summary>
        /// ����Ȩ��
        /// </summary>
        public void SetPower()
        {
            if (object.Equals(null, Request.Form["power"]) ||
                object.Equals(null, Request.Form["userid"]) 
                )
            {
                Response.Write("{success:false,errorInfo:'�Ƿ�����'}");
                return;
            }
            string strPower = Request.Form["power"].ToString();
            string strUserID = Request.Form["userid"].ToString();
            int iIndex = strPower.LastIndexOf(',');
            if (iIndex > 0)
            {
                strPower = strPower.Substring(0, iIndex);
            }
            //������ݿ�
            SmsServer.BLL.SmsManager manager = new SmsServer.BLL.SmsManager();
            if (manager.SetPower(strUserID, strPower))
            {
                Response.Write("{success:true}");                
            }
            else //ʧ��
            {
                Response.Write("{success:false,errorInfo:'��̨����'}");
            }           
        }
    }
}

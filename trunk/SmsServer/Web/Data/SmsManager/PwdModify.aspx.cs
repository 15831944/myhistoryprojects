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
    public partial class PwdModify : System.Web.UI.Page
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

            try
            {
                if (object.Equals(null, Request.Form["jdata"]))  //û�����Ϳ��Ʋ���
                {
                    Response.Write("{success:false,errorInfo:'�Ƿ�����'}");
                    return;
                }
                string strJson = Request.Form["jdata"].ToString();
                strJson = strJson.Replace("ReNewManagerPwd", "ExtraInfo");
                string strErrorInfo = string.Empty;
                SmsServer.BLL.SmsManager bll = new SmsServer.BLL.SmsManager();
                if (bll.ModifyPwd(strJson, Session["admin"].ToString(), out strErrorInfo))
                {
                    Response.Write("{success:true}");
                }
                else
                {
                    Response.Write("{success:false,errorInfo:'" + strErrorInfo + "'}");
                }
            }
            catch (System.Exception ee)
            {
                Response.Write("{success:false,errorInfo:'�������쳣'}");
                return;
            }
        }
    }
}

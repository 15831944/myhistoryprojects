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

namespace SmsServer.Web.Data.SmsSpgate
{
    public partial class SpgateType_Bus_List : System.Web.UI.Page
    {
        protected void Page_Load(object sender, EventArgs e)
        {
            if (object.Equals(null, Request.Form["type"]))  //û�����Ϳ��Ʋ���
            {
               // Response.Write("{success:false,errorInfo:'�Ƿ�����'}");
                //return;
            }
            string strType = "1";
            //string strType = Request.Form["type"].ToString();
            if (strType == "1")     //����
            {
                GetBusinessList();
                return;
            }
            else if (strType == "2")    //֧�ֵĺŶ�
            {
               // ModifySpgate();
                return;
            }
            else if (strType == "2")    //����������ͨ���б�
            {
                //DeleteSpgate();
                return;
            }
        }
        private void GetBusinessList()
        {
            try
            {
                string strOut =SmsServer.BLL.SmsBusiness.GetBusinessList();
                Response.Write(strOut);
            }
            catch (System.Exception e)
            {
                Response.Write("{success:false,errorInfo:'�������쳣'}");
            }
            
        }
    }
}

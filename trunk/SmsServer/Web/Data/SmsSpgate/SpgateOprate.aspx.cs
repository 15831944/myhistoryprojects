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
    public partial class SpgateOprate : System.Web.UI.Page
    {
        SmsServer.BLL.SmsSpgate bll = new SmsServer.BLL.SmsSpgate();
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
                    AddSpgate();
                    return;
                }
                else if (strType == "1")    //�༭
                {
                    ModifySpgate();
                    return;
                }
                else if (strType == "2")    //ɾ��
                {
                    DeleteSpgate();
                    return;
                }
                else if (strType == "3")    //��ȡָ��ͨ������Ϣ
                {
                    GetSpgateData();
                    return;
                }
                else if (strType == "4")    //��ȡָ������Ա�����ͨ��
                {
                    GetManagerSpgates();
                    return;
                }
                else if (strType == "5")    //��ȡָ��ͨ��������
                {
                    GetSelectSpgateFields();
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
        /// ��ȡָ�������̵�ͨ����Ϣ�б�
        /// </summary>
        public void GetManagerSpgates()
        {
            SmsServer.BLL.SmsSpgate bll = new SmsServer.BLL.SmsSpgate();
            string strJson = bll.GetSpgate(Session["admin"].ToString());
            Response.Write(strJson);
        }
        /// <summary>
        /// ��ȡָ��ͨ��������
        /// </summary>
        public void GetSelectSpgateFields()
        {
            if (object.Equals(null, Request.Form["spgateno"]))
            {
                Response.Write("{success:false,errorInfo:'�Ƿ�����'}");
                return;
            }
            string strSpgate= Request.Form["spgateno"].ToString();
            SmsServer.BLL.SmsSpgate bll = new SmsServer.BLL.SmsSpgate();
            string strJson = bll.GetSpgateFields(strSpgate);
            Response.Write(strJson);
        }
        /// <summary>
        /// ɾ��ѡ�й���Ա
        /// </summary>
        public void DeleteSpgate()
        {
            if (object.Equals(null, Request.Form["spgateno"]))
            {
                Response.Write("{success:false,errorInfo:'�Ƿ�����'}");
                return;
            }
            string strUserIDList = Request.Form["spgateno"].ToString();
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
            if (bll.Delete(strArray))
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
        public void GetSpgateData()
        {
            if (object.Equals(null, Request.Form["spgate"]))
            {
                Response.Write("{success:false,errorInfo:'�Ƿ�����'}");
                return;
            }
            string strSpgate = Request.Form["spgate"].ToString();
            if (strSpgate == "")
            {
                Response.Write("{success:false,errorInfo:'�Ƿ�����'}");
                return;
            }
            string strTest = bll.GetSelectSpgate(strSpgate);
            Response.Write(strTest);
        }
        /// <summary>
        /// �޸��˻���Ϣ
        /// </summary>
        public void ModifySpgate()
        {
            if (object.Equals(null, Request.Form["jdata"]))
            {
                Response.Write("{success:false,errorInfo:'�Ƿ�����'}");
                return;
            }
            string strJson = Request.Form["jdata"].ToString();
            string strOut = string.Empty;
            if (bll.Update(strJson, out strOut))
            {
                Response.Write("{success:true}");
            }
            else //ʧ��
            {
                Response.Write("{success:false,errorInfo:'" + strOut + "'}");
            }
        }
        /// <summary>
        /// ����˻���Ϣ
        /// </summary>
        public void AddSpgate()
        {
            if (object.Equals(null, Request.Form["jdata"]))
            {
                Response.Write("{success:false,errorInfo:'�Ƿ�����'}");
                return;
            }
            string strJson = Request.Form["jdata"].ToString();
            string strout = "";
            if (bll.Add(strJson,out strout))
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

using System;
using System.Collections.Generic;
using System.Text;
using NBearLite;//�����������
namespace SmsServer.DAL
{
    class MyDataBase
    {
        public static Database db = new Database("ConnString");
    }
}

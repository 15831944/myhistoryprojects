#ifndef DATA_TYPE_H
#define DATA_TYPE_H
#include <list>
#include "tinyxml/tinyxmlEx.h"
#include "../Util/Function.h"
using std::list;
/*
<?xml version="1.0" encoding="UTF-8" ?> 
- <!--  XML�ĵ�����Ϊ meeting.xml 
--> 
- <!--  updatetimeΪ��XML�ĵ�����ʱ�䣬 versionΪ�汾�ţ�ĿǰΪ"V1.0" 
--> 
- <root updatetime="2011-07-01 13:00:00" version="V1.0">
- <!--  boxnos��¼��box�µĻ�����Ϣ����Ҫ�·����ն˱�ţ��������ն˱����"-"���ӣ����������ն˱����";"���� 
--> 
- <box boxnos="BOX00001-BOX00010;BOX00100">
- <!--  ÿ�������¼Ϊһ��record 
-->  
- <record>
- <!--  �����ұ�� 
--> 
<roomid>0001</roomid> 
- <!--  ���������� 
--> 
- <roomname>
- <![CDATA[ ������A
]]> 
</roomname>
- <!--  ���鿪ʼʱ�� 
--> 
<starttime>10:00</starttime> 
- <!--  �������ʱ�� 
--> 
<endtime>11:00</endtime> 
- <!--  ��������
--> 
- <topic>
- <![CDATA[ ACM2010������̳
]]> 
</topic>
- <!--  ���������� 
--> 
<chairman>С��</chairman> 
- <!--  ����Ԥ���� 
--> 
<bookpeople>С��</bookpeople> 
- <!--  ���������Ա 
--> 
- <peoples>
+ <people>
- <![CDATA[ ����A
]]> 
</people>
+ <people>
- <![CDATA[ ����B
]]> 
</people>
</peoples>
- <!--  �û������Ƿ����ڵ�ǰ¥��, ֵΪtrue��false 
--> 
<currentfloor>true</currentfloor> 
- <!--  ��ʾ��ʽ 
--> 
+ <displayformat>
- <!--  �Ƿ�����Ļ��ͻ����ʾ, ֵΪtrue��false 
--> 
<ishighlight>true</ishighlight> 
</displayformat>
</record>
- <!--  �޻�����Ϣ��ʱ��Σ�Record�������� 
--> 
- <record>
<roomId>0001</roomId> 
- <roomname>
- <![CDATA[ ������A
]]> 
</roomname>
<starttime>11:00</starttime> 
<endtime>11:30</endtime> 
+ <topic>
- <![CDATA[ �����ҿ���
]]> 
</topic>
<chairman /> 
<bookpeople /> 
<peoples /> 
<currentfloor>true</currentfloor> 
- <displayformat>
<ishighlight>false</ishighlight> 
</displayformat>
</record>
</box>
</root>
*/

typedef struct _RoomInfo
{
	CString RoomName;
	_RoomInfo()
	{
		RoomName = "";
	}
}RoomInfo;

typedef struct _ConferenceSource 
{
	CString ConfId;
	CString ConfName;
	CString StartTime;
	CString EndTime;
	CString Department;
	RoomInfo Room;
	CString ParticipantNumber;
	CString ConfState;
	CString ConfType;
	_ConferenceSource()
	{
		ConfId.Empty();
		ConfName.Empty();
		StartTime.Empty();
		EndTime.Empty();
		Department.Empty();
		ParticipantNumber.Empty();
		ConfState.Empty();
		ConfType.Empty();
	}
}ConferenceSource;

typedef list<ConferenceSource> ConferenceSourceList;

typedef struct _DisplayFormat
{
	bool ishighlight;
}DisplayFormat;
//record
typedef struct _ConferenceRecord 
{
	CString roomid;
	CString roomname;
	CString starttime;
	CString endtime;
	CString topic;
	CString chairman;
	CString bookpeople;
	list<CString> peoples;
	bool currentfloor;
	DisplayFormat displayformat;
	_ConferenceRecord()
	{
		roomid.Empty();
		roomname.Empty();
		starttime.Empty();
		endtime.Empty();
		topic.Empty();
		chairman.Empty();
		bookpeople.Empty();
		peoples.clear();
		currentfloor = false;
	}
}ConferenceRecord;

//conference
class ConferenceInfo
{
public:
	ConferenceInfo()
	{
		boxnos.Empty();
		version.Empty();
		updatetime.Empty();
		record.clear();
	}
	bool ToXml(const char* szPath);
	bool SetRecord(TiXmlElement* boxElement, list<ConferenceRecord>::iterator dataRecord);

	CString boxnos;
	CString version;
	CString updatetime;
	list<ConferenceRecord> record;	
};
#endif
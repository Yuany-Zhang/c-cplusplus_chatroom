#include "Config.h"
#define  _CRT_SECURE_NO_WARNINGS

//��̬��Ա��ֵ
Config* Config::m_instance = NULL;

//���캯��
Config::Config()
{

}

//��������
Config::~Config()
{
	std::vector<LPConfigItem>::iterator pos;
	for (pos = m_ConfigItemList.begin(); pos != m_ConfigItemList.end(); ++pos)
	{
		delete (*pos);
	}
	m_ConfigItemList.clear();
}

//װ�������ļ�
bool Config::Load(const char* pconfName)
{
	FILE *fp;
	fp = fopen(pconfName, "r");
	if (fp == NULL)
		return false;

	//ÿһ�������ļ��������ŵ�����
	char linebuf[501];
	while(!feof(fp))
	{
		if (fgets(linebuf, 500, fp) == NULL)
			continue;
		if (linebuf[0] == 0)
			continue;
		if (*linebuf == ';' || *linebuf == ' ' || *linebuf == '#' || *linebuf == '\t' || *linebuf == '\n')
			continue;

	lblprocstring:
		if (strlen(linebuf) > 0)
		{
			if (linebuf[strlen(linebuf) - 1] == 10
				|| linebuf[strlen(linebuf) - 1] == 13
				|| linebuf[strlen(linebuf) - 1] == 32)
			{
				linebuf[strlen(linebuf) - 1] = 0;
				goto lblprocstring;
			}
		}
		if (linebuf[0] == 0)
			continue;
		if (*linebuf == '[')  //[��ͷ��Ҳ������
			continue;

		//���ߵ��������ģ�������д�Ϸ���������
		char *ptmp = strchr(linebuf, '=');
		if (ptmp != NULL)
		{
			LPConfigItem p_configitem = new ConfigItem;
			memset(p_configitem, 0, sizeof(ConfigItem));
			//�Ⱥ����Ŀ�����p_configitem->ItemName
			strncpy(p_configitem->ItemName, linebuf, (int)(ptmp - linebuf));
			//�Ⱥ��Ҳ�Ŀ�����p_configitem->ItemContent
			strcpy(p_configitem->ItemContent, ptmp + 1);

			Rtrim(p_configitem->ItemName);
			Ltrim(p_configitem->ItemName);
			Rtrim(p_configitem->ItemContent);
			Ltrim(p_configitem->ItemContent);

			m_ConfigItemList.push_back(p_configitem);  //�ڴ�Ҫ�ͷţ���Ϊ�����new������
		} //end if
	} //end while(!feof(fp))

	fclose(fp);  //��Ҫ���ǹر��ļ�
	return true;
}

//����ItemName��ȡ������Ϣ�ַ���
const char* Config::GetString(const char *p_itemname)
{
	std::vector<LPConfigItem>::iterator pos;
	for (pos = m_ConfigItemList.begin(); pos != m_ConfigItemList.end(); ++pos)
	{
		if (StrcaseCmp((*pos)->ItemName, p_itemname))
			return (*pos)->ItemContent;
	}
	return NULL;
}

//����ItemName��ȡ��������������Ϣ
int Config::GetIntDefault(const char*p_itemname, const int def)
{
	std::vector<LPConfigItem>::iterator pos;
	for (pos = m_ConfigItemList.begin(); pos != m_ConfigItemList.end(); ++pos)
	{
		if (StrcaseCmp((*pos)->ItemName, p_itemname) )
		{
			return atoi((*pos)->ItemContent);
		}
	}
	return def;
}
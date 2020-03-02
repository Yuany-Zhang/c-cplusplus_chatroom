#include "Config.h"
#define  _CRT_SECURE_NO_WARNINGS

//静态成员赋值
Config* Config::m_instance = NULL;

//构造函数
Config::Config()
{

}

//析构函数
Config::~Config()
{
	std::vector<LPConfigItem>::iterator pos;
	for (pos = m_ConfigItemList.begin(); pos != m_ConfigItemList.end(); ++pos)
	{
		delete (*pos);
	}
	m_ConfigItemList.clear();
}

//装载配置文件
bool Config::Load(const char* pconfName)
{
	FILE *fp;
	fp = fopen(pconfName, "r");
	if (fp == NULL)
		return false;

	//每一行配置文件读出来放到这里
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
		if (*linebuf == '[')  //[开头的也不处理
			continue;

		//能走到这里来的，都是书写合法的配置项
		char *ptmp = strchr(linebuf, '=');
		if (ptmp != NULL)
		{
			LPConfigItem p_configitem = new ConfigItem;
			memset(p_configitem, 0, sizeof(ConfigItem));
			//等号左侧的拷贝到p_configitem->ItemName
			strncpy(p_configitem->ItemName, linebuf, (int)(ptmp - linebuf));
			//等号右侧的拷贝到p_configitem->ItemContent
			strcpy(p_configitem->ItemContent, ptmp + 1);

			Rtrim(p_configitem->ItemName);
			Ltrim(p_configitem->ItemName);
			Rtrim(p_configitem->ItemContent);
			Ltrim(p_configitem->ItemContent);

			m_ConfigItemList.push_back(p_configitem);  //内存要释放，因为这里的new出来的
		} //end if
	} //end while(!feof(fp))

	fclose(fp);  //不要忘记关闭文件
	return true;
}

//根据ItemName获取配置信息字符串
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

//根据ItemName获取数字类型配置信息
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
#ifndef _Config_h_
#define _Config_h_


//包含头文件
#include "Func.h"
#include "Global.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <iterator>
#include <math.h>





class Config {
private:
	Config();
	static Config* m_instance;

public:
	~Config();
	static Config* GetInstance()
	{
		if (m_instance == NULL)
		{
			if (m_instance == NULL)
			{
				m_instance = new Config();
				static CGarhuishou cl;
			}
		}
		return m_instance;
	}
	class CGarhuishou  //类中套类，用于释放对象
	{
	public:
		~CGarhuishou()
		{
			if (Config::m_instance)
			{
				delete Config::m_instance;
				Config::m_instance = NULL;
			}
		}
	};

	bool Load(const char* pconfName); //装载配置文件
	const char *GetString(const char* p_itemname);
	int GetIntDefault(const char* p_itemname, const int def);

	std::vector<LPConfigItem> m_ConfigItemList;  //存储配置信息的列表
		//存的是指针， LPConfigItem是结构指针
};


#endif // !_Config_h_

#ifndef _Config_h_
#define _Config_h_


//����ͷ�ļ�
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
	class CGarhuishou  //�������࣬�����ͷŶ���
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

	bool Load(const char* pconfName); //װ�������ļ�
	const char *GetString(const char* p_itemname);
	int GetIntDefault(const char* p_itemname, const int def);

	std::vector<LPConfigItem> m_ConfigItemList;  //�洢������Ϣ���б�
		//�����ָ�룬 LPConfigItem�ǽṹָ��
};


#endif // !_Config_h_

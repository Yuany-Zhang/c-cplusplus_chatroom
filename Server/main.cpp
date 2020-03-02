#include "Func.h"
#include "Config.h"
#include "Server.h"






int main()
{
#ifdef _WIN32
	system("chcp 65001");
	system("cls");
#endif // _WIN32

	Config *p_config = Config::GetInstance();
	if (p_config->Load("config.conf") == false)
	{
		printf("配置文件读取失败\n");
		exit(0);
	}
	const char *ip = p_config->GetString("ip");
	const int port = p_config->GetIntDefault("port", 9876);
	const int any = p_config->GetIntDefault("anyaddr",0);
	p_config->~Config();

	Server chatServer;
	chatServer.InitSocket();
	chatServer.Bind_Listen(ip, port, (const bool)any);
	//chatServer.Accept();
	while (true)
		chatServer.OnRun();
	//chatServer.PrintUser();
	//chatServer.Close();

	getchar();

	return 0;
}
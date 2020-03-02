#include "Client.h"
#include "Config.h"
#include "Func.h"
#include "Global.h"
#include <thread>
#include <iostream>

extern unsigned int _thisClientId;  //当前客户端在服务器中的id
extern int _thisClientSocket;  //当前客户端的socket
extern char _thisClientName[32];  //当前客户端用户名
extern std::vector<LPUserList> _userList;
extern int _thisChatClientId ;  //
extern char _thisChatClientName[32] ;
bool isLogin = false;
char cname[32] = "";
extern std::mutex mu;
void cmdThread(Client* client)
{

	while (true)
	{
		char cmdBuf[8192] = {};
		char content[8192] = {};
		std::cin >> cmdBuf;
		if (0 == strcmp(cmdBuf, "__logout"))
		{
			printf("<socket=%d,userName=%s,serverId=%d>退出\n",
				_thisClientSocket, _thisClientName, _thisClientId);
			Logout logout;
			logout.userId = _thisClientId;
			strcpy(logout.userName, _thisClientName);
			client->SendData(&logout);
			break;
		}
		else if (0 == strcmp(cmdBuf, "__broadcast"))
		{
			printf("向所有服务器广播,请输入要广播的内容\n");
			std::cin >> content;
			Broadcast _broadcast;
			_broadcast._userId = _thisClientId;
			strcpy(_broadcast._userName, _thisClientName);
			strcpy(_broadcast._content, content);
			client->SendData(&_broadcast);
			printf("广播结束\n");
			if (_thisChatClientId)
			{
				printf("-->%s:", _thisChatClientName);
			}
			else
			{
				printf("NULL:");
			}
		}
		else if (0 == strcmp(cmdBuf, "__change"))
		{
			printf("请输入想要和谁聊天,输入对方的服务器编号\n");
			unsigned int _u = 0;
			char name[32] = "";
			std::cin >> _u;
			ChangeUser _changeuser;

			std::vector<LPUserList>::iterator iter = _userList.begin();
			for (; iter != _userList.end(); ++iter)
			{
				if ((*iter)->_userId == _u)
					break;
			}
			if (iter != _userList.end())
			{
				strcpy(name, (*iter)->_userName);
				strcpy(_thisChatClientName, name);
				_changeuser.userId = _u;
				_thisChatClientId = _u;
				strcpy(_changeuser.userName, name);
				client->SendData(&_changeuser);
				strcpy(_thisChatClientName, name);
				printf("已经把聊天对象改为<serverId=%d,userName=%s>\n", _u, name);
				if (_thisChatClientId)
				{
					printf("-->%s:", _thisChatClientName);
				}
				else
				{
					printf("NULL:");
				}
			}
			else
			{
				printf("抱歉,没有该用户\n");
			}
		}
		else if (0 == strcmp(cmdBuf, "__getlist"))
		{
			GetUserList _geu;
			client->SendData(&_geu);
			//mu.lock();
			//printf("打印客户端列表\n");
			//std::vector<LPUserList>::iterator ubegin = _userList.begin();
			//std::vector<LPUserList>::iterator uend = _userList.end();
			//std::vector<LPUserList>::iterator iter = _userList.begin();
			//for (iter = ubegin; iter != uend; ++iter)
			//{
			//	if ((*iter)->_userId == _thisClientId)
			//		printf("-> ");
			//	printf("Name:%s\t\t\tserverId:%d\n", (*iter)->_userName, (*iter)->_userId);
			//}
			//mu.unlock();
			//printf("客户端列表打印完毕\n");
		}
		else if (0 == strcmp(cmdBuf, "__login"))
		{
			if (!isLogin)
			{
				printf("Logining Name=%s\n", _thisClientName);
				Login _login;
				strcpy(_login.userName, _thisClientName);
				strcpy(_login.passWord, "NULL");
				client->SendData(&_login);
				isLogin = true;
				printf("Login Succeed\n");
			}
		}
		else if (strcmp(cmdBuf, "__getorder") == 0)
		{
			printf("\n打印命令\n");
			printf("__getlist\t\t获取客户端列表\n\
__change\t\t更改聊天对象\n\
__broadcast\t\t向所有客户端广播\n\
__logout\t\t向服务器发送注销命令客户端退出\n");
			printf("命令打印完毕\n\n");
		}
		else
		{
			if (_thisChatClientId == 0)
			{
				printf("please choose a user to chat\n");
				continue;
			}
			Msg _msg;
			if (_thisChatClientId)
			{
				printf("-->%s:", _thisChatClientName);
			}
			else
			{
				printf("NULL:");
			}
			_msg._toUserId = _thisChatClientId;
			strcpy(_msg._Msg, cmdBuf);
			strcpy(_msg._userName, _thisClientName);
			client->SendData(&_msg);

		}
	}
}



int main()
{
//#ifdef _WIN32
//	system("chcp 65001");
//	system("cls");
//#endif // _WIN32
	Config* p_config = Config::GetInstance();
	p_config->Load("config.conf");
	const char* ip = p_config->GetString("ip");
	const int port = p_config->GetIntDefault("port", 4567);
	char IP[48] = "";
	strcpy(IP, ip);
	printf("ip地址为:%s\n", ip);
	printf("port端口为:%d\n", port);

	std::cout << "please input a name for you" << std::endl;
	std::cin >> cname;
	strcpy(_thisClientName, cname);

	Client c;
	c.InitSocket();
	c.Connect(IP, (const short)port);
	
	std::thread t1(cmdThread, &c);
	t1.detach();
	while (true)
	{
		c.OnRun();
	}
	getchar();

	return 0;
}
#include "Client.h"
#include "Config.h"
#include "Func.h"
#include "Global.h"
#include <thread>
#include <iostream>

extern unsigned int _thisClientId;  //��ǰ�ͻ����ڷ������е�id
extern int _thisClientSocket;  //��ǰ�ͻ��˵�socket
extern char _thisClientName[32];  //��ǰ�ͻ����û���
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
			printf("<socket=%d,userName=%s,serverId=%d>�˳�\n",
				_thisClientSocket, _thisClientName, _thisClientId);
			Logout logout;
			logout.userId = _thisClientId;
			strcpy(logout.userName, _thisClientName);
			client->SendData(&logout);
			break;
		}
		else if (0 == strcmp(cmdBuf, "__broadcast"))
		{
			printf("�����з������㲥,������Ҫ�㲥������\n");
			std::cin >> content;
			Broadcast _broadcast;
			_broadcast._userId = _thisClientId;
			strcpy(_broadcast._userName, _thisClientName);
			strcpy(_broadcast._content, content);
			client->SendData(&_broadcast);
			printf("�㲥����\n");
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
			printf("��������Ҫ��˭����,����Է��ķ��������\n");
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
				printf("�Ѿ�����������Ϊ<serverId=%d,userName=%s>\n", _u, name);
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
				printf("��Ǹ,û�и��û�\n");
			}
		}
		else if (0 == strcmp(cmdBuf, "__getlist"))
		{
			GetUserList _geu;
			client->SendData(&_geu);
			//mu.lock();
			//printf("��ӡ�ͻ����б�\n");
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
			//printf("�ͻ����б��ӡ���\n");
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
			printf("\n��ӡ����\n");
			printf("__getlist\t\t��ȡ�ͻ����б�\n\
__change\t\t�����������\n\
__broadcast\t\t�����пͻ��˹㲥\n\
__logout\t\t�����������ע������ͻ����˳�\n");
			printf("�����ӡ���\n\n");
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
	printf("ip��ַΪ:%s\n", ip);
	printf("port�˿�Ϊ:%d\n", port);

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
#include "Global.h"
#include "Client.h"
#include <iostream>
std::vector<LPUserList> _userList;  //用户列表【客户端列表】
unsigned int _thisClientId = 0;  //当前客户端在服务器中的id
int _thisClientSocket = INVALID_SOCKET;  //当前客户端的socket
char _thisClientName[32] = "";  //当前客户端用户名
int _thisChatClientId = 0;  //
char _thisChatClientName[32] = "";

bool isNeedUpdateList = false;
extern char cname[32];

std::mutex mu;

Client::Client()
{
	_sock = INVALID_SOCKET;
}

Client::~Client()
{
	Close();
}

void Client::InitSocket()
{
#ifdef _WIN32
	WORD ver = MAKEWORD(2, 2);
	WSADATA dat;
	WSAStartup(ver, &dat);
#endif // _WIN32
	if (_sock != INVALID_SOCKET)
	{
		printf("关闭旧连接<socket=%d>\n", _sock);
		Close();
	}
	_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (_sock == INVALID_SOCKET) {
		printf("socket()初始化函数创建失败\n");
		return;
	}
	else {
		printf("<socket=%d> 创建成功\n", _sock);
	}
	_thisClientSocket = _sock;
	return;
}


int Client::Connect(const char*ip, const short port)
{
	if (_sock == INVALID_SOCKET)
	{
		InitSocket();
	}
	sockaddr_in _sin = {};
	_sin.sin_port = htons(port);
	_sin.sin_family = AF_INET;
#ifdef _WIN32
	_sin.sin_addr.S_un.S_addr = inet_addr(ip);
#else
	_sin.sin_addr.s_addr = inet_addr(ip);
#endif // _WIN32
	int ret = connect(_sock, (const sockaddr*)&_sin, sizeof(_sin));
	if (ret == SOCKET_ERROR)
	{
		printf("connect()函数连接服务器<%s:%d>失败\n", ip, port);
		return ret;
	}
	else
	{
		printf("connec()函数连接服务器<%s:%d>成功\n", ip, port);
		Login _lo;
		strcpy(_lo.passWord, "NULL");
		strcpy(_lo.userName, cname);
		send(_sock, (const char*)&_lo, sizeof(_lo), 0);
		printf("%s\n", _lo.userName);
		char sz[8192] = "";
		recv(_sock, sz, sizeof(_lo), 0);
		Login * _l = (Login*)sz;
		_thisClientId = _l->clientId;
		printf("thisclientid=%d\n", _thisClientId);
		return ret;
	}
	return ret;
}


void Client::Close()
{
	if (_sock == INVALID_SOCKET)
		return;
#ifdef _WIN32
	closesocket(_sock);
	WSACleanup();
#else
	close(_sock);
#endif // _WIN32
	printf("<socket=%d>已经被关闭了\n", _sock);
	_sock = INVALID_SOCKET;
}


bool Client::OnRun()
{
	if (IsRun())
	{
		fd_set fdReads;
		FD_ZERO(&fdReads);
		FD_SET(_sock, &fdReads);
		timeval t = { 1,0 };
		int ret = select(_sock + 1, &fdReads, NULL, NULL, &t);
		if (ret < 0)
		{
			printf("<socket=%d> select function is failed\n", _sock);
			Close();
			return false;
		}
		if (FD_ISSET(_sock, &fdReads))
		{
			FD_CLR(_sock, &fdReads);
			if (-1 == RecvData())
			{
				printf("<socket=%d> select function mission finished\n", _sock);
				return false;
			}
		}
		return true;
	}
	return false;
}


bool Client::IsRun()
{
	return _sock != INVALID_SOCKET;
}

int Client::RecvData()
{
	char szRecv[10240] = {};

	int nLen = recv(_sock, szRecv, sizeof(DataHeader), 0);
	DataHeader *header = (DataHeader*)szRecv;

	if (nLen <= 0)
	{
		printf("<socket=%d> 与服务器断开连接，任务结束\n", _sock);
		return -1;
	}
	recv(_sock, szRecv + sizeof(DataHeader), header->dataLenght - sizeof(DataHeader), 0);
	//处理信息函数
	OnNetMsg(header);
	return 0;
}

//处理接收到的信息
void Client::OnNetMsg(DataHeader * header)
{
	switch (header->cmd)
	{
	case CMD_NEW_USER_JOIN:
	{
		NewUserJoin* newuserjoin = (NewUserJoin*)header;
		printf("新用户加入<Name:%s,ServerId=%d>,欢迎您和Ta聊天\n",
			newuserjoin->userName, newuserjoin->userId);
		LPUserList _newC = new UserList;
		_newC->_userId = newuserjoin->userId;
		strcpy(_newC->_userName, newuserjoin->userName);
		_userList.push_back(_newC);
	}
	break;

	case CMD_LOGOUT:
	{
		Logout* logout = (Logout*)header;
		std::vector<LPUserList>::iterator iter = _userList.begin();
		for (; iter != _userList.end(); ++iter)
		{
			if ((*iter)->_userId == logout->userId)
				break;
		}
		if ((iter) != _userList.end())
		{
			delete(*iter);
			_userList.erase(iter);
		}

		printf("用户<Name:%s>退出了\n", logout->userName);
	}
	break;

	case CMD_MSG:
	{
		Msg* msg = (Msg*)header;
		//printf("【%s<--】:%s\n",msg->_userName, msg->_Msg);
		std::cout << "【" << msg->_userName << "<--】:" << msg->_Msg << std::endl;
	}
	break;

	case CMD_BROADCAST:
	{
		Broadcast* broadcast = (Broadcast*)header;
		printf("< Name:%s,ServerId:%d >广播:【%s】\n",
			broadcast->_userName, broadcast->_userId, broadcast->_content);
	}
	break;

	//每新增一个客户端，其他客户端就受到一个新增客户端列表信息
	case CMD_GET_USER_LIST:
	{
		/*UserList* users = (UserList*)header;
		LPUserList _news = new UserList;
		_news->_userId = users->_userId;
		_news->_uSock = users->_uSock;
		strcpy(_news->_userName, users->_userName);
		_userList.push_back(_news);*/
		GetUserList *uuu = (GetUserList*)header;
		int uNmu = uuu->userNum;
		mu.lock();
		for (int ii = 0; ii < _userList.size(); ++ii)
		{
			delete(_userList[ii]);
		}
		_userList.clear();
		for (int i = 0; i < uNmu; ++i )
		{
			UserList u;
			recv(_sock, (char*)&u, sizeof(u), 0);
			
			UserList* _newUUU = new UserList;
			_newUUU->_chatId = u._chatId;
			_newUUU->_userId = u._userId;
			_newUUU->_uSock = u._uSock;
			strcpy(_newUUU->_userName, u._userName);
			_userList.push_back(_newUUU);
		}
		if (!isNeedUpdateList)
		{
			printf("开始打印客户端列表\n");
			for (int ii = 0; ii < _userList.size(); ++ii)
			{
				printf("serverId:%d\t\t\tuserName:%s", _userList[ii]->_userId, _userList[ii]->_userName);
				if (_userList[ii]->_userId == _thisClientId)
					printf("  <--");
				printf("\n");
			}
			printf("客户端列表打印完毕\n");
		}
		else
		{

		}

		mu.unlock();

		
	}
	break;

	case CMD_LOGIN :
	{
		Login * _log = (Login*)header;
		_thisClientId = _log->clientId;
		printf("%d\n", _thisClientId);
		printf("%d\n", _log->clientId);

	}
	break;

	}
}




//发送数据
int Client::SendData(DataHeader * header)
{
	if (IsRun() && header)
	{
		return send(_sock, (const char*)header, header->dataLenght, 0);
		
	}
	return SOCKET_ERROR;
}
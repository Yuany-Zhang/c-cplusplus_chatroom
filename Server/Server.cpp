#include "Server.h"
#include "Global.h"
#include <iostream>
using namespace std;

unsigned int _userTargetId = 0;   //【用于标识客户端】
std::vector<LPUserList> _userList;  //用户列表【客户端列表】


Server::Server()
{
	_sock = INVALID_SOCKET;
}

Server::~Server()
{
	Close();
}

void Server::InitSocket()
{
#ifdef _WIN32
	WORD ver = MAKEWORD(2, 2);
	WSADATA dat;
	WSAStartup(ver, &dat);
#endif // _WIN32
	//建立一个socket
	if (INVALID_SOCKET != _sock)
	{
		printf("<socket=%d> 关闭旧连接\n", _sock);
		Close();
	}
	_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (INVALID_SOCKET == _sock)
	{
		printf("错误，socket()函数建立失败\n");
	}
	else {
		printf("<socket=%d> 建立成功\n", _sock);
	}
}


//绑定-监听
int Server::Bind_Listen(const char*ip, const short port, const bool anyaddr)
{
	if (ip == NULL && !anyaddr)
		return SOCKET_ERROR;
	sockaddr_in _sin = {};
	_sin.sin_family = AF_INET;
	_sin.sin_port = htons(port);
	if (anyaddr)
	{
	#ifdef _WIN32
		_sin.sin_addr.S_un.S_addr = INADDR_ANY;
	#else
		_sin.sin_addr.s_addr = INADDR_ANY;
	#endif // _WIN32
	}
	else
	{
	#ifdef _WIN32
		_sin.sin_addr.S_un.S_addr = inet_addr(ip);

	#else
		_sin.sin_addr.s_addr = inet_addr(ip);
	#endif // _WIN32
	}

	int ret = bind(_sock, (sockaddr*)&_sin, sizeof(sockaddr_in));
	if (ret == SOCKET_ERROR)
	{
		printf("绑定<ip=%s>端口<port=%d>失败\n",( anyaddr ? "INADDR_ANY" : ip ), port);
		return SOCKET_ERROR;
	}
	else {
		printf("绑定<ip = %s>端口<port = %d>成功\n", (anyaddr ? "INADDR_ANY" : ip), port);
	}

	ret = listen(_sock, 32);
	if (ret == SOCKET_ERROR)
	{
		printf("监听<ip=%s>端口<port=%d>失败\n", (anyaddr ? "INADDR_ANY" : ip), port);
		return SOCKET_ERROR;
	}
	else {
		printf("监听<ip = %s>端口<port = %d>成功\n", (anyaddr ? "INADDR_ANY" : ip), port);
	}
	return 1;
}


void Server::OnRun()
{
	fd_set fdReads;
	fd_set fdWrites;
	fd_set fdExps;
	FD_ZERO(&fdReads);
	FD_ZERO(&fdWrites);
	FD_ZERO(&fdExps);

	FD_SET(_sock, &fdReads);
	FD_SET(_sock, &fdWrites);
	FD_SET(_sock, &fdExps);

	timeval t = { 1,0 };
	int ret = select(_sock + 1, &fdReads, &fdWrites, &fdExps, &t);
	if (ret < 0)
	{
		printf("select function error,mission finished\n");
		//break;
		return;
	}
	if (FD_ISSET(_sock, &fdReads))
	{
		FD_CLR(_sock, &fdReads);
		sockaddr_in clientAdr;
		int nAddrLen = sizeof(sockaddr_in);
		SOCKET _cSock = INVALID_SOCKET;
		
		_cSock = (int)accept(_sock, (sockaddr*)&clientAdr, &nAddrLen);
		if (_cSock == INVALID_SOCKET)
		{
			printf("error, accept function failed\n");
		}
		else
		{
			char _cname[4096] = "";
			int nameRet = recv(_cSock, _cname, sizeof(Login), 0);
			Login* _login = (Login*)_cname;
			for (int n = _userList.size() - 1; n >= 0; n--)
			{
				NewUserJoin _newuerjoin;
				_newuerjoin.userId = _userTargetId + 1;
				strcpy(_newuerjoin.userName, _login->userName);
				send(_userList[n]->_uSock, (const char*)&_newuerjoin, sizeof(_newuerjoin), 0);
				printf("1, accept function failed\n");
			}
			LPUserList _newUs = new UserList;
			_newUs->_uSock = _cSock;
			_newUs->_userId = (++_userTargetId);
			strcpy(_newUs->_userName, _login->userName);
			_userList.push_back(_newUs);
			printf("2, accept function failed\n");
		}
	}

	for (int n = (int)_userList.size() - 1; n >= 0; n--)
	{
		if (FD_ISSET(_userList[n], &fdReads))
		{
			Processor(_userList[n]->_uSock);
		}
	}
	printf("zai chuli qita shiinqg \n");
}


int Server::Processor(SOCKET _csock)
{
	char szRecv[10240] = {};

	int nLen = recv(_csock, szRecv, sizeof(DataHeader), 0);
	DataHeader* header = (DataHeader*)szRecv;
	if (nLen <= 0)
	{
		return -1;
	}
	switch (header->cmd)
	{
	case CMD_CHANGE_USER:
	{
		recv(_csock, szRecv + sizeof(DataHeader), header->dataLenght - sizeof(DataHeader), 0);
		ChangeUser* _change = (ChangeUser*)szRecv;
		auto pos = _userList.begin();
		for (; pos != _userList.end(); ++pos)
		{
			if ((*pos)->_uSock == _csock)
			{
				break;
			}
		}
		if (pos != _userList.end())
		{
			(*pos)->_chatId = _change->userId;
		}

	}
	break;

	case CMD_MSG:
	{
		recv(_csock, szRecv + sizeof(DataHeader), header->dataLenght - sizeof(DataHeader), 0);
		Msg* _mgs = (Msg*)szRecv;
		unsigned int  toid = _mgs->_toUserId;
		auto ite = _userList.begin();
		for (; ite != _userList.end(); ++ite)
		{
			if ((*ite)->_userId == toid)
				break;
		}
		SOCKET toSock = INVALID_SOCKET;
		if (ite != _userList.end())
		{
			toSock = (*ite)->_uSock;
		}
		send(toSock, _mgs->_Msg, 8192, 0);
	}
	break;

	default:
		break;
	}
}


//等待客户端连接
int Server::Accept()
{
	if (_sock == INVALID_SOCKET)
	{
		printf("旧<socket=%d>无效，重新创建\n", _sock);
		InitSocket();
	}
	else {
		sockaddr_in _cSin = {};
		socklen_t nLen = sizeof(_cSin);
		SOCKET _cSock = accept(_sock, (sockaddr*)&_cSin, &nLen);
		if (_cSock == SOCKET_ERROR)
		{
			printf("accept()函数失败\n");
			return SOCKET_ERROR;
		}
		else
		{
			printf("accpet()函数连接成功,<Client_Socket=%d>\n", _cSock);
			AddClientInList(_cSock, "userName");  //把新加入的客户端加入到客户端列表
			return _cSock;
		}
	}
	return SOCKET_ERROR;
}

//关闭套接字socket
void Server::Close()
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


void Server::PrintUser()
{
	auto iter = _userList.begin();
	for (iter; iter != _userList.end(); ++iter)
	{
		printf("%s,%d,%d\n", (*iter)->_userName, (*iter)->_userId, (*iter)->_uSock);
	}
}


//把新加入的客户端加入到客户端列表
void Server::AddClientInList(SOCKET _usock, char const *userName)
{
	LPUserList _user = new UserList;
	_user->_uSock = _usock;
	strcpy(_user->_userName, userName);
	_user->_userId = _userTargetId++;
	return;
}

//把退出的客户端从客户端列表中移除
void Server::DeleteClientInList(SOCKET _usock)
{
	std::vector<LPUserList>::iterator iter = _userList.begin();
	for (; iter != _userList.end(); ++iter)
	{
		if ((*iter)->_uSock == _usock)
		{
			break;
		}
	}
	if (iter != _userList.end())
	{
		delete(*iter);   //释放资源
		_userList.erase(iter);  //从列表中把改客户端信息删除
	}
	return;
}
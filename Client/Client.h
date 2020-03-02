#ifndef _Clien_h_
#define _Client_h_

#define WIN32_LEAN_AND_MEAN
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Global.h"

#ifdef _WIN32
	#define WIN32_LEAN_AND_MEAN
	#include <windows.h>
	#include <WinSock2.h>
	#pragma comment(lib,"ws2_32.lib")
#else
	#include <unistd.h>
	#include <arpa/inet.h>
	#define SOCKET int
	#define SOCKET_ERROR (SOCKET)(-1)
	#define INVALID_SOCKET (SOCKET) (~0)
#endif // !_WIN32


class Client {
private:
	SOCKET _sock;
public:
	Client();
	~Client();
	void InitSocket();
	int Connect(const char* ip, const short port);
	void Close();
	bool OnRun();
	bool IsRun();

	int RecvData();
	void OnNetMsg(DataHeader *header);
	int SendData(DataHeader *header);
};


#endif // !_Clien_h_

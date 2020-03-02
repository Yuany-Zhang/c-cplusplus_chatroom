#ifndef _Server_h_
#define _Server_h_
#define WIN32_LEAN_AND_MEAN
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
	#define WIN32_LEAN_AND_MEAN
	#include <windows.h>
	#include <WinSock2.h>
	#pragma comment(lib,"ws2_32.lib")
	#define socklen_t int
#else
	#include <unistd.h>
	#include <arpa/inet.h>
	#define SOCKET int
	#define SOCKET_ERROR (SOCKET)(-1)
	#define INVALID_SOCKET (SOCKET) (~0)
#endif // !_WIN32



class Server {
private:
	SOCKET _sock;

public:
	Server();
	~Server();
	void InitSocket();
	int Bind_Listen(const char* ip, const short port, const bool anyaddr);
	int Accept();
	void Close();

	void OnRun();
	int Processor(SOCKET _csock);

	void AddClientInList(SOCKET _usock,char const *userName);
	void DeleteClientInList(SOCKET _usock);
	void PrintUser();

};







#endif // !_Server_h_



#ifndef _Global_h_
#define _Global_h_

#include <vector>
#include <mutex>
typedef struct
{
	char _userName[32];
	unsigned int _userId;
	unsigned int _uSock;
	unsigned int _chatId = 0;
}UserList, *LPUserList;



extern std::vector<LPUserList> _userList;

extern unsigned int _thisClientId;  //��ǰ�ͻ����ڷ������е�id
extern int _thisClientSocket;  //��ǰ�ͻ��˵�socket
extern char _thisClientName[32];  //��ǰ�ͻ����û���

//����һЩ ȫ����Ҫʹ�õ��Ľṹ��������

//�ṹ����
typedef struct
{
	char ItemName[50];
	char ItemContent[500];
}ConfigItem, *LPConfigItem;


enum CMD {
	CMD_LOGIN,  //��¼
	CMD_LOGOUT,  //�˳�
	CMD_GET_USER_LIST,  //��ȡ�û��б�
	CMD_BROADCAST,  //�㲥����
	CMD_CHANGE_USER,  //���������û�
	CMD_NEW_USER_JOIN,  //���û�����
	CMD_MSG,  //��Ϣ
	CMD_ERROR  //��������
};

//��Ϣ��ͷ
typedef struct
{
	short dataLenght; //��Ϣ����
	short cmd;  //����
}DataHeader, *LPDataHeader;



typedef struct GetUserList :public DataHeader
{
	GetUserList()
	{
		dataLenght = sizeof(GetUserList);
		cmd = CMD_GET_USER_LIST;
	}
	unsigned int userNum = 0;
}GetUserList;

typedef struct Msg : public DataHeader
{
	Msg()
	{
		dataLenght = sizeof(Msg);
		cmd = CMD_MSG;
	}
	unsigned int _toUserId = 0;
	char _userName[50];
	char _Msg[8192];
}Msg;

typedef struct Broadcast :public DataHeader
{
	Broadcast()
	{
		dataLenght = sizeof(Broadcast);
		cmd = CMD_BROADCAST;
	}
	unsigned int _userId;
	char _userName[50];		//˭����Ĺ㲥
	char _content[8192];   //�㲥����
}Broadcast;

typedef struct Login :public DataHeader
{
	Login()
	{
		dataLenght = sizeof(Login);
		cmd = CMD_LOGIN;
	}
	char userName[50];
	char passWord[50];
	unsigned int clientId = 0;
}Login;

typedef struct Logout :public DataHeader
{
	Logout()
	{
		dataLenght = sizeof(Logout);
		cmd = CMD_LOGOUT;
	}
	char userName[50];
	unsigned int userId;
}Logout;

typedef struct ChangeUser :public DataHeader
{
	ChangeUser()
	{
		dataLenght = sizeof(ChangeUser);
		cmd = CMD_CHANGE_USER;
	}
	char userName[50];
	unsigned int userId;
}ChangeUser;

typedef struct NewUserJoin :public DataHeader
{
	NewUserJoin()
	{
		dataLenght = sizeof(NewUserJoin);
		cmd = CMD_NEW_USER_JOIN;
	}
	char userName[50];
	unsigned int userId;
}NewUserJoin;



#endif // !_Global_h_

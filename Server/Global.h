#ifndef _Global_h_
#define _Global_h_
#include <vector>

typedef struct
{
	char _userName[32];
	unsigned int _userId;
	unsigned int _uSock;
	unsigned int _chatId = 0;
}UserList, *LPUserList;


extern std::vector<LPUserList> _userList;
extern unsigned int _userTargetId;


//定义一些 全局需要使用到的结构、变量等

//结构定义
typedef struct
{
	char ItemName[50];
	char ItemContent[500];
}ConfigItem, *LPConfigItem;


enum CMD {
	CMD_LOGIN,  //登录
	CMD_LOGOUT,  //退出
	CMD_GET_USER_LIST,  //获取用户列表
	CMD_BROADCAST,  //广播命令
	CMD_CHANGE_USER,  //更换聊天用户
	CMD_NEW_USER_JOIN,  //新用户加入
	CMD_MSG,  //消息
	CMD_ERROR  //错误命令
};

//消息包头
typedef struct
{
	short dataLenght; //消息长度
	short cmd;  //命令
}DataHeader, *LPDataHeader;


typedef struct GetUserList :public DataHeader
{

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
	char _userName[50];		//谁发起的广播
	char _content[8192];   //广播内容
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

#include "stdio.h"
#include "stdlib.h"
//#include "iostream"
#include "string.h"
//using namespace std;
struct log
{
	long long int time;
	float longitude,latitude;
	struct log *next_log;
};
typedef struct log logNode;
/******Creat link list******/
struct user
{
	char name[5];
	int ID_Num;
	int code;
	struct log *log_info;
	struct user *next_user;
};
typedef struct user userNode;
/**********创建链表************/
struct log *Create_log(long long int time,float longitude,float latitude);

/**********链表加入信息**********/
void Add(logNode *head,long long int time,float longitude,float latitude);

/*************************在初始化外层链表的同时初始化内层链表************************/
userNode *Creat_user(char name[],int ID_Num,int code,long long int time,float longitude,float latitude);

void ergodic(userNode *user);


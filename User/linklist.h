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
/**********��������************/
struct log *Create_log(long long int time,float longitude,float latitude);

/**********���������Ϣ**********/
void Add(logNode *head,long long int time,float longitude,float latitude);

/*************************�ڳ�ʼ����������ͬʱ��ʼ���ڲ�����************************/
userNode *Creat_user(char name[],int ID_Num,int code,long long int time,float longitude,float latitude);

void ergodic(userNode *user);


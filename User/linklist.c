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
	unsigned int ID_Num[8];
	int code;
	struct log *log_info;
	struct user *next_user;
};
typedef struct user userNode;
/**********创建链表************/
struct log *Create_log(long long int time,float longitude,float latitude)
{
	struct log *head,*tail,*pnew;
	head=(struct log *)malloc(sizeof(struct log));
	head->next_log=NULL;
	head->latitude=latitude;
	head->longitude=longitude;
	return head;
}
/**********链表加入信息**********/
void Add(logNode *head,long long int time,float longitude,float latitude)
{
/********通过head指针将所有链表遍历一遍，找到链表的末端********/
	struct log *tail,*pnew;
	tail=head;//->next_log;
	while(tail->next_log!= NULL)
		tail=tail->next_log;//tail指向链表末端
	pnew=(struct log *)malloc(sizeof(struct log));
	head->next_log=pnew;
	tail=pnew;
//	head->next_log=pnew;
	tail->time=time;
	tail->latitude=latitude;
	tail->longitude=longitude;
	tail->next_log=NULL;
//	tail->next_log=tail;
}
/*************************在初始化外层链表的同时初始化内层链表************************/
userNode *Creat_user(char name[],unsigned int ID_Num[],int code,long long int time,float longitude,float latitude)
{
	unsigned int i;
	struct user *head,*tail,*pnew;
	head=(struct user *)malloc(sizeof(struct user));
	for(i=0;i<5;i++)
		head->name[i]=name[i];
	head->code=code;
	for(i=0;i<8;i++)
		head->ID_Num[i]=ID_Num[i];
	head->next_user=NULL;
	head->log_info=(struct log *)malloc(sizeof(struct log));
	head->log_info->time=time;
	head->log_info->latitude=latitude;
	head->log_info->longitude=longitude;
	head->log_info->next_log=NULL;
	return head;
}
/*void ergodic(userNode *user)
{
	unsigned char i=1;
	cout<<user->name<<endl<<user->ID_Num<<endl;
	cout<<user->code<<endl;	
	do	
	{
		cout<<"Number "<<i<<"time"<<endl;i++;
		cout<<user->log_info->time<<endl;
		cout<<user->log_info->longitude<<endl;
		cout<<user->log_info->latitude<<endl;
		if(user->log_info->next_log==NULL)break;
		user->log_info=user->log_info->next_log;		
	}while(user->log_info->next_log==NULL);
}
*/
/*void main()
{
	user *a;
	a=Creat_user("tom",1305054145,87654321,201704211911,113.876,34.5865);
//	printf("%c",a->name);
	cout<<"user's name:"<<a->name<<endl;
	cout<<"user's code:"<<a->code<<endl;
	cout<<"user's ID:"<<a->ID_Num<<endl;
	cout<<"user's log info:"<<endl<<"latitude:"<<a->log_info->latitude<<endl;
	cout<<"user's log info:"<<endl<<"longitude:"<<a->log_info->longitude<<endl;
	cout<<"user's log info:"<<endl<<"time:"<<a->log_info->time<<endl;
*//*	struct log *pnew;
	pnew=(struct log *)malloc(sizeof(struct log));
	pnew->next_log=NULL;
	pnew->time=201802072355;
	pnew->latitude=100.987;
	pnew->longitude=45.085;
	a->log_info->next_log=pnew;*//*
	Add(a->log_info,201805060935,108.987,54.687);
	cout<<"user's log info:"<<endl<<"latitude:"<<a->log_info->latitude<<endl;
	cout<<"user's log info:"<<endl<<"longitude:"<<a->log_info->longitude<<endl;
	cout<<"user's log info:"<<endl<<"time:"<<a->log_info->time<<endl;
	ergodic(a);
}*/
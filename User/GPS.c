#include "bsp_usart3.h"
#include "pcf8563.h"
#include "time.h"
#include "stdlib.h"
_SaveData Save_Data;
//TIME today;
//__int64 time2;
struct	tm *now_GPS;

void errorLog(int num)
{
	
	while (1)
	{
	  	printf("ERROR%d\r\n",num);
	}
}

void parseGpsBuffer()
{
	char *subString;
	char *subStringNext;
	char i = 0;
	now_GPS=(struct tm*)malloc(sizeof(struct tm));	
	if (Save_Data.isGetData)
	{
		Save_Data.isGetData = false;
		printf("**************\r\n");
		printf(Save_Data.GPS_Buffer);

		
		for (i = 0 ; i <= 9 ; i++)
		{
			if (i == 0)
			{
				if ((subString = strstr(Save_Data.GPS_Buffer, ",")) == NULL)
					errorLog(1);	//解析错误
			}
			else
			{
				subString++;
				if ((subStringNext = strstr(subString, ",")) != NULL)//strstr:判断字符串","是否是subString的子串
				{
					char usefullBuffer[2]; 
					switch(i)
					{
						case 1:memcpy(Save_Data.UTCTime, subString, subStringNext - subString);break;	//获取UTC时间
						case 2:memcpy(usefullBuffer, subString, subStringNext - subString);break;	//获取UTC时间
						case 3:memcpy(Save_Data.latitude, subString, subStringNext - subString);break;	//获取纬度信息
						case 4:memcpy(Save_Data.N_S, subString, subStringNext - subString);break;	//获取N/S
						case 5:memcpy(Save_Data.longitude, subString, subStringNext - subString);break;	//获取经度信息
						case 6:memcpy(Save_Data.E_W, subString, subStringNext - subString);break;	//获取E/W
						case 9:memcpy(Save_Data.UTCDate, subString, subStringNext - subString);break;	//获取E/W
						default:break;
					}

					subString = subStringNext;
					Save_Data.isParseData = true;
					if(usefullBuffer[0] == 'A')
						Save_Data.isUsefull = true;
					else if(usefullBuffer[0] == 'V')
						Save_Data.isUsefull = false;

				}
				else
				{
					errorLog(2);	//解析错误
				}
			}


		}
	
		now_GPS->tm_sec=(Save_Data.UTCTime[4]-'0')*10+(Save_Data.UTCTime[5]-'0');
		now_GPS->tm_min=(Save_Data.UTCTime[2]-'0')*10+(Save_Data.UTCTime[3]-'0');
		now_GPS->tm_hour=(Save_Data.UTCTime[0]-'0')*10+(Save_Data.UTCTime[1]-'0');
		now_GPS->tm_mday=(Save_Data.UTCDate[0]-'0')*10+(Save_Data.UTCDate[1]-'0');
		now_GPS->tm_mon=(Save_Data.UTCDate[2]-'0')*10+(Save_Data.UTCDate[3]-'0');
		now_GPS->tm_year=(Save_Data.UTCDate[4]-'0')*10+(Save_Data.UTCDate[5]-'0');
		PCF_SetTime(now_GPS->tm_year,now_GPS->tm_mon,now_GPS->tm_mday,now_GPS->tm_hour,now_GPS->tm_min,now_GPS->tm_sec);
	 printf("GPS 时间已设置 \n");
		printf("GPS时间 :");	printf("%d年 ",now_GPS->tm_year);printf("%d月 ",now_GPS->tm_mon);printf("%d日 ",now_GPS->tm_mday);
		printf("%d时 ",now_GPS->tm_hour);printf("%d分 ",now_GPS->tm_min);printf("%d秒 \n",now_GPS->tm_sec);
//		time2=mktime(now_GPS);
//		printf("time2_GPS %lld \n",time2);
	}
}

void printGpsBuffer()
{
	if (Save_Data.isParseData)
	{
		Save_Data.isParseData = false;
		
		printf("Save_Data.UTCTime = ");
		printf(Save_Data.UTCTime);
//		printf("\n");
//		printf("Save_Data.UTCTime = ");
//		printf("%c  %c  %c  ",Save_Data.UTCTime[0],Save_Data.UTCTime[1],Save_Data.UTCTime[2]);
//		printf("%c  %c  %c  ",Save_Data.UTCTime[3],Save_Data.UTCTime[4],Save_Data.UTCTime[5]);
		printf("\r\n");

		printf("Save_Data.UTCDate = ");
		printf(Save_Data.UTCDate);
		printf("\r\n");
		if(Save_Data.isUsefull)
		{
			Save_Data.isUsefull = false;
			printf("Save_Data.latitude = ");
			printf(Save_Data.latitude);
			printf("\r\n");


			printf("Save_Data.N_S = ");
			printf(Save_Data.N_S);
			printf("\r\n");

			printf("Save_Data.longitude = ");
			printf(Save_Data.longitude);
			printf("\r\n");

			printf("Save_Data.E_W = ");
			printf(Save_Data.E_W);
			printf("\r\n");

		}
		else
		{
			printf("GPS DATA is not usefull!\r\n");
		}
		
	}
}

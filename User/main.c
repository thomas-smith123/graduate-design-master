/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   2.4g无线模块/nrf24l01+/slave 测试
  ******************************************************************************
  * @attention
  *
  * 实验平台:野火 iSO STM32 开发板 
  * 论坛    :http://www.chuxue123.com
  * 淘宝    :http://firestm32.taobao.com
  *
  ******************************************************************************
  */
  
#include "stm32f10x.h"
#include "bsp_usart1.h"
#include "bsp_usart3.h"
#include "bsp_spi_nrf.h"
#include "bsp_ili9341_lcd.h"
#include "AES.h"
//#include "ADC.h"
#include "stdlib.h"
#include "sim900a.h"
#include "sys.h"
#include "bsp_usart2.h"
#include "GPS.h"
#include "pcf8563.h"
#include "linklist.h"
#include "time.h"
#include "math.h"
u8 status;		 //用于判断接收/发送状态
u8 txbuf[4];	 //发送缓冲
u8 i; 
extern __IO uint16_t ADC_ConvertedValue;

// 局部变量，用于保存转换计算后的电压值 	 
float ADC_ConvertedValueLocal;        

 /**
  * @brief  主函数
  * @param  无
  * @retval 无
  */
int main(void)
{     
	int diff_time;	
	struct tm *now_GPS;
	struct tm *now_pcf;
	u8 j,res;
		uint8_t i, r;
	u8 rxbuf[16];	 //接收缓冲
	uint8_t ID_Num[10],mingwen1[16],mingwen2[16],miwen1[16],miwen2[16];
	/* 128 bit key */
	uint8_t key[] = {
		1, 3, 0, 5, 0, 5, 4, 1, 
		4, 5, 1, 2, 3, 4, 5, 6};
	uint8_t plaintext[16];// = "1305054145jrd";  
	uint8_t ciphertext[AES_BLOCK_SIZE];
	uint8_t frame[42]=
		{1,3,0,5,0,5,4,1,4,5,
									0,123,0,0,
									2,0,1,7,0,5,0,1,1,3,4,5,
									0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
		};

	const uint8_t const_cipher[AES_BLOCK_SIZE] = {
		0xff, 0x0b, 0x84, 0x4a, 0x08, 0x53, 0xbf, 0x7c,
		0x69, 0x34, 0xab, 0x43, 0x64, 0x14, 0x8f, 0xb9};	
	uint8_t roundkeys[AES_ROUND_KEY_SIZE];
int rand_num;
		time_t time1,time2;
		char verify_code[20]="abcde";
extern	TIME today;
		TIME now;
	_SaveData Save_Data;
	uint8_t *syn,*seq,*ack,*fin,*time,*data;
	now_GPS=(struct tm*)malloc(sizeof(struct tm));	
		now_pcf=(struct tm*)malloc(sizeof(struct tm));
	syn=frame+10;seq=frame+11;
	ack=frame+12;fin=frame+13;
	time=frame+14;data=frame+26;
  /* 串口1初始化 */
			delay_init();	    	 //延时函数初始化	  
	NVIC_Configuration(); 	 //设置NVIC中断分组2:2位抢占优先级，2位响应优先级 	LED_Init();			     //LED端口初始化
	USART2_Init(115200);	//初始化串口2 
	USART1_Init(9600);	//LOG信息
	uart_init(9600);
//  USART1_Config(); 
  /*SPI接口初始化*/   
  SPI_NRF_Init();		
		PCF8563_Init();
//	ADC1_Init();
//  LCD_Init();	
//	LCD_Clear(0, 0, 240, 320,BACKGROUND);	
	UART1SendString("系统初始化完毕\r\n",strlen("系统初始化完毕\r\n"));	

	printf("Initiated");		
		printf("\n--------------------------------------------------------\n");
/*/	printf("Plain text:\n");
//	for (i = 0; i < AES_BLOCK_SIZE; i++) {
//		printf("%2x ", plaintext[i]);
//	}
//	printf("\n\n");	*/

	// key schedule
	aes_key_schedule_128(key, roundkeys);//密钥扩充
	printf("Round Keys:\n");
	for ( r = 0; r <= AES_ROUNDS; r++ ) {
		for (i = 0; i < AES_BLOCK_SIZE; i++) {
			printf("%2x ", roundkeys[r*AES_BLOCK_SIZE+i]);
		}
		printf("\n");
	}
	printf("\n");

	// encryption
//	aes_encrypt_128(roundkeys, plaintext, ciphertext);//明文、密文、轮密钥
//	printf("Cipher text:\n");
//	for (i = 0; i < AES_BLOCK_SIZE; i++) {
//		printf("%2x ", ciphertext[i]);
//	}
//	for (i = 0; i < AES_BLOCK_SIZE; i++) {
//		if ( ciphertext[i] != const_cipher[i] ) { break; }
//	}
//	if ( AES_BLOCK_SIZE != i ) { printf("\nENCRYPT WRONG\n\n"); }
//	else { printf("\nENCRYPT CORRECT\n\n"); }
	// decryption
	printf("Plain text:\n");
	for (i = 0; i < AES_BLOCK_SIZE; i++) {
		printf("%2x ", ciphertext[i]);
	}
	for (i = 0; i < AES_BLOCK_SIZE; i++) {
		if ( ciphertext[i] != plaintext[i] ) { break; }
	}
	if ( AES_BLOCK_SIZE != i ) { printf("\nDECRYPT WRONG\n\n"); }
	else { printf("\nDECRYPT CORRECT\n\n"); }

  /*检测NRF模块与MCU的连接*/
  status = NRF_Check();   		
  if(status == SUCCESS)	   
	{
		printf("\r\n   NRF与MCU连接成功\r\n"); 
//		LCD_DispStr(10, 10, (uint8_t *)"SUCCESSFULLY CONNECTED!", RED);	
	} 
  else	  
	{
		printf("\r\n   正在检测NRF与MCU是否正常连接。。。\r\n");
//		LCD_DispStr(10, 10, (uint8_t *)"CHECKING CONNECTED SITUATION...", RED);	
	}
	/********************伪随机数*********************/
	srand(48);
	srand(rand());//伪随机数
	rand_num=rand();
	printf("%d \n", rand_num); 
		rand_num=rand();
	printf("%d \n", rand_num);
	rand_num=rand();
	printf("%d \n", rand_num);
	memset(verify_code, 0x00, 8);
  sprintf(verify_code, "%d", rand_num);//数字转字符串
	/***********************************************/

		while(res)
	{
		res=GSM_Dect();
		delay_ms(2000);
	}	
	res=1;
//	while(res)	//只需要修改电话号和内容即可
//	{
//	//	res=sim900a_send_enmessage_zc((u8*)"18435132958",(u8*)verify_code);	//发送验证码
//	}
	while(1)
	{
		parseGpsBuffer();
//		printGpsBuffer();
//		now =PCF8563_GetTime();	
//		Creat_user("thomas","1305054145",)
//		printf("pcf8563 time %d %d %d %d %d %d\n",now.year,now.month,now.day,now.hour,now.mint,now.second);	
//		UART1SendString("空闲中\r\n",strlen("空闲中\r\n"));	delay_ms(5000);
		    printf("\r\n 从机端 进入接收模式\r\n"); 
//		LCD_DispStr(10, 30, (uint8_t *)"SLAVE IN RX_MODE.", RED);	
    printf("\r \n主机进入接收模式");
    NRF_RX_Mode();
    /*等待接收数据*/
    status = NRF_Rx_Dat(ID_Num);
		status = NRF_Rx_Dat(ID_Num+4);
		status = NRF_Rx_Dat(ID_Num+8);
		status = NRF_Rx_Dat(ID_Num+12);
		status = NRF_Rx_Dat(miwen1);
		status = NRF_Rx_Dat(miwen1+4);
		status = NRF_Rx_Dat(miwen1+8);
		status = NRF_Rx_Dat(miwen1+12);
		status = NRF_Rx_Dat(miwen2);
		status = NRF_Rx_Dat(miwen2+4);
		status = NRF_Rx_Dat(miwen2+8);
		status = NRF_Rx_Dat(miwen2+12);
		printf("接收成功1\n");
		now =PCF8563_GetTime();	
		printf("pcf8563 time %d %d %d %d %d %d\n",now.year,now.month,now.day,now.hour,now.mint,now.second);
		now_pcf->tm_hour=now.hour;
		now_pcf->tm_sec=now.second;
		now_pcf->tm_min=now.mint;
		now_pcf->tm_mday=now.day;
		now_pcf->tm_mon=now.month;
		now_pcf->tm_year=2000+now.year;
//		time1=mktime(now_pcf);
//		printf("time1_pcf: %lld \n",time1);
		/*******************查找对应ID的密钥***********假设密码已知*********/
//		aes_decrypt_128(roundkeys,ID_Num,ID_Num);
		aes_decrypt_128(roundkeys,miwen1,miwen1);
		aes_decrypt_128(roundkeys,miwen2,miwen2);
		printf("接收到的数据包：");
		for(i=0;i<10;i++)printf("%d",ID_Num[i]);
		printf("\n");
		for(i=0;i<16;i++)printf("%d",miwen1[i]);
		printf("\n");
		for(i=0;i<16;i++)printf("%d",miwen2[i]);
		printf("\n");
		for(i=0;i<10;i++)frame[i]=ID_Num[i];
		for(i=0;i<16;i++)
		{
			frame[10+i]=miwen1[i];
			frame[26+i]=miwen2[i];
		}//帧重建	
		now_GPS->tm_year=*time*1000+*(time+1)*100+*(time+2)*10+*(time+3);
		now_GPS->tm_mon=*(time+4)*10+*(time+5);
		now_GPS->tm_mday=*(time+6)*10+*(time+7);
		now_GPS->tm_hour=*(time+8)*10+*(time+9);
		now_GPS->tm_min=*(time+10)*10+*(time+11);
		now_GPS->tm_sec=*(time+12)*10+*(time+13);
//		time2=mktime(now_GPS);
//		printf("GPS_mktime %lld \n",time2);
		/********************判断包的时间******************/
//		time1=20*100000000000+now.year*10000000000+now.month*100000000+now.day*1000000+now.hour*10000+now.mint*100+now.second;////////////////////////////////next///////
//		time2=20*100000000000+(Save_Data.UTCDate[4]*10+Save_Data.UTCDate[5])*10000000000+(Save_Data.UTCDate[2]*10+Save_Data.UTCDate[3])*100000000+(Save_Data.UTCDate[0]*10+Save_Data.UTCDate[1])*1000000+
//		(Save_Data.UTCTime[0]*10+Save_Data.UTCTime[1])*10000+(Save_Data.UTCTime[2]*10+Save_Data.UTCTime[3])*100+(Save_Data.UTCTime[4]*10+Save_Data.UTCTime[5]);//GPS:UTC Date:ddmmyy
		diff_time=difftime(mktime(now_GPS),mktime(now_pcf));////////////////////next///////
		printf("%d diff_time",diff_time);
		if(abs(diff_time)<=60)//误差在允许范围内
		{
			if(*syn==1)
			{	
				*ack=++*seq;*syn=1;
				*seq=rand();
				now=PCF8563_GetTime();
				/*****************year******************/
				*time=2;//2
				*(time+1)=now.year%1000/100;//0
				*(time+2)=now.year%100/10;//1
				*(time+3)=now.year%10;
				/********************month**************/
				*(time+4)=now.month/10;
				*(time+5)=now.month%10;
				/*******************day*****************/
				*(time+6)=now.day/10;
				*(time+7)=now.day%10;
				/******************hour****************/
				*(time+8)=now.hour/10;
				*(time+9)=now.hour%10;
				/*****************minute**************/
				*(time+10)=now.mint/10;
				*(time+11)=now.mint%10;
				/*****************second***************/
				*(time+12)=now.second/10;
				*(time+13)=now.second%10;
				for(i=0;i<16;i++)
				{
					mingwen1[i]=frame[i+10];
					mingwen2[i]=frame[i+26];
				}
					// encryption
				aes_encrypt_128(roundkeys, mingwen1, miwen1);//明文、密文、轮密钥
				aes_encrypt_128(roundkeys, mingwen2, miwen2);
				NRF_TX_Mode();
				delay_ms(3);
				status = NRF_Tx_Dat(ID_Num);//ID
				status = NRF_Tx_Dat(ID_Num+4);
				status = NRF_Tx_Dat(ID_Num+8);
				status = NRF_Tx_Dat(ID_Num+12);
				status = NRF_Tx_Dat(miwen1);
				status = NRF_Tx_Dat(miwen1+4);
				status = NRF_Tx_Dat(miwen1+8);
				status = NRF_Tx_Dat(miwen1+12);
				status = NRF_Tx_Dat(miwen2);
				status = NRF_Tx_Dat(miwen2+4);
				status = NRF_Tx_Dat(miwen2+8);
				status = NRF_Tx_Dat(miwen2+12);
				printf("发送成功1\n");
				NRF_RX_Mode();
				status = NRF_Rx_Dat(ID_Num);
				status = NRF_Rx_Dat(ID_Num+4);
				status = NRF_Rx_Dat(ID_Num+8);
				status = NRF_Rx_Dat(ID_Num+12);
				status = NRF_Rx_Dat(miwen1);
				status = NRF_Rx_Dat(miwen1+4);
				status = NRF_Rx_Dat(miwen1+8);
				status = NRF_Rx_Dat(miwen1+12);
				status = NRF_Rx_Dat(miwen2);
				status = NRF_Rx_Dat(miwen2+4);
				status = NRF_Rx_Dat(miwen2+8);
				status = NRF_Rx_Dat(miwen2+12);
				printf("接收成功2!\n");
//				aes_decrypt_128(roundkeys,ID_Num,ID_Num);
				aes_decrypt_128(roundkeys,miwen1,miwen1);
				aes_decrypt_128(roundkeys,miwen2,miwen2);
				if(*syn==0&&miwen1[2]==(*seq+1))
				{printf("握手成功！\n");}
			}
		}
		else printf("时间误差超出预期 \n");continue;
/*		if(now.year==*(time+2)*10+ *(time+3))
			if(now.month==*(time+4)*10+*(time+5))
				if(now.day==*(time+6)*10+*(time+7))
	*/				
//    /*判断接收状态*/
    if(status == RX_DR)
    {	
			aes_decrypt_128(roundkeys, rxbuf, rxbuf);//解密
      for(i=0;i<16;i++)
      {	
        printf("%2x ",rxbuf[i]);
      }
    }   

	}
/*	while(1)	
	{		
		parseGpsBuffer();
		printGpsBuffer();
	};
*/
	
  while(1)
  { 
    printf("\r\n 从机端 进入接收模式\r\n"); 
//		LCD_DispStr(10, 30, (uint8_t *)"SLAVE IN RX_MODE.", RED);	
//		ADC_ConvertedValueLocal =(float) ADC_ConvertedValue/4096*3.3; // 读取转换的AD值	

//		printf("\r\n %f V \r",ADC_ConvertedValueLocal); 

    NRF_RX_Mode();
    /*等待接收数据*/
    status = NRF_Rx_Dat(rxbuf);
		status = NRF_Rx_Dat(rxbuf+4);
		status = NRF_Rx_Dat(rxbuf+8);
		status = NRF_Rx_Dat(rxbuf+12);
//    /*判断接收状态*/
    if(status == RX_DR)
    {	
			aes_decrypt_128(roundkeys, rxbuf, rxbuf);//解密
      for(i=0;i<16;i++)
      {	
        printf("%2x ",rxbuf[i]);
      }
    }   
  } 
}
/*********************************************END OF FILE**********************/

/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   2.4g����ģ��/nrf24l01+/slave ����
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:Ұ�� iSO STM32 ������ 
  * ��̳    :http://www.chuxue123.com
  * �Ա�    :http://firestm32.taobao.com
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
u8 status;		 //�����жϽ���/����״̬
u8 txbuf[4];	 //���ͻ���
u8 i; 
extern __IO uint16_t ADC_ConvertedValue;

// �ֲ����������ڱ���ת�������ĵ�ѹֵ 	 
float ADC_ConvertedValueLocal;        

 /**
  * @brief  ������
  * @param  ��
  * @retval ��
  */
int main(void)
{     
	int diff_time;	
	struct tm *now_GPS;
	struct tm *now_pcf;
	u8 j,res;
		uint8_t i, r;
	u8 rxbuf[16];	 //���ջ���
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
  /* ����1��ʼ�� */
			delay_init();	    	 //��ʱ������ʼ��	  
	NVIC_Configuration(); 	 //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ� 	LED_Init();			     //LED�˿ڳ�ʼ��
	USART2_Init(115200);	//��ʼ������2 
	USART1_Init(9600);	//LOG��Ϣ
	uart_init(9600);
//  USART1_Config(); 
  /*SPI�ӿڳ�ʼ��*/   
  SPI_NRF_Init();		
		PCF8563_Init();
//	ADC1_Init();
//  LCD_Init();	
//	LCD_Clear(0, 0, 240, 320,BACKGROUND);	
	UART1SendString("ϵͳ��ʼ�����\r\n",strlen("ϵͳ��ʼ�����\r\n"));	

	printf("Initiated");		
		printf("\n--------------------------------------------------------\n");
/*/	printf("Plain text:\n");
//	for (i = 0; i < AES_BLOCK_SIZE; i++) {
//		printf("%2x ", plaintext[i]);
//	}
//	printf("\n\n");	*/

	// key schedule
	aes_key_schedule_128(key, roundkeys);//��Կ����
	printf("Round Keys:\n");
	for ( r = 0; r <= AES_ROUNDS; r++ ) {
		for (i = 0; i < AES_BLOCK_SIZE; i++) {
			printf("%2x ", roundkeys[r*AES_BLOCK_SIZE+i]);
		}
		printf("\n");
	}
	printf("\n");

	// encryption
//	aes_encrypt_128(roundkeys, plaintext, ciphertext);//���ġ����ġ�����Կ
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

  /*���NRFģ����MCU������*/
  status = NRF_Check();   		
  if(status == SUCCESS)	   
	{
		printf("\r\n   NRF��MCU���ӳɹ�\r\n"); 
//		LCD_DispStr(10, 10, (uint8_t *)"SUCCESSFULLY CONNECTED!", RED);	
	} 
  else	  
	{
		printf("\r\n   ���ڼ��NRF��MCU�Ƿ��������ӡ�����\r\n");
//		LCD_DispStr(10, 10, (uint8_t *)"CHECKING CONNECTED SITUATION...", RED);	
	}
	/********************α�����*********************/
	srand(48);
	srand(rand());//α�����
	rand_num=rand();
	printf("%d \n", rand_num); 
		rand_num=rand();
	printf("%d \n", rand_num);
	rand_num=rand();
	printf("%d \n", rand_num);
	memset(verify_code, 0x00, 8);
  sprintf(verify_code, "%d", rand_num);//����ת�ַ���
	/***********************************************/

		while(res)
	{
		res=GSM_Dect();
		delay_ms(2000);
	}	
	res=1;
//	while(res)	//ֻ��Ҫ�޸ĵ绰�ź����ݼ���
//	{
//	//	res=sim900a_send_enmessage_zc((u8*)"18435132958",(u8*)verify_code);	//������֤��
//	}
	while(1)
	{
		parseGpsBuffer();
//		printGpsBuffer();
//		now =PCF8563_GetTime();	
//		Creat_user("thomas","1305054145",)
//		printf("pcf8563 time %d %d %d %d %d %d\n",now.year,now.month,now.day,now.hour,now.mint,now.second);	
//		UART1SendString("������\r\n",strlen("������\r\n"));	delay_ms(5000);
		    printf("\r\n �ӻ��� �������ģʽ\r\n"); 
//		LCD_DispStr(10, 30, (uint8_t *)"SLAVE IN RX_MODE.", RED);	
    printf("\r \n�����������ģʽ");
    NRF_RX_Mode();
    /*�ȴ���������*/
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
		printf("���ճɹ�1\n");
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
		/*******************���Ҷ�ӦID����Կ***********����������֪*********/
//		aes_decrypt_128(roundkeys,ID_Num,ID_Num);
		aes_decrypt_128(roundkeys,miwen1,miwen1);
		aes_decrypt_128(roundkeys,miwen2,miwen2);
		printf("���յ������ݰ���");
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
		}//֡�ؽ�	
		now_GPS->tm_year=*time*1000+*(time+1)*100+*(time+2)*10+*(time+3);
		now_GPS->tm_mon=*(time+4)*10+*(time+5);
		now_GPS->tm_mday=*(time+6)*10+*(time+7);
		now_GPS->tm_hour=*(time+8)*10+*(time+9);
		now_GPS->tm_min=*(time+10)*10+*(time+11);
		now_GPS->tm_sec=*(time+12)*10+*(time+13);
//		time2=mktime(now_GPS);
//		printf("GPS_mktime %lld \n",time2);
		/********************�жϰ���ʱ��******************/
//		time1=20*100000000000+now.year*10000000000+now.month*100000000+now.day*1000000+now.hour*10000+now.mint*100+now.second;////////////////////////////////next///////
//		time2=20*100000000000+(Save_Data.UTCDate[4]*10+Save_Data.UTCDate[5])*10000000000+(Save_Data.UTCDate[2]*10+Save_Data.UTCDate[3])*100000000+(Save_Data.UTCDate[0]*10+Save_Data.UTCDate[1])*1000000+
//		(Save_Data.UTCTime[0]*10+Save_Data.UTCTime[1])*10000+(Save_Data.UTCTime[2]*10+Save_Data.UTCTime[3])*100+(Save_Data.UTCTime[4]*10+Save_Data.UTCTime[5]);//GPS:UTC Date:ddmmyy
		diff_time=difftime(mktime(now_GPS),mktime(now_pcf));////////////////////next///////
		printf("%d diff_time",diff_time);
		if(abs(diff_time)<=60)//���������Χ��
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
				aes_encrypt_128(roundkeys, mingwen1, miwen1);//���ġ����ġ�����Կ
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
				printf("���ͳɹ�1\n");
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
				printf("���ճɹ�2!\n");
//				aes_decrypt_128(roundkeys,ID_Num,ID_Num);
				aes_decrypt_128(roundkeys,miwen1,miwen1);
				aes_decrypt_128(roundkeys,miwen2,miwen2);
				if(*syn==0&&miwen1[2]==(*seq+1))
				{printf("���ֳɹ���\n");}
			}
		}
		else printf("ʱ������Ԥ�� \n");continue;
/*		if(now.year==*(time+2)*10+ *(time+3))
			if(now.month==*(time+4)*10+*(time+5))
				if(now.day==*(time+6)*10+*(time+7))
	*/				
//    /*�жϽ���״̬*/
    if(status == RX_DR)
    {	
			aes_decrypt_128(roundkeys, rxbuf, rxbuf);//����
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
    printf("\r\n �ӻ��� �������ģʽ\r\n"); 
//		LCD_DispStr(10, 30, (uint8_t *)"SLAVE IN RX_MODE.", RED);	
//		ADC_ConvertedValueLocal =(float) ADC_ConvertedValue/4096*3.3; // ��ȡת����ADֵ	

//		printf("\r\n %f V \r",ADC_ConvertedValueLocal); 

    NRF_RX_Mode();
    /*�ȴ���������*/
    status = NRF_Rx_Dat(rxbuf);
		status = NRF_Rx_Dat(rxbuf+4);
		status = NRF_Rx_Dat(rxbuf+8);
		status = NRF_Rx_Dat(rxbuf+12);
//    /*�жϽ���״̬*/
    if(status == RX_DR)
    {	
			aes_decrypt_128(roundkeys, rxbuf, rxbuf);//����
      for(i=0;i<16;i++)
      {	
        printf("%2x ",rxbuf[i]);
      }
    }   
  } 
}
/*********************************************END OF FILE**********************/

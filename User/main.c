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
	u8 j,res;
		uint8_t i, r;
u8 rxbuf[16];	 //接收缓冲

	/* 128 bit key */
	uint8_t key[] = {
		0x0f, 0x15, 0x71, 0xc9, 0x47, 0xd9, 0xe8, 0x59, 
		0x0c, 0xb7, 0xad, 0xd6, 0xaf, 0x7f, 0x67, 0x98};
	uint8_t plaintext[16];// = "1305054145jrd";  
	uint8_t ciphertext[AES_BLOCK_SIZE];
	const uint8_t const_cipher[AES_BLOCK_SIZE] = {
		0xff, 0x0b, 0x84, 0x4a, 0x08, 0x53, 0xbf, 0x7c,
		0x69, 0x34, 0xab, 0x43, 0x64, 0x14, 0x8f, 0xb9};	
	uint8_t roundkeys[AES_ROUND_KEY_SIZE];
int rand_num;
		char verify_code[20]="abcde";
extern	TIME today;
		TIME now;
	_SaveData Save_Data;
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
		now =PCF8563_GetTime();	
		printf("pcf8563 time %d %d %d %d %d\n",now.year,now.month,now.day,now.hour,now.mint);	
//		UART1SendString("空闲中\r\n",strlen("空闲中\r\n"));	delay_ms(5000);
		    printf("\r\n 从机端 进入接收模式\r\n"); 
//		LCD_DispStr(10, 30, (uint8_t *)"SLAVE IN RX_MODE.", RED);	

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
	while(1)	
	{		
		parseGpsBuffer();
		printGpsBuffer();
	};

	
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

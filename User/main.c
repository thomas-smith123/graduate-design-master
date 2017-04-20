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
	u8 j,res;
		uint8_t i, r;
u8 rxbuf[16];	 //���ջ���

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
		now =PCF8563_GetTime();	
		printf("pcf8563 time %d %d %d %d %d\n",now.year,now.month,now.day,now.hour,now.mint);	
//		UART1SendString("������\r\n",strlen("������\r\n"));	delay_ms(5000);
		    printf("\r\n �ӻ��� �������ģʽ\r\n"); 
//		LCD_DispStr(10, 30, (uint8_t *)"SLAVE IN RX_MODE.", RED);	

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
	while(1)	
	{		
		parseGpsBuffer();
		printGpsBuffer();
	};

	
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

#include "pcf8563.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_i2c.h"

void PCF8563_Init(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure; 

	/* ʹ���� I2C1 �йص�ʱ�� */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1,ENABLE);  
    
  /* PB6-I2C1_SCL��PB7-I2C1_SDA*/
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6 | GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;	       // ��©���
  GPIO_Init(GPIOB, &GPIO_InitStructure);
}
static void I2C_Mode_Config(void)
{
  I2C_InitTypeDef  I2C_InitStructure; 

  /* I2C ���� */
  I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
	
	/* �ߵ�ƽ�����ȶ����͵�ƽ���ݱ仯 SCL ʱ���ߵ�ռ�ձ� */
  I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
	
  I2C_InitStructure.I2C_OwnAddress1 =I2C1_OWN_ADDRESS7; 
  I2C_InitStructure.I2C_Ack = I2C_Ack_Enable ;
	
	/* I2C��Ѱַģʽ */
  I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	
	/* ͨ������ */
  I2C_InitStructure.I2C_ClockSpeed = I2C_Speed;
  
	/* I2C1 ��ʼ�� */
  I2C_Init(I2C1, &I2C_InitStructure);
  
	/* ʹ�� I2C1 */
  I2C_Cmd(I2C1, ENABLE);   
}
void PCF8563_Write(u8 PCF_addr, u8 PCF_cmd)
{
  while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY)); // Added by Najoua 27/08/2008    
  /* Send START condition */
  I2C_GenerateSTART(I2C1, ENABLE);  
  /* Test on EV5 and clear it */
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));  
  /* Send PCF8563 address for write */
  I2C_Send7bitAddress(I2C1, Write_PCF8563, I2C_Direction_Transmitter);  
  /* Test on EV6 and clear it */
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)); 
  /* Send the PCF8563's internal address to write to */    
  I2C_SendData(I2C1, PCF_addr); 
  /* Test on EV8 and clear it */
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
  /* While there is data to be written */
	I2C_SendData(I2C1, PCF_cmd);
  /* Send STOP condition */
  I2C_GenerateSTOP(I2C1, ENABLE);
}
u8 PCF8563_Read(u8 PCF_addr)
{
	u8 rev_data;	
	I2C_GenerateSTART(I2C1, ENABLE);
	while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_MODE_SELECT)); 
	I2C_Send7bitAddress(I2C1, Write_PCF8563, I2C_Direction_Transmitter);
	while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
	I2C_SendData(I2C1, PCF_addr);
	while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_TRANSMITTED));
	I2C_GenerateSTART(I2C1, ENABLE);
	while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_MODE_SELECT)); 
	I2C_Send7bitAddress(I2C1, Read_PCF8563, I2C_Direction_Receiver);
	while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));
	while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_RECEIVED));
	rev_data = I2C_ReceiveData(I2C1);
	I2C_AcknowledgeConfig(I2C1, DISABLE);
	I2C_GenerateSTOP(I2C1, ENABLE);

	return rev_data;
	}
void PCF_SetTime(unsigned char year, unsigned char month, unsigned char day, unsigned char hour, unsigned char mint, unsigned char second)
{
//	PCF8563_Init();
	I2C_Mode_Config();	
	PCF8563_Write(control_status1,0);
	PCF8563_Write(control_status2,0);
	PCF8563_Write(Seconds, second);
	PCF8563_Write(Minutes, mint);
	PCF8563_Write(Hours, hour);
	PCF8563_Write(Days, day);
	PCF8563_Write(Months, month);
	PCF8563_Write(Years, year);
	PCF8563_Write(0x00, 0x00);  
  return;  
}
TIME PCF8563_GetTime(void)
{
  int tmp;
  TIME getTIME;
  /***********�㷨������**************/
  tmp = PCF8563_Read(Seconds);//&0x7f;
 // tmp =(tmp/16)*10+tmp%16;
	tmp=tmp&0x7f;
  getTIME.second = tmp;//????????????????

   tmp = PCF8563_Read(Minutes);//&0x7f;
//  tmp =(tmp/16)*10+tmp%16;
		tmp=tmp&0x7f;	
  getTIME.mint = tmp;

  //???????????????,??????
  tmp = PCF8563_Read(Hours);//&0x3f;
//  tmp =(tmp/16)*10+tmp%16;
		tmp=tmp&0x3f;
  getTIME.hour = tmp;

  tmp = PCF8563_Read(Days);//&0x3f;
//  tmp =(tmp/16)*10+tmp%16;
	tmp=tmp&0x3f;
  getTIME.day = tmp;

  tmp = PCF8563_Read(Months);//&0x1f;
//  tmp =(tmp/16)*10+tmp%16;
tmp=tmp&0x1f;
  getTIME.month = tmp;

  tmp = PCF8563_Read(Years);//&0x1f;
//  tmp =(tmp/16)*10+tmp%16;
  getTIME.year = tmp;
 
  return getTIME;
}

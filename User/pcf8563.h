#include "stm32f10x.h"
#ifndef __PCF8563_H
#define __PCF8563_H	
#define I2C1_OWN_ADDRESS7 0x0A
#define Write_PCF8563 0xa2
#define Read_PCF8563  0xa3
#define I2C_Speed 400000
#define control_status1 0x00
#define control_status2 0x01
#define Seconds 0x02
#define Minutes 0x03
#define Hours 0x04
#define Days 0x05
#define Weeks 0x06
#define Months 0x07
#define Years 0x08
#define Minute_Alarm 0x09
#define Hour_Alarm 0x0A
#define Day_Alarm 0x0B
#define Weekday_Alarm 0x0C
#define CLOCK_Frequency 0x0D
#define Timer_Control 0x0E
#define Timer_Countdown_Value 0x0F
typedef struct 
{
 unsigned char year;
 unsigned char month;
 unsigned char day;
 unsigned char hour;
 unsigned char mint;
 unsigned char second;
}TIME;

		//time init
void PCF8563_Init(void);
static void I2C_Mode_Config(void);
void PCF_SetTime(unsigned char year, unsigned char month, 
								 unsigned char day, unsigned char hour, 
								 unsigned char mint, unsigned char second);
TIME PCF8563_GetTime(void);
u8 PCF8563_Read(u8 rtc_addr);
#endif  

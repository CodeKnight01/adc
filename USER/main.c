#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "dht11.h"
#include "bh1750.h"
#include "stdlib.h"
#include "DS18B20.h"
#include <stdio.h>
#include "stdlib.h"
#include "adc.h"
#include "math.h"







extern u32 result_lx;
extern int stats;
char str[25];
extern u8 Res;
extern int data_send;

int main(void)
{ 
//u8 temperature,humidity; 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	delay_init(25);   
	uart_init(115200);	
	LED_Init();	
	Adc_Init();
  Init_BH1750();
//	DS18B20_Init();
	u16 adget;
 	unsigned short temperature;
	//Adc_Init();         
	ds18b20_start();
//	u16 temperatureH;
//	u16 temperatureL;
	
	
	

   
	while(1)
	{ 
                         
      LED0=!LED0;
			adget=Get_Adc(9);
	
			temperature=ds18b20_read();
		Start_BH1750();  
		delay_ms(180);
    Read_BH1750();     
    Convert_BH1750(); 

		
 //sned humidity
		if(data_send==1)
		{
		USART_SendData(USART1,0x10);
    USART_SendData(USART1,'d');
    USART_SendData(USART1,':');
		USART_SendData(USART1,adget);
		//USART_SendData(USART1,0);
		//USART_SendData(USART1,0);
		//USART_SendData(USART1,0);
		//USART_SendData(USART1,'\0');
    

    //delay_ms(10);
	 
		////send temperature
		//USART_SendData(USART1,0x10);
    //USART_SendData(USART1,'t');
    //USART_SendData(USART1,':');
		USART_SendData(USART1,temperature & 0xFF);
		USART_SendData(USART1,(temperature >> 8) & 0xFF);
		//USART_SendData(USART1,(temperature >> 16)  & 0xFF);
		//USART_SendData(USART1,(temperature >> 24)  & 0xFF);
		//USART_SendData(USART1,'\0');
    
		//delay_ms(10);
		//USART_SendData(USART1,0x10);
    //USART_SendData(USART1,'g');
    //USART_SendData(USART1,':');
		USART_SendData(USART1,result_lx & 0xFF);
		USART_SendData(USART1,(result_lx >> 8) & 0xFF);
		USART_SendData(USART1,(result_lx >> 16)  & 0xFF);
		USART_SendData(USART1,(result_lx >> 24)  & 0xFF);
		USART_SendData(USART1,'\0');
		data_send=0;
	  }
    //delay_ms(20);
		
		if(stats==1)
		{
			LED1=0;
		}
		
		if(stats==0)
		{
			LED1=1;
		}


	}

}



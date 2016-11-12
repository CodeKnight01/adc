//========================================================  
  
//        DS18B20.C    By ligh  
  
//========================================================  
  
#include "stm32f10x.h"  
#include "DS18B20.h"  
#include "delay.h"
   
  
#define EnableINT()    
#define DisableINT()   
  
#define DS_PORT   GPIOE 
#define DS_DQIO    GPIO_Pin_12
#define DS_RCC_PORT  RCC_APB2Periph_GPIOE
#define DS_PRECISION 0x7f   //�������üĴ��� 1f=9λ; 3f=10λ; 5f=11λ; 7f=12λ;  
#define DS_AlarmTH  0x64  
#define DS_AlarmTL  0x8a  
#define DS_CONVERT_TICK 1000  
  
#define ResetDQ() GPIO_ResetBits(DS_PORT,DS_DQIO)  
#define SetDQ()  GPIO_SetBits(DS_PORT,DS_DQIO)  
#define GetDQ()  GPIO_ReadInputDataBit(DS_PORT,DS_DQIO)  
   
  unsigned char TemperatureL2,TemperatureH2;    
static unsigned char TempX_TAB2[16]={0x00,0x01,0x01,0x02,0x03,0x03,0x04,0x04,0x05,0x06,0x06,0x07,0x08,0x08,0x09,0x09};  
  
  
void Delay_us2(u32 Nus)   
{    
 SysTick->LOAD=Nus*9;          //ʱ�����         
 SysTick->CTRL|=0x01;             //��ʼ����       
 while(!(SysTick->CTRL&(1<<16))); //�ȴ�ʱ�䵽��    
 SysTick->CTRL=0X00000000;        //�رռ�����   
 SysTick->VAL=0X00000000;         //��ռ�����        
}    
  
   
  
unsigned char ResetDS18B202(void)  
{  
 unsigned char resport;  
 SetDQ();  
 Delay_us2(50);  
   
 ResetDQ();  
 Delay_us2(500);  //500us ����ʱ���ʱ�䷶Χ���Դ�480��960΢�룩  
 SetDQ();  
 Delay_us2(40);  //40us  
 //resport = GetDQ();  
 while(GetDQ());  
 Delay_us2(500);  //500us  
 SetDQ();  
 return resport;  
}  
  
void DS18B20WriteByte2(unsigned char Dat)  
{  
 unsigned char i;  
 for(i=8;i>0;i--)  
 {  
   ResetDQ();     //��15u���������������ϣ�DS18B20��15-60u����  
  Delay_us2(5);    //5us  
  if(Dat & 0x01)  
   SetDQ();  
  else  
   ResetDQ();  
  Delay_us2(65);    //65us  
  SetDQ();  
  Delay_us2(2);    //������λ��Ӧ����1us  
  Dat >>= 1;   
 }   
}  
  
  
unsigned char DS18B20ReadByte2(void)  
{  
 unsigned char i,Dat;  
 SetDQ();  
 Delay_us2(5);  
 for(i=8;i>0;i--)  
 {  
   Dat >>= 1;  
    ResetDQ();     //�Ӷ�ʱ��ʼ�������ź��߱�����15u�ڣ��Ҳ�������������15u�����  
  Delay_us2(5);   //5us  
  SetDQ();  
  Delay_us2(5);   //5us  
  if(GetDQ())  
    Dat|=0x80;  
  else  
   Dat&=0x7f;    
  Delay_us2(65);   //65us  
  SetDQ();  
 }  
 return Dat;  
}  
  
  
void ReadRom2(unsigned char *Read_Addr)  
{  
 unsigned char i;  
  
 DS18B20WriteByte2(ReadROM);  
    
 for(i=8;i>0;i--)  
 {  
  *Read_Addr=DS18B20ReadByte2();  
  Read_Addr++;  
 }  
}  
  
  
void DS18B20Init2(unsigned char Precision,unsigned char AlarmTH,unsigned char AlarmTL)  
{  
 DisableINT();  
 ResetDS18B202();  
 DS18B20WriteByte2(SkipROM);   
 DS18B20WriteByte2(WriteScratchpad);  
 DS18B20WriteByte2(AlarmTL);  
 DS18B20WriteByte2(AlarmTH);  
 DS18B20WriteByte2(Precision);  
  
 ResetDS18B202();  
 DS18B20WriteByte2(SkipROM);   
 DS18B20WriteByte2(CopyScratchpad);  
 EnableINT();  
  
 while(!GetDQ());  //�ȴ�������� ///////////  
}  
  
  
void DS18B20StartConvert2(void)  
{  
 DisableINT();  
 ResetDS18B202();  
 DS18B20WriteByte2(SkipROM);   
 DS18B20WriteByte2(StartConvert);   
 EnableINT();  
}  
  
void DS18B20_Configuration2(void)  
{  
 GPIO_InitTypeDef GPIO_InitStructure;  
   
 RCC_APB2PeriphClockCmd(DS_RCC_PORT, ENABLE);  
  
 GPIO_InitStructure.GPIO_Pin = DS_DQIO;  
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD; //��©���  
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //2Mʱ���ٶ�  
 GPIO_Init(DS_PORT, &GPIO_InitStructure);  
}  
  
  
void ds18b20_start2 (void)  
{  
 DS18B20_Configuration2();  
 DS18B20Init2(DS_PRECISION, DS_AlarmTH, DS_AlarmTL);  
 DS18B20StartConvert2();  
}  
  
  
unsigned short ds18b20_read2(void)  
{  

 unsigned int  Temperature;  
  
 DisableINT();  
  ResetDS18B202();  
 DS18B20WriteByte2(SkipROM);   
 DS18B20WriteByte2(ReadScratchpad);  
 TemperatureL2=DS18B20ReadByte2();  
 TemperatureH2=DS18B20ReadByte2();   
 ResetDS18B202();  
 EnableINT();  
  
 if(TemperatureH2 & 0x80)  
  {  
  TemperatureH2=(~TemperatureH2) | 0x08;  
  TemperatureL2=~TemperatureL2+1;  
  if(TemperatureL2==0)  
   TemperatureH2+=1;  
  }  
  
 TemperatureH2=(TemperatureH2<<4)+((TemperatureL2&0xf0)>>4);  
 TemperatureL2=TempX_TAB2[TemperatureL2&0x0f];  
  
 //bit0-bit7ΪС��λ��bit8-bit14Ϊ����λ��bit15Ϊ����λ  
 Temperature=TemperatureH2*10+TemperatureL2;
  
 DS18B20StartConvert2();  
  
 return  Temperature;  
}  

This project is used to get temperature, humidity and light intense data from BH1750, ADC and DS18B20. 
Data send and receive protocol:
1.get 'D' from USART, send 11 int data. the first three data is the data initial iditification prefix : 0x10,'d',':'. The last data is the data end iditification prefix : '\0'
2.get 'O' from USART, stm32 board will control the light to turn it on.
3.get 'S' from USART, stm32 board will control the light to turn it off.

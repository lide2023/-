
#include "fat_core.h"
#include "usart.h"
#include "to_pass.h"


#define UART_RX_SIZE 500
#define Uart_TimeCount 2 //串口接收数据空闲时间计数


char Uart1_RecvBuf[UART_RX_SIZE] = {0};	   //串口接收缓存
char Uart2_RecvBuf[UART_RX_SIZE] = {0};	   //串口接收缓存
unsigned short int Uart1_RecvCount = 0;		   //串口接收计数
unsigned short int Uart2_RecvCount = 0;		   //串口接收计数
unsigned char Uart1_EnterRecvFlg = 0;		   //串口有数据接收标识
unsigned char Uart2_EnterRecvFlg = 0;		   //串口有数据接收标识

unsigned char Uart1_EnterCont = 0;		   //串口有数据接收标识
unsigned char Uart2_EnterCont = 0;		   //串口有数据接收标识



unsigned char Uart1_RecvFinishFlg = 0;		   //串口接收一包数据标识
unsigned char Uart2_RecvFinishFlg = 0;		   //串口接收一包数据2



void uart1_recive(char ch)//放到串口中断里面
 
  {
      Uart1_EnterRecvFlg=1;//有接收数据就变成1，
	  Uart1_EnterCont=0;   //有数据就清零
	 
	  if(Uart1_RecvCount<UART_RX_SIZE)
		{	   
		 Uart1_RecvBuf[Uart1_RecvCount++]=ch;
		}

  }
 
 
  
  
 void uart2_recive(char ch)//放到串口中断里面
 
  {
      Uart2_EnterRecvFlg=1;//有接收数据就变成1，
	  Uart2_EnterCont=0;   //有数据就清零
	 
	  if(Uart2_RecvCount<UART_RX_SIZE)
		{	   
		 Uart2_RecvBuf[Uart2_RecvCount++]=ch;
		}

  } 
  
  

  
void uart1_timer(void)  //放到时间终端里面
	 
 {
   if(Uart1_EnterRecvFlg==1)//在串口终端中值1
		 {
		    Uart1_EnterCont++;
			if(Uart1_EnterCont>=Uart_TimeCount)
				{
				 
				 Uart1_EnterRecvFlg=0;
				 Uart1_RecvFinishFlg=1;
				 Uart1_EnterCont=0;
				 Uart1_RecvCount = 0;
				
				 }
			
			
		 }
 
 }
 
 
 
 
 void uart2_timer(void)  //放到时间终端里面
	 
 {
   if(Uart2_EnterRecvFlg==1)//在串口终端中值1
		 {
		    Uart2_EnterCont++;
			if(Uart2_EnterCont>=Uart_TimeCount)
				{
				 
				 Uart2_EnterRecvFlg=0;
				 Uart2_RecvFinishFlg=1;
				 Uart2_EnterCont=0;
				 Uart2_RecvCount = 0;
				 }
			
		 }
 
 }

 
 
 void excue_uart(void)
 { 
  if(Uart1_RecvFinishFlg==1)
  {   
   fat_uart_send_str(Uart1_RecvBuf);//cat串口是串口2
   //FAT_DBGPRT_INFO(Uart1_RecvBuf);  
   memset(Uart1_RecvBuf, 0, UART_RX_SIZE);	  
   Uart1_RecvFinishFlg=0;
  } 
  
 if(Uart2_RecvFinishFlg==1)
  {   
   FAT_DBGPRT_INFO(Uart2_RecvBuf);
   memset(Uart2_RecvBuf, 0, UART_RX_SIZE);
	//Uart2_RecvCount = 0;  
   Uart2_RecvFinishFlg=0; 
  } 
  
 }
 
 
 
/*接收数据
 应该是设置两个接收的数组
 一个是处理命令的
 一个是处理数据的。
 
 */ 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
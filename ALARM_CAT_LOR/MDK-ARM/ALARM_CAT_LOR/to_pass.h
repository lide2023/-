#ifndef __TO_PASS_H_
#define __TOPASS_H_

#include "main.h"



//extern char Uart1_RecvBuf[UART_RX_SIZE] ;	   //串口接收缓存
//extern char Uart2_RecvBuf[UART_RX_SIZE] ;	   //串口接收缓存
//extern unsigned short int Uart1_RecvCount ;		   //串口接收计数
//extern unsigned short int Uart2_RecvCount ;		   //串口接收计数
//extern unsigned char Uart1_EnterRecvFlg ;		   //串口有数据接收标识
//extern unsigned char Uart2_EnterRecvFlg ;		   //串口有数据接收标识

//extern unsigned char Uart1_EnterCont ;		   //串口有数据接收标识
//extern unsigned char Uart2_EnterCont ;		   //串口有数据接收标识



//extern unsigned char Uart1_RecvFinishFlg;		   //串口接收一包数据标识
//extern unsigned char Uart2_RecvFinishFlg ;		   //串口接收一包数据2




void uart1_recive(char ch);//放到串口中断里面
void uart1_timer(void);  //放到时间终端里面
void uart2_recive(char ch);//放到串口中断里面
void uart2_timer(void);  //放到时间终端里面

void excue_uart(void);

#endif

#ifndef __TO_PASS_H_
#define __TOPASS_H_

#include "main.h"



//extern char Uart1_RecvBuf[UART_RX_SIZE] ;	   //���ڽ��ջ���
//extern char Uart2_RecvBuf[UART_RX_SIZE] ;	   //���ڽ��ջ���
//extern unsigned short int Uart1_RecvCount ;		   //���ڽ��ռ���
//extern unsigned short int Uart2_RecvCount ;		   //���ڽ��ռ���
//extern unsigned char Uart1_EnterRecvFlg ;		   //���������ݽ��ձ�ʶ
//extern unsigned char Uart2_EnterRecvFlg ;		   //���������ݽ��ձ�ʶ

//extern unsigned char Uart1_EnterCont ;		   //���������ݽ��ձ�ʶ
//extern unsigned char Uart2_EnterCont ;		   //���������ݽ��ձ�ʶ



//extern unsigned char Uart1_RecvFinishFlg;		   //���ڽ���һ�����ݱ�ʶ
//extern unsigned char Uart2_RecvFinishFlg ;		   //���ڽ���һ������2




void uart1_recive(char ch);//�ŵ������ж�����
void uart1_timer(void);  //�ŵ�ʱ���ն�����
void uart2_recive(char ch);//�ŵ������ж�����
void uart2_timer(void);  //�ŵ�ʱ���ն�����

void excue_uart(void);

#endif

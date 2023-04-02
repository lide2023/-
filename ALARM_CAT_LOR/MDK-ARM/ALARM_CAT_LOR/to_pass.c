
#include "fat_core.h"
#include "usart.h"
#include "to_pass.h"


#define UART_RX_SIZE 500
#define Uart_TimeCount 2 //���ڽ������ݿ���ʱ�����


char Uart1_RecvBuf[UART_RX_SIZE] = {0};	   //���ڽ��ջ���
char Uart2_RecvBuf[UART_RX_SIZE] = {0};	   //���ڽ��ջ���
unsigned short int Uart1_RecvCount = 0;		   //���ڽ��ռ���
unsigned short int Uart2_RecvCount = 0;		   //���ڽ��ռ���
unsigned char Uart1_EnterRecvFlg = 0;		   //���������ݽ��ձ�ʶ
unsigned char Uart2_EnterRecvFlg = 0;		   //���������ݽ��ձ�ʶ

unsigned char Uart1_EnterCont = 0;		   //���������ݽ��ձ�ʶ
unsigned char Uart2_EnterCont = 0;		   //���������ݽ��ձ�ʶ



unsigned char Uart1_RecvFinishFlg = 0;		   //���ڽ���һ�����ݱ�ʶ
unsigned char Uart2_RecvFinishFlg = 0;		   //���ڽ���һ������2



void uart1_recive(char ch)//�ŵ������ж�����
 
  {
      Uart1_EnterRecvFlg=1;//�н������ݾͱ��1��
	  Uart1_EnterCont=0;   //�����ݾ�����
	 
	  if(Uart1_RecvCount<UART_RX_SIZE)
		{	   
		 Uart1_RecvBuf[Uart1_RecvCount++]=ch;
		}

  }
 
 
  
  
 void uart2_recive(char ch)//�ŵ������ж�����
 
  {
      Uart2_EnterRecvFlg=1;//�н������ݾͱ��1��
	  Uart2_EnterCont=0;   //�����ݾ�����
	 
	  if(Uart2_RecvCount<UART_RX_SIZE)
		{	   
		 Uart2_RecvBuf[Uart2_RecvCount++]=ch;
		}

  } 
  
  

  
void uart1_timer(void)  //�ŵ�ʱ���ն�����
	 
 {
   if(Uart1_EnterRecvFlg==1)//�ڴ����ն���ֵ1
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
 
 
 
 
 void uart2_timer(void)  //�ŵ�ʱ���ն�����
	 
 {
   if(Uart2_EnterRecvFlg==1)//�ڴ����ն���ֵ1
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
   fat_uart_send_str(Uart1_RecvBuf);//cat�����Ǵ���2
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
 
 
 
/*��������
 Ӧ���������������յ�����
 һ���Ǵ��������
 һ���Ǵ������ݵġ�
 
 */ 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
#ifndef __FAT_CORE_H
#define __FAT_CORE_H

#define FAT_UART_RX_SIZE 50    //fat ���ڽ��ջ����С
#define FAT_UART_RX_IDLETIME 2 //fat ���ڽӿڿ���
#define FAT_TIMER_VAL 10     //fat ��ʱ�����

#define FAT_DBG_INFO_FLG 1 //FAT������Ϣ�����־

#define _FAT_DBGPRT_INFO(fmt, ...) printf(fmt, ##__VA_ARGS__)

#if FAT_DBG_INFO_FLG
#define FAT_DBGPRT_INFO(f, a...) _FAT_DBGPRT_INFO(f, ##a)
#else
#define FAT_DBGPRT_INFO(f, a...)
#endif



extern char cFatUartRecvBuf[FAT_UART_RX_SIZE];
void fat_tim_proc(void);                                                 //fat ��ʱ��������Ҫ�û������Ķ�ʱ���жϴ�����
void fat_uart_recv_proc(unsigned char ch);                               //fat ���ڽ��մ�����Ҫ�û����õĽ����жϴ�����
void reg_fat_uart_send_byte(void (*uart_send_byte)(unsigned char data)); //ע�� ���ڷ���һ���ֽں���
void fat_delay(unsigned short int val);                                  //��ʱ����

int wait_timeout(unsigned short int time); // ������ʱ�������ﵽ���ö�ʱֵʱ����1

int fat_send_wait_cmdres_blocking(char *cmd, unsigned short int timeoutval);    //��������󣬿�����ʱ�����������õĶ�ʱʱ�䵽��󷵻�1���û���ʱ���Խ��ж��������Ӧ������д���
int fat_send_wait_cmdres_nonblocking(char *cmd, unsigned short int timeoutval); //��������󣬿�����ʱ�����������õĶ�ʱʱ�䵽������������Ӧ����ʱ����1���û���ʱ���Խ��ж��������Ӧ������д���

int fat_send_wait_cmdres_nonblocking_hex(char *cmd,int length, unsigned short int timeoutval);
int fat_cmdres_keyword_matching(char *res); //������Ӧ�����в��ҹؼ���

#endif

#include "fat_core.h"
#include "stdio.h"
#include "string.h"

void fat_uart_send_byte(unsigned char data);

void (*uart_send_byte)(unsigned char data) = fat_uart_send_byte; //���崮�ڷ���һ���ֽ����ݺ���ָ��

char cFatUartRecvBuf[FAT_UART_RX_SIZE] = {0};	   //���ڽ��ջ���
unsigned short int usFatUartRecvCount = 0;		   //���ڽ��ռ���
unsigned char ucFATUartEnterRecvFlg = 0;		   //���������ݽ��ձ�ʶ
unsigned short int usFatUartRecvIdleTimeCount = 0; //���ڽ������ݿ���ʱ�����
unsigned char ucFatUartRecvFinishFlg = 0;		   //���ڽ���һ�����ݱ�ʶ

unsigned short int usDelayCount = 0;	  // ��ʱ����
unsigned char ucTimeOutStartFlg = 0;	  // ������ʱ������ʶ��0����ʱ����δ������1����ʱ����������
unsigned short int usTimeOutCount = 0;	  // ��ʱʱ�����
unsigned short int usTimeOutVal = 0;	  // ��ʱʱ��ֵ
unsigned char ucTimeOutValReachedFlg = 0; // ��ʱ�����ﵽ��ʱֵ��ʶ

unsigned char ucSendCmdWaitOutTimeFlg = 0; //��������ȴ���ʱʱ�䵽�0��uart����һ֡��ﵽ���õĳ�ʱʱ��Ŵ���1���ﵽ�����õĳ�ʱʱ��Ŵ���
unsigned char ucOverTimeSetFlg = 0;		   //���ó�ʱ��ʶ

/**
 * @description: ����һ���ֽ�����
 * @param data: Ҫ���������
 * @return None
   Ҳ���Ƕ�����һ���պ��������������������ã����ڸ�������һ����ֵ�ͻ��в�һ���Ĺ��ܣ�Ҳ�����Ƕ�����һ�����������������û�и�ֵ�����ڿ��Ը�������һ����ֵ
 */
void fat_uart_send_byte(unsigned char data) {}

/**
 * @description: �����ַ���
 * @param Ҫ������ַ���
 * @return None
 */
void fat_uart_send_str(unsigned char *buf)
{
	while (*buf != '\0')
	{
		uart_send_byte(*buf++);
	}
}

/**
 * @description: �����ַ�����
 * @param data:	Ҫ������ַ�����
 * @param length: Ҫ���͵ĳ���
 * @return None
 */
void fat_uart_send_buf(unsigned char *data, int length)
{
	for (int i = 0; i < length; i++)
	{
		uart_send_byte(data[i]);
	}
}

/**
 * @description: ע��UART�������ݺ���
 * @param uart_send_byte: ���ͺ���ԭ��
 * @return None
 */
void reg_fat_uart_send_byte(void (*_uart_send_byte)(unsigned char data))
{
	if (_uart_send_byte != NULL)
	{
		uart_send_byte = _uart_send_byte;
	}
}

/**
 * @description: ���ڽ����жϴ���
 * @param ch: ���յ�����
 * @return None
 */
void fat_uart_recv_proc(unsigned char ch)//�ŵ����ڽ����ж�����
{
	ucFATUartEnterRecvFlg = 1;//ʱ������꿪ʼ��־�������ݣ�ʱ���ж�����usFatUartRecvIdleTimeCount�Ϳ�ʼ������
	usFatUartRecvIdleTimeCount = 0;//�����ݵ��˱�־λ�����㣬���ݲ����ˣ�ʱ���ж�����ͼӼӣ���һ����ʱ��
	if (usFatUartRecvCount < FAT_UART_RX_SIZE) //��ֹ���
	{
		cFatUartRecvBuf[usFatUartRecvCount++] = ch;
	}
}

/**
 * @description: ���uart���ջ��沢������ر�־λ
 * @param None
 * @return None
 */
void fat_uart_clean(void)
{
	usFatUartRecvCount = 0;
	ucFatUartRecvFinishFlg = 0;
	memset(cFatUartRecvBuf, 0, FAT_UART_RX_SIZE);
}

/**
 * @description: ��ʱ��������
 * @param None
 * @return None
 */
void fat_tim_proc(void)//�ŵ�ʱ���ж���
{
	usDelayCount++;

	/* ���ڽ��տ��д����ﵽ���õĴ��ڽ��տ���ʱ����Ϊ���յ�һ֡���� */
	if (ucFATUartEnterRecvFlg)
	{
		usFatUartRecvIdleTimeCount++;
		if (usFatUartRecvIdleTimeCount > FAT_UART_RX_IDLETIME)//���ڿ��дﵽһ��ʱ�䣬set������ϱ�־λ
		{
			ucFATUartEnterRecvFlg = 0;//�رռ�ʱ���أ��´����������ˣ��ڴ��ڴ������ݺ����ڴ�,�ȴ����ڽ���������ϣ���ʼ��ʱ
			ucFatUartRecvFinishFlg = 1;//���ڽ�����ϱ�־��λ�����崮���ڴ�ʱ����˱�־λ
			usFatUartRecvIdleTimeCount = 0;
		}
	}

	/* ������ʱ���� */
	if (ucTimeOutStartFlg)
	{
		
		usTimeOutCount++;
		
		if (usTimeOutCount > usTimeOutVal)
		{
			ucTimeOutValReachedFlg = 1;
		}
		
	}
}



/**
 * @description: ��������󣬿�����ʱ�����������õĶ�ʱʱ�䵽��󷵻�1���û���ʱ���Խ��ж��������Ӧ������д���
 * @param cmd��Ҫ���͵�����
 * @param timeoutval����ʱʱ��
 * @return 0��δ���ﶨʱʱ�䣬1����ʱʱ�䵽��
 */
int fat_send_wait_cmdres_blocking(char *cmd, unsigned short int timeoutval)
{
	/* �������������ʱ���� */
	if (ucTimeOutStartFlg == 0)
	{
		FAT_DBGPRT_INFO("uart send: %s\r\n",cmd);//����˭�ģ�
		fat_uart_clean();//��������֮ǰ������ջ���ͽ��յ�����
		fat_uart_send_str((unsigned char *)cmd);
		usTimeOutVal = timeoutval / FAT_TIMER_VAL;
		ucTimeOutStartFlg = 1;//������ʱ
	}

	/* �����趨�Ķ�ʱʱ�� */
	if (ucTimeOutValReachedFlg)
	{
		ucTimeOutValReachedFlg = 0;
		ucTimeOutStartFlg = 0;
		usTimeOutCount = 0;
		return 1;
	}
	return 0;
}

/**
 * @description: ��������󣬿�����ʱ�����������õĶ�ʱʱ�䵽������������Ӧ����ʱ����1���û���ʱ���Խ��ж��������Ӧ������д���
 * @param cmd: Ҫ���͵�����
 * @param timeoutval: ��ʱʱ��
 * @return 0:δ�ﵽ��ʱʱ�䣬1����ʱʱ�䵽�����������Ӧ����
 */
int fat_send_wait_cmdres_nonblocking(char *cmd, unsigned short int timeoutval)
{
	/* �������������ʱ���� */
	if (ucTimeOutStartFlg == 0)
	{
		FAT_DBGPRT_INFO("uart send: %s\r\n",cmd);
		fat_uart_clean();//��������֮ǰ������ջ���ͽ��յ�����
		fat_uart_send_str((unsigned char *)cmd);
		usTimeOutVal = timeoutval / FAT_TIMER_VAL;
		ucTimeOutStartFlg = 1;
	}
	/* �����趨�Ķ�ʱʱ�����յ�һ֡���� */
	if (ucTimeOutValReachedFlg || ucFatUartRecvFinishFlg)//�����־�ڴ��ڽ��պ��������ʱ��û���µ����ݵ����͵���1
	{
		ucTimeOutValReachedFlg = 0;
		ucTimeOutStartFlg = 0;
		usTimeOutCount = 0;
		return 1;
	}
	return 0;
}


/**
 * @description: ��������󣬿�����ʱ�����������õĶ�ʱʱ�䵽������������Ӧ����ʱ����1���û���ʱ���Խ��ж��������Ӧ������д���
 * @param cmd: Ҫ���͵�����
 * @param timeoutval: ��ʱʱ��
 * @return 0:δ�ﵽ��ʱʱ�䣬1����ʱʱ�䵽�����������Ӧ����
 */
int fat_send_wait_cmdres_nonblocking_hex(char *cmd,int length,unsigned short int timeoutval)
{
	/* �������������ʱ���� */
	if (ucTimeOutStartFlg == 0)
	{
		FAT_DBGPRT_INFO("uart send: %s\r\n",cmd);
		fat_uart_clean();//��������֮ǰ������ջ���ͽ��յ�����
		//fat_uart_send_str((unsigned char *)cmd);
        fat_uart_send_buf((unsigned char *)cmd,length);
        
		usTimeOutVal = timeoutval / FAT_TIMER_VAL;
		ucTimeOutStartFlg = 1;
	}
	/* �����趨�Ķ�ʱʱ�����յ�һ֡���� */
	if (ucTimeOutValReachedFlg || ucFatUartRecvFinishFlg)//�����־�ڴ��ڽ��պ��������ʱ��û���µ����ݵ����͵���1
	{
		ucTimeOutValReachedFlg = 0;
		ucTimeOutStartFlg = 0;
		usTimeOutCount = 0;
		return 1;
	}
	return 0;
}




/**
 * @description: ��ʱ�������������ö�ʱ��ʱ�����й�
 * @param val:��ʱʱ�� val * ��ʱ��ʱ����
 * @return None
 */
void fat_delay(unsigned short int val)
{
	usDelayCount = 0;
	while (usDelayCount < val / FAT_TIMER_VAL)
	{
	}
}

/**
 * @description: ������ʱ�������ﵽ���ö�ʱֵʱ����1
 * @param timeoutval:��ʱʱ��
 * @return 0:δ�ﵽ����ʱ��, 1:�Ѵﵽ����ʱ��
 */
int wait_timeout(unsigned short int timeoutval)
{
	/* ��ʼ��ʱ������ */
	if (ucTimeOutStartFlg == 0)
	{
		usTimeOutVal = timeoutval / FAT_TIMER_VAL;
		ucTimeOutStartFlg = 1;
	}
	/* ��ʱʱ�䵽�� */
	if (ucTimeOutValReachedFlg)
	{
		ucTimeOutValReachedFlg = 0;
		ucTimeOutStartFlg = 0;
		usTimeOutCount = 0;
		return 1;
	}
	return 0;
}

/**
* @description: ������Ӧ�����в��ҹؼ���
* @param <keyword> Ҫ�������Ĺؼ���
* @return 0��δ�������ؼ��֣� 1���������˹ؼ���
*/
int fat_cmdres_keyword_matching(char *keyword)
{
	/* ��������Ӧ������Ӧ�����в��ҹؼ��� */
	if (ucFatUartRecvFinishFlg)
	{
		FAT_DBGPRT_INFO("uart recv: %s\r\n",cFatUartRecvBuf);
		/* �����������ؼ��� */
		if (strstr((const char *)(cFatUartRecvBuf), keyword) != NULL)
		{
			return 1;
		}
		ucFatUartRecvFinishFlg = 0;
	}
	return 0;
}



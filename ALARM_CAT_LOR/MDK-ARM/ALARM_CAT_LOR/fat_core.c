#include "fat_core.h"
#include "stdio.h"
#include "string.h"

void fat_uart_send_byte(unsigned char data);

void (*uart_send_byte)(unsigned char data) = fat_uart_send_byte; //定义串口发送一个字节数据函数指针

char cFatUartRecvBuf[FAT_UART_RX_SIZE] = {0};	   //串口接收缓存
unsigned short int usFatUartRecvCount = 0;		   //串口接收计数
unsigned char ucFATUartEnterRecvFlg = 0;		   //串口有数据接收标识
unsigned short int usFatUartRecvIdleTimeCount = 0; //串口接收数据空闲时间计数
unsigned char ucFatUartRecvFinishFlg = 0;		   //串口接收一包数据标识

unsigned short int usDelayCount = 0;	  // 延时计数
unsigned char ucTimeOutStartFlg = 0;	  // 开启定时计数标识，0：定时计数未开启，1，定时计数开启中
unsigned short int usTimeOutCount = 0;	  // 定时时间计数
unsigned short int usTimeOutVal = 0;	  // 定时时间值
unsigned char ucTimeOutValReachedFlg = 0; // 定时计数达到定时值标识

unsigned char ucSendCmdWaitOutTimeFlg = 0; //发送命令等待超时时间到达，0：uart接收一帧或达到设置的超时时间才处理，1：达到了设置的超时时间才处理
unsigned char ucOverTimeSetFlg = 0;		   //设置超时标识

/**
 * @description: 发送一个字节数据
 * @param data: 要传输的数据
 * @return None
   也就是定义了一个空函数，方便其他函数调用，后期给它赋不一样的值就会有不一样的功能，也就像是定义了一个变量，这个变量并没有赋值，后期可以给他付不一样的值
 */
void fat_uart_send_byte(unsigned char data) {}

/**
 * @description: 发送字符串
 * @param 要传输的字符串
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
 * @description: 发送字符数组
 * @param data:	要传输的字符数组
 * @param length: 要发送的长度
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
 * @description: 注册UART发送数据函数
 * @param uart_send_byte: 发送函数原型
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
 * @description: 串口接收中断处理
 * @param ch: 接收的数据
 * @return None
 */
void fat_uart_recv_proc(unsigned char ch)//放到串口接收中断里面
{
	ucFATUartEnterRecvFlg = 1;//时间计数标开始标志，来数据，时间中断里面usFatUartRecvIdleTimeCount就开始计数。
	usFatUartRecvIdleTimeCount = 0;//有数据到此标志位就清零，数据不传了，时间中断里面就加加，到一定的时间
	if (usFatUartRecvCount < FAT_UART_RX_SIZE) //防止溢出
	{
		cFatUartRecvBuf[usFatUartRecvCount++] = ch;
	}
}

/**
 * @description: 清空uart接收缓存并重置相关标志位
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
 * @description: 定时器处理函数
 * @param None
 * @return None
 */
void fat_tim_proc(void)//放到时间中断中
{
	usDelayCount++;

	/* 串口接收空闲处理，达到设置的串口接收空闲时间认为接收到一帧数据 */
	if (ucFATUartEnterRecvFlg)
	{
		usFatUartRecvIdleTimeCount++;
		if (usFatUartRecvIdleTimeCount > FAT_UART_RX_IDLETIME)//串口空闲达到一定时间，set接收完毕标志位
		{
			ucFATUartEnterRecvFlg = 0;//关闭计时开关，下次有数据来了，在串口处理数据函数在打开,等待串口接收数据完毕，开始计时
			ucFatUartRecvFinishFlg = 1;//串口接收完毕标志置位，在清串口内存时清除此标志位
			usFatUartRecvIdleTimeCount = 0;
		}
	}

	/* 开启定时计数 */
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
 * @description: 发送命令后，开启定时计数，在设置的定时时间到达后返回1，用户此时可以进行对命令的响应结果进行处理
 * @param cmd：要发送的命令
 * @param timeoutval：定时时间
 * @return 0：未到达定时时间，1：定时时间到达
 */
int fat_send_wait_cmdres_blocking(char *cmd, unsigned short int timeoutval)
{
	/* 发送命令，开启定时计数 */
	if (ucTimeOutStartFlg == 0)
	{
		FAT_DBGPRT_INFO("uart send: %s\r\n",cmd);//发给谁的？
		fat_uart_clean();//发送命令之前清除接收缓存和接收到命令
		fat_uart_send_str((unsigned char *)cmd);
		usTimeOutVal = timeoutval / FAT_TIMER_VAL;
		ucTimeOutStartFlg = 1;//启动计时
	}

	/* 到达设定的定时时间 */
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
 * @description: 发送命令后，开启定时计数，在设置的定时时间到达后或命令有响应数据时返回1，用户此时可以进行对命令的响应结果进行处理
 * @param cmd: 要发送的命令
 * @param timeoutval: 定时时间
 * @return 0:未达到定时时间，1：定时时间到达或命令有响应数据
 */
int fat_send_wait_cmdres_nonblocking(char *cmd, unsigned short int timeoutval)
{
	/* 发送命令，开启定时计数 */
	if (ucTimeOutStartFlg == 0)
	{
		FAT_DBGPRT_INFO("uart send: %s\r\n",cmd);
		fat_uart_clean();//发送命令之前清除接收缓存和接收到命令
		fat_uart_send_str((unsigned char *)cmd);
		usTimeOutVal = timeoutval / FAT_TIMER_VAL;
		ucTimeOutStartFlg = 1;
	}
	/* 到达设定的定时时间或接收到一帧数据 */
	if (ucTimeOutValReachedFlg || ucFatUartRecvFinishFlg)//这个标志在串口接收函数里，超过时间没有新的数据到来就等于1
	{
		ucTimeOutValReachedFlg = 0;
		ucTimeOutStartFlg = 0;
		usTimeOutCount = 0;
		return 1;
	}
	return 0;
}


/**
 * @description: 发送命令后，开启定时计数，在设置的定时时间到达后或命令有响应数据时返回1，用户此时可以进行对命令的响应结果进行处理
 * @param cmd: 要发送的命令
 * @param timeoutval: 定时时间
 * @return 0:未达到定时时间，1：定时时间到达或命令有响应数据
 */
int fat_send_wait_cmdres_nonblocking_hex(char *cmd,int length,unsigned short int timeoutval)
{
	/* 发送命令，开启定时计数 */
	if (ucTimeOutStartFlg == 0)
	{
		FAT_DBGPRT_INFO("uart send: %s\r\n",cmd);
		fat_uart_clean();//发送命令之前清除接收缓存和接收到命令
		//fat_uart_send_str((unsigned char *)cmd);
        fat_uart_send_buf((unsigned char *)cmd,length);
        
		usTimeOutVal = timeoutval / FAT_TIMER_VAL;
		ucTimeOutStartFlg = 1;
	}
	/* 到达设定的定时时间或接收到一帧数据 */
	if (ucTimeOutValReachedFlg || ucFatUartRecvFinishFlg)//这个标志在串口接收函数里，超过时间没有新的数据到来就等于1
	{
		ucTimeOutValReachedFlg = 0;
		ucTimeOutStartFlg = 0;
		usTimeOutCount = 0;
		return 1;
	}
	return 0;
}




/**
 * @description: 延时函数，根据所用定时器时间间隔有关
 * @param val:延时时间 val * 定时器时间间隔
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
 * @description: 开启定时计数，达到设置定时值时返回1
 * @param timeoutval:定时时间
 * @return 0:未达到设置时间, 1:已达到设置时间
 */
int wait_timeout(unsigned short int timeoutval)
{
	/* 开始定时器计数 */
	if (ucTimeOutStartFlg == 0)
	{
		usTimeOutVal = timeoutval / FAT_TIMER_VAL;
		ucTimeOutStartFlg = 1;
	}
	/* 定时时间到达 */
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
* @description: 命令响应数据中查找关键字
* @param <keyword> 要被检索的关键字
* @return 0：未检索到关键字， 1：检索到了关键字
*/
int fat_cmdres_keyword_matching(char *keyword)
{
	/* 命令有响应，在响应数据中查找关键字 */
	if (ucFatUartRecvFinishFlg)
	{
		FAT_DBGPRT_INFO("uart recv: %s\r\n",cFatUartRecvBuf);
		/* 检索检索到关键词 */
		if (strstr((const char *)(cFatUartRecvBuf), keyword) != NULL)
		{
			return 1;
		}
		ucFatUartRecvFinishFlg = 0;
	}
	return 0;
}



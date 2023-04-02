/********************************************************************************
* @file    	  lte.c
* @function   LTE模块 AT命令配置
* @brief 	    用3.5.0版本库建的工程模板
* @attention	该例程仅作为参考，如需实际开发，请根据实际需求更改
* @version 	  V1.1
* @company  	深圳市飞思创电子科技有限公司
* @website  	http://www.freestrong.com
* @tel				0755-86528386
* @Author     freestrong
* @date    		2020/11/21
********************************************************************************/
#include "string.h"
#include "stdlib.h"
#include "usart.h"
#include "gpio.h"
#include "lte_demo.h"
#include "fat_core.h"

#include "stdio.h"


#define LTE_POWER_ON_WAIT_TIME 2000 					//LTE开机等待时间
#define SIGNALMIN 15                                    //信号质量最低阀值
#define SIGNALMAX 31                                    //信号质量最低阀值
#define SOCKET_BUF_SIZE 128                             //Socket数据缓存大小
char cSocketRecvBuf[SOCKET_BUF_SIZE] = {0};             //socket数据接收缓存
char cSocketSendBuf[SOCKET_BUF_SIZE] = {0};             //socket数据发送缓存
uint8_t ucStateNum = 0;                                 //命令执行顺序标识值
uint8_t retrytimes = 0;                                 //命令重试次数
uint8_t ucErrorTimes = 0;                               //错误次数累计值
uint8_t ucFlightModeTimes = 0;							//进入飞行模式次数

typedef enum
{
    MD_RESET,        				//复位模块
    MD_AT_REQ,       				//AT握手
    MD_WORK_STA_CHK, 				//工作状态检测
    MD_CONNETINIT,   				//连接配置信息初始化
    MD_CONNETED,     				//数据通信
	MD_FLIGHTMODE,			   	    //飞行模式
    MD_OK = 0xFE,    				//正常
    MD_ERR = 0xFF,   				//异常
} MD_RUN_STATE;



typedef struct    
{
	unsigned char IMEI[20];//IMEI
	unsigned char pubtopic[25];
	unsigned char subtopic[25];
	unsigned char username[12];
	unsigned char passwore[12];
	unsigned char upIMEI[47];
}EC600u_init; 

EC600u_init EC600uinit;





/**
 * @description:复位LTE模块
 * @param None
 * @return None
 */
int module_reset(void)
{
    switch (ucStateNum)
    {
    //拉低PEN引脚
    case 0:
        PEN_GPIO_SET_LOW;
        ucStateNum++;
        break;
    //拉高PEN引脚
    case 1:
        if (wait_timeout(1000))
        {
            PEN_GPIO_SET_HIGH;
            ucStateNum = 0;
            return 1;
        }
        break;
    default:
        break;
    }
    return 0;
}

/**
 * @description:
 * @param str：要检索的字符串
 * @param minval：要匹配信号质量区间最小值
 * @param minval：要匹配信号质量区间最大值
 * @return 0:信号质量不满足正常工作状态, 1:信号质量满足正常工作状态
 */
int match_csq(char *str, int minval, int maxval)
{
    int lpCsqVal = 0;
    char tmp[5] = {0};
    char *p = NULL, *q = NULL;
		
    p = strstr(str, "+CSQ:");
    if (p == NULL)
    {
        return 0;
    }
		
    p = p + 5;//读取+CSQ:后面的数值
	
	
    while (*p < 0x30 || *p > 0x39)//判断后面几个数字 0x30是0的16进制表示，0x30是9的16进制
      {
        p++;
      }
    q = p;

    while (*p != ',')
    {
        p++;
    }
	
	
    memcpy(tmp, q, p - q);//内存复制函数，destination, source
    lpCsqVal = atoi(tmp); //字符串 转换成整数
    /* 判断信号质量是否在设置的区间内 */
    if (lpCsqVal >= minval && lpCsqVal <= maxval)
    {
        return 1;
    }
    return 0;
}

/**
 * @description: 检测模块工作状态是否就绪
 * @param None
 * @return 0：检测未完成；MD_OK：模块已就绪；MD_ERR：错误，不满足工作状态
 */
int module_is_ready(void)
{
    switch (ucStateNum)
    {
    //关闭AT命令回显
    case 0x00:
        if (fat_send_wait_cmdres_blocking("ATE0\r\n", 500))
        {
						//收到OK
            if (fat_cmdres_keyword_matching("OK"))//命令响应数据中查找关键字
            {
                ucErrorTimes = 0;
                ucStateNum++;
            }
            else
            {
								//发送10次得不到正确应答
                if (ucErrorTimes++ > 10)
                {
                    ucStateNum = MD_ERR;
                }
            }
        }
        break;
    //读卡
    case 0x01:
        if (fat_send_wait_cmdres_blocking("AT+CPIN?\r\n", 500))
        {
						//收到+CPIN: READY
            if (fat_cmdres_keyword_matching("+CPIN: READY"))
            {
                ucErrorTimes = 0;
                ucStateNum++;
            }
            else
            {     
								//发送10次得不到正确应答
                if (ucErrorTimes++ > 10)
                {
                    ucStateNum = MD_ERR;
                }
            }
        }
        break;
    //查询信号质量
    case 0x02:
        if (fat_send_wait_cmdres_blocking("AT+CSQ\r\n", 200))
			{
							//收到OK
				if (fat_cmdres_keyword_matching("OK"))
				{
								//收到的是99（射频信号未初始化）
								if (fat_cmdres_keyword_matching("+CSQ: 99,99"))
									{
											//发送30次得不到正确应答
											if (ucErrorTimes++ > 30)
											{
													ucStateNum = MD_ERR;
											}
									}
							  else
								{    
														//信号值在SIGNALMIN~SIGNALMAX这个区间
										if (match_csq(cFatUartRecvBuf, SIGNALMIN, SIGNALMAX))
										  {
										   ucErrorTimes = 0;
										   ucStateNum++;
										  }
										else 
										 {
											 ucStateNum = MD_ERR;
										 }
								}
				}
						//没收到应答
            else
				{
									//发送30次不应答
					if (ucErrorTimes++ > 30)
					{
						ucStateNum = MD_ERR;
					}
				}
        }
        break;
				
		//读取IMEI号
		case 0x03:
					if (fat_send_wait_cmdres_blocking("AT+GSN\r\n", 500))
							
						{
										
							if(cFatUartRecvBuf[2]>='0'&&cFatUartRecvBuf[2]<='9')//获取模块IMEI的值获取的值，本身就是字符型的
							{
								memset(( char *)EC600uinit.pubtopic,0,sizeof(EC600uinit.pubtopic));//清零内存	最快的清内存函数 
								memset(( char *)EC600uinit.subtopic,0,sizeof(EC600uinit.subtopic));//清零内存	最快的清内存函数  
								memset(( char *)EC600uinit.IMEI,0,sizeof(EC600uinit.IMEI));        //清零内存	最快的清内存函数  	
								
								
								memcpy(( char *)EC600uinit.IMEI,cFatUartRecvBuf+2,15);//内存拷贝	从ec600_RxBuffer【2】开始拷贝15个数到	EC600uinit.IMEI数组中											
								sprintf(( char *)EC600uinit.pubtopic,"app/%s",EC600uinit.IMEI);//%s站位符号，数据结构对应起来
								sprintf(( char *)EC600uinit.subtopic,"dev/%s",EC600uinit.IMEI);
				
								ucErrorTimes = 0;
								ucStateNum++;
										
							}
							else
								{
									//发送3次得不到正确应答
									if (ucErrorTimes++ > 3)
									{
										ucStateNum = MD_ERR;
									}
								}
						}
	
		break;	
				
			//查看当前GPRS附着状态
    case 0x04:
        if (fat_send_wait_cmdres_blocking("AT+CGATT?\r\n", 500))
        {
						//收到+CGATT: 1
            if (fat_cmdres_keyword_matching("+CGATT: 1"))
               {
                ucErrorTimes = 0;
				        ucStateNum = MD_OK;
                }
            else
             {   
						//发送30次得不到正确应答
            if (ucErrorTimes++ > 30)
                  { 
                    ucStateNum = MD_ERR;
                  }
            }
        }
        break;
				
		//错误跳至飞行模式
		case MD_ERR:
				ucStateNum = 0;
				return MD_ERR;
    //完成
        case MD_OK:
				ucStateNum = 0;
        return MD_OK;
        default:
        break;
    }
    return 0;
}

/**
 * @description: 连接和相关配置初始化
 * @param None
 * @return 0：检测未完成；MD_OK：模块已就绪；MD_ERR：错误，不满足工作状态
 */
int module_connet_parm_init(void)
{
    switch (ucStateNum)
    {
		//配置接收模式:从服务器接收的 MQTT 消息不以 URC 的形式上报
    case 0x00:
				//AT+QMTCFG="recv/mode",<client_idx>[,<msg_recv_mode>[,<msg_len_enable>]]
     //AT+QMTCFG="dataformat",0,0,0   
    if (fat_send_wait_cmdres_blocking("AT+QMTCFG=\"dataformat\",0,1,1\r\n", 500))
    //if (fat_send_wait_cmdres_blocking("AT+QMTCFG=\"recv/mode\",0,1,1\r\n", 200))
        {
						//收到OK
            if (fat_cmdres_keyword_matching("OK"))
            {
                ucErrorTimes = 0;
                ucStateNum++;
            }
            else
            {
								//发送10次得不到正确应答
                if (ucErrorTimes++ > 10)
                {
                    ucStateNum = MD_ERR;
                }
            }
        }
        break;
    //打开MQTT客户端网络
    case 0x01:
        if (fat_send_wait_cmdres_blocking("AT+QMTOPEN=0,\"bangmuyun.com\",1883\r\n", 500))
        {
						//收到+QMTOPEN: 0,0
            if (fat_cmdres_keyword_matching("+QMTOPEN: 0,0"))
            {
                ucErrorTimes = 0;
                ucStateNum++;
            }
            else
            {
								//发送10次得不到正确应答
                if (ucErrorTimes++ > 10)
                {
                    ucStateNum = MD_ERR;
                }
            }
        }
        break;
    //将MQTT客户端连接服务器
    case 0x02:
			  memset(EC600uinit.upIMEI,0,sizeof(EC600uinit.upIMEI));
		    sprintf(EC600uinit.upIMEI,"AT+QMTCONN=0,\"%s\",\"device\",\"123456\"\r\n",EC600uinit.IMEI);// \"\" 就是表示双引号，s%表示占位符,12345678是密码  debice是用户名
        if (fat_send_wait_cmdres_blocking(EC600uinit.upIMEI, 500))
        {
						//收到+QMTCONN: 0,0,0
            if (fat_cmdres_keyword_matching("+QMTCONN: 0,0,0"))
            {
                ucErrorTimes = 0;
                ucStateNum++;
            }
            else
            {
								//发送10次得不到正确应答
                if (ucErrorTimes++ > 10)
                {
                    ucStateNum = MD_ERR;
                }
            }
        }
        break;
		//订阅主题
    case 0x03:
    memset(EC600uinit.upIMEI,0,sizeof(EC600uinit.upIMEI));    
		sprintf(EC600uinit.upIMEI,"AT+QMTSUB=0,1,\"%s\",0\r\n",EC600uinit.subtopic);//订阅主题    
		if (fat_send_wait_cmdres_blocking(EC600uinit.upIMEI, 500))
        {
						//收到+QMTSUB: 0,1,0,0
            if (fat_cmdres_keyword_matching("+QMTSUB: 0,1,0,0"))
            {
                ucErrorTimes = 0;
                ucStateNum = MD_OK;
            }
            else
            {
								//发送5次得不到正确应答
                if (ucErrorTimes++ > 5)
                {
                    ucStateNum = MD_ERR;
                }
            }
        }
        break;
    //完成
    case MD_OK:
				ucStateNum = 0;
        return MD_OK;
    //错误跳至飞行模式
    case MD_ERR:
				ucStateNum = 0;
        return MD_ERR;
    default:
        break;
    }
    return 0;
}

/**
 * @description: 截取socket接收缓存区的Topic
 * @param cmdres：命令响应数据
 * @param recvbuf：socket接收缓存
 * @return 接收Topic的长度
   从报文标志位向后查0x2c;
 +QMTRECV: 0,0,"/public/test",12,"A55A20000001"

 */
int match_recv_topic(char *cmdres, char *recvbuf)
{
    int length = 0;
    char *p = NULL, *q = NULL;
    p = strstr(cmdres, "+QMTRECV: 0,0,\"");//这个QMTRECV就是标志收到数据
    if (p == NULL)
    {
        return 0;
    }
    p = p + 15;//这个15是模块的EMI码的位数;
    q = p;
    while (*p != 0x2C)//ASC码的02C是什么？是“，”
    {
        p++;
    }
    length = p - q - 1;
    memset(recvbuf, 0, SOCKET_BUF_SIZE);//内存清零
    memcpy(recvbuf, q, length);
    return length;
}

/**
 * @description: 截取socket接收缓存区的Topic的内容
 * @param cmdres：命令响应数据
 * @param recvbuf：socket接收缓存
 * @return 接收数据的长度
   从报文后端0K向前查到0x22;
 */
int match_recv_data(char *cmdres, char *recvbuf)
{
    int length = 0;
    char *p = NULL, *q = NULL;
		//定位至应答的OK
    p = strstr(cmdres, "OK");
    if (p == NULL)
    {
        return 0;
    }
		//定位至Topic内容后的"
		p = p - 5;
		q = p;
		do
		{
        p--;
    }
    while (*p != 0x22);//ASC码的02C是什么？是“"”
    length = q - p - 1;	
    memset(recvbuf, 0, SOCKET_BUF_SIZE);
    memcpy(recvbuf, p + 1, length);
    return length;
}

/**
 * @description: 数据收发部分
 * @return 0：检测未完成；MD_ERR：错误
 */


//按下面这个历程写，结构紧凑，且方便。
//把发送数据作为一个线程去做
//查询发送数据有一个好处是不会和发送数据掺和到一起

int module_data(void)
{
    int ret = 0;//数据长度
    uint8_t data_len;//发送数据的长度，定长发送需要知道数据长度
    static uint8_t sand_dat_int[10]={"helloo"};
    static uint32_t add_test=0;
    static uint8_t wei_ling[4]="he";
    static uint8_t sand_dat_c[30];//这个必须是静态变量 因为它在第二线程里面使用
    //static char *sand_dat_c;
    switch (ucStateNum)
    {
    //定长主题发布数据
    /*整形数组大小：sizeof(数组名)/sizeof(数组类型)。
      字符数组大小：strlen(数组名)+1,1因为要加上'/0'，所以再加1.
      data_len = strlen(Addr_Buf)/sizeof(char);//计算数据长度  
    */
    case 0x00:
            /*需要增加计算数据长度的语句*/ 
            //memcpy(Lora_send_buf, (uint8_t *)&ADC_V1_AVR, 2); ADC_V1_AVR是16位整形变量，组帧语句特别好用
            
            //sprintf(sand_dat_c,"%d",add_test);//sprintf主要作用是吧变量数值转化成字符型
            //sprintf(sand_dat_c,"%d","hello");//sprintf给数组赋值这样也是对的，用strcpy()也可以给数组赋值
            //指针赋值，这样是对的sand_dat_c="heloo";
            
            memcpy(sand_dat_c+3,(uint8_t *)&add_test,1);
            memcpy(sand_dat_c+2,(uint8_t *)&add_test+1,1);
            memcpy(sand_dat_c+1,(uint8_t *)&add_test+2,1);
            memcpy(sand_dat_c,(uint8_t *)&add_test+3,1);
//          memcpy(sand_dat_c+2,(uint8_t *)&wei_ling,4);
            data_len = strlen(sand_dat_c);
            memset(EC600uinit.upIMEI,0,sizeof(EC600uinit.upIMEI));		
		    //sprintf(EC600uinit.upIMEI,"AT+QMTPUBEX=0,0,0,0,\"%s\",%d\r\n",EC600uinit.pubtopic,data_len);
		    sprintf((char *)EC600uinit.upIMEI,"AT+QMTPUBEX=0,0,0,0,\"%s\",%d\r\n",EC600uinit.pubtopic,4);//需要计算数据长度
       

    if (fat_send_wait_cmdres_nonblocking((char *)EC600uinit.upIMEI, 1000))
        {
						//收到>
            if (fat_cmdres_keyword_matching(">"))
            {
                ucErrorTimes = 0;
                ucStateNum++;
            }
            else
            {
								//发送3次得不到正确应答
                if (ucErrorTimes++ > 3)
                {
                    ucStateNum = MD_ERR;
                }
            }
        }
        break;
		//数据
    case 0x01:
        //if (fat_send_wait_cmdres_nonblocking(sand_dat_c, 1000))
        if (fat_send_wait_cmdres_nonblocking_hex(sand_dat_c,4,1000))
        
        
        
        {
						//收到OK
            if (fat_cmdres_keyword_matching("OK"))
            {
                ucErrorTimes = 0;
                ucStateNum++;
            }
            else
            {
								//发送3次得不到正确应答
                if (ucErrorTimes++ > 3)
                {
                    ucStateNum = MD_ERR;
                }
            }
        }
        break;
				
		//不明白他这个查询数据
		//需要实际测试一下
		//其实可以每次查询串口接收完成标志位，然后matching("+QMTRECV:")，是则处理，不是则忽略
				//好像有点明白了，发送数据的时候不去处理接收，发送完成了才去处理接收数据。
				//发送命令会清除接收缓冲区
				//如果数据一直接收，而且没有处理的话，会不会溢出。
				/*
				原来那个程序是怎么弄的需要确定一下
				*/
    //查询socket缓存是否有数据,在设置模式：AT+QMTCFG="recv/mode",0,1,1 有效，否则是AT+QMTCFG="dataformat",0,0,0 这种模式直接输出
    case 0x02:
        if (fat_send_wait_cmdres_nonblocking("AT+QMTRECV?\r\n", 1000))
        {
						//收到+QMTRECV:
            if (fat_cmdres_keyword_matching("+QMTRECV:"))
            { 
								/*//收到1表示有数据。返回的参数1为clientID，后5参数为<store_status>
				                //可以缓存5个数据
                                    //AT+QMTRECV?
                                    //+QMTRECV: 0,0,0,0,0,0
                                    //AT+QMTRECV?
                                    //+QMTRECV: 0,1,1,1,1,1
                                */
								if (fat_cmdres_keyword_matching("1")) //有1表示至少接收到一个数据
								{
									ucErrorTimes = 0;
									ucStateNum++;
								}
								else
								{
									ucErrorTimes = 0;
									ucStateNum = 0x04;
								}	
            }
            else
            {
								//发送3次得不到正确应答
                if (ucErrorTimes++ > 3)
                {
                    //ucStateNum = MD_ERR;
                    ucStateNum = 0x04;
                }
            }
        }
        break;
		//截取Topic和Data
		/*
		发送命令 AT+QMTRECV=0
		有数据则返回+QMTRECV: 0,0,"/public/test",12,"A55A20000001"
		有几条就一次性返回几条
		没有数据则返回 ok
		用查询方式，就不用两个接收数组了，因为它不受命令ULR的影响，用查询命令就等于有一个接收缓存了，需要解决的问题就是怎么接收多个数据
        用接收数组其实也是用查询命令，
		*/
		
    case 0x03:
				if (fat_send_wait_cmdres_nonblocking("AT+QMTRECV=0\r\n", 1000))//发送命令时间到达执行；
				{
						if (fat_cmdres_keyword_matching("+QMTRECV:"))//命令响应字中查找关键字
						{
								//截取主题名
								ret = match_recv_topic(cFatUartRecvBuf, cSocketRecvBuf);//把cFatUartRecvBuf的主题 放到 cSocketRecvBuf里面 返回的是数据长度
								FAT_DBGPRT_INFO("socket recv topic:%d , %s\r\n",ret , cSocketRecvBuf);
								//截取数据内容
								ret = match_recv_data(cFatUartRecvBuf, cSocketRecvBuf);//把cFatUartRecvBuf的数据 放到 cSocketRecvBuf里面

								FAT_DBGPRT_INFO("socket recv data:%d , %s\r\n",ret , cSocketRecvBuf);
								
								ucErrorTimes = 0;
								ucStateNum++;
						}
						else
						{
								//发送3次得不到正确应答
                if (ucErrorTimes++ > 3)
                {
                    //ucStateNum = MD_ERR;
                    ucStateNum = 0x04;
                }
						}
				}

	break;
    /*
    增加发送触发语句
    */            
   case 0x04:
    add_test++;  
	ucStateNum = 0x00;			

	break;                
                
                
                
		//错误跳至飞行模式
    case MD_ERR:
		ucStateNum = 0;
        return MD_ERR;
      default:
        break;
    }
    return 0;
}

/**
 * @description: 飞行模式处理函数
 * @param None
 * @return 0：检测未完成；MD_WORK_STA_CHK：重新开启网络跳至模块状态检测；MD_ERR：错误
 */
int module_flightmode()
{
		switch (ucStateNum)
    {
			case 0x00:
				ucFlightModeTimes++;
				ucStateNum++;
				FAT_DBGPRT_INFO("进入飞行模式次数：%d\r\n",ucFlightModeTimes);
				break;
			case 0x01:
				if (ucFlightModeTimes == 2)
				{
						FAT_DBGPRT_INFO("第二次进入飞行模式，复位模块\r\n");
						ucStateNum = MD_ERR;
				}
				else
					ucStateNum++;
				break;
			case 0x02:
        if (fat_send_wait_cmdres_blocking("AT+CFUN=0\r\n", 2000))
        {
						//收到OK
            if (fat_cmdres_keyword_matching("OK"))
            {
								ucErrorTimes = 0;
                ucStateNum++;
            }
						else
            {
								//发送5次得不到正确应答，跳至MD_ERR
                if (ucErrorTimes++ > 5)
                {
                    ucStateNum = MD_ERR;
                }
            }
        }
        break;
			case 0x03:
        if (wait_timeout(5000))
						ucStateNum++;
        break;
			case 0x04:
        if (fat_send_wait_cmdres_blocking("AT+CFUN=1\r\n", 2000))
        {
						//收到OK,状态更为MD_WORK_STA_CHK，跳至模块状态检测
            if (fat_cmdres_keyword_matching("OK"))
            {
								FAT_DBGPRT_INFO("再开启功能\r\n");
								ucStateNum = 0;
								return MD_WORK_STA_CHK;
            }
						else
            {
								//发送5次得不到正确应答，跳至MD_ERR
                if (ucErrorTimes++ > 5)
                {
                    ucStateNum = MD_ERR;
                }
            }
        }
        break;
			case MD_ERR:
				ucStateNum = 0;
				return MD_ERR;
			default:
        break;
    }
    return 0;	
}

/**
 * @description: LTE模块MQTT协议
 * @param None
 * @return None
 */

void module_MQTT(void)
{
    static int state = MD_RESET;
    int ret = 0;
    switch (state)
    {
    //复位模块
    case MD_RESET:
        if (module_reset())
        {
						wait_timeout(LTE_POWER_ON_WAIT_TIME);
						state = MD_AT_REQ;
        }
        break;
    //AT握手
    case MD_AT_REQ:
        if (fat_send_wait_cmdres_blocking("AT\r\n", 3000))//5s
        {
            if (fat_cmdres_keyword_matching("OK"))
            {
                ucErrorTimes = 0;
                state = MD_WORK_STA_CHK;
            }
            else
            {
                if (ucErrorTimes++ > 10)
                {
                    state = MD_ERR;
                }
            }
        }
        break;
    //模块状态检测
    case MD_WORK_STA_CHK:
        ret = module_is_ready();
        if (ret == MD_OK)
        {
            state = MD_CONNETINIT;
        }
		else if (ret == MD_ERR)
        {
            state = MD_FLIGHTMODE;
        }
        break;
    //连接参数初始化
    case MD_CONNETINIT:
        ret = module_connet_parm_init();
        if (ret == MD_OK)
        {
            state = MD_CONNETED;
        }
        else if (ret == MD_ERR)
        {
            state = MD_FLIGHTMODE;
        }
        break;
    //数据通信处理
    case MD_CONNETED:
        if (module_data() == MD_ERR)
        {
            state = MD_FLIGHTMODE;
        }
        break;
		//飞行模式处理
		case MD_FLIGHTMODE:
				ret = module_flightmode();
				if(ret == MD_WORK_STA_CHK)
        {
            state = MD_WORK_STA_CHK;
        }
				else if(ret == MD_ERR)
        {
						ucFlightModeTimes = 0;
            state = MD_ERR;
        }
				break;
    //错误
    case MD_ERR:
	 		  ucErrorTimes = 0;
				state = MD_RESET;
        break;
      default:
        break;
    }
}


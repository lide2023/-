/********************************************************************************
* @file    	  lte.c
* @function   LTEģ�� AT��������
* @brief 	    ��3.5.0�汾�⽨�Ĺ���ģ��
* @attention	�����̽���Ϊ�ο�������ʵ�ʿ����������ʵ���������
* @version 	  V1.1
* @company  	�����з�˼�����ӿƼ����޹�˾
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


#define LTE_POWER_ON_WAIT_TIME 2000 					//LTE�����ȴ�ʱ��
#define SIGNALMIN 15                                    //�ź�������ͷ�ֵ
#define SIGNALMAX 31                                    //�ź�������ͷ�ֵ
#define SOCKET_BUF_SIZE 128                             //Socket���ݻ����С
char cSocketRecvBuf[SOCKET_BUF_SIZE] = {0};             //socket���ݽ��ջ���
char cSocketSendBuf[SOCKET_BUF_SIZE] = {0};             //socket���ݷ��ͻ���
uint8_t ucStateNum = 0;                                 //����ִ��˳���ʶֵ
uint8_t retrytimes = 0;                                 //�������Դ���
uint8_t ucErrorTimes = 0;                               //��������ۼ�ֵ
uint8_t ucFlightModeTimes = 0;							//�������ģʽ����

typedef enum
{
    MD_RESET,        				//��λģ��
    MD_AT_REQ,       				//AT����
    MD_WORK_STA_CHK, 				//����״̬���
    MD_CONNETINIT,   				//����������Ϣ��ʼ��
    MD_CONNETED,     				//����ͨ��
	MD_FLIGHTMODE,			   	    //����ģʽ
    MD_OK = 0xFE,    				//����
    MD_ERR = 0xFF,   				//�쳣
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
 * @description:��λLTEģ��
 * @param None
 * @return None
 */
int module_reset(void)
{
    switch (ucStateNum)
    {
    //����PEN����
    case 0:
        PEN_GPIO_SET_LOW;
        ucStateNum++;
        break;
    //����PEN����
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
 * @param str��Ҫ�������ַ���
 * @param minval��Ҫƥ���ź�����������Сֵ
 * @param minval��Ҫƥ���ź������������ֵ
 * @return 0:�ź�������������������״̬, 1:�ź�����������������״̬
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
		
    p = p + 5;//��ȡ+CSQ:�������ֵ
	
	
    while (*p < 0x30 || *p > 0x39)//�жϺ��漸������ 0x30��0��16���Ʊ�ʾ��0x30��9��16����
      {
        p++;
      }
    q = p;

    while (*p != ',')
    {
        p++;
    }
	
	
    memcpy(tmp, q, p - q);//�ڴ渴�ƺ�����destination, source
    lpCsqVal = atoi(tmp); //�ַ��� ת��������
    /* �ж��ź������Ƿ������õ������� */
    if (lpCsqVal >= minval && lpCsqVal <= maxval)
    {
        return 1;
    }
    return 0;
}

/**
 * @description: ���ģ�鹤��״̬�Ƿ����
 * @param None
 * @return 0�����δ��ɣ�MD_OK��ģ���Ѿ�����MD_ERR�����󣬲����㹤��״̬
 */
int module_is_ready(void)
{
    switch (ucStateNum)
    {
    //�ر�AT�������
    case 0x00:
        if (fat_send_wait_cmdres_blocking("ATE0\r\n", 500))
        {
						//�յ�OK
            if (fat_cmdres_keyword_matching("OK"))//������Ӧ�����в��ҹؼ���
            {
                ucErrorTimes = 0;
                ucStateNum++;
            }
            else
            {
								//����10�εò�����ȷӦ��
                if (ucErrorTimes++ > 10)
                {
                    ucStateNum = MD_ERR;
                }
            }
        }
        break;
    //����
    case 0x01:
        if (fat_send_wait_cmdres_blocking("AT+CPIN?\r\n", 500))
        {
						//�յ�+CPIN: READY
            if (fat_cmdres_keyword_matching("+CPIN: READY"))
            {
                ucErrorTimes = 0;
                ucStateNum++;
            }
            else
            {     
								//����10�εò�����ȷӦ��
                if (ucErrorTimes++ > 10)
                {
                    ucStateNum = MD_ERR;
                }
            }
        }
        break;
    //��ѯ�ź�����
    case 0x02:
        if (fat_send_wait_cmdres_blocking("AT+CSQ\r\n", 200))
			{
							//�յ�OK
				if (fat_cmdres_keyword_matching("OK"))
				{
								//�յ�����99����Ƶ�ź�δ��ʼ����
								if (fat_cmdres_keyword_matching("+CSQ: 99,99"))
									{
											//����30�εò�����ȷӦ��
											if (ucErrorTimes++ > 30)
											{
													ucStateNum = MD_ERR;
											}
									}
							  else
								{    
														//�ź�ֵ��SIGNALMIN~SIGNALMAX�������
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
						//û�յ�Ӧ��
            else
				{
									//����30�β�Ӧ��
					if (ucErrorTimes++ > 30)
					{
						ucStateNum = MD_ERR;
					}
				}
        }
        break;
				
		//��ȡIMEI��
		case 0x03:
					if (fat_send_wait_cmdres_blocking("AT+GSN\r\n", 500))
							
						{
										
							if(cFatUartRecvBuf[2]>='0'&&cFatUartRecvBuf[2]<='9')//��ȡģ��IMEI��ֵ��ȡ��ֵ����������ַ��͵�
							{
								memset(( char *)EC600uinit.pubtopic,0,sizeof(EC600uinit.pubtopic));//�����ڴ�	�������ڴ溯�� 
								memset(( char *)EC600uinit.subtopic,0,sizeof(EC600uinit.subtopic));//�����ڴ�	�������ڴ溯��  
								memset(( char *)EC600uinit.IMEI,0,sizeof(EC600uinit.IMEI));        //�����ڴ�	�������ڴ溯��  	
								
								
								memcpy(( char *)EC600uinit.IMEI,cFatUartRecvBuf+2,15);//�ڴ濽��	��ec600_RxBuffer��2����ʼ����15������	EC600uinit.IMEI������											
								sprintf(( char *)EC600uinit.pubtopic,"app/%s",EC600uinit.IMEI);//%sվλ���ţ����ݽṹ��Ӧ����
								sprintf(( char *)EC600uinit.subtopic,"dev/%s",EC600uinit.IMEI);
				
								ucErrorTimes = 0;
								ucStateNum++;
										
							}
							else
								{
									//����3�εò�����ȷӦ��
									if (ucErrorTimes++ > 3)
									{
										ucStateNum = MD_ERR;
									}
								}
						}
	
		break;	
				
			//�鿴��ǰGPRS����״̬
    case 0x04:
        if (fat_send_wait_cmdres_blocking("AT+CGATT?\r\n", 500))
        {
						//�յ�+CGATT: 1
            if (fat_cmdres_keyword_matching("+CGATT: 1"))
               {
                ucErrorTimes = 0;
				        ucStateNum = MD_OK;
                }
            else
             {   
						//����30�εò�����ȷӦ��
            if (ucErrorTimes++ > 30)
                  { 
                    ucStateNum = MD_ERR;
                  }
            }
        }
        break;
				
		//������������ģʽ
		case MD_ERR:
				ucStateNum = 0;
				return MD_ERR;
    //���
        case MD_OK:
				ucStateNum = 0;
        return MD_OK;
        default:
        break;
    }
    return 0;
}

/**
 * @description: ���Ӻ�������ó�ʼ��
 * @param None
 * @return 0�����δ��ɣ�MD_OK��ģ���Ѿ�����MD_ERR�����󣬲����㹤��״̬
 */
int module_connet_parm_init(void)
{
    switch (ucStateNum)
    {
		//���ý���ģʽ:�ӷ��������յ� MQTT ��Ϣ���� URC ����ʽ�ϱ�
    case 0x00:
				//AT+QMTCFG="recv/mode",<client_idx>[,<msg_recv_mode>[,<msg_len_enable>]]
     //AT+QMTCFG="dataformat",0,0,0   
    if (fat_send_wait_cmdres_blocking("AT+QMTCFG=\"dataformat\",0,1,1\r\n", 500))
    //if (fat_send_wait_cmdres_blocking("AT+QMTCFG=\"recv/mode\",0,1,1\r\n", 200))
        {
						//�յ�OK
            if (fat_cmdres_keyword_matching("OK"))
            {
                ucErrorTimes = 0;
                ucStateNum++;
            }
            else
            {
								//����10�εò�����ȷӦ��
                if (ucErrorTimes++ > 10)
                {
                    ucStateNum = MD_ERR;
                }
            }
        }
        break;
    //��MQTT�ͻ�������
    case 0x01:
        if (fat_send_wait_cmdres_blocking("AT+QMTOPEN=0,\"bangmuyun.com\",1883\r\n", 500))
        {
						//�յ�+QMTOPEN: 0,0
            if (fat_cmdres_keyword_matching("+QMTOPEN: 0,0"))
            {
                ucErrorTimes = 0;
                ucStateNum++;
            }
            else
            {
								//����10�εò�����ȷӦ��
                if (ucErrorTimes++ > 10)
                {
                    ucStateNum = MD_ERR;
                }
            }
        }
        break;
    //��MQTT�ͻ������ӷ�����
    case 0x02:
			  memset(EC600uinit.upIMEI,0,sizeof(EC600uinit.upIMEI));
		    sprintf(EC600uinit.upIMEI,"AT+QMTCONN=0,\"%s\",\"device\",\"123456\"\r\n",EC600uinit.IMEI);// \"\" ���Ǳ�ʾ˫���ţ�s%��ʾռλ��,12345678������  debice���û���
        if (fat_send_wait_cmdres_blocking(EC600uinit.upIMEI, 500))
        {
						//�յ�+QMTCONN: 0,0,0
            if (fat_cmdres_keyword_matching("+QMTCONN: 0,0,0"))
            {
                ucErrorTimes = 0;
                ucStateNum++;
            }
            else
            {
								//����10�εò�����ȷӦ��
                if (ucErrorTimes++ > 10)
                {
                    ucStateNum = MD_ERR;
                }
            }
        }
        break;
		//��������
    case 0x03:
    memset(EC600uinit.upIMEI,0,sizeof(EC600uinit.upIMEI));    
		sprintf(EC600uinit.upIMEI,"AT+QMTSUB=0,1,\"%s\",0\r\n",EC600uinit.subtopic);//��������    
		if (fat_send_wait_cmdres_blocking(EC600uinit.upIMEI, 500))
        {
						//�յ�+QMTSUB: 0,1,0,0
            if (fat_cmdres_keyword_matching("+QMTSUB: 0,1,0,0"))
            {
                ucErrorTimes = 0;
                ucStateNum = MD_OK;
            }
            else
            {
								//����5�εò�����ȷӦ��
                if (ucErrorTimes++ > 5)
                {
                    ucStateNum = MD_ERR;
                }
            }
        }
        break;
    //���
    case MD_OK:
				ucStateNum = 0;
        return MD_OK;
    //������������ģʽ
    case MD_ERR:
				ucStateNum = 0;
        return MD_ERR;
    default:
        break;
    }
    return 0;
}

/**
 * @description: ��ȡsocket���ջ�������Topic
 * @param cmdres��������Ӧ����
 * @param recvbuf��socket���ջ���
 * @return ����Topic�ĳ���
   �ӱ��ı�־λ����0x2c;
 +QMTRECV: 0,0,"/public/test",12,"A55A20000001"

 */
int match_recv_topic(char *cmdres, char *recvbuf)
{
    int length = 0;
    char *p = NULL, *q = NULL;
    p = strstr(cmdres, "+QMTRECV: 0,0,\"");//���QMTRECV���Ǳ�־�յ�����
    if (p == NULL)
    {
        return 0;
    }
    p = p + 15;//���15��ģ���EMI���λ��;
    q = p;
    while (*p != 0x2C)//ASC���02C��ʲô���ǡ�����
    {
        p++;
    }
    length = p - q - 1;
    memset(recvbuf, 0, SOCKET_BUF_SIZE);//�ڴ�����
    memcpy(recvbuf, q, length);
    return length;
}

/**
 * @description: ��ȡsocket���ջ�������Topic������
 * @param cmdres��������Ӧ����
 * @param recvbuf��socket���ջ���
 * @return �������ݵĳ���
   �ӱ��ĺ��0K��ǰ�鵽0x22;
 */
int match_recv_data(char *cmdres, char *recvbuf)
{
    int length = 0;
    char *p = NULL, *q = NULL;
		//��λ��Ӧ���OK
    p = strstr(cmdres, "OK");
    if (p == NULL)
    {
        return 0;
    }
		//��λ��Topic���ݺ��"
		p = p - 5;
		q = p;
		do
		{
        p--;
    }
    while (*p != 0x22);//ASC���02C��ʲô���ǡ�"��
    length = q - p - 1;	
    memset(recvbuf, 0, SOCKET_BUF_SIZE);
    memcpy(recvbuf, p + 1, length);
    return length;
}

/**
 * @description: �����շ�����
 * @return 0�����δ��ɣ�MD_ERR������
 */


//�������������д���ṹ���գ��ҷ��㡣
//�ѷ���������Ϊһ���߳�ȥ��
//��ѯ����������һ���ô��ǲ���ͷ������ݲ��͵�һ��

int module_data(void)
{
    int ret = 0;//���ݳ���
    uint8_t data_len;//�������ݵĳ��ȣ�����������Ҫ֪�����ݳ���
    static uint8_t sand_dat_int[10]={"helloo"};
    static uint32_t add_test=0;
    static uint8_t wei_ling[4]="he";
    static uint8_t sand_dat_c[30];//��������Ǿ�̬���� ��Ϊ���ڵڶ��߳�����ʹ��
    //static char *sand_dat_c;
    switch (ucStateNum)
    {
    //�������ⷢ������
    /*���������С��sizeof(������)/sizeof(��������)��
      �ַ������С��strlen(������)+1,1��ΪҪ����'/0'�������ټ�1.
      data_len = strlen(Addr_Buf)/sizeof(char);//�������ݳ���  
    */
    case 0x00:
            /*��Ҫ���Ӽ������ݳ��ȵ����*/ 
            //memcpy(Lora_send_buf, (uint8_t *)&ADC_V1_AVR, 2); ADC_V1_AVR��16λ���α�������֡����ر����
            
            //sprintf(sand_dat_c,"%d",add_test);//sprintf��Ҫ�����ǰɱ�����ֵת�����ַ���
            //sprintf(sand_dat_c,"%d","hello");//sprintf�����鸳ֵ����Ҳ�ǶԵģ���strcpy()Ҳ���Ը����鸳ֵ
            //ָ�븳ֵ�������ǶԵ�sand_dat_c="heloo";
            
            memcpy(sand_dat_c+3,(uint8_t *)&add_test,1);
            memcpy(sand_dat_c+2,(uint8_t *)&add_test+1,1);
            memcpy(sand_dat_c+1,(uint8_t *)&add_test+2,1);
            memcpy(sand_dat_c,(uint8_t *)&add_test+3,1);
//          memcpy(sand_dat_c+2,(uint8_t *)&wei_ling,4);
            data_len = strlen(sand_dat_c);
            memset(EC600uinit.upIMEI,0,sizeof(EC600uinit.upIMEI));		
		    //sprintf(EC600uinit.upIMEI,"AT+QMTPUBEX=0,0,0,0,\"%s\",%d\r\n",EC600uinit.pubtopic,data_len);
		    sprintf((char *)EC600uinit.upIMEI,"AT+QMTPUBEX=0,0,0,0,\"%s\",%d\r\n",EC600uinit.pubtopic,4);//��Ҫ�������ݳ���
       

    if (fat_send_wait_cmdres_nonblocking((char *)EC600uinit.upIMEI, 1000))
        {
						//�յ�>
            if (fat_cmdres_keyword_matching(">"))
            {
                ucErrorTimes = 0;
                ucStateNum++;
            }
            else
            {
								//����3�εò�����ȷӦ��
                if (ucErrorTimes++ > 3)
                {
                    ucStateNum = MD_ERR;
                }
            }
        }
        break;
		//����
    case 0x01:
        //if (fat_send_wait_cmdres_nonblocking(sand_dat_c, 1000))
        if (fat_send_wait_cmdres_nonblocking_hex(sand_dat_c,4,1000))
        
        
        
        {
						//�յ�OK
            if (fat_cmdres_keyword_matching("OK"))
            {
                ucErrorTimes = 0;
                ucStateNum++;
            }
            else
            {
								//����3�εò�����ȷӦ��
                if (ucErrorTimes++ > 3)
                {
                    ucStateNum = MD_ERR;
                }
            }
        }
        break;
				
		//�������������ѯ����
		//��Ҫʵ�ʲ���һ��
		//��ʵ����ÿ�β�ѯ���ڽ�����ɱ�־λ��Ȼ��matching("+QMTRECV:")�����������������
				//�����е������ˣ��������ݵ�ʱ��ȥ������գ���������˲�ȥ����������ݡ�
				//���������������ջ�����
				//�������һֱ���գ�����û�д���Ļ����᲻�������
				/*
				ԭ���Ǹ���������ôŪ����Ҫȷ��һ��
				*/
    //��ѯsocket�����Ƿ�������,������ģʽ��AT+QMTCFG="recv/mode",0,1,1 ��Ч��������AT+QMTCFG="dataformat",0,0,0 ����ģʽֱ�����
    case 0x02:
        if (fat_send_wait_cmdres_nonblocking("AT+QMTRECV?\r\n", 1000))
        {
						//�յ�+QMTRECV:
            if (fat_cmdres_keyword_matching("+QMTRECV:"))
            { 
								/*//�յ�1��ʾ�����ݡ����صĲ���1ΪclientID����5����Ϊ<store_status>
				                //���Ի���5������
                                    //AT+QMTRECV?
                                    //+QMTRECV: 0,0,0,0,0,0
                                    //AT+QMTRECV?
                                    //+QMTRECV: 0,1,1,1,1,1
                                */
								if (fat_cmdres_keyword_matching("1")) //��1��ʾ���ٽ��յ�һ������
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
								//����3�εò�����ȷӦ��
                if (ucErrorTimes++ > 3)
                {
                    //ucStateNum = MD_ERR;
                    ucStateNum = 0x04;
                }
            }
        }
        break;
		//��ȡTopic��Data
		/*
		�������� AT+QMTRECV=0
		�������򷵻�+QMTRECV: 0,0,"/public/test",12,"A55A20000001"
		�м�����һ���Է��ؼ���
		û�������򷵻� ok
		�ò�ѯ��ʽ���Ͳ����������������ˣ���Ϊ����������ULR��Ӱ�죬�ò�ѯ����͵�����һ�����ջ����ˣ���Ҫ��������������ô���ն������
        �ý���������ʵҲ���ò�ѯ���
		*/
		
    case 0x03:
				if (fat_send_wait_cmdres_nonblocking("AT+QMTRECV=0\r\n", 1000))//��������ʱ�䵽��ִ�У�
				{
						if (fat_cmdres_keyword_matching("+QMTRECV:"))//������Ӧ���в��ҹؼ���
						{
								//��ȡ������
								ret = match_recv_topic(cFatUartRecvBuf, cSocketRecvBuf);//��cFatUartRecvBuf������ �ŵ� cSocketRecvBuf���� ���ص������ݳ���
								FAT_DBGPRT_INFO("socket recv topic:%d , %s\r\n",ret , cSocketRecvBuf);
								//��ȡ��������
								ret = match_recv_data(cFatUartRecvBuf, cSocketRecvBuf);//��cFatUartRecvBuf������ �ŵ� cSocketRecvBuf����

								FAT_DBGPRT_INFO("socket recv data:%d , %s\r\n",ret , cSocketRecvBuf);
								
								ucErrorTimes = 0;
								ucStateNum++;
						}
						else
						{
								//����3�εò�����ȷӦ��
                if (ucErrorTimes++ > 3)
                {
                    //ucStateNum = MD_ERR;
                    ucStateNum = 0x04;
                }
						}
				}

	break;
    /*
    ���ӷ��ʹ������
    */            
   case 0x04:
    add_test++;  
	ucStateNum = 0x00;			

	break;                
                
                
                
		//������������ģʽ
    case MD_ERR:
		ucStateNum = 0;
        return MD_ERR;
      default:
        break;
    }
    return 0;
}

/**
 * @description: ����ģʽ������
 * @param None
 * @return 0�����δ��ɣ�MD_WORK_STA_CHK�����¿�����������ģ��״̬��⣻MD_ERR������
 */
int module_flightmode()
{
		switch (ucStateNum)
    {
			case 0x00:
				ucFlightModeTimes++;
				ucStateNum++;
				FAT_DBGPRT_INFO("�������ģʽ������%d\r\n",ucFlightModeTimes);
				break;
			case 0x01:
				if (ucFlightModeTimes == 2)
				{
						FAT_DBGPRT_INFO("�ڶ��ν������ģʽ����λģ��\r\n");
						ucStateNum = MD_ERR;
				}
				else
					ucStateNum++;
				break;
			case 0x02:
        if (fat_send_wait_cmdres_blocking("AT+CFUN=0\r\n", 2000))
        {
						//�յ�OK
            if (fat_cmdres_keyword_matching("OK"))
            {
								ucErrorTimes = 0;
                ucStateNum++;
            }
						else
            {
								//����5�εò�����ȷӦ������MD_ERR
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
						//�յ�OK,״̬��ΪMD_WORK_STA_CHK������ģ��״̬���
            if (fat_cmdres_keyword_matching("OK"))
            {
								FAT_DBGPRT_INFO("�ٿ�������\r\n");
								ucStateNum = 0;
								return MD_WORK_STA_CHK;
            }
						else
            {
								//����5�εò�����ȷӦ������MD_ERR
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
 * @description: LTEģ��MQTTЭ��
 * @param None
 * @return None
 */

void module_MQTT(void)
{
    static int state = MD_RESET;
    int ret = 0;
    switch (state)
    {
    //��λģ��
    case MD_RESET:
        if (module_reset())
        {
						wait_timeout(LTE_POWER_ON_WAIT_TIME);
						state = MD_AT_REQ;
        }
        break;
    //AT����
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
    //ģ��״̬���
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
    //���Ӳ�����ʼ��
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
    //����ͨ�Ŵ���
    case MD_CONNETED:
        if (module_data() == MD_ERR)
        {
            state = MD_FLIGHTMODE;
        }
        break;
		//����ģʽ����
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
    //����
    case MD_ERR:
	 		  ucErrorTimes = 0;
				state = MD_RESET;
        break;
      default:
        break;
    }
}


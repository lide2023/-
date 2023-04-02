#ifndef __EC600U_H
#define __EC600U_H




#ifdef __cplusplus
extern "C" {
#endif

#include "sys.h"
#include "main.h"
#include "stdio.h"
#include "string.h"	
#include "delay.h"
#include "stm32g0xx_hal.h"	
#include "tim.h"
#include <string.h>
#include <stdlib.h>	
#include "stmflash.h"	




#define ALME_TEMES  3
#define STORE_TEMES  3
#define STATE_TEMES  3
#define TEMP_TEMES   1


//L1010  if  (CAT1_Send_Data_dif_2(0x1000,0,storage_1,49,32))   //存储数据有变化则上传，程序有变化，需要更改上传变量数。  


	
extern u8 ec600_RxBuffer[100];
extern u8 ec600_RxBuffer_test[100];	
extern u8 CATrecvbuf[100];
	
extern u16 ec600u_receive_count;
extern u16 ec600u_receive_count_test;	
extern char buf[100];
extern u16 Control_data[30];
extern u16 CtrSend_data[30];
extern u16 TCtrSend_data[30];//返回暂存函数


extern	u8 cat1_receive_biao;
extern 	u8 cat1_receive_timer;//放到时间中断中	

extern	u8 cat1_receive_biao_D;
extern 	u8 cat1_receive_timer_D;//放到时间中断中	


extern	 uint8_t ucTimetStartFlg;//保证执行一次开关
extern 	 uint16_t usTimeOutCount;//记录主语句完成时间
extern   u8 excue_biao;	

extern  u16 CAT_Drass;
extern  u16 CAT_Data;		
extern	u8  CAT_back_biao;

extern  u8 Data_dif_times;

extern u8 ucFatUartRecvFinishFlg;

extern uint8_t  feedback_biao;//全局变量 
extern uint16_t cat_feedback[2];//全局变量 

void cat1_receive(void);	

void Uart2_SendDATA(char *data,unsigned char len);
void Uart2_SendStr(char*SendBuf);
void Uart1_SendStr(char*SendBuf);
void Clear_Buffer(void);
void Clear_Buf(void);
void Clear_Buffer_D(void);

void Clear_Buf_Buf(uint8_t *p);

int OPEN_CAT1_2(void);



void IWDG_Feed(void);
void MX_IWDG_Init(void);
void cat1_timeadd(void);
void cat1_excute(void);
void Ask_for(void);



int match_recv_topic(char *cmdres, char *recvbuf);

void Uart1_SendDATA(char *data,unsigned char len);
int fat_send_wait( unsigned short int timeoutval);
int fat_send_cmd(char *cmd, unsigned short int timeoutval); 
int fat_send_cmd_2(char *cmd,unsigned char length, unsigned short int timeoutval);


int fat_send_Ncmd(char *cmd, unsigned short int timeoutval); 
int fat_send_Ncmd_2(char *cmd,unsigned char length, unsigned short int timeoutval);


 void StrToHex(unsigned char *pbDest, unsigned char *pbSrc, int nLen);
 int fat_cmdres_keyword_matching(char *keyword);

int module_data3(uint16_t Variable_addr,uint16_t *Dev_Data,u8 number);

int CAT1_Send_Data_dif_2(u16 base ,u16 addr,u16 *data,u8 number,u8 time);

void CAT1_Send_Data_Str(u16 base ,u16 addr,u16 *data,u8 number);

 int CAT1_Send_Data_Nd(u16 base ,u16 addr,u16* data);
 int CAT1_Send_Data_ResNd(u16 base ,u16 addr,u16* data);

int match_csq(char *str);
int Enquiries_CSG(u8 time);//time秒后读取一次信号质量
int Enquiries_CSG2(u16 time);//time秒后读取一次信号质量


void Read_cat(void);

void Clear_Buf_lcd(void);

void myitoa2(int value, char *string);

void all_cat_senddata(void);

void Cat_chushihua(void);

extern uint8_t ucStateNum;
extern uint16_t ucErrorTimes;


extern 	u8 send_reg;
extern 	u8 send_reg1;


extern  u8 send_ok_biao;
extern  u16 addr_data;
extern  u16 *send_data;
extern  u8  data_number;
extern	u16 data_a1[20];
extern  u16  CAT_storage[100];
extern  u8 cat1_switch;



extern volatile int G_lpCsqVal;

extern   u8 allStateNum;
extern   u8 temp_allStateNum; 

extern	  u16 Cat1_data_dif_temp;
extern		u8  CAT1_data_dif_num;	
extern    u16 CAT1_data_res_biao;
extern   void clear_alarm(void);

typedef struct    
{
	unsigned char IMEI[20];//IMEI
	unsigned char pubtopic[25];
	unsigned char subtopic[25];
	unsigned char username[12];
	unsigned char passwore[12];
	
}EC600u_init; 



typedef enum
{
    MD_OPEN_OK=0xFB,       				//复位模块
    MD_DATA_OK=0xFC,       				//AT握手
    MD_DATA_SEND=0xFD, 				//工作状态检测
//	MD_EMI,                         //获取EMI
//    MD_CONNETINIT,   				//连接配置信息初始化
//    MD_CONNETED,     				//数据通信
//	MD_FLIGHTMODE,			   	    //飞行模式
    MD_OK = 0xFE,    				//正常
    MD_ERR = 0xFF,              	//异常
} MD_RUN_STATE;




#ifdef __cplusplus
}
#endif



#endif

#include "ec600u.h"
#include "modbus.h"
#include "control.h"
#include "queue.h"

u8 ec600_RxBuffer[100];
u8 ec600_RxBuffer_test[100];
u8 CATrecvbuf[100];

char buf[100];

u16 Control_data[30];
u16 CtrSend_data[30];
u16 TCtrSend_data[30];
	u8 cat1_receive_biao=1;
	u8 cat1_receive_timer;//放到时间中断中
	
	u8 cat1_receive_biao_D;
 	u8 cat1_receive_timer_D;//放到时间中断中	
	

	 uint8_t ucTimetStartFlg=0;//保证执行一次开关
	 uint16_t usTimeOutCount;//记录主语句完成时间
 uint8_t ucStateNum=0X00; 
 uint16_t ucErrorTimes=0;
 u8 excue_biao=0;
 

EC600u_init  EC600uinit; 


	u8 send_reg=0;
	u8 send_reg1=0;	
    u8 send_ok_biao=1;
    u16 addr_data=0;
    u16 *send_data;    
	u8  data_number;
	u16 data_a1[20]={0};
  u16  CAT_storage[100]={0xffff};//CAT1_Send_Data_dif()中间函数 
	
u8 ucFatUartRecvFinishFlg=0;
	// u16 *data_a2;
	//u8 cat1_switch=1;

u16 ec600u_receive_count=0;
u16 ec600u_receive_count_test=0;

volatile int G_lpCsqVal = 0; 	
	
	
	
IWDG_HandleTypeDef hiwdg;

  u8 allStateNum=0;
  u8 temp_allStateNum=0; 

  u16 CAT_Drass;
  u16 CAT_Data;
	u16 CAT_head;
	u8  CAT_back_biao=0;	
		
	u16 Cat1_data_dif_temp=0;
	u8  CAT1_data_dif_num;		

u16 CAT1_data_res_biao;//区分是主动发的数据还是反馈数据
u8 Data_dif_times;  //发送存储变计数，不一样就发送

uint8_t feedback_biao=1;//全局变量 
uint16_t cat_feedback[2];//全局变量 

/*
*编程很多都是语法问题
*一定要从最小的基本功能单位编辑
*2022年4月23号心得
*/


  
void Uart2_SendDATA(char *data,unsigned char len)
{
	unsigned char i;
	for(i=0;i<len;i++)
	{
   while((USART2->ISR&0X40)==0){}//等待发送完成 
   USART2->TDR = (uint8_t) data[i]; 
	 }
}




void Uart4_SendDATA(char *data,unsigned char len)
{
	unsigned char i;
	
	DE485=1;
	delay_ms(1);
	for(i=0;i<len;i++)
	{
   while((USART4->ISR&0X40)==0){}//等待发送完成 
   USART4->TDR = (uint8_t) data[i]; 
	 }
	
	 delay_ms(1);
		DE485=0;
	delay_ms(1);
	 
	 
}


  
void Uart1_SendDATA(char *data,unsigned char len)
{
	unsigned char i;
	for(i=0;i<len;i++)
	{
   while((USART1->ISR&0X40)==0){}//等待发送完成 
   USART1->TDR = (uint8_t) data[i]; 
	 }
}


void Uart2_SendStr(char*SendBuf)//串口1打印数据
{
	while(*SendBuf)
	{
        while((USART2->ISR&0X40)==0){};//等待发送完成 
        USART2->TDR = (uint8_t) *SendBuf; 
		    SendBuf++;
	}
}



void Uart1_SendStr(char*SendBuf)//串口1打印数据
{
	while(*SendBuf)
	{
        while((USART1->ISR&0X40)==0){};//等待发送完成 
        USART1->TDR = (uint8_t) *SendBuf; 
		SendBuf++;
	}
}


void Uart4_SendStr(char*SendBuf)//串口1打印数据
{
		DE485=1;
	delay_ms(1);
	
	while(*SendBuf)
	{
        while((USART4->ISR&0X40)==0){};//等待发送完成 
        USART4->TDR = (uint8_t) *SendBuf; 
		SendBuf++;
	}
	delay_ms(1);
		DE485=0;
	delay_ms(1);
	
}




int fat_send_cmd(char *cmd, unsigned short int timeoutval)//只执行一次，timeoutval时间以后可以执行其他后续处理
{
//	 uint8_t ucTimetStartFlg=0;//保证执行一次开关
//	 uint16_t usTimeOutCount;//记录主语句完成时间
//	char temp[20];
	
	static u16 biao;

	/* 发送命令，开启定时计数 */
	if (ucTimetStartFlg == 0)
	{
		
		
		 //sprintf(temp,cmd);	    
	     //Uart2_SendStr(temp);
	   Clear_Buffer();
		
		 Uart2_SendStr(cmd);	
	   Uart4_SendStr(cmd);	
		 ucTimetStartFlg = 1;//启动计时确保执行1次
	}

	/* 到达设定的定时时间 */
	
	
	if(usTimeOutCount>timeoutval)
	  {
	    ucTimetStartFlg=0;
		  usTimeOutCount = 0; 
		//return 1;  
		  biao=1;
	   }
	else biao=0;
	return biao;
}


int fat_send_Ncmd(char *cmd, unsigned short int timeoutval)//只执行一次，timeoutval时间以后可以执行其他后续处理
{
//	 uint8_t ucTimetStartFlg=0;//保证执行一次开关
//	 uint16_t usTimeOutCount;//记录主语句完成时间
//	char temp[20];
	
	static u16 biao;

	/* 发送命令，开启定时计数 */
	if (ucTimetStartFlg == 0)
	{
		
		
		 //sprintf(temp,cmd);	    
	     //Uart2_SendStr(temp);
	   Clear_Buffer();
		 Uart2_SendStr(cmd);	
	   Uart4_SendStr(cmd);	
		 ucTimetStartFlg = 1;//启动计时确保执行1次
	}

	/* 到达设定的定时时间 */
	
	
	if((usTimeOutCount>timeoutval)||( ucFatUartRecvFinishFlg==1))
	  {
	    ucTimetStartFlg=0;
		  usTimeOutCount = 0;
			ucFatUartRecvFinishFlg=0;
		//return 1;  
		  biao=1;
	   }
	else biao=0;
	return biao;
}



 
int fat_send_cmd_2(char *cmd,unsigned char length, unsigned short int timeoutval)//只执行一次，timeoutval时间以后可以执行其他后续处理
{
//	 uint8_t ucTimetStartFlg=0;//保证执行一次开关
//	 uint16_t usTimeOutCount;//记录主语句完成时间
	
	
	static u16 biao;
	
	/* 发送命令，开启定时计数 */
	if (ucTimetStartFlg == 0)
	{

		//sprintf(temp,cmd);	    
	     //Uart2_SendStr(temp);
	     Clear_Buffer();
		 Uart2_SendDATA(cmd,length);
		// Uart4_SendDATA(cmd,length);		
		 ucTimetStartFlg = 1;//启动计时确保执行1次
	}

	/* 到达设定的定时时间 */
	
	
	if(usTimeOutCount>timeoutval)
	  {
	    ucTimetStartFlg=0;
		usTimeOutCount = 0; 
		biao=1;  
		  
	   }
	else biao=0;
	return biao;
 }


 
 
int fat_send_Ncmd_2(char *cmd,unsigned char length, unsigned short int timeoutval)//只执行一次，timeoutval时间以后可以执行其他后续处理
{
//	 uint8_t ucTimetStartFlg=0;//保证执行一次开关
//	 uint16_t usTimeOutCount;//记录主语句完成时间
	
	
	static u16 biao;
	
	/* 发送命令，开启定时计数 */
	if (ucTimetStartFlg == 0)
	{

		//sprintf(temp,cmd);	    
	     //Uart2_SendStr(temp);
	     Clear_Buffer();
		 Uart2_SendDATA(cmd,length);
		// Uart4_SendDATA(cmd,length);		
		 ucTimetStartFlg = 1;//启动计时确保执行1次
	}

	/* 到达设定的定时时间 */
	
	
	if((usTimeOutCount>timeoutval)||( ucFatUartRecvFinishFlg==1))
	  {
	    ucTimetStartFlg=0;
		  usTimeOutCount = 0; 
			ucFatUartRecvFinishFlg=0;
		  biao=1;  
		  
	   }
	else biao=0;
	return biao;
 }




int fat_send_wait( unsigned short int timeoutval)//只执行一次，timeoutval时间以后可以执行其他后续处理
{
	
	//   if(ucTimetStartFlg==1) { usTimeOutCount++ ;}//放到时间中断里面

    
	
	/* 发送命令，开启定时计数 */
	if (ucTimetStartFlg == 0)
	{
		
         usTimeOutCount = 0;
		 ucTimetStartFlg = 1;//启动计时确保执行1次
	}

	/* 到达设定的定时时间 */
	
	
	if(usTimeOutCount>timeoutval)
	  {
	    ucTimetStartFlg=0;
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
		
		/* 检索检索到关键词 */
		if (strstr((const char *)(ec600_RxBuffer), keyword) != NULL)//强制类型转换(const char *)
		{
			
//			Uart1_SendStr((char *)ec600_RxBuffer);
			//Clear_Buffer();
			return 1;			
						
		}
	
	return 0;
}




/*
*带返回值的函数，里面包括static语句
*如果完成一个分支，并且break;
*那么没有return返回case值
*如果有return则返回return后面的的值
*
*/

void Ask_for(void)//应答查询信号

{
 
    
if((Cat_fedback_data.Lenght>=1)&&(feedback_biao==1))//feedback_biao全局变量，放到发送函数里面置1;	Cat_fedback_data.Lenght 队列里面有数据
{
// uint16_t cat_feedback[2];//全局变量
 feedback_biao=0;//执行一次    
 CAT_back_biao=1;//发送数据标志
 Read_RingBuff_cat(&Cat_fedback_data, cat_feedback);
 
//if(send_reg1==2) allStateNum=27;//发送完到下一个发送  V 61版本没有，确保反馈立即返回。 
    
    
}
  
    
    
     if(Control_data[6]==1) //查询电机1电流
		  {
		   A_current_biao=1;
		   Control_data[6]=9;		  
		   }  

		 if(Control_data[7]==1) //查询电机2电流
		  {
		   B_current_biao=1;
		   Control_data[7]=9;		  
		   }  

    if(Control_data[8]==1)  //查询电机3电流
		  {
		   C_current_biao=1;
		   Control_data[8]=9;		  
		   }  
    
		 if(Control_data[9]==1) //发送存储数据
		  { 
		   send_str_biao=1;
		   Control_data[9]=9;	
			 send_mem_nember=0;	
		   } 
 
    
			if(shuiwei_xinhao==0)  PresentLevel= TCtrSend_data[19]; 
	    
			if(	shuiwei_xinhao==1)
				{
						if(PresentLevel_temp!=PresentLevel)  
						{
							PresentLevel_temp=PresentLevel;
							PresentLevel_biao=1;
						}
			  }
		 
			 
}

void clear_alarm(void)

{
 //接收地址 0x3000;
      if(TCtrSend_data[0]==1)
		  {
		   alarm_quex_biao1=0;
		   TCtrSend_data[0]=0;		  
		  }
 	  if(TCtrSend_data[1]==1)
		  {
		   alarm_quex_biao2=0;
		   TCtrSend_data[1]=0;		  
		  }
	  
	  if(TCtrSend_data[2]==1)
		  {
		   alarm_quex_biao3=0;
		   TCtrSend_data[2]=0;		  
		  }

	  
  	  if(TCtrSend_data[3]==1)
		  {
		   alarm_well_biao1=0;
		   TCtrSend_data[3]=0;		  
		  }
	  
	  
	  
	  	  if(TCtrSend_data[4]==1)
		  {
		   alarm_well_biao2=0;
		   TCtrSend_data[4]=0;		  
		  }
	  
	  if(TCtrSend_data[5]==1)
		  {
		   alarm_well_biao3=0;
		   TCtrSend_data[5]=0;		  
		  }


	  if(TCtrSend_data[6]==1)
		  {
		   alarm_pull_biao=0;
		   TCtrSend_data[6]=0;		  
		  }  
	  if(TCtrSend_data[7]==1)
		  {
		   alarm_noelec_biao=0;
		   TCtrSend_data[7]=0;	  
		  }   
  
      if(TCtrSend_data[8]==1)
		  {
		   alarm_upelec_biao=0;
		   TCtrSend_data[8]=0;		  
		   } 
//显示的状态			 
		if(TCtrSend_data[9]==CtrSend_data[9])
		  {
		    Crelay_state1=0;
		    TCtrSend_data[9]=0xffff;		  
		  }  
			 
		if(TCtrSend_data[10]==CtrSend_data[10])
		  {
		    Cvalve_state1=0;
		    TCtrSend_data[10]=0xffff;		  
		  }   
			 
						 
		if(TCtrSend_data[11]==CtrSend_data[11])
		  {
		    Crelay_state2=0;
		    TCtrSend_data[11]=0xffff;		  
		  }  
			 
		if(TCtrSend_data[12]==CtrSend_data[12])
		  {
		    Cvalve_state2=0;
		    TCtrSend_data[12]=0xffff;		  
		  }    
			 
			 
		if(TCtrSend_data[13]==CtrSend_data[13])
		  {
		    Crelay_state3=0;
		    TCtrSend_data[13]=0xffff;		  
		  }  
			 
		if(TCtrSend_data[14]==CtrSend_data[14])
		  {
		    Cvalve_state3=0;
		    TCtrSend_data[14]=0xffff;		  
		  }    	
//显示的状态显示的状态
			

       //CtrSend_data 向外发送的中间值 CtrSend_data[15]= A_current  TCtrSend_data[15] 是反馈回来的数值

			if(TCtrSend_data[15]==CtrSend_data[15])
		  {
		   A_current_biao=0;
		   TCtrSend_data[15]=0xffff;		  
		   }  

		 if(TCtrSend_data[16]==CtrSend_data[16])
		  {
		   B_current_biao=0;
		   TCtrSend_data[16]=0xffff;		  
		   }  

    if(TCtrSend_data[17]==CtrSend_data[17])
		  {
		   C_current_biao=0;
		   TCtrSend_data[17]=0xffff;		  
		   }  
			
			 
			 
			 if(res_storage_1[CAT1_data_dif_num]==storage_1[CAT1_data_dif_num])
		   {
			  //CAT_storage[CAT1_data_dif_num]= storage_1[CAT1_data_dif_num];////CAT_storage[]==storage_1[]关闭发送 需要看函数里面
			  Data_dif_times=0;
				 CAT1_Send_Data_dif_biao=0;
				res_storage_1[CAT1_data_dif_num]=0xffff; 
			 }
			 
//		if(res_storage_1[CAT1_data_dif_num]==storage_1[CAT1_data_dif_num])
//		   {
//			  CAT_storage[CAT1_data_dif_num]= storage_1[CAT1_data_dif_num];////CAT_storage[]==storage_1[]关闭发送 需要看函数里面
//			  Data_dif_times=0;				
//				res_storage_1[CAT1_data_dif_num]=0xffff; 
//			 }
			 
}


void all_cat_senddata(void)
{
//  u8 allStateNum=0;
//  u8 temp_allStateNum=0; 	
	switch (allStateNum)
    {
		case 0:			
		OPEN_CAT1_2();		

		if(ucStateNum == 0x20)  allStateNum=2;		
	  break;
		
		case 1:		

//        send_reg1=0;
//        ucStateNum=0x0D;//进入链接网络
//		if(send_reg1==1)
//		{
//		module_data3(addr_data,send_data,data_number);//send_data是主动加上0x1000的数据,此函数的作用是组帧然后发送
//		}
		
		module_data3(addr_data,send_data,data_number);//send_data是主动加上0x1000的数据,此函数的作用是组帧然后发送
        
        
//       Ask_for();
        
        
		//addr_data  send_data  data_number  是全局变量 发送数据的地址，发送数据数组名  发送数据个数
		if(send_reg1==0) allStateNum=0;		
		
//        if (send_reg1==2) allStateNum=temp_allStateNum+1;//发送完到下一个发送
        
		if((send_reg1==2)&&(CAT_back_biao==1)) allStateNum=27;//发送完到下一个发送
        else if (send_reg1==2) allStateNum=temp_allStateNum+1;//发送完到下一个发送
        
		break;
		
		case 2:		

		
		if( CAT_wait_ds(&cwat_1,60,&alarm_quex_biao1))//三个元素，等待时间，缺相标alarm_quex_biao1，发送多次间隔时间60
		  
			  {
			    alarm_quex_biao1++;					
					if(alarm_quex_biao1>ALME_TEMES) alarm_quex_biao1=0;//关闭发送
          
					CtrSend_data[0]=1;//电机1缺相报警		  
			    CAT1_Send_Data_Nd(0x3000,0,CtrSend_data); 
					temp_allStateNum=allStateNum;//记录执行完成要去的下一个执行地址
					allStateNum=1;//跳转到发送地址
               				  
			  }
		  else  allStateNum=3;
			  
		break;
		
		case 3:		

		  	if( CAT_wait_ds(&cwat_2,50,&alarm_quex_biao2))
	  
			  {
			   
					alarm_quex_biao2++;
					if(alarm_quex_biao2>ALME_TEMES) alarm_quex_biao2=0;
					
					CtrSend_data[1]=1;//电机2缺相报警		
			   CAT1_Send_Data_Nd(0x3000,1,CtrSend_data);
					   
			   temp_allStateNum=allStateNum;//记录执行完成要去的下一个执行地址
          allStateNum=1;//跳转到发送地址
               				  
			  }
		  else  allStateNum=4;
			  
		break;
		
		
		
			  
		case 4:		

		  if( CAT_wait_ds(&cwat_3,50,&alarm_quex_biao3))	  
				  {
				   
						alarm_quex_biao3++;
					if(alarm_quex_biao3>ALME_TEMES) alarm_quex_biao3=0;
						
						CtrSend_data[2]=1;//电机3缺相报警		
				   CAT1_Send_Data_Nd(0x3000,2,CtrSend_data);					   
			     temp_allStateNum=allStateNum;
           allStateNum=1; //跳转到发送地址             				  
			    }
		   else  allStateNum=5;
			  
		break;	  
			  
			  
			  
			case 5:		

		  	 if( CAT_wait_ds(&cwat_4,50,&alarm_well_biao1))	  
			  {
				
					alarm_well_biao1++;
					if(alarm_well_biao1>ALME_TEMES) alarm_well_biao1=0;
					
					
					CtrSend_data[3]=1;//水井1缺水报警		
				CAT1_Send_Data_Nd(0x3000,3,CtrSend_data);					   
			   temp_allStateNum=allStateNum;
         allStateNum=1;
               				  
			  }
		  else  allStateNum=6;
			  
		break;	  	  
			  
			  
			  
				case 6:		

		  	  if( CAT_wait_ds(&cwat_5,50,&alarm_well_biao2))	  
			  {
				 
           alarm_well_biao2++;
					 if(alarm_well_biao2>ALME_TEMES) alarm_well_biao2=0;
					
			  CtrSend_data[4]=1;//水井2缺水报警		  
			  CAT1_Send_Data_Nd(0x3000,4,CtrSend_data);
					   
			   temp_allStateNum=allStateNum;
               allStateNum=1;
               				  
			  }
		  else  allStateNum=7;
			  
		break;	  	  
			  	  
			  
		case 7:		

					   
			if( CAT_wait_ds(&cwat_6,50,&alarm_well_biao3))	  
			  {
				  
			     alarm_well_biao3++;
					 if(alarm_well_biao3>ALME_TEMES) alarm_well_biao3=0;
					
					
					CtrSend_data[5]=1;//水井3缺水报警		
			   CAT1_Send_Data_Nd(0x3000,5,CtrSend_data);
					   
			   temp_allStateNum=allStateNum;
               allStateNum=1;
               				  
			  }
		  else  allStateNum=8;
			  
		break;	  
			  
			case 8:		

		  	  if( CAT_wait_ds(&cwat_7,10,&alarm_pull_biao))	  
			  {
				  
					
					 alarm_pull_biao++;
					 if(alarm_pull_biao>ALME_TEMES) alarm_pull_biao=0;
					
					
			   CtrSend_data[6]=1;//缺引水报警		
			   CAT1_Send_Data_Nd(0x3000,10,CtrSend_data);
					   
			   temp_allStateNum=allStateNum;
         allStateNum=1;
               				  
			  }
		  else  allStateNum=9;
			  
		break;		  
			  
		case 9:		

		  	 if( CAT_wait_ds(&cwat_8,20,&alarm_noelec_biao))	  
				{		 
				  
					
					alarm_noelec_biao++;
					 if(alarm_noelec_biao>ALME_TEMES) alarm_noelec_biao=0;
					
					CtrSend_data[7]=1;//断电报警		
				  CAT1_Send_Data_Nd(0x3000,7,CtrSend_data);	  
					   
			      temp_allStateNum=allStateNum;
                  allStateNum=1;
               				  
			     }
		     else  allStateNum=10;
			  
		break;	  	  
			  	  	  

		case 10:
//#define ALME_TEMES  2;
//#define STORE_TEMES  2;
//#define STATE_TEMES  2;
//#define TEMP_TEMES   1;
			 if( CAT_wait_ds(&cwat_9,20,&alarm_upelec_biao))	  
				  {			  
					 
					 alarm_upelec_biao++;
					 if(alarm_upelec_biao>ALME_TEMES) alarm_upelec_biao=0;


						CtrSend_data[8]=1;//上电报警		
					 CAT1_Send_Data_Nd(0x3000,8,CtrSend_data);						   
					 temp_allStateNum=allStateNum;
					 allStateNum=1;               				  
				   }
			 else  allStateNum=11;
				  
			 break;	  
			  
		
		case 11:	//发送存储变量

		if( CAT_wait_ds(&cwat_10,20,&send_str_biao)) 	  
		  {
			 send_str_biao++; 
			 if(send_str_biao>STORE_TEMES)
			  {
			  send_str_biao=0;				
			  } 
					
				CAT1_Send_Data_Str(0x1000 ,0,storage_1,50);//发送多少个变量需要	module_data3()函数决定	  
				  
			 temp_allStateNum=allStateNum;
			 allStateNum=1;
								  
		  }
		  
		 else  allStateNum=12;			  
		 break;	  	  				 		
		

		case 12:	

         // 水泵1 3009 0000是启动   0001 是停止  0002是暂停
         if( CAT_wait_ds(&cwat_11,20,&Crelay_state1))//	水井1状态  
				  {			  
					 
						Crelay_state1++;
					if( Crelay_state1>STATE_TEMES) Crelay_state1=0;  
						
						
						CtrSend_data[9]=relay_state1;   //水井1状态		
					 CAT1_Send_Data_Nd(0x3000,9,CtrSend_data);						   
					 temp_allStateNum=allStateNum;
					 allStateNum=1; 
           //Crelay_state1=0;					  
				   }
			 else  allStateNum=13;
				  
			 break;	  
        
		 case 13:	
			 
		   //阀门1 300a  0000是关闭状态  0001是启动状态 0003 是关闭进程 
		 
  	     if( CAT_wait_ds(&cwat_12,20,&Cvalve_state1))//	水井1阀门状态  
				  {	

            Cvalve_state1++;
					if( Cvalve_state1>STATE_TEMES) Cvalve_state1=0;  

						
					 CtrSend_data[10]=valve_state1;   //水井1阀门状态					  
					 CAT1_Send_Data_Nd(0x3000,10,CtrSend_data);	
					  
					 temp_allStateNum=allStateNum;
					 allStateNum=1; 
           //Cvalve_state1=0;					  
				   }
			 else  allStateNum=14;
				  
			 break;	  
		  
	case 14:	

         // 水泵2 300b 0000是启动   0001 是停止  0002是暂停
         if( CAT_wait_ds(&cwat_13,20,&Crelay_state2))//	水井2状态  
				  {			  
					 
						
						 Crelay_state2++;
					if( Crelay_state2>STATE_TEMES) Crelay_state2=0;  
						
						CtrSend_data[11]=relay_state2;   //水井2状态		
					 CAT1_Send_Data_Nd(0x3000,11,CtrSend_data);						   
					 temp_allStateNum=allStateNum;
					 allStateNum=1; 
           //Crelay_state2=0;					  
				   }
			 else  allStateNum=15;
				  
			 break;	  
        
		 case 15:	
			 
		   //阀门2 300c  0000是关闭状态  0001是启动状态 0003 是关闭进程 
		 
  	     if( CAT_wait_ds(&cwat_14,10,&Cvalve_state2))//	水井2阀门状态  
				  {			  
					 
						
					 Cvalve_state2++;
					if( Cvalve_state2>STATE_TEMES) Cvalve_state2=0;  
						
						CtrSend_data[12]=valve_state2;   //水井2阀门状态					  
					 CAT1_Send_Data_Nd(0x3000,12,CtrSend_data);	
					  
					 temp_allStateNum=allStateNum;
					 allStateNum=1; 
           //Cvalve_state2=0;					  
				   }
			 else  allStateNum=16;
				  
			 break;	  
		  
		case 16:	

         // 水泵3 300d 0000是启动   0001 是停止  0002是暂停
         if( CAT_wait_ds(&cwat_15,10,&Crelay_state3))//	水井3状态  
				  {	

            Crelay_state3++;
					if( Crelay_state3>STATE_TEMES) Crelay_state3=0;  

						
					 CtrSend_data[13]=relay_state3;   //水井3状态		
					 CAT1_Send_Data_Nd(0x3000,13,CtrSend_data);						   
					 temp_allStateNum=allStateNum;
					 allStateNum=1; 
          // Crelay_state3=0;					  
				   }
			 else  allStateNum=17;
				  
			 break;	  

	 case 17:	
			 
		   //阀门3 300e  0000是关闭状态  0001是启动状态 0003 是关闭进程 
		 
  	     if( CAT_wait_ds(&cwat_16,10,&Cvalve_state3))//	水井3阀门状态  
				  {			  
					 
						
					Cvalve_state3++;
					if( Cvalve_state3>STATE_TEMES) Cvalve_state3=0;
						
						
						CtrSend_data[14]=valve_state3;   //水井3阀门状态					  
					 CAT1_Send_Data_Nd(0x3000,14,CtrSend_data);	
					  
					 temp_allStateNum=allStateNum;
					 allStateNum=1; 
           //Cvalve_state3=0;					  
				   }
			 else  allStateNum=18;
				  
			 break;	  
						 
		 case 18:	
	
 
		 if( CAT_wait_ds(&cwat_17,10,&A_current_biao))//	 A相电流
				  {			  
					
              A_current_biao++;
					if( A_current_biao>STATE_TEMES) A_current_biao=0;

						CtrSend_data[15]=A_current;   //				  
					 CAT1_Send_Data_Nd(0x3000,15,CtrSend_data);	
					  
					 temp_allStateNum=allStateNum;
					 allStateNum=1;
						
				   }
			 else  allStateNum=19;
				  
			 break;	  				 
					 
		  case 19:	
	
			
  	     if( CAT_wait_ds(&cwat_18,10,&B_current_biao))//	B相电流
				  {			  
					 
						B_current_biao++;
					if( B_current_biao>STATE_TEMES) B_current_biao=0;
						
						CtrSend_data[16]=B_current;   //				  
					 CAT1_Send_Data_Nd(0x3000,16,CtrSend_data);	
					  
					 temp_allStateNum=allStateNum;//存储上一次的进程
					 allStateNum=1;//确定进入发送进程
						
				   }
			else  allStateNum=20;
				  
			 break;	  				 		 
					 
					 
//#define ALME_TEMES   2;
//#define STORE_TEMES  2;
//#define STATE_TEMES  2;
//#define TEMP_TEMES   1;



			case 20:	
	
  	     if( CAT_wait_ds(&cwat_19,10,&C_current_biao))//	 C相电流
				  {			  
							
					C_current_biao++;
					if( C_current_biao>STATE_TEMES) C_current_biao=0;
						
					 CtrSend_data[17]=C_current;//			  
					 CAT1_Send_Data_Nd(0x3000,17,CtrSend_data);	
					  
					 temp_allStateNum=allStateNum;
					 allStateNum=1;						
				   }
			   else  allStateNum=21;
					 
			   break;


		case 21:	//信号质量//信号质量//信号质量//信号质量//信号质量//信号质量//信号质量//信号质量//信号质量//信号质量
	
		 if( Enquiries_CSG2(3500))  allStateNum=22;//CtrSend_data[18] 在函数体里面向外发送数据 信号质量信号质量信号质量
	
		 break;	  		


		 case 22:	
	
  	     if( CAT_wait_ds(&cwat_21,200,&PresentLevel_biao))// 当前水位高度
				  {			  
				
					 CtrSend_data[19] =PresentLevel;//	  
					 CAT1_Send_Data_ResNd(0x3000,19,CtrSend_data);					  
					 temp_allStateNum=allStateNum;
					 allStateNum=1;
						PresentLevel_biao=0;
				   }
			    else  allStateNum=23;				  
			    break;	  		
					
      case 23:	
	
  	     if( CAT_wait_ds(&cwat_22,10,&again_startT_biao1))//   电机A暂停时间
				  {			  
					again_startT_biao1++;
					if( again_startT_biao1>STATE_TEMES) again_startT_biao1=0;
					// CtrSend_data[20] ;//	  
					 CAT1_Send_Data_Nd(0x3000,20,CtrSend_data);					  
					 temp_allStateNum=allStateNum;
					 allStateNum=1;
					 //again_startT_biao1=0;
				   }
			    else  allStateNum=24;				  
			    break;	  		
            
					 
			case 24:	
	
  	     if( CAT_wait_ds(&cwat_23,10,&again_startT_biao2))//    电机B暂停时间
				  {			  
					 
						 again_startT_biao2++;
					if( again_startT_biao2>STATE_TEMES) again_startT_biao2=0;
						
					// CtrSend_data[21] ;//	  
					 CAT1_Send_Data_Nd(0x3000,21,CtrSend_data);					  
					 temp_allStateNum=allStateNum;
					 allStateNum=1;
					 
				   }
			    else  allStateNum=25;				  
			    break;	  		
           			 
			 case 25:	

			 

			 
			 
			 
  	     if( CAT_wait_ds(&cwat_24,10,&again_startT_biao3))//     电机C暂停时间
				  {			  
					
          again_startT_biao3++;
					if( again_startT_biao3>STATE_TEMES) again_startT_biao3=0;
		
					// CtrSend_data[20] ;//	  
					 CAT1_Send_Data_Nd(0x3000,22,CtrSend_data);					  
					 temp_allStateNum=allStateNum;
					 allStateNum=1;
					 
				   }
			    else  allStateNum=26;				  
			    break;	  		

			 case 26:	
	

    if  (CAT1_Send_Data_dif_2(0x1000,0,storage_1,49,32))   //存储数据有变化则上传，程序有变化，需要更改上传变量数。  
       {			                                              //轮询49次才能轮询一遍，每次轮询一个数。
			        				 
				Data_dif_times++;
				
        if(Data_dif_times>=STORE_TEMES)//存储数据没有收到返回信息只发两次	
					
				{					
					Data_dif_times=0;
					CAT1_Send_Data_dif_biao=0;
				}
				 
			 }
		else allStateNum=27;	
		break; 
	 
		 case 27:	   //返回收到的数据 只返回一次
			 
		   //	static   u16 CAT_Drass;
       //	static   u16 CAT_Data;
       // u8 CAT_back_biao; 
		 
  	   if( CAT_wait_ds(&cwat_25,2,&CAT_back_biao)) //反馈数据
				  {					   			  
					                       
                      CAT1_Send_Data_ResNd(cat_feedback[0],0,&cat_feedback[1]);	//CAT_Drass地址,全局变量	//CAT_Data数据，全局变量	
                     // CAT1_Send_Data_ResNd(CAT_Drass,0,&CAT_Data);	//CAT_Drass地址,全局变量	//CAT_Data数据，全局变量				  
					 temp_allStateNum=allStateNum;//存储分支数据
					 allStateNum=1;//返回到起始到发送数据
                     CAT_back_biao=0;					  
				  }
			 else  allStateNum=2;				  
			 break;	  
		  
		case 28:
			
			  allStateNum=2;				  
			  break;	  

		default:
			allStateNum=2;
		
			break;
		
// 水泵1 3009 0000是启动   0001 是停止  0002是暂停
// 阀门1  300a  0000是关闭状态  0001是启动状态 0003 是关闭进程 
// 水泵2 300b 0000是启动   0001 是停止  0002是暂停
// 阀门2 300c  0000是关闭状态  0001是启动状态 0003 是关闭进程 
// 水泵3 300d 0000是启动   0001 是停止  0002是暂停
// 阀门3 300e  0000是关闭状态  0001是启动状态 0003 是关闭进程 

	}
	
}

int OPEN_CAT1_2(void)

{
   u8 i;
//   static u8 reset_biao=0;
//   static u16 reset_timer;
//   static u8 reset_timer_biao=0;
//	
//	if(TIMER_1MS%10<=2)
//		{
//			 if(reset_timer_biao==0) //不等于0的时候置1//执行1次
//			{
//			 if(reset_biao==1) reset_timer++;
//			 else reset_timer=0;
//			 reset_timer_biao=1;
//			}
//		}
//	else reset_timer_biao=0;
//	
	
	
   switch (ucStateNum) 
	   
	   {
	   
	   case 0x00:	

             HAL_GPIO_WritePin(GPIOC, GPIO_PIN_3,GPIO_PIN_RESET);//
		         ucStateNum=0x01;
		         ucErrorTimes = 0;
//             Uart1_SendStr("1111111\r\n"); 

             
			break;
	   case 0x01:
			 
		  if(fat_send_wait(1000))	
			//if(fat_send_wait(500))		
					{
					 ucStateNum=0x30;
					 
		             ucErrorTimes = 0;
					 HAL_GPIO_WritePin(GPIOC, GPIO_PIN_3,GPIO_PIN_SET);//
						
//						Uart1_SendStr("2222222\r\n"); 
					}
		 
		 
			  			  
			break;	
		   
	
					
					
					
		 case 0x30:
			//if(fat_send_wait(2000)) 
		 	if(fat_send_wait(1100))
					
					{
					 ucStateNum=0x031;					 
		             ucErrorTimes = 0;
					 HAL_GPIO_WritePin(GPIOC, GPIO_PIN_3,GPIO_PIN_RESET);//
						
//						Uart1_SendStr("23232323\r\n"); 
					}
		 
		 
			  			  
			break;				
					
					
			 case 0x31:
			//if(fat_send_wait(3000)) 
		 	if(fat_send_wait(2300))
					
					{
					    ucStateNum=0x03;
					 
		                ucErrorTimes = 0;
					    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_3,GPIO_PIN_SET);//
						
//						Uart1_SendStr("2424242424\r\n"); 
					}
			  			  
			break;						
					
					
					
         case 0x03:		   
   
          if( fat_send_cmd("AT\r\n",250))
			  {
				  
				
				  
				if( fat_cmdres_keyword_matching("OK") ) 
				    {
					 
					
					   ucErrorTimes = 0;
             ucStateNum=0x04;
					  }  
				   					  
					else 
						  {
						
							//Uart1_SendStr("5151\r\n");
							  
							  if(ucErrorTimes++>50)
							   {
								ucStateNum = 0x00;
								   
								   
							   }
						
						   }		 
				   
			     }
		          
			break;				
				
		case 0x04:		   
   
          if( fat_send_cmd("ATE0&W\r\n",250))
			  {
				// Uart1_SendStr("555\r\n");
                			  
				if( fat_cmdres_keyword_matching("OK") ) 
				  {
					 //HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_12);	
					
					   ucErrorTimes = 0;
                       ucStateNum=0x05;
					  
					  
				   }  
				  
				   
				   else 
				  {
				
				   if(ucErrorTimes++>5)
					   {
						ucStateNum = 0x00;
					    }
				
				   }		
				   
				   
			   }
			  
			               
			break;
				
			case 0x05:
				if (fat_send_cmd("AT+QMTDISC=0\r\n",150))
						{
				         //Uart1_SendStr("6666666\r\n");  
						ucStateNum=0x06;	
							
						}
				break;		   
            
	         case 0x06:
					if (fat_send_cmd("AT+CIMI\r\n", 300))
					{
//					     Uart1_SendStr("7777777\r\n");
						
						if (fat_cmdres_keyword_matching("460"))
						{
							ucErrorTimes = 0;
							ucStateNum=0x07;
						}
						else
						{     
											
							if (ucErrorTimes++ >=50)
							{
								ucStateNum = 0X00;
							}
						}
						
					}
					break;

			   
			  case 0x07:
					if (fat_send_cmd("AT+QICSGP=1,1,\"CMNET\",\"\",\"\",0\r\n", 150))
					//if (fat_send_cmd("AT+QICSGP=1,1,\"CMNET\",\"device\",\"12345678\",0\r\n", 150))	
					{
					 
						//Uart1_SendStr("88888888\r\n");
						
						ucStateNum=0x08;					
					  
					}
					break;
					
				case 0x08:
						if (fat_send_cmd("AT+CGATT?\r\n", 500))
						{
							//Uart1_SendStr("9999999\r\n");		
							if (fat_cmdres_keyword_matching("+CGATT: 1"))
							{
								ucErrorTimes = 0;
								ucStateNum=0x09;
							}
							else
							{     
												//发送10次得不到正确应答
								if (ucErrorTimes++ >=15)
								{
									ucStateNum = 0X00;
								}
							}
							
						}
						break;	
						
				case 0x09:
					
						if (fat_send_cmd("AT+CSQ\r\n", 150))
							{
								//Uart1_SendStr("101010\r\n");			
								if (fat_cmdres_keyword_matching("OK"))
								{
												//收到的是99（射频信号未初始化）
												if (fat_cmdres_keyword_matching("+CSQ: 99,99"))
													{
															
															if (ucErrorTimes++ > 30)
															{
																	ucStateNum = 0x00;
															}
													}
											  else
												{ 																	
														   ucErrorTimes = 0;
														   ucStateNum=0x0A;
													
				//										if (match_csq(cFatUartRecvBuf, SIGNALMIN, SIGNALMAX))
				//										  {
				//										   ucErrorTimes = 0;
				//										   ucStateNum++;
				//										  }
				//										else 
				//										 {
				//											 ucStateNum = MD_ERR;
				//										 }
												}
								}
										
							else
								{
													//发送30次不应答
									if (ucErrorTimes++ > 5)
									{
										ucStateNum = 0X00;
									}
								}
						}
						break;						
					 		
				 case 0x0A:
						
				        if (fat_send_cmd("AT+QIACT=1\r\n", 150))
							{
								
							//Uart1_SendStr("121212\r\n");	
							 ucStateNum=0x0B;				
							 
							}
							break;										
			     	
					 case 0x0B:
						if (fat_send_cmd("AT+QIACT?\r\n", 150))
						{
						 
						//Uart1_SendStr("131313\r\n");	
						ucStateNum=0x0C;					
						  
						}
						break;						
							
						case 0x0C:
								if (fat_send_cmd("AT+GSN\r\n", 25))
										
									{
													
										if(ec600_RxBuffer[2]>='0'&&ec600_RxBuffer[2]<='9')//获取模块IMEI的值获取的值，本身就是字符型的
										{
												 
											
											memset(( char *)EC600uinit.pubtopic,0,sizeof(EC600uinit.pubtopic));//清零内存	最快的清内存函数 
											memset(( char *)EC600uinit.subtopic,0,sizeof(EC600uinit.subtopic));//清零内存	最快的清内存函数  
											memset(( char *)EC600uinit.IMEI,0,sizeof(EC600uinit.IMEI));        //清零内存	最快的清内存函数  	
											
											
											memcpy(( char *)EC600uinit.IMEI,ec600_RxBuffer+2,15);//内存拷贝	从ec600_RxBuffer【2】开始拷贝15个数到	EC600uinit.IMEI数组中											
											sprintf(( char *)EC600uinit.pubtopic,"app/%s",EC600uinit.IMEI);//%s站位符号，数据结构对应起来
                      sprintf(( char *)EC600uinit.subtopic,"dev/%s",EC600uinit.IMEI);
													
													
//													Uart1_SendStr("获取到设备IMEI号码：\r\n");													
												   //Uart1_SendDATA((char *)EC600uinit.subtopic,19);
//													Uart1_SendStr((char *)EC600uinit.subtopic);						
//											        Uart1_SendStr("\r\n");	
											ucErrorTimes = 0;
											ucStateNum=0x0D;
													
										}
										else
											{
												//发送3次得不到正确应答
												if (ucErrorTimes++ > 3)
												{
													ucStateNum = 0x00;
												}
											}
									}
									break;	
						
					case 0x0D:
					if (fat_send_cmd("AT+QMTOPEN=0,\"bangmuyun.com\",1883\r\n", 250))//300有时候卡壳	
					//if (fat_send_cmd("AT+QMTOPEN=0,\"broker.emqx.io\",1883\r\n", 250))//300有时候卡壳
					//if (fat_send_cmd("AT+QMTOPEN=0,\"39.98.107.130\",1883\r\n", 250))//300有时候卡壳
						{							
							//收到+QMTOPEN: 0,0
							if (fat_cmdres_keyword_matching("+QMTOPEN: 0,0"))
							{
								ucErrorTimes = 0;
								ucStateNum=0x0E;
							}
							else
							{
												//发送10次得不到正确应答
								if (ucErrorTimes++ > 30)
								{
									ucStateNum = 0x00;
								}
							}
						}
					 break;		
						
					 case 0x0E:
				
				    sprintf(buf,"AT+QMTCONN=0,\"%s\",\"device\",\"123456\"\r\n",EC600uinit.IMEI);// \"\" 就是表示双引号，s%表示占位符,12345678是密码  debice是用户名
			
							if (fat_send_cmd(buf,250))
							{
											//收到+QMTCONN: 0,0,0
								if (fat_cmdres_keyword_matching("+QMTCONN: 0,0,0"))
									{
									  ucErrorTimes = 0;
									  ucStateNum=0x0F;
									 }
								else
									{
														//发送10次得不到正确应答
										if (ucErrorTimes++ > 3)
										{
											ucStateNum = 0x00;
										}
									}
							}
							
							break;						
	
					  case 0x0F:
		
							 sprintf(buf,"AT+QMTSUB=0,1,\"%s\",0\r\n",EC600uinit.subtopic);//订阅主题      
						
							if (fat_send_cmd(buf, 250))
							{
											//收到+QMTSUB: 0,1,0,0
								if (fat_cmdres_keyword_matching("+QMTSUB: 0,1,0,0"))
								{
									ucErrorTimes = 0;
									//ucStateNum = MD_OPEN_OK;
									ucStateNum = 0x10;
									break;
								}
								else
								{
													//发送5次得不到正确应答
									if (ucErrorTimes++ > 3)
									{
										ucStateNum = 0x00;
									}
								}
							}
							break;
		
						
						//case   MD_OPEN_OK: //返回数和MD_OPEN_OK是同一个会出现问题	
						case   0x10:
						send_reg1=2;
						ucStateNum = 0x20;
						//Uart1_SendStr("MD_OPEN_OK\r\n");//放在这个位置可以执行
						return MD_OPEN_OK;//返回数和MD_OPEN_OK是一个回出现问题
						//Uart1_SendStr("MD_OPEN_OK\r\n");//放在这个位置不执行 
						break;
						
		   }

		   
	   }


/*
*return 后面的语句不会在执行了
*/


void cat1_timeadd(void) //放到时间中断中

{
 
 if(cat1_receive_biao==1) cat1_receive_timer++;
 if(cat1_receive_biao_D==1) cat1_receive_timer_D++;

}



void cat1_receive(void)//放到串口接收回调函数里面
  {
	
       cat1_receive_biao=1;  //有中断就开启计时开关
       cat1_receive_timer=0; //只要有中断，计时就清零
	  
	     cat1_receive_biao_D=1;  //有中断就开启计时开关
       cat1_receive_timer_D=0; //只要有中断，计时就清零，不等于零的时候说明数据接收数据就完成了
	  
	  ec600_RxBuffer[ ec600u_receive_count++]=aRxBuffer2[0];	  
	  if(ec600u_receive_count>100) ec600u_receive_count=100;
	  
    ec600_RxBuffer_test[ ec600u_receive_count_test++]=aRxBuffer2[0];	  
	  if(ec600u_receive_count_test>100) ec600u_receive_count_test=100;
	
   }


/*
放到主函数里面
*/



 void cat1_excute(void)

{
	
	char *p = NULL;	
	int cat_receiv_biao=1;
	
	
if(cat1_receive_timer_D>=2)//超过一定时间开始处理数据
	{	 
	
	  cat1_receive_biao_D=0;//关闭计时累加时间
    cat1_receive_timer_D=0;//确保这个函数只执行1次	
		ucFatUartRecvFinishFlg=1;//串口接收完成标志
//		//strcat(test,ec600_RxBuffer_test);
//		//Uart1_SendStr(ec600_RxBuffer);		
//		Uart1_SendStr((char*)ec600_RxBuffer_test);
		cat_receiv_biao=match_recv_topic((char*)ec600_RxBuffer_test, (char *)CATrecvbuf); // "+QMTRECV: 0,0,\""  截取有效数据到 CATrecvbuf
		
//		Uart1_SendStr((char*)CATrecvbuf);
//    Uart1_SendStr((char*)ec600_RxBuffer_test);		
//		Rs4851send_int16_Data_1(0x12);		
		if(cat_receiv_biao!=0)
		     {
			  
//				 Rs4851send_int16_Data_1(0x13); 
				 Read_cat();         //把对应地址数据转换成16进制并写入到对应的数组里面
				 
//			  Rs4851send_int16_Data_1(0x14); 
				 
			  cat_receiv_biao=0;	 
				 
             }
		Clear_Buffer_D();  
 
	 }

}




/**
 * @description: 截取socket接收缓存区的的数值
 * @param cmdres：命令响应数据
 * @param recvbuf：socket接收缓存
 * @return 接收Topic的长度
 */
/*+QMTRECV: 0,0,"dev/869537052769784","5AA51001003283"
*
*  ,的asc码是0x2C "的ASC码0x22
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
    p = p + 15;
    //q = p;
    while (*p != 0x2C)//计算后面有效数值的字节数，一直加加到“””停止
     {
        p++;
      }
	 
	 q = p+2;  //根据实际确定的
	 
    while (*p != 0x00)
     {
        p++;
      }

	  
    length = p - q-3 ;//根据实际情况确定的
	if(length>=90) length=0; 
    memset(recvbuf, 0, 100);//内存清零
    memcpy(recvbuf, q, length);//截取有效数据
    return length;
}






// C prototype : void StrToHex(unsigned char *pbDest, unsigned char *pbSrc, int nLen)
// parameter(s): [OUT] pbDest - 输出缓冲区
// [IN] pbSrc - 字符串
// [IN] nLen - 16进制数的字节数(字符串的长度/2)
// return value:
// remarks : 将字符串转化为16进制数

void StrToHex(unsigned char *pbDest, unsigned char *pbSrc, int nLen)
{
	char h1,h2;
	unsigned char s1,s2;
	int i;

	for (i=0; i<nLen; i++)
		{
			h1 = pbSrc[2*i];
			h2 = pbSrc[2*i+1];

			s1 = toupper(h1) - 0x30;
			if (s1 > 9)
				s1 -= 7;

			s2 = toupper(h2) - 0x30;
			if (s2 > 9)
				s2 -= 7;

			pbDest[i] = s1*16 + s2;
		}
}




//如果收到接收数据标志则执行下面的函数


 void Read_cat(void)//从LCD中读取数据对应地址写到对应的变量中，要和显示的对应起来
  {
//	static   u16 CAT_Drass;
//	static   u16 CAT_Data;
//           u8 CAT_back_biao;

	 static   u8 CAT_receive[30];//这个是给字符转数字函数用的
	 int nLen; 
	 static  u16 temp[3]; 	
	 u8 i; 
	  //storage_1[i] 在别的地方定义了是主存储数组里面	  
	 
 nLen = strlen((char*)CATrecvbuf)/sizeof(char);//计算字符转长度
	  
 StrToHex(CAT_receive, CATrecvbuf,  nLen/2);//吧字符转换成数字

	    	temp[0]=CAT_receive[0];
			temp[0]=(temp[0]<<8)+CAT_receive[1] ;  //temp1字头
        
		CAT_head=temp[0];
      
    		temp[1]=CAT_receive[2];
			temp[1]=(temp[1]<<8)+CAT_receive[3] ;  //temp2接收的时地址	  
			  
		CAT_Drass=temp[1];//CAT_Drass全局便量接收到的地址
				
			temp[2]=CAT_receive[4];
			temp[2]=(temp[2]<<8)+CAT_receive[5] ;   //temp2接收的地址里面的数	
	            
		CAT_Data=temp[2];//CAT_Data全局变量	接收到的数据
        
	
//	if(CAT_head==0xA55A)   CAT_back_biao=1; //CAT_back_biao全局变量只有符合标头的才能反馈	，开启反馈标识			
//  
//	/*
//	下面这里和老版本有区别
//	 老版本存在不稳定的因素
//	 这个不存在了。
//	*/
//	
//	if(CAT_back_biao==1)  
//	{ 
//		if(send_reg1==2)
//			{
//			allStateNum=27;
//			}
//	}



    
   if(CAT_head==0xA55A)  
	{ 
		Write_RingBuff_cat(&Cat_fedback_data, CAT_Drass,CAT_Data); 
	} 
    
    
 
	
	
   for(i=0;i<100;i++)//注意 不要超出数组的定义范围
		  {
		  
			if(CAT_Drass==(0x1000+i)&&(CAT_head==0xA55A)) //存储数组1000
			  {               				
				  if(storage_1[i]!=CAT_Data) //  不一样则存储到数组里面
					{
					  CAT_storage[i]=CAT_Data;//   CAT_storage[i]是存储设定值的数组，单片机设置值变换以后，
						                      //   要与这个数组的数值进行比较，不一样就上发数组，一边小程序可以更新数值
					  storage_1[i]=CAT_Data;  //	 storage_1[i] 设定值数组  
					  write_eepall_1(0) ; 
					}
			   }
			
				 
			if(CAT_Drass==(0x1000+i)&&(CAT_head==0xB55B)) //反馈数据存储反馈数组1000
			  {               				
				  if(res_storage_1[i]!=CAT_Data) 
					{ 
					  res_storage_1[i]=CAT_Data;						 
					}
			   }	 
				 
		  }
		  

		  
    for(i=0;i<30;i++)//收到的控制命令数组2000
		  {
		 		  
			if(CAT_Drass==(0x2000+i)&&(CAT_head==0xA55A)) 
			  {  
               				
				  if(Control_data[i]!=CAT_Data) 
					  { 
					   Control_data[i]=CAT_Data;						 
					  }
					
			   }
			  
		  }		  
	  

 for(i=0;i<30;i++)//收到的反馈3000数据
		  {
		 		  
			if(CAT_Drass==(0x3000+i)&&(CAT_head==0xB55B)) 
			  {  
               				
				  if(TCtrSend_data[i]!=CAT_Data) 
					 { 
					  TCtrSend_data[i]=CAT_Data;						 
					 }
					
			   }
			  
		  }  
		  
			for(i=0;i<100;i++)
			
			{
			 CATrecvbuf[i]=0;//清除缓存
			}
			
			
  }




 void MX_IWDG_Init(void)
{

  /* USER CODE BEGIN IWDG_Init 0 */

  /* USER CODE END IWDG_Init 0 */

  /* USER CODE BEGIN IWDG_Init 1 */

  /* USER CODE END IWDG_Init 1 */
  hiwdg.Instance = IWDG;
  hiwdg.Init.Prescaler = IWDG_PRESCALER_64;
  hiwdg.Init.Window =2000;
  hiwdg.Init.Reload = 2000;
  if (HAL_IWDG_Init(&hiwdg) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN IWDG_Init 2 */

  /* USER CODE END IWDG_Init 2 */

}

void IWDG_Feed(void)
{
	 HAL_IWDG_Refresh(&hiwdg);//喂狗
}

void Clear_Buffer(void)//清空缓存
{
    uint8_t i;
    
    //for(i=0;i<ec600u_receive_count;i++)//ec600u_receive_count自动在终端里面增加的数
	
 for(i=0;i<100;i++)//ec600u_receive_count自动在终端里面增加的数
   { 
	   ec600_RxBuffer[i]=0;//缓存	
	   
   }
	
    ec600u_receive_count=0;
    IWDG_Feed();//喂狗
}


void Clear_Buffer_D(void)//清空缓存
{
    uint8_t i;
    
    //for(i=0;i<ec600u_receive_count;i++)//ec600u_receive_count自动在终端里面增加的数
	
 for(i=0;i<100;i++)//ec600u_receive_count自动在终端里面增加的数
   { 
	 
	   ec600_RxBuffer_test[i]=0;//缓存
   }
	ec600u_receive_count_test=0;
     
}


//void Clear_Buf_Buf(uint8_t *p)//清空缓存

//{

// memset(p,0,sizeof(p));

//}

void Clear_Buf(void)//清空缓存
{
    uint8_t i;
    //Uart1_SendStr((char*)RxBuffer);
    for(i=0;i<100;i++)//ec600u_receive_count自动在终端里面增加的数
    buf[i]=0;//缓存
    
   // IWDG_Feed();//喂狗
}

//模块发送数据，一起发送多几个，变量有变化就发送，不变化不发送//主要是反馈收到的信息 
//发送已经设置好的数值
//if  (CAT1_Send_Data_dif_2(0x1000,0,storage_1,49,32))   
int CAT1_Send_Data_dif_2(u16 base ,u16 addr,u16 *data,u8 number,u8 time)
  {
	 static u8 i=0;		
	 static u8 Senddif_timer=0;
   static  u8 Senddif_timer_biao=0;
		
		
	 if(TIMER_1MS%100<=30) 			 
			{
			 if(Senddif_timer_biao==1)	
			   {
  				 Senddif_timer++;
				   Senddif_timer_biao=0;
				   if(Senddif_timer>=time) Senddif_timer=time;
				 } 
			}
	  else 			  
			{
			 Senddif_timer_biao=1;  
			}

  if(send_ok_biao==1) //两个数值一样标志，也就是说单片机没有设置数值		
		   { 
			  i++;
			  send_ok_biao=0;
			  if(i>=number) i=0;
		   }  
			
	if(data[addr+i]!= CAT_storage[addr+i])	//查询单片机设置了数值没有
			{		
				CAT1_Send_Data_dif_biao=1;//两个数值不一样标志，也就是说单片机设置了数值
				Data_dif_times=0;//全局变量，清零发送时间，感觉没有也可以。
				CAT_storage[addr+i]=data[addr+i];
				Senddif_timer=time-3;	//	立马发送		
			}
	
	if((send_reg1==2)&&(Senddif_timer>=time))//表示module_data2函数数据发送完成
	   {  
      if(CAT1_Send_Data_dif_biao==1)	//两个数值不一样标志，也就是说单片机设置了数值		  
			 {			  
			   Cat1_data_dif_temp= data[addr+i];
				 CAT1_data_dif_num=addr+i;             //把i转移出去
				 
			   addr_data=base+addr+i;
			   send_data=&data[addr+i];
				 
			   data_number=1;//告诉module_data3（）发送一个数据			   
			   send_reg1=1;//启动一次发送
			   ucStateNum=0x20;  
			   excue_biao=0;
			   Senddif_timer=0;
				CAT1_data_res_biao=0xA55A;
				
    		 temp_allStateNum=allStateNum;
				 allStateNum=1;		
				return 1;
			  }

		   else //两个数值一样标志，也就是说单片机没有设置数值		
			  {
				  send_ok_biao=1;
			  }

		 }
   return 0;//必须放在最后，或者使用全局变量		
 }

//模块发送数据，一起发送多几个，变量有变化就发送，不变化不发送//开机的时候发送信息 
 //发送已经设置好的变量组 
void CAT1_Send_Data_Str(u16 base ,u16 addr,u16 *data,u8 number)
  {
	  static u8 i=0;

	  if(send_reg1==2)//表示module_data2函数数据发送完成
	   {  
			   addr_data=base+addr;
			   send_data=&data[addr];		   
			   data_number=number;//告诉module_data3（）发送几个数据
			   send_reg1=1;//启动一次发送
	       ucStateNum=0x20;  
			   excue_biao=0;
         CAT1_data_res_biao=0xB55B;

		 }
   
 }

 
 //模块发送1个数据，
 //发送的数据需要时发送时设置，或者原来设置好的
 int CAT1_Send_Data_Nd(u16 base ,u16 addr,u16* data)   
  {   	  
	  if(send_reg1==2)//表示module_data2函数数据发送完成
	   { 		   
			   addr_data=base+addr;	 //		addr_data全局变量给发送数据函数   
			   send_data=&data[addr];//   send_data全局变量给发送数据函数
			   data_number=1;//           全局变量给发送数据函数
			   send_reg1=1;//启动一次发送不让其他的发送再介入
			   CAT1_data_res_biao=0xA55A;//
			   ucStateNum=0x20;  //
			   excue_biao=0;	// 在函数 module_data3（）里面执行数据地址解析
		      
		 } 
 }

 
 //模块发送数据，发送一个反馈数据 
 //发送的数据需要时发送时设置，或者原来设置好的
 // CAT1_Send_Data_ResNd(0x3000,19,CtrSend_data);//CtrSend_data这是个数组名字
 int CAT1_Send_Data_ResNd (u16 base ,u16 addr,u16* data)  
  {
   
	  
	  if(send_reg1==2)//表示module_data2函数数据发送完成
	   { 
		   
			   addr_data=base+addr;		//		全局变量给发送数据函数 	   
			   send_data=&data[addr]; //    全局变量给发送数据函数，把一个数组元素的地址赋值。
			   data_number=1;//             全局变量给发送数据函数
			   send_reg1=1;//启动一次发送不让其他的发送再介入			  
			   CAT1_data_res_biao=0xB55B;//发送字头选择标志
			   ucStateNum=0x20;       //全局变量，进入module_data3的发送部分
			   excue_biao=0;	        //全局变量  进入module_data3后执行发送数据组帧
		 
		 }
 
 } 

//module_data3(addr_data,send_data,data_number);
int module_data3(uint16_t Variable_addr,uint16_t*Dev_Data,u8 number)//一次最多发送20个整形变量加校验。
 { 

//int ret = 0;	
     static char Addr_Buf[300];//决定一次最多变量个数，一个整形数需要
     static char Dev_Buf[4];
	   static uint8_t data_len;
     uint8_t i;	 
//	 static u8 excue_biao=0; //全局变量
	 
	 if(excue_biao==0)
     {
			 
			 memset(Addr_Buf,0,sizeof(Addr_Buf));
			 memset(Dev_Buf,0,sizeof(Dev_Buf));
			 
			 if( CAT1_data_res_biao==0xA55A)  sprintf(Addr_Buf,"A55A%x",Variable_addr); //Variable_addr 地址，地址为什么没有转化成ASC码？
			                                                                            //答：双引号本身就是就是转化成字字符型了，所以地址 经过sprintf函数以后就编程字符型了
			 
			 if( CAT1_data_res_biao==0xB55B)  sprintf(Addr_Buf,"B55B%x",Variable_addr); //Variable_addr 地址
				 
				 
//				 for (i=0;i<number;i++)
//							{
//						
//							myitoa2(Dev_Data[i], Dev_Buf);	//Dev_Data[](整型)转换成ASC码到dev_Buf	这里为什么不用sprintf函数？用sprintf函数是否会是一样的？
//								                              //答一的，当时我不知道有格式输出符号，所以输出的都是1   2    3  我希望输出的是 0001  0002，所以弄了一个 转换函数
//                                              //其实可以		sprintf(buf4,"%04x\n",Dev_Data[i]);						
//							strcat(Addr_Buf,Dev_Buf);      //strcat(str,ptr)是将字符串ptr内容连接到字符串str后
//						
//							}
			 
/*
上下实现的功能是一样的
*/
			 
			 for (i=0;i<number;i++)
							{						
							sprintf(Dev_Buf,"%04x",Dev_Data[i]);
			                strcat(Addr_Buf,Dev_Buf);						
							}
			
					data_len = strlen(Addr_Buf)/sizeof(char);//计算数据长度
					
					if(number>1) 
						{
						
						 crc=crc16(Addr_Buf,data_len);  
						 myitoa2(crc, Dev_Buf);	        //转换成整形转换成ASC码到dev_Buf		
						 strcat(Addr_Buf,Dev_Buf);      //strcat(str,ptr)是将字符串ptr内容连接到字符串str后	
						
						}
							

						data_len = strlen(Addr_Buf)/sizeof(char);
						sprintf(buf,"AT+QMTPUBEX=0,0,0,0,\"%s\",%d\r\n",EC600uinit.pubtopic,data_len);
						excue_biao=1;

					}
	 
    switch (ucStateNum)
    { 
		case 0x20:
			//if (fat_send_cmd(buf, 80))//30（15毫秒）最小了
			if (fat_send_Ncmd(buf, 300))//30（15毫秒）最小了	
			
			
				{
					//Uart1_SendStr("MD_DATA_OK\r\n");

					//收到>+QMTPUBEX: 0,0,0
					if (fat_cmdres_keyword_matching(">"))
					//if (fat_cmdres_keyword_matching("+QMTPUBEX: 0,0,0"))	
					{
						ucErrorTimes = 0;
						ucStateNum = 0x21;
						
						
					}
					else
					{
										//发送3次得不到正确应答
						if (ucErrorTimes++ > 20)
						{
							//ucStateNum = MD_ERR;
							ucStateNum = 23;
							
						}
					}
				}
				
				break;		
	     case 0x21:
        
				//Uart1_SendDATA(Addr_Buf,data_len);	
				//if (fat_send_cmd_2(Addr_Buf,data_len,80))//带字节长度发送函数
				if (fat_send_Ncmd_2(Addr_Buf,data_len,80))//带字节长度发送函数	
					{
									//收到OK
						if (fat_cmdres_keyword_matching("+QMTPUBEX: 0,0,0"))	
								//if (fat_cmdres_keyword_matching("OK"))
								{
									ucErrorTimes = 0;
									ucStateNum=0x22;
									feedback_biao=1;//使能反馈发送队列读标志，发送完了可以开启下次读取缓存
									//Uart1_SendDATA("xxxxxx",6);
								}
						else
						{
											//发送3次得不到正确应答
								if (ucErrorTimes++ > 20)
								{
									ucStateNum = MD_ERR;
								}
						 }

					 }
					break;	
				
							
		case   0x22:	    	
		send_reg1=2;
		
		//ucStateNum=0x20;
	    break;
        
		case   MD_ERR:
	    send_reg1=0;
      ucStateNum=0x0D;//进入链接网络		
	    break;

		
		  case   23:
	    send_reg1=0;
      ucStateNum= 0x0F;//进入链接网络		
	    break;
		
		
		
    }
    
		

    }
		
		

 
 
 int Enquiries_CSG(u8 time)//time秒后读取一次信号质量
    {
   
   static   int lpCsqVal = 0;   
   static u8 CSQ_timer=0;
   static u8 CSQ_timer_biao=0;  
	
//标准计时结构//标准计时结构//标准计时结构//标准计时结构//标准计时结构
	  
	  if(TIMER_1MS%1000<=10) 
			 
			{
			 if(CSQ_timer_biao==1)
				 {
				   CSQ_timer++;
				   CSQ_timer_biao=0;
				 } 
			}
	  else 
		  
			{
			 CSQ_timer_biao=1;  
			}
  
  if((send_reg1==2)&&(CSQ_timer>=time))	
	  {
		  send_reg1=3;
		  CSQ_timer=0;//重新开始计时
		  
	  }	

 if(send_reg1==3)
 {
		if (fat_send_cmd("AT+CSQ\r\n", 50))
							{
								//Uart1_SendStr("101010\r\n");			
								if (fat_cmdres_keyword_matching("OK"))
								{
												//收到的是99（射频信号未初始化）
												if (fat_cmdres_keyword_matching("+CSQ: 99,99"))
													{
															
															if (ucErrorTimes++ > 30)
															{
																	send_reg1=0;
                                  ucStateNum=0x00;	
															}
													}
											  else
												{ 													
				 									 // G_lpCsqVal =  match_csq((char*)ec600_RxBuffer);
													   
													   lpCsqVal=  match_csq((char*)ec600_RxBuffer);
													   send_reg1=2;		                                               
													   excue_biao=0;													
													   ucErrorTimes=0;//2022_5_7
													  //return lpCsqVal;放在这里会出现问题
														
												 }
								}
										
							else
								{
													
								if (ucErrorTimes++ > 5)
									 {
										send_reg1=0;
                    ucStateNum=0x00;	
									  }
								}
						}
						
			 
		}
     return lpCsqVal;//必须放在最后，或者使用全局变量
		
	}
  
 
int Enquiries_CSG2(u16 time)//time秒后读取一次信号质量
    {
   
   static   int lpCsqVal = 0;   
   static u16 CSQ_timer=2000;
   static u8 CSQ_timer_biao=0;  
   static u8 ture_biao=1;
   
//标准计时结构//标准计时结构//标准计时结构//标准计时结构//标准计时结构
	
	  
	  if(TIMER_1MS%100<=50) 
			 
			{
			 if(CSQ_timer_biao==1)
				 {
				   CSQ_timer++;
				   if(CSQ_timer>=time)  CSQ_timer= time;
				   CSQ_timer_biao=0;
				 } 
			}
	  else 
		  
			{
			 CSQ_timer_biao=1;  
			}
  
  if((send_reg1==2)&&(CSQ_timer>=time))	
	  {
		  send_reg1=3;
		  CSQ_timer=0;//重新开始计时
		  
	  }	
 	
	  
 if(send_reg1==3)
 {
		if (fat_send_cmd("AT+CSQ\r\n", 50))
							{
								//Uart1_SendStr("101010\r\n");			
								if (fat_cmdres_keyword_matching("OK"))
								{
												//收到的是99（射频信号未初始化）
												if (fat_cmdres_keyword_matching("+CSQ: 99,99"))
													{
															
															if (ucErrorTimes++ > 30)
															{
																  send_reg1=0;
                                                                  ucStateNum=0x00;
																  allStateNum=0;
															}
													}
											  else
												{ 													
				 									 // G_lpCsqVal =  match_csq((char*)ec600_RxBuffer);
													   
													   lpCsqVal=  match_csq((char*)ec600_RxBuffer);
													   send_reg1=2;		//CAT1_Send_Data_Nd   需要                                               
													   excue_biao=0;													
													   ucErrorTimes=0;//2022_5_7
													   
													 CtrSend_data[18] =lpCsqVal;//	  
					                                 CAT1_Send_Data_ResNd(0x3000,18,CtrSend_data);					  
					                                 temp_allStateNum=allStateNum;
					                                 allStateNum=1;					
													
													  //return lpCsqVal;放在这里会出现问题
														
												 }
								}
										
							else
								{
													
								if (ucErrorTimes++ > 5)
									 {
										send_reg1=0;
                                        ucStateNum=0x00;
										allStateNum=0; 
									  }
								}
						}
						
		return 0;	 
		}
     return 1;//必须放在最后，或者使用全局变量		他执行完就推出函数了
	}
 
 /**
 * @description:
 * @param str：要检索的字符串
 * @param minval：要匹配信号质量区间最小值
 * @param minval：要匹配信号质量区间最大值
 * @return 0:信号质量不满足正常工作状态, 1:信号质量满足正常工作状态
 */
int match_csq(char *str)
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
    return lpCsqVal; 
	
	/* 判断信号质量是否在设置的区间内 */

//    if (lpCsqVal >= minval && lpCsqVal <= maxval)
//    {
//        return 1;
//    }
//    return 0;
		
}
 


void myitoa2(int value, char *string)  //16进制整形转换成ASC码到dev_Buf	

{
  uint8_t str[4];
  uint8_t i;
	
  str[0]=(value>>12)&0x0f;
  str[1]=(value>>8)&0x0f;
  str[2]=(value>>4)&0x0f;
  str[3]=value&0x0f;
  for(i=0;i<4;i++)	
     {
	 
	 if (str[i] < 10)
	   {
		 string[i] = str[i] + '0';
		}
	 
	else
	   {
		string[i] = str[i] -10 +'a';
	   }
	 
	 }
}



void Cat_chushihua(void)
{
 u8 i;
	
for (i=0;i<80;i++)
				 {
					 
					  CAT_storage[i]=storage_1[i];
					  
				 }

} 




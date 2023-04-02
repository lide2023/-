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
	u8 cat1_receive_timer;//�ŵ�ʱ���ж���
	
	u8 cat1_receive_biao_D;
 	u8 cat1_receive_timer_D;//�ŵ�ʱ���ж���	
	

	 uint8_t ucTimetStartFlg=0;//��ִ֤��һ�ο���
	 uint16_t usTimeOutCount;//��¼��������ʱ��
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
  u16  CAT_storage[100]={0xffff};//CAT1_Send_Data_dif()�м亯�� 
	
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

u16 CAT1_data_res_biao;//�����������������ݻ��Ƿ�������
u8 Data_dif_times;  //���ʹ洢���������һ���ͷ���

uint8_t feedback_biao=1;//ȫ�ֱ��� 
uint16_t cat_feedback[2];//ȫ�ֱ��� 

/*
*��̺ܶ඼���﷨����
*һ��Ҫ����С�Ļ������ܵ�λ�༭
*2022��4��23���ĵ�
*/


  
void Uart2_SendDATA(char *data,unsigned char len)
{
	unsigned char i;
	for(i=0;i<len;i++)
	{
   while((USART2->ISR&0X40)==0){}//�ȴ�������� 
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
   while((USART4->ISR&0X40)==0){}//�ȴ�������� 
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
   while((USART1->ISR&0X40)==0){}//�ȴ�������� 
   USART1->TDR = (uint8_t) data[i]; 
	 }
}


void Uart2_SendStr(char*SendBuf)//����1��ӡ����
{
	while(*SendBuf)
	{
        while((USART2->ISR&0X40)==0){};//�ȴ�������� 
        USART2->TDR = (uint8_t) *SendBuf; 
		    SendBuf++;
	}
}



void Uart1_SendStr(char*SendBuf)//����1��ӡ����
{
	while(*SendBuf)
	{
        while((USART1->ISR&0X40)==0){};//�ȴ�������� 
        USART1->TDR = (uint8_t) *SendBuf; 
		SendBuf++;
	}
}


void Uart4_SendStr(char*SendBuf)//����1��ӡ����
{
		DE485=1;
	delay_ms(1);
	
	while(*SendBuf)
	{
        while((USART4->ISR&0X40)==0){};//�ȴ�������� 
        USART4->TDR = (uint8_t) *SendBuf; 
		SendBuf++;
	}
	delay_ms(1);
		DE485=0;
	delay_ms(1);
	
}




int fat_send_cmd(char *cmd, unsigned short int timeoutval)//ִֻ��һ�Σ�timeoutvalʱ���Ժ����ִ��������������
{
//	 uint8_t ucTimetStartFlg=0;//��ִ֤��һ�ο���
//	 uint16_t usTimeOutCount;//��¼��������ʱ��
//	char temp[20];
	
	static u16 biao;

	/* �������������ʱ���� */
	if (ucTimetStartFlg == 0)
	{
		
		
		 //sprintf(temp,cmd);	    
	     //Uart2_SendStr(temp);
	   Clear_Buffer();
		
		 Uart2_SendStr(cmd);	
	   Uart4_SendStr(cmd);	
		 ucTimetStartFlg = 1;//������ʱȷ��ִ��1��
	}

	/* �����趨�Ķ�ʱʱ�� */
	
	
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


int fat_send_Ncmd(char *cmd, unsigned short int timeoutval)//ִֻ��һ�Σ�timeoutvalʱ���Ժ����ִ��������������
{
//	 uint8_t ucTimetStartFlg=0;//��ִ֤��һ�ο���
//	 uint16_t usTimeOutCount;//��¼��������ʱ��
//	char temp[20];
	
	static u16 biao;

	/* �������������ʱ���� */
	if (ucTimetStartFlg == 0)
	{
		
		
		 //sprintf(temp,cmd);	    
	     //Uart2_SendStr(temp);
	   Clear_Buffer();
		 Uart2_SendStr(cmd);	
	   Uart4_SendStr(cmd);	
		 ucTimetStartFlg = 1;//������ʱȷ��ִ��1��
	}

	/* �����趨�Ķ�ʱʱ�� */
	
	
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



 
int fat_send_cmd_2(char *cmd,unsigned char length, unsigned short int timeoutval)//ִֻ��һ�Σ�timeoutvalʱ���Ժ����ִ��������������
{
//	 uint8_t ucTimetStartFlg=0;//��ִ֤��һ�ο���
//	 uint16_t usTimeOutCount;//��¼��������ʱ��
	
	
	static u16 biao;
	
	/* �������������ʱ���� */
	if (ucTimetStartFlg == 0)
	{

		//sprintf(temp,cmd);	    
	     //Uart2_SendStr(temp);
	     Clear_Buffer();
		 Uart2_SendDATA(cmd,length);
		// Uart4_SendDATA(cmd,length);		
		 ucTimetStartFlg = 1;//������ʱȷ��ִ��1��
	}

	/* �����趨�Ķ�ʱʱ�� */
	
	
	if(usTimeOutCount>timeoutval)
	  {
	    ucTimetStartFlg=0;
		usTimeOutCount = 0; 
		biao=1;  
		  
	   }
	else biao=0;
	return biao;
 }


 
 
int fat_send_Ncmd_2(char *cmd,unsigned char length, unsigned short int timeoutval)//ִֻ��һ�Σ�timeoutvalʱ���Ժ����ִ��������������
{
//	 uint8_t ucTimetStartFlg=0;//��ִ֤��һ�ο���
//	 uint16_t usTimeOutCount;//��¼��������ʱ��
	
	
	static u16 biao;
	
	/* �������������ʱ���� */
	if (ucTimetStartFlg == 0)
	{

		//sprintf(temp,cmd);	    
	     //Uart2_SendStr(temp);
	     Clear_Buffer();
		 Uart2_SendDATA(cmd,length);
		// Uart4_SendDATA(cmd,length);		
		 ucTimetStartFlg = 1;//������ʱȷ��ִ��1��
	}

	/* �����趨�Ķ�ʱʱ�� */
	
	
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




int fat_send_wait( unsigned short int timeoutval)//ִֻ��һ�Σ�timeoutvalʱ���Ժ����ִ��������������
{
	
	//   if(ucTimetStartFlg==1) { usTimeOutCount++ ;}//�ŵ�ʱ���ж�����

    
	
	/* �������������ʱ���� */
	if (ucTimetStartFlg == 0)
	{
		
         usTimeOutCount = 0;
		 ucTimetStartFlg = 1;//������ʱȷ��ִ��1��
	}

	/* �����趨�Ķ�ʱʱ�� */
	
	
	if(usTimeOutCount>timeoutval)
	  {
	    ucTimetStartFlg=0;
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
		
		/* �����������ؼ��� */
		if (strstr((const char *)(ec600_RxBuffer), keyword) != NULL)//ǿ������ת��(const char *)
		{
			
//			Uart1_SendStr((char *)ec600_RxBuffer);
			//Clear_Buffer();
			return 1;			
						
		}
	
	return 0;
}




/*
*������ֵ�ĺ������������static���
*������һ����֧������break;
*��ôû��return����caseֵ
*�����return�򷵻�return����ĵ�ֵ
*
*/

void Ask_for(void)//Ӧ���ѯ�ź�

{
 
    
if((Cat_fedback_data.Lenght>=1)&&(feedback_biao==1))//feedback_biaoȫ�ֱ������ŵ����ͺ���������1;	Cat_fedback_data.Lenght ��������������
{
// uint16_t cat_feedback[2];//ȫ�ֱ���
 feedback_biao=0;//ִ��һ��    
 CAT_back_biao=1;//�������ݱ�־
 Read_RingBuff_cat(&Cat_fedback_data, cat_feedback);
 
//if(send_reg1==2) allStateNum=27;//�����굽��һ������  V 61�汾û�У�ȷ�������������ء� 
    
    
}
  
    
    
     if(Control_data[6]==1) //��ѯ���1����
		  {
		   A_current_biao=1;
		   Control_data[6]=9;		  
		   }  

		 if(Control_data[7]==1) //��ѯ���2����
		  {
		   B_current_biao=1;
		   Control_data[7]=9;		  
		   }  

    if(Control_data[8]==1)  //��ѯ���3����
		  {
		   C_current_biao=1;
		   Control_data[8]=9;		  
		   }  
    
		 if(Control_data[9]==1) //���ʹ洢����
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
 //���յ�ַ 0x3000;
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
//��ʾ��״̬			 
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
//��ʾ��״̬��ʾ��״̬
			

       //CtrSend_data ���ⷢ�͵��м�ֵ CtrSend_data[15]= A_current  TCtrSend_data[15] �Ƿ�����������ֵ

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
			  //CAT_storage[CAT1_data_dif_num]= storage_1[CAT1_data_dif_num];////CAT_storage[]==storage_1[]�رշ��� ��Ҫ����������
			  Data_dif_times=0;
				 CAT1_Send_Data_dif_biao=0;
				res_storage_1[CAT1_data_dif_num]=0xffff; 
			 }
			 
//		if(res_storage_1[CAT1_data_dif_num]==storage_1[CAT1_data_dif_num])
//		   {
//			  CAT_storage[CAT1_data_dif_num]= storage_1[CAT1_data_dif_num];////CAT_storage[]==storage_1[]�رշ��� ��Ҫ����������
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
//        ucStateNum=0x0D;//������������
//		if(send_reg1==1)
//		{
//		module_data3(addr_data,send_data,data_number);//send_data����������0x1000������,�˺�������������֡Ȼ����
//		}
		
		module_data3(addr_data,send_data,data_number);//send_data����������0x1000������,�˺�������������֡Ȼ����
        
        
//       Ask_for();
        
        
		//addr_data  send_data  data_number  ��ȫ�ֱ��� �������ݵĵ�ַ����������������  �������ݸ���
		if(send_reg1==0) allStateNum=0;		
		
//        if (send_reg1==2) allStateNum=temp_allStateNum+1;//�����굽��һ������
        
		if((send_reg1==2)&&(CAT_back_biao==1)) allStateNum=27;//�����굽��һ������
        else if (send_reg1==2) allStateNum=temp_allStateNum+1;//�����굽��һ������
        
		break;
		
		case 2:		

		
		if( CAT_wait_ds(&cwat_1,60,&alarm_quex_biao1))//����Ԫ�أ��ȴ�ʱ�䣬ȱ���alarm_quex_biao1�����Ͷ�μ��ʱ��60
		  
			  {
			    alarm_quex_biao1++;					
					if(alarm_quex_biao1>ALME_TEMES) alarm_quex_biao1=0;//�رշ���
          
					CtrSend_data[0]=1;//���1ȱ�౨��		  
			    CAT1_Send_Data_Nd(0x3000,0,CtrSend_data); 
					temp_allStateNum=allStateNum;//��¼ִ�����Ҫȥ����һ��ִ�е�ַ
					allStateNum=1;//��ת�����͵�ַ
               				  
			  }
		  else  allStateNum=3;
			  
		break;
		
		case 3:		

		  	if( CAT_wait_ds(&cwat_2,50,&alarm_quex_biao2))
	  
			  {
			   
					alarm_quex_biao2++;
					if(alarm_quex_biao2>ALME_TEMES) alarm_quex_biao2=0;
					
					CtrSend_data[1]=1;//���2ȱ�౨��		
			   CAT1_Send_Data_Nd(0x3000,1,CtrSend_data);
					   
			   temp_allStateNum=allStateNum;//��¼ִ�����Ҫȥ����һ��ִ�е�ַ
          allStateNum=1;//��ת�����͵�ַ
               				  
			  }
		  else  allStateNum=4;
			  
		break;
		
		
		
			  
		case 4:		

		  if( CAT_wait_ds(&cwat_3,50,&alarm_quex_biao3))	  
				  {
				   
						alarm_quex_biao3++;
					if(alarm_quex_biao3>ALME_TEMES) alarm_quex_biao3=0;
						
						CtrSend_data[2]=1;//���3ȱ�౨��		
				   CAT1_Send_Data_Nd(0x3000,2,CtrSend_data);					   
			     temp_allStateNum=allStateNum;
           allStateNum=1; //��ת�����͵�ַ             				  
			    }
		   else  allStateNum=5;
			  
		break;	  
			  
			  
			  
			case 5:		

		  	 if( CAT_wait_ds(&cwat_4,50,&alarm_well_biao1))	  
			  {
				
					alarm_well_biao1++;
					if(alarm_well_biao1>ALME_TEMES) alarm_well_biao1=0;
					
					
					CtrSend_data[3]=1;//ˮ��1ȱˮ����		
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
					
			  CtrSend_data[4]=1;//ˮ��2ȱˮ����		  
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
					
					
					CtrSend_data[5]=1;//ˮ��3ȱˮ����		
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
					
					
			   CtrSend_data[6]=1;//ȱ��ˮ����		
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
					
					CtrSend_data[7]=1;//�ϵ籨��		
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


						CtrSend_data[8]=1;//�ϵ籨��		
					 CAT1_Send_Data_Nd(0x3000,8,CtrSend_data);						   
					 temp_allStateNum=allStateNum;
					 allStateNum=1;               				  
				   }
			 else  allStateNum=11;
				  
			 break;	  
			  
		
		case 11:	//���ʹ洢����

		if( CAT_wait_ds(&cwat_10,20,&send_str_biao)) 	  
		  {
			 send_str_biao++; 
			 if(send_str_biao>STORE_TEMES)
			  {
			  send_str_biao=0;				
			  } 
					
				CAT1_Send_Data_Str(0x1000 ,0,storage_1,50);//���Ͷ��ٸ�������Ҫ	module_data3()��������	  
				  
			 temp_allStateNum=allStateNum;
			 allStateNum=1;
								  
		  }
		  
		 else  allStateNum=12;			  
		 break;	  	  				 		
		

		case 12:	

         // ˮ��1 3009 0000������   0001 ��ֹͣ  0002����ͣ
         if( CAT_wait_ds(&cwat_11,20,&Crelay_state1))//	ˮ��1״̬  
				  {			  
					 
						Crelay_state1++;
					if( Crelay_state1>STATE_TEMES) Crelay_state1=0;  
						
						
						CtrSend_data[9]=relay_state1;   //ˮ��1״̬		
					 CAT1_Send_Data_Nd(0x3000,9,CtrSend_data);						   
					 temp_allStateNum=allStateNum;
					 allStateNum=1; 
           //Crelay_state1=0;					  
				   }
			 else  allStateNum=13;
				  
			 break;	  
        
		 case 13:	
			 
		   //����1 300a  0000�ǹر�״̬  0001������״̬ 0003 �ǹرս��� 
		 
  	     if( CAT_wait_ds(&cwat_12,20,&Cvalve_state1))//	ˮ��1����״̬  
				  {	

            Cvalve_state1++;
					if( Cvalve_state1>STATE_TEMES) Cvalve_state1=0;  

						
					 CtrSend_data[10]=valve_state1;   //ˮ��1����״̬					  
					 CAT1_Send_Data_Nd(0x3000,10,CtrSend_data);	
					  
					 temp_allStateNum=allStateNum;
					 allStateNum=1; 
           //Cvalve_state1=0;					  
				   }
			 else  allStateNum=14;
				  
			 break;	  
		  
	case 14:	

         // ˮ��2 300b 0000������   0001 ��ֹͣ  0002����ͣ
         if( CAT_wait_ds(&cwat_13,20,&Crelay_state2))//	ˮ��2״̬  
				  {			  
					 
						
						 Crelay_state2++;
					if( Crelay_state2>STATE_TEMES) Crelay_state2=0;  
						
						CtrSend_data[11]=relay_state2;   //ˮ��2״̬		
					 CAT1_Send_Data_Nd(0x3000,11,CtrSend_data);						   
					 temp_allStateNum=allStateNum;
					 allStateNum=1; 
           //Crelay_state2=0;					  
				   }
			 else  allStateNum=15;
				  
			 break;	  
        
		 case 15:	
			 
		   //����2 300c  0000�ǹر�״̬  0001������״̬ 0003 �ǹرս��� 
		 
  	     if( CAT_wait_ds(&cwat_14,10,&Cvalve_state2))//	ˮ��2����״̬  
				  {			  
					 
						
					 Cvalve_state2++;
					if( Cvalve_state2>STATE_TEMES) Cvalve_state2=0;  
						
						CtrSend_data[12]=valve_state2;   //ˮ��2����״̬					  
					 CAT1_Send_Data_Nd(0x3000,12,CtrSend_data);	
					  
					 temp_allStateNum=allStateNum;
					 allStateNum=1; 
           //Cvalve_state2=0;					  
				   }
			 else  allStateNum=16;
				  
			 break;	  
		  
		case 16:	

         // ˮ��3 300d 0000������   0001 ��ֹͣ  0002����ͣ
         if( CAT_wait_ds(&cwat_15,10,&Crelay_state3))//	ˮ��3״̬  
				  {	

            Crelay_state3++;
					if( Crelay_state3>STATE_TEMES) Crelay_state3=0;  

						
					 CtrSend_data[13]=relay_state3;   //ˮ��3״̬		
					 CAT1_Send_Data_Nd(0x3000,13,CtrSend_data);						   
					 temp_allStateNum=allStateNum;
					 allStateNum=1; 
          // Crelay_state3=0;					  
				   }
			 else  allStateNum=17;
				  
			 break;	  

	 case 17:	
			 
		   //����3 300e  0000�ǹر�״̬  0001������״̬ 0003 �ǹرս��� 
		 
  	     if( CAT_wait_ds(&cwat_16,10,&Cvalve_state3))//	ˮ��3����״̬  
				  {			  
					 
						
					Cvalve_state3++;
					if( Cvalve_state3>STATE_TEMES) Cvalve_state3=0;
						
						
						CtrSend_data[14]=valve_state3;   //ˮ��3����״̬					  
					 CAT1_Send_Data_Nd(0x3000,14,CtrSend_data);	
					  
					 temp_allStateNum=allStateNum;
					 allStateNum=1; 
           //Cvalve_state3=0;					  
				   }
			 else  allStateNum=18;
				  
			 break;	  
						 
		 case 18:	
	
 
		 if( CAT_wait_ds(&cwat_17,10,&A_current_biao))//	 A�����
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
	
			
  	     if( CAT_wait_ds(&cwat_18,10,&B_current_biao))//	B�����
				  {			  
					 
						B_current_biao++;
					if( B_current_biao>STATE_TEMES) B_current_biao=0;
						
						CtrSend_data[16]=B_current;   //				  
					 CAT1_Send_Data_Nd(0x3000,16,CtrSend_data);	
					  
					 temp_allStateNum=allStateNum;//�洢��һ�εĽ���
					 allStateNum=1;//ȷ�����뷢�ͽ���
						
				   }
			else  allStateNum=20;
				  
			 break;	  				 		 
					 
					 
//#define ALME_TEMES   2;
//#define STORE_TEMES  2;
//#define STATE_TEMES  2;
//#define TEMP_TEMES   1;



			case 20:	
	
  	     if( CAT_wait_ds(&cwat_19,10,&C_current_biao))//	 C�����
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


		case 21:	//�ź�����//�ź�����//�ź�����//�ź�����//�ź�����//�ź�����//�ź�����//�ź�����//�ź�����//�ź�����
	
		 if( Enquiries_CSG2(3500))  allStateNum=22;//CtrSend_data[18] �ں������������ⷢ������ �ź������ź������ź�����
	
		 break;	  		


		 case 22:	
	
  	     if( CAT_wait_ds(&cwat_21,200,&PresentLevel_biao))// ��ǰˮλ�߶�
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
	
  	     if( CAT_wait_ds(&cwat_22,10,&again_startT_biao1))//   ���A��ͣʱ��
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
	
  	     if( CAT_wait_ds(&cwat_23,10,&again_startT_biao2))//    ���B��ͣʱ��
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

			 

			 
			 
			 
  	     if( CAT_wait_ds(&cwat_24,10,&again_startT_biao3))//     ���C��ͣʱ��
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
	

    if  (CAT1_Send_Data_dif_2(0x1000,0,storage_1,49,32))   //�洢�����б仯���ϴ��������б仯����Ҫ�����ϴ���������  
       {			                                              //��ѯ49�β�����ѯһ�飬ÿ����ѯһ������
			        				 
				Data_dif_times++;
				
        if(Data_dif_times>=STORE_TEMES)//�洢����û���յ�������Ϣֻ������	
					
				{					
					Data_dif_times=0;
					CAT1_Send_Data_dif_biao=0;
				}
				 
			 }
		else allStateNum=27;	
		break; 
	 
		 case 27:	   //�����յ������� ֻ����һ��
			 
		   //	static   u16 CAT_Drass;
       //	static   u16 CAT_Data;
       // u8 CAT_back_biao; 
		 
  	   if( CAT_wait_ds(&cwat_25,2,&CAT_back_biao)) //��������
				  {					   			  
					                       
                      CAT1_Send_Data_ResNd(cat_feedback[0],0,&cat_feedback[1]);	//CAT_Drass��ַ,ȫ�ֱ���	//CAT_Data���ݣ�ȫ�ֱ���	
                     // CAT1_Send_Data_ResNd(CAT_Drass,0,&CAT_Data);	//CAT_Drass��ַ,ȫ�ֱ���	//CAT_Data���ݣ�ȫ�ֱ���				  
					 temp_allStateNum=allStateNum;//�洢��֧����
					 allStateNum=1;//���ص���ʼ����������
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
		
// ˮ��1 3009 0000������   0001 ��ֹͣ  0002����ͣ
// ����1  300a  0000�ǹر�״̬  0001������״̬ 0003 �ǹرս��� 
// ˮ��2 300b 0000������   0001 ��ֹͣ  0002����ͣ
// ����2 300c  0000�ǹر�״̬  0001������״̬ 0003 �ǹرս��� 
// ˮ��3 300d 0000������   0001 ��ֹͣ  0002����ͣ
// ����3 300e  0000�ǹر�״̬  0001������״̬ 0003 �ǹرս��� 

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
//			 if(reset_timer_biao==0) //������0��ʱ����1//ִ��1��
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
												//����10�εò�����ȷӦ��
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
												//�յ�����99����Ƶ�ź�δ��ʼ����
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
													//����30�β�Ӧ��
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
													
										if(ec600_RxBuffer[2]>='0'&&ec600_RxBuffer[2]<='9')//��ȡģ��IMEI��ֵ��ȡ��ֵ����������ַ��͵�
										{
												 
											
											memset(( char *)EC600uinit.pubtopic,0,sizeof(EC600uinit.pubtopic));//�����ڴ�	�������ڴ溯�� 
											memset(( char *)EC600uinit.subtopic,0,sizeof(EC600uinit.subtopic));//�����ڴ�	�������ڴ溯��  
											memset(( char *)EC600uinit.IMEI,0,sizeof(EC600uinit.IMEI));        //�����ڴ�	�������ڴ溯��  	
											
											
											memcpy(( char *)EC600uinit.IMEI,ec600_RxBuffer+2,15);//�ڴ濽��	��ec600_RxBuffer��2����ʼ����15������	EC600uinit.IMEI������											
											sprintf(( char *)EC600uinit.pubtopic,"app/%s",EC600uinit.IMEI);//%sվλ���ţ����ݽṹ��Ӧ����
                      sprintf(( char *)EC600uinit.subtopic,"dev/%s",EC600uinit.IMEI);
													
													
//													Uart1_SendStr("��ȡ���豸IMEI���룺\r\n");													
												   //Uart1_SendDATA((char *)EC600uinit.subtopic,19);
//													Uart1_SendStr((char *)EC600uinit.subtopic);						
//											        Uart1_SendStr("\r\n");	
											ucErrorTimes = 0;
											ucStateNum=0x0D;
													
										}
										else
											{
												//����3�εò�����ȷӦ��
												if (ucErrorTimes++ > 3)
												{
													ucStateNum = 0x00;
												}
											}
									}
									break;	
						
					case 0x0D:
					if (fat_send_cmd("AT+QMTOPEN=0,\"bangmuyun.com\",1883\r\n", 250))//300��ʱ�򿨿�	
					//if (fat_send_cmd("AT+QMTOPEN=0,\"broker.emqx.io\",1883\r\n", 250))//300��ʱ�򿨿�
					//if (fat_send_cmd("AT+QMTOPEN=0,\"39.98.107.130\",1883\r\n", 250))//300��ʱ�򿨿�
						{							
							//�յ�+QMTOPEN: 0,0
							if (fat_cmdres_keyword_matching("+QMTOPEN: 0,0"))
							{
								ucErrorTimes = 0;
								ucStateNum=0x0E;
							}
							else
							{
												//����10�εò�����ȷӦ��
								if (ucErrorTimes++ > 30)
								{
									ucStateNum = 0x00;
								}
							}
						}
					 break;		
						
					 case 0x0E:
				
				    sprintf(buf,"AT+QMTCONN=0,\"%s\",\"device\",\"123456\"\r\n",EC600uinit.IMEI);// \"\" ���Ǳ�ʾ˫���ţ�s%��ʾռλ��,12345678������  debice���û���
			
							if (fat_send_cmd(buf,250))
							{
											//�յ�+QMTCONN: 0,0,0
								if (fat_cmdres_keyword_matching("+QMTCONN: 0,0,0"))
									{
									  ucErrorTimes = 0;
									  ucStateNum=0x0F;
									 }
								else
									{
														//����10�εò�����ȷӦ��
										if (ucErrorTimes++ > 3)
										{
											ucStateNum = 0x00;
										}
									}
							}
							
							break;						
	
					  case 0x0F:
		
							 sprintf(buf,"AT+QMTSUB=0,1,\"%s\",0\r\n",EC600uinit.subtopic);//��������      
						
							if (fat_send_cmd(buf, 250))
							{
											//�յ�+QMTSUB: 0,1,0,0
								if (fat_cmdres_keyword_matching("+QMTSUB: 0,1,0,0"))
								{
									ucErrorTimes = 0;
									//ucStateNum = MD_OPEN_OK;
									ucStateNum = 0x10;
									break;
								}
								else
								{
													//����5�εò�����ȷӦ��
									if (ucErrorTimes++ > 3)
									{
										ucStateNum = 0x00;
									}
								}
							}
							break;
		
						
						//case   MD_OPEN_OK: //��������MD_OPEN_OK��ͬһ�����������	
						case   0x10:
						send_reg1=2;
						ucStateNum = 0x20;
						//Uart1_SendStr("MD_OPEN_OK\r\n");//�������λ�ÿ���ִ��
						return MD_OPEN_OK;//��������MD_OPEN_OK��һ���س�������
						//Uart1_SendStr("MD_OPEN_OK\r\n");//�������λ�ò�ִ�� 
						break;
						
		   }

		   
	   }


/*
*return �������䲻����ִ����
*/


void cat1_timeadd(void) //�ŵ�ʱ���ж���

{
 
 if(cat1_receive_biao==1) cat1_receive_timer++;
 if(cat1_receive_biao_D==1) cat1_receive_timer_D++;

}



void cat1_receive(void)//�ŵ����ڽ��ջص���������
  {
	
       cat1_receive_biao=1;  //���жϾͿ�����ʱ����
       cat1_receive_timer=0; //ֻҪ���жϣ���ʱ������
	  
	     cat1_receive_biao_D=1;  //���жϾͿ�����ʱ����
       cat1_receive_timer_D=0; //ֻҪ���жϣ���ʱ�����㣬���������ʱ��˵�����ݽ������ݾ������
	  
	  ec600_RxBuffer[ ec600u_receive_count++]=aRxBuffer2[0];	  
	  if(ec600u_receive_count>100) ec600u_receive_count=100;
	  
    ec600_RxBuffer_test[ ec600u_receive_count_test++]=aRxBuffer2[0];	  
	  if(ec600u_receive_count_test>100) ec600u_receive_count_test=100;
	
   }


/*
�ŵ�����������
*/



 void cat1_excute(void)

{
	
	char *p = NULL;	
	int cat_receiv_biao=1;
	
	
if(cat1_receive_timer_D>=2)//����һ��ʱ�俪ʼ��������
	{	 
	
	  cat1_receive_biao_D=0;//�رռ�ʱ�ۼ�ʱ��
    cat1_receive_timer_D=0;//ȷ���������ִֻ��1��	
		ucFatUartRecvFinishFlg=1;//���ڽ�����ɱ�־
//		//strcat(test,ec600_RxBuffer_test);
//		//Uart1_SendStr(ec600_RxBuffer);		
//		Uart1_SendStr((char*)ec600_RxBuffer_test);
		cat_receiv_biao=match_recv_topic((char*)ec600_RxBuffer_test, (char *)CATrecvbuf); // "+QMTRECV: 0,0,\""  ��ȡ��Ч���ݵ� CATrecvbuf
		
//		Uart1_SendStr((char*)CATrecvbuf);
//    Uart1_SendStr((char*)ec600_RxBuffer_test);		
//		Rs4851send_int16_Data_1(0x12);		
		if(cat_receiv_biao!=0)
		     {
			  
//				 Rs4851send_int16_Data_1(0x13); 
				 Read_cat();         //�Ѷ�Ӧ��ַ����ת����16���Ʋ�д�뵽��Ӧ����������
				 
//			  Rs4851send_int16_Data_1(0x14); 
				 
			  cat_receiv_biao=0;	 
				 
             }
		Clear_Buffer_D();  
 
	 }

}




/**
 * @description: ��ȡsocket���ջ������ĵ���ֵ
 * @param cmdres��������Ӧ����
 * @param recvbuf��socket���ջ���
 * @return ����Topic�ĳ���
 */
/*+QMTRECV: 0,0,"dev/869537052769784","5AA51001003283"
*
*  ,��asc����0x2C "��ASC��0x22
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
    p = p + 15;
    //q = p;
    while (*p != 0x2C)//���������Ч��ֵ���ֽ�����һֱ�Ӽӵ�������ֹͣ
     {
        p++;
      }
	 
	 q = p+2;  //����ʵ��ȷ����
	 
    while (*p != 0x00)
     {
        p++;
      }

	  
    length = p - q-3 ;//����ʵ�����ȷ����
	if(length>=90) length=0; 
    memset(recvbuf, 0, 100);//�ڴ�����
    memcpy(recvbuf, q, length);//��ȡ��Ч����
    return length;
}






// C prototype : void StrToHex(unsigned char *pbDest, unsigned char *pbSrc, int nLen)
// parameter(s): [OUT] pbDest - ���������
// [IN] pbSrc - �ַ���
// [IN] nLen - 16���������ֽ���(�ַ����ĳ���/2)
// return value:
// remarks : ���ַ���ת��Ϊ16������

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




//����յ��������ݱ�־��ִ������ĺ���


 void Read_cat(void)//��LCD�ж�ȡ���ݶ�Ӧ��ַд����Ӧ�ı����У�Ҫ����ʾ�Ķ�Ӧ����
  {
//	static   u16 CAT_Drass;
//	static   u16 CAT_Data;
//           u8 CAT_back_biao;

	 static   u8 CAT_receive[30];//����Ǹ��ַ�ת���ֺ����õ�
	 int nLen; 
	 static  u16 temp[3]; 	
	 u8 i; 
	  //storage_1[i] �ڱ�ĵط������������洢��������	  
	 
 nLen = strlen((char*)CATrecvbuf)/sizeof(char);//�����ַ�ת����
	  
 StrToHex(CAT_receive, CATrecvbuf,  nLen/2);//���ַ�ת��������

	    	temp[0]=CAT_receive[0];
			temp[0]=(temp[0]<<8)+CAT_receive[1] ;  //temp1��ͷ
        
		CAT_head=temp[0];
      
    		temp[1]=CAT_receive[2];
			temp[1]=(temp[1]<<8)+CAT_receive[3] ;  //temp2���յ�ʱ��ַ	  
			  
		CAT_Drass=temp[1];//CAT_Drassȫ�ֱ������յ��ĵ�ַ
				
			temp[2]=CAT_receive[4];
			temp[2]=(temp[2]<<8)+CAT_receive[5] ;   //temp2���յĵ�ַ�������	
	            
		CAT_Data=temp[2];//CAT_Dataȫ�ֱ���	���յ�������
        
	
//	if(CAT_head==0xA55A)   CAT_back_biao=1; //CAT_back_biaoȫ�ֱ���ֻ�з��ϱ�ͷ�Ĳ��ܷ���	������������ʶ			
//  
//	/*
//	����������ϰ汾������
//	 �ϰ汾���ڲ��ȶ�������
//	 ����������ˡ�
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
    
    
 
	
	
   for(i=0;i<100;i++)//ע�� ��Ҫ��������Ķ��巶Χ
		  {
		  
			if(CAT_Drass==(0x1000+i)&&(CAT_head==0xA55A)) //�洢����1000
			  {               				
				  if(storage_1[i]!=CAT_Data) //  ��һ����洢����������
					{
					  CAT_storage[i]=CAT_Data;//   CAT_storage[i]�Ǵ洢�趨ֵ�����飬��Ƭ������ֵ�任�Ժ�
						                      //   Ҫ������������ֵ���бȽϣ���һ�����Ϸ����飬һ��С������Ը�����ֵ
					  storage_1[i]=CAT_Data;  //	 storage_1[i] �趨ֵ����  
					  write_eepall_1(0) ; 
					}
			   }
			
				 
			if(CAT_Drass==(0x1000+i)&&(CAT_head==0xB55B)) //�������ݴ洢��������1000
			  {               				
				  if(res_storage_1[i]!=CAT_Data) 
					{ 
					  res_storage_1[i]=CAT_Data;						 
					}
			   }	 
				 
		  }
		  

		  
    for(i=0;i<30;i++)//�յ��Ŀ�����������2000
		  {
		 		  
			if(CAT_Drass==(0x2000+i)&&(CAT_head==0xA55A)) 
			  {  
               				
				  if(Control_data[i]!=CAT_Data) 
					  { 
					   Control_data[i]=CAT_Data;						 
					  }
					
			   }
			  
		  }		  
	  

 for(i=0;i<30;i++)//�յ��ķ���3000����
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
			 CATrecvbuf[i]=0;//�������
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
	 HAL_IWDG_Refresh(&hiwdg);//ι��
}

void Clear_Buffer(void)//��ջ���
{
    uint8_t i;
    
    //for(i=0;i<ec600u_receive_count;i++)//ec600u_receive_count�Զ����ն��������ӵ���
	
 for(i=0;i<100;i++)//ec600u_receive_count�Զ����ն��������ӵ���
   { 
	   ec600_RxBuffer[i]=0;//����	
	   
   }
	
    ec600u_receive_count=0;
    IWDG_Feed();//ι��
}


void Clear_Buffer_D(void)//��ջ���
{
    uint8_t i;
    
    //for(i=0;i<ec600u_receive_count;i++)//ec600u_receive_count�Զ����ն��������ӵ���
	
 for(i=0;i<100;i++)//ec600u_receive_count�Զ����ն��������ӵ���
   { 
	 
	   ec600_RxBuffer_test[i]=0;//����
   }
	ec600u_receive_count_test=0;
     
}


//void Clear_Buf_Buf(uint8_t *p)//��ջ���

//{

// memset(p,0,sizeof(p));

//}

void Clear_Buf(void)//��ջ���
{
    uint8_t i;
    //Uart1_SendStr((char*)RxBuffer);
    for(i=0;i<100;i++)//ec600u_receive_count�Զ����ն��������ӵ���
    buf[i]=0;//����
    
   // IWDG_Feed();//ι��
}

//ģ�鷢�����ݣ�һ���Ͷ༸���������б仯�ͷ��ͣ����仯������//��Ҫ�Ƿ����յ�����Ϣ 
//�����Ѿ����úõ���ֵ
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

  if(send_ok_biao==1) //������ֵһ����־��Ҳ����˵��Ƭ��û��������ֵ		
		   { 
			  i++;
			  send_ok_biao=0;
			  if(i>=number) i=0;
		   }  
			
	if(data[addr+i]!= CAT_storage[addr+i])	//��ѯ��Ƭ����������ֵû��
			{		
				CAT1_Send_Data_dif_biao=1;//������ֵ��һ����־��Ҳ����˵��Ƭ����������ֵ
				Data_dif_times=0;//ȫ�ֱ��������㷢��ʱ�䣬�о�û��Ҳ���ԡ�
				CAT_storage[addr+i]=data[addr+i];
				Senddif_timer=time-3;	//	������		
			}
	
	if((send_reg1==2)&&(Senddif_timer>=time))//��ʾmodule_data2�������ݷ������
	   {  
      if(CAT1_Send_Data_dif_biao==1)	//������ֵ��һ����־��Ҳ����˵��Ƭ����������ֵ		  
			 {			  
			   Cat1_data_dif_temp= data[addr+i];
				 CAT1_data_dif_num=addr+i;             //��iת�Ƴ�ȥ
				 
			   addr_data=base+addr+i;
			   send_data=&data[addr+i];
				 
			   data_number=1;//����module_data3��������һ������			   
			   send_reg1=1;//����һ�η���
			   ucStateNum=0x20;  
			   excue_biao=0;
			   Senddif_timer=0;
				CAT1_data_res_biao=0xA55A;
				
    		 temp_allStateNum=allStateNum;
				 allStateNum=1;		
				return 1;
			  }

		   else //������ֵһ����־��Ҳ����˵��Ƭ��û��������ֵ		
			  {
				  send_ok_biao=1;
			  }

		 }
   return 0;//���������󣬻���ʹ��ȫ�ֱ���		
 }

//ģ�鷢�����ݣ�һ���Ͷ༸���������б仯�ͷ��ͣ����仯������//������ʱ������Ϣ 
 //�����Ѿ����úõı����� 
void CAT1_Send_Data_Str(u16 base ,u16 addr,u16 *data,u8 number)
  {
	  static u8 i=0;

	  if(send_reg1==2)//��ʾmodule_data2�������ݷ������
	   {  
			   addr_data=base+addr;
			   send_data=&data[addr];		   
			   data_number=number;//����module_data3�������ͼ�������
			   send_reg1=1;//����һ�η���
	       ucStateNum=0x20;  
			   excue_biao=0;
         CAT1_data_res_biao=0xB55B;

		 }
   
 }

 
 //ģ�鷢��1�����ݣ�
 //���͵�������Ҫʱ����ʱ���ã�����ԭ�����úõ�
 int CAT1_Send_Data_Nd(u16 base ,u16 addr,u16* data)   
  {   	  
	  if(send_reg1==2)//��ʾmodule_data2�������ݷ������
	   { 		   
			   addr_data=base+addr;	 //		addr_dataȫ�ֱ������������ݺ���   
			   send_data=&data[addr];//   send_dataȫ�ֱ������������ݺ���
			   data_number=1;//           ȫ�ֱ������������ݺ���
			   send_reg1=1;//����һ�η��Ͳ��������ķ����ٽ���
			   CAT1_data_res_biao=0xA55A;//
			   ucStateNum=0x20;  //
			   excue_biao=0;	// �ں��� module_data3��������ִ�����ݵ�ַ����
		      
		 } 
 }

 
 //ģ�鷢�����ݣ�����һ���������� 
 //���͵�������Ҫʱ����ʱ���ã�����ԭ�����úõ�
 // CAT1_Send_Data_ResNd(0x3000,19,CtrSend_data);//CtrSend_data���Ǹ���������
 int CAT1_Send_Data_ResNd (u16 base ,u16 addr,u16* data)  
  {
   
	  
	  if(send_reg1==2)//��ʾmodule_data2�������ݷ������
	   { 
		   
			   addr_data=base+addr;		//		ȫ�ֱ������������ݺ��� 	   
			   send_data=&data[addr]; //    ȫ�ֱ������������ݺ�������һ������Ԫ�صĵ�ַ��ֵ��
			   data_number=1;//             ȫ�ֱ������������ݺ���
			   send_reg1=1;//����һ�η��Ͳ��������ķ����ٽ���			  
			   CAT1_data_res_biao=0xB55B;//������ͷѡ���־
			   ucStateNum=0x20;       //ȫ�ֱ���������module_data3�ķ��Ͳ���
			   excue_biao=0;	        //ȫ�ֱ���  ����module_data3��ִ�з���������֡
		 
		 }
 
 } 

//module_data3(addr_data,send_data,data_number);
int module_data3(uint16_t Variable_addr,uint16_t*Dev_Data,u8 number)//һ����෢��20�����α�����У�顣
 { 

//int ret = 0;	
     static char Addr_Buf[300];//����һ��������������һ����������Ҫ
     static char Dev_Buf[4];
	   static uint8_t data_len;
     uint8_t i;	 
//	 static u8 excue_biao=0; //ȫ�ֱ���
	 
	 if(excue_biao==0)
     {
			 
			 memset(Addr_Buf,0,sizeof(Addr_Buf));
			 memset(Dev_Buf,0,sizeof(Dev_Buf));
			 
			 if( CAT1_data_res_biao==0xA55A)  sprintf(Addr_Buf,"A55A%x",Variable_addr); //Variable_addr ��ַ����ַΪʲôû��ת����ASC�룿
			                                                                            //��˫���ű�����Ǿ���ת�������ַ����ˣ����Ե�ַ ����sprintf�����Ժ�ͱ���ַ�����
			 
			 if( CAT1_data_res_biao==0xB55B)  sprintf(Addr_Buf,"B55B%x",Variable_addr); //Variable_addr ��ַ
				 
				 
//				 for (i=0;i<number;i++)
//							{
//						
//							myitoa2(Dev_Data[i], Dev_Buf);	//Dev_Data[](����)ת����ASC�뵽dev_Buf	����Ϊʲô����sprintf��������sprintf�����Ƿ����һ���ģ�
//								                              //��һ�ģ���ʱ�Ҳ�֪���и�ʽ������ţ���������Ķ���1   2    3  ��ϣ��������� 0001  0002������Ū��һ�� ת������
//                                              //��ʵ����		sprintf(buf4,"%04x\n",Dev_Data[i]);						
//							strcat(Addr_Buf,Dev_Buf);      //strcat(str,ptr)�ǽ��ַ���ptr�������ӵ��ַ���str��
//						
//							}
			 
/*
����ʵ�ֵĹ�����һ����
*/
			 
			 for (i=0;i<number;i++)
							{						
							sprintf(Dev_Buf,"%04x",Dev_Data[i]);
			                strcat(Addr_Buf,Dev_Buf);						
							}
			
					data_len = strlen(Addr_Buf)/sizeof(char);//�������ݳ���
					
					if(number>1) 
						{
						
						 crc=crc16(Addr_Buf,data_len);  
						 myitoa2(crc, Dev_Buf);	        //ת��������ת����ASC�뵽dev_Buf		
						 strcat(Addr_Buf,Dev_Buf);      //strcat(str,ptr)�ǽ��ַ���ptr�������ӵ��ַ���str��	
						
						}
							

						data_len = strlen(Addr_Buf)/sizeof(char);
						sprintf(buf,"AT+QMTPUBEX=0,0,0,0,\"%s\",%d\r\n",EC600uinit.pubtopic,data_len);
						excue_biao=1;

					}
	 
    switch (ucStateNum)
    { 
		case 0x20:
			//if (fat_send_cmd(buf, 80))//30��15���룩��С��
			if (fat_send_Ncmd(buf, 300))//30��15���룩��С��	
			
			
				{
					//Uart1_SendStr("MD_DATA_OK\r\n");

					//�յ�>+QMTPUBEX: 0,0,0
					if (fat_cmdres_keyword_matching(">"))
					//if (fat_cmdres_keyword_matching("+QMTPUBEX: 0,0,0"))	
					{
						ucErrorTimes = 0;
						ucStateNum = 0x21;
						
						
					}
					else
					{
										//����3�εò�����ȷӦ��
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
				//if (fat_send_cmd_2(Addr_Buf,data_len,80))//���ֽڳ��ȷ��ͺ���
				if (fat_send_Ncmd_2(Addr_Buf,data_len,80))//���ֽڳ��ȷ��ͺ���	
					{
									//�յ�OK
						if (fat_cmdres_keyword_matching("+QMTPUBEX: 0,0,0"))	
								//if (fat_cmdres_keyword_matching("OK"))
								{
									ucErrorTimes = 0;
									ucStateNum=0x22;
									feedback_biao=1;//ʹ�ܷ������Ͷ��ж���־���������˿��Կ����´ζ�ȡ����
									//Uart1_SendDATA("xxxxxx",6);
								}
						else
						{
											//����3�εò�����ȷӦ��
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
      ucStateNum=0x0D;//������������		
	    break;

		
		  case   23:
	    send_reg1=0;
      ucStateNum= 0x0F;//������������		
	    break;
		
		
		
    }
    
		

    }
		
		

 
 
 int Enquiries_CSG(u8 time)//time����ȡһ���ź�����
    {
   
   static   int lpCsqVal = 0;   
   static u8 CSQ_timer=0;
   static u8 CSQ_timer_biao=0;  
	
//��׼��ʱ�ṹ//��׼��ʱ�ṹ//��׼��ʱ�ṹ//��׼��ʱ�ṹ//��׼��ʱ�ṹ
	  
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
		  CSQ_timer=0;//���¿�ʼ��ʱ
		  
	  }	

 if(send_reg1==3)
 {
		if (fat_send_cmd("AT+CSQ\r\n", 50))
							{
								//Uart1_SendStr("101010\r\n");			
								if (fat_cmdres_keyword_matching("OK"))
								{
												//�յ�����99����Ƶ�ź�δ��ʼ����
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
													  //return lpCsqVal;����������������
														
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
     return lpCsqVal;//���������󣬻���ʹ��ȫ�ֱ���
		
	}
  
 
int Enquiries_CSG2(u16 time)//time����ȡһ���ź�����
    {
   
   static   int lpCsqVal = 0;   
   static u16 CSQ_timer=2000;
   static u8 CSQ_timer_biao=0;  
   static u8 ture_biao=1;
   
//��׼��ʱ�ṹ//��׼��ʱ�ṹ//��׼��ʱ�ṹ//��׼��ʱ�ṹ//��׼��ʱ�ṹ
	
	  
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
		  CSQ_timer=0;//���¿�ʼ��ʱ
		  
	  }	
 	
	  
 if(send_reg1==3)
 {
		if (fat_send_cmd("AT+CSQ\r\n", 50))
							{
								//Uart1_SendStr("101010\r\n");			
								if (fat_cmdres_keyword_matching("OK"))
								{
												//�յ�����99����Ƶ�ź�δ��ʼ����
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
													   send_reg1=2;		//CAT1_Send_Data_Nd   ��Ҫ                                               
													   excue_biao=0;													
													   ucErrorTimes=0;//2022_5_7
													   
													 CtrSend_data[18] =lpCsqVal;//	  
					                                 CAT1_Send_Data_ResNd(0x3000,18,CtrSend_data);					  
					                                 temp_allStateNum=allStateNum;
					                                 allStateNum=1;					
													
													  //return lpCsqVal;����������������
														
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
     return 1;//���������󣬻���ʹ��ȫ�ֱ���		��ִ������Ƴ�������
	}
 
 /**
 * @description:
 * @param str��Ҫ�������ַ���
 * @param minval��Ҫƥ���ź�����������Сֵ
 * @param minval��Ҫƥ���ź������������ֵ
 * @return 0:�ź�������������������״̬, 1:�ź�����������������״̬
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
    return lpCsqVal; 
	
	/* �ж��ź������Ƿ������õ������� */

//    if (lpCsqVal >= minval && lpCsqVal <= maxval)
//    {
//        return 1;
//    }
//    return 0;
		
}
 


void myitoa2(int value, char *string)  //16��������ת����ASC�뵽dev_Buf	

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




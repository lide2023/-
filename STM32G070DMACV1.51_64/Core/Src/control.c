
#include "control.h"
#include "dacaiLCD.h"
#include "ec600u.h"
 u16  Win_Word_receive=0;
 u16  win_control_Word=0;
 
  u8 open1=0,stop1=0;
  u8 cat_open1=0,cat_stop1=0;
  
  u8 open2=0,stop2=0;
  u8 cat_open2=0,cat_stop2=0;
   
  u8 open3=0,stop3=0;
  u8 cat_open3=0,cat_stop3=0;
	
	
	
	u8 valve_open1=0,valve_stop1=0;
  u8 cat_valve_open1=0,cat_valve_stop1=0;
  
  u8 valve_open2=0,valve_stop2=0;
  u8 cat_valve_open2=0,cat_valve_stop2=0;
   
  u8 valve_open3=0,valve_stop3=0;
  u8 cat_valve_open3=0,cat_valve_stop3=0;
	
   
  u16 auto_button_1,auto_button_2,auto_button_3;//�Զ�����ѡ��
  u16 biaoding_shuiwei;
	u16 shuiwei_xinhao;
	u16 lock_biao;
 	u8 Kai_moto1=0,Guan_moto1=0;
	u8 Kai_moto2=0,Guan_moto2=0;
	u8 Kai_moto3=0,Guan_moto3=0;
	u8 Kai_moto4=0,Guan_moto4=0;
	u8 Kai_moto5=0,Guan_moto5=0;
	u8 Kai_moto6=0,Guan_moto6=0;
	u8 A_LEVEL=1,B_LEVEL=1,C_LEVEL=1;
	
	u16 ALoadCurrent,BLoadCurrent,CLoadCurrent;//���ص���

	u16 A_current, B_current, C_current;//��ǰ����
	u16 PresentLevel;//��ǰˮλ
	u16 PresentLevel_temp;//����������
	u8 A_current_biao, B_current_biao, C_current_biao;//�ϴ���־��ǰ����
	u8 PresentLevel_biao; //��ǰˮλ�ϴ���־
	
	
    C0N_WAT wat_1,wat_2,wat_3,wat_4,wat_5,wat_6,wat_7,wat_8;
	
   CatWAT cwat_1,cwat_2,cwat_3,cwat_4,cwat_5,cwat_6,cwat_7,cwat_8,cwat_9,cwat_10,cwat_11,cwat_12,cwat_13,cwat_14
	      ,cwat_15,cwat_16,cwat_17,cwat_18,cwat_19,cwat_20,cwat_21,cwat_22,cwat_23,cwat_24,cwat_25,cwat_26; 
  u8 CourseNum=0;
	u8 CourseNum2=0;
	u8 CourseNum3=0;
  u8 CourseSeNum=1;
	
	u8 hand_CourseNum=0;
	u8 hand_CourseNum2=0;
	u8 hand_CourseNum3=0;
	
	
	
	u8 valve_hand_CourseNum=0;
	u8 valve_hand_CourseNum2=0;
	u8 valve_hand_CourseNum3=0;
	
	
	
	uint16_t VolDta1,VolDta2,VolDta3,VolDta4,VolDta0;//�ο���ѹֵ��ADC1_Ch4������ѹֵ����λV

    u16 start_N,absence_DelayT,up_waterT,NOwater_T;
	
    u8 open_Num1=0;
		u8 open_Num2=0;
		u8 open_Num3=0;
    u16 set_leve[16];
	u16 set_time[16];
	u16 again_startT1,again_startT1_test;
	u16 again_startT2,again_startT2_test;
	u16 again_startT3,again_startT3_test;
	u8 again_startT_biao1;
	u8 again_startT_biao2;
	u8 again_startT_biao3;
	
 u8  alarm_noelec_biao=0;
 u8  alarm_upelec_biao=1;	
	

 u8  alarm_quex_biao1=0;
 u8  alarm_quex_biao2=0;
 u8  alarm_quex_biao3=0;
 u8  alarm_well_biao1=0;
 u8  alarm_well_biao2=0;
 u8  alarm_well_biao3=0;
 u8  alarm_pull_biao=0;

 u8 send_str_biao=1;//�������ʹ洢���ݱ�־

u8 CAT1_Send_Data_dif_biao=0;



 u8 send_mem_nember=0; 

 u8 relay_state1=1;
 u8 relay_state2=1;
 u8 relay_state3=1;
 
 u8 Brelay_state1=5;
 u8 Brelay_state2=100;
 u8 Brelay_state3=100;
 
 u8 Crelay_state1=0;//4G���Ϳ���
 u8 Crelay_state2=0;//4G���Ϳ���
 u8 Crelay_state3=0;//4G���Ϳ���
 
 
 u8 valve_state1=0;
 u8 valve_state2=0;
 u8 valve_state3=0;
 
 u8 Bvalve_state1=5;
 u8 Bvalve_state2=100;
 u8 Bvalve_state3=100;
 
 
 u8 Cvalve_state1=0;
 u8 Cvalve_state2=0;
 u8 Cvalve_state3=0;
 
 
u16 	kongzhi_fangshi; 
u16 	protection_switch_1=0; 
u16 	protection_switch_2=0; 
u16 	protection_switch_3=0; 
u16 	valve_travel_time; 
 
	
//typedef struct
//{
//    u16   TimeOutCount; 
//    u8    TimetStartFlg; 
//}C0N_WAT,*TROL_WAT;	


//storage_1[81]=0;


//storage_1[40]=9;


//u16 start_N,absence_DelayT,have_waterT,Nwater_T;

// extern     u16 set_leve[16];
// extern 	u16 set_time[16]; 

void store_bridge(u8 type)

{
 
if(type==0)
 {
	  u8 i,j;
	  storage_1[0]=ALoadCurrent;
	  storage_1[1]=BLoadCurrent;
	  storage_1[2]=CLoadCurrent;
	  storage_1[3]=start_N;
	  storage_1[4]=absence_DelayT;
	  storage_1[5]=up_waterT;
	  storage_1[6]=NOwater_T;	 
	 
	  if(set_leve[4]<=0) set_leve[4]=0;	 
	  if(set_leve[4]>=set_leve[3]) set_leve[4]=set_leve[3]-1;
    if(set_leve[3]<=set_leve[4]) set_leve[3]=set_leve[4]+1;
	  if(set_leve[2]<=set_leve[3]) set_leve[2]=set_leve[3]+1;
    if(set_leve[1]<=set_leve[2]) set_leve[1]=set_leve[2]+1;
	  if(set_leve[0]<=set_leve[1]) set_leve[0]=set_leve[1]+1;
	 
	  if(set_leve[4+5]<=0) set_leve[4+5]=0;	 
	  if(set_leve[4+5]>=set_leve[3+5]) set_leve[4+5]=set_leve[3+5]-1;
    if(set_leve[3+5]<=set_leve[4+5]) set_leve[3+5]=set_leve[4+5]+1;
	  if(set_leve[2+5]<=set_leve[3+5]) set_leve[2+5]=set_leve[3+5]+1;
    if(set_leve[1+5]<=set_leve[2+5]) set_leve[1+5]=set_leve[2+5]+1;
	  if(set_leve[0+5]<=set_leve[1+5]) set_leve[0+5]=set_leve[1+5]+1;
	 
	  if(set_leve[4+10]<=0) set_leve[5+10]=0;	 
	  if(set_leve[4+10]>=set_leve[3+10]) set_leve[4+10]=set_leve[3+10]-1;
    if(set_leve[3+10]<=set_leve[4+10]) set_leve[3+10]=set_leve[4+10]+1;
	  if(set_leve[2+10]<=set_leve[3+10]) set_leve[2+10]=set_leve[3+10]+1;
    if(set_leve[1+10]<=set_leve[2+10]) set_leve[1+10]=set_leve[2+10]+1;
	  if(set_leve[0+10]<=set_leve[1+10]) set_leve[0+10]=set_leve[1+10]+1;


	 for(i=0;i<16;i++)	 
		 {
		  
		  storage_1[7+i]= set_leve[i];
		 
		 }
	  for(j=0;j<16;j++)	 
		 {
			 
		     if(set_time[j]>1000) set_time[j]=1000;
			 storage_1[23+j]= set_time[j];//���һ����38
		 
		 }
	
    storage_1[39]=auto_button_1;
	  storage_1[40]=auto_button_2;
    storage_1[41]=auto_button_3;
		storage_1[42]=biaoding_shuiwei;
    storage_1[43]=shuiwei_xinhao;
    storage_1[44]=lock_biao;
		
		storage_1[45]=kongzhi_fangshi;
		storage_1[46]=protection_switch_1;
		storage_1[47]=protection_switch_2;
		storage_1[48]=protection_switch_3;
		storage_1[49]=valve_travel_time;
		 
		 
 }
 
 
 
/*
    storage_1[39]=auto_button_1;      ˮ��1�Զ�����
	  storage_1[40]=auto_button_2;      ˮ��2�Զ�����
    storage_1[41]=auto_button_3;	    ˮ��3�Զ�����             A55A10290001
	  storage_1[42]=biaoding_shuiwei;	  �궨ˮλ                  A55A102a0001
    storage_1[43]=shuiwei_xinhao;	    ˮλ�ź���Դ							A55A102b0001 ����   A55A102b0000����  
	  storage_1[44]=lock_biao;	        ��������							    A55A102c0001  

V2.0	  storage_1[45]=kongzhi_fangshi;	  ���Ʒ�ʽ�ֶ��Զ�ת��			A55A102d0000 �ֶ�
	                                                                  A55A102d0001 �Զ�
V2.0	  storage_1[46]=protection_switch_1;   ��������1                  A55A102e0001 ��
                                                                     A55A102e0000 ��
V2.0	  storage_1[47]=protection_switch_2;   ��������2                  A55A102f0001 ��
                                                                    A55A102f0000 ��
V2.0    storage_1[48]=protection_switch_3;   ��������3                  A55A10300001 ��
                                                                    A55A10300000 ��
V2.0    storage_1[49]=valve_travel_time;   �����г�ʱ��             A55A1031xxxx 	

 */
  

if(type==1)
 {
	  u8 i,j;
		 
	  ALoadCurrent=storage_1[0];
	  BLoadCurrent=storage_1[1];
	  CLoadCurrent=storage_1[2];

	  start_N       =storage_1[3];
	  absence_DelayT=storage_1[4];
	  up_waterT     =storage_1[5];	 
	  NOwater_T     =storage_1[6]; 
	 
		 
	for(i=0;i<16;i++)	 
		 {
		   set_leve[i]=storage_1[7+i];		 
		 }
	for(j=0;j<16;j++)	 
		 {
		  set_time[j]=storage_1[23+j];//���һ����38		 
		 } 	 
 
	auto_button_1 =storage_1[39];
	auto_button_2 =storage_1[40];
  auto_button_3 =storage_1[41];
	biaoding_shuiwei=	storage_1[42]; 
	shuiwei_xinhao=storage_1[43]; 	 
	lock_biao=storage_1[44];

  kongzhi_fangshi=storage_1[45];
  protection_switch_1=storage_1[46];
  protection_switch_2=storage_1[47];
  protection_switch_3=storage_1[48];
  valve_travel_time=storage_1[49];
		 
  }

}


	
void KongZhi1(void)	
	
{	

 if((stop1==1)&&(open1==0)) 
	 {	   
       stop1=0;
       if((CourseNum==6)||(CourseNum==7)||(CourseNum==15)||(CourseNum==8)||(CourseNum==21))  CourseNum=20; 			 
	   else CourseNum=12; 		 
	 }
	 
  if((stop1==0)&&(open1==1))   
     {	  
	  open1=0;
	  CourseNum=1;		
	 }
	 
	cat_open1=Control_data[0];
	cat_stop1=Control_data[1];	 

		 
	 if((cat_stop1==1)&&(cat_open1==0)) 
		 {	   
		   cat_stop1=0;
		  if((CourseNum==6)||(CourseNum==7)||(CourseNum==15)||(CourseNum==8)||(CourseNum==21))  CourseNum=20; 			 
	   else CourseNum=12; 		 
		 }


	 if((cat_stop1==0)&&(cat_open1==1))   
		 {	  
		   cat_open1=0;     
		   CourseNum=1; 		 
		 }

	Control_data[0]=cat_open1;//�ֶ�״̬������APP
	Control_data[1]=cat_stop1;//�ֶ�״̬������APP

}



void KongZhi2(void)	
	
{	

 if((stop2==1)&&(open2==0)) 
	 {	   
       stop2=0;
       if((CourseNum2==6)||(CourseNum2==7)||(CourseNum2==15)||(CourseNum2==8)||(CourseNum2==16))  CourseNum2=20; 			 
	   else CourseNum2=12; 		 
	 }
	 
  if((stop2==0)&&(open2==1))   
     {	  
	  open2=0;
	  CourseNum2=1;		
	 }
	 
	cat_open2=Control_data[2];
	cat_stop2=Control_data[3];	 

		 
	 if((cat_stop2==1)&&(cat_open2==0)) 
		 {	   
		   cat_stop2=0;
		   if((CourseNum2==6)||(CourseNum2==7)||(CourseNum2==15)||(CourseNum2==8)||(CourseNum2==16))  CourseNum2=20; 			 
	   else CourseNum2=12; 			 
		 }


	 if((cat_stop2==0)&&(cat_open2==1))   
		 {	  
		   cat_open2=0;     
		   CourseNum2=1; 		 
		 }

	Control_data[2]=cat_open2;//�ֶ�״̬������APP
	Control_data[3]=cat_stop2;//�ֶ�״̬������APP

}


void KongZhi3(void)	
	
{	

 if((stop3==1)&&(open3==0)) 
	 {	   
       stop3=0;
       if((CourseNum3==6)||(CourseNum3==7)||(CourseNum3==15)||(CourseNum3==8)||(CourseNum3==16))  CourseNum3=20; 			 
	     else CourseNum3=12; 		 
	 }
	 
  if((stop3==0)&&(open3==1))   
     {	  
	  open3=0;
	  CourseNum3=1;		
	 }
	 
	cat_open3=Control_data[4];
	cat_stop3=Control_data[5];	 

		 
	 if((cat_stop3==1)&&(cat_open3==0)) 
		 {	   
		   cat_stop3=0;
		   if((CourseNum3==6)||(CourseNum3==7)||(CourseNum3==15)||(CourseNum3==8)||(CourseNum3==16))  CourseNum3=20; 			 
	     else CourseNum3=12; 	 		 
		 }


	 if((cat_stop3==0)&&(cat_open3==1))   
		 {	  
		   cat_open3=0;     
		   CourseNum3=1; 		 
		 }

	Control_data[4]=cat_open3;//�ֶ�״̬������APP
	Control_data[5]=cat_stop3;//�ֶ�״̬������APP

}



void Hand_KongZhi1(void)	//��1���غ�cat.1����ָ��
	
{	

 if((valve_stop1==1)&&(valve_open1==0)) 
	  {	   
       valve_stop1=0;
       hand_CourseNum=4;//����رշ����߳�	 
	  }
	 
  if((valve_stop1==0)&&(valve_open1==1))   
   {	  
	  valve_open1=0;
	  hand_CourseNum=1;	//���뿪�������߳�	
	 }
	 
	cat_valve_open1=Control_data[10];
	cat_valve_stop1=Control_data[11];	 

		 
	 if((cat_valve_stop1==1)&&(cat_valve_open1==0)) 
		 {	   
		   cat_valve_stop1=0;
		   hand_CourseNum=4;//����رշ����߳�	 
		 }


	 if((cat_valve_stop1==0)&&(cat_valve_open1==1))   
		 {	  
		   cat_valve_open1=0;     
		   hand_CourseNum=1;	//���뿪�������߳�		 
		 }

	Control_data[10]=cat_valve_open1;//�ֶ�״̬������APP
	Control_data[11]=cat_valve_open1;//�ֶ�״̬������APP
		 

		 
  if((stop1==1)&&(open1==0)) 
	 {	   
       stop1=0;
       CourseNum=7;
	 }
	 
  if((stop1==0)&&(open1==1))   
     {	  
	    open1=0;
	    CourseNum=1;
	   }
	 
	cat_open1=Control_data[0];
	cat_stop1=Control_data[1];	 

		 
	 if((cat_stop1==1)&&(cat_open1==0)) 
		 {	   
		   cat_stop1=0;
       CourseNum=7;
			 
		 }

	 if((cat_stop1==0)&&(cat_open1==1))   
		 {	  
		   cat_open1=0;     
		   CourseNum=1; 
		 }

	Control_data[0]=cat_open1;//�ֶ�״̬������APP
	Control_data[1]=cat_stop1;//�ֶ�״̬������APP	 
 
		 
}



void Hand_KongZhi2(void)	//����2���غ�cat.1����ָ��
	
{	

 if((valve_stop2==1)&&(valve_open2==0)) 
	 {	   
       valve_stop2=0;
       hand_CourseNum2=4;//����رշ����߳�	 		 
	 }
	 
  if((valve_stop2==0)&&(valve_open2==1))   
     {	  
	  valve_open2=0;
	   hand_CourseNum2=1;	//���뿪�������߳�		
	 }
	 
	cat_valve_open2=Control_data[12];
	cat_valve_stop2=Control_data[13];	 

		 
	 if((cat_valve_stop2==1)&&(cat_valve_open2==0)) 
		 {	   
		   cat_valve_stop2=0;
		   hand_CourseNum2=4;//����رշ����߳�	 		 
		 }


	 if((cat_valve_stop2==0)&&(cat_valve_open2==1))   
		 {	  
		   cat_valve_open2=0;     
		   hand_CourseNum2=1;	//���뿪�������߳�  202212��24���޸�			 
		 }

	Control_data[12]=cat_valve_open2;//�ֶ�״̬������APP
	Control_data[13]=cat_valve_stop2;//�ֶ�״̬������APP

		 
	if((stop2==1)&&(open2==0)) 
	 {	   
       stop2=0;
       CourseNum2=7;
	 }
	 
  if((stop2==0)&&(open2==1))   
     {	  
	    open2=0;
	    CourseNum2=1;
	   }
	 
	cat_open2=Control_data[2];
	cat_stop2=Control_data[3];	 

		 
	 if((cat_stop2==1)&&(cat_open2==0)) 
		 {	   
		   cat_stop2=0;
       CourseNum2=7;
			 
		 }

	 if((cat_stop2==0)&&(cat_open2==1))   
		 {	  
		   cat_open2=0;     
		   CourseNum2=1; 
		 }

	Control_data[2]=cat_open2;//�ֶ�״̬������APP
	Control_data[3]=cat_stop2;//�ֶ�״̬������APP	 	 

		 
}


void Hand_KongZhi3(void)	//����3���غ�cat.1����ָ��
	
{	

 if((valve_stop3==1)&&(valve_open3==0)) 
	 {	   
       valve_stop3=0;
      hand_CourseNum3=4;//����رշ����߳�	 	 		 
	 }
	 
  if((valve_stop3==0)&&(valve_open3==1))   
     {	  
	  valve_open3=0;
	  hand_CourseNum3=1;	//���뿪�������߳�				
	 }
	 
	cat_valve_open3=Control_data[14];
	cat_valve_stop3=Control_data[15];	 

		 
	 if((cat_valve_stop3==1)&&(cat_valve_open3==0)) 
		 {	   
		   cat_valve_stop3=0;
		    hand_CourseNum3=4;//����رշ����߳�	 		 		 
		 }

	 if((cat_valve_stop3==0)&&(cat_valve_open3==1))   
		 {	  
		   cat_valve_open3=0;     
		   hand_CourseNum3=1;	//���뿪�������߳�			 
		 }

	Control_data[14]=cat_valve_open3;//�ֶ�״̬������APP
	Control_data[15]=cat_valve_stop3;//�ֶ�״̬������APP
		 
		 
		 
	if((stop3==1)&&(open3==0)) 
	 {	   
       stop3=0;
       CourseNum3=7;
	 }
	 
  if((stop3==0)&&(open3==1))   
     {	  
	    open3=0;
	    CourseNum3=1;
	   }
	 
	cat_open3=Control_data[4];
	cat_stop3=Control_data[5];	 

		 
	 if((cat_stop3==1)&&(cat_open3==0)) 
		 {	   
		   cat_stop3=0;
       CourseNum3=7;
			 
		 }

	 if((cat_stop3==0)&&(cat_open3==1))   
		 {	  
		   cat_open3=0;     
		   CourseNum3=1; 
		 }

	Control_data[4]=cat_open3;//�ֶ�״̬������APP
	Control_data[5]=cat_stop3;//�ֶ�״̬������APP	 	 	 
	 

}


void KongZhi_guozheng1_hand(void)
{
  Hand_KongZhi1();
 
	if( relay_state1!=Brelay_state1)//״̬�����仯��ִ��
		   {
		     if(relay_state1==0)  {AnimationPlayFrame(0,17,0); RELAY1=1;}     //ˮ��1����
		     if(relay_state1==1)  {AnimationPlayFrame(0,17,1); RELAY1=0;}     //ˮ��1ֹͣ
			   if(relay_state1==2)  {AnimationPlayFrame(0,17,2); RELAY1=0;}     //ˮ��1��ͣ
         if(relay_state1==3)  {AnimationPlayFrame(0,17,3); RELAY1=0;}     //ˮ��1ȱˮ
		     if(relay_state1==4)  {AnimationPlayFrame(0,17,4); RELAY1=0;}     //ˮ��1����
	
			   Brelay_state1=  relay_state1;
			   Crelay_state1=  1;	//4G���Ϳ���
		   } 
	   
		   
	  if( valve_state1!=Bvalve_state1)
		   {
		     
         if(valve_state1==0) { AnimationPlayFrame(0,14,0);   AnimationPlayFrame(0,15,0);}//���Źر�״̬
			   if(valve_state1==1) { AnimationPlayFrame(0,14,1);   AnimationPlayFrame(0,15,1);}//���ſ���״̬
			   if(valve_state1==3) { AnimationPlayFrame(0,14,3);   AnimationPlayFrame(0,15,3);}//���Źرս���			 
			   Bvalve_state1=  valve_state1;			 
			   Cvalve_state1=  1;
			 
		   }

 if(protection_switch_1==0)   hand_no_protection(&CourseNum,&relay_state1);//�ֶ�û�б���
 if(protection_switch_1==1)    hand_have_protection_1();//�ֶ��б���	 
	  valve_hand(&hand_CourseNum,&valve_state1,&wat_4 );	
		
	//hand_have_protection_1();//�ֶ��б���		 
			 
			 
}





void KongZhi_guozheng2_hand(void)
{
  Hand_KongZhi2();
 
	if( relay_state2!=Brelay_state2)//״̬�����仯��ִ��
		   {
		     if(relay_state2==0)  {AnimationPlayFrame(0,25,0); RELAY2=1;}     //ˮ��1����
		     if(relay_state2==1)  {AnimationPlayFrame(0,25,1); RELAY2=0;}     //ˮ��1ֹͣ
			   if(relay_state2==2)  {AnimationPlayFrame(0,25,2); RELAY2=0;}     //ˮ��1��ͣ
         if(relay_state2==3)  {AnimationPlayFrame(0,25,3); RELAY2=0;}     //ˮ��1ȱˮ
		     if(relay_state2==4)  {AnimationPlayFrame(0,25,4); RELAY2=0;}     //ˮ��1����
	
			   Brelay_state2=  relay_state2;
			   Crelay_state2=  1;	//4G���Ϳ���
		   } 
	   
		   
	  if( valve_state2!=Bvalve_state2)
		   {
		     
         if(valve_state2==0) { AnimationPlayFrame(0,20,0);   AnimationPlayFrame(0,23,0);}//���Źر�״̬
			   if(valve_state2==1) { AnimationPlayFrame(0,20,1);   AnimationPlayFrame(0,23,1);}//���ſ���״̬
			   if(valve_state2==3) { AnimationPlayFrame(0,20,3);   AnimationPlayFrame(0,23,3);}//���Źرս���			 
			   Bvalve_state2=  valve_state2;			 
			   Cvalve_state2=  1;
			 
		   }

 if(protection_switch_2==0)    hand_no_protection(&CourseNum2,&relay_state2);//�ֶ�û�б���
 if(protection_switch_2==1)    hand_have_protection_2();//�ֶ��б���	 
	  valve_hand(&hand_CourseNum2,&valve_state2,&wat_6 );	
		
	
			 
			 
}



void KongZhi_guozheng3_hand(void)
{
  Hand_KongZhi3();
 
	if( relay_state3!=Brelay_state3)//״̬�����仯��ִ��
		   {
		     if(relay_state3==0)  {AnimationPlayFrame(0,24,0); RELAY3=1;}     //ˮ��1����
		     if(relay_state3==1)  {AnimationPlayFrame(0,24,1); RELAY3=0;}     //ˮ��1ֹͣ
			   if(relay_state3==2)  {AnimationPlayFrame(0,24,2); RELAY3=0;}     //ˮ��1��ͣ
         if(relay_state3==3)  {AnimationPlayFrame(0,24,3); RELAY3=0;}     //ˮ��1ȱˮ
		     if(relay_state3==4)  {AnimationPlayFrame(0,24,4); RELAY3=0;}     //ˮ��1����
	
			   Brelay_state3=  relay_state3;
			   Crelay_state3=  1;	//4G���Ϳ���
		   } 
	   
		   
	  if( valve_state3!=Bvalve_state3)
		   {
		     
         if(valve_state3==0) { AnimationPlayFrame(0,22,0);   AnimationPlayFrame(0,21,0);}//���Źر�״̬
			   if(valve_state3==1) { AnimationPlayFrame(0,22,1);   AnimationPlayFrame(0,21,1);}//���ſ���״̬
			   if(valve_state3==3) { AnimationPlayFrame(0,22,3);   AnimationPlayFrame(0,21,3);}//���Źرս���			 
			   Bvalve_state3=  valve_state3;			 
			   Cvalve_state3=  1;
			 
		   }

 if(protection_switch_3==0)   hand_no_protection(&CourseNum3,&relay_state3);//�ֶ�û�б���
 if(protection_switch_3==1)    hand_have_protection_3();//�ֶ��б���	 
	  valve_hand(&hand_CourseNum3,&valve_state3,&wat_7 );	
		
	//hand_have_protection_1();//�ֶ��б���		 
			 
			 
}



/*

�ֶ����ط��ź���
��������Ǻ�ˮ�õĽ��̷ֿ���

*/



void valve_hand(u8 *hand_CourseNumx,u8 *valve_statex,TROL_WAT wat_x )

  {
	
	switch (*hand_CourseNumx)
		
	  {			 
		 
			case 1://��������				
			//valve_state1=1;//��������־
		  *valve_statex=3;//�ط����б�־
		  //valve_state1=0;//���رձ�־
			*hand_CourseNumx=2;		
      break;
				
		  case 2://��������		
			
			if(control_wait_s(wat_x,valve_travel_time))
				{
				  *hand_CourseNumx=3;						
				}
			break;	
		
		  case 3://�������
	 
		  *valve_statex=1;//��������־
		  // valve_state1=3;//�ط����б�־
		  //valve_state1=0;//���رձ�־
			break;

    	case 4://�رշ���
				
			//valve_state1=1;//��������־
		  *valve_statex=3;//�ط����б�־
		  //valve_state1=0;//���رձ�־
			*hand_CourseNumx=5;		
      break;
				
		  case 5://�رչ���
			
			if(control_wait_s(wat_x,valve_travel_time))
				{
				  *hand_CourseNumx=6;	
				}
			break;				
		  case 6://�ر����
	 
		  //valve_state1=1;//��������־
		  //valve_state1=3;//�ط����б�־
		  *valve_statex=0;//���رձ�־
			break;
			
		}			
	
	}

	

void hand_no_protection(u8 *hand_CourseNum,u8 *relay_state1)
{	 
		switch (*hand_CourseNum)
		
	  {	
	
	    case 1://����ˮ��
	
			*relay_state1=0;//ˮ������
			//relay_state1=1;//ˮ��ֹͣ
			//relay_state1=2;//ˮ����ͣ
	
			break;
	 
	    case 7://ֹͣˮ��
	 
			
			//relay_state1=0;//ˮ������
			*relay_state1=1;//ˮ��ֹͣ
			//relay_state1=2;//ˮ����ͣ
	
			break;	 
		
	}	 
	
 }

 
 
// extern	u8 hand_CourseNum;
// extern	u8 hand_CourseNum2;
// extern	u8 hand_CourseNum3;
 
 
/*
 
*/ 
 

void hand_have_protection_1(void) 
{	 
		switch (CourseNum)
		
	  {	
	
	    case 1://����ˮ��	
			relay_state1=0;//ˮ������
			//relay_state1=1;//ˮ��ֹͣ
			//relay_state1=2;//ˮ����ͣ
			//relay_state1=3;//ˮ��ȱˮ
      //relay_state1=4;//ˮ�ù���				
			
			if(control_wait_s(&wat_1,absence_DelayT))
	
				{  				
				  if((A_current>5)&&(A_current<500)) CourseNum=3;	//û��ȱ���ж��Ƿ����
				  else CourseNum=2;	//ȱ����߶�ת �������			
				}				
			break;
			case 2://���ȱ���ת����
 		
		   alarm_quex_biao1=1;//����ȱ�౨��

		   CourseNum=102;
		
		   break;
		
		   case 102://�������ֹͣ����		
       relay_state1=4;			 
		   break;
			
			case 3://�ж��Ƿ����
		
			if(control_wait_s(&wat_1,10))		
				{				  
				  if(A_current<10)  CourseNum=2;//ȱ����߶�ת �������	
				  if(A_current>500)  CourseNum=2;//ȱ����߶�ת �������	
					
					//if(A_current>=ALoadCurrent*2)  CourseNum=2;//ȱ����߶�ת �������				
				  //if((A_current>=10)&&(A_current<=(ALoadCurrent/5))) CourseNum=4; //������	
					if((A_current>=10)&&(A_current<=(200))) CourseNum=4; //������	
				  //if((A_current>(ALoadCurrent/5))&&(A_current<(ALoadCurrent*2))) CourseNum=5;//��������	
					if((A_current>(200))&&(A_current<(500))) CourseNum=5;//��������	
				}
				
	    break;			
				
		case 4://������		
		
    alarm_well_biao1=1;	
    CourseNum=101;		
		break;

		case 101://������		
		relay_state1=3;//ˮ��1ȱˮֹͣ	
    
		break;
		
		
		case 5://����			

			if(control_wait_s(&wat_1,1))
				 
				{  
					
					if(A_current<10)  CourseNum=6;//ȱ����߶�ת �������	
					if(A_current>500)  CourseNum=6;//ȱ����߶�ת �������	
					//if(A_current>=ALoadCurrent*2)  CourseNum=6;//ȱ����߶�ת �������	
          if((A_current>=10)&&(A_current<=(200))) CourseNum=6; //������						
					//if((A_current>=10)&&(A_current<=(ALoadCurrent/5))) CourseNum=13; //������Ӧ����ת�� ��ˮλ�ӳ�ʱ�����					
					//if((A_current>(ALoadCurrent/5))&&(A_current<(ALoadCurrent*2))) CourseNum=5;//��������
           if((A_current>(200))&&(A_current<(500))) CourseNum=5;//��������						
				}
			
			break;
				
      case 6://ȥ��

			if(control_wait_s(&wat_1,2))
				 
				{ 					
					if(A_current<10)  CourseNum=2;//ȱ����߶�ת �������	
					if(A_current>500)  CourseNum=2;//ȱ����߶�ת �������	
					//if(A_current>=ALoadCurrent*2)  CourseNum=2;//ȱ����߶�ת �������	
          if((A_current>=10)&&(A_current<=(200))) CourseNum=4; //������						
					//if((A_current>=10)&&(A_current<=(ALoadCurrent/5))) CourseNum=13; //������Ӧ����ת�� ��ˮλ�ӳ�ʱ�����					
					//if((A_current>(ALoadCurrent/5))&&(A_current<(ALoadCurrent*2))) CourseNum=5;//��������
           if((A_current>(200))&&(A_current<(500))) CourseNum=5;//��������						
				}
			
			break;
				
		case 7://ֹͣ
		relay_state1=1;//ˮ��ֹͣ
		break;		

				
	}	 
	
 }

void hand_have_protection_2(void)  
{	 
		switch (CourseNum2)
		
	  {	
	
	    case 1://����ˮ��	
			relay_state2=0;//ˮ������
			//relay_state1=1;//ˮ��ֹͣ
			//relay_state1=2;//ˮ����ͣ
			//relay_state1=3;//ˮ��ȱˮ
      //relay_state1=4;//ˮ�ù���				
			
			if(control_wait_s(&wat_2,absence_DelayT))
	
				{  				
				  if((A_current>5)&&(A_current<500)) CourseNum2=3;	//û��ȱ���ж��Ƿ����
				  else CourseNum2=2;	//ȱ����߶�ת �������			
				}				
			break;
			case 2://���ȱ���ת����
 		
		   alarm_quex_biao2=1;//����ȱ�౨��

		   CourseNum2=102;
		
		   break;
		
		   case 102://�������ֹͣ����		
       relay_state2=4;			 
		   break;
			
			case 3://�ж��Ƿ����
		
			if(control_wait_s(&wat_2,10))		
				{				  
				  if(A_current<10)  CourseNum2=2;//ȱ����߶�ת �������	
				  if(A_current>500)  CourseNum2=2;//ȱ����߶�ת �������	
					
					//if(A_current>=ALoadCurrent*2)  CourseNum=2;//ȱ����߶�ת �������				
				  //if((A_current>=10)&&(A_current<=(ALoadCurrent/5))) CourseNum=4; //������	
					if((A_current>=10)&&(A_current<=(200))) CourseNum2=4; //������	
				  //if((A_current>(ALoadCurrent/5))&&(A_current<(ALoadCurrent*2))) CourseNum=5;//��������	
					if((A_current>(200))&&(A_current<(500))) CourseNum2=5;//��������	
				}
				
	    break;			
				
		case 4://������		
		
    alarm_well_biao2=1;	
    CourseNum2=101;		
		break;

		case 101://������		
		relay_state2=3;//ˮ��1ȱˮֹͣ	
    
		break;
		
		
		case 5://����			

			if(control_wait_s(&wat_2,1))
				 
				{  
					
					if(A_current<10)  CourseNum2=6;//ȱ����߶�ת �������	
					if(A_current>500)  CourseNum2=6;//ȱ����߶�ת �������	
					//if(A_current>=ALoadCurrent*2)  CourseNum=6;//ȱ����߶�ת �������	
          if((A_current>=10)&&(A_current<=(200))) CourseNum2=6; //������						
					//if((A_current>=10)&&(A_current<=(ALoadCurrent/5))) CourseNum=13; //������Ӧ����ת�� ��ˮλ�ӳ�ʱ�����					
					//if((A_current>(ALoadCurrent/5))&&(A_current<(ALoadCurrent*2))) CourseNum=5;//��������
           if((A_current>(200))&&(A_current<(500))) CourseNum2=5;//��������						
				}
			
			break;
				
      case 6://ȥ��

			if(control_wait_s(&wat_2,2))
				 
				{ 					
					if(A_current<10)  CourseNum2=2;//ȱ����߶�ת �������	
					if(A_current>500)  CourseNum2=2;//ȱ����߶�ת �������	
					//if(A_current>=ALoadCurrent*2)  CourseNum=2;//ȱ����߶�ת �������	
          if((A_current>=10)&&(A_current<=(200))) CourseNum2=4; //������						
					//if((A_current>=10)&&(A_current<=(ALoadCurrent/5))) CourseNum=13; //������Ӧ����ת�� ��ˮλ�ӳ�ʱ�����					
					//if((A_current>(ALoadCurrent/5))&&(A_current<(ALoadCurrent*2))) CourseNum=5;//��������
           if((A_current>(200))&&(A_current<(500))) CourseNum2=5;//��������						
				}
			
			break;
				
		case 7://ֹͣ
		relay_state2=1;//ˮ��ֹͣ
		break;		

				
	}	 
	
 }

 
 
 
 void hand_have_protection_3(void) 
{	 
		switch (CourseNum3)
		
	  {	
	
	    case 1://����ˮ��	
			relay_state3=0;//ˮ������
			//relay_state1=1;//ˮ��ֹͣ
			//relay_state1=2;//ˮ����ͣ
			//relay_state1=3;//ˮ��ȱˮ
      //relay_state1=4;//ˮ�ù���				
			
			if(control_wait_s(&wat_3,absence_DelayT))
	
				{  				
				  if((A_current>5)&&(A_current<500)) CourseNum3=3;	//û��ȱ���ж��Ƿ����
				  else CourseNum3=2;	//ȱ����߶�ת �������			
				}				
			break;
			case 2://���ȱ���ת����
 		
		   alarm_quex_biao3=1;//����ȱ�౨��

		   CourseNum3=102;
		
		   break;
		
		   case 102://�������ֹͣ����		
       relay_state3=4;			 
		   break;
			
			case 3://�ж��Ƿ����
		
			if(control_wait_s(&wat_3,10))		
				{				  
				  if(A_current<10)  CourseNum3=2;//ȱ����߶�ת �������	
				  if(A_current>500)  CourseNum3=2;//ȱ����߶�ת �������	
					
					//if(A_current>=ALoadCurrent*2)  CourseNum=2;//ȱ����߶�ת �������				
				  //if((A_current>=10)&&(A_current<=(ALoadCurrent/5))) CourseNum=4; //������	
					if((A_current>=10)&&(A_current<=(200))) CourseNum3=4; //������	
				  //if((A_current>(ALoadCurrent/5))&&(A_current<(ALoadCurrent*2))) CourseNum=5;//��������	
					if((A_current>(200))&&(A_current<(500))) CourseNum3=5;//��������	
				}
				
	    break;			
				
		case 4://������		
		
    alarm_well_biao3=1;	
    CourseNum3=101;		
		break;

		case 101://������		
		relay_state3=3;//ˮ��1ȱˮֹͣ	
    
		break;
		
		
		case 5://����			

			if(control_wait_s(&wat_3,1))
				 
				{  
					
					if(A_current<10)  CourseNum3=6;//ȱ����߶�ת �������	
					if(A_current>500)  CourseNum3=6;//ȱ����߶�ת �������	
					//if(A_current>=ALoadCurrent*2)  CourseNum=6;//ȱ����߶�ת �������	
          if((A_current>=10)&&(A_current<=(200))) CourseNum3=6; //������						
					//if((A_current>=10)&&(A_current<=(ALoadCurrent/5))) CourseNum=13; //������Ӧ����ת�� ��ˮλ�ӳ�ʱ�����					
					//if((A_current>(ALoadCurrent/5))&&(A_current<(ALoadCurrent*2))) CourseNum=5;//��������
           if((A_current>(200))&&(A_current<(500))) CourseNum3=5;//��������						
				}
			
			break;
				
      case 6://ȥ��

			if(control_wait_s(&wat_3,2))
				 
				{ 					
					if(A_current<10)  CourseNum3=2;//ȱ����߶�ת �������	
					if(A_current>500)  CourseNum3=2;//ȱ����߶�ת �������	
					//if(A_current>=ALoadCurrent*2)  CourseNum=2;//ȱ����߶�ת �������	
          if((A_current>=10)&&(A_current<=(200))) CourseNum3=4; //������						
					//if((A_current>=10)&&(A_current<=(ALoadCurrent/5))) CourseNum=13; //������Ӧ����ת�� ��ˮλ�ӳ�ʱ�����					
					//if((A_current>(ALoadCurrent/5))&&(A_current<(ALoadCurrent*2))) CourseNum=5;//��������
           if((A_current>(200))&&(A_current<(500))) CourseNum3=5;//��������						
				}
			
			break;
				
		case 7://ֹͣ
		relay_state3=1;//ˮ��ֹͣ
		break;		

				
	}	 
	
 }


 
void KongZhi_guozheng_1(void)

{	
	
//	u16 ALoadCurrent,BLoadCurrent,CLoadCurrent;//���ص���	
//	u16 PresentLevel;//��ǰˮλ	
//	u16 A_current, B_current, C_current;//��ǰ����
	
	static u16 a_timer=0;//��ʱ����
	static u8  at_biao=0;//

	
/*//CtrSend_data[0]  alarm_quex_biao1
	//CtrSend_data[1]  alarm_quex_biao2
	//CtrSend_data[2]  alarm_quex_biao3
	//CtrSend_data[3]  alarm_well_biao1
	//CtrSend_data[4]  alarm_well_biao2
	//CtrSend_data[5]  alarm_well_biao3
	//CtrSend_data[6]  alarm_pull_biao
	//CtrSend_data[7]  alarm_noelec_biao
	//CtrSend_data[8]  alarm_upelec_biao
*/	
	
	
	
	KongZhi1();	

if( relay_state1!=Brelay_state1)//״̬�����仯��ִ��
		   {
		     if(relay_state1==0)  {AnimationPlayFrame(0,17,0); RELAY1=1;}     //ˮ��1����
		     if(relay_state1==1)  {AnimationPlayFrame(0,17,1); RELAY1=0;}     //ˮ��1ֹͣ
			   if(relay_state1==2)  {AnimationPlayFrame(0,17,2); RELAY1=0;}     //ˮ��1��ͣ
         if(relay_state1==3)  {AnimationPlayFrame(0,17,3); RELAY1=0;}     //ˮ��1ȱˮ
		     if(relay_state1==4)  {AnimationPlayFrame(0,17,4); RELAY1=0;}     //ˮ��1����
			   Brelay_state1=  relay_state1;
			   Crelay_state1=  1;	//4G���Ϳ���
		   } 
		   
		   
	  if( valve_state1!=Bvalve_state1)
		   {
		     
         if(valve_state1==0) { AnimationPlayFrame(0,14,0);   AnimationPlayFrame(0,15,0);}//���Źر�״̬
			   if(valve_state1==1) { AnimationPlayFrame(0,14,1);   AnimationPlayFrame(0,15,1);}//���ſ���״̬
			   if(valve_state1==3) { AnimationPlayFrame(0,14,3);   AnimationPlayFrame(0,15,3);}//���Źرս���			 
			   Bvalve_state1=  valve_state1;			 
			   Cvalve_state1=  1;
			 
		   } 	   


			 
 switch (CourseNum)
		
	{		
		case 1://��ˮ��������ת������			
		
			relay_state1=0;//����ˮ��״̬					
			if(control_wait_s(&wat_1,absence_DelayT))
	
				{  				
				  if((A_current>5)&&(A_current<500)) CourseNum=3;	//û��ȱ���ж��Ƿ����
				  else CourseNum=2;	//ȱ����߶�ת �������			
				}				
		
		break;
		
		case 2://���ȱ���ת����
		
		alarm_quex_biao1=1;//����ȱ�౨��
    relay_state1=4;
		CourseNum=23;
		
		break;
		
		case 23://���ֹͣ����
		RELAY1=0;
    AnimationPlayFrame(0,17,1);//ֹͣ����
		relay_state1=1;//ֹͣ״̬	
		break;
		
		
		case 3://�ж��Ƿ����
		
			if(control_wait_s(&wat_1,10))		
				{				  
				  if(A_current<10)  CourseNum=2;//ȱ����߶�ת �������	
				  if(A_current>=ALoadCurrent*2)  CourseNum=2;//ȱ����߶�ת �������				
				  //if((A_current>=10)&&(A_current<=(ALoadCurrent/5))) CourseNum=4; //������	
					if((A_current>=10)&&(A_current<=(200))) CourseNum=4; //������	
				  //if((A_current>(ALoadCurrent/5))&&(A_current<(ALoadCurrent*2))) CourseNum=5;//��������	
					if((A_current>(200))&&(A_current<(500))) CourseNum=5;//��������	
				}
				
	    break;		
		
		
		case 4://������
		
		  relay_state1=2;//ˮ��1��ͣ״̬
			CourseNum=6;  
			break;
		
		case 5://����			

			if(control_wait_s(&wat_1,2))
				 
				{ 
					
					a_timer=0;//���ʱ��
					open_Num1=0;//�����ˮ����
					if(A_current<10)  CourseNum=101;//ȱ����߶�ת �������	
					if(A_current>=ALoadCurrent*2)  CourseNum=101;//ȱ����߶�ת �������	
          if((A_current>=10)&&(A_current<=(200))) CourseNum=101; //������						
					//if((A_current>=10)&&(A_current<=(ALoadCurrent/5))) CourseNum=13; //������Ӧ����ת�� ��ˮλ�ӳ�ʱ�����					
					//if((A_current>(ALoadCurrent/5))&&(A_current<(ALoadCurrent*2))) CourseNum=5;//��������
          if((A_current>(200))&&(A_current<(500))) CourseNum=5;//��������
					
				}
			
			break;
		
				
				
			case 101://����			

			if(control_wait_s(&wat_1,2))
				 
				{ 					
					if(A_current<10)  CourseNum=2;//ȱ����߶�ת �������	
					if(A_current>=ALoadCurrent*2)  CourseNum=2;//ȱ����߶�ת �������	          
          if(auto_button_1==1)         //û���Զ���ͣ
						{ 					
					   if((A_current>=10)&&(A_current<=(200))) CourseNum=11; //ȱ��ˮ�����ڵ�  					
						}
					else 
					  {					
					   if((A_current>=10)&&(A_current<=(200))) CourseNum=13; //������						
					  }
					
					//if((A_current>=10)&&(A_current<=(ALoadCurrent/5))) CourseNum=13; //������Ӧ����ת�� ��ˮλ�ӳ�ʱ�����					
					//if((A_current>(ALoadCurrent/5))&&(A_current<(ALoadCurrent*2))) CourseNum=5;//��������
          if((A_current>(200))&&(A_current<(500))) CourseNum=5;//��������
					
				}
			
			break;		
				
			
				
		case 6://����ˮ
			Kai_moto1=1;//������
			Kai_moto2=1;//��ˮ
			
		    valve_state1=1;//��������־
		   
		   
		    if( control_wait_s( &wat_1,10))//��ʱ10����ֹͣ��ˮ�����������ʱ����ˮ��״̬�ǿ���״̬��
			   {		
			    Kai_moto1=0;
			    Kai_moto2=0;				   
          CourseNum=7;				   
			   }

			break;	
		
		case 7:			
		   if(A_LEVEL==1)	CourseNum=8;//ˮλ����ת���رշ���	
		   if( control_wait_s( &wat_1,NOwater_T))//�ȴ�ȱ��ˮʱ�䵽��ת��ȱˮ���� 
			    {		
			     CourseNum=15;//ȱ��ˮ			
			    }		
			break;	
				
				
		case 15:	//ȱ��ˮ������ͣ���ֹͣ		
	     Guan_moto1=1;//�رշ���
		   Guan_moto2=1;//�ر�����		   
		   valve_state1=3;//�ط����б�־
		   
		   if( control_wait_s( &wat_1,10)) //��ʱ10�� ֹͣ�رշ���
			   {		
			    Guan_moto1=0;
		      Guan_moto2=0;				 
		      valve_state1=0;//���رձ�־ 				   
          CourseNum=9;//����ȱ��ˮ����	   
			   }
		
			break;			
				
				
		
		case 8:	//�رշ���		
		   Guan_moto1=1;//�رշ���
		   Guan_moto2=1;//�ر�����		
		   valve_state1=3;//�ط����б�־		
		   if( control_wait_s( &wat_1,10)) //��ʱ10�� ֹͣ�رշ���
			   {

					valve_state1=0;//���رձ�־
				   
			    Guan_moto1=0;
		      Guan_moto2=0;
          CourseNum=10;//�ٴ��������	
					 
			   }
		
			break;	
		
		case 9:	//ȱ��ˮ������ͣ���ֹͣ		

		  alarm_pull_biao=1;		
		  CourseNum=12;		
		  break;
		
	 case 10://�ٴ�����ˮ��			
		

		open_Num1++;
		
		if(open_Num1<3)  
		  {			
			CourseNum=1;
		  }
		  
		if(open_Num1>=3) 
		  {
		   CourseNum=11;//����ʧ��//����ˮ��ȱˮ
          }
		  
		break;		
		case 11:	//����ʧ��//����ˮ��ȱˮ
			
        alarm_well_biao1=1;
		    relay_state1=3;
        CourseNum=102;		
		break;
		
		
		case 102:	//����ʧ��//����ˮ��ȱˮ
			
     relay_state1=3;//ȱˮֹͣ
		 valve_state1=0;//���رձ�־  
		break;
		
		
		
		case 12:	//����ֹͣ����������ڵ�
		
		relay_state1=1;//ֹͣ״̬		
		valve_state1=0;//���رձ�־
		
		break;
		
  case 13:	//��ͣ״̬
		
    relay_state1=2;//��ͣ״̬
	
		CourseNum=22;		
		break;
   
  
  case 22://ˮ��ȱˮ����
	  alarm_well_biao1=1;
	  again_startT_biao1=1;
	  //CtrSend_data[20]	=again_startT1-(a_timer/60);//�������ݵ�APP		
	
	
	  again_startT1_test=0xffff;
	
    CourseNum=14;
      break;
		
  case 14:	//��ʱ������

	
	
	  if(PresentLevel<=set_leve[5]) again_startT1=set_time[5];		
		if((PresentLevel>set_leve[5])&&(PresentLevel<=set_leve[4])) again_startT1=set_time[4];
		if((PresentLevel>set_leve[4])&&(PresentLevel<=set_leve[3])) again_startT1=set_time[3];
		if((PresentLevel>set_leve[3])&&(PresentLevel<=set_leve[2])) again_startT1=set_time[2];
		if((PresentLevel>set_leve[2])&&(PresentLevel<=set_leve[1])) again_startT1=set_time[1];
		if(PresentLevel>set_leve[1]) again_startT1=set_time[1];
	
	
	
	
	if(again_startT1_test!=again_startT1)
	 {
	  again_startT1_test=again_startT1; 
		CtrSend_data[20]	=again_startT1-(a_timer/60);//�������ݵ�APP		
	 }
	
	
		if(((TIMER_1MS%1000)>450)) 
		 {
			 if(at_biao==0)	
			 { 
			   a_timer++;	            
			   at_biao=1;	
				 SetTextValue(0,32,"Լ"); 
				 SetTextInt32(0,34,(again_startT1-(a_timer/60)),0,1);
				 SetTextValue(0,33,"�ֺ�����"); 
         SetTextInt32(0,36,a_timer%60,0,1);
         //CtrSend_data[20]	=again_startT1-(a_timer/60);//�������ݵ�APP			 
			 }
		  }
		 else at_biao=0;
		
		if( control_wait_s( &wat_1,(again_startT1*60)))//�ȴ�ȱ��ˮʱ�䵽��ת��ȱˮ���� 
			{		
			  CourseNum=1;//				
			}		
		
    break;

	
    case 20:	

		   
		   valve_state1=3;//���Źرս���	
       Kai_moto1=0;
		   Kai_moto2=0;		
 
		   if( control_wait_s( &wat_1,2)) //��ʱ10�� ֹͣ�رշ���
			   {			
             CourseNum=21;//�ٴ��������				   
			   }
		
			break;			


	case 21:	//�رշ���		
		   Guan_moto1=1;//�رշ���
		   Guan_moto2=1;//�ر�����
			 valve_state1=3;//���Źرս��̱�־
	
		   if( control_wait_s( &wat_1,10)) //��ʱ10�� ֹͣ�رշ���
			   {

				  valve_state1=0;//���رձ�־
			    Guan_moto1=0;
		      Guan_moto2=0;
          CourseNum=12;//����رյ���ڵ�				   
			   }
		
			break;					   
		
	}

}


void KongZhi_guozheng_2(void)

{	
	
//	u16 ALoadCurrent,BLoadCurrent,CLoadCurrent;//���ص���	
//	u16 PresentLevel;//��ǰˮλ	
//	u16 A_current, B_current, C_current;//��ǰ����
	
	static u16 a_timer=0;//��ʱ����
	static u8  at_biao=0;//

	
/*	//CtrSend_data[0]  alarm_quex_biao1
	//CtrSend_data[1]  alarm_quex_biao2
	//CtrSend_data[2]  alarm_quex_biao3
	//CtrSend_data[3]  alarm_well_biao1
	//CtrSend_data[4]  alarm_well_biao2
	//CtrSend_data[5]  alarm_well_biao3
	//CtrSend_data[6]  alarm_pull_biao
	//CtrSend_data[7]  alarm_noelec_biao
	//CtrSend_data[8]  alarm_upelec_biao
*/	
	
	
	KongZhi2();	
	

	 	
	 if( relay_state2!=Brelay_state2)
		   {
		     


         if(relay_state2==0)  {AnimationPlayFrame(0,25,0); RELAY2=1;}     //ˮ��1����
		     if(relay_state2==1)  {AnimationPlayFrame(0,25,1); RELAY2=0;}     //ˮ��1ֹͣ
			   if(relay_state2==2)  {AnimationPlayFrame(0,25,2); RELAY2=0;}     //ˮ��1��ͣ
         if(relay_state2==3)  {AnimationPlayFrame(0,25,3); RELAY2=0;}     //ˮ��1ȱˮ
		     if(relay_state2==4)  {AnimationPlayFrame(0,25,4); RELAY2=0;}     //ˮ��1����
			   Brelay_state2=  relay_state2;
			   Crelay_state2=  1;	//4G���Ϳ���


		   } 

		   
	  if( valve_state2!=Bvalve_state2)
		   {		     
       if(valve_state2==0) { AnimationPlayFrame(0,20,0);   AnimationPlayFrame(0,23,0);}//���Źر�״̬
			 if(valve_state2==1) { AnimationPlayFrame(0,20,1);   AnimationPlayFrame(0,23,1);}//���ſ���״̬
			 if(valve_state2==3) { AnimationPlayFrame(0,20,3);   AnimationPlayFrame(0,23,3);}//���Źرս���			 
			 Bvalve_state2=  valve_state2;			 
			 Cvalve_state2=  1;			 
		   } 	   


			 
	switch (CourseNum2)
		
	{		
		case 1: //��ˮ��������ת������		
	
			relay_state2=0;//����ˮ��״̬	
			if(control_wait_s(&wat_2,absence_DelayT))//absence_DelayT ȱ��ȴ�ʱ��
	
				{  				
				  if((B_current>5)&&(B_current<500)) CourseNum2=3;	//û��ȱ���ж��Ƿ����
				  else CourseNum2=2;	//ȱ����߶�ת �������			
				}				
		
		
		
		break;
		
		case 2://���ȱ���ת����
	
		alarm_quex_biao2=1;//����ȱ�౨��
    relay_state2=4;//��ʾ�������
		CourseNum2=23;
		
		break;

		case 23://���ֹͣ����
			relay_state2=1;//ֹͣ״̬	
		break;		
		
		case 3://�ж��Ƿ����
		
			if(control_wait_s(&wat_2,10))		
				{				  
				  if(B_current<10)  CourseNum2=2;//ȱ����߶�ת �������	
				  if(B_current>=BLoadCurrent*2)  CourseNum2=2;//ȱ����߶�ת �������				
				  //if((A_current>=10)&&(A_current<=(ALoadCurrent/5))) CourseNum=4; //������	
					if((B_current>=10)&&(B_current<=(200))) CourseNum2=4; //������	
				  //if((A_current>(ALoadCurrent/5))&&(A_current<(ALoadCurrent*2))) CourseNum=5;//��������	
					if((B_current>(200))&&(B_current<(500))) CourseNum2=5;//��������	
				}
				
	    break;		
		
		
		case 4://������			
		  relay_state2=2;//ˮ��2��ͣ״̬
			CourseNum2=6;  
		break;
		
		case 5://����			

			if(control_wait_s(&wat_2,2))
				 
				{ a_timer=0;//���ʱ��
					open_Num2=0;//�����ˮ����
					if(B_current<10)  CourseNum2=101;//ȱ����߶�ת �������	
					if(B_current>=BLoadCurrent*2)  CourseNum2=101;//ȱ����߶�ת �������	
          if((B_current>=10)&&(B_current<=(200))) CourseNum2=101; //������						
					//if((A_current>=10)&&(A_current<=(ALoadCurrent/5))) CourseNum=13; //������Ӧ����ת�� ��ˮλ�ӳ�ʱ�����					
					//if((A_current>(ALoadCurrent/5))&&(A_current<(ALoadCurrent*2))) CourseNum=5;//��������
          if((B_current>(200))&&(B_current<(500))) CourseNum2=5;//��������						
				}
			
			break;
				
			case 101://����			

			if(control_wait_s(&wat_2,2))
				 
				{ 
					if(B_current<10)  CourseNum2=2;//ȱ����߶�ת �������	
					if(B_current>=BLoadCurrent*2)  CourseNum2=2;//ȱ����߶�ת �������	
					
					
					if(auto_button_1==1)         //û���Զ���ͣ
						{ 					
					   if((B_current>=10)&&(B_current<=(200))) CourseNum2=11; //ȱˮ��������					
						}
					else 
					  {					
					   if((B_current>=10)&&(B_current<=(200))) CourseNum2=13; //������								
					  }
					
					
          				
					//if((A_current>=10)&&(A_current<=(ALoadCurrent/5))) CourseNum=13; //������Ӧ����ת�� ��ˮλ�ӳ�ʱ�����					
					//if((A_current>(ALoadCurrent/5))&&(A_current<(ALoadCurrent*2))) CourseNum=5;//��������
          if((B_current>(200))&&(B_current<(500))) CourseNum2=5;//��������						
				}
			
			break;		
				
				
				
				
		
		case 6://����ˮ
			Kai_moto3=1;//������
			Kai_moto4=1;//��ˮ
			
		    valve_state2=1;//��������־		    
		   
		    if( control_wait_s( &wat_2,10))//��ʱ10����ֹͣ��ˮ�����������ʱ����ˮ��״̬�ǿ���״̬��
			   {		
			    Kai_moto3=0;
			    Kai_moto4=0;				   
          CourseNum2=7;				   
			   }

			break;	
		
		case 7:			
		   if(B_LEVEL==1)	CourseNum2=8;//ˮλ����ת���رշ���	
		   if( control_wait_s( &wat_2,NOwater_T))//�ȴ�ȱ��ˮʱ�䵽��ת��ȱˮ���� 
			    {		
			     CourseNum2=15;//ȱ��ˮ			
			    }		
			break;	
				
				
		case 15:	//ȱ��ˮ������ͣ���ֹͣ		
	     Guan_moto3=1;//�رշ���
		   Guan_moto4=1;//�ر�����
		  
		   valve_state2=3;//�ط����б�־
		   
		
		
		   if( control_wait_s( &wat_2,10)) //��ʱ10�� ֹͣ�رշ���
			   {		
			    Guan_moto3=0;
		      Guan_moto4=0;				   
				
		        valve_state2=0;//���رձ�־   
				   
            CourseNum2=9;//����ȱ��ˮ����	   
			   }
		
			break;			
				
				
		
		case 8:	//�رշ���		
		   Guan_moto3=1;//�رշ���
		   Guan_moto4=1;//�ر�����		 
		   valve_state2=3;//�ط����б�־	
		   if( control_wait_s( &wat_2,10)) //��ʱ10�� ֹͣ�رշ���
			   {
		      valve_state2=0;//���رձ�־
			    Guan_moto3=0;
		      Guan_moto4=0;
          CourseNum2=10;//�ٴ��������				   
			   }
		
			break;	
		
		case 9:	//ȱ��ˮ������ͣ���ֹͣ		

		alarm_pull_biao=1;	//����������һ��	
		CourseNum2=12;		
		break;
		
	case 10://�ٴ�����ˮ��			
		

		open_Num2++;
		
		if(open_Num2<3)  
		  {			
			CourseNum2=1;
		  }
		  
		if(open_Num2>=3) 
		  {
		   CourseNum2=11;//����ʧ��//����ˮ��ȱˮ
       }
		  
		break;		
		case 11:	//����ʧ��//����ˮ��ȱˮ
			
        alarm_well_biao2=1;
        relay_state2=3;		      
        CourseNum2=102;		
		break;
		
		case 102:	//����ʧ��//����ˮ��ȱˮ
			
     relay_state2=3;//ȱˮֹͣ
		 valve_state2=0;//���رձ�־  
		break;
		
		
		case 12:	//ֹͣ״̬
		   
		 relay_state2=1;//ֹͣ״̬       
		 valve_state2=0;//���رձ�־
		
		break;
		
  case 13:	//��ͣ״̬
		   
    relay_state2=2;//��ͣ״̬		 
		CourseNum2=22;		
		break;
   
  
  case 22://ˮ��ȱˮ����
	    alarm_well_biao2=1;
	    again_startT_biao2=1;
	    //CtrSend_data[21]	=again_startT2-(a_timer/60);//�������ݵ�APP	
	    again_startT2_test=0xffff;
      CourseNum2=14;
      break;
		
  case 14:	//��ʱ������

		
	  if(PresentLevel<=set_leve[10])                               again_startT2=set_time[10];		
		if((PresentLevel>set_leve[10])&&(PresentLevel<=set_leve[9])) again_startT2=set_time[9];
		if((PresentLevel>set_leve[9]) &&(PresentLevel<=set_leve[8])) again_startT2=set_time[8];
		if((PresentLevel>set_leve[8]) &&(PresentLevel<=set_leve[7])) again_startT2=set_time[7];
		if((PresentLevel>set_leve[7]) &&(PresentLevel<=set_leve[6])) again_startT2=set_time[6];
		if(PresentLevel> set_leve[6])                                again_startT2=set_time[6];
	
	
	if(again_startT2_test!=again_startT2)
	 {
	  again_startT2_test=again_startT2; 
		CtrSend_data[21]	=again_startT2-(a_timer/60);//�������ݵ�APP		
	 }
	
	
		if(((TIMER_1MS%1000)>450)) 
		 {
			 if(at_biao==0)	
			 { 
			   a_timer++;	
         			 
			   at_biao=1;			
		     SetTextValue(0,38,"Լ"); 
				 SetTextInt32(0,35,(again_startT2-(a_timer/60)),0,1);
				 SetTextValue(0,39,"�ֺ�����"); 
         SetTextInt32(0,37,a_timer%60,0,1);	
        // CtrSend_data[21]	=again_startT2-(a_timer/60);//�������ݵ�APP	

				 
			 }
		  }
		 else at_biao=0;
		
		if( control_wait_s( &wat_2,(again_startT2*60)))//�ȴ�ȱ��ˮʱ�䵽��ת��ȱˮ���� 
			{		
			  CourseNum2=1;//				
			}		
		
    break;




   case 20:	
		
		   valve_state2=3;//�ط����б�־
		    		
       Kai_moto3=0;
		   Kai_moto4=0;		
 
		   if( control_wait_s( &wat_2,2)) //��ʱ10�� ֹͣ�رշ���
			   {			
                CourseNum2=21;//�ٴ��������				   
			   }
		
			break;			


	case 21:	//�رշ���		
		   Guan_moto3=1;//�رշ���
		   Guan_moto4=1;//�ر����� 		
		   valve_state2=3;//�ط����б�־	   
		
		   if( control_wait_s( &wat_2,10)) //��ʱ10�� ֹͣ�رշ���
			   {		
				  valve_state2=0;//���رձ�־		   
			    Guan_moto3=0;
		      Guan_moto4=0;
          CourseNum2=12;//�ٴ��������				   
			   }
		
			break;					   
			
	}

	
}






void KongZhi_guozheng_3(void)

{	
	
//	u16 ALoadCurrent,BLoadCurrent,CLoadCurrent;//���ص���	
//	u16 PresentLevel;//��ǰˮλ	
//	u16 A_current, B_current, C_current;//��ǰ����
	
	static u16 a_timer=0;//��ʱ����
	static u8  at_biao=0;//

	
/*	//CtrSend_data[0]  alarm_quex_biao1
	//CtrSend_data[1]  alarm_quex_biao2
	//CtrSend_data[2]  alarm_quex_biao3
	//CtrSend_data[3]  alarm_well_biao1
	//CtrSend_data[4]  alarm_well_biao2
	//CtrSend_data[5]  alarm_well_biao3
	//CtrSend_data[6]  alarm_pull_biao
	//CtrSend_data[7]  alarm_noelec_biao
	//CtrSend_data[8]  alarm_upelec_biao
*/	
	
	
	KongZhi3();	
	 	
	  	
	 if( relay_state3!=Brelay_state3)
		   {
		     


         if(relay_state3==0)  {AnimationPlayFrame(0,24,0); RELAY3=1;}     //ˮ��1����
		     if(relay_state3==1)  {AnimationPlayFrame(0,24,1); RELAY3=0;}     //ˮ��1ֹͣ
			   if(relay_state3==2)  {AnimationPlayFrame(0,24,2); RELAY3=0;}     //ˮ��1��ͣ
         if(relay_state3==3)  {AnimationPlayFrame(0,24,3); RELAY3=0;}     //ˮ��1ȱˮ
		     if(relay_state3==4)  {AnimationPlayFrame(0,24,4); RELAY3=0;}     //ˮ��1����
			   Brelay_state3=  relay_state3;
			   Crelay_state3=  1;	//4G���Ϳ���


		   } 

		   
	  if( valve_state3!=Bvalve_state3)
		   {		     
       if(valve_state3==0) { AnimationPlayFrame(0,22,0);   AnimationPlayFrame(0,21,0);}//���Źر�״̬
			 if(valve_state3==1) { AnimationPlayFrame(0,22,1);   AnimationPlayFrame(0,21,1);}//���ſ���״̬
			 if(valve_state3==3) { AnimationPlayFrame(0,22,3);   AnimationPlayFrame(0,21,3);}//���Źرս���			 
			 Bvalve_state3=  valve_state3;			 
			 Cvalve_state3=  1;			 
		   } 	   
			 
	switch (CourseNum3)
		
	{		
		case 1://������ת������			
		
		
			
			relay_state3=0;//����״̬	
			
				
			if(control_wait_s(&wat_3,absence_DelayT))//absence_DelayT ȱ��ȴ�ʱ��
	
				{  				
				  if((C_current>5)&&(C_current<500)) CourseNum3=3;	//û��ȱ���ж��Ƿ����
				  else CourseNum3=2;	//ȱ����߶�ת �������			
				}				
		
		
		
		break;
		
		case 2://���ȱ���ת����
		
		alarm_quex_biao3=1;//����ȱ�౨��
    relay_state3=4;//��ʾ�������
		CourseNum3=23;
		
		break;
		
		case 23://���ֹͣ����		
			relay_state3=1;//ֹͣ״̬	
		break;
		
		
		case 3://�ж��Ƿ����
		
			if(control_wait_s(&wat_3,10))		
				{				  
				  if(C_current<10)  CourseNum3=2;//ȱ����߶�ת �������	
				  if(C_current>=CLoadCurrent*2)  CourseNum3=2;//ȱ����߶�ת �������				
				  //if((A_current>=10)&&(A_current<=(ALoadCurrent/5))) CourseNum=4; //������	
					if((C_current>=10)&&(C_current<=(200))) CourseNum3=4; //������	
				  //if((A_current>(ALoadCurrent/5))&&(A_current<(ALoadCurrent*2))) CourseNum=5;//��������	
					if((C_current>(200))&&(C_current<(500))) CourseNum3=5;//��������	
				}
				
	    break;		
		
		
		case 4://������
			
		  relay_state3=2;//ˮ��2��ͣ״̬
			CourseNum3=6;  
			break;
		
		case 5://����			

			if(control_wait_s(&wat_3,2))
				 
				{   a_timer=0;//���ʱ��
					  open_Num3=0;//�����ˮ����
					if(C_current<10)  CourseNum3=101;//ȱ����߶�ת �������	
					if(C_current>=CLoadCurrent*2)  CourseNum3=101;//ȱ����߶�ת �������	
          if((C_current>=10)&&(C_current<=(200))) CourseNum3=101; //������						
					//if((A_current>=10)&&(A_current<=(ALoadCurrent/5))) CourseNum=13; //������Ӧ����ת�� ��ˮλ�ӳ�ʱ�����					
					//if((A_current>(ALoadCurrent/5))&&(A_current<(ALoadCurrent*2))) CourseNum=5;//��������
          if((C_current>(200))&&(C_current<(500))) CourseNum3=5;//��������						
				}
			
			break;
		
				
				
				
			case 101://����			

			if(control_wait_s(&wat_3,2))
				 
				{   
					if(C_current<10)  CourseNum3=2;//ȱ����߶�ת �������	
					if(C_current>=CLoadCurrent*2)  CourseNum3=2;//ȱ����߶�ת �������	
					
					if(auto_button_3==1)         //û���Զ���ͣ
						{ 					
					   if((C_current>=10)&&(C_current<=(200))) CourseNum3=11;//ȱˮ��������					
						}
					else 
					  {					
					   if((C_current>=10)&&(C_current<=(200))) CourseNum3=13; //������						
					  }
					
					
					
          				
					//if((A_current>=10)&&(A_current<=(ALoadCurrent/5))) CourseNum=13; //������Ӧ����ת�� ��ˮλ�ӳ�ʱ�����					
					//if((A_current>(ALoadCurrent/5))&&(A_current<(ALoadCurrent*2))) CourseNum=5;//��������
          if((C_current>(200))&&(C_current<(500))) CourseNum3=5;//��������						
				}
			
			break;	
				
				
				
				
				
				
				
		case 6://����ˮ
			Kai_moto5=1;//������
			Kai_moto6=1;//��ˮ
			
		    valve_state3=1;//��������־		   
		    if( control_wait_s( &wat_3,10))//��ʱ10����ֹͣ��ˮ�����������ʱ����ˮ��״̬�ǿ���״̬��
			   {		
			    Kai_moto5=0;
			    Kai_moto6=0;				   
          CourseNum3=7;				   
			   }

			break;	
		
		case 7:			
		   if(C_LEVEL==1)	CourseNum3=8;//ˮλ����ת���رշ���	
		   if( control_wait_s( &wat_3,NOwater_T))//�ȴ�ȱ��ˮʱ�䵽��ת��ȱˮ���� 
			    {		
			     CourseNum3=15;//ȱ��ˮ			
			    }		
			break;	
				
				
		case 15:	//ȱ��ˮ������ͣ���ֹͣ		
	     Guan_moto5=1;//�رշ���
		   Guan_moto6=1;//�ر�����
		  
		   valve_state3=3;//�ط����б�־
		   
		   if( control_wait_s( &wat_3,10)) //��ʱ10�� ֹͣ�رշ���
			   {		
			    Guan_moto5=0;
		      Guan_moto6=0;
				
		        valve_state3=0;//���رձ�־   
				   
            CourseNum3=9;//����ȱ��ˮ����	   
			   }
		
			break;			
				
				
		
		case 8:	//�رշ���		
		   Guan_moto5=1;//�رշ���
		   Guan_moto6=1;//�ر�����
		
		   valve_state3=3;//�ط����б�־		  
		
		   if( control_wait_s( &wat_3,10)) //��ʱ10�� ֹͣ�رշ���
			   {

			
		    valve_state3=0;//���رձ�־			   
			    Guan_moto5=0;
		      Guan_moto6=0;
          CourseNum3=10;//�ٴ��������				   
			   }
		
			break;	
		
		case 9:	//ȱ��ˮ������ͣ���ֹͣ		

		alarm_pull_biao=1;	//����������һ��	
		CourseNum3=12;		
		break;
		
	    case 10://�ٴ�����ˮ��			
		

		open_Num3++;
		
		if(open_Num3<3)  
		  {			
			CourseNum3=1;
		  }
		  
		if(open_Num3>=3) 
		  {
		   CourseNum3=11;//����ʧ��//����ˮ��ȱˮ
       }
		  
		break;		
		case 11:	//����ʧ��//����ˮ��ȱˮ
			
        alarm_well_biao3=1;	
		    relay_state3=3;	
        CourseNum3=102;		
		break;
		
		
		case 102:	//����ʧ��//����ˮ��ȱˮ
			
     relay_state3=3;//ȱˮֹͣ
		 valve_state3=0;//���رձ�־  
		break;
		
		
		case 12:	//ֹͣ״̬
		   
		  relay_state3=1;//ֹͣ״̬      
		  valve_state3=0;//���رձ�־
		
		break;
		
  case 13:	//��ͣ״̬
		   
        relay_state3=2;//��ͣ״̬
		
		
		 
		CourseNum3=22;		
		break;
   
  
  case 22://ˮ��ȱˮ����
	    alarm_well_biao3=1;
	    again_startT_biao3=1;
	   // CtrSend_data[22]	=again_startT3-(a_timer/60);//�������ݵ�APP	
	    again_startT3_test=0xffff;
      CourseNum3=14;
      break;
		
  case 14:	//��ʱ������

		
	
	
	  if(PresentLevel<=set_leve[15])                                 again_startT3=set_time[15];		
		if((PresentLevel>set_leve[15])&&(PresentLevel<=set_leve[14]))  again_startT3=set_time[14];
		if((PresentLevel>set_leve[14]) &&(PresentLevel<=set_leve[13])) again_startT3=set_time[13];
		if((PresentLevel>set_leve[13]) &&(PresentLevel<=set_leve[12])) again_startT3=set_time[12];
		if((PresentLevel>set_leve[12]) &&(PresentLevel<=set_leve[11])) again_startT3=set_time[11];
		if(PresentLevel> set_leve[11])                                 again_startT3=set_time[11];
	
	
	if(again_startT3_test!=again_startT3)
	 {
	  again_startT3_test=again_startT3; 
		CtrSend_data[22]	=again_startT3-(a_timer/60);//�������ݵ�APP		
	 }
	
	
	
		if(((TIMER_1MS%1000)>450)) 
		 {
			 if(at_biao==0)	
			 { 
			   a_timer++;	            
			   at_biao=1;			
		     SetTextValue(0,42,"Լ"); 
				 SetTextInt32(0,40,(again_startT3-(a_timer/60)),0,1);
				 SetTextValue(0,43,"�ֺ�����"); 
         SetTextInt32(0,41,a_timer%60,0,1);
				 //CtrSend_data[22]	=again_startT3-(a_timer/60);//�������ݵ�APP	

				 
			 }
		  }
		 else at_biao=0;
		
		if( control_wait_s( &wat_3,(again_startT3*60)))//�ȴ�ȱ��ˮʱ�䵽��ת��ȱˮ���� 
			{		
			  CourseNum3=1;//				
			}		
		
    break;




   case 20:	
		
		  
		   valve_state3=3;//�ط����б�־		
       Kai_moto5=0;
		   Kai_moto6=0;		
 
		   if( control_wait_s( &wat_3,2)) //��ʱ10�� ֹͣ�رշ���
			   {			
                CourseNum3=21;//�ٴ��������				   
			   }
		
			break;			


	case 21:	//�رշ���		
		   Guan_moto5=1;//�رշ���
		   Guan_moto6=1;//�ر�����		  
		   valve_state3=3;//�ط����б�־	   
		
		   if( control_wait_s( &wat_3,10)) //��ʱ10�� ֹͣ�رշ���
			   {
				
				  valve_state3=0;//���رձ�־				   
			    Guan_moto5=0;
		      Guan_moto6=0;
          CourseNum3=12;//�ٴ��������				   
			   }
		
			break;					   
			
	}


	
}



	
	
int control_wait( TROL_WAT wat,u16 timeoutval)//ִֻ��һ�Σ�timeoutvalʱ���Ժ����ִ��������������  
	

	{

		
		
		if((*wat).TIMER_1MS!=TIMER_1MS)
		   {

			   if((*wat).TimetStartFlg==1)
				
			    {
 				
				  if((TIMER_1MS>(*wat).TIMER_1MS))  (*wat).TimeOutCount=(*wat).TimeOutCount+(TIMER_1MS-(*wat).TIMER_1MS);
				  if((TIMER_1MS<(*wat).TIMER_1MS))  (*wat).TimeOutCount=(*wat).TimeOutCount+(40000-(*wat).TIMER_1MS)+TIMER_1MS; //֮������40000 ����Ϊ TIMER_1MS�ӵ�40000����
				  (*wat).TIMER_1MS=TIMER_1MS;
					
				}
			else  (*wat).TIMER_1MS=TIMER_1MS;	
		   
		   }
		
		if ((*wat).TimetStartFlg==0)
			{				
				 (*wat).TimeOutCount = 0;
				 (*wat).TimetStartFlg = 1;//������ʱȷ��ִ��1��
			}
			

		/* �����趨�Ķ�ʱʱ�� */		
		
		if((*wat).TimeOutCount>=timeoutval)
		  {
			(*wat).TimetStartFlg=0;
			(*wat).TimeOutCount = 0; 
			return 1;  					  
		  }		
		return 0;
		   
	}


	
	
	
int control_wait_s( TROL_WAT wat,u16 timeoutval)//ִֻ��һ�Σ�timeoutvalʱ���Ժ����ִ��������������  

	{
 		
		if((TIMER_1MS%1000)<50)
			
		  {
		   
			   if((*wat).TimetStartFlg==1) 
			      {
					  
					if((*wat).Timer_biao==0) 
                    {			  
					  
				     (*wat).TimeOutCount++;
					 (*wat).Timer_biao=1;
						
			         }
			      
			      }
			  
		  }
		  
		 else (*wat).Timer_biao=0; 
		
			
		
		if ((*wat).TimetStartFlg==0)
			{				
				 (*wat).TimeOutCount  = 0;
				 (*wat).TimetStartFlg = 1;//������ʱȷ��ִ��1��
			}
			

		/* �����趨�Ķ�ʱʱ�� */		
		
		if((*wat).TimeOutCount>=timeoutval)
		  {
			(*wat).TimetStartFlg=0;
			(*wat).TimeOutCount = 0; 
			return 1;  					  
		  }		
		return 0;
		   
	}
		

 
int CAT_wait_ds(Tcat_WAT wat,u16 timeoutval,u8 *biao)  //��һʱ��ִ�У�ִ������ڼӸ�ʱ��ִ��
  {
   
	  
	  if(TIMER_1MS%100<=47) 
			 
			{
				
				
				if((*wat).Timer_biao==0) 
                    {			  
					  
				     (*wat).TimeOutCount++;
					 (*wat).Timer_biao=1;
					 if((*wat).TimeOutCount>=timeoutval)	 (*wat).TimeOutCount=timeoutval;
			         }
				
			
			}
	  else 
			  
			{
			 (*wat).Timer_biao=0;  
			}

			
//��׼��ʱ�ṹ//��׼��ʱ�ṹ//��׼��ʱ�ṹ//��׼��ʱ�ṹ//��׼��ʱ�ṹ		
	
	  
		if(((*wat).TimeOutCount>=timeoutval)&&(*biao>0)&&(send_reg1==2)) 
		  {
			
			(*wat).TimeOutCount = 0; 
			return 1;  					  
		  }		
		return 0;
  
 }
 

	
	
	

void Control_Word_pack(void)
	
{
	
//	u8 Kai_moto1=0,Guan_moto1=0;
//	u8 Kai_moto2=0,Guan_moto2=0;
//	u8 Kai_moto3=0,Guan_moto3=0;
//	u8 Kai_moto4=0,Guan_moto4=0;
//	u8 Kai_moto5=0,Guan_moto5=0;
//	u8 Kai_moto6=0,Guan_moto6=0;
	
	
	if(Kai_moto1==1)  win_control_Word|=0x0001;
	if(Guan_moto1==1) win_control_Word|=0x0002;
	if(Kai_moto2==1)  win_control_Word|=0x0004;
	if(Guan_moto2==1) win_control_Word|=0x0008;
	if(Kai_moto3==1)  win_control_Word|=0x0010;
	if(Guan_moto3==1) win_control_Word|=0x0020;
	if(Kai_moto4==1)  win_control_Word|=0x0040;
	if(Guan_moto4==1) win_control_Word|=0x0080;

  if(Kai_moto5==1)  win_control_Word|=0x0100;
	if(Guan_moto5==1) win_control_Word|=0x0200;
	if(Kai_moto6==1)  win_control_Word|=0x0400;
	if(Guan_moto6==1) win_control_Word|=0x0800;
    
	
	if(Kai_moto1==0)  win_control_Word&=~(0x01<<0);
	if(Guan_moto1==0) win_control_Word&=~(0x01<<1);
	if(Kai_moto2==0)  win_control_Word&=~(0x01<<2);
	if(Guan_moto2==0) win_control_Word&=~(0x01<<3);
	if(Kai_moto3==0)  win_control_Word&=~(0x01<<4);
	if(Guan_moto3==0) win_control_Word&=~(0x01<<5);
	if(Kai_moto4==0)  win_control_Word&=~(0x01<<6);
	if(Guan_moto4==0) win_control_Word&=~(0x01<<7);
	
	if(Kai_moto5==0)  win_control_Word&=~(0x01<<8);
	if(Guan_moto5==0) win_control_Word&=~(0x01<<9);
	if(Kai_moto6==0)  win_control_Word&=~(0x01<<10);
	if(Guan_moto6==0) win_control_Word&=~(0x01<<11);
	
  
}




void Win_Word_parse_mast(void)
	
  {
 
  if(Win_Word_receive&0x1000) A_LEVEL=1;
	else A_LEVEL=0;
	if(Win_Word_receive&0x2000) B_LEVEL=1;
	else B_LEVEL=0;
	if(Win_Word_receive&0x4000) C_LEVEL=1;
	else C_LEVEL =0;

  }
  

	
/*
	ͨ����CAT�յ������ݱ�����ʼ���������ͷ��ͱ�����ֵ��һ������һ���ϴ�����
*/	
	
	
	
void zhuangtai_chushihua(void)	
{

  TCtrSend_data[9]=0xffff;
  TCtrSend_data[10]=0xffff;
	TCtrSend_data[11]=0xffff;
	TCtrSend_data[12]=0xffff;
	TCtrSend_data[13]=0xffff;
	TCtrSend_data[14]=0xffff;
	

	
}	
	
	

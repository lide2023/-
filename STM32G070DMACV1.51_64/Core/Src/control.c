
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
	
   
  u16 auto_button_1,auto_button_2,auto_button_3;//自动运行选择
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
	
	u16 ALoadCurrent,BLoadCurrent,CLoadCurrent;//带载电流

	u16 A_current, B_current, C_current;//当前电流
	u16 PresentLevel;//当前水位
	u16 PresentLevel_temp;//发送数据用
	u8 A_current_biao, B_current_biao, C_current_biao;//上传标志当前电流
	u8 PresentLevel_biao; //当前水位上传标志
	
	
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
	
	
	
	uint16_t VolDta1,VolDta2,VolDta3,VolDta4,VolDta0;//参考电压值，ADC1_Ch4采样电压值，单位V

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

 u8 send_str_biao=1;//开机发送存储数据标志

u8 CAT1_Send_Data_dif_biao=0;



 u8 send_mem_nember=0; 

 u8 relay_state1=1;
 u8 relay_state2=1;
 u8 relay_state3=1;
 
 u8 Brelay_state1=5;
 u8 Brelay_state2=100;
 u8 Brelay_state3=100;
 
 u8 Crelay_state1=0;//4G发送开关
 u8 Crelay_state2=0;//4G发送开关
 u8 Crelay_state3=0;//4G发送开关
 
 
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
			 storage_1[23+j]= set_time[j];//最后一个是38
		 
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
    storage_1[39]=auto_button_1;      水泵1自动开关
	  storage_1[40]=auto_button_2;      水泵2自动开关
    storage_1[41]=auto_button_3;	    水泵3自动开关             A55A10290001
	  storage_1[42]=biaoding_shuiwei;	  标定水位                  A55A102a0001
    storage_1[43]=shuiwei_xinhao;	    水位信号来源							A55A102b0001 发送   A55A102b0000接收  
	  storage_1[44]=lock_biao;	        锁机命令							    A55A102c0001  

V2.0	  storage_1[45]=kongzhi_fangshi;	  控制方式手动自动转换			A55A102d0000 手动
	                                                                  A55A102d0001 自动
V2.0	  storage_1[46]=protection_switch_1;   保护开关1                  A55A102e0001 开
                                                                     A55A102e0000 关
V2.0	  storage_1[47]=protection_switch_2;   保护开关2                  A55A102f0001 开
                                                                    A55A102f0000 关
V2.0    storage_1[48]=protection_switch_3;   保护开关3                  A55A10300001 开
                                                                    A55A10300000 关
V2.0    storage_1[49]=valve_travel_time;   阀门行程时间             A55A1031xxxx 	

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
		  set_time[j]=storage_1[23+j];//最后一个是38		 
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

	Control_data[0]=cat_open1;//手动状态反馈给APP
	Control_data[1]=cat_stop1;//手动状态反馈给APP

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

	Control_data[2]=cat_open2;//手动状态反馈给APP
	Control_data[3]=cat_stop2;//手动状态反馈给APP

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

	Control_data[4]=cat_open3;//手动状态反馈给APP
	Control_data[5]=cat_stop3;//手动状态反馈给APP

}



void Hand_KongZhi1(void)	//阀1开关和cat.1发送指令
	
{	

 if((valve_stop1==1)&&(valve_open1==0)) 
	  {	   
       valve_stop1=0;
       hand_CourseNum=4;//进入关闭阀门线程	 
	  }
	 
  if((valve_stop1==0)&&(valve_open1==1))   
   {	  
	  valve_open1=0;
	  hand_CourseNum=1;	//进入开启阀门线程	
	 }
	 
	cat_valve_open1=Control_data[10];
	cat_valve_stop1=Control_data[11];	 

		 
	 if((cat_valve_stop1==1)&&(cat_valve_open1==0)) 
		 {	   
		   cat_valve_stop1=0;
		   hand_CourseNum=4;//进入关闭阀门线程	 
		 }


	 if((cat_valve_stop1==0)&&(cat_valve_open1==1))   
		 {	  
		   cat_valve_open1=0;     
		   hand_CourseNum=1;	//进入开启阀门线程		 
		 }

	Control_data[10]=cat_valve_open1;//手动状态反馈给APP
	Control_data[11]=cat_valve_open1;//手动状态反馈给APP
		 

		 
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

	Control_data[0]=cat_open1;//手动状态反馈给APP
	Control_data[1]=cat_stop1;//手动状态反馈给APP	 
 
		 
}



void Hand_KongZhi2(void)	//阀门2开关和cat.1发送指令
	
{	

 if((valve_stop2==1)&&(valve_open2==0)) 
	 {	   
       valve_stop2=0;
       hand_CourseNum2=4;//进入关闭阀门线程	 		 
	 }
	 
  if((valve_stop2==0)&&(valve_open2==1))   
     {	  
	  valve_open2=0;
	   hand_CourseNum2=1;	//进入开启阀门线程		
	 }
	 
	cat_valve_open2=Control_data[12];
	cat_valve_stop2=Control_data[13];	 

		 
	 if((cat_valve_stop2==1)&&(cat_valve_open2==0)) 
		 {	   
		   cat_valve_stop2=0;
		   hand_CourseNum2=4;//进入关闭阀门线程	 		 
		 }


	 if((cat_valve_stop2==0)&&(cat_valve_open2==1))   
		 {	  
		   cat_valve_open2=0;     
		   hand_CourseNum2=1;	//进入开启阀门线程  202212月24号修改			 
		 }

	Control_data[12]=cat_valve_open2;//手动状态反馈给APP
	Control_data[13]=cat_valve_stop2;//手动状态反馈给APP

		 
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

	Control_data[2]=cat_open2;//手动状态反馈给APP
	Control_data[3]=cat_stop2;//手动状态反馈给APP	 	 

		 
}


void Hand_KongZhi3(void)	//阀门3开关和cat.1发送指令
	
{	

 if((valve_stop3==1)&&(valve_open3==0)) 
	 {	   
       valve_stop3=0;
      hand_CourseNum3=4;//进入关闭阀门线程	 	 		 
	 }
	 
  if((valve_stop3==0)&&(valve_open3==1))   
     {	  
	  valve_open3=0;
	  hand_CourseNum3=1;	//进入开启阀门线程				
	 }
	 
	cat_valve_open3=Control_data[14];
	cat_valve_stop3=Control_data[15];	 

		 
	 if((cat_valve_stop3==1)&&(cat_valve_open3==0)) 
		 {	   
		   cat_valve_stop3=0;
		    hand_CourseNum3=4;//进入关闭阀门线程	 		 		 
		 }

	 if((cat_valve_stop3==0)&&(cat_valve_open3==1))   
		 {	  
		   cat_valve_open3=0;     
		   hand_CourseNum3=1;	//进入开启阀门线程			 
		 }

	Control_data[14]=cat_valve_open3;//手动状态反馈给APP
	Control_data[15]=cat_valve_stop3;//手动状态反馈给APP
		 
		 
		 
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

	Control_data[4]=cat_open3;//手动状态反馈给APP
	Control_data[5]=cat_stop3;//手动状态反馈给APP	 	 	 
	 

}


void KongZhi_guozheng1_hand(void)
{
  Hand_KongZhi1();
 
	if( relay_state1!=Brelay_state1)//状态发生变化则执行
		   {
		     if(relay_state1==0)  {AnimationPlayFrame(0,17,0); RELAY1=1;}     //水泵1启动
		     if(relay_state1==1)  {AnimationPlayFrame(0,17,1); RELAY1=0;}     //水泵1停止
			   if(relay_state1==2)  {AnimationPlayFrame(0,17,2); RELAY1=0;}     //水泵1暂停
         if(relay_state1==3)  {AnimationPlayFrame(0,17,3); RELAY1=0;}     //水泵1缺水
		     if(relay_state1==4)  {AnimationPlayFrame(0,17,4); RELAY1=0;}     //水泵1故障
	
			   Brelay_state1=  relay_state1;
			   Crelay_state1=  1;	//4G发送开关
		   } 
	   
		   
	  if( valve_state1!=Bvalve_state1)
		   {
		     
         if(valve_state1==0) { AnimationPlayFrame(0,14,0);   AnimationPlayFrame(0,15,0);}//阀门关闭状态
			   if(valve_state1==1) { AnimationPlayFrame(0,14,1);   AnimationPlayFrame(0,15,1);}//阀门开启状态
			   if(valve_state1==3) { AnimationPlayFrame(0,14,3);   AnimationPlayFrame(0,15,3);}//阀门关闭进程			 
			   Bvalve_state1=  valve_state1;			 
			   Cvalve_state1=  1;
			 
		   }

 if(protection_switch_1==0)   hand_no_protection(&CourseNum,&relay_state1);//手动没有保护
 if(protection_switch_1==1)    hand_have_protection_1();//手动有保护	 
	  valve_hand(&hand_CourseNum,&valve_state1,&wat_4 );	
		
	//hand_have_protection_1();//手动有保护		 
			 
			 
}





void KongZhi_guozheng2_hand(void)
{
  Hand_KongZhi2();
 
	if( relay_state2!=Brelay_state2)//状态发生变化则执行
		   {
		     if(relay_state2==0)  {AnimationPlayFrame(0,25,0); RELAY2=1;}     //水泵1启动
		     if(relay_state2==1)  {AnimationPlayFrame(0,25,1); RELAY2=0;}     //水泵1停止
			   if(relay_state2==2)  {AnimationPlayFrame(0,25,2); RELAY2=0;}     //水泵1暂停
         if(relay_state2==3)  {AnimationPlayFrame(0,25,3); RELAY2=0;}     //水泵1缺水
		     if(relay_state2==4)  {AnimationPlayFrame(0,25,4); RELAY2=0;}     //水泵1故障
	
			   Brelay_state2=  relay_state2;
			   Crelay_state2=  1;	//4G发送开关
		   } 
	   
		   
	  if( valve_state2!=Bvalve_state2)
		   {
		     
         if(valve_state2==0) { AnimationPlayFrame(0,20,0);   AnimationPlayFrame(0,23,0);}//阀门关闭状态
			   if(valve_state2==1) { AnimationPlayFrame(0,20,1);   AnimationPlayFrame(0,23,1);}//阀门开启状态
			   if(valve_state2==3) { AnimationPlayFrame(0,20,3);   AnimationPlayFrame(0,23,3);}//阀门关闭进程			 
			   Bvalve_state2=  valve_state2;			 
			   Cvalve_state2=  1;
			 
		   }

 if(protection_switch_2==0)    hand_no_protection(&CourseNum2,&relay_state2);//手动没有保护
 if(protection_switch_2==1)    hand_have_protection_2();//手动有保护	 
	  valve_hand(&hand_CourseNum2,&valve_state2,&wat_6 );	
		
	
			 
			 
}



void KongZhi_guozheng3_hand(void)
{
  Hand_KongZhi3();
 
	if( relay_state3!=Brelay_state3)//状态发生变化则执行
		   {
		     if(relay_state3==0)  {AnimationPlayFrame(0,24,0); RELAY3=1;}     //水泵1启动
		     if(relay_state3==1)  {AnimationPlayFrame(0,24,1); RELAY3=0;}     //水泵1停止
			   if(relay_state3==2)  {AnimationPlayFrame(0,24,2); RELAY3=0;}     //水泵1暂停
         if(relay_state3==3)  {AnimationPlayFrame(0,24,3); RELAY3=0;}     //水泵1缺水
		     if(relay_state3==4)  {AnimationPlayFrame(0,24,4); RELAY3=0;}     //水泵1故障
	
			   Brelay_state3=  relay_state3;
			   Crelay_state3=  1;	//4G发送开关
		   } 
	   
		   
	  if( valve_state3!=Bvalve_state3)
		   {
		     
         if(valve_state3==0) { AnimationPlayFrame(0,22,0);   AnimationPlayFrame(0,21,0);}//阀门关闭状态
			   if(valve_state3==1) { AnimationPlayFrame(0,22,1);   AnimationPlayFrame(0,21,1);}//阀门开启状态
			   if(valve_state3==3) { AnimationPlayFrame(0,22,3);   AnimationPlayFrame(0,21,3);}//阀门关闭进程			 
			   Bvalve_state3=  valve_state3;			 
			   Cvalve_state3=  1;
			 
		   }

 if(protection_switch_3==0)   hand_no_protection(&CourseNum3,&relay_state3);//手动没有保护
 if(protection_switch_3==1)    hand_have_protection_3();//手动有保护	 
	  valve_hand(&hand_CourseNum3,&valve_state3,&wat_7 );	
		
	//hand_have_protection_1();//手动有保护		 
			 
			 
}



/*

手动开关阀门函数
这个进程是和水泵的进程分开的

*/



void valve_hand(u8 *hand_CourseNumx,u8 *valve_statex,TROL_WAT wat_x )

  {
	
	switch (*hand_CourseNumx)
		
	  {			 
		 
			case 1://开启阀门				
			//valve_state1=1;//阀开启标志
		  *valve_statex=3;//关阀进行标志
		  //valve_state1=0;//阀关闭标志
			*hand_CourseNumx=2;		
      break;
				
		  case 2://开启过程		
			
			if(control_wait_s(wat_x,valve_travel_time))
				{
				  *hand_CourseNumx=3;						
				}
			break;	
		
		  case 3://开启完成
	 
		  *valve_statex=1;//阀开启标志
		  // valve_state1=3;//关阀进行标志
		  //valve_state1=0;//阀关闭标志
			break;

    	case 4://关闭阀门
				
			//valve_state1=1;//阀开启标志
		  *valve_statex=3;//关阀进行标志
		  //valve_state1=0;//阀关闭标志
			*hand_CourseNumx=5;		
      break;
				
		  case 5://关闭过程
			
			if(control_wait_s(wat_x,valve_travel_time))
				{
				  *hand_CourseNumx=6;	
				}
			break;				
		  case 6://关闭完成
	 
		  //valve_state1=1;//阀开启标志
		  //valve_state1=3;//关阀进行标志
		  *valve_statex=0;//阀关闭标志
			break;
			
		}			
	
	}

	

void hand_no_protection(u8 *hand_CourseNum,u8 *relay_state1)
{	 
		switch (*hand_CourseNum)
		
	  {	
	
	    case 1://开启水泵
	
			*relay_state1=0;//水泵启动
			//relay_state1=1;//水泵停止
			//relay_state1=2;//水泵暂停
	
			break;
	 
	    case 7://停止水泵
	 
			
			//relay_state1=0;//水泵启动
			*relay_state1=1;//水泵停止
			//relay_state1=2;//水泵暂停
	
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
	
	    case 1://开启水泵	
			relay_state1=0;//水泵启动
			//relay_state1=1;//水泵停止
			//relay_state1=2;//水泵暂停
			//relay_state1=3;//水泵缺水
      //relay_state1=4;//水泵故障				
			
			if(control_wait_s(&wat_1,absence_DelayT))
	
				{  				
				  if((A_current>5)&&(A_current<500)) CourseNum=3;	//没有缺项判断是否带载
				  else CourseNum=2;	//缺项或者堵转 电机故障			
				}				
			break;
			case 2://电机缺相堵转故障
 		
		   alarm_quex_biao1=1;//开启缺相报警

		   CourseNum=102;
		
		   break;
		
		   case 102://电机故障停止运行		
       relay_state1=4;			 
		   break;
			
			case 3://判断是否带载
		
			if(control_wait_s(&wat_1,10))		
				{				  
				  if(A_current<10)  CourseNum=2;//缺项或者堵转 电机故障	
				  if(A_current>500)  CourseNum=2;//缺项或者堵转 电机故障	
					
					//if(A_current>=ALoadCurrent*2)  CourseNum=2;//缺项或者堵转 电机故障				
				  //if((A_current>=10)&&(A_current<=(ALoadCurrent/5))) CourseNum=4; //不带载	
					if((A_current>=10)&&(A_current<=(200))) CourseNum=4; //不带载	
				  //if((A_current>(ALoadCurrent/5))&&(A_current<(ALoadCurrent*2))) CourseNum=5;//带载正常	
					if((A_current>(200))&&(A_current<(500))) CourseNum=5;//带载正常	
				}
				
	    break;			
				
		case 4://不带载		
		
    alarm_well_biao1=1;	
    CourseNum=101;		
		break;

		case 101://不带载		
		relay_state1=3;//水泵1缺水停止	
    
		break;
		
		
		case 5://带载			

			if(control_wait_s(&wat_1,1))
				 
				{  
					
					if(A_current<10)  CourseNum=6;//缺项或者堵转 电机故障	
					if(A_current>500)  CourseNum=6;//缺项或者堵转 电机故障	
					//if(A_current>=ALoadCurrent*2)  CourseNum=6;//缺项或者堵转 电机故障	
          if((A_current>=10)&&(A_current<=(200))) CourseNum=6; //不带载						
					//if((A_current>=10)&&(A_current<=(ALoadCurrent/5))) CourseNum=13; //不带载应该跳转到 按水位延长时间控制					
					//if((A_current>(ALoadCurrent/5))&&(A_current<(ALoadCurrent*2))) CourseNum=5;//带载正常
           if((A_current>(200))&&(A_current<(500))) CourseNum=5;//带载正常						
				}
			
			break;
				
      case 6://去抖

			if(control_wait_s(&wat_1,2))
				 
				{ 					
					if(A_current<10)  CourseNum=2;//缺项或者堵转 电机故障	
					if(A_current>500)  CourseNum=2;//缺项或者堵转 电机故障	
					//if(A_current>=ALoadCurrent*2)  CourseNum=2;//缺项或者堵转 电机故障	
          if((A_current>=10)&&(A_current<=(200))) CourseNum=4; //不带载						
					//if((A_current>=10)&&(A_current<=(ALoadCurrent/5))) CourseNum=13; //不带载应该跳转到 按水位延长时间控制					
					//if((A_current>(ALoadCurrent/5))&&(A_current<(ALoadCurrent*2))) CourseNum=5;//带载正常
           if((A_current>(200))&&(A_current<(500))) CourseNum=5;//带载正常						
				}
			
			break;
				
		case 7://停止
		relay_state1=1;//水泵停止
		break;		

				
	}	 
	
 }

void hand_have_protection_2(void)  
{	 
		switch (CourseNum2)
		
	  {	
	
	    case 1://开启水泵	
			relay_state2=0;//水泵启动
			//relay_state1=1;//水泵停止
			//relay_state1=2;//水泵暂停
			//relay_state1=3;//水泵缺水
      //relay_state1=4;//水泵故障				
			
			if(control_wait_s(&wat_2,absence_DelayT))
	
				{  				
				  if((A_current>5)&&(A_current<500)) CourseNum2=3;	//没有缺项判断是否带载
				  else CourseNum2=2;	//缺项或者堵转 电机故障			
				}				
			break;
			case 2://电机缺相堵转故障
 		
		   alarm_quex_biao2=1;//开启缺相报警

		   CourseNum2=102;
		
		   break;
		
		   case 102://电机故障停止运行		
       relay_state2=4;			 
		   break;
			
			case 3://判断是否带载
		
			if(control_wait_s(&wat_2,10))		
				{				  
				  if(A_current<10)  CourseNum2=2;//缺项或者堵转 电机故障	
				  if(A_current>500)  CourseNum2=2;//缺项或者堵转 电机故障	
					
					//if(A_current>=ALoadCurrent*2)  CourseNum=2;//缺项或者堵转 电机故障				
				  //if((A_current>=10)&&(A_current<=(ALoadCurrent/5))) CourseNum=4; //不带载	
					if((A_current>=10)&&(A_current<=(200))) CourseNum2=4; //不带载	
				  //if((A_current>(ALoadCurrent/5))&&(A_current<(ALoadCurrent*2))) CourseNum=5;//带载正常	
					if((A_current>(200))&&(A_current<(500))) CourseNum2=5;//带载正常	
				}
				
	    break;			
				
		case 4://不带载		
		
    alarm_well_biao2=1;	
    CourseNum2=101;		
		break;

		case 101://不带载		
		relay_state2=3;//水泵1缺水停止	
    
		break;
		
		
		case 5://带载			

			if(control_wait_s(&wat_2,1))
				 
				{  
					
					if(A_current<10)  CourseNum2=6;//缺项或者堵转 电机故障	
					if(A_current>500)  CourseNum2=6;//缺项或者堵转 电机故障	
					//if(A_current>=ALoadCurrent*2)  CourseNum=6;//缺项或者堵转 电机故障	
          if((A_current>=10)&&(A_current<=(200))) CourseNum2=6; //不带载						
					//if((A_current>=10)&&(A_current<=(ALoadCurrent/5))) CourseNum=13; //不带载应该跳转到 按水位延长时间控制					
					//if((A_current>(ALoadCurrent/5))&&(A_current<(ALoadCurrent*2))) CourseNum=5;//带载正常
           if((A_current>(200))&&(A_current<(500))) CourseNum2=5;//带载正常						
				}
			
			break;
				
      case 6://去抖

			if(control_wait_s(&wat_2,2))
				 
				{ 					
					if(A_current<10)  CourseNum2=2;//缺项或者堵转 电机故障	
					if(A_current>500)  CourseNum2=2;//缺项或者堵转 电机故障	
					//if(A_current>=ALoadCurrent*2)  CourseNum=2;//缺项或者堵转 电机故障	
          if((A_current>=10)&&(A_current<=(200))) CourseNum2=4; //不带载						
					//if((A_current>=10)&&(A_current<=(ALoadCurrent/5))) CourseNum=13; //不带载应该跳转到 按水位延长时间控制					
					//if((A_current>(ALoadCurrent/5))&&(A_current<(ALoadCurrent*2))) CourseNum=5;//带载正常
           if((A_current>(200))&&(A_current<(500))) CourseNum2=5;//带载正常						
				}
			
			break;
				
		case 7://停止
		relay_state2=1;//水泵停止
		break;		

				
	}	 
	
 }

 
 
 
 void hand_have_protection_3(void) 
{	 
		switch (CourseNum3)
		
	  {	
	
	    case 1://开启水泵	
			relay_state3=0;//水泵启动
			//relay_state1=1;//水泵停止
			//relay_state1=2;//水泵暂停
			//relay_state1=3;//水泵缺水
      //relay_state1=4;//水泵故障				
			
			if(control_wait_s(&wat_3,absence_DelayT))
	
				{  				
				  if((A_current>5)&&(A_current<500)) CourseNum3=3;	//没有缺项判断是否带载
				  else CourseNum3=2;	//缺项或者堵转 电机故障			
				}				
			break;
			case 2://电机缺相堵转故障
 		
		   alarm_quex_biao3=1;//开启缺相报警

		   CourseNum3=102;
		
		   break;
		
		   case 102://电机故障停止运行		
       relay_state3=4;			 
		   break;
			
			case 3://判断是否带载
		
			if(control_wait_s(&wat_3,10))		
				{				  
				  if(A_current<10)  CourseNum3=2;//缺项或者堵转 电机故障	
				  if(A_current>500)  CourseNum3=2;//缺项或者堵转 电机故障	
					
					//if(A_current>=ALoadCurrent*2)  CourseNum=2;//缺项或者堵转 电机故障				
				  //if((A_current>=10)&&(A_current<=(ALoadCurrent/5))) CourseNum=4; //不带载	
					if((A_current>=10)&&(A_current<=(200))) CourseNum3=4; //不带载	
				  //if((A_current>(ALoadCurrent/5))&&(A_current<(ALoadCurrent*2))) CourseNum=5;//带载正常	
					if((A_current>(200))&&(A_current<(500))) CourseNum3=5;//带载正常	
				}
				
	    break;			
				
		case 4://不带载		
		
    alarm_well_biao3=1;	
    CourseNum3=101;		
		break;

		case 101://不带载		
		relay_state3=3;//水泵1缺水停止	
    
		break;
		
		
		case 5://带载			

			if(control_wait_s(&wat_3,1))
				 
				{  
					
					if(A_current<10)  CourseNum3=6;//缺项或者堵转 电机故障	
					if(A_current>500)  CourseNum3=6;//缺项或者堵转 电机故障	
					//if(A_current>=ALoadCurrent*2)  CourseNum=6;//缺项或者堵转 电机故障	
          if((A_current>=10)&&(A_current<=(200))) CourseNum3=6; //不带载						
					//if((A_current>=10)&&(A_current<=(ALoadCurrent/5))) CourseNum=13; //不带载应该跳转到 按水位延长时间控制					
					//if((A_current>(ALoadCurrent/5))&&(A_current<(ALoadCurrent*2))) CourseNum=5;//带载正常
           if((A_current>(200))&&(A_current<(500))) CourseNum3=5;//带载正常						
				}
			
			break;
				
      case 6://去抖

			if(control_wait_s(&wat_3,2))
				 
				{ 					
					if(A_current<10)  CourseNum3=2;//缺项或者堵转 电机故障	
					if(A_current>500)  CourseNum3=2;//缺项或者堵转 电机故障	
					//if(A_current>=ALoadCurrent*2)  CourseNum=2;//缺项或者堵转 电机故障	
          if((A_current>=10)&&(A_current<=(200))) CourseNum3=4; //不带载						
					//if((A_current>=10)&&(A_current<=(ALoadCurrent/5))) CourseNum=13; //不带载应该跳转到 按水位延长时间控制					
					//if((A_current>(ALoadCurrent/5))&&(A_current<(ALoadCurrent*2))) CourseNum=5;//带载正常
           if((A_current>(200))&&(A_current<(500))) CourseNum3=5;//带载正常						
				}
			
			break;
				
		case 7://停止
		relay_state3=1;//水泵停止
		break;		

				
	}	 
	
 }


 
void KongZhi_guozheng_1(void)

{	
	
//	u16 ALoadCurrent,BLoadCurrent,CLoadCurrent;//带载电流	
//	u16 PresentLevel;//当前水位	
//	u16 A_current, B_current, C_current;//当前电流
	
	static u16 a_timer=0;//延时启动
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

if( relay_state1!=Brelay_state1)//状态发生变化则执行
		   {
		     if(relay_state1==0)  {AnimationPlayFrame(0,17,0); RELAY1=1;}     //水泵1启动
		     if(relay_state1==1)  {AnimationPlayFrame(0,17,1); RELAY1=0;}     //水泵1停止
			   if(relay_state1==2)  {AnimationPlayFrame(0,17,2); RELAY1=0;}     //水泵1暂停
         if(relay_state1==3)  {AnimationPlayFrame(0,17,3); RELAY1=0;}     //水泵1缺水
		     if(relay_state1==4)  {AnimationPlayFrame(0,17,4); RELAY1=0;}     //水泵1故障
			   Brelay_state1=  relay_state1;
			   Crelay_state1=  1;	//4G发送开关
		   } 
		   
		   
	  if( valve_state1!=Bvalve_state1)
		   {
		     
         if(valve_state1==0) { AnimationPlayFrame(0,14,0);   AnimationPlayFrame(0,15,0);}//阀门关闭状态
			   if(valve_state1==1) { AnimationPlayFrame(0,14,1);   AnimationPlayFrame(0,15,1);}//阀门开启状态
			   if(valve_state1==3) { AnimationPlayFrame(0,14,3);   AnimationPlayFrame(0,15,3);}//阀门关闭进程			 
			   Bvalve_state1=  valve_state1;			 
			   Cvalve_state1=  1;
			 
		   } 	   


			 
 switch (CourseNum)
		
	{		
		case 1://按水泵启动跳转到这里			
		
			relay_state1=0;//启动水泵状态					
			if(control_wait_s(&wat_1,absence_DelayT))
	
				{  				
				  if((A_current>5)&&(A_current<500)) CourseNum=3;	//没有缺项判断是否带载
				  else CourseNum=2;	//缺项或者堵转 电机故障			
				}				
		
		break;
		
		case 2://电机缺相堵转故障
		
		alarm_quex_biao1=1;//开启缺相报警
    relay_state1=4;
		CourseNum=23;
		
		break;
		
		case 23://电机停止运行
		RELAY1=0;
    AnimationPlayFrame(0,17,1);//停止运行
		relay_state1=1;//停止状态	
		break;
		
		
		case 3://判断是否带载
		
			if(control_wait_s(&wat_1,10))		
				{				  
				  if(A_current<10)  CourseNum=2;//缺项或者堵转 电机故障	
				  if(A_current>=ALoadCurrent*2)  CourseNum=2;//缺项或者堵转 电机故障				
				  //if((A_current>=10)&&(A_current<=(ALoadCurrent/5))) CourseNum=4; //不带载	
					if((A_current>=10)&&(A_current<=(200))) CourseNum=4; //不带载	
				  //if((A_current>(ALoadCurrent/5))&&(A_current<(ALoadCurrent*2))) CourseNum=5;//带载正常	
					if((A_current>(200))&&(A_current<(500))) CourseNum=5;//带载正常	
				}
				
	    break;		
		
		
		case 4://不带载
		
		  relay_state1=2;//水泵1暂停状态
			CourseNum=6;  
			break;
		
		case 5://带载			

			if(control_wait_s(&wat_1,2))
				 
				{ 
					
					a_timer=0;//清除时间
					open_Num1=0;//清除进水次数
					if(A_current<10)  CourseNum=101;//缺项或者堵转 电机故障	
					if(A_current>=ALoadCurrent*2)  CourseNum=101;//缺项或者堵转 电机故障	
          if((A_current>=10)&&(A_current<=(200))) CourseNum=101; //不带载						
					//if((A_current>=10)&&(A_current<=(ALoadCurrent/5))) CourseNum=13; //不带载应该跳转到 按水位延长时间控制					
					//if((A_current>(ALoadCurrent/5))&&(A_current<(ALoadCurrent*2))) CourseNum=5;//带载正常
          if((A_current>(200))&&(A_current<(500))) CourseNum=5;//带载正常
					
				}
			
			break;
		
				
				
			case 101://带载			

			if(control_wait_s(&wat_1,2))
				 
				{ 					
					if(A_current<10)  CourseNum=2;//缺项或者堵转 电机故障	
					if(A_current>=ALoadCurrent*2)  CourseNum=2;//缺项或者堵转 电机故障	          
          if(auto_button_1==1)         //没有自动启停
						{ 					
					   if((A_current>=10)&&(A_current<=(200))) CourseNum=11; //缺引水报警节点  					
						}
					else 
					  {					
					   if((A_current>=10)&&(A_current<=(200))) CourseNum=13; //不带载						
					  }
					
					//if((A_current>=10)&&(A_current<=(ALoadCurrent/5))) CourseNum=13; //不带载应该跳转到 按水位延长时间控制					
					//if((A_current>(ALoadCurrent/5))&&(A_current<(ALoadCurrent*2))) CourseNum=5;//带载正常
          if((A_current>(200))&&(A_current<(500))) CourseNum=5;//带载正常
					
				}
			
			break;		
				
			
				
		case 6://加引水
			Kai_moto1=1;//放气开
			Kai_moto2=1;//加水
			
		    valve_state1=1;//阀开启标志
		   
		   
		    if( control_wait_s( &wat_1,10))//延时10秒钟停止加水放气输出，此时气阀水阀状态是开启状态。
			   {		
			    Kai_moto1=0;
			    Kai_moto2=0;				   
          CourseNum=7;				   
			   }

			break;	
		
		case 7:			
		   if(A_LEVEL==1)	CourseNum=8;//水位到跳转到关闭阀门	
		   if( control_wait_s( &wat_1,NOwater_T))//等待缺引水时间到跳转到缺水报警 
			    {		
			     CourseNum=15;//缺引水			
			    }		
			break;	
				
				
		case 15:	//缺饮水代码暂停变成停止		
	     Guan_moto1=1;//关闭阀门
		   Guan_moto2=1;//关闭气阀		   
		   valve_state1=3;//关阀进行标志
		   
		   if( control_wait_s( &wat_1,10)) //延时10秒 停止关闭阀门
			   {		
			    Guan_moto1=0;
		      Guan_moto2=0;				 
		      valve_state1=0;//阀关闭标志 				   
          CourseNum=9;//发送缺引水代码	   
			   }
		
			break;			
				
				
		
		case 8:	//关闭阀门		
		   Guan_moto1=1;//关闭阀门
		   Guan_moto2=1;//关闭气阀		
		   valve_state1=3;//关阀进行标志		
		   if( control_wait_s( &wat_1,10)) //延时10秒 停止关闭阀门
			   {

					valve_state1=0;//阀关闭标志
				   
			    Guan_moto1=0;
		      Guan_moto2=0;
          CourseNum=10;//再次启动电机	
					 
			   }
		
			break;	
		
		case 9:	//缺饮水代码暂停变成停止		

		  alarm_pull_biao=1;		
		  CourseNum=12;		
		  break;
		
	 case 10://再次启动水泵			
		

		open_Num1++;
		
		if(open_Num1<3)  
		  {			
			CourseNum=1;
		  }
		  
		if(open_Num1>=3) 
		  {
		   CourseNum=11;//启动失败//发送水井缺水
          }
		  
		break;		
		case 11:	//启动失败//发送水井缺水
			
        alarm_well_biao1=1;
		    relay_state1=3;
        CourseNum=102;		
		break;
		
		
		case 102:	//启动失败//发送水井缺水
			
     relay_state1=3;//缺水停止
		 valve_state1=0;//阀关闭标志  
		break;
		
		
		
		case 12:	//按下停止键进入这个节点
		
		relay_state1=1;//停止状态		
		valve_state1=0;//阀关闭标志
		
		break;
		
  case 13:	//暂停状态
		
    relay_state1=2;//暂停状态
	
		CourseNum=22;		
		break;
   
  
  case 22://水井缺水报警
	  alarm_well_biao1=1;
	  again_startT_biao1=1;
	  //CtrSend_data[20]	=again_startT1-(a_timer/60);//发送数据到APP		
	
	
	  again_startT1_test=0xffff;
	
    CourseNum=14;
      break;
		
  case 14:	//延时再启动

	
	
	  if(PresentLevel<=set_leve[5]) again_startT1=set_time[5];		
		if((PresentLevel>set_leve[5])&&(PresentLevel<=set_leve[4])) again_startT1=set_time[4];
		if((PresentLevel>set_leve[4])&&(PresentLevel<=set_leve[3])) again_startT1=set_time[3];
		if((PresentLevel>set_leve[3])&&(PresentLevel<=set_leve[2])) again_startT1=set_time[2];
		if((PresentLevel>set_leve[2])&&(PresentLevel<=set_leve[1])) again_startT1=set_time[1];
		if(PresentLevel>set_leve[1]) again_startT1=set_time[1];
	
	
	
	
	if(again_startT1_test!=again_startT1)
	 {
	  again_startT1_test=again_startT1; 
		CtrSend_data[20]	=again_startT1-(a_timer/60);//发送数据到APP		
	 }
	
	
		if(((TIMER_1MS%1000)>450)) 
		 {
			 if(at_biao==0)	
			 { 
			   a_timer++;	            
			   at_biao=1;	
				 SetTextValue(0,32,"约"); 
				 SetTextInt32(0,34,(again_startT1-(a_timer/60)),0,1);
				 SetTextValue(0,33,"分后启动"); 
         SetTextInt32(0,36,a_timer%60,0,1);
         //CtrSend_data[20]	=again_startT1-(a_timer/60);//发送数据到APP			 
			 }
		  }
		 else at_biao=0;
		
		if( control_wait_s( &wat_1,(again_startT1*60)))//等待缺引水时间到跳转到缺水报警 
			{		
			  CourseNum=1;//				
			}		
		
    break;

	
    case 20:	

		   
		   valve_state1=3;//阀门关闭进程	
       Kai_moto1=0;
		   Kai_moto2=0;		
 
		   if( control_wait_s( &wat_1,2)) //延时10秒 停止关闭阀门
			   {			
             CourseNum=21;//再次启动电机				   
			   }
		
			break;			


	case 21:	//关闭阀门		
		   Guan_moto1=1;//关闭阀门
		   Guan_moto2=1;//关闭气阀
			 valve_state1=3;//阀门关闭进程标志
	
		   if( control_wait_s( &wat_1,10)) //延时10秒 停止关闭阀门
			   {

				  valve_state1=0;//阀关闭标志
			    Guan_moto1=0;
		      Guan_moto2=0;
          CourseNum=12;//进入关闭电机节点				   
			   }
		
			break;					   
		
	}

}


void KongZhi_guozheng_2(void)

{	
	
//	u16 ALoadCurrent,BLoadCurrent,CLoadCurrent;//带载电流	
//	u16 PresentLevel;//当前水位	
//	u16 A_current, B_current, C_current;//当前电流
	
	static u16 a_timer=0;//延时启动
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
		     


         if(relay_state2==0)  {AnimationPlayFrame(0,25,0); RELAY2=1;}     //水泵1启动
		     if(relay_state2==1)  {AnimationPlayFrame(0,25,1); RELAY2=0;}     //水泵1停止
			   if(relay_state2==2)  {AnimationPlayFrame(0,25,2); RELAY2=0;}     //水泵1暂停
         if(relay_state2==3)  {AnimationPlayFrame(0,25,3); RELAY2=0;}     //水泵1缺水
		     if(relay_state2==4)  {AnimationPlayFrame(0,25,4); RELAY2=0;}     //水泵1故障
			   Brelay_state2=  relay_state2;
			   Crelay_state2=  1;	//4G发送开关


		   } 

		   
	  if( valve_state2!=Bvalve_state2)
		   {		     
       if(valve_state2==0) { AnimationPlayFrame(0,20,0);   AnimationPlayFrame(0,23,0);}//阀门关闭状态
			 if(valve_state2==1) { AnimationPlayFrame(0,20,1);   AnimationPlayFrame(0,23,1);}//阀门开启状态
			 if(valve_state2==3) { AnimationPlayFrame(0,20,3);   AnimationPlayFrame(0,23,3);}//阀门关闭进程			 
			 Bvalve_state2=  valve_state2;			 
			 Cvalve_state2=  1;			 
		   } 	   


			 
	switch (CourseNum2)
		
	{		
		case 1: //按水泵启动跳转到这里		
	
			relay_state2=0;//启动水泵状态	
			if(control_wait_s(&wat_2,absence_DelayT))//absence_DelayT 缺相等待时间
	
				{  				
				  if((B_current>5)&&(B_current<500)) CourseNum2=3;	//没有缺项判断是否带载
				  else CourseNum2=2;	//缺项或者堵转 电机故障			
				}				
		
		
		
		break;
		
		case 2://电机缺相堵转故障
	
		alarm_quex_biao2=1;//开启缺相报警
    relay_state2=4;//显示电机故障
		CourseNum2=23;
		
		break;

		case 23://电机停止运行
			relay_state2=1;//停止状态	
		break;		
		
		case 3://判断是否带载
		
			if(control_wait_s(&wat_2,10))		
				{				  
				  if(B_current<10)  CourseNum2=2;//缺项或者堵转 电机故障	
				  if(B_current>=BLoadCurrent*2)  CourseNum2=2;//缺项或者堵转 电机故障				
				  //if((A_current>=10)&&(A_current<=(ALoadCurrent/5))) CourseNum=4; //不带载	
					if((B_current>=10)&&(B_current<=(200))) CourseNum2=4; //不带载	
				  //if((A_current>(ALoadCurrent/5))&&(A_current<(ALoadCurrent*2))) CourseNum=5;//带载正常	
					if((B_current>(200))&&(B_current<(500))) CourseNum2=5;//带载正常	
				}
				
	    break;		
		
		
		case 4://不带载			
		  relay_state2=2;//水泵2暂停状态
			CourseNum2=6;  
		break;
		
		case 5://带载			

			if(control_wait_s(&wat_2,2))
				 
				{ a_timer=0;//清除时间
					open_Num2=0;//清除进水次数
					if(B_current<10)  CourseNum2=101;//缺项或者堵转 电机故障	
					if(B_current>=BLoadCurrent*2)  CourseNum2=101;//缺项或者堵转 电机故障	
          if((B_current>=10)&&(B_current<=(200))) CourseNum2=101; //不带载						
					//if((A_current>=10)&&(A_current<=(ALoadCurrent/5))) CourseNum=13; //不带载应该跳转到 按水位延长时间控制					
					//if((A_current>(ALoadCurrent/5))&&(A_current<(ALoadCurrent*2))) CourseNum=5;//带载正常
          if((B_current>(200))&&(B_current<(500))) CourseNum2=5;//带载正常						
				}
			
			break;
				
			case 101://带载			

			if(control_wait_s(&wat_2,2))
				 
				{ 
					if(B_current<10)  CourseNum2=2;//缺项或者堵转 电机故障	
					if(B_current>=BLoadCurrent*2)  CourseNum2=2;//缺项或者堵转 电机故障	
					
					
					if(auto_button_1==1)         //没有自动启停
						{ 					
					   if((B_current>=10)&&(B_current<=(200))) CourseNum2=11; //缺水报警几点					
						}
					else 
					  {					
					   if((B_current>=10)&&(B_current<=(200))) CourseNum2=13; //不带载								
					  }
					
					
          				
					//if((A_current>=10)&&(A_current<=(ALoadCurrent/5))) CourseNum=13; //不带载应该跳转到 按水位延长时间控制					
					//if((A_current>(ALoadCurrent/5))&&(A_current<(ALoadCurrent*2))) CourseNum=5;//带载正常
          if((B_current>(200))&&(B_current<(500))) CourseNum2=5;//带载正常						
				}
			
			break;		
				
				
				
				
		
		case 6://加引水
			Kai_moto3=1;//放气开
			Kai_moto4=1;//加水
			
		    valve_state2=1;//阀开启标志		    
		   
		    if( control_wait_s( &wat_2,10))//延时10秒钟停止加水放气输出，此时气阀水阀状态是开启状态。
			   {		
			    Kai_moto3=0;
			    Kai_moto4=0;				   
          CourseNum2=7;				   
			   }

			break;	
		
		case 7:			
		   if(B_LEVEL==1)	CourseNum2=8;//水位到跳转到关闭阀门	
		   if( control_wait_s( &wat_2,NOwater_T))//等待缺引水时间到跳转到缺水报警 
			    {		
			     CourseNum2=15;//缺引水			
			    }		
			break;	
				
				
		case 15:	//缺饮水代码暂停变成停止		
	     Guan_moto3=1;//关闭阀门
		   Guan_moto4=1;//关闭气阀
		  
		   valve_state2=3;//关阀进行标志
		   
		
		
		   if( control_wait_s( &wat_2,10)) //延时10秒 停止关闭阀门
			   {		
			    Guan_moto3=0;
		      Guan_moto4=0;				   
				
		        valve_state2=0;//阀关闭标志   
				   
            CourseNum2=9;//发送缺引水代码	   
			   }
		
			break;			
				
				
		
		case 8:	//关闭阀门		
		   Guan_moto3=1;//关闭阀门
		   Guan_moto4=1;//关闭气阀		 
		   valve_state2=3;//关阀进行标志	
		   if( control_wait_s( &wat_2,10)) //延时10秒 停止关闭阀门
			   {
		      valve_state2=0;//阀关闭标志
			    Guan_moto3=0;
		      Guan_moto4=0;
          CourseNum2=10;//再次启动电机				   
			   }
		
			break;	
		
		case 9:	//缺饮水代码暂停变成停止		

		alarm_pull_biao=1;	//三个井公用一个	
		CourseNum2=12;		
		break;
		
	case 10://再次启动水泵			
		

		open_Num2++;
		
		if(open_Num2<3)  
		  {			
			CourseNum2=1;
		  }
		  
		if(open_Num2>=3) 
		  {
		   CourseNum2=11;//启动失败//发送水井缺水
       }
		  
		break;		
		case 11:	//启动失败//发送水井缺水
			
        alarm_well_biao2=1;
        relay_state2=3;		      
        CourseNum2=102;		
		break;
		
		case 102:	//启动失败//发送水井缺水
			
     relay_state2=3;//缺水停止
		 valve_state2=0;//阀关闭标志  
		break;
		
		
		case 12:	//停止状态
		   
		 relay_state2=1;//停止状态       
		 valve_state2=0;//阀关闭标志
		
		break;
		
  case 13:	//暂停状态
		   
    relay_state2=2;//暂停状态		 
		CourseNum2=22;		
		break;
   
  
  case 22://水井缺水报警
	    alarm_well_biao2=1;
	    again_startT_biao2=1;
	    //CtrSend_data[21]	=again_startT2-(a_timer/60);//发送数据到APP	
	    again_startT2_test=0xffff;
      CourseNum2=14;
      break;
		
  case 14:	//延时再启动

		
	  if(PresentLevel<=set_leve[10])                               again_startT2=set_time[10];		
		if((PresentLevel>set_leve[10])&&(PresentLevel<=set_leve[9])) again_startT2=set_time[9];
		if((PresentLevel>set_leve[9]) &&(PresentLevel<=set_leve[8])) again_startT2=set_time[8];
		if((PresentLevel>set_leve[8]) &&(PresentLevel<=set_leve[7])) again_startT2=set_time[7];
		if((PresentLevel>set_leve[7]) &&(PresentLevel<=set_leve[6])) again_startT2=set_time[6];
		if(PresentLevel> set_leve[6])                                again_startT2=set_time[6];
	
	
	if(again_startT2_test!=again_startT2)
	 {
	  again_startT2_test=again_startT2; 
		CtrSend_data[21]	=again_startT2-(a_timer/60);//发送数据到APP		
	 }
	
	
		if(((TIMER_1MS%1000)>450)) 
		 {
			 if(at_biao==0)	
			 { 
			   a_timer++;	
         			 
			   at_biao=1;			
		     SetTextValue(0,38,"约"); 
				 SetTextInt32(0,35,(again_startT2-(a_timer/60)),0,1);
				 SetTextValue(0,39,"分后启动"); 
         SetTextInt32(0,37,a_timer%60,0,1);	
        // CtrSend_data[21]	=again_startT2-(a_timer/60);//发送数据到APP	

				 
			 }
		  }
		 else at_biao=0;
		
		if( control_wait_s( &wat_2,(again_startT2*60)))//等待缺引水时间到跳转到缺水报警 
			{		
			  CourseNum2=1;//				
			}		
		
    break;




   case 20:	
		
		   valve_state2=3;//关阀进行标志
		    		
       Kai_moto3=0;
		   Kai_moto4=0;		
 
		   if( control_wait_s( &wat_2,2)) //延时10秒 停止关闭阀门
			   {			
                CourseNum2=21;//再次启动电机				   
			   }
		
			break;			


	case 21:	//关闭阀门		
		   Guan_moto3=1;//关闭阀门
		   Guan_moto4=1;//关闭气阀 		
		   valve_state2=3;//关阀进行标志	   
		
		   if( control_wait_s( &wat_2,10)) //延时10秒 停止关闭阀门
			   {		
				  valve_state2=0;//阀关闭标志		   
			    Guan_moto3=0;
		      Guan_moto4=0;
          CourseNum2=12;//再次启动电机				   
			   }
		
			break;					   
			
	}

	
}






void KongZhi_guozheng_3(void)

{	
	
//	u16 ALoadCurrent,BLoadCurrent,CLoadCurrent;//带载电流	
//	u16 PresentLevel;//当前水位	
//	u16 A_current, B_current, C_current;//当前电流
	
	static u16 a_timer=0;//延时启动
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
		     


         if(relay_state3==0)  {AnimationPlayFrame(0,24,0); RELAY3=1;}     //水泵1启动
		     if(relay_state3==1)  {AnimationPlayFrame(0,24,1); RELAY3=0;}     //水泵1停止
			   if(relay_state3==2)  {AnimationPlayFrame(0,24,2); RELAY3=0;}     //水泵1暂停
         if(relay_state3==3)  {AnimationPlayFrame(0,24,3); RELAY3=0;}     //水泵1缺水
		     if(relay_state3==4)  {AnimationPlayFrame(0,24,4); RELAY3=0;}     //水泵1故障
			   Brelay_state3=  relay_state3;
			   Crelay_state3=  1;	//4G发送开关


		   } 

		   
	  if( valve_state3!=Bvalve_state3)
		   {		     
       if(valve_state3==0) { AnimationPlayFrame(0,22,0);   AnimationPlayFrame(0,21,0);}//阀门关闭状态
			 if(valve_state3==1) { AnimationPlayFrame(0,22,1);   AnimationPlayFrame(0,21,1);}//阀门开启状态
			 if(valve_state3==3) { AnimationPlayFrame(0,22,3);   AnimationPlayFrame(0,21,3);}//阀门关闭进程			 
			 Bvalve_state3=  valve_state3;			 
			 Cvalve_state3=  1;			 
		   } 	   
			 
	switch (CourseNum3)
		
	{		
		case 1://启动跳转到这里			
		
		
			
			relay_state3=0;//启动状态	
			
				
			if(control_wait_s(&wat_3,absence_DelayT))//absence_DelayT 缺相等待时间
	
				{  				
				  if((C_current>5)&&(C_current<500)) CourseNum3=3;	//没有缺项判断是否带载
				  else CourseNum3=2;	//缺项或者堵转 电机故障			
				}				
		
		
		
		break;
		
		case 2://电机缺相堵转故障
		
		alarm_quex_biao3=1;//开启缺相报警
    relay_state3=4;//显示电机故障
		CourseNum3=23;
		
		break;
		
		case 23://电机停止运行		
			relay_state3=1;//停止状态	
		break;
		
		
		case 3://判断是否带载
		
			if(control_wait_s(&wat_3,10))		
				{				  
				  if(C_current<10)  CourseNum3=2;//缺项或者堵转 电机故障	
				  if(C_current>=CLoadCurrent*2)  CourseNum3=2;//缺项或者堵转 电机故障				
				  //if((A_current>=10)&&(A_current<=(ALoadCurrent/5))) CourseNum=4; //不带载	
					if((C_current>=10)&&(C_current<=(200))) CourseNum3=4; //不带载	
				  //if((A_current>(ALoadCurrent/5))&&(A_current<(ALoadCurrent*2))) CourseNum=5;//带载正常	
					if((C_current>(200))&&(C_current<(500))) CourseNum3=5;//带载正常	
				}
				
	    break;		
		
		
		case 4://不带载
			
		  relay_state3=2;//水泵2暂停状态
			CourseNum3=6;  
			break;
		
		case 5://带载			

			if(control_wait_s(&wat_3,2))
				 
				{   a_timer=0;//清除时间
					  open_Num3=0;//清除进水次数
					if(C_current<10)  CourseNum3=101;//缺项或者堵转 电机故障	
					if(C_current>=CLoadCurrent*2)  CourseNum3=101;//缺项或者堵转 电机故障	
          if((C_current>=10)&&(C_current<=(200))) CourseNum3=101; //不带载						
					//if((A_current>=10)&&(A_current<=(ALoadCurrent/5))) CourseNum=13; //不带载应该跳转到 按水位延长时间控制					
					//if((A_current>(ALoadCurrent/5))&&(A_current<(ALoadCurrent*2))) CourseNum=5;//带载正常
          if((C_current>(200))&&(C_current<(500))) CourseNum3=5;//带载正常						
				}
			
			break;
		
				
				
				
			case 101://带载			

			if(control_wait_s(&wat_3,2))
				 
				{   
					if(C_current<10)  CourseNum3=2;//缺项或者堵转 电机故障	
					if(C_current>=CLoadCurrent*2)  CourseNum3=2;//缺项或者堵转 电机故障	
					
					if(auto_button_3==1)         //没有自动启停
						{ 					
					   if((C_current>=10)&&(C_current<=(200))) CourseNum3=11;//缺水报警几点					
						}
					else 
					  {					
					   if((C_current>=10)&&(C_current<=(200))) CourseNum3=13; //不带载						
					  }
					
					
					
          				
					//if((A_current>=10)&&(A_current<=(ALoadCurrent/5))) CourseNum=13; //不带载应该跳转到 按水位延长时间控制					
					//if((A_current>(ALoadCurrent/5))&&(A_current<(ALoadCurrent*2))) CourseNum=5;//带载正常
          if((C_current>(200))&&(C_current<(500))) CourseNum3=5;//带载正常						
				}
			
			break;	
				
				
				
				
				
				
				
		case 6://加引水
			Kai_moto5=1;//放气开
			Kai_moto6=1;//加水
			
		    valve_state3=1;//阀开启标志		   
		    if( control_wait_s( &wat_3,10))//延时10秒钟停止加水放气输出，此时气阀水阀状态是开启状态。
			   {		
			    Kai_moto5=0;
			    Kai_moto6=0;				   
          CourseNum3=7;				   
			   }

			break;	
		
		case 7:			
		   if(C_LEVEL==1)	CourseNum3=8;//水位到跳转到关闭阀门	
		   if( control_wait_s( &wat_3,NOwater_T))//等待缺引水时间到跳转到缺水报警 
			    {		
			     CourseNum3=15;//缺引水			
			    }		
			break;	
				
				
		case 15:	//缺饮水代码暂停变成停止		
	     Guan_moto5=1;//关闭阀门
		   Guan_moto6=1;//关闭气阀
		  
		   valve_state3=3;//关阀进行标志
		   
		   if( control_wait_s( &wat_3,10)) //延时10秒 停止关闭阀门
			   {		
			    Guan_moto5=0;
		      Guan_moto6=0;
				
		        valve_state3=0;//阀关闭标志   
				   
            CourseNum3=9;//发送缺引水代码	   
			   }
		
			break;			
				
				
		
		case 8:	//关闭阀门		
		   Guan_moto5=1;//关闭阀门
		   Guan_moto6=1;//关闭气阀
		
		   valve_state3=3;//关阀进行标志		  
		
		   if( control_wait_s( &wat_3,10)) //延时10秒 停止关闭阀门
			   {

			
		    valve_state3=0;//阀关闭标志			   
			    Guan_moto5=0;
		      Guan_moto6=0;
          CourseNum3=10;//再次启动电机				   
			   }
		
			break;	
		
		case 9:	//缺饮水代码暂停变成停止		

		alarm_pull_biao=1;	//三个井公用一个	
		CourseNum3=12;		
		break;
		
	    case 10://再次启动水泵			
		

		open_Num3++;
		
		if(open_Num3<3)  
		  {			
			CourseNum3=1;
		  }
		  
		if(open_Num3>=3) 
		  {
		   CourseNum3=11;//启动失败//发送水井缺水
       }
		  
		break;		
		case 11:	//启动失败//发送水井缺水
			
        alarm_well_biao3=1;	
		    relay_state3=3;	
        CourseNum3=102;		
		break;
		
		
		case 102:	//启动失败//发送水井缺水
			
     relay_state3=3;//缺水停止
		 valve_state3=0;//阀关闭标志  
		break;
		
		
		case 12:	//停止状态
		   
		  relay_state3=1;//停止状态      
		  valve_state3=0;//阀关闭标志
		
		break;
		
  case 13:	//暂停状态
		   
        relay_state3=2;//暂停状态
		
		
		 
		CourseNum3=22;		
		break;
   
  
  case 22://水井缺水报警
	    alarm_well_biao3=1;
	    again_startT_biao3=1;
	   // CtrSend_data[22]	=again_startT3-(a_timer/60);//发送数据到APP	
	    again_startT3_test=0xffff;
      CourseNum3=14;
      break;
		
  case 14:	//延时再启动

		
	
	
	  if(PresentLevel<=set_leve[15])                                 again_startT3=set_time[15];		
		if((PresentLevel>set_leve[15])&&(PresentLevel<=set_leve[14]))  again_startT3=set_time[14];
		if((PresentLevel>set_leve[14]) &&(PresentLevel<=set_leve[13])) again_startT3=set_time[13];
		if((PresentLevel>set_leve[13]) &&(PresentLevel<=set_leve[12])) again_startT3=set_time[12];
		if((PresentLevel>set_leve[12]) &&(PresentLevel<=set_leve[11])) again_startT3=set_time[11];
		if(PresentLevel> set_leve[11])                                 again_startT3=set_time[11];
	
	
	if(again_startT3_test!=again_startT3)
	 {
	  again_startT3_test=again_startT3; 
		CtrSend_data[22]	=again_startT3-(a_timer/60);//发送数据到APP		
	 }
	
	
	
		if(((TIMER_1MS%1000)>450)) 
		 {
			 if(at_biao==0)	
			 { 
			   a_timer++;	            
			   at_biao=1;			
		     SetTextValue(0,42,"约"); 
				 SetTextInt32(0,40,(again_startT3-(a_timer/60)),0,1);
				 SetTextValue(0,43,"分后启动"); 
         SetTextInt32(0,41,a_timer%60,0,1);
				 //CtrSend_data[22]	=again_startT3-(a_timer/60);//发送数据到APP	

				 
			 }
		  }
		 else at_biao=0;
		
		if( control_wait_s( &wat_3,(again_startT3*60)))//等待缺引水时间到跳转到缺水报警 
			{		
			  CourseNum3=1;//				
			}		
		
    break;




   case 20:	
		
		  
		   valve_state3=3;//关阀进行标志		
       Kai_moto5=0;
		   Kai_moto6=0;		
 
		   if( control_wait_s( &wat_3,2)) //延时10秒 停止关闭阀门
			   {			
                CourseNum3=21;//再次启动电机				   
			   }
		
			break;			


	case 21:	//关闭阀门		
		   Guan_moto5=1;//关闭阀门
		   Guan_moto6=1;//关闭气阀		  
		   valve_state3=3;//关阀进行标志	   
		
		   if( control_wait_s( &wat_3,10)) //延时10秒 停止关闭阀门
			   {
				
				  valve_state3=0;//阀关闭标志				   
			    Guan_moto5=0;
		      Guan_moto6=0;
          CourseNum3=12;//再次启动电机				   
			   }
		
			break;					   
			
	}


	
}



	
	
int control_wait( TROL_WAT wat,u16 timeoutval)//只执行一次，timeoutval时间以后可以执行其他后续处理  
	

	{

		
		
		if((*wat).TIMER_1MS!=TIMER_1MS)
		   {

			   if((*wat).TimetStartFlg==1)
				
			    {
 				
				  if((TIMER_1MS>(*wat).TIMER_1MS))  (*wat).TimeOutCount=(*wat).TimeOutCount+(TIMER_1MS-(*wat).TIMER_1MS);
				  if((TIMER_1MS<(*wat).TIMER_1MS))  (*wat).TimeOutCount=(*wat).TimeOutCount+(40000-(*wat).TIMER_1MS)+TIMER_1MS; //之所以是40000 是因为 TIMER_1MS加到40000变零
				  (*wat).TIMER_1MS=TIMER_1MS;
					
				}
			else  (*wat).TIMER_1MS=TIMER_1MS;	
		   
		   }
		
		if ((*wat).TimetStartFlg==0)
			{				
				 (*wat).TimeOutCount = 0;
				 (*wat).TimetStartFlg = 1;//启动计时确保执行1次
			}
			

		/* 到达设定的定时时间 */		
		
		if((*wat).TimeOutCount>=timeoutval)
		  {
			(*wat).TimetStartFlg=0;
			(*wat).TimeOutCount = 0; 
			return 1;  					  
		  }		
		return 0;
		   
	}


	
	
	
int control_wait_s( TROL_WAT wat,u16 timeoutval)//只执行一次，timeoutval时间以后可以执行其他后续处理  

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
				 (*wat).TimetStartFlg = 1;//启动计时确保执行1次
			}
			

		/* 到达设定的定时时间 */		
		
		if((*wat).TimeOutCount>=timeoutval)
		  {
			(*wat).TimetStartFlg=0;
			(*wat).TimeOutCount = 0; 
			return 1;  					  
		  }		
		return 0;
		   
	}
		

 
int CAT_wait_ds(Tcat_WAT wat,u16 timeoutval,u8 *biao)  //第一时间执行，执行完毕在加个时间执行
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

			
//标准计时结构//标准计时结构//标准计时结构//标准计时结构//标准计时结构		
	
	  
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
	通过给CAT收到的数据变量初始化，让它和发送变量数值不一样启动一次上传数据
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
	
	

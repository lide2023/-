#ifndef __CONTROL_H__
#define __CONTROL_H__


#include "sys.h"
#include "modbus.h"
#include "stmflash.h"


typedef struct
{
    u16   TimeOutCount; 
    u8    TimetStartFlg; 
	  u16   TIMER_1MS;
	  u8    Timer_biao;
}C0N_WAT,*TROL_WAT;


typedef struct
{
  u16   TimeOutCount;    
	u8    Timer_biao;
}CatWAT,*Tcat_WAT;


void store_bridge(u8 type);
void Control_Word_pack(void);
void Win_Word_parse(void);
void Win_Word_parse_mast(void);
int control_wait( TROL_WAT wat,u16 timeoutval);
int control_wait_s( TROL_WAT wat,u16 timeoutval);
void KongZhi1(void)	;
void KongZhi2(void)	;
void KongZhi3(void)	;

void Hand_KongZhi1(void);
void Hand_KongZhi2(void);
void Hand_KongZhi3(void);

void KongZhi_guozheng_1(void);
void KongZhi_guozheng_2(void);
void KongZhi_guozheng_3(void);
void KongZhi_guozheng1_hand(void);
void KongZhi_guozheng2_hand(void);
void KongZhi_guozheng3_hand(void);

void zhuangtai_chushihua(void);


void hand_have_protection_1(void);
void hand_have_protection_2(void);
void hand_have_protection_3(void);

void hand_no_protection(u8 *hand_CourseNum,u8 *relay_state1);
//void valve_hand(u8 *hand_CourseNum,u8 *valve_state1,TROL_WAT wat_4 );
void valve_hand(u8 *hand_CourseNumx,u8 *valve_statex,TROL_WAT wat_x );

int CAT_wait_ds(Tcat_WAT wat,u16 timeoutval,u8 *biao);

extern  u16  Win_Word_receive;
extern  u16  win_control_Word;
extern  C0N_WAT wat_1,wat_2,wat_3,wat_4,wat_5,wat_6,wat_7,wat_8;
extern   CatWAT cwat_1,cwat_2,cwat_3,cwat_4,cwat_5,cwat_6,cwat_7,cwat_8,cwat_9,cwat_10,cwat_11,cwat_12,cwat_13,cwat_14
	      ,cwat_15,cwat_16,cwat_17,cwat_18,cwat_19,cwat_20,cwat_21,cwat_22,cwat_23,cwat_24,cwat_25,cwat_26; 
extern  u8 CourseNum,CourseNum2,CourseNum3;


extern	u8 hand_CourseNum;
extern	u8 hand_CourseNum2;
extern	u8 hand_CourseNum3;


extern	u8 valve_hand_CourseNum;
extern 	u8 valve_hand_CourseNum2;
extern	u8 valve_hand_CourseNum3;


extern  uint16_t VolDta1,VolDta2,VolDta3,VolDta4,VolDta0;//参考电压值，ADC1_Ch4采样电压值，单位V


extern  u8 open1,stop1;
extern  u8 cat_open1,cat_stop1;

extern  u8 open2,stop2;
extern  u8 cat_open2,cat_stop2;

extern  u8 open3,stop3;
extern  u8 cat_open3,cat_stop3;


extern	u8 valve_open1,valve_stop1;
extern  u8 cat_valve_open1,cat_valve_stop1;
  
extern  u8 valve_open2,valve_stop2;
extern  u8 cat_valve_open2,cat_valve_stop2;
   
extern  u8 valve_open3,valve_stop3;
extern  u8 cat_valve_open3,cat_valve_stop3;

extern  u16 auto_button_1,auto_button_2,auto_button_3;//自动运行选择
extern  u16 biaoding_shuiwei;
extern  u16 shuiwei_xinhao;
extern  u16 lock_biao;
extern  u16 start_N,absence_DelayT,up_waterT,NOwater_T;

 extern  	u8 Kai_moto1,Guan_moto1;
 extern 	u8 Kai_moto2,Guan_moto2;
 extern 	u8 Kai_moto3,Guan_moto3;
 extern 	u8 Kai_moto4,Guan_moto4;
 extern 	u8 Kai_moto5,Guan_moto5;
 extern 	u8 Kai_moto6,Guan_moto6;
 extern     u8 A_LEVEL,B_LEVEL,C_LEVEL;
 extern     u16 A_current, B_current, C_current;
 extern     u16 PresentLevel;
 extern     u16 PresentLevel_temp;
 extern     u8 A_current_biao, B_current_biao, C_current_biao;//上传标志当前电流
 extern   	u8 PresentLevel_biao; //当前水位上传标志
 
 
 extern     u16 ALoadCurrent,BLoadCurrent,CLoadCurrent;
 extern     u8 open_Num1;
 extern     u8 open_Num2;
 extern     u8 open_Num3;
 
 extern   u16 set_leve[16];
 extern 	u16 set_time[16];
 
extern	u16 again_startT1,again_startT1_test;
extern 	u16 again_startT2,again_startT2_test;
extern	u16 again_startT3,again_startT3_test;
 
 extern   u8 again_startT_biao1;
 extern 	u8 again_startT_biao2;
 extern 	u8 again_startT_biao3; 

 extern     u8  alarm_noelec_biao;
 extern     u8  alarm_upelec_biao; 
 extern     u8  alarm_quex_biao1;
 extern     u8  alarm_quex_biao2;
 extern     u8  alarm_quex_biao3;
 extern     u8  alarm_well_biao1;
 extern     u8  alarm_well_biao2;
 extern     u8  alarm_well_biao3;
 extern     u8  alarm_pull_biao;
 extern     u8  send_str_biao; 
 extern     u8 CAT1_Send_Data_dif_biao;
 extern     u8  send_mem_nember;


extern u8 relay_state1;
extern u8 relay_state2;
extern u8 relay_state3;
 
extern u8 Brelay_state1;
extern u8 Brelay_state2;
extern u8 Brelay_state3;


extern u8 Crelay_state1;
extern u8 Crelay_state2;
extern u8 Crelay_state3;

 
extern u8 valve_state1;
extern u8 valve_state2;
extern u8 valve_state3;
 
extern u8 Bvalve_state1;
extern u8 Bvalve_state2;
extern u8 Bvalve_state3;
 

extern u8 Cvalve_state1;
extern u8 Cvalve_state2;
extern u8 Cvalve_state3;


extern u16 	kongzhi_fangshi; 
extern u16 	protection_switch_1; 
extern u16 	protection_switch_2; 
extern u16 	protection_switch_3; 
extern u16 	valve_travel_time;


#endif
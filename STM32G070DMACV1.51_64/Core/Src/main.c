/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
 /*
 比V61版本增加了反馈环形队列
 v61版本存在不能第一时间反馈反馈数据问题，电流查询的时候特别明显
 V6.2增加了ASK_for 反馈第一时间反馈
 V6.4 增加的 ASK注释  
      control.c len 521 开阀修改
 */ 
  
  
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "dma.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include "stdio.h"
#include "delay.h"
#include "modbus.h"
#include "control.h"
#include  "stmflash.h"
#include "uartLCD.h"
#include "ec600u.h"
#include "dacaiLCD.h"
#include "queue.h"


void SystemClock_Config(void);



#ifdef __GNUC__
  
  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */

PUTCHAR_PROTOTYPE
{
  	HAL_UART_Transmit( &huart2 , (uint8_t *)&ch, 1, 0xFFFF);
  return ch;
}

  __IO   uint16_t   aADCxConvertedData[ADC_CONVERTED_DATA_BUFFER_SIZE]; //存放3组数据，分别是ADC1_Ch4,温度，参考电压
	__IO   uint8_t ubDmaTransferStatus = 2;//定义变量，在ADC采样结束，DMA传送完成置位，方便主程序取数据
	

//ADC数据采样结束回调函数，用于置位采样结束标志位
 void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
  /* Prevent unused argument(s) compilation warning */
   UNUSED(hadc);
   ubDmaTransferStatus=1;//置1
}


//增加功能外设要包括对应外设的C文件和头文件，主要在Stm32g0xx_hal_conf.h文件里面LINE 40 300
//另外需要增加用户外设头文件例如#include"usart.h"
//stm32g0xx_it.c里面需要增加中断函数和声明结构体变量


int main(void)
{
  
uint32_t tmp_index_adc_converted_data = 0;
 u8 kaiji_biao_elec=0;

// C0N_WAT wat_1,wat_2;

   static u8 guanji_biao=1;
   static u8 guanji_time=0; 

	
  HAL_Init();

 
  SystemClock_Config();
  delay_init(64);//    需要手动添加这个启动
 
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_ADC1_Init();
  MX_USART1_UART_Init();
  MX_USART2_UART_Init();
  MX_USART3_UART_Init();
  MX_USART4_UART_Init();
  MX_TIM3_Init();
	
 HAL_GPIO_WritePin(GPIOC, GPIO_PIN_3,GPIO_PIN_RESET);//CAT模块电源控制引脚开机处于拉低状态
	
  MX_IWDG_Init(); 
  u8 storage_0[10];
  u8 buf_test[10];
  
     for (tmp_index_adc_converted_data = 0; tmp_index_adc_converted_data < ADC_CONVERTED_DATA_BUFFER_SIZE; tmp_index_adc_converted_data++)
		  {
			aADCxConvertedData[tmp_index_adc_converted_data] = VAR_CONVERTED_DATA_INIT_VALUE;  //采样数组清0
		  }
	  if (HAL_ADCEx_Calibration_Start(&hadc1) != HAL_OK)//ADC自校准
		  {
			/* Calibration Error */
			while(1);
		  }
	  if (HAL_ADC_Start_DMA(&hadc1,(uint32_t *)aADCxConvertedData,ADC_CONVERTED_DATA_BUFFER_SIZE) != HAL_OK)//启动ADC DMA传输
		  {
			/* ADC conversion start error */
			while(1);
		  }  
  
  
	if (HAL_TIM_Base_Start_IT(&htim3) != HAL_OK)//启动TIM3,这个代码需要手动增加
		 {
			 while(1);
		 }	  
	  HAL_UART_Receive_IT(&huart1, aRxBuffer1, 1);//启动串口1中断
      HAL_UART_Receive_IT(&huart2, aRxBuffer2, 1);//启动串口1中断  		 
      HAL_UART_Receive_IT(&huart4, aRxBuffer, 1);//启动串口4中断  
		 
    Read_all_1();	 
		Cat_chushihua(); 
	Send_control_word();	
    queue_reset();//清理窗口屏缓存 
    cwat_10.TimeOutCount=2000;
   zhuangtai_chushihua();

/*
*真正编程的时候不要有大的delay_ms()占用资源的函数，必须模拟实际使用环境
*需要用LED指示的时候就用时间中断
*如果用大的延时，容易造成错误		 
*/		 
 
	
		IWDG_Feed();//喂狗    
	 
	  EN_LCD=0;	 
		 
 
	  delay_ms(2000); 		 
		


u8 i=0;

for(i=0;i<100;i++)
{
 res_storage_1[i]=0xffff;

}



	 
  while (1)
  {
	  
	
IWDG_Feed();//喂狗   




static u8 timer=0,timer_biao=0;
static u8 aaa=0;
static u8 send_biao=0;

static u8 ccc=0;
static u8 ddd=0;
static u8 eee=0;
static u16 bbb=0;
static u8 biao_bbb=0;
static u8 biao_aaa=0;
static u8 i=0; 
static qsize  size = 0; 

cat1_excute();//处理接收到的数据
sure_write_eepall_1(); 

if(VolDta4>650)

	{
		EN_LCD=0;	 
		EN_BAT=0;
		alarm_noelec_biao=0;//开启停电报警
		kaiji_biao_elec=1;
		guanji_time=0;
	}

if((VolDta4<=650)&&(kaiji_biao_elec==1))

 {
	  EN_LCD=1;
//	  CtrSend_data[7]=1;	
//	  CAT1_Send_Data_Nd(0x3000,7,CtrSend_data,&alarm_noelec_biao); 

	  kaiji_biao_elec=0;	  
    alarm_quex_biao1=0;
	  alarm_quex_biao2=0;
	  alarm_quex_biao3=0;
	  alarm_well_biao1=0;
	  alarm_well_biao2=0;
	  alarm_well_biao3=0;
    alarm_pull_biao=0;
	  alarm_noelec_biao=1;//停电报警
//	  alarm_upelec_biao=0;
//    send_str_biao=0;	 
	
 }

 if(kaiji_biao_elec==0)
 
 {
	 
	if((TIMER_1MS%1000)<50)
		  {
			  if(guanji_biao==1) 
			   {
				  guanji_time++;
				  guanji_biao=0;
			   }
		  }
     else guanji_biao=1;		  
	 
 	if(guanji_time>=1)    		
		{
		  EN_BAT=1;	
		  guanji_time=5;
	    
		}

 }
 

if( control_wait( &wat_5,990))     		
	{
     ddd++;
     HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_12);
		
		if(CourseNum!=14)
			{				
		     SetTextValue(0,32,"");				
				 SetTextValue(0,34,"");
				 SetTextValue(0,33,"");         
		     SetTextValue(0,36,"");
			}
		

			
			
		if(CourseNum2!=14)
			{				
		     SetTextValue(0,38,"");				
				 SetTextValue(0,35,"");
				 SetTextValue(0,39,"");         
		     SetTextValue(0,37,"");
			}
		
		
		if(CourseNum3!=14)
			{				
		     SetTextValue(0,42,"");				
				 SetTextValue(0,40,"");
				 SetTextValue(0,43,"");         
		     SetTextValue(0,41,"");
			}	
			
				SetTextInt32(0,12,PresentLevel,0,1); 
		    SetTextInt32(0,47,VolDta4,0,1); 
      SetTextValue(0,11,"当前水位"); 
	}	


	
 size = queue_find_cmd(cmd_buffer,CMD_MAX_SIZE);//从缓冲区中获取一条指令 

	if(size>0&&cmd_buffer[1]!=0x07)                                                   //接收到指令 ，及判断是否为开机提示
           {                                                                           
             ProcessMessage((PCTRL_MSG)cmd_buffer, size);                           //指令处理 			
           }	

if(kongzhi_fangshi==0)//自动控制方式
	 {
	 
	  KongZhi_guozheng_1();
    KongZhi_guozheng_2();
    KongZhi_guozheng_3();	 
	 
	 }
 else{
 
 
  KongZhi_guozheng1_hand(); 					 
  KongZhi_guozheng2_hand(); 
  KongZhi_guozheng3_hand(); 
 
 
 } 

					 
					 
					 
if(((TIMER_1MS%500)>450)) 
 {
	 
  if(biao_aaa==0) 
  { 
		
	 HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
 		
		 if( current_screen_id==3)
	  {
	   SetTextInt32(3,2,ALoadCurrent,0,1);//带载电流
	   SetTextInt32(3,3,BLoadCurrent,0,1); 
	   SetTextInt32(3,4,CLoadCurrent,0,1); 

	  
	  SetTextInt32(3,5,A_current,0,1);
    SetTextInt32(3,6,B_current,0,1);	  
	  SetTextInt32(3,7,C_current,0,1);
			
		if(protection_switch_1==1) AnimationPlayFrame(3,11,1);
		 else  AnimationPlayFrame(3,11,0);			  	
			
			if(protection_switch_2==1) AnimationPlayFrame(3,13,1);
			  else  AnimationPlayFrame(3,13,0);			
		
			if(protection_switch_3==1) AnimationPlayFrame(3,15,1);
			  else  AnimationPlayFrame(3,15,0);				
			
	  }
		

		
	  if( current_screen_id==5)
	  {
	    
			SetTextInt32(5,2,start_N,0,1); 	
      SetTextInt32(5,3,absence_DelayT,0,1);
      SetTextInt32(5,4,up_waterT,0,1); 
      SetTextInt32(5,5,NOwater_T,0,1); 
			SetTextInt32(5,6,valve_travel_time,0,1); 
			SetTextInt32(5,7,biaoding_shuiwei,0,1); 
			
      if(kongzhi_fangshi==1) {AnimationPlayFrame(5,8,0); AnimationPlayFrame(5,10,1);}
			else  {AnimationPlayFrame(5,8,1); AnimationPlayFrame(5,10,0);}		
			
	  }
	
		
		
	if( current_screen_id==2)
	  {
	   u8 i;
	   for(i=2;i<=31;i++)
		 {
		 if(i%2==0)
                 {					
					 
					SetTextInt32(2,i,set_leve[(i/2)-1],0,1);  
					 
				 }
				 
		  if(i%2==1)
				 {
                  
             SetTextInt32(2,i,set_time[(i/2)-1],0,1);  
					 
				 }	 
		 
		 }
		 
		 
//		  SetTextInt32(2,2,Sure_write_timer,0,1);  
		 
		 
	 if(auto_button_1==0) AnimationPlayFrame(2,32,1);
	 else  AnimationPlayFrame(2,32,0);			
	
	 if(auto_button_2==0) AnimationPlayFrame(2,33,1);
	 else  AnimationPlayFrame(2,33,0);
	
    if(auto_button_3==0) AnimationPlayFrame(2,34,1);
	  else  AnimationPlayFrame(2,34,0);				 

	  } 
	  
	 biao_aaa=1;
	  
  }	 
 }

else biao_aaa=0;//不能用else 这种格式
 

//if(CAT_back_biao==0)
//{
//  Ask_for();//只有应当反馈信号执行完以后才能执行这个发送，否则的话，有可能小程序返回信号快了，能报上次没来的急回馈的
//	           // 的反馈内容给冲了            
//	}
 

 

 
 
Ask_for();
  
 clear_alarm();


 all_cat_senddata(); 
 


 
 
 
Control_Word_pack(); //控制字打包 主副机通用
	 
 if(TIMER_1MS_send>=200) 
	{
		TIMER_1MS_send=0;	
        timer++;    
		if(timer>=4) timer=0;
		
	}
	
	if((timer_biao!=timer)&&(timer==0))
	  {
			timer_biao=timer;			
			Send_control_word();			
			
	   }


	if((timer_biao!=timer)&&(timer==1))
	  {
			timer_biao=timer;			
			Send_control_word();	
	   }


	if((timer_biao!=timer)&&(timer==2))
	  {
			timer_biao=timer;			
			Send_control_word();	
	   }

    if((timer_biao!=timer)&&(timer==3))
	  {
			timer_biao=timer;			
			Send_control_word();	
	   }


moud_excute1_mast();     //接收中断，接收完数据，处理处理数据，把有用的数据从数组里面解析出来
Win_Word_parse_mast();	//主机解析485返回值	读取副机水位开关状态
	
 
		 
//	  A_current=VolDta0;
//	  B_current=VolDta1;
//		C_current=VolDta2;
//    if(shuiwei_xinhao==1) PresentLevel=VolDta3; 


		 
	  if (ubDmaTransferStatus == 1)//判断数据是否已经转换完成
			{
					  VolDta1=aADCxConvertedData[0]*3300/4096;
					  VolDta2=aADCxConvertedData[1]*3300/4096;
				    VolDta3=aADCxConvertedData[2]*3300/4096;//4-20毫安
				    VolDta4=aADCxConvertedData[3]*3300/4096;//VolDta4外部电压
				    VolDta0=aADCxConvertedData[4]*3300/4096; 
				   
			  for (tmp_index_adc_converted_data = 0; tmp_index_adc_converted_data < ADC_CONVERTED_DATA_BUFFER_SIZE; tmp_index_adc_converted_data++)
			  {
				aADCxConvertedData[tmp_index_adc_converted_data] = VAR_CONVERTED_DATA_INIT_VALUE;//采样数组清0
			  }
			  /* Update status variable of DMA transfer */
			  ubDmaTransferStatus = 0;//完成标志清0
			}
  }
  
}




/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Configure the main internal regulator output voltage
  */
  HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = RCC_PLLM_DIV1;
  RCC_OscInitStruct.PLL.PLLN = 16;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the peripherals clocks
  */
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART1|RCC_PERIPHCLK_USART2
                              |RCC_PERIPHCLK_ADC;
  PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK1;
  PeriphClkInit.Usart2ClockSelection = RCC_USART2CLKSOURCE_PCLK1;
  PeriphClkInit.AdcClockSelection = RCC_ADCCLKSOURCE_SYSCLK;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

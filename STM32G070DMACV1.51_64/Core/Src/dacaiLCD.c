#include "dacaiLCD.h"
#include "delay.h"
#include "modbus.h"
#include "stdio.h"
#include "stmflash.h"
#include "control.h"




#define TX_8(P1) SEND_DATA((P1)&0xFF)                    //发送单个字节
#define TX_8N(P,N) SendNU8((uint8 *)P,N)                 //发送N个字节
#define TX_16(P1) TX_8((P1)>>8);TX_8(P1)                 //发送16位整数
#define TX_16N(P,N) SendNU16((uint16 *)P,N)              //发送N个16位整数
#define TX_32(P1) TX_16((P1)>>16);TX_16((P1)&0xFFFF)     //发送32位整数

#define PTR2U16(PTR) ((((u8 *)(PTR))[0]<<8)|((u8 *)(PTR))[1])  //从缓冲区取16位数据
#define PTR2U32(PTR) ((((u8 *)(PTR))[0]<<24)|(((u8 *)(PTR))[1]<<16)|(((u8 *)(PTR))[2]<<8)|((u8 *)(PTR))[3])  //从缓冲区取32位数据


#define SEND_DATA(P) SendChar(P)          //发送一个字节
#define BEGIN_CMD() TX_8(0XEE)            //帧头
#define END_CMD() TX_32(0XFFFCFFFF)       //帧尾


#define CMD_HEAD 0XEE                                                  //帧头
#define CMD_TAIL 0XFFFCFFFF 



u16 current_screen_id = 0;
u8  cmd_buffer[CMD_MAX_SIZE];  



                                           //指令队列
 u32 cmd_state = 0;                                           //队列帧尾检测状态
 qsize cmd_pos = 0;                                              //当前指令指针位置

 QUEUE que = {0,0,0};  


void DaCai_Send_Data(u8 *buf)
{

	  while((USART1->ISR&0X40)==0){}//等待发送完成 这种方式比HAL_UART_Transmit(&huart1,buf,len,1000) 稳定
    USART1->TDR =  *buf; 

}



void SendChar(u8 num)
    {

       DaCai_Send_Data(&num);	

     }




/*! 
*  \brief  串口发送送字符串
*  \param  字符串
*/
void SendStrings(u8 *str)
{
    while(*str)
    {
        TX_8(*str);
        str++;
    }
}




/*! 
*  \brief     设置文本值
*  \param  screen_id 画面ID
*  \param  control_id 控件ID
*  \param  str 文本值
*/
void SetTextValue(u16 screen_id,u16 control_id,u8 *str)
{
    BEGIN_CMD();
    TX_8(0xB1);
    TX_8(0x10);
    TX_16(screen_id);
    TX_16(control_id);
    SendStrings(str);
    END_CMD();
}



	 
	 
	 
/*! 
*  \brief     设置文本为整数值，要求FIRMWARE_VER>=908
*  \param  screen_id 画面ID
*  \param  control_id 控件ID
*  \param  value 文本数值
*  \param  sign 0-无符号，1-有符号
*  \param  fill_zero 数字位数，不足时左侧补零
*/	 
	 
void SetTextInt32(u16 screen_id,u16 control_id,u32 value,u8 sign,u8 fill_zero)
{
    BEGIN_CMD();
    TX_8(0xB1);
    TX_8(0x07);
    TX_16(screen_id);
    TX_16(control_id);
    TX_8(sign?0X01:0X00);
    TX_8((fill_zero&0x0f)|0x80);
    TX_32(value);
    END_CMD();
}






/*! 
*  \brief     播放制定帧
*  \param  screen_id 画面ID
*  \param  control_id 控件ID
*  \param  frame_id 帧ID
*/
void AnimationPlayFrame(u16 screen_id,u16 control_id,u8 frame_id)
{
    BEGIN_CMD();
    TX_8(0xB1);
    TX_8(0x23);
    TX_16(screen_id);
    TX_16(control_id);
    TX_8(frame_id);
    END_CMD();
}





                                                          


/*! 
*  \brief  清空指令数据
*/
void queue_reset()
{
    que._head = que._tail = 0;
    cmd_pos = cmd_state = 0;
}
/*! 
* \brief  添加指令数据
* \detial 串口接收的数据，通过此函数放入指令队列 
*  \param  _data 指令数据
*/

void queue_push(void) 
{
    
	qsize pos = (que._head+1)%QUEUE_MAX_SIZE;  
    if(pos!=que._tail)                                                //非满状态
    {
        que._data[que._head] = aRxBuffer1[0];
        que._head = pos;
    }
}

//从队列中取一个数据
 void queue_pop(qdata* _data)
{
    if(que._tail!=que._head)                                          //非空状态
    {
        *_data = que._data[que._tail];
        que._tail = (que._tail+1)%QUEUE_MAX_SIZE;
    }
}

//获取队列中有效数据个数
 u16 queue_size()
{
    return ((que._head+QUEUE_MAX_SIZE-que._tail)%QUEUE_MAX_SIZE);
}
/*! 
*  \brief  从指令队列中取出一条完整的指令
*  \param  cmd 指令接收缓存区
*  \param  buf_len 指令接收缓存区大小
*  \return  指令长度，0表示队列中无完整指令
*/

u16 queue_find_cmd(u8 *buffer,u16 buf_len)

{
    qsize cmd_size = 0;
    qdata _data = 0;

    while(queue_size()>0)
    {
        //取一个数据
        queue_pop(&_data);

        if(cmd_pos==0&&_data!=CMD_HEAD)                               //指令第一个字节必须是帧头，否则跳过
        {
           continue;
        }
		
		
        //    LED2_ON;
        if(cmd_pos<buf_len)                                           //防止缓冲区溢出
            
		     buffer[cmd_pos++] = _data;

        cmd_state = ((cmd_state<<8)|_data);                           //拼接最后4个字节，组成一个32位整数

        //最后4个字节与帧尾匹配，得到完整帧
        if(cmd_state==CMD_TAIL)
        {
            //LED2_ON;
            cmd_size = cmd_pos;                                       //指令字节长度
            cmd_state = 0;                                            //重新检测帧尾巴
            cmd_pos = 0;                                              //复位指令指针

#if(CRC16_ENABLE)
            //去掉指令头尾EE，尾FFFCFFFF共计5个字节，只计算数据部分CRC
            if(!CheckCRC16(buffer+1,cmd_size-5))                      //CRC校验
                return 0;

            cmd_size -= 2;                                            //去掉CRC16（2字节）
#endif
            return cmd_size;
        }
    }
    return 0;                                                         //没有形成完整的一帧
}





/*! 
*  \brief  消息处理流程
*  \param msg 待处理消息
*  \param size 消息长度
*/
void ProcessMessage( PCTRL_MSG msg, u16 size )
{
	
	
	
	
    u8 cmd_type = msg->cmd_type;                                                  //指令类型
    u8 ctrl_msg = msg->ctrl_msg;                                                  //消息的类型
    u8 control_type = msg->control_type;                                          //控件类型

    u16 screen_id = msg->screen_id;
     u16 control_id = msg->control_id;	
	 
     screen_id = PTR2U16(&screen_id);                                      //画面ID
     control_id = PTR2U16(&control_id);                                    //控件ID
      u32 value = PTR2U32(msg->param);                                     //数值                                            //数值


    	// Rs4851send_int16_Data_1(control_id);  
	  

    switch(cmd_type)
    {  
    case NOTIFY_TOUCH_PRESS:                                                        //触摸屏按下
    case NOTIFY_TOUCH_RELEASE:                                                      //触摸屏松开
        NotifyTouchXY(cmd_buffer[1],PTR2U16(cmd_buffer+2),PTR2U16(cmd_buffer+4)); 
        break;                                                                    
    case NOTIFY_WRITE_FLASH_OK:                                                     //写FLASH成功
        NotifyWriteFlash(1);                                                      
        break;                                                                    
    case NOTIFY_WRITE_FLASH_FAILD:                                                  //写FLASH失败
        NotifyWriteFlash(0);                                                      
        break;                                                                    
    case NOTIFY_READ_FLASH_OK:                                                      //读取FLASH成功
        NotifyReadFlash(1,cmd_buffer+2,size-6);                                     //去除帧头帧尾
        break;                                                                    
    case NOTIFY_READ_FLASH_FAILD:                                                   //读取FLASH失败
        NotifyReadFlash(0,0,0);                                                   
        break;                                                                    
    case NOTIFY_READ_RTC:                                                           //读取RTC时间
        NotifyReadRTC(cmd_buffer[2],cmd_buffer[3],cmd_buffer[4],cmd_buffer[5],cmd_buffer[6],cmd_buffer[7],cmd_buffer[8]);
        break;
    case NOTIFY_CONTROL:
        {
            if(ctrl_msg==MSG_GET_CURRENT_SCREEN)                                    //画面ID变化通知
            {
                NotifyScreen(screen_id);                                            //画面切换调动的函数
            }
            else
            {
                switch(control_type)
                {
                case kCtrlButton:                                                    //按钮控件
                    NotifyButton(screen_id,control_id,msg->param[1]);                  
                    break;                                                             
                case kCtrlText:                                                     //文本控件
                    NotifyText(screen_id,control_id,msg->param);                       
                    break;                                                             
                case kCtrlProgress:                                                 //进度条控件
                    NotifyProgress(screen_id,control_id,value);                        
                    break;                                                             
                case kCtrlSlider:                                                   //滑动条控件
                    NotifySlider(screen_id,control_id,value);                          
                    break;                                                             
                case kCtrlMeter:                                                    //仪表控件
                    NotifyMeter(screen_id,control_id,value);                           
                    break;                                                             
                case kCtrlMenu:                                                     //菜单控件
                    NotifyMenu(screen_id,control_id,msg->param[0],msg->param[1]);      
                    break;                                                              
                case kCtrlSelector:                                                 //选择控件
                    NotifySelector(screen_id,control_id,msg->param[0]);                
                    break;                                                              
                case kCtrlRTC:                                                      //倒计时控件
                    NotifyTimer(screen_id,control_id);
                    break;
                default:
                    break;
                }
            } 
            break;  
        } 
    default:
        break;
    }
}

/*! 
*  \brief  画面切换通知
*  \details  当前画面改变时(或调用GetScreen)，执行此函数
*  \param screen_id 当前画面ID
*/
void NotifyScreen(u16 screen_id)
{
    //TODO: 添加用户代码
    current_screen_id = screen_id;                                                   //在工程配置中开启画面切换通知，记录当前画面ID

	
//	Rs4851send_int16_Data_1(screen_id);
	
	
	
    //进到画面3亮起一个按钮
    if(screen_id == 3)
    {
//        SetButtonValue(3,1,1);
    }
    //进到画面自动播放GIF
    if(current_screen_id == 9)
    {
//        AnimationStart(9,1);                                                         //动画开始播放
    }

    //进到二维码页面生成二维码
    if(current_screen_id==14)                                   
    {
        //二维码控件显示中文字符时，需要转换为UTF8编码，
        //通过“指令助手”，转换“www.gz-dc.com” ，得到字符串编码如下
//        u8 dat[] = {0x77,0x77,0x77,0x2E,0x67,0x7A,0x2D,0x64,0x63,0x2E,0x63,0x6F,0x6D};
//        SetTextValue(14,1,dat);                                                      //发送二维码字符编码                     
    }

    //数据记录显示
    if(current_screen_id == 15)
    {
//        Record_SetEvent(15,1,0,0);  
//        Record_SetEvent(15,1,1,0);
//        Record_SetEvent(15,1,2,0);
//        Record_SetEvent(15,1,3,0);
//        Record_SetEvent(15,1,4,0);
//        Record_SetEvent(15,1,5,0);
//        Record_SetEvent(15,1,6,0);
//        Record_SetEvent(15,1,7,0);
//        delay_ms(2000);                                                              //延时两秒
//        Record_ResetEvent(15,1,0,0);
//        Record_ResetEvent(15,1,1,0);
//        Record_ResetEvent(15,1,2,0);

    }
    //进入音乐画面自动播放
    if(current_screen_id == 18)   
    {
//        u8 buffer[19] = {0x94,0x61 ,0x3A ,0x2F ,0x73 ,0x6F ,0x75 ,0x6E ,0x64 ,0x73 ,0x2F , 0x30 ,0x31 ,0x2E ,0x6D ,0x70 ,0x33};
//        SetButtonValue(18,3,1);
//        PlayMusic(buffer);                                                           //播放音乐
    }
}

/*! 
*  \brief  触摸坐标事件响应
*  \param press 1按下触摸屏，3松开触摸屏
*  \param x x坐标
*  \param y y坐标
*/
void NotifyTouchXY(u8 press,u16 x,u16 y)
{ 
    //TODO: 添加用户代码
}


/*! 
*  \brief  更新数据
*/ 
void UpdateUI()
{

}

/*! 
*  \brief  按钮控件通知
*  \details  当按钮状态改变(或调用GetControlValue)时，执行此函数
*  \param screen_id 画面ID
*  \param control_id 控件ID
*  \param state 按钮状态：0弹起，1按下
*/
void NotifyButton(u16 screen_id, u16 control_id, u8  state)
{ 
 

/*
*主控页面 
*
*	
*/

 static  u8 state_biao_11=1;//=1 可以立马反应出来问题

  if(screen_id == 0)
    {
		 if(control_id==5) //泵1启动                                                           
			{ 	  
			  if((state==1)&&(state_biao_11==1))  //state 函数自带的参数 按下=1；弹起=0；
				  {
				   state_biao_11=0;
				   open1=1;					  
				  }
			  if(state==0)  state_biao_11=1;	 //state 按钮状态：0弹起，1按下 
				  
			 }  
				  
			  
		   if(control_id==6)//泵1停止                                                     
				{

				if((state==1)&&(state_biao_11==1))  //state 函数自带的参数 按下=1；弹起=0；
				  {
				   state_biao_11=0;
					 stop1=1;  	  
				  
				  }
			  if(state==0)  state_biao_11=1;	  	
	
				}



			if(control_id==7) //泵2启动                                                          
				{		
				
			 if((state==1)&&(state_biao_11==1))  //state 函数自带的参数 按下=1；弹起=0；
				  {
				   state_biao_11=0;
				   open2=1;	
					
				  }
			  if(state==0)  state_biao_11=1;	 //state 按钮状态：0弹起，1按下 

				}
				
				
			if(control_id==8) //泵2停止                                                      
				{
	
         if((state==1)&&(state_biao_11==1))  //state 函数自带的参数 按下=1；弹起=0；
				  {
				   state_biao_11=0;
					 stop2=1;  	  
				  
				  }
			  if(state==0)  state_biao_11=1;	  	

				}


			if(control_id==9) //泵3启动                                                          
				{	
               		
			 if((state==1)&&(state_biao_11==1))  //state 函数自带的参数 按下=1；弹起=0；
				  {
				   state_biao_11=0;
				   open3=1;					  
				  }
			  if(state==0)  state_biao_11=1;	 //state 按钮状态：0弹起，1按下 
	
				}
				
				
			if(control_id==10) //泵3停止                                                       
				{
					
				if((state==1)&&(state_biao_11==1))  //state 函数自带的参数 按下=1；弹起=0；
				  {
				   state_biao_11=0;
					 stop3=1;  	  
				  
				  }
			  if(state==0)  state_biao_11=1;	  	
				
					
	
				}

				
				
				
		if(control_id==49) //阀1启动                                                           
			{ 	  
			  if((state==1)&&(state_biao_11==1))  //state 函数自带的参数 按下=1；弹起=0；
				  {
				   state_biao_11=0;
				   valve_open1=1;					  
				  }
			  if(state==0)  state_biao_11=1;	 //state 按钮状态：0弹起，1按下 
				  
			 }  
				
				
		if(control_id==50) //阀1关闭                                                     
				{

				if((state==1)&&(state_biao_11==1))  //state 函数自带的参数 按下=1；弹起=0；
				  {
				   state_biao_11=0;
					 valve_stop1=1;  	  
				  
				  }
			  if(state==0)  state_biao_11=1;	  	
	
				}		
				
				
				
   if(control_id==51) //阀2启动                                                           
			{ 	  
			  if((state==1)&&(state_biao_11==1))  //state 函数自带的参数 按下=1；弹起=0；
				  {
				   state_biao_11=0;
				   valve_open2=1;					  
				  }
			  if(state==0)  state_biao_11=1;	 //state 按钮状态：0弹起，1按下 
				  
			 }  
				
				
		if(control_id==52) //阀2关闭                                                     
				{

				if((state==1)&&(state_biao_11==1))  //state 函数自带的参数 按下=1；弹起=0；
				  {
				   state_biao_11=0;
					 valve_stop2=1;  	  
				  
				  }
			  if(state==0)  state_biao_11=1;	  	
	
				}						
				
				



   if(control_id==53) //阀3启动                                                           
			{ 	  
			  if((state==1)&&(state_biao_11==1))  //state 函数自带的参数 按下=1；弹起=0；
				  {
				   state_biao_11=0;
				   valve_open3=1;					  
				  }
			  if(state==0)  state_biao_11=1;	 //state 按钮状态：0弹起，1按下 
				  
			 }  
				
				
		if(control_id==54) //阀3关闭                                                     
				{

				if((state==1)&&(state_biao_11==1))  //state 函数自带的参数 按下=1；弹起=0；
				  {
				   state_biao_11=0;
					 valve_stop3=1;  	  
				  
				  }
			  if(state==0)  state_biao_11=1;	  	
	
				}						
				
		
    }



	
	

  if(screen_id == 2)
    {
     


  if(control_id==35) //自动启停                                                      
        {		
			
			  if((state==1)&&(state_biao_11==1))  //state 函数自带的参数 按下=1；弹起=0；
				  {
				   state_biao_11=0;
					  
				   auto_button_1=!auto_button_1;
					write_eepall_1(1) ;  
					
				  }
			  if(state==0)  state_biao_11=1;
			
			  if(auto_button_1==0) AnimationPlayFrame(2,32,1);
			  else  AnimationPlayFrame(2,32,0);			
			  
			
        }
   

		
	if(control_id==36) //自动启停                                                         
        {		
			  
			  
			
			  if((state==1)&&(state_biao_11==1))
				  {
				   state_biao_11=0;
					  
				   auto_button_2=!auto_button_2;
					 write_eepall_1(1) ;  
					//store_bridge(0);  
				  }
			  if(state==0)  state_biao_11=1;
				  
				  
			
			  if(auto_button_2==0) AnimationPlayFrame(2,33,1);
			  else  AnimationPlayFrame(2,33,0);			
			  
			
        }	
		
		
		
		
		
	if(control_id==37) //自动启停                                                          
        {		
			  
			  
			
			  if((state==1)&&(state_biao_11==1))
				  {
				   
					 state_biao_11=0;					  
				   auto_button_3=!auto_button_3;
					 write_eepall_1(1) ;  
						
				  }
			  if(state==0)  state_biao_11=1;
			
			  if(auto_button_3==0) AnimationPlayFrame(2,34,1);
			  else  AnimationPlayFrame(2,34,0);			
			  
			
        }	
	
		
    }

		
		
		
	 if(screen_id == 3)
    {
     


  if(control_id==12) //自动启停                                                      
        {		
			
			  if((state==1)&&(state_biao_11==1))  //state 函数自带的参数 按下=1；弹起=0；
				  {
				   state_biao_11=0;					  
				   protection_switch_1=!protection_switch_1;
					 write_eepall_1(1) ;					
				  }
			  if(state==0)  state_biao_11=1;			
			  if(protection_switch_1==1) AnimationPlayFrame(3,11,1);
			  else  AnimationPlayFrame(3,11,0);			  
			
        }
 
		
	if(control_id==14) //自动启停                                                         
        {		
			
			  if((state==1)&&(state_biao_11==1))
				  {
				   state_biao_11=0;					  
				   protection_switch_2=!protection_switch_2;
					 write_eepall_1(1) ;  
					//store_bridge(0);  
				  }
			  if(state==0)  state_biao_11=1;					
			  if(protection_switch_2==1) AnimationPlayFrame(3,13,1);
			  else  AnimationPlayFrame(3,13,0);			
			  
			
        }	
		
		
		
	if(control_id==16) //自动启停                                                          
        {		
			
			  if((state==1)&&(state_biao_11==1))
				  {				   
					 state_biao_11=0;					  
				   protection_switch_3=!protection_switch_3;
					 write_eepall_1(1) ; 						
				  }
			  if(state==0)  state_biao_11=1;			
			  if(protection_switch_3==1) AnimationPlayFrame(3,15,1);
			  else  AnimationPlayFrame(3,15,0);				  
			
        }	
		
    }
	
		
		
	

  if(screen_id == 5)
    {
     

			
					if(control_id==9)//自动                                                 
        {		
			  
			  
			
			  if((state==1)&&(state_biao_11==1))
				  {
				   state_biao_11=0;
					  
				   kongzhi_fangshi=0;
					 write_eepall_1(1) ;  
					//store_bridge(0);  
				  }
			  if(state==0)  state_biao_11=1;
				  
				  
			  if(kongzhi_fangshi==1) {AnimationPlayFrame(5,8,0); AnimationPlayFrame(5,10,1);}
			  else  {AnimationPlayFrame(5,8,1); AnimationPlayFrame(5,10,0);}		
			 
			  
			
        }	
			
			
			
	if(control_id==11) //手动                                                     
        {		
			  
			  
			
			  if((state==1)&&(state_biao_11==1))
				  {
				   state_biao_11=0;
					  
				   kongzhi_fangshi=1;
					 write_eepall_1(1) ;  
					//store_bridge(0);  
				  }
			  if(state==0)  state_biao_11=1;
				  
				  
			
			  if(kongzhi_fangshi==1) {AnimationPlayFrame(5,8,0); AnimationPlayFrame(5,10,1);}
			  else  {AnimationPlayFrame(5,8,1); AnimationPlayFrame(5,10,0);}		
			  
			
        }	
			
			
			
			
			
		
    }


	
}

/*! 
*  \brief  文本控件通知
*  \details  当文本通过键盘更新(或调用GetControlValue)时，执行此函数
*  \details  文本控件的内容以字符串形式下发到MCU，如果文本控件内容是浮点值，
*  \details  则需要在此函数中将下发字符串重新转回浮点值。
*  \param screen_id 画面ID
*  \param control_id 控件ID
*  \param str 文本控件内容
*/
void NotifyText(u16 screen_id, u16 control_id, u8 *str)
{
  

 if(screen_id==0)                                                                 //常用设置
    {                                                                            
        u16 value=0;                                                            
       
         sscanf(str,"%ld",&value);
		

		   
       if(control_id==47)                                                           
			{  

				if(shuiwei_xinhao==1) PresentLevel=value; 
	 			
			}   


			
		
    }
	
	
    if(screen_id==2)                                                                 //常用设置
    {                                                                            
        u16 value=0;                                                            
        u8 i=0;
         sscanf(str,"%ld",&value);
	

   
		for(i=2;i<=31;i++)
		
		{
	                                                       
			if(control_id==i)                                                            
			{   
							
			   //SetTextInt32(2,(i),value,0,1); 
         if(i%2==0)
        {					
				  
				  if(value!=set_leve[(i/2)-1]) 
				  { 
				   set_leve[(i/2)-1]=	value;
				   write_eepall_1(1);  
				  } 
					 
					 
				 }
				
				 if(i%2==1)
				 {
                  
					if(set_time[(i/2)-1]!=	value) 
					 
					 set_time[(i/2)-1]=	value;
           write_eepall_1(1);  					
				 }	 
			} 
			
		
		}
		

			
    }



 if(screen_id==3)                                                                 //电机参数
    {                                                                            
        u16 value=0;                                                            
        sscanf(str,"%ld",&value);  





        if(control_id==8)                                                           
			{  

        if(value!=A_current)  
				
			   { 
				  A_current=value;	  
				  
				}		
  			
			}   


        if(control_id==9)                                                           
			{  

         if(value!=B_current)  
				
			   { 
				  B_current=value;	  
				  
				}		
  			
			}   




        if(control_id==10)                                                           
			{  

         if(value!=C_current)  
				
			   { 
				   C_current=value;	  
				  
				}		
  			
			}   







        if(control_id==2)                                                           
			{                                                   
				 

			  if(value!=ALoadCurrent)  
				{ 
				  ALoadCurrent=value;
				  write_eepall_1(1);
	
				 
				}		  

       SetTextInt32(3,2,ALoadCurrent,0,1);
			
				
			} 
        if(control_id==3) 

			{ 

        if(value!=BLoadCurrent)  
				{ 
				  BLoadCurrent=value;	  
				  write_eepall_1(1) ;
				}		  

           
				
				SetTextInt32(3,3,BLoadCurrent,0,1); 			
			}
			
        if(control_id==4)                                                           
			{  

              if(value!=CLoadCurrent)  
				{ 
				  CLoadCurrent=value;	  
				  write_eepall_1(1) ;
				}		
                
				
				SetTextInt32(3,4,CLoadCurrent,0,1); 			
			}   




			
    }



//  start_N       =storage_1[3];
//  absence_DelayT=storage_1[4];
//  up_waterT     =storage_1[5];	 
//  NOwater_T     =storage_1[6]; 	
//	



	
	
if(screen_id==5)                                                                 //控制设置
    {                                                                            
        u16 value=0;                                                            
        sscanf(str,"%ld",&value);                                                   
        if(control_id==2)                                                          
			{                                                   
				
				if(value!=CLoadCurrent)  
				{ 
				  start_N=value;	  
				  write_eepall_1(1) ;
				}		
				
				
				SetTextInt32(5,2,start_N,0,1); 			
			} 
        if(control_id==3)                                                           
			{                                                   
				
				if(value!=absence_DelayT)  
				{ 
				  absence_DelayT=value;	  
				  write_eepall_1(1) ;
				}	
				
				SetTextInt32(5,3,absence_DelayT,0,1); 			
			}      
        if(control_id==4)                                                            
			{                                                   
				
				
				if(value!=up_waterT)   
				{ 
				  up_waterT=value;	  
				  write_eepall_1(1) ;
				}	
				
				SetTextInt32(5,4,up_waterT,0,1); 			
			} 

        if(control_id==5)                                                            
			{                                                   
				
				
				if(value!=NOwater_T)  
				{ 
				  NOwater_T=value;	  
				  write_eepall_1(1) ;
				}	
				SetTextInt32(5,5,NOwater_T,0,1); 			
			}      		

   if(control_id==6)                                                            
			{                                                   
				
				
				if(value!=valve_travel_time)  
				{ 
				  valve_travel_time=value;	  
				  write_eepall_1(1) ;
				}	
				SetTextInt32(5,6,valve_travel_time,0,1); 			
			}      		

			
		if(control_id==7)                                                            
			{                                                   
				
				
				if(value!=biaoding_shuiwei)  
				{ 
				  biaoding_shuiwei=value;	  
				  write_eepall_1(1) ;
				}	
				SetTextInt32(5,7,biaoding_shuiwei,0,1); 			
			}      		
	
	
     		
    }
	
	
	
	
	



	
}                                                                                

/*!                                                                              
*  \brief  进度条控件通知                                                       
*  \details  调用GetControlValue时，执行此函数                                  
*  \param screen_id 画面ID                                                      
*  \param control_id 控件ID                                                     
*  \param value 值                                                              
*/                                                                              
void NotifyProgress(u16 screen_id, u16 control_id, u32 value)           
{  
    //TODO: 添加用户代码   
}                                                                                

/*!                                                                              
*  \brief  滑动条控件通知                                                       
*  \details  当滑动条改变(或调用GetControlValue)时，执行此函数                  
*  \param screen_id 画面ID                                                      
*  \param control_id 控件ID                                                     
*  \param value 值                                                              
*/                                                                              
void NotifySlider(u16 screen_id, u16 control_id, u32 value)             
{                                                             
   
}

/*! 
*  \brief  仪表控件通知
*  \details  调用GetControlValue时，执行此函数
*  \param screen_id 画面ID
*  \param control_id 控件ID
*  \param value 值
*/
void NotifyMeter(u16 screen_id, u16 control_id, u32 value)
{
    //TODO: 添加用户代码
}

/*! 
*  \brief  菜单控件通知
*  \details  当菜单项按下或松开时，执行此函数
*  \param screen_id 画面ID
*  \param control_id 控件ID
*  \param item 菜单项索引
*  \param state 按钮状态：0松开，1按下
*/
void NotifyMenu(u16 screen_id, u16 control_id, u8 item, u8 state)
{
    //TODO: 添加用户代码
}

/*! 
*  \brief  选择控件通知
*  \details  当选择控件变化时，执行此函数
*  \param screen_id 画面ID
*  \param control_id 控件ID
*  \param item 当前选项
*/
void NotifySelector(u16 screen_id, u16 control_id, u8  item)
{
  
}

/*! 
*  \brief  定时器超时通知处理
*  \param screen_id 画面ID
*  \param control_id 控件ID
*/
void NotifyTimer(u16 screen_id, u16 control_id)
{

}

/*! 
*  \brief  读取用户FLASH状态返回
*  \param status 0失败，1成功
*  \param _data 返回数据
*  \param length 数据长度
*/
void NotifyReadFlash(u8 status,u8 *_data,u16 length)
{
    //TODO: 添加用户代码
}

/*! 
*  \brief  写用户FLASH状态返回
*  \param status 0失败，1成功
*/
void NotifyWriteFlash(u8 status)
{
    //TODO: 添加用户代码
}

/*! 
*  \brief  读取RTC时间，注意返回的是BCD码
*  \param year 年（BCD）
*  \param month 月（BCD）
*  \param week 星期（BCD）
*  \param day 日（BCD）
*  \param hour 时（BCD）
*  \param minute 分（BCD）
*  \param second 秒（BCD）
*/
void NotifyReadRTC(u8 year,u8 month,u8 week,u8 day,u8 hour,u8 minute,u8 second)
{
    u8 years;
    u8 months;
    u8 weeks;
    u8 days;
    u8 hours;
    u8 minutes;
    u8 sec;
    
    sec =(0xff & (second>>4))*10 +(0xf & second);                                    //BCD码转十进制
    years =(0xff & (year>>4))*10 +(0xf & year);                                      
    months =(0xff & (month>>4))*10 +(0xf & month);                                     
    weeks =(0xff & (week>>4))*10 +(0xf & week);                                      
    days =(0xff & (day>>4))*10 +(0xf & day);                                      
    hours =(0xff & (hour>>4))*10 +(0xf & hour);                                       
    minutes =(0xff & (minute>>4))*10 +(0xf & minute);                                      
                                      
}
























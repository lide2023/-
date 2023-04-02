#include "dacaiLCD.h"
#include "delay.h"
#include "modbus.h"
#include "stdio.h"
#include "stmflash.h"
#include "control.h"




#define TX_8(P1) SEND_DATA((P1)&0xFF)                    //���͵����ֽ�
#define TX_8N(P,N) SendNU8((uint8 *)P,N)                 //����N���ֽ�
#define TX_16(P1) TX_8((P1)>>8);TX_8(P1)                 //����16λ����
#define TX_16N(P,N) SendNU16((uint16 *)P,N)              //����N��16λ����
#define TX_32(P1) TX_16((P1)>>16);TX_16((P1)&0xFFFF)     //����32λ����

#define PTR2U16(PTR) ((((u8 *)(PTR))[0]<<8)|((u8 *)(PTR))[1])  //�ӻ�����ȡ16λ����
#define PTR2U32(PTR) ((((u8 *)(PTR))[0]<<24)|(((u8 *)(PTR))[1]<<16)|(((u8 *)(PTR))[2]<<8)|((u8 *)(PTR))[3])  //�ӻ�����ȡ32λ����


#define SEND_DATA(P) SendChar(P)          //����һ���ֽ�
#define BEGIN_CMD() TX_8(0XEE)            //֡ͷ
#define END_CMD() TX_32(0XFFFCFFFF)       //֡β


#define CMD_HEAD 0XEE                                                  //֡ͷ
#define CMD_TAIL 0XFFFCFFFF 



u16 current_screen_id = 0;
u8  cmd_buffer[CMD_MAX_SIZE];  



                                           //ָ�����
 u32 cmd_state = 0;                                           //����֡β���״̬
 qsize cmd_pos = 0;                                              //��ǰָ��ָ��λ��

 QUEUE que = {0,0,0};  


void DaCai_Send_Data(u8 *buf)
{

	  while((USART1->ISR&0X40)==0){}//�ȴ�������� ���ַ�ʽ��HAL_UART_Transmit(&huart1,buf,len,1000) �ȶ�
    USART1->TDR =  *buf; 

}



void SendChar(u8 num)
    {

       DaCai_Send_Data(&num);	

     }




/*! 
*  \brief  ���ڷ������ַ���
*  \param  �ַ���
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
*  \brief     �����ı�ֵ
*  \param  screen_id ����ID
*  \param  control_id �ؼ�ID
*  \param  str �ı�ֵ
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
*  \brief     �����ı�Ϊ����ֵ��Ҫ��FIRMWARE_VER>=908
*  \param  screen_id ����ID
*  \param  control_id �ؼ�ID
*  \param  value �ı���ֵ
*  \param  sign 0-�޷��ţ�1-�з���
*  \param  fill_zero ����λ��������ʱ��ಹ��
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
*  \brief     �����ƶ�֡
*  \param  screen_id ����ID
*  \param  control_id �ؼ�ID
*  \param  frame_id ֡ID
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
*  \brief  ���ָ������
*/
void queue_reset()
{
    que._head = que._tail = 0;
    cmd_pos = cmd_state = 0;
}
/*! 
* \brief  ���ָ������
* \detial ���ڽ��յ����ݣ�ͨ���˺�������ָ����� 
*  \param  _data ָ������
*/

void queue_push(void) 
{
    
	qsize pos = (que._head+1)%QUEUE_MAX_SIZE;  
    if(pos!=que._tail)                                                //����״̬
    {
        que._data[que._head] = aRxBuffer1[0];
        que._head = pos;
    }
}

//�Ӷ�����ȡһ������
 void queue_pop(qdata* _data)
{
    if(que._tail!=que._head)                                          //�ǿ�״̬
    {
        *_data = que._data[que._tail];
        que._tail = (que._tail+1)%QUEUE_MAX_SIZE;
    }
}

//��ȡ��������Ч���ݸ���
 u16 queue_size()
{
    return ((que._head+QUEUE_MAX_SIZE-que._tail)%QUEUE_MAX_SIZE);
}
/*! 
*  \brief  ��ָ�������ȡ��һ��������ָ��
*  \param  cmd ָ����ջ�����
*  \param  buf_len ָ����ջ�������С
*  \return  ָ��ȣ�0��ʾ������������ָ��
*/

u16 queue_find_cmd(u8 *buffer,u16 buf_len)

{
    qsize cmd_size = 0;
    qdata _data = 0;

    while(queue_size()>0)
    {
        //ȡһ������
        queue_pop(&_data);

        if(cmd_pos==0&&_data!=CMD_HEAD)                               //ָ���һ���ֽڱ�����֡ͷ����������
        {
           continue;
        }
		
		
        //    LED2_ON;
        if(cmd_pos<buf_len)                                           //��ֹ���������
            
		     buffer[cmd_pos++] = _data;

        cmd_state = ((cmd_state<<8)|_data);                           //ƴ�����4���ֽڣ����һ��32λ����

        //���4���ֽ���֡βƥ�䣬�õ�����֡
        if(cmd_state==CMD_TAIL)
        {
            //LED2_ON;
            cmd_size = cmd_pos;                                       //ָ���ֽڳ���
            cmd_state = 0;                                            //���¼��֡β��
            cmd_pos = 0;                                              //��λָ��ָ��

#if(CRC16_ENABLE)
            //ȥ��ָ��ͷβEE��βFFFCFFFF����5���ֽڣ�ֻ�������ݲ���CRC
            if(!CheckCRC16(buffer+1,cmd_size-5))                      //CRCУ��
                return 0;

            cmd_size -= 2;                                            //ȥ��CRC16��2�ֽڣ�
#endif
            return cmd_size;
        }
    }
    return 0;                                                         //û���γ�������һ֡
}





/*! 
*  \brief  ��Ϣ��������
*  \param msg ��������Ϣ
*  \param size ��Ϣ����
*/
void ProcessMessage( PCTRL_MSG msg, u16 size )
{
	
	
	
	
    u8 cmd_type = msg->cmd_type;                                                  //ָ������
    u8 ctrl_msg = msg->ctrl_msg;                                                  //��Ϣ������
    u8 control_type = msg->control_type;                                          //�ؼ�����

    u16 screen_id = msg->screen_id;
     u16 control_id = msg->control_id;	
	 
     screen_id = PTR2U16(&screen_id);                                      //����ID
     control_id = PTR2U16(&control_id);                                    //�ؼ�ID
      u32 value = PTR2U32(msg->param);                                     //��ֵ                                            //��ֵ


    	// Rs4851send_int16_Data_1(control_id);  
	  

    switch(cmd_type)
    {  
    case NOTIFY_TOUCH_PRESS:                                                        //����������
    case NOTIFY_TOUCH_RELEASE:                                                      //�������ɿ�
        NotifyTouchXY(cmd_buffer[1],PTR2U16(cmd_buffer+2),PTR2U16(cmd_buffer+4)); 
        break;                                                                    
    case NOTIFY_WRITE_FLASH_OK:                                                     //дFLASH�ɹ�
        NotifyWriteFlash(1);                                                      
        break;                                                                    
    case NOTIFY_WRITE_FLASH_FAILD:                                                  //дFLASHʧ��
        NotifyWriteFlash(0);                                                      
        break;                                                                    
    case NOTIFY_READ_FLASH_OK:                                                      //��ȡFLASH�ɹ�
        NotifyReadFlash(1,cmd_buffer+2,size-6);                                     //ȥ��֡ͷ֡β
        break;                                                                    
    case NOTIFY_READ_FLASH_FAILD:                                                   //��ȡFLASHʧ��
        NotifyReadFlash(0,0,0);                                                   
        break;                                                                    
    case NOTIFY_READ_RTC:                                                           //��ȡRTCʱ��
        NotifyReadRTC(cmd_buffer[2],cmd_buffer[3],cmd_buffer[4],cmd_buffer[5],cmd_buffer[6],cmd_buffer[7],cmd_buffer[8]);
        break;
    case NOTIFY_CONTROL:
        {
            if(ctrl_msg==MSG_GET_CURRENT_SCREEN)                                    //����ID�仯֪ͨ
            {
                NotifyScreen(screen_id);                                            //�����л������ĺ���
            }
            else
            {
                switch(control_type)
                {
                case kCtrlButton:                                                    //��ť�ؼ�
                    NotifyButton(screen_id,control_id,msg->param[1]);                  
                    break;                                                             
                case kCtrlText:                                                     //�ı��ؼ�
                    NotifyText(screen_id,control_id,msg->param);                       
                    break;                                                             
                case kCtrlProgress:                                                 //�������ؼ�
                    NotifyProgress(screen_id,control_id,value);                        
                    break;                                                             
                case kCtrlSlider:                                                   //�������ؼ�
                    NotifySlider(screen_id,control_id,value);                          
                    break;                                                             
                case kCtrlMeter:                                                    //�Ǳ�ؼ�
                    NotifyMeter(screen_id,control_id,value);                           
                    break;                                                             
                case kCtrlMenu:                                                     //�˵��ؼ�
                    NotifyMenu(screen_id,control_id,msg->param[0],msg->param[1]);      
                    break;                                                              
                case kCtrlSelector:                                                 //ѡ��ؼ�
                    NotifySelector(screen_id,control_id,msg->param[0]);                
                    break;                                                              
                case kCtrlRTC:                                                      //����ʱ�ؼ�
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
*  \brief  �����л�֪ͨ
*  \details  ��ǰ����ı�ʱ(�����GetScreen)��ִ�д˺���
*  \param screen_id ��ǰ����ID
*/
void NotifyScreen(u16 screen_id)
{
    //TODO: ����û�����
    current_screen_id = screen_id;                                                   //�ڹ��������п��������л�֪ͨ����¼��ǰ����ID

	
//	Rs4851send_int16_Data_1(screen_id);
	
	
	
    //��������3����һ����ť
    if(screen_id == 3)
    {
//        SetButtonValue(3,1,1);
    }
    //���������Զ�����GIF
    if(current_screen_id == 9)
    {
//        AnimationStart(9,1);                                                         //������ʼ����
    }

    //������ά��ҳ�����ɶ�ά��
    if(current_screen_id==14)                                   
    {
        //��ά��ؼ���ʾ�����ַ�ʱ����Ҫת��ΪUTF8���룬
        //ͨ����ָ�����֡���ת����www.gz-dc.com�� ���õ��ַ�����������
//        u8 dat[] = {0x77,0x77,0x77,0x2E,0x67,0x7A,0x2D,0x64,0x63,0x2E,0x63,0x6F,0x6D};
//        SetTextValue(14,1,dat);                                                      //���Ͷ�ά���ַ�����                     
    }

    //���ݼ�¼��ʾ
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
//        delay_ms(2000);                                                              //��ʱ����
//        Record_ResetEvent(15,1,0,0);
//        Record_ResetEvent(15,1,1,0);
//        Record_ResetEvent(15,1,2,0);

    }
    //�������ֻ����Զ�����
    if(current_screen_id == 18)   
    {
//        u8 buffer[19] = {0x94,0x61 ,0x3A ,0x2F ,0x73 ,0x6F ,0x75 ,0x6E ,0x64 ,0x73 ,0x2F , 0x30 ,0x31 ,0x2E ,0x6D ,0x70 ,0x33};
//        SetButtonValue(18,3,1);
//        PlayMusic(buffer);                                                           //��������
    }
}

/*! 
*  \brief  ���������¼���Ӧ
*  \param press 1���´�������3�ɿ�������
*  \param x x����
*  \param y y����
*/
void NotifyTouchXY(u8 press,u16 x,u16 y)
{ 
    //TODO: ����û�����
}


/*! 
*  \brief  ��������
*/ 
void UpdateUI()
{

}

/*! 
*  \brief  ��ť�ؼ�֪ͨ
*  \details  ����ť״̬�ı�(�����GetControlValue)ʱ��ִ�д˺���
*  \param screen_id ����ID
*  \param control_id �ؼ�ID
*  \param state ��ť״̬��0����1����
*/
void NotifyButton(u16 screen_id, u16 control_id, u8  state)
{ 
 

/*
*����ҳ�� 
*
*	
*/

 static  u8 state_biao_11=1;//=1 ��������Ӧ��������

  if(screen_id == 0)
    {
		 if(control_id==5) //��1����                                                           
			{ 	  
			  if((state==1)&&(state_biao_11==1))  //state �����Դ��Ĳ��� ����=1������=0��
				  {
				   state_biao_11=0;
				   open1=1;					  
				  }
			  if(state==0)  state_biao_11=1;	 //state ��ť״̬��0����1���� 
				  
			 }  
				  
			  
		   if(control_id==6)//��1ֹͣ                                                     
				{

				if((state==1)&&(state_biao_11==1))  //state �����Դ��Ĳ��� ����=1������=0��
				  {
				   state_biao_11=0;
					 stop1=1;  	  
				  
				  }
			  if(state==0)  state_biao_11=1;	  	
	
				}



			if(control_id==7) //��2����                                                          
				{		
				
			 if((state==1)&&(state_biao_11==1))  //state �����Դ��Ĳ��� ����=1������=0��
				  {
				   state_biao_11=0;
				   open2=1;	
					
				  }
			  if(state==0)  state_biao_11=1;	 //state ��ť״̬��0����1���� 

				}
				
				
			if(control_id==8) //��2ֹͣ                                                      
				{
	
         if((state==1)&&(state_biao_11==1))  //state �����Դ��Ĳ��� ����=1������=0��
				  {
				   state_biao_11=0;
					 stop2=1;  	  
				  
				  }
			  if(state==0)  state_biao_11=1;	  	

				}


			if(control_id==9) //��3����                                                          
				{	
               		
			 if((state==1)&&(state_biao_11==1))  //state �����Դ��Ĳ��� ����=1������=0��
				  {
				   state_biao_11=0;
				   open3=1;					  
				  }
			  if(state==0)  state_biao_11=1;	 //state ��ť״̬��0����1���� 
	
				}
				
				
			if(control_id==10) //��3ֹͣ                                                       
				{
					
				if((state==1)&&(state_biao_11==1))  //state �����Դ��Ĳ��� ����=1������=0��
				  {
				   state_biao_11=0;
					 stop3=1;  	  
				  
				  }
			  if(state==0)  state_biao_11=1;	  	
				
					
	
				}

				
				
				
		if(control_id==49) //��1����                                                           
			{ 	  
			  if((state==1)&&(state_biao_11==1))  //state �����Դ��Ĳ��� ����=1������=0��
				  {
				   state_biao_11=0;
				   valve_open1=1;					  
				  }
			  if(state==0)  state_biao_11=1;	 //state ��ť״̬��0����1���� 
				  
			 }  
				
				
		if(control_id==50) //��1�ر�                                                     
				{

				if((state==1)&&(state_biao_11==1))  //state �����Դ��Ĳ��� ����=1������=0��
				  {
				   state_biao_11=0;
					 valve_stop1=1;  	  
				  
				  }
			  if(state==0)  state_biao_11=1;	  	
	
				}		
				
				
				
   if(control_id==51) //��2����                                                           
			{ 	  
			  if((state==1)&&(state_biao_11==1))  //state �����Դ��Ĳ��� ����=1������=0��
				  {
				   state_biao_11=0;
				   valve_open2=1;					  
				  }
			  if(state==0)  state_biao_11=1;	 //state ��ť״̬��0����1���� 
				  
			 }  
				
				
		if(control_id==52) //��2�ر�                                                     
				{

				if((state==1)&&(state_biao_11==1))  //state �����Դ��Ĳ��� ����=1������=0��
				  {
				   state_biao_11=0;
					 valve_stop2=1;  	  
				  
				  }
			  if(state==0)  state_biao_11=1;	  	
	
				}						
				
				



   if(control_id==53) //��3����                                                           
			{ 	  
			  if((state==1)&&(state_biao_11==1))  //state �����Դ��Ĳ��� ����=1������=0��
				  {
				   state_biao_11=0;
				   valve_open3=1;					  
				  }
			  if(state==0)  state_biao_11=1;	 //state ��ť״̬��0����1���� 
				  
			 }  
				
				
		if(control_id==54) //��3�ر�                                                     
				{

				if((state==1)&&(state_biao_11==1))  //state �����Դ��Ĳ��� ����=1������=0��
				  {
				   state_biao_11=0;
					 valve_stop3=1;  	  
				  
				  }
			  if(state==0)  state_biao_11=1;	  	
	
				}						
				
		
    }



	
	

  if(screen_id == 2)
    {
     


  if(control_id==35) //�Զ���ͣ                                                      
        {		
			
			  if((state==1)&&(state_biao_11==1))  //state �����Դ��Ĳ��� ����=1������=0��
				  {
				   state_biao_11=0;
					  
				   auto_button_1=!auto_button_1;
					write_eepall_1(1) ;  
					
				  }
			  if(state==0)  state_biao_11=1;
			
			  if(auto_button_1==0) AnimationPlayFrame(2,32,1);
			  else  AnimationPlayFrame(2,32,0);			
			  
			
        }
   

		
	if(control_id==36) //�Զ���ͣ                                                         
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
		
		
		
		
		
	if(control_id==37) //�Զ���ͣ                                                          
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
     


  if(control_id==12) //�Զ���ͣ                                                      
        {		
			
			  if((state==1)&&(state_biao_11==1))  //state �����Դ��Ĳ��� ����=1������=0��
				  {
				   state_biao_11=0;					  
				   protection_switch_1=!protection_switch_1;
					 write_eepall_1(1) ;					
				  }
			  if(state==0)  state_biao_11=1;			
			  if(protection_switch_1==1) AnimationPlayFrame(3,11,1);
			  else  AnimationPlayFrame(3,11,0);			  
			
        }
 
		
	if(control_id==14) //�Զ���ͣ                                                         
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
		
		
		
	if(control_id==16) //�Զ���ͣ                                                          
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
     

			
					if(control_id==9)//�Զ�                                                 
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
			
			
			
	if(control_id==11) //�ֶ�                                                     
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
*  \brief  �ı��ؼ�֪ͨ
*  \details  ���ı�ͨ�����̸���(�����GetControlValue)ʱ��ִ�д˺���
*  \details  �ı��ؼ����������ַ�����ʽ�·���MCU������ı��ؼ������Ǹ���ֵ��
*  \details  ����Ҫ�ڴ˺����н��·��ַ�������ת�ظ���ֵ��
*  \param screen_id ����ID
*  \param control_id �ؼ�ID
*  \param str �ı��ؼ�����
*/
void NotifyText(u16 screen_id, u16 control_id, u8 *str)
{
  

 if(screen_id==0)                                                                 //��������
    {                                                                            
        u16 value=0;                                                            
       
         sscanf(str,"%ld",&value);
		

		   
       if(control_id==47)                                                           
			{  

				if(shuiwei_xinhao==1) PresentLevel=value; 
	 			
			}   


			
		
    }
	
	
    if(screen_id==2)                                                                 //��������
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



 if(screen_id==3)                                                                 //�������
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



	
	
if(screen_id==5)                                                                 //��������
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
*  \brief  �������ؼ�֪ͨ                                                       
*  \details  ����GetControlValueʱ��ִ�д˺���                                  
*  \param screen_id ����ID                                                      
*  \param control_id �ؼ�ID                                                     
*  \param value ֵ                                                              
*/                                                                              
void NotifyProgress(u16 screen_id, u16 control_id, u32 value)           
{  
    //TODO: ����û�����   
}                                                                                

/*!                                                                              
*  \brief  �������ؼ�֪ͨ                                                       
*  \details  ���������ı�(�����GetControlValue)ʱ��ִ�д˺���                  
*  \param screen_id ����ID                                                      
*  \param control_id �ؼ�ID                                                     
*  \param value ֵ                                                              
*/                                                                              
void NotifySlider(u16 screen_id, u16 control_id, u32 value)             
{                                                             
   
}

/*! 
*  \brief  �Ǳ�ؼ�֪ͨ
*  \details  ����GetControlValueʱ��ִ�д˺���
*  \param screen_id ����ID
*  \param control_id �ؼ�ID
*  \param value ֵ
*/
void NotifyMeter(u16 screen_id, u16 control_id, u32 value)
{
    //TODO: ����û�����
}

/*! 
*  \brief  �˵��ؼ�֪ͨ
*  \details  ���˵���»��ɿ�ʱ��ִ�д˺���
*  \param screen_id ����ID
*  \param control_id �ؼ�ID
*  \param item �˵�������
*  \param state ��ť״̬��0�ɿ���1����
*/
void NotifyMenu(u16 screen_id, u16 control_id, u8 item, u8 state)
{
    //TODO: ����û�����
}

/*! 
*  \brief  ѡ��ؼ�֪ͨ
*  \details  ��ѡ��ؼ��仯ʱ��ִ�д˺���
*  \param screen_id ����ID
*  \param control_id �ؼ�ID
*  \param item ��ǰѡ��
*/
void NotifySelector(u16 screen_id, u16 control_id, u8  item)
{
  
}

/*! 
*  \brief  ��ʱ����ʱ֪ͨ����
*  \param screen_id ����ID
*  \param control_id �ؼ�ID
*/
void NotifyTimer(u16 screen_id, u16 control_id)
{

}

/*! 
*  \brief  ��ȡ�û�FLASH״̬����
*  \param status 0ʧ�ܣ�1�ɹ�
*  \param _data ��������
*  \param length ���ݳ���
*/
void NotifyReadFlash(u8 status,u8 *_data,u16 length)
{
    //TODO: ����û�����
}

/*! 
*  \brief  д�û�FLASH״̬����
*  \param status 0ʧ�ܣ�1�ɹ�
*/
void NotifyWriteFlash(u8 status)
{
    //TODO: ����û�����
}

/*! 
*  \brief  ��ȡRTCʱ�䣬ע�ⷵ�ص���BCD��
*  \param year �꣨BCD��
*  \param month �£�BCD��
*  \param week ���ڣ�BCD��
*  \param day �գ�BCD��
*  \param hour ʱ��BCD��
*  \param minute �֣�BCD��
*  \param second �루BCD��
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
    
    sec =(0xff & (second>>4))*10 +(0xf & second);                                    //BCD��תʮ����
    years =(0xff & (year>>4))*10 +(0xf & year);                                      
    months =(0xff & (month>>4))*10 +(0xf & month);                                     
    weeks =(0xff & (week>>4))*10 +(0xf & week);                                      
    days =(0xff & (day>>4))*10 +(0xf & day);                                      
    hours =(0xff & (hour>>4))*10 +(0xf & hour);                                       
    minutes =(0xff & (minute>>4))*10 +(0xf & minute);                                      
                                      
}
























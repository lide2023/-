#include "uartLCD.h"
#include "modbus.h"
#include "usart.h"
#include "stmflash.h"
		u8 LCD_receive_count=0;
		
		u8 LCD_Receive[30]; 
        
		u16  Lcd_Drass=0;
        u16  Lcd_Data=0;








void Clear_Buf_lcd(void)//清空缓存
{
    uint8_t i;
    
    for(i=0;i<30;i++)//
    LCD_Receive[i]=0;//缓存
    
   
}







void LCD_receive(void)


 {
		
//		u8 LCD_receive_count;
//		u8 LCD_Receive[15];    
	
	 
//	    HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_12);
	   // LCD_receive_count++;
	    LCD_Receive[ LCD_receive_count++]=aRxBuffer1[0];   
			
		if(LCD_Receive[0]==0x5A)//收到帧头	//如果从零开始会出现问题	
			  {
			   if(LCD_receive_count>=4)
			    {
			     if(LCD_Receive[1]!=0xA5) LCD_receive_count=0;
			     if(LCD_Receive[2]!=0x09) LCD_receive_count=0;		
                }
				
			  }
			  
		
         else  LCD_receive_count=0;
			
         if(LCD_receive_count>=10)
		   {
			 LCD_receive_count=10; //说明接收完一组数据	
		   }  
       } 	
	   

	   
void LCD_Reception_execute(void)
   {
		u16 temp1,temp2;
	    u8 i;
	   if(LCD_receive_count>=9)  //说明接收完一组数据
		{			   
           
			if(LCD_Receive[3]==0x83)
			 {	
				temp1=LCD_Receive[4];
				temp1=(temp1<<8)+LCD_Receive[5] ;  //temp1接收的时地址
				Lcd_Drass=temp1;			
				temp2=LCD_Receive[7];
				temp2=(temp2<<8)+LCD_Receive[8] ;   //temp2接收的地址里面的数	
			 
				Lcd_Data=temp2;	
               
//				 LCD_send_int8_Data(LCD_Receive[6]); 

//                LCD_send_int8_Data(LCD_Receive[4]); 
//			    LCD_send_int8_Data(LCD_Receive[5]); 
                // LCD_send_int16_Data(Lcd_Drass);

                LCD_send_int16_Data( Lcd_Data);
                LCD_send_int16_Data( storage_1[((Lcd_Drass-0x1000))]);
                LCD_send_int16_Data( lcd_storage_1[((Lcd_Drass-0x1000))]);

				 
//				 for(i=0;i<LCD_Receive[6];i++)
//				      {
//					   LCD_send_int16_Data( storage_1[((Lcd_Drass-0x1000)+i)]);
//					  } 

			 }	
  

			  
			  if(LCD_Receive[3]==0x82)              //查询地址变量数值5A A5 06 82 100X 0X
			   {
			   
			
                    temp1=LCD_Receive[4];
				    temp1=(temp1<<8)+LCD_Receive[5] ;  //temp1接收的时地址
				    Lcd_Drass=temp1;			

// 				     LCD_send_int8_Data(LCD_Receive[0]);
//					 LCD_send_int8_Data(LCD_Receive[1]); 
//					 LCD_send_int8_Data(LCD_Receive[2]); 
//					 LCD_send_int8_Data(LCD_Receive[3]); 
					 LCD_send_int8_Data(LCD_Receive[4]); 
					 LCD_send_int8_Data(LCD_Receive[5]); 
					 LCD_send_int8_Data(LCD_Receive[6]); 
					
				    for(i=0;i<LCD_Receive[6];i++)
				      {
					   LCD_send_int16_Data( storage_1[((Lcd_Drass-0x1000)+i)]);
					  } 
			   
			   }
          			  
		    LCD_receive_count=0;
            Read_lcd();
            
			
		  }
		  
	}  
 
	
	
//void LCD_Send_Data(u8 *buf,u8 len)
//{
//	
//	
//	
//	
// 
//	
//	if(HAL_UART_Transmit(&huart1,buf,len,1000)!= HAL_OK)//串口1发送数据 这种方式，中断接收，然后大批量放松数据，容易造成串口死机，应该是失去中断了。
//                                                      //换成  USART1->TDR =  *buf;就没有在发生，挑错，挑了6个小时才弄过来
//	{
//		while(1);
//    }
//	
//	
//}




void LCD_Send_Data(u8 *buf,u8 len)
{
	
	

	while((USART1->ISR&0X40)==0){}//等待发送完成 这种方式比HAL_UART_Transmit(&huart1,buf,len,1000) 稳定
    USART1->TDR =  *buf; 
	
	
	
	
}





void LCD_send_int8_Data(u8 num)
    {

     LCD_Send_Data(&num,1);	

     }
 
	 
void LCD_send_int16_Data(u16 num)  //发送16位数据 
  {
	u8 high_8, low_8;
	low_8 = num & 0x00FF;
	high_8 = (num>>8) & 0x00FF;
	 
	LCD_Send_Data(&high_8,1);	 
	LCD_Send_Data(&low_8,1); 
	 
   }	
   

void transter_oneword (u16 lcd_data,u16 address)	//发送一个变量
	{
	
        LCD_send_int8_Data(0x5A);
		LCD_send_int8_Data(0xA5);
		LCD_send_int8_Data(0x05);    //发送的字节数
        LCD_send_int8_Data(0x82);
		LCD_send_int16_Data(address);//发送的地址 
		LCD_send_int16_Data(lcd_data);    //发送给屏的数据
			   
	 }



	 
 void Read_lcd(void)//从LCD中读取数据对应地址写到对应的变量中，要和显示的对应起来
  {
     u8 i;
	  for(i=0;i<100;i++)
	  {
	  
	  if(Lcd_Drass==(0x1000+i)) lcd_storage_1[i]=Lcd_Data;
		
	  if(lcd_storage_1[i]!=storage_1[i]) 
	    { 
	      storage_1[i]=lcd_storage_1[i];	  
	      write_eepall_1(0) ;
	    }		  
	

	  }
   
  }


  
#include "modbus.h"
#include "usart.h"
#include "control.h"
#include "delay.h"
#include "uartLCD.h"
#include "ec600u.h"
#include "dacaiLCD.H"

        u8 aRxBuffer[1];
		u8 aRxBuffer1[1];
		u8 aRxBuffer2[1];
		u8 aRxBuffer3[1];
		u8 modbus_biao;
		u8 modbus_receivec_zero;
		u8 modbus_receive_count;
		u8 modbus_Receive[20];
        u16 crc;

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
 
//	static u8 RS485_RX_CNT=0;
//	
//	static u8 RS485_RX_BUF[64];

 
		
if(huart->Instance==USART4)
	{
	
     moud_recave();
     HAL_UART_Receive_IT(&huart4, aRxBuffer, 1);  
	

		
	}
	
	
	if(huart->Instance==USART1)
	{
		
		
		
		     LCD_receive();
		
		
		
           queue_push();
		
		  //  HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_12); 		
		  HAL_UART_Receive_IT(&huart1, aRxBuffer1, 1);//开中断，这个顺序感觉比较好
//	     if (HAL_UART_Receive_IT(&huart1, aRxBuffer1, 1) != HAL_OK)//在括号里面加程序不好用，这是个坑，以为这个是判断中断等待中断完成，去执行，结果不执行
//           {
//			 
//			   
//            }
	
		
	}
	
	if(huart->Instance==USART2)
	{
		cat1_receive(); 
		HAL_UART_Receive_IT(&huart2, aRxBuffer2, 1);   //继续开启接收中断1字节进入中断
	}
	
	
}


void USART_Send_Data_1(u8 *buf,u8 len)
{
	
//	HAL_UART_Transmit(&huart4,buf,len,1000);

	while((USART4->ISR&0X40)==0){}//等待发送完成 这种方式比HAL_UART_Transmit(&huart1,buf,len,1000) 稳定
  USART4->TDR =  *buf; 
	
}



void send_int8_Data_1(u8 num)
    {

     USART_Send_Data_1(&num,1);	

     }


void send_int16_Data_1(u16 num)  //发送16位数据 
  {
	u8 high_8, low_8;
	low_8 = num & 0x00FF;
	high_8 = (num>>8) & 0x00FF;
	 
	USART_Send_Data_1(&high_8,1);	 
	USART_Send_Data_1(&low_8,1); 
	
	 
   }	
   
   
   




void moud_recave(void) //放到接收中断里面
	
{ 
//      u8 modbus_biao;
//		u8 modbus_receivec_zero;
//		u8 modbus_receive_count;
//		u8 modbus_Receive[20];
	
	
	  modbus_biao=1;
	  modbus_receivec_zero=0; 
	
	  if(modbus_receive_count<=16)  
	    {	
	     modbus_Receive[modbus_receive_count++]=aRxBuffer[0]; 
        }
  			
		if(modbus_receive_count>=15)	modbus_receive_count=15; //停止接收进来的新数，等待check_modbushi()处理完数据在接受
 } 
	
 
 
 void moud_timejia(void)//放到时间中断里面,中断时间是xx毫秒
{
	
	
	  static   uint16_t T_timer1;
	
	T_timer1++;
	
	if(T_timer1>=100)
	{
	//HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
	T_timer1=0;	
	}
	
	
	if(modbus_biao==1) modbus_receivec_zero++;
	
	
//	 read_485shidu_biao++;
//	if(read_485shidu_biao>=800) read_485shidu_biao=800;
	
}	 






void moud_excute1_slave(void)  //接收到数据处理函数，

 { u16 crc;
   u16 shidu;
	

	 
	 if(modbus_receivec_zero>=10) //接收完数据10毫秒以后处理数据
   {
	      modbus_receive_count=0;
	      modbus_receivec_zero=0;		     
		    modbus_biao=0;//串口中断没有接受到数据moud_receivec_zero就停止计数
		 
//		    if(receive_count>8)////////////////////address 03 04 00 00 00 01 crc crc
//               {     
			  
	           
		          crc=crc16(modbus_Receive,7);
         
			
//		        if(modbus_Receive[0]==0x01&&modbus_Receive[1]==0x03&&(crc==((modbus_Receive[8]&0x00ff)|(0xff00&(modbus_Receive[7]<<8)))))
//		           { 
				  
                if(modbus_Receive[0]==0x01&&modbus_Receive[1]==0x03)
		           { 


					   
//								 shidu=modbus_Receive[3];
//								 shidu=shidu<<8;
//								 shidu=(shidu+modbus_Receive[4])/10;
					   
					             Win_Word_receive=modbus_Receive[3];
								 Win_Word_receive=Win_Word_receive<<8;
								 Win_Word_receive=Win_Word_receive+modbus_Receive[4];
					   
								 
//							   read_485shidu_biao=0;//湿度读取成功标志

//                               send_int8_Data_1(modbus_Receive[0]);
//					     	   send_int8_Data_1(modbus_Receive[1]);
//                               send_int8_Data_1(modbus_Receive[2]);
//	                           send_int8_Data_1(modbus_Receive[3]);
//                               send_int8_Data_1(modbus_Receive[4]);
//                               send_int8_Data_1(modbus_Receive[5]);
//                               send_int8_Data_1(modbus_Receive[6]);
                                
								modbus_Receive[4]= win_control_Word& 0x00FF;
								modbus_Receive[3]=(win_control_Word>>8)& 0x00FF;
								
								
//								 crc=crc16(modbus_Receive,8);
//		                         modbus_Receive[8]=crc>>8;
//	                             modbus_Receive[9]=crc&0xff;
								
								
								
                                RS4851_Send_Data(modbus_Receive,7);
			        
			         }	

             else  shidu=0;
		 
	
   }
 }




void moud_excute1_mast(void)  //接收到数据处理函数，

 { u16 crc;
   
	 
 if(modbus_receivec_zero>=3) //接收完数据10毫秒以后处理数据
   {
	      modbus_receive_count=0;
	      modbus_receivec_zero=0;		     
		    modbus_biao=0;//串口中断没有接受到数据moud_receivec_zero就停止计数
		 
//		    if(receive_count>8)////////////////////address 03 04 00 00 00 01 crc crc
//               {     
			  
	           
		          crc=crc16(modbus_Receive,7);
         
			
//		        if(modbus_Receive[0]==0x01&&modbus_Receive[1]==0x03&&(crc==((modbus_Receive[8]&0x00ff)|(0xff00&(modbus_Receive[7]<<8)))))
//		           { 
				  
                if(modbus_Receive[0]==0x01&&modbus_Receive[1]==0x03)
		           { 



					   
					             Win_Word_receive=modbus_Receive[3];
								 Win_Word_receive=Win_Word_receive<<8;
								 Win_Word_receive=Win_Word_receive+modbus_Receive[4];
					   
			        
			         }	

	 
	
   }
 }




 
 
void Send_control_word(void) 

{
	
	static u8 command_control[10];	
      u16 crc=0;
	
	
		    command_control[0]=0x01; 
		    command_control[1]=0x03;
			command_control[2]=0x00;
			
	
	
	
	        command_control[4]= win_control_Word& 0x00FF;
			command_control[3]=(win_control_Word>>8)& 0x00FF;
	        
		    command_control[5]=0x02;
	        
	        crc=crc16(command_control,6);			
	        command_control[7]=crc&0x00ff;
	        command_control[6]=(crc>>8)&0x00ff;
	
			RS4851_Send_Data(command_control,8);	///把接收到的数据在发送出去		 
	   
	
} 
 
 


void RS4851_Send_Data(u8 *buf,u8 len)
{
	u8 t;
  	
	DE485=1;

	delay_ms(1);
	for(t=0;t<len;t++)		//循环发送数据
	  { 
	    send_int8_Data_1(buf[t]);
	   }	 
	delay_ms(1);
	DE485=0;
//	LED485=0;
}




void Rs4851send_int16_Data_1(u16 num)  //发送16位数据 
  {

	u8 high_8, low_8;
	  
	 	DE485=1;
//	LED485=1;
	delay_ms(1); 
	  
	  
	low_8 = num & 0x00FF;
	high_8 = (num>>8) & 0x00FF;
	 
	USART_Send_Data_1(&high_8,1);	 
	USART_Send_Data_1(&low_8,1); 
		DE485=1;
//	LED485=1;
	delay_ms(1);

   }



void check_modbus(void)
       {
		        u8 di,addr;
				u8 sendBuf[20];
				u16 crc=0;
            if(modbus_receive_count>7)////////////////////address 03 00 00 00 01 crc crc
               {     
			  
	              modbus_receive_count=0;
		          crc=crc16(modbus_Receive,6);
//                addr=P2>>4;
//				        addr=~(addr&0x0f);
//				        addr=addr&0x0f;

//                di=~(P2&0X0f);
//				        di=di&0x0f;
			
		        if(modbus_Receive[0]==addr&&modbus_Receive[1]==0x03&&(crc==((modbus_Receive[7]&0x00ff)|(0xff00&(modbus_Receive[6]<<8)))))
		           { 
				
			           sendBuf[0]=modbus_Receive[0];
				       sendBuf[1]=modbus_Receive[1];
				       sendBuf[2]=0x02;
				       sendBuf[3]=0x00;
				       sendBuf[4]=di;
		               crc=crc16(sendBuf,5);
			           sendBuf[5]=crc>>8;
                       sendBuf[6]=crc&0xff;	
				      //send_num=7;
				   	  // beginsend();
			      }
				  
              }
           }




/***********************CRC校验*************************/
// CRC 高位字节值表
 u8  auchCRCHi[260] = { 
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 
0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 
0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 
0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 
0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 
0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 
0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 
0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 
0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 
0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 
0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 
0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 
0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40 
} ; 
// CRC低位字节值表
u8  auchCRCLo[260] = { 
0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06, 
0x07, 0xC7, 0x05, 0xC5, 0xC4, 0x04, 0xCC, 0x0C, 0x0D, 0xCD, 
0x0F, 0xCF, 0xCE, 0x0E, 0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09, 
0x08, 0xC8, 0xD8, 0x18, 0x19, 0xD9, 0x1B, 0xDB, 0xDA, 0x1A, 
0x1E, 0xDE, 0xDF, 0x1F, 0xDD, 0x1D, 0x1C, 0xDC, 0x14, 0xD4, 
0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3, 
0x11, 0xD1, 0xD0, 0x10, 0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3, 
0xF2, 0x32, 0x36, 0xF6, 0xF7, 0x37, 0xF5, 0x35, 0x34, 0xF4, 
0x3C, 0xFC, 0xFD, 0x3D, 0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A, 
0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38, 0x28, 0xE8, 0xE9, 0x29, 
0xEB, 0x2B, 0x2A, 0xEA, 0xEE, 0x2E, 0x2F, 0xEF, 0x2D, 0xED, 
0xEC, 0x2C, 0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26, 
0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0, 0xA0, 0x60, 
0x61, 0xA1, 0x63, 0xA3, 0xA2, 0x62, 0x66, 0xA6, 0xA7, 0x67, 
0xA5, 0x65, 0x64, 0xA4, 0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F, 
0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB, 0x69, 0xA9, 0xA8, 0x68, 
0x78, 0xB8, 0xB9, 0x79, 0xBB, 0x7B, 0x7A, 0xBA, 0xBE, 0x7E,  
0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C, 0xB4, 0x74, 0x75, 0xB5, 
0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71, 
0x70, 0xB0, 0x50, 0x90, 0x91, 0x51, 0x93, 0x53, 0x52, 0x92, 
0x96, 0x56, 0x57, 0x97, 0x55, 0x95, 0x94, 0x54, 0x9C, 0x5C, 
0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E, 0x5A, 0x9A, 0x9B, 0x5B, 
0x99, 0x59, 0x58, 0x98, 0x88, 0x48, 0x49, 0x89, 0x4B, 0x8B, 
0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C, 
0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42, 
0x43, 0x83, 0x41, 0x81, 0x80, 0x40 
} ;






u16 crc16(u8 *puchMsg, u16 usDataLen)  
{ 
 u8 uchCRCHi = 0xFF ; //* 高CRC字节初始化 
 u8 uchCRCLo = 0xFF ; //* 低CRC 字节初始化  
 unsigned long uIndex ; // CRC循环中的索引 
 while (usDataLen--) // 传输消息缓冲区 
 { 
  uIndex = uchCRCHi ^ *puchMsg++ ; // 计算CRC  
  uchCRCHi = uchCRCLo ^ auchCRCHi[uIndex] ; 
  uchCRCLo = auchCRCLo[uIndex] ; 
 } 
 return (uchCRCHi << 8 | uchCRCLo);
}


u16 crc1616(u16 *puchMsg, u16 usDataLen)  
{ 
 u8 uchCRCHi = 0xFF ; //* 高CRC字节初始化 
 u8 uchCRCLo = 0xFF ; //* 低CRC 字节初始化  
 unsigned long uIndex ; // CRC循环中的索引 
 while (usDataLen--) // 传输消息缓冲区 
 { 
  uIndex = uchCRCHi ^ ((*puchMsg++)&0xff); // 计算CRC  
  uchCRCHi = uchCRCLo ^ auchCRCHi[uIndex] ; 
  uchCRCLo = auchCRCLo[uIndex] ;  
	 
  uIndex = uchCRCHi ^ (((*puchMsg)>>8)&0xff); // 计算CRC  
  uchCRCHi = uchCRCLo ^ auchCRCHi[uIndex] ; 
  uchCRCLo = auchCRCLo[uIndex] ; 
	 
  
	 
	 
 } 
 return (uchCRCHi << 8 | uchCRCLo);
}



//high = (unit >> 8) & 0xff; //高8位
//low = unit & 0xff; //低8位



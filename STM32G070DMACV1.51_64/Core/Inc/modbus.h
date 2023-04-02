


#ifndef __MODBUS_H
#define __MODBUS_H	



#include "sys.h" 
#include "stm32g0xx_hal.h"
    extern u8 aRxBuffer[1]; 
    extern u8 aRxBuffer1[1];
	extern u8 aRxBuffer2[1];
	extern u8 aRxBuffer3[1];


	extern u8 modbus_biao;
	extern u8 modbus_receivec_zero;
	extern 	u8 modbus_receive_count;
	extern 	u8 modbus_Receive[20];
	extern  u16  crc;
	
void USART_Send_Data_1(u8 *buf,u8 len);
void send_int16_Data_1(u16 num);
//void transter_oneword_1 (u16 buf,u16 address);
void send_int8_Data_1(u8 num);
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart);	
	
	
void moud_recave(void);
void moud_timejia(void);
void moud_excute1_mast(void);
void moud_excute1_slave(void);
void RS4851_Send_Data(u8 *buf,u8 len);
u16 crc16(u8 *puchMsg, u16 usDataLen);
u16 crc1616(u16 *puchMsg, u16 usDataLen);

void Send_control_word(void) ;

void Rs4851send_int16_Data_1(u16 num) ;

#endif
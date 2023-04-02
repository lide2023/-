
#ifndef __uartLCD_H__
#define __uartLCD_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "sys.h"

	
	
	extern 	u8 LCD_receive_count;
	extern	u8 LCD_Receive[30]; 
    extern    u16  Lcd_Drass;
     extern   u16  Lcd_Data;	
	

	
	
void LCD_send_int8_Data(u8 num);	
void transter_oneword (u16 buf,u16 address);	
void LCD_receive(void);
void LCD_Reception_execute(void);	
void Read_lcd(void);
void LCD_send_int16_Data(u16 num);	
void Clear_Buf_lcd(void);
#ifdef __cplusplus
}
#endif

#endif /* __TIM_H__ */


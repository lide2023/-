#ifndef __STMFLASH_H__
#define __STMFLASH_H__
#include "sys.h"  
#include <stdbool.h> 
#include "ec600u.h"


//小容量产品16K至32K    STM32_FLASH_PAGE=31 
//中容量产品64k至128K   STM32_FLASH_PAGE=127 
//大容量产品256K至512K  STM32_FLASH_PAGE=255

//FLASH起始地址
#define STM32_FLASH_BASE 0x08000000 	//STM32 FLASH的起始地址


#define STM_SECTOR_SIZE	2048

#define STM32_FLASH_PAGE  64//STMG070 128K 每页2k 共64页 整页擦除，每次写8字节

#define CONTROLLER_ADDRESS 		(STM32_FLASH_PAGE-1)*STM_SECTOR_SIZE+STM32_FLASH_BASE

#define CONTROLLER_ADDRESS_2 		(STM32_FLASH_PAGE-2)*STM_SECTOR_SIZE+STM32_FLASH_BASE



typedef struct _CONTROLLER_STORAGE
{
	u16 storage_1_JG[200];

} CONTROLLER_STORAGE, *PCONTROLLER_STORAGE;



typedef struct _CONTROLLER_STORAGE_2
{
	u16 storage_2_JG[200];
	
} CONTROLLER_STORAGE_2, *PCONTROLLER_STORAGE_2;


extern uint32_t GetPage(uint32_t Address);//获取页

extern CONTROLLER_STORAGE  Controller_Storage;
extern CONTROLLER_STORAGE_2  Controller_Storage_2;

extern u16 storage_1[100];
extern u16 res_storage_1[100];
extern u16 storage_2[100];
extern u16 lcd_storage_1[100];

extern u8 Sure_write_biao;   
extern u8 Sure_write_timer;



bool WritecontrollerInfo(void );

bool WritecontrollerInfo_2(void);

bool   ReadcontrollerInfo(PCONTROLLER_STORAGE pControllerStorage);
bool   ReadcontrollerInfo_2(PCONTROLLER_STORAGE_2 pControllerStorage);
void   Read_all_2(void);
void   Read_all_1(void);
void write_eepall_1(u8 type);
void sure_write_eepall_1(void);

void   write_eepall_2(void); 

u32 WriteFlash(u32 startAddr, u8 *pData, u32 size);
u32 ReadFlash(u32 startAddr, u8 *pData, u32 size);

#endif


















#ifndef __STMFLASH_H__
#define __STMFLASH_H__
#include "sys.h"  
#include <stdbool.h> 
#include "ec600u.h"


//С������Ʒ16K��32K    STM32_FLASH_PAGE=31 
//��������Ʒ64k��128K   STM32_FLASH_PAGE=127 
//��������Ʒ256K��512K  STM32_FLASH_PAGE=255

//FLASH��ʼ��ַ
#define STM32_FLASH_BASE 0x08000000 	//STM32 FLASH����ʼ��ַ


#define STM_SECTOR_SIZE	2048

#define STM32_FLASH_PAGE  64//STMG070 128K ÿҳ2k ��64ҳ ��ҳ������ÿ��д8�ֽ�

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


extern uint32_t GetPage(uint32_t Address);//��ȡҳ

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


















#include "stm32g0xx_hal.h"
#include "stmflash.h"
#include "uartLCD.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK战舰STM32开发板
//STM32 FLASH 驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2012/9/13
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////
 
//读取指定地址的半字(16位数据)
//faddr:读地址(此地址必须为2的倍数!!)
//返回值:对应数据.

CONTROLLER_STORAGE     Controller_Storage;
CONTROLLER_STORAGE_2   Controller_Storage_2; 


u16 STMFLASH_BUF[STM_SECTOR_SIZE/2];//最多是2K字节


uint32_t FirstPage = 0, NbOfPages = 0;//页管理变量
uint32_t Address = 0, PageError = 0;//地址管理变量
__IO uint32_t data32 = 0 , MemoryProgramStatus = 0;//状态变量
static FLASH_EraseInitTypeDef EraseInitStruct;


u16 storage_1[100];
u16 storage_2[100];
u16 res_storage_1[100]={0xffff};//这种只能赋值第一个数组变量，其他其他的还是零
u16 lcd_storage_1[100];

u8 Sure_write_biao;   
u8 Sure_write_timer=0;

u32 WriteFlash(u32 startAddr, u8 *pData, u32 size)
{
	u64 * pBuf = (u64 *) pData; 

	u32 writeCount = 0;

	 HAL_FLASH_Unlock();//FLASH解锁
	FirstPage = GetPage(startAddr);//获取起始地址所在页
	//FirstPage = CONTROLLER_ADDRESS;//获取起始地址所在页
   NbOfPages = 1;//1页
  /* Fill EraseInit structure*/
    EraseInitStruct.TypeErase   = FLASH_TYPEERASE_PAGES;//擦除
    EraseInitStruct.Page        = FirstPage;
    EraseInitStruct.NbPages     = NbOfPages;
	
	
	
	
	if (HAL_FLASHEx_Erase(&EraseInitStruct, &PageError) != HAL_OK)//擦除FLASH
	  {
		while (1)//如果擦除失败，LED闪烁
		{
		   //HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_4);
			//	 HAL_Delay(1000);//1s等待
			delay_ms(1000);
			     break;
		}
	  }
	
	  Address = startAddr;
	  
	while (writeCount < size /8)//sizeof(int)  在32位系统下就是 4，在16位系统下就是2 。
			{
			  
			 if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, Address, pBuf[writeCount]) == HAL_OK)//把0x1234567812345678写入0x08008000开始的地址
				{
				  Address = Address + 8;
				  writeCount += 1;
				}
			  else 
			  
				{
				  HAL_FLASH_Lock();//FLASH锁
					return 0;
				}		  
			  
			}
	
     HAL_FLASH_Lock();//FLASH锁;
	return writeCount * sizeof(int);			

//	while (writeCount < size / sizeof(int))//sizeof(int)  在32位系统下就是 4，在16位系统下就是2 。
//	{
//		FLASHStatus = FLASH_ProgramWord(startAddr, pBuf[writeCount]);
//		if (FLASHStatus != FLASH_COMPLETE)
//		{
//			FLASH_Lock();
//			return 0;
//		}
//		writeCount += 1;
//		startAddr += sizeof(int);
//	}
//	FLASH_Lock();
//	return writeCount * sizeof(int);
}
//输入大小为几个4字节
u32 ReadFlash(u32 startAddr, u8 *pData, u32 size)
{
	int i = 0;
	u8 * pBuf = (u8 *) startAddr;
	while (i < size)
	{
		*(pData + i) = *pBuf;
		pBuf++;
		i++;
	}
	return i;
}




bool WritecontrollerInfo(void )
{
	u32 writebyte;
	
	u32 startAddr = CONTROLLER_ADDRESS;
	
	writebyte = WriteFlash(startAddr, (u8*)&Controller_Storage,
			sizeof(CONTROLLER_STORAGE));
	if (writebyte != sizeof(CONTROLLER_STORAGE))
	{
		return false;
	}
	return true;
}



bool WritecontrollerInfo_2(void)
{
	u32 writebyte;
	
	u32 startAddr = CONTROLLER_ADDRESS_2;
	writebyte = WriteFlash(startAddr, (u8*)&Controller_Storage_2,
			sizeof(CONTROLLER_STORAGE_2));
	if (writebyte != sizeof(CONTROLLER_STORAGE_2))
	{
		return false;
	}
	return true;
}









bool ReadcontrollerInfo(PCONTROLLER_STORAGE pControllerStorage)
{
	u32 readbyte;
	u32 startAddr = CONTROLLER_ADDRESS;
	readbyte = ReadFlash(startAddr, (u8*) pControllerStorage,
			sizeof(CONTROLLER_STORAGE));
	if (readbyte != sizeof(CONTROLLER_STORAGE))
	{
		return false;
	}
	return true;
}




bool ReadcontrollerInfo_2(PCONTROLLER_STORAGE_2 pControllerStorage)
{
	u32 readbyte;
	u32 startAddr = CONTROLLER_ADDRESS_2;
	readbyte = ReadFlash(startAddr, (u8*) pControllerStorage,
			sizeof(CONTROLLER_STORAGE_2));
	if (readbyte != sizeof(CONTROLLER_STORAGE_2))
	{
		return false;
	}
	return true;
}


extern  uint32_t GetPage(uint32_t Addr)
{
  return (Addr - FLASH_BASE) / FLASH_PAGE_SIZE;;
}


void Read_all_1(void) //开机放到主程序初始化

	    { u8 i;
			  
				ReadcontrollerInfo(&Controller_Storage); 
			
				for (i=0;i<80;i++)
				 {
					 storage_1[i]=Controller_Storage.storage_1_JG[i];
					 lcd_storage_1[i]=storage_1[i];//输出到LCD内存
					 
				 }
				
                store_bridge(1);

				 
			 }
		  

  
  void Read_all_2(void)

	    { u8 i;
			  
				ReadcontrollerInfo_2(&Controller_Storage_2); 
			
				for (i=0;i<100;i++)
				 {
					 storage_2[i]=Controller_Storage_2.storage_2_JG[i];
				 }
		 
			 } 

 
  
  
			 
			 
	
void write_eepall_1(u8 type)  
{
	 
	 
			 
	  if(type==1)  store_bridge(0);
	  if(type==0)  store_bridge(1);
			
       
      Sure_write_biao=1;   
      Sure_write_timer=0;
					
//			WritecontrollerInfo();	
//		  Read_all_1();
	
}

		 
void sure_write_eepall_1(void)  
{

/*
这个函数可以减缓存储耗费的时间
如何和write_eepall_1（）集成到一起占用CPU时间太多造成	CAT运行不稳定

*/
	
	
	 //u8 Sure_write_biao;    需要定义成全局变量
   //u8 Sure_write_timer=0; 需要定义成全局变量
   static  u8 Sure_write_timer_biao=0;
		u8 i;//少儿杂的变量用匹配变量，多而单的变量直接用数组，节省变量
		
		  if(TIMER_1MS%100<=30) 
			 
			{
			 if(Sure_write_timer_biao==1)
	
			    {
				    Sure_write_timer++;
				    Sure_write_timer_biao=0;
				    
						if((Sure_write_timer>=100)) 
	
							 {
								Sure_write_timer=100;
							 }			
						
				   } 
			}
	  else 
			  
			{
			 Sure_write_timer_biao=1;  
			}
		
	
	
	  if((Sure_write_timer>=100)&&(Sure_write_biao==1)) 
	
	   {
		  
			Sure_write_biao=0; 			 
				for (i=0;i<52;i++)
			   {
			    Controller_Storage.storage_1_JG[i]=storage_1[i];
			    }
 
			 
			 
		 	WritecontrollerInfo();	
		  Read_all_1();
		 }
	
	
	
           

	
}			 
			 
			 

void write_eepall_2(void) 
{
	 u8 i;//少儿杂的变量用匹配变量，多而单的变量直接用数组，节省变量
	 
			 
				for (i=0;i<100;i++)
			   {
			    Controller_Storage_2.storage_2_JG[i]=storage_2[i];
			    }


			WritecontrollerInfo_2();	
		    Read_all_2();
	
}

			 
			 
			 
			 
			 
			 
			 
			 
			 
			 
			 
			 
			 
			 
			 
			 
			 
			 
			 
			 



#include "stm32g0xx_hal.h"
#include "stmflash.h"
#include "uartLCD.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEKս��STM32������
//STM32 FLASH ��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/9/13
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////
 
//��ȡָ����ַ�İ���(16λ����)
//faddr:����ַ(�˵�ַ����Ϊ2�ı���!!)
//����ֵ:��Ӧ����.

CONTROLLER_STORAGE     Controller_Storage;
CONTROLLER_STORAGE_2   Controller_Storage_2; 


u16 STMFLASH_BUF[STM_SECTOR_SIZE/2];//�����2K�ֽ�


uint32_t FirstPage = 0, NbOfPages = 0;//ҳ�������
uint32_t Address = 0, PageError = 0;//��ַ�������
__IO uint32_t data32 = 0 , MemoryProgramStatus = 0;//״̬����
static FLASH_EraseInitTypeDef EraseInitStruct;


u16 storage_1[100];
u16 storage_2[100];
u16 res_storage_1[100]={0xffff};//����ֻ�ܸ�ֵ��һ��������������������Ļ�����
u16 lcd_storage_1[100];

u8 Sure_write_biao;   
u8 Sure_write_timer=0;

u32 WriteFlash(u32 startAddr, u8 *pData, u32 size)
{
	u64 * pBuf = (u64 *) pData; 

	u32 writeCount = 0;

	 HAL_FLASH_Unlock();//FLASH����
	FirstPage = GetPage(startAddr);//��ȡ��ʼ��ַ����ҳ
	//FirstPage = CONTROLLER_ADDRESS;//��ȡ��ʼ��ַ����ҳ
   NbOfPages = 1;//1ҳ
  /* Fill EraseInit structure*/
    EraseInitStruct.TypeErase   = FLASH_TYPEERASE_PAGES;//����
    EraseInitStruct.Page        = FirstPage;
    EraseInitStruct.NbPages     = NbOfPages;
	
	
	
	
	if (HAL_FLASHEx_Erase(&EraseInitStruct, &PageError) != HAL_OK)//����FLASH
	  {
		while (1)//�������ʧ�ܣ�LED��˸
		{
		   //HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_4);
			//	 HAL_Delay(1000);//1s�ȴ�
			delay_ms(1000);
			     break;
		}
	  }
	
	  Address = startAddr;
	  
	while (writeCount < size /8)//sizeof(int)  ��32λϵͳ�¾��� 4����16λϵͳ�¾���2 ��
			{
			  
			 if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, Address, pBuf[writeCount]) == HAL_OK)//��0x1234567812345678д��0x08008000��ʼ�ĵ�ַ
				{
				  Address = Address + 8;
				  writeCount += 1;
				}
			  else 
			  
				{
				  HAL_FLASH_Lock();//FLASH��
					return 0;
				}		  
			  
			}
	
     HAL_FLASH_Lock();//FLASH��;
	return writeCount * sizeof(int);			

//	while (writeCount < size / sizeof(int))//sizeof(int)  ��32λϵͳ�¾��� 4����16λϵͳ�¾���2 ��
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
//�����СΪ����4�ֽ�
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


void Read_all_1(void) //�����ŵ��������ʼ��

	    { u8 i;
			  
				ReadcontrollerInfo(&Controller_Storage); 
			
				for (i=0;i<80;i++)
				 {
					 storage_1[i]=Controller_Storage.storage_1_JG[i];
					 lcd_storage_1[i]=storage_1[i];//�����LCD�ڴ�
					 
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
����������Լ����洢�ķѵ�ʱ��
��κ�write_eepall_1�������ɵ�һ��ռ��CPUʱ��̫�����	CAT���в��ȶ�

*/
	
	
	 //u8 Sure_write_biao;    ��Ҫ�����ȫ�ֱ���
   //u8 Sure_write_timer=0; ��Ҫ�����ȫ�ֱ���
   static  u8 Sure_write_timer_biao=0;
		u8 i;//�ٶ��ӵı�����ƥ�������������ı���ֱ�������飬��ʡ����
		
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
	 u8 i;//�ٶ��ӵı�����ƥ�������������ı���ֱ�������飬��ʡ����
	 
			 
				for (i=0;i<100;i++)
			   {
			    Controller_Storage_2.storage_2_JG[i]=storage_2[i];
			    }


			WritecontrollerInfo_2();	
		    Read_all_2();
	
}

			 
			 
			 
			 
			 
			 
			 
			 
			 
			 
			 
			 
			 
			 
			 
			 
			 
			 
			 
			 



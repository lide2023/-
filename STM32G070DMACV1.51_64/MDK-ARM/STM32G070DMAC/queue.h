
#ifndef  QUEUE_H
#define  QUEUE_H
#include "stdint.h"
#ifdef   __cplusplus
extern "C" 
{
#endif   /* #ifdef __cplusplus */



#define  RINGBUFF_LEN          (32)     //�����������ֽ��� 500
#define  RINGBUFF_OK           1     
#define  RINGBUFF_ERR          0   
typedef unsigned          char uint8_t;
typedef unsigned short     int uint16_t;

typedef struct
{

   volatile uint16_t Head; //����ͷ���ڵ�λ��          
   volatile uint16_t Tail;//����β���ڵ�λ��
   volatile uint16_t Lenght;//���еĳ��� 
   volatile uint8_t  Ring_data[RINGBUFF_LEN];//��������
}RingBuff_t;


typedef struct
{

   volatile uint16_t Head; //����ͷ���ڵ�λ��          
   volatile uint16_t Tail;//����β���ڵ�λ��
   volatile uint16_t Lenght;//���еĳ��� 
   volatile uint16_t Ring_data[RINGBUFF_LEN];//��������
}RingBuff_cat_feedback;

extern RingBuff_cat_feedback Cat_fedback_data;

void RingBuff_Init(RingBuff_t *ringBuff);
uint8_t Read_RingBuff(RingBuff_t *ringBuff,uint8_t *rData);
uint8_t Write_RingBuff(RingBuff_t *ringBuff ,uint8_t data);
uint8_t Write_RingBuff_cat(RingBuff_cat_feedback *ringBuff, uint16_t add,uint16_t data);
uint8_t Read_RingBuff_cat(RingBuff_cat_feedback *ringBuff, uint16_t *rData);
#ifdef __cplusplus
	}
#endif

#endif	//#ifndef  __TYPE_H

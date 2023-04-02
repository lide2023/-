
#include "queue.h"



//RingBuff_t R_debug_buf;//����һ�����Դ��ڵĽ��ջ�����

//RingBuff_t  R_485_buf;  //����һ��485���մ��ڵĻ�����
RingBuff_cat_feedback Cat_fedback_data;

/**
* @brief  RingBuff_Init
* @param  void
* @return void
* @note   ��ʼ�����λ�����
*/
void RingBuff_Init(RingBuff_t *ringBuff)
{
    //��ʼ�������Ϣ
    ringBuff->Head = 0;//����ͷ���ڵ�λ��
    ringBuff->Tail = 0;//����β���ڵ�λ��
    ringBuff->Lenght = 0;//���еĳ���
}
/**
* @brief  Write_RingBuff
* @param  uint8_t data
* @return FLASE:���λ�����������д��ʧ��;TRUE:д��ɹ�
* @note   �����λ�����д��uint8_t���͵�����
*/



uint8_t Write_RingBuff(RingBuff_t *ringBuff, uint8_t data)
{
    if(ringBuff->Lenght >= RINGBUFF_LEN) //�жϻ������Ƿ�����
    {
        return RINGBUFF_ERR;
    }

    ringBuff->Ring_data[ringBuff->Tail] = data;
    ringBuff->Tail = (ringBuff->Tail + 1) % RINGBUFF_LEN; //��ֹԽ��Ƿ�����
    ringBuff->Lenght++;
    return RINGBUFF_OK;
}



 

uint8_t Write_RingBuff_cat(RingBuff_cat_feedback *ringBuff, uint16_t add,uint16_t data)
{
    if(ringBuff->Lenght >= RINGBUFF_LEN) //�жϻ������Ƿ�����
    {
        return RINGBUFF_ERR;
    }

    ringBuff->Ring_data[ringBuff->Tail] = add;
    ringBuff->Ring_data[ringBuff->Tail+1] = data;
    ringBuff->Tail = (ringBuff->Tail + 2) % RINGBUFF_LEN; //��ֹԽ��Ƿ�����
    ringBuff->Lenght++;
    ringBuff->Lenght++;//��Ҫ������
    return RINGBUFF_OK;
}








/**
* @brief  Read_RingBuff
* @param  uint8_t *rData�����ڱ����ȡ������
* @return FLASE:���λ�����û�����ݣ���ȡʧ��;TRUE:��ȡ�ɹ�
* @note   �ӻ��λ�������ȡһ��u8���͵�����
*/
uint8_t Read_RingBuff(RingBuff_t *ringBuff, uint8_t *rData)
{
    if(ringBuff->Lenght == 0)//�жϷǿ�
    {
        return RINGBUFF_ERR;
    }

    *rData = ringBuff->Ring_data[ringBuff->Head];//�Ƚ��ȳ�FIFO���ӻ�����ͷ��
    ringBuff->Head = (ringBuff->Head + 1) % RINGBUFF_LEN; //��ֹԽ��Ƿ�����
    ringBuff->Lenght--;
    return RINGBUFF_OK;
}



uint8_t Read_RingBuff_cat(RingBuff_cat_feedback *ringBuff, uint16_t *rData)  
{
    if(ringBuff->Lenght == 0)//�жϷǿ�
    {
        return RINGBUFF_ERR;
    }

    rData[0] = ringBuff->Ring_data[ringBuff->Head];//�Ƚ��ȳ�FIFO���ӻ�����ͷ��
    rData[1] = ringBuff->Ring_data[ringBuff->Head+1];//�Ƚ��ȳ�FIFO���ӻ�����ͷ��
    ringBuff->Head = (ringBuff->Head + 2) % RINGBUFF_LEN; //��ֹԽ��Ƿ�����
    ringBuff->Lenght--;
    ringBuff->Lenght--;
    return RINGBUFF_OK;
}
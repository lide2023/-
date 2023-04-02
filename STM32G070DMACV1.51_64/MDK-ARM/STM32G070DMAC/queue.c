
#include "queue.h"



//RingBuff_t R_debug_buf;//创建一个调试串口的接收缓冲区

//RingBuff_t  R_485_buf;  //创建一个485接收串口的缓冲区
RingBuff_cat_feedback Cat_fedback_data;

/**
* @brief  RingBuff_Init
* @param  void
* @return void
* @note   初始化环形缓冲区
*/
void RingBuff_Init(RingBuff_t *ringBuff)
{
    //初始化相关信息
    ringBuff->Head = 0;//队列头所在的位置
    ringBuff->Tail = 0;//队列尾所在的位置
    ringBuff->Lenght = 0;//队列的长度
}
/**
* @brief  Write_RingBuff
* @param  uint8_t data
* @return FLASE:环形缓冲区已满，写入失败;TRUE:写入成功
* @note   往环形缓冲区写入uint8_t类型的数据
*/



uint8_t Write_RingBuff(RingBuff_t *ringBuff, uint8_t data)
{
    if(ringBuff->Lenght >= RINGBUFF_LEN) //判断缓冲区是否已满
    {
        return RINGBUFF_ERR;
    }

    ringBuff->Ring_data[ringBuff->Tail] = data;
    ringBuff->Tail = (ringBuff->Tail + 1) % RINGBUFF_LEN; //防止越界非法访问
    ringBuff->Lenght++;
    return RINGBUFF_OK;
}



 

uint8_t Write_RingBuff_cat(RingBuff_cat_feedback *ringBuff, uint16_t add,uint16_t data)
{
    if(ringBuff->Lenght >= RINGBUFF_LEN) //判断缓冲区是否已满
    {
        return RINGBUFF_ERR;
    }

    ringBuff->Ring_data[ringBuff->Tail] = add;
    ringBuff->Ring_data[ringBuff->Tail+1] = data;
    ringBuff->Tail = (ringBuff->Tail + 2) % RINGBUFF_LEN; //防止越界非法访问
    ringBuff->Lenght++;
    ringBuff->Lenght++;//需要加两次
    return RINGBUFF_OK;
}








/**
* @brief  Read_RingBuff
* @param  uint8_t *rData，用于保存读取的数据
* @return FLASE:环形缓冲区没有数据，读取失败;TRUE:读取成功
* @note   从环形缓冲区读取一个u8类型的数据
*/
uint8_t Read_RingBuff(RingBuff_t *ringBuff, uint8_t *rData)
{
    if(ringBuff->Lenght == 0)//判断非空
    {
        return RINGBUFF_ERR;
    }

    *rData = ringBuff->Ring_data[ringBuff->Head];//先进先出FIFO，从缓冲区头出
    ringBuff->Head = (ringBuff->Head + 1) % RINGBUFF_LEN; //防止越界非法访问
    ringBuff->Lenght--;
    return RINGBUFF_OK;
}



uint8_t Read_RingBuff_cat(RingBuff_cat_feedback *ringBuff, uint16_t *rData)  
{
    if(ringBuff->Lenght == 0)//判断非空
    {
        return RINGBUFF_ERR;
    }

    rData[0] = ringBuff->Ring_data[ringBuff->Head];//先进先出FIFO，从缓冲区头出
    rData[1] = ringBuff->Ring_data[ringBuff->Head+1];//先进先出FIFO，从缓冲区头出
    ringBuff->Head = (ringBuff->Head + 2) % RINGBUFF_LEN; //防止越界非法访问
    ringBuff->Lenght--;
    ringBuff->Lenght--;
    return RINGBUFF_OK;
}
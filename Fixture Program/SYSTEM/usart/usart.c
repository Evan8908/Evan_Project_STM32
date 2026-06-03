/**************************************************

注意：要使用自定义串口库函数，务必设置 Options Target  
在Target右上角栏目勾选 USE Micro LIB，否则会死机。
USART1：Tx--PA9  Rx--PA10    
USART2：Tx--PA2  Rx--PA3     
USART3：Tx--PB10 Rx--PB11    
UART4： Tx--PC10 Rx--PC11
UART5： Tx--PC12 Rx--PD2
***************************************************/
#include "usart.h"
#include "sys.h"
#include "stdio.h"
#include "string.h"
#include "stdarg.h"
#include "delay.h"
#include "led.h"
#include "24cxx.h" 
#include "timer.h"
#include "adc.h"
//#include "app.h"
volatile uint8_t usart3_Rx_flag=0;
volatile double lux_value=0;

char rx1_buffer[RX1_BUFF_MAX] = "";
u8 rx1_buffer_cnt = 0;
char rx2_buffer[RX2_BUFF_MAX] = "";
u8 rx2_buffer_cnt = 0;
char rx3_buffer[RX3_BUFF_MAX] = "";
u8 rx3_buffer_cnt = 0;

char tx1_buffer[TX1_BUFF_MAX] = "";
u8 tx1_buffer_cnt = 0;
char tx2_buffer[TX2_BUFF_MAX] = "";
u8 tx2_buffer_cnt = 0;
char tx3_buffer[TX3_BUFF_MAX] = "";
u8 tx3_buffer_cnt = 0;


void Usart_init(u8 Usartx,u32 baud)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
  
  if((Usartx&Usart1)==Usart1)	//&使初始化时可以用|运算符
  {
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO, ENABLE);
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;         //Tx1
    GPIO_Init(GPIOA, &GPIO_InitStructure);
   
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;   //Rx1
    GPIO_Init(GPIOA, &GPIO_InitStructure); 
    
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0 ;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;		//
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
    NVIC_Init(&NVIC_InitStructure);	//根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器USART1
    
    USART_InitStructure.USART_BaudRate = baud;//波特率
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;//8位数据
    USART_InitStructure.USART_StopBits = USART_StopBits_1;//在帧结尾传输1个停止位
    USART_InitStructure.USART_Parity = USART_Parity_No;//无校验
    USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;//发送和接收都使能
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//硬件流控制失能
    USART_Init(USART1,&USART_InitStructure);
    
    USART_ClearFlag(USART1, USART_FLAG_TC);   //防止第一个数据被覆盖
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//使能或者失能指定的USART中断
    
    USART_Cmd(USART1, ENABLE);//使能或者失能USART外设
    
//    delay_ms(10);
    USART_printf(Usart1," #USART1 IS OK!\r\n");
  }
  
  if((Usartx&Usart2)==Usart2)
  {
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);//使能或者失能APB1外设时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO, ENABLE);
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;         //Tx2
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;   //Rx2
    GPIO_Init(GPIOA, &GPIO_InitStructure); 
    
    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0 ;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;		//
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
    NVIC_Init(&NVIC_InitStructure);	//根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器USART1
    
    USART_InitStructure.USART_BaudRate = baud;//波特率
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;//8位数据
    USART_InitStructure.USART_StopBits = USART_StopBits_1;//在帧结尾传输1个停止位
    USART_InitStructure.USART_Parity = USART_Parity_No;//无校验
    USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;//发送和接收都使能
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//硬件流控制失能
    USART_Init(USART2,&USART_InitStructure);
    
    USART_ClearFlag(USART2, USART_FLAG_TC);   //防止第一个数据被覆盖
    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//使能或者失能指定的USART中断
    
    USART_Cmd(USART2, ENABLE);//使能或者失能USART外设
    
//    delay_ms(10);
//    USART_printf(Usart2," #USART2 IS OK!\r\n");
//    USART_printf(Usart2,"Ready!\n");
  }
  
  if((Usartx&Usart3)==Usart3)
  {
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);//使能或者失能APB1外设时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO, ENABLE);
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;         //Tx3
    GPIO_Init(GPIOB, &GPIO_InitStructure);
   
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;   //Rx3
    GPIO_Init(GPIOB, &GPIO_InitStructure); 
    
    NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0 ;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;		//
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
    NVIC_Init(&NVIC_InitStructure);	//根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器USART1
    
    USART_InitStructure.USART_BaudRate = baud;//波特率
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;//8位数据
    USART_InitStructure.USART_StopBits = USART_StopBits_1;//在帧结尾传输1个停止位
    USART_InitStructure.USART_Parity = USART_Parity_No;//无校验
    USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;//发送和接收都使能
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//硬件流控制失能
    USART_Init(USART3,&USART_InitStructure);
    
    USART_ClearFlag(USART3, USART_FLAG_TC);   //防止第一个数据被覆盖
    USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//使能或者失能指定的USART中断
    
    USART_Cmd(USART3, ENABLE);//使能或者失能USART外设
    
//    delay_ms(10);
    USART_printf(Usart3," #USART3 IS OK!\r\n");
  }
}

void Rx1_Get_Buff(void)
{
  rx1_buffer[rx1_buffer_cnt++] = (u8)USART_ReceiveData(USART1);
  if(rx1_buffer_cnt >= RX1_BUFF_MAX)rx1_buffer_cnt = 0;
}
void Rx2_Get_Buff(void)
{
  rx2_buffer[rx2_buffer_cnt++] = (u8)USART_ReceiveData(USART2);

  if(rx2_buffer_cnt >= RX2_BUFF_MAX)rx2_buffer_cnt = 0;
}
void Rx3_Get_Buff(void)
{
    rx3_buffer[rx3_buffer_cnt++] = (u8)USART_ReceiveData(USART3);
    if(rx3_buffer_cnt >= RX3_BUFF_MAX)rx3_buffer_cnt = 0;
    
    // 检查帧头标志
    if (rx3_buffer[0] != 0x5A || rx3_buffer[1] != 0x5A) {
        return ; // 帧头标志不正确，返回 0 表示校验失败
    }
    // 检查数据量
    if (rx3_buffer[3] != 0x04) {
        return ; // 数据量不正确，返回 0 表示校验失败
    }
    if(rx3_buffer_cnt==9)
    {
        // 计算 Lux 的初始值
        unsigned int Lux = (rx3_buffer[4] << 24) | (rx3_buffer[5] << 16) | (rx3_buffer[6] << 8) | rx3_buffer[7];
        // 进行除法运算
        lux_value = (double)Lux / 100;
        // 输出结果
        usart3_Rx_flag=1;
        memset(rx3_buffer,0,sizeof(rx3_buffer));
        rx3_buffer_cnt = 0;
    }
}
void USART_Send_Byte(u8 Usartx,u8 Byte)//单字节发送
{
    if(Usartx == Usart1)
    {
        USART1->DR = (Byte & (uint16_t)0x01FF);
        while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
    }
    if(Usartx == Usart2)
    {
        USART2->DR = (Byte & (uint16_t)0x01FF);
        while(USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET);
    }
    if(Usartx == Usart3)
    {
        USART3->DR = (Byte & (uint16_t)0x01FF);
        while(USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET);
    }
}

void USART_printf(u8 Usartx,char *fmt, ...)//格式化发送字符串                       
{
  char buffer[CMD_BUFFER_LEN+1];  // CMD_BUFFER_LEN长度自己定义吧
  char i = 0;
  
  va_list arg_ptr;
  va_start(arg_ptr, fmt);  
  vsnprintf(buffer, CMD_BUFFER_LEN+1, fmt, arg_ptr);
  while (buffer[i])
  {
      USART_Send_Byte(Usartx,(u8) buffer[i++]);
  }
  va_end(arg_ptr);
}
void USART_SendString(u8 USARTx, u8 *point)// 发送字符串
{
    const unsigned char *p = point;
    while (*p != '\0')
    {
        USART_Send_Byte(USARTx, *p);
        p++;
    }
}
void USART1_IRQHandler(void)
{
  if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)//检查指定的USART中断发生与否
  {
    Rx1_Get_Buff();
    USART_ClearITPendingBit(USART1, USART_IT_RXNE);//清除USARTx的中断待处理位
  }
  
  if(USART_GetITStatus(USART1, USART_IT_TC) != RESET)
  {
//    if(tx1_buffer_cnt)USART1->DR = (tx1_buffer[TX1_BUFF_MAX - tx1_buffer_cnt] & (uint16_t)0x01FF);
    USART_ClearITPendingBit(USART1, USART_IT_TC);//清除USARTx的中断待处理位
  }
}

void USART2_IRQHandler(void)
{
  if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)//检查指定的USART中断发生与否
  {
    Rx2_Get_Buff();
    USART_ClearITPendingBit(USART2, USART_IT_RXNE);//清除USARTx的中断待处理位
  }
  
  if(USART_GetITStatus(USART2, USART_IT_TC) != RESET)
  {
    
    USART_ClearITPendingBit(USART2, USART_IT_TC);//清除USARTx的中断待处理位
  }
}

void USART3_IRQHandler(void)
{
  if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)//检查指定的USART中断发生与否
  {
    Rx3_Get_Buff();
//    USART_Send_Byte(Usart1,(u8)USART_ReceiveData(USART3));
    USART_ClearITPendingBit(USART3, USART_IT_RXNE);//清除USARTx的中断待处理位
  }
  
  if(USART_GetITStatus(USART3, USART_IT_TC) != RESET)
  {
    
    USART_ClearITPendingBit(USART3, USART_IT_TC);//清除USARTx的中断待处理位
  }
}



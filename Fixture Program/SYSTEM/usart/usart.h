#ifndef __USART_H
#define __USART_H

#include "sys.h"

#define CMD_BUFFER_LEN 128

#define Usart1 1<<0
#define Usart2 1<<1
#define Usart3 1<<2

#define RX1_BUFF_MAX 128
#define RX2_BUFF_MAX 128
#define RX3_BUFF_MAX 128

#define TX1_BUFF_MAX 128
#define TX2_BUFF_MAX 128
#define TX3_BUFF_MAX 128

#define USART1_FRAME_HEAD    "$Set:"
#define USART1_END_OF_FRAME  "\r\n"
#define USART2_FRAME_HEAD    "$Set:"
#define USART2_END_OF_FRAME  "\r\n"
#define USART3_FRAME_HEAD    "$Set:"
#define USART3_END_OF_FRAME  "\n"

#define CNT_MAX 0xffff


#define COMMAND_MAX_LENTH 30 //指令最大长度(字节)
//#define COMMAND_LIST_NUM 9 //指令数量，需要和实际完全一致，否则会出错
typedef struct
{
    char command[COMMAND_MAX_LENTH];
//    uint32_t input;
//    uint32_t output;
    void (*func)(void);
}Com_Prot_Struct;


extern volatile double lux_value;
extern volatile uint8_t usart3_Rx_flag;
extern char rx1_buffer[RX1_BUFF_MAX];
extern u8 rx1_buffer_cnt;
extern char rx2_buffer[RX2_BUFF_MAX];
extern u8 rx2_buffer_cnt;
extern char rx3_buffer[RX3_BUFF_MAX];
extern u8 rx3_buffer_cnt;

void Usart_init(u8 Usartx,u32 baud);
void deal_rx_buff(uint16_t tim);//处理串口数据

void USART_Send_Byte(u8 Usartx,u8 Byte);
void USART_printf(u8 Usartx,char *fmt, ...);    //用之前必须打开相应串口，否则会死机,首次发送将自动删去首字符
void USART_SendString(u8 USARTx, u8 *point);
#endif

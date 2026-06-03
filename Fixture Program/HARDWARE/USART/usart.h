#ifndef __USART_H_
#define __USART_H_

//#include "stdio.h"
#include "sys.h"

#define CMD_BUFFER_LEN 128

#define Usart1 1<<0
#define Usart2 1<<1
#define Usart3 1<<2

#define RX1_BUFF_MAX 128

#define FRAME_HEAD    "$Set:"
#define END_OF_FRAME  "end#"

extern char rx1_buffer[RX1_BUFF_MAX];
extern u8 rx1_buffer_cnt;

extern u8 mode ;//0平面直角平坐标模式   1极坐标模式  bit0 编码器1  bit1 编码器2
extern u8 status ;//状态寄存器 bit0 工作使能  bit1 清零使能1 byte2 清零使能2 
extern u16 t_f ;//发送频率
extern u8 out_sta ;//输出状态
extern u16 ful_cicl_pul ;//整圈脉冲数1-65535

void Usart_init(u8 Usartx,u32 baud);
void Rx1_Buffer_Init(void);
void deal_rx1_buff(void);//处理串口1数据

void USART1_Send_Byte(u8 Byte);
void USART1_printf (char *fmt, ...);    //用之前必须打开相应串口，否则会死机,首次发送将自动删去首字符

#endif


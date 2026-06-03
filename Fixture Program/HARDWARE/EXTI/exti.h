#ifndef __MYEXTI_H
#define __MYEXTI_H	 
#include "sys.h"

#define exti_GPIOA 1<<0
#define exti_GPIOB 1<<1
#define exti_GPIOC 1<<2
#define exti_GPIOD 1<<3
#define exti_GPIOE 1<<4
#define exti_GPIOF 1<<5
#define exti_GPIOG 1<<6

#define exti_Line0  1<<0
#define exti_Line1  1<<1
#define exti_Line2  1<<2
#define exti_Line3  1<<3
#define exti_Line4  1<<4
#define exti_Line5  1<<5
#define exti_Line6  1<<6
#define exti_Line7  1<<7
#define exti_Line8  1<<8
#define exti_Line9  1<<9
#define exti_Line10 1<<10
#define exti_Line11 1<<11
#define exti_Line12 1<<12
#define exti_Line13 1<<13
#define exti_Line14 1<<14
#define exti_Line15 1<<15
#define exti_Lineall 0xffff

#define X1 PBin(5)
#define A1 PBin(6)
#define B1 PBin(7)

#define X2 PCin(4)
#define A2 PBin(0)
#define B2 PBin(1)

#define EX_Trig PAin(12)


extern long pul_cnt[2] ;
extern u8 dir[2] ;//0正转顺时针   1反转逆时针
//extern u8 reverse ;//0正常 1取反
extern long nboftns[2] ;//圈数

//extern u8 mode ;//0平面直角平坐标模式   1极坐标模式  bit0 编码器1  bit1 编码器2
//extern u8 status ;//状态寄存器 bit0 工作使能  bit1 清零使能1 byte2 清零使能2 
//extern u16 t_f ;//发送频率
//extern u8 out_sta ;//输出状态
//extern u16 ful_cicl_pul ;//整圈脉冲数1-65535

void EXTI_init(u8 MyEXTI_GPIOx,u16 MYEXTI_Linex);
		 					    
#endif


#ifndef __LED_H
#define __LED_H	 
#include "stm32f10x.h"
#include "sys.h"
//Mini STM32开发板
//LED驱动代码			 
//正点原子@ALIENTEK
//2012/2/27

//LED端口定义
#define LED PBout(4)

void LED_Init(void);//初始化

		 				    
#endif

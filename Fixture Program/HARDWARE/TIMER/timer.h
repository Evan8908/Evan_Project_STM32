#ifndef __TIMER_H
#define __TIMER_H
#include "sys.h"

/*******定义定时器******/
#define Tim1 1<<0
#define Tim2 1<<1
#define Tim3 1<<2
#define Tim4 1<<3

/*******定义功能******/
#define COMMON  1<<0  //普通功能            表示将二进制数 1 向左移动 0 位，结果仍然是 1，对应的二进制表示为 0001
#define FRE_CAP 1<<1  //频率捕获            表示将二进制数 1 向左移动 1 位，结果是 2，对应的二进制表示为 0010。
#define PWM_OUT 1<<2  //PWM输出             表示将二进制数 1 向左移动 2 位，结果是 4，对应的二进制表示为 0100。
#define PWM_CAP 1<<3  //PWM脉宽捕获         表示将二进制数 1 向左移动 3 位，结果是 8，对应的二进制表示为 1000。

extern uint64_t systim_ms ;//系统时间 ms

void Timer_init(u8 function,u8 My_TIMx,u16 arr,u16 psc);


#endif

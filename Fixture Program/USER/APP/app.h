#ifndef __APP_H
#define __APP_H
//#include "stm32f10x.h"

#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>
#include "sys.h"
#include "usart.h"
#include "timer.h"
#include "adc.h"
#include "delay.h"
#include "24cxx.h"
#include "mystr.h"
#include "mymath.h"
#include "..\FIXTURE\IO_def.h"


#define Version "\r\nHW:V1.5 2026.05.28 SW:V1.5 2026.05.28"    /* 当前有效版本：硬件*/

//typedef enum {DISABLE = 0, ENABLE = !DISABLE} EventStatus, ControlStatus;



#define Check_Start_Test_Short() (test_status&0x01) //检查是否开始测短路
#define Start_Test_Short() test_status |= 0x01 //设置开始测短路
#define Stop_Test_Short() test_status &= 0xfe;//设置停止测短路

#define Check_Start_Test_Open() (test_status&0x02) //检查是否开始测开路
#define Start_Test_Open() test_status |= 0x02 //设置开始测开路
#define Stop_Test_Open() test_status &= 0xfd;//设置停止测开路

#define Check_Start_Test_WINLED() (test_status&0x04) //检查是否开始测WINLED
#define Start_Test_WINLED() test_status |= 0x04 //设置开始测WINLED
#define Stop_Test_WINLED() test_status &= 0xfb;//设置停止测WINLED

#define CYLINDER_STOP() do_reg &= 0x03ff //关闭第11 和 第12个数字输出
//#define CYLINDER_DOWN() do_reg |= (1<<11);do_reg &= ~(1<<10)   //打开第12个数字输出，关闭第11个数字输出
//#define CYLINDER_UP()   do_reg |= (1<<10);do_reg &= ~(1<<11)   //打开第11个数字输出，关闭第12个数字输出
//#define Check_Cylinder_Down()   (di_reg&0x0800)//读取第12个数字输入    治具没有装按键感应器
//#define Check_Cylinder_Up()     (di_reg&0x0400)//读取第11个数字输入    治具没有装按键感应器


#define CYLINDER_DOWN() BTN_PRESS()
#define CYLINDER_UP()   BTN_RELEASE()


extern uint16_t do_reg ;//数字输出控制寄存器
extern uint16_t di_reg ;//数字输入缓存寄存器
extern uint16_t pwm_freq[2] ;//定时器3 4的频率，对应PWM1~PWM4 和 PWM5~PWM8的频率
extern uint16_t pwm_duty[8] ;//PWM1~PWM8的脉宽百分比



extern const Com_Prot_Struct com1_lst[];

uint8_t app_init(void);//初始化
uint8_t select_channel(uint16_t ch1, uint16_t ch2); //选择通道
void set_12digit_out(uint16_t do_temp);//设置12路数字输出
uint16_t get_12digit_input(void);//获取12路数字输入
void run(uint16_t tim);//运行，执行主要逻辑功能
void select_gear(FunctionalState auto_en, uint16_t tim_ms, uint16_t swcg_tim_ms); //选择档位

void Print_Lux_Sensor(void);
void Cal_lux(void);
double Usart3_Event(void);
void READ_LX1(void);
void READ_LX2(void);
void READ_LX3(void);
void READ_LX4(void);
void RGBC_1(void);
void RGBC_2(void);
void RGBC_3(void);
void RGBC_4(void);
void SET_CLA_RGB(void);
void SET_PWM30(void);
void RESET_FIXTURE(void);
void READ_RGB(void);
void STOP_WLED(void);
void START_WLED(void);
void VINWLED(void);
void com1_protocol_0(void);//串口1的协议0，测试或者握手
void com1_protocol_1(void);//串口1的协议1，读取电压
void com1_protocol_2(void);//串口1的协议2，格式化读取数字输入，$Read DI %b\r\n      支持%b %d %x
void com1_protocol_3(void);//串口1的协议3，设置数字输出，$Set DO:255\r\n 或者$Set DOx:255\r\n
void com1_protocol_4(void);//串口1的协议4，设置PWM频率，$Set PWM Freq:f1,f2,sum\r\n  f1-第一组（PWM1~PWM4）频率  f2-第二组（PWM5~PWM8）频率  sum ASCII校验和，f1,f2 取 1~50k
void com1_protocol_5(void);//串口1的协议5，设置PWM脉宽，$Set PWM Duty 3:11,14\r\n 或者 $Set PWM Duty :p1,p2,p3,p4,p5,p6,p7,p8,sum\r\n   p1~p8 占空比1~8   sum ASCII校验和
void com1_protocol_6(void);//串口1的协议6，关门
void com1_protocol_7(void);//串口1的协议7，开门
void com1_protocol_8(void);//串口1的协议8，读取门状态
void com1_protocol_9(void);//串口1的协议9，开始测短路
void com1_protocol_10(void);//串口1的协议10，开始测开路
void com1_protocol_11(void);//串口1的协议11，设置5V 2A输出
void com1_protocol_12(void);
void com2_protocol_0(void);
void com3_protocol_0(void);

#endif


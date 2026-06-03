#ifndef _MYMATH_H__
#define _MYMATH_H__

#include "math.h"

#ifndef u8
#define u8 unsigned char
#endif

#ifndef u16
#define u16 unsigned int
#endif

#ifndef u32
#define u32 unsigned long
#endif

u16 Find_Max(float *add,u16 num);//寻找最大值，返回地址偏移量
u16 Find_Min(float *add,u16 num);//寻找最小值，返回地址偏移量
float Weighted_Mean(float *add,u16 num);//计算一组数的加权平均值





#endif





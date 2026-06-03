#include "mymath.h"


/****************************************************
函数名：寻找最大值
功能  ：寻找最大值
参数一：add，数组地址
参数二：num，计算的数量
返回值：返回原地址的偏移量
版本  ：V1.0
作者  ：小龑
日期  ：2024.01.11
****************************************************/
u16 Find_Max(float *add,u16 num)
{
  float x = *add;
  u8 i = 1;
  u16 res = 0;
  for(;i<num;i++)
  {
    if(x < *(add+i))
    {
      x = *(add+i);
      res = i;
    }
  }
  return res;
}

/****************************************************
函数名：寻找最小值
功能  ：寻找最小值
参数一：add，数组地址
参数二：num，计算的数量
返回值：返回原地址的偏移量
版本  ：V1.0
作者  ：小龑
日期  ：2024.01.11
****************************************************/
u16 Find_Min(float *add,u16 num)
{
  float x = *add;
  u8 i = 1;
  u16 res = 0;
  for(;i<num;i++)
  {
    if(x > *(add+i))
    {
      x = *(add+i);
      res = i;
    }
  }
  return res;
}

/****************************************************
函数名：加权平均计算
功能  ：计算一个组数的加权平均值
参数一：add，数组地址
参数二：num，计算的数量
返回值：返回加权平均数
版本  ：V1.0
作者  ：小龑
日期  ：2024.01.11
****************************************************/
float Weighted_Mean(float *add,u16 num)
{
  float res = 0;
  if(num > 2)
  {
//    float max = *(add + Find_Max(add,num));
//    float min = *(add + Find_Min(add,num));
    u16 i = 0;
    for(;i<num;i++ )
    {
      if((i!=Find_Max(add,num)) && (i!=Find_Min(add,num)))res+= *(add+i);
    }
    res /= (num-2);
    
  }
  return res;
}







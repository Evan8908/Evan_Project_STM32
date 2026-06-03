#include "mystr.h"
#include "string.h"


/****************************************************
函数名：寻找字符串的字符
功能  ：取字符串中指定字符位置
参数一：str，被寻找的字符串
参数二：chr，要寻找的字符
返回值：找到返回位置，从1开始，未找到返回0
版本  ：V1.0
作者  ：小龑
日期  ：2022.09.18
****************************************************/
unsigned int Find_Str_Char(char *str,char chr)//找到返回位置，从1开始，没找到返回0
{
  unsigned int i = 0;
  for(i = 0;i < strlen(str);i++)
  {
    if(*(str + i) == chr)return i+1;
  }
  return 0;
}


/****************************************************
函数名：寻找字符串的字符串
功能  ：取字符串中指定字符串位置
参数一：src，被寻找的字符串
参数二：str，要寻找的字符串
返回值：找到返回位置，从1开始，未找到返回0
版本  ：V1.0
作者  ：小龑
日期  ：2022.09.18
****************************************************/
unsigned int Find_Str_Str(char *src,char *str)
{
  unsigned int i = 0,j = 0,k = 0;
  
  while(j + strlen(str) < strlen(src))
  {
    i = Find_Str_Char(src + j,*str);
    if(i)//找到一个头
    { 
      j += i;
      for(k = 0; k < strlen(str); k++)
      {
        if(*(src + j + k - 1) != *(str + k))break;
      }
      if(k == strlen(str))return j ;
    }
    else break;
  }
  return 0;
}


/****************************************************
函数名：寻找字符串的字符
功能  ：从字符串中取出第n个目标字符的位置
参数一：str，被寻找的字符串
参数二：chr，要寻找的字符
参数三：n  ，第n个
返回值：找到返回位置，从1开始，未找到返回0
版本  ：V1.0
作者  ：小龑
日期  ：2023.01.25
****************************************************/
unsigned int Find_Str_nChar(char *src, char chr, unsigned char n)
{
  u8 i = 0;
  u16 num = 0;
  u16 sum = 0;
  for(; i < n; i++ )
  {
    num = Find_Str_Char(src,chr);
    if(num==0)return 0;
    else
    {
      src = src + num;
      sum += num;
    }
  }
  return sum;
}

/****************************************************
函数名： Get_Middle_Str
功能  ：从源字符串中取出指定左边和右边字符串的中间字符串，返回第一次找到左边字符串相对源字符串位置
参数一：get_ad，保存地址
参数二：src，源字符串
参数三：lft，左边
参数四：rit，右边
返回值：0失败 其他
版本  ：V1.0
作者  ：小龑
日期  ：2024.04.13
****************************************************/
unsigned char Get_Middle_Str(char *save_ad,char *src,char *lft,char *rit)
{
  unsigned char res = 0;
  unsigned int num1 = Find_Str_Str(src,lft);
  unsigned int num2 = Find_Str_Str(src+num1,rit);
//  xdata unsigned char str_temp[RX1_BUFF_MAX-2] = "";
//  Clear_String(str_temp);
//  sprintf(str_temp,"%d,%d\n%d,%d",num1,num2,strlen(lft),strlen(rit));
//  USART_Send_String(USART_1,str_temp);
  if(num2 > strlen(lft))
  {
    Clear_String(save_ad,sizeof(save_ad));
//    strncpy(str_temp,src+num1,num2-1);
    strncpy(save_ad,src+num1+strlen(lft)-1,num2-strlen(lft));
//    USART_Send_String(USART_1,str_temp);
    res = num1;
  }
  return res;
}


/****************************************************
函数名：清空字符串
功能  ：从字符串中取出第n个目标字符的位置
参数一：add，字符串地址
参数二：num，清空数量
版本  ：V1.0
作者  ：小龑
日期  ：2023.01.27
****************************************************/
void Clear_String(char *add,u16 num)
{
  u16 i = 0;
  while(i < num)
  {
    add[i++] = 0;
  }
}


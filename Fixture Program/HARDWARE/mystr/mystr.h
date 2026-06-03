#ifndef _MYSTR_H
#define _MYSTR_H

#ifndef u8
#define u8 unsigned char
#endif

#ifndef u16
#define u16 unsigned int
#endif

#ifndef u32
#define u32 unsigned long
#endif

unsigned int Find_Str_Char(char *str,char chr);//找到返回位置，从1开始，没找到返回0
unsigned int Find_Str_Str(char *src,char *str);//找到返回位置，从1开始，没找到返回0
unsigned int Find_Str_nChar(char *src, char chr, unsigned char n);//从字符串中取出第n个目标字符的位置，找到返回位置，从1开始，未找到返回0
unsigned char Get_Middle_Str(char *save_ad,char *src,char *lft,char *rit);//从源字符串中取出指定左边和右边字符串的中间字符串，返回第一次找到左边字符串相对源字符串位置
void Clear_String(char *add,u16 num);






#endif

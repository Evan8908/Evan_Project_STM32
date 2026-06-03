#ifndef __24CXX_H
#define __24CXX_H
#include "myiic.h"   

//Mini STM32开发板
//24CXX驱动函数(适合24C01~24C16,24C32~256未经过测试!有待验证!)
//正点原子@ALIENTEK
//2010/6/10
//V1.2
#define AT24C01		127
#define AT24C02		255
#define AT24C04		511
#define AT24C08		1023
#define AT24C16		2047
#define AT24C32		4095
#define AT24C64	    8191
#define AT24C128	16383
#define AT24C256	32767  

#define EE_TYPE AT24C04

#define ENCODER_1_EEAD 0  //编码器1位置保存地址
#define ENCODER_2_EEAD 10 //编码器2位置保存地址
#define RUN_PARA_EEAD1  20 //运行参数1保存地址
#define RUN_PARA_EEAD2  30 //运行参数2保存地址
#define RUN_PARA_EEAD3  40 //运行参数3保存地址

//byte0            编码器1脉冲计数正负0正1负
//byte1 ~ byte4    编码器1脉冲计数值，低位在前  exti.h中 pul_cnt[0]
//byte5            编码器1圈数正负0正1负
//byte6 ~ byte9    编码器1圈数计数值，低位在前  exti.h中 nboftns[0]

//byte10           编码器2脉冲计数正负0正1负
//byte11~ byte14   编码器2脉冲计数值，低位在前  exti.h中 pul_cnt[1]
//byte15           编码器2圈数正负0正1负
//byte16 ~ byte19  编码器2圈数计数值，低位在前  exti.h中 nboftns[1]

//byte20           运行模式 usart.h 中mode  编码器1 2的模式
//byte21~ byte22   发送频率1-2000 usart.h中t_f
//byte23           输出状态 usart.h中的 out_sta
//byte24~ byte25   整圈脉冲数1 usart.h中的 ful_cicl_pul[0]
//byte26~ byte27   整圈脉冲数2 usart.h中的 ful_cicl_pul[1]

//byte30~byte31    减速比1 usart.h中的 jsb[0]
//byte32~byte33    减速比2 usart.h中的 jsb[1]

u8 AT24CXX_ReadOneByte(u16 ReadAddr);							//指定地址读取一个字节
void AT24CXX_WriteOneByte(u16 WriteAddr,u8 DataToWrite);		//指定地址写入一个字节
void AT24CXX_WriteLenByte(u16 WriteAddr,u32 DataToWrite,u8 Len);//指定地址开始写入指定长度的数据
u32 AT24CXX_ReadLenByte(u16 ReadAddr,u8 Len);					//指定地址开始读取指定长度数据
void AT24CXX_Write(u16 WriteAddr,u8 *pBuffer,u16 NumToWrite);	//从指定地址开始写入指定长度的数据
void AT24CXX_Read(u16 ReadAddr,u8 *pBuffer,u16 NumToRead);   	//从指定地址开始读出指定长度的数据

u8 AT24CXX_Check(void);  //检查器件
void AT24CXX_Init(void); //初始化IIC
#endif

















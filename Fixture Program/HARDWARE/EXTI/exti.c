#include "exti.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "stdlib.h"

long pul_cnt[2] = {0,0};//脉冲数*4  byte0 编码器1
long nboftns[2] = {0,0};//圈数  byte0 编码器1
u8 dir[2] = {0};//1正转顺时针   0反转逆时针
//u8 reverse = 0;//0正常 1取反
//

/**********************自己的外部中断库************************/
//
// MyEXTI_GPIOx : exti_GPIOA~exti_GPIOG
//
// MYEXTI_Linex : exti_Line0~exti_Line15

void EXTI_init(u8 MyEXTI_GPIOx,u16 MYEXTI_Linex)
{
  GPIO_InitTypeDef GPIO_InitStructure;
 	EXTI_InitTypeDef EXTI_InitStructure;
 	NVIC_InitTypeDef NVIC_InitStructure;
  
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
  
  /**********MyEXTI_GPIOA**********/
  if((MyEXTI_GPIOx&exti_GPIOA)==exti_GPIOA)
  {
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    if((MYEXTI_Linex&exti_Line0)==exti_Line0)
    {
      GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0;
      GPIO_Init(GPIOA, &GPIO_InitStructure);
    }
    if((MYEXTI_Linex&exti_Line1)==exti_Line1)
    {
      GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_1;
      GPIO_Init(GPIOA, &GPIO_InitStructure);
    }
    if((MYEXTI_Linex&exti_Line2)==exti_Line2)
    {
      GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_2;
      GPIO_Init(GPIOA, &GPIO_InitStructure);
    }
    if((MYEXTI_Linex&exti_Line3)==exti_Line3)
    {
      GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_3;
      GPIO_Init(GPIOA, &GPIO_InitStructure);
    }
    if((MYEXTI_Linex&exti_Line4)==exti_Line4)
    {
      GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_4;
      GPIO_Init(GPIOA, &GPIO_InitStructure);
    }
    if((MYEXTI_Linex&exti_Line5)==exti_Line5)
    {
      GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_5;
      GPIO_Init(GPIOA, &GPIO_InitStructure);
    }
    if((MYEXTI_Linex&exti_Line6)==exti_Line6)
    {
      GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_6;
      GPIO_Init(GPIOA, &GPIO_InitStructure);
    }
    if((MYEXTI_Linex&exti_Line7)==exti_Line7)
    {
      GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_7;
      GPIO_Init(GPIOA, &GPIO_InitStructure);
    }
    if((MYEXTI_Linex&exti_Line8)==exti_Line8)
    {
      GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_8;
      GPIO_Init(GPIOA, &GPIO_InitStructure);
    }
    if((MYEXTI_Linex&exti_Line9)==exti_Line9)
    {
      GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_9;
      GPIO_Init(GPIOA, &GPIO_InitStructure);
    }
    if((MYEXTI_Linex&exti_Line10)==exti_Line10)
    {
      GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_10;
      GPIO_Init(GPIOA, &GPIO_InitStructure);
    }
    if((MYEXTI_Linex&exti_Line11)==exti_Line11)
    {
      GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_11;
      GPIO_Init(GPIOA, &GPIO_InitStructure);
    }
    if((MYEXTI_Linex&exti_Line12)==exti_Line12)
    {
      GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_12;
      GPIO_Init(GPIOA, &GPIO_InitStructure);
    }
    if((MYEXTI_Linex&exti_Line13)==exti_Line13)
    {
      GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_13;
      GPIO_Init(GPIOA, &GPIO_InitStructure);
    }
    if((MYEXTI_Linex&exti_Line14)==exti_Line14)
    {
      GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_14;
      GPIO_Init(GPIOA, &GPIO_InitStructure);
    }
    if((MYEXTI_Linex&exti_Line15)==exti_Line15)
    {
      GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_15;
      GPIO_Init(GPIOA, &GPIO_InitStructure);
    }
    
    if((MYEXTI_Linex&exti_Line0)==exti_Line0)GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource0);
    if((MYEXTI_Linex&exti_Line1)==exti_Line1)GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource1);
    if((MYEXTI_Linex&exti_Line2)==exti_Line2)GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource2);
    if((MYEXTI_Linex&exti_Line3)==exti_Line3)GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource3);
    if((MYEXTI_Linex&exti_Line4)==exti_Line4)GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource4);
    if((MYEXTI_Linex&exti_Line5)==exti_Line5)GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource5);
    if((MYEXTI_Linex&exti_Line6)==exti_Line6)GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource6);
    if((MYEXTI_Linex&exti_Line7)==exti_Line7)GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource7);
    if((MYEXTI_Linex&exti_Line8)==exti_Line8)GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource8);
    if((MYEXTI_Linex&exti_Line9)==exti_Line9)GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource9);
    if((MYEXTI_Linex&exti_Line10)==exti_Line10)GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource10);
    if((MYEXTI_Linex&exti_Line11)==exti_Line11)GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource11);
    if((MYEXTI_Linex&exti_Line12)==exti_Line12)GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource12);
    if((MYEXTI_Linex&exti_Line13)==exti_Line13)GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource13);
    if((MYEXTI_Linex&exti_Line14)==exti_Line14)GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource14);
    if((MYEXTI_Linex&exti_Line15)==exti_Line15)GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource15);
    
  }
  
  /**********MyEXTI_GPIOB**********/
  if((MyEXTI_GPIOx&exti_GPIOB)==exti_GPIOB)
  {
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
    if((MYEXTI_Linex&exti_Line0)==exti_Line0)
    {
      GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0;
      GPIO_Init(GPIOB, &GPIO_InitStructure);
    }
    if((MYEXTI_Linex&exti_Line1)==exti_Line1)
    {
      GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_1;
      GPIO_Init(GPIOB, &GPIO_InitStructure);
    }
    if((MYEXTI_Linex&exti_Line2)==exti_Line2)
    {
      GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_2;
      GPIO_Init(GPIOB, &GPIO_InitStructure);
    }
    if((MYEXTI_Linex&exti_Line3)==exti_Line3)
    {
      GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_3;
      GPIO_Init(GPIOB, &GPIO_InitStructure);
    }
    if((MYEXTI_Linex&exti_Line4)==exti_Line4)
    {
      GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_4;
      GPIO_Init(GPIOB, &GPIO_InitStructure);
    }
    if((MYEXTI_Linex&exti_Line5)==exti_Line5)
    {
      GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_5;
      GPIO_Init(GPIOB, &GPIO_InitStructure);
    }
    if((MYEXTI_Linex&exti_Line6)==exti_Line6)
    {
      GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_6;
      GPIO_Init(GPIOB, &GPIO_InitStructure);
    }
    if((MYEXTI_Linex&exti_Line7)==exti_Line7)
    {
      GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_7;
      GPIO_Init(GPIOB, &GPIO_InitStructure);
    }
    if((MYEXTI_Linex&exti_Line8)==exti_Line8)
    {
      GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_8;
      GPIO_Init(GPIOB, &GPIO_InitStructure);
    }
    if((MYEXTI_Linex&exti_Line9)==exti_Line9)
    {
      GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_9;
      GPIO_Init(GPIOB, &GPIO_InitStructure);
    }
    if((MYEXTI_Linex&exti_Line10)==exti_Line10)
    {
      GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_10;
      GPIO_Init(GPIOB, &GPIO_InitStructure);
    }
    if((MYEXTI_Linex&exti_Line11)==exti_Line11)
    {
      GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_11;
      GPIO_Init(GPIOB, &GPIO_InitStructure);
    }
    if((MYEXTI_Linex&exti_Line12)==exti_Line12)
    {
      GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_12;
      GPIO_Init(GPIOB, &GPIO_InitStructure);
    }
    if((MYEXTI_Linex&exti_Line13)==exti_Line13)
    {
      GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_13;
      GPIO_Init(GPIOB, &GPIO_InitStructure);
    }
    if((MYEXTI_Linex&exti_Line14)==exti_Line14)
    {
      GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_14;
      GPIO_Init(GPIOB, &GPIO_InitStructure);
    }
    if((MYEXTI_Linex&exti_Line15)==exti_Line15)
    {
      GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_15;
      GPIO_Init(GPIOB, &GPIO_InitStructure);
    }
    
    if((MYEXTI_Linex&exti_Line0)==exti_Line0)GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource0);
    if((MYEXTI_Linex&exti_Line1)==exti_Line1)GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource1);
    if((MYEXTI_Linex&exti_Line2)==exti_Line2)GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource2);
    if((MYEXTI_Linex&exti_Line3)==exti_Line3)GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource3);
    if((MYEXTI_Linex&exti_Line4)==exti_Line4)GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource4);
    if((MYEXTI_Linex&exti_Line5)==exti_Line5)GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource5);
    if((MYEXTI_Linex&exti_Line6)==exti_Line6)GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource6);
    if((MYEXTI_Linex&exti_Line7)==exti_Line7)GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource7);
    if((MYEXTI_Linex&exti_Line8)==exti_Line8)GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource8);
    if((MYEXTI_Linex&exti_Line9)==exti_Line9)GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource9);
    if((MYEXTI_Linex&exti_Line10)==exti_Line10)GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource10);
    if((MYEXTI_Linex&exti_Line11)==exti_Line11)GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource11);
    if((MYEXTI_Linex&exti_Line12)==exti_Line12)GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource12);
    if((MYEXTI_Linex&exti_Line13)==exti_Line13)GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource13);
    if((MYEXTI_Linex&exti_Line14)==exti_Line14)GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource14);
    if((MYEXTI_Linex&exti_Line15)==exti_Line15)GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource15);
  }
  
    /**********MyEXTI_GPIOC**********/
  
  if((MyEXTI_GPIOx&exti_GPIOC)==exti_GPIOC)
  {
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    if((MYEXTI_Linex&exti_Line0)==exti_Line0)
    {
      GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0;
      GPIO_Init(GPIOC, &GPIO_InitStructure);
    }
    if((MYEXTI_Linex&exti_Line1)==exti_Line1)
    {
      GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_1;
      GPIO_Init(GPIOC, &GPIO_InitStructure);
    }
    if((MYEXTI_Linex&exti_Line2)==exti_Line2)
    {
      GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_2;
      GPIO_Init(GPIOC, &GPIO_InitStructure);
    }
    if((MYEXTI_Linex&exti_Line3)==exti_Line3)
    {
      GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_3;
      GPIO_Init(GPIOC, &GPIO_InitStructure);
    }
    if((MYEXTI_Linex&exti_Line4)==exti_Line4)
    {
      GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_4;
      GPIO_Init(GPIOC, &GPIO_InitStructure);
    }
    if((MYEXTI_Linex&exti_Line5)==exti_Line5)
    {
      GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_5;
      GPIO_Init(GPIOC, &GPIO_InitStructure);
    }
    if((MYEXTI_Linex&exti_Line6)==exti_Line6)
    {
      GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_6;
      GPIO_Init(GPIOC, &GPIO_InitStructure);
    }
    if((MYEXTI_Linex&exti_Line7)==exti_Line7)
    {
      GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_7;
      GPIO_Init(GPIOC, &GPIO_InitStructure);
    }
    if((MYEXTI_Linex&exti_Line8)==exti_Line8)
    {
      GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_8;
      GPIO_Init(GPIOC, &GPIO_InitStructure);
    }
    if((MYEXTI_Linex&exti_Line9)==exti_Line9)
    {
      GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_9;
      GPIO_Init(GPIOC, &GPIO_InitStructure);
    }
    if((MYEXTI_Linex&exti_Line10)==exti_Line10)
    {
      GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_10;
      GPIO_Init(GPIOC, &GPIO_InitStructure);
    }
    if((MYEXTI_Linex&exti_Line11)==exti_Line11)
    {
      GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_11;
      GPIO_Init(GPIOC, &GPIO_InitStructure);
    }
    if((MYEXTI_Linex&exti_Line12)==exti_Line12)
    {
      GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_12;
      GPIO_Init(GPIOC, &GPIO_InitStructure);
    }
    if((MYEXTI_Linex&exti_Line13)==exti_Line13)
    {
      GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_13;
      GPIO_Init(GPIOC, &GPIO_InitStructure);
    }
    if((MYEXTI_Linex&exti_Line14)==exti_Line14)
    {
      GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_14;
      GPIO_Init(GPIOC, &GPIO_InitStructure);
    }
    if((MYEXTI_Linex&exti_Line15)==exti_Line15)
    {
      GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_15;
      GPIO_Init(GPIOC, &GPIO_InitStructure);
    }
    
    if((MYEXTI_Linex&exti_Line0)==exti_Line0)GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource0);
    if((MYEXTI_Linex&exti_Line1)==exti_Line1)GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource1);
    if((MYEXTI_Linex&exti_Line2)==exti_Line2)GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource2);
    if((MYEXTI_Linex&exti_Line3)==exti_Line3)GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource3);
    if((MYEXTI_Linex&exti_Line4)==exti_Line4)GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource4);
    if((MYEXTI_Linex&exti_Line5)==exti_Line5)GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource5);
    if((MYEXTI_Linex&exti_Line6)==exti_Line6)GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource6);
    if((MYEXTI_Linex&exti_Line7)==exti_Line7)GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource7);
    if((MYEXTI_Linex&exti_Line8)==exti_Line8)GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource8);
    if((MYEXTI_Linex&exti_Line9)==exti_Line9)GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource9);
    if((MYEXTI_Linex&exti_Line10)==exti_Line10)GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource10);
    if((MYEXTI_Linex&exti_Line11)==exti_Line11)GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource11);
    if((MYEXTI_Linex&exti_Line12)==exti_Line12)GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource12);
    if((MYEXTI_Linex&exti_Line13)==exti_Line13)GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource13);
    if((MYEXTI_Linex&exti_Line14)==exti_Line14)GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource14);
    if((MYEXTI_Linex&exti_Line15)==exti_Line15)GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource15);
  }
  
    /**********MyEXTI_GPIOD**********/
  
  if((MyEXTI_GPIOx&exti_GPIOD)==exti_GPIOD)
  {
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD,ENABLE);
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    if((MYEXTI_Linex&exti_Line0)==exti_Line0)
    {
      GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0;
      GPIO_Init(GPIOD, &GPIO_InitStructure);
    }
    if((MYEXTI_Linex&exti_Line1)==exti_Line1)
    {
      GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_1;
      GPIO_Init(GPIOD, &GPIO_InitStructure);
    }
    if((MYEXTI_Linex&exti_Line2)==exti_Line2)
    {
      GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_2;
      GPIO_Init(GPIOD, &GPIO_InitStructure);
    }
    if((MYEXTI_Linex&exti_Line3)==exti_Line3)
    {
      GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_3;
      GPIO_Init(GPIOD, &GPIO_InitStructure);
    }
    if((MYEXTI_Linex&exti_Line4)==exti_Line4)
    {
      GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_4;
      GPIO_Init(GPIOD, &GPIO_InitStructure);
    }
    if((MYEXTI_Linex&exti_Line5)==exti_Line5)
    {
      GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_5;
      GPIO_Init(GPIOD, &GPIO_InitStructure);
    }
    if((MYEXTI_Linex&exti_Line6)==exti_Line6)
    {
      GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_6;
      GPIO_Init(GPIOD, &GPIO_InitStructure);
    }
    if((MYEXTI_Linex&exti_Line7)==exti_Line7)
    {
      GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_7;
      GPIO_Init(GPIOD, &GPIO_InitStructure);
    }
    if((MYEXTI_Linex&exti_Line8)==exti_Line8)
    {
      GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_8;
      GPIO_Init(GPIOD, &GPIO_InitStructure);
    }
    if((MYEXTI_Linex&exti_Line9)==exti_Line9)
    {
      GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_9;
      GPIO_Init(GPIOD, &GPIO_InitStructure);
    }
    if((MYEXTI_Linex&exti_Line10)==exti_Line10)
    {
      GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_10;
      GPIO_Init(GPIOD, &GPIO_InitStructure);
    }
    if((MYEXTI_Linex&exti_Line11)==exti_Line11)
    {
      GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_11;
      GPIO_Init(GPIOD, &GPIO_InitStructure);
    }
    if((MYEXTI_Linex&exti_Line12)==exti_Line12)
    {
      GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_12;
      GPIO_Init(GPIOD, &GPIO_InitStructure);
    }
    if((MYEXTI_Linex&exti_Line13)==exti_Line13)
    {
      GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_13;
      GPIO_Init(GPIOD, &GPIO_InitStructure);
    }
    if((MYEXTI_Linex&exti_Line14)==exti_Line14)
    {
      GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_14;
      GPIO_Init(GPIOD, &GPIO_InitStructure);
    }
    if((MYEXTI_Linex&exti_Line15)==exti_Line15)
    {
      GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_15;
      GPIO_Init(GPIOD, &GPIO_InitStructure);
    }
    
    if((MYEXTI_Linex&exti_Line0)==exti_Line0)GPIO_EXTILineConfig(GPIO_PortSourceGPIOD,GPIO_PinSource0);
    if((MYEXTI_Linex&exti_Line1)==exti_Line1)GPIO_EXTILineConfig(GPIO_PortSourceGPIOD,GPIO_PinSource1);
    if((MYEXTI_Linex&exti_Line2)==exti_Line2)GPIO_EXTILineConfig(GPIO_PortSourceGPIOD,GPIO_PinSource2);
    if((MYEXTI_Linex&exti_Line3)==exti_Line3)GPIO_EXTILineConfig(GPIO_PortSourceGPIOD,GPIO_PinSource3);
    if((MYEXTI_Linex&exti_Line4)==exti_Line4)GPIO_EXTILineConfig(GPIO_PortSourceGPIOD,GPIO_PinSource4);
    if((MYEXTI_Linex&exti_Line5)==exti_Line5)GPIO_EXTILineConfig(GPIO_PortSourceGPIOD,GPIO_PinSource5);
    if((MYEXTI_Linex&exti_Line6)==exti_Line6)GPIO_EXTILineConfig(GPIO_PortSourceGPIOD,GPIO_PinSource6);
    if((MYEXTI_Linex&exti_Line7)==exti_Line7)GPIO_EXTILineConfig(GPIO_PortSourceGPIOD,GPIO_PinSource7);
    if((MYEXTI_Linex&exti_Line8)==exti_Line8)GPIO_EXTILineConfig(GPIO_PortSourceGPIOD,GPIO_PinSource8);
    if((MYEXTI_Linex&exti_Line9)==exti_Line9)GPIO_EXTILineConfig(GPIO_PortSourceGPIOD,GPIO_PinSource9);
    if((MYEXTI_Linex&exti_Line10)==exti_Line10)GPIO_EXTILineConfig(GPIO_PortSourceGPIOD,GPIO_PinSource10);
    if((MYEXTI_Linex&exti_Line11)==exti_Line11)GPIO_EXTILineConfig(GPIO_PortSourceGPIOD,GPIO_PinSource11);
    if((MYEXTI_Linex&exti_Line12)==exti_Line12)GPIO_EXTILineConfig(GPIO_PortSourceGPIOD,GPIO_PinSource12);
    if((MYEXTI_Linex&exti_Line13)==exti_Line13)GPIO_EXTILineConfig(GPIO_PortSourceGPIOD,GPIO_PinSource13);
    if((MYEXTI_Linex&exti_Line14)==exti_Line14)GPIO_EXTILineConfig(GPIO_PortSourceGPIOD,GPIO_PinSource14);
    if((MYEXTI_Linex&exti_Line15)==exti_Line15)GPIO_EXTILineConfig(GPIO_PortSourceGPIOD,GPIO_PinSource15);
  }
  
    /**********MyEXTI_GPIOE**********/
  
  if((MyEXTI_GPIOx&exti_GPIOE)==exti_GPIOE)
  {
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE,ENABLE);
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    if((MYEXTI_Linex&exti_Line0)==exti_Line0)
    {
      GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0;
      GPIO_Init(GPIOE, &GPIO_InitStructure);
    }
    if((MYEXTI_Linex&exti_Line1)==exti_Line1)
    {
      GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_1;
      GPIO_Init(GPIOE, &GPIO_InitStructure);
    }
    if((MYEXTI_Linex&exti_Line2)==exti_Line2)
    {
      GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_2;
      GPIO_Init(GPIOE, &GPIO_InitStructure);
    }
    if((MYEXTI_Linex&exti_Line3)==exti_Line3)
    {
      GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_3;
      GPIO_Init(GPIOE, &GPIO_InitStructure);
    }
    if((MYEXTI_Linex&exti_Line4)==exti_Line4)
    {
      GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_4;
      GPIO_Init(GPIOE, &GPIO_InitStructure);
    }
    if((MYEXTI_Linex&exti_Line5)==exti_Line5)
    {
      GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_5;
      GPIO_Init(GPIOE, &GPIO_InitStructure);
    }
    if((MYEXTI_Linex&exti_Line6)==exti_Line6)
    {
      GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_6;
      GPIO_Init(GPIOE, &GPIO_InitStructure);
    }
    if((MYEXTI_Linex&exti_Line7)==exti_Line7)
    {
      GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_7;
      GPIO_Init(GPIOE, &GPIO_InitStructure);
    }
    if((MYEXTI_Linex&exti_Line8)==exti_Line8)
    {
      GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_8;
      GPIO_Init(GPIOE, &GPIO_InitStructure);
    }
    if((MYEXTI_Linex&exti_Line9)==exti_Line9)
    {
      GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_9;
      GPIO_Init(GPIOE, &GPIO_InitStructure);
    }
    if((MYEXTI_Linex&exti_Line10)==exti_Line10)
    {
      GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_10;
      GPIO_Init(GPIOE, &GPIO_InitStructure);
    }
    if((MYEXTI_Linex&exti_Line11)==exti_Line11)
    {
      GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_11;
      GPIO_Init(GPIOE, &GPIO_InitStructure);
    }
    if((MYEXTI_Linex&exti_Line12)==exti_Line12)
    {
      GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_12;
      GPIO_Init(GPIOE, &GPIO_InitStructure);
    }
    if((MYEXTI_Linex&exti_Line13)==exti_Line13)
    {
      GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_13;
      GPIO_Init(GPIOE, &GPIO_InitStructure);
    }
    if((MYEXTI_Linex&exti_Line14)==exti_Line14)
    {
      GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_14;
      GPIO_Init(GPIOE, &GPIO_InitStructure);
    }
    if((MYEXTI_Linex&exti_Line15)==exti_Line15)
    {
      GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_15;
      GPIO_Init(GPIOE, &GPIO_InitStructure);
    }
    
    if((MYEXTI_Linex&exti_Line0)==exti_Line0)GPIO_EXTILineConfig(GPIO_PortSourceGPIOE,GPIO_PinSource0);
    if((MYEXTI_Linex&exti_Line1)==exti_Line1)GPIO_EXTILineConfig(GPIO_PortSourceGPIOE,GPIO_PinSource1);
    if((MYEXTI_Linex&exti_Line2)==exti_Line2)GPIO_EXTILineConfig(GPIO_PortSourceGPIOE,GPIO_PinSource2);
    if((MYEXTI_Linex&exti_Line3)==exti_Line3)GPIO_EXTILineConfig(GPIO_PortSourceGPIOE,GPIO_PinSource3);
    if((MYEXTI_Linex&exti_Line4)==exti_Line4)GPIO_EXTILineConfig(GPIO_PortSourceGPIOE,GPIO_PinSource4);
    if((MYEXTI_Linex&exti_Line5)==exti_Line5)GPIO_EXTILineConfig(GPIO_PortSourceGPIOE,GPIO_PinSource5);
    if((MYEXTI_Linex&exti_Line6)==exti_Line6)GPIO_EXTILineConfig(GPIO_PortSourceGPIOE,GPIO_PinSource6);
    if((MYEXTI_Linex&exti_Line7)==exti_Line7)GPIO_EXTILineConfig(GPIO_PortSourceGPIOE,GPIO_PinSource7);
    if((MYEXTI_Linex&exti_Line8)==exti_Line8)GPIO_EXTILineConfig(GPIO_PortSourceGPIOE,GPIO_PinSource8);
    if((MYEXTI_Linex&exti_Line9)==exti_Line9)GPIO_EXTILineConfig(GPIO_PortSourceGPIOE,GPIO_PinSource9);
    if((MYEXTI_Linex&exti_Line10)==exti_Line10)GPIO_EXTILineConfig(GPIO_PortSourceGPIOE,GPIO_PinSource10);
    if((MYEXTI_Linex&exti_Line11)==exti_Line11)GPIO_EXTILineConfig(GPIO_PortSourceGPIOE,GPIO_PinSource11);
    if((MYEXTI_Linex&exti_Line12)==exti_Line12)GPIO_EXTILineConfig(GPIO_PortSourceGPIOE,GPIO_PinSource12);
    if((MYEXTI_Linex&exti_Line13)==exti_Line13)GPIO_EXTILineConfig(GPIO_PortSourceGPIOE,GPIO_PinSource13);
    if((MYEXTI_Linex&exti_Line14)==exti_Line14)GPIO_EXTILineConfig(GPIO_PortSourceGPIOE,GPIO_PinSource14);
    if((MYEXTI_Linex&exti_Line15)==exti_Line15)GPIO_EXTILineConfig(GPIO_PortSourceGPIOE,GPIO_PinSource15);
  }
  
  /**********MyEXTI_GPIOF**********/
  
  if((MyEXTI_GPIOx&exti_GPIOF)==exti_GPIOF)
  {
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF,ENABLE);
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    if((MYEXTI_Linex&exti_Line0)==exti_Line0)
    {
      GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0;
      GPIO_Init(GPIOF, &GPIO_InitStructure);
    }
    if((MYEXTI_Linex&exti_Line1)==exti_Line1)
    {
      GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_1;
      GPIO_Init(GPIOF, &GPIO_InitStructure);
    }
    if((MYEXTI_Linex&exti_Line2)==exti_Line2)
    {
      GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_2;
      GPIO_Init(GPIOF, &GPIO_InitStructure);
    }
    if((MYEXTI_Linex&exti_Line3)==exti_Line3)
    {
      GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_3;
      GPIO_Init(GPIOF, &GPIO_InitStructure);
    }
    if((MYEXTI_Linex&exti_Line4)==exti_Line4)
    {
      GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_4;
      GPIO_Init(GPIOF, &GPIO_InitStructure);
    }
    if((MYEXTI_Linex&exti_Line5)==exti_Line5)
    {
      GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_5;
      GPIO_Init(GPIOF, &GPIO_InitStructure);
    }
    if((MYEXTI_Linex&exti_Line6)==exti_Line6)
    {
      GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_6;
      GPIO_Init(GPIOF, &GPIO_InitStructure);
    }
    if((MYEXTI_Linex&exti_Line7)==exti_Line7)
    {
      GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_7;
      GPIO_Init(GPIOF, &GPIO_InitStructure);
    }
    if((MYEXTI_Linex&exti_Line8)==exti_Line8)
    {
      GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_8;
      GPIO_Init(GPIOF, &GPIO_InitStructure);
    }
    if((MYEXTI_Linex&exti_Line9)==exti_Line9)
    {
      GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_9;
      GPIO_Init(GPIOF, &GPIO_InitStructure);
    }
    if((MYEXTI_Linex&exti_Line10)==exti_Line10)
    {
      GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_10;
      GPIO_Init(GPIOF, &GPIO_InitStructure);
    }
    if((MYEXTI_Linex&exti_Line11)==exti_Line11)
    {
      GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_11;
      GPIO_Init(GPIOF, &GPIO_InitStructure);
    }
    if((MYEXTI_Linex&exti_Line12)==exti_Line12)
    {
      GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_12;
      GPIO_Init(GPIOF, &GPIO_InitStructure);
    }
    if((MYEXTI_Linex&exti_Line13)==exti_Line13)
    {
      GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_13;
      GPIO_Init(GPIOF, &GPIO_InitStructure);
    }
    if((MYEXTI_Linex&exti_Line14)==exti_Line14)
    {
      GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_14;
      GPIO_Init(GPIOF, &GPIO_InitStructure);
    }
    if((MYEXTI_Linex&exti_Line15)==exti_Line15)
    {
      GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_15;
      GPIO_Init(GPIOF, &GPIO_InitStructure);
    }
    
    if((MYEXTI_Linex&exti_Line0)==exti_Line0)GPIO_EXTILineConfig(GPIO_PortSourceGPIOF,GPIO_PinSource0);
    if((MYEXTI_Linex&exti_Line1)==exti_Line1)GPIO_EXTILineConfig(GPIO_PortSourceGPIOF,GPIO_PinSource1);
    if((MYEXTI_Linex&exti_Line2)==exti_Line2)GPIO_EXTILineConfig(GPIO_PortSourceGPIOF,GPIO_PinSource2);
    if((MYEXTI_Linex&exti_Line3)==exti_Line3)GPIO_EXTILineConfig(GPIO_PortSourceGPIOF,GPIO_PinSource3);
    if((MYEXTI_Linex&exti_Line4)==exti_Line4)GPIO_EXTILineConfig(GPIO_PortSourceGPIOF,GPIO_PinSource4);
    if((MYEXTI_Linex&exti_Line5)==exti_Line5)GPIO_EXTILineConfig(GPIO_PortSourceGPIOF,GPIO_PinSource5);
    if((MYEXTI_Linex&exti_Line6)==exti_Line6)GPIO_EXTILineConfig(GPIO_PortSourceGPIOF,GPIO_PinSource6);
    if((MYEXTI_Linex&exti_Line7)==exti_Line7)GPIO_EXTILineConfig(GPIO_PortSourceGPIOF,GPIO_PinSource7);
    if((MYEXTI_Linex&exti_Line8)==exti_Line8)GPIO_EXTILineConfig(GPIO_PortSourceGPIOF,GPIO_PinSource8);
    if((MYEXTI_Linex&exti_Line9)==exti_Line9)GPIO_EXTILineConfig(GPIO_PortSourceGPIOF,GPIO_PinSource9);
    if((MYEXTI_Linex&exti_Line10)==exti_Line10)GPIO_EXTILineConfig(GPIO_PortSourceGPIOF,GPIO_PinSource10);
    if((MYEXTI_Linex&exti_Line11)==exti_Line11)GPIO_EXTILineConfig(GPIO_PortSourceGPIOF,GPIO_PinSource11);
    if((MYEXTI_Linex&exti_Line12)==exti_Line12)GPIO_EXTILineConfig(GPIO_PortSourceGPIOF,GPIO_PinSource12);
    if((MYEXTI_Linex&exti_Line13)==exti_Line13)GPIO_EXTILineConfig(GPIO_PortSourceGPIOF,GPIO_PinSource13);
    if((MYEXTI_Linex&exti_Line14)==exti_Line14)GPIO_EXTILineConfig(GPIO_PortSourceGPIOF,GPIO_PinSource14);
    if((MYEXTI_Linex&exti_Line15)==exti_Line15)GPIO_EXTILineConfig(GPIO_PortSourceGPIOF,GPIO_PinSource15);
  }

  /**********MyEXTI_GPIOF**********/
  
  if((MyEXTI_GPIOx&exti_GPIOG)==exti_GPIOG)
  {
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG,ENABLE);
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    if((MYEXTI_Linex&exti_Line0)==exti_Line0)
    {
      GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0;
      GPIO_Init(GPIOG, &GPIO_InitStructure);
    }
    if((MYEXTI_Linex&exti_Line1)==exti_Line1)
    {
      GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_1;
      GPIO_Init(GPIOG, &GPIO_InitStructure);
    }
    if((MYEXTI_Linex&exti_Line2)==exti_Line2)
    {
      GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_2;
      GPIO_Init(GPIOG, &GPIO_InitStructure);
    }
    if((MYEXTI_Linex&exti_Line3)==exti_Line3)
    {
      GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_3;
      GPIO_Init(GPIOG, &GPIO_InitStructure);
    }
    if((MYEXTI_Linex&exti_Line4)==exti_Line4)
    {
      GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_4;
      GPIO_Init(GPIOG, &GPIO_InitStructure);
    }
    if((MYEXTI_Linex&exti_Line5)==exti_Line5)
    {
      GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_5;
      GPIO_Init(GPIOG, &GPIO_InitStructure);
    }
    if((MYEXTI_Linex&exti_Line6)==exti_Line6)
    {
      GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_6;
      GPIO_Init(GPIOG, &GPIO_InitStructure);
    }
    if((MYEXTI_Linex&exti_Line7)==exti_Line7)
    {
      GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_7;
      GPIO_Init(GPIOG, &GPIO_InitStructure);
    }
    if((MYEXTI_Linex&exti_Line8)==exti_Line8)
    {
      GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_8;
      GPIO_Init(GPIOG, &GPIO_InitStructure);
    }
    if((MYEXTI_Linex&exti_Line9)==exti_Line9)
    {
      GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_9;
      GPIO_Init(GPIOG, &GPIO_InitStructure);
    }
    if((MYEXTI_Linex&exti_Line10)==exti_Line10)
    {
      GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_10;
      GPIO_Init(GPIOG, &GPIO_InitStructure);
    }
    if((MYEXTI_Linex&exti_Line11)==exti_Line11)
    {
      GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_11;
      GPIO_Init(GPIOG, &GPIO_InitStructure);
    }
    if((MYEXTI_Linex&exti_Line12)==exti_Line12)
    {
      GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_12;
      GPIO_Init(GPIOG, &GPIO_InitStructure);
    }
    if((MYEXTI_Linex&exti_Line13)==exti_Line13)
    {
      GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_13;
      GPIO_Init(GPIOG, &GPIO_InitStructure);
    }
    if((MYEXTI_Linex&exti_Line14)==exti_Line14)
    {
      GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_14;
      GPIO_Init(GPIOG, &GPIO_InitStructure);
    }
    if((MYEXTI_Linex&exti_Line15)==exti_Line15)
    {
      GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_15;
      GPIO_Init(GPIOG, &GPIO_InitStructure);
    }
    
    if((MYEXTI_Linex&exti_Line0)==exti_Line0)GPIO_EXTILineConfig(GPIO_PortSourceGPIOG,GPIO_PinSource0);
    if((MYEXTI_Linex&exti_Line1)==exti_Line1)GPIO_EXTILineConfig(GPIO_PortSourceGPIOG,GPIO_PinSource1);
    if((MYEXTI_Linex&exti_Line2)==exti_Line2)GPIO_EXTILineConfig(GPIO_PortSourceGPIOG,GPIO_PinSource2);
    if((MYEXTI_Linex&exti_Line3)==exti_Line3)GPIO_EXTILineConfig(GPIO_PortSourceGPIOG,GPIO_PinSource3);
    if((MYEXTI_Linex&exti_Line4)==exti_Line4)GPIO_EXTILineConfig(GPIO_PortSourceGPIOG,GPIO_PinSource4);
    if((MYEXTI_Linex&exti_Line5)==exti_Line5)GPIO_EXTILineConfig(GPIO_PortSourceGPIOG,GPIO_PinSource5);
    if((MYEXTI_Linex&exti_Line6)==exti_Line6)GPIO_EXTILineConfig(GPIO_PortSourceGPIOG,GPIO_PinSource6);
    if((MYEXTI_Linex&exti_Line7)==exti_Line7)GPIO_EXTILineConfig(GPIO_PortSourceGPIOG,GPIO_PinSource7);
    if((MYEXTI_Linex&exti_Line8)==exti_Line8)GPIO_EXTILineConfig(GPIO_PortSourceGPIOG,GPIO_PinSource8);
    if((MYEXTI_Linex&exti_Line9)==exti_Line9)GPIO_EXTILineConfig(GPIO_PortSourceGPIOG,GPIO_PinSource9);
    if((MYEXTI_Linex&exti_Line10)==exti_Line10)GPIO_EXTILineConfig(GPIO_PortSourceGPIOG,GPIO_PinSource10);
    if((MYEXTI_Linex&exti_Line11)==exti_Line11)GPIO_EXTILineConfig(GPIO_PortSourceGPIOG,GPIO_PinSource11);
    if((MYEXTI_Linex&exti_Line12)==exti_Line12)GPIO_EXTILineConfig(GPIO_PortSourceGPIOG,GPIO_PinSource12);
    if((MYEXTI_Linex&exti_Line13)==exti_Line13)GPIO_EXTILineConfig(GPIO_PortSourceGPIOG,GPIO_PinSource13);
    if((MYEXTI_Linex&exti_Line14)==exti_Line14)GPIO_EXTILineConfig(GPIO_PortSourceGPIOG,GPIO_PinSource14);
    if((MYEXTI_Linex&exti_Line15)==exti_Line15)GPIO_EXTILineConfig(GPIO_PortSourceGPIOG,GPIO_PinSource15);
  }
  
/***********************与GPIO无关的EXTI配置***************************/
  
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  if((MYEXTI_Linex&exti_Line0)==exti_Line0)
  {
    EXTI_InitStructure.EXTI_Line=EXTI_Line0;
    EXTI_Init(&EXTI_InitStructure);
  }
  if((MYEXTI_Linex&exti_Line1)==exti_Line1)
  {
    EXTI_InitStructure.EXTI_Line=EXTI_Line1;
    EXTI_Init(&EXTI_InitStructure);
  }
  if((MYEXTI_Linex&exti_Line2)==exti_Line2)
  {
    EXTI_InitStructure.EXTI_Line=EXTI_Line2;
    EXTI_Init(&EXTI_InitStructure);
  }
  if((MYEXTI_Linex&exti_Line3)==exti_Line3)
  {
    EXTI_InitStructure.EXTI_Line=EXTI_Line3;
    EXTI_Init(&EXTI_InitStructure);
  }
  if((MYEXTI_Linex&exti_Line4)==exti_Line4)
  {
    EXTI_InitStructure.EXTI_Line=EXTI_Line4;
    EXTI_Init(&EXTI_InitStructure);
  }
  if((MYEXTI_Linex&exti_Line5)==exti_Line5)
  {
    EXTI_InitStructure.EXTI_Line=EXTI_Line5;
    EXTI_Init(&EXTI_InitStructure);
  }
  if((MYEXTI_Linex&exti_Line6)==exti_Line6)
  {
    EXTI_InitStructure.EXTI_Line=EXTI_Line6;
    EXTI_Init(&EXTI_InitStructure);
  }
  if((MYEXTI_Linex&exti_Line7)==exti_Line7)
  {
    EXTI_InitStructure.EXTI_Line=EXTI_Line8;
    EXTI_Init(&EXTI_InitStructure);
  }
  if((MYEXTI_Linex&exti_Line8)==exti_Line8)
  {
    EXTI_InitStructure.EXTI_Line=EXTI_Line8;
    EXTI_Init(&EXTI_InitStructure);
  }
  if((MYEXTI_Linex&exti_Line9)==exti_Line9)
  {
    EXTI_InitStructure.EXTI_Line=EXTI_Line8;
    EXTI_Init(&EXTI_InitStructure);
  }
  if((MYEXTI_Linex&exti_Line10)==exti_Line10)
  {
    EXTI_InitStructure.EXTI_Line=EXTI_Line10;
    EXTI_Init(&EXTI_InitStructure);
  }
  if((MYEXTI_Linex&exti_Line11)==exti_Line11)
  {
    EXTI_InitStructure.EXTI_Line=EXTI_Line11;
    EXTI_Init(&EXTI_InitStructure);
  }
  if((MYEXTI_Linex&exti_Line12)==exti_Line12)
  {
    EXTI_InitStructure.EXTI_Line=EXTI_Line12;
    EXTI_Init(&EXTI_InitStructure);
  }
  if((MYEXTI_Linex&exti_Line13)==exti_Line13)
  {
    EXTI_InitStructure.EXTI_Line=EXTI_Line13;
    EXTI_Init(&EXTI_InitStructure);
  }
  if((MYEXTI_Linex&exti_Line14)==exti_Line14)
  {
    EXTI_InitStructure.EXTI_Line=EXTI_Line14;
    EXTI_Init(&EXTI_InitStructure);
  }
  if((MYEXTI_Linex&exti_Line15)==exti_Line15)
  {
    EXTI_InitStructure.EXTI_Line=EXTI_Line15;
    EXTI_Init(&EXTI_InitStructure);
  }
  
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	//先占优先级4位,共16级
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;					//从优先级
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//使能外部中断通道
  if((MYEXTI_Linex&exti_Line0)==exti_Line0)
  {
    NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
    NVIC_Init(&NVIC_InitStructure);
  }
  if((MYEXTI_Linex&exti_Line1)==exti_Line1)
  {
    NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;
    NVIC_Init(&NVIC_InitStructure);
  }
  if((MYEXTI_Linex&exti_Line2)==exti_Line2)
  {
    NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;
    NVIC_Init(&NVIC_InitStructure);
  }
  if((MYEXTI_Linex&exti_Line3)==exti_Line3)
  {
    NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn;
    NVIC_Init(&NVIC_InitStructure);
  }
  if((MYEXTI_Linex&exti_Line4)==exti_Line4)
  {
    NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;
    NVIC_Init(&NVIC_InitStructure);
  }
  if((MYEXTI_Linex&exti_Line5)==exti_Line5)
  {
    NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
    NVIC_Init(&NVIC_InitStructure);
  }
  if((MYEXTI_Linex&exti_Line6)==exti_Line6)
  {
    NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
    NVIC_Init(&NVIC_InitStructure);
  }
  if((MYEXTI_Linex&exti_Line7)==exti_Line7)
  {
    NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
    NVIC_Init(&NVIC_InitStructure);
  }
  if((MYEXTI_Linex&exti_Line8)==exti_Line8)
  {
    NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
    NVIC_Init(&NVIC_InitStructure);
  }
  if((MYEXTI_Linex&exti_Line9)==exti_Line9)
  {
    NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
    NVIC_Init(&NVIC_InitStructure);
  }
  if((MYEXTI_Linex&exti_Line10)==exti_Line10)
  {
    NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
    NVIC_Init(&NVIC_InitStructure);
  }
  if((MYEXTI_Linex&exti_Line11)==exti_Line11)
  {
    NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
    NVIC_Init(&NVIC_InitStructure);
  }
  if((MYEXTI_Linex&exti_Line12)==exti_Line12)
  {
    NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
    NVIC_Init(&NVIC_InitStructure);
  }
  if((MYEXTI_Linex&exti_Line13)==exti_Line13)
  {
    NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
    NVIC_Init(&NVIC_InitStructure);
  }
  if((MYEXTI_Linex&exti_Line14)==exti_Line14)
  {
    NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
    NVIC_Init(&NVIC_InitStructure);
  }
  if((MYEXTI_Linex&exti_Line15)==exti_Line15)
  {
    NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
    NVIC_Init(&NVIC_InitStructure);
  }
  
  
//  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
//  GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_12;
//  GPIO_Init(GPIOA, &GPIO_InitStructure);
  
}
 


void EXTI0_IRQHandler(void)
{
  if(EXTI_GetITStatus(EXTI_Line0) != RESET)	  //检查指定的EXTI0线路触发请求发生与否
	{
        if(A2)//上升沿中断
        {
          dir[1] = B2;
          if(B2 == 0)pul_cnt[1]-- ;//dir = 0;
          else pul_cnt[1]++ ;//dir = 1;
        }
        else//下降沿中断
        {
          dir[1] = !B2;
          if(B2 == 1)pul_cnt[1]-- ;//dir = 0;
          else pul_cnt[1]++ ;//dir = 1;
        }
        if(mode&(1<<1))//编码器2极坐标
        {
          if(abs(pul_cnt[1]) >= ful_cicl_pul[1])
          {
            pul_cnt[1] = 0;
          }
        }
    //    
        EXTI_ClearITPendingBit(EXTI_Line0);  //清除EXTI0线路挂起位
	}
}

void EXTI1_IRQHandler(void)
{
  if(EXTI_GetITStatus(EXTI_Line1) != RESET)	  //检查指定的EXTI0线路触发请求发生与否
	{	  
        if(B2)//上升沿中断
        {
          dir[1] = !A2;
          if(A2 == 0)pul_cnt[1]++ ;//dir = 0;
          else pul_cnt[1]-- ;//dir = 1;
        }
        else//下降沿中断
        {
          dir[1] = A2;
          if(A2 == 1)pul_cnt[1]++ ;//dir = 0;
          else pul_cnt[1]-- ;//dir = 1;
        }
        if(mode&(1<<1))//编码器2极坐标
        {
          if(abs(pul_cnt[1]) >= ful_cicl_pul[1])
          {
            pul_cnt[1] = 0;
          }
        }
        EXTI_ClearITPendingBit(EXTI_Line1);  //清除EXTI0线路挂起位
	}
}

void EXTI2_IRQHandler(void)
{
  if(EXTI_GetITStatus(EXTI_Line2) != RESET)	  //检查指定的EXTI0线路触发请求发生与否
	{
    
    EXTI_ClearITPendingBit(EXTI_Line2);  //清除EXTI0线路挂起位
	}
}

void EXTI3_IRQHandler(void)
{
  if(EXTI_GetITStatus(EXTI_Line3) != RESET)	  //检查指定的EXTI0线路触发请求发生与否
	{	  
    
    EXTI_ClearITPendingBit(EXTI_Line3);  //清除EXTI0线路挂起位
	}
}

void EXTI4_IRQHandler(void)
{
  if(EXTI_GetITStatus(EXTI_Line4) != RESET)	  //检查指定的EXTI0线路触发请求发生与否
	{
        if(status&0x01)
        {
            if(X2)//上升沿  X2改为固定的距离模式
            {
                if(dir[1]==1)nboftns[1]++;
                else nboftns[1]--;//计数
            }
        }
        EXTI_ClearITPendingBit(EXTI_Line4);  //清除EXTI0线路挂起位
	}
}

extern void Set_Out(void);
void EXTI9_5_IRQHandler(void)
{
  if(EXTI_GetITStatus(EXTI_Line5) != RESET)	  //检查指定的EXTI0线路触发请求发生与否
	{
        if(status&0x01)
        {
            if(X1)//上升沿
            {
                if(mode&0x01)//角度模式
                {
                    if(first_flag&0x01)//不是第一次碰到X
                    {
                        if(dir[0]==1)nboftns[0]++;
                        else nboftns[0]--;
                        if((nboftns[0]%jsb[0])==0)
                        {
                            nboftns[0] = 0;
                            pul_cnt[0] = 0;
                        }
                    }
                    else//第一次碰到X
                    {
                        nboftns[0] = 0;
                        pul_cnt[0] = 0;
                        nboftns[1] = 0;
                        pul_cnt[1] = 0;
                        first_flag |= 0x01;
                        out_sta |= 0x80;//打开最后一个输出
                        Set_Out();
                    }
                }
                else//距离模式
                {
                    if(dir[0]==1)nboftns[0]++;
                    else nboftns[0]--;
                }
            }
        }
        
        EXTI_ClearITPendingBit(EXTI_Line5);  //清除EXTI0线路挂起位
	}
  
  if(EXTI_GetITStatus(EXTI_Line6) != RESET)	  //检查指定的EXTI0线路触发请求发生与否
	{
		if(A1)//上升沿中断
        {
          dir[0] = B1;
          if(B1 == 0)pul_cnt[0]-- ;//dir = 0;
          else pul_cnt[0]++ ;//dir = 1;
        }
        else//下降沿中断
        {
          dir[0] = !B1;
          if(B1 == 1)pul_cnt[0]-- ;//dir = 0;
          else pul_cnt[0]++ ;//dir = 1;
        }
        EXTI_ClearITPendingBit(EXTI_Line6);  //清除EXTI0线路挂起位
	}
  
  if(EXTI_GetITStatus(EXTI_Line7) != RESET)	  //检查指定的EXTI0线路触发请求发生与否
	{	  
		if(B1)//上升沿中断
        {
          dir[0] = !A1;
          if(A1 == 0)pul_cnt[0]++ ;//dir = 0;
          else pul_cnt[0]-- ;//dir = 1;
        }
        else//下降沿中断
        {
          dir[0] = A1;
          if(A1 == 1)pul_cnt[0]++ ;//dir = 0;
          else pul_cnt[0]-- ;//dir = 1;
        }
        EXTI_ClearITPendingBit(EXTI_Line7);  //清除EXTI0线路挂起位
	}
  
  if(EXTI_GetITStatus(EXTI_Line8) != RESET)	  //检查指定的EXTI0线路触发请求发生与否
	{	  
		
    status |= (1<<1);
    EXTI_ClearITPendingBit(EXTI_Line8);  //清除EXTI0线路挂起位
	}
  
  if(EXTI_GetITStatus(EXTI_Line9) != RESET)	  //检查指定的EXTI0线路触发请求发生与否
	{	  
		
    EXTI_ClearITPendingBit(EXTI_Line9);  //清除EXTI0线路挂起位
	}
}
 
void EXTI15_10_IRQHandler(void)
{ 
  if(EXTI_GetITStatus(EXTI_Line10) != RESET)	  //检查指定的EXTI0线路触发请求发生与否
	{	  
    
    EXTI_ClearITPendingBit(EXTI_Line10);  //清除EXTI0线路挂起位
	}
  
  if(EXTI_GetITStatus(EXTI_Line11) != RESET)	  //检查指定的EXTI0线路触发请求发生与否
	{
    status |= (1<<2);
    EXTI_ClearITPendingBit(EXTI_Line11);  //清除EXTI0线路挂起位
	}
  
  if(EXTI_GetITStatus(EXTI_Line12) != RESET)	  //检查指定的EXTI0线路触发请求发生与否
	{	
    if(!EX_Trig)//选择下降沿
    {
      if(status&(1<<4))//软件滤波标志，隔50ms才可再次触发
      {
        status |= (1<<5);//主程序检测到触发
        status &= ~(1<<4);//软件滤波标志，隔1s才可再次触发
      }
    }
    EXTI_ClearITPendingBit(EXTI_Line12);  //清除EXTI0线路挂起位
	}
  
  if(EXTI_GetITStatus(EXTI_Line13) != RESET)	  //检查指定的EXTI0线路触发请求发生与否
	{
    
    EXTI_ClearITPendingBit(EXTI_Line13);  //清除EXTI0线路挂起位
	}
  
  if(EXTI_GetITStatus(EXTI_Line14) != RESET)	  //检查指定的EXTI0线路触发请求发生与否
	{
		
    EXTI_ClearITPendingBit(EXTI_Line14);  //清除EXTI0线路挂起位
	}
  
  if(EXTI_GetITStatus(EXTI_Line15) != RESET)	  //检查指定的EXTI0线路触发请求发生与否
	{
    
    EXTI_ClearITPendingBit(EXTI_Line15);  //清除EXTI0线路挂起位
	}
}

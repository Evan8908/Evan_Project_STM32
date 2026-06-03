/**************************************************

注意：要使用自定义串口库函数，务必设置 Options Target  
在Target右上角栏目勾选 USE Micro LIB，否则会死机。
USART1：Tx--PA9  Rx--PA10    
USART2：Tx--PA2  Rx--PA3     
USART3：Tx--PB10 Rx--PB11    
UART4： Tx--PC10 Rx--PC11
UART5： Tx--PC12 Rx--PD2
***************************************************/
#include "usart.h"
#include "sys.h"
#include "stdio.h"
#include "string.h"
#include "stdarg.h"
#include "delay.h"
#include "led.h"
#include "24cxx.h" 
#include "timer.h"
#include "exti.h"


char rx1_buffer[RX1_BUFF_MAX] = "";
u8 rx1_buffer_cnt = 0;

u8 mode = 0;//0平面直角平坐标模式   1极坐标模式  bit0 编码器1  bit1 编码器2
u8 status = 0;//状态寄存器 bit0 工作使能  bit1 清零使能1 byte2 清零使能2 
u16 t_f = 1;//发送频率 1-2000
u8 out_sta = 0;//输出状态
u16 ful_cicl_pul = 1000;//整圈脉冲数1-65000

void Usart_init(u8 Usartx,u32 baud)
{
  
  GPIO_InitTypeDef GPIO_InitStructure;
  USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
  
  if((Usartx&Usart1)==Usart1)	//&使初始化时可以用|运算符
  {
    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO, ENABLE);
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;         //Tx1
    GPIO_Init(GPIOA, &GPIO_InitStructure);
   
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;   //Rx1
    GPIO_Init(GPIOA, &GPIO_InitStructure); 
    
    
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0 ;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;		//
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
    NVIC_Init(&NVIC_InitStructure);	//根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器USART1
    
    USART_InitStructure.USART_BaudRate = baud;//波特率
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;//8位数据
    USART_InitStructure.USART_StopBits = USART_StopBits_1;//在帧结尾传输1个停止位
    USART_InitStructure.USART_Parity = USART_Parity_No;//无校验
    USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;//发送和接收都使能
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//硬件流控制失能
    USART_Init(USART1,&USART_InitStructure);
    
    USART_ClearFlag(USART1, USART_FLAG_TC);   //防止第一个数据被覆盖
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//使能或者失能指定的USART中断
    
    USART_Cmd(USART1, ENABLE);//使能或者失能USART外设
    
//    printf("USART1 IS OK!\r\n");
  }
  
  if((Usartx&Usart2)==Usart2)
  {
    
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);//使能或者失能APB1外设时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO, ENABLE);
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;         //Tx2
    GPIO_Init(GPIOA, &GPIO_InitStructure);
   
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;   //Rx2
    GPIO_Init(GPIOA, &GPIO_InitStructure); 
    
    
    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0 ;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;		//
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
    NVIC_Init(&NVIC_InitStructure);	//根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器USART1
    
    USART_InitStructure.USART_BaudRate = baud;//波特率
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;//8位数据
    USART_InitStructure.USART_StopBits = USART_StopBits_1;//在帧结尾传输1个停止位
    USART_InitStructure.USART_Parity = USART_Parity_No;//无校验
    USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;//发送和接收都使能
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//硬件流控制失能
    USART_Init(USART2,&USART_InitStructure);
    
    USART_ClearFlag(USART2, USART_FLAG_TC);   //防止第一个数据被覆盖
    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//使能或者失能指定的USART中断
    
    USART_Cmd(USART2, ENABLE);//使能或者失能USART外设
    
//    USART2_printf(" USART2 IS OK!\r\n");    //初次发送留1空格
  }
  
  if((Usartx&Usart3)==Usart3)
  {
    
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);//使能或者失能APB1外设时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO, ENABLE);
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;         //Tx3
    GPIO_Init(GPIOB, &GPIO_InitStructure);
   
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;   //Rx3
    GPIO_Init(GPIOB, &GPIO_InitStructure); 
    
    
    NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0 ;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;		//
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
    NVIC_Init(&NVIC_InitStructure);	//根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器USART1
    
    USART_InitStructure.USART_BaudRate = baud;//波特率
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;//8位数据
    USART_InitStructure.USART_StopBits = USART_StopBits_1;//在帧结尾传输1个停止位
    USART_InitStructure.USART_Parity = USART_Parity_No;//无校验
    USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;//发送和接收都使能
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//硬件流控制失能
    USART_Init(USART3,&USART_InitStructure);
    
    USART_ClearFlag(USART3, USART_FLAG_TC);   //防止第一个数据被覆盖
    USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//使能或者失能指定的USART中断
    
    USART_Cmd(USART3, ENABLE);//使能或者失能USART外设
    
//    USART3_printf("USART3 IS OK!\r\n");
  }
}

void Rx1_Get_Buff(void)
{
  rx1_buffer[rx1_buffer_cnt++] = (u8)USART_ReceiveData(USART1);
  if(rx1_buffer_cnt >= RX1_BUFF_MAX)rx1_buffer_cnt = 0;
}

void Rx1_Buffer_Init(void)
{
  u8 i;
  for(i=0;i<=RX1_BUFF_MAX;i++)
  {
    rx1_buffer[i]=0;
  }
  rx1_buffer_cnt=0;    //数组当前位指针
}

void deal_rx1_buff(void)//处理串口1数据
{
  u8 flag = 0;
  u8 num1 = 0;
  static u8 rst_flag = 0;
  if(rst_flag)
  {
    if(Find_Str_Str(rx1_buffer,"$yes"))//确认恢复默认参数
    {
      u8 str_temp[10] = "";
      u32 num_temp = 0;
      USART1_printf("#reseting...\r\n");
      rst_flag = 0;
      flag = 1;
      mode = 0;
      t_f = 1;
      pul_cnt[0] = 0;
      pul_cnt[1] = 0;
      nboftns[0] = 0;
      nboftns[1] = 0;
      out_sta = 0;
      ful_cicl_pul = 1000;
      
      //保存编码器1数据
      Clear_String(str_temp,10);
      AT24CXX_Write(ENCODER_1_EEAD,str_temp,10);
      
      //保存编码器2数据
      Clear_String(str_temp,10);
      AT24CXX_Write(ENCODER_2_EEAD,str_temp,10);
      
      //保存运行配置
//      str_temp[0] = 0;
//      str_temp[1] = (u8)t_f;
//      str_temp[2] = 0;
      str_temp[0] = mode;
      str_temp[1] = (u8)t_f;
      str_temp[2] = (u8)(t_f>>8);
      str_temp[3] = out_sta;
      str_temp[4] = (u8)ful_cicl_pul;
      str_temp[5] = (u8)(ful_cicl_pul>>8);
      AT24CXX_Write(RUN_PARA_EEAD,str_temp,6);
      
      Timer_init(COMMON,Tim1,7200,10000/t_f);//10000对应1s  1-2000频率对应   10000-5 数值 
      
      USART1_printf("#Parameter was reset!\r\n");
    }
    else if(Find_Str_Str(rx1_buffer,"$no"))//取消操作
    {
      rst_flag = 0;
      USART1_printf("#Option was canceled!\r\n");
    }
  }
  else
  {
    if(Find_Str_Str(rx1_buffer,"$Hello"))//检测指令
    {
      USART1_printf("#Hi!\r\n");
      flag = 1;
    }
    else if(Find_Str_Str(rx1_buffer,"$Read"))//恢复默认参数
    {
      u16 temp = (u8)(mode&(1<<0)) + (u8)(mode&(1<<1)) + out_sta + t_f + ful_cicl_pul;
      USART1_printf("#cfg:%d,%d,%d,%d,%d,%d\r\n",(u8)(mode&(1<<0)),(u8)(mode&(1<<1)),out_sta,t_f,ful_cicl_pul,temp);
      flag = 1;
    }
    else if(Find_Str_Str(rx1_buffer,"$Start"))//开始发送
    {
      USART1_printf("#Start!\r\n");
      status |= (1<<0);
      flag = 1;
    }
    else if(Find_Str_Str(rx1_buffer,"$Stop"))//停止发送
    {
      USART1_printf("#Stop!\r\n");
      status &= ~(1<<0);
      flag = 1;
    }
    else if(Find_Str_Str(rx1_buffer,"$Clear 1"))//清空编码器1脉冲计数
    {
  //    USART1_printf("#Clr 1!");
      status |= (1<<1);
      flag = 1;
    }
    else if(Find_Str_Str(rx1_buffer,"$Clear 2"))//清空编码器2脉冲计数
    {
  //    USART1_printf("#Clr 2!");
      status |= (1<<2);
      flag = 1;
    }
    else if(Find_Str_Str(rx1_buffer,"$RESET!!!"))//恢复默认参数
    {
      USART1_printf("#Will be reset,yes or no?\r\n");
      flag = 1;
      rst_flag = 1;
    }
    
    //$set:编码器1模式,编码器2模式,8位输出状态,上传频率,整圈脉冲数,ASCII模式16位校验和end#
    //$set:0,0,1,1,1000,1002end#
    //错误0 参数总长度过短
    //错误1 和校验错误
    //错误2 编码器模式错误
    //错误3 输出状态错误
    //错误4 频率范围错误
    //错误5 整圈脉冲数范围错误
    num1 = Find_Str_Str(rx1_buffer,END_OF_FRAME);
    if(num1)
    {
      u8 num2 = Find_Str_Str(rx1_buffer,FRAME_HEAD);
      if(num1 > num2+14)
      {
        char str_temp[10] = "";
        u8 i = 0;
        u16 num_temp[6] = {0,0,0,0,0,0};//第5位：累加，用于和校验
        u16 jyh = 0;//接收和校验字节
        for(i=0;i<5;i++)
        {
          if(i==0)
          {
            num1 = num2 + strlen(FRAME_HEAD);
            num2 = Find_Str_nChar(rx1_buffer,',',1);
          }
          else if(i<5)
          {
            num1 = Find_Str_nChar(rx1_buffer,',',i);
            num2 = Find_Str_nChar(rx1_buffer,',',i+1);
          }
          if(num2 > num1+1)
          {
            Clear_String(str_temp,strlen(str_temp));
            strncpy(str_temp,rx1_buffer+num1,num2-num1);
  //          USART1_printf("(%d,%d),%s",num1,num2,str_temp);
            num_temp[i] = (u16)atoi(str_temp);
            num_temp[5] += num_temp[i];
          }
        }
        //单独解析校验和数据
        num1 = Find_Str_nChar(rx1_buffer,',',5);
        num2 = Find_Str_Str(rx1_buffer,END_OF_FRAME);
        if(num2 > num1)
        {
          Clear_String(str_temp,strlen(str_temp));
          strncpy(str_temp,rx1_buffer+num1,num2-num1-1);
  //        USART1_printf("\n(%d,%d),%s",num1,num2,str_temp);
          jyh = (u16)atoi(str_temp);
        }
        
        if(num_temp[5]==jyh)
        {
          u8 num = 0;
          if((num_temp[0]<2) && (num_temp[1]<2))
          {
            if(num_temp[0])mode |= (1<<0);else mode &= ~(1<<0);
            if(num_temp[1])mode |= (1<<1);else mode &= ~(1<<1);
            num++ ;
          }
          else USART1_printf("#Error 2\r\n");//编码器模式错误
          if(num_temp[2] <= 255)
          {
            out_sta = num_temp[2];
            num++ ;
          }
          else USART1_printf("#Error 3\r\n");//输出位数错误
          if(num_temp[3]&&(num_temp[3]<=2000))
          {
            t_f = num_temp[3];
            num++ ;
          }
//          else USART1_printf("#Error 4\r\n");//频率范围错误 //0000 0010 0100 0110 1000 1010 1100 1110 e4
          else
          {
//            USART1_printf("#Error 4\r\n");
            USART1_printf("num_temp[3]:%i\n",num_temp[3]);
            
          }
          if(num_temp[4]&&(num_temp[4]<=65000))
          {
            ful_cicl_pul = num_temp[4];
            num++ ;
          }
          else USART1_printf("#Error 5\r\n");//整圈脉冲数范围错误
          if(num == 4)
          {
            USART1_printf("#Success\r\n");
            Clear_String(str_temp,strlen(str_temp));
            str_temp[0] = mode;
            str_temp[1] = (u8)t_f;
            str_temp[2] = (u8)(t_f>>8);
            str_temp[3] = out_sta;
            str_temp[4] = (u8)ful_cicl_pul;
            str_temp[5] = (u8)(ful_cicl_pul>>8);
            AT24CXX_Write(RUN_PARA_EEAD,str_temp,6);
            Timer_init(COMMON,Tim1,7200,10000/t_f);//10000对应1s  1-2000频率对应   10000-5 数值 
          }
        }
        else USART1_printf("#Error 1\r\n");//和校验错误
        
  //      USART1_printf("\njyh:%d",jyh);
      }
      else USART1_printf("#Error 0");//参数总长度过短
      flag = 1;
    }
    
  }
  
  if(flag)Rx1_Buffer_Init();
}

void USART1_Send_Byte(u8 Byte)
{
  USART1->DR = (Byte & (uint16_t)0x01FF);
  while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
}

void USART1_printf (char *fmt, ...)                         
{
  char buffer[CMD_BUFFER_LEN+1];  // CMD_BUFFER_LEN长度自己定义吧
  char i = 0;
  
  va_list arg_ptr;
  va_start(arg_ptr, fmt);  
  vsnprintf(buffer, CMD_BUFFER_LEN+1, fmt, arg_ptr);
  while (buffer[i])
  {
          USART1_Send_Byte((u8) buffer[i++]);
  }
  va_end(arg_ptr);
}

void USART1_IRQHandler(void)
{
  if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)//检查指定的USART中断发生与否
  {
    Rx1_Get_Buff();
    USART_ClearITPendingBit(USART1, USART_IT_RXNE);//清除USARTx的中断待处理位
  }
  
  if(USART_GetITStatus(USART1, USART_IT_TC) != RESET)
  {
    
    USART_ClearITPendingBit(USART1, USART_IT_TC);//清除USARTx的中断待处理位
  }
}



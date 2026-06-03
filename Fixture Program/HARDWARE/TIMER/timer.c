/*******************功能说明***********************/
//TIM1:  CH1~CH4----------PA8~PA11(RCT6)   PA8,PB13~PB15 (C8T6)
//TIM2:  CH1~CH4----------PA0~PA3
//TIM3:  CH1~CH4----------PA6、PA7、PB0、PB1
//TIM4:  CH1~CH4----------PB6~PB9
//TIM5:  CH1~CH4----------PA0~PA3
//TIM8:  CH1~CH4----------PC6~PC9

//      Timer1用于LED指示以及语音播报定时
//      Timer2用于为A4988步进电机驱动模块模块产生脉冲
//      Timer3用于判断串口接收到的指令
/*************************************************/
#include "timer.h"
#include "delay.h"
#include "led.h"
#include "..\FIXTURE\IO_def.h"
#include "..\FIXTURE\Fixture_Run.h"


uint64_t systim_ms = 0;//系统时间 ms  1ms加一次，需要100多年才会溢出

/*********************************************************************************************
//function ：要使用的功能   COMMON-公用定时  FRE_CAP-频率测量  PWM_OUT-PWM输出  PWM_CAP-PWM捕获
//My_TIMx  ：定时器
//arr      ：自动重装值。
//psc      ：时钟预分频数  arr=10000 psc=7200  对应1s

实际传进入的数字：4，TIM3,1440,50000/0
**********************************************************************************************/
void Timer_init(u8 function,u8 My_TIMx,u16 arr,u16 psc)
{
  
  TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
  NVIC_InitTypeDef NVIC_InitStructure;
  TIM_ICInitTypeDef TIM_ICInitStructure;
  TIM_OCInitTypeDef TIM_OCInitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;
  /*********TIM1**********/
  if((My_TIMx&Tim1)==Tim1)
  {
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);//使能或者失能APB1外设时钟
    TIM_TimeBaseStructure.TIM_Prescaler = psc-1;//预分频值
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;//向上计数模式
    TIM_TimeBaseStructure.TIM_Period = arr;//在下一个更新事件装入活动的自动重装载寄存器周期的值
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;//TDTS = Tck_tim
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;//重复计数值
    TIM_TimeBaseInit(TIM1,&TIM_TimeBaseStructure);//初始化
    
    switch(function)
    {
      case COMMON  : 
                    TIM_ITConfig(TIM1, TIM_IT_Update,ENABLE);//使能或者失能指定的TIM中断
      
                    NVIC_InitStructure.NVIC_IRQChannel=TIM1_UP_IRQn;
                    NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
                    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;
                    NVIC_InitStructure.NVIC_IRQChannelSubPriority=2;
                    NVIC_Init(&NVIC_InitStructure);
                    break;
      
      case FRE_CAP : break;
      
      case PWM_OUT :
                    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO,ENABLE);//使能或者失能APB2外设时钟
                    
                    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //TIM1_CH1
                    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出
                    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
                    GPIO_Init(GPIOA, &GPIO_InitStructure);
      
                    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //选择定时器模式:TIM脉冲宽度调制模式2
                    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
                    TIM_OCInitStructure.TIM_Pulse = 0; //设置待装入捕获比较寄存器的脉冲值
                    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //输出极性:TIM输出比较极性高
                    TIM_OC2Init(TIM1, &TIM_OCInitStructure);  //根据TIM_OCInitStruct中指定的参数初始化外设TIMx
                    TIM_OC2PreloadConfig(TIM1, TIM_OCPreload_Enable);  //使能TIMx在CCR2上的预装载寄存器
                    
                    TIM_ARRPreloadConfig(TIM1, ENABLE); //使能TIMx在ARR上的预装载寄存器
      
                    TIM_CtrlPWMOutputs(TIM1, ENABLE);   //#######不同于普通定时器#######/
                    break;
      
      case PWM_CAP ://8~11
                    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
                    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_11; //TIM1_CH1
                    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //下拉输入
                    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
                    GPIO_Init(GPIOA, &GPIO_InitStructure);   
                    
                    TIM_ICInitStructure.TIM_Channel = TIM_Channel_1|TIM_Channel_4;             //选择通道1
                    TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;  //上升沿触发还是下降沿触发的设置
                    TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //映射到TT1上
                    TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;        //配置输入分频 不分频
                    TIM_ICInitStructure.TIM_ICFilter = 0x00;                        //滤波的问题 先不管
                    TIM_ICInit(TIM1, &TIM_ICInitStructure);
                    
                    TIM_SelectInputTrigger(TIM1, TIM_TS_TI2FP2);                //选择IC2为始终触发源
                    TIM_SelectSlaveMode(TIM1, TIM_SlaveMode_Reset);
                     //TIM从模式：触发信号的上升沿重新初始化计数器和触发寄存器的更新事件
                    TIM_SelectMasterSlaveMode(TIM1, TIM_MasterSlaveMode_Enable); //启动定时器的被动触发
                    
                    //中断设置
                    NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn;  //TIM1发生中断 其实就是上面发生你设置上升沿和下降沿触发事件
                    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;  //先占中断等级
                    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;         //从中断等级
                    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //使能中断通道ü
                    NVIC_Init(&NVIC_InitStructure);  //初始化
                    NVIC_InitStructure.NVIC_IRQChannel = TIM1_CC_IRQn;
                    NVIC_Init(&NVIC_InitStructure);  //初始化

                    TIM_ITConfig(TIM1,TIM_IT_Update|TIM_IT_CC1|TIM_IT_CC4,ENABLE);//使能通道 1 4 中断
                    TIM_CCxCmd(TIM1, TIM_Channel_1,TIM_CCx_Enable);//使能或失能TIM捕获比较通道x
                    break;
      default : break;
    }
    
    TIM_Cmd(TIM1, ENABLE);//使能或者失能TIMx外设
    
  }
  
  /*********TIM2**********/
  if((My_TIMx&Tim2)==Tim2)
  {
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);//使能或者失能APB1外设时钟
    TIM_TimeBaseStructure.TIM_Prescaler = psc-1;//预分频值
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;//向上计数模式
    TIM_TimeBaseStructure.TIM_Period = arr;//在下一个更新事件装入活动的自动重装载寄存器周期的值
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;//TDTS = Tck_tim
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;//重复计数值
    TIM_TimeBaseInit(TIM2,&TIM_TimeBaseStructure);//初始化

    switch(function)
    {
      case COMMON  : 
                    TIM_ITConfig(TIM2, TIM_IT_Update,ENABLE);//使能或者失能指定的TIM中断
      
                    NVIC_InitStructure.NVIC_IRQChannel=TIM2_IRQn;
                    NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
                    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;
                    NVIC_InitStructure.NVIC_IRQChannelSubPriority=1;
                    NVIC_Init(&NVIC_InitStructure);
                    break;
      
      case FRE_CAP : break;
      
      case PWM_OUT :
                    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO,ENABLE);//使能或者失能APB2外设时钟
                    
                    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3; //TIM2_CH2
                    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出
                    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
                    GPIO_Init(GPIOA, &GPIO_InitStructure);
      
                    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //选择定时器模式:TIM脉冲宽度调制模式2
                    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
                    TIM_OCInitStructure.TIM_Pulse = 0; //设置待装入捕获比较寄存器的脉冲值
                    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //输出极性:TIM输出比较极性高
                    TIM_OC4Init(TIM2, &TIM_OCInitStructure);  //根据TIM_OCInitStruct中指定的参数初始化外设TIMx
                    TIM_OC4PreloadConfig(TIM2, TIM_OCPreload_Enable);  //使能TIMx在CCR2上的预装载寄存器
                    
                    TIM_ARRPreloadConfig(TIM2, ENABLE); //使能TIMx在ARR上的预装载寄存器
                    break;
      
      case PWM_CAP : break;
      default : break;
    }
    
    TIM_Cmd(TIM2, ENABLE);//使能或者失能TIMx外设
  }
  
  /*********TIM3**********/
  if((My_TIMx&Tim3)==Tim3)
  {
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
    
    TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	 80K
    TIM_TimeBaseStructure.TIM_Prescaler =psc-1; //设置用来作为TIMx时钟频率除数的预分频值  不分频
    TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
    
    switch(function)
    {
      case COMMON  : 
                    TIM_ITConfig(TIM3, TIM_IT_Update,ENABLE);//使能或者失能指定的TIM中断
      
                    NVIC_InitStructure.NVIC_IRQChannel=TIM3_IRQn;
                    NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
                    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;
                    NVIC_InitStructure.NVIC_IRQChannelSubPriority=0;
                    NVIC_Init(&NVIC_InitStructure);
                    break;
      
      case FRE_CAP : break;
      
      case PWM_OUT :
                    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO,ENABLE);//使能或者失能APB2外设时钟
                    
                    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7; //TIM3_CH1 CH2
                    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出
                    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
                    GPIO_Init(GPIOA, &GPIO_InitStructure);
                    
                    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 |GPIO_Pin_1; //TIM3_CH3 CH4
                    GPIO_Init(GPIOB, &GPIO_InitStructure);
      
                    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //选择定时器模式:TIM脉冲宽度调制模式2
                    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
                    TIM_OCInitStructure.TIM_Pulse = 0; //设置待装入捕获比较寄存器的脉冲值
                    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //输出极性:TIM输出比较极性高
                    
                    TIM_OC1Init(TIM3, &TIM_OCInitStructure);  //根据TIM_OCInitStruct中指定的参数初始化外设TIMx
                    TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);  //使能TIMx在CCR2上的预装载寄存器
                    
                    TIM_OC2Init(TIM3, &TIM_OCInitStructure);  //根据TIM_OCInitStruct中指定的参数初始化外设TIMx
                    TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);  //使能TIMx在CCR2上的预装载寄存器
                    
                    TIM_OC3Init(TIM3, &TIM_OCInitStructure);  //根据TIM_OCInitStruct中指定的参数初始化外设TIMx
                    TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);  //使能TIMx在CCR2上的预装载寄存器
                    
                    TIM_OC4Init(TIM3, &TIM_OCInitStructure);  //根据TIM_OCInitStruct中指定的参数初始化外设TIMx
                    TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);  //使能TIMx在CCR2上的预装载寄存器
                    
                    TIM_ARRPreloadConfig(TIM3, ENABLE); //使能TIMx在ARR上的预装载寄存器
                    break;
      
      case PWM_CAP : break;
      default : break;
    }
   
    TIM_Cmd(TIM3, ENABLE);  //使能TIMx外设
  }
  
  /*********TIM4**********/
  if((My_TIMx&Tim4)==Tim4)
  {
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);//使能或者失能APB1外设时钟
    TIM_TimeBaseStructure.TIM_Prescaler = psc-1;//预分频值
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;//向上计数模式
    TIM_TimeBaseStructure.TIM_Period = arr;//在下一个更新事件装入活动的自动重装载寄存器周期的值
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;//TDTS = Tck_tim
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;//重复计数值
    TIM_TimeBaseInit(TIM4,&TIM_TimeBaseStructure);//初始化
    
    switch(function)
    {
      case COMMON  : 
                    TIM_ITConfig(TIM4, TIM_IT_Update,ENABLE);//使能或者失能指定的TIM中断
    
                    NVIC_InitStructure.NVIC_IRQChannel=TIM4_IRQn;
                    NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
                    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;
                    NVIC_InitStructure.NVIC_IRQChannelSubPriority=3;
                    NVIC_Init(&NVIC_InitStructure);
                    break;
      
      case FRE_CAP : break;
      
      case PWM_OUT :
                    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD | RCC_APB2Periph_AFIO,ENABLE);//使能或者失能APB2外设时钟
                    
                    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15; //TIM4_CH1~4
                    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出
                    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
                    GPIO_Init(GPIOD, &GPIO_InitStructure);
                    
                    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //选择定时器模式:TIM脉冲宽度调制模式2
                    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
                    TIM_OCInitStructure.TIM_Pulse = 0; //设置待装入捕获比较寄存器的脉冲值
                    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //输出极性:TIM输出比较极性高c
      
                    TIM_OC1Init(TIM4, &TIM_OCInitStructure);  //根据TIM_OCInitStruct中指定的参数初始化外设TIMx
                    TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);  //使能TIMx在CCR2上的预装载寄存器
      
                    TIM_OC2Init(TIM4, &TIM_OCInitStructure);  //根据TIM_OCInitStruct中指定的参数初始化外设TIMx
                    TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);  //使能TIMx在CCR2上的预装载寄存器
      
                    TIM_OC3Init(TIM4, &TIM_OCInitStructure);  //根据TIM_OCInitStruct中指定的参数初始化外设TIMx
                    TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);  //使能TIMx在CCR2上的预装载寄存器
      
                    TIM_OC4Init(TIM4, &TIM_OCInitStructure);  //根据TIM_OCInitStruct中指定的参数初始化外设TIMx
                    TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);  //使能TIMx在CCR2上的预装载寄存器
                    
                    TIM_ARRPreloadConfig(TIM4, ENABLE); //使能TIMx在ARR上的预装载寄存器
                    break;
      
      case PWM_CAP : break;
      default : break;
    }
    
    TIM_Cmd(TIM4, ENABLE);//使能或者失能TIMx外设
  }
  
}
/***********TIM1中断服务函数***********/
uint16_t count_LED=0;
void TIM1_UP_IRQHandler(void)
{
  if(TIM_GetITStatus(TIM1, TIM_IT_Update) != RESET)
  {  
    systim_ms++;
    count_LED++;
    if(count_LED ==1000)
    {
        count_LED=0;
        SYSTEM_LED_FAN();//定时1S ，闪烁LED 
    }
    FixtureRun();
    TIM_ClearITPendingBit(TIM1, TIM_IT_Update);//清除TIMx的中断待处理位
  }
}

void TIM1_CC_IRQHandler(void)
{
  if(TIM_GetITStatus(TIM1, TIM_IT_CC1) != RESET) //CH1通道（PA8）发生触发事件
  { 
    
    TIM_ClearITPendingBit(TIM1, TIM_IT_CC1);       //清除TIM的中断待处理位
  }
}

/***********TIM2中断服务函数***********/
void TIM2_IRQHandler(void)
{
  if(TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
  {
    
    TIM_ClearITPendingBit(TIM2, TIM_IT_Update);//清除TIMx的中断待处理位
  }
  
}

/***********TIM3中断服务函数***********/
void TIM3_IRQHandler(void)
{
  if(TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
  {
    TIM_ClearITPendingBit(TIM3, TIM_IT_Update);//清除TIMx的中断待处理位
  }
}
/***********TIM4中断服务函数***********/
void TIM4_IRQHandler(void)
{
  if(TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)
  {
    TIM_ClearITPendingBit(TIM4, TIM_IT_Update);//清除TIMx的中断待处理位
  }
}


#ifndef __IODEF_H__
#define __IODEF_H__

#include "sys.h"


#define ON      1
#define OFF     0


typedef union
{
    struct
    {
        unsigned Q0:1;
        unsigned Q1:1;
        unsigned Q2:1;
        unsigned Q3:1;
        unsigned Q4:1;
        unsigned Q5:1;
        unsigned Q6:1;
        unsigned Q7:1;
    } Bits;
    uint8_t Value;
    
} _flag;


#define     Flag_RESET_OK           flag.Bits.Q0
#define     Flag_BOX_OPEN           flag.Bits.Q1
#define     Flag_BOX_CLOSE          flag.Bits.Q2
#define     rx_sof                  flag.Bits.Q3
#define     rx_start                flag.Bits.Q4
#define     bDispUpd                flag.Bits.Q5
#define     Flg_1KHZ                flag.Bits.Q6




/********************************输出IO定义*********************************/

typedef enum
{
    __100R,
    __10K,
    __1M
}ENUM_GEAR;

extern ENUM_GEAR gear ;//档位


typedef enum
{
    TMUX1,
    TMUX1_1,
    TMUX1_2,
    TMUX2,
    TMUX2_1,
    TMUX2_2
}TMUX1208_NUMBER;



#define ADG704_A0 PEout(9)
#define ADG704_A1 PEout(8)

#define TMUX1_A0 PCout(1)
#define TMUX1_A1 PCout(9)
#define TMUX1_A2 PCout(2)
#define TMUX1_A0_1 PGout(8)
#define TMUX1_A1_1 PCout(6)
#define TMUX1_A2_1 PCout(7)
#define TMUX1_A0_2 PBout(9)
#define TMUX1_A1_2 PBout(8)
#define TMUX1_A2_2 PAout(0)

#define TMUX2_A0 PCout(0)
#define TMUX2_A1 PAout(8)
#define TMUX2_A2 PCout(8)
#define TMUX2_A0_1 PEout(7)
#define TMUX2_A1_1 PBout(5)
#define TMUX2_A2_1 PBout(4)
#define TMUX2_A0_2 PBout(3)
#define TMUX2_A1_2 PDout(7)
#define TMUX2_A2_2 PDout(6)
/*全部0，就是D-S8*/
#define TestIO_ON() do {\
    ADG704_A0=1;\
    ADG704_A1=1;\
    TMUX1_A0=1;\
    TMUX1_A1=1;\
    TMUX1_A2=1;\
    TMUX1_A0_1=1;\
    TMUX1_A1_1=1;\
    TMUX1_A2_1=1;\
    TMUX1_A0_2=1;\
    TMUX1_A1_2=1;\
    TMUX1_A2_2=1;\
    TMUX2_A0=1;\
    TMUX2_A1=1;\
    TMUX2_A2=1;\
    TMUX2_A0_1=1;\
    TMUX2_A1_1=1;\
    TMUX2_A2_1=1;\
    TMUX2_A0_2=1;\
    TMUX2_A1_2=1;\
    TMUX2_A2_2 =1;\
}while(0)


/*全部0，就是D-S1*/
#define TestIO_OFF() do {\
    ADG704_A0=0;\
    ADG704_A1=0;\
    TMUX1_A0=0;\
    TMUX1_A1=0;\
    TMUX1_A2=0;\
    TMUX1_A0_1=0;\
    TMUX1_A1_1=0;\
    TMUX1_A2_1=0;\
    TMUX1_A0_2=0;\
    TMUX1_A1_2=0;\
    TMUX1_A2_2=0;\
    TMUX2_A0=0;\
    TMUX2_A1=0;\
    TMUX2_A2=0;\
    TMUX2_A0_1=0;\
    TMUX2_A1_1=0;\
    TMUX2_A2_1=0;\
    TMUX2_A0_2=0;\
    TMUX2_A1_2=0;\
    TMUX2_A2_2 =0;\
}while(0)
#define D_OUT1 PEout(5)
#define D_OUT2 PEout(4)
#define D_OUT3 PEout(3)
#define D_OUT4 PEout(2)
#define D_OUT5 PDout(0)
#define D_OUT6 PDout(1)
#define D_OUT7 PDout(2)
#define D_OUT8 PDout(3)
#define D_OUT9 PCout(12)
#define D_OUT10 PCout(11)
#define D_OUT11 PCout(10)
#define D_OUT12 PAout(15)

#define PWM1 PAout(6)
#define PWM2 PAout(7)
#define PWM3 PBout(0)
#define PWM4 PBout(1)
#define PWM5 PDout(12)
#define PWM6 PDout(13)
#define PWM7 PDout(14)
#define PWM8 PDout(15)

#define _5V_EN  PEout(11) //控制板上5V电源的控制脚，控制NMOS管

#define EX_VTEN PCout(5)

/**********************************************************************************************/


/************************************输入IO定义************************************************/
//

//#define  LDR_Sensor  HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_4)
//#define  IN1  HAL_GPIO_ReadPin(GPIOF, GPIO_PIN_10)
//#define  IN2  HAL_GPIO_ReadPin(GPIOF, GPIO_PIN_9)
//#define  IN3  HAL_GPIO_ReadPin(GPIOF, GPIO_PIN_8)
//#define  IN4  HAL_GPIO_ReadPin(GPIOF, GPIO_PIN_7)
//#define  IN5  HAL_GPIO_ReadPin(GPIOF, GPIO_PIN_6)
//#define  IN6  HAL_GPIO_ReadPin(GPIOF, GPIO_PIN_5)
//#define  IN7  HAL_GPIO_ReadPin(GPIOF, GPIO_PIN_4)
//#define  IN8  HAL_GPIO_ReadPin(GPIOF, GPIO_PIN_3)
//#define  IN9  HAL_GPIO_ReadPin(GPIOF, GPIO_PIN_2)
//#define  IN10  HAL_GPIO_ReadPin(GPIOF, GPIO_PIN_1)
//#define  IN11  HAL_GPIO_ReadPin(GPIOF, GPIO_PIN_0)
//#define  IN12  HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_6)
#define  LDR_Sensor  PCin(4)

#define  IN1  PFin(10)
#define  IN2  PFin(9)
#define  IN3  PFin(8)
#define  IN4  PFin(7)
#define  IN5  PFin(6)
#define  IN6  PFin(5)
#define  IN7  PFin(4)
#define  IN8  PFin(3)
#define  IN9  PFin(2)
#define  IN10  PFin(1)
#define  IN11  PFin(0)
#define  IN12  PEin(6)


#define Fixture_OUT_Sensor()        IN1
#define Fixture_IN_Sensor()         IN2
#define Fixture_UP_Sensor()         IN3
#define Fixture_DOWN_Sensor()       IN4
#define Start_Btn()                 IN5
#define Stop_Btn()                  IN6
#define safe()                      IN7


//系统呼吸灯，每隔一秒亮灭一次
//PB12
#define SYSTEM_LED_On()         {PAout(11)=1;}
#define SYSTEM_LED_Off()        {PAout(11)=0;}
#define SYSTEM_LED_FAN()        {PAout(11)=!PAout(11);}

#define OUT1_ON()       {PEout(5)=1;} 
#define OUT1_OFF()      {PEout(5)=0;} 
#define OUT2_ON()       {PEout(4)=1;} 
#define OUT2_OFF()      {PEout(4)=0;} 
#define OUT3_ON()       {PEout(3)=1;} 
#define OUT3_OFF()      {PEout(3)=0;} 
#define OUT4_ON()       {PEout(2)=1;} 
#define OUT4_OFF()      {PEout(2)=0;} 
#define OUT5_ON()       {PDout(0)=1;} 
#define OUT5_OFF()      {PDout(0)=0;} 
#define OUT6_ON()       {PDout(1)=1;} 
#define OUT6_OFF()      {PDout(1)=0;} 
#define OUT7_ON()       {PDout(2)=1;} 
#define OUT7_OFF()      {PDout(2)=0;} 
#define OUT8_ON()       {PDout(3)=1;} 
#define OUT8_OFF()      {PDout(3)=0;} 
#define OUT9_ON()       {PCout(12)=1;} 
#define OUT9_OFF()      {PCout(12)=0;} 
#define OUT10_ON()       {PCout(11)=1;} 
#define OUT10_OFF()      {PCout(11)=0;} 
#define OUT11_ON()       {PCout(10)=1;} 
#define OUT11_OFF()      {PCout(10)=0;} 
#define OUT12_ON()       {PAout(15)=1;} 
#define OUT12_OFF()      {PAout(15)=0;} 


#define Fixture_IN()        {OUT1_OFF();    OUT2_ON();}
#define Fixture_OUT()       {OUT1_ON();     OUT2_OFF();}
#define Fixture_DOWN()      {OUT3_OFF();    OUT4_ON();}
#define Fixture_UP()        {OUT3_ON();     OUT4_OFF();}

#define _5V_ON()            {_5V_EN=1;}
#define _5V_OFF()           {_5V_EN=0;}

#define BTN_PRESS()         {OUT5_ON();}
#define BTN_RELEASE()       {OUT5_OFF();}

#define ADSORD_ON()         {OUT6_ON();}
#define ADSORD_OFF()        {OUT6_OFF();}

//控制LED驱动电源点亮
#define Relay_WIN_LED_ON()      {D_OUT7=1;}
#define Relay_WIN_LED_OFF()     {D_OUT7=0;}
//控制LED驱动芯片，供电，使能
#define Relay_VIN_EN_ON()       {D_OUT8=1;}
#define Relay_VIN_EN_OFF()      {D_OUT8=0;}
//控制 开短路测试点与控制板隔开
#define Relay_ISOLATION_ON()       {D_OUT9=1;D_OUT10=1;D_OUT11=1;}
#define Relay_ISOLATION_OFF()      {D_OUT9=0;D_OUT10=0;D_OUT11=0;}
/*********************************************************************************************/

#endif



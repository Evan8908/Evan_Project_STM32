#include "sys.h"
#include "delay.h"
#include "led.h"
#include "exti.h"
#include "stdlib.h"
#include "mystr.h"
#include "mymath.h"
#include "app.h"
#include "..\FIXTURE\IO_def.h"
#include "Flash_Save.H"
void All_init(void);


char outputBuffer[50];

int main(void)
{
    All_init();
    Fixture_UP();
    while(Fixture_UP_Sensor()==1)
    {
        
    }
    Fixture_OUT();
    
//    while(1)
//    {
//        BTN_PRESS();
//        delay_ms(500);
//        BTN_RELEASE()
//        delay_ms(500);
//    }
//    
    
    Flash_ReadStruct(StartPageAddress, &Cal);
    sprintf(outputBuffer, "LED1:%d\nLED1_cal:%d\nLED2:%d \nLED2_cal:%d\nLED3:%d \nLED3_cal:%d\nLED4:%d \nLED4_cal:%d\n\r\n", 
    Cal.LED1,Cal.LED1_Cal,Cal.LED2, Cal.LED2_Cal, Cal.LED3,Cal.LED3_Cal,Cal.LED4,Cal.LED4_Cal);
    USART_printf(Usart1,outputBuffer);
    USART_printf(Usart1,"System init is OK\r\n");
    while(1)
    {
        select_gear(ENABLE,1,1);//选择测试档位，100欧，10K，1M
        run(2);
        deal_rx_buff(10);      //处理串口数据
        Print_Lux_Sensor();
//        TestIO_ON();
//        delay_ms(3000);
//        TestIO_OFF();
//        delay_ms(3000);
//        select_channel(1,3);
        
    }
}

void All_init(void)
{
    SystemInit();
    delay_init(72);	     //延时初始化
    NVIC_Configuration();
    //    LED_Init();
    Timer_init(COMMON,Tim1,7200,10);//1ms
    Usart_init(Usart1 | Usart2 | Usart3,115200);//USART1-232-PC  USART2-232-串口屏  USART3-485-保留
    Adc_Init();
    AT24CXX_Init();
    delay_ms(10);
//    GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3, DISABLE);       // 使能TIM3重映射
    GPIO_PinRemapConfig(GPIO_Remap_TIM4, ENABLE);       // 使能TIM3重映射
    Timer_init(PWM_OUT,Tim3,1440,50000/pwm_freq[0]);//周期20us，频率50k  最大 (1440,50000) 1HZ
    Timer_init(PWM_OUT,Tim4,1440,50000/pwm_freq[1]);//周期20us，频率50k  最大 (1440,50000) 1HZ
    const char *str2 = "$Set PWM Duty 1:30,31";
    strcpy(rx1_buffer, str2);
    com1_protocol_5();
    //读取保存的数据
    {
        uint8_t str_temp[10] = "";
//        AT24CXX_Write(0,str_temp,10);
        AT24CXX_Read(0,str_temp,10);//读取编码器1数据
        //没有信息读出
//        USART_printf(Usart1,"eep:%s\n",str_temp);
    }
    
    
    
    {
//        u8 str_temp[10] = "";
//        u32 num_temp = 0;
//        Clear_String(str_temp,strlen(str_temp));
//        AT24CXX_Read(ENCODER_1_EEAD,str_temp,10);//读取编码器1数据
//        num_temp = str_temp[4];num_temp <<= 8;
//        num_temp |= str_temp[3];num_temp <<= 8;
//        num_temp |= str_temp[2];num_temp <<= 8;
//        num_temp |= str_temp[1];
//        pul_cnt[0] = num_temp;
//        if(str_temp[0])pul_cnt[0] = -pul_cnt[0];
//    
//        num_temp = str_temp[9];num_temp <<= 8;
//        num_temp |= str_temp[8];num_temp <<= 8;
//        num_temp |= str_temp[7];num_temp <<= 8;
//        num_temp |= str_temp[6];
//        nboftns[0] = num_temp;
//        if(str_temp[5])nboftns[0] = -nboftns[0];
//    
//        Clear_String(str_temp,strlen(str_temp));
//        AT24CXX_Read(ENCODER_2_EEAD,str_temp,10);//读取编码器2数据
//        num_temp = str_temp[4];num_temp <<= 8;
//        num_temp |= str_temp[3];num_temp <<= 8;
//        num_temp |= str_temp[2];num_temp <<= 8;
//        num_temp |= str_temp[1];
//        pul_cnt[1] = num_temp;
//        if(str_temp[0])pul_cnt[1] = -pul_cnt[1];
//        num_temp = str_temp[9];num_temp <<= 8;
//        num_temp |= str_temp[8];num_temp <<= 8;
//        num_temp |= str_temp[7];num_temp <<= 8;
//        num_temp |= str_temp[6];
//        nboftns[1] = num_temp;
//        if(str_temp[5])nboftns[1] = -nboftns[1];

//        Clear_String(str_temp,strlen(str_temp));
//        AT24CXX_Read(RUN_PARA_EEAD1,str_temp,8);//读取运行参数1
//        mode = str_temp[0];
//        t_f = str_temp[2];t_f <<= 8;
//        t_f |= str_temp[1];
//        out_sta = str_temp[3];
//        ful_cicl_pul[0] = str_temp[5];ful_cicl_pul[0] <<= 8;
//        ful_cicl_pul[0] |= str_temp[4];
//        if(ful_cicl_pul[0]==0||ful_cicl_pul[0]>65000)ful_cicl_pul[0] = 1000;
//    
//        ful_cicl_pul[1] = str_temp[7];ful_cicl_pul[1] <<= 8;
//        ful_cicl_pul[1] |= str_temp[6];
//        if(ful_cicl_pul[1]==0||ful_cicl_pul[1]>65000)ful_cicl_pul[1] = 1000;
//    
//        if(t_f==0)t_f = 1;
//        else if(t_f>2000)t_f = 1;
//    
//        Clear_String(str_temp,strlen(str_temp));
//        AT24CXX_Read(RUN_PARA_EEAD2,str_temp,4);//读取运行参数2
//        jsb[0] = str_temp[1];jsb[0] <<= 8;
//        jsb[0] |= str_temp[0];
//        if(jsb[0]==0||jsb[0]>9999)jsb[0] = 1;
//        jsb[1] = str_temp[3];jsb[1] <<= 8;
//        jsb[1] |= str_temp[2];
//        if(jsb[1]==0||jsb[1]>9999)jsb[1] = 1;
//    
//    
//        num_temp = (u8)(mode&(1<<0)) + (u8)(mode&(1<<1)) + out_sta + t_f + ful_cicl_pul[0] + ful_cicl_pul[1] + jsb[0] + jsb[1] ;
//        USART1_printf("#cfg:%d,%d,%d,%d,%d,%d,%d,%d,%d\n",\
//                                                        (u8)(mode&(1<<0)),\
//                                                        (u8)(mode&(1<<1)),\
//                                                        out_sta,\
//                                                        t_f,ful_cicl_pul[0],\
//                                                        ful_cicl_pul[1],\
//                                                        jsb[0],\
//                                                        jsb[1],\
//                                                        num_temp);
                                            
//    USART1_printf("\n#data:%d,%d,%d,%d\r\n",pul_cnt[0],nboftns[0],pul_cnt[1],nboftns[1]);
    }
    switch(app_init())
    {
        case 0 : 
                {
                    //系统开启后打印的
                    USART_printf(Usart1,"%s\nReady!\n",Version);
                    USART_printf(Usart2,"%s\nReady!\n",Version);
                    USART_printf(Usart3,"%s\nReady!\n",Version);
                }
                break;
        case 1 : 
                {
                    while(1)
                    {
                        USART_printf(Usart1,"%s\nEEPROM Error!\n",Version);
                        USART_printf(Usart2,"%s\nEEPROM Error!\n",Version);
                        USART_printf(Usart3,"%s\nEEPROM Error!\n",Version);
                        delay_ms(1000);
                        if(app_init() != 1)break;
                    }
                }
                break;
        default : break;
        
        
    }
    delay_ms(10);
}

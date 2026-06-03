#include "Fixture_Run.h"
#include "app.h"

//动作溢出时间
#define OverTime 10000
volatile uint8_t DUT1_STATE;
uint16_t delayCnt;
uint16_t delayCnt2;
uint16_t delayCnt3;
uint16_t delayCnt4;
uint8_t DUT1_PASS_FAIL;
uint8_t Flg_Stop;

_flag flag;


void Fixture_Reset()
{
//    STOP_WLED();    //控制LED灭
}

//===============================治具动作流程==================================================
void FixtureRun(void)
{
//    if(LDR_Sensor==0)
//    {
//        ADSORD_OFF();
//    }else if(Fixture_DOWN_Sensor()!=0)
//    {
//        ADSORD_ON();
//    }
    
    if (!safe()) //安全光栅 感应到
    {
        if (DUT1_STATE == state1) //下压过程中
        {
            DUT1_STATE = state4;
        }
    }

    if (!Stop_Btn()) //急停
    {
        delayCnt4 = 0;
        if (++delayCnt3>=3)
        {
            if (Flg_Stop==0)
            {
                Flg_Stop = 1;
                DUT1_STATE = state4;
            }
        }
    }
    else
    {
        delayCnt3 = 0;

        if (++delayCnt4>=3)
        {
            Flg_Stop = 0;
            delayCnt4 =0;
        }
    }

    switch (DUT1_STATE)
    {
        case state0:    //启动
            if (Flg_Stop==0) //急停标志位：1 按下；0 释放
            {
                if (!Start_Btn() || Flag_BOX_CLOSE) //
                {
                    if (++delayCnt>=3)
                    {
                        delayCnt = 0;
                        delayCnt2 = 0;
                        Fixture_IN();
                        Flag_RESET_OK = 0;  //复位指令标志清零
                        Flag_BOX_OPEN = 0;
                        Flag_BOX_CLOSE = 0;
                        DUT1_STATE = state1;
                    }
                }
                else delayCnt = 0;
            }

            break;

        case state1:    // 检测进到位
            if (!Fixture_IN_Sensor()) //等待进到位
            {
                //检测到进到位
                delayCnt = 0;
                Fixture_DOWN();
                if (++delayCnt2>=3)   //确认3次
                {
                    //检测到进到位信号
                    delayCnt2 = 0;
                    DUT1_STATE = state2;
                }
            }
            else
            {
                //未检测到进到位
                if (++delayCnt>=OverTime)     //
                {
                    //检测超时
                    delayCnt = 0;
                    USART_printf(Usart1,"Chinese：治具进到位感应未触发\r\n"); //报错
                    USART_printf(Usart1,"English：The fixture in-place sensor is not triggered\r\n"); //报错
                    DUT1_STATE = state4;    //转到治具复位状态
                }

                delayCnt2 = 0;
            }

            break;

        case state2:    //检测下到位
            if (!Fixture_DOWN_Sensor()) //等待下到位
            {
                //检测到进到位
                delayCnt = 0;
                if (++delayCnt2>=3)   //确认3次
                {
                    //检测到进到位信号
                    delayCnt = 0;
                    delayCnt2 = 0;
                    //治具压合到位
                    ADSORD_OFF();
                    USART_printf(Usart1,"BOX_CLOSE_READY\r\n");
                    USART_printf(Usart2,"BOX_CLOSE_READY\r\n");
                    DUT1_STATE = state30;
                }
            }
            else
            {
                //未检测到进到位
                if (++delayCnt>=OverTime)     //
                {
                    //检测超时
                    delayCnt = 0;
                    USART_printf(Usart1,"Chinese：治具下到位感应未触发\r\n"); //报错
                    USART_printf(Usart1,"English：The fixture down-place sensor is not triggered\r\n"); //报错
                    DUT1_STATE = state4;    //转到治具复位状态
                }

                delayCnt2 = 0;
            }

            break;

        /*******************************底部气缸 顶上去********************************************/
        case state9:    //检测下到位
            
            break;

        /***************************************************************************/
        case state30:
            if (Start_Btn())
            {
                if (++delayCnt>=3)
                {
                    delayCnt = 0;
                    DUT1_STATE = state3;
                }
            }
            else delayCnt = 0;

            break;

        case state3:    // 等待上位机发复位指令
            if (Flag_RESET_OK|| Flag_BOX_OPEN || (!Start_Btn()))
            {
//                ADSORD_ON();
                Fixture_Reset();
                DUT1_STATE = state4;
            }

            break;

        //--------------------------------------------------------------------------------------
        case state4:
            Fixture_UP();
            DUT1_STATE = state10;
            break;

        /********************************治具复位*******************************************/
        case state10:
           
            DUT1_STATE = state5;
            Flg_1KHZ = 0;  //关闭speaker
            break;
        /***************************************************************************/
        case state5:    //检测上升到位
            if (!Fixture_UP_Sensor()) //等待上升到位
            {
                //检测到上升到位信号
                delayCnt = 0;
                Flag_BOX_CLOSE =0;
                Fixture_OUT();
                if (++delayCnt2>=3)   //确认3次
                {
                    //
                    delayCnt2 = 0;
//                    OUT_PUSH();//进出气缸 出去
                    DUT1_STATE = state6;
                }
            }
            else
            {
                //未检测到进到位
                if (++delayCnt>=OverTime)     //
                {
                    //检测超时
                    delayCnt = 0;
                    USART_printf(Usart1,"Chinese：治具上到位感应未触发\r\n"); //报错
                    USART_printf(Usart1,"English：The fixture up-place sensor is not triggered\r\n"); //报错
                    DUT1_STATE = state0;
                }

                delayCnt2 = 0;
            }
            break;
        case state6: //检测出到位
            if (!Fixture_OUT_Sensor()) //等待出到位
            {
                //检测到出到位
                delayCnt = 0;

                if (++delayCnt2>=3)      //确认3次
                {
                    //检测到进到位信号
                    delayCnt2 = 0;
                    ADSORD_OFF();
                    DUT1_STATE = state0;
                    _5V_OFF();
                    if (Flag_RESET_OK)
                    {
                        USART_printf(Usart1, "RESET_OK\r\n");
                        USART_printf(Usart2, "RESET_OK\r\n");
                        Flag_RESET_OK = 0;
                    }
                    else
                    {
                        USART_printf(Usart1, "BOX_OPEN_READY\r\n");
                        USART_printf(Usart2, "BOX_OPEN_READY\r\n");
                        Flag_BOX_OPEN = 0;
                    }
                }
            }
            else
            {
                //未检测到进到位
                if (++delayCnt>=OverTime)     //
                {
                    //检测超时
                    delayCnt = 0;
                    USART_printf(Usart1,"Chinese：治具出到位感应未触发\r\n"); //报错
                    USART_printf(Usart1,"English：The fixture out-place sensor is not triggered\r\n"); //报错
                    DUT1_STATE = state0;
                }
                delayCnt2 = 0;
            }
            break;
    }
}





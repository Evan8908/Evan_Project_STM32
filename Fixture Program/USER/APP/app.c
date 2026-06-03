#include "app.h"
#include "..\FIXTURE\Fixture_Run.h"
#include <string.h>
#include "Flash_Save.h"





volatile uint8_t read_lux_flag=0;

uint16_t do_reg = 0x0000;//数字输出控制寄存器
uint16_t di_reg = 0x0000;//数字输入缓存寄存器

//50000 代表50KHZ
uint16_t pwm_freq[2] = {50000, 50000}; //定时器3 4的频率，对应PWM1~PWM4 和 PWM5~PWM8的频率 1~50k Hz
uint16_t pwm_duty[8] = {0, 0, 0, 0, 0, 0, 0, 0}; //PWM1~PWM8的脉宽百分比 0~100%


uint8_t test_point_short_tab[][2] = //需要测量短路的点的数组
{
    {1, 93},   {104, 103}, {7, 51},    {9, 53},    {55, 11},  {57, 13},  {15, 59},   {17, 61},//8
    {63, 19},  {105, 106}, {23, 67},   {25, 69},   {71, 27},  {73, 29},  {31, 75},   {33, 77},//16
    {79, 35},  {81, 37},   {39, 83},   {43, 85},   {87, 44},  {2, 46},   {48, 4},    {50, 6}, //24
    {8, 52},   {10, 54},   {56, 12},   {58, 14},   {16, 60},  {18, 62},  {64, 20},   {66, 22},//32
    {24, 68},  {26, 70},   {72, 28},   {74, 30},   {32, 76},  {34, 78},  {80, 36},   {82, 38},//40
    {40, 84},  {45, 94},   {47, 95},   {65, 97},   {86, 100}, {88, 100}, {111, 112},          //47
};

uint8_t test_point_open_tab[][2] = //需要测量开路的点的数组
{
    {1, 41},  {1, 3},   {3, 5},   {5, 7},   {7, 9},    {9, 11},  {11, 13}, {13, 15},//8
    {15, 17}, {17, 19}, {19, 21}, {21, 23}, {23, 25},  {25, 27}, {27, 29}, {29, 31},//16 
    {31, 33}, {35, 37}, {37, 39}, {39, 43}, {2, 42},   {2, 4},   {4, 6},   {6, 8},  //24
    {8, 10},  {10, 12}, {12, 14}, {14, 16}, {16, 18},  {18, 20}, {20, 22}, {22, 24},//32
    {24, 26}, {26, 28}, {28, 30}, {30, 32}, {32, 34},  {34, 36}, {36, 38}, {38, 40},//40
    {40, 44}, {47, 49}, {63, 65}, {65, 67}, {91, 92},  {91, 96}, {94, 93}, {93, 98},//48
    {104, 103}, {104, 106}, {101, 102},{101, 109},//52
    
};
//uint16_t test_point_adc_temp[] = {0,0};//每个待测电压的缓存 max sizeof(test_point_tab)/sizeof(test_point_tab[0])

uint16_t test_cnt = 0; //当前测试序号

uint16_t test_point_short_tab_max   = sizeof(test_point_short_tab)/sizeof(test_point_short_tab[0]);     //短路最大测试数量
uint16_t test_point_open_tab_max    = sizeof(test_point_open_tab)/sizeof(test_point_open_tab[0]);       //开路最大测试数量

ENUM_GEAR gear = __1M;//档位

uint8_t test_status = 0;//测试状态 b0 0未开始  b1 0

const Com_Prot_Struct com1_lst[] = //串口PC 指令
{
    //自定义指令
    {"$Hello", com1_protocol_0},        //测试或者握手
    {"$Read ADC", com1_protocol_1},     //读取电压
    {"$Read DI %", com1_protocol_2},    //格式化读取数字输入，$Read DI %b\r\n 支持%b %d %x
    {"$Set DO", com1_protocol_3},       //设置数字输出，$Set DO:255\r\n 或者$Set DOx:255\r\n
    {"SET_FREQ", com1_protocol_4},      //设置PWM频率，$Set PWM Freq:f1,f2,sum\r\n  f1-第一组（PWM1~PWM4）频率  f2-第二组（PWM5~PWM8）频率  sum ASCII校验和，f1,f2 取 1~50k
    {"SET_DUTY", com1_protocol_5},      //设置PWM占空比，$Set PWM Duty 3:11,14\r\n 或者 $Set PWM Duty :p1,p2,p3,p4,p5,p6,p7,p8,sum\r\n   p1~p8 占空比1~8   sum ASCII校验和

    //与机器通信指令
    {"BOX_CLOSE", com1_protocol_6},     //关门
    {"BOX_OPEN", com1_protocol_7},      //开门
    {"BOX_STATUS", com1_protocol_8},    //读取门状态
    {"start_short", com1_protocol_9},   //开始测短路
    {"start_open", com1_protocol_10},   //开始测开路
    {"_5V_ON", com1_protocol_11},       //设置5V 2A输出
    {"_5V_OFF", com1_protocol_12},      //设置5V 2A关闭
    {"VINWLED", VINWLED},               //读取J3_VIN_WLED 的电压值，要先把VIN,WLED_EN,上5V使能

    {"START_WLED", START_WLED},         //打开LED
    {"STOP_WLED", STOP_WLED},           //关闭LED
    {"READ_RGB", READ_RGB},             //读取LED RGB值
    {"RESET", RESET_FIXTURE},           //治具复位
    {"SET_PWM30", SET_PWM30},
    {"CLA_RGB", SET_CLA_RGB},
    {"RGBC 1", RGBC_1},
    {"RGBC 2", RGBC_2},
    {"RGBC 3", RGBC_3},
    {"RGBC 4", RGBC_4},
    {"READ_LUX1", READ_LX1},
    {"READ_LUX2", READ_LX2},
    {"READ_LUX3", READ_LX3},
    {"READ_LUX4", READ_LX4},
    {"CAL_LUX888", Cal_lux},
};

uint8_t com1_protocols_max = sizeof(com1_lst)/sizeof(com1_lst[0]);

const Com_Prot_Struct com2_lst[] = //串口触摸屏指令
{
    {"BOX_CLOSE", com1_protocol_6},     //关门
    {"BOX_OPEN", com1_protocol_7},      //开门
    {"BOX_STATUS", com1_protocol_8},    //读取门状态
    {"RESET", RESET_FIXTURE},           //治具复位
    {"start_short", com1_protocol_9},   //开始测短路
    {"start_open", com1_protocol_10},   //开始测开路
};
uint8_t com2_protocols_max = sizeof(com2_lst)/sizeof(com2_lst[0]);

const Com_Prot_Struct com3_lst[] = //RS485  Led指令
{
    {"$Hello", com3_protocol_0}, //$Helloend$
    //    {"$Set Charge:",Task_4},//$Set Charge:0
    //    {"$Set Printf:",Task_6},//$Set Printf:3
    //    {"$Set Lowpower:",Task_7},//$Set Lowpower:1
};
uint8_t com3_protocols_max = sizeof(com3_lst)/sizeof(com3_lst[0]);

/********************************************
*函数名： app_init
*功能：应用程序初始化
*参数：无
*返回：错误代码 0-成功   1-24CXX异常   2-
*作者：柯龑
*日期：2025.05.09
********************************************/
uint8_t app_init(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD | \
                           RCC_APB2Periph_GPIOE | RCC_APB2Periph_GPIOF | RCC_APB2Periph_GPIOG, ENABLE);

    //取消JTAG，作为GPIO使用
    {
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
        GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
    }

    //矩阵板通道选择引脚初始化
    {
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;//ADG704_A0 ADG704_A1
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
        GPIO_Init(GPIOE, &GPIO_InitStructure);

        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_9 | GPIO_Pin_2;//TMUX1_A0 TMUX1_A1 TMUX1_A2
        GPIO_Init(GPIOC, &GPIO_InitStructure);

        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;//TMUX1_A0_1
        GPIO_Init(GPIOG, &GPIO_InitStructure);
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;//TMUX1_A1_1 TMUX1_A2_1
        GPIO_Init(GPIOC, &GPIO_InitStructure);

        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_8| GPIO_Pin_12;//TMUX1_A0_2 TMUX1_A1_2  sys_led
        GPIO_Init(GPIOB, &GPIO_InitStructure);
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;//TMUX1_A2_2
        GPIO_Init(GPIOA, &GPIO_InitStructure);

        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_8;//TMUX2_A0 TMUX2_A2
        GPIO_Init(GPIOC, &GPIO_InitStructure);
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;//TMUX2_A1
        GPIO_Init(GPIOA, &GPIO_InitStructure);

        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;//TMUX2_A0_1
        GPIO_Init(GPIOE, &GPIO_InitStructure);
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_4;//TMUX2_A1_1 TMUX2_A2_1
        GPIO_Init(GPIOB, &GPIO_InitStructure);

        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;//TMUX2_A0_2
        GPIO_Init(GPIOB, &GPIO_InitStructure);
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_6;//TMUX2_A1_2 TMUX2_A2_2
        GPIO_Init(GPIOD, &GPIO_InitStructure);

        //        select_channel(test_point_short_tab[0][0],test_point_short_tab[0][1]);//选择测试点1
    }

    //数字输入输出初始化
    {
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5;//DO_4 ~ DO_1
        GPIO_Init(GPIOE, &GPIO_InitStructure);
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 ;//DO_5 ~ DO_8
        GPIO_Init(GPIOD, &GPIO_InitStructure);
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_11 | GPIO_Pin_10 ;//DO_9 ~ DO_11
        GPIO_Init(GPIOC, &GPIO_InitStructure);
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15| GPIO_Pin_11 ;//DO_15
        GPIO_Init(GPIOA, &GPIO_InitStructure);

        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;//_5V_EN
        GPIO_Init(GPIOE, &GPIO_InitStructure);

        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;//EX_VTEN
        GPIO_Init(GPIOC, &GPIO_InitStructure);

        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | \
                                      GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 ;//DI_11 ~ DI1
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//电路带上拉
        GPIO_Init(GPIOF, &GPIO_InitStructure);

        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;//DI_12
        GPIO_Init(GPIOE, &GPIO_InitStructure);

        set_12digit_out(do_reg);
    }

    //    while(AT24CXX_Check())
    //    {
    //        uint8_t x = 10;
    //        if(x == 0)
    //        {
    //            return 1;
    //        }
    //        x--;
    //        delay_ms(100);
    //    }
    return 0;
}


/********************************************
*函数名： select_gear
*功能：选择档位
*参数1：自动切换使能 ENABLE DISABLE
*参数2：调用周期 ms >=1
*参数3：稳定档位切换时间 ms
*返回：无
*日期：2025.05.09
********************************************/
void select_gear(FunctionalState auto_en, uint16_t tim_ms, uint16_t swcg_tim_ms)
{
    static uint16_t swch_gear_cnt = 0;
    static uint64_t ttp = 0;

    if (systim_ms >= ttp + tim_ms)
    {
        ttp = systim_ms;

        switch (gear)
        {
            case __100R :
            {
                ADG704_A1 = 0;
                ADG704_A0 = 0;

                if (auto_en == ENABLE)
                {
                    if (adc_val[0] >= 3755) //电压高于11/12  测出来电阻大于1.1k
                    {
                        if (++swch_gear_cnt >= swcg_tim_ms/tim_ms) //持续时间达到1ms
                        {
                            gear = __10K;//切换到10k档位
                            //                                        USART_printf(Usart1,"# __100R To __10K %d,%d\r\n",adc_val[0],swch_gear_cnt);
                            swch_gear_cnt = 0;//清空计数
                        }
                    }
                    else swch_gear_cnt = 0;//清空计数  采样电阻100R，测出电阻在0~1k
                }
            }
            break;

            case __10K :
            {
                ADG704_A1 = 0;
                ADG704_A0 = 1;

                if (auto_en == ENABLE)
                {
                    if (adc_val[0] >= 3755) //电压高于11/12  测出来电阻大于110k
                    {
                        if (++swch_gear_cnt >= swcg_tim_ms/tim_ms) //持续时间达到200ms
                        {
                            gear = __1M;//切换到1M档位
                            //                                        USART_printf(Usart1,"# __10K To __1M %d,%d\r\n",adc_val[0],swch_gear_cnt);
                            swch_gear_cnt = 0;//清空计数
                        }
                    }
                    else if (adc_val[0] < 338) //电压低于9/109  测出来电阻小于0.9k
                    {
                        if (++swch_gear_cnt >= swcg_tim_ms/tim_ms) //持续时间达到200ms
                        {
                            gear = __100R;//切换到100R档位
                            //                                        USART_printf(Usart1,"# __10K To __100R %d,%d\r\n",adc_val[0],swch_gear_cnt);
                            swch_gear_cnt = 0;//清空计数
                        }
                    }
                    else swch_gear_cnt = 0;//清空计数  采样电阻10k，测出电阻在1~100k
                }
            }
            break;

            case __1M :
            {
                ADG704_A1 = 1;
                ADG704_A0 = 0;

                if (auto_en == ENABLE)
                {
                    if (adc_val[0] < 338) //电压低于9/109   测出来电阻小于90k
                    {
                        if (++swch_gear_cnt >= swcg_tim_ms/tim_ms) //持续时间达到200ms
                        {
                            gear = __10K;//切换到10k档位
                            //                                        USART_printf(Usart1,"# __1M To __10K %d,%d\r\n",adc_val[0],swch_gear_cnt);
                            swch_gear_cnt = 0;//清空计数
                        }
                    }
                    else swch_gear_cnt = 0;//清空计数  采样电阻1M，测出电阻在100k~...
                }
            }
            break;

            default :
                break;
        }
    }

}



/********************************************
*函数名： set_tmux1208_add
*功能：设置模拟开关的地址，电路未知，暂时考虑不相干IO
*参数1：TMUX1208编号  TMUX1208_NUMBER 定义
*参数2：设置的值 1~8
*返回：无
*作者：柯龑
*日期：2025.05.09
********************************************/
void set_tmux1208_add(TMUX1208_NUMBER number, uint8_t val)
{
    switch (number)
    {
        case TMUX1 :
        {
            switch (val)
            {
                case 0 :
                    TMUX1_A2 = 0;
                    TMUX1_A1 = 0;
                    TMUX1_A0 = 0;
                    break;

                case 1 :
                    TMUX1_A2 = 0;
                    TMUX1_A1 = 0;
                    TMUX1_A0 = 1;
                    break;

                case 2 :
                    TMUX1_A2 = 0;
                    TMUX1_A1 = 1;
                    TMUX1_A0 = 0;
                    break;

                case 3 :
                    TMUX1_A2 = 0;
                    TMUX1_A1 = 1;
                    TMUX1_A0 = 1;
                    break;

                case 4 :
                    TMUX1_A2 = 1;
                    TMUX1_A1 = 0;
                    TMUX1_A0 = 0;
                    break;

                case 5 :
                    TMUX1_A2 = 1;
                    TMUX1_A1 = 0;
                    TMUX1_A0 = 1;
                    break;

                case 6 :
                    TMUX1_A2 = 1;
                    TMUX1_A1 = 1;
                    TMUX1_A0 = 0;
                    break;

                case 7 :
                    TMUX1_A2 = 1;
                    TMUX1_A1 = 1;
                    TMUX1_A0 = 1;
                    break;
            }
        }
        break;

        case TMUX1_1 :
        {
            switch (val)
            {
                case 0 :
                    TMUX1_A2_1 = 0;
                    TMUX1_A1_1 = 0;
                    TMUX1_A0_1 = 0;
                    break;

                case 1 :
                    TMUX1_A2_1 = 0;
                    TMUX1_A1_1 = 0;
                    TMUX1_A0_1 = 1;
                    break;

                case 2 :
                    TMUX1_A2_1 = 0;
                    TMUX1_A1_1 = 1;
                    TMUX1_A0_1 = 0;
                    break;

                case 3 :
                    TMUX1_A2_1 = 0;
                    TMUX1_A1_1 = 1;
                    TMUX1_A0_1 = 1;
                    break;

                case 4 :
                    TMUX1_A2_1 = 1;
                    TMUX1_A1_1 = 0;
                    TMUX1_A0_1 = 0;
                    break;

                case 5 :
                    TMUX1_A2_1 = 1;
                    TMUX1_A1_1 = 0;
                    TMUX1_A0_1 = 1;
                    break;

                case 6 :
                    TMUX1_A2_1 = 1;
                    TMUX1_A1_1 = 1;
                    TMUX1_A0_1 = 0;
                    break;

                case 7 :
                    TMUX1_A2_1 = 1;
                    TMUX1_A1_1 = 1;
                    TMUX1_A0_1 = 1;
                    break;
            }
        }
        break;

        case TMUX1_2 :
        {
            switch (val)
            {
                case 0 :
                    TMUX1_A2_2 = 0;
                    TMUX1_A1_2 = 0;
                    TMUX1_A0_2 = 0;
                    break;

                case 1 :
                    TMUX1_A2_2 = 0;
                    TMUX1_A1_2 = 0;
                    TMUX1_A0_2 = 1;
                    break;

                case 2 :
                    TMUX1_A2_2 = 0;
                    TMUX1_A1_2 = 1;
                    TMUX1_A0_2 = 0;
                    break;

                case 3 :
                    TMUX1_A2_2 = 0;
                    TMUX1_A1_2 = 1;
                    TMUX1_A0_2 = 1;
                    break;

                case 4 :
                    TMUX1_A2_2 = 1;
                    TMUX1_A1_2 = 0;
                    TMUX1_A0_2 = 0;
                    break;

                case 5 :
                    TMUX1_A2_2 = 1;
                    TMUX1_A1_2 = 0;
                    TMUX1_A0_2 = 1;
                    break;

                case 6 :
                    TMUX1_A2_2 = 1;
                    TMUX1_A1_2 = 1;
                    TMUX1_A0_2 = 0;
                    break;

                case 7 :
                    TMUX1_A2_2 = 1;
                    TMUX1_A1_2 = 1;
                    TMUX1_A0_2 = 1;
                    break;
            }
        }
        break;

        case TMUX2 :
        {
            switch (val)
            {
                case 0 :
                    TMUX2_A2 = 0;
                    TMUX2_A1 = 0;
                    TMUX2_A0 = 0;
                    break;

                case 1 :
                    TMUX2_A2 = 0;
                    TMUX2_A1 = 0;
                    TMUX2_A0 = 1;
                    break;

                case 2 :
                    TMUX2_A2 = 0;
                    TMUX2_A1 = 1;
                    TMUX2_A0 = 0;
                    break;

                case 3 :
                    TMUX2_A2 = 0;
                    TMUX2_A1 = 1;
                    TMUX2_A0 = 1;
                    break;

                case 4 :
                    TMUX2_A2 = 1;
                    TMUX2_A1 = 0;
                    TMUX2_A0 = 0;
                    break;

                case 5 :
                    TMUX2_A2 = 1;
                    TMUX2_A1 = 0;
                    TMUX2_A0 = 1;
                    break;

                case 6 :
                    TMUX2_A2 = 1;
                    TMUX2_A1 = 1;
                    TMUX2_A0 = 0;
                    break;

                case 7 :
                    TMUX2_A2 = 1;
                    TMUX2_A1 = 1;
                    TMUX2_A0 = 1;
                    break;
            }
        }
        break;

        case TMUX2_1 :
        {
            switch (val)
            {
                case 0 :
                    TMUX2_A2_1 = 0;
                    TMUX2_A1_1 = 0;
                    TMUX2_A0_1 = 0;
                    break;

                case 1 :
                    TMUX2_A2_1 = 0;
                    TMUX2_A1_1 = 0;
                    TMUX2_A0_1 = 1;
                    break;

                case 2 :
                    TMUX2_A2_1 = 0;
                    TMUX2_A1_1 = 1;
                    TMUX2_A0_1 = 0;
                    break;

                case 3 :
                    TMUX2_A2_1 = 0;
                    TMUX2_A1_1 = 1;
                    TMUX2_A0_1 = 1;
                    break;

                case 4 :
                    TMUX2_A2_1 = 1;
                    TMUX2_A1_1 = 0;
                    TMUX2_A0_1 = 0;
                    break;

                case 5 :
                    TMUX2_A2_1 = 1;
                    TMUX2_A1_1 = 0;
                    TMUX2_A0_1 = 1;
                    break;

                case 6 :
                    TMUX2_A2_1 = 1;
                    TMUX2_A1_1 = 1;
                    TMUX2_A0_1 = 0;
                    break;

                case 7 :
                    TMUX2_A2_1 = 1;
                    TMUX2_A1_1 = 1;
                    TMUX2_A0_1 = 1;
                    break;
            }
        }
        break;

        case TMUX2_2 :
        {
            switch (val)
            {
                case 0 :
                    TMUX2_A2_2 = 0;
                    TMUX2_A1_2 = 0;
                    TMUX2_A0_2 = 0;
                    break;

                case 1 :
                    TMUX2_A2_2 = 0;
                    TMUX2_A1_2 = 0;
                    TMUX2_A0_2 = 1;
                    break;

                case 2 :
                    TMUX2_A2_2 = 0;
                    TMUX2_A1_2 = 1;
                    TMUX2_A0_2 = 0;
                    break;

                case 3 :
                    TMUX2_A2_2 = 0;
                    TMUX2_A1_2 = 1;
                    TMUX2_A0_2 = 1;
                    break;

                case 4 :
                    TMUX2_A2_2 = 1;
                    TMUX2_A1_2 = 0;
                    TMUX2_A0_2 = 0;
                    break;

                case 5 :
                    TMUX2_A2_2 = 1;
                    TMUX2_A1_2 = 0;
                    TMUX2_A0_2 = 1;
                    break;

                case 6 :
                    TMUX2_A2_2 = 1;
                    TMUX2_A1_2 = 1;
                    TMUX2_A0_2 = 0;
                    break;

                case 7 :
                    TMUX2_A2_2 = 1;
                    TMUX2_A1_2 = 1;
                    TMUX2_A0_2 = 1;
                    break;
            }
        }
        break;


    }
}

/********************************************
*函数名： select_channel
*功能：选择通道
*参数1：点1接通的通道 1-192
*参数2：点2接通的通道 1-192
*返回：无
*作者：柯龑
*日期：2025.05.09
********************************************/
uint8_t select_channel(uint16_t ch1, uint16_t ch2)
{
    if (ch1&&ch2 == 0)return 0; //通道错误

    if (ch1 <= 32)
    {
        ch1 -= 1;
        set_tmux1208_add(TMUX1, ch1%8);
        set_tmux1208_add(TMUX1_1, ch1>>3);
    }
    else if (ch1 <= 56)
    {
        ch1 -= 33;
        set_tmux1208_add(TMUX1, ch1%8);
        set_tmux1208_add(TMUX1_2, ch1>>3);
        set_tmux1208_add(TMUX1_1, 4);
    }
    else if (ch1 <= 184)
    {
        ch1 -= 57;
        set_tmux1208_add(TMUX1, ch1%8);
        set_tmux1208_add(TMUX1_2, 4 + (ch1>>3)%4);
        set_tmux1208_add(TMUX1_1, 4 + (ch1>>5)%4);
    }
    else if (ch1 <= 192)
    {
        ch1 -= 185;
        set_tmux1208_add(TMUX1_1, 5 + (ch1>>2));
        set_tmux1208_add(TMUX1_2, ch1%4);
    }
    else return 0;

    if (ch2 <= 32)
    {
        ch2 -= 1;
        set_tmux1208_add(TMUX2, ch2%8);
        set_tmux1208_add(TMUX2_1, ch2>>3);
    }
    else if (ch2 <= 56)
    {
        ch2 -= 33;
        set_tmux1208_add(TMUX2, ch2%8);
        set_tmux1208_add(TMUX2_2, ch2>>3);
        set_tmux1208_add(TMUX2_1, 4);
    }
    else if (ch2 <= 184)
    {
        ch2 -= 57;
        set_tmux1208_add(TMUX2, ch2%8);
        set_tmux1208_add(TMUX2_2, 4 + (ch2>>3)%4);
        set_tmux1208_add(TMUX2_1, 4 + (ch2>>5)%4);
    }
    else if (ch2 <= 192)
    {
        ch2 -= 185;
        set_tmux1208_add(TMUX2_1, 5 + (ch2>>2));
        set_tmux1208_add(TMUX2_2, ch2%4);
    }
    else return 0;

    return 1;
}

/********************************************
*函数名： set_12digit_out
*功能：设置12路数字输出
*参数1：输出的数值
*返回：无
*作者：柯龑
*日期：2025.05.09
********************************************/
void set_12digit_out(uint16_t do_temp)
{
    //上述有问题，使用如下傻瓜式代码
    if (do_temp & (1<<0))D_OUT1 = 1;
    else D_OUT1 = 0;

    if (do_temp & (1<<1))D_OUT2 = 1;
    else D_OUT2 = 0;

    if (do_temp & (1<<2))D_OUT3 = 1;
    else D_OUT3 = 0;

    if (do_temp & (1<<3))D_OUT4 = 1;
    else D_OUT4 = 0;

    if (do_temp & (1<<4))D_OUT5 = 1;
    else D_OUT5 = 0;

    if (do_temp & (1<<5))D_OUT6 = 1;
    else D_OUT6 = 0;

    if (do_temp & (1<<6))D_OUT7 = 1;
    else D_OUT7 = 0;

    if (do_temp & (1<<7))D_OUT8 = 1;
    else D_OUT8 = 0;

    if (do_temp & (1<<8))D_OUT9 = 1;
    else D_OUT9 = 0;

    if (do_temp & (1<<9))D_OUT10 = 1;
    else D_OUT10 = 0;

    if (do_temp & (1<<10))D_OUT11 = 1;
    else D_OUT11 = 0;

    if (do_temp & (1<<11))D_OUT12 = 1;
    else D_OUT12 = 0;



}

/********************************************
*函数名： get_12digit_input
*功能：获取12路数字输入
*参数：无
*返回：12路数字输入的值
*日期：2025.05.09
********************************************/
uint16_t get_12digit_input(void)
{
    uint16_t res = 0;
    uint8_t i = 0;
    uint16_t iput_temp = 0;
    iput_temp = GPIOF->IDR;

    for (i=0; i<11; i++)
    {
        res <<= 1;

        if (iput_temp & 0x0001)res |= 0x0001; //获取低11位  0000 0100 0000 0000

        iput_temp >>= 1;
    }

    if (GPIOE->IDR & (1<<6))res |= (1<<11); //获取高1位

    return res;
}


// 函数用于打印数组元素
void print_rx3_buffer(char rx3_buffer[], int size)
{
    for (int i = 0; i < size; i++)
    {
        USART_Send_Byte(Usart1, rx3_buffer[i]);
    }
}

/********************************************
*函数名： deal_rx_buff
*功能：处理串口缓存
*参数1：执行周期 ms
*返回：无
*日期：2025.05.09
********************************************/
void deal_rx_buff(uint16_t tim)//处理串口数据
{
    static uint64_t ttp = 0;

    if (systim_ms >= ttp + tim) //tim ms do one time
    {
        ttp = systim_ms;

        if (strstr(rx1_buffer, USART1_END_OF_FRAME) != NULL)
        {
            for (uint8_t i=0; i< com1_protocols_max; i++)
            {
                if (strstr(rx1_buffer, com1_lst[i].command) != NULL)
                    com1_lst[i].func();
            }

            memset(rx1_buffer, 0, sizeof(rx1_buffer));
            rx1_buffer_cnt = 0;
        }

        if (strstr(rx2_buffer, USART2_END_OF_FRAME) != NULL)
        {
            for (uint8_t i=0; i< com2_protocols_max; i++)
            {
                if (strstr(rx2_buffer, com2_lst[i].command) != NULL)
                    com2_lst[i].func();
            }

            memset(rx2_buffer, 0, sizeof(rx2_buffer));
            rx2_buffer_cnt = 0;
        }

        //         // 检查帧头标志
        //        if (rx3_buffer[0] != 0x5A || rx3_buffer[1] != 0x5A) {
        //            return ; // 帧头标志不正确，返回 0 表示校验失败
        //        }
        //        // 检查数据量
        //        if (rx3_buffer[3] != 0x04) {
        //            return ; // 数据量不正确，返回 0 表示校验失败
        //        }
        //        if(rx3_buffer_cnt==9)
        //        {
        //            lux_value=Usart3_Event();
        //            memset(rx3_buffer,0,sizeof(rx3_buffer));
        //            rx3_buffer_cnt = 0;
        //        }

        //        int size = sizeof(rx3_buffer) / sizeof(rx3_buffer[0]);
        //        print_rx3_buffer(rx3_buffer, size);
        /*--------------------------------------------------------------------------------*/
        //        if(strstr(rx3_buffer,USART3_END_OF_FRAME) != NULL)
        //        {
        ///**************************判断指令*********************************/
        ////            for(uint8_t i=0;i< com3_protocols_max;i++ )
        ////            {
        ////                if(strstr(rx3_buffer,com3_lst[i].command) != NULL)
        ////                com3_lst[i].func();
        ////            }
        ///***********************************************************/
        //
        ///**************************直接将接收信息，通过USART1发送出去*********************************/
        ////            USART_SendString(Usart1,(u8 *)&rx3_buffer);// 用串口1（PC）转发串口3 RS485接收到的信息
        ///******************************************************************************************/
        //            memset(rx3_buffer,0,sizeof(rx3_buffer));
        //            rx3_buffer_cnt = 0;
        //        }
        /*--------------------------------------------------------------------------------*/
    }
}

double Usart3_Event()
{
    // 计算 Lux 的初始值
    unsigned int Lux = (rx3_buffer[4] << 24) | (rx3_buffer[5] << 16) | (rx3_buffer[6] << 8) | rx3_buffer[7];
    // 进行除法运算
    lux_value = (double)Lux / 100;
    // 输出结果
    return lux_value;
}

void RESET_FIXTURE(void)//串口指令，治具复位
{
    Flag_RESET_OK=1;
}


//校正LED LUX值
void Cal_lux(void)
{
    READ_LX1();
    delay_ms(10);

    Cal.LED1=152000-lux_value;
    Cal.LED1_Cal=(152000-lux_value)/30;
    usart3_Rx_flag=0;
    READ_LX2();
    delay_ms(10);
    Cal.LED2=152000-lux_value;
    Cal.LED2_Cal=(152000-lux_value)/30;
    usart3_Rx_flag=0;
    READ_LX3();
    delay_ms(10);
    Cal.LED3=152000-lux_value;
    Cal.LED3_Cal=(152000-lux_value)/30;
    usart3_Rx_flag=0;
    READ_LX4();
    delay_ms(10);
    Cal.LED4=152000-lux_value;
    Cal.LED4_Cal=(152000-lux_value)/30;
    usart3_Rx_flag=0;
    Flash_WriteStruct(StartPageAddress, &Cal);
    USART_printf(Usart1, "Calibration successful\r\n");
}

void Print_Lux_Sensor()
{
    if (usart3_Rx_flag==1)
    {
        usart3_Rx_flag=0;

        //        USART_printf(Usart1,"------------------------------------");
        switch (read_lux_flag)
        {
            case 1:
                //                 lux_value=lux_value+(Cal.LED1_Cal*pwm_duty[0]);
                USART_printf(Usart1, "LED%d:%.2fLUX\r\n", read_lux_flag, lux_value);
                break;

            case 2:
                //                 lux_value=lux_value+(Cal.LED2_Cal*pwm_duty[0]);
                USART_printf(Usart1, "LED%d:%.2fLUX\r\n", read_lux_flag, lux_value);
                break;

            case 3:
                //                 lux_value=lux_value+(Cal.LED3_Cal*pwm_duty[0]);
                USART_printf(Usart1, "LED%d:%.2fLUX\r\n", read_lux_flag, lux_value);
                break;

            case 4:
                //                 lux_value=lux_value+(Cal.LED4_Cal*pwm_duty[0]);
                USART_printf(Usart1, "LED%d:%.2fLUX\r\n", read_lux_flag, lux_value);
                break;
        }

        //    USART_printf(Usart1,"%.f\r\n",lux_value);
    }
}

void READ_LX1()
{
    // a5 01 a6
    // a5 02 a7
    // a5 03 a8
    // a5 04 a9
    read_lux_flag=1;
    USART_Send_Byte(Usart3, 0XA5);
    USART_Send_Byte(Usart3, 0X01);
    USART_Send_Byte(Usart3, 0XA6);
}

void READ_LX2()
{
    // a5 01 a6
    // a5 02 a7
    // a5 03 a8
    // a5 04 a9
    read_lux_flag=2;
    USART_Send_Byte(Usart3, 0XA5);
    USART_Send_Byte(Usart3, 0X02);
    USART_Send_Byte(Usart3, 0XA7);
}

void READ_LX3()
{
    // a5 01 a6
    // a5 02 a7
    // a5 03 a8
    // a5 04 a9
    read_lux_flag=3;
    USART_Send_Byte(Usart3, 0XA5);
    USART_Send_Byte(Usart3, 0X03);
    USART_Send_Byte(Usart3, 0XA8);
}

void READ_LX4()
{
    // a5 01 a6
    // a5 02 a7
    // a5 03 a8
    // a5 04 a9
    read_lux_flag=4;
    USART_Send_Byte(Usart3, 0XA5);
    USART_Send_Byte(Usart3, 0X04);
    USART_Send_Byte(Usart3, 0XA9);
}

void RGBC_1(void)
{
    USART_printf(Usart3, "RGBC 1\r\n");
}

void RGBC_2(void)
{
    USART_printf(Usart3, "RGBC 2\r\n");
}

void RGBC_3(void)
{
    USART_printf(Usart3, "RGBC 3\r\n");
}

void RGBC_4(void)
{
    USART_printf(Usart3, "RGBC 4\r\n");
}


void SET_CLA_RGB(void)
{
    USART_printf(Usart3, "cal_rgb\r\n");
    USART_printf(Usart1, "cal_rgb pass\r\n");
}

void READ_RGB(void)     //读取LED 颜色值
{
    USART_printf(Usart3, "RGBC 1\r\n");
}

void STOP_WLED(void)    //控制LED灭
{
    /*
    1：先设置PWM， $Set PWM Duty 1:0,1
    2：再关5V
    */
    const char *str = "SET_DUTY 1:0,1";
    strcpy(rx1_buffer, str);
    com1_protocol_5();
    _5V_OFF();//打开5V，
    Relay_WIN_LED_OFF();
}

void START_WLED(void)   //控制LED亮
{
    /*
    1：先设置PWM， $Set PWM Duty 1:50,51
    2：再开5V
    */
    //    const char *str1 = "$Set PWM Freq:50000,50000,100000";
    //    strcpy(rx1_buffer, str1);
    //    com1_protocol_4();
    const char *str2 = "SET_DUTY 1:30,31";
    strcpy(rx1_buffer, str2);
    com1_protocol_5();
    _5V_ON();//打开5V，
    Relay_WIN_LED_ON();
}

void SET_PWM30(void)   //控制LED亮
{
    /*
    1：先设置PWM， $Set PWM Duty 1:50,51
    2：再开5V
    */
    const char *str = "$Set PWM Duty 1:20,21";
    strcpy(rx1_buffer, str);
    com1_protocol_5();
    D_OUT7=1;
    _5V_ON();//打开5V，
}

void VINWLED(void)      //读取芯片VINWLED 引脚是否5V输出
{
    Start_Test_WINLED();
}

/********************************************
*函数名： com1_protocol_0
*功能： 串口1的协议0，测试或者握手
*参数：无
*返回：无
*日期：2025.05.09
********************************************/
void com1_protocol_0(void)
{
    USART_printf(Usart1, "#Hello Com1!\r\n");
}

/********************************************
*函数名： com1_protocol_1
*功能： 串口1的协议1，读取电压
*参数：无
*返回：无
*日期：2025.05.09
********************************************/
void com1_protocol_1(void)
{
    float ftemp[2] = {.0, .0};
    ftemp[0] = 3.3*(float)adc_val[0]/4096;
    ftemp[1] = 3.3*(float)adc_val[1]/4096;
    USART_printf(Usart1, "#vol:%.3f,%.3f\r\n", ftemp[0], ftemp[1]);
}

/********************************************
*函数名： com1_protocol_2
*功能： 串口1的协议2，格式化读取数字输入，$Read DI %b\r\n      支持%b %d %x
*参数：无
*返回：无
*日期：2025.05.11
********************************************/
void com1_protocol_2(void)
{
    char str_temp[10] = "";
    uint8_t num1 = Get_Middle_Str(str_temp, rx1_buffer, "$Read DI %", USART1_END_OF_FRAME);

    switch (str_temp[0])
    {
        case 'b' :
        {
            USART_printf(Usart1, "#Digit Input Is:");

            for (uint8_t i = 0; i<12; i++)
            {
                if (di_reg&(1<<(11-i)))USART_Send_Byte(Usart1, '1');
                else USART_Send_Byte(Usart1, '0');

                if ((11-i)%4==0)USART_Send_Byte(Usart1, ' ');
            }
        }
        break;

        case 'd' :
            USART_printf(Usart1, "#Digit Input Is:%d", di_reg);
            break;

        case 'x' :
            USART_printf(Usart1, "#Digit Input Is:%x", di_reg);
            break;

        default :
            USART_printf(Usart1, "#Format Input Error,Only Support %%b,%%d,%%x !\r\n");
            break;
    }
}

/********************************************
*函数名： com1_protocol_3
*功能： 串口1的协议3，设置数字输出，$Set DO :x\r\n 或者$Set DOx:y\r\n x取1~12
*参数：无
*返回：无
*日期：2025.05.09
********************************************/
void com1_protocol_3(void)
{
    char str_temp[10] = "";
    uint8_t num1 = Get_Middle_Str(str_temp, rx1_buffer, "$Set DO", ":");

    if (num1)
    {
        uint16_t num2 = atoi(str_temp);

        if (num2&&(num2<=12)) //1~12输入
        {
            char num3 = 0;

            if (num2 <= 9)num3 = rx1_buffer[num1+8];
            else num3 = rx1_buffer[num1+9];

            if (num3)
            {
                if (num3 == '0')do_reg &= ~(1<<(num2-1));
                else do_reg |= 1<<(num2-1);

                USART_printf(Usart1, "#Set Digit Output%d Success:%c,b:%x\r\n", num2, num3, do_reg);
            }
            else USART_printf(Usart1, "#Set Digit Output Fail:1\r\n", str_temp);
        }
        else if (rx1_buffer[num1+7] == ':')
        {
            num1 = Get_Middle_Str(str_temp, rx1_buffer, "$Set DO :", USART1_END_OF_FRAME);

            if (num1)
            {
                num2 = atoi(str_temp);

                if (num2 <= 0xfff)
                {
                    do_reg = num2;
                    USART_printf(Usart1, "#Set Digit Output Success:%d\r\n", do_reg);
                }
                else USART_printf(Usart1, "#Set Digit Output Fail:4\r\n", str_temp);
            }
            else USART_printf(Usart1, "#Set Digit Output Fail:3\r\n", str_temp);
        }
        else USART_printf(Usart1, "#Set Digit Output Fail:2\r\n", str_temp);

        //        set_12digit_out(do_reg);//加快响应速度
    }
    else USART_printf(Usart1, "#Set Digit Output Fail:0\r\n");
}

/********************************************
*函数名： com1_protocol_4
*功能： 串口1的协议4，设置PWM频率，$Set PWM Freq:f1,f2,sum\r\n  f1-第一组（PWM1~PWM4）频率  f2-第二组（PWM5~PWM8）频率  sum ASCII校验和，f1,f2 取 1~50k
*参数：无
*返回：无
*日期：2025.05.12
********************************************/
void com1_protocol_4(void)
{
    // 定义一个长度为 10 的字符数组 str_temp 并初始化为空字符串，用于存储从字符串中提取的子字符串
    char str_temp[10] = "";

    // 调用 Get_Middle_Str 函数，从 rx1_buffer 中提取从 "$Set PWM Freq:" 开始到第一个逗号之间的子字符串，
    // 并将提取的子字符串长度存储在 num1 中
    uint8_t num1 = Get_Middle_Str(str_temp, rx1_buffer, "SET_FREQ:", ",");

    // 注释说明输入字符串的格式：
    // 输入字符串格式为：$Set PWM Freq:f1,f2,sum\r\n
    // f1 - 第一组（PWM1~PWM4）频率
    // f2 - 第二组（PWM5~PWM8）频率
    // sum - ASCII 校验和
    // 检查是否成功提取到子字符串（即 num1 是否大于 0）
    if (num1)
    {
        // 定义一个长度为 2 的无符号 16 位整数数组 pwm_freq_temp，用于缓存提取到的 PWM 频率值，初始化为 0
        uint16_t pwm_freq_temp[2] = {0, 0};
        // 定义一个无符号 32 位整数变量 pwm_freq_sum，用于存储提取到的校验和，初始化为 0
        uint32_t pwm_freq_sum = 0;
        // 定义一个无符号 8 位整数变量 res，用于记录参数验证通过的数量，初始化为 0
        uint8_t res = 0;

        // 将提取到的子字符串转换为长整型数值，并存储在 pwm_freq_temp[0] 中
        pwm_freq_temp[0] = atol(str_temp);

        // 检查 pwm_freq_temp[0] 是否大于 0 且小于等于 50000，如果满足条件则 res 加 1
        if (pwm_freq_temp[0] && (pwm_freq_temp[0]<=50000))res++;

        // 调用 Get_Middle_Str 函数，从 rx1_buffer 中 num1 位置开始，提取从第一个逗号到第二个逗号之间的子字符串，
        // 并将提取的子字符串长度存储在 num2 中
        uint8_t num2 = Get_Middle_Str(str_temp, rx1_buffer+num1, ",", ",");

        // 检查是否成功提取到子字符串（即 num2 是否大于 0）
        if (num2)
        {
            // 将提取到的子字符串转换为长整型数值，并存储在 pwm_freq_temp[1] 中
            pwm_freq_temp[1] = atol(str_temp);

            // 检查 pwm_freq_temp[1] 是否大于 0 且小于等于 50000，如果满足条件则 res 加 1
            if (pwm_freq_temp[1] && (pwm_freq_temp[1]<=50000))res++;
        }

        // 调用 Get_Middle_Str 函数，从 rx1_buffer 中 num1 + num2 位置开始，提取从第二个逗号到 USART1_END_OF_FRAME 之间的子字符串，
        // 并将提取的子字符串长度存储在 num3 中
        uint8_t num3 = Get_Middle_Str(str_temp, rx1_buffer+num1+num2, ",", USART1_END_OF_FRAME);

        // 检查是否成功提取到子字符串（即 num3 是否大于 0）
        if (num3)
        {
            // 将提取到的子字符串转换为长整型数值，并存储在 pwm_freq_sum 中
            pwm_freq_sum = atol(str_temp);

            // 检查 pwm_freq_sum 是否大于 0 且小于等于 100000，如果满足条件则 res 加 1
            if (pwm_freq_sum && (pwm_freq_sum<=100000))res++;
        }

        // 检查参数验证通过的数量是否为 3
        if (res == 3)
        {
            // 检查校验和是否等于两组 PWM 频率之和
            if (pwm_freq_sum == pwm_freq_temp[0]+pwm_freq_temp[1])
            {
                // 如果校验和正确，将缓存的 PWM 频率值赋值给 pwm_freq 数组
                pwm_freq[0] = pwm_freq_temp[0];
                pwm_freq[1] = pwm_freq_temp[1];
                // 调用 Timer_init 函数初始化定时器 3 用于 PWM 输出，设置周期和频率
                // 周期 20us，频率 50k，最大 (1440,50000) 1HZ
                Timer_init(PWM_OUT, Tim3, 1440, 50000/pwm_freq[0]);
                // 调用 Timer_init 函数初始化定时器 4 用于 PWM 输出，设置周期和频率
                // 周期 20us，频率 50k，最大 (1440,50000) 1HZ
                Timer_init(PWM_OUT, Tim4, 1440, 50000/pwm_freq[1]);
                // 通过串口 1 发送设置成功的消息，包含两组 PWM 频率值
                USART_printf(Usart1, "#Set PWM Freq Success:f1:%d,f2:%d\r\n", pwm_freq[0], pwm_freq[1]);
            }
            else
                // 如果校验和错误，通过串口 1 发送设置失败的消息，错误代码为 2
                USART_printf(Usart1, "#Set PWM Freq Fail:2\r\n");
        }
        else
            // 如果参数验证通过的数量不为 3，通过串口 1 发送设置失败的消息，错误代码为 1
            USART_printf(Usart1, "#Set PWM Freq Fail:1\r\n");
    }
    else
        // 如果没有成功提取到子字符串，通过串口 1 发送设置失败的消息，错误代码为 0
        USART_printf(Usart1, "#Set PWM Freq Fail:0\r\n");
}

/********************************************
*函数名： com1_protocol_5
*功能： 串口1的协议5，设置PWM占空比，$Set PWM Duty 3:11,14\r\n 或者 $Set PWM Duty :p1,p2,p3,p4,p5,p6,p7,p8,sum\r\n   p1~p8 占空比1~8   sum ASCII校验和
*参数：无
*返回：无
*日期：2025.05.12
********************************************/
void com1_protocol_5(void)
{
    // 协议格式：$Set PWM Duty 3:11,14\r\n 或者 $Set PWM Duty :p1,p2,p3,p4,p5,p6,p7,p8,sum\r\n
    // p1~p8 表示占空比1~8，sum 是ASCII校验和
    // 用于存储 sscanf 函数的返回值，即成功匹配并赋值的参数个数
    int num1 = 0;
    // 定义一个长度为10的整型数组，用于存储从串口接收缓冲区解析出来的整数
    // 初始化为全0
    int int_temp[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    // 使用 sscanf 函数从串口1的接收缓冲区 rx1_buffer 中解析数据
    // 尝试匹配 $Set PWM Duty %1d:%d,%d,%d,%d,%d,%d,%d,%d,%d 格式
    // 并将解析出的整数依次存储到 int_temp 数组中
    // num1 存储成功匹配并赋值的参数个数
    //    num1 = sscanf(rx1_buffer,"$Set PWM Duty %1d:%d,%d,%d,%d,%d,%d,%d,%d,%d",
    //                 &int_temp[0],&int_temp[1],&int_temp[2],&int_temp[3],&int_temp[4],
    //                 &int_temp[5],&int_temp[6],&int_temp[7],&int_temp[8],&int_temp[9]);
    num1 = sscanf(rx1_buffer, "SET_DUTY %1d:%d,%d,%d,%d,%d,%d,%d,%d,%d",
                  &int_temp[0], &int_temp[1], &int_temp[2], &int_temp[3], &int_temp[4],
                  &int_temp[5], &int_temp[6], &int_temp[7], &int_temp[8], &int_temp[9]);

    // 这行代码被注释掉了，原本用于调试输出解析结果
    // USART_printf(Usart1,"1:%d,%d,%d,%d\r\n",int_temp[0],int_temp[1],int_temp[2],num1);
    // 如果 int_temp[0] 为 0，表示是统一设置模式
    if (int_temp[0] == 0)
    {
        // 重新使用 sscanf 函数从串口1的接收缓冲区 rx1_buffer 中解析数据
        // 尝试匹配 $Set PWM Duty :%d,%d,%d,%d,%d,%d,%d,%d,%d 格式
        // 并将解析出的整数依次存储到 int_temp 数组的第 1 到 9 个元素中
        // num1 存储成功匹配并赋值的参数个数
        num1 = sscanf(rx1_buffer, "$Set PWM Duty :%d,%d,%d,%d,%d,%d,%d,%d,%d",
                      &int_temp[1], &int_temp[2], &int_temp[3], &int_temp[4],
                      &int_temp[5], &int_temp[6], &int_temp[7], &int_temp[8], &int_temp[9]);

        // 这行代码被注释掉了，原本用于调试输出解析结果
        // USART_printf(Usart1,"2:%d,%d,%d,%d\r\n",int_temp[0],int_temp[1],int_temp[2],num1);
        // 如果成功匹配并赋值的参数个数为 9，表示接收到的指令格式正确
        if (num1 == 9)
        {
            // 定义一个无符号8位整型变量 i，用于循环计数，初始化为 1
            uint8_t i = 1;
            // 定义一个无符号16位整型变量 sum，用于计算占空比的总和，初始化为 0
            uint16_t sum = 0;

            // 循环遍历 int_temp 数组的第 1 到 8 个元素
            for (; i<9; i++)
            {
                // 如果占空比小于 0，则将其设为 0
                int_temp[i] = int_temp[i]<0?0:int_temp[i];
                // 如果占空比超过 100，则将其设为 100
                int_temp[i] = int_temp[i]<100?int_temp[i]:100;
                // 累加占空比到 sum 中
                sum += int_temp[i];
            }

            // 如果计算得到的总和等于接收到的校验和 int_temp[9]
            if (sum == int_temp[9])
            {
                // 循环遍历 pwm_duty 数组，将解析出的占空比依次赋值给 pwm_duty 数组
                for (i=0; i<8; i++)
                    pwm_duty[i] = int_temp[i+1];

                // 向串口1发送设置成功的消息，包含设置好的占空比
                USART_printf(Usart1, "#Set PWM Duty Success:%d,%d,%d,%d,%d,%d,%d,%d\r\n",
                             pwm_duty[0], pwm_duty[1], pwm_duty[2], pwm_duty[3], pwm_duty[4], pwm_duty[5], pwm_duty[6], pwm_duty[7]);
            }
            else
                // 如果校验和不匹配，向串口1发送设置失败的消息，错误代码为 2
                USART_printf(Usart1, "#Set PWM Duty Fail:2\r\n");
        }
        else
            // 如果成功匹配并赋值的参数个数不为 9，向串口1发送设置失败的消息，错误代码为 1
            USART_printf(Usart1, "#Set PWM Duty Fail:1\r\n");
    }
    // 如果 int_temp[0] 不为 0 且小于等于 8，表示是独立设置模式
    else if (int_temp[0] && (int_temp[0]<=8))
    {
        // 如果成功匹配并赋值的参数个数为 3，表示接收到的指令格式正确
        if (num1 == 3)
        {
            // 如果占空比小于 0，则将其设为 0
            int_temp[1] = int_temp[1]<0?0:int_temp[1];
            // 如果占空比超过 100，则将其设为 100
            int_temp[1] = int_temp[1]<100?int_temp[1]:100;

            // 如果接收到的校验和 int_temp[2] 等于 int_temp[0] 与 int_temp[1] 的和
            if (int_temp[2] == int_temp[0]+int_temp[1])
            {
                // 再次检查 int_temp[0] 是否在 1 到 8 的范围内
                if (int_temp[0] && (int_temp[0]<=8))
                {
                    // 将解析出的占空比赋值给 pwm_duty 数组中对应的元素
                    pwm_duty[int_temp[0]-1] = int_temp[1];
                    // 向串口1发送设置成功的消息，包含设置的 PWM 序号和占空比
                    //USART_printf(Usart1,"#Set PWM Duty %d Success:%d\r\n",int_temp[0],int_temp[1]);
                    USART_printf(Usart1, "SUCCESS:%d\r\n", int_temp[1 ]);
                }
                else
                    // 如果 PWM 序号不在 1 到 8 的范围内，向串口1发送设置失败的消息，错误代码为 3
                    USART_printf(Usart1, "#Set PWM Duty Fail:3\r\n");
            }
            else
                // 如果校验和不匹配，向串口1发送设置失败的消息，错误代码为 2
                USART_printf(Usart1, "#Set PWM Duty Fail:2\r\n");
        }
        else
            // 如果成功匹配并赋值的参数个数不为 3，向串口1发送设置失败的消息，错误代码为 1
            USART_printf(Usart1, "#Set PWM Duty Fail:1\r\n");
    }
    else
        // 如果 int_temp[0] 不满足上述条件，向串口1发送设置失败的消息，错误代码为 0
        USART_printf(Usart1, "#Set PWM Duty Fail:0\r\n");
}

/********************************************
*函数名： com1_protocol_6
*功能： 串口1的协议6，关门
*参数：无
*返回：无
*作者：柯龑
*日期：2025.05.12
********************************************/
void com1_protocol_6(void)
{
    Flag_BOX_CLOSE=1;
}

/********************************************
*函数名： com1_protocol_7
*功能： 串口1的协议7，开门
*参数：无
*返回：无
*日期：2025.05.12
********************************************/
void com1_protocol_7(void)
{
    Flag_BOX_OPEN=1;
}

/********************************************
*函数名： com1_protocol_8
*功能： 串口1的协议8，读取门状态
*参数：无
*返回：无
*日期：2025.05.12
********************************************/
void com1_protocol_8(void)
{
    enum {UNKNOWN = 0, OPEN, CLOSE};
    uint8_t door_sta = UNKNOWN;//缓存门状态
    //要做的事，结果传到door_sta中

    if (Fixture_DOWN_Sensor()==0)
        door_sta=CLOSE;

    if (Fixture_OUT_Sensor()==0)
        door_sta=OPEN;

    switch (door_sta)
    {
        case UNKNOWN :
            USART_printf(Usart1, "BOX_STATUS_UNKNOWN\r\n");
            break;//未知状态

        case OPEN    :
            USART_printf(Usart1, "BOX_STATUS_OPEN\r\n");
            break;//打开状态

        case CLOSE   :
            USART_printf(Usart1, "BOX_STATUS_CLOSE\r\n");
            break;//关闭状态

        default :
            break;
    }
}

/********************************************
*函数名： com1_protocol_9
*功能： 串口1的协议9，开始测短路
*参数：无
*返回：无
*日期：2025.05.12
********************************************/
void com1_protocol_9(void)//开始测短路
{
    Start_Test_Short();
    delay_ms(10);
    //    USART_printf(Usart1,"#OK Start Test Short!\r\n");//开始测试短路
}

/********************************************
*函数名： com1_protocol_10
*功能： 串口1的协议10，开始测开路
*参数：无
*返回：无
*日期：2025.05.12
********************************************/
void com1_protocol_10(void)//开始测开路
{
    Start_Test_Open();
    delay_ms(10);
    //    USART_printf(Usart1,"#OK Start Test Open!\r\n");//开始测试短路
}

/********************************************
*函数名： com1_protocol_11
*功能： 串口1的协议11，设置5V 2A输出
*参数：无
*返回：无
*日期：2025.05.12
********************************************/
void com1_protocol_11(void)
{
    //RET107:XXXXEND
    //    Start_Test_WINLED();
    //    delay_ms(10);
    //    USART_printf(Usart1,"#OK Start Test WINLED!\r\n");//开始测试短路
    //    float f_temp = .0;
    //    f_temp = 0.001611*adc_val[2];//6.6*adc_val[2]/4096;
    _5V_EN = 1;
    USART_printf(Usart1, "_5V_ON_READY\r\n");
    //    EX_VTEN = 1;
    //    USART_printf(Usart1,"RET107:%.1f\r\n",f_temp);
}

void com1_protocol_12(void)
{
    _5V_EN = 0;
    USART_printf(Usart1, "_5V_OFF_READY\r\n");
}

/********************************************
*函数名： com2_protocol_0
*功能： 串口2的协议0，测试或者握手
*参数：无
*返回：无
*日期：2025.05.12
********************************************/
void com2_protocol_0(void)
{
    USART_printf(Usart2, "#Hello Com2!\r\n");
}

/********************************************
*函数名： com3_protocol_0
*功能： 串口3的协议0，测试或者握手
*参数：无
*返回：无
*日期：2025.05.12
********************************************/
void com3_protocol_0(void)
{
    USART_printf(Usart3, "#Hello Com3!\r\n");
}

/********************************************
*函数名：run
*功能：运行，执行主要逻辑功能
*参数：运行周期 ms >=1
*返回：无
*日期：2025.05.09
********************************************/
void run(uint16_t tim)//程序测试开短路执行函数
{
    static uint64_t ttp = 0;

    if (systim_ms >= ttp + tim)
    {
        ttp = systim_ms;    //获取系统定时时间
        double r = 0;       //测试电阻值
        //        select_channel(1,191);
        //        USART_printf(Usart1,"#v:%d,%d\r\n",adc_val[0],adc_val[1]);

        //        select_channel(1,185);
        //        switch(gear)
        //        {
        //            case __100R : r = 100.0f/(4096.0f/(float)adc_val[0] - 1);break;// - 9.3f;break;//9.3是各个IC内部导通阻抗和，粗略按照9.3计算 1~32和185~192  33~184内阻增加2.7~2.8R
        //            case __10K : r = 10000.0f/(4096.0f/(float)adc_val[0] - 1);break;
        //            case __1M : r = 1000000.0f/(4096.0f/(float)adc_val[0] - 1);break;
        //            default : break;
        //        }
        //        if(r < 1000)USART_printf(Usart1,"#r1-%d:%.1fR,%d,dw:%d\r\n",xx,r,adc_val[0],gear);
        //        else if(r < 1000000)USART_printf(Usart1,"#r1-%d:%.2fK,%d,dw:%d\r\n",xx,r/1000,adc_val[0],gear);
        //        else USART_printf(Usart1,"#r1-%d:%.2fM,%d,dw:%d\r\n",xx,r/1000000,adc_val[0],gear);


        /************************************************************************************************/
        if (Check_Start_Test_Short()) // 检查是否开始短路测试
        {
            if (test_cnt < test_point_short_tab_max) // 若测试点计数未达到最大值
            {
                Relay_ISOLATION_ON(); // 打开隔离继电器

                static uint8_t gear_temp = 0; // 保存上一次的量程档位
                static uint16_t same_cnt = 0; // 同档位累计次数，用于判断量程稳定
                float r_compensate = 0;       // 补偿电阻值
                static uint8_t test_en = 0;   // 测试使能，47号测试点需气缸动作后才能测试
                static uint8_t cylinder_status = 0; // 气缸状态 0起始 1下压 2上升 3回位

                if (test_cnt == 46) // 第47个点测试前需要先气缸下压
                {
                    static uint16_t over_tim_cnt = 0; // 气缸动作超时计数

                    switch (cylinder_status)
                    {
                        case 0:
                            CYLINDER_DOWN();        // 控制气缸下压
                            cylinder_status = 1;    // 进入下压状态
                            test_en = 0;            // 暂停测试
                            break;
                        case 1:
                            if (++over_tim_cnt >= 1000) // 下压延时1s（等待稳定）
                            {
                                //USART_printf(Usart1, "计数时间：%d Ms\r\n", over_tim_cnt );
                                over_tim_cnt = 0;   // 清空延时计数
                                test_en = 1;        // 允许开始测试
                            }
                            break;
                        default:
                            break;
                    }
                }
                else test_en = 1; // 非47号点，直接允许测试
                
                if (test_en) // 允许测试
                {
                    select_channel(test_point_short_tab[test_cnt][0], test_point_short_tab[test_cnt][1]); // 选择测试两端通道
                    // 判断补偿电阻，根据经过的芯片数量决定补偿值
                    if ((test_point_short_tab[test_cnt][0] <= 32) || (test_point_short_tab[test_cnt][0] >= 185))
                    {
                        if ((test_point_short_tab[test_cnt][1] <= 32) || (test_point_short_tab[test_cnt][1] >= 185)) r_compensate = 9;
                        else r_compensate = 11.5;
                    }
                    else
                    {
                        if ((test_point_short_tab[test_cnt][1] <= 32) || (test_point_short_tab[test_cnt][1] >= 185)) r_compensate = 11.5;
                        else r_compensate = 9;
                    }
                    if (gear_temp == gear) // 若量程档位未改变
                    {
                        if (++same_cnt >= 5) // 累计5次相同档位，认为量程已稳定
                        {
                            switch (gear) // 根据档位计算电阻值
                            {
                                case __100R:
                                    r = 200.0f / (4096.0f / (float)adc_val[0] - 1) - r_compensate;
                                    break;
                                case __10K:
                                    r = 10000.0f / (4096.0f / (float)adc_val[0] - 1);
                                    break;
                                case __1M:
                                    r = 1000000.0f / (4096.0f / (float)adc_val[0] - 1);
                                    break;
                                default:
                                    break;
                            }
                            r = r < 0 ? 0 : r; // 防止出现负数
                            USART_printf(Usart1, "RET%d:%dEND\r\n", test_cnt + 1, (uint32_t)r); // 输出格式 RET1:XXXXEND
                            same_cnt = 0; // 清空计数
                            

                            if (test_cnt == 46) // 第47点测完后抬升气缸
                            {
                                if (cylinder_status == 1)
                                {
                                    CYLINDER_UP();          // 气缸抬升
                                    cylinder_status = 0;    // 状态复位
                                    test_cnt++;             // 测试点 +1，此时 test_cnt = 48
                                }
                                test_en = 0;                // 禁止继续测试，等待下一次循环
                            }
                            else
                            {
                                test_cnt++;                 // 正常情况下直接进入下一个测试点
                            }
                            
                            
                            
                        }
                    }
                    else // 量程变化，重新计数
                    {
                        gear_temp = gear;
                        same_cnt = 0;
                    }
                }

            }
            else // 测试完成
            {
                //USART_printf(Usart1, "测试完成\r\n"); 
                Relay_ISOLATION_OFF();  // 关闭隔离继电器
                Stop_Test_Short();      // 停止短路测试流程
                test_cnt = 0;           // 复位计数
            }
        }

        /************************************************************************************************/
        else if (Check_Start_Test_Open()) //开始测试开路
        {
            if (test_cnt < test_point_open_tab_max)
            {
                Relay_ISOLATION_ON();
                static uint8_t gear_temp = 0;
                static uint16_t same_cnt = 0;
                float r_compensate = 0;//补偿电阻
                select_channel(test_point_open_tab[test_cnt][0], test_point_open_tab[test_cnt][1]);
                if ((test_point_open_tab[test_cnt][0]<=32) || (test_point_open_tab[test_cnt][0]>=185))
                {
                    if ((test_point_open_tab[test_cnt][1]<=32) || (test_point_open_tab[test_cnt][1]>=185))r_compensate = 9;
                    else r_compensate = 11.5;
                }
                else
                {
                    if ((test_point_open_tab[test_cnt][1]<=32) || (test_point_open_tab[test_cnt][1]>=185))r_compensate = 11.5;
                    else r_compensate = 9;
                }

                if (gear_temp == gear)
                {
                    if (++same_cnt >= 5) //自动量程已稳定
                    {
                        switch (gear)
                        {
                            case __100R :
                                r = 200.0f/(4096.0f/(float)adc_val[0] - 1) - r_compensate;
                                break;// - 9.3f;break;//9.3是各个IC内部导通阻抗和，粗略按照9.3计算 1~32和185~192  33~184内阻增加2.7~2.8R

                            case __10K :
                                r = 10000.0f/(4096.0f/(float)adc_val[0] - 1);
                                break;

                            case __1M :
                                r = 1000000.0f/(4096.0f/(float)adc_val[0] - 1);
                                break;

                            default :
                                break;
                        }

                        //USART_printf(Usart1,"test%d & %d-------",test_point_short_tab[test_cnt][0],test_point_short_tab[test_cnt][1]);
                        USART_printf(Usart1, "RET%d:%dEND\r\n", test_cnt+48, (uint32_t)r); //按照 RET1:XXXXEND 发送数据
                        same_cnt = 0;
                        test_cnt++;
                    }
                }
                else
                {
                    gear_temp = gear;
                    same_cnt = 0;
                }

            }
            else//测试完毕
            {
                Relay_ISOLATION_OFF();
                Stop_Test_Open();
                test_cnt = 0;
                //USART_printf(Usart1,"#Open Test Finish!\r\n");//测试完毕
            }
        }
        /************************************************************************************************/
        else if (Check_Start_Test_WINLED()) //开始测试WINLED
        {
            static uint8_t gear_temp = 0;
            static uint16_t same_cnt = 0;
            _5V_EN = 1;
            EX_VTEN = 1;
            Relay_VIN_EN_ON();

            if (gear_temp == gear)
            {
                if (++same_cnt >= 1000) //自动量程已稳定
                {
                    float f_temp = .0;
                    f_temp = 0.001611*adc_val[2];//6.6*adc_val[2]/4096;
                    USART_printf(Usart1, "RET106:%.2fEND\r\n", f_temp);

                    _5V_EN = 0;
                    EX_VTEN = 0;
                    Relay_VIN_EN_OFF();
                    Stop_Test_WINLED();
                    same_cnt = 0;
                }
            }
            else
            {
                gear_temp = gear;
                same_cnt = 0;
            }
        }

        /************************************************************************************************/

        //set_12digit_out(do_reg);          //更新数字输出--------------------------------Evan 20250521
        //di_reg = get_12digit_input();     //更新数字输入--------------------------------Evan 20250521

        //        //更新PWM
        TIM_SetCompare4(TIM3, (float)pwm_duty[0]*14.4);
        TIM_SetCompare3(TIM3, (float)pwm_duty[1]*14.4);
        TIM_SetCompare2(TIM3, (float)pwm_duty[2]*14.4);
        TIM_SetCompare1(TIM3, (float)pwm_duty[3]*14.4);

        TIM_SetCompare4(TIM4, (float)pwm_duty[4]*14.4);
        TIM_SetCompare3(TIM4, (float)pwm_duty[5]*14.4);
        TIM_SetCompare2(TIM4, (float)pwm_duty[6]*14.4);
        TIM_SetCompare1(TIM4, (float)pwm_duty[7]*14.4);
        //

    }
}



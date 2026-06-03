#include "adc.h"

//u16 adc = 0;


uint16_t adc_val[3] = {0,0,0};//0 test point   1 RPR_AI测光敏   2 EXAI测外部电压 0~6.6
float test_point_vol = .0;//test point voltage
float phtres_vol = .0;//photoresistor voltage

uint16_t adc_cnt = 0;
uint16_t adc_max = 0;
uint16_t adc_min = 4095;
uint32_t adc_sum = 0;
uint8_t adc_point = 0;//循环采集 0--test point  1--photoresistor

void  Adc_Init(void)
{ 	
	ADC_InitTypeDef ADC_InitStructure; 
	GPIO_InitTypeDef GPIO_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;
  
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA |RCC_APB2Periph_ADC1	, ENABLE );	  //使能ADC1通道时钟
//    NVIC_PriorityGroupConfig();
    NVIC_InitStructure.NVIC_IRQChannel = ADC1_2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_Init(&NVIC_InitStructure);
    
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);   //72M/6=12,ADC最大时间不能超过14M
//	//PA1 PA2 作为模拟通道输入引脚                         
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;		//模拟输入引脚
//	GPIO_Init(GPIOA, &GPIO_InitStructure);
  //PA1                        
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_4 | GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;		//模拟输入引脚
	GPIO_Init(GPIOA, &GPIO_InitStructure);
    
	ADC_DeInit(ADC1);  //将外设 ADC1 的全部寄存器重设为缺省值
    
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;	//ADC工作模式:ADC1和ADC2工作在独立模式
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;	//模数转换工作在单通道模式
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;	//模数转换工作在单次转换模式
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//转换由软件而不是外部触发启动
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	//ADC数据右对齐
	ADC_InitStructure.ADC_NbrOfChannel = 1;	//顺序进行规则转换的ADC通道的数目
	ADC_Init(ADC1, &ADC_InitStructure);	//根据ADC_InitStruct中指定的参数初始化外设ADCx的寄存器   
    
	ADC_Cmd(ADC1, ENABLE);	//使能指定的ADC1
    
	ADC_ResetCalibration(ADC1);	//重置指定的ADC1的校准寄存器
	 
	while(ADC_GetResetCalibrationStatus(ADC1));	//获取ADC1重置校准寄存器的状态,设置状态则等待
	
	ADC_StartCalibration(ADC1);		//开始指定ADC1的校准状态
    
	while(ADC_GetCalibrationStatus(ADC1)){};		//获取指定ADC1的校准程序,设置状态则等待
    
    ADC_ITConfig(ADC1,ADC_IT_EOC,ENABLE);
    ADC_RegularChannelConfig(ADC1, 1, 1, ADC_SampleTime_239Cycles5 );
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);		//使能指定的ADC1的软件转换启动功能
    
}
//获得ADC值
//ch:通道值 0~3
u16 Get_Adc(u8 ch)   
{
  	//设置指定ADC的规则组通道，设置它们的转化顺序和采样时间
	ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_239Cycles5 );	//ADC1,ADC通道3,规则采样顺序值为1,采样时间为239.5周期	  			    
  
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);		//使能指定的ADC1的软件转换启动功能	
	 
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));//等待转换结束

	return ADC_GetConversionValue(ADC1);	//返回最近一次ADC1规则组的转换结果
}

void Get_Adc_Data(void)
{
    uint16_t adc_temp = 0;
    if(++adc_cnt > 2)
    {
        adc_temp = ADC_GetConversionValue(ADC1);
        adc_max = adc_temp>adc_max?adc_temp:adc_max;
        adc_min = adc_temp<adc_min?adc_temp:adc_min;
        adc_sum += adc_temp;
        if(adc_cnt == 20)
        {
            adc_sum -= (adc_min+adc_max);
            if(adc_point==0)
            {
                adc_val[0] = (uint16_t)(adc_sum>>4);
                ADC_RegularChannelConfig(ADC1, 4, 1, ADC_SampleTime_239Cycles5 );
                adc_point++;
            }
            else if(adc_point == 1)
            {
                adc_val[1] = (uint16_t)(adc_sum>>4);
                ADC_RegularChannelConfig(ADC1, 5, 1, ADC_SampleTime_239Cycles5 );
                adc_point = 2;
            }
            else if(adc_point == 2)
            {
                adc_val[2] = (uint16_t)(adc_sum>>4);
                ADC_RegularChannelConfig(ADC1, 1, 1, ADC_SampleTime_239Cycles5 );
                adc_point = 0;
            }
            
            adc_cnt = 0;
            adc_sum = 0;
            adc_max = 0;
            adc_min = 4095;
        }
    }
//    if(adc_cnt < 20)
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);		//使能指定的ADC1的软件转换启动功能	

}

void ADC1_2_IRQHandler(void)
{
    if(ADC_GetITStatus(ADC1,ADC_IT_EOC))
    {
        Get_Adc_Data();
        ADC_ClearITPendingBit(ADC1,ADC_IT_EOC);
    }
    
}



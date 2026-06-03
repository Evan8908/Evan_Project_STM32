#ifndef __ADC_H
#define __ADC_H
#include "stm32f10x.h"

#define TEST_POINT_CH    ADC_Channel_0
#define PHOTORESISTOR_CH ADC_Channel_0

#define CONVERT_FINISH(ch) conv_sta |= (1<<ch) //adc_val[ch]转换完毕

#define CHECK_CONVERT_STATUS(ch) (conv_sta&(1<<ch)) //检查adc_val[ch] 是否转换完毕 1是


extern uint16_t adc_val[3] ;//
extern float test_point_vol ;//test point voltage
extern float phtres_vol ;//photoresistor voltage

extern uint16_t adc_cnt ;
extern uint16_t adc_max ;
extern uint16_t adc_min ;
extern uint32_t adc_sum ;
extern uint8_t adc_point ;//循环采集 0--test point  1--photoresistor


void Adc_Init(void);
u16  Get_Adc(u8 ch);  

#endif 

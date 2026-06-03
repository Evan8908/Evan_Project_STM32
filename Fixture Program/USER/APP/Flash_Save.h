#ifndef __FLASH_SAVE_H_
#define __FLASH_SAVE_H_
#include "sys.h"
#include "stdlib.h"
typedef struct
{
    uint32_t  LED1;
    uint32_t  LED1_Cal;
    uint32_t  LED2;
    uint32_t  LED2_Cal;
    uint32_t  LED3;
    uint32_t  LED3_Cal;
    uint32_t  LED4;
    uint32_t  LED4_Cal;
} Calibration;

extern const uint32_t StartPageAddress;
extern void FLASH_WriteData(uint32_t StartPageAddress, uint32_t Data1);
extern void FLASH_ReadData(uint32_t StartPageAddress, uint32_t *Data1, uint32_t *Data2);
extern void Flash_WriteStruct(uint32_t StartPageAddress, Calibration * St);
extern void Flash_ReadStruct(uint32_t StartPageAddress, Calibration * St);

extern  Calibration Cal; //存储结构体，存判断电流的最小值和最大值

#endif

#include "sys.h"
#include "string.h"
#include <stdio.h>
#include "Flash_Save.h"


Calibration Cal;
//
const uint32_t StartPageAddress = 0x0807F800;  // Flash 最后一页（2KB）,不要太靠前了

uint32_t data1 = 0, data2 = 0;      //验证程序用的


void FLASH_WriteData(uint32_t Address, uint32_t Data1)
{
    FLASH_Status status;
    // 解锁FLASH
    FLASH_Unlock();
    // 清除所有相关的FLASH标记位
    FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);

    status = FLASH_ErasePage(Address);  //擦除整页FLASH,不是单个地址

    if (status == FLASH_COMPLETE)
    {
        // 在指定地址写入数据
        FLASH_ProgramWord(Address, Data1);
        //        FLASH_ProgramWord(StartPageAddress + 4, Data2); //写第二个数据
    }

    // 锁定FLASH
    FLASH_Lock();
}

void FLASH_ReadData(uint32_t Address, uint32_t *Data1, uint32_t *Data2)
{
    // 直接从FLASH的地址读取数据
    *Data1 = *(volatile uint32_t*)Address;
    *Data2 = *(volatile uint32_t*)(Address + 4);
}

// 将结构体写入Flash
void Flash_WriteStruct(uint32_t Address, Calibration * St)
{
    uint32_t* flashPtr;
    uint32_t i ;
    FLASH_Unlock();                         // 解锁Flash
    FLASH_ErasePage(Address);      // 擦除指定的Flash页面
    flashPtr = (uint32_t*)St;               // 写入数据到Flash

    for (i = 0; i < sizeof(Cal) / 4; i++)     // 注意这里的sizeof(c),c是全局的结构体。不是St参数
    {
        FLASH_ProgramWord(Address + i * 4, flashPtr[i]);
    }

    FLASH_Lock();                           // 锁定Flash
}

// 从Flash的指定地址读取结构体数据
void Flash_ReadStruct(uint32_t Address, Calibration * St)
{
    uint32_t* flashPtr;
    uint32_t* dataPtr ;
    uint32_t i;
    flashPtr = (uint32_t*)Address;
    dataPtr = (uint32_t*)St;

    // 从Flash读取数据
    for (i = 0; i < sizeof(Cal) / 4; i++)     // 注意这里的sizeof(c),c是全局的结构体。不是St参数
    {
        dataPtr[i] = flashPtr[i];
    }

    //      memcpy(St, (void*)StartPageAddress, sizeof(c));
}

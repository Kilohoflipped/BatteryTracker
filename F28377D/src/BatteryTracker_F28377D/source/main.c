
/**
 * main.c
 */

#define FLASH_RUN 1
#define SRAM_RUN 2
#define RUN_TYPE SRAM_RUN
#if RUN_TYPE==FLASH_RUN
    extern Uint16 RamfuncsLoadStart;
    extern Uint16 RamfuncsLoadSize;
    extern Uint16 RamfuncsRunStart;
#endif

#include <F28x_Project.h>
#include <string.h>
#include <stdint.h>
#include "KiloLib.h"




int main(void)
{
    // 初始化系统控制
    InitSysCtrl();
    // 计算低速外设时钟频率
    LSPCLKFreq = LSPCLKCal();
    // 禁止所有中断
    DINT;
    IER = 0x0000;
    IFR = 0x0000;
    // 初始化PIE控制器
    InitPieCtrl();
    // 初始化PIE向量表
    InitPieVectTable();


    return 0;
}

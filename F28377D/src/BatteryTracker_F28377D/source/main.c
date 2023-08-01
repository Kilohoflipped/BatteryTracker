
/**
 * main.c
 */

#include <F28x_Project.h>
#include "PeriDevices.h"
#include "KiloLib.h"
#include <string.h>
#include <stdint.h>

#define FLASH_RUN 1
#define SRAM_RUN 2
#define RUN_TYPE SRAM_RUN
// #define _STAND_ALONE
#if RUN_TYPE==FLASH_RUN
    extern Uint16 RamfuncsLoadStart;
    extern Uint16 RamfuncsLoadSize;
    extern Uint16 RamfuncsRunStart;
#endif

void MemCopy(Uint16 *SourceAddr, Uint16* SourceEndAddr, Uint16* DestAddr);

int main(void)
{
    extern Uint32 LSPCLKFreq;
    // 初始化系统控制
    InitSysCtrl();
    #ifdef _STAND_ALONE
        #if RUN_TYPE==FLASH_RUN
            IPCBootCPU2(C1C2_BROM_BOOTMODE_BOOT_FROM_FLASH);
        #else
            IPCBootCPU2(C1C2_BROM_BOOTMODE_BOOT_FROM_RAM);
        #endif
    #endif
    // 计算低速外设时钟频率
    LSPCLKFreq = 50000000;//LSPCLKCal();
    // 禁止所有中断
    DINT;
    IER = 0x0000;
    IFR = 0x0000;
    // 初始化PIE控制器
    InitPieCtrl();
    // 初始化PIE向量表
    InitPieVectTable();
    #if RUN_TYPE==FLASH_RUN
        MemCopy(&RamfuncsLoadStart, &RamfuncsLoadEnd, &RamfuncsRunStart);
        InitFlash();
    #endif
    EALLOW;
    ClkCfgRegs.CLKSRCCTL3.bit.XCLKOUTSEL = 0;  //使用PLLSysClk
    ClkCfgRegs.XCLKOUTDIVSEL.bit.XCLKOUTDIV = 3;  // 使用8分频
    GPIO_SetupPinMux(73, GPIO_MUX_CPU1, 3);          // 初始化73脚的功能为Xclkout
    GPIO_SetupPinOptions(73, GPIO_OUTPUT, GPIO_ASYNC);
    EDIS;

    SCIAFIFOSetup();
    ConfigureSciA();
    ConfigureADC();
    ConfigureEPWM1();
    SetupADCChannel(0);
    PieCtrlRegs.PIEIER1.bit.INTx1 = 1;  // 在PIE中使能ADCINT中断
    IER |= M_INT1;                      // Enable CPU Interrupt 1
    EINT;                               // 使能全局中断
    ERTM;                               // 使能全局中断调试
    while (1)
    {
        SCIAMsg("!!!!!!!!!!\0");
        1;
    }
    return 0;
}

void MemCopy(Uint16 *SourceAddr, Uint16* SourceEndAddr, Uint16* DestAddr)
{
    while(SourceAddr < SourceEndAddr)
    {
        *DestAddr++ = *SourceAddr++;
    }
    return;
}


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
#if RUN_TYPE==FLASH_RUN
    extern Uint16 RamfuncsLoadStart;
    extern Uint16 RamfuncsLoadSize;
    extern Uint16 RamfuncsRunStart;
#endif

void MemCopy(Uint16 *SourceAddr, Uint16* SourceEndAddr, Uint16* DestAddr);
Uint16 AdcaIntFlag=0;         //进入AdcaInt中断标志

int main(void)
{
    extern Uint32 LSPCLKFreq;
    InitSysCtrl();                          // 初始化系统控制
    InitGpio();                             //
    LSPCLKFreq = 50000000;//LSPCLKCal()     // 计算低速外设时钟频率
    DINT;                                   // 禁止所有中断
    IER = 0x0000;
    IFR = 0x0000;
    InitPieCtrl();                          // 初始化PIE控制器
    InitPieVectTable();                     // 初始化PIE向量表
    #if RUN_TYPE==FLASH_RUN
        MemCopy(&RamfuncsLoadStart, &RamfuncsLoadEnd, &RamfuncsRunStart);
        InitFlash();
    #endif
    ConfigureGpio();
    ConfigureSciaFifo();
    ConfigureSciA();
    ConfigureADC();
    ConfigureEPWM1();
    //ConfigureEPWM2();
    SetupADCChannel();
    IER |= M_INT1;                          // 使能CPU中断组1
    EINT;                                   // 使能全局中断
    ERTM;                                   // 使能全局中断调试

    GPIO_WritePin(92, 1);
    EALLOW;
    PieCtrlRegs.PIEIER1.bit.INTx1 = 1;      // 在PIE中使能ADCINT中断
    CpuSysRegs.PCLKCR0.bit.TBCLKSYNC = 1;   // 同步ePWM,开始TB计数
    EPwm1Regs.TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN;
    EDIS;
    SciaMsg("!!!!!!!!!!");                  //引导
    while (1)
    {
        if(AdcaIntFlag==1)
        {
            //char ADCLowChar[] = "00000";
            //char ADCHighChar[] = "00000";
            Uint32 ADCResult0 = AdcaResultRegs.ADCRESULT0;         // 转换结果
            //Uint32 ADCReLow8 = ADCResult0 & 0xFF;                  // 获取低八位
            //Uint32 ADCReHigh8 = (ADCResult0>>8) & 0xFF;            // 获取高八位(取决于转换模式)
            while(AdcaRegs.ADCINTFLG.bit.ADCINT1 == 0);
            AdcaIntFlag = 0;
            //Int2Str(ADCReLow8,ADCLowChar);                  // 低八位数字转化为字符串
            //Int2Str(ADCReHigh8,ADCHighChar);                // 高八位数字转化为字符串
            SciaMsg("AD=");
            SciaXmit(ADCResult0 & 0xFF);
            SciaXmit((ADCResult0>>8) & 0xFF);
            //SciaMsg("!\n");
        }
    }
}

void MemCopy(Uint16 *SourceAddr, Uint16* SourceEndAddr, Uint16* DestAddr)
{
    while(SourceAddr < SourceEndAddr)
    {
        *DestAddr++ = *SourceAddr++;
    }
    return;
}

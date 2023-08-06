/*
 * PeriADC.c
 *
 *  Created on: 2023年7月25日
 *      Author: Administrator
 */
#include "F28x_Project.h"
#include "PeriDevices.h"
#include "KiloLib.h"
extern Uint16 AdcaIntFlag;
extern Uint16 AdcaFirtstTime;

Uint16 ADCResult0;
Uint16 ADCResult1;
Uint16 ADCLOW8;
Uint16 ADCHIGH8;

void ConfigureADC(void)
{
    // 配置ADC的中断函数
    EALLOW;
    PieVectTable.ADCA1_INT = &ADCA_INT1_ISR;
    EDIS;

    EALLOW;
    // 时钟时钟预分频至输入时钟的1/4,主频200M,ADC频率最大50M
    AdcaRegs.ADCCTL2.bit.PRESCALE = 6;
    AdcbRegs.ADCCTL2.bit.PRESCALE = 6;
    AdccRegs.ADCCTL2.bit.PRESCALE = 6;
    AdcdRegs.ADCCTL2.bit.PRESCALE = 6;
    // 设定ADC模块工作模式，工作于单端输入，此时精度仅能设定为12Bit
    AdcSetMode(ADC_ADCA, ADC_RESOLUTION_12BIT, ADC_SIGNALMODE_SINGLE);
    AdcSetMode(ADC_ADCB, ADC_RESOLUTION_12BIT, ADC_SIGNALMODE_SINGLE);
    AdcSetMode(ADC_ADCC, ADC_RESOLUTION_12BIT, ADC_SIGNALMODE_SINGLE);
    AdcSetMode(ADC_ADCD, ADC_RESOLUTION_12BIT, ADC_SIGNALMODE_SINGLE);
    // 配置ADC模块的中断为发生在转换结束时(EOC)
    AdcaRegs.ADCCTL1.bit.INTPULSEPOS = 1;
    AdcbRegs.ADCCTL1.bit.INTPULSEPOS = 1;
    AdccRegs.ADCCTL1.bit.INTPULSEPOS = 1;
    AdcdRegs.ADCCTL1.bit.INTPULSEPOS = 1;
    // 给ADC模块上电
    AdcaRegs.ADCCTL1.bit.ADCPWDNZ = 1;
    AdcbRegs.ADCCTL1.bit.ADCPWDNZ = 1;
    AdccRegs.ADCCTL1.bit.ADCPWDNZ = 1;
    AdcdRegs.ADCCTL1.bit.ADCPWDNZ = 1;
    DELAY_US(1000); // 延时等待ADC模块上电完毕
    EDIS;
}

void SetupADCChannel()
{
    Uint16 acqps; // 采样窗口的大小

    // 基于采样精度计算以SYSCLKS为单位的最小采样窗口

    if (ADC_RESOLUTION_12BIT == AdcaRegs.ADCCTL2.bit.RESOLUTION) // 采样精度为12Bit
    {
        acqps = 14; // 设置采样窗口为14个SYSCLK周期，约为75ns
    }
    else // 采样精度为16Bit
    {
        acqps = 63; // 设置采样窗口为63个SYSCLK周期，约为320ns
    }
    //
    // 配置各个采样通道
    EALLOW;
    // ADCA
    AdcaRegs.ADCSOC0CTL.bit.CHSEL = 0;              // 设置SOC0的输入信号通道A0
    AdcaRegs.ADCSOC0CTL.bit.ACQPS = acqps;          // 设置SOC0的采样窗口大小
    AdcaRegs.ADCSOC0CTL.bit.TRIGSEL = 5;            // 设置SOC0的触发源0:仅软件,5:ePWM1,SOC/A
//    AdcaRegs.ADCINTSEL1N2.bit.INT1E = 1;            // 使能INT1中断
//    AdcaRegs.ADCINTSEL1N2.bit.INT1SEL = 0;          // SOC0转换完成后使得INT置位
//    AdcaRegs.ADCINTFLGCLR.bit.ADCINT1 = 1;          // 初始化：清零中断标志位

    AdcaRegs.ADCSOC1CTL.bit.CHSEL = 1;              // 设置SOC1的输入信号通道A1
    AdcaRegs.ADCSOC1CTL.bit.ACQPS = acqps;          // 设置SOC1的采样窗口大小
    AdcaRegs.ADCSOC1CTL.bit.TRIGSEL = 5;            // 设置SOC0的触发源0:仅软件,5:ePWM1,SOC/A
    AdcaRegs.ADCINTSEL1N2.bit.INT1E = 1;            // 使能INT1中断
    AdcaRegs.ADCINTSEL1N2.bit.INT1SEL = 1;          // SOC1转换完成后使得INT置位
    AdcaRegs.ADCINTFLGCLR.bit.ADCINT1 = 1;          // 初始化：清零中断标志位

    // ADCB
    AdcbRegs.ADCSOC0CTL.bit.CHSEL = 0;              // 设置SOC0的输入信号通道B0
    AdcbRegs.ADCSOC0CTL.bit.ACQPS = acqps;          // 设置SOC0的采样窗口大小
    AdcbRegs.ADCSOC0CTL.bit.TRIGSEL = 0;            // 设置SOC0的触发源0:仅软件,5:ePWM1,SOC/A
    AdcbRegs.ADCINTSEL1N2.bit.INT1E = 1;            // 使能INT1中断
    AdcbRegs.ADCINTSEL1N2.bit.INT1SEL = 0;          // SOC0转换完成后使得INT置位
    AdcbRegs.ADCINTFLGCLR.bit.ADCINT1 = 1;          // 初始化：清零中断标志位
    // ADCC
    AdccRegs.ADCSOC0CTL.bit.CHSEL = 0;              // 设置SOC0的输入信号通道C0
    AdccRegs.ADCSOC0CTL.bit.ACQPS = acqps;          // 设置采样窗口大小
    AdccRegs.ADCSOC0CTL.bit.TRIGSEL = 0;            // 设置SOC0的触发源0:仅软件,5:ePWM1,SOC/A
    AdccRegs.ADCINTSEL1N2.bit.INT1E = 1;            // 使能INT1中断
    AdccRegs.ADCINTSEL1N2.bit.INT1SEL = 0;          // SOC0转换完成后使得INT置位
    AdccRegs.ADCINTFLGCLR.bit.ADCINT1 = 1;          // 初始化：清零中断标志位
    // ADCD
    AdcdRegs.ADCSOC0CTL.bit.CHSEL = 0;              // 设置SOC0的输入信号通道D0
    AdcdRegs.ADCSOC0CTL.bit.ACQPS = acqps;          // 设置SOC0的采样窗口大小
    AdcdRegs.ADCSOC0CTL.bit.TRIGSEL = 0;            // 设置SOC0的触发源0:仅软件,5:ePWM1,SOC/A
    AdcdRegs.ADCINTSEL1N2.bit.INT1E = 1;            // 使能INT1中断
    AdcdRegs.ADCINTSEL1N2.bit.INT1SEL = 0;          // SOC0转换完成后使得INT置位
    AdcdRegs.ADCINTFLGCLR.bit.ADCINT1 = 1;          // 初始化：清零中断标志位
    EDIS;
}

interrupt void ADCA_INT1_ISR(void)                  // 在SOC0和SOC1转换完成后通过SCI向串口发送数据
{
    if (AdcaFirtstTime)
    {
        SciaMsg("!ADC!");
        AdcaFirtstTime = 0;
    }
    ADCResult0 = AdcaResultRegs.ADCRESULT0;         // 通道0转换结果
    ADCResult1 = AdcaResultRegs.ADCRESULT1;         // 通道1转换结果

    //数据段开始引导符
    SciaXmit(255);
    SciaXmit(240);
    //SciaMsg("AD");
    //SciaXmit(210);                                   // 0xD2 表示此次信息发送使用了2个通道

    //ADCLOW8 = ADCResult0 & 0xFF;
    //ADCHIGH8 = (ADCResult0>>8) & 0xFF;

    //SciaXmit(192);                                  // 0xC0 表示channel0(好蠢
    SciaXmit(ADCResult0 & 0xFF);
    SciaXmit((ADCResult0>>8) & 0xFF);

    //SciaXmit(193);                                  // 0xC1 表示channel1(好蠢
    SciaXmit(ADCResult1 & 0xFF);
    SciaXmit((ADCResult1>>8) & 0xFF);

    //SciaXmit(238);                                  // 0xEE 表示数据发送结束

    //AdcaIntFlag = 1;
    // 清除中断标志
    AdcaRegs.ADCINTFLGCLR.bit.ADCINT1 = 1;
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
}

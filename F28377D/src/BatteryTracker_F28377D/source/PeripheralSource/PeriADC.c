/*
 * PeriADC.c
 *
 *  Created on: 2023年7月25日
 *      Author: Administrator
 */
#include "F28x_Project.h"
#include "PeriDevices.h"

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
    // 给ADC模块上电
    AdcaRegs.ADCCTL1.bit.ADCPWDNZ = 1;
    AdcbRegs.ADCCTL1.bit.ADCPWDNZ = 1;
    AdccRegs.ADCCTL1.bit.ADCPWDNZ = 1;
    AdcdRegs.ADCCTL1.bit.ADCPWDNZ = 1;
    DELAY_US(1000); // 延时等待ADC模块上电完毕
    EDIS;
}

void SetupADCChannel(Uint16 inputChannel)
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
    AdcaRegs.ADCSOC0CTL.bit.ACQPS = acqps;          // 设置SOC0的采样窗口大小
    AdcaRegs.ADCSOC0CTL.bit.CHSEL = inputChannel;   // 设置SOC0的输入信号通道
    AdcaRegs.ADCSOC0CTL.bit.TRIGSEL = 5;            // 设置SOC0的触发源0:仅软件,5:ePWM1,SOC/A
    AdcaRegs.ADCINTSEL1N2.bit.INT1E = 1;            // 使能INT1中断
    AdcaRegs.ADCINTSEL1N2.bit.INT1SEL = 0;          // SOC0转换完成后使得INT置位
    AdcaRegs.ADCINTFLGCLR.bit.ADCINT1 = 1;          // 初始化：清零中断标志位
    // ADCB
    AdcbRegs.ADCSOC0CTL.bit.ACQPS = acqps;          // 设置SOC0的采样窗口大小
    AdcbRegs.ADCSOC0CTL.bit.CHSEL = inputChannel;   // 设置SOC0的输入信号通道
    AdcbRegs.ADCSOC0CTL.bit.TRIGSEL = 0;            // 设置SOC0的触发源0:仅软件,5:ePWM1,SOC/A
    AdcbRegs.ADCINTSEL1N2.bit.INT1E = 1;            // 使能INT1中断
    AdcbRegs.ADCINTSEL1N2.bit.INT1SEL = 0;          // SOC0转换完成后使得INT置位
    AdcbRegs.ADCINTFLGCLR.bit.ADCINT1 = 1;          // 初始化：清零中断标志位
    // ADCC
    AdccRegs.ADCSOC0CTL.bit.ACQPS = acqps;          // 设置采样窗口大小
    AdccRegs.ADCSOC0CTL.bit.CHSEL = inputChannel;   // 设置SOC0的输入信号通道
    AdccRegs.ADCSOC0CTL.bit.TRIGSEL = 0;            // 设置SOC0的触发源0:仅软件,5:ePWM1,SOC/A
    AdccRegs.ADCINTSEL1N2.bit.INT1E = 1;            // 使能INT1中断
    AdccRegs.ADCINTSEL1N2.bit.INT1SEL = 0;          // SOC0转换完成后使得INT置位
    AdccRegs.ADCINTFLGCLR.bit.ADCINT1 = 1;          // 初始化：清零中断标志位
    // ADCD
    AdcdRegs.ADCSOC0CTL.bit.ACQPS = acqps;          // 设置SOC0的采样窗口大小
    AdcdRegs.ADCSOC0CTL.bit.CHSEL = inputChannel;   // 设置SOC0的输入信号通道
    AdcdRegs.ADCSOC0CTL.bit.TRIGSEL = 0;            // 设置SOC0的触发源0:仅软件,5:ePWM1,SOC/A
    AdcdRegs.ADCINTSEL1N2.bit.INT1E = 1;            // 使能INT1中断
    AdcdRegs.ADCINTSEL1N2.bit.INT1SEL = 0;          // SOC0转换完成后使得INT置位
    AdcdRegs.ADCINTFLGCLR.bit.ADCINT1 = 1;          // 初始化：清零中断标志位
    EDIS;
}

interrupt void ADCA_INT1_ISR(void)
{
    char ADCLowChar[] = "00000";
    char ADCHighChar[] = "00000";
    ADCResult0 = AdcaResultRegs.ADCRESULT0;         // 转换结果
    ADCReLow8 = ADCResult0 & 0xFF                   // 获取低八位
    ADCReHigh8 = (ADCResult0>>8) & 0xFF             // 获取高八位(取决于转换模式)
    Int2Str(ADCReLow8,ADCLowChar);                  // 低八位数字转化为字符串
    Int2Str(ADCReHigh8,ADCHighChar);                // 高八位数字转化为字符串
    SCIAXmit('!');
    SCIAMsg(ADCReLow8);
    SCIAMsg(ADCReHigh8);
    SCIAXmit('\n');
}

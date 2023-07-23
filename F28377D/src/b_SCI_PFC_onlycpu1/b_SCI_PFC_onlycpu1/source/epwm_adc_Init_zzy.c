/*
 * epwm_adc_Init_zzy.c
 *
 *  Created on: 2017年8月21日
 *      Author: zzy
 */
#include "F28x_Project.h"
#include "epwm_adc_Init_zzy.h"
#include "gpio_exinterrupt_Init_zzy.h"
extern interrupt void adc_isr(void);
/****************************************************************************
*功    能：ConfigureADC
*入口参数：无
*出口参数：无
*说    明：Write ADC configurations and power up the ADC for both ADC A and ADC B
****************************************************************************/
void ConfigureADC(void)
{
    // Map ISR functions
    EALLOW;
    PieVectTable.ADCA1_INT = &adc_isr; //function for ADCA interrupt 1
    EDIS;
    EALLOW;
    //write configurations
    AdcaRegs.ADCCTL2.bit.PRESCALE = 6; //set ADCCLK divider to /4,主频200M，AD频率最大50M
    AdcbRegs.ADCCTL2.bit.PRESCALE = 6; //set ADCCLK divider to /4
    AdccRegs.ADCCTL2.bit.PRESCALE = 6; //set ADCCLK divider to /4
    AdcdRegs.ADCCTL2.bit.PRESCALE = 6; //set ADCCLK divider to /4
    AdcSetMode(ADC_ADCA, ADC_RESOLUTION_12BIT, ADC_SIGNALMODE_SINGLE);//如果采样频率高于1M，只能使用12位模式
    AdcSetMode(ADC_ADCB, ADC_RESOLUTION_12BIT, ADC_SIGNALMODE_SINGLE);
    AdcSetMode(ADC_ADCC, ADC_RESOLUTION_12BIT, ADC_SIGNALMODE_SINGLE);
    AdcSetMode(ADC_ADCD, ADC_RESOLUTION_12BIT, ADC_SIGNALMODE_SINGLE);
    //Set pulse positions to late
    AdcaRegs.ADCCTL1.bit.INTPULSEPOS = 1;
    AdcbRegs.ADCCTL1.bit.INTPULSEPOS = 1;
    AdccRegs.ADCCTL1.bit.INTPULSEPOS = 1;
    AdcdRegs.ADCCTL1.bit.INTPULSEPOS = 1;
    //power up the ADC
    AdcaRegs.ADCCTL1.bit.ADCPWDNZ = 1;
    AdcbRegs.ADCCTL1.bit.ADCPWDNZ = 1;
    AdccRegs.ADCCTL1.bit.ADCPWDNZ = 1;
    AdcdRegs.ADCCTL1.bit.ADCPWDNZ = 1;
    //delay for 1ms to allow ADC time to power up
    DELAY_US(1000);
    EDIS;
}
//
//
//
/****************************************************************************
*功    能：ConfigureEPWM1
*入口参数：无
*出口参数：无
*说    明：增减计数，在脉冲高电平及低电平中间进行采样,控制采样
****************************************************************************/
void ConfigureEPWM1(void)
{
    InitEPwm1Gpio();                             //初始化GPIO，使用GPIO145、GPIO146。注意：28377有两个管脚可以产生pwm，此处注意配置
    EPwm1Regs.TBPRD = 1000;                      //100k=100e6/（2*1000）
    // Set Compare values
    EPwm1Regs.CMPA.bit.CMPA = 1000-250;                 //设定A通道占空比
    EPwm1Regs.TBPHS.bit.TBPHS = 0;                 //将相位寄存器清零
    EPwm1Regs.TBCTR = 0;                            //将时间基准计数器清零
    // Setup TBCLK
    EPwm1Regs.TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN;      //设定为增计数模式 增减计数则为对称
    EPwm1Regs.TBCTL.bit.PHSEN = TB_DISABLE;         //禁止相位控制
    EPwm1Regs.TBCTL.bit.HSPCLKDIV =0x00 ;           //不分频
    EPwm1Regs.TBCTL.bit.CLKDIV = 0x00;              //不分频
    // Set actions
    EPwm1Regs.AQCTLA.bit.CAU = AQ_SET;              // Set PWM3A on Zero
    EPwm1Regs.AQCTLA.bit.CAD = AQ_CLEAR;
    EPwm1Regs.AQCTLB.bit.CAU = AQ_CLEAR;            // Set PWM3A on Zero
    EPwm1Regs.AQCTLB.bit.CAD = AQ_SET;    //设定死区
    EPwm1Regs.DBCTL.bit.OUT_MODE = DB_FULL_ENABLE;            //设为两边都有死区
    EPwm1Regs.DBCTL.bit.POLSEL = DB_ACTV_HIC;                //必须这么设置，否则两边都加5%。则失效
//    EPwm1Regs.DBRED.bit.DBRED = 2;                                  //死区为10%
//    EPwm1Regs.DBFED.bit.DBFED = 2;
//    EALLOW;
//    // Assumes ePWM clock is already enabled
//    EPwm1Regs.ETSEL.bit.SOCAEN    = 1;    // Disable SOC on A group
//    EPwm1Regs.ETSEL.bit.SOCASEL    = 1;   // 3：一个周期采样两次，只适用于增减计数。2:一次 4 一次
//    EPwm1Regs.ETPS.bit.SOCAPRD = 1;       // Generate pulse on 1st event
//    EDIS;
}
/****************************************************************************
*功    能：ConfigureEPWM5 - Configure EPWM SOC and compare values
*入口参数：无
*出口参数：无
*说    明：产生pwm控制mos
****************************************************************************/
void ConfigureEPWM6(void)
{
    InitEPwm6Gpio();                             //初始化GPIO，使用GPIO147、GPIO148。注意：28377有两个管脚可以产生pwm，此处注意配置
    EPwm6Regs.TBPRD = 1000;                      //100k=100e6/（2*1000）
    // Set Compare values
    EPwm6Regs.CMPA.bit.CMPA = 1000-250;                 //设定A通道占空比
    EPwm6Regs.TBPHS.bit.TBPHS = 0;                 //将相位寄存器清零
    EPwm6Regs.TBCTR = 0;                            //将时间基准计数器清零
    // Setup TBCLK
    EPwm6Regs.TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN;      //设定为增计数模式 增减计数则为对称
    EPwm6Regs.TBCTL.bit.PHSEN = TB_DISABLE;         //禁止相位控制
    EPwm6Regs.TBCTL.bit.HSPCLKDIV =0x00 ;           //不分频
    EPwm6Regs.TBCTL.bit.CLKDIV = 0x00;              //不分频
    // Set actions
    EPwm6Regs.AQCTLA.bit.CAU = AQ_SET;              // Set PWM3A on Zero
    EPwm6Regs.AQCTLA.bit.CAD = AQ_CLEAR;
    EPwm6Regs.AQCTLB.bit.CAU = AQ_CLEAR;            // Set PWM3B on Zero
    EPwm6Regs.AQCTLB.bit.CAD = AQ_SET;    //设定死区
    EPwm6Regs.DBCTL.bit.OUT_MODE = DB_FULL_ENABLE;            //设为两边都有死区
    EPwm6Regs.DBCTL.bit.POLSEL = DB_ACTV_HIC;                //必须这么设置，否则两边都加5%。则失效
    //    EPwm5Regs.DBRED.bit.DBRED = 100;                                  //死区为10%
    //    EPwm5Regs.DBFED.bit.DBFED = 100;
    EALLOW;
    // Assumes ePWM clock is already enabled
    EPwm6Regs.ETSEL.bit.SOCAEN    = 1;    // Disable SOC on A group
    EPwm6Regs.ETSEL.bit.SOCASEL    = 2;   // 001: Enable event time-base counter equal to zero. (TBCTR = 0x00)
    //010: Enable event time-base counter equal to period (TBCTR =TBPRD)
    //011: Enable event time-base counter equal to zero or period (TBCTR = 0x00 or TBCTR = TBPRD). This mode is useful in updown count mode.
    EPwm6Regs.ETPS.bit.SOCAPRD = 1;       // Generate pulse on 1st event
    EDIS;
}
/****************************************************************************
*功    能：SetupADCEpwm - Setup ADC EPWM acquisition window
*入口参数：无
*出口参数：无
*说    明：由于A与B均使用0通道，故用一个初始化函数
****************************************************************************/
void SetupADCEpwm(Uint16 channel)
{
    Uint16 acqps;
    //
    //determine minimum acquisition window (in SYSCLKS) based on resolution
    //
    if(ADC_RESOLUTION_12BIT == AdcaRegs.ADCCTL2.bit.RESOLUTION)
    {
        acqps = 14; //75ns
    }
    else //resolution is 16-bit
    {
        acqps = 63; //320ns
    }
    //
    //Select the channels to convert and end of conversion flag
    //ADCA
    //
    EALLOW;
    AdcaRegs.ADCSOC0CTL.bit.CHSEL = channel;  //SOC0 will convert pin A0
    AdcaRegs.ADCSOC0CTL.bit.ACQPS = acqps; //sample window is 100 SYSCLK cycles
    AdcaRegs.ADCSOC0CTL.bit.TRIGSEL = 15; //5:trigger on ePWM1 SOCA/C  0D:trigger on ePWM1 SOCA/C
    AdcaRegs.ADCINTSEL1N2.bit.INT1SEL = 0; //end of SOC0 will set INT1 flag
    AdcaRegs.ADCINTSEL1N2.bit.INT1E = 1;   //enable INT1 flag
    AdcaRegs.ADCINTFLGCLR.bit.ADCINT1 = 1; //make sure INT1 flag is cleared
    //
    //ADCB
    //
    AdcbRegs.ADCSOC0CTL.bit.CHSEL = channel;  //SOC0 will convert pin B0
    AdcbRegs.ADCSOC0CTL.bit.ACQPS = acqps; //sample window is 100 SYSCLK cycles
    AdcbRegs.ADCSOC0CTL.bit.TRIGSEL = 15; //5:trigger on ePWM1 SOCA/C  0D:trigger on ePWM1 SOCA/C
    AdcbRegs.ADCINTSEL1N2.bit.INT1SEL = 0; //end of SOC1 will set INT1 flag
    AdcbRegs.ADCINTSEL1N2.bit.INT1E = 1;   //enable INT1 flag
    AdcbRegs.ADCINTFLGCLR.bit.ADCINT1 = 1; //make sure INT1 flag is cleared
    //
    //ADCD
    //
    AdcdRegs.ADCSOC0CTL.bit.CHSEL = channel;  //SOC0 will convert pin B0
    AdcdRegs.ADCSOC0CTL.bit.ACQPS = acqps; //sample window is 100 SYSCLK cycles
    AdcdRegs.ADCSOC0CTL.bit.TRIGSEL = 15; //5:trigger on ePWM1 SOCA/C  0D:trigger on ePWM1 SOCA/C
    AdcdRegs.ADCINTSEL1N2.bit.INT1SEL = 0; //end of SOC1 will set INT1 flag
    AdcdRegs.ADCINTSEL1N2.bit.INT1E = 1;   //enable INT1 flag
    AdcdRegs.ADCINTFLGCLR.bit.ADCINT1 = 1; //make sure INT1 flag is cleared
    //
    //ADCC  只有C2
    //
    AdccRegs.ADCSOC0CTL.bit.CHSEL = 2;  //SOC0 will convert pin B0
    AdccRegs.ADCSOC0CTL.bit.ACQPS = acqps; //sample window is 100 SYSCLK cycles
    AdccRegs.ADCSOC0CTL.bit.TRIGSEL = 15; //5:trigger on ePWM1 SOCA/C  0D:trigger on ePWM1 SOCA/C
    AdccRegs.ADCINTSEL1N2.bit.INT1SEL = 0; //end of SOC1 will set INT1 flag
    AdccRegs.ADCINTSEL1N2.bit.INT1E = 1;   //enable INT1 flag
    AdccRegs.ADCINTFLGCLR.bit.ADCINT1 = 1; //make sure INT1 flag is cleared
    EDIS;
}

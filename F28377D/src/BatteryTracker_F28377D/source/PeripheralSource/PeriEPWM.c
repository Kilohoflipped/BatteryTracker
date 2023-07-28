/*
 * PeriEPWM.c
 *
 *  Created on: 2023年7月26日
 *      Author: Administrator
 */

#include "F28x_Project.h"
#include "PeriEPWM.h"

void ConfigureEPWM1(void)
{
    InitEPwm1Gpio();        // 初始化GPIO，使用GPIO145、GPIO146。注意：28377有两个管脚可以产生pwm，此处注意配置
    EPwm1Regs.TBPRD = 1000; // 100k=100e6/（2*1000）
    // Set Compare values
    EPwm1Regs.CMPA.bit.CMPA = 1000 - 250; // 设定A通道占空比
    EPwm1Regs.TBPHS.bit.TBPHS = 0;        // 将相位寄存器清零
    EPwm1Regs.TBCTR = 0;                  // 将时间基准计数器清零
    // Setup TBCLK
    EPwm1Regs.TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN; // 设定为增计数模式 增减计数则为对称
    EPwm1Regs.TBCTL.bit.PHSEN = TB_DISABLE;        // 禁止相位控制
    EPwm1Regs.TBCTL.bit.HSPCLKDIV = 0x00;          // 不分频
    EPwm1Regs.TBCTL.bit.CLKDIV = 0x00;             // 不分频
    // Set actions
    EPwm1Regs.AQCTLA.bit.CAU = AQ_SET; // Set PWM3A on Zero
    EPwm1Regs.AQCTLA.bit.CAD = AQ_CLEAR;
    EPwm1Regs.AQCTLB.bit.CAU = AQ_CLEAR;           // Set PWM3A on Zero
    EPwm1Regs.AQCTLB.bit.CAD = AQ_SET;             // 设定死区
    EPwm1Regs.DBCTL.bit.OUT_MODE = DB_FULL_ENABLE; // 设为两边都有死区
    EPwm1Regs.DBCTL.bit.POLSEL = DB_ACTV_HIC;      // 必须这么设置，否则两边都加5%。则失效
    //    EPwm1Regs.DBRED.bit.DBRED = 2;                                  //死区为10%
    //    EPwm1Regs.DBFED.bit.DBFED = 2;
    //    EALLOW;
    //    // Assumes ePWM clock is already enabled
    //    EPwm1Regs.ETSEL.bit.SOCAEN    = 1;    // Disable SOC on A group
    //    EPwm1Regs.ETSEL.bit.SOCASEL    = 1;   // 3：一个周期采样两次，只适用于增减计数。2:一次 4 一次
    //    EPwm1Regs.ETPS.bit.SOCAPRD = 1;       // Generate pulse on 1st event
    //    EDIS;
}

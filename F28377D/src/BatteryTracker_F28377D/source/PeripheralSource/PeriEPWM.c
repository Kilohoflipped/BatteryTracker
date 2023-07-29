/*
 * PeriEPWM.c
 *
 *  Created on: 2023年7月26日
 *      Author: Administrator
 */

#include "F28x_Project.h"
#include "PeriEPWM.h"
#include <math.h>

void ConfigureEPWM1(void)
{
    // 初始化GPIO，使用GPIO145、GPIO146，注意：28377有两对管脚可以产生PWM，此处注意配置
    InitEPwm1Gpio();
    EALLOW;
    // 设置TB周期计数器的值
    // 对于增减计数模式来说：F(EPWM)=PLLSYSCLK/2/TBPRD
    EPwm1Regs.TBPRD = 1000;                         // 设定EPWM频率为100k=200*1000*1000/2/1000
    // 设置比较寄存器的值
    EPwm1Regs.CMPA.bit.CMPA = floor(0.75*1000);     // 设定A通道占空比
    EPwm1Regs.CMPB.bit.CMPB = floor(0.75*1000);     // 设定B通道占空比
    EPwm1Regs.TBPHS.bit.TBPHS = 0;                  // 将相位寄存器清零
    EPwm1Regs.TBCTR = 0;                            // 将时间基准计数器清零
    // 设置EPWM模块的计数方式
    EPwm1Regs.TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN;  // 设定为增减计数模式，产生对称的EPMW波
    EPwm1Regs.TBCTL.bit.PHSEN = TB_DISABLE;         // 禁止相位控制
    EPwm1Regs.TBCTL.bit.HSPCLKDIV = 0x00;           // 不分频
    EPwm1Regs.TBCTL.bit.CLKDIV = 0x00;              // 不分频
    // 设置EPWM动作方式
    EPwm1Regs.AQCTLA.bit.CAU = AQ_SET;              // 置位
    EPwm1Regs.AQCTLA.bit.CAD = AQ_CLEAR;
    EPwm1Regs.AQCTLB.bit.CBU = AQ_CLEAR;            // 清零
    EPwm1Regs.AQCTLB.bit.CBD = AQ_SET;
    // 设定死区发生器
    EPwm1Regs.DBCTL.bit.OUT_MODE = DB_FULL_ENABLE;  // 设为两边都有死区
    EPwm1Regs.DBCTL.bit.POLSEL = DB_ACTV_HIC;       // 必须这么设置，否则两边都加5%。则失效
    EPwm1Regs.DBRED.bit.DBRED = 2;                  // 死区为10%
    EPwm1Regs.DBFED.bit.DBFED = 2;
    // 假设EPWM时钟已经被使能
    EPwm1Regs.ETSEL.bit.SOCAEN = 1;                 // 是否使能ADC启动脉冲SOC/A
    EPwm1Regs.ETSEL.bit.SOCASEL = ET_CTR_ZERO;      // ET事件选择
    EPwm1Regs.ETPS.bit.SOCAPRD = ET_1ST;            // 每一次事件都产生一个采样脉冲
    EPwm1Regs.ETSEL.bit.SOCBEN = 0;                 // 是否使能ADC启动脉冲SOC/B
    EPwm1Regs.ETSEL.bit.SOCBSEL = ET_CTR_ZERO;      // ET事件选择
    EPwm1Regs.ETPS.bit.SOCBPRD = ET_1ST;            // 每一次事件都产生一个采样脉冲
    EDIS;
}

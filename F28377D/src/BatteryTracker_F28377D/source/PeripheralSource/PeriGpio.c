/*
 * PeriGpio.c
 *
 *  Created on: 2023年8月1日
 *      Author: Administrator
 */

#include "F28x_Project.h"
#include "PeriDevices.h"

void ConfigureGpio()
{
    EALLOW;
    GPIO_SetupPinMux(92, GPIO_MUX_CPU1, 0);                 //92脚输出3.3V信号
    GPIO_SetupPinOptions(92, GPIO_OUTPUT, GPIO_PUSHPULL);
    GPIO_SetupPinMux(145, GPIO_MUX_CPU1, 1);                //145脚输出EPWM信号
    GPIO_SetupPinOptions(145, GPIO_OUTPUT, GPIO_ASYNC);

    ClkCfgRegs.CLKSRCCTL3.bit.XCLKOUTSEL = 0;               //使用PLLSysClk
    ClkCfgRegs.XCLKOUTDIVSEL.bit.XCLKOUTDIV = 3;            // 使用8分频
    GPIO_SetupPinMux(73, GPIO_MUX_CPU1, 3);                 // 初始化73脚的功能为Xclkout
    GPIO_SetupPinOptions(73, GPIO_OUTPUT, GPIO_ASYNC);
    EDIS;
}

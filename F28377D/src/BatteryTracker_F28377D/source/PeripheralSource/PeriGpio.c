/*
 * PeriGpio.c
 *
 *  Created on: 2023��8��1��
 *      Author: Administrator
 */

#include "F28x_Project.h"
#include "PeriDevices.h"

void ConfigureGpio()
{
    EALLOW;
    GPIO_SetupPinMux(92, GPIO_MUX_CPU1, 0);                 //92�����3.3V�ź�
    GPIO_SetupPinOptions(92, GPIO_OUTPUT, GPIO_PUSHPULL);
    GPIO_SetupPinMux(145, GPIO_MUX_CPU1, 1);                //145�����EPWM�ź�
    GPIO_SetupPinOptions(145, GPIO_OUTPUT, GPIO_ASYNC);

    ClkCfgRegs.CLKSRCCTL3.bit.XCLKOUTSEL = 0;               //ʹ��PLLSysClk
    ClkCfgRegs.XCLKOUTDIVSEL.bit.XCLKOUTDIV = 3;            // ʹ��8��Ƶ
    GPIO_SetupPinMux(73, GPIO_MUX_CPU1, 3);                 // ��ʼ��73�ŵĹ���ΪXclkout
    GPIO_SetupPinOptions(73, GPIO_OUTPUT, GPIO_ASYNC);
    EDIS;
}

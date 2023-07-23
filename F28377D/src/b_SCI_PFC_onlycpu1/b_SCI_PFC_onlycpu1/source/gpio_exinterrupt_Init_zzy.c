/*
 * gpio_exinterrupt_Init_zzy.c
 *
 *  Created on: 2017年8月21日
 *      Author: zzy
 */
#include "F28x_Project.h"
#include "epwm_adc_Init_zzy.h"
#include "gpio_exinterrupt_Init_zzy.h"
extern interrupt void xint1_isr(void);
extern interrupt void xint2_isr(void);
/****************************************************************************
*功    能：led,key初始化
*入口参数：无
*出口参数：无
*说    明：led1 LED2作为一组pwm管脚，触发AD采样，故不能使用其功能
****************************************************************************/
void ledkeyInit(void)
{
    //配置GPIO
    EALLOW;
    //输出
//    GpioCtrlRegs.GPEGMUX2.bit.GPIO145=0;                      //将管脚设置为GPIO
//    GpioCtrlRegs.GPEGMUX2.bit.GPIO146=0;
//    GpioCtrlRegs.GPEGMUX2.bit.GPIO147=0;
//    GpioCtrlRegs.GPEGMUX2.bit.GPIO148=0;
    GpioCtrlRegs.GPEGMUX2.bit.GPIO149=0;
    GpioCtrlRegs.GPEGMUX2.bit.GPIO150=0;
    GpioCtrlRegs.GPEGMUX2.bit.GPIO151=0;
    GpioCtrlRegs.GPEGMUX2.bit.GPIO152=0;
//    GpioCtrlRegs.GPEDIR.bit.GPIO145=1;                       //将管脚设置为输出
//    GpioCtrlRegs.GPEDIR.bit.GPIO146=1;
//    GpioCtrlRegs.GPEDIR.bit.GPIO147=1;
//    GpioCtrlRegs.GPEDIR.bit.GPIO148=1;
    GpioCtrlRegs.GPEDIR.bit.GPIO149=1;
    GpioCtrlRegs.GPEDIR.bit.GPIO150=1;
    GpioCtrlRegs.GPEDIR.bit.GPIO151=1;
    GpioCtrlRegs.GPEDIR.bit.GPIO152=1;
    EDIS;
    //初始均关闭
//    LED1_OFF;
//    LED2_OFF;
//    LED3_OFF;
//    LED4_OFF;
    LED5_OFF;
    LED6_OFF;
    LED7_OFF;
    LED8_OFF;
}
/****************************************************************************
*功    能：外部中断初始化
*入口参数：无
*出口参数：无
*说    明：无
****************************************************************************/
void exInterruptInit(void)
{
    //
    // Interrupts that are used in this example are re-mapped to
    // ISR functions found within this file.
    //
    EALLOW;  // This is needed to write to EALLOW protected registers
    PieVectTable.XINT1_INT = &xint1_isr;
    PieVectTable.XINT2_INT = &xint2_isr;
    EDIS;    // This is needed to disable write to EALLOW protected registers
    //
    // GPIO2 and GPIO3 are inputs
    //
    EALLOW;
    GpioCtrlRegs.GPAMUX1.bit.GPIO2 = 0;         // GPIO
    GpioCtrlRegs.GPADIR.bit.GPIO2 = 0;          // input
    GpioCtrlRegs.GPAQSEL1.bit.GPIO2 = 0;        // XINT1 Qual using 6 samples
    GpioCtrlRegs.GPAMUX1.bit.GPIO3 = 0;         // GPIO
    GpioCtrlRegs.GPADIR.bit.GPIO3 = 0;          // input
    GpioCtrlRegs.GPAQSEL1.bit.GPIO3 = 0;        // XINT2 Qual using 6 samples
    GpioCtrlRegs.GPACTRL.bit.QUALPRD0 = 0;   // Each sampling window
    // is 510*SYSCLKOUT
    EDIS;
    //
    // GPIO2 is XINT1, GPIO3 is XINT2
    //
    GPIO_SetupXINT1Gpio(2);
    GPIO_SetupXINT2Gpio(3);
    //
    // Configure XINT1
    //
    XintRegs.XINT1CR.bit.POLARITY = 1;          // 0:Falling edge interrupt,  1:Rising edge interrupt
    XintRegs.XINT2CR.bit.POLARITY = 1;          //
    //
    // Enable XINT1 and XINT2
    //
    XintRegs.XINT1CR.bit.ENABLE = 1;            // Enable XINT1
    XintRegs.XINT2CR.bit.ENABLE = 1;            // Enable XINT2
}

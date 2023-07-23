/*
 * gpio_exinterrupt_Init_zzy.h
 *
 *  Created on: 2017年8月21日
 *      Author: zzy
 */

#ifndef INCLUDE_GPIO_EXINTERRUPT_INIT_ZZY_H_
#define INCLUDE_GPIO_EXINTERRUPT_INIT_ZZY_H_

//关LED,低电平
#define LED1_OFF  GpioDataRegs.GPECLEAR.bit.GPIO145 = 1
#define LED2_OFF  GpioDataRegs.GPECLEAR.bit.GPIO146 = 1
#define LED3_OFF  GpioDataRegs.GPECLEAR.bit.GPIO147 = 1
#define LED4_OFF  GpioDataRegs.GPECLEAR.bit.GPIO148 = 1
#define LED5_OFF  GpioDataRegs.GPECLEAR.bit.GPIO149 = 1
#define LED6_OFF  GpioDataRegs.GPECLEAR.bit.GPIO150 = 1
#define LED7_OFF  GpioDataRegs.GPECLEAR.bit.GPIO151 = 1
#define LED8_OFF  GpioDataRegs.GPECLEAR.bit.GPIO152 = 1


////开LED，高电平
#define LED1_ON  GpioDataRegs.GPESET.bit.GPIO145 = 1
#define LED2_ON  GpioDataRegs.GPESET.bit.GPIO146 = 1
#define LED3_ON  GpioDataRegs.GPESET.bit.GPIO147 = 1
#define LED4_ON  GpioDataRegs.GPESET.bit.GPIO148 = 1
#define LED5_ON  GpioDataRegs.GPESET.bit.GPIO149 = 1
#define LED6_ON  GpioDataRegs.GPESET.bit.GPIO150 = 1
#define LED7_ON  GpioDataRegs.GPESET.bit.GPIO151 = 1
#define LED8_ON  GpioDataRegs.GPESET.bit.GPIO152 = 1
////翻转电平
#define LED1_ON_OFF  GpioDataRegs.GPETOGGLE.bit.GPIO145 = 1
#define LED2_ON_OFF  GpioDataRegs.GPETOGGLE.bit.GPIO146 = 1
#define LED3_ON_OFF  GpioDataRegs.GPETOGGLE.bit.GPIO147 = 1
#define LED4_ON_OFF  GpioDataRegs.GPETOGGLE.bit.GPIO148 = 1
#define LED5_ON_OFF  GpioDataRegs.GPETOGGLE.bit.GPIO149 = 1
#define LED6_ON_OFF  GpioDataRegs.GPETOGGLE.bit.GPIO150 = 1
#define LED7_ON_OFF  GpioDataRegs.GPETOGGLE.bit.GPIO151 = 1
#define LED8_ON_OFF  GpioDataRegs.GPETOGGLE.bit.GPIO152 = 1




extern void ledkeyInit(void);
extern void exInterruptInit(void);



#endif /* INCLUDE_GPIO_EXINTERRUPT_INIT_ZZY_H_ */

/*
 * sci_zzy.c
 *
 *  Created on: 2019年2月20日
 *      Author: zzy
 */
#include "F28x_Project.h"
#include "sci_zzy.h"
//
//  scia_echoback_init - Test 1,SCIA  DLB, 8-bit word, baud rate 0x000F,
//                       default, 1 STOP bit, no parity
//
void ConfigureSciA()
{
    //
    // For this example, only init the pins for the SCI-A port.
    //  GPIO_SetupPinMux() - Sets the GPxMUX1/2 and GPyMUX1/2 register bits
    //  GPIO_SetupPinOptions() - Sets the direction and configuration of the GPIOS
    // These functions are found in the F2837xD_Gpio.c file.
    //
    GPIO_SetupPinMux(28, GPIO_MUX_CPU1, 1);
    GPIO_SetupPinOptions(28, GPIO_INPUT, GPIO_PUSHPULL);
    GPIO_SetupPinMux(29, GPIO_MUX_CPU1, 1);
    GPIO_SetupPinOptions(29, GPIO_OUTPUT, GPIO_ASYNC);
    //
    // Note: Clocks were turned on to the SCIA peripheral
    // in the InitSysCtrl() function
    //
    SciaRegs.SCICCR.all = 0x0007;   // 1 stop bit,  No loopback
    // No parity,8 char bits,
    // async mode, idle-line protocol
    SciaRegs.SCICTL1.all = 0x0003;  // enable TX, RX, internal SCICLK,
    // Disable RX ERR, SLEEP, TXWAKE
    SciaRegs.SCICTL2.all = 0x0003;
    SciaRegs.SCICTL2.bit.TXINTENA = 1;
    SciaRegs.SCICTL2.bit.RXBKINTENA = 1;
    //
    // SCIA at 9600 baud
    // @LSPCLK = 50 MHz (200 MHz SYSCLK) HBAUD = 0x02 and LBAUD = 0x8B.
    // @LSPCLK = 30 MHz (120 MHz SYSCLK) HBAUD = 0x01 and LBAUD = 0x86.
    //
    SciaRegs.SCIHBAUD.all = 0x0002;
    SciaRegs.SCILBAUD.all = 0x008B;
    SciaRegs.SCICTL1.all = 0x0023;  // Relinquish SCI from Reset
}
//
// scia_xmit - Transmit a character from the SCI
//
void scia_xmit(int a)
{
    while (SciaRegs.SCIFFTX.bit.TXFFST != 0) {}
    SciaRegs.SCITXBUF.all =a;
}
//
// scia_msg - Transmit message via SCIA
//
void scia_msg(char * msg)
{
    int i;
    i = 0;
    while(msg[i] != '\0')
    {
        scia_xmit(msg[i]);
        i++;
    }
}
//
// scia_fifo_init - Initialize the SCI FIFO
//
void scia_fifo_init()
{
    SciaRegs.SCIFFTX.all = 0xE040;
    SciaRegs.SCIFFRX.all = 0x2044;
    SciaRegs.SCIFFCT.all = 0x0;
}
/****************************************************************************
*功    能：进制转换函数--把长整形数转换为任意进制的字符串的函数
*入口参数：g整数位位数（符号占一位） l小数位位数
*出口参数：无
*说   明：
****************************************************************************/
void ftoa(char *p,float Val)
{
    float Temp;
    char i,Sign;
    float n;
    if(Val<0)
    {
        Sign=1;
        Val=-Val;
    }
    else
    {
        Sign=0;
    }
    if(Val>=1000)
    {
        Temp=1000;
    }
    else if(Val>=100)
    {
        Temp=100;
    }
    else if(Val>=10)
    {
        Temp=10;
    }
    else
    {
        Temp=1;
    }
    if(Sign)
    {
        *p++='-';
    }
    for(i=0; i<5; i++)   //5指的是位数比如123.45 不包含小数点
    {
        n=Val/Temp;
        *p++=(Uint16)n+0x30;
        Val-=(Uint16)n*Temp;
        if(Temp==1)
        {
            *p++='.';
        }
        Temp=Temp/10;
        if((i==2)&(Sign))
        {
            break;
        }
    }
    *p=0;
}//
//
// End of file
//

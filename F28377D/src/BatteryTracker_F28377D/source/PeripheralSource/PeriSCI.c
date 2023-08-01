/*
 * PeriSCI.c
 *
 *  Created on: 2023年7月28日
 *      Author: Administrator
 */

#include "F28x_Project.h"
#include "PeriDevices.h"
#include "KiloLib.h"
#include <math.h>

void ConfigureSciA()
{
    Uint32 BaudInf[5] = {0,0,0,0,0};                    // 用于计算波特率信息,0:输出波特率,1:HBAUD,2:LBAUD,3:误差率,4:波特率是否过高

    // 在这段代码中，只有SCI-A模块的引脚被初始化了
    // GPIO_SetupPinMux() - Sets the GPxMUX1/2 and GPyMUX1/2 register bits
    // GPIO_SetupPinOptions() - Sets the direction and configuration of the GPIOS

    GPIO_SetupPinMux(64, GPIO_MUX_CPU1, 6);             // 初始化64脚的功能为SCI-A模块RX接收引脚
    GPIO_SetupPinOptions(64, GPIO_INPUT, GPIO_PUSHPULL);
    GPIO_SetupPinMux(65, GPIO_MUX_CPU1, 6);             // 初始化65脚的功能为SCI-A模块TX发送引脚
    GPIO_SetupPinOptions(65, GPIO_OUTPUT, GPIO_ASYNC);

    // 注意: 在InitSysCtrl()函数中，SCI-A模块的时钟已经被打开
    SciaRegs.SCICCR.all = 0x0007;
    SciaRegs.SCICCR.bit.SCICHAR = 7;                    // 通信消息字符长度为8Bit
    SciaRegs.SCICCR.bit.ADDRIDLE_MODE = 0;              // 选择空闲线模式协议
    SciaRegs.SCICCR.bit.LOOPBKENA = 0;                  // 是否使能禁用环回测试模式，RX引脚和TX引脚不内部连接
    SciaRegs.SCICCR.bit.PARITY = 0;                     // 0:使用奇校验 1:使用偶校验
    SciaRegs.SCICCR.bit.PARITYENA = 0;                  // 是否使用奇偶校验
    SciaRegs.SCICCR.bit.STOPBITS = 0;                   // 0:一个停止位 1:两个停止位

    SciaRegs.SCICTL1.all = 0x0003;
    SciaRegs.SCICTL1.bit.RXENA = 1;                     // 是否使能SCI-A模块RX接收功能
    SciaRegs.SCICTL1.bit.TXENA = 1;                     // 是否使能SCI-A模块TX发送功能
    SciaRegs.SCICTL1.bit.SLEEP = 0;                     // 是否使能SCI-A模块休眠模式
    SciaRegs.SCICTL1.bit.TXWAKE = 0;                    // SCI发送器唤醒方式选择，是否启用数字唤醒功能

    SciaRegs.SCICTL2.all = 0x0003;
    SciaRegs.SCICTL2.bit.TXINTENA = 1;                  // 是否使能发送就绪中断
    SciaRegs.SCICTL2.bit.RXBKINTENA = 1;                // 是否使能接收就绪中断

    // 设置SCIA的波特率
    // @LSPCLK = 50 MHz (200 MHz SYSCLK)
    // HBAUD = 0x02 and LBAUD = 0x8B.
    BaudCalculate(9600,BaudInf);
    SciaRegs.SCIHBAUD.bit.BAUD = (int)BaudInf[1];            // 波特率选择器高八位
    SciaRegs.SCILBAUD.bit.BAUD = (int)BaudInf[2];            // 波特率选择器低八位
    SciaRegs.SCIHBAUD.all = 0x0002;
    SciaRegs.SCILBAUD.all = 0x008B;
    SciaRegs.SCICTL1.all = 0x0023;                      // 从复位状态释放，使其工作于正常工作状态
    SciaRegs.SCICTL1.bit.SWRESET = 1;                   // 软件复位，更新状态
}

void SCIAFIFOSetup()
{
    SciaRegs.SCIFFTX.bit.TXFFIENA = 0;                  // 是否使能FF发送中断
    SciaRegs.SCIFFTX.bit.TXFFIL = 0;                    // 设置FFTX中断级别
    SciaRegs.SCIFFTX.bit.TXFFINTCLR = 1;                // 清除TXFFINT中断标志位
    SciaRegs.SCIFFTX.bit.TXFIFORESET = 1;               // 复位FIFO发送功能
    SciaRegs.SCIFFTX.bit.SCIFFENA = 1;                  // 使能SCIFIFO功能
    SciaRegs.SCIFFTX.bit.SCIRST = 1;                    // 复位SCI的发送和接收通道
    SciaRegs.SCIFFTX.all = 0xE040;

    SciaRegs.SCIFFRX.bit.RXFFIENA = 0;                  // 是否使能FF接收中断
    SciaRegs.SCIFFRX.bit.RXFFIL = 4;                    // 设置FFRX中断级别
    SciaRegs.SCIFFRX.bit.RXFFINTCLR = 1;                // 清除RXFFINT中断标志位
    SciaRegs.SCIFFRX.bit.RXFIFORESET = 1;               // 复位FIFO接收功能
    SciaRegs.SCIFFRX.all = 0x2044;

    SciaRegs.SCIFFCT.all = 0x0000;                      // 禁用自动波特率和延迟
}

void BaudCalculate(Uint32 desiredBaudRate,Uint32* BaudInf)
{

    extern Uint32 LSPCLKFreq;
    float64 BRR = (LSPCLKFreq/8/desiredBaudRate)-1;                     // 根据公式计算波特率选择寄存器取值
    Uint32 roundBRR = (Uint32)round(BRR);                               // 进行四舍五入
    float64 calBaud1 = LSPCLKFreq / ((roundBRR+1)*8);                   // 根据计算出的整数BRR计算输出波特率值1
    float64 calBaud2 = LSPCLKFreq / 16;                                 // 当BRR取0时的输出波特率值
    Uint32 finalBRR = 0;

    if (fabs(calBaud1-desiredBaudRate)>=fabs(calBaud2-desiredBaudRate)) // 比较哪个输出波特率和目标波特率更接近
    {
        BaudInf[0] = (Uint32)round(calBaud2);                                  // 波特率2较接近则选波特率2
        BaudInf[3] = fabs(calBaud2-desiredBaudRate)/desiredBaudRate;    // 计算误差率
        finalBRR = 0;
    }else
    {
        BaudInf[0] = (Uint32)round(calBaud1);                                  // 否则选波特率1
        BaudInf[3] = fabs(calBaud1-desiredBaudRate)/desiredBaudRate;    // 计算误差率
        finalBRR = roundBRR;
    }

    if (finalBRR==0)
    {
        BaudInf[1] = 0;                                                 // 高八位
        BaudInf[2] = 0;                                                 // 低八位
        BaudInf[4] = 0;                                                 // 范围正常
    }else if (finalBRR >= 1 && finalBRR <= 65535)
    {
        BaudInf[1] = (finalBRR>>8) & 0xFF;                              // 高八位
        BaudInf[2] = finalBRR & 0xFF;                                   // 低八位
        BaudInf[4] = 0;                                                 // 范围正常
    }else if (finalBRR > 65535)
    {
        BaudInf[1] = 255;                                               // 高八位
        BaudInf[2] = 255;                                               // 低八位
        BaudInf[4] = 1;                                                 // 范围异常正常
    }
}

// 通过SCI-A模块发送一个字符(8位)
void SCIAXmit(int data)
{
    while (SciaRegs.SCIFFTX.bit.TXFFST != 0) {}                         //等待上一个数据
    SciaRegs.SCITXBUF.all =data;
}
// 通过SCI-A模块发送一条消息(char数组)
void SCIAMsg(char* msg)
{
    int i;
    i = 0;
    while(msg[i] != '\0')
    {
        SCIAXmit(msg[i]);
        i++;
    }
}

/*
 * PeriSCI.c
 *
 *  Created on: 2023��7��28��
 *      Author: Administrator
 */

#include "F28x_Project.h"
#include "PeriDevices.h"
#include "KiloLib.h"
#include <math.h>

void ConfigureSciA()
{
    Uint32 BaudInf[5] = {0,0,0,0,0};                    // ���ڼ��㲨������Ϣ,0:���������,1:HBAUD,2:LBAUD,3:�����,4:�������Ƿ����

    // ����δ����У�ֻ��SCI-Aģ������ű���ʼ����
    // GPIO_SetupPinMux() - Sets the GPxMUX1/2 and GPyMUX1/2 register bits
    // GPIO_SetupPinOptions() - Sets the direction and configuration of the GPIOS

    GPIO_SetupPinMux(64, GPIO_MUX_CPU1, 6);             // ��ʼ��64�ŵĹ���ΪSCI-Aģ��RX��������
    GPIO_SetupPinOptions(64, GPIO_INPUT, GPIO_PUSHPULL);
    GPIO_SetupPinMux(65, GPIO_MUX_CPU1, 6);             // ��ʼ��65�ŵĹ���ΪSCI-Aģ��TX��������
    GPIO_SetupPinOptions(65, GPIO_OUTPUT, GPIO_ASYNC);

    // ע��: ��InitSysCtrl()�����У�SCI-Aģ���ʱ���Ѿ�����
    SciaRegs.SCICCR.all = 0x0007;
    SciaRegs.SCICCR.bit.SCICHAR = 7;                    // ͨ����Ϣ�ַ�����Ϊ8Bit
    SciaRegs.SCICCR.bit.ADDRIDLE_MODE = 0;              // ѡ�������ģʽЭ��
    SciaRegs.SCICCR.bit.LOOPBKENA = 0;                  // �Ƿ�ʹ�ܽ��û��ز���ģʽ��RX���ź�TX���Ų��ڲ�����
    SciaRegs.SCICCR.bit.PARITY = 0;                     // 0:ʹ����У�� 1:ʹ��żУ��
    SciaRegs.SCICCR.bit.PARITYENA = 0;                  // �Ƿ�ʹ����żУ��
    SciaRegs.SCICCR.bit.STOPBITS = 0;                   // 0:һ��ֹͣλ 1:����ֹͣλ

    SciaRegs.SCICTL1.all = 0x0003;
    SciaRegs.SCICTL1.bit.RXENA = 1;                     // �Ƿ�ʹ��SCI-Aģ��RX���չ���
    SciaRegs.SCICTL1.bit.TXENA = 1;                     // �Ƿ�ʹ��SCI-Aģ��TX���͹���
    SciaRegs.SCICTL1.bit.SLEEP = 0;                     // �Ƿ�ʹ��SCI-Aģ������ģʽ
    SciaRegs.SCICTL1.bit.TXWAKE = 0;                    // SCI���������ѷ�ʽѡ���Ƿ��������ֻ��ѹ���

    SciaRegs.SCICTL2.all = 0x0003;
    SciaRegs.SCICTL2.bit.TXINTENA = 1;                  // �Ƿ�ʹ�ܷ��;����ж�
    SciaRegs.SCICTL2.bit.RXBKINTENA = 1;                // �Ƿ�ʹ�ܽ��վ����ж�

    // ����SCIA�Ĳ�����
    // @LSPCLK = 50 MHz (200 MHz SYSCLK)
    // HBAUD = 0x02 and LBAUD = 0x8B.
    BaudCalculate(9600,BaudInf);
    SciaRegs.SCIHBAUD.bit.BAUD = (int)BaudInf[1];            // ������ѡ�����߰�λ
    SciaRegs.SCILBAUD.bit.BAUD = (int)BaudInf[2];            // ������ѡ�����Ͱ�λ
    SciaRegs.SCIHBAUD.all = 0x0002;
    SciaRegs.SCILBAUD.all = 0x008B;
    SciaRegs.SCICTL1.all = 0x0023;                      // �Ӹ�λ״̬�ͷţ�ʹ�乤������������״̬
    SciaRegs.SCICTL1.bit.SWRESET = 1;                   // �����λ������״̬
}

void SCIAFIFOSetup()
{
    SciaRegs.SCIFFTX.bit.TXFFIENA = 0;                  // �Ƿ�ʹ��FF�����ж�
    SciaRegs.SCIFFTX.bit.TXFFIL = 0;                    // ����FFTX�жϼ���
    SciaRegs.SCIFFTX.bit.TXFFINTCLR = 1;                // ���TXFFINT�жϱ�־λ
    SciaRegs.SCIFFTX.bit.TXFIFORESET = 1;               // ��λFIFO���͹���
    SciaRegs.SCIFFTX.bit.SCIFFENA = 1;                  // ʹ��SCIFIFO����
    SciaRegs.SCIFFTX.bit.SCIRST = 1;                    // ��λSCI�ķ��ͺͽ���ͨ��
    SciaRegs.SCIFFTX.all = 0xE040;

    SciaRegs.SCIFFRX.bit.RXFFIENA = 0;                  // �Ƿ�ʹ��FF�����ж�
    SciaRegs.SCIFFRX.bit.RXFFIL = 4;                    // ����FFRX�жϼ���
    SciaRegs.SCIFFRX.bit.RXFFINTCLR = 1;                // ���RXFFINT�жϱ�־λ
    SciaRegs.SCIFFRX.bit.RXFIFORESET = 1;               // ��λFIFO���չ���
    SciaRegs.SCIFFRX.all = 0x2044;

    SciaRegs.SCIFFCT.all = 0x0000;                      // �����Զ������ʺ��ӳ�
}

void BaudCalculate(Uint32 desiredBaudRate,Uint32* BaudInf)
{

    extern Uint32 LSPCLKFreq;
    float64 BRR = (LSPCLKFreq/8/desiredBaudRate)-1;                     // ���ݹ�ʽ���㲨����ѡ��Ĵ���ȡֵ
    Uint32 roundBRR = (Uint32)round(BRR);                               // ������������
    float64 calBaud1 = LSPCLKFreq / ((roundBRR+1)*8);                   // ���ݼ����������BRR�������������ֵ1
    float64 calBaud2 = LSPCLKFreq / 16;                                 // ��BRRȡ0ʱ�����������ֵ
    Uint32 finalBRR = 0;

    if (fabs(calBaud1-desiredBaudRate)>=fabs(calBaud2-desiredBaudRate)) // �Ƚ��ĸ���������ʺ�Ŀ�겨���ʸ��ӽ�
    {
        BaudInf[0] = (Uint32)round(calBaud2);                                  // ������2�Ͻӽ���ѡ������2
        BaudInf[3] = fabs(calBaud2-desiredBaudRate)/desiredBaudRate;    // ���������
        finalBRR = 0;
    }else
    {
        BaudInf[0] = (Uint32)round(calBaud1);                                  // ����ѡ������1
        BaudInf[3] = fabs(calBaud1-desiredBaudRate)/desiredBaudRate;    // ���������
        finalBRR = roundBRR;
    }

    if (finalBRR==0)
    {
        BaudInf[1] = 0;                                                 // �߰�λ
        BaudInf[2] = 0;                                                 // �Ͱ�λ
        BaudInf[4] = 0;                                                 // ��Χ����
    }else if (finalBRR >= 1 && finalBRR <= 65535)
    {
        BaudInf[1] = (finalBRR>>8) & 0xFF;                              // �߰�λ
        BaudInf[2] = finalBRR & 0xFF;                                   // �Ͱ�λ
        BaudInf[4] = 0;                                                 // ��Χ����
    }else if (finalBRR > 65535)
    {
        BaudInf[1] = 255;                                               // �߰�λ
        BaudInf[2] = 255;                                               // �Ͱ�λ
        BaudInf[4] = 1;                                                 // ��Χ�쳣����
    }
}

// ͨ��SCI-Aģ�鷢��һ���ַ�(8λ)
void SCIAXmit(int data)
{
    while (SciaRegs.SCIFFTX.bit.TXFFST != 0) {}                         //�ȴ���һ������
    SciaRegs.SCITXBUF.all =data;
}
// ͨ��SCI-Aģ�鷢��һ����Ϣ(char����)
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

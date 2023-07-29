/*
 * PeriSCI.c
 *
 *  Created on: 2023��7��28��
 *      Author: Administrator
 */

#include "F28x_Project.h"
#include "PeriSCI.h"
#include "KiloLib.h"

void SciAConfigure()
{
    Uint32 BaudInf[5] = {0,0,0,0,0}                     // ���ڼ��㲨������Ϣ,0:���������,1:HBAUD,2:LBAUD,3:�����,4:�������Ƿ����

    // ����δ����У�ֻ��SCI-Aģ������ű���ʼ����
    // GPIO_SetupPinMux() - Sets the GPxMUX1/2 and GPyMUX1/2 register bits
    // GPIO_SetupPinOptions() - Sets the direction and configuration of the GPIOS

    GPIO_SetupPinMux(28, GPIO_MUX_CPU1, 1);             // ��ʼ��28�ŵĹ���ΪSCI-Aģ��RX��������
    GPIO_SetupPinOptions(28, GPIO_INPUT, GPIO_PUSHPULL);
    GPIO_SetupPinMux(29, GPIO_MUX_CPU1, 1);             // ��ʼ��29�ŵĹ���ΪSCI-Aģ��TX��������
    GPIO_SetupPinOptions(29, GPIO_OUTPUT, GPIO_ASYNC);

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
    SciaRegs.SCIHBAUD.bit.BAUD = BaudInf[1]             // ������ѡ�����߰�λ
    SciaRegs.SCIHBAUD.bit.BAUD = BaudInf[2]             // ������ѡ�����Ͱ�λ
    //SciaRegs.SCIHBAUD.all = 0x0002;
    //SciaRegs.SCILBAUD.all = 0x008B;
    SciaRegs.SCICTL1.all = 0x0023;                      // �Ӹ�λ״̬�ͷţ�ʹ�乤������������״̬
    SciaRegs.SCICTL1.bit.SWRESET = 1;                   // ������λ������״̬
}

void BaudCalculate(Uint32 desiredBaudRate,Uint32* BaudInf)
{

    float64 BRR = (LSPCLKFreq/8/desiredBaudRate)-1;                     // ���ݹ�ʽ���㲨����ѡ��Ĵ���ȡֵ
    Uint32 roundBRR = (Uint32)round(BRR);                               // ������������
    float64 calBaud1 = LSPCLKFreq / ((roundBRR+1)*8);                   // ���ݼ����������BRR�������������ֵ1
    float64 calBaud2 = LSPCLKFreq / 16;                                 // ��BRRȡ0ʱ�����������ֵ
    Uint32 finalBRR = 0;

    if (fabs(calBaud1-desiredBaudRate)>=fabs(calBaud2-desiredBaudRate)) // �Ƚ��ĸ���������ʺ�Ŀ�겨���ʸ��ӽ�
    {
        BaudInf[0] = (Uint32)calBaud2;                                  // ������2�Ͻӽ���ѡ������2
        BaudInf[3] = fabs(calBaud2-desiredBaudRate)/desiredBaudRate     // ���������
        finalBRR = 0;
    }else
    {
        BaudInf[0] = (Uint32)calBaud1;                                  // ����ѡ������1
        BaudInf[3] = fabs(calBaud1-desiredBaudRate)/desiredBaudRate     // ���������
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
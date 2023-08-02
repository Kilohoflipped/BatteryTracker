
/**
 * main.c
 */

#include <F28x_Project.h>
#include "PeriDevices.h"
#include "KiloLib.h"
#include <string.h>
#include <stdint.h>

#define FLASH_RUN 1
#define SRAM_RUN 2
#define RUN_TYPE SRAM_RUN
#if RUN_TYPE==FLASH_RUN
    extern Uint16 RamfuncsLoadStart;
    extern Uint16 RamfuncsLoadSize;
    extern Uint16 RamfuncsRunStart;
#endif

void MemCopy(Uint16 *SourceAddr, Uint16* SourceEndAddr, Uint16* DestAddr);
Uint16 AdcaIntFlag=0;         //����AdcaInt�жϱ�־

int main(void)
{
    extern Uint32 LSPCLKFreq;
    InitSysCtrl();                          // ��ʼ��ϵͳ����
    InitGpio();                             //
    LSPCLKFreq = 50000000;//LSPCLKCal()     // �����������ʱ��Ƶ��
    DINT;                                   // ��ֹ�����ж�
    IER = 0x0000;
    IFR = 0x0000;
    InitPieCtrl();                          // ��ʼ��PIE������
    InitPieVectTable();                     // ��ʼ��PIE������
    #if RUN_TYPE==FLASH_RUN
        MemCopy(&RamfuncsLoadStart, &RamfuncsLoadEnd, &RamfuncsRunStart);
        InitFlash();
    #endif
    ConfigureGpio();
    ConfigureSciaFifo();
    ConfigureSciA();
    ConfigureADC();
    ConfigureEPWM1();
    //ConfigureEPWM2();
    SetupADCChannel();
    IER |= M_INT1;                          // ʹ��CPU�ж���1
    EINT;                                   // ʹ��ȫ���ж�
    ERTM;                                   // ʹ��ȫ���жϵ���

    GPIO_WritePin(92, 1);
    EALLOW;
    PieCtrlRegs.PIEIER1.bit.INTx1 = 1;      // ��PIE��ʹ��ADCINT�ж�
    CpuSysRegs.PCLKCR0.bit.TBCLKSYNC = 1;   // ͬ��ePWM,��ʼTB����
    EPwm1Regs.TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN;
    EDIS;
    SciaMsg("!!!!!!!!!!");                  //����
    while (1)
    {
        if(AdcaIntFlag==1)
        {
            //char ADCLowChar[] = "00000";
            //char ADCHighChar[] = "00000";
            Uint32 ADCResult0 = AdcaResultRegs.ADCRESULT0;         // ת�����
            //Uint32 ADCReLow8 = ADCResult0 & 0xFF;                  // ��ȡ�Ͱ�λ
            //Uint32 ADCReHigh8 = (ADCResult0>>8) & 0xFF;            // ��ȡ�߰�λ(ȡ����ת��ģʽ)
            while(AdcaRegs.ADCINTFLG.bit.ADCINT1 == 0);
            AdcaIntFlag = 0;
            //Int2Str(ADCReLow8,ADCLowChar);                  // �Ͱ�λ����ת��Ϊ�ַ���
            //Int2Str(ADCReHigh8,ADCHighChar);                // �߰�λ����ת��Ϊ�ַ���
            SciaMsg("AD=");
            SciaXmit(ADCResult0 & 0xFF);
            SciaXmit((ADCResult0>>8) & 0xFF);
            //SciaMsg("!\n");
        }
    }
}

void MemCopy(Uint16 *SourceAddr, Uint16* SourceEndAddr, Uint16* DestAddr)
{
    while(SourceAddr < SourceEndAddr)
    {
        *DestAddr++ = *SourceAddr++;
    }
    return;
}

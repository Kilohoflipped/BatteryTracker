
/**
 * main.c
 */

#define FLASH_RUN 1
#define SRAM_RUN 2
#define RUN_TYPE SRAM_RUN
#if RUN_TYPE==FLASH_RUN
    extern Uint16 RamfuncsLoadStart;
    extern Uint16 RamfuncsLoadSize;
    extern Uint16 RamfuncsRunStart;
#endif

#include <F28x_Project.h>
#include "PeriDevices.h"
#include "KiloLib.h"
#include <string.h>
#include <stdint.h>

int main(void)
{
    extern Uint32 LSPCLKFreq;
    // ��ʼ��ϵͳ����
    InitSysCtrl();
    // �����������ʱ��Ƶ��
    LSPCLKFreq = LSPCLKCal();
    // ��ֹ�����ж�
    DINT;
    IER = 0x0000;
    IFR = 0x0000;
    // ��ʼ��PIE������
    InitPieCtrl();
    // ��ʼ��PIE������
    InitPieVectTable();
    SCIAFIFOSetup();
    ConfigureSciA();
    ConfigureADC();
    ConfigureEPWM1();
    SetupADCChannel(0);
    PieCtrlRegs.PIEIER1.bit.INTx1 = 1;  // ��PIE��ʹ��ADCINT�ж�
    IER |= M_INT1;                      // Enable CPU Interrupt 1
    EINT;                               // ʹ��ȫ���ж�
    ERTM;                               // ʹ��ȫ���жϵ���
    return 0;
}

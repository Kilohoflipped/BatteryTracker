
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
#include <string.h>
#include <stdint.h>
#include "KiloLib.h"




int main(void)
{
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


    return 0;
}

/*
 * PeriADC.c
 *
 *  Created on: 2023��7��25��
 *      Author: Administrator
 */
#include "F28x_Project.h"
#include "PeriDevices.h"
#include "KiloLib.h"
extern Uint16 AdcaIntFlag;

void ConfigureADC(void)
{
    // ����ADC���жϺ���
    EALLOW;
    PieVectTable.ADCA1_INT = &ADCA_INT1_ISR;
    EDIS;

    EALLOW;
    // ʱ��ʱ��Ԥ��Ƶ������ʱ�ӵ�1/4,��Ƶ200M,ADCƵ�����50M
    AdcaRegs.ADCCTL2.bit.PRESCALE = 6;
    AdcbRegs.ADCCTL2.bit.PRESCALE = 6;
    AdccRegs.ADCCTL2.bit.PRESCALE = 6;
    AdcdRegs.ADCCTL2.bit.PRESCALE = 6;
    // �趨ADCģ�鹤��ģʽ�������ڵ������룬��ʱ���Ƚ����趨Ϊ12Bit
    AdcSetMode(ADC_ADCA, ADC_RESOLUTION_12BIT, ADC_SIGNALMODE_SINGLE);
    AdcSetMode(ADC_ADCB, ADC_RESOLUTION_12BIT, ADC_SIGNALMODE_SINGLE);
    AdcSetMode(ADC_ADCC, ADC_RESOLUTION_12BIT, ADC_SIGNALMODE_SINGLE);
    AdcSetMode(ADC_ADCD, ADC_RESOLUTION_12BIT, ADC_SIGNALMODE_SINGLE);
    // ����ADCģ����ж�Ϊ������ת������ʱ(EOC)
    AdcaRegs.ADCCTL1.bit.INTPULSEPOS = 1;
    AdcbRegs.ADCCTL1.bit.INTPULSEPOS = 1;
    AdccRegs.ADCCTL1.bit.INTPULSEPOS = 1;
    AdcdRegs.ADCCTL1.bit.INTPULSEPOS = 1;
    // ��ADCģ���ϵ�
    AdcaRegs.ADCCTL1.bit.ADCPWDNZ = 1;
    AdcbRegs.ADCCTL1.bit.ADCPWDNZ = 1;
    AdccRegs.ADCCTL1.bit.ADCPWDNZ = 1;
    AdcdRegs.ADCCTL1.bit.ADCPWDNZ = 1;
    DELAY_US(1000); // ��ʱ�ȴ�ADCģ���ϵ����
    EDIS;
}

void SetupADCChannel()
{
    Uint16 acqps; // �������ڵĴ�С

    // ���ڲ������ȼ�����SYSCLKSΪ��λ����С��������

    if (ADC_RESOLUTION_12BIT == AdcaRegs.ADCCTL2.bit.RESOLUTION) // ��������Ϊ12Bit
    {
        acqps = 14; // ���ò�������Ϊ14��SYSCLK���ڣ�ԼΪ75ns
    }
    else // ��������Ϊ16Bit
    {
        acqps = 63; // ���ò�������Ϊ63��SYSCLK���ڣ�ԼΪ320ns
    }
    //
    // ���ø�������ͨ��
    EALLOW;
    // ADCA
    AdcaRegs.ADCSOC0CTL.bit.CHSEL = 0;              // ����SOC0�������ź�ͨ��A0
    AdcaRegs.ADCSOC0CTL.bit.ACQPS = acqps;          // ����SOC0�Ĳ������ڴ�С
    AdcaRegs.ADCSOC0CTL.bit.TRIGSEL = 5;            // ����SOC0�Ĵ���Դ0:�����,5:ePWM1,SOC/A
    AdcaRegs.ADCINTSEL1N2.bit.INT1E = 1;            // ʹ��INT1�ж�
    AdcaRegs.ADCINTSEL1N2.bit.INT1SEL = 0;          // SOC0ת����ɺ�ʹ��INT��λ
    AdcaRegs.ADCINTFLGCLR.bit.ADCINT1 = 1;          // ��ʼ���������жϱ�־λ
    // ADCB
    AdcbRegs.ADCSOC0CTL.bit.CHSEL = 0;              // ����SOC0�������ź�ͨ��B0
    AdcbRegs.ADCSOC0CTL.bit.ACQPS = acqps;          // ����SOC0�Ĳ������ڴ�С
    AdcbRegs.ADCSOC0CTL.bit.TRIGSEL = 0;            // ����SOC0�Ĵ���Դ0:�����,5:ePWM1,SOC/A
    AdcbRegs.ADCINTSEL1N2.bit.INT1E = 1;            // ʹ��INT1�ж�
    AdcbRegs.ADCINTSEL1N2.bit.INT1SEL = 0;          // SOC0ת����ɺ�ʹ��INT��λ
    AdcbRegs.ADCINTFLGCLR.bit.ADCINT1 = 1;          // ��ʼ���������жϱ�־λ
    // ADCC
    AdccRegs.ADCSOC0CTL.bit.CHSEL = 0;              // ����SOC0�������ź�ͨ��C0
    AdccRegs.ADCSOC0CTL.bit.ACQPS = acqps;          // ���ò������ڴ�С
    AdccRegs.ADCSOC0CTL.bit.TRIGSEL = 0;            // ����SOC0�Ĵ���Դ0:�����,5:ePWM1,SOC/A
    AdccRegs.ADCINTSEL1N2.bit.INT1E = 1;            // ʹ��INT1�ж�
    AdccRegs.ADCINTSEL1N2.bit.INT1SEL = 0;          // SOC0ת����ɺ�ʹ��INT��λ
    AdccRegs.ADCINTFLGCLR.bit.ADCINT1 = 1;          // ��ʼ���������жϱ�־λ
    // ADCD
    AdcdRegs.ADCSOC0CTL.bit.CHSEL = 0;              // ����SOC0�������ź�ͨ��D0
    AdcdRegs.ADCSOC0CTL.bit.ACQPS = acqps;          // ����SOC0�Ĳ������ڴ�С
    AdcdRegs.ADCSOC0CTL.bit.TRIGSEL = 0;            // ����SOC0�Ĵ���Դ0:�����,5:ePWM1,SOC/A
    AdcdRegs.ADCINTSEL1N2.bit.INT1E = 1;            // ʹ��INT1�ж�
    AdcdRegs.ADCINTSEL1N2.bit.INT1SEL = 0;          // SOC0ת����ɺ�ʹ��INT��λ
    AdcdRegs.ADCINTFLGCLR.bit.ADCINT1 = 1;          // ��ʼ���������жϱ�־λ
    EDIS;
}

interrupt void ADCA_INT1_ISR(void)
{
    AdcaIntFlag = 1;
    // ����жϱ�־
    AdcaRegs.ADCINTFLGCLR.bit.ADCINT1 = 1;
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
}

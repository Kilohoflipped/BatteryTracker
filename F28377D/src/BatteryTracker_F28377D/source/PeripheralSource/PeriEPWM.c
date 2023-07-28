/*
 * PeriEPWM.c
 *
 *  Created on: 2023��7��26��
 *      Author: Administrator
 */

#include "F28x_Project.h"
#include "PeriEPWM.h"

void ConfigureEPWM1(void)
{
    InitEPwm1Gpio();        // ��ʼ��GPIO��ʹ��GPIO145��GPIO146��ע�⣺28377�������ܽſ��Բ���pwm���˴�ע������
    EPwm1Regs.TBPRD = 1000; // 100k=100e6/��2*1000��
    // Set Compare values
    EPwm1Regs.CMPA.bit.CMPA = 1000 - 250; // �趨Aͨ��ռ�ձ�
    EPwm1Regs.TBPHS.bit.TBPHS = 0;        // ����λ�Ĵ�������
    EPwm1Regs.TBCTR = 0;                  // ��ʱ���׼����������
    // Setup TBCLK
    EPwm1Regs.TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN; // �趨Ϊ������ģʽ ����������Ϊ�Գ�
    EPwm1Regs.TBCTL.bit.PHSEN = TB_DISABLE;        // ��ֹ��λ����
    EPwm1Regs.TBCTL.bit.HSPCLKDIV = 0x00;          // ����Ƶ
    EPwm1Regs.TBCTL.bit.CLKDIV = 0x00;             // ����Ƶ
    // Set actions
    EPwm1Regs.AQCTLA.bit.CAU = AQ_SET; // Set PWM3A on Zero
    EPwm1Regs.AQCTLA.bit.CAD = AQ_CLEAR;
    EPwm1Regs.AQCTLB.bit.CAU = AQ_CLEAR;           // Set PWM3A on Zero
    EPwm1Regs.AQCTLB.bit.CAD = AQ_SET;             // �趨����
    EPwm1Regs.DBCTL.bit.OUT_MODE = DB_FULL_ENABLE; // ��Ϊ���߶�������
    EPwm1Regs.DBCTL.bit.POLSEL = DB_ACTV_HIC;      // ������ô���ã��������߶���5%����ʧЧ
    //    EPwm1Regs.DBRED.bit.DBRED = 2;                                  //����Ϊ10%
    //    EPwm1Regs.DBFED.bit.DBFED = 2;
    //    EALLOW;
    //    // Assumes ePWM clock is already enabled
    //    EPwm1Regs.ETSEL.bit.SOCAEN    = 1;    // Disable SOC on A group
    //    EPwm1Regs.ETSEL.bit.SOCASEL    = 1;   // 3��һ�����ڲ������Σ�ֻ����������������2:һ�� 4 һ��
    //    EPwm1Regs.ETPS.bit.SOCAPRD = 1;       // Generate pulse on 1st event
    //    EDIS;
}

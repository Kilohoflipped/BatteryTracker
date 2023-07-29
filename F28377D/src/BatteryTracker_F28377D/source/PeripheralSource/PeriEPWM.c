/*
 * PeriEPWM.c
 *
 *  Created on: 2023��7��26��
 *      Author: Administrator
 */

#include "F28x_Project.h"
#include "PeriEPWM.h"
#include <math.h>

void ConfigureEPWM1(void)
{
    // ��ʼ��GPIO��ʹ��GPIO145��GPIO146��ע�⣺28377�����Թܽſ��Բ���PWM���˴�ע������
    InitEPwm1Gpio();
    EALLOW;
    // ����TB���ڼ�������ֵ
    // ������������ģʽ��˵��F(EPWM)=PLLSYSCLK/2/TBPRD
    EPwm1Regs.TBPRD = 1000;                         // �趨EPWMƵ��Ϊ100k=200*1000*1000/2/1000
    // ���ñȽϼĴ�����ֵ
    EPwm1Regs.CMPA.bit.CMPA = floor(0.75*1000);     // �趨Aͨ��ռ�ձ�
    EPwm1Regs.CMPB.bit.CMPB = floor(0.75*1000);     // �趨Bͨ��ռ�ձ�
    EPwm1Regs.TBPHS.bit.TBPHS = 0;                  // ����λ�Ĵ�������
    EPwm1Regs.TBCTR = 0;                            // ��ʱ���׼����������
    // ����EPWMģ��ļ�����ʽ
    EPwm1Regs.TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN;  // �趨Ϊ��������ģʽ�������ԳƵ�EPMW��
    EPwm1Regs.TBCTL.bit.PHSEN = TB_DISABLE;         // ��ֹ��λ����
    EPwm1Regs.TBCTL.bit.HSPCLKDIV = 0x00;           // ����Ƶ
    EPwm1Regs.TBCTL.bit.CLKDIV = 0x00;              // ����Ƶ
    // ����EPWM������ʽ
    EPwm1Regs.AQCTLA.bit.CAU = AQ_SET;              // ��λ
    EPwm1Regs.AQCTLA.bit.CAD = AQ_CLEAR;
    EPwm1Regs.AQCTLB.bit.CBU = AQ_CLEAR;            // ����
    EPwm1Regs.AQCTLB.bit.CBD = AQ_SET;
    // �趨����������
    EPwm1Regs.DBCTL.bit.OUT_MODE = DB_FULL_ENABLE;  // ��Ϊ���߶�������
    EPwm1Regs.DBCTL.bit.POLSEL = DB_ACTV_HIC;       // ������ô���ã��������߶���5%����ʧЧ
    EPwm1Regs.DBRED.bit.DBRED = 2;                  // ����Ϊ10%
    EPwm1Regs.DBFED.bit.DBFED = 2;
    // ����EPWMʱ���Ѿ���ʹ��
    EPwm1Regs.ETSEL.bit.SOCAEN = 1;                 // �Ƿ�ʹ��ADC��������SOC/A
    EPwm1Regs.ETSEL.bit.SOCASEL = ET_CTR_ZERO;      // ET�¼�ѡ��
    EPwm1Regs.ETPS.bit.SOCAPRD = ET_1ST;            // ÿһ���¼�������һ����������
    EPwm1Regs.ETSEL.bit.SOCBEN = 0;                 // �Ƿ�ʹ��ADC��������SOC/B
    EPwm1Regs.ETSEL.bit.SOCBSEL = ET_CTR_ZERO;      // ET�¼�ѡ��
    EPwm1Regs.ETPS.bit.SOCBPRD = ET_1ST;            // ÿһ���¼�������һ����������
    EDIS;
}

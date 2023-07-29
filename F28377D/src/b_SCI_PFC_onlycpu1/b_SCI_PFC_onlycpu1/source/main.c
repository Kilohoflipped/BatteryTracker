/*
����д��falsh 2019/08/21
*/
#include "F28x_Project.h"
#include "DCL.h"
#include "epwm_adc_Init_zzy.h"
#include "gpio_exinterrupt_Init_zzy.h"
#include "sci_zzy.h"
//============================================================================
/*-----------------------��������-----------------------------*/
void MemCopy(Uint16 *SourceAddr, Uint16* SourceEndAddr, Uint16* DestAddr);
//===========================================================
#define _STANDALONE 0
#define _FLASH  1



float A0; //��ȡ�Ĵ�������
float B0; //��ȡ�Ĵ�������
float C2; //��ȡ�Ĵ�������
float D0; //��ȡ�Ĵ�������



//----------------------For control-----------------------
float vs=0,iL=0,vo=0;         //��̬����ֵ
float vref=0;                 //��̬��ѹ�ο�
float vrefWork=400;           //������̬��ѹ�ο�
float vrefStart=250;           //������ѹ�ο�
float Ts=0.00002;            //��������

float vs_RMS=220;            //�����ѹ����Чֵ
float vs_pp=311.08;          //�����ѹ�ķ�ֵ

//����ʽpi���Ʋ���
float ev=0,ev_pre=0;       //��ѹ�⻷ ����� ev��ǰ��� ev_pre֮ǰ���
float duv=0,uv=0,uv_sin=0;        //duv��ѹ�⻷
float ei=0,ei_pre=0;       //�����ڻ� ����� ei��ǰ��� ei_pre֮ǰ���
float dui=0,ui=0;        //dui�����ڻ�

float iL2=0;  //�Ƶ����ݵ���

//----------------------For storage and estiamtion-----------------------

float vsDis[250]; //�洢˲̬��ѹֵ
float iLDis[250]; //�洢˲̬����ֵ
float voDis[250]; //�洢˲̬��ѹֵ
float iL2Dis[250]; //�洢˲̬��ѹֵ
//float uv_sinDis[250]; //�洢˲̬��ѹֵ


//-----------------------------------------For SCI---------------------
char vsSend[6];
char iLSend[6];
char voSend[6];
char iL2Send[6];
float vsOne=0, iLOne=0,voOne=0,iL2One=0;
Uint16  ii=0,jj=0,mm=0,nn=0;

//-----------------------------------------others---------------------

Uint16 sendFlag=0;       //�����ⲿ�жϱ�־
Uint16 finishFlag=1;       //�����ⲿ�жϱ�־
Uint16 transFlag=0;         //����ADC�жϱ�־
Uint16 adcFlag=0;         //����ADC�жϱ�־
Uint16 i=0;

float voPre=0;

char *msg;


/****************************************************************************
*��    �ܣ�������
*��ڲ�������
*���ڲ�������
*˵    ������
****************************************************************************/
void main(void)
{
    InitSysCtrl();
/*
    //----------------------------------------------------
    #ifdef _STANDALONE
    #ifdef _FLASH
        //  Send boot command to allow the CPU02 application to begin execution
    //   IPCBootCPU2(C1C2_BROM_BOOTMODE_BOOT_FROM_FLASH);
    #else
        //  Send boot command to allow the CPU02 application to begin execution
    //  IPCBootCPU2(C1C2_BROM_BOOTMODE_BOOT_FROM_RAM);
    #endif
    #endif
        //дFLASH����  �������ADC֮ǰ�����������ADC��ʼ��֮ǰ-
        MemCopy(&RamfuncsLoadStart, &RamfuncsLoadEnd, &RamfuncsRunStart);
        InitFlash();
*/
//-----------------------------------------------------
    InitPieCtrl();                                                  //��ʼ��PIE
    DINT;
    IER = 0x0000;                                                   //��ֹ�����жϣ�����������жϱ�
    IFR = 0x0000;
    InitPieVectTable();
    //��Ӵ���----------------------------------------------------
    scia_fifo_init();       // Initialize the SCI FIFO
    SciAConfigure();   // Initialize SCI for echoback
    ConfigureADC();        //����
    ConfigureEPWM1();       //Configure the ePWM,AD����
    ConfigureEPWM6();       //����boost
    SetupADCEpwm(0);       //Setup the ADC for ePWM triggered conversions on channel 0
    ledkeyInit();          //��ʼ��led
    exInterruptInit();     //��ʼ���ⲿ�ж�
    //������,����ʹ���ж�----------------------------------------------------
    // Enable XINT1 and XINT2 in the PIE: Group 1 interrupt 4 & 5
    // Enable ADCINT in PIE
    // Enable INT1 which is connected to WAKEINT:
    PieCtrlRegs.PIECTRL.bit.ENPIE = 1;          // Enable the PIE block
//    PieCtrlRegs.PIEIER1.bit.INTx4 = 1;          // Enable PIE Group 1 INT4���ⲿ�����ж�
    //    PieCtrlRegs.PIEIER1.bit.INTx5 = 1;          // Enable PIE Group 1 INT5
    PieCtrlRegs.PIEIER1.bit.INTx1 = 1;          // Enable ADCINT in PIE
    IER |= M_INT1; // Enable CPU Interrupt 1
    EINT;          // Enable Global interrupt INTM
    ERTM;          // Enable Global realtime interrupt DBGM
    for(;;)
    {

        // Example
//        float Value=789.34;
//        scia_msg("  You sent: \0");
//        scia_msg("  \r\n  \0");
//        scia_msg("  12.345: \0");
//        scia_msg("  \r\n  \0");
//        ftoa(vsSend,Value);
//        scia_msg(vsSend);
//        scia_msg("  \r\n  \0");

/*
        if(sendFlag==1 && finishFlag==1)
        {
            finishFlag=0;
            scia_msg("----------------vs-----------------\r\n\0");
            do
            {
                vsOne=vsDis[ii];
                ftoa(vsSend,vsOne);
                scia_msg(vsSend);
                scia_msg("\r\n\0");
                ii++;
            }
            while (ii<250);

            scia_msg("-------------------iL---------------\r\n\0");
            do
            {
                iLOne=iLDis[ii];
                ftoa(iLSend,iLOne);
                scia_msg(iLSend);
                scia_msg("\r\n\0");
                ii++;
            }
            while (jj<250);

            scia_msg("-------------------vo---------------\r\n\0");
            do
            {
                voOne=voDis[ii];
                ftoa(voSend,voOne);
                scia_msg(voSend);
                scia_msg("\r\n\0");
                ii++;
            }
            while (mm<250);

            scia_msg("-------------------iL2---------------\r\n\0");
            do
            {
                iL2One=iL2Dis[ii];
                ftoa(iL2Send,iL2One);
                scia_msg(iL2Send);
                scia_msg("\r\n\0");
                ii++;
            }
            while (nn<250);

            scia_msg("******************* Stop Send ******************* \r\n\0");
            ii=jj=mm=nn=0;

            sendFlag=0;
            finishFlag=1;


        }
*/



    }
}
/****************************************************************************
*��    �ܣ�adca1_isr - Read ADC Buffer in ISR
*��ڲ�������
*���ڲ�������
*˵    ������
****************************************************************************/
interrupt void adc_isr(void)
{
    LED5_ON;

    A0 = (AdcaResultRegs.ADCRESULT0);
       B0 = (AdcbResultRegs.ADCRESULT0);
       C2 = (AdccResultRegs.ADCRESULT0);


       vs =  0.1326*A0 + 1.397;                  //
       iL =  0.00119*B0-0.1395;                 //
       vo=  0.1565*C2+15.31;



       //�Ĳο� ������
       if(vo<250)
       {
           vref=vrefStart;
           vrefStart++;
           if(vrefStart>400)
           {
               vrefStart=250;
           }
       }
       else
       {
           vref=vrefWork;
       }


   //-------------------------------------����ʽPI-------------------------------------------
   //��ѹ�⻷
       ev=vref-vo;
       duv=0.01*(ev-ev_pre)+0.3*Ts*ev;   //duv=kp*(eu-eu_pre)+ki*eu
       uv=uv+duv;                     //��ѹ�����
       ev_pre=ev;
       if(uv<0.01)                      //uv��ѹ�������Ϊ�������ο� �޷�
       {
           uv=0.01;
       }
       if (uv>5)
       {
           uv=5;
       }
   //���һ�
       uv_sin=uv*vs/vs_pp;

   //�����ڻ�
       ei=uv_sin-iL;
       dui=0.24*(ei-ei_pre)+690*Ts*ei;   //dui=kp*(ei-ei_pre)+ki*ei;

       ui=ui+dui;                  //���������
       ei_pre=ei;
       if(ui<0)
       {
           ui=0;
       }
       if (ui>0.99)
       {
           ui=0.99;
       }

       EPwm6Regs.CMPA.bit.CMPA = 1000-1000*ui;

       iL2=iL*(1-ui);

     //--------------------------------------------------------------------------------

       if(vo>420 && voPre>420)
         {
           transFlag=1;
         }

       if(transFlag==1)
       {
           adcFlag++;
           if (adcFlag>3)
           {
               adcFlag=0;
               i++;
           }

           vsDis[i]=vs;
           iLDis[i]=iL;
           voDis[i]=vo;
           iL2Dis[i]=iL2;
            //    uv_sinDis[i]=uv_sin;
           if(i>250)
           {
               i=0;
               transFlag=0;
           }

       }


       voPre=vo;

       LED5_OFF;

    AdcaRegs.ADCINTFLGCLR.bit.ADCINT1 = 1; //clear INT1 flag
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
}
/****************************************************************************
*��    �ܣ�xint1_isr - External Interrupt 1 ISR
*��ڲ�������
*���ڲ�������
*˵    ������
****************************************************************************/
interrupt void xint1_isr(void)
{
//    exintFlag=1;
    //
    // Acknowledge this interrupt to get more from group 1
    //
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
}
/****************************************************************************
*��    �ܣ�xint2_isr - External Interrupt 2 ISR
*��ڲ�������
*���ڲ�������
*˵    ������
****************************************************************************/
interrupt void xint2_isr(void)
{
//    exintFlag=1;
    //
    // Acknowledge this interrupt to get more from group 1
    //
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
}
//==========================================================================
void MemCopy(Uint16 *SourceAddr, Uint16* SourceEndAddr, Uint16* DestAddr)
{
    while(SourceAddr < SourceEndAddr)
    {
        *DestAddr++ = *SourceAddr++;
    }
    return;
}

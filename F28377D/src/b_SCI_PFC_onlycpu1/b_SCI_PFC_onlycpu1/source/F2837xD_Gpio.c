//###########################################################################
// FILE:    F2837xD_Gpio.c
// TITLE:   GPIO module support functions
// AUTHOR:  Adam Haun
// NOTES:
//###########################################################################
// $TI Release: F2837xD Support Library v160 $
// $Release Date: Mon Jun 15 13:36:23 CDT 2015 $
// $Copyright: Copyright (C) 2013-2015 Texas Instruments Incorporated -
//             http://www.ti.com/ ALL RIGHTS RESERVED $
//###########################################################################

#include "F2837xD_device.h"
#include "F2837xD_Examples.h"

//Low-level functions for GPIO configuration (CPU1 only)

#ifdef CPU1
//Sets all pins to be muxed to GPIO in input mode with pull-ups enabled.
//Also resets CPU control to CPU1 and disables open drain and polarity
//inversion and sets the qualification to synchronous. Also unlocks all GPIOs
//Only one CPU should call this function.
void InitGpio()
{
    volatile Uint32 *gpioBaseAddr;
    Uint16 regOffset;
    //Disable pin locks
    EALLOW;
    GpioCtrlRegs.GPALOCK.all = 0x00000000;
    GpioCtrlRegs.GPBLOCK.all = 0x00000000;
    GpioCtrlRegs.GPCLOCK.all = 0x00000000;
    GpioCtrlRegs.GPDLOCK.all = 0x00000000;
    GpioCtrlRegs.GPELOCK.all = 0x00000000;
    GpioCtrlRegs.GPFLOCK.all = 0x00000000;
    //Fill all registers with zeros. Writing to each register separately
    //for six GPIO modules would make this function *very* long. Fortunately,
    //we'd be writing them all with zeros anyway, so this saves a lot of space.
    gpioBaseAddr = (Uint32 *)&GpioCtrlRegs;
    for (regOffset = 0; regOffset < sizeof(GpioCtrlRegs)/2; regOffset++)
    {
        //Hack to avoid enabling pull-ups on all pins. GPyPUD is offset
        //0x0C in each register group of 0x40 words. Since this is a
        //32-bit pointer, the addresses must be divided by 2.
        if (regOffset % (0x40/2) != (0x0C/2))
        {
            gpioBaseAddr[regOffset] = 0x00000000;
        }
    }
    gpioBaseAddr = (Uint32 *)&GpioDataRegs;
    for (regOffset = 0; regOffset < sizeof(GpioDataRegs)/2; regOffset++)
    {
        gpioBaseAddr[regOffset] = 0x00000000;
    }
    //-------------------SCIA----------------------------------
    GpioCtrlRegs.GPAPUD.bit.GPIO8 = 0;    // Enable pull-up for GPIO8 (SCITXDB)
    GpioCtrlRegs.GPAQSEL1.bit.GPIO9 = 3;  // Asynch input GPIO9 (SCIRXDB)
    GpioCtrlRegs.GPAGMUX1.bit.GPIO8=1;
    GpioCtrlRegs.GPAMUX1.bit.GPIO8 =2;   // Configure GPIO8 for SCITXDA operation
    GpioCtrlRegs.GPAGMUX1.bit.GPIO9=1;
    GpioCtrlRegs.GPAMUX1.bit.GPIO9 = 2;  // Configure GPIO9 for SCIRXDA operation
    GpioCtrlRegs.GPADIR.bit.GPIO8 =1;
    GpioCtrlRegs.GPADIR.bit.GPIO9 =0;
    //------------------IUART——LED----------------------------------
    GpioCtrlRegs.GPAPUD.bit.GPIO18 = 0;    //  GPIO_txdb，0010
    GpioCtrlRegs.GPAQSEL2.bit.GPIO18 = 3;
    GpioCtrlRegs.GPAGMUX2.bit.GPIO18=0;
    GpioCtrlRegs.GPAMUX2.bit.GPIO18 =2;
    GpioCtrlRegs.GPADIR.bit.GPIO18 =1;
    GpioCtrlRegs.GPAPUD.bit.GPIO11 = 0;    //  GPIOrxdb 0010
    GpioCtrlRegs.GPAQSEL1.bit.GPIO11 = 3;
    GpioCtrlRegs.GPAGMUX1.bit.GPIO11=0;
    GpioCtrlRegs.GPAMUX1.bit.GPIO11 =2;
    GpioCtrlRegs.GPADIR.bit.GPIO11 =0;
    //------------------I2C------------------------------------
    GpioCtrlRegs.GPAPUD.bit.GPIO0 =0; //SDAA
    GpioCtrlRegs.GPAQSEL1.bit.GPIO0 =3;
    GpioCtrlRegs.GPAGMUX1.bit.GPIO0=1;
    GpioCtrlRegs.GPAMUX1.bit.GPIO0=2;
    GpioCtrlRegs.GPAPUD.bit.GPIO1 =0; //SCLA
    GpioCtrlRegs.GPAQSEL1.bit.GPIO1 =3;
    GpioCtrlRegs.GPAGMUX1.bit.GPIO1=1;
    GpioCtrlRegs.GPAMUX1.bit.GPIO1=2;
    GpioCtrlRegs.GPADIR.bit.GPIO1 =1;
    //-------------------SPIB------------------------------------控制板载EEPROm
    GpioCtrlRegs.GPBPUD.bit.GPIO63 = 0;
    GpioCtrlRegs.GPCPUD.bit.GPIO64 = 1;
    GpioCtrlRegs.GPCPUD.bit.GPIO65 = 0;
    GpioCtrlRegs.GPEPUD.bit.GPIO143 = 0;//上拉有效
    GpioCtrlRegs.GPBGMUX2.bit.GPIO63= 3;
    GpioCtrlRegs.GPBMUX2.bit.GPIO63 = 3;
    GpioCtrlRegs.GPCGMUX1.bit.GPIO64= 3;
    GpioCtrlRegs.GPCMUX1.bit.GPIO64 = 3;
    GpioCtrlRegs.GPCDIR.bit.GPIO64=0;
    GpioCtrlRegs.GPCGMUX1.bit.GPIO65= 3;
    GpioCtrlRegs.GPCMUX1.bit.GPIO65= 3;
    GpioCtrlRegs.GPEGMUX1.bit.GPIO143= 0;
    GpioCtrlRegs.GPEMUX1.bit.GPIO143= 0;
    GpioCtrlRegs.GPEDIR.bit.GPIO143=1;
//-------------------SPIA------------------------------------控制YC板载EEPROm
    GpioCtrlRegs.GPBPUD.bit.GPIO54 = 0;
    GpioCtrlRegs.GPBPUD.bit.GPIO55 = 1;
    GpioCtrlRegs.GPBPUD.bit.GPIO56 = 0;
    GpioCtrlRegs.GPDPUD.bit.GPIO115 = 0;//上拉有效   SPICS3
    GpioCtrlRegs.GPDPUD.bit.GPIO117 = 0;//上拉有效   SPICS2
    GpioCtrlRegs.GPEPUD.bit.GPIO130 = 0;//上拉有效，SPICS1
    GpioCtrlRegs.GPBPUD.bit.GPIO32  = 0; //上拉有效,SPICS4
    GpioCtrlRegs.GPCPUD.bit.GPIO67  = 0; //上拉有效,SPICS5
    GpioCtrlRegs.GPBGMUX2.bit.GPIO54= 0;
    GpioCtrlRegs.GPBMUX2.bit.GPIO54 = 1;
    GpioCtrlRegs.GPBGMUX2.bit.GPIO55= 0;
    GpioCtrlRegs.GPBMUX2.bit.GPIO55 = 1;
    GpioCtrlRegs.GPBGMUX2.bit.GPIO56= 0;
    GpioCtrlRegs.GPBMUX2.bit.GPIO56 = 1;
    GpioCtrlRegs.GPDGMUX2.bit.GPIO115= 0;
    GpioCtrlRegs.GPDMUX2.bit.GPIO115= 0;
    GpioCtrlRegs.GPDDIR.bit.GPIO115=1;
    GpioCtrlRegs.GPDGMUX2.bit.GPIO117= 0;
    GpioCtrlRegs.GPDMUX2.bit.GPIO117= 0;
    GpioCtrlRegs.GPDDIR.bit.GPIO117=1;
    GpioCtrlRegs.GPEGMUX1.bit.GPIO130= 0;
    GpioCtrlRegs.GPEMUX1.bit.GPIO130= 0;
    GpioCtrlRegs.GPEDIR.bit.GPIO130=1;
    GpioCtrlRegs.GPBGMUX1.bit.GPIO32= 0;
    GpioCtrlRegs.GPBMUX1.bit.GPIO32= 0;
    GpioCtrlRegs.GPBDIR.bit.GPIO32=1;
    GpioCtrlRegs.GPCGMUX1.bit.GPIO67= 0;
    GpioCtrlRegs.GPCMUX1.bit.GPIO67= 0;
    GpioCtrlRegs.GPCDIR.bit.GPIO67=1;
    GpioCtrlRegs.GPDPUD.bit.GPIO110 = 0;
    GpioDataRegs.GPDSET.bit.GPIO110 = 1;
    GpioCtrlRegs.GPDMUX1.bit.GPIO110 =0;
    GpioCtrlRegs.GPDDIR.bit.GPIO110 =1; //output
    GpioDataRegs.GPESET.bit.GPIO130=1;
    GpioDataRegs.GPDSET.bit.GPIO117=1;
    GpioDataRegs.GPDSET.bit.GPIO115=1;
    GpioDataRegs.GPBSET.bit.GPIO32=1;
    GpioDataRegs.GPCSET.bit.GPIO67=1;
//-------------------YK_CTRL---------------------------
    GpioCtrlRegs.GPEGMUX1.bit.GPIO137= 0;//YK_CHOOSE3
    GpioCtrlRegs.GPEMUX1.bit.GPIO137= 0;
    GpioCtrlRegs.GPEDIR.bit.GPIO137=1;
    GpioCtrlRegs.GPDGMUX2.bit.GPIO125= 0;//YK_CHOOSE1
    GpioCtrlRegs.GPDMUX2.bit.GPIO125= 0;
    GpioCtrlRegs.GPDDIR.bit.GPIO125=1;
    GpioCtrlRegs.GPCGMUX2.bit.GPIO84= 0;//YK_CHOOSE2
    GpioCtrlRegs.GPCMUX2.bit.GPIO84= 0;
    GpioCtrlRegs.GPCDIR.bit.GPIO84=1;
    GpioCtrlRegs.GPCGMUX1.bit.GPIO68= 0;//ACT
    GpioCtrlRegs.GPCMUX1.bit.GPIO68= 0;
    GpioCtrlRegs.GPCDIR.bit.GPIO68=1;
    GpioCtrlRegs.GPCGMUX2.bit.GPIO94= 0;//ACT2
    GpioCtrlRegs.GPCMUX2.bit.GPIO94= 0;
    GpioCtrlRegs.GPCDIR.bit.GPIO94=1;
    GpioCtrlRegs.GPEGMUX1.bit.GPIO138= 0;//T0
    GpioCtrlRegs.GPEMUX1.bit.GPIO138= 0;
    GpioCtrlRegs.GPEDIR.bit.GPIO138=1;
    GpioCtrlRegs.GPEGMUX1.bit.GPIO136= 0;//T1
    GpioCtrlRegs.GPEMUX1.bit.GPIO136= 0;
    GpioCtrlRegs.GPEDIR.bit.GPIO136=1;
    GpioCtrlRegs.GPEGMUX1.bit.GPIO135= 0;//H0
    GpioCtrlRegs.GPEMUX1.bit.GPIO135= 0;
    GpioCtrlRegs.GPEDIR.bit.GPIO135=1;
    GpioCtrlRegs.GPEGMUX1.bit.GPIO134= 0;//H1
    GpioCtrlRegs.GPEMUX1.bit.GPIO134= 0;
    GpioCtrlRegs.GPEDIR.bit.GPIO134=1;
    GpioDataRegs.GPCSET.bit.GPIO68=1;
    GpioDataRegs.GPCSET.bit.GPIO94=1;
    GpioDataRegs.GPESET.bit.GPIO134=1;
    GpioDataRegs.GPESET.bit.GPIO135=1;
    GpioDataRegs.GPESET.bit.GPIO136=1;
    GpioDataRegs.GPESET.bit.GPIO138=1;
    GpioDataRegs.GPCCLEAR.bit.GPIO84=1;//选择通道0
    GpioDataRegs.GPDCLEAR.bit.GPIO125=1;
    GpioDataRegs.GPECLEAR.bit.GPIO137=1;
//-------------------AD_CTRL---------------------------
    GpioCtrlRegs.GPEGMUX1.bit.GPIO133= 0;//AD_CONVERT
    GpioCtrlRegs.GPEMUX1.bit.GPIO133= 0;
    GpioCtrlRegs.GPEDIR.bit.GPIO133=1;
    GpioCtrlRegs.GPDGMUX2.bit.GPIO119= 0;//ADRESET
    GpioCtrlRegs.GPDMUX2.bit.GPIO119= 0;
    GpioCtrlRegs.GPDDIR.bit.GPIO119=1;
    GpioCtrlRegs.GPEGMUX1.bit.GPIO132= 0;//AD_int
    GpioCtrlRegs.GPEMUX1.bit.GPIO132= 0;
    GpioCtrlRegs.GPEDIR.bit.GPIO132=0;
//-------------------DATA_CTRL---------------------------
    GpioCtrlRegs.GPDMUX2.bit.GPIO116= 0; //data_dir
    GpioCtrlRegs.GPDDIR.bit.GPIO116=1;
    GpioDataRegs.GPDSET.bit.GPIO116=1;
    GpioCtrlRegs.GPDGMUX2.bit.GPIO127= 0;//DATA_OE,用于控制外设上电延时时间;
    GpioCtrlRegs.GPDMUX2.bit.GPIO127= 0;
    GpioCtrlRegs.GPDDIR.bit.GPIO127=1;
    GpioDataRegs.GPDSET.bit.GPIO127=1;
    GpioCtrlRegs.GPEGMUX1.bit.GPIO129= 0;//YK_CTRL,采用反逻辑，在上电瞬间采用低电平，不使能遥控板输出,低电平，遥控板无输出，高电平，有输出
    GpioCtrlRegs.GPEMUX1.bit.GPIO129= 0;
    GpioCtrlRegs.GPEDIR.bit.GPIO129=1;
    GpioDataRegs.GPESET.bit.GPIO129=1;
//---------------XA19_0----------------------------
//---------------XD15_0----------------------------
    GpioCtrlRegs.GPCMUX2.bit.GPIO92 = 2;  // XA19
    GpioCtrlRegs.GPCMUX2.bit.GPIO91 = 2;  // XA18
    GpioCtrlRegs.GPCMUX2.bit.GPIO90 = 2;  // XA17
    GpioCtrlRegs.GPCMUX2.bit.GPIO89 = 2;  // XA16
    GpioCtrlRegs.GPCMUX2.bit.GPIO88 = 2;  // XA15
    GpioCtrlRegs.GPCMUX2.bit.GPIO87 = 2;  // XA14
    GpioCtrlRegs.GPCMUX2.bit.GPIO86 = 2;  // XA13
    GpioCtrlRegs.GPBMUX2.bit.GPIO52 = 2;  // XA12
    GpioCtrlRegs.GPBMUX2.bit.GPIO51 = 2;  // XA11
    GpioCtrlRegs.GPBMUX2.bit.GPIO50 = 2;  // XA10
    GpioCtrlRegs.GPBMUX2.bit.GPIO49 = 2;  // XA9
    GpioCtrlRegs.GPBMUX2.bit.GPIO48 = 2;  // XA8
    GpioCtrlRegs.GPBMUX1.bit.GPIO47 = 2;  // XA7
    GpioCtrlRegs.GPBMUX1.bit.GPIO46 = 2;  // XA6
    GpioCtrlRegs.GPBMUX1.bit.GPIO45 = 2;  // XA5
    GpioCtrlRegs.GPBMUX1.bit.GPIO44 = 2;  // XA4
    GpioCtrlRegs.GPBMUX1.bit.GPIO41 = 2;  // XA3
    GpioCtrlRegs.GPBMUX1.bit.GPIO40 = 2;  // XA2
    GpioCtrlRegs.GPBMUX1.bit.GPIO39 = 2;  // XA1
    GpioCtrlRegs.GPBMUX1.bit.GPIO38 = 2;  // XA0
    GpioCtrlRegs.GPCMUX1.bit.GPIO69 = 2;  // XD15
    GpioCtrlRegs.GPCMUX1.bit.GPIO70 = 2;  // XD14
    GpioCtrlRegs.GPCMUX1.bit.GPIO71 = 2;  // XD13
    GpioCtrlRegs.GPCMUX1.bit.GPIO72 = 2;  // XD12
    GpioCtrlRegs.GPCMUX1.bit.GPIO73 = 2;  // XD11
    GpioCtrlRegs.GPCMUX1.bit.GPIO74 = 2;  // XD10
    GpioCtrlRegs.GPCMUX1.bit.GPIO75 = 2;  // XD19
    GpioCtrlRegs.GPCMUX1.bit.GPIO76 = 2;  // XD8
    GpioCtrlRegs.GPCMUX1.bit.GPIO77 = 2;  // XD7
    GpioCtrlRegs.GPCMUX1.bit.GPIO78 = 2;  // XD6
    GpioCtrlRegs.GPCMUX1.bit.GPIO79 = 2;  // XD5
    GpioCtrlRegs.GPCMUX2.bit.GPIO80 = 2;  // XD4
    GpioCtrlRegs.GPCMUX2.bit.GPIO81 = 2;  // XD3
    GpioCtrlRegs.GPCMUX2.bit.GPIO82 = 2;  // XD2
    GpioCtrlRegs.GPCMUX2.bit.GPIO83 = 2;  // XD1
    GpioCtrlRegs.GPCMUX2.bit.GPIO85 = 2;  // XD0
    GpioCtrlRegs.GPBMUX1.bit.GPIO35 = 0;  // CS3
    GpioCtrlRegs.GPBMUX1.bit.GPIO34 = 2;  // CS2
    GpioCtrlRegs.GPAMUX2.bit.GPIO31 = 2;  // WE
    GpioCtrlRegs.GPBMUX1.bit.GPIO37 = 2;  // RD
    GpioCtrlRegs.GPBMUX1.bit.GPIO36 = 2;  // WAIT
    GpioCtrlRegs.GPADIR.bit.GPIO31 = 1;
    GpioCtrlRegs.GPBDIR.bit.GPIO35 = 1;  // CS3
    GpioCtrlRegs.GPBDIR.bit.GPIO34 = 1;  // CS2
    GpioCtrlRegs.GPADIR.bit.GPIO31 = 1;  // WE
    GpioCtrlRegs.GPBDIR.bit.GPIO37 = 1;  // RD
    GpioCtrlRegs.GPBDIR.bit.GPIO36 = 1;  // WAIT
    GpioCtrlRegs.GPCDIR.bit.GPIO88 = 1;  // XA15
    GpioCtrlRegs.GPCDIR.bit.GPIO87 = 1;  // XA14
    GpioCtrlRegs.GPCDIR.bit.GPIO86 = 1;  // XA13
    GpioCtrlRegs.GPBDIR.bit.GPIO52 = 1;  // XA12
    GpioCtrlRegs.GPBDIR.bit.GPIO51 = 1;  // XA11
    GpioCtrlRegs.GPBDIR.bit.GPIO50 = 1;  // XA10
    GpioCtrlRegs.GPBDIR.bit.GPIO49 = 1;  // XA9
    GpioCtrlRegs.GPBDIR.bit.GPIO48 = 1;  // XA8
    GpioCtrlRegs.GPBDIR.bit.GPIO47 = 1;  // XA7
    GpioCtrlRegs.GPBDIR.bit.GPIO46 = 1;  // XA6
    GpioCtrlRegs.GPBDIR.bit.GPIO45 = 1;  // XA5
    GpioCtrlRegs.GPBDIR.bit.GPIO44 = 1;  // XA4
    GpioCtrlRegs.GPBDIR.bit.GPIO41 = 1;  // XA3
    GpioCtrlRegs.GPBDIR.bit.GPIO40 = 1;  // XA2
    GpioCtrlRegs.GPBDIR.bit.GPIO39 = 1;  // XA1
    GpioCtrlRegs.GPBDIR.bit.GPIO38 = 1;  // XA0
    //-------------------------------------------------
    GPIO_SetupPinOptions(145, GPIO_OUTPUT, GPIO_PUSHPULL);//EPWM1
    GPIO_SetupPinMux(145, GPIO_MUX_CPU1, 0);
    GPIO_SetupPinOptions(146, GPIO_OUTPUT, GPIO_PUSHPULL);
    GPIO_SetupPinMux(146, GPIO_MUX_CPU1, 0);
    GPIO_SetupPinOptions(147, GPIO_OUTPUT, GPIO_PUSHPULL);//EPWM2
    GPIO_SetupPinMux(147, GPIO_MUX_CPU1, 0);
    GPIO_SetupPinOptions(148, GPIO_OUTPUT, GPIO_PUSHPULL);
    GPIO_SetupPinMux(148, GPIO_MUX_CPU1, 0);
    GPIO_SetupPinOptions(149, GPIO_OUTPUT, GPIO_PUSHPULL);//EPWM3
    GPIO_SetupPinMux(149, GPIO_MUX_CPU1, 0);
    GPIO_SetupPinOptions(150, GPIO_OUTPUT, GPIO_PUSHPULL);
    GPIO_SetupPinMux(150, GPIO_MUX_CPU1, 0);
    GPIO_SetupPinOptions(151, GPIO_OUTPUT, GPIO_PUSHPULL);//EPWM4
    GPIO_SetupPinMux(151, GPIO_MUX_CPU1, 0);
    GPIO_SetupPinOptions(152, GPIO_OUTPUT, GPIO_PUSHPULL);
    GPIO_SetupPinMux(152, GPIO_MUX_CPU1, 0);
    GPIO_SetupPinOptions(153, GPIO_OUTPUT, GPIO_PUSHPULL);//EPWM5
    GPIO_SetupPinMux(153, GPIO_MUX_CPU1, 0);
    GPIO_SetupPinOptions(154, GPIO_OUTPUT, GPIO_PUSHPULL);
    GPIO_SetupPinMux(154, GPIO_MUX_CPU1, 0);
    GPIO_SetupPinOptions(155, GPIO_OUTPUT, GPIO_PUSHPULL);//EPWM6
    GPIO_SetupPinMux(155, GPIO_MUX_CPU1, 0);
    GPIO_SetupPinOptions(156, GPIO_OUTPUT, GPIO_PUSHPULL);
    GPIO_SetupPinMux(156, GPIO_MUX_CPU1, 0);
    //--------------------------------------------------------------
    GPIO_SetupPinOptions(157, GPIO_OUTPUT, GPIO_PUSHPULL);//EPWM7
    GPIO_SetupPinMux(157, GPIO_MUX_CPU1, 0);
    GPIO_SetupPinOptions(158, GPIO_OUTPUT, GPIO_PUSHPULL);
    GPIO_SetupPinMux(158, GPIO_MUX_CPU1, 0);
    GPIO_SetupPinOptions(159, GPIO_OUTPUT, GPIO_PUSHPULL);//EPWM8
    GPIO_SetupPinMux(159, GPIO_MUX_CPU1, 0);
    GPIO_SetupPinOptions(160, GPIO_OUTPUT, GPIO_PUSHPULL);
    GPIO_SetupPinMux(160, GPIO_MUX_CPU1, 0);
    GPIO_SetupPinOptions(161, GPIO_OUTPUT, GPIO_PUSHPULL);//EPWM9
    GPIO_SetupPinMux(161, GPIO_MUX_CPU1, 0);
    GPIO_SetupPinOptions(162, GPIO_OUTPUT, GPIO_PUSHPULL);
    GPIO_SetupPinMux(162, GPIO_MUX_CPU1, 0);
    GPIO_SetupPinOptions(163, GPIO_OUTPUT, GPIO_PUSHPULL);//EPWM10
    GPIO_SetupPinMux(163, GPIO_MUX_CPU1, 0);
    GPIO_SetupPinOptions(164, GPIO_OUTPUT, GPIO_PUSHPULL);
    GPIO_SetupPinMux(164, GPIO_MUX_CPU1, 0);
    GPIO_SetupPinOptions(165, GPIO_OUTPUT, GPIO_PUSHPULL);//EPWM11
    GPIO_SetupPinMux(165, GPIO_MUX_CPU1, 0);
    GPIO_SetupPinOptions(166, GPIO_OUTPUT, GPIO_PUSHPULL);
    GPIO_SetupPinMux(166, GPIO_MUX_CPU1, 0);
    GPIO_SetupPinOptions(167, GPIO_OUTPUT, GPIO_PUSHPULL);//EPWM12
    GPIO_SetupPinMux(167, GPIO_MUX_CPU1, 0);
    GPIO_SetupPinOptions(168, GPIO_OUTPUT, GPIO_PUSHPULL);
    GPIO_SetupPinMux(168, GPIO_MUX_CPU1, 0);
    //----------------------------EPWM7_12使能------------------------
    GPIO_SetupPinOptions(118, GPIO_OUTPUT, GPIO_PUSHPULL);
    GPIO_SetupPinMux(118, GPIO_MUX_CPU1, 0);
    GpioDataRegs.GPDCLEAR.bit.GPIO118=1;
    //---------------------------------------------------------------
    GpioCtrlRegs.GPBPUD.bit.GPIO58 = 0;    // Enable pull-up on GPIO12 (TZ1)
    GpioCtrlRegs.GPBQSEL2.bit.GPIO58= 3;  // Asynch input  (TZ1)
    InputXbarRegs.INPUT1SELECT = 58;  //GPIO58 is input1 source; EPWM(TZ1)
    //-----------------------------CAP3_4口配置---------------------------
    //-------------------LED_CTRL----------------------------------------
    GPIO_SetupPinOptions(33, GPIO_OUTPUT, GPIO_PUSHPULL);
    GPIO_SetupPinMux(33, GPIO_MUX_CPU1, 0);
    GPIO_SetupPinOptions(124, GPIO_OUTPUT, GPIO_PUSHPULL);
    GPIO_SetupPinMux(124, GPIO_MUX_CPU1, 0);
    //--------------------测试T0占空比----------------------------------------
    GPIO_SetupPinOptions(131, GPIO_OUTPUT, GPIO_PUSHPULL);//MGP4
    GPIO_SetupPinMux(131, GPIO_MUX_CPU1, 0);
    EDIS;
}


//Set the peripheral muxing for the specified pin. The appropriate
//parameters can be found in the pinout spreadsheet.
void GPIO_SetupPinMux(Uint16 pin, Uint16 cpu, Uint16 peripheral)
{
    volatile Uint32 *gpioBaseAddr;
    volatile Uint32 *mux, *gmux, *csel;
    Uint16 pin32, pin16, pin8;
    pin32 = pin % 32;
    pin16 = pin % 16;
    pin8 = pin % 8;
    gpioBaseAddr = (Uint32 *)&GpioCtrlRegs + (pin/32)*GPY_CTRL_OFFSET;
    //Sanity check for valid cpu and peripheral values
    if (cpu > GPIO_MUX_CPU2CLA || peripheral > 0xF)
    {
        return;
    }
    //Create pointers to the appropriate registers. This is a workaround
    //for the way GPIO registers are defined. The standard definition
    //in the header file makes it very easy to do named accesses of one
    //register or bit, but hard to do arbitrary numerical accesses. It's
    //easier to have an array of GPIO modules with identical registers,
    //including arrays for multi-register groups like GPyCSEL1-4. But
    //the header file doesn't define anything we can turn into an array,
    //so manual pointer arithmetic is used instead.
    mux = gpioBaseAddr + GPYMUX + pin32/16;
    gmux = gpioBaseAddr + GPYGMUX + pin32/16;
    csel = gpioBaseAddr + GPYCSEL + pin32/8;
    //Now for the actual function
    EALLOW;
    //To change the muxing, set the peripheral mux to 0/GPIO first to avoid
    //glitches, then change the group mux, then set the peripheral mux to
    //its target value. Finally, set the CPU select. This procedure is
    //described in the TRM. Unfortunately, since we don't know the pin in
    //advance we can't hardcode a bitfield reference, so there's some tricky
    //bit twiddling here.
    *mux &= ~(0x3UL << (2*pin16));
    *gmux &= ~(0x3UL << (2*pin16));
    *gmux |= (Uint32)((peripheral >> 2) & 0x3UL) << (2*pin16);
    *mux |= (Uint32)(peripheral & 0x3UL) << (2*pin16);
    *csel &= ~(0x3L << (4*pin8));
    *csel |= (Uint32)(cpu & 0x3L) << (4*pin8);
    //WARNING: This code does not touch the analog mode select registers,
    //which are needed to give the USB module control of its IOs.
    EDIS;
}


//Setup up the GPIO input/output options for the specified pin.
//The flags are a 16-bit mask produced by ORing together options.
//For input pins, the valid flags are:
//GPIO_PULLUP   Enable pull-up
//GPIO_INVERT   Enable input polarity inversion
//GPIO_SYNC     Synchronize the input latch to PLLSYSCLK (default -- you don't need to specify this)
//GPIO_QUAL3    Use 3-sample qualification
//GPIO_QUAL6    Use 6-sample qualification
//GPIO_ASYNC    Do not use synchronization or qualification
//(Note: only one of SYNC, QUAL3, QUAL6, or ASYNC is allowed)
//
//For output pins, the valid flags are:
//GPIO_OPENDRAIN    Output in open drain mode
//GPIO_PULLUP       If open drain enabled, also enable the pull-up
//and the input qualification flags (SYNC/QUAL3/QUAL6/SYNC) listed above.
//
//With no flags, the default input state is synchronous with no
//pull-up or polarity inversion. The default output state is
//the standard digital output.
void GPIO_SetupPinOptions(Uint16 pin, Uint16 output, Uint16 flags)
{
    volatile Uint32 *gpioBaseAddr;
    volatile Uint32 *dir, *pud, *inv, *odr, *qsel;
    Uint32 pin32, pin16, pinMask, qual;
    pin32 = pin % 32;
    pin16 = pin % 16;
    pinMask = 1UL << pin32;
    gpioBaseAddr = (Uint32 *)&GpioCtrlRegs + (pin/32)*GPY_CTRL_OFFSET;
    //Create pointers to the appropriate registers. This is a workaround
    //for the way GPIO registers are defined. The standard definition
    //in the header file makes it very easy to do named accesses of one
    //register or bit, but hard to do arbitrary numerical accesses. It's
    //easier to have an array of GPIO modules with identical registers,
    //including arrays for multi-register groups like GPyQSEL1-2. But
    //the header file doesn't define anything we can turn into an array,
    //so manual pointer arithmetic is used instead.
    dir = gpioBaseAddr + GPYDIR;
    pud = gpioBaseAddr + GPYPUD;
    inv = gpioBaseAddr + GPYINV;
    odr = gpioBaseAddr + GPYODR;
    qsel = gpioBaseAddr + GPYQSEL + pin32/16;
    EALLOW;
    //Set the data direction
    *dir &= ~pinMask;
    if (output == 1)
    {
        //Output, with optional open drain mode and pull-up
        *dir |= pinMask;
        //Enable open drain if necessary
        if (flags & GPIO_OPENDRAIN)
        {
            *odr |= pinMask;
        }
        else
        {
            *odr &= ~pinMask;
        }
        //Enable pull-up if necessary. Open drain mode must be active.
        if (flags & (GPIO_OPENDRAIN | GPIO_PULLUP))
        {
            *pud &= ~pinMask;
        }
        else
        {
            *pud |= pinMask;
        }
    }
    else
    {
        //Input, with optional pull-up, qualification, and polarity inversion
        *dir &= ~pinMask;
        //Enable pull-up if necessary
        if (flags & GPIO_PULLUP)
        {
            *pud &= ~pinMask;
        }
        else
        {
            *pud |= pinMask;
        }
        //Invert polarity if necessary
        if (flags & GPIO_INVERT)
        {
            *inv |= pinMask;
        }
        else
        {
            *inv &= ~pinMask;
        }
    }
    //Extract the qualification parameter and load it into the register. This is
    //also needed for open drain outputs, so we might as well do it all the time.
    qual = (flags & GPIO_ASYNC) / GPIO_QUAL3;
    *qsel &= ~(0x3L << (2 * pin16));
    if (qual != 0x0)
    {
        *qsel |= qual << (2 * pin16);
    }
    EDIS;
}

//Enable or disable the GPIO register bit lock for the specified pin.
//The valid flags are:
//GPIO_UNLOCK   Unlock the pin setup register bits for the specified pin
//GPIO_LOCK     Lock the pin setup register bits for the specified pin
void GPIO_SetupLock(Uint16 pin, Uint16 flags)
{
    volatile Uint32 *gpioBaseAddr;
    volatile Uint32 *lock;
    Uint32 pin32, pinMask;
    pin32 = pin % 32;
    pinMask = 1UL << pin32;
    gpioBaseAddr = (Uint32 *)&GpioCtrlRegs + (pin/32)*GPY_CTRL_OFFSET;
    //Create pointers to the appropriate registers. This is a workaround
    //for the way GPIO registers are defined. The standard definition
    //in the header file makes it very easy to do named accesses of one
    //register or bit, but hard to do arbitrary numerical accesses. It's
    //easier to have an array of GPIO modules with identical registers,
    //including arrays for multi-register groups like GPyQSEL1-2. But
    //the header file doesn't define anything we can turn into an array,
    //so manual pointer arithmetic is used instead.
    lock = gpioBaseAddr + GPYLOCK;
    EALLOW;
    if(flags)
    {
        //Lock the pin
        *lock |= pinMask;
    }
    else
    {
        //Unlock the pin
        *lock &= ~pinMask;
    }
    EDIS;
}

//External interrupt setup
void GPIO_SetupXINT1Gpio(Uint16 pin)
{
    EALLOW;
    InputXbarRegs.INPUT4SELECT = pin;       //Set XINT1 source to GPIO-pin
    EDIS;
}
void GPIO_SetupXINT2Gpio(Uint16 pin)
{
    EALLOW;
    InputXbarRegs.INPUT5SELECT = pin;       //Set XINT2 source to GPIO-pin
    EDIS;
}
void GPIO_SetupXINT3Gpio(Uint16 pin)
{
    EALLOW;
    InputXbarRegs.INPUT6SELECT = pin;       //Set XINT3 source to GPIO-pin
    EDIS;
}
void GPIO_SetupXINT4Gpio(Uint16 pin)
{
    EALLOW;
    InputXbarRegs.INPUT13SELECT = pin;  //Set XINT4 source to GPIO-pin
    EDIS;
}
void GPIO_SetupXINT5Gpio(Uint16 pin)
{
    EALLOW;
    InputXbarRegs.INPUT14SELECT = pin;  //Set XINT5 source to GPIO-pin
    EDIS;
}


//Enable pullups for the unbonded GPIOs on the 176PTP package:
//GPIOs     Grp Bits
//95-132    C   31
//          D   31:0
//          E   4:0
//134-168   E   31:6
//          F   8:0
void GPIO_EnableUnbondedIOPullupsFor176Pin()
{
    EALLOW;
    GpioCtrlRegs.GPCPUD.all = ~0x80000000;  //GPIO 95
    GpioCtrlRegs.GPDPUD.all = ~0xFFFFFFF7;  //GPIOs 96-127
    GpioCtrlRegs.GPEPUD.all = ~0xFFFFFFDF;  //GPIOs 128-159 except for 133
    GpioCtrlRegs.GPFPUD.all = ~0x000001FF;  //GPIOs 160-168
    EDIS;
}

//Enable pullups for the unbonded GPIOs on the 100PZ package:
//GPIOs     Grp Bits
//0-1       A   1:0
//5-9       A   9:5
//22-40     A   31:22
//          B   8:0
//44-57     B   25:12
//67-68     C   4:3
//74-77     C   13:10
//79-83     C   19:15
//93-168    C   31:29
//          D   31:0
//          E   31:0
//          F   8:0
void GPIO_EnableUnbondedIOPullupsFor100Pin()
{
    EALLOW;
    GpioCtrlRegs.GPAPUD.all = ~0xFFC003E3;  //GPIOs 0-1, 5-9, 22-31
    GpioCtrlRegs.GPBPUD.all = ~0x03FFF1FF;  //GPIOs 32-40, 44-57
    GpioCtrlRegs.GPCPUD.all = ~0xE10FBC18;  //GPIOs 67-68, 74-77, 79-83, 93-95
    GpioCtrlRegs.GPDPUD.all = ~0xFFFFFFF7;  //GPIOs 96-127
    GpioCtrlRegs.GPEPUD.all = ~0xFFFFFFFF;  //GPIOs 128-159
    GpioCtrlRegs.GPFPUD.all = ~0x000001FF;  //GPIOs 160-168
    EDIS;
}

//InitSysCtrl would call this function this takes care of stuff.
void GPIO_EnableUnbondedIOPullups()
{
    unsigned char pin_count = ((DevCfgRegs.PARTIDL.all & 0x00000700) >> 8) ; //bits 8-10 have pin count*/
    /*
     * 5 = 100 pin
     * 6 = 176 pin
     * 7 = 337 pin
     */
    if(pin_count == 5)
    {
        GPIO_EnableUnbondedIOPullupsFor100Pin();
    }
    else if (pin_count == 6)
    {
        GPIO_EnableUnbondedIOPullupsFor176Pin();
    }
    else
    {
        //do nothing - this is 337 pin package
    }
}



#endif //CPU1


//Read the GPyDAT register bit for the specified pin. Note that this returns
//the actual state of the pin, not the state of the output latch.
Uint16 GPIO_ReadPin(Uint16 pin)
{
    volatile Uint32 *gpioDataReg;
    Uint16 pinVal;
    gpioDataReg = (volatile Uint32 *)&GpioDataRegs + (pin/32)*GPY_DATA_OFFSET;
    pinVal = (gpioDataReg[GPYDAT] >> (pin % 32)) & 0x1;
    return pinVal;
}

//Set the GPyDAT register bit for the specified pin.
void GPIO_WritePin(Uint16 pin, Uint16 outVal)
{
    volatile Uint32 *gpioDataReg;
    Uint32 pinMask;
    gpioDataReg = (volatile Uint32 *)&GpioDataRegs + (pin/32)*GPY_DATA_OFFSET;
    pinMask = 1UL << (pin % 32);
    if (outVal == 0)
    {
        gpioDataReg[GPYCLEAR] = pinMask;
    }
    else
    {
        gpioDataReg[GPYSET] = pinMask;
    }
}




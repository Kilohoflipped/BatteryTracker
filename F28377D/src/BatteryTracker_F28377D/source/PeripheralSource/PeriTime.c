/*
 * PeriTime.c
 *
 *  Created on: 2023Äê7ÔÂ28ÈÕ
 *      Author: Administrator
 */

#include "F28x_Project.h"
#include "PeriDevices.h"

Uint32 LSPCLKFreq;

Uint32 LSPCLKCal()
{
    Uint32 LSPCLKFreq = 0;
    switch(ClkCfgRegs.LOSPCP.bit.LSPCLKDIV)
    {
        case(0):
            LSPCLKFreq = SYSCLKFreq/LSPCLKDIVNUM_0;
        case(1):
            LSPCLKFreq = SYSCLKFreq/LSPCLKDIVNUM_1;
        case(2):
            LSPCLKFreq = SYSCLKFreq/LSPCLKDIVNUM_2;
        case(3):
            LSPCLKFreq = SYSCLKFreq/LSPCLKDIVNUM_3;
        case(4):
            LSPCLKFreq = SYSCLKFreq/LSPCLKDIVNUM_4;
        case(5):
            LSPCLKFreq = SYSCLKFreq/LSPCLKDIVNUM_5;
        case(6):
            LSPCLKFreq = SYSCLKFreq/LSPCLKDIVNUM_6;
        case(7):
            LSPCLKFreq = SYSCLKFreq/LSPCLKDIVNUM_7;
    }
    return LSPCLKFreq;
}

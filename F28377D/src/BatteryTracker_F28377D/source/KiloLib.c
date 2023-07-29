/*
 * KiloLib.c
 *
 *  Created on: 2023年7月28日
 *      Author: Administrator
 */

#include "F28x_Project.h"
#include "KiloLib.h"

void Int2Str(Uint32 IntNum,char* CharNum) // IntNum<=65535
{
    Uint16 i=0;
    Uint16 factor = 10000;
    for (i=0;i<5;i++)
    {
        CharNum[i] = (char)(IntNum/factor)+('0');
        IntNum /= 10;
        factor /= 10;
    }
    CharNum[5] = '\0';  //添加结束符
}

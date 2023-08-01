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

void ftoa(Uint16 *p,float Val)
{
    float32 Temp;
    Uint16 i,Sign;
    float n;
    if(Val<0)
    {
        Sign=1;
        Val=-Val;
    }
    else
    {
        Sign=0;
    }
    if(Val>=1000)
    {
        Temp=1000;
    }
    else if(Val>=100)
    {
        Temp=100;
    }
    else if(Val>=10)
    {
        Temp=10;
    }
    else
    {
        Temp=1;
    }
    if(Sign)
    {
        *p++='-';
    }
    for(i=0; i<4; i++)
    {
        n=Val/Temp;
        *p++=(Uint16)n+0x30;
        Val-=(Uint16)n*Temp;
        if(Temp==1)
        {
            *p++='.';
        }
        Temp=Temp/10;
        if((i==2)&(Sign))
        {
            break;
        }
    }
    *p=0;
}

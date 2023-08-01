/*
 * PeriSCI.h
 *
 *  Created on: 2023Äê7ÔÂ28ÈÕ
 *      Author: Administrator
 */

#ifndef INCLUDE_PERIPHERALHEADERS_PERISCI_H_
#define INCLUDE_PERIPHERALHEADERS_PERISCI_H_

void ConfigureSciA(void);
void ConfigureSciaFifo(void);
void BaudCalculate(Uint32 desiredBaudRate,Uint32* BaudInf);
void SciaXmit(int data);
void SciaMsg(char * msg);

#endif /* INCLUDE_PERIPHERALHEADERS_PERISCI_H_ */

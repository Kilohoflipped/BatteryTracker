/*
 * PeriSCI.h
 *
 *  Created on: 2023Äê7ÔÂ28ÈÕ
 *      Author: Administrator
 */

#ifndef INCLUDE_PERIPHERALHEADERS_PERISCI_H_
#define INCLUDE_PERIPHERALHEADERS_PERISCI_H_

void ConfigureSciA();
void SCIAFIFOSetup();
void BaudCalculate(Uint32 desiredBaudRate,Uint32* BaudInf);
void SCIAXmit(int data);
void SCIAMsg(char* msg);

#endif /* INCLUDE_PERIPHERALHEADERS_PERISCI_H_ */

/*
 * PeriADC.h
 *
 *  Created on: 2023��7��25��
 *      Author: Administrator
 */

#ifndef INCLUDE_PERIPHERALHEADERS_PERIADC_H_
#define INCLUDE_PERIPHERALHEADERS_PERIADC_H_

void ConfigureADC(void);
void SetupADCChannel();
interrupt void ADCA_INT1_ISR(void);

#endif /* INCLUDE_PERIPHERALHEADERS_PERIADC_H_ */

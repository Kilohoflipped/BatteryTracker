/*
 * sci_zzy.h
 *
 *  Created on: 2019Äê2ÔÂ20ÈÕ
 *      Author: zzy
 */

#ifndef INCLUDE_SCI_ZZY_H_
#define INCLUDE_SCI_ZZY_H_


//
// Function Prototypes
//
extern void ConfigureSciA(void);
extern void scia_fifo_init(void);
extern void scia_xmit(int a);
extern void scia_msg(char *msg);
void ftoa(char *p,float Val);




#endif /* INCLUDE_SCI_ZZY_H_ */

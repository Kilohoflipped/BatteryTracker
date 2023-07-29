/*
 * PeriTime.h
 *
 *  Created on: 2023��7��28��
 *      Author: Administrator
 */

#ifndef INCLUDE_PERIPHERALHEADERS_PERITIME_H_
#define INCLUDE_PERIPHERALHEADERS_PERITIME_H_

//��Ƶ200MHz
#define SYSCLKFreq 200000000

// �����������ʱ�ӷ�Ƶϵ��ѡ��Ĵ���ȡֵ��Ӧ�ķ�Ƶϵ��
#define LSPCLKDIVNUM_0 1
#define LSPCLKDIVNUM_1 2
#define LSPCLKDIVNUM_2 4
#define LSPCLKDIVNUM_3 6
#define LSPCLKDIVNUM_4 8
#define LSPCLKDIVNUM_5 10
#define LSPCLKDIVNUM_6 12
#define LSPCLKDIVNUM_7 14

Uint32 LSPCLKCal();
Uint32 LSPCLKFreq;

#endif /* INCLUDE_PERIPHERALHEADERS_PERITIME_H_ */

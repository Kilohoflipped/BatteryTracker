/*
 * spirom.h
 *
 *  Created on: 2016Äê6ÔÂ5ÈÕ
 *      Author: Administrator
 */

#ifndef SPIROM_H_
#define SPIROM_H_
void Write_EEPROM(int Write_Addr,Uint16  *lpByte,int Write_Lens,Uint16  AC_No);
void Write_Word_To_EEPROM(int Write_Addr,Uint16 Write_Byte,Uint16  AC_No);
void Read_EEPROM(int Read_Addr,Uint16  *lpByte,int Read_Lens,Uint16  AC_No);
void Read_Word_From_EEPROM(int Read_Addr,Uint16 *lpByte,Uint16 AC_No);
void Channel_Check_Allow_Charge(Uint16  Line_index);
void Line_Value_Load(Uint16  Line_index);
void Write_PDZ_Para_To_EEPROM(void);
void Read_PDZ_Para_From_EEPROM(void);
void Write_Channel_Para_To_EEPROM(void);
void Read_Channel_Para_From_EEPROM(void);
void Write_Channel_Line_To_EEPROM(void);
void Read_Channel_Line_From_EEPROM(void);
void Write_Pro_Value_To_EEPROM(void);
void Read_Pro_Value_From_EEPROM(Uint16  Line_index);
void Write_Pro_Control_To_EEPROM(void);
void Read_Pro_Control_From_EEPROM(Uint16  Line_index);
void Write_Additional_Value_To_EEPROM(void);
void Read_Additional_Value_From_EEPROM(Uint16  Line_index);
void Write_Additional_Control_To_EEPROM(void);
void Read_Additional_Control_From_EEPROM(Uint16  Line_index);
void Write_Line_Para_To_EEPROM(Uint16  Line_index);
void Read_Line_Para_From_EEPROM(Uint16  Line_index);





#endif /* SPIROM_H_ */

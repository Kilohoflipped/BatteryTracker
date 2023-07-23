/*
 * SPI.h
 *
 *  Created on: 2016Äê4ÔÂ22ÈÕ
 *      Author: Administrator
 */

#ifndef SPI_H_
#define SPI_H_
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
#endif /* SPI_H_ */

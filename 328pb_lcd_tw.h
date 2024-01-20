/* 
 * File:   328pb_lcd_tw.h
 * Author: DONY THOMAS
 *
 * Created on 7 January, 2024, 2:00 PM
 */

#ifndef ATMEGA328PB_LCD_TW_H
#define	ATMEGA328PB_LCD_TW_H

void lcd_init(void);
void lcd_send_cmd(unsigned char cmd);
void lcd_send_character(unsigned char character);
void lcd_send_string(const char* string);
void lcd_send_byte(unsigned char byte);

#endif	/* ATMEGA328PB_LCD_TW_H */


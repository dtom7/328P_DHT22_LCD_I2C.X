/* 
 * File:   328pb_tw_mt.h
 * Author: DONY THOMAS
 *
 * Created on 7 January, 2024, 11:48 AM
 */

/* 
	
    PCF8574 has 8-Bit Port for I/O operation 
    P0 = RS;		//--- Register Select
    P1 = RW;		//--- Read / Write Operation Select
    P2 = EN;		//--- Latch to Data Register Enable Pin
    P3 = BL;	    //--- LCD Backlight Control
    P4 = D4;		//--- LCD pin D4
    P5 = D5;		//--- LCD pin D5
    P6 = D6;		//--- LCD pin D6
    P7 = D7;		//--- LCD pin D7	

 */

#define F_CPU 16000000UL // 16 MHz

#include "328pb_lcd_tw.h"
#include "328pb_tw_mt.h"
#include <avr/io.h>
#include <util/delay.h>

volatile unsigned char lcd = 0x00;

void lcd_init(void) {
    tw0_init();
    lcd = 0x04; //--- EN = 1 for 25us initialize Sequence
    lcd_send_byte(lcd);
    _delay_ms(25);
    lcd_send_cmd(0x03); //--- Initialize Sequence
    lcd_send_cmd(0x03); //--- Initialize Sequence
    lcd_send_cmd(0x03); //--- Initialize Sequence
    lcd_send_cmd(0x02); //--- Return to Home
    lcd_send_cmd(0x28); //--- 16X2 LCD in 4 bit mode
    lcd_send_cmd(0x0C); //--- Display on, cursor off
    lcd_send_cmd(0x01); //--- Clear LCD
    lcd_send_cmd(0x06); //--- Auto increment Cursor
    lcd_send_cmd(0x80); //--- Cursor at row 1 column 1 position
}

void lcd_send_nibble(unsigned char nibble) {
    unsigned char temp = 0x00; //--- temp variable for data operation

    lcd &= 0x0F; //--- Masking last four bit to prevent the RS, RW, EN, Backlight
    temp = (nibble & 0xF0); //--- Masking higher 4-Bit of Data and send it LCD
    lcd |= temp; //--- 4-Bit Data and LCD control Pin
    lcd |= (0x04); //--- Latching Data to LCD EN = 1
    lcd_send_byte(lcd); //--- Send Data From PCF8574 to LCD PORT
    _delay_ms(5); //--- 1us Delay
    lcd &= ~(0x04); //--- Latching Complete
    lcd_send_byte(lcd); //--- Send Data From PCF8574 to LCD PORT 
    _delay_ms(5); //--- 5us Delay to Complete Latching

    temp = ((nibble & 0x0F) << 4); //--- Masking Lower 4-Bit of Data and send it LCD
    lcd &= 0x0F; //--- Masking last four bit to prevent the RS, RW, EN, Backlight					
    lcd |= temp; //--- 4-Bit Data and LCD control Pin
    lcd |= (0x04); //--- Latching Data to LCD EN = 1
    lcd_send_byte(lcd); //--- Send Data From PCF8574 to LCD PORT
    _delay_ms(5); //--- 1us Delay
    lcd &= ~(0x04); //--- Latching Complete
    lcd_send_byte(lcd); //--- Send Data From PCF8574 to LCD PORT
    _delay_ms(5); //--- 5us Delay to Complete Latching
}

void lcd_send_cmd(unsigned char cmd) {
    lcd = 0x08; //--- Enable Backlight Pin
    lcd &= ~(0x01); //--- Select Command Register By RS = 0
    lcd_send_byte(lcd); //--- Send Data From PCF8574 to LCD PORT
    lcd_send_nibble(cmd); //--- Function to Write 4-bit data to LCD ;
}

void lcd_send_character(unsigned char character) {
    lcd |= 0x09; //--- Enable Backlight Pin & Select Data Register By RS = 1
    lcd_send_byte(lcd); //--- Send Data From PCF8574 to LCD PORT	
    lcd_send_nibble(character); //--- Function to Write 4-bit data to LCD
}

void lcd_send_string(const char* string) {
    while (*string != '\0')
        lcd_send_character(*string++);
}

void lcd_send_byte(unsigned char byte) {
    tw0_start();
    tw0_send_slaw(0x27);
    tw0_send_byte(byte);
    tw0_stop();
}



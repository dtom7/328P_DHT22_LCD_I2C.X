/*
 * File:   main.c
 * Author: DONY THOMAS
 *
 * Created on 13 January, 2024, 5:11 PM
 */

#define F_CPU 16000000UL // 16 MHz

#include "328pb_lcd_tw.h"
#include "328pb_dht22.h"
#include <util/delay.h>
#include <stdio.h>

int main(void) {
    unsigned char rh_high, rh_low, t_high, t_low, checksum; // 8 bits
    double temperature, relative_humidity; // 32 bits
    unsigned char read_error = 0;
    lcd_init();
    while (1) {
        _delay_ms(2000); // check DHT22 every 2s
        start_dht22();
        read_dht22(&rh_high);
        read_dht22(&rh_low);
        read_dht22(&t_high);
        read_dht22(&t_low);
        read_dht22(&checksum);
        if (((unsigned char) (rh_high + rh_low + t_high + t_low)) != checksum) {
            read_error = 1;
            lcd_send_cmd(0x01); // clear lcd
            lcd_send_cmd(0x80); //--- Cursor at row 1 column 1 position
            lcd_send_string("** READ ERROR **");
        } else {
            if (read_error == 1) {
                lcd_send_cmd(0x01); // clear lcd
                read_error = 0;
            }
            relative_humidity = (((unsigned int) rh_high << 8) + rh_low) / 10.0;
            char str_buffer_1[25];
            if (relative_humidity >= 100.0) {
                sprintf(str_buffer_1, "RH:%.1f PERCENT", relative_humidity);
            } else {
                sprintf(str_buffer_1, "RH: %.1f PERCENT", relative_humidity);
            }
            lcd_send_cmd(0x80); //--- Cursor at row 1 column 1 position
            lcd_send_string(str_buffer_1);
            char str_buffer_2[25];
            if (t_high & (1 << 7)) { // check bit 7 of t_high is set
                t_high &= ~(1 << 7); //clear bit 7 of t_high 
                temperature = (((unsigned int) t_high << 8) + t_low) / 10.0;
                sprintf(str_buffer_2, "TP:-%04.1f CELCIUS", temperature); // negative temperature
            } else {
                temperature = (((unsigned int) t_high << 8) + t_low) / 10.0;
                sprintf(str_buffer_2, "TP: %04.1f CELCIUS", temperature);
            }
            lcd_send_cmd(0xC0); //--- Cursor at row 2 column 1 position
            lcd_send_string(str_buffer_2);
        }
    }
}
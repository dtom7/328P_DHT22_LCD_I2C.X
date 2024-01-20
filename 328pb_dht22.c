/*
 * File:   328pb_dht22.c
 * Author: DONY THOMAS
 *
 * Created on 8 January, 2024, 8:42 PM
 */
#define F_CPU 16000000UL // 16 MHz

#include "328pb_dht22.h"
#include <avr/io.h>
#include <util/delay.h>

void start_dht22(void) {
    DDRB |= (1 << PB2); // PB2 - DHT22 data line - out
    PORTB &= ~(1 << PB2); // PB2 - low
    _delay_us(1200); // wait
    PORTB |= (1 << PB2); // PB2 - high
    _delay_us(40); // wait
    DDRB &= ~(1 << PB2); // PB2 - DHT22 data line - in
    PORTB |= (1 << PB2); // PB2 - enable pullup resistor
    _delay_us(40); // wait
    while (PINB & (1 << PB2)); // wait until PB2 - low
    while (!(PINB & (1 << PB2))); // wait until PB2 - high
    while (PINB & (1 << PB2)); // wait until PB2 - low
}

void read_dht22(unsigned char* byte) {
    for (unsigned char i = 0; i < 8; i++) {
        while (!(PINB & (1 << PB2))); // wait until PB2 - high
        _delay_us(50); // wait
        if (PINB & (1 << PB2)) {
            *byte = (*byte << 1) | 0x01; // bit is 1
        } else {
            *byte = (*byte << 1); // bit is 0
        }
        while (PINB & (1 << PB2)); // wait until PB2 - low
    }
}

/* 
 * File:   328pb_tw_mt.h
 * Author: DONY THOMAS
 *
 * Created on 7 January, 2024, 11:48 AM
 */
#define F_CPU 16000000UL // 16 MHz
#define TW_FREQ	100000UL // 100 KHz
#define TW_SCL0	PC5
#define TW_SDA0	PC4

#include "328pb_tw_mt.h"
#include <avr/io.h>
#include <util/twi.h>

void tw0_init() {
    PORTC |= (1 << TW_SCL0) | (1 << TW_SDA0); // enable internal pull-up resistors
    TWSR &= ~(1 << TWPS1) & ~(1 << TWPS1); // set prescaler to 1
    TWBR = ((F_CPU / TW_FREQ) - 16) / 2; // set bit rate register   
}

char tw0_start() {
    TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
    while (!(TWCR & (1 << TWINT))); // wait for interrupt
    if ((TWSR & 0xF8) != TW_START) {
        return -1;
    } else {
        return 1;
    }
}

void tw0_stop() {
    TWCR = (1 << TWINT) | (1 << TWSTO) | (1 << TWEN);
}

char tw0_send_slaw(char sla) {
    unsigned char tw_slarw = 0x00;
    tw_slarw = TW_WRITE;
    tw_slarw |= sla << 1; // slave address
    TWDR = tw_slarw;
    TWCR = (1 << TWINT) | (1 << TWEN); // send sla+w
    while (!(TWCR & (1 << TWINT))); // wait for interrupt
    if ((TWSR & 0xF8) != TW_MT_SLA_ACK) {
        return -1;
    } else {
        return 1;
    }
}

char tw0_send_byte(char byte) {
    TWDR = byte;
    TWCR = (1 << TWINT) | (1 << TWEN); // send byte
    while (!(TWCR & (1 << TWINT))); // wait for interrupt
    if ((TWSR & 0xF8) != TW_MT_DATA_ACK) {
        return -1;
    } else {
        return 1;
    }
}

/* 
 * File:   328pb_tw_mt.h
 * Author: DONY THOMAS
 *
 * Created on 7 January, 2024, 11:48 AM
 */

#ifndef ATMEGA328PB_TW_MT_H
#define	ATMEGA328PB_TW_MT_H

void tw0_init();
char tw0_start();
char tw0_send_slaw(char sla);
char tw0_send_byte(char byte);
void tw0_stop();

#endif	/* ATMEGA328PB_TW_MT_H */


/*
 * serial_manager.h
 *
 * Created: 2022/01/21 22:32:01
 *  Author: Owner
 */ 


#ifndef SERIAL_MANAGER_H_
#define SERIAL_MANAGER_H_

#include "avr/io.h"
#include "avr/interrupt.h"
#include "xprintf.h"
#include "ringbuff.h"

#define F_CPU 3333333UL
#define BAUD_RATE(BAUD_RATE) ((float)((F_CPU) * 64 / (16 * (float)BAUD_RATE)) + 0.5)

extern RingBuff_t midi_buffer;

void	init_serial(void);			//シリアル通信の初期化

void	debug_putc(uint8_t chr);	//1文字送信
uint8_t debug_getc(void);			//1文字受信

#endif /* SERIAL_MANAGER_H_ */
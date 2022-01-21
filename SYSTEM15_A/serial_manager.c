/*
 * serial_manager.c
 *
 * Created: 2022/01/21 22:32:10
 *  Author: Owner
 */ 

#include "serial_manager.h"

RingBuff_t midi_buffer;

void	init_serial(void){
	cli();
	
	/*USART0(MIDI)*/
	USART0.BAUD			 =	BAUD_RATE(31250);	/*ボーレートを31250bpsに設定*/
	USART0.CTRLC		 =	0x03;				/*N81,調歩同期*/
	USART0.CTRLB		 =	0xC0;				/*USART0 TX&RX有効化*/
	PORTA.DIR			|=	0x01;				/*(TX(PA0)を出力に設定*/
	USART0.CTRLA		 =	0xC0;				/*USART0 TX&RX割り込み有効化*/
	
	/*USART2(デバッグ)*/
	PORTMUX.USARTROUTEA	 =	0x10;				/*USART2をPF4~6に割り当て*/
	USART2.BAUD			 =	BAUD_RATE(38400);	/*ボーレートを38400bpsに設定*/
	USART2.CTRLC		 =	0x03;				/*N81,調歩同期*/
	PORTF.DIR			|=	0x10;				/*TX(PF4)を出力に設定*/
	USART2.CTRLB		 =	0xC0;				/*USART2 TX&RX有効化*/
	
	sei();
	
	/*printfライブラリの設定*/
	xdev_in(debug_getc);
	xdev_out(debug_putc);
	
	
	/*リングバッファの初期化*/
	RingBuffer_InitBuffer(&midi_buffer);

	return;
}

void	debug_putc(uint8_t chr){
	while(!(USART2.STATUS  & 0x20)){};	/*DREIFが1=送信可能*/
	USART2.TXDATAL = chr;
	return;
}

uint8_t debug_getc(void){
	while(!(USART2.STATUS  & 0x80)){};	/*RXCIFが1=データあり*/
	return USART2.RXDATAL;
}
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
	USART0.BAUD			 =	BAUD_RATE(31250);	/*�{�[���[�g��31250bps�ɐݒ�*/
	USART0.CTRLC		 =	0x03;				/*N81,��������*/
	USART0.CTRLB		 =	0xC0;				/*USART0 TX&RX�L����*/
	PORTA.DIR			|=	0x01;				/*(TX(PA0)���o�͂ɐݒ�*/
	USART0.CTRLA		 =	0xC0;				/*USART0 TX&RX���荞�ݗL����*/
	
	/*USART2(�f�o�b�O)*/
	PORTMUX.USARTROUTEA	 =	0x10;				/*USART2��PF4~6�Ɋ��蓖��*/
	USART2.BAUD			 =	BAUD_RATE(38400);	/*�{�[���[�g��38400bps�ɐݒ�*/
	USART2.CTRLC		 =	0x03;				/*N81,��������*/
	PORTF.DIR			|=	0x10;				/*TX(PF4)���o�͂ɐݒ�*/
	USART2.CTRLB		 =	0xC0;				/*USART2 TX&RX�L����*/
	
	sei();
	
	/*printf���C�u�����̐ݒ�*/
	xdev_in(debug_getc);
	xdev_out(debug_putc);
	
	
	/*�����O�o�b�t�@�̏�����*/
	RingBuffer_InitBuffer(&midi_buffer);

	return;
}

void	debug_putc(uint8_t chr){
	while(!(USART2.STATUS  & 0x20)){};	/*DREIF��1=���M�\*/
	USART2.TXDATAL = chr;
	return;
}

uint8_t debug_getc(void){
	while(!(USART2.STATUS  & 0x80)){};	/*RXCIF��1=�f�[�^����*/
	return USART2.RXDATAL;
}
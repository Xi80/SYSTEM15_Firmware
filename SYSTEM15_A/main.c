/*
 * SYSTEM15_A.c
 *
 * Created: 2022/01/21 22:30:32
 * Author : Owner
 */ 



/*モジュール読み込み*/
#include "serial_manager.h"			//シリアル通信
#include "midi_parser.h"

#include <avr/io.h>
#include <util/delay.h>
/*割り込み処理*/
ISR(USART0_RXC_vect)
{
	uint8_t recv = USART0.RXDATAL;
	RingBuffer_Insert(&midi_buffer,recv);
}

void init_io(void){
	xprintf("init io\n");
	PORTD.DIR = 0xFF;		//データバスをすべて出力に設定
	PORTC.DIR = 0x3F;		//PC0~PC5を出力に設定
	PORTE.DIR = 0x07;		//PE0~PE2を出力、PE3を入力に設定
		
	PORTE.OUT |= 0x07;		//WR,RD,ICをネゲート
	PORTC.OUT |= 0x3F;		//CSをすべてネゲート
	PORTC.OUT &= ~(0x01);	//A0を0に
	PORTD.OUT = 0x00;		//データバスを0x00に
}

void reset_ic(void){
	PORTE.OUT &= ~(0x04);	//ICをアサート
	for(uint16_t i = 0;i < 1024;i++){
		asm("nop;");
	}
	PORTE.OUT |= 0x04;		//ICをネゲート
	for(uint16_t i = 0;i < 16384;i++){
		asm("nop;");
	}
}


int main(void)
{
	init_serial();
	init_io();
	reset_ic();
	init_midi_parser();
	
	
    /* Replace with your application code */
	xprintf("Hello,World!\n");
    while (1) 
    {
		
		if(RingBuffer_GetCount(&midi_buffer)){
			uint8_t data = RingBuffer_Remove(&midi_buffer);
			parse_midi_data(data);
		}
    }
}

